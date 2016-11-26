#include<windows.h>
#include "..\bitmap.h"
#include<stdio.h>
#include<math.h>
void Fail(const char *funcname,HWND hwnd=NULL)
{
	DWORD error=GetLastError();
	printf("%s failed (%d)\n",funcname,error);
	FlushConsoleInputBuffer(GetStdHandle(STD_INPUT_HANDLE));
	getchar();
	ExitProcess(1);
}
int main()
{
	const int bold=6;
	const double PI=3.141592653589793;
	int i,j,k,l;
	Bitmap bmp;
	if (BCreateBitmap(&bmp,3600,206)==false)
		Fail("BCreateBitmap");
	for (i=0;i<3600;i++)
		for (j=0;j<206;j++)
			if (BSetPixel(bmp,i,j,RGB(255,255,255))==-1)
				Fail("BSetPixel");
	for (i=0;i<360;i+=1)
	{
		j=(sin(i/180.0*PI)+1)*100;
		for (k=-bold/2;k<=bold/2;k++)
			for (l=-bold/2;l<=bold/2;l++)
				if ((i*10+k>=0) && (j+l+3>=0) && (i*10+k<3600) && (j+l+3<206))
					if (BSetPixel(bmp,i*10+k,j+l+3,RGB(0,0,0))==-1)
						Fail("BSetPixel");
	}
	if (BExportToFile(bmp,"output.bmp")==false)
		Fail("BExportToFile");
	return 0;
}
