#include "stdafx.h"
#include "Picture.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CPicture::CPicture()
{
	m_pPicture	= NULL;
}

CPicture::~CPicture()
{
	UnLoad();
}

BOOL CPicture::Load(CString sResourceType, CString sResource)
{
	BOOL bResult = FALSE;

	if (m_pPicture != NULL)
		UnLoad();

	if (m_pPicture == NULL)
	{
		BYTE *pBuff = NULL;
		int		nSize = 0;
		if (GetResource(sResource.GetBuffer(0), sResourceType.GetBuffer(0), pBuff, nSize))
		{
			if (nSize > 0)
			{
				pBuff = new BYTE[nSize];

				if (GetResource(sResource.GetBuffer(0), sResourceType.GetBuffer(0), pBuff, nSize))
				{
					if (LoadFromBuffer(pBuff, nSize))
						bResult = TRUE;
				}

				delete [] pBuff;
			}
		}
	}
	return bResult;
}

BOOL CPicture::Load(CString sFileName)
{
	BOOL bResult = false;

	if (m_pPicture != NULL)
		UnLoad();

	if (m_pPicture == NULL)
	{
		CFile			cFile;
		CFileException	e;

		if (cFile.Open(sFileName, CFile::modeRead | CFile::typeBinary, &e))
		{
			BYTE *pBuff = new BYTE[cFile.GetLength()];

			if (cFile.Read(pBuff, cFile.GetLength()) > 0)
			{
				if (LoadFromBuffer(pBuff, cFile.GetLength()))
					bResult = true;
			}

			delete [] pBuff;
		}
	}
	return bResult;
}

void CPicture::UnLoad()
{
	if (m_pPicture != NULL)
	{
		m_pPicture->Release();
		m_pPicture = NULL;
	}
}

BOOL CPicture::Draw(CDC *pDC, int x, int y, int w, int h)
{
	long hmWidth;
	long hmHeight;
	m_pPicture->get_Width(&hmWidth);
	m_pPicture->get_Height(&hmHeight);
	
	if (m_pPicture->Render(pDC->m_hDC, x, y, w, h, 0, hmHeight, hmWidth, -hmHeight, NULL) == S_OK)
		return true;

	return false;
}

BOOL CPicture::DrawCBitmap(CDC *pDC, int x, int y, int w, int h) // CBitmap style (finally rendered using the CBitmap::StretchBlt call)
{
	// our mem DC
	CDC dcMem;
	dcMem.CreateCompatibleDC(pDC);
	CBitmap *memBitmap = new CBitmap;
	CBitmap *memOldBitmap;
	    
	// Get bitmap width/height
	long hmWidth;
	long hmHeight;
	m_pPicture->get_Width(&hmWidth);
	m_pPicture->get_Height(&hmHeight);

	int nWidth	= MulDiv(hmWidth,	pDC->GetDeviceCaps(LOGPIXELSX), HIMETRIC_INCH);
	int nHeight	= MulDiv(hmHeight,	pDC->GetDeviceCaps(LOGPIXELSY), HIMETRIC_INCH);
	
	memBitmap->CreateCompatibleBitmap(pDC, nWidth, nHeight);//24,396);
    memOldBitmap = dcMem.SelectObject(memBitmap);
		
	Draw(&dcMem, 0,0, nWidth, nHeight);
		             
	pDC->StretchBlt(x, y, w, h, &dcMem, 0, 0, nWidth, nHeight, SRCCOPY);

	dcMem.SelectObject(memOldBitmap);

	delete memBitmap;

	return TRUE;
}

BOOL CPicture::Draw(CDC *pDC, CPoint Pos, CSize Size)
{
	if (m_pPicture != NULL)
	{
		long hmWidth;
		long hmHeight;
		m_pPicture->get_Width(&hmWidth);
		m_pPicture->get_Height(&hmHeight);

		int nWidth	= Size.cx;
		int nHeight	= Size.cy;
						
		return Draw(pDC, Pos.x, Pos.y, nWidth, nHeight);
	}
	return false;
}

BOOL CPicture::LoadFromBuffer(BYTE *pBuff, int nSize)
{
	BOOL bResult = false;

	HGLOBAL hGlobal = GlobalAlloc(GMEM_MOVEABLE, nSize);
	void *pData = GlobalLock(hGlobal);
	memcpy(pData, pBuff, nSize);
	GlobalUnlock(hGlobal);

	IStream *pStream = NULL;

	if (CreateStreamOnHGlobal(hGlobal, TRUE, &pStream) == S_OK)
	{
		HRESULT hr;
		if ((hr = OleLoadPicture(pStream, nSize, FALSE, IID_IPicture, (LPVOID *)&m_pPicture)) == S_OK)
			bResult = true;
	
		pStream->Release();
	}

	return bResult;
}

BOOL CPicture::GetResource(LPSTR lpName, LPSTR lpType, void *pResource, int& nBufSize)
{ 
	HRSRC		hResInfo;
	HANDLE		hRes;
	HMODULE		hInst	= NULL; 
	LPSTR		lpRes	= NULL; 
	int			nLen	= 0;
	BOOL		bResult	= FALSE;

	// Find the resource
	hResInfo = FindResource(hInst, lpName, lpType);

	if (hResInfo == NULL) 
		return false;

	// Load the resource
	hRes = LoadResource(hInst, hResInfo);

	if (hRes == NULL) 
		return false;

	// Lock the resource
	lpRes = (char *)LockResource(hRes);

	if (lpRes != NULL)
	{ 
		if (pResource == NULL)
		{
			nBufSize = SizeofResource(hInst, hResInfo);
			bResult = true;
		}
		else
		{
			if (nBufSize >= (int)SizeofResource(hInst, hResInfo))
			{
				memcpy(pResource, lpRes, nBufSize);
				bResult = true;
			}
		} 

		UnlockResource(hRes);  
	}

	// Free the resource
	FreeResource(hRes);

	return bResult;
}

CSize CPicture::GetSize(CDC *pDC)
{
	CSize rcResult = CSize(0,0);
	
	if (m_pPicture != NULL)
	{
		long hmWidth;
		long hmHeight;
		m_pPicture->get_Width(&hmWidth);
		m_pPicture->get_Height(&hmHeight);

		rcResult.cx	= MulDiv(hmWidth,	pDC->GetDeviceCaps(LOGPIXELSX), HIMETRIC_INCH);
		rcResult.cy	= MulDiv(hmHeight,	pDC->GetDeviceCaps(LOGPIXELSY), HIMETRIC_INCH);
	}

	return rcResult;
}
