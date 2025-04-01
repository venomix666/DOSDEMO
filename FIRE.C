/* Fire effect - Copyright (c) 2025 Henrik Löfgren
 * This program is distributable under the terms of the 2-clause BSD license.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <conio.h>
#include <time.h>
#include "gfxlib.h"

void setPalette(void);

int main(void)
{
	unsigned char buffer[64640];

	int x,y,cnt;
	int ypos;
	float runtime;
	clock_t	start_time, end_time;                      
	setVideoMode(0x13);
	setPalette();

	start_time = clock();
	
	for(cnt=0; cnt<60000; cnt++) {
		// Seed lines
		for(x=0; x<320;x++) {
			if(x<160) {
				buffer[64000+x] = (x>>1) + (unsigned char)(rand() % (96+x));
			} else {
				buffer[64000+x] = (unsigned char)(rand() % (256-(x-160)));
			}

			if(x<160) {
				buffer[64320+x] = (unsigned char)(rand() %(96+x));
			} else {
				buffer[64320+x] = (unsigned char)(rand() % (256-(x-160)))+
								  ((x-160)>>1);
			}
		}	

		// Fire effect
		for(y=1; y<200; y++) {
			ypos = (y<<8) + (y<<6);
			for(x=0; x<320; x++) {
				buffer[ypos + x] = ((buffer[ypos+x-320]<<2)+
						    (buffer[ypos+x+320]<<2)+
						    (buffer[ypos+x+319]<<1)+
						    (buffer[ypos+x+321]<<1)+
						    (buffer[ypos+x+640]<<2))>>4;
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

void setPalette(void)
{
	int i;
	for(i=0; i<64; i++) setPalReg(i,0,0,0);
	for(i=64; i<96; i++) setPalReg(i,(i-64)<<1,0,0);
	for(i=96; i<160; i++) setPalReg(i,63,(i-96),0);
	for(i=160; i<224; i++) setPalReg(i,63,63,(i-160));
	for(i=224; i<256; i++) setPalReg(i,63,63,63);
}

