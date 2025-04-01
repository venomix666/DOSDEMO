/* Graphics library for DOS - Copyright (c) 2025 Henrik Löfgren
 * This program is distributable under the terms of the 2-clause BSD license.
 */
#include <conio.h>
#include <stdio.h>
#include <string.h>
#include "gfxlib.h"

char *screen = (char *)0xa0000;

void setVideoMode(char mode) {
	_asm {
		mov ah, 0
		mov al, mode
		int 10h
	}

}

char isVsync(void)
{
	char vsync;

	vsync = (inp(0x3da) & 0x08)>>3;
	return vsync;
}

void setPalReg(char color, char r, char g, char b)
{
	outp(0x3c8, color);
	outp(0x3c9, r);
	outp(0x3c9, g);
	outp(0x3c9, b);
}

void updateScreen(char *buffer)
{
	while(!isVsync());
	memcpy(screen, buffer, 64000);
}

void readPCX(const char *filename, char *buffer, char *pal_r, char *pal_g,
	     char *pal_b)
{
	// Read a PCX file, for now assume it is 320x200
	FILE *image;
	int pos = 0;
	int i;
	unsigned char data_i;
	unsigned char len;
	image = fopen(filename, "rb");

	// Skip header
	fseek(image, 0x80, SEEK_SET);

	while(pos<64000) {
		fread(&data_i, sizeof(data_i), 1, image);
		// RLE or data?
		if((data_i & 0xc0) == 0xc0) {
			// RLE
			len = data_i & 0x3f;
			fread(&data_i, sizeof(data_i), 1, image);
				
			for(i = 0; i<len; i++) {
				buffer[pos] = data_i;
				pos++;
			}
		}
		else {
			// Data
			buffer[pos] = data_i;
			pos++;
		}
	}

	// Seek to palette position
	fseek(image, -0x300, SEEK_END);
	// Read palette
	for(i=0; i<255; i++) {
		fread(&data_i, sizeof(data_i), 1, image);
		pal_r[i]=data_i<<3;
		fread(&data_i, sizeof(data_i), 1, image);
		pal_g[i]=data_i<<3;
		fread(&data_i, sizeof(data_i), 1, image);
		pal_b[i]=data_i<<3;
	}
	fclose(image);
}
