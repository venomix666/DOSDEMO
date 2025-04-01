#ifndef GFXLIB_H_
#define GFXLIB_H_
void setVideoMode(char mode);
char isVsync(void);
void setPalReg(char color, char r, char g, char b);
void updateScreen(char *buffer);
void readPCX(const char *filename, char *buffer, char *pal_r, char *pal_g,
             char *pal_b);
#endif // GFXLIB_H_
