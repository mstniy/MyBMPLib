#include<windows.h>
#include "..\bitmap.h"
#include<stdio.h>
#include<string>
#include<iostream>
using namespace std;
void Fail(const char *error)
{
	printf("%s failed (%d)\n",error,GetLastError());
	FlushConsoleInputBuffer(GetStdHandle(STD_INPUT_HANDLE));
	getchar();
	ExitProcess(1);
}
int main()
{
	COLORREF colortmp;
	int i,j,k,l,newi,newj,c,rsum,gsum,bsum,RAD;
	string filename;
	Bitmap bmp,bmpnew;
	getline(cin,filename);
	printf("Enter radius (bigger radius -> more blurring) : ");
	scanf("%d",&RAD);
	if (BImportFromFile(&bmp,filename.c_str())==false)
		Fail("BImportFromFile");
	if (BCreateBitmap(&bmpnew,bmp.nWidth,bmp.nHeight)==false)
		Fail("BCreateBitmap");
	for (i=0;i<bmp.nHeight;i++)
	{
		rsum=0;
		gsum=0;
		bsum=0;
		c=0;
		for (k=-RAD;k<=RAD;k++)
		{
			for (l=0;l<=RAD-abs(k);l++)
			{
				colortmp=BGetPixel(bmp,l,i+k);
				if (colortmp==CLR_INVALID)
					continue;
				c++;
				rsum+=GetRValue(colortmp);
				gsum+=GetGValue(colortmp);
				bsum+=GetBValue(colortmp);
			}
		}
		if (BSetPixel(bmpnew,0,i,RGB(rsum/c,gsum/c,bsum/c))==CLR_INVALID)
			Fail("BSetPixel");
		for (j=1;j<bmp.nWidth;j++)
		{
			for (k=-RAD;k<=RAD;k++)
			{
				colortmp=BGetPixel(bmp,j+abs(k)-RAD-1,i+k);
				if (colortmp==CLR_INVALID)
					continue;
				c--;
				rsum-=GetRValue(colortmp);
				gsum-=GetGValue(colortmp);
				bsum-=GetBValue(colortmp);
			}
			for (k=-RAD;k<=RAD;k++)
			{
				colortmp=BGetPixel(bmp,j+RAD-abs(k),i+k);
				if (colortmp==CLR_INVALID)
					continue;
				c++;
				rsum+=GetRValue(colortmp);
				gsum+=GetGValue(colortmp);
				bsum+=GetBValue(colortmp);
			}
			if (BSetPixel(bmpnew,j,i,RGB(rsum/c,gsum/c,bsum/c))==CLR_INVALID)
				Fail("BSetPixel");
		}
	}
	if (BExportToFile(bmpnew,"output.bmp")==false)
		Fail("BExportToFile");
	return 0;
}
