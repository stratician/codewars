// TabCtrlEx.cpp : implementation file
//

#include "Build.h"
#include "stdafx.h"
#include "GameApp.h"
#include "GameAppView.h"
#include "TabCtrlEx.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTabCtrlEx

CTabCtrlEx::CTabCtrlEx()
{
	m_pParent = NULL;
	m_SelectedTabN = 0;

	m_crSelected = 0;
	m_crDisabled = 0;
	m_crNormal = 0;
	m_crMouseOver = 0;
	m_crTab = 0;
	m_crTabSelected = 0;

	m_bColorSelected  = FALSE;
	m_bColorDisabled  = FALSE;
	m_bColorNormal    = FALSE;
	m_bColorMouseOver = FALSE;
	m_bColorTab = FALSE;
	m_bColorTabSelected = FALSE;

	m_iIndexMouseOver = -1;

	m_bMouseOver = FALSE;

	m_bSetPaddingSize = FALSE;
	m_padSize = CSize(0, 0);
	m_tabTotalRect = CRect(0,0,0,0);
	m_created = FALSE;

	m_bSelChanged = FALSE;
}

CTabCtrlEx::~CTabCtrlEx()
{
}

void CTabCtrlEx::PassParent(CWnd *pParent)
{
	m_pParent = pParent;
}

BEGIN_MESSAGE_MAP(CTabCtrlEx, CTabCtrl)
	//{{AFX_MSG_MAP(CTabCtrlEx)
	ON_WM_CREATE()
	ON_WM_DESTROY()
	ON_WM_ERASEBKGND()
	ON_WM_MOVE()
	//}}AFX_MSG_MAP
	ON_NOTIFY_REFLECT(TCN_SELCHANGE, OnSelchange)
	ON_NOTIFY_REFLECT(TCN_SELCHANGING, OnSelchanging)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTabCtrlEx message handlers

int CTabCtrlEx::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CTabCtrl::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	// TODO: Add your specialized creation code here
	m_totalItems = 0;
	m_totalColumns = 0;
	m_maxItems = 0;
	m_columnHeight = 0;
	m_cornerPtTabs = CPoint(4, 28);
	m_SelectedTabN = 0;
	m_bSelChanged = FALSE;

	if(m_bSetPaddingSize)
		SetPadding(m_padSize);

	//int tabs = GetItemCount()
	//int m_rowCount = GetRowCount()
	//CSize size = CSize(5, 5);
	//SetPadding(size);
	
	// override colours
	//m_bColorNormal = TRUE;
	//m_crNormal = RGB(0, 0, 0);
	//m_bColorSelected = TRUE;
	//m_crSelected = RGB(0, 0, 255);

	SetCurSel(0); // always select first tab #0

	//this->ModifyStyle(WS_BORDER, 0);
	//this->ModifyStyle(WS_DLGFRAME|WS_EX_STATICEDGE|WS_THICKFRAME, WS_BORDER);

	return 0;
}

void CTabCtrlEx::OnDestroy() 
{
	CTabCtrl::OnDestroy();
	
	// TODO: Add your message handler code here
	
}

// Note: must call this function before Create(... is called - important! Otherwise will have no effect.
void CTabCtrlEx::SetPaddingSize(CSize padSize)
{
	m_bSetPaddingSize = true;
	m_padSize.cx = padSize.cx;
	m_padSize.cy = padSize.cy;
}

void CTabCtrlEx::SetCornerPointTabs(CPoint pt)
{
	m_cornerPtTabs.x = pt.x;
	m_cornerPtTabs.y = pt.y;
}

void CTabCtrlEx::SetMouseOverColor(COLORREF cr) 
{
	m_bColorMouseOver = TRUE;
	m_crMouseOver = cr;
}


void CTabCtrlEx::SetDisabledColor(COLORREF cr) 
{
	m_bColorDisabled = TRUE;
	m_crDisabled = cr;
}

void CTabCtrlEx::SetSelectedColor(COLORREF cr)
{
	m_bColorSelected = TRUE;
	m_crSelected = cr;
}


void CTabCtrlEx::SetNormalColor(COLORREF cr)
{
	m_bColorNormal = TRUE;
	m_crNormal = cr;
}

void CTabCtrlEx::SetTabColor(COLORREF cr)
{
	m_bColorTab = TRUE;
	m_crTab = cr;
}

void CTabCtrlEx::SetTabSelectedColor(COLORREF cr)
{
	m_bColorTabSelected = TRUE;
	m_crTabSelected = cr;
}

void CTabCtrlEx::CreateItem(int n, LPTSTR lpszCaption, CWnd* pWnd)
{	
	TCITEM item;
	item.mask = TCIF_TEXT|TCIF_PARAM|TCIF_IMAGE;
	item.lParam = (LPARAM) pWnd;
	item.pszText = lpszCaption;
	item.iImage = n;

	InsertItem(m_totalItems, &item);
	
	pWnd->SetWindowPos(NULL, m_cornerPtTabs.x, m_cornerPtTabs.y , 0, 0,
					   SWP_FRAMECHANGED | SWP_NOSIZE | SWP_NOZORDER);
	
	if(n == 0) pWnd->ShowWindow(SW_SHOW);
	else pWnd->ShowWindow(SW_HIDE);

	m_totalItems++;
}

void CTabCtrlEx::DrawItem( LPDRAWITEMSTRUCT lpDrawItemStruct )
{	
	CRect rect = lpDrawItemStruct->rcItem;
	rect.top += ::GetSystemMetrics(SM_CYEDGE);

	int nTabIndex = lpDrawItemStruct->itemID;
	
	if (nTabIndex < 0) return;

	BOOL bSelected = (nTabIndex == GetCurSel());

	// *NEW - to make non-selected tabs same size as selected ones
	if(!bSelected)
		rect.InflateRect(4,4);

	COLORREF crSelected = m_bColorSelected ? m_crSelected : GetSysColor(COLOR_BTNTEXT);
	COLORREF crNormal  = m_bColorNormal   ? m_crNormal   : GetSysColor(COLOR_BTNTEXT);
	COLORREF crDisabled = m_bColorDisabled ? m_crDisabled : GetSysColor(COLOR_GRAYTEXT);
	COLORREF crTab = m_bColorTab ? m_crTab : GetSysColor(COLOR_BTNFACE);
	COLORREF crTabSelected = m_bColorTabSelected ? m_crTabSelected : GetSysColor(COLOR_BTNFACE);

	char label[64];
	TC_ITEM item;
	item.mask = TCIF_TEXT|TCIF_IMAGE;
	item.pszText = label;     
	item.cchTextMax = 63;    	
	if (!GetItem(nTabIndex, &item))
		return;

	CDC* pDC = CDC::FromHandle(lpDrawItemStruct->hDC);
	if (!pDC)
		return;

	int nSavedDC = pDC->SaveDC();

	CRect rectItem;
	POINT pt;

	GetItemRect(nTabIndex, &rectItem);
	GetCursorPos(&pt);
	ScreenToClient(&pt);

	if (rectItem.PtInRect(pt))
		m_iIndexMouseOver = nTabIndex;

	pDC->SetBkMode(TRANSPARENT);
	
	// Tabs main rectangle area color
	if (bSelected) 
	{
		pDC->FillSolidRect(rect, crTabSelected);
				
/*		CPen pen(PS_SOLID, 5, RGB(0,0,0));
		CBrush brush(crTabSelected);//RGB(20,20,20));
		CPen *oldPen = pDC->SelectObject(&pen);
		CBrush *oldBrush = pDC->SelectObject(&brush);
		pDC->Rectangle(rect);//( 0, 0, m_cx, 100+1);
		pDC->SelectObject(oldPen);
		pDC->SelectObject(oldBrush);*/
	}
	else 
	{
		pDC->FillSolidRect(rect, crTab);

/*		CPen pen(PS_SOLID, 5, RGB(0,0,0));
		CBrush brush(crTabSelected);//RGB(20,20,20));
		CPen *oldPen = pDC->SelectObject(&pen);
		CBrush *oldBrush = pDC->SelectObject(&brush);
		pDC->Rectangle(rect);//( 0, 0, m_cx, 100+1);
		pDC->SelectObject(oldPen);
		pDC->SelectObject(oldBrush);*/
	}

	//**  Draw the image
	CImageList* pImageList = GetImageList();
	if (pImageList && item.iImage >= 0) 
	{

		rect.left += pDC->GetTextExtent(_T(" ")).cx;

		IMAGEINFO info;
		pImageList->GetImageInfo(item.iImage, &info);
		CRect ImageRect(info.rcImage);
		int nYpos = rect.top;

		pImageList->Draw(pDC, item.iImage, CPoint(rect.left, nYpos), ILD_TRANSPARENT);
		rect.left += ImageRect.Width();
	}

	// check for invalid tab range
	if(nTabIndex > m_totalItems || nTabIndex < 0)
	{
		pDC->SetTextColor(crDisabled);
		rect.top -= ::GetSystemMetrics(SM_CYEDGE);
		pDC->DrawText(label, rect, DT_SINGLELINE|DT_VCENTER|DT_CENTER);
	}
	else
	{
		//** selected item -----
		if (bSelected)
			pDC->SetTextColor(crSelected);
		else //** other item ---
		{
			//if (m_bColorMouseOver && nTabIndex == m_iIndexMouseOver) 
			//{
			//	pDC->SetTextColor(m_crMouseOver);	
			//}
			//else
			//{
				pDC->SetTextColor(crNormal);
			//}
		}

		rect.top -= ::GetSystemMetrics(SM_CYEDGE);
		pDC->DrawText(label, rect, DT_SINGLELINE|DT_VCENTER|DT_CENTER);

	}

	pDC->RestoreDC(nSavedDC);
}

void CTabCtrlEx::OnSelchange(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	int iNewTab = GetCurSel();
	
	m_bSelChanged = TRUE;

	// check for valid tab range
	if(iNewTab > m_totalItems || iNewTab < 0)
	{
		SetCurSel(m_SelectedTabN);
	}
	else
	{
		TCITEM item;
		CWnd* pWnd;

		item.mask = TCIF_PARAM;
		
		// hide current tab
		GetItem(m_SelectedTabN, &item);
		pWnd = reinterpret_cast<CWnd*> (item.lParam);
		ASSERT_VALID(pWnd);
		pWnd->ShowWindow(SW_HIDE);

		// show selected tab
		GetItem(iNewTab, &item);
		pWnd = reinterpret_cast<CWnd*> (item.lParam);
		ASSERT_VALID(pWnd);
		pWnd->ShowWindow(SW_SHOW);

		//m_pParent->UpdateWindow();
		//m_pParent->ShowWindow(SW_SHOW);
		m_pParent->Invalidate();
	}

	*pResult = 0;
}

void CTabCtrlEx::OnSelchanging(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	m_SelectedTabN = GetCurSel();

	*pResult = 0;
}
/*
void CTabCtrlEx::OnTimer(UINT nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default
	POINT pt;
	GetCursorPos(&pt);
	CRect rectItem, rectScreen;

	GetItemRect(m_iIndexMouseOver, rectItem);
	rectScreen = rectItem;
	ClientToScreen(rectScreen);
	
	// If mouse leaves, show normal
	if (!rectScreen.PtInRect(pt)) 
	{
		KillTimer (1);
		m_bMouseOver = false;
		m_iIndexMouseOver = -1;
		InvalidateRect(rectItem);
	}
	
	CTabCtrl::OnTimer(nIDEvent);
}

void CTabCtrlEx::OnMouseMove(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	
	//** if we should change the color of the tab ctrl ---
	if (m_bColorMouseOver)
	{
		SetTimer(1,10,NULL);

		if (m_iIndexMouseOver != -1)
		{
			CRect rectItem;
			GetItemRect(m_iIndexMouseOver, rectItem);
			if (!rectItem.PtInRect(point))
			{
				CRect rectOldItem;
				GetItemRect(m_iIndexMouseOver, rectOldItem);
				m_iIndexMouseOver = -1;
				InvalidateRect(rectOldItem);
				return;
			}
		}
	

		if (!m_bMouseOver)
		{
			TCHITTESTINFO hitTest;

			m_bMouseOver = true;
			hitTest.pt = point;

			int iItem = HitTest(&hitTest);
			if (iItem != -1 && m_arrayStatusTab[iItem])
			{
				RECT rectItem;
				GetItemRect(iItem, &rectItem);

				InvalidateRect(&rectItem);
			}
		}
	}
	
	CTabCtrl::OnMouseMove(nFlags, point);
}
*/

BOOL CTabCtrlEx::OnEraseBkgnd(CDC* pDC) 
{
	// TODO: Add your message handler code here and/or call default
	
	return 0;
	//return CTabCtrl::OnEraseBkgnd(pDC);
}

CRect CTabCtrlEx::GetTabTotalRect(void)
{
	CRect tabRect;
	CSize tabSize;
	CRect tabTotalRect = CRect(0,0,0,0);
	int tabCount = GetItemCount();
	
	for(int n=0;n<tabCount;n++)
	{
		GetItemRect(n, &tabRect);

		if(n==0) tabTotalRect = tabRect;
		else
		{
			tabSize = tabRect.Size();
			tabTotalRect.right += tabSize.cx;
		}
	}

	m_tabTotalRect = tabTotalRect;
	//tabSize = tabTotalRect.Size();
	
	return tabTotalRect;
}

void CTabCtrlEx::OnMove(int x, int y) 
{
	CTabCtrl::OnMove(x, y);
	
}
