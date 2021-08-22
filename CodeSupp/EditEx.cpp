// EditEx.cpp : implementation file
//

//http://codeguru.earthweb.com/richedit/RichEdit20.shtml <-- Good help if need info. on URLs for CRichEditCtrl

#include "stdafx.h"
#include "GameApp.h"
#include "EditEx.h"
#include "GameAppView.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CEditEx

CEditEx::CEditEx()
{
	bHasFocus = false;
}

CEditEx::~CEditEx()
{
}

BEGIN_MESSAGE_MAP(CEditEx, CRichEditCtrl)//CEdit)
	//{{AFX_MSG_MAP(CEditEx)
	ON_WM_MOUSEWHEEL()
	//}}AFX_MSG_MAP
	//ON_NOTIFY_REFLECT_EX(EN_LINK, OnRichEditExLink )
	ON_WM_KILLFOCUS()
	ON_WM_ACTIVATE()
	ON_WM_LBUTTONDOWN()
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CEditEx message handlers

void CEditEx::SetColour(COLORREF color)
{ 
	CHARFORMAT cf;
	cf.cbSize = sizeof(CHARFORMAT);
	cf.dwMask = CFM_COLOR;

	GetSelectionCharFormat(cf);
	cf.crTextColor = color;
	if( cf.dwEffects & CFE_AUTOCOLOR )
	{
		cf.dwEffects ^= CFE_AUTOCOLOR;
	}
	
	SetSelectionCharFormat(cf);
}

void CEditEx::SetUnderlined(void)
{
	CHARFORMAT cf;
	cf.cbSize = sizeof(CHARFORMAT);
	cf.dwMask = CFM_UNDERLINE;
 
	DWORD dwSelMask = GetSelectionCharFormat(cf);
	// If selection is all the same toggle UNDERLINE style
	// turn it on otherwise over the whole selection
	if( (cf.dwMask & CFM_UNDERLINE) & (dwSelMask & CFM_UNDERLINE) )
	{
		cf.dwEffects ^= CFE_UNDERLINE; 
	}
	else
	{
		cf.dwEffects |= CFE_UNDERLINE;
	}
	
	cf.dwMask = CFM_UNDERLINE;
	SetSelectionCharFormat(cf);
}

void CEditEx::SetBold(void)
{
	CHARFORMAT cf;
	cf.cbSize = sizeof(CHARFORMAT);
	cf.dwMask = CFM_BOLD;
 
	DWORD dwSelMask = GetSelectionCharFormat(cf);
	// If selection is all the same toggle BOLD style
	// turn it on otherwise over the whole selection
	if( (cf.dwMask & CFM_BOLD) & (dwSelMask & CFM_BOLD) )
	{
		cf.dwEffects ^= CFE_BOLD; 
	}
	else
	{
		cf.dwEffects |= CFE_BOLD;
	}

	cf.dwMask = CFM_BOLD;
	SetSelectionCharFormat(cf);
}

void CEditEx::SetItalic(void)
{
	CHARFORMAT cf;
	cf.cbSize = sizeof(CHARFORMAT);
	cf.dwMask = CFM_ITALIC;
 
	DWORD dwSelMask = GetSelectionCharFormat(cf);
	// If selection is all the same toggle ITALIC style
	// turn it on otherwise over the whole selection
	if( (cf.dwMask & CFM_ITALIC) & (dwSelMask & CFM_ITALIC) )
	{
		cf.dwEffects ^= CFE_ITALIC; 
	}
	else
	{
		cf.dwEffects |= CFE_ITALIC;
	}
	cf.dwMask = CFM_ITALIC;
	SetSelectionCharFormat(cf);
}

void CEditEx::SetLink(void)
{
	CHARFORMAT cf;
	cf.cbSize = sizeof(CHARFORMAT);
	cf.dwMask = CFM_LINK;
 
	DWORD dwSelMask = GetSelectionCharFormat(cf);
	// If selection is all the same toggle PROTECTED style
	// turn it on otherwise over the whole selection
	if( (cf.dwMask & CFM_LINK) & (dwSelMask & CFM_LINK) )
	{
		cf.dwEffects ^= CFE_LINK; 
	}
	else
	{
		cf.dwEffects |= CFE_LINK;
	}

	cf.dwMask = CFM_LINK;
	SetSelectionCharFormat(cf);
}

void CEditEx::SetProtected(void)
{
	CHARFORMAT cf;
	cf.cbSize = sizeof(CHARFORMAT);
	cf.dwMask = CFM_PROTECTED;
 
	DWORD dwSelMask = GetSelectionCharFormat(cf);
	// If selection is all the same toggle PROTECTED style
	// turn it on otherwise over the whole selection
	if( (cf.dwMask & CFM_PROTECTED) & (dwSelMask & CFM_PROTECTED) )
	{
		cf.dwEffects ^= CFE_PROTECTED; 
	}
	else
	{
		cf.dwEffects |= CFE_PROTECTED;
	}

	cf.dwMask = CFM_PROTECTED;
	SetSelectionCharFormat(cf);
}

int CEditEx::GetFontSize(void)
{
	CHARFORMAT cf;
	int nPointSize = 0;
	cf.cbSize = sizeof(CHARFORMAT);
	cf.dwMask = CFM_SIZE;
	DWORD dwSelMask = GetSelectionCharFormat(cf);
	// return only the font size it is the same over the whole selection
	if( (cf.dwMask & CFM_SIZE) & (dwSelMask & CFM_SIZE) )
	{
		nPointSize = cf.yHeight/20;// convert from twips to points
	}

	return nPointSize;
}

void CEditEx::SetFontSize(int nPointSize)
{
	CHARFORMAT cf;
	cf.cbSize = sizeof(CHARFORMAT);
	cf.dwMask = CFM_SIZE;
	GetSelectionCharFormat(cf);
	nPointSize *= 20;	// convert from points to twips
	cf.yHeight = nPointSize;
	cf.dwMask = CFM_SIZE;
	SetSelectionCharFormat(cf);
}

CString CEditEx::GetFontName(void)
{
	CHARFORMAT cf;
	cf.cbSize = sizeof(CHARFORMAT);
	cf.dwMask = CFM_FACE;
	DWORD dwSelMask = GetSelectionCharFormat(cf);
	CString strName =_T("");
	// return only the font name it is the same over the whole selection
	if( (cf.dwMask & CFM_FACE) & (dwSelMask & CFM_FACE) )
	{
		strName = cf.szFaceName;
	}

	return strName;
}

void CEditEx::SetFontName(const CString strFontName)
{
	int i;
	CHARFORMAT cf;
	cf.cbSize = sizeof(CHARFORMAT);
	cf.dwMask = CFM_FACE;
	GetSelectionCharFormat(cf);
	CString strName = strFontName.Left(31);  
	for (i = 0; i <= strName.GetLength()-1; i++)
	cf.szFaceName[i] = strName[i];

	cf.szFaceName[i]='\0';
	cf.dwMask = CFM_FACE;
	SetSelectionCharFormat(cf);
}


BOOL CEditEx::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	// TODO: Add your message handler code here and/or call default
#ifndef MAIN_SERVER
	// If we have set our shot
	if (g_gameAppView)
	{
		if (!bHasFocus)
		{
			// If our chat doesn't have focus, then do the MouseWheel (adjusting power bar etc. code)
			g_gameAppView->MouseWheel(zDelta);
			return true;
		}
		else
		{
			// Chat has focus, so mousewheel should scroll the chat only
			return CRichEditCtrl::OnMouseWheel(nFlags, zDelta, pt);
		}
	}

#endif

	// Make sure we have focus, before scrolling the RichEdit
	
	
	return CRichEditCtrl::OnMouseWheel(nFlags, zDelta, pt);

}


void CEditEx::OnKillFocus(CWnd* pNewWnd)
{
	CRichEditCtrl::OnKillFocus(pNewWnd);

	// TODO: Add your message handler code here
}



void CEditEx::OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized)
{
	CRichEditCtrl::OnActivate(nState, pWndOther, bMinimized);

	// TODO: Add your message handler code here
}


void CEditEx::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default

	// We have clicked on the Chat, so set it's Focus to true
	// This means we'll allow scrolling of the chat now, when mousewheel is used
	this->bHasFocus = true;

	CRichEditCtrl::OnLButtonDown(nFlags, point);
}


void CEditEx::SetHasFocus(bool focus)
{
	bHasFocus = focus;
}