#include<windows.h>
#include "..\bitmap.h"
#include<stdio.h>
#define RED(x) GetRValue(x)
#define GREEN(x) GetGValue(x)
#define BLUE(x) GetBValue(x)
int main()
{
	COLORREF color;
	HBITMAP hbmp;
	HDC hdcscr,hdcmem;
	Bitmap bmp;
	if (BCreateBitmap(&bmp,100,100)==false)
		return 1;
	if (BSetPixel(bmp,0,0,RGB(1,2,3))==CLR_INVALID)
		return 1;
	if (BSetPixel(bmp,99,99,RGB(4,5,6))==CLR_INVALID)
		return 1;
	printf("DEBUG : %d %d %d - %d %d %d\n",RED(BGetPixel(bmp,0,0)),GREEN(BGetPixel(bmp,0,0)),BLUE(BGetPixel(bmp,0,0)),RED(BGetPixel(bmp,99,99)),GREEN(BGetPixel(bmp,99,99)),BLUE(BGetPixel(bmp,99,99)));
	hdcscr=CreateDC("DISPLAY",NULL,NULL,NULL);
	if (hdcscr==NULL)
		return 1;
	hdcmem=CreateCompatibleDC(hdcscr);
	if (hdcmem==NULL)
		return 1;
	hbmp=CreateCompatibleBitmap(hdcscr,100,100);
	if (hbmp==NULL)
		return 1;
	if (BExportToDDB(bmp,hbmp)==false)
		return 1;
	if (SelectObject(hdcmem,hbmp)==NULL)
		return 1;
	color=GetPixel(hdcmem,0,0);
	if (color==CLR_INVALID)
		return 1;
	printf("%d %d %d\n",RED(color),GREEN(color),BLUE(color));
	color=GetPixel(hdcmem,99,99);
	if (color==CLR_INVALID)
		return 1;
	printf("%d %d %d\n",RED(color),GREEN(color),BLUE(color));
	if (BSetPixel(bmp,0,0,RGB(255,255,255))==CLR_INVALID)
		return 1;
	if (BSetPixel(bmp,99,99,RGB(255,255,255))==CLR_INVALID)
		return 1;
    if (BImportFromDDB(&bmp,hbmp,hdcscr)==false)
		return 1;
	color=BGetPixel(bmp,0,0);
	if (color==CLR_INVALID)
		return 1;
	printf("%d %d %d\n",RED(color),GREEN(color),BLUE(color));
	color=BGetPixel(bmp,99,99);
	if (color==CLR_INVALID)
		return 1;
	printf("%d %d %d\n",RED(color),GREEN(color),BLUE(color));
	getchar();
	return 0;
}
