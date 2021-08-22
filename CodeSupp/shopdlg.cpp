// shopdlg.cpp : implementation file
//

#include "stdafx.h"
//#include "..\Graphics\realsnooker.h"

#include "C:\Program Files (x86)\Microsoft DirectX SDK (June 2010)\Include\d3dx9.h"

#include "GameApp.h"
#include "shopdlg.h"
#include "GameAppView.h"
#include "PersonalDetailsDlg.h"
#include "../Code/Effects.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


#define TOTAL_TEXT_COLORS		13

unsigned char group_select;
char g_action[128];
char g_purchased;
char g_selected_cue, g_selected_text;

DWORD color_lst[] = {
RGB(5,5,5),				// Normal/Default
RGB(64,150,236),		// Blue
RGB(219,162,52),		// Peach
RGB(212,47,212),		// Pink
RGB(105,88,52),			// Mud
RGB(235,0,0),			// Red
RGB(190,190,0),			// Yellow
RGB(2,163,92),			// Ocean
RGB(0,220,0),			// Green (8)
RGB(147,122,67),		// Tan
RGB(57,149,130),		// Jade
RGB(255,0,128),			// Rose
RGB(102,172,102),		// SoftGreen
RGB(255,255,255),		// Unused
RGB(255,255,255),		// Unused
RGB(255,255,255),		// Unused (15)
};


char *color_lstn[] = {
"1. Normal - (Cost $10 Virtual Cash)", // 0
"2. Blue - (Cost $10 Virtual Cash)",
"3. Peach - (Cost $10 Virtual Cash)",
"4. Pink - (Cost $10 Virtual Cash)",
"5. Mud - (Cost $10 Virtual Cash)",
"6. Red - (Cost $10 Virtual Cash)",
"7. Yellow - (Cost $10 Virtual Cash)",
"8. LtBlue - (Cost $10 Virtual Cash)",
"9. Green - (Cost $10 Virtual Cash)", // 8
"10. Tan - (Cost $10 Virtual Cash)",
"11. Jade - (Cost $10 Virtual Cash)",
"12. Rose - (Cost $10 Virtual Cash)",
"13. SoftGreen - (Cost $10 Virtual Cash)",
"14. White - (Cost $10 Virtual Cash)",
"15. White - (Cost $10 Virtual Cash)",
"16. White - (Cost $10 Virtual Cash)", // 15
// ---------------------- //
"ABlue - (Cost $10 Virtual Cash)",
"APurple - (Cost $10 Virtual Cash)",
"AOrange - (Cost $10 Virtual Cash)",
"ATeal - (Cost $10 Virtual Cash)",
"MRed - (Cost $10 Virtual Cash)",
"MBlue - (Cost $10 Virtual Cash)",
"MYellow - (Cost $10 Virtual Cash)",
"MGreen - (Cost $10 Virtual Cash)",
"MGray - (Cost $10 Virtual Cash)",
};


char *cue_lstn[] = {"1. Standard Cue - (Cost $15 Virtual Cash)", 
					"2. BlackWhite Cue - (Cost $15 Virtual Cash)",
					"3. Charcoal Cue - (Cost $15 Virtual Cash)",
					"4. Sea Cue - (Cost $15 Virtual Cash)",
					"5. Stealth Cue - (Cost $15 Virtual Cash)",
					"6. Vampire Cue - (Cost $15 Virtual Cash)",
					"7. Demon Cue - (Cost $15 Virtual Cash)",
					"8. Wood Cue - (Cost $15 Virtual Cash)"};



/////////////////////////////////////////////////////////////////////////////
// CShopDlg dialog


CShopDlg::CShopDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CShopDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CShopDlg)
	m_combo = _T("Cues");
	//}}AFX_DATA_INIT
	m_pParent = NULL;
	m_delay = 0;
	m_bPaintFirstTime = TRUE;
	m_shop_cue_idx = 0;
	m_shop_text_idx = 0;
	m_changed = 0;
	m_pSwapChain = NULL;
	m_pRenderSurface = NULL;
}


void CShopDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CShopDlg)
	DDX_Control(pDX, IDC_COMBO_SHOP, m_combo_ctrl);
	DDX_Control(pDX, IDC_BUTTON_P, m_prev);
	DDX_Control(pDX, IDC_BUTTON_N, m_next);
	DDX_CBString(pDX, IDC_COMBO_SHOP, m_combo);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CShopDlg, CDialog)
	//{{AFX_MSG_MAP(CShopDlg)
	ON_WM_CREATE()
	ON_WM_PAINT()
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BUTTON_N, OnButtonN)
	ON_BN_CLICKED(IDC_BUTTON_P, OnButtonP)
	ON_WM_ERASEBKGND()
	ON_WM_ACTIVATE()
	ON_CBN_EDITCHANGE(IDC_COMBO_SHOP, OnEditchangeComboShop)
	ON_CBN_SELCHANGE(IDC_COMBO_SHOP, OnSelchangeComboShop)
	ON_BN_CLICKED(IDC_BUTTON_B1, OnButtonB1)
	ON_WM_SHOWWINDOW()
	ON_WM_CAPTURECHANGED()
	ON_BN_CLICKED(IDC_BUTTON_USE, OnButtonUse)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

void CShopDlg::GetParent(CWnd *pParent)
{
	m_pParent = pParent;
}


/////////////////////////////////////////////////////////////////////////////
// CShopDlg message handlers




int CShopDlg::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	char *cue_lst[] = {"Standard", "BlackWhite", "Charcoal", "Sea", "Stealth", "Vampire", "Demon", "Wood"};	

	if (CDialog::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	// TODO: Add your specialized creation code here
	
	CenterWindow();
	SetTimer(0, (1000.0f / 20.0f), NULL);
	m_delay = 0;
	
	CRect rect;
	rect.SetRect(0, 0, 200, 20);
	rect.OffsetRect(0,0);

	group_select = 0;
	sprintf(g_action, "Browsing Cues: %s (%d/8)", cue_lst[0], 1);
	g_purchased = 0;
	g_selected_text = (m_player[g_gameAppView->m_playerIdx].textFormat>>4)&0xf;
	g_selected_cue = 0;
	if(m_player[g_gameAppView->m_playerIdx].cuetype==2) g_selected_cue = 1;
	else if(m_player[g_gameAppView->m_playerIdx].cuetype==4) g_selected_cue = 2;
	else if(m_player[g_gameAppView->m_playerIdx].cuetype==8) g_selected_cue = 3;
	else if(m_player[g_gameAppView->m_playerIdx].cuetype==16) g_selected_cue = 4;
	else if(m_player[g_gameAppView->m_playerIdx].cuetype==32) g_selected_cue = 5;
	else if(m_player[g_gameAppView->m_playerIdx].cuetype==64) g_selected_cue = 6;
	else if(m_player[g_gameAppView->m_playerIdx].cuetype==128) g_selected_cue = 7;

	m_shop_text_idx = g_selected_text;
	m_shop_cue_idx = g_selected_cue;

	HRESULT hr;


	D3DPRESENT_PARAMETERS d3dpp;
	d3dpp.BackBufferWidth					= 640;
	d3dpp.BackBufferHeight					= 256;
	d3dpp.BackBufferFormat					= D3DFMT_A8R8G8B8; // D3DFMT_UNKNOWN; //D3DFMT_A8R8G8B8;
	d3dpp.BackBufferCount					= 1;
	d3dpp.MultiSampleType					= D3DMULTISAMPLE_NONE;
	d3dpp.MultiSampleQuality				= 0;
	d3dpp.SwapEffect						= D3DSWAPEFFECT_DISCARD;
	d3dpp.hDeviceWindow						= m_hWnd;
	d3dpp.Windowed							= true;
	d3dpp.EnableAutoDepthStencil			= true;
	d3dpp.AutoDepthStencilFormat			= D3DFMT_D24S8;	// Depth Format
	d3dpp.Flags								= 0;//D3DPRESENTFLAG_DISCARD_DEPTHSTENCIL;
	d3dpp.FullScreen_RefreshRateInHz		= D3DPRESENT_RATE_DEFAULT;
	d3dpp.PresentationInterval				= D3DPRESENT_INTERVAL_IMMEDIATE;
	g_pDevice->CreateAdditionalSwapChain(&d3dpp, &m_pSwapChain);



	//D3DSURFACE_DESC desc;
	//pRenderSurface->GetDesc( &desc );

	// create our surface as a render target //
	//HRESULT hr = g_pDevice->CreateRenderTarget( 512, 256, desc.Format, desc.MultiSampleType, desc.MultiSampleQuality, false, &m_pRenderSurface, NULL);
	
	

	


	return 0;
}



int PURCHASED_CUE(char idx)
{
	if(idx==0) return 1;
	else if(m_player[g_gameAppView->m_playerIdx].inventory&(1<<(16+idx))) return 1;
return 0;
}

int PURCHASED_TEXT(char idx)
{
	if(idx==0) return 1;
	else if(m_player[g_gameAppView->m_playerIdx].inventory&(1<<(idx-1))) return 1;
	
return 0;
}	


void CShopDlg::OnPaint() 
{
	CPaintDC dc(this); // device context for painting

	if(m_hWnd) if(!IsWindowVisible()) return;

	// TODO: Add your message handler code here

	if(m_bPaintFirstTime)
	{
			
		// Select the first item in the combo box.
		int nCount = m_combo_ctrl.GetCount();
		m_combo_ctrl.SetCurSel(0);


		// create compatible DC double buffer
		m_dblDC.CreateCompatibleDC(&dc);
		


		// init double buffer with empty bitmap
		CRect rcClient;
		GetClientRect(rcClient);
		m_dblBitmap.CreateCompatibleBitmap(&dc, rcClient.Width(), rcClient.Height());
		CBitmap *pOldBitmap = m_dblDC.SelectObject(&m_dblBitmap);
	}
	
	// display player attributes
	int n;
	char buff[128];
	CPen pen(PS_SOLID, 2, RGB(0,0,0));
	CPen pen2(PS_NULL, 2, RGB(0,0,0));
	CPen penYellow(PS_SOLID, 2, RGB(255,255,0));
	CPen penBlue(PS_SOLID, 2, RGB(0,0,255));
	CPen pen3(PS_SOLID, 2, RGB(255,255,255));
	CBrush brushBG(::GetSysColor(COLOR_BTNFACE));
	CBrush brush1(RGB(255,0,0));
	CBrush brushLtGreen(RGB(100,255,100));
	CBrush brushRed(RGB(255,0,0));
	CBrush brushBlue1(RGB(0,0,55));
	CBrush brushBlue2(RGB(80,0,0));
	CBrush brushGreen(RGB(0,255,0));
	CBrush brushGrey(RGB(127,127,127));
	CBrush brushWhite(RGB(250,250,250));
	CBrush brushBlack(RGB(0,0,0));
	CBrush btn(GetSysColor(COLOR_BTNFACE));
	
	//choose the font
	CFont m_Font;
	LOGFONT* m_pLF;
	char buf[80];
	m_pLF=(LOGFONT*)calloc(1,sizeof(LOGFONT));
	strncpy(m_pLF->lfFaceName,"Arial Bold",31);
	m_pLF->lfHeight=16;
	m_pLF->lfWeight=200;
	m_pLF->lfItalic=0;
	m_pLF->lfUnderline=0;
	m_Font.CreateFontIndirect(m_pLF);

	CPen *oldPen = m_dblDC.SelectObject(&pen);
	CBrush *oldBrush = m_dblDC.SelectObject(&brush1);	
	CFont *oldFont = m_dblDC.SelectObject(&((CGameAppView *)m_pParent)->m_listCtrlFont);
	UINT oldAlign = m_dblDC.SetTextAlign(TA_CENTER);


	// draw background
	CRect clip;
	m_dblDC.GetClipBox(clip);
	m_dblDC.SelectObject(&pen2);
	m_dblDC.SelectObject(&brushBG);
	m_dblDC.Rectangle(clip.left, clip.top, clip.right, clip.bottom);

	m_dblDC.SelectObject(&pen);
	m_dblDC.SelectObject(&brush1);



	CRect rcClient;
	GetClientRect(rcClient);
	
	CRect rect1;
	rect1.left = 0;
	rect1.top = 0;
	rect1.right = rcClient.Width();
	rect1.bottom = 20;	



	

	//////////////////////////////////////////////
	// clip portion of dialogue for sprite surface
	if(m_changed)
	{
		int x1, y1, w1, h1;
		x1 = 0;
		y1 = 20;
		w1 = rcClient.Width();
		h1 = 100;
		CRect rectSurf = CRect(x1, y1, x1+w1, y1+h1);
		dc.ExcludeClipRect(&rectSurf);
	}
	//////////////////////////////////////////////



	// blt buffered Surface
	int w, h;
	int x, y;
		
	//if(m_bPaintFirstTime)
	//	{
		w = rcClient.Width();
		h = rcClient.Height();
		x = rcClient.left;
		y = rcClient.top;
	//	}	
	/*else{
		CRect rcClip;
		dc.GetClipBox(rcClip);

		if(m_bPaintFirstTime)
		w  = rcClip.Width();
		h = rcClip.Height();
		x       = rcClip.left;
		y       = rcClip.top;
		}
*/

	
	if(m_bPaintFirstTime) 
		{
		dc.BitBlt(x, y, w, h, &m_dblDC, x, y, SRCCOPY);
		}

	m_bPaintFirstTime = FALSE;

	m_changed=0;


	
	dc.FillRect(rcClient,&btn);
	COLORREF rgb;
	rgb = ::GetSysColor(COLOR_BTNFACE);
	BYTE r = GetRValue(rgb);
	BYTE g = GetGValue(rgb);
	BYTE b = GetBValue(rgb);
	CBrush brushgb(RGB(r-85,g-85,b-85));
	dc.FillRect(CRect(0,20,rcClient.right, 120),&brushgb);

	// Draw Title //
	if(group_select==0 && m_shop_cue_idx==g_selected_cue) dc.FillRect(rect1, &brushBlue1);
	else if(group_select==1 && m_shop_text_idx==g_selected_text) dc.FillRect(rect1, &brushBlue2);
	else dc.FillRect(rect1, &brushBlack);
	
	dc.SetTextColor(RGB(255,255,255));
	dc.SetBkMode(TRANSPARENT);
	CFont* pOldFont=NULL;
	if (m_Font.m_hObject) 
		pOldFont = dc.SelectObject(&m_Font);
	else 
		dc.SelectObject(GetStockObject(DEFAULT_GUI_FONT));

	dc.SetTextAlign(TA_CENTER);
	
	if(group_select==0)	
	{
		if(PURCHASED_CUE(m_shop_cue_idx)) sprintf(buf, "%s - PURCHASED", cue_lstn[m_shop_cue_idx]);
		else sprintf(buf, "%s", cue_lstn[m_shop_cue_idx]);
	}
	else if(group_select==1) 
	{
		if(PURCHASED_TEXT(m_shop_text_idx)) sprintf(buf, "%s - PURCHASED", color_lstn[m_shop_text_idx]);
		else sprintf(buf, "%s", color_lstn[m_shop_text_idx]);
		dc.SetTextColor(color_lst[m_shop_text_idx]);
	}

	
	dc.TextOut(0.5*rcClient.Width(), 0, buf);

	dc.SetTextAlign(TA_LEFT);
	dc.FillRect(CRect(120,200,rcClient.right,280), &btn);
	dc.SetTextColor(RGB(45,45,45));
	dc.SetBkMode(TRANSPARENT);
	sprintf(buf, "Name: %s", m_player[g_gameAppView->m_playerIdx].name);
	dc.TextOut(120,200,buf);
	sprintf(buf, "Current Virtual Cash: $%ld.%02ld", m_player[g_gameAppView->m_playerIdx].cash/100, m_player[g_gameAppView->m_playerIdx].cash%100);
	dc.TextOut(120,220,buf);
	if(g_purchased==0) dc.SetTextColor(RGB(45,45,45));
	else if(g_purchased==-1) dc.SetTextColor(RGB(190,45,45));
		else dc.SetTextColor(RGB(45,190,45));
	g_purchased = 0;
	dc.TextOut(120,240,g_action);




	// Do not call CDialog::OnPaint() for painting messages
}





extern Mgfx_spr g_spr3dSurface;


void CShopDlg::Draw3D(void)
{

#ifndef MAIN_SERVER


	if(m_hWnd) if(!IsWindowVisible()) return;

	if(!g_pDevice) return;
	//
	// Set the projection matrix.
	//

	
	CRect clientrect;
	CRect srect;
	CRect drect;
	
	HRESULT hr;

	

	::GetWindowRect(m_hWnd, clientrect);
	
	

	// Set render target to Mini Camera BackBuffer //
	//HRESULT hr = g_pDevice->SetRenderTarget( 0, m_pRenderSurface );



	hr = g_pDevice->SetRenderTarget( 0, pRenderSurfaceShop );
	hr = m_pSwapChain->GetBackBuffer( 0, D3DBACKBUFFER_TYPE_MONO, &pRenderSurfaceShop);


	D3DVIEWPORT9 mainViewPort;

	int x, y, w, h;
	x = 0;
	y = 20;
	w = clientrect.Width();
	h = 100;

	int p;

    mainViewPort.X      = 0;
    mainViewPort.Y      = 0;
    mainViewPort.Width  = 512;
	mainViewPort.Height = 256;
    mainViewPort.MinZ   = 0.0f;
    mainViewPort.MaxZ   = 1.0f;


	float aspect = 1.0f;//(float)mainViewPort.Width / (float)mainViewPort.Height;
	aspect = (float)w / (float)h;

	g_pCamera->_cameraFov = D3DX_PI * 0.10f;

	D3DXMATRIX proj;
	D3DXMatrixPerspectiveFovLH(
			&proj,
			g_pCamera->_cameraFov,
			aspect,
			100.0f,
			10000.0f);
	g_pDevice->SetTransform(D3DTS_PROJECTION, &proj);

	// Make sure our Camera knows what Aspect Ratio we are using //
	g_pCamera->_cameraAspect = aspect;

	//
	// Get Offset and Starting Positions //
	//

	srect.left = 0;
	srect.top = 0;
	srect.right = 512;//mainViewPort.Width;
	srect.bottom = 256;//mainViewPort.Height;

	drect.left = x;
	drect.top = y;
	drect.right = x+w;
	drect.bottom = y+h;


	M3d_vtx pos;
	M3d_vtx at;
	M3d_vtx up;

	if(g_build.gameType == GAMETYPE_SNOOKER)
	{
		g_pCamera->SetPosition(D3DXVECTOR3(7000.0f, 0.0f, 3800.0f));
		g_pCamera->_look.x = 500.0f;
	}
	else
	{
		g_pCamera->SetPosition(D3DXVECTOR3(5400.0f, 0.0f, 3400.0f));
		g_pCamera->_look.x = 200.0f;
	}

	g_pCamera->SetPosition(D3DXVECTOR3(0.0f, 1500.0f, 0.0f));

	g_pCamera->_look[0] = 0;
	g_pCamera->_look[1] = 0;
	g_pCamera->_look[2] = 0;
	g_pCamera->_pos[0] = 0;
	g_pCamera->_pos[1] = 1500;
	g_pCamera->_pos[2] = 0;
	g_pCamera->_up[0] = 0;
	g_pCamera->_up[1] = 0;
	g_pCamera->_up[2] = 1;

	g_pCamera->_cameraFov = 1.570795f/4.0f;


	// Position and aim the camera //
	D3DXMATRIX V;
	D3DXMatrixLookAtLH(&V, &g_pCamera->_pos, &g_pCamera->_look, &g_pCamera->_up);
	g_pDevice->SetTransform(D3DTS_VIEW, &V);


	g_gameAppView->m_snooker->CameraPosAtUpFovAspect(pos, at, up, g_pCamera->_cameraFov, g_pCamera->_cameraAspect);



    //
    // Render to the Main viewport
    //

	D3DXMATRIX matView;
    D3DXMATRIX matWorld;
    D3DXMATRIX matRotation;
    D3DXMATRIX matTranslation;

    //g_pDevice->SetViewport( &mainViewPort );

	//HRESULT hr;
	//hr = g_pDevice->SetRenderTarget(0, pRenderSurface);

	g_pDevice->SetViewport( &mainViewPort );

    // Now we can clear just view-port's portion of the buffer to red...
    hr = g_pDevice->Clear( 0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER,
                         D3DCOLOR_COLORVALUE( 0.0f, 0.2f, 0.2f, 1.0f ), 1.0f, 0L );

    g_pDevice->BeginScene();

	//g_gameAppView->Render3DSnookerTable();

/*
	// Back Up Light Settings //
	M3d_vtx lightPos_old[3];
	for(p=0;p<3;p++)
		{
		lightPos_old[p][0] = ((CGameAppView *)m_pParent)->m_environment->m_lightPos[p][0];
		lightPos_old[p][1] = ((CGameAppView *)m_pParent)->m_environment->m_lightPos[p][1];
		lightPos_old[p][2] = ((CGameAppView *)m_pParent)->m_environment->m_lightPos[p][2];
		}


	//hr = g_pDevice->SetRenderState(D3DRS_ZENABLE, true);
	hr = g_pDevice->SetRenderState(D3DRS_LIGHTING, true);
	//hr = g_pDevice->SetRenderState(D3DRS_NORMALIZENORMALS, true);
	//g_pDevice->SetRenderState(D3DRS_ZWRITEENABLE, true);


	((CGameAppView *)m_pParent)->m_environment->LightPos(0,0,800,0);
	
	if(g_build.gameType == GAMETYPE_SNOOKER) 
		{
		((CGameAppView *)m_pParent)->m_environment->LightPos(1,500,900,0);
		((CGameAppView *)m_pParent)->m_environment->LightPos(2,0,500,900);
		}


	
	((CGameAppView *)m_pParent)->m_environment->Update();
	((CGameAppView *)m_pParent)->m_environment->Process();

*/


	//M3d_zbuffer(1);

	//hr = g_pDevice->SetRenderState(D3DRS_ZWRITEENABLE, true);
	//g_pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, true);
	//g_pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, false);


	g_pDevice->SetTexture(0, NULL);

	switch(group_select)
	{
	case 0:	// Cues //
			//hr = g_pDevice->SetRenderState(D3DRS_ZENABLE, false);
			Render3DCue();
			//if(PURCHASED_CUE(m_shop_cue_idx)) Render3DPurchased();
			
			break;
	case 1:	// Text Colors //
			//hr = g_pDevice->SetRenderState(D3DRS_ZENABLE, false);
			Render3DPen();
			//if(PURCHASED_TEXT(m_shop_text_idx)) Render3DPurchased();
			break;
	}	



	

	//Render3DOverLayHelper();



	g_pDevice->SetRenderState(D3DRS_ZENABLE, TRUE);
	g_pDevice->SetRenderState(D3DRS_LIGHTING, true);
	g_pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, false);

 	
	g_pDevice->EndScene();
 
	//g_pDevice->Present( srect, drect, m_hWnd, NULL );

	//hr = g_pDevice->SetRenderTarget(0, pRenderSurfaceMiniCam);


	//CRect rect;
	//::GetClientRect(m_hWnd, &rect);
	//::ShowWindow(g_hwndDX, SW_SHOW);
	//::MoveWindow(g_hwndDX, 200, 200, 512, 256, true);

	//g_pSwapChain->Present( srect, drect, g_hwndDX, NULL, NULL);
	//g_pSwapChain->Present( srect, drect, g_hwnd, NULL, NULL);
	m_pSwapChain->Present( srect, drect, m_hWnd, NULL, NULL);
	

	//hr = g_pDevice->StretchRect(pRenderSurfaceShop, NULL, pRenderSurface, NULL, D3DTEXF_NONE);
	//g_pDevice->Present( srect, drect, m_hWnd, NULL );



	/*
	hr = g_pSwapChain->GetBackBuffer( 0, D3DBACKBUFFER_TYPE_MONO, &pRenderSurfaceMiniCam);


	// Restore Light Settings //

	((CGameAppView *)m_pParent)->m_environment->LightPos(0,lightPos_old[0][0],lightPos_old[0][1],lightPos_old[0][2]);
	((CGameAppView *)m_pParent)->m_environment->LightPos(1,lightPos_old[1][0],lightPos_old[1][1],lightPos_old[1][2]);
	((CGameAppView *)m_pParent)->m_environment->LightPos(2,lightPos_old[2][0],lightPos_old[2][1],lightPos_old[2][2]);
	((CGameAppView *)m_pParent)->m_environment->Update();
	((CGameAppView *)m_pParent)->m_environment->Process();

	*/

#endif


	
/*

	if(m_hWnd) if(!IsWindowVisible()) return;

	int p;
	CDC *pdc = GetDC();
	if(pdc)
	{
		CRect rcClient;
		GetClientRect(rcClient);
		
		// set clip
		//Mgfx_clip(m_origin.left,m_origin.top, rightMargin, 250);

		COLORREF rgb;
		rgb = ::GetSysColor(COLOR_BTNFACE);
		BYTE r = GetRValue(rgb);
		BYTE g = GetGValue(rgb);
		BYTE b = GetBValue(rgb);


		// set viewport usa
		int x, y, w, h;
		x = 0;
		y = 20;
		w = rcClient.Width();
		h = 100;
		M3d_viewport(0, 0, w, h, 0, 1);
		
		if(group_select==0 && m_shop_cue_idx==g_selected_cue) M3d_clear(r-85, g-85+sin(((float)m_delay*16)*FF2RAD)*10, b-85);
		else if(group_select==1 && m_shop_text_idx==g_selected_text) M3d_clear(r-85, g-85+sin(((float)m_delay*16)*FF2RAD)*10, b-85);
		else M3d_clear(r-85,g-85,b-85);

		
		//M3d_clear(80,80,80);
		
		// 2D ////////////////////////////////////////////////////////////////
		M3d_transformview2d();
		M2d_sprshape(M2D_SHAPE_RECT);

		trans_mtx.m3d_proj[0][0] = 1.0f;//0.6f;//ASPECT_RATIO_MINICAM;

		lpd3dd->SetTransform(D3DTRANSFORMSTATE_VIEW,&trans_mtx.d3d_view);	// Change Viewing Matrix.
		lpd3dd->SetTransform(D3DTRANSFORMSTATE_PROJECTION,&trans_mtx.d3d_proj);	// Change Projection Matrix.

		// Setup Lighting Position
		//((CGameAppView *)m_pParent)->m_environment->LightPos(0,0,200,900);
				
		// Sets up the camera mode positioning/Look at / nearclip etc.
		M3d_camera.near_clip = 1.0f;
		M3d_camera.far_clip = 10000.0f;

		M3d_camera.at[0] = 0;
		M3d_camera.at[1] = 0;
		M3d_camera.at[2] = 0;

		M3d_camera.pos[0] = 0;
		M3d_camera.pos[1] = 1500;
		M3d_camera.pos[2] = 0;

		M3d_camera.up[0] = 0;
		M3d_camera.up[1] = 0;
		M3d_camera.up[2] = 1;
		M3d_camera.fov = 1.570795f/8.0f;
		
		float f = 1024 / 768;//((float)m_SizeDesktop.cy / (float)m_SizeDesktop.cx);
		float k = 1.0f;//0.6f;//ASPECT_RATIO_TABLE;//0.525f;

		M3d_camera.aspect = k * (0.75f/f)*0.28;

		M3d_transformview();


		// Back Up Light Settings //
		M3d_vtx lightPos_old[3];
		for(p=0;p<3;p++)
			{
			lightPos_old[p][0] = ((CGameAppView *)m_pParent)->m_environment->m_lightPos[p][0];
			lightPos_old[p][1] = ((CGameAppView *)m_pParent)->m_environment->m_lightPos[p][1];
			lightPos_old[p][2] = ((CGameAppView *)m_pParent)->m_environment->m_lightPos[p][2];
			}

		((CGameAppView *)m_pParent)->m_environment->LightPos(0,0,1200,0);
		
		if(!g_build.gameType == GAMETYPE_SNOOKER) 
			{
			((CGameAppView *)m_pParent)->m_environment->LightPos(1,900,900,0);
			((CGameAppView *)m_pParent)->m_environment->LightPos(2,0,900,900);
			}
		
		((CGameAppView *)m_pParent)->m_environment->Update();
		((CGameAppView *)m_pParent)->m_environment->Process();


		// render 3D Cue Model
		Mgfx_push();
		Mgfx_surf(g_spr3dSurface.surf);


		//CSnooker *pSnooker = ((CGameAppView *)m_pParent)->m_snooker;
		//M3d_zbuffer(1);
	
		switch(group_select)
			{
			case 0:	// Cues //
					Render3DCue();
					if(PURCHASED_CUE(m_shop_cue_idx)) Render3DPurchased();
					
					break;
			case 1:	// Text Colors //
					Render3DPen();
					if(PURCHASED_TEXT(m_shop_text_idx)) Render3DPurchased();
					break;
			}	
		
		// Restore Light Settings //

		((CGameAppView *)m_pParent)->m_environment->LightPos(0,lightPos_old[0][0],lightPos_old[0][1],lightPos_old[0][2]);
		((CGameAppView *)m_pParent)->m_environment->LightPos(1,lightPos_old[1][0],lightPos_old[1][1],lightPos_old[1][2]);
		((CGameAppView *)m_pParent)->m_environment->LightPos(2,lightPos_old[2][0],lightPos_old[2][1],lightPos_old[2][2]);
		((CGameAppView *)m_pParent)->m_environment->Update();
		((CGameAppView *)m_pParent)->m_environment->Process();

		HDC hdc;
		HRESULT retval;
		do {
		retval = lpddsx->GetDC(&hdc); // Get Device Context from DirectDraw-Surface1
		} while(retval==DDERR_WASSTILLDRAWING);
		if(retval==DD_OK)
		{	
			CDC *pDC = NULL;
			pDC = CDC::FromHandle(hdc);
			
		
			pdc->BitBlt(x,y,w,h, pDC, 0, 0, SRCCOPY);

			retval = lpddsx->ReleaseDC(hdc);
		}
	
		Mgfx_pop();
	

		ReleaseDC(pdc);
	}
*/
}



void CShopDlg::Render3DCue(void)
{
#ifndef MAIN_SERVER

	M3d_vtx pos;
	M3d_vtx scale;
	M3d_vtx rot;
	float angle2D = 0;
	int n;

		
	rot[0] = 0;//-8.5*FF2RAD;
	rot[1] = 0;
	rot[2] = 64*FF2RAD + ((float)m_delay*1)*FF2RAD;

	pos[0] = 0;
	pos[1] = 0;
	pos[2] = 0;

	scale[0] = 3.0f;
	scale[1] = 1.0f;
	scale[2] = 3.0f;
	

	D3DXMATRIX matView;
    D3DXMATRIX matWorld;
    D3DXMATRIX matRotation;
    D3DXMATRIX matTranslation;

	if( g_pDevice )
	{
		D3DXMATRIX matRotX, matRotY, matRotZ, matTrans, matScale;

		// Calculate Scale matrix
		D3DXMatrixScaling(&matScale, scale[0], scale[1], scale[2]);

		// Calculate rotation matrix
		D3DXMatrixRotationX( &matRotX, rot[0] );
		D3DXMatrixRotationY( &matRotY, rot[1] );
		D3DXMatrixRotationZ( &matRotZ, rot[2] );
		
		// Calculate a translation matrix
		D3DXMatrixTranslation(&matTrans, pos[0], pos[1], pos[2]);
	

		D3DXMATRIX matWorld = matScale*(matRotX*matRotY*matRotZ)*matTrans;
			
		D3DXMatrixTranslation(&matTrans, 0, 800, 0);
		matWorld = matTrans*matWorld;


		g_pDevice->SetTransform(D3DTS_WORLD, &matWorld);


		// Set the Cue to it's Default Normal Type, just incase //
		CSnooker *pSnooker = ((CGameAppView *)m_pParent)->m_snooker;
		pSnooker->m_M3d_Cue_ptr = &m_M3d_Cue[m_shop_cue_idx];

		for(n=0;n<pSnooker->m_M3d_Cue_ptr->mesh_n;n++)
		{
			pSnooker->m_M3d_Cue_ptr->mat[n].d3dm.Diffuse.a = 1.0f;
			pSnooker->m_M3d_Cue_ptr->mat[n].d3dm.Specular.a = 1.0f;
			pSnooker->m_M3d_Cue_ptr->mat[n].d3dm.Ambient.r = 0.0f;
			pSnooker->m_M3d_Cue_ptr->mat[n].d3dm.Ambient.g = 0.0f;
			pSnooker->m_M3d_Cue_ptr->mat[n].d3dm.Ambient.b = 0.0f;
			pSnooker->m_M3d_Cue_ptr->mat[n].d3dm.Ambient.a = 1.0f;
			pSnooker->m_M3d_Cue_ptr->mat[n].d3dm.Emissive.a = 1.0f;
			pSnooker->m_M3d_Cue_ptr->mat[n].d3dm.Power = D3D_MATERIAL_POWER;//10;
		}


		M3d_objectdraw(pSnooker->m_M3d_Cue_ptr);
		

		/*
		//M3d_transparancy(0);
		M3d_transformobject(pos, rot, scale); // For still (non keyframes) meshes.
		pos[0] = 0;
		pos[1] = 800;
		pos[2] = 0;
		M3d_transformlink(pos, I_rot, I_scale);

		lpd3dd->SetRenderState(D3DRENDERSTATE_NORMALIZENORMALS, TRUE);
		M3d_objectdraw(pSnooker->m_M3d_Cue_ptr);
		lpd3dd->SetRenderState(D3DRENDERSTATE_NORMALIZENORMALS, FALSE);
		//M3d_transparancy(1);
		*/
	}
	
#endif
}



int first=1;

float power = 0;

void CShopDlg::Render3DPen(void)
{

	
	M3d_vtx pos;
	M3d_vtx scale;
	M3d_vtx rot;
	float angle2D = 0;
	int n;
	int i;
		
	rot[0] = 64*FF2RAD;
	rot[1] = 0;
	rot[2] = ((float)m_delay*1)*FF2RAD;

	pos[0] = 0;
	pos[1] = 0;
	pos[2] = 0;
		

	scale[0] = 1.7f;
	scale[1] = 1.7f;
	scale[2] = 1.7f;
	
	
	D3DXMATRIX matView;
    D3DXMATRIX matWorld;
    D3DXMATRIX matRotation;
    D3DXMATRIX matTranslation;

	if( g_pDevice )
	{
		D3DXMATRIX matRotX, matRotY, matRotZ, matTrans, matScale;

		// Calculate Scale matrix
		D3DXMatrixScaling(&matScale, scale[0], scale[1], scale[2]);

		// Calculate rotation matrix
		D3DXMatrixRotationX( &matRotX, rot[0] );
		D3DXMatrixRotationY( &matRotY, rot[1] );
		D3DXMatrixRotationZ( &matRotZ, rot[2] );
		
		// Calculate a translation matrix
		D3DXMatrixTranslation(&matTrans, pos[0], pos[1], pos[2]);
		

		D3DXMATRIX matWorld = matScale*(matRotX*matRotY*matRotZ)*matTrans;
		
		
		//D3DXMatrixTranslation(&matTrans, pos[0], pos[1], pos[2]);
		//matWorld = matTrans*matWorld;
		
		g_pDevice->SetTransform(D3DTS_WORLD, &matWorld);

		// Set Up the Materials //
		for(n=0;n<m_M3d_Pen.mesh_n;n++)
		{
			m_M3d_Pen.mat[n].d3dm.Diffuse.r=1.0f;
			m_M3d_Pen.mat[n].d3dm.Diffuse.g=1.0f;
			m_M3d_Pen.mat[n].d3dm.Diffuse.b=1.0f;
			m_M3d_Pen.mat[n].d3dm.Diffuse.a=1.0f;

			m_M3d_Pen.mat[n].d3dm.Specular.r = 0.5f;
			m_M3d_Pen.mat[n].d3dm.Specular.g = 0.5f;
			m_M3d_Pen.mat[n].d3dm.Specular.b = 0.5f;
			m_M3d_Pen.mat[n].d3dm.Specular.a = 1.0f;

			m_M3d_Pen.mat[n].d3dm.Emissive.r = 0.0f;
			m_M3d_Pen.mat[n].d3dm.Emissive.g = 0.0f;
			m_M3d_Pen.mat[n].d3dm.Emissive.b = 0.0f;
			m_M3d_Pen.mat[n].d3dm.Emissive.a = 1.0f;

			m_M3d_Pen.mat[n].d3dm.Ambient.r = 0.0f;
			m_M3d_Pen.mat[n].d3dm.Ambient.g = 0.0f;
			m_M3d_Pen.mat[n].d3dm.Ambient.b = 0.0f;
			m_M3d_Pen.mat[n].d3dm.Ambient.a = 1.0f;

			m_M3d_Pen.mat[n].d3dm.Power = 30; //8*D3D_MATERIAL_POWER;
		}

		M3d_objectfindreset(&m_M3d_Pen);
		while((i = M3d_objectfind("body"))!=-1) // woodtop# & frameside#
		{

		// Set the Color //
		float r = (float)((color_lst[m_shop_text_idx]&0x0000FF))/255;
		float g = (float)((color_lst[m_shop_text_idx]&0x00FF00)>>8)/255;
		float b = (float)((color_lst[m_shop_text_idx]&0xFF0000)>>16)/255;

		m_M3d_Pen.mat[i].d3dm.Diffuse.r = r;
		m_M3d_Pen.mat[i].d3dm.Diffuse.g = g;
		m_M3d_Pen.mat[i].d3dm.Diffuse.b = b;
		m_M3d_Pen.mat[i].d3dm.Diffuse.a = 1.0;

		m_M3d_Pen.mat[i].d3dm.Specular.r = 0.0;
		m_M3d_Pen.mat[i].d3dm.Specular.g = 0.0;
		m_M3d_Pen.mat[i].d3dm.Specular.b = 0.0;
		m_M3d_Pen.mat[i].d3dm.Specular.a = 1.0f;
		m_M3d_Pen.mat[i].d3dm.Power = D3D_MATERIAL_POWER;
		}

		//M3d_transparancy(0);
		//M3d_transformobject(pos, rot, scale); // For still (non keyframes) meshes.
		M3d_objectdraw(&m_M3d_Pen);
		//M3d_transparancy(1);
	}

}


void CShopDlg::Render3DPurchased(void)
{
	/*
	((CGameAppView *)m_pParent)->m_environment->LightPos(0,sin(((float)m_delay*8)*FF2RAD)*500,cos(((float)m_delay*9)*FF2RAD)*500,cos(((float)m_delay*5)*FF2RAD)*500);
	((CGameAppView *)m_pParent)->m_environment->LightPos(1,sin(((float)m_delay*8)*FF2RAD+0.5)*500,cos(((float)m_delay*9)*FF2RAD+2.5)*500,cos(((float)m_delay*5+1.6)*FF2RAD)*500);
	((CGameAppView *)m_pParent)->m_environment->LightPos(2,sin(((float)m_delay*8)*FF2RAD+1.7)*500,cos(((float)m_delay*9)*FF2RAD+0.4)*500,cos(((float)m_delay*5+0.2)*FF2RAD)*400);
	((CGameAppView *)m_pParent)->m_environment->Update();
	((CGameAppView *)m_pParent)->m_environment->Process();
	*/

	M3d_vtx pos;
	M3d_vtx scale;
	M3d_vtx rot;
	float angle2D = 0;
	int n;
	int i;
	

	rot[0] = 64*FF2RAD;
	rot[1] = 0;
	rot[2] = ((float)m_delay*1)*FF2RAD;

	pos[0] = 0;
	pos[1] = 150;
	pos[2] = 0;
		
	scale[0] = 1.0;
	scale[1] = 1.0;
	scale[2] = 1.0;


	D3DXMATRIX matView;
    D3DXMATRIX matWorld;
    D3DXMATRIX matRotation;
    D3DXMATRIX matTranslation;

	if( g_pDevice )
	{
		D3DXMATRIX matRotX, matRotY, matRotZ, matTrans, matScale;

		// Calculate Scale matrix
		D3DXMatrixScaling(&matScale, scale[0], scale[1], scale[2]);

		// Calculate rotation matrix
		D3DXMatrixRotationX( &matRotX, rot[0] );
		D3DXMatrixRotationY( &matRotY, rot[1] );
		D3DXMatrixRotationZ( &matRotZ, rot[2] );
		
		// Calculate a translation matrix
		D3DXMatrixTranslation(&matTrans, pos[0], pos[1], pos[2]);
		

		D3DXMATRIX matWorld = matScale*(matRotX*matRotY*matRotZ)*matTrans;
		
		
		//D3DXMatrixTranslation(&matTrans, pos[0], pos[1], pos[2]);
		//matWorld = matTrans*matWorld;
		
		g_pDevice->SetTransform(D3DTS_WORLD, &matWorld);

	
		// Set Up the Materials //
		for(n=0;n<m_M3d_Purchased.mesh_n;n++)
		{
			m_M3d_Purchased.mat[n].d3dm.Diffuse.r = 1.0f;
			m_M3d_Purchased.mat[n].d3dm.Diffuse.g = 1.0f;
			m_M3d_Purchased.mat[n].d3dm.Diffuse.b = 1.0f;
			m_M3d_Purchased.mat[n].d3dm.Diffuse.a = 1.0f;

			m_M3d_Purchased.mat[n].d3dm.Specular.r = 0.0f;
			m_M3d_Purchased.mat[n].d3dm.Specular.g = 0.0f;
			m_M3d_Purchased.mat[n].d3dm.Specular.b = 0.0f;
			m_M3d_Purchased.mat[n].d3dm.Specular.a = 1.0f;

			m_M3d_Purchased.mat[n].d3dm.Emissive.r = 0.0f;
			m_M3d_Purchased.mat[n].d3dm.Emissive.g = 0.0f;
			m_M3d_Purchased.mat[n].d3dm.Emissive.b = 0.0f;
			m_M3d_Purchased.mat[n].d3dm.Emissive.a = 1.0f;

			m_M3d_Purchased.mat[n].d3dm.Ambient.r = 0.0f;
			m_M3d_Purchased.mat[n].d3dm.Ambient.g = 0.0f;
			m_M3d_Purchased.mat[n].d3dm.Ambient.b = 0.0f;
			m_M3d_Purchased.mat[n].d3dm.Ambient.a = 1.0f;

			//m_M3d_Purchased.mat[n].d3dm.Power = 8*D3D_MATERIAL_POWER;
		}

	
		//M3d_transparancy(1);
		//M3d_transformobject(pos, rot, scale); // For still (non keyframes) meshes.
		/*
		rot[0] = 64*FF2RAD;
		rot[1] = 64*FF2RAD;
		rot[2] = 0;

		pos[0] = 50;
		pos[1] = 0;
		pos[2] = 0;
		*/

		//M3d_transformlink(pos, rot, I_scale);
	

		M3d_objectdraw(&m_M3d_Purchased);
	}
	
}

//

void CShopDlg::OnTimer(UINT nIDEvent) 
{
	//return; 
	// TODO: Add your message handler code here and/or call default
	if(m_delay > 1)
	{
		Draw3D();
	}
	m_delay++;

	CDialog::OnTimer(nIDEvent);
}



void CShopDlg::OnButtonN() 
{
	// TODO: Add your control notification handler code here
	char *cue_lst[] = {"Standard", "BlackWhite", "Charcoal", "Sea", "Stealth", "Vampire", "Demon", "Wood"};	
	g_purchased = 0;

	switch (group_select)
		{
		case 0: // Cue Types //
				m_shop_cue_idx++;
				if(m_shop_cue_idx>7) m_shop_cue_idx=7;
				else{
					sprintf(g_action, "Browsing Cues: %s (%d/8)", cue_lst[m_shop_cue_idx], m_shop_cue_idx+1);
					m_changed = 1, Invalidate(); // causes an ondraw message
					}
				break;
		case 1: // Text Colors //
				m_shop_text_idx++;
				if(m_shop_text_idx>=TOTAL_TEXT_COLORS) m_shop_text_idx=(TOTAL_TEXT_COLORS-1);
				else{
					sprintf(g_action, "Browsing Text Colors: %s (%d/13)", tfmt_color_lst[m_shop_text_idx], m_shop_text_idx+1);
					m_changed = 1, Invalidate(); // causes an ondraw message
					}
				break;
		}
	
}

void CShopDlg::OnButtonP() 
{
	char *cue_lst[] = {"Standard", "BlackWhite", "Charcoal", "Sea", "Stealth", "Vampire", "Demon", "Wood"};
	g_purchased = 0;


	// TODO: Add your control notification handler code here
	switch (group_select)
		{
		case 0: // Cue Types //
				m_shop_cue_idx--;
				if(m_shop_cue_idx<0) m_shop_cue_idx=0;
				else{
					if(PURCHASED_CUE(m_shop_cue_idx)) sprintf(g_action, "Browsing Cues: %s (%d/8) PURCHASED", cue_lst[m_shop_cue_idx], m_shop_cue_idx+1);
						else sprintf(g_action, "Browsing Cues: %s (%d/8)", cue_lst[m_shop_cue_idx], m_shop_cue_idx+1);
					m_changed = 1, Invalidate(); // causes an ondraw message
					}
				break;
		case 1: // Text Colors //
				m_shop_text_idx--;
				if(m_shop_text_idx<0) m_shop_text_idx=0;
				else{ 
					if(PURCHASED_TEXT(m_shop_text_idx)) sprintf(g_action, "Browsing Text Colors: %s (%d/13) PURCHASED", tfmt_color_lst[m_shop_text_idx], m_shop_text_idx+1);
						else sprintf(g_action, "Browsing Text Colors: %s (%d/13)", tfmt_color_lst[m_shop_text_idx], m_shop_text_idx+1);
					m_changed = 1, Invalidate(); // causes an ondraw message
					}
				break;
		}
}

BOOL CShopDlg::OnEraseBkgnd(CDC* pDC) 
{
	// TODO: Add your message handler code here and/or call default
	return FALSE;

	return CDialog::OnEraseBkgnd(pDC);
}

void CShopDlg::OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized) 
{
	CDialog::OnActivate(nState, pWndOther, bMinimized);
	
	// TODO: Add your message handler code here
	if(nState != WA_INACTIVE)
		{
		//m_bPaintFirstTime = TRUE;
		}
	
}

void CShopDlg::OnEditchangeComboShop() 
{
	// TODO: Add your control notification handler code here
	
}


void CShopDlg::OnSelchangeComboShop() 
{
	// TODO: Add your control notification handler code here
	char *cue_lst[] = {"Standard", "BlackWhite", "Charcoal", "Sea", "Stealth", "Vampire", "Demon", "Wood"};
	group_select = m_combo_ctrl.GetCurSel();
	if(group_select==0) sprintf(g_action, "Browsing Cues: %s (%d/8)", cue_lst[m_shop_cue_idx], m_shop_cue_idx+1);
		else sprintf(g_action, "Browsing Text Colors: %s (%d/13)", tfmt_color_lst[m_shop_text_idx], m_shop_text_idx+1);
	m_changed = 1;
	Invalidate();
}



void CShopDlg::OnButtonB1() 
{
	// TODO: Add your control notification handler code here
	char *cue_lst[] = {"Standard", "BlackWhite", "Charcoal", "Sea", "Stealth", "Vampire", "Demon", "Wood"};

	// Check If we are Selected on a Cue or Text //
	int idx;
	char msg[256];
	// Cues // 
	if(group_select==0)
		{
		if(PURCHASED_CUE(m_shop_cue_idx)) sprintf(g_action, "You already have this Cue"), g_purchased = -1;
			else{
				if(m_player[g_gameAppView->m_playerIdx].cash>=1500)
					{
					sprintf(msg, "/buy %s", cue_lst[m_shop_cue_idx]);
					sprintf(g_action, "Purchased %s for $15.00 Virtual Cash", cue_lst[m_shop_cue_idx]);
					mainShell->ChatSend(msg);
					g_purchased = 1;
					// Update Local Variable //
					m_player[g_gameAppView->m_playerIdx].inventory |= (1<<(16+m_shop_cue_idx));
					m_player[g_gameAppView->m_playerIdx].cash-=1500;

					// Update m_personalDetailsDlg //
					char buf[256];
					sprintf(buf, "Standard"); 
					if(m_player[((CGameAppView *)m_pParent)->m_playerIdx].inventory&(1<<17)) strcat(buf, "\r\nBlackWhite");
					if(m_player[((CGameAppView *)m_pParent)->m_playerIdx].inventory&(1<<18)) strcat(buf, "\r\nCharcoal");
					if(m_player[((CGameAppView *)m_pParent)->m_playerIdx].inventory&(1<<19)) strcat(buf, "\r\nSea");
					if(m_player[((CGameAppView *)m_pParent)->m_playerIdx].inventory&(1<<20)) strcat(buf, "\r\nStealth");
					if(m_player[((CGameAppView *)m_pParent)->m_playerIdx].inventory&(1<<21)) strcat(buf, "\r\nVampire");
					if(m_player[((CGameAppView *)m_pParent)->m_playerIdx].inventory&(1<<22)) strcat(buf, "\r\nDemon");
					if(m_player[((CGameAppView *)m_pParent)->m_playerIdx].inventory&(1<<23)) strcat(buf, "\r\nWood");
					((CGameAppView *)m_pParent)->m_personalDetailsDlg.m_cuetype = _T(buf);	
					((CGameAppView *)m_pParent)->m_personalDetailsDlg.UpdateData(FALSE);

					// Update m_statsDlg //
					if(g_build.gameType == GAMETYPE_SNOOKER) 
						{
						if(m_player[g_gameAppView->m_playerIdx].cash>=0) sprintf(buf, "$%ld.%02ld",m_player[g_gameAppView->m_playerIdx].cash/100, (m_player[g_gameAppView->m_playerIdx].cash)%100);
							else sprintf(buf, "$-%ld.%02ld",-(m_player[g_gameAppView->m_playerIdx].cash)/100, (-m_player[g_gameAppView->m_playerIdx].cash)%100);
						((CGameAppView *)m_pParent)->m_statsDlg.m_cash = _T(buf);
						((CGameAppView *)m_pParent)->m_statsDlg.UpdateData(FALSE);
						}
					// Update m_statsDlg2 //
					else{
						if(m_player[g_gameAppView->m_playerIdx].cash>=0) sprintf(buf, "$%ld.%02ld",m_player[g_gameAppView->m_playerIdx].cash/100, (m_player[g_gameAppView->m_playerIdx].cash)%100);
							else sprintf(buf, "$-%ld.%02ld",-(m_player[g_gameAppView->m_playerIdx].cash)/100, (-m_player[g_gameAppView->m_playerIdx].cash)%100);
						((CGameAppView *)m_pParent)->m_statsDlg2.m_cash = _T(buf);
						((CGameAppView *)m_pParent)->m_statsDlg2.UpdateData(FALSE);
						}
					
					}
				else{
					sprintf(g_action, "Sorry, you must have at least $15.00 for this cue"), g_purchased = -1;
					}
				}
		m_changed = 1;
		Invalidate(); // causes an ondraw message
		}	
	else if(group_select==1)
		{
		if(PURCHASED_TEXT(m_shop_text_idx)) sprintf(g_action, "You already have this Text color"), g_purchased = -1;
			else{
				if(m_player[g_gameAppView->m_playerIdx].cash>=1000)
					{
					sprintf(msg, "/buy %s", tfmt_color_lst[m_shop_text_idx]);
					sprintf(g_action, "Purchased %s for $10.00 Virtual Cash", tfmt_color_lst[m_shop_text_idx]);
					mainShell->ChatSend(msg);
					g_purchased = 1;
					// Update Local Variable //
					m_player[g_gameAppView->m_playerIdx].inventory |= (1<<(m_shop_text_idx-1));
					m_player[g_gameAppView->m_playerIdx].cash-=1000;

					// Update m_personalDetailsDlg //
					char buf[256];
					sprintf(buf, "Normal");
					if(m_player[((CGameAppView *)m_pParent)->m_playerIdx].inventory&1) strcat(buf, "\r\nBlue");
					if(m_player[((CGameAppView *)m_pParent)->m_playerIdx].inventory&2) strcat(buf, "\r\nPeach");
					if(m_player[((CGameAppView *)m_pParent)->m_playerIdx].inventory&4) strcat(buf, "\r\nPink");
					if(m_player[((CGameAppView *)m_pParent)->m_playerIdx].inventory&8) strcat(buf, "\r\nMud");
					if(m_player[((CGameAppView *)m_pParent)->m_playerIdx].inventory&16) strcat(buf, "\r\nRed");
					if(m_player[((CGameAppView *)m_pParent)->m_playerIdx].inventory&32) strcat(buf, "\r\nYellow");
					if(m_player[((CGameAppView *)m_pParent)->m_playerIdx].inventory&64) strcat(buf, "\r\nLtBlue");
					if(m_player[((CGameAppView *)m_pParent)->m_playerIdx].inventory&128) strcat(buf, "\r\nGreen");
					if(m_player[((CGameAppView *)m_pParent)->m_playerIdx].inventory&256) strcat(buf, "\r\nTan");
					if(m_player[((CGameAppView *)m_pParent)->m_playerIdx].inventory&512) strcat(buf, "\r\nJade");
					if(m_player[((CGameAppView *)m_pParent)->m_playerIdx].inventory&1024) strcat(buf, "\r\nRose");
					if(m_player[((CGameAppView *)m_pParent)->m_playerIdx].inventory&2048) strcat(buf, "\r\nSoftGreen");
					((CGameAppView *)m_pParent)->m_personalDetailsDlg.m_inventory = _T(buf);
					((CGameAppView *)m_pParent)->m_personalDetailsDlg.UpdateData(FALSE);

					// Update m_statsDlg //
					if(g_build.gameType == GAMETYPE_SNOOKER) 
						{
						if(m_player[g_gameAppView->m_playerIdx].cash>=0) sprintf(buf, "$%ld.%02ld",m_player[g_gameAppView->m_playerIdx].cash/100, (m_player[g_gameAppView->m_playerIdx].cash)%100);
							else sprintf(buf, "$-%ld.%02ld",-(m_player[g_gameAppView->m_playerIdx].cash)/100, (-m_player[g_gameAppView->m_playerIdx].cash)%100);
						((CGameAppView *)m_pParent)->m_statsDlg.m_cash = _T(buf);
						((CGameAppView *)m_pParent)->m_statsDlg.UpdateData(FALSE);
						}
					// Update m_statsDlg2 //
					else{
						if(m_player[g_gameAppView->m_playerIdx].cash>=0) sprintf(buf, "$%ld.%02ld",m_player[g_gameAppView->m_playerIdx].cash/100, (m_player[g_gameAppView->m_playerIdx].cash)%100);
							else sprintf(buf, "$-%ld.%02ld",-(m_player[g_gameAppView->m_playerIdx].cash)/100, (-m_player[g_gameAppView->m_playerIdx].cash)%100);
						((CGameAppView *)m_pParent)->m_statsDlg2.m_cash = _T(buf);
						((CGameAppView *)m_pParent)->m_statsDlg2.UpdateData(FALSE);
						}

					}
				else{
					sprintf(g_action, "Sorry, you must have at least $10.00 for this color"), g_purchased = -1;
					}	
				}
				
		m_changed = 1;
		Invalidate(); // causes an ondraw message
		}	

}

void CShopDlg::OnShowWindow(BOOL bShow, UINT nStatus) 
{
	CDialog::OnShowWindow(bShow, nStatus);
	
	// TODO: Add your message handler code here
	Invalidate(); // causes an ondraw message
	//m_bPaintFirstTime = 1;
	
}

void CShopDlg::OnCaptureChanged(CWnd *pWnd) 
{
	// TODO: Add your message handler code here
	CDialog::OnCaptureChanged(pWnd);



}

void CShopDlg::OnButtonUse() 
{
	// TODO: Add your control notification handler code here
	char *cue_lst[] = {"Standard", "BlackWhite", "Charcoal", "Sea", "Stealth", "Vampire", "Demon", "Wood"};

	// Check If we are Selected on a Cue or Text //
	int idx;
	char msg[256];
	// Cues // 
	if(group_select==0)
		{
		if(PURCHASED_CUE(m_shop_cue_idx)) 
			{
			sprintf(msg, "/cue %s", cue_lst[m_shop_cue_idx]);
			sprintf(g_action, "Using %s cue", cue_lst[m_shop_cue_idx]);
			mainShell->ChatSend(msg);
			g_purchased = 1;
			g_selected_cue = m_shop_cue_idx;
			// Update m_personalDetailsDlg //
			char buf[256];
			sprintf(buf, "Standard"); if(g_selected_cue==0) strcat(buf, " (S)"); 
			if(m_player[((CGameAppView *)m_pParent)->m_playerIdx].inventory&(1<<17)) { strcat(buf, "\r\nBlackWhite"); if(g_selected_cue==1) strcat(buf, " (S)"); }
			if(m_player[((CGameAppView *)m_pParent)->m_playerIdx].inventory&(1<<18)) { strcat(buf, "\r\nCharcoal"); if(g_selected_cue==2) strcat(buf, " (S)"); } 
			if(m_player[((CGameAppView *)m_pParent)->m_playerIdx].inventory&(1<<19)) { strcat(buf, "\r\nSea"); if(g_selected_cue==3) strcat(buf, " (S)"); }
			if(m_player[((CGameAppView *)m_pParent)->m_playerIdx].inventory&(1<<20)) { strcat(buf, "\r\nStealth"); if(g_selected_cue==4) strcat(buf, " (S)"); }
			if(m_player[((CGameAppView *)m_pParent)->m_playerIdx].inventory&(1<<21)) { strcat(buf, "\r\nVampire"); if(g_selected_cue==5) strcat(buf, " (S)"); }
			if(m_player[((CGameAppView *)m_pParent)->m_playerIdx].inventory&(1<<22)) { strcat(buf, "\r\nDemon"); if(g_selected_cue==6) strcat(buf, " (S)"); }
			if(m_player[((CGameAppView *)m_pParent)->m_playerIdx].inventory&(1<<23)) { strcat(buf, "\r\nWood"); if(g_selected_cue==7) strcat(buf, " (S)"); }
			((CGameAppView *)m_pParent)->m_personalDetailsDlg.m_cuetype = _T(buf);	
			((CGameAppView *)m_pParent)->m_personalDetailsDlg.UpdateData(FALSE);
			}
		else{
			sprintf(g_action, "You don't have this Cue"), g_purchased = -1;
			}
				
		m_changed = 1;
		Invalidate(); // causes an ondraw message
		}		
	else if(group_select==1)
		{
		if(PURCHASED_TEXT(m_shop_text_idx)) 
			{
			sprintf(msg, "/text %s", tfmt_color_lst[m_shop_text_idx]);
			sprintf(g_action, "Using %s text", tfmt_color_lst[m_shop_text_idx]);
			mainShell->ChatSend(msg);
			g_purchased = 1;
			g_selected_text = m_shop_text_idx;
			// Update m_personalDetailsDlg //

			
			char buf[256];
			sprintf(buf, "Normal"); if(g_selected_text==0) strcat(buf, " (S)");
			if(m_player[((CGameAppView *)m_pParent)->m_playerIdx].inventory&1) { strcat(buf, "\r\nBlue"); if(g_selected_text==1) strcat(buf, " (S)"); }
			if(m_player[((CGameAppView *)m_pParent)->m_playerIdx].inventory&2) { strcat(buf, "\r\nPeach"); if(g_selected_text==2) strcat(buf, " (S)"); }
			if(m_player[((CGameAppView *)m_pParent)->m_playerIdx].inventory&4) { strcat(buf, "\r\nPink"); if(g_selected_text==3) strcat(buf, " (S)"); }
			if(m_player[((CGameAppView *)m_pParent)->m_playerIdx].inventory&8) { strcat(buf, "\r\nMud"); if(g_selected_text==4) strcat(buf, " (S)"); }
			if(m_player[((CGameAppView *)m_pParent)->m_playerIdx].inventory&16) { strcat(buf, "\r\nRed"); if(g_selected_text==5) strcat(buf, " (S)"); }
			if(m_player[((CGameAppView *)m_pParent)->m_playerIdx].inventory&32) { strcat(buf, "\r\nYellow"); if(g_selected_text==6) strcat(buf, " (S)"); }
			if(m_player[((CGameAppView *)m_pParent)->m_playerIdx].inventory&64) { strcat(buf, "\r\nLtBlue"); if(g_selected_text==7) strcat(buf, " (S)"); }
			if(m_player[((CGameAppView *)m_pParent)->m_playerIdx].inventory&128) { strcat(buf, "\r\nGreen"); if(g_selected_text==8) strcat(buf, " (S)"); }
			if(m_player[((CGameAppView *)m_pParent)->m_playerIdx].inventory&256) { strcat(buf, "\r\nTan"); if(g_selected_text==9) strcat(buf, " (S)"); }
			if(m_player[((CGameAppView *)m_pParent)->m_playerIdx].inventory&512) { strcat(buf, "\r\nJade"); if(g_selected_text==10) strcat(buf, " (S)"); }
			if(m_player[((CGameAppView *)m_pParent)->m_playerIdx].inventory&1024) { strcat(buf, "\r\nRose"); if(g_selected_text==11) strcat(buf, " (S)"); }
			if(m_player[((CGameAppView *)m_pParent)->m_playerIdx].inventory&2048) { strcat(buf, "\r\nSoftGreen"); if(g_selected_text==12) strcat(buf, " (S)"); }
			((CGameAppView *)m_pParent)->m_personalDetailsDlg.m_inventory = _T(buf);
			((CGameAppView *)m_pParent)->m_personalDetailsDlg.UpdateData(FALSE);
			}
		else{
			sprintf(g_action, "You don't have this Text color"), g_purchased = -1;
			}
				
		m_changed = 1;
		Invalidate(); // causes an ondraw message
		}	
}
