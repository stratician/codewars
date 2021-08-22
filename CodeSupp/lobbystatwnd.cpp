// lobbystatwnd.cpp : implementation file
//

#include "../CodeSupp/stdafx.h"
#include "lobbystatwnd.h"
#include "../Code/LobbyStat.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern CWnd *g_wnd;

/////////////////////////////////////////////////////////////////////////////
// CLobbyStatWnd

CLobbyStatWnd::CLobbyStatWnd()
{
	m_bActive = FALSE;
	m_pParent = NULL;
	m_mode = LSWMODE_MINUTE;
}

CLobbyStatWnd::~CLobbyStatWnd()
{
}


BEGIN_MESSAGE_MAP(CLobbyStatWnd, CWnd)
	//{{AFX_MSG_MAP(CLobbyStatWnd)
	ON_WM_PAINT()
	ON_WM_CLOSE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CLobbyStatWnd message handlers

#define HEADER_POS		16
#define FOOTER_POS		16
#define SIDEHEADER_POS	16

void CLobbyStatWnd::OnPaint() 
{
	int offx,offy;
	int winx,winy;
	CPaintDC dc(this); // device context for painting
	CDC *pDC;
	pDC = &dc; // I prefer pointers! - more portable

	CRect clientRect;
	GetClientRect(&clientRect);
	
	CBrush *poldBrush;
	CPen *poldPen;
	CBrush brush(RGB(255,255,255));
	CPen pen(PS_SOLID, 1, RGB(255,255,255));

	poldBrush = pDC->SelectObject(&brush);
	poldPen = pDC->SelectObject(&pen);
 	pDC->Rectangle(&clientRect);
	// ------------------------------------- //
	int minute_idx;
	CTime timeCurrent = CTime::GetCurrentTime();
	minute_idx = timeCurrent.GetHour() * 60;
	minute_idx+= timeCurrent.GetMinute();

	
	if(m_pParent)
	{
	switch(m_mode)
		{
		case LSWMODE_HOUR:
						{
							CBrush colorBrush(RGB(255,0,0));
							CPen colorPen(PS_SOLID, 1, RGB(0,0,0));

							pDC->SelectObject(&colorBrush);
							pDC->SelectObject(&colorPen);
							// Every Hour //
							int n;
							int step_s,step_e;
							int i;
							step_s = (clientRect.right-clientRect.left)/24;

							for(n=0;n<24;n++)
							{
							step_s = n*(clientRect.right-clientRect.left);
							step_e = (n+1)*(clientRect.right-clientRect.left);
							step_s/=24;
							step_e/=24;

							pDC->Rectangle(step_s,clientRect.bottom-FOOTER_POS-(rand()%100),step_e,clientRect.bottom-FOOTER_POS);
							}

							CBrush colorBrush2(RGB(0,0,0));
							CPen colorPen2(PS_SOLID, 1, RGB(0,0,0));

							pDC->SelectObject(&colorBrush2);
							pDC->SelectObject(&colorPen2);
							
							char buffer[8];
							
							for(n=0;n<24;n++)
							{
							i = n*(clientRect.right-clientRect.left);
							i/= 24;
							sprintf(buffer,"%d",n);
							pDC->TextOut(i,clientRect.bottom-FOOTER_POS, buffer);
							}
							break;
						}
		case LSWMODE_MINUTE:
						{
							offx = SIDEHEADER_POS;
							offy = HEADER_POS;
							winx = clientRect.right-clientRect.left-offx;
							winy = clientRect.bottom-clientRect.top-FOOTER_POS-offy;

							LStatData *pData;
							CBrush colorBrush(RGB(255,0,0));
							CPen colorPen(PS_SOLID, 1, RGB(255,0,0));

							CBrush RcolorBrush(RGB(0,0,255));
							CPen RcolorPen(PS_SOLID, 1, RGB(0,0,255));


							pDC->SelectObject(&colorBrush);
							pDC->SelectObject(&colorPen);
							// Fine //
							int n;
							int i;
							int v;
							int vtot;

							pData = &m_pParent->m_data;
							
							vtot = 1;
							for(n=0;n<1440;n++)
							if(pData->playersDay[n]>vtot) vtot = pData->playersDay[n];
							
							
							BOOL bFirst = TRUE;
							for(n=0;n<(clientRect.right-clientRect.left);n++)
							{
							i = n*1440;
							i/= winx;
							if(i<0) i = 0; // Lock it
							else if(i>1440-1) i = 1440-1;

							
							v = pData->playersDay[i];
							v*=winy;
							v/=vtot;

							if(i>minute_idx && bFirst) {
											bFirst = FALSE;
											CBrush *poldBrush;
											CPen *poldPen;
											CBrush brush(RGB(0,0,0));
											CPen pen(PS_SOLID, 1, RGB(0,0,0));

											poldBrush = pDC->SelectObject(&brush);
											poldPen = pDC->SelectObject(&pen); 	

											pDC->Rectangle(offx+n,offy,offx+n+1,offy+winy);

											//pDC->SelectObject(poldBrush); // restore brush.
											//pDC->SelectObject(poldPen); // restore brush.

											pDC->SelectObject(&RcolorBrush);
											pDC->SelectObject(&RcolorPen);
											}
							else pDC->Rectangle(offx+n,offy+winy-v,offx+n+1,offy+winy);
							}

							CBrush colorBrush2(RGB(0,0,0));
							CPen colorPen2(PS_SOLID, 1, RGB(0,0,0));

							pDC->SelectObject(&colorBrush2);
							pDC->SelectObject(&colorPen2);
							
							char buffer[8];
							
							for(n=0;n<24;n++)
							{
							i = n*winx;
							i/= 24;
							sprintf(buffer,"%d",n);
							pDC->TextOut(offx+i,offy+winy, buffer);
							}
							for(n=1;n<5;n++)
							{
							i = n*winy;
							i/= 4;
							float f;
							f = n;
							f/=4; // 0-1 instead of 0-4
							f*=(float) vtot;
							sprintf(buffer,"%.2f", f);
							pDC->TextOut(0,offy+winy-i, buffer);
							}// -SIDEFOOTER_POS
							break;
						}
		} // end switch
	} // end if m_pParent

	// ------------------------------------- //
	pDC->SelectObject(poldBrush); // restore brush.	
	pDC->SelectObject(poldPen); // restore brush.	
	// Do not call CWnd::OnPaint() for painting messages
}

void CLobbyStatWnd::Appear(void)
{
	if(m_bActive) return;
	CRect rect;
	rect.SetRect(0,0,600,300);
	rect.OffsetRect(100,100);
	
	// BAP_FIX //
	//Create(NULL,"Lobby Statistics - Players Today", WS_POPUPWINDOW | WS_CAPTION | /*WS_CHILD |*/ WS_VISIBLE | WS_BORDER,rect, NULL /*this*/, 0);
	// Can't use WS_POPUPWINDOW with Create and the window handle must be valid also //
	
	//Create(NULL,"Lobby Statistics - Players Today", WS_POPUPWINDOW | WS_CAPTION | WS_CHILD | WS_VISIBLE | WS_BORDER,rect, wndDesktop, 0);
	//CWnd *wndDesktop = GetDesktopWindow();
	CreateEx(NULL, NULL,"Lobby Statistics - Players Today", WS_POPUPWINDOW | WS_CAPTION | WS_CHILD | WS_VISIBLE | WS_BORDER,rect, NULL, 0);
		
	m_bActive = TRUE;
}	

void CLobbyStatWnd::Disappear(void)
{
	if(!m_bActive) return;
	m_bActive = FALSE;
	DestroyWindow();
}

void CLobbyStatWnd::OnClose() 
{
	// TODO: Add your message handler code here and/or call default
	m_bActive = FALSE;
	CWnd::OnClose();
}
