// ReplayDlg.cpp : implementation file
//

#include "stdafx.h"
#include "GameApp.h"
#include "GameAppView.h"
#include "ReplayDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CReplayDlg dialog


CReplayDlg::CReplayDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CReplayDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CReplayDlg)
	m_time = _T("");
	//}}AFX_DATA_INIT
	//m_bToggle = FALSE;
	m_pParent = NULL;
}


void CReplayDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CReplayDlg)
	DDX_Control(pDX, IDC_SLIDER1, m_slider);
	DDX_Text(pDX, IDC_STATIC_TIME, m_time);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CReplayDlg, CDialog)
	//{{AFX_MSG_MAP(CReplayDlg)
	ON_WM_CREATE()
	ON_BN_CLICKED(IDC_REPLAY_LL, OnReplayLl)
	ON_BN_CLICKED(IDC_REPLAY_R, OnReplayR)
	ON_BN_CLICKED(IDC_REPLAY_LOAD, OnReplayLoad)
	ON_BN_CLICKED(IDC_REPLAY_SAVE, OnReplaySave)
	ON_NOTIFY(NM_CLICK, IDC_SLIDER1, OnClickSlider1)
	ON_NOTIFY(NM_RETURN, IDC_SLIDER1, OnReturnSlider1)
	ON_WM_HSCROLL()
	ON_BN_CLICKED(IDC_REPLAY_L, OnReplayL)
	ON_BN_CLICKED(IDC_REPLAY_RR, OnReplayRr)
	ON_BN_CLICKED(IDC_REPLAY_P, OnReplayP)
	//}}AFX_MSG_MAP
	//ON_COMMAND(IDCANCEL, CancelFn) // press ESC (or click cancel) to trigger this window event
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CReplayDlg message handlers

void CReplayDlg::GetParent(CWnd *pParent)
{
	m_pParent = pParent;
}

int CReplayDlg::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CDialog::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	// TODO: Add your specialized creation code here
	CRect tmpRect;
	CSize tmpSize;
	CPoint tmpPoint;

	CenterWindow();
	
	CReplay *m_replay = ((CGameAppView *)m_pParent)->m_replay;
	
	static BOOL firstTime = TRUE;
	if(firstTime)
	{
		firstTime = FALSE;
		GetWindowRect(&tmpRect);
		m_toggleMax = tmpRect.Size();
		m_toggleMin = m_toggleMax;
		m_toggleMin.cy *= 0.45f;
	}
		
	GetWindowRect(&tmpRect);
	tmpSize = tmpRect.Size();
	tmpPoint =  tmpRect.TopLeft();
	m_bToggle = FALSE;//
	if(m_bToggle)
		SetWindowPos(&wndTop, tmpPoint.x, tmpPoint.y, m_toggleMax.cx, m_toggleMax.cy, SWP_SHOWWINDOW);
	else
		SetWindowPos(&wndTop, tmpPoint.x, tmpPoint.y, m_toggleMin.cx, m_toggleMin.cy, SWP_SHOWWINDOW);
	RedrawWindow(NULL, NULL, RDW_INVALIDATE | RDW_UPDATENOW | RDW_ERASE | RDW_FRAME);

	//m_replayEdit.ShowScrollBar(SB_VERT);
	//m_replayEdit.SetScrollRange(SB_VERT,0, INT_MAX);
	
	return 0;
}

int CReplayDlg::GetSliderPos(void)
{
	CReplay *m_replay = ((CGameAppView *)m_pParent)->m_replay;
	if(m_replay->m_strikelist_idx<2) return 0; // if no moves, or one move, we can't do calc!
	return (m_replay->m_strikelist_step*100-100) / (m_replay->m_strikelist_idx-1);
}

void CReplayDlg::OnReplayLl() 
{
	// TODO: Add your control notification handler code here
	CReplay *m_replay = ((CGameAppView *)m_pParent)->m_replay;

	// Replay Loader
	//m_replay->LoadDaTxT("[BAdnCAAAAn5miEDnZOEDB,oIHCBNpllEDCYoMED,oIHCBAro09CuWO4,Al,k,9AA9gB,5CB,o2iEDSKu0CBA.DXCBAA.nUqCDEruTED,oIHCBtR9nEBAnp7ED,oIHCBA8RG8A0,FC.AAAAAAAAkAfR6ABCqRWCDcZNVEDi,zSCBA]");

	if(m_replay->GetStrike(0)==NULL) return; // [+] Should Gray this button out

	// REPLAY: Reset /////////////////////////////////////////////////////////////////
	m_replay->GotoStrike(0);
	CReplayStrike *strike = m_replay->StepStrike(); // First command should be rerack
		
	((CGameAppView *)m_pParent)->HandleReplayCommand(strike);
	m_slider.SetPos(GetSliderPos());

	m_replay->PeekNextState();
	// ///////////////////////////////////////////////////////////////////////////////
	
	// Replay Saver
	//m_replay->GenerateDaTxT()
}

void CReplayDlg::OnReplayR() 
{
	// TODO: Add your control notification handler code here
	CReplay *m_replay = ((CGameAppView *)m_pParent)->m_replay;

	if(m_replay->GetStrike(0)==NULL) return; // [+] Should Gray this button out
	// REPLAY: Step Forward /////////////////////////////////////////////////////////////////
	CReplayStrike *strike;

	if(m_replay->m_strikelist_step<m_replay->m_strikelist_idx);
	else {
		 return; // End of replay
		 }
	do {
	strike = m_replay->StepStrike();
	((CGameAppView *)m_pParent)->HandleReplayCommand(strike);
	m_slider.SetPos(GetSliderPos());
	if(strike->m_type==STRIKETYPE_BALL) break;
	} while(m_replay->m_strikelist_step<m_replay->m_strikelist_idx);
	
	m_replay->m_bPeeked = FALSE;

	// ///////////////////////////////////////////////////////////////////////////////
}

void CReplayDlg::PlayStep(void)
{
	CReplay *m_replay = ((CGameAppView *)m_pParent)->m_replay;

	if(m_replay->GetStrike(0)==NULL) return; // [+] Should Gray this button out
	// REPLAY: Step Forward /////////////////////////////////////////////////////////////////
	CReplayStrike *strike;

	if(m_replay->m_strikelist_step<m_replay->m_strikelist_idx);
	else {
		 m_replay->m_bPlay = 0;
		 UpdatePlayState();
		 return; // End of replay
		 }
	do {
	strike = m_replay->StepStrike();
	((CGameAppView *)m_pParent)->HandleReplayCommand(strike);
	m_slider.SetPos(GetSliderPos());
	if(strike->m_type==STRIKETYPE_BALL) break;
	} while(m_replay->m_strikelist_step<m_replay->m_strikelist_idx);
	
	// ///////////////////////////////////////////////////////////////////////////////
}

void CReplayDlg::OnReplayLoad() 
{
	// TODO: Add your control notification handler code here
	static char BASED_CODE szFilter[] = "Replay Files (*.rep) | *.rep ||"; 

	CFileDialog dlg(TRUE, "rep", "*.rep", OFN_NOCHANGEDIR | OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, szFilter);

	///////////////////////////////////////
    if (dlg.DoModal() == IDOK)
	{
	CReplay *m_replay = ((CGameAppView *)m_pParent)->m_replay;

	m_replay->LoadRaw(dlg.GetPathName().GetBuffer(0));

	// REPLAY: Reset /////////////////////////////////////////////////////////////////
	m_replay->GotoStrike(0);
	CReplayStrike *strike = m_replay->StepStrike(); // First command should be rerack
		
	((CGameAppView *)m_pParent)->HandleReplayCommand(strike);
	m_slider.SetPos(GetSliderPos());

	m_replay->PeekNextState();

	// ///////////////////////////////////////////////////////////////////////////////
	UpdateDisplay("---");
	}
}

/*
 * This load the File, without Gui Dialog interface...
 */
bool CReplayDlg::LoadReplay(char *fname)
{
	CReplay *m_replay = ((CGameAppView *)m_pParent)->m_replay;

	if(m_replay->LoadRaw(fname) == false) return false;

	// REPLAY: Reset /////////////////////////////////////////////////////////////////
	m_replay->GotoStrike(0);
	CReplayStrike *strike = m_replay->StepStrike(); // First command should be rerack
		
	((CGameAppView *)m_pParent)->HandleReplayCommand(strike);
	m_slider.SetPos(GetSliderPos());

	m_replay->PeekNextState();

	// ///////////////////////////////////////////////////////////////////////////////
	UpdateDisplay("---");

	return true;
}

void CReplayDlg::OnReplaySave() 
{
	// TODO: Add your control notification handler code here
	static char BASED_CODE szFilter[] = "Replay Files (*.rep) | *.rep ||"; 

	CFileDialog dlg(FALSE, "rep", "*.rep", OFN_NOCHANGEDIR | OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, szFilter);

	///////////////////////////////////////
    if (dlg.DoModal() == IDOK)
	{
	CReplay *m_replay = ((CGameAppView *)m_pParent)->m_replay;
	
	m_replay->SaveRaw(dlg.GetPathName().GetBuffer(0));
	}
}

void CReplayDlg::ReplayLoad(void)
{
	OnReplayLoad();

	// set menu 'players' - '1P Vs 2P'
	CMenu *menu = AfxGetMainWnd()->GetMenu();
	CMenu *subMenu;
	if(menu)
	{
		((CGameAppView *)m_pParent)->m_bPlayOffline = FALSE;
		((CGameAppView *)m_pParent)->m_snooker->m_playersType = -1;
				
		subMenu = menu->GetSubMenu(PLAYERS_MENU);
		if(subMenu)
		{
			subMenu->CheckMenuItem(PLAYERS_1PVS2P, MF_UNCHECKED | MF_BYPOSITION);
		}
	}
}

void CReplayDlg::ReplaySave(void)
{
	OnReplaySave();
}

void CReplayDlg::UpdateDisplay(CString str) 
{
	// TODO: Add your control notification handler code here
	m_time = str;
	UpdateData(FALSE);
}

void CReplayDlg::OnClickSlider1(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// unused
	*pResult = 0;
}

void CReplayDlg::OnReturnSlider1(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// unused
	*pResult = 0;
}

void CReplayDlg::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
	// TODO: Add your message handler code here and/or call default
	int pos;
	if(nSBCode == SB_THUMBPOSITION || nSBCode == SB_THUMBTRACK || 
	   nSBCode == SB_PAGELEFT || nSBCode == SB_PAGERIGHT)
	{	
	pos = m_slider.GetPos();

	CReplay *m_replay = ((CGameAppView *)m_pParent)->m_replay;

	if(m_replay->m_strikelist_idx<2);
	else {
		 pos = pos * (m_replay->m_strikelist_idx-1);
		 pos = pos / 100;//(m_replay->m_strikelist_step*100-100);

		 m_replay->ZoomPosition(pos);
		 }
	}
	CDialog::OnHScroll(nSBCode, nPos, pScrollBar);
}

void CReplayDlg::OnReplayL() 
{
	CReplay *m_replay = ((CGameAppView *)m_pParent)->m_replay;

	if(m_replay->GetStrike(0)==NULL) return; // [+] Should Gray this button out
	// REPLAY: Step Forward /////////////////////////////////////////////////////////////////
	CReplayStrike *strike;

	if(!m_replay->m_strikelist_step) return;

	do {
	strike = m_replay->BackStepStrike();
	if(strike->m_type!=STRIKETYPE_BALL)
	((CGameAppView *)m_pParent)->HandleReplayCommand(strike);
	m_slider.SetPos(GetSliderPos());

	if(strike->m_type==STRIKETYPE_STATE) break;
	} while(m_replay->m_strikelist_step>0);
	
	// ///////////////////////////////////////////////////////////////////////////////
}

void CReplayDlg::OnReplayRr() 
{
	// TODO: Add your control notification handler code here
	int pos;
	m_slider.SetPos(100);
	CReplay *m_replay = ((CGameAppView *)m_pParent)->m_replay;

	if(m_replay->m_strikelist_idx<2);
	else {
		 pos = m_replay->m_strikelist_idx-1;
		 m_replay->ZoomPosition(pos);
		 }
}

void CReplayDlg::OnReplayP() 
{
	// TODO: Add your control notification handler code here
	CReplay *m_replay = ((CGameAppView *)m_pParent)->m_replay;
	m_replay->m_bPlay ^=1;
	UpdatePlayState();
}

void CReplayDlg::OnCancel()
{
	// TODO:
	((CGameAppView *)m_pParent)->m_bReplayMode = FALSE;
	
	// make sure we back to true practice mode, we're nolonger in replay mode
	((CGameAppView *)m_pParent)->m_snooker->m_bTurn1P = TRUE;
	
	CDialog::OnCancel();
}

void CReplayDlg::CancelFn() // alternative function to implement OnCancel
{
	// TODO:
	
	CDialog::OnCancel();
}

void CReplayDlg::UpdatePlayState(void)
{
	// enable/disable child ctrls based on auto-play state
	CButton *pButton;
	CReplay *m_replay = ((CGameAppView *)m_pParent)->m_replay;

	if(m_replay->m_bPlay)
	{
		pButton = (CButton *)GetDlgItem(IDC_REPLAY_LL);
		if(pButton) pButton->EnableWindow(FALSE);

		pButton = (CButton *)GetDlgItem(IDC_REPLAY_L);
		if(pButton) pButton->EnableWindow(FALSE);

		pButton = (CButton *)GetDlgItem(IDC_REPLAY_P);
		if(pButton) pButton->SetWindowText("| |");

		pButton = (CButton *)GetDlgItem(IDC_REPLAY_R);
		if(pButton) pButton->EnableWindow(FALSE);

		pButton = (CButton *)GetDlgItem(IDC_REPLAY_RR);
		if(pButton) pButton->EnableWindow(FALSE);

		pButton = (CButton *)GetDlgItem(IDC_REPLAY_LOAD);
		if(pButton) pButton->EnableWindow(FALSE);

		pButton = (CButton *)GetDlgItem(IDC_REPLAY_SAVE);
		if(pButton) pButton->EnableWindow(FALSE);

		m_slider.EnableWindow(FALSE);
	}
	else
	{
		pButton = (CButton *)GetDlgItem(IDC_REPLAY_LL);
		if(pButton) pButton->EnableWindow(TRUE);

		pButton = (CButton *)GetDlgItem(IDC_REPLAY_L);
		if(pButton) pButton->EnableWindow(TRUE);

		pButton = (CButton *)GetDlgItem(IDC_REPLAY_P);
		if(pButton) pButton->SetWindowText(">");

		pButton = (CButton *)GetDlgItem(IDC_REPLAY_R);
		if(pButton) pButton->EnableWindow(TRUE);

		pButton = (CButton *)GetDlgItem(IDC_REPLAY_RR);
		if(pButton) pButton->EnableWindow(TRUE);

		pButton = (CButton *)GetDlgItem(IDC_REPLAY_LOAD);
		if(pButton) pButton->EnableWindow(TRUE);

		pButton = (CButton *)GetDlgItem(IDC_REPLAY_SAVE);
		if(pButton) pButton->EnableWindow(TRUE);

		m_slider.EnableWindow(TRUE);
	}
}