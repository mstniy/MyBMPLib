#include<windows.h>
#include "..\bitmap.h"
#include<string>
#include<iostream>
#include<stdio.h>
using namespace std;
void Fail(const char *error)
{
	printf("%s failed (%d)\n",error,GetLastError());
	FlushConsoleInputBuffer(GetStdHandle(STD_INPUT_HANDLE));
	getchar();
	ExitProcess(1);
}
int color_distance(COLORREF f,COLORREF s)
{
	int a=GetRValue(f)-GetRValue(s);
	int b=GetGValue(f)-GetGValue(s);
	int c=GetBValue(f)-GetBValue(s);
	return a*a+b*b+c*c;
}
int main()
{
	const int yon[8][2]={
							{0,1},
							{1,0},
							{0,-1},
							{-1,0},
							{1,1},
							{1,-1},
							{-1,-1},
							{-1,1}
						};
	COLORREF now,other;
	int i,j,k,newi,newj,TRESHOLD;
	Bitmap bmp;
	string filename;
	getline(cin,filename);
	printf("Enter the treshold (bigger treshold -> less edges) : ");
	scanf("%d",&TRESHOLD);
	if (BImportFromFile(&bmp,filename.c_str())==false)
		Fail("BImportFromFile");
	int **scores=(int **)malloc(sizeof(int)*bmp.nHeight);
	for (i=0;i<bmp.nHeight;i++)
		scores[i]=(int *)malloc(sizeof(int)*bmp.nWidth);
	for (i=0;i<bmp.nHeight;i++)
		for (j=0;j<bmp.nWidth;j++)
		{
			now=BGetPixel(bmp,j,i);
			if (now==CLR_INVALID)
				Fail("BGetPixel");
			scores[i][j]=0;
			for (k=0;k<8;k++)
			{
				newi=i+yon[k][0];
				newj=j+yon[k][1];
				other=BGetPixel(bmp,newj,newi);
				if (other==CLR_INVALID)
					continue;
                scores[i][j]+=color_distance(now,other);
			}
		}
	if (BDeleteBitmap(bmp)==false)
		Fail("BDeleteBitmap");
	if (BCreateBitmap(&bmp,bmp.nWidth,bmp.nHeight)==false)
		Fail("BCreateBitmap");
	for (i=0;i<bmp.nHeight;i++)
		for (j=0;j<bmp.nWidth;j++)
		{
			if (scores[i][j]<=TRESHOLD)
			{
				if (BSetPixel(bmp,j,i,RGB(255,255,255))==CLR_INVALID)
					Fail("BSetPixel");
			}
			else
			{
				if (BSetPixel(bmp,j,i,RGB(0,0,0))==CLR_INVALID)
					Fail("BSetPixel");
			}
		}
	if (BExportToFile(bmp,"output.bmp")==false)
		Fail("BExportToFile");
	if (BDeleteBitmap(bmp)==false)
		Fail("BDeleteBitmap");
	puts("Success");
	return 0;
}
