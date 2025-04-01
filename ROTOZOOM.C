/* Rotozoom effect - Copyright (c) 2025 Henrik Löfgren
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
void genSineTable(void);
int sine_table[5760];
unsigned char r_pal[256];
unsigned char g_pal[256];
unsigned char b_pal[256];

unsigned char buffer[64000];
unsigned char texture[64000];

int main(void)
{
	int x,y,cnt;
	int ypos;
	float runtime;
	int u,v;
	clock_t	start_time, end_time;
	int c,s;
	genSineTable();
	setVideoMode(0x13);

	// Read graphics and palette data from PCX file
	readPCX("rotozoom.pcx", &texture, &r_pal, &g_pal, &b_pal);
	setPalette();

	start_time = clock();

	for(cnt=0; cnt<60000; cnt++) {
		s = sine_table[(cnt<<2) % 5760];
		c = -1*sine_table[((cnt<<2) + 1440) % 5760];
		for(y=0; y<200; y++) {
		    	ypos = (y<<8) + (y<<6);
			for(x=0; x<320; x++) {
				u = abs((((((x*c-y*s))*((s<<1)+2))>>20) % 320));
				v = abs((((((x*s+y*c))*((s<<1)+2))>>20) % 200));
				if((u>319) || (u<0) || (v>199) || (v<0))
					printf("u: %d\tv: %d\n", u, v);
				buffer[ypos + x] = texture[(v<<8)+(v<<6)+u];
			}
		}
		updateScreen(&buffer);
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

void genSineTable(void)
{
	// Generate a sine table, 12 bit fixed point, 1/16 degrees resolution
	int i;

	for(i=0; i<5760; i++)
		sine_table[i] = (int)(1024.0*sin((float)i*3.1415927/(180.0*16)));
}
