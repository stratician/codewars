
// CBitmapEX

#include "stdafx.h"
#include "BitmapEx.h"
#include <windows.h>
#include <windowsx.h>
#include <mmsystem.h>
#include <memirlib.h>
//#include "Picture.h"

// CBitmapEx construction/destruction
CBitmapEx::CBitmapEx()
{
}

CBitmapEx::~CBitmapEx()
{
}

BOOL CBitmapEx::CreateBitmap( int nWidth, int nHeight, UINT nPlanes, UINT nBitcount, const void* lpBits )
{
	m_nWidth = nWidth;
	m_nHeight = nHeight;
	
	return CBitmap::CreateBitmap(nWidth, nHeight, nPlanes, nBitcount, lpBits);
}

void CBitmapEx::LoadBitmap(char *fname)
{
	BYTE *buf;
	DWORD w,h;
	buf = Mgfx_bitmapload(fname, w, h);

	CreateBitmap(w,h, 1, Mgfx_bpp, buf);

	delete buf;
}


void CBitmapEx::Draw(CDC * pDC, int x, int y)
{					 
       CDC dcImage;

       // Create a memory dc for the image
       dcImage.CreateCompatibleDC(pDC);
       					
       // Select the image into the appropriate dc
       CBitmap* pOldBitmapImage = dcImage.SelectObject(this);
             
	   pDC->BitBlt(x, y, m_nWidth, m_nHeight, &dcImage, 0, 0, SRCCOPY);
		
	   dcImage.SelectObject(pOldBitmapImage);
}

void CBitmapEx::Draw(CDC * pDC, int x, int y, CRect rect)
{					 
       CDC dcImage;

       // Create a memory dc for the image
       dcImage.CreateCompatibleDC(pDC);
       					
       // Select the image into the appropriate dc
       CBitmap* pOldBitmapImage = dcImage.SelectObject(this);
             
	   CSize size = rect.Size();
	   if(size.cx > m_nWidth) size.cx = m_nWidth;
	   if(size.cy > m_nHeight) size.cy = m_nHeight;
	   pDC->BitBlt(x, y, size.cx/*m_nWidth*/, size.cy/*m_nHeight*/, &dcImage, 0+rect.left, 0+rect.top, SRCCOPY);
		
	   dcImage.SelectObject(pOldBitmapImage);
}

void CBitmapEx::DrawTransparent(CDC * pDC, int x, int y, COLORREF crColor)
{
	   COLORREF crColorOld = pDC->SetBkColor(crColor);
       CDC dcImage, dcTrans;

       // Create two memory dcs for the image and the mask
       dcImage.CreateCompatibleDC(pDC);
       dcTrans.CreateCompatibleDC(pDC);

       // Select the image into the appropriate dc
       CBitmap* pOldBitmapImage = dcImage.SelectObject(this);

       // Create the mask bitmap
	   CBitmap bitmapTrans;
	   bitmapTrans.CreateBitmap(m_nWidth, m_nHeight, 1, 1, NULL);

       // Select the mask bitmap into the appropriate dc
       CBitmap* pOldBitmapTrans = dcTrans.SelectObject(&bitmapTrans);

       // Build mask based on transparent colour
       dcImage.SetBkColor(crColor);
       dcTrans.BitBlt(0, 0, m_nWidth, m_nHeight, &dcImage, 0, 0, SRCCOPY);

       // Do the work - True Mask method - cool if not actual display
	   pDC->BitBlt(x, y, m_nWidth, m_nHeight, &dcImage, 0, 0, SRCINVERT);
       pDC->BitBlt(x, y, m_nWidth, m_nHeight, &dcTrans, 0, 0, SRCAND);
	   pDC->BitBlt(x, y, m_nWidth, m_nHeight, &dcImage, 0, 0, SRCINVERT);
	   
       // Restore settings
       dcImage.SelectObject(pOldBitmapImage);
       dcTrans.SelectObject(pOldBitmapTrans);
	   pDC->SetBkColor(crColorOld);
}

void CBitmapEx::DrawScale(CDC * pDC, int x, int y, int w, int h)
{					 
       CDC dcImage;

       // Create a memory dc for the image
       dcImage.CreateCompatibleDC(pDC);
       					
       // Select the image into the appropriate dc
       CBitmap* pOldBitmapImage = dcImage.SelectObject(this);
	                
	   pDC->StretchBlt(x, y, w, h, &dcImage, 0, 0, m_nWidth, m_nHeight, SRCCOPY);

	   dcImage.SelectObject(pOldBitmapImage);
}

void CBitmapEx::DrawTransparentScale(CDC * pDC, int x, int y, int w, int h, COLORREF crColor)
{
	   COLORREF crColorOld = pDC->SetBkColor(crColor);
       CDC dcImage, dcTrans;

       // Create two memory dcs for the image and the mask
       dcImage.CreateCompatibleDC(pDC);
       dcTrans.CreateCompatibleDC(pDC);

       // Select the image into the appropriate dc
       CBitmap* pOldBitmapImage = dcImage.SelectObject(this);

       // Create the mask bitmap
	   CBitmap bitmapTrans;
	   bitmapTrans.CreateBitmap(m_nWidth, m_nHeight, 1, 1, NULL);

       // Select the mask bitmap into the appropriate dc
       CBitmap* pOldBitmapTrans = dcTrans.SelectObject(&bitmapTrans);

       // Build mask based on transparent colour
       dcImage.SetBkColor(crColor);
       dcTrans.BitBlt(0, 0, m_nWidth, m_nHeight, &dcImage, 0, 0, SRCCOPY);

       // Do the work - True Mask method - cool if not actual display
	   pDC->StretchBlt(x, y, w, h, &dcImage, 0, 0, m_nWidth, m_nHeight, SRCINVERT);
       pDC->StretchBlt(x, y, w, h, &dcTrans, 0, 0, m_nWidth, m_nHeight, SRCAND);
	   pDC->StretchBlt(x, y, w, h, &dcImage, 0, 0, m_nWidth, m_nHeight, SRCINVERT);
	   	   
       // Restore settings
       dcImage.SelectObject(pOldBitmapImage);
       dcTrans.SelectObject(pOldBitmapTrans);
	   pDC->SetBkColor(crColorOld);
}

void CBitmapEx::SetBitmap(CBitmap *srcBitmap, COLORREF crColor)
{
	// Get bitmap width/height
    BITMAP bm;
    GetBitmap(&bm);
    int nWidth = bm.bmWidth;
    int nHeight = bm.bmHeight;

	DWORD *pBuff;
	pBuff = new DWORD[4*nWidth*nHeight];
	srcBitmap->GetBitmapBits(4*nWidth*nHeight, pBuff);
	
	if(crColor != -1) // check for color change param!
	for(int i=0;i<nWidth*nHeight;i++)
	if(pBuff[i] == RGB(255,255,255)); // ignore transparent color!
	else {
	int r = pBuff[i]&0xff;
	int g = (pBuff[i]>>8)&0xff;
	int b = (pBuff[i]>>16)&0xff;
	
	float newR = (float)(crColor&0xff);
	float newG = (float)((crColor>>8)&0xff);
	float newB = (float)((crColor>>16)&0xff);

	float factorR = newR / 255;
	float factorG = newG / 255;
	float factorB = newB / 255;

	float SumR = (newR + r) / 1.3f;
	float SumG = (newG + g) / 1.3f;
	float SumB = (newB + b) / 1.3f;

	if(SumR > 255.0f) SumR = 255.0f;
	if(SumG > 255.0f) SumG = 255.0f;
	if(SumB > 255.0f) SumB = 255.0f;

	pBuff[i] = RGB((BYTE)SumB,(BYTE)SumG,(BYTE)SumR);
	}
	
	SetBitmapBits(4*nWidth*nHeight, pBuff);

	delete pBuff;
}
void CBitmapEx::SetBitmap(CDC *pDC, COLORREF crColor)
{
	// Get bitmap width/height
    BITMAP bm;
    GetBitmap(&bm);
    int nWidth = bm.bmWidth;
    int nHeight = bm.bmHeight;
		
	DWORD *pBuff;
	pBuff = new DWORD[4*nWidth*nHeight];
			
	for(int y=0;y<nHeight;y++)
	for(int x=0;x<nWidth;x++)
	{
		pBuff[y*nWidth+x] = pDC->GetPixel(x,y);
		int r = pBuff[y*nWidth+x]&0xff;
		int g = (pBuff[y*nWidth+x]>>8)&0xff;
		int b = (pBuff[y*nWidth+x]>>16)&0xff;
		pBuff[y*nWidth+x] = RGB(b,g,r);
		if(crColor != -1) // check for color change param!
		{
			if(pBuff[y*nWidth+x] == RGB(255,255,255)); // ignore transparent color!
			else {
			int r = pBuff[y*nWidth+x]&0xff;
			int g = (pBuff[y*nWidth+x]>>8)&0xff;
			int b = (pBuff[y*nWidth+x]>>16)&0xff;
			
			float newR = (float)(crColor&0xff);
			float newG = (float)((crColor>>8)&0xff);
			float newB = (float)((crColor>>16)&0xff);

			float factorR = newR / 255;
			float factorG = newG / 255;
			float factorB = newB / 255;

			float SumR = (newR + r) / 1.3f;
			float SumG = (newG + g) / 1.3f;
			float SumB = (newB + b) / 1.3f;

			if(SumR > 255.0f) SumR = 255.0f;
			if(SumG > 255.0f) SumG = 255.0f;
			if(SumB > 255.0f) SumB = 255.0f;


			pBuff[y*nWidth+x] = RGB((BYTE)SumB,(BYTE)SumG,(BYTE)SumR);
			}
		}
	}
		
	SetBitmapBits(4*nWidth*nHeight, pBuff);

	delete pBuff;
}
/*
void CBitmapEx::SetBitmap(CDC *pDC, CPicture *pPicture, COLORREF crColor)
{		
	int HIMETRIC_INCH	= 2540;
	CBitmap memBitmap;
	CBitmap *memOldBitmap;
		
	// our mem DC
	CDC dcMem;
	dcMem.CreateCompatibleDC(pDC);
	    
	// Get bitmap width/height
	long hmWidth;
	long hmHeight;
	pPicture->m_pPicture->get_Width(&hmWidth);
	pPicture->m_pPicture->get_Height(&hmHeight);

	int nWidth	= MulDiv(hmWidth,	dcMem.GetDeviceCaps(LOGPIXELSX), HIMETRIC_INCH);
	int nHeight	= MulDiv(hmHeight,	dcMem.GetDeviceCaps(LOGPIXELSY), HIMETRIC_INCH);
		
	memBitmap.CreateCompatibleBitmap(pDC, nWidth, nHeight);
    memOldBitmap = dcMem.SelectObject(&memBitmap);
			
	pPicture->Draw(&dcMem, 0,0, nWidth, nHeight);
		
	SetBitmap(&dcMem, -1);//crColor);
}
*/