// InvitationDlg.cpp : implementation file
//

#include "stdafx.h"
#include "GameApp.h"
#include "GameAppView.h"
#include "InvitationDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CInvitationDlg dialog


CInvitationDlg::CInvitationDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CInvitationDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CInvitationDlg)
	m_inviteReplyEdit = _T("");
	m_bIgnorePlayer = FALSE;
	m_bDeclineAll = FALSE;	
	m_bDeclineAllGuests = FALSE;
	//}}AFX_DATA_INIT

	m_pParent = NULL;
	m_hostSockIdx = -1;
	m_hostName[0] = 0;
	m_initialHostTbl = -1;
	m_table = -1;
	m_enterType = 0;
	m_reracks = 0;
	m_totalGames = 0;
}


void CInvitationDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CInvitationDlg)
	DDX_Text(pDX, IDC_INVITE_REPLY, m_inviteReplyEdit);
	DDV_MaxChars(pDX, m_inviteReplyEdit, 80);
	DDX_Check(pDX, IDC_INVITE_IGNOREPLAYER, m_bIgnorePlayer);
	DDX_Check(pDX, IDC_INVITE_DECLINEALL, m_bDeclineAll);
	DDX_Check(pDX, IDC_INVITE_DECLINEALLGUESTS, m_bDeclineAllGuests);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CInvitationDlg, CDialog)
	//{{AFX_MSG_MAP(CInvitationDlg)
	ON_WM_CREATE()
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_INVITE_DECLINEALL, OnInviteDeclineall)
	ON_BN_CLICKED(IDC_INVITE_DECLINEALLGUESTS, OnInviteDeclineallguests)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CInvitationDlg message handlers

void CInvitationDlg::GetParent(CWnd *pParent)
{
	m_pParent = pParent;
}

void CInvitationDlg::GetInviterHostSockIdx(int hostSockIdx, char *hostName, int initialHostTbl, int table, int enterType, int reracks, int totalGames)
{
	m_hostSockIdx = hostSockIdx;
	strcpy(m_hostName, hostName);
	m_initialHostTbl = initialHostTbl;
	m_table = table; // used for overriding host's tbl no.(NOTE: to be used by admin only or AUTOINVITE for tourney)
	m_enterType = enterType;
	m_reracks = reracks;
	m_totalGames = totalGames;
}

void CInvitationDlg::OnOK() 
{
	// TODO: Add extra validation here
	// 'YES' button
	if(m_hostSockIdx != -1)
	{
/*
		{
			char buf[256];
			sprintf(buf, "m_table = %d, m_enterType = %d", m_table+1, m_enterType);
			ErrorMessage( ((CGameAppView *)m_pParent), buf);
		}
*/

		
		if(m_hostSockIdx == 0) // if the server has autoinvited you? (i.e. in tourney lobbies)
		{
			((CGameAppView *)m_pParent)->SendInvitePressed(m_hostSockIdx, ((CGameAppView *)m_pParent)->m_playerSockIdx, NULL, NULL, TRUE, NULL, m_initialHostTbl, m_table, m_enterType); // network game only!

			// We will also refresh our stats (vCash), since this may have been changed by the Tourney System (outside of the game)
			((CGameAppView *)m_pParent)->SendServerUpdateVCash();

			goto end;
		}

		int hostIdx = ((CGameAppView *)m_pParent)->m_lobby->GetPlayerIdx(m_hostSockIdx);
		if(hostIdx!=-1)
		{
			if( strcmpi(m_player[hostIdx].name, m_hostName) == 0)
			{
				((CGameAppView *)m_pParent)->SendInvitePressed(m_hostSockIdx, ((CGameAppView *)m_pParent)->m_playerSockIdx, NULL, NULL, TRUE, NULL, m_initialHostTbl, m_table, m_enterType); // network game only!
				goto end;
			}
		}

	// message to say inviter has left lobby, so ignore this iinvitation
	char _buffer[MAX_CHARS];
	sprintf(_buffer,"*** Invitation cancelled! (%s has left this lobby) ***", m_hostName);
	((CGameAppView *)m_pParent)->Message(_buffer,10); // personal message	
	}
	end:;

	CDialog::OnOK();
}

void CInvitationDlg::OnCancel() 
{
	// TODO: Add extra cleanup here

	// 'NO' button
	if(m_hostSockIdx != -1 && m_hostSockIdx != 0)
	{
		int hostIdx = ((CGameAppView *)m_pParent)->m_lobby->GetPlayerIdx(m_hostSockIdx);
		if(hostIdx!=-1)
		{

			// If host (Player who's inviting) is in a game, Don't send Decline Invite response //
			if(((CGameAppView *)m_pParent)->m_lobby->PlayingAGame(m_hostSockIdx))
				{
				char _buffer[MAX_CHARS];
				sprintf(_buffer,"*** Invitation cancelled! (%s has already started a game) ***", m_hostName);
				((CGameAppView *)m_pParent)->Message(_buffer,10); // personal message
				goto end;
				}

			if( strcmpi(m_player[hostIdx].name, m_hostName) == 0)
			{
				UpdateData(TRUE);
				m_inviteReplyEdit += _T(" ");
				((CGameAppView *)m_pParent)->SendInvitePressed(m_hostSockIdx, ((CGameAppView *)m_pParent)->m_playerSockIdx, NULL, NULL, TRUE,
															   m_inviteReplyEdit.GetBuffer(0),
															   m_initialHostTbl, m_table, m_enterType); // network game only!
				goto end;
			}
		}
	
	// message to say inviter has left lobby, so ignore this iinvitation
	char _buffer[MAX_CHARS];
	sprintf(_buffer,"*** Invitation cancelled! (%s has left this lobby) ***", m_hostName);
	((CGameAppView *)m_pParent)->Message(_buffer,10); // personal message	
	}
	end:;

	CDialog::OnCancel();
}

int CInvitationDlg::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CDialog::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO: Add your specialized creation code here
	if(m_table == -1) // default player invite
	{
		SetWindowText("Invitation");
	}
	else // auto invites
	{
		
		
		if (m_table != -1)
		if (g_gameAppView)
		if (g_gameAppView->IsLadderMatch(m_table)) SetWindowText("Ladder Match Invitation");
		else
		{
			if (g_bTournamentLobby)	SetWindowText("Tournament Invitation");
			else SetWindowText("Invitation");
		}
		
	}

	CenterWindow();

	CRect rect;
	//rect.SetRect(0, 0, 250, 32*3);
	//rect.OffsetRect(10,10);
	rect.SetRect(0, 0, 285+20, 32*4);
	rect.OffsetRect(10,10);

	if(m_table == -1) // default player invite
	{
		int myPlrIdx = ((CGameAppView *)m_pParent)->m_lobby->GetPlayerIdx(((CGameAppView *)m_pParent)->m_playerSockIdx);	
		int plrIdx = ((CGameAppView *)m_pParent)->m_lobby->GetPlayerIdx(m_hostSockIdx);	
		CString optionsStr = ((CGameAppView *)m_pParent)->m_lobby->GetTableOptionsItemStr(m_player[plrIdx].table);
		
		char sReracks[64];		
		sReracks[0] = 0;
		#ifndef THE_POOL_CLUB
		///////////////////////////////////////////////////////////
		// show Unfinished / Games ratio of the inviter				
		sprintf(sReracks, " (Reracked Games: %d of %d)", m_reracks, m_totalGames);
		///////////////////////////////////////////////////////////
		#endif //!THE_POOL_CLUB

		char str[MAX_CHARS];	
		if(m_player[plrIdx].rating > 0)
			sprintf(str, "%s (%d) has invited you to table %d.%s\n\n%s",m_player[plrIdx].name, m_player[plrIdx].rating, m_player[plrIdx].table+1, sReracks, optionsStr.GetBuffer(0));
		else
			sprintf(str, "%s has invited you to table %d.\n\n%s",m_player[plrIdx].name, m_player[plrIdx].table+1, optionsStr.GetBuffer(0));
		m_staticText.Create(str, WS_CHILD | WS_VISIBLE, rect, this); 
	}
	else // special case: 'AutoInvite'
	{
		CString optionsStr = ((CGameAppView *)m_pParent)->m_lobby->GetTableOptionsItemStr(m_table);
		char str[MAX_CHARS];

		if(g_bTournamentLobby)
			sprintf(str, "%s has Invited you to table %d.\n\n%s", m_hostName, m_table+1, optionsStr.GetBuffer(0));
		else
			sprintf(str, "You have been Auto-Invited to table %d.\n\n%s", m_table+1, optionsStr.GetBuffer(0));

		m_staticText.Create(str, WS_CHILD | WS_VISIBLE, rect, this);
	}

	return 0;
}

void CInvitationDlg::OnDestroy() 
{
	CDialog::OnDestroy();
	
	// TODO: Add your message handler code here
	m_hostSockIdx = -1;
}

void CInvitationDlg::OnInviteDeclineallguests() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);

	g_gfxDetails.bDeclineAllGuests = ((CGameAppView *)m_pParent)->m_lobby->m_bDeclineAllGuests = m_bDeclineAllGuests;

	CMenu *subMenu = AfxGetMainWnd()->GetMenu()->GetSubMenu(GAME_MENU);
	if(subMenu)
	{
		CMenu *subSubMenu = subMenu->GetSubMenu(GAME_INVITATIONS);
		if(subSubMenu)
		{
			// set/clear option
			if(((CGameAppView *)m_pParent)->m_lobby->m_bDeclineAllGuests)
			{
				subSubMenu->CheckMenuItem(GAME_INVITATIONS_DECLINEALLGUESTS, MF_CHECKED | MF_BYPOSITION);
			}
			else
			{
				subSubMenu->CheckMenuItem(GAME_INVITATIONS_DECLINEALLGUESTS, MF_UNCHECKED | MF_BYPOSITION);
			}
		}
	}
}

void CInvitationDlg::OnInviteDeclineall() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	g_gfxDetails.bDeclineAll = ((CGameAppView *)m_pParent)->m_lobby->m_bDeclineAll = m_bDeclineAll;

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
