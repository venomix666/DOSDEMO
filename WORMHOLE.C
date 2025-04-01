/* Wormhole effect - Copyright (c) 2025 Henrik Löfgren
 * This program is distributable under the terms of the 2-clause BSD license.
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <conio.h>
#include <time.h>
#include <math.h>
#include "gfxlib.h"

void setPalette(void);
void genPalette(void);
void rotatePalette(void);
unsigned char r_pal[256];
unsigned char g_pal[256];
unsigned char b_pal[256];

int main(void)
{
	unsigned char buffer[64000];

	int x,y,cnt;
	int ypos;
	float runtime;
	clock_t	start_time, end_time;                      

	setVideoMode(0x13);

	// Read graphics and palette data from PCX file
	readPCX("wormhole.pcx", &buffer, &r_pal, &g_pal, &b_pal);
	setPalette();

	start_time = clock();

	// Draw screen once
	updateScreen(&buffer);

	for(cnt=0; cnt<60000; cnt++) {
		// Rotate palette once per frame
		while(!isVsync());
		rotatePalette();
		setPalette();
		while(isVsync());
		if(kbhit()) break;
	}

	end_time=clock();
	getch();
	runtime = (float)(end_time-start_time)/((float)CLOCKS_PER_SEC);
	setVideoMode(0x03);	
	printf("Frames: %d\n", cnt);
	printf("Runtime: %f\n", runtime);
	printf("FPS: %f\n", (float)cnt/runtime);
	return 0;
}

void setPalette(void) {
	int i;
	for(i=0; i<256; i++) {
		setPalReg(i, r_pal[i], g_pal[i], b_pal[i]);
	}
}

void rotatePalette(void)
{
	// Rotate all colors in the palette one step
	int i;
	unsigned char r,g,b;
	r=r_pal[0];
	g=g_pal[0];
	b=b_pal[0];
    	for(i=0; i<255; i++) {
		r_pal[i] = r_pal[(i+1)];
		g_pal[i] = g_pal[(i+1)];
		b_pal[i] = b_pal[(i+1)];
    	}
	r_pal[255] = r;
	g_pal[255] = g;
	b_pal[255] = b;
}
