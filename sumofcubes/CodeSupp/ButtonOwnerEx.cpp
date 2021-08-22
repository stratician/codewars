// ButtonOwnerEx.cpp : implementation file
//

#include "stdafx.h"
#include "GameApp.h"
#include "GameAppView.h"
#include "ButtonOwnerEx.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CButtonOwnerEx

CButtonOwnerEx::CButtonOwnerEx()
{
	m_bColorTab = FALSE;

	m_hIcon = 0;
	m_hBitmap = 0;
	m_hBitmapDisabled = 0;

	m_bDefault = FALSE;
	m_nOldState = 0;
	m_nOldAction = 0;

	m_nImageOffsetFromBorder = 4;
	m_nTextOffsetFromImage = 8;
	m_bUseOffset = TRUE;

	//m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

	m_strText = _T("");
	m_pointText = CPoint(0,0);
}

CButtonOwnerEx::~CButtonOwnerEx()
{
	if( m_hIcon != 0 )
		DeleteObject( m_hIcon );
	if( m_hBitmap != 0 )
		DeleteObject( m_hBitmap );
	if( m_hBitmapDisabled != 0 )
		DeleteObject( m_hBitmapDisabled );
}

BEGIN_MESSAGE_MAP(CButtonOwnerEx, CButton)
	//{{AFX_MSG_MAP(CButtonOwnerEx)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CButtonOwnerEx message handlers

inline void CButtonOwnerEx::Redraw()
{
	if( m_hWnd != NULL )
		Invalidate();
}

//
//	Various Attribute setting functions
//

//	Image functions
BOOL CButtonOwnerEx::SetIcon( UINT nID, int nWidth, int nHeight )
{
	m_hIcon = (HICON)::LoadImage(AfxGetApp()->m_hInstance, MAKEINTRESOURCE(nID), IMAGE_ICON, nWidth, nHeight, 0);

	if( m_hIcon == 0 )
		return FALSE;

	m_nImageWidth = nWidth;
	m_nImageHeight = nHeight;

	m_hBitmap = 0;

	Redraw();

	return TRUE;
}

//BOOL CButtonOwnerEx::SetBitmap( UINT nID, int nWidth, int nHeight )
//{
//	return SetBitmapCommon( nID, nWidth, nHeight, 0, FALSE /* no mask */ );
//}
 
//BOOL CButtonOwnerEx::SetMaskedBitmap( UINT nID, int nWidth, int nHeight, COLORREF crTransparentMask )
//{
//	return SetBitmapCommon( nID, nWidth, nHeight, crTransparentMask, TRUE /* mask */ );
//}

/*
BOOL CButtonOwnerEx::SetBitmapCommon( UINT nID, int nWidth, int nHeight, COLORREF crTransparentMask, BOOL bUseMask )
{
	// If it is not a masked bitmap, just go through the
	// motions as if it was, but set then number of color mappings to 0
	COLORMAP	mapColor;
	mapColor.from = crTransparentMask;
	mapColor.to  = ::GetSysColor( COLOR_BTNFACE );
	HBITMAP bmTemp = (HBITMAP)::CreateMappedBitmap(AfxGetApp()->m_hInstance, nID, IMAGE_BITMAP, &mapColor, bUseMask ? 1 : 0 );
	m_hBitmap = (HBITMAP)::CopyImage( bmTemp, IMAGE_BITMAP, nWidth, nHeight, LR_COPYDELETEORG );

	// Create disabled bitmap.  We need to make the masked area white so
	// it will appear transparent when the bitmap is rendered as an
	// 'embossed' (disabled) image in DrawItem() below.  Since DrawState 
	// converts the image to monochrome, white is transparent, black is 
	// graphics data.
	mapColor.to  = RGB( 255, 255, 255 );
	bmTemp = (HBITMAP)::CreateMappedBitmap(AfxGetApp()->m_hInstance, nID, IMAGE_BITMAP, &mapColor, bUseMask ? 1 : 0 );
	m_hBitmapDisabled = (HBITMAP)::CopyImage( bmTemp, IMAGE_BITMAP, nWidth, nHeight, LR_COPYDELETEORG );
  
	if( m_hBitmap == 0 || m_hBitmapDisabled == 0 )
		return FALSE; 

	m_nImageWidth = nWidth;
	m_nImageHeight = nHeight;

	m_hIcon = 0;

	Redraw();

	return TRUE;
}

void CButtonOwnerEx::SetColorTab(COLORREF crTab)
{	
	m_bColorTab = TRUE;
	m_crColorTab = crTab;

	Redraw();
}

BOOL CButtonOwnerEx::SetDefaultButton( BOOL bState )
{
	CDialog *pDialog = (CDialog *)GetOwner();
//	ASSERT( pDialog->IsKindOf( RUNTIME_CLASS( CDialog ) ) );

	pDialog->SetDefID( GetDlgCtrlID() );

	BOOL bPrevious = m_bDefault;
	m_bDefault = bState;

	Redraw();

	// Return previous state
	return bPrevious;
}

//	Positioning Functions
int CButtonOwnerEx::SetImageOffset( int nPixels )
{
	int nPrevious = m_nImageOffsetFromBorder;

	m_bUseOffset = TRUE; 
	m_nImageOffsetFromBorder = nPixels; 

	Redraw();

	return nPrevious;
}

int CButtonOwnerEx::SetTextOffset( int nPixels ) 
{ 
	int nPrevious = m_nTextOffsetFromImage;

	m_bUseOffset = TRUE; 
	m_nTextOffsetFromImage = nPixels; 

	Redraw();

	return nPrevious;
}
*/
CPoint CButtonOwnerEx::SetImagePos( CPoint p ) 
{ 
	CPoint pointPrevious = m_pointImage;

	m_bUseOffset = FALSE; 
	m_pointImage = p; 

	Redraw();

	return pointPrevious;
}

CPoint CButtonOwnerEx::SetTextPos( CPoint p ) 
{ 
	CPoint pointPrevious = m_pointText;

	m_bUseOffset = FALSE; 
	m_pointText = p; 

	Redraw();

	return pointPrevious;
}

CString CButtonOwnerEx::SetText( CString s )
{ 
	CString strPrevious = m_strText;

	m_strText = s;

	Redraw();

	return strPrevious;
}

//	Centering a point helper function
void CButtonOwnerEx::CheckPointForCentering( CPoint &p, int nWidth, int nHeight )
{
	CRect	rectControl;
	GetClientRect( rectControl );

	if( p.x == BUTTONOWNEREX_CENTER )
		p.x = ( ( rectControl.Width() - nWidth ) >> 1 );
	if( p.y == BUTTONOWNEREX_CENTER )
		p.y = ( ( rectControl.Height() - nHeight ) >> 1 );
}

void CButtonOwnerEx::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct) 
{
	// TODO: Add your code to draw the specified item
	CDC* pDC = CDC::FromHandle(lpDrawItemStruct->hDC);

	CRect	rectControl(lpDrawItemStruct->rcItem);
	
	UINT	nOffset = 0;			// For position adjustment of a pushed button
	UINT	nFrameStyle=0;
	BOOL	bDRAWFOCUSONLY = FALSE;	// Optimize a bit
	int		nStateFlag;				// Normal or Disabled
	HBITMAP	hBitmapToDraw;			// Normal or Disabled bitmap (not used if uses icon)
	
	UINT	nNewState = lpDrawItemStruct->itemState;
	UINT	nNewAction = lpDrawItemStruct->itemAction;
	
	//	Find out what state the control and set some drawing flags 
	//	according to the state.
	if ( nNewState & ODS_SELECTED)
	{
		nFrameStyle = DFCS_PUSHED;
		nOffset += 1;
	}

	if( nNewState & ODS_DISABLED )
	{
		nStateFlag = DSS_DISABLED;
		hBitmapToDraw = m_hBitmapDisabled;
	}
	else
	{
		nStateFlag = DSS_NORMAL;
		hBitmapToDraw = m_hBitmap;
	}

	// If only the focus is changing, don't redraw the whole control
	if (nNewAction == ODA_FOCUS )
		bDRAWFOCUSONLY = TRUE;

	// If this is the defualt button, deflate the control so everything 
	// we do below (icon, text, focus ) is adjusted properly
	if( m_bDefault )
		rectControl.DeflateRect( 1, 1 );

	if( !bDRAWFOCUSONLY )
	{
		//
		// Draw 'default button' rectangle
		//
		if( m_bDefault ) // Can't use ODS_DEFAULT w/owner draw!!
		{
			CPen *pOldPen = (CPen*)pDC->SelectStockObject(BLACK_PEN);
							
			pDC->Rectangle( &lpDrawItemStruct->rcItem ); // don't use deflated rectangle
			pDC->SelectObject( pOldPen );
		}

		//
		//	Draw button frame
		//
		pDC->DrawFrameControl(&rectControl, DFC_BUTTON, DFCS_BUTTONPUSH | nFrameStyle);

		//
		//	Draw color tab
		//
		if (m_bColorTab)
		{
			CPen penTab; 
			
			#define COLORTABSIZE 8
			if( penTab.CreatePen( PS_SOLID, 1, m_crColorTab) )
			{
				CPen* pOldPen = pDC->SelectObject( &penTab );
							
				int nXOffset = rectControl.left+1 + nOffset;
				int nYOffset = rectControl.top+1 + nOffset;
				for (UINT nStep = 0; nStep < COLORTABSIZE; nStep++)
				{
//					pDC->MoveTo( nXOffset, nYOffset + nStep );
//					pDC->LineTo( nXOffset + (COLORTABSIZE-nStep)-1, nYOffset + nStep );			
				}
        
				pDC->SelectObject( pOldPen );
			}
		}

		//	Get control text
		CString	strTitle;
		this->GetWindowText(strTitle);
		strTitle = m_strText; // *new - instead of using title of button

		//
		//	Draw Image
		//
		if( HasImage() )
		{
			CPoint pt;

			if( m_bUseOffset )
			{
				pt.x = strTitle.IsEmpty() ? BUTTONOWNEREX_CENTER : rectControl.left + m_nImageOffsetFromBorder;
				pt.y = BUTTONOWNEREX_CENTER;
			}
			else
				pt = m_pointImage;

			CheckPointForCentering( pt, m_nImageWidth, m_nImageHeight );
			
			pt.Offset( nOffset, nOffset );

			if( m_hIcon )
				pDC->DrawState( pt, CSize(m_nImageWidth, m_nImageHeight), (HICON)m_hIcon, DST_ICON | nStateFlag, (CBrush *)NULL );
			else if( m_hBitmap )
				pDC->DrawState( pt, CSize(m_nImageWidth, m_nImageHeight), (HBITMAP)hBitmapToDraw, DST_BITMAP | nStateFlag );
		}

		//	
		//	Draw Text
		//
		if ( !strTitle.IsEmpty() )
		{
			CPoint	pt(0,0);
			CSize	sizeText = pDC->GetTextExtent(strTitle);

			if( m_bUseOffset )
			{
				pt.x = !HasImage() ? BUTTONOWNEREX_CENTER : m_nImageWidth + m_nTextOffsetFromImage + m_nImageOffsetFromBorder;
				pt.y = BUTTONOWNEREX_CENTER; 
			}
			else
				pt = m_pointText;

			//	If we are centering the text vertically, it looks best of we
			//	center based on the height of the text, then move it up 1 more pixel
			int nOffsetFixY = pt.y == BUTTONOWNEREX_CENTER ? -1 : 0;

			CheckPointForCentering( pt, sizeText.cx, sizeText.cy );

			pt.Offset( nOffset, nOffset + nOffsetFixY );

			pDC->SetBkMode(TRANSPARENT);
			pDC->DrawState( pt, CSize(0,0), strTitle, DST_PREFIXTEXT|nStateFlag, TRUE, 0, (CBrush*)NULL );
		}
 
	} // End !focus only
  
	//
	//	Draw focus rectange
	//
	if( !( nNewState & ODS_DISABLED ) )	// Make sure it's not disabled
	{
		// Redraw the focus if:
		//		1. There is a change in focus state 
		// OR	2. The entire control was just redrawn and Focus is set
		if( ( nNewState & ODS_FOCUS ) ^ ( m_nOldState & ODS_FOCUS ) ||
			( !bDRAWFOCUSONLY && ( nNewState & ODS_FOCUS ) ) )
		{

			#define FOCUSOFFSET 3
			CRect rect( rectControl );

			// As control gets smaller, decrease focus size
			int nDeflate = min( FOCUSOFFSET,
								min( rect.Width(), rect.Height() ) >> 2 );
			rect.DeflateRect( nDeflate, nDeflate);
			pDC->DrawFocusRect(&rect);
		}
	}

	m_nOldAction = nNewAction;
	m_nOldState = nNewState;
}
