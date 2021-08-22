// ListCtrlEx.cpp : implementation file
//

#include "Build.h"
#include "stdafx.h"
#include "GameApp.h"
#include "GameAppView.h"
#include "ListCtrlEx.h"
#include "InvitationDlg.h"
#include "memdc.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


/////////////////////////////////////////////////////////////////////////////
// CListCtrlEx

// Ratings
// -------
// Red =	2100+
// Orange = 1800 - 2099
// Purple = 1500 - 1799
// Cyan =	1200 - 1499
// Green =	0 - 1199
// Grey =	Provisional
COLORREF ratingColor[7] = {
RGB(206,48,49),		// Red
RGB(255,162,99),	// Orange
RGB(156,101,206),	// Purple
RGB(99,154,206),	// Cyan
RGB(99,154,99),		// green
RGB(132,130,132),	// grey
RGB(177,129,0),		// Brown
};

COLORREF poolTableColor[6] = {
RGB(0,0,200),		// Blue
RGB(255,0,0),		// Red
RGB(0,128,0),		// green
RGB(128,0,128),		// Purple
RGB(128,128,128),	// grey
RGB(255,255,50),	// Yellow
};

CFont *fnt1;
CFont *fnt2;
CFont *PlayerListFont[2];
CDC g_dcMemoryBtn[8];

CDC g_temp;

//CDC dcMem_buf;


CListCtrlEx::CListCtrlEx()
{
		m_pParent = NULL;
		
		m_WatchIcon = AfxGetApp()->LoadIcon(IDI_WATCH);//IDR_MAINFRAME);
		m_PlayIcon = AfxGetApp()->LoadIcon(IDI_PLAY);//IDR_MAINFRAME);
		
		for(int n=0;n<MAX_TABLEITEMS;n++)
			m_splitText[n] = 0;
		
		m_columnWidth = new int[MAX_COLUMNS];

		m_bToggleName = 0;
		m_bToggleRating = 0;
		m_bToggleTable = 0;

		m_columnHeight = 0;
		m_totalButtons = 0;
		m_selected = -1;
		m_selectedold = -1;
		just_selected = 0;
		m_firsttime = 1;	// First tiem for DrawItem
		m_ebkfirst = 1;	// First time for Erase BackGround

		m_bBackground = FALSE;	// Assume all PlayerLists have no background by default //
		m_buttonLst = NULL;

}

CListCtrlEx::~CListCtrlEx()
{
	int n;
	
	delete m_columnWidth;

	for(n=0;n<MAX_TABLEITEMS;n++)
	if(m_splitText[n])
		delete m_splitText[n];

	if(m_totalButtons)
		DestroyButtons();

	
}

void CListCtrlEx::PassParent(CWnd *pParent)
{
	m_pParent = pParent;
}

BEGIN_MESSAGE_MAP(CListCtrlEx, CListCtrl)
	//{{AFX_MSG_MAP(CListCtrlEx)
	ON_NOTIFY_REFLECT(LVN_COLUMNCLICK, OnColumnclick)
	ON_WM_LBUTTONDBLCLK()
	ON_WM_VSCROLL()
	ON_WM_HSCROLL()
	ON_WM_SIZE()
	ON_WM_MEASUREITEM()
	ON_WM_LBUTTONDOWN()
	ON_WM_CREATE()
	ON_WM_ERASEBKGND()
	//ON_WM_PAINT()
	ON_WM_DESTROY()
	ON_WM_DRAWITEM()
	ON_WM_KEYDOWN()
	ON_WM_VKEYTOITEM()
	ON_WM_MOUSEMOVE()
	ON_WM_MOUSEWHEEL()
	ON_WM_RBUTTONUP()
	ON_WM_RBUTTONDOWN()
	//}}AFX_MSG_MAP
	
END_MESSAGE_MAP()

int CListCtrlEx::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CListCtrl::OnCreate(lpCreateStruct) == -1)
		return -1;
	/*
	CDC m_dcMemory;
	CDC m_dcbb;
	m_dcbb.::GetDC(lpCreateStruct->hwndParent)
	*/
	//m_dcbb.CreateCompatibleDC(GetDC());

	// TODO: Add your specialized creation code here
	m_maxItems = 0;
	m_totalColumns = 0;
	m_totalItems = 0;
	m_totalButtons = 0;
	m_bAllowColumnSizing = TRUE;
	m_columnHeight = 0;
	m_flags = 0;//LISTCTRLEX_GRIDVERT;
	m_crGridColor1st = RGB(255,0,0);
	m_crGridColor2nd = RGB(255,0,0);
	m_crTextColor = RGB(255,255,255);
	m_crTextBkColor = RGB(55,255,55);
	m_gridSplitOffsetStartX = 0;
	m_gridSplitOffsetEndX = 0;
	m_lastSelectedNItem = -1;
	m_lastSelectedName[0] = 0;
	m_iSubItem = -1;


	// Common ListCtrlEx Font //
	fnt1 = new CFont;
	fnt1->CreateFont (15, 6, 0, 0, FW_NORMAL, FALSE, 0, 0, DEFAULT_CHARSET, NULL,
											   NULL, DEFAULT_QUALITY, DEFAULT_PITCH |
											   FF_DONTCARE, "Arial");

	fnt2 = new CFont;
	fnt2->CreateFont (14, 6, 0, 0, FW_NORMAL, FALSE, 0, 0, DEFAULT_CHARSET, NULL,
											   NULL, DEFAULT_QUALITY, DEFAULT_PITCH |
											   FF_DONTCARE, "Arial");

	
	

	// Create FW_THIN + NORMAL FONT //
	PlayerListFont[0] = new CFont;
	PlayerListFont[0]->CreateFont (15, 0, 0, 0, FW_THIN, FALSE, 0, 0, DEFAULT_CHARSET, OUT_CHARACTER_PRECIS,
												   CLIP_CHARACTER_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH |
												   FF_DONTCARE, "Arial");

	
	// Create FW_THIN + ITALIC FONT //
	PlayerListFont[1] = new CFont;
	PlayerListFont[1]->CreateFont (15, 0, 0, 0, FW_THIN, TRUE, 0, 0, DEFAULT_CHARSET, OUT_CHARACTER_PRECIS,
												   CLIP_CHARACTER_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH |
												   FF_DONTCARE, "Arial");

	/*
	// Create FW_BOLD + NORMAL FONT //
	PlayerListFont[2] = new CFont;
	PlayerListFont[2]->CreateFont (15, 0, 0, 0, FW_THIN, FALSE, 0, 0, DEFAULT_CHARSET, OUT_CHARACTER_PRECIS,
												   CLIP_CHARACTER_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH |
												   FF_DONTCARE, "Arial");

	// Create FW_BOLD + ITALIC FONT //
	PlayerListFont[3] = new CFont;
	PlayerListFont[3]->CreateFont (15, 0, 0, 0, FW_THIN, TRUE, 0, 0, DEFAULT_CHARSET, OUT_CHARACTER_PRECIS,
												   CLIP_CHARACTER_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH |
												   FF_DONTCARE, "Arial");

	*/

	return 0;
}



void CListCtrlEx::OnDestroy() 
{
	CListCtrl::OnDestroy();
	
	// TODO: Add your message handler code here

}

void CListCtrlEx::SetMaxItems(int maxItems)
{
	m_maxItems = maxItems;
}

int CListCtrlEx::CreateColumn(int n, char *str, int cx)
{
	// setup columns
	LV_COLUMN lvcol;
	lvcol.mask = LVCF_FMT | LVCF_TEXT | LVCF_WIDTH | LVCF_SUBITEM;
	lvcol.fmt = /*LVCFMT_CENTER*/LVCFMT_LEFT;
	lvcol.cx = cx;
	lvcol.pszText = (LPSTR)(const char *)str;
	lvcol.iSubItem = n;

	int idx = InsertColumn(n, &lvcol);
	if(idx!=-1) m_totalColumns++;

	m_columnWidth[n] = cx;
	
	return idx;
}

int CListCtrlEx::CreateItem(int n, CString *str)
{
	LV_ITEM lvitem;
	lvitem.mask = LVIF_TEXT | LVIF_PARAM;
	lvitem.lParam = m_totalItems; // point to row index #0, to gain access to column #0
	lvitem.iItem = m_totalItems; // point to row #
	lvitem.pszText = (LPSTR)(const char *)str[0]; 
	lvitem.iSubItem = 0; // set this to zero for column #0 access

	if( GetItemCount() <= m_totalItems )
		InsertItem(&lvitem);
	else
		SetItem(&lvitem);
	
	// now add subitems
	for(int i=1;i<m_totalColumns;i++)
	{
		SetItemText(m_totalItems, i, (LPTSTR)(const char *)str[i]);
	}

	m_totalItems++;

	return 0;
}

int CListCtrlEx::CreateItemLabel(int n, CString *str)
{
	LV_ITEM lvitem;
	lvitem.mask = LVIF_TEXT | LVIF_PARAM;
	lvitem.lParam = m_totalItems; // point to row index #0, to gain access to column #0
	lvitem.iItem = m_totalItems; // point to row #
	lvitem.pszText = (LPSTR)(const char *)str[0]; 
	lvitem.iSubItem = 0; // set this to zero for column #0 access
	
	InsertItem(&lvitem); 

	m_totalItems++;

	return 0;
}

int CListCtrlEx::AddSubItemText(int columnN, int subItemN, CString *str)
{
	// add a subitem
	if(columnN < MAX_TABLEITEMS)
	{
		SetItemText(columnN, subItemN, (LPTSTR)(const char *)str[0]);
		return 1;
	} 

	return 0;
}

void CListCtrlEx::AddSplitText(int n, char *str)
{
	if(n>=0 && n < MAX_TABLEITEMS)
	{
		if(m_splitText[n])
			strcpy(m_splitText[n], str);
	}
}


//HBITMAP Skin_TL[4];
//CBitmapEx Skin_TL[8];
HBITMAP Skin_TL[8];

int CListCtrlEx::CreateButtons(int totalButtons, BOOL bSplit)
{
	float h = (float)m_columnHeight / (float)(1+bSplit);
	CRect rect;
	rect.SetRect(0,0,64,h);
	rect.OffsetRect(0,0);

#ifndef MAIN_SERVER
	// Load Bitmap Images //
	Skin_TL[0] = (HBITMAP)LoadImage(0, "Graphics/skin/tl_view.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
	Skin_TL[1] = (HBITMAP)LoadImage(0, "Graphics/skin/tl_watch.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
	Skin_TL[2] = (HBITMAP)LoadImage(0, "Graphics/skin/tl_joinp1.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
	Skin_TL[3] = (HBITMAP)LoadImage(0, "Graphics/skin/tl_joinp2.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);

	Skin_TL[4] = (HBITMAP)LoadImage(0, "Graphics/skin/tl_view_selected.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
	Skin_TL[5] = (HBITMAP)LoadImage(0, "Graphics/skin/tl_watchoff.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
	Skin_TL[6] = (HBITMAP)LoadImage(0, "Graphics/skin/tl_joinp1off.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
	Skin_TL[7] = (HBITMAP)LoadImage(0, "Graphics/skin/tl_joinp2off.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
#endif	
	//m_buttonLst = new CButtonListCtrlEx *[totalButtons];
	m_buttonLst = new CTableButton *[totalButtons];

	
	for(int n=0;n<totalButtons;n++)
	{
		m_buttonLst[n] = new CTableButton;
		//m_buttonLst[n]->Create("Empty!", BS_PUSHBUTTON /*| WS_VISIBLE*/ | WS_CHILD | WS_BORDER | WS_DISABLED, rect, this, IDC_LISTCTRLTABLESBUTTONS);
		
		
		
		if( (n%4) == 0) m_buttonLst[n]->Create(0, "Lobby TV");
		else if( (n%4) == 1) m_buttonLst[n]->Create(1, "Watch");
		else if( (n%4) == 2) m_buttonLst[n]->Create(2, "Join P1");
		else m_buttonLst[n]->Create(3, "Join P2");
		
		m_buttonLst[n]->m_listCtrlIdx = n;
	}
	
	m_totalButtons = totalButtons;

	return 0;
}

void CListCtrlEx::DestroyButtons(void)
{
	if(m_buttonLst)
	{
		for(int n=0;n<m_totalButtons;n++)
			delete m_buttonLst[n];

		delete m_buttonLst;
		m_buttonLst = NULL;
	}
}




int CListCtrlEx::DrawItemButton(CDC *m_pDC, int n, char *str, int nItem, int nSubItem, BOOL bSplit, BOOL bShow)
{

#ifndef MAIN_SERVER	
	// Load Bitmaps if first time //
	static char first_time = 1;
	int i;
	if(first_time)
		{
		for(i=0;i<8;i++)
			{
			g_dcMemoryBtn[i].CreateCompatibleDC(m_pDC);
			g_dcMemoryBtn[i].SelectObject(Skin_TL[i]);
			}
		first_time = 0;
		}
#endif


	if(m_buttonLst[n]->m_seated) bShow = FALSE;

	//if(nItem > (start+4)) return -1;

	if(m_buttonLst)
	if(m_buttonLst[n])
	{
		CRect ItemRect;
		GetItemRect(nItem, ItemRect, LVIR_BOUNDS);
		
		int AddWidths = 0;
		for(int m=0;m<nSubItem;m++)
			AddWidths += m_columnWidth[m];
		 
		ItemRect.left = AddWidths;
		ItemRect.right = AddWidths + m_columnWidth[nSubItem];
		float ht = m_columnHeight;
		if(bSplit)
		{
			ht = (float)m_columnHeight / 2.0f;
			ItemRect.bottom -= ht;
		}
			
		//if( !m_buttonLst[n]->IsWindowEnabled() ) m_buttonLst[n]->EnableWindow();

		CSize size;
		CPoint point;

		size = ItemRect.Size();
		point =  ItemRect.TopLeft();
		
		
		if(bShow) m_buttonLst[n]->Show();
			else m_buttonLst[n]->Hide();
		
		//m_buttonLst[n]->RedrawWindow(NULL, NULL, RDW_INVALIDATE | RDW_UPDATENOW | RDW_NOERASE | RDW_NOFRAME);
#ifndef MAIN_SERVER	
		// Draw the image to the window //
		// Draw a Lobby TV Button //
		//DrawItemButton(n*4+0, "View", n, 1, TRUE);//, no);
		int bmpidx = (1-m_buttonLst[n]->m_enabled)*4 + n%4;
		//if(bmpidx == 6) bmpidx = 1;	// Remember the Lobby TV Button is always Enabled //
		BITMAP bmpInfo;
		GetObject( g_dcMemoryBtn[bmpidx], sizeof( bmpInfo ), &bmpInfo );
	
		BITMAP bm;
		GetObject(Skin_TL[bmpidx], sizeof(BITMAP), (LPSTR)&bm);

		//m_pDC->BitBlt(ItemRect.left, ItemRect.top, Skin_TL[bmpidx].m_nWidth, Skin_TL[bmpidx].m_nHeight, &g_dcMemoryBtn[bmpidx], 0, 0, SRCCOPY);
		m_pDC->BitBlt(ItemRect.left, ItemRect.top, bm.bmWidth, bm.bmHeight, &g_dcMemoryBtn[bmpidx], 0, 0, SRCCOPY);

#else
		// If we are the server, then just display a grey rectange //
		CBrush brush = RGB(39,9,0);
		m_pDC->FillRect(CRect(ItemRect.left, ItemRect.top, ItemRect.left+ItemRect.Width(), ItemRect.top+ItemRect.Height()), &brush);
#endif		



		return 0;
	}


	return -1;
}

int CListCtrlEx::HideItemButton(int n)
{
	if(m_buttonLst)
	if(m_buttonLst[n])
	{
		//m_buttonLst[n]->SetWindowPos(&wndTop, 0,0,32,32, SWP_HIDEWINDOW);
		//m_buttonLst[n]->RedrawWindow(NULL, NULL, RDW_INVALIDATE | RDW_UPDATENOW | RDW_ERASE | RDW_FRAME);
		
		
		// no longer use the VISIBILITY of the button to determine whether we are seated. This is a bad method as 
		// it means we can't hide buttons out of the screen //

		m_buttonLst[n]->m_seated = 1;
		m_buttonLst[n]->m_enabled = 0;
		m_buttonLst[n]->Hide();

		return 0;
	}

	return -1;
}

int CListCtrlEx::ShowItemButton(int n, BOOL bState)
{
	if(m_buttonLst)
	if(m_buttonLst[n])
	{
		m_buttonLst[n]->Show();
		m_buttonLst[n]->m_seated = 0;
		if(bState) m_buttonLst[n]->m_enabled = 1;
			else m_buttonLst[n]->m_enabled = 0;
		return 0;
	}

	return -1;
}


void CListCtrlEx::EnableItemButton(int n, BOOL bState)
{
	
	if(m_buttonLst)
	if(m_buttonLst[n])
		{
		// diable buttons to sub-item fields (3 buttons per item)
		if(bState) m_buttonLst[n]->m_enabled = 1;
			else m_buttonLst[n]->m_enabled = 0;
		}
}

void CListCtrlEx::AllowColumnSizing(BOOL allowColumnSizing)
{
	m_bAllowColumnSizing = allowColumnSizing;
}


// special Table functions!!
void CListCtrlEx::CreateTableItem(int n, CString *str, char *options)
{
	// add split text to sub-item field
	if(n < MAX_TABLEITEMS)
	AddSplitText(m_totalItems, options);
		
	// add text to sub-item fields
	CreateItem(m_totalItems, str);

	/*
	if(n < MAX_TABLEITEMS)
	{
	// add buttons to sub-item fields (4 buttons per item)
	DrawItemButton(n*4+0, "View", n, 1, TRUE);
	DrawItemButton(n*4+1, "Watch", n, 2, TRUE);
	DrawItemButton(n*4+2, "Join", n, 3, TRUE);
	DrawItemButton(n*4+3, "Join", n, 4, TRUE);


	// enable the buttons
	EnableItemButton(n*4+0, TRUE);
	EnableItemButton(n*4+1, TRUE);
	EnableItemButton(n*4+2, TRUE);
	EnableItemButton(n*4+3, TRUE);
	}
	*/
}

void CListCtrlEx::DeleteTableItem(int n)
{

}

void CListCtrlEx::DeleteAllTableItems(void)
{

}


// FALSE = TABLE
// TRUE = PLAYERLIST
void PlayerNameText(CWnd *pWnd, CDC *pDC, int x, int y, int plrIdx, int cx, BOOL type, char selected)
{
BYTE state;
int xWeight;
BOOL bItalic;
int r,g,b;
COLORREF oldcolor;
state = m_player[plrIdx].state;

r = 255;
g = 255;
b = 255;

#ifndef THE_POOL_CLUB // iSnooker
	if(m_player[plrIdx].Max147s>0 || m_player[plrIdx].pMax147s>0) // 147 players are in gold.
	r = 255, g = 255, b = 125;
	else if(m_player[plrIdx]._100s>=1000 || m_player[plrIdx]._p100s>=200) // 100-Century players are in gold too.
	r = 255, g = 255, b = 125;
	else r = 255, g = 255, b = 255;

#else // iPool
	
	float Rate[3];
	Rate[0] = m_player[plrIdx]._100s;
	Rate[0] /= 1000;
	Rate[1] = m_player[plrIdx]._p90s;
	Rate[1] /= 1000;
	Rate[2] = m_player[plrIdx]._p100s;
	Rate[2] /= 1000;


	if(Rate[0] > g_GoldThreshold8Ball || Rate[1] > g_GoldThreshold9Ball || Rate[2] > g_GoldThresholdEng)  
		{
		r = 255;
		g = 255;
		b = 125;	// GOLD NAME When playing well //
		}
	/*
	if(m_player[plrIdx]._50s>=1000) r = 255, g = 255, b = 155;	// 1000+ 8-Ball Run-Outs STD for GOLD
	else if(m_player[plrIdx]._60s>=500) r = 255, g = 255, b = 155;	// 500+ 9-Ball Run-Outs STD for GOLD
	else if(m_player[plrIdx]._70s>=1000) r = 255, g = 255, b = 155;	// 1000+ UK-Ball Run-Outs STD for GOLD
	else if(m_player[plrIdx]._80s>=700) r = 255, g = 255, b = 155;	// 1000+ 8-Ball Run-Outs PRO for GOLD
	else if(m_player[plrIdx]._90s>=350) r = 255, g = 255, b = 155;	// 500+ 9-Ball Run-Outs PRO for GOLD
	else if(m_player[plrIdx]._p50s>=700) r = 255, g = 255, b = 155;	// 1000+ UK-Ball Run-Outs PRO for GOLD
	else if(m_player[plrIdx]._80s>=500) r = 255, g = 255, b = 155;	// 1000+ 8-Ball Run-Outs UK for GOLD
	else if(m_player[plrIdx]._90s>=250) r = 255, g = 255, b = 155;	// 500+ 9-Ball Run-Outs UK for GOLD
	else if(m_player[plrIdx]._p50s>=500) r = 255, g = 255, b = 155;	// 1000+ UK-Ball Run-Outs UK for GOLD
	else r = 255, g = 255, b = 255;
	*/
#endif



if((state&PLRSTATE_AUTHMASK)) 
	{
	//r = 255;
	//g = 255;
	//b = 255;
	xWeight = FW_BOLD; // Administrators/Moderators have bold names
	}
else{
	//r = 230;
	//g = 230;
	//b = 230;
	xWeight = FW_THIN;
	}
if((state&PLRSTATE_STATUSMASK) == PLRSTATE_AVAILABLE) 
	{
	bItalic = FALSE;
	}
else{
	r*=0.6f, g*=0.6f, b*=0.6f, bItalic = TRUE;
	}


// STEALTH ////////////////////////////////
if(m_player[plrIdx].state2&PLRSTATE2_STEALTH) // make admin/moderator appear normal.
	{
	//r = 230;
	//g = 230;
	//b = 230;
	xWeight = FW_THIN;
	}

// Ban or Selected Colors ////////////////////////////////
int BansCheating = ((m_player[plrIdx].bandata)&0xF0)>>4;			// Bits 4-7		= BansCheating //

if(selected) 
	{
	if(BansCheating) oldcolor = pDC->SetTextColor(RGB(255,180,0));
		else oldcolor = pDC->SetTextColor(RGB(255,255,0));
	}
else{
	if(BansCheating) oldcolor = pDC->SetTextColor(RGB(255,60,0));
		else oldcolor = pDC->SetTextColor(RGB(r,g,b));
	}




CFont *oldFont;
if(!type) 
	{
	if(bItalic == FALSE) oldFont = pDC->SelectObject(PlayerListFont[0]);
		else oldFont = pDC->SelectObject(PlayerListFont[1]);
	}
else{
	if(bItalic == FALSE) oldFont = pDC->SelectObject(PlayerListFont[0]);
		else oldFont = pDC->SelectObject(PlayerListFont[1]);
	}


char name[22];


int tblIdx = m_player[plrIdx].table;

if(type == TRUE && tblIdx != -1)	// Table List //
	{
	// If this is a Doubles Table, then we should display the Team Name "Player1 / Player1b" OR "Player2 / Player2b"//
	if(m_table[tblIdx].doubles)
		{
		char namea[22];
		char nameb[22];
		int pidx = -1;


		// If we are Player1 //
		if(m_player[plrIdx].playerSocketIdx == m_table[tblIdx].player1SockIdx)
			{
			// P1 //
			strcpy(namea, m_player[plrIdx].name);
			// P1b //
			pidx = ((CGameAppView *)g_wnd)->m_lobby->GetPlayerIdx(m_table[tblIdx].player1bSockIdx);
			if(pidx != -1) strcpy(nameb, m_player[pidx].name);
				else nameb[0] = 0;
			}
		// If we are Player1b //
		else if(m_player[plrIdx].playerSocketIdx == m_table[tblIdx].player1bSockIdx)
			{
			// P1b //
			strcpy(namea, m_player[plrIdx].name);
			// P1 //
			pidx = ((CGameAppView *)g_wnd)->m_lobby->GetPlayerIdx(m_table[tblIdx].player1SockIdx);
			if(pidx != -1) strcpy(nameb, m_player[pidx].name);
				else nameb[0] = 0;
			}
		// If we are Player2 //
		if(m_player[plrIdx].playerSocketIdx == m_table[tblIdx].player2SockIdx)
			{
			// P2 //
			strcpy(namea, m_player[plrIdx].name);
			// P2b //
			pidx = ((CGameAppView *)g_wnd)->m_lobby->GetPlayerIdx(m_table[tblIdx].player2bSockIdx);
			if(pidx != -1) strcpy(nameb, m_player[pidx].name);
				else nameb[0] = 0;
			}
		// If we are Player2b //
		else if(m_player[plrIdx].playerSocketIdx == m_table[tblIdx].player2bSockIdx)
			{
			// P2b //
			strcpy(namea, m_player[plrIdx].name);
			// P2 //
			pidx = ((CGameAppView *)g_wnd)->m_lobby->GetPlayerIdx(m_table[tblIdx].player2SockIdx);
			if(pidx != -1) strcpy(nameb, m_player[pidx].name);
				else nameb[0] = 0;
			}

		namea[4] = 0;
		nameb[4] = 0;
		
		sprintf(name, "%s/%s", namea, nameb);
		}
	// For Tabel List (singles) we just have one name from plrIdx
	else sprintf(name, m_player[plrIdx].name);
	}
// For Player List, we just have one name, which is from plrIdx //
else sprintf(name, m_player[plrIdx].name);



if(xWeight == FW_THIN) TextOutEllipsis(pDC, x, y-1, name, cx);
else{
	TextOutEllipsis(pDC, x, y-1, name, cx);
	TextOutEllipsis(pDC, x+1, y-1, name, cx);
	}

pDC->SelectObject(oldFont);
pDC->SetTextColor(oldcolor);
}



void PlayerRatingText(CWnd *pWnd, CDC *pDC, int x, int y, int plrIdx, int cx, BOOL type, char selected, char *rating_str)
{
BYTE state;
int xWeight;
BOOL bItalic;
int r,g,b;
COLORREF oldcolor;
state = m_player[plrIdx].state;

r = 255;
g = 255;
b = 255;

#ifndef THE_POOL_CLUB // iSnooker
	if(m_player[plrIdx].Max147s>0 || m_player[plrIdx].pMax147s>0) // 147 players are in gold.
	r = 255, g = 255, b = 125;
	else if(m_player[plrIdx]._100s>=1000 || m_player[plrIdx]._p100s>=200) // 100-Century players are in gold too.
	r = 255, g = 255, b = 125;
	else r = 255, g = 255, b = 255;
#else
	
	float Rate[3];
	Rate[0] = m_player[plrIdx]._100s;
	Rate[0] /= 1000;
	Rate[1] = m_player[plrIdx]._p90s;
	Rate[1] /= 1000;
	Rate[2] = m_player[plrIdx]._p100s;
	Rate[2] /= 1000;


	if(Rate[0] > g_GoldThreshold8Ball || Rate[1] > g_GoldThreshold9Ball || Rate[2] > g_GoldThresholdEng)  
		{
		r = 255;
		g = 255;
		b = 125;	// GOLD NAME When playing well //
		}


#endif


if((state&PLRSTATE_AUTHMASK)) 
	{
	//r = 255;
	//g = 140;	//
	//b = 26;
	xWeight = FW_BOLD; // Administrators/Moderators have bold names
	}
else{
	//r = 230;
	//g = 230;
	//b = 230;
	xWeight = FW_THIN;
	}
if((state&PLRSTATE_STATUSMASK) == PLRSTATE_AVAILABLE) 
	{
	bItalic = FALSE;
	}
else{
	r*=0.6f, g*=0.6f, b*=0.6f, bItalic = TRUE;
	}


// STEALTH ////////////////////////////////
if(m_player[plrIdx].state2&PLRSTATE2_STEALTH) // make admin/moderator appear normal.
	{
	//r = 230;
	//g = 230;
	//b = 230;
	xWeight = FW_THIN;
	}

// Ban or Selected Colors ////////////////////////////////
int BansCheating = ((m_player[plrIdx].bandata)&0xF0)>>4;			// Bits 4-7		= BansCheating //

if(selected) 
	{
	if(BansCheating) oldcolor = pDC->SetTextColor(RGB(255,180,0));
		else oldcolor = pDC->SetTextColor(RGB(255,255,0));
	}
else{
	if(BansCheating) oldcolor = pDC->SetTextColor(RGB(255,60,0));
		else oldcolor = pDC->SetTextColor(RGB(r,g,b));
	}




CFont *oldFont;
if(!type) 
	{
	if(bItalic == FALSE) oldFont = pDC->SelectObject(PlayerListFont[0]);
		else oldFont = pDC->SelectObject(PlayerListFont[1]);
	}
else{
	if(bItalic == FALSE) oldFont = pDC->SelectObject(PlayerListFont[0]);
		else oldFont = pDC->SelectObject(PlayerListFont[1]);
	}



if(xWeight == FW_THIN) TextOutEllipsis(pDC, x, y, rating_str, cx);
else{
	TextOutEllipsis(pDC, x, y, rating_str, cx);
	TextOutEllipsis(pDC, x+1, y, rating_str, cx);
	}


pDC->SelectObject(oldFont);
pDC->SetTextColor(oldcolor);
}


void PlayerLevelProgressBar(CDC *pDC, int x, int y, int width, int height, int plrIdx)
{
	float level_progress;
	float level;
	
	BYTE state;
	COLORREF xColor;
	char *xStr;
	BOOL guest = FALSE;
	BOOL trial = FALSE;
	
	level_progress = m_player[plrIdx].level_progress;
	level = m_player[plrIdx].level;

	int level_progress_max = (int)(pow(level, 1.5f) * 100);

	
	state = m_player[plrIdx].state;
	

	if(m_player[plrIdx].bIsGuest) guest = TRUE;

	if(m_player[plrIdx].state&PLRSTATE_GUESTMEMBER) trial = TRUE;

	int progress_width = width-4-15;
	
	// Draw Level Progress Background - Gray Colour	
	CPen pen(PS_NULL, 0, RGB(100,100,100));
	CBrush brush(RGB(68,30,12));
					
	CPen *oldPen = pDC->SelectObject(&pen);
	CBrush *oldBrush = pDC->SelectObject(&brush);

	pDC->Rectangle(x+15, y+14, x+15+progress_width,y+16);
	

	// Draw Level Progress Bar over Background (Blue)
	CBrush brush2(RGB(39,139,241));
	pDC->SelectObject(&brush2);

	// Get length of bar to draw - [(level_progress_total - level_progress) / level_progress_total] * progress_width
	int len = (int)((level_progress / level_progress_max) * progress_width);
	pDC->Rectangle(x+15, y+14, x+15+len, y+16);


	

	pDC->SelectObject(oldPen);
	pDC->SelectObject(oldBrush);
	
	/*
	COLORREF oldcolor = pDC->SetTextColor(RGB(20,20,20));
	if(!type) pDC->TextOut(x+1+2+textoffsetx,y+2-2, xStr);
	else pDC->TextOut(x+1+2+textoffsetx,y+2-2+1, xStr);
	pDC->SetTextColor(oldcolor);
	*/
	
}

void PlayerIcon(CDC *pDC, int x, int y, int plrIdx, BOOL type)
{// [+] (state&PLRSTATE_STATUSMASK) gotta integrate that with icons like pizza = lunch, phone = phone, away = door etc.
	int ratingIdx;
	int rating;
	BYTE state;
	COLORREF xColor;
	char *xStr;
	BOOL guest = FALSE;
	BOOL trial = FALSE;
	rating = m_player[plrIdx].rating;
	state = m_player[plrIdx].state;
	

	if(m_player[plrIdx].bIsGuest) guest = TRUE;

	if(m_player[plrIdx].state&PLRSTATE_GUESTMEMBER) trial = TRUE;

	// Red =	2100+
	// Orange = 1800 - 2099
	// Purple = 1500 - 1799
	// Cyan =	1200 - 1499
	// Green =	0 - 1199
	// Grey =	Provisional
	
	// Calculate rating colour
	//if(rating != -1 && rating != 0) // Guest => (rating = 0) 
	int textoffsetx = 0;
	if(!guest)
	{
		extern COLORREF ballColor[];
		if(rating < 1200) ratingIdx = 4; // Green
		else if(rating < 1500) ratingIdx = 3; // Cyan
		else if(rating < 1800) ratingIdx = 2; // Purple
		else if(rating < 2100) ratingIdx = 1; // Orange
		else if(rating < 2400) ratingIdx = 0; // Red
		else ratingIdx = 6; // Yellow

		pDC->SelectObject(fnt2);

		if((state&PLRSTATE_AUTHMASK)==PLRSTATE_ADMIN)
		{
		xColor = RGB(255,255,255);
		xStr = "A";
		textoffsetx = 1;
		} else if((state&PLRSTATE_AUTHMASK)==PLRSTATE_MODERATOR)
		{
		xColor = ratingColor[ratingIdx];
		xStr = "M";
		} 
		else if((state&PLRSTATE_AUTHMASK)==PLRSTATE_LEADMODERATOR)
		{
		xColor = RGB(255,255,255);
		xStr = "S";
		} 
		else {
		BOOL bShowTrialMembers = FALSE;
		xColor = ratingColor[ratingIdx];
		int myPlrIdx = ((CGameAppView *)g_wnd)->m_lobby->GetPlayerIdx( ((CGameAppView *)g_wnd)->m_playerSockIdx );
		
		if( m_player[myPlrIdx].state&PLRSTATE_MODERATOR || m_player[myPlrIdx].state&PLRSTATE_ADMIN )
			bShowTrialMembers = TRUE;
		if(g_serverConfig.flags&SERVERSENDCONFIG_FLAGS_SEERT) bShowTrialMembers = TRUE; // everyone can see them.

		if(bShowTrialMembers && trial) 
			{
			xStr = "T";
			textoffsetx = 1;
			}
		else{
			xStr = "R";
			textoffsetx = 0;
			}
		
		}

		// STEALTH ////////////////////////////////
		if(m_player[plrIdx].state2&PLRSTATE2_STEALTH) // make admin/moderator appear normal.
		{
			BOOL bShowTrialMembers = FALSE;
			xColor = ratingColor[ratingIdx];
			int myPlrIdx = ((CGameAppView *)g_wnd)->m_lobby->GetPlayerIdx( ((CGameAppView *)g_wnd)->m_playerSockIdx );
			if( m_player[myPlrIdx].state&PLRSTATE_MODERATOR || m_player[myPlrIdx].state&PLRSTATE_ADMIN )
				bShowTrialMembers = TRUE;
			if(g_serverConfig.flags&SERVERSENDCONFIG_FLAGS_SEERT) bShowTrialMembers = TRUE; // everyone can see them.

			if(bShowTrialMembers && trial) 
			{
				textoffsetx = 1;		
				xStr = "T";
			}
			else 
			{
				textoffsetx = 0;
				xStr = "R";
			}

		
		}
		// ~STEALTH ////////////////////////////////
		
		CPen pen(PS_NULL, 0, RGB(100,100,100));
		CBrush brush(xColor);
				
		CPen *oldPen = pDC->SelectObject(&pen);
		CBrush *oldBrush = pDC->SelectObject(&brush);
		if(!type) pDC->Rectangle(x+2,y+2,x+1+11,y+2+12);
		else pDC->Rectangle(x+1,y+2,x+14,y+16);
		
		pDC->SelectObject(oldPen);
		pDC->SelectObject(oldBrush);
								
		COLORREF oldcolor = pDC->SetTextColor(RGB(20,20,20));
		if(!type) pDC->TextOut(x+2+textoffsetx,y+2-2, xStr);
		else pDC->TextOut(x+2+textoffsetx,y+2-2+1, xStr);
		pDC->SetTextColor(oldcolor);

		
	}
}

/////////////////////////////////////////////////////////////////////////////
// CListCtrlEx message handlers

CDC dcMemory2;

void CListCtrlEx::DrawItem( LPDRAWITEMSTRUCT lpDrawItemStruct )
{


	// TODO: Add your message handler code here and/or call default
	CHeaderCtrl * pHdr = (CHeaderCtrl *)GetDlgItem(0);
	if( pHdr == NULL )
	{
		ASSERT(0);
		return;
	}


	

	BOOL selected = 0;
	BOOL update = 0;
  
	m_pDC = CDC::FromHandle(lpDrawItemStruct->hDC);
	if(m_firsttime == 1)
	{
		if(m_dcMemory == NULL) 
		{
			m_dcMemory.CreateCompatibleDC(m_pDC);
			CBitmap tmp;
			tmp.CreateCompatibleBitmap(m_pDC,640, 800);
			m_dcMemory.SelectObject(&tmp);
		}
		if(m_dcbb == NULL) 
		{
			m_dcbb.CreateCompatibleDC(m_pDC);
			CBitmap tmp;
			tmp.CreateCompatibleBitmap(m_pDC,640, 800);
			m_dcbb.SelectObject(&tmp);
		}

		m_firsttime = 0;
	}

	
	int n = lpDrawItemStruct->itemID;
	/*
	if(m_flags&TABLE)
	if(n == 49)
		{
		CBrush brush = RGB(59,19,0);
		CRect winRect;
		GetClientRect(winRect);
		m_pDC->FillRect(CRect(winRect.left, winRect.bottom-14, winRect.right, winRect.bottom), &brush);			
		}
	*/

	m_dcbb.SelectObject(fnt1);
	m_pDC->SelectObject(fnt1);




/*
	if(!(m_flags&TABLE))
	if(just_selected) 
		{
		just_selected = 0;
		//OnEraseBkgnd(m_pDC);
		RedrawItems(0, m_totalItems);
		}
*/


	if(m_flags&TEXTCOLOR) // use my own colours
	{
		m_dcbb.SetTextColor(m_crTextColor);
		m_dcbb.SetBkColor(m_crTextBkColor);

		m_pDC->SetTextColor(m_crTextColor);
		m_pDC->SetBkColor(m_crTextBkColor);
		//m_pDC->SetBkMode(TRANSPARENT);
		
	}
	
	if( lpDrawItemStruct->itemAction & (ODA_DRAWENTIRE | ODA_SELECT) )
	{ 	
		COLORREF crOldText;     // never permanently alter the original CDC attributes
		COLORREF crOldBack;   // never permanently alter the original CDC attributes

		// If the item is selected then highlight the item.	
		if( !(m_flags&HIGHLIGHTROWDISABLED) )
		if( lpDrawItemStruct->itemState & ODS_SELECTED )
		{
			int ItemIdx = lpDrawItemStruct->itemData;//HitTest(point);
			m_selectedold = m_selected;
			m_selectedoldrect.left = m_selectedrect.left;
			m_selectedoldrect.top = m_selectedrect.top;
			m_selectedoldrect.right = m_selectedrect.right;
			m_selectedoldrect.bottom = m_selectedrect.bottom;
			
			CRect trech = lpDrawItemStruct->rcItem;
			m_selectedrect.left = trech.left;
			m_selectedrect.top = trech.top;
			m_selectedrect.right = trech.right;
			m_selectedrect.bottom = trech.bottom;

			m_selected = ItemIdx;
			

			/*if(m_flags&TEXTCOLOR) // use my own colours
			{
				crOldText = m_pDC->SetTextColor(m_crTextColor);
				crOldBack = m_pDC->SetBkColor(m_crTextBkColor);
			}
			else // use default system colors*/
			{
				//crOldText = m_pDC->SetTextColor(::GetSysColor(COLOR_HIGHLIGHTTEXT) );
				//crOldBack = m_pDC->SetBkColor(::GetSysColor(COLOR_HIGHLIGHT) );
				crOldText = m_pDC->SetTextColor(RGB(255,255,255));
				crOldBack = m_pDC->SetBkColor(RGB(0,95,206));
				selected = 1;
				
			}

			// this is the trick! we always know the row number of the selection (the one with highlight)
			// remember this row for later and update box text
			// this code is kinda redundant (dw *is* lpDrawItemStruct->itemData)
			DWORD dw = GetItemData(lpDrawItemStruct->itemData); 

			//////////////////////////////////////////
			// set last selected item name/idx
			if(!(m_flags&TABLE))
			{
				
				if(ItemIdx!=-1)
				{
					selected = 1;
					char buffer[80];
					CString name = GetItemText(ItemIdx,0);
					//m_lastSelectedNItem = ItemIdx;
					strcpy(m_lastSelectedName, name.GetBuffer(0));
				}
				else
				{
					//m_lastSelectedNItem = -1;
					m_lastSelectedName[0] = 0;
				}				
			}
		
		//////////////////////////////////////////
		CBrush brush1 = RGB(20,105,250);
		if(m_flags&LOBBYSELECT) 	
		//if(!(m_flags&TABLE))
			{
	   			// Erase the entire area. Using ExtTextOut is a neat alternative to FillRect and quicker, too!
				//m_pDC->FillRect(&lpDrawItemStruct->rcItem, &brush1);
				

				// ReDraw BackGround for this ROW, and Replace old Selected Item //
				// For LobbySelect, BackGround is just color RGB(59,19,0);
				CBrush brush = RGB(159,79,50);
				CRect r = lpDrawItemStruct->rcItem;
				CSize s = r.Size();
				m_dcbb.FillRect(&lpDrawItemStruct->rcItem, &brush);
				//update = 1;
				//m_pDC->FillRect(&lpDrawItemStruct->rcItem, &brush1);

			}
	
		if((m_flags&TABLE) && lpDrawItemStruct->itemID == MAX_TABLEITEMS-1) //if(m_maxItems-1)
			{
				CRect r = lpDrawItemStruct->rcItem;
				CSize s = r.Size();
				r.top+=s.cy;
				r.bottom = r.top + s.cy;

				//m_pDC->ExtTextOut(r.left, r.top, ETO_CLIPPED, &r, "", 0, NULL);
			}
		
		}
		
	
		//CRect windRect;
		//GetWindowRect(windRect);
		//CRect rect = lpDrawItemStruct->rcItem;
		//rect.OffsetRect(windRect.left,windRect.top);
		//((CGameAppView *)m_pParent)->m_bitmapLobby.Draw(m_pDC,lpDrawItemStruct->rcItem.left,
		//													lpDrawItemStruct->rcItem.top,
		//													rect);//&(lpDrawItemStruct->rcItem));

				
		// do some setup
		CString s;
		int x, y;
		x = lpDrawItemStruct->rcItem.left;
		y =lpDrawItemStruct->rcItem.top;

		HD_ITEM hditem;
		hditem.mask = HDI_WIDTH;
		
		CPen pen1(PS_SOLID, 2, m_crGridColor1st);
		CPen pen2(PS_SOLID, 1, m_crGridColor2nd);

		CBrush brush2(RGB(0,95,236));

		//if(m_flags&GRID1GAP | m_flags&GRID2GAP)
		//{
		//	m_pDC->SelectObject(pen1);
		//}
		//else if(m_flags&GRIDSPLIT)
		//{
			//if(lpDrawItemStruct->itemID&1) m_pDC->SelectObject(pen1);
			//else m_pDC->SelectObject(pen2);
			
			

			

			// draw horizontal lines
			//m_pDC->MoveTo( lpDrawItemStruct->rcItem.left, lpDrawItemStruct->rcItem.bottom-1 ); 
			//m_pDC->LineTo( lpDrawItemStruct->rcItem.right-2, lpDrawItemStruct->rcItem.bottom -1);

		//}
		
		
		if(m_flags&GRIDHORZ)
		if( !(m_flags&GRID2GAP) || (m_flags&GRID2GAP && lpDrawItemStruct->itemID&1) )
		{
			// draw horizontal lines
			
			

			if(m_flags&TABLE)
				{

				CPen *oldPen1 = m_pDC->SelectObject(&pen1);
				m_pDC->MoveTo( lpDrawItemStruct->rcItem.left, lpDrawItemStruct->rcItem.bottom-1); 
				m_pDC->LineTo( lpDrawItemStruct->rcItem.right-2, lpDrawItemStruct->rcItem.bottom-1);
				m_pDC->SelectObject(oldPen1);
				}
			else{
				CPen *oldPen1 = m_dcbb.SelectObject(&pen1);
				m_dcbb.MoveTo( lpDrawItemStruct->rcItem.left, lpDrawItemStruct->rcItem.bottom-1); 
				m_dcbb.LineTo( lpDrawItemStruct->rcItem.right-2, lpDrawItemStruct->rcItem.bottom-1);
				m_dcbb.SelectObject(oldPen1);
				}

			// draw horizontal split lines
			if(m_flags&GRIDSPLIT)
			{
				float cy;
				cy = (lpDrawItemStruct->rcItem.bottom -1) - (lpDrawItemStruct->rcItem.top+1);
				cy /= 2.0f;

				// blank region
				{
					CRect rect;
					rect.left = lpDrawItemStruct->rcItem.left + m_gridSplitOffsetStartX;
					rect.top = lpDrawItemStruct->rcItem.bottom-1 - cy;
					rect.right = lpDrawItemStruct->rcItem.right-2 - m_gridSplitOffsetEndX;
					rect.bottom = lpDrawItemStruct->rcItem.bottom-1;
					if(m_flags&TABLE) m_pDC->ExtTextOut(rect.left, rect.top, ETO_OPAQUE, &rect,"", 0, NULL );
						else m_dcbb.ExtTextOut(rect.left, rect.top, ETO_OPAQUE, &rect,"", 0, NULL );
				}
								
				CPen *oldPen2 = m_dcbb.SelectObject(&pen2);
				
				if(m_flags&TABLE)
					{
					/*
					m_pDC->MoveTo( lpDrawItemStruct->rcItem.left + m_gridSplitOffsetStartX,
								 lpDrawItemStruct->rcItem.bottom-1 - cy); 
					m_pDC->LineTo( lpDrawItemStruct->rcItem.right-2 - m_gridSplitOffsetEndX,
								 lpDrawItemStruct->rcItem.bottom-1 - cy);
					m_pDC->SelectObject(oldPen2);
					*/
					}
				else{
					m_dcbb.MoveTo( lpDrawItemStruct->rcItem.left + m_gridSplitOffsetStartX,
								 lpDrawItemStruct->rcItem.bottom-1 - cy); 
					m_dcbb.LineTo( lpDrawItemStruct->rcItem.right-2 - m_gridSplitOffsetEndX,
								 lpDrawItemStruct->rcItem.bottom-1 - cy);
					m_dcbb.SelectObject(oldPen2);
					}
			}
		}

		// draw vertical lines
		BOOL here = FALSE;
		for( int i = 0; i < m_totalColumns; i++)
		{
			// draw a table icon
			if((m_flags&TABLE) && !i)
			{
				// blank region
				{
					pHdr->GetItem( i, &hditem);
					int offsetX = hditem.cxy;
					CRect rect = lpDrawItemStruct->rcItem;
					rect.right = rect.left + offsetX;
					rect.bottom-=1;
					m_pDC->ExtTextOut(rect.left, rect.top, ETO_OPAQUE, &rect,"", 0, NULL );
				}

				//#ifdef MAIN_SERVER
				//if(g_build.gameType == GAMETYPE_SNOOKER)
				//{
				//	if(m_table[lpDrawItemStruct->itemID].status)
				//		m_pDC->DrawIcon(x+0+4,y+0-1, m_TableIcon);
				//	else m_pDC->DrawIcon(x+0+4,y+0-1, m_TableDimIcon);
				//}
				//#endif //MAIN_SERVER
				
				#ifndef MAIN_SERVER
				if(g_build.gameType == GAMETYPE_SNOOKER)
				{
					// *NEW:
					BOOL bTourneyTableUsed = FALSE;	
					if(m_table[lpDrawItemStruct->itemID].tournament >= 2)
						bTourneyTableUsed = TRUE;

					int index = 0;
					if(!m_table[lpDrawItemStruct->itemID].status && !bTourneyTableUsed) index = 0; // table OFF
					else
					{
						BOOL wild = FALSE;
						int gameType = m_table[lpDrawItemStruct->itemID].gameType;
						//int tblColour = m_table[lpDrawItemStruct->itemID].tableColour;

						// check if a WILD table setting is set
						wild = ((CGameAppView *)m_pParent)->m_lobby->IsTableWILD(lpDrawItemStruct->itemID);
						
						if(wild) index = 8; // Wild Table //
							else if(gameType ==	0) index = 1;	// 15 Reds
							else if(gameType ==	1) index = 2;	// 10 Reds
							else if(gameType ==	2) index = 3;	// 6 Reds
							else if(gameType ==	3) index = 4;	// ReSpot Black
							else if(gameType ==	4) index = 5;	// Billiards 50 pts
							else if(gameType ==	5) index = 6;	// Billiards 100 pts
							else if(gameType ==	6) index = 7;	// Billiards 200 pts
							else index = 1;	// Default to 15 Reds if we can't find a match //
					}
										
					if(index >= 0 && index <= 8)
					((CGameAppView *)m_pParent)->DrawBitmap(m_pDC, x+2, y+2, ((CGameAppView *)m_pParent)->m_bitmapTable2[index]);
					//((CGameAppView *)m_pParent)->m_bitmapTable2[index].Draw(m_pDC, x+2, y+2);
				}
				else if(g_build.gameType == GAMETYPE_POOL)
				{		
					BOOL wild = FALSE;
					int index = 0;
					int tableType = m_table[lpDrawItemStruct->itemID].tableType;
					int gameType = m_table[lpDrawItemStruct->itemID].gameType;
					int tblColour = m_table[lpDrawItemStruct->itemID].tableColour;

					// for extra table colours, overrides
					if(tblColour == 9) tblColour = 0; // Blue & Black
					if(tblColour == 10) tblColour = 1; // Red & Black
					if(tblColour == 11) tblColour = 5; // Black & Black
											
					// check if a WILD table setting is set
					wild = ((CGameAppView *)m_pParent)->m_lobby->IsTableWILD(lpDrawItemStruct->itemID);
					
					// If we have selected an English Pool Table, then force the table Color to Green //
					if(tableType == 2) tblColour = 2;

					if(!wild) index = 9*gameType + tblColour; // normal table types
					else index = 9*8 + tblColour; // 72 + table colours // WILD table types
					
					
					if(index >= 0 && index <= 80)
					((CGameAppView *)m_pParent)->DrawBitmap(m_pDC, x+2, y+1, ((CGameAppView *)m_pParent)->m_bitmapTable[index]);
					//((CGameAppView *)m_pParent)->m_bitmapTable[index].Draw(m_pDC, x+2, y+1);
				}
				#endif //!MAIN_SERVER
			}
			
			s = GetItemText(lpDrawItemStruct->itemData, i);
			// paint text
 			//COLORREF oldTextColor = m_pDC->SetTextColor(m_pDC->GetTextColor());

			if(m_flags&TABLE) // table list
			{
				// blank region - buttons
				int n = lpDrawItemStruct->itemID;
				if(i==3 || i==4)
				if(m_buttonLst)
				if(m_buttonLst[n*4+(i-1)]->m_seated)
				{
					pHdr->GetItem( i, &hditem);
					int offsetX = hditem.cxy;
					CRect rect = lpDrawItemStruct->rcItem;
					CSize size = rect.Size();
					rect.left += x;
					rect.right = rect.left + offsetX;
					rect.bottom = rect.top + size.cy*0.5f;
					m_pDC->ExtTextOut(rect.left, rect.top, ETO_OPAQUE, &rect,"", 0, NULL );
				}

				// blank region
				if(i==4)
				{
					pHdr->GetItem( i, &hditem);
					int offsetX = hditem.cxy;
					CRect rect = lpDrawItemStruct->rcItem;
					CSize size = rect.Size();
					rect.left += (x+offsetX);
					rect.bottom = rect.top + size.cy*0.5f;
					m_pDC->ExtTextOut(rect.left, rect.top, ETO_OPAQUE, &rect,"", 0, NULL );
				}

				if(!i) m_pDC->TextOut(  x+2+40 - ((float)s.GetLength() / 2.0f), y+6, s, s.GetLength());
				else
				{
					if(m_pParent && i<5) // Player 1 = 2,Player 2 = 3
					{
					if(s.GetLength())
					{
						// get rating
						int plrIdx = ((CGameAppView *)m_pParent)->m_lobby->FindPlayerIdx(s);										
						if(plrIdx!=-1)
						{
							PlayerIcon(m_pDC, x, y, plrIdx, TRUE);
						}
					}
	
					// draw text
					//m_pDC->TextOut(  x+3 + 12, y, s, s.GetLength()  );

					//pHdr->GetItem( i, &hditem);
					//TextOutEllipsis(m_pDC, x+3 + 12, y, s, hditem.cxy - (3 + 12));

					int plrIdx = ((CGameAppView *)m_pParent)->m_lobby->FindPlayerIdx(s);			

					pHdr->GetItem( i, &hditem);
					

					if(plrIdx!=-1)
						PlayerNameText(this, m_pDC, x+3 + 12 + 1, y, plrIdx, hditem.cxy - (3 + 12), TRUE, 0);
					}
					else // Column (i = 4)
					{
						// draw text
						//m_pDC->TextOut(  x + 12, y, s, s.GetLength()  );
						pHdr->GetItem( i, &hditem);
						TextOutEllipsis(m_pDC, x + 12, y, s, hditem.cxy - 12);
					}
				}
			} else if(m_flags&LOBBYSELECT)
			{
				if(i==2) // Players #
				{
				if(((CGameAppView *)m_pParent)->m_gatewaylst)
					{
					//lpDrawItemStruct->itemData
					int idx = lpDrawItemStruct->itemID;
					char buffer[80];
					if(((CGameAppView *)m_pParent)->m_gatewaylst->GetLobby(idx))
						{
						if(((CGameAppView *)m_pParent)->m_gatewaylst->m_players[idx]==-1)
						sprintf(buffer,"?");
						else sprintf(buffer,"(%d/127)",((CGameAppView *)m_pParent)->m_gatewaylst->m_players[idx]);
						//else sprintf(buffer,"(%d/%d)",((CGameAppView *)m_pParent)->m_gatewaylst->m_players[idx], MAX_LOBBY_PLAYERS-10);//MAX_NORMAL_PLAYERS_ONLINE
						UINT oldAlign = m_dcbb.SetTextAlign(TA_CENTER);
						m_dcbb.TextOut(  x+30, y, buffer);
						m_dcbb.SetTextAlign(oldAlign);
						}
					}
				}
				else {
				CString str = GetItemText(lpDrawItemStruct->itemData, i);
				//m_pDC->TextOut(  x, y, str);
				pHdr->GetItem(i, &hditem);
				TextOutEllipsis(&m_dcbb, x, y, str, hditem.cxy);
				}
			}
			else // player list
			{
			
				// table field
				CString plrName = GetItemText(lpDrawItemStruct->itemData, 0);
				int plrIdx = ((CGameAppView *)m_pParent)->m_lobby->FindPlayerIdx(plrName);

				
				// draw ranking box!
				//if(0)
				if(!i)
				{
					// get rating
					if(plrIdx!=-1)
					if(m_bBackground == FALSE) PlayerIcon(&m_dcbb, x, y, plrIdx, FALSE);
						else PlayerIcon(&m_dcbb, x, y, plrIdx, FALSE);


					// Add Level Progress Graphics

					CRect rect = lpDrawItemStruct->rcItem;

					PlayerLevelProgressBar(&m_dcbb, x, y, rect.Width(), rect.Height(), plrIdx);


				} // end !i
			
				// draw text
		
				if(i == 1) // rating field
				{
					CString ratingText;
					int rating = -1;
					ratingText = GetItemText(lpDrawItemStruct->itemData, 1);
					if( ratingText.GetLength() )
						sscanf(ratingText.GetBuffer(0), "%d", &rating);

					//CFont *oldFont = m_dcbb.SelectObject(fnt1);
					//if(selected) m_dcbb.SetTextColor(RGB(255,255,0));
					//	else m_dcbb.SetTextColor(RGB(255,255,255));

					if(rating == -1 || rating == 0)
						{
						if(m_bBackground == FALSE) m_dcbb.TextOut(  x+2 + 6, y, "-", 1  );
							else m_dcbb.TextOut(  x+2 + 6, y, "-", 1  );
						}
					else{
						if(m_bBackground == FALSE) PlayerRatingText(this, &m_dcbb, x+2, y, plrIdx, hditem.cxy - (2 + 12), FALSE, selected, ratingText.GetBuffer(0));
							else PlayerRatingText(this, &m_dcbb, x+2, y, plrIdx, hditem.cxy - (2 + 12), FALSE, selected, ratingText.GetBuffer(0));
						//m_dcbb.TextOut(  x+2 + 6, y, ratingText, ratingText.GetLength()  );
						//m_pDC->TextOut(  x+2 + 12, y, s, s.GetLength()  ); // <- this shouldn't work??
						}
				}
				else if(i == 2)
				{
					// If we're inside table (then it means we're using Listbox which only has columns 0,1 and 2
					// column 2 is our Level

					//if(g_gameAppView->m_lobby->m_bInLobby == false)
					//if(m_player[plrIdx].table != -1 && ) // player is in the lobby
					//if( this != &((CGameAppView *)m_pParent)->m_listPlayers && this != &((CGameAppView *)m_pParent)->m_inviteDlg.m_listPlayers && this != &((CGameAppView *)m_pParent)->m_playerListDlg.m_listPlayers)
					if( this == &((CGameAppView *)m_pParent)->m_listBox)
					{
						// level 
						CString level_str;
						level_str.Format("%d", m_player[plrIdx].level);
						m_dcbb.TextOut(x+4, y, level_str, level_str.GetLength());
					}
					else
					{

						// table field
						if(m_player[plrIdx].table == -1) // player is in the lobby
						{
						
								if(m_player[plrIdx].bCanHostKnown==FALSE)
									{
									if(m_bBackground == FALSE) m_dcbb.TextOut(x+31,y, "?");
										else m_dcbb.TextOut(x+31,y, "?");
									}
								//else if(m_player[plrIdx].bCanHost)
								//	m_pDC->TextOut(x+31,y, "+");						
						
						}
						else
						{
						
							// player is at a table, is s/he watching or playing?
							if( ((m_table[m_player[plrIdx].table].player1SockIdx == m_player[plrIdx].playerSocketIdx) &&
								 (m_table[m_player[plrIdx].table].player2SockIdx != -1)) ||
								((m_table[m_player[plrIdx].table].player2SockIdx == m_player[plrIdx].playerSocketIdx) &&
								 (m_table[m_player[plrIdx].table].player1SockIdx != -1)) )
							{
								// this player is playing... (only if both seats are occupied)
								if(m_bBackground == FALSE) m_dcbb.DrawIcon(x+1,y+3, m_PlayIcon);
									else m_dcbb.DrawIcon(x+1,y+3, m_PlayIcon);
							}
							else
							{
								// this player is watching...
								if(m_bBackground == FALSE) m_dcbb.DrawIcon(x+1,y+3, m_WatchIcon);
									else m_dcbb.DrawIcon(x+1,y+3, m_WatchIcon);
							}
						
							// if player is the 'Host' of the table
							if(m_table[m_player[plrIdx].table].hostSockIdx == m_player[plrIdx].playerSocketIdx)
							{
								//COLORREF oldcolor = m_pDC->SetTextColor(RGB(20,20,20));
								if(m_bBackground == FALSE) m_dcbb.TextOut(x+31,y, "*");
									else m_dcbb.TextOut(x+31,y, "*");
								//m_pDC->SetTextColor(oldcolor);
							}
							else {
								if(m_player[plrIdx].bCanHostKnown==FALSE)
									{
									if(m_bBackground == FALSE) m_dcbb.TextOut(x+31,y, "?");
										else m_dcbb.TextOut(x+31,y, "?");
									}
								//else if(m_player[plrIdx].bCanHost)
								//	m_pDC->TextOut(x+31,y, "+");						
								}

						}
					
						if(m_bBackground == FALSE) m_dcbb.TextOut(  x+2 + 12 + 4, y, s, s.GetLength()  );					
							else m_dcbb.TextOut(  x+2 + 12 + 4, y, s, s.GetLength()  );					
					}
				}
				else if(i == 3)
				{
					// level 
					//char level_str[16];
					CString level_str;
					int level = m_player[plrIdx].level;
					level_str.Format("%d", m_player[plrIdx].level);
					m_dcbb.TextOut(x+4, y, level_str, level_str.GetLength());
				}
				else
				{
					
					// Get player index of this name
					int plrIdx = ((CGameAppView *)m_pParent)->m_lobby->FindPlayerIdx(s);
					pHdr->GetItem( i, &hditem);
					if(plrIdx!=-1) 
						{
						if(m_bBackground == FALSE) PlayerNameText(this, &m_dcbb, x+12 + 2 + 1, y, plrIdx, hditem.cxy - (2 + 12), FALSE, selected);
							else PlayerNameText(this, &m_dcbb, x+12 + 2 + 1, y, plrIdx, hditem.cxy - (2 + 12), FALSE, selected);
						}
					//TextOutEllipsis(m_pDC, x+2 + 12, y, s, hditem.cxy - (2 + 12));
	
				}
			}


			

			// paint split text (i.e. text below the split line - optional)
			if(!i) // only paint in 1st column only
			if(m_flags&GRIDSPLIT)
			if( m_splitText[lpDrawItemStruct->itemID] )
			{
				float cy;
				cy = (lpDrawItemStruct->rcItem.bottom -1) - (lpDrawItemStruct->rcItem.top+1);
				cy /= 2.0f;
				
				char buff[MAX_CHARS];
				sprintf(buff, "%s", m_splitText[lpDrawItemStruct->itemID]);
				if(m_flags&TABLE) m_pDC->TextOut(  x+2+m_gridSplitOffsetStartX, lpDrawItemStruct->rcItem.bottom-1 - cy, buff, strlen(buff));
					else m_dcbb.TextOut(  x+2+m_gridSplitOffsetStartX, lpDrawItemStruct->rcItem.bottom-1 - cy, buff, strlen(buff));
			}

			//m_pDC->SetTextColor(oldTextColor);
			
			// bump to next column based on header's tab stops
			pHdr->GetItem( i, &hditem);
			x += hditem.cxy;
			
			if(m_flags&GRIDVERT)
			{
				CPen *oldPen2 = m_dcbb.SelectObject(&pen2);
				CBrush *oldBrush2 = m_dcbb.SelectObject(&brush2);

				
				if(m_flags&GRIDSPLIT) // do half verticle line for item
				{
					float height = 0.5f * m_columnHeight;//0.25f * (float)(lpDrawItemStruct->rcItem.bottom +1);
					// 2 is probably a text metric (frame?)
					if(m_flags&TABLE)
						{
						//m_pDC->MoveTo( x-2, lpDrawItemStruct->rcItem.top -1); 
						//m_pDC->LineTo( x-2, lpDrawItemStruct->rcItem.bottom +1 - height);
						}
					else{
						//m_dcbb.MoveTo( x-2, lpDrawItemStruct->rcItem.top -1); 
						//m_dcbb.LineTo( x-2, lpDrawItemStruct->rcItem.bottom +1 - height);
						}
				}
				else // do while verticle line for item
				{
					/*
					// 2 is probably a text metric (frame?)
					if(m_flags&TABLE)
						{
						m_pDC->MoveTo( x-2, lpDrawItemStruct->rcItem.top -1); 
						m_pDC->LineTo( x-2, lpDrawItemStruct->rcItem.bottom +1 -1);
						}
					else{
						m_dcbb.MoveTo( x-2, lpDrawItemStruct->rcItem.top -1); 
						m_dcbb.LineTo( x-2, lpDrawItemStruct->rcItem.bottom +1 -1);
						}
						*/
				}
				
				if(m_flags&TABLE)
					{
					m_pDC->SelectObject(oldBrush2);
					m_pDC->SelectObject(oldPen2);
					}
				else{
					m_dcbb.SelectObject(oldBrush2);
					m_dcbb.SelectObject(oldPen2);
					}
			}
		}

		// Draw the focus state.
		if( lpDrawItemStruct->itemState & ODA_FOCUS )
		{
			if(m_flags&TABLE) m_pDC->DrawFocusRect( &(lpDrawItemStruct->rcItem) );  
				else m_dcbb.DrawFocusRect( &(lpDrawItemStruct->rcItem) );  
		}

		// Return the device context to its original state.
		if( !(m_flags&HIGHLIGHTROWDISABLED) )
		if( lpDrawItemStruct->itemState & ODS_SELECTED )
		{
			if(m_flags&TABLE)
				{
				m_pDC->SetTextColor( crOldText );
				m_pDC->SetBkColor( crOldBack );
				}
			else{
				m_dcbb.SetTextColor( crOldText );
				m_dcbb.SetBkColor( crOldBack );
				}

			if(m_lastSelectedNItem!=lpDrawItemStruct->itemID) // Change in selection
				{
				m_lastSelectedNItem = lpDrawItemStruct->itemID;
				if(m_flags&LOBBYSELECT)
				((CGameAppView *)m_pParent)->m_lobbySelectDlg.Delta(m_lastSelectedNItem);
				}
		}

	} // end of if on	ODA_DRAWENTIRE | ODA_SELECT

   	if( lpDrawItemStruct->itemAction & ODA_FOCUS )
	{
		if(m_flags&TABLE) m_pDC->DrawFocusRect( &(lpDrawItemStruct->rcItem) );
			else m_dcbb.DrawFocusRect( &(lpDrawItemStruct->rcItem) );
		
	}
	
/*
	m_pDC = CDC::FromHandle(lpDrawItemStruct->hDC);
	m_pDC->BitBlt(lpDrawItemStruct->rcItem.left, lpDrawItemStruct->rcItem.top, size.cx, size.cy, &dcMem, 0, 0, SRCCOPY);//


	if(memBitmap)
	{
		delete memBitmap;
		memBitmap = 0;
	}
*/


	
	n = lpDrawItemStruct->itemID;
	int start = GetScrollPos(SB_VERT);
	int countperpage = GetCountPerPage();
	int end = start + GetCountPerPage();

	if(m_totalItems == 0) update = 1;

	// Only do this for Player List and Lobby Select Tabs //
	
	if(!(m_flags&TABLE)) 
	//if(n == (m_totalItems-1) || n >= (end-1) || update == 1)
	//if(n == (m_totalItems-1) || update == 1)
	if(n == (m_totalItems-1) || n == (end-1) || update == 1)
		{
		CRect winRect;
		GetClientRect(winRect);
		if(m_flags&LOBBYSELECT) 	
		{

			// Create an in-memory DC compatible with the
			// display DC we're using to paint
			/*
			static int shit = 1;
			if(shit)
			{
				g_playerlistbgbmp = (HBITMAP)LoadImage(0, "Graphics/skin/playerlistbg.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
				//g_dcMemory.CreateCompatibleDC(pDC);
				// Select the bitmap into the in-memory DC
				//g_dcMemory.SelectObject(g_playerlistbgbmp);
				dcMemory2.CreateCompatibleDC(m_pDC);
				CBitmap tmp;
				tmp.CreateCompatibleBitmap(m_pDC, winRect.Width(), winRect.Height());
				dcMemory2.SelectObject(&tmp);
				shit = 0;
			}
			CBrush brush = RGB(255,255,0);
			//m_pDC->FillRect(winRect, &brush);
			*/
			//CDC *tdc = NULL;
			//tdc = CDC::FromHandle(lpDrawItemStruct->hDC);
			//tdc->SelectObject(g_playerlistbgbmp);
			//dcMemory2.FillRect(winRect, &brush);
			m_pDC->BitBlt(winRect.left, winRect.top, winRect.Width(), winRect.Height(), &m_dcbb, 0, 0, SRCCOPY);
		}
		else{
				if(m_bBackground) 
				{
					m_pDC->BitBlt(winRect.left, winRect.top, winRect.Width(), winRect.Height(), &m_dcbb, 0, 0, SRCCOPY);
				}
				else 
				{
					m_pDC->BitBlt(winRect.left, winRect.top, winRect.Width(), winRect.Height(), &m_dcbb, 0, 0, SRCCOPY);
				}
			}
		}


	//m_pDC->BitBlt(0, 0, Skin_TL[0].m_nWidth, Skin_TL[0].m_nHeight, &g_temp, 0, 0, SRCCOPY);
	//CBrush brush = RGB(0,215,205);
	//m_pDC->FillRect(CRect(0, 0, Skin_TL[0].m_nWidth, Skin_TL[0].m_nHeight), &brush);


	// Draw Bitmaps for our Button Areas (i.e. for table listctrl)
	n = lpDrawItemStruct->itemID;
	if(m_flags&TABLE)
	{
	if(m_totalButtons)
	if(m_buttonLst)
		{
		if(!m_buttonLst[n*4+0]->m_seated) DrawItemButton(m_pDC, n*4+0, "View", n, 1, TRUE);//, no);
		if(!m_buttonLst[n*4+1]->m_seated) DrawItemButton(m_pDC, n*4+1, "Watch", n, 2, TRUE);//, no);
		if(!m_buttonLst[n*4+2]->m_seated) DrawItemButton(m_pDC, n*4+2, "Join", n, 3, TRUE);//, no);
		if(!m_buttonLst[n*4+3]->m_seated) DrawItemButton(m_pDC, n*4+3, "Join", n, 4, TRUE);//, no);
		}

	}


/*
	// draw some buttons (i.e. for table listctrl)
	n = lpDrawItemStruct->itemID;
	if(m_flags&TABLE)
	{
	int start = GetScrollPos(SB_VERT);
	int countperpage = GetCountPerPage();
	int end = start + GetCountPerPage();

	if(m_totalButtons)
	if(m_buttonLst)
		{
		if(!m_buttonLst[n*4+0]->m_seated) DrawItemButton(n*4+0, "View", n, 1, TRUE);//, no);
		if(!m_buttonLst[n*4+1]->m_seated) DrawItemButton(n*4+1, "Watch", n, 2, TRUE);//, no);
		if(!m_buttonLst[n*4+2]->m_seated) DrawItemButton(n*4+2, "Join", n, 3, TRUE);//, no);
		if(!m_buttonLst[n*4+3]->m_seated) DrawItemButton(n*4+3, "Join", n, 4, TRUE);//, no);
		}

	}
*/

	// Count how many button are active

	
}

void CListCtrlEx::OnColumnclick(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	
	// TODO: Add your control notification handler code here
	m_iSubItem = pNMListView->iSubItem;

	if( !(m_flags&TABLE) && !(m_flags&LOBBYSELECT) ) // Player Lists
	if(m_iSubItem != -1)
	{
		if(m_iSubItem == 0) // Name
		{
			m_bToggleName ^= 1;
			m_bToggleRating = 0;
			m_bToggleTable = 0;
		}
		else if(m_iSubItem == 1) // Rating
		{
			m_bToggleName = 0;		
			m_bToggleRating ^= 1;
			m_bToggleTable = 0;
		}
		else if(m_iSubItem == 2) // Table
		{
			m_bToggleName = 0;		
			m_bToggleRating = 0;
			m_bToggleTable ^= 1;
		}

		
		if(this == &((CGameAppView *)m_pParent)->m_listPlayers)
			((CGameAppView *)m_pParent)->m_lobby->RefreshPlayerItem();
		else if(this == &((CGameAppView *)m_pParent)->m_inviteDlg.m_listPlayers)
			((CGameAppView *)m_pParent)->m_inviteDlg.RefreshPlayerItem();
		else if(this == &((CGameAppView *)m_pParent)->m_playerListDlg.m_listPlayers)		
			((CGameAppView *)m_pParent)->m_playerListDlg.RefreshPlayerItem();
		
		//////////////////////
		// update column headings

		LVCOLUMN lvcol;
		lvcol.mask = LVCF_TEXT;
		//lvcol.fmt = LVCFMT_LEFT;
		//lvcol.cx = 120;
		//lvcol.pszText = "*Name";
		//SetColumn(0, &lvcol);



		for(int n=0;n<3;n++)
		{
			if(n==0)
			{
				if(m_bToggleName) lvcol.pszText = "*Name";
				else lvcol.pszText = "Name";
			}
			
			if(n==1)
				{
				if(m_bToggleRating) lvcol.pszText = "*Rtng";
				else lvcol.pszText = "Rating";
				}
				
			if(n==2)
				{
				if(m_bToggleTable) lvcol.pszText = "*Tbl";
				else lvcol.pszText = "Table";
				}
			SetColumn(n, &lvcol);
		}
		//////////////////////

	}

	*pResult = 0;
}

void CListCtrlEx::OnLButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	if(!(m_flags&TABLE))
	{
		int ItemIdx = HitTest(point);
		if(ItemIdx!=-1)
		{
			char buffer[80];
			CString name = GetItemText(ItemIdx,0);
			//m_lastSelectedNItem = ItemIdx;
			strcpy(m_lastSelectedName, name.GetBuffer(0));
		}
		else
		{
			//m_lastSelectedNItem = -1;
			m_lastSelectedName[0] = 0;
		}

		if(m_lastSelectedNItem!=ItemIdx) // Change in selection
			{
			m_lastSelectedNItem = ItemIdx;
			if(m_flags&LOBBYSELECT)
			((CGameAppView *)m_pParent)->m_lobbySelectDlg.Delta(m_lastSelectedNItem);
			}

	just_selected = 1;
	}

	else if((m_flags&TABLE))
	{
		// Lets try something new for Table List, we will draw the buttons manually as images. 
		// This is much faster than using actual BitMap Buttons //

		int ItemIdx = HitTest(point);
		if(ItemIdx!=-1)
		{
			char buffer[80];
			CString name = GetItemText(ItemIdx,0);
			// Find the Column //
			
			CRect ItemRect;
			int column = -1;
			int WidthCount = 0;
			for(int n=0;n<5;n++)
				{
				//GetItemRect(0, ItemRect, LVIR_BOUNDS);
				if( (point.x >= WidthCount) && (point.x <= (WidthCount + m_columnWidth[n])) ) column = n;
				WidthCount += m_columnWidth[n];		
				}
   			column = column;


			if(column>=1 && column<=4)
			if(((CGameAppView *)m_pParent))
			{
				//char buffer[80];
				//sprintf(buffer,"%d",gameAppViewParent->m_cyc);
				//MessageBox(buffer,"",MB_OK);
			
				if(column > 1)	
					{
					if(m_buttonLst[(ItemIdx*4)+(column-1)]->m_enabled)
					if(((CGameAppView *)m_pParent)->m_tableNo == -1)
						{
						((CGameAppView *)m_pParent)->m_tableNo = ItemIdx;
						((CGameAppView *)m_pParent)->m_tableEnter = (column)-2; // 0 - View / 1 - Watch / 2 - 1P Join / 3 - 2P Join
						}
					}
				else{
					
					for(int i=0;i<MAX_TABLEITEMS;i++) m_buttonLst[i*4]->m_enabled = 1;
					m_buttonLst[ItemIdx*4]->m_enabled = 0;
					//RedrawItems(ItemIdx, ItemIdx);
					//DrawItemButton(m_pDC, n*4+0, "View", ItemIdx, 1, FALSE);//, no);

					// 'Lobby TV' Button Click, so we need to update which Table We're watching //
					g_LobbyTVInfo.lobbytablewatch = ItemIdx;
					g_gameAppView->UpdateLobbyTableWatch(ItemIdx);
					// Update the Server Also //
					g_gameAppView->UpdateLobbyTVServer(g_gameAppView->m_playerSockIdx, ItemIdx);

					// Change Lobby to Viewing TV
					g_gameAppView->m_bLobbyMode = 2;
					g_gameAppView->Invalidate();

					}
		
	}



		}
		else
		{
			//m_lastSelectedNItem = -1;
			//m_lastSelectedName[0] = 0;
		}


		just_selected = 1;

	}
	
	
	RedrawItems(0, m_totalItems);

	CListCtrl::OnLButtonDown(nFlags, point);
}

void CListCtrlEx::OnLButtonDblClk(UINT nFlags, CPoint point) 
{

	// TODO: Add your message handler code here and/or call default
	if(m_flags&LOBBYSELECT)
	{
		int ItemIdx = HitTest(point);
		if(ItemIdx!=-1)
		{
			{
			m_lastSelectedNItem = ItemIdx;
			((CGameAppView *)m_pParent)->m_lobbySelectDlg.Delta(m_lastSelectedNItem);
			((CGameAppView *)m_pParent)->m_lobbySelectDlg.Select();
			}
		}
	}
	else if(!(m_flags&TABLE))
	{
		int ItemIdx = HitTest(point);
		if(ItemIdx!=-1)
		{
			char buffer[80];
			CString name = GetItemText(ItemIdx,0);
			// ------------------------------------------- //
			// setup this player stats details for dialog
			int plrIdx = ((CGameAppView *)m_pParent)->m_lobby->FindPlayerIdx(name);
			if(plrIdx != -1)
			{
				if(!m_player[plrIdx].bIsGuest)
					((CGameAppView *)m_pParent)->SendRequestPlayerBio(plrIdx); // Request Bio. stats
				else
				{
					((CGameAppView *)m_pParent)->m_personalDetailsDlg.m_comments = _T("");
					((CGameAppView *)m_pParent)->m_personalDetailsDlg.m_town = _T("");
					((CGameAppView *)m_pParent)->m_personalDetailsDlg.m_age = _T("");
					((CGameAppView *)m_pParent)->m_personalDetailsDlg.m_name = _T("");
					((CGameAppView *)m_pParent)->m_personalDetailsDlg.m_sex = _T("");
				}

			if(!g_build.main_server) // Client Apps
				{
				((CGameAppView *)m_pParent)->SendRequestPlayerDetails(plrIdx); // Request stats, then get back to us.
				}
			} // end if plrIdx!=-1
		}
	}
	
	CListCtrl::OnLButtonDblClk(nFlags, point);
	
}

/*
void CListCtrlEx::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	// TODO: Add your message handler code here
	
	// First let the control do its default drawing.
	const MSG *msg = GetCurrentMessage();
	DefWindowProc( msg->message, msg->wParam, msg->lParam );
	
	// Draw the lines only for LVS_REPORT mode
	if( (GetStyle() & LVS_TYPEMASK) == LVS_REPORT )
	{
		// Get the number of columns
		CClientDC dc(this );
		CHeaderCtrl* pHeader = (CHeaderCtrl*)GetDlgItem(0);
		int nColumnCount = pHeader->GetItemCount();

		// The bottom of the header corresponds to the top of the line 
		RECT rect;
		pHeader->GetClientRect( &rect );
		int top = rect.bottom;

		// Now get the client rect so we know the line length and
		// when to stop
		GetClientRect( &rect );

		// The border of the column is offset by the horz scroll
		int borderx = 0 - GetScrollPos( SB_HORZ );
		for( int i = 0; i < nColumnCount; i++ )
		{
			// Get the next border
			borderx += GetColumnWidth( i );

			// if next border is outside client area, break out
			if( borderx >= rect.right ) break;

			// Draw the line.
			dc.MoveTo( borderx-1, top);
			dc.LineTo( borderx-1, rect.bottom );
		}

		//dc.SetTextColor(RGB(255,0,0));
		//dc.SetBkColor(RGB(6,60,60));
		//dc.TextOut(0,0,"Hello!");
		//SetTextColor(RGB(255,0,0));
		//SetItemText(2,1,"Hello");// there!!!!!dsdf");
		//SetItemText(2,2,"Hello there!!!!!dsdf");
	}
	
	// Do not call CListCtrl::OnPaint() for painting messages
}
*/

BOOL CListCtrlEx::OnChildNotify(UINT message, WPARAM wParam, LPARAM lParam, LRESULT* pLResult) 
{
	// TODO: Add your specialized code here and/or call the base class
	//ErrorMessage(g_wnd, "OnChildNotify");
	
	return CListCtrl::OnChildNotify(message, wParam, lParam, pLResult);
}

BOOL CListCtrlEx::OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult) 
{
	// TODO: Add your specialized code here and/or call the base class
	HD_NOTIFY FAR * phdn = (HD_NOTIFY FAR *) lParam; 
    NMHDR   hdr=phdn->hdr;
    HD_ITEM FAR* pitem=phdn->pitem; 
    int iItem = phdn->iItem;
	int iButton = phdn->iButton;
	 
    switch(hdr.code) 
    {   
    case HDN_BEGINTRACKW:
    case HDN_BEGINTRACKA:
	case HDN_ENDTRACKW:
	case HDN_ENDTRACKA:
	case HDN_DIVIDERDBLCLICKW:
	case HDN_DIVIDERDBLCLICKA:
		{
			if(!m_bAllowColumnSizing)
			{
				*pResult = TRUE;
				CListCtrl::OnNotify(wParam, lParam, pResult);
				return TRUE;
			}
        }
        break;
	}
	
	return CListCtrl::OnNotify(wParam, lParam, pResult);
}
/*
int CListCtrlEx::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CListCtrl::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	// TODO: Add your specialized creation code here
	///////////////////////SET UP THE MULTILINE HEADER CONTROL


	m_NewHeaderFont.CreatePointFont(190,"MS Serif"); 

	CHeaderCtrl* pHeader = NULL;
	pHeader = (CHeaderCtrl *)GetWindow(GW_CHILD);//GetDlgItem(0);//GetHeaderCtrl();
	
	if(pHeader == NULL)
		MessageBox("","",MB_OK);
	//	return 0;
		
	//VERIFY(m_HeaderCtrl.SubclassWindow(pHeader->m_hWnd));	
	m_HeaderCtrl.SubclassWindow(pHeader->m_hWnd);	

	//A BIGGER FONT MAKES THE CONTROL BIGGER
	m_HeaderCtrl.SetFont(&m_NewHeaderFont);

	HD_ITEM hdItem;

	hdItem.mask = HDI_FORMAT;

	for(int i=0; i < m_HeaderCtrl.GetItemCount(); i++)
	{
		m_HeaderCtrl.GetItem(i,&hdItem);

		hdItem.fmt|= HDF_OWNERDRAW;
			
		m_HeaderCtrl.SetItem(i,&hdItem);
	}
	
	return 0;
}
*/

void CListCtrlEx::SetGridFlags(unsigned int flags)
{
	m_flags = flags;
	
	// for table types only
	if(m_flags&TABLE)
	for(int n=0;n<MAX_TABLEITEMS;n++)
	if(!m_splitText[n])
		m_splitText[n] = new char[MAX_CHARS];
}

void CListCtrlEx::SetGridColor(BOOL n, COLORREF cr)
{
	if(!n) m_crGridColor1st = cr;
	else m_crGridColor2nd = cr;
}

void CListCtrlEx::SetGridSplitOffsetX(int startX, int endX)
{
	m_gridSplitOffsetStartX = startX;
	m_gridSplitOffsetEndX = endX;
}

void CListCtrlEx::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{

	// TODO: Add your message handler code here and/or call default
	
	// call base class member first - this sorts the button render order 
	CListCtrl::OnVScroll(nSBCode, nPos, pScrollBar);
	
	
	if(m_flags&TABLE)
	{
	int n;
	int start = GetScrollPos(SB_VERT);
	int countperpage = GetCountPerPage();
	int end = start + GetCountPerPage();

	// First we need to Hide all Buttons, then display only the ones in the view //
	HideAllButtons();
	}
	else
	{
	RedrawItems(0, m_totalItems);
	}

}


void CListCtrlEx::HideAllButtons(void) 
{
	if(m_flags&TABLE)
	{
	int n;
	int start = GetScrollPos(SB_VERT);
	int countperpage = GetCountPerPage();
	int end = start + GetCountPerPage();

	for(int i=0;i<MAX_TABLEITEMS;i++)
		{
			m_buttonLst[i*4+0]->Hide();
			m_buttonLst[i*4+1]->Hide();
			m_buttonLst[i*4+2]->Hide();
			m_buttonLst[i*4+3]->Hide();
		}

	}
	

}


void CListCtrlEx::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
	return;
	// call base class member first - this sorts the button render order
	CListCtrl::OnHScroll(nSBCode, nPos, pScrollBar);

	// TODO: Add your message handler code here and/or call default

	//CListCtrl::OnHScroll(nSBCode, nPos, pScrollBar);
}

void CListCtrlEx::OnSize(UINT nType, int cx, int cy) 
{
	CListCtrl::OnSize(nType, cx, cy);
	
	
	

	// TODO: Add your message handler code here
	//MessageBox("","",MB_OK);
	//RedrawWindow(NULL, NULL, RDW_INVALIDATE | RDW_UPDATENOW | RDW_ERASE | RDW_FRAME);
/*
	if(m_totalButtons)
	if(m_flags&BUTTONS)
	{
		for(int n=0;n<MAX_TABLEITEMS;n++)
		if( m_buttonLst[n]->m_IsWindowEnabled() )
		{
			DrawItemButton(n*4+0, "View", n, 1, TRUE);
			DrawItemButton(n*4+1, "Watch", n, 2, TRUE);
			DrawItemButton(n*4+2, "Join", n, 3, TRUE);
			DrawItemButton(n*4+3, "Join", n, 4, TRUE);
		}
	}
*/
}

void CListCtrlEx::OnMeasureItem(int nIDCtl, LPMEASUREITEMSTRUCT lpMeasureItemStruct) 
{
	// TODO: Add your message handler code here and/or call default
	
	//MessageBox("","",MB_OK);
	LOGFONT lf;
	GetFont()->GetLogFont( &lf );

	if( lf.lfHeight < 0 )
		lpMeasureItemStruct->itemHeight = -lf.lfHeight; 
	else
		lpMeasureItemStruct->itemHeight = lf.lfHeight;
	
	CListCtrl::OnMeasureItem(nIDCtl, lpMeasureItemStruct);
}
										
void CListCtrlEx::TriggerOnMeasureItem(UINT nID)
{
	// by Sending a WM_WINDOWPOSCHANGED message - this causing the control to be resized, thus calls OnMeasureItem(...)
	// note: also OnMeasureItem(...) is called on creation
	CRect rc;
	GetWindowRect( &rc );

	WINDOWPOS wp;
	wp.hwnd = m_hWnd;
	wp.cx = rc.Width();
	wp.cy = rc.Height();
	wp.flags = SWP_NOACTIVATE | SWP_NOMOVE | SWP_NOOWNERZORDER | SWP_NOZORDER;
	SendMessage( WM_WINDOWPOSCHANGED, nID, (LPARAM)&wp );
}

void CListCtrlEx::SetColumnHeight(int cy)
{
	m_columnHeight = cy;
}

void CListCtrlEx::SetColumnFontHeight(float factor)
{
	CFont *pFont = NULL;
	LOGFONT lf;
	pFont = GetFont();
	if(pFont)
	if( pFont->GetLogFont( &lf ) )
	{
		
		
		if( lf.lfHeight < 0 )
			m_columnHeight = -lf.lfHeight;
		else
			m_columnHeight = lf.lfHeight;
	
		m_columnHeight = (float)(factor * m_columnHeight);
	}
}



CDC g_dcMemory;
HBITMAP b1;

BOOL CListCtrlEx::OnEraseBkgnd(CDC* pDC) 
{
		
#ifndef MAIN_SERVER

	//if(m_flags&TABLE) return FALSE;
	// TODO: Add your message handler code here and/or call default
	//int nItem = GetItemCount();
	//if(nItem)
	//{
	// manually perform erase background (just erase the empty L shaped Column-item region of the listctrl)

	
	// Lets get our background bitmap into memory - quicker accessing //
	if(g_playerlistbgbmp == NULL) 
		{
		//g_playerlistbgbmp.LoadBitmap("Graphics/skin/playerlistbg.bmp");
		g_playerlistbgbmp = (HBITMAP)LoadImage(0, "Graphics/skin/playerlistbg.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
		g_dcMemory.CreateCompatibleDC(pDC);
		// Select the bitmap into the in-memory DC
		g_dcMemory.SelectObject(g_playerlistbgbmp);
		}
		
	
	if(pDC)
	{
		CRect winRect;
		GetClientRect(winRect);
		BITMAP bmpInfo;

		
		if(m_ebkfirst)
		{
			m_ebkfirst = 0;
			
			// Create an in-memory DC compatible with the display DC we're using to paint
			if(m_dcbb == NULL) 
			{
				m_dcbb.CreateCompatibleDC(pDC);
				CBitmap tmp;
				tmp.CreateCompatibleBitmap(pDC, 900, 1024);
				m_dcbb.SelectObject(&tmp);
			}

			
			//b1 = (HBITMAP)LoadImage(0, "Graphics/skin/tablebtnswood.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);		
			/*
			CRect rc = CRect(0,0,640,1024);
			CBitmap bitmap1;
			bitmap1.CreateCompatibleBitmap(pDC, rc.Width(), rc.Height());
			CBitmap* pOldBitmap1 = m_dcbb.SelectObject(&bitmap1);
			*/
			//m_dcbb.SelectObject(b1);

			m_dcbb.SetTextColor(RGB(255,255,255));
			m_dcbb.SetBkColor(RGB(0,0,0));
			m_dcbb.SetBkMode(TRANSPARENT);
			m_dcbb.SelectObject(fnt2);
		}

		// Copy the bits from the in-memory DC into the on-
		// screen DC to actually do the painting. Use the centerpoint
		// we computed for the target offset.
		if(m_flags&TABLE)
			{
			int start = GetScrollPos(SB_VERT);
			int countperpage = GetCountPerPage();
			int end = start + GetCountPerPage();
			if(end >= 50)
				{
				CBrush brush = RGB(59,19,0);
				pDC->FillRect(CRect(winRect.left, winRect.bottom-40, winRect.right, winRect.bottom), &brush);			
				}
			}
		else if(m_flags&LOBBYSELECT) 
			{
			CBrush brush = RGB(59,19,0);
			m_dcbb.FillRect(winRect, &brush);
			}
		else{
			// If we have no background image, then use a plain color //
			if(m_bBackground == FALSE)
				{
				CBrush brush = RGB(59,19,0);
				m_dcbb.FillRect(winRect, &brush);
				}
			else{
				CBrush brush = RGB(59,19,0);
				m_dcbb.FillRect(winRect, &brush);
				
				// If we're offline, then draw this to pDC //
				// If Height of ListCtrl Window is widthin 840 pixels, then just need to use part of the g_playerlistbgbmp to fill BackGround //
				if(((CGameAppView *)g_wnd)->m_playerSockIdx == -1)
					{
					if(winRect.Height() <= 640)
						{
						pDC->BitBlt(winRect.left, winRect.top, winRect.Width(), winRect.Height(), &g_dcMemory, 0, 0, SRCCOPY);
						}
					// If Height is larger than this, then we use the entire g_playerlistbgbmp plus a rectangle of color RGB(15,16,19) to fill rest //
					else{
						pDC->BitBlt(winRect.left, winRect.top, winRect.Width(), 640, &g_dcMemory, 0, 0, SRCCOPY);
						CBrush brushbg = RGB(15,16,19);
						pDC->FillRect(CRect(winRect.left, winRect.top+640, winRect.Width(), winRect.top+winRect.Height()), &brushbg);
						}
					}
				else{
					if(winRect.Height() <= 640)
						{
						m_dcbb.BitBlt(winRect.left, winRect.top, winRect.Width(), winRect.Height(), &g_dcMemory, 0, 0, SRCCOPY);
						}
					// If Height is larger than this, then we use the entire g_playerlistbgbmp plus a rectangle of color RGB(15,16,19) to fill rest //
					else{
						m_dcbb.BitBlt(winRect.left, winRect.top, winRect.Width(), 640, &g_dcMemory, 0, 0, SRCCOPY);
						CBrush brushbg = RGB(15,16,19);
						m_dcbb.FillRect(CRect(winRect.left, winRect.top+640, winRect.Width(), winRect.top+winRect.Height()), &brushbg);
						}
					}
				
				}
			}
		/*
		if(m_selected == -1) pDC->BitBlt(winRect.left, winRect.top, bmpInfo.bmWidth, bmpInfo.bmHeight, &dcMemory, 0, 0, SRCCOPY);
		else{
			pDC->BitBlt(winRect.left, winRect.top+(m_selected*20), bmpInfo.bmWidth, winRect.top+(m_selected+1)*20, &dcMemory, 0, 0, SRCCOPY);
			}
		*/
	}
#else
	if(m_flags&TABLE);
	else if(m_flags&LOBBYSELECT);
	else{
		if(pDC)
			{
			CRect winRect;
			GetClientRect(winRect);
			CBrush brushbg = RGB(59,19,0);
			pDC->FillRect(CRect(winRect.left, winRect.top, winRect.Width(), winRect.top+winRect.Height()), &brushbg);
			}
		}
	
	
#endif

	return 0;
	//}

	//return CListCtrl::OnEraseBkgnd(pDC);
}

/*
int CListCtrlEx::GetItemHeight()
{
    CRect ItemRect;
    GetSubItemRect(1, 1, LVIR_BOUNDS, ItemRect);
    return ItemRect.bottom - ItemRect.top;
}
*/
/*
void CListCtrlEx::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	// TODO: Add your message handler code here
	
	// Do not call CListCtrl::OnPaint() for painting messages
}
*/

CPoint CListCtrlEx::GetGridBottomRight()
{
    CRect column;
    int iColumn = GetHeaderCtrl()->GetItemCount() - 1;
    GetHeaderCtrl()->GetItemRect(iColumn, &column);
    CRect item;
    int nItem = GetItemCount() - 1;
    GetItemRect(nItem, &item, LVIR_BOUNDS);

    return CPoint(column.right, item.bottom);
}

void CListCtrlEx::OnDrawItem(int nIDCtl, LPDRAWITEMSTRUCT lpDrawItemStruct) 
{
	// TODO: Add your message handler code here and/or call default
	//ErrorMessage(m_pParent, "OnDrawItem!");
	
	CListCtrl::OnDrawItem(nIDCtl, lpDrawItemStruct);
}

BOOL CListCtrlEx::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
	/*
	int n = -1;
	switch(pMsg->message)
	{
	case WM_ACTIVATE: n = 0; break;
	case WM_ACTIVATEAPP: n = 1; break;
	case WM_ASKCBFORMATNAME: n = 2; break;
	case WM_CANCELMODE: n = 3; break;
	case WM_CAPTURECHANGED: n = 4; break;
	case WM_CHANGECBCHAIN: n = 5; break;
	case WM_CHAR: n = 6; break;
	case WM_CHARTOITEM: n = 7; break;
	case WM_CHILDACTIVATE: n = 8; break;
	case WM_CLOSE: n = 9; break;
	case WM_COMPACTING: n = 10; break;
	case WM_COMPAREITEM: n = 11; break;
	case WM_CONTEXTMENU: n = 12; break;
	case WM_CREATE: n = 13; break;
	case WM_CTLCOLOR: n = 14; break;
	case WM_DEADCHAR: n = 15; break;
	case WM_DELETEITEM: n = 16; break;
	case WM_DESTROY: n = 17; break;
	case WM_DESTROYCLIPBOARD: n = 18; break;
	case WM_DEVICECHANGE: n = 19; break;
	case WM_DEVMODECHANGE: n = 20; break;
	case WM_DRAWCLIPBOARD: n = 21; break;
	case WM_DRAWITEM: n = 22; break;
	case WM_DROPFILES: n = 23; break;
	case WM_ENABLE: n = 24; break;
	case WM_ENDSESSION: n = 25; break;
	case WM_ENTERIDLE: n = 26; break;
	case WM_ERASEBKGND: n = 27; break;
	case WM_FONTCHANGE: n = 28; break;
	case WM_GETDLGCODE: n = 29; break;
	case WM_GETMINMAXINFO: n = 30; break;
	//case WM_HELPINFO: n = 31; break;
	case WM_HSCROLL: n = 32; break;
	case WM_HSCROLLCLIPBOARD: n = 33; break;
	case WM_ICONERASEBKGND: n = 34; break;
	case WM_INITMENU: n = 35; break;
	case WM_INITMENUPOPUP: n = 36; break;
	case WM_KEYDOWN: n = 37; break;
	case WM_KEYUP: n = 38; break;
	case WM_KILLFOCUS: n = 39; break;
	case WM_LBUTTONDBLCLK: n = 39; break;
	case WM_LBUTTONDOWN: n = 39; break;
	case WM_LBUTTONUP: n = 39; break;
	case WM_MBUTTONDBLCLK: n = 39; break;
	case WM_MBUTTONDOWN: n = 39; break;
	case WM_MBUTTONUP: n = 39; break;
	case WM_MDIACTIVATE: n = 39; break;
	case WM_MEASUREITEM: n = 39; break;
	case WM_MENUCHAR: n = 39; break;
	case WM_MENUSELECT: n = 39; break;
	case WM_MOUSEACTIVATE: n = 39; break;
	case WM_MOUSEMOVE: n = 39; break;
	case WM_MOVE: n = 39; break;
	case WM_MOVING: n = 39; break;
	case WM_NCACTIVATE: n = 40; break;
	case WM_NCCALCSIZE: n = 40; break;
	case WM_NCCREATE: n = 40; break;
	case WM_NCDESTROY: n = 40; break;
	case WM_NCHITTEST: n = 40; break;
	case WM_NCLBUTTONDBLCLK: n = 40; break;
	case WM_NCLBUTTONDOWN: n = 40; break;
	case WM_NCLBUTTONUP: n = 40; break;
	case WM_NCMBUTTONDBLCLK: n = 40; break;
	case WM_NCMBUTTONDOWN: n = 40; break;
	case WM_NCMBUTTONUP: n = 40; break;
	case WM_NCMOUSEMOVE: n = 40; break;
	case WM_NCPAINT: n = 40; break;
	case WM_NCRBUTTONDBLCLK: n = 40; break;
	case WM_NCRBUTTONDOWN: n = 40; break;
	case WM_NCRBUTTONUP: n = 40; break;
	case WM_PAINT: n = 50; break;
	case WM_PAINTCLIPBOARD: n = 50; break;
	case WM_PALETTECHANGED: n = 50; break;
	case WM_PALETTEISCHANGING: n = 50; break;
	case WM_PARENTNOTIFY: n = 50; break;
	case WM_QUERYDRAGICON: n = 50; break;
	case WM_QUERYENDSESSION: n = 50; break;
	case WM_QUERYNEWPALETTE: n = 50; break;
	case WM_QUERYOPEN: n = 50; break;
	case WM_RBUTTONDBLCLK: n = 50; break;
	case WM_RBUTTONDOWN: n = 50; break;
	case WM_RBUTTONUP: n = 50; break;
	case WM_RENDERALLFORMATS: n = 50; break;
	case WM_RENDERFORMAT: n = 50; break;
	case WM_SETCURSOR: n = 50; break;
	case WM_SETFOCUS: n = 50; break;
	case WM_SHOWWINDOW: n = 50; break;
	case WM_SIZE: n = 50; break;
	case WM_SIZECLIPBOARD: n = 50; break;
	case WM_SIZING: n = 50; break;
	case WM_SPOOLERSTATUS: n = 50; break;
	case WM_STYLECHANGED: n = 50; break;
	case WM_STYLECHANGING: n = 50; break;
	case WM_SYSCHAR: n = 50; break;
	case WM_SYSCOLORCHANGE: n = 50; break;
	case WM_SYSCOMMAND: n = 50; break;
	case WM_SYSDEADCHAR: n = 50; break;
	case WM_SYSKEYDOWN: n = 50; break;
	case WM_SYSKEYUP: n = 50; break;
	case WM_TCARD: n = 50; break;
	case WM_TIMECHANGE: n = 50; break;
	case WM_TIMER: n = 50; break;
	case WM_VKEYTOITEM: n = 50; break;
	case WM_VSCROLL: n = 50; break;
	case WM_VSCROLLCLIPBOARD: n = 50; break;
	case WM_WINDOWPOSCHANGED: n = 50; break;
	case WM_WINDOWPOSCHANGING: n = 50; break;
	case WM_WININICHANGE: n = 50; break;
	}
		
	char buffer[81];
	sprintf(buffer, "n = %d, pMsg->message = %d", n, pMsg->message);
	ErrorMessage(m_pParent, buffer);
	*/
	

	if(pMsg->message == WM_MOUSEWHEEL)
	{
		/*
		if(m_flags&TABLE)
		{
			return 1;
		}
		*/

	}
	else if(pMsg->message == WM_KEYDOWN)
	{
		switch(pMsg->wParam)
		{
		case VK_DOWN:
        case VK_UP:
		case VK_LEFT:
        case VK_RIGHT:
        case VK_HOME: 
        case VK_END:
		case VK_PRIOR:  
        case VK_NEXT:
			//if(m_flags&BUTTONS)
			if(m_flags&TABLE)
			{
				return 1;
			}
		break;
		}		
	}
	
	return CListCtrl::PreTranslateMessage(pMsg);
}

void CListCtrlEx::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	// TODO: Add your message handler code here and/or call default

	//char buffer[80];
	//sprintf(buffer, "nChar = %d", nChar);
	//ErrorMessage(m_pParent, buffer);

	switch (nChar)
    {
		case VK_UP:
			//OnVScroll(SB_LINEUP, 0, NULL);
			break;
        case VK_DOWN:
            //OnVScroll(SB_LINEDOWN, 0, NULL);
            break;
        case VK_LEFT:
            //OnHScroll(SB_LINELEFT, 0, NULL);
            break;
        case VK_RIGHT:
            //OnHScroll(SB_LINERIGHT, 0, NULL);
            break;
        case VK_HOME:
            //OnHScroll(SB_LEFT, 0, NULL);
            break;
        case VK_END:
            //OnHScroll(SB_RIGHT,0,NULL);
            break;
        case VK_PRIOR:
            //OnVScroll(SB_PAGEUP, 0, NULL);
            break;
        case VK_NEXT:
            //OnVScroll(SB_PAGEDOWN, 0, NULL);
            break;
    }
	
	CListCtrl::OnKeyDown(nChar, nRepCnt, nFlags);
}

int CListCtrlEx::OnVKeyToItem(UINT nKey, CListBox* pListBox, UINT nIndex) 
{
	// TODO: Add your message handler code here and/or call default

//	char buffer[80];
//	sprintf(buffer, "nKey = %d", nKey);
//	ErrorMessage(m_pParent, buffer);
	
	return CListCtrl::OnVKeyToItem(nKey, pListBox, nIndex);
}

void CListCtrlEx::OnMouseMove(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	/*
	int hit = HitTest(point);
	if(hit!=-1)
	{
		if((m_flags&TABLE) || m_flags&LOBBYSELECT);
		else
			{
				ErrorMessage(g_wnd, "Here!");

			}
	}
	*/
	CListCtrl::OnMouseMove(nFlags, point);
}


BOOL CListCtrlEx::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt) 
{

	// TODO: Add your message handler code here and/or call default
	
	
	if(m_flags&TABLE)
	{
	int n;
	int start = GetScrollPos(SB_VERT);
	int countperpage = GetCountPerPage();
	int end = start + GetCountPerPage();

	// First we need to Hide all Buttons, then display only the ones in the view //
	HideAllButtons();
	}
	else
	{
	RedrawItems(0, m_totalItems);
	}


#ifndef MAIN_SERVER
	
	if(!g_gameAppView) return 0;


	// If we're in Camear Mode, do appropriate code //
	if(g_pCameraMode)
	{
		// Get Unit Vector in direction we're looking at //

		g_pCameraFree->_distancev -= zDelta * 0.1f;
		g_gameAppView->m_bUpdateTable = true;
	}


	// If we have set our shot
	if(g_gameAppView->m_snooker->m_cueBallSelect == 5)
	{
		float percent = g_gameAppView->m_snooker->m_cuePowerPercentage;
		if(zDelta>0)
			{
			if(percent < 100) percent+=1;
			}
		else{
			if(percent > 0) percent-=1;
			}
		g_gameAppView->UpdatePower((float)(percent/100));
	}
#endif



	// return CListCtrl::OnMouseWheel(nFlags, zDelta, pt);
	return true;
}


void CListCtrlEx::OnRButtonUp(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	//ErrorMessage(g_wnd, "OnRButtonUp");
	
	CListCtrl::OnRButtonUp(nFlags, point);
}

void CListCtrlEx::OnRButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	//ErrorMessage(g_wnd, "OnRButtonDown");

	// deal with the user's choice here
	//int ItemIdx = HitTest(point);
	//char buffer[80];
	//sprintf(buffer, "ItemIdx = %d", ItemIdx);
	//ErrorMessage(g_wnd, buffer);

	int hitItemIdx = HitTest(point);
	if(hitItemIdx!=-1)
	{
		if((m_flags&TABLE) || m_flags&LOBBYSELECT);
		else // player list types
			{			
				//char buff[80];
				//sprintf(buff, "hitItemIdx = %d", hitItemIdx);
				//ErrorMessage(g_wnd, buff);
				
				//if we get through here, we have trapped the right click event of the richeditctrl! 
				 CPoint point;                                            
				 ::GetCursorPos(&point); //where is the mouse?
				 CMenu menu; //lets display out context menu :) 
				  DWORD dwSelectionMade;                                       
				 VERIFY(menu.LoadMenu(IDR_MENU2) );  
				 CMenu *pmenuPopup = menu.GetSubMenu(0);
				 				 
				 ASSERT(pmenuPopup != NULL);

				 int myPlrIdx = ((CGameAppView *)g_wnd)->m_lobby->GetPlayerIdx( ((CGameAppView *)g_wnd)->m_playerSockIdx );
				 if( m_player[myPlrIdx].state&PLRSTATE_MODERATOR || m_player[myPlrIdx].state&PLRSTATE_ADMIN );
				 else
				 {
					// dim-out certain menu items
					pmenuPopup->EnableMenuItem(0,MF_GRAYED | MF_BYPOSITION);
				 }

				 dwSelectionMade = pmenuPopup->TrackPopupMenu( (TPM_LEFTALIGN|TPM_LEFTBUTTON|
                                                       TPM_NONOTIFY|TPM_RETURNCMD),
                                                       point.x, point.y, this
														);

		/*if(ItemIdx!=-1)
		{
			char buffer[80];
			CString name = GetItemText(ItemIdx,0);
			// ------------------------------------------- //
			// setup this player stats details for dialog
			int plrIdx = ((CGameAppView *)m_pParent)->m_lobby->FindPlayerIdx(name);
			if(plrIdx != -1)
			{
				if(!m_player[plrIdx].bIsGuest)
					((CGameAppView *)m_pParent)->SendRequestPlayerBio(plrIdx); // Request Bio. stats
				else
				{
					((CGameAppView *)m_pParent)->m_personalDetailsDlg.m_comments = _T("");
					((CGameAppView *)m_pParent)->m_personalDetailsDlg.m_town = _T("");
					((CGameAppView *)m_pParent)->m_personalDetailsDlg.m_age = _T("");
					((CGameAppView *)m_pParent)->m_personalDetailsDlg.m_name = _T("");
					((CGameAppView *)m_pParent)->m_personalDetailsDlg.m_sex = _T("");
				}

			if(!g_build.main_server) // Client Apps
				{
				((CGameAppView *)m_pParent)->SendRequestPlayerDetails(plrIdx); // Request stats, then get back to us.
				}
			} usa*/


				 switch(dwSelectionMade)
				 {											  
				 case ID_PLAYER_MUTE:
					 {
						//TODO: ODIN - will need server to intercept and block the message
						// build /msg <name> <text>
						CString name = GetItemText(hitItemIdx,0);
						char buffer[MAX_CHARS];
						sprintf(buffer, "/mute %s", name.GetBuffer(0));		
						((CGameAppView *)m_pParent)->MessageSend(buffer);

						/*
						// Note: this code only mutes player-to-player,  not all player-to-all
						char buffer[80];
						CString name = GetItemText(hitItemIdx,0);
						int plrIdx = ((CGameAppView *)m_pParent)->m_lobby->FindPlayerIdx(name);
						if(plrIdx != -1)
						{						
							#ifndef THE_POOL_CLUB // iSnooker
							((CGameAppView *)m_pParent)->m_statsDlg.m_bIgnore^=1; // toggle
							m_player[plrIdx].bIgnore = ((CGameAppView *)m_pParent)->m_statsDlg.m_bIgnore;
							#else // iPool
							((CGameAppView *)m_pParent)->m_statsDlg2.m_bIgnore^=1; // toggle
							m_player[plrIdx].bIgnore = ((CGameAppView *)m_pParent)->m_statsDlg2.m_bIgnore;
							#endif						
						}
						*/
					 }
				 break;
				 case ID_PLAYER_PRIVATEMSG:
					 {
					char buffer[80];
					CString name = GetItemText(hitItemIdx,0);
					 //((CGameAppView *)m_pParent)->m_statsDlg.OnToggle();
					((CGameAppView *)m_pParent)->m_statsTabCtrlDlg.DestroyWindow();

					// We can only have one Private Message Chat Window, so Destroy any others //
					((CGameAppView *)m_pParent)->m_statsChatDlg.DestroyWindow();
					
					((CGameAppView *)m_pParent)->m_statsChatDlg.m_name = name;
					((CGameAppView *)m_pParent)->m_statsChatDlg.m_msg = _T("");
					
					((CGameAppView *)m_pParent)->m_statsChatDlg.Create(IDD_STATSCHAT);
					((CGameAppView *)m_pParent)->m_statsChatDlg.CenterWindow();
					((CGameAppView *)m_pParent)->m_statsChatDlg.ShowWindow(SW_SHOW);					
					CButton *pButton = (CButton *)((CGameAppView *)m_pParent)->m_statsChatDlg.GetDlgItem(IDC_TOGGLE);
					if(pButton)
						{
							pButton->EnableWindow(FALSE);			
						}
					 }
				 break;
				 case ID_PLAYER_STATS:
					{
						char buffer[80];
						CString name = GetItemText(hitItemIdx,0);
						// ------------------------------------------- //
						// setup this player stats details for dialog
						int plrIdx = ((CGameAppView *)m_pParent)->m_lobby->FindPlayerIdx(name);
						if(plrIdx != -1)
						{
							if(!m_player[plrIdx].bIsGuest)
								((CGameAppView *)m_pParent)->SendRequestPlayerBio(plrIdx); // Request Bio. stats
							else
							{
								((CGameAppView *)m_pParent)->m_personalDetailsDlg.m_comments = _T("");
								((CGameAppView *)m_pParent)->m_personalDetailsDlg.m_town = _T("");
								((CGameAppView *)m_pParent)->m_personalDetailsDlg.m_age = _T("");
								((CGameAppView *)m_pParent)->m_personalDetailsDlg.m_name = _T("");
								((CGameAppView *)m_pParent)->m_personalDetailsDlg.m_sex = _T("");
							}

						if(!g_build.main_server) // Client Apps
							{
							((CGameAppView *)m_pParent)->SendRequestPlayerDetails(plrIdx); // Request stats, then get back to us.
							}
						}
					}
				 break;
				case ID_PLAYER_IGNORE:
					{
						((CGameAppView *)m_pParent)->m_invitationDlg->m_bDeclineAll^=1; // toggle
						g_gfxDetails.bDeclineAll = ((CGameAppView *)m_pParent)->m_lobby->m_bDeclineAll = ((CGameAppView *)m_pParent)->m_invitationDlg->m_bDeclineAll;

						CMenu *subMenu = AfxGetMainWnd()->GetMenu()->GetSubMenu(GAME_MENU);
						if(subMenu)
						{
							CMenu *subSubMenu = subMenu->GetSubMenu(GAME_INVITATIONS);
							if(subSubMenu)
							{
								// set/clear option
								if(((CGameAppView *)m_pParent)->m_lobby->m_bDeclineAll)
								{
									subSubMenu->CheckMenuItem(GAME_INVITATIONS_DECLINEALL, MF_CHECKED | MF_BYPOSITION);
								}
								else
								{
									subSubMenu->CheckMenuItem(GAME_INVITATIONS_DECLINEALL, MF_UNCHECKED | MF_BYPOSITION);
								}
							}
						}
					}
				break;
				}	 		 
			}
	}

	CListCtrl::OnRButtonDown(nFlags, point);
}
