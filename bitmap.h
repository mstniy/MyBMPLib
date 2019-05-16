#ifndef BITMAP_H

#define BITMAP_H
#include<windows.h>
#ifndef PCVOID
#define PCVOID const void*
#endif
typedef struct Bitmap
{
	HBITMAP hbmp;
	COLORREF *Bits;
	LONG nWidth,nHeight;
} Bitmap;
#include "bitmap.c"
#endif
