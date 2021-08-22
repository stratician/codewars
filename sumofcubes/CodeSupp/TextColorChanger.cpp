// TextColorChangerDlg.cpp : implementation file
//

#include "stdafx.h"
#include "..\Graphics\resource.h"
#include "TextColorChangerDlg.h"
#include "../CodeSupp/GameAppView.h"
#include "ImageDataObject.h"
#include "BitmapEx.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


#define MAX_SMILIES			32

#define GRID_SIZE_X			30
#define GRID_SIZE_Y			30

#define GRID_OFFSET_X		5
#define GRID_OFFSET_Y		30
#define GRID_BORDER_SIZE	3
#define MAX_BMP_SIZE_X		22
#define MAX_BMP_SIZE_Y		22


/////////////////////////////////////////////////////////////////////////////
// CTextColorChangerDlg dialog


CTextColorChangerDlg::CTextColorChangerDlg(CWnd* pParent)
	: CDialog(CTextColorChangerDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSmileySelectDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT


	FirstTime = true;

	access_level = 3;

	// Initialise Cursor Grid Coordinates to -1, so we don't draw a box by default //
	g_grid_x = -1;
	g_grid_y = -1;
	g_grid_oldx = g_grid_x;
	g_grid_oldy = g_grid_y;
	Text_Fade = 0; // Text Caption Defaultly Off

	m_bRefresh = false;
	m_bOpen = false;

	m_bMouseOver = false;

	// Default Text Color is a gray/white
	m_textColor = RGB(215, 215, 215);

}


void CTextColorChangerDlg::Init(BOOL bFirstTime)
{
	// Initialise Cursor Grid Coordinates to -1, so we don't draw a box by default //
	g_grid_x = -1;
	g_grid_y = -1;
	g_grid_oldx = g_grid_x;
	g_grid_oldy = g_grid_y;
	
	m_bOpen = true;
	

	if(bFirstTime)
	{
		// load bitmaps
		

		// BackGround
		smileyselectbg = (HBITMAP)LoadImage(0, "Graphics/images/colorpalette.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);

	}

	//  Back-Up our existing color
	m_textColorOld = m_textColor;
}


void CTextColorChangerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSmileySelectDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CTextColorChangerDlg, CDialog)
	//{{AFX_MSG_MAP(CSmileySelectDlg)
	ON_WM_PAINT()
	ON_BN_CLICKED(IDC_BUTTON1, OnButton1)
	ON_WM_CAPTURECHANGED()
	ON_WM_KILLFOCUS()
	ON_WM_MOUSEMOVE()
	ON_WM_ERASEBKGND()
	ON_WM_LBUTTONDOWN()
	ON_WM_DESTROY()
	ON_MESSAGE(WM_MOUSEHOVER, OnMouseOver)
	ON_MESSAGE(WM_MOUSELEAVE, OnMouseOut)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTextColorChangerDlg message handlers

void CTextColorChangerDlg::GetParent(CWnd *pParent)
{
	m_pParent = pParent;
}


void CTextColorChangerDlg::Process(void)
{
	/*
	if(m_bMouseOver)
	if(g_gameAppView->m_hCursorHand != NULL)
	{
		::SetCursor(g_gameAppView->m_hCursorHand);
	}
	*/
}


CBrush ColMouseOver;//(RGB(255,255,255));


// Double Buffer Method :D //
void CTextColorChangerDlg::OnPaint() 
{


	CPaintDC dc(this); // device context for painting
	
	// TODO: Add your message handler code here
	int i,j;
	int x,y;
	int idx;
	int nWidth, nHeight;
	int bWidth, bHeight;
		
	// Set Up Brush / Pen Styles //
	CBrush BrushBG(RGB(235,240,253));
	CBrush BrushWhite(RGB(255,255,255));
	CBrush BrushBlue(RGB(100,120,230));
	CBrush BrushBlue2(RGB(140,170,250));
	
	
	

	if(FirstTime || m_bRefresh)
	{
		FirstTime = false;
		m_bRefresh = false;


		ColMouseOver.CreateSolidBrush(m_textColor);



		// Create Surfaces //
		dcMem_buf.DeleteDC();
		dcMem.DeleteDC();


		dcMem_buf.CreateCompatibleDC(&dc);
		dcMem.CreateCompatibleDC(&dc);
	


		// Set BackGround Color //
		//dcMem_buf.FillRect(rc,&BrushBG);

		CPen thePen(PS_SOLID, 1, RGB(200,210,238));
		CPen thePenB(PS_SOLID, 1, RGB(175,180,222));
		CPen *pOldPen = dcMem_buf.SelectObject(&thePenB); 

	
		nWidth  = MAX_BMP_SIZE_X;
		nHeight = MAX_BMP_SIZE_Y;

		// Set Up BackGround and Initialise dcMed_buf layer with empty bitmap //
		CRect rcClient;
		GetClientRect(rcClient);
		CBitmap bitmap1;
		bitmap1.CreateCompatibleBitmap(&dc, rcClient.Width(), rcClient.Height());
		dcMem_buf.SelectObject(&bitmap1);

	
		// fill memory device context with the desired brush
		dcMem_buf.FillRect(rcClient, &BrushBG);

		// Background
		dcMem.SelectObject(smileyselectbg);
		dcMem_buf.BitBlt(0, 0, 256, 100, &dcMem, 0, 0, SRCCOPY);

   
		// dcMem_buf.Rectangle(0,150,rcClient.right,rcClient.bottom);


		dcMem_buf.SelectObject(&thePen);
	


		
		// Draw Color Mouse Over
		//dcMem_buf.Rectangle(58, 142, 140, 160);
		//dcMem_buf.BitBlt(58,2,140,20, &dcMem, 0, 0, SRCCOPY);

		// Only do this if we are level 3 or above

		if(g_gameAppView->m_playerSockIdx != -1 && g_gameAppView->m_playerIdx >= 0)
		{
			if( m_player[g_gameAppView->m_playerIdx].level >= access_level)
			{
				// Update TextColorChangerButton
				int pitch = 4;
				int width, height;
				CRect rect;
				g_gameAppView->m_sayButton.GetWindowRect(&rect);
		
				// First Create a Bitamp (plain color that will be used to update our button
				width = rect.Width();
				height = rect.Height();

				int bufSize = width * height * 4;
				BYTE *buf = new BYTE[bufSize];

				for(int j=0;j<height;j++)
				for(int i=0;i<width;i++)
				{
					buf[(i*pitch)+(j*pitch)*width] = (m_textColor>>16)&0xff;
					buf[(i*pitch)+(j*pitch)*width+1] = (m_textColor>>8)&0xff;
					buf[(i*pitch)+(j*pitch)*width+2] = (m_textColor)&0xff;
					buf[(i*pitch)+(j*pitch)*width+3] = 0;//col&0xff;
				}

				HBITMAP tmpBitmap = CreateBitmap(width, height, 1, 32, buf); 
	
				g_gameAppView->m_sayButton.SetBitmap(tmpBitmap);
			}

		}
	}

	
	
	//Blt the buffered Surface //
	CRect rcClip;
	dc.GetClipBox(rcClip);

	nWidth  = rcClip.Width();
	nHeight = rcClip.Height();
	x       = rcClip.left;
	y       = rcClip.top;

	dc.BitBlt(x, y, nWidth, nHeight, &dcMem_buf, x, y, SRCCOPY); 
	

	/*
	CRect rectCol;
	rectCol.left = 214;
	rectCol.top = 2;
	rectCol.right = 252;
	rectCol.bottom = 30;

	dc.FillRect(rectCol, &ColMouseOver);
	*/
	



	// Do not call CDialog::OnPaint() for painting messages
}



void CTextColorChangerDlg::OnButton1() 
{
	// TODO: Add your control notification handler code here
	//DestroyWindow();
	
}

void CTextColorChangerDlg::OnCaptureChanged(CWnd *pWnd) 
{
	// TODO: Add your message handler code here
	//DestroyWindow();

	CDialog::OnCaptureChanged(pWnd);
}

void CTextColorChangerDlg::OnKillFocus(CWnd* pNewWnd) 
{
	CDialog::OnKillFocus(pNewWnd);

	// TODO: Add your message handler code here
	
}



void CTextColorChangerDlg::OnMouseMove(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	


	TRACKMOUSEEVENT tme;
    tme.cbSize = sizeof(tme);
    tme.hwndTrack = m_hWnd;
    tme.dwFlags = TME_LEAVE|TME_HOVER;
    tme.dwHoverTime = 1;
    TrackMouseEvent(&tme);



	// Update the Mouse Over Color

	
	COLORREF col;

	col = dcMem_buf.GetPixel(point.x, point.y);
	
	// Update our Selected Colour too.
	UpdateColor(col, false);

	ColMouseOver.DeleteObject();
	ColMouseOver.CreateSolidBrush(col);
	/*
	int R = col&0xff;
	int G = (col>>8)&0xff;
	int B = (col>>16)&0xff;
	*/

	
	//ColMouseOver = col;//RGB(R,G,B);
		
	
	// Get Color that we are moused over
	
	Invalidate();
		
	CDialog::OnMouseMove(nFlags, point);
}




BOOL CTextColorChangerDlg::OnEraseBkgnd(CDC* pDC) 
{
	// TODO: Add your message handler code here and/or call default
	
	// All drawing moved in OnPaint
    return FALSE;

	return CDialog::OnEraseBkgnd(pDC);
}

void CTextColorChangerDlg::OnLButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default

	// Update our TextColorChanger Button

	if(g_gameAppView)
	{
		

		
		int col = dcMem_buf.GetPixel(point.x, point.y);
		UpdateColor(col);
		

		// Update SendEdit if necessary
		g_gameAppView->m_sendEdit.UpdateTextColor();

		// Notify Server of this if we're online
		g_gameAppView->SendTextColorChange(m_textColor);

	}

	CDialog::OnLButtonDown(nFlags, point);
}


void CTextColorChangerDlg::UpdateColor(COLORREF col, bool bUpdate) 
{
	if(g_gameAppView->m_playerIdx == -1) return;

	// Only do this if we have access level
	if( m_player[g_gameAppView->m_playerIdx].level < access_level)
	{
		// If we don't have access then set the button back to default
		m_textColor = 14803425;

		// Set button back to default
		g_gameAppView->m_sayButton.SetBitmap(Skin_ButtonSayOn);

		return;
	}
	

	if(bUpdate)
	{
		m_textColor = col;
		m_textColorOld = m_textColor;
	}

	int pitch = 4;
	int width, height;
	CRect rect;
	g_gameAppView->m_sayButton.GetWindowRect(&rect);
		
	// First Create a Bitamp (plain color that will be used to update our button
	width = rect.Width();
	height = rect.Height();

	int bufSize = width * height * 4;


	BYTE *buf = new BYTE[bufSize];
	for(int j=0;j<height;j++)
	for(int i=0;i<width;i++)
	{			
			
		buf[(i*pitch)+(j*pitch)*width] = (col>>16)&0xff;
		buf[(i*pitch)+(j*pitch)*width+1] = (col>>8)&0xff;
		buf[(i*pitch)+(j*pitch)*width+2] = (col)&0xff;
		buf[(i*pitch)+(j*pitch)*width+3] = 0;//col&0xff;
			
			
	}

	HBITMAP tmpBitmap = CreateBitmap(width, height, 1, 32, buf); 
	
	g_gameAppView->m_sayButton.SetBitmap(tmpBitmap);

	
}


void CTextColorChangerDlg::OnDestroy() 
{
	m_bOpen = false;

	CDialog::OnDestroy();
	
	// TODO: Add your message handler code here

	// Make sure we have the previous colour
	UpdateColor(m_textColorOld);
}



LRESULT CTextColorChangerDlg::OnMouseOver(WPARAM wparam, LPARAM lparam) 
{
	// OnMouseOver

	// Change Mouse to Hand / Pointer
	m_bMouseOver = true;

	/*
    CDC* pDC = GetWindowDC();
    CRect rect;
    GetWindowRect(&rect);
    rect.OffsetRect( -rect.left, -rect.top);
    CBrush brush( RGB(0, 0, 255));
    pDC->FrameRect( &rect, &brush);
    ReleaseDC(pDC);
	*/
    return true;
}


LRESULT CTextColorChangerDlg::OnMouseOut(WPARAM wparam, LPARAM lparam) 
{
	// OnMouseOut

	

	// Change Mouse to cursor
	
	m_bMouseOver = false;

	/*
    CDC* pDC = GetWindowDC();
    CRect rect;
    GetWindowRect(&rect);
    rect.OffsetRect( -rect.left, -rect.top);
    CBrush brush( RGB(0, 0, 255));
    pDC->FrameRect( &rect, &brush);
    ReleaseDC(pDC);
	*/
    return true;
}

