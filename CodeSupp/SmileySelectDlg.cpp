// SmileySelectDlg.cpp : implementation file
//

#include "stdafx.h"
#include "..\Graphics\resource.h"
#include "SmileySelectDlg.h"
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

//IRichEditOle	*m_pRichEditOle;

int g_grid_x, g_grid_oldx;
int g_grid_y, g_grid_oldy;

BOOL FirstTime = TRUE;
float Text_Fade;

CBitmap stitle;
CBitmap sfooter;
CBitmap bitmap[35];

HBITMAP smileyselectbg;
HBITMAP stitleEx;
HBITMAP bitmapEx[32];


char *smiley_lst_name[32] ={"Smile",
							"Wink",						
							"Laugh",
							"Sad",
							"Shock",
							"Slap",
							"Mad",
							"Tongue",
							"Puzzled",
							"Cool",
							"Blush",
							"Surprise",
							"Cry", //13
							"BigGrin",
							"Mischief",
							"Mouth",
							"Girl", //17
							"Devil",
							"Pleased",
							"Santa", //20
							"Pirate",
							"Sad",
							"Santa",
							"Sleep",
							"Sly",
							"Smile",
							"Tongue",
							"UnSure",
							"Whistle",
							"Wink",
							"Woot",
							"Wub"};

char *smileyFName[1+32] = {
"Graphics/Smileys/smileyselect_title.bmp",
"Graphics/Smileys/f/smile.bmp",
"Graphics/Smileys/f/wink.bmp",
"Graphics/Smileys/f/laugh.bmp",
"Graphics/Smileys/f/sad.bmp",
"Graphics/Smileys/f/shock.bmp",
"Graphics/Smileys/f/slap.bmp",
"Graphics/Smileys/f/mad.bmp",
"Graphics/Smileys/f/tongue.bmp",
"Graphics/Smileys/f/puzzled.bmp",
"Graphics/Smileys/f/cool.bmp",
"Graphics/Smileys/f/blush.bmp",
"Graphics/Smileys/f/surprise.bmp",
"Graphics/Smileys/f/cry.bmp",
"Graphics/Smileys/f/biggrin.bmp",
"Graphics/Smileys/f/mischief.bmp",
"Graphics/Smileys/f/mouth.bmp",
"Graphics/Smileys/f/girl.bmp",
"Graphics/Smileys/f/devil.bmp",
"Graphics/Smileys/f/pleased.bmp",
"Graphics/Smileys/f/santa.bmp",
"Graphics/Smileys/b/pirate.bmp",
"Graphics/Smileys/b/sad.bmp",
"Graphics/Smileys/b/santa.bmp",
"Graphics/Smileys/b/sleep.bmp",
"Graphics/Smileys/b/sly.bmp",
"Graphics/Smileys/b/smile.bmp",
"Graphics/Smileys/b/tongue.bmp",
"Graphics/Smileys/b/unsure.bmp",
"Graphics/Smileys/b/whistle.bmp",
"Graphics/Smileys/b/wink.bmp",
"Graphics/Smileys/b/woot.bmp",
"Graphics/Smileys/b/wub.bmp"
};
						
	

/////////////////////////////////////////////////////////////////////////////
// CSmileySelectDlg dialog


CSmileySelectDlg::CSmileySelectDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSmileySelectDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSmileySelectDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

/*
	stitle.LoadBitmap(IDB_STITLE);
	sfooter.LoadBitmap(IDB_SFOOTER);
			
	bitmap[0].LoadBitmap(IDB_BITMAP19);
	bitmap[1].LoadBitmap(IDB_BITMAP20);
	bitmap[2].LoadBitmap(IDB_BITMAP21);
	bitmap[3].LoadBitmap(IDB_BITMAP22);
	bitmap[4].LoadBitmap(IDB_BITMAP23);
	bitmap[5].LoadBitmap(IDB_BITMAP24);
	bitmap[6].LoadBitmap(IDB_BITMAP25);
	bitmap[7].LoadBitmap(IDB_BITMAP26);
	bitmap[8].LoadBitmap(IDB_BITMAP27);
	bitmap[9].LoadBitmap(IDB_BITMAP28);
	bitmap[10].LoadBitmap(IDB_BITMAP29);
	bitmap[11].LoadBitmap(IDB_BITMAP30);
	bitmap[12].LoadBitmap(IDB_BITMAP31);
	bitmap[13].LoadBitmap(IDB_BITMAP32);
	bitmap[14].LoadBitmap(IDB_BITMAP33);
	bitmap[15].LoadBitmap(IDB_BITMAP34);
	bitmap[16].LoadBitmap(IDB_BITMAP35);
	bitmap[17].LoadBitmap(IDB_BITMAP36);
	bitmap[18].LoadBitmap(IDB_BITMAP37);
	bitmap[19].LoadBitmap(IDB_BITMAP38);
	bitmap[20].LoadBitmap(IDB_BITMAP39);
	bitmap[21].LoadBitmap(IDB_BITMAP40);
	bitmap[22].LoadBitmap(IDB_BITMAP41);
	bitmap[23].LoadBitmap(IDB_BITMAP42);
	bitmap[24].LoadBitmap(IDB_BITMAP43);
	bitmap[25].LoadBitmap(IDB_BITMAP44);
	bitmap[26].LoadBitmap(IDB_BITMAP45);
	bitmap[27].LoadBitmap(IDB_BITMAP46);
	bitmap[28].LoadBitmap(IDB_BITMAP47);
	bitmap[29].LoadBitmap(IDB_BITMAP48);
	bitmap[30].LoadBitmap(IDB_BITMAP49);
	bitmap[31].LoadBitmap(IDB_BITMAP50);
	*/

	// Initialise Cursor Grid Coordinates to -1, so we don't draw a box by default //
	g_grid_x = -1;
	g_grid_y = -1;
	g_grid_oldx = g_grid_x;
	g_grid_oldy = g_grid_y;
	Text_Fade = 0; // Text Caption Defaultly Off

	m_bRefresh = false;
	m_bOpen = false;
}



void CSmileySelectDlg::Init(BOOL bFirstTime)
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
		smileyselectbg = (HBITMAP)LoadImage(0, "Graphics/Smileys/smileyselect_bg.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);

		// Title
		stitleEx = (HBITMAP)LoadImage(0, smileyFName[0], IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);



		
		for(int n=0;n<32;n++)
			bitmapEx[n] = (HBITMAP)LoadImage(0, smileyFName[1+n], IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
			//bitmapEx[n].LoadBitmap(smileyFName[1+n]);
	}
}



void CSmileySelectDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSmileySelectDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSmileySelectDlg, CDialog)
	//{{AFX_MSG_MAP(CSmileySelectDlg)
	ON_WM_PAINT()
	ON_BN_CLICKED(IDC_BUTTON1, OnButton1)
	ON_WM_CAPTURECHANGED()
	ON_WM_KILLFOCUS()
	ON_WM_MOUSEMOVE()
	ON_WM_ERASEBKGND()
	ON_WM_LBUTTONDOWN()
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSmileySelectDlg message handlers

void CSmileySelectDlg::GetParent(CWnd *pParent)
{
	m_pParent = pParent;
}



HBITMAP smiley_test	= (HBITMAP)LoadImage(0, "Graphics/Smileys/b/biggrin2.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
CButton m_smileyselect_btn[MAX_SMILIES];



CDC dcMem_buf;
CDC dcMem;
CRect rc;
BITMAP bmp;

// Double Buffer Method :D //
void CSmileySelectDlg::OnPaint() 
{
	
	CPaintDC dc(this); // device context for painting
	/*
	CPen pen(PS_SOLID, 2, RGB(0,0,0));
	CPen pen2(PS_NULL, 2, RGB(0,0,0));
	CBrush brush1(RGB(255,0,0));
	CBrush brushLtGreen(RGB(100,255,100));
	CBrush brushRed(RGB(255,0,0));
	CBrush brushGreen(RGB(0,255,0));
	CBrush brushGrey(RGB(127,127,127));

	CPen *oldPen = dc.SelectObject(&pen);
	CBrush *oldBrush = dc.SelectObject(&brush1);	
	
	dc.Rectangle(0,0,200,200);
	stitleEx.Draw(&dc, 10, 10);

	CDC *pDC = GetDC();
	if(pDC)
	{
		stitleEx.Draw(pDC, 50, 50);
		ReleaseDC(pDC);
	}

	dc.SelectObject(oldPen);
	dc.SelectObject(oldBrush);	
	*/

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
		dcMem_buf.BitBlt(1,26,240,120, &dcMem, 0, 0, SRCCOPY);

   
		dcMem_buf.Rectangle(0,150,rcClient.right,rcClient.bottom);


		dcMem_buf.SelectObject(&thePen);
	


		// Draw Title and Footer //
		dcMem.SelectObject(stitleEx);
		dcMem_buf.BitBlt(58,2,140,20, &dcMem, 0, 0, SRCCOPY);


		// Draw Smiley Array ///
		// 4x8 array
		idx=0;
		int xpos,ypos,xoffset,yoffset;
	
		// We give 1 Smiley per level

		for(j=0;j<4;j++)
		for(i=0;i<8;i++)
		{
			if(idx<=31)
			{
				dcMem_buf.Rectangle(CRect((GRID_OFFSET_X+i*GRID_SIZE_X)-GRID_BORDER_SIZE,(GRID_OFFSET_Y+j*GRID_SIZE_Y)-GRID_BORDER_SIZE,(GRID_OFFSET_X+i*GRID_SIZE_X)+nWidth+GRID_BORDER_SIZE,(GRID_OFFSET_Y+j*GRID_SIZE_Y)+nHeight+GRID_BORDER_SIZE));
				//pOldBitmap = 
				dcMem.SelectObject(bitmapEx[idx]);

			
				// Get BMP Coords //
				xoffset = 2;
				yoffset = 2;
				//if(idx==30) xoffset=-2;
				//if(idx==31) xoffset=0;
				xpos = GRID_OFFSET_X+i*GRID_SIZE_X+xoffset;
				ypos = GRID_OFFSET_Y+j*GRID_SIZE_Y+yoffset;
			
				// Blt Bitmap to Temp Surface //
				//dcMem_buf.BitBlt(xpos, ypos, bWidth, bHeight, &dcMem, 0, 0, SRCCOPY); // Note : bWidth/bHeight not set!!
				BITMAP bmpInfo;
				GetObject( bitmapEx[idx], sizeof( bmpInfo ), &bmpInfo );
				dcMem_buf.BitBlt(xpos, ypos, bmpInfo.bmWidth, bmpInfo.bmHeight, &dcMem, 0, 0, SRCCOPY);
			}

			idx++;
			
		
			
			// If we're offline (only give 1 emotion)
			// If Online, we break out if we have given Level Emoticons already
			if(g_gameAppView->m_playerSockIdx != -1 && g_gameAppView->m_playerIdx >= 0) 
			{
				if((idx*2) >= m_player[g_gameAppView->m_playerIdx].level)
				{
					idx = 32;
					break;
				}
			}
			else 
			{
				
				if(idx>=1)
				{
					idx = 32;
					break;
				}
				
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

	// Clear Old CursorSelect //
	if(g_grid_x!=-1 && g_grid_y!=-1 )
		{
		dc.FrameRect(CRect(GRID_OFFSET_X+g_grid_oldx*GRID_SIZE_X-GRID_BORDER_SIZE, GRID_OFFSET_Y+g_grid_oldy*GRID_SIZE_Y-GRID_BORDER_SIZE, GRID_OFFSET_X+g_grid_oldx*GRID_SIZE_X+nWidth+GRID_BORDER_SIZE, GRID_OFFSET_Y+g_grid_oldy*GRID_SIZE_Y+nHeight+GRID_BORDER_SIZE), &BrushBG); 
		dc.FrameRect(CRect(GRID_OFFSET_X+g_grid_oldx*GRID_SIZE_X-GRID_BORDER_SIZE+1, GRID_OFFSET_Y+g_grid_oldy*GRID_SIZE_Y-GRID_BORDER_SIZE+1, GRID_OFFSET_X+g_grid_oldx*GRID_SIZE_X+nWidth+GRID_BORDER_SIZE-1, GRID_OFFSET_Y+g_grid_oldy*GRID_SIZE_Y+nHeight+GRID_BORDER_SIZE-1), &BrushBG); 
		}
*/
	// Draw Cursor Select //
	nWidth  = MAX_BMP_SIZE_X;
	nHeight = MAX_BMP_SIZE_Y;
	if(g_grid_x!=-1 && g_grid_y!=-1 )
		{
		dc.FrameRect(CRect(GRID_OFFSET_X+g_grid_x*GRID_SIZE_X-GRID_BORDER_SIZE, GRID_OFFSET_Y+g_grid_y*GRID_SIZE_Y-GRID_BORDER_SIZE, GRID_OFFSET_X+g_grid_x*GRID_SIZE_X+nWidth+GRID_BORDER_SIZE, GRID_OFFSET_Y+g_grid_y*GRID_SIZE_Y+nHeight+GRID_BORDER_SIZE), &BrushBlue); 
		dc.FrameRect(CRect(GRID_OFFSET_X+g_grid_x*GRID_SIZE_X-GRID_BORDER_SIZE+1, GRID_OFFSET_Y+g_grid_y*GRID_SIZE_Y-GRID_BORDER_SIZE+1, GRID_OFFSET_X+g_grid_x*GRID_SIZE_X+nWidth+GRID_BORDER_SIZE-1, GRID_OFFSET_Y+g_grid_y*GRID_SIZE_Y+nHeight+GRID_BORDER_SIZE-1), &BrushBlue); 
		}

		
	// Display the Caption //
	// This will use a Fade On / Off effect //

	if(g_grid_x!=-1 && g_grid_y!=-1 )
		{
		//choose the font
		CFont m_Font;
		LOGFONT* m_pLF;
		char buf[80];
		m_pLF=(LOGFONT*)calloc(1,sizeof(LOGFONT));
		strncpy(m_pLF->lfFaceName,"Times New Roman",31);
		m_pLF->lfHeight=16;
		m_pLF->lfWeight=200;
		m_pLF->lfItalic=0;
		m_pLF->lfUnderline=0;
		m_Font.CreateFontIndirect(m_pLF);

		//select the font in the dc
		//dc.Attach(TmpDC);
		CFont* pOldFont=NULL;
		if (m_Font.m_hObject) 
			pOldFont = dc.SelectObject(&m_Font);
		else 
			dc.SelectObject(GetStockObject(DEFAULT_GUI_FONT));

		//Set text color
		dc.SetTextColor(RGB(0,0,0));
		//Set text position;
		RECT pos = {10,156,0,0};
		//draw the Footer text with Smiley Short Cut Code
		dc.SetBkMode(TRANSPARENT);
		sprintf(buf,"Smiley Code:");
		dc.DrawText(buf,strlen(buf),&pos,DT_CALCRECT);
		dc.DrawText(buf,strlen(buf),&pos,0);
		

		/*m_pLF->lfHeight=20;
		m_pLF->lfWeight=600;
		
		m_Font.DeleteObject();
		m_Font.CreateFontIndirect(m_pLF);
		dc.SelectObject(&m_Font);*/
		RECT pos2 = {100,156,0,0};
		sprintf(buf,"(%s)    %s", smiley_lst_name[g_grid_x+g_grid_y*8], smiley_lst[g_grid_x+g_grid_y*8]);
		dc.DrawText(buf,strlen(buf),&pos2,DT_CALCRECT);
		dc.DrawText(buf,strlen(buf),&pos2,0);


		//cleanup 
		if (pOldFont) dc.SelectObject(pOldFont);
		m_Font.DeleteObject();
		//dc.Detach();
		free(m_pLF);
		}	
	

	// Update the Old Grid Coordinate //
	g_grid_oldx = g_grid_x;
	g_grid_oldy = g_grid_y;

	// Do not call CDialog::OnPaint() for painting messages
}



void CSmileySelectDlg::OnButton1() 
{
	// TODO: Add your control notification handler code here
	//DestroyWindow();
}

void CSmileySelectDlg::OnCaptureChanged(CWnd *pWnd) 
{
	// TODO: Add your message handler code here
	//DestroyWindow();

	CDialog::OnCaptureChanged(pWnd);
}

void CSmileySelectDlg::OnKillFocus(CWnd* pNewWnd) 
{
	CDialog::OnKillFocus(pNewWnd);

	// TODO: Add your message handler code here
	
}

void CSmileySelectDlg::OnMouseMove(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	
	// Update the Curser Select global Grid Coordinaates
	if(point.x>=GRID_OFFSET_X)
	if(point.y>=GRID_OFFSET_Y)
		{
		g_grid_x = ((point.x-GRID_OFFSET_X)/GRID_SIZE_X);
		g_grid_y = ((point.y-GRID_OFFSET_Y)/GRID_SIZE_Y);

		// Restrict to levels
		if(g_gameAppView->m_playerSockIdx != -1 && g_gameAppView->m_playerIdx >= 0)
		{
			if( (g_grid_x*2 + g_grid_y*8*2) >= m_player[g_gameAppView->m_playerIdx].level )
			{
				g_grid_x = -1;
				g_grid_y = -1;
			}
		}
		else
		{
			if( (g_grid_x*2 + g_grid_y*8*2) >= 1 )
			{
				g_grid_x = -1;
				g_grid_y = -1;
			}
		}

		// Bounding Boxes Detections //
		if(g_grid_x>7) g_grid_x = 7;
		if(g_grid_y>3) g_grid_y = 3;
		//if(g_grid_y==3) if(g_grid_x>5) g_grid_x=5;
		}

	// Cause a Redraw Message, if the New Grid Position is different to the Old

	if(g_grid_x!=g_grid_oldx || g_grid_y!=g_grid_oldy) Invalidate(); // causes an ondraw message
		
	CDialog::OnMouseMove(nFlags, point);
}




BOOL CSmileySelectDlg::OnEraseBkgnd(CDC* pDC) 
{
	// TODO: Add your message handler code here and/or call default
	
	// All drawing moved in OnPaint
    return FALSE;

	return CDialog::OnEraseBkgnd(pDC);
}

void CSmileySelectDlg::OnLButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	
	// Send Smiley ShortCut to SendEdit Chat box //
	
	// Get Smiley Idx (1-40)
	
	int c_grid_x = -1, c_grid_y = -1;
	
	// Get Current Mouse Grid Coords //
	if(point.x>=GRID_OFFSET_X)
	if(point.y>=GRID_OFFSET_Y)
	{
		c_grid_x = ((point.x-GRID_OFFSET_X)/GRID_SIZE_X);
		c_grid_y = ((point.y-GRID_OFFSET_Y)/GRID_SIZE_Y);
	}


	// Restrict to levels
	if(g_gameAppView->m_playerSockIdx != -1 && g_gameAppView->m_playerIdx >= 0)
	{
		if( (c_grid_x*2 + c_grid_y*8*2) >= m_player[g_gameAppView->m_playerIdx].level )
		{
			return;
		}
	}
	else
	{
		if( (c_grid_x*2 + c_grid_y*8*2) >= 1 )
		{
			return;
		}
	}



	// Bounding Boxes Detections //
	//if(c_grid_y==3) if(c_grid_x>5) c_grid_x=-1;
	if(c_grid_x>7) c_grid_x = -1;
	if(c_grid_y>3) c_grid_y = -1;
	

	if(c_grid_x>=0 && c_grid_y>=0)
		{
		int smiley_idx;
		smiley_idx = g_grid_x+g_grid_y*8;

		// Setup the Text Format & Get current sendEdit Chat //
		CHARFORMAT cf;
		cf.dwMask = CFM_ITALIC | CFM_UNDERLINE | CFM_COLOR;
		cf.dwEffects = 0;
		cf.crTextColor = RGB(215,215,215);

		
		/*
		// Modify the paragraph format so that the text is centered. 
		PARAFORMAT pf;
		pf.cbSize = sizeof(PARAFORMAT);
		pf.dwMask = PFM_ALIGNMENT;
		pf.wAlignment = PFA_LEFT;
		g_gameAppView->m_sendEdit.SetParaFormat(pf);
		*/
		

		// Append Smiley Code at End //
		int iTotalTextLength = g_gameAppView->m_sendEdit.GetWindowTextLength();
		//g_gameAppView->m_sendEdit.SetSelectionCharFormat(cf);
		g_gameAppView->m_sendEdit.SetSel(iTotalTextLength, iTotalTextLength);

		cf.cbSize = sizeof(CHARFORMAT);	
		g_gameAppView->m_sendEdit.SetSelectionCharFormat(cf);
		
		/*
		// Let try put in the real Smiley Picture //
		m_pRichEditOle = g_gameAppView->m_sendEdit.GetIRichEditOle();
		ASSERT(m_pRichEditOle != NULL);
		// Only do if the m_pRichEditOle is Valid //
		if(m_pRichEditOle != NULL) CImageDataObject::InsertBitmap(m_pRichEditOle, smiley[smiley_idx], smiley_idx);

		m_editExChat.HideSelection(TRUE, FALSE);
		*/
		//g_gameAppView->m_sendEdit.HideSelection(TRUE, FALSE);
	

		g_gameAppView->m_sendEdit.ReplaceSel(smiley_lst[smiley_idx]);

		/*for(int n=0;n<20;n++)
			{
			iTotalTextLength = g_gameAppView->m_sendEdit.GetWindowTextLength();
			g_gameAppView->m_sendEdit.SetSel(iTotalTextLength, iTotalTextLength);
			g_gameAppView->m_sendEdit.ReplaceSel(smiley_lst[rand()%32]);
			}
		*/
		
		// g_gameAppView->m_sayButton.SetBitmap(Skin_ButtonSayOn); // Set Say Button to On
		g_gameAppView->m_sendEdit.m_buttonstate = TRUE;
		//g_gameAppView->m_sendEdit.SetFocus(); // return the focus back to the sendEdit

		DestroyWindow();
		g_gameAppView->m_sendEdit.SetFocus(); // return the focus back to the sendEdit 
		iTotalTextLength = g_gameAppView->m_sendEdit.GetWindowTextLength();
		g_gameAppView->m_sendEdit.SetSel(iTotalTextLength, iTotalTextLength);
		
		// Replace any Smiley Codes by their Image //
		g_gameAppView->m_sendEdit.TextAdjust();

		}

	CDialog::OnLButtonDown(nFlags, point);
}

void CSmileySelectDlg::OnDestroy() 
{
	
	m_bOpen = false;

	CDialog::OnDestroy();
	
	// TODO: Add your message handler code here

	
}
