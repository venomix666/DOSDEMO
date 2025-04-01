/* Plasma effect - Copyright (c) 2025 Henrik Löfgren
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
void rotatePalette(void);
int sine_table[5760];
unsigned char r_pal[256];
unsigned char g_pal[256];
unsigned char b_pal[256];

int main(void)
{
	unsigned char buffer[64640];
	int cnt;
	int x, y;
	int ypos;
	int a,b,c;
	float runtime;
	clock_t	start_time, end_time;                      
	setVideoMode(0x13);
	genPalette();
	setPalette();
	genSineTable();
	start_time = clock();
	
	for(cnt=0; cnt<60000; cnt++) {
		// Generate the plasma effect using fixed point math
		for(y=0; y<200; y++) {
			ypos = (y<<8) + (y<<6);
			b=1024+sine_table[((y<<4)+1024+
					  sine_table[(cnt<<2)%5760])%5760];
			for(x=0; x<320; x++) {
				a=(1024+sine_table[((x<<6)+(cnt<<3)+y)%5760])*
				  (1024-sine_table[(((x+y)<<3)+(y<<2)+
				   (cnt<<4)+1440)%5760]);
				c=(1024+sine_table[((a>>12)+(b<<1))%5760])>>3;
				buffer[ypos + x] = (unsigned char)c;
			}
		}

		updateScreen(&buffer);
		// Rotate the palette every four frames
		if(!(cnt%4)) {
			rotatePalette();
			setPalette();
		}
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

void genSineTable(void)
{
	// Generate a sine table, 12 bit fixed point, 1/16 degrees resolution
	int i;

	for(i=0; i<5760; i++)
		sine_table[i] = (int)(1024.0*sin((float)i*3.1415927/(180.0*16)));
}

void genPalette(void)
{
	// Generate a cyclic palette to allow rotation
	int i;
	for(i=0; i<64; i++) {
		r_pal[i] = 0;
		g_pal[i] = 0;
		b_pal[i] = i;
	}
	for(i=64; i<128; i++) {
		r_pal[i] = i-64;
		g_pal[i] = 0;
		b_pal[i] = 63 - (i-64);
	}
	for(i=128; i<192; i++) {
		r_pal[i] = 63;
		g_pal[i] = (i-128);
		b_pal[i] = 0;
	}
	for(i=192; i<256; i++) {
		r_pal[i] = 63 - (i-192);
		g_pal[i] = 63 - (i-192);
		b_pal[i] = 0;
	}
}

void setPalette(void) {
	int i;
	for(i=0; i<256; i++) setPalReg(i, r_pal[i], g_pal[i], b_pal[i]);
}

void rotatePalette(void)
{
	// Rotate all colors in the palette one step
	unsigned char r,g,b;
	int i;
    	for(i=0; i<256; i++) {
		r_pal[i] = r_pal[(i+1)%256];
		g_pal[i] = g_pal[(i+1)%256];
		b_pal[i] = b_pal[(i+1)%256];
    	}
}
