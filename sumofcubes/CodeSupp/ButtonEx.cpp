// ButtonEx.cpp : implementation file
//

#include "stdafx.h"
#include "GameApp.h"
#include "ButtonEx.h"
#include "GameAppView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CButtonEx

CButtonEx::CButtonEx()
{
	m_pParent = NULL;
	m_name[0] = 0;

}

CButtonEx::~CButtonEx()
{
}

BEGIN_MESSAGE_MAP(CButtonEx, CButton)
	//{{AFX_MSG_MAP(CButtonEx)
	ON_WM_ENABLE()
	ON_CONTROL_REFLECT(BN_CLICKED, OnClicked)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CButtonEx message handlers

void CButtonEx::SetId(int id)
{
	m_id = id;
}

void CButtonEx::PassParent(CCoreEx *pParent)
{
	m_pParent = pParent;
}

void CButtonEx::GetParent(CWnd *pParent)
{
	m_parent = pParent;
}

void CButtonEx::OnEnable(BOOL bEnable) 
{
	CButton::OnEnable(bEnable);
	
	// TODO: Add your message handler code here
	
}

void CButtonEx::OnClicked() 
{
	// TODO: Add your control notification handler code here
	Clicked();
}

void CButtonEx::Clicked() 
{
	//if(m_pParent==NULL) MessageBox("Parent Not Passed","Error",MB_OK);
	//else MessageBox("Parent Is Passed","Error",MB_OK);

	// handle sending of sit/stand request via gameShell
	//if(m_pParent) m_pParent->OnEvent(m_id);
	
	if(m_parent)
	{
		////
		// for returning lost connection player
		int plrIdx = ((CGameAppView *)m_parent)->m_lobby->GetPlayerIdx(((CGameAppView *)m_parent)->m_playerSockIdx);
		int tblIdx = ((CGameAppView *)m_parent)->m_lobby->GetTableIdxFromPlayer(((CGameAppView *)m_parent)->m_playerSockIdx);
		if(tblIdx != -1)
		{
			if(m_table[tblIdx].bLockSeats) ////NEW
				{
					if(!strcmpi(m_table[tblIdx].P1StartName, m_player[plrIdx].name) || !strcmpi(m_table[tblIdx].P2StartName, m_player[plrIdx].name))
					{
						//ErrorMessage(m_parent, "ABCD!");
						// ok to proceed
					}
					else			
					{
						// dont sit!
						//ErrorMessage(m_parent, "WXYZ!");

						//((CGameAppView *)m_parent)->m_tableNo = -1;
						((CGameAppView *)m_parent)->m_lobby->LostConnectionMessageCantSit();

						goto _skipClicked;
					}
				}		
		}
		////
		((CGameAppView *)m_parent)->SitClicked(m_id);
_skipClicked:;
	}
}


void CButtonEx::OnMouseMove(UINT nFlags, CPoint point)
{

}