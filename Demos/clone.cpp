#include<windows.h>
#include "..\bitmap.h"
#include<stdio.h>
int main()
{
	Bitmap bmp,bmp2;
	if (BCreateBitmap(&bmp,100,100)==false)
		return 1;
	BSetPixel(bmp,0,0,RGB(1,2,3));
	BSetPixel(bmp,99,99,RGB(4,5,6));
	if (BCloneBitmap(&bmp2,bmp)==false)
		return 1;
	if (BGetPixel(bmp2,0,0)!=RGB(1,2,3))
		return 1;
	if (BGetPixel(bmp2,99,99)!=RGB(4,5,6))
		return 1;
    BSetPixel(bmp,0,0,RGB(255,255,255));
    if (BGetPixel(bmp2,0,0)!=RGB(1,2,3))
		return 1;
	if (BGetPixel(bmp,0,0)!=RGB(255,255,255))
		return 1;
	return 0;
}
