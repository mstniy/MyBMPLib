#include<windows.h>
#include "..\bitmap.h"
#include<string>
#include<iostream>
#include<stdio.h>
using namespace std;
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
	string filename;
	Bitmap bmp;
	getline(cin,filename);
	if (BImportFromFile(&bmp,filename.c_str())==false)
		Fail("BImportFromFile");
	BSetPixel(bmp,0,0,RGB(0,0,0));
	BSetPixel(bmp,1,0,RGB(0,0,0));
	if (BExportToFile(bmp,TEXT("output.bmp"))==false)
		Fail("BExportToFile");
	return 0;
}
