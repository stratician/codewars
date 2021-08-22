// BitmapEx.h : interface of the CBitmapEx class
//
/////////////////////////////////////////////////////////////////////////////

#ifndef __BITMAPEX_H__
#define __BITMAPEX_H__

#include "BitmapEx.h"
//#include "Picture.h"

// this extended class supports transparent bitmaps!!
class CBitmapEx : public CBitmap
{
public:
	CBitmapEx(); // constructor
	~CBitmapEx(); // destructor
	
	int m_nWidth;
	int m_nHeight;
	COLORREF m_crWhite;
	COLORREF m_crBlack;

	BOOL CreateBitmap(int nWidth, int nHeight, UINT nPlanes, UINT nBitcount, const void* lpBits);
	void LoadBitmap(char *fname);
	void DrawTransparent(CDC * pDC, int x, int y, COLORREF crColor);
	void Draw(CDC * pDC, int x, int y);
	void Draw(CDC * pDC, int x, int y, CRect rect);
	void DrawScale(CDC * pDC, int x, int y, int w, int h);
	void DrawTransparentScale(CDC * pDC, int x, int y, int w, int h, COLORREF crColor);
	void SetBitmap(CBitmap *srcBitmap, COLORREF crColor = -1);
	void SetBitmap(CDC *pDC, COLORREF crColor = -1);
//	void SetBitmap(CDC *pDC, CPicture *pPicture, COLORREF crColor = -1);
};

#endif // __BITMAPEX_H__
