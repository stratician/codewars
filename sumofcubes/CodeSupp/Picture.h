// Picture.h: interface for the CPicture class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(__AFX_PICTURE_H__)
#define __AFX_PICTURE_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CPicture  
{
public:
	CPicture();
	virtual ~CPicture();

	BOOL			Load(CString sResourceType, CString sResource);
	BOOL			Load(CString sFileName);
	BOOL			Draw(CDC *pDC, int x, int y, int w, int h);
	BOOL			DrawCBitmap(CDC *pDC,  int x, int y, int w, int h);
	BOOL			Draw(CDC *pDC, CPoint Pos, CSize Size);
	CSize			GetSize(CDC *pDC);
	IPicture		*m_pPicture;
private:
	static	BOOL	GetResource(LPSTR lpName, LPSTR lpType, void *pResource, int& nBufSize);
	void			UnLoad();
	BOOL			LoadFromBuffer(BYTE *pBuff, int nSize);
	enum
	{
		HIMETRIC_INCH	= 2540
	};
};

#endif // __AFX_PICTURE_H__
