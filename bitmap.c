bool BCreateBitmap(Bitmap *bmp,LONG nWidth,LONG nHeight)
{
	ZeroMemory(bmp,sizeof(Bitmap));
	BITMAPINFO bmi;
	bmi.bmiHeader.biSize=sizeof(BITMAPINFOHEADER);
	bmi.bmiHeader.biWidth=nWidth;
	bmi.bmiHeader.biHeight=nHeight;
	bmi.bmiHeader.biPlanes=1;
	bmi.bmiHeader.biBitCount=32;
	bmi.bmiHeader.biCompression=BI_RGB;
	bmi.bmiHeader.biSizeImage=0;
	bmi.bmiHeader.biXPelsPerMeter=0;
	bmi.bmiHeader.biYPelsPerMeter=0;
	bmi.bmiHeader.biClrUsed=0;
	bmi.bmiHeader.biClrImportant=0;
	bmp->hbmp=CreateDIBSection(NULL,
					&bmi,
					DIB_RGB_COLORS,
					(VOID**) &bmp->Bits,
					NULL,
					0);
	if (bmp->hbmp==NULL)
		return false;
	bmp->nHeight=nHeight;
	bmp->nWidth=nWidth;
	return true;
}

bool BDeleteBitmap(Bitmap bmp)
{
	return DeleteObject(bmp.hbmp);
}

bool BCloneBitmap(Bitmap *dbmp,Bitmap sbmp)
{
	if (dbmp->Bits == sbmp.Bits)
		return true;
	BDeleteBitmap(*dbmp);
	if (BCreateBitmap(dbmp,sbmp.nWidth,sbmp.nHeight)==false)
		return false;
	memcpy(dbmp->Bits,sbmp.Bits,sbmp.nHeight*sbmp.nWidth*sizeof(COLORREF));
	return true;
}

COLORREF BGetPixel(Bitmap bmp,LONG nXPos,LONG nYPos)
{
	if (nXPos<0 || nYPos<0)
		return CLR_INVALID;
	if (nXPos>=bmp.nWidth || nYPos>=bmp.nHeight)
		return CLR_INVALID;
	return *(bmp.Bits + bmp.nWidth * (bmp.nHeight-nYPos-1) + nXPos);
}

COLORREF BSetPixel(Bitmap bmp,LONG nXPos,LONG nYPos,COLORREF Color)
{
	if (nXPos<0 || nYPos<0)
		return CLR_INVALID;
	if (nXPos>=bmp.nWidth || nYPos>=bmp.nHeight)
		return CLR_INVALID;
	return *(bmp.Bits + bmp.nWidth * (bmp.nHeight-nYPos-1) + nXPos) = Color;
}

bool BExportToDDB(Bitmap bmp,HBITMAP hbmp)
{
	BITMAPINFO bmi={0};
	bmi.bmiHeader.biSize=sizeof(BITMAPINFOHEADER);
	bmi.bmiHeader.biWidth=bmp.nWidth;
	bmi.bmiHeader.biHeight=bmp.nHeight;
	bmi.bmiHeader.biPlanes=1;
	bmi.bmiHeader.biBitCount=32;
	bmi.bmiHeader.biCompression=BI_RGB;
	return SetDIBits(NULL,hbmp,0,bmp.nHeight,bmp.Bits,&bmi,DIB_RGB_COLORS);
}

bool BImportFromDDB(Bitmap *bmp,HBITMAP hbmp,HDC hdc)
{
	BITMAPINFO bmi={0};
	Bitmap newbmp;
	BITMAP *bitmap,*bitmaporig;
	bitmap=(BITMAP *)malloc(sizeof(BITMAP)+3);
	if (bitmap==NULL)
		return false;
	bitmaporig=bitmap;
	*(BYTE **)&bitmap += 4-(size_t)bitmap%4;
	if (GetObject(hbmp,sizeof(BITMAP),bitmap)==false)
	{
		free(bitmaporig);
		return false;
	}
	if (BCreateBitmap(&newbmp,bitmap->bmWidth,bitmap->bmHeight)==false)
	{
		free(bitmaporig);
		return false;
	}
	bmi.bmiHeader.biSize=sizeof(BITMAPINFOHEADER);
	bmi.bmiHeader.biWidth=bmp->nWidth;
	bmi.bmiHeader.biHeight=bmp->nHeight;
	bmi.bmiHeader.biPlanes=1;
	bmi.bmiHeader.biBitCount=32;
	bmi.bmiHeader.biCompression=BI_RGB;
	if (GetDIBits(hdc,hbmp,0,bitmap->bmHeight,newbmp.Bits,&bmi,DIB_RGB_COLORS)==false)
	{
		free(bitmaporig);
		BDeleteBitmap(newbmp);
		return false;
	}
	memcpy(bmp,&newbmp,sizeof(Bitmap));
	free(bitmaporig);
	return true;
}

bool BExportToFile(Bitmap bmp,LPCTSTR lpFileName)
{
	HANDLE hfile;
	DWORD written;
	BITMAPFILEHEADER bmpfh;
	BITMAPINFOHEADER bmpih;
	hfile=CreateFile(lpFileName,
					GENERIC_WRITE,
					0,
					NULL,
					CREATE_ALWAYS,
					FILE_ATTRIBUTE_NORMAL,
					NULL);
	if (hfile==INVALID_HANDLE_VALUE)
		return false;
	bmpfh.bfType='MB';
	bmpfh.bfSize=sizeof(BITMAPFILEHEADER)+sizeof(BITMAPINFOHEADER)+bmp.nHeight*bmp.nWidth*4;
	bmpfh.bfReserved1=0;
	bmpfh.bfReserved2=0;
	bmpfh.bfOffBits=sizeof(BITMAPFILEHEADER)+sizeof(BITMAPINFOHEADER);
	bmpih.biSize=sizeof(BITMAPINFOHEADER);
	bmpih.biWidth=bmp.nWidth;
	bmpih.biHeight=bmp.nHeight;
	bmpih.biPlanes=1;
	bmpih.biBitCount=32;
	bmpih.biCompression=BI_RGB;
    bmpih.biSizeImage=0;
    bmpih.biXPelsPerMeter=0;
    bmpih.biYPelsPerMeter=0;
	bmpih.biClrUsed=0;
	bmpih.biClrImportant=0;
    if (WriteFile(hfile,(PCVOID)&bmpfh,sizeof(BITMAPFILEHEADER),&written,NULL)==FALSE)
	{
		CloseHandle(hfile);
		return false;
	}
    if (WriteFile(hfile,(PCVOID)&bmpih,sizeof(BITMAPINFOHEADER),&written,NULL)==FALSE)
	{
		CloseHandle(hfile);
		return false;
	}
    if (WriteFile(hfile,(PCVOID)bmp.Bits,bmp.nHeight*bmp.nWidth*4,&written,NULL)==FALSE)
	{
		CloseHandle(hfile);
		return false;
	}
	CloseHandle(hfile);
	return true;
}

bool BImportFromFile(Bitmap *bmp,LPCTSTR lpFileName)
{
	ZeroMemory(bmp,sizeof(Bitmap));
	DWORD dwtmp;
	BITMAP *bitmap=(BITMAP *)malloc(sizeof(BITMAP)+3); // extra memory for possible alignment issues
	BITMAP *origbitmap=bitmap;
	int curbyteorig,curpixelnew,width,height,bbp,padding,curcolumn;
	LPVOID bits;
	if (bitmap==NULL)
		return false;
	*(BYTE **)&bitmap += 4-(size_t)bitmap%4; // 4-byte alignment,as required by GetObject
	HBITMAP hbmpli=(HBITMAP)LoadImageA(NULL,lpFileName,IMAGE_BITMAP,0,0,LR_CREATEDIBSECTION | LR_LOADFROMFILE);
	if (hbmpli==NULL)
	{
		free((void *)origbitmap);
		return false;
	}
	if (GetObject(hbmpli,sizeof(BITMAP),(LPVOID)bitmap)==0)
	{
		free((void *)origbitmap);
		DeleteObject(hbmpli);
		return false;
	}
	width=bitmap->bmWidth;
	height=bitmap->bmHeight;
	bbp=bitmap->bmBitsPixel;
	bits=bitmap->bmBits;
	if (bbp==32)
	{
		bmp->hbmp=hbmpli;
		bmp->Bits=(COLORREF *)bitmap->bmBits;
		bmp->nHeight=bitmap->bmHeight;
		bmp->nWidth=bitmap->bmWidth;
		free((void *)origbitmap);
		return true;
	}
	if (BCreateBitmap(bmp,width,height)==false)
	{
		free((void *)origbitmap);
		DeleteObject(hbmpli);
		return false;
	}
	if (bbp==24)
	{
		padding=4-(width*3)%4;
		if (padding==4)
			padding=0;
		for (curbyteorig=0,curpixelnew=0,curcolumn=0;curpixelnew<height*width;curbyteorig+=3,curpixelnew++,curcolumn++)
		{
			if (curcolumn==width)
			{
				curbyteorig+=padding;
				curcolumn=0;
			}
			bmp->Bits[curpixelnew]=( *(DWORD *)((BYTE *)bits+curbyteorig) ) & 0x00ffffff;
		}
	}
	else
	{
		free((void *)origbitmap);
		DeleteObject(hbmpli);
		return false;
	}
	free((void *)origbitmap);
	DeleteObject(hbmpli);
	return true;
}
