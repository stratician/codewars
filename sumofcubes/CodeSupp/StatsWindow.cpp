


#include "stdafx.h"
#include "GameAppView.h"
#include "StatsWindow.h"
#include "C:\Program Files (x86)\Microsoft DirectX SDK (June 2010)\Include\d3dx9.h"


CStatsWindow::CStatsWindow()
{
	m_pParent = NULL;
	m_pSwapChain = NULL;
	pRenderSurface = NULL;
	m_hWnd = NULL;

	// Mouse Button States
	m_bLButtonDown = 0;
	m_bLButtonJustUp = 0;

	m_bRButtonDown = 0;
	m_bRButtonJustUp = 0;

	m_newx = 0;
	m_newy = 0;
	m_oldx = 0;
	m_oldy = 0;

	m_dragWindow = false;
	
}

CStatsWindow::~CStatsWindow()
{
	if (sprite)
	{
		sprite->Release();
		sprite = 0;
	}

	if (tex)
	{
		tex->Release();
		tex = 0;
	}


	// Release Font
	
	if (g_Font != NULL)
	{
		g_Font->Release();
		g_Font = NULL;
	}
}


void CStatsWindow::InitD2D(HWND hWnd, int rWidth, int rHeight)
{

}

void CStatsWindow::InitD3D(HWND hWnd, int rx, int ry, int rWidth, int rHeight)
{

	_bFirstTime = true;

	m_hWnd = hWnd;

	
	m_xpos = rx;
	m_ypos = ry;
	m_width = rWidth;
	m_height = rHeight;


	HRESULT hr;

	// Create our Swap Chain, so we can render to our Window's surface

	D3DPRESENT_PARAMETERS d3dpp;
	d3dpp.BackBufferWidth = rWidth;
	d3dpp.BackBufferHeight = rHeight;
	d3dpp.BackBufferFormat = D3DFMT_A8R8G8B8; // D3DFMT_UNKNOWN; //D3DFMT_A8R8G8B8;
	d3dpp.BackBufferCount = 1;
	d3dpp.MultiSampleType = D3DMULTISAMPLE_NONE;
	d3dpp.MultiSampleQuality = 0;
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
	d3dpp.hDeviceWindow = m_hWnd;
	d3dpp.Windowed = true;
	d3dpp.EnableAutoDepthStencil = true;
	d3dpp.AutoDepthStencilFormat = D3DFMT_D24S8;	// Depth Format
	d3dpp.Flags = 0;//D3DPRESENTFLAG_DISCARD_DEPTHSTENCIL;
	d3dpp.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;
	d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;

	hr = g_pDevice->CreateAdditionalSwapChain(&d3dpp, &m_pSwapChain);


	// Create Render Surface
	D3DSURFACE_DESC desc;
	

	// hr = g_pDevice->GetRenderTarget(0, &pRenderSurface);

	hr = g_pDevice->CreateRenderTarget(rWidth, rHeight, D3DFMT_A8R8G8B8, D3DMULTISAMPLE_NONE, 0, false, &pRenderSurface, NULL);



	// Set up Sprite
	m_Sprite = new Sprite3D();
	bool rval = m_Sprite->loadSprite(g_pDevice, "Graphics/images/chalks/Chalk_Blue.png");

	hr = D3DXCreateTextureFromFile(g_pDevice, "Graphics/images/chalks/Chalk_Blue.png", &texture);

	
	g_pDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
	g_pDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);

	g_pFont1 = new Font(g_pDevice);
	g_pFont1->Load(1280, 160, 10, "Graphics/HUD/font4.png", "ABCDEFGHIJKLMNOPQRSTUVWXYZ,.<>/\\?;:1234567890!@#$%^&*()-_=+[]{}°abcdefghijklmnopqrstuvwxyz");
	// _tSpr = NewSprite(g_pDevice, "Graphics/images/chalks/Chalk_Blue.png");

	position.x = 0;
	position.y = 0;
	position.z = 0;

	color = D3DCOLOR_ARGB(255, 255, 255, 255);

	initialised = false;

	// Init Sprite
	Initialize(g_pDevice, "Graphics/images/ladder/bg1.png", 600, 400);

	// Init Font
	InitFont(g_pDevice, rWidth, rHeight);
}


void CStatsWindow::InitFont(LPDIRECT3DDEVICE9 device, int sWidth, int sHeight)
{
	// Create our DirectX Font
	D3DXFONT_DESC FontDesc = { 24,
		0,
		400,
		0,
		false,
		DEFAULT_CHARSET,
		OUT_TT_PRECIS,
		CLIP_DEFAULT_PRECIS,
		DEFAULT_PITCH,
		"Arial" };
	
	m_FontPosition.top = 0;
	m_FontPosition.left = 0;
	m_FontPosition.right = sWidth;
	m_FontPosition.bottom = sHeight;

	//create font
	D3DXCreateFontIndirect(device, &FontDesc, &g_Font);
}


bool CStatsWindow::Initialize(LPDIRECT3DDEVICE9 device, std::string file, int width, int height)
{
	HRESULT hr = D3DXCreateTextureFromFileEx(device, file.c_str(), width, height, D3DX_DEFAULT, 0, D3DFMT_UNKNOWN, D3DPOOL_MANAGED, D3DX_DEFAULT, D3DX_DEFAULT, 0, NULL, NULL, &tex);
	
	if (!SUCCEEDED(hr))
	{
		std::string s = "There was an issue creating the texture. Make sure the requested image is available. Requested image: " + file;
		::MessageBox(NULL, s.c_str(), NULL, NULL);
		return false;
			
	}

	// Attempt to create the sprite
	if (!SUCCEEDED(D3DXCreateSprite(device, &sprite)))
	{
		::MessageBox(NULL, "There was an issue creating the sprite.", NULL, NULL);
		return false;
	}

	initialised = true;

	return true;
}


bool CStatsWindow::IsInitialised()
{
	return initialised;
}

void CStatsWindow::Draw()
{
	if (sprite && tex)
	{
		sprite->Begin(D3DXSPRITE_ALPHABLEND);
		sprite->Draw(tex, NULL, NULL, &position, color);
		sprite->End();
	}
}


void CStatsWindow::Render()
{
	if (!m_hWnd) return;

	Render2D();

	// Render3D();


	

	if (_bFirstTime)
	{
		_bFirstTime = false;

		if (::IsWindowVisible(m_hWnd) == false)
		{
			::ShowWindow(m_hWnd, SW_SHOW);
		}
	}
}


void CStatsWindow::Render2D()
{
#ifndef MAIN_SERVER

	// Check that our Window is visible before bothering with rendering
	if (!m_hWnd) return;
	else
	{
		if (::IsWindowVisible(m_hWnd) == false) return;
	}

	if (!g_pDevice) return;
	//
	// Set the projection matrix.
	//


	CRect clientrect;
	CRect srect;
	CRect drect;

	HRESULT hr;



	::GetWindowRect(m_hWnd, clientrect);


	hr = g_pDevice->SetRenderTarget(0, pRenderSurface);
	hr = m_pSwapChain->GetBackBuffer(0, D3DBACKBUFFER_TYPE_MONO, &pRenderSurface);

	// Now we can clear just view-port's portion of the buffer to red...
	hr = g_pDevice->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER,
		D3DCOLOR_COLORVALUE(0.0f, 0.2f, 0.2f, 0.3f), 1.0f, 0L);

	g_pDevice->BeginScene();
	
	Draw();
	DrawFont();
	g_pDevice->EndScene();

	m_pSwapChain->Present(NULL, NULL, m_hWnd, NULL, NULL);

#endif

}


void CStatsWindow::DrawFont()
{
	m_FontPosition.top = 0;         //position
	g_Font->DrawText(NULL,
		"Direct3D Tutorial 09: Text With D3DXFont",
		-1,
		&m_FontPosition,
		DT_CENTER,
		0xffffffff); //draw text

	m_FontPosition.top = 100;       //position
	g_Font->DrawText(NULL,
		"Text Sample Using D3DXFont",
		-1,
		&m_FontPosition,
		DT_CENTER,
		0xffff0000); //draw text
}


void CStatsWindow::Render3D()
{
#ifndef MAIN_SERVER

	// Check that our Window is visible before bothering with rendering
	if (!m_hWnd) return;
	else
	{
		if (::IsWindowVisible(m_hWnd) == false) return;
	}

	if (!g_pDevice) return;
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



	hr = g_pDevice->SetRenderTarget(0, pRenderSurface);
	hr = m_pSwapChain->GetBackBuffer(0, D3DBACKBUFFER_TYPE_MONO, &pRenderSurface);


	D3DVIEWPORT9 mainViewPort;

	
	mainViewPort.X = 0;
	mainViewPort.Y = 0;
	mainViewPort.Width = clientrect.Width();
	mainViewPort.Height = clientrect.Height();
	mainViewPort.MinZ = 0.0f;
	mainViewPort.MaxZ = 1.0f;


	float aspect = 1.0f;//(float)mainViewPort.Width / (float)mainViewPort.Height;
	// aspect = (float)w / (float)h;

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
	srect.right = mainViewPort.Width;
	srect.bottom = mainViewPort.Height;

	drect.left = 0;
	drect.top = 0;
	drect.right = clientrect.Width();
	drect.bottom = clientrect.Height();


	M3d_vtx pos;
	M3d_vtx at;
	M3d_vtx up;


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

	g_pCamera->_cameraFov = 1.570795f / 4.0f;

	/*
	// Position and aim the camera //
	D3DXMATRIX V;
	D3DXMatrixLookAtLH(&V, &g_pCamera->_pos, &g_pCamera->_look, &g_pCamera->_up);
	g_pDevice->SetTransform(D3DTS_VIEW, &V);


	g_gameAppView->m_snooker->CameraPosAtUpFovAspect(pos, at, up, g_pCamera->_cameraFov, g_pCamera->_cameraAspect);
	*/


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

	g_pDevice->SetViewport(&mainViewPort);

	// Now we can clear just view-port's portion of the buffer to red...
	hr = g_pDevice->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER,
		D3DCOLOR_COLORVALUE(0.0f, 0.2f, 0.2f, 1.0f), 1.0f, 0L);

	g_pDevice->BeginScene();


	CVector sfocus;
	CVector spos;

	sfocus.x = 0.0f; 
	sfocus.y = 0.0f; 
	sfocus.z = 0.0f;

	spos.x = 0.0f; 
	spos.y = 0.0f; 
	spos.z = 0.0f;


	// Set up 2D Projection for Main 3D ViewPort (Radio 2:1), so we'll use 1600 x 800 resolution for 2D //
	g_pDevice->SetRenderState(D3DRS_LIGHTING, false);
	M3d_transformview2d();


	// Draw Title
	g_pFont1->Render("Ladder Points", 600, 100, 0.0f, 1.0f, 1.0f, 1.0f, MGFX_FNTDRAW_CENTER);

	Draw();

	char buffer[128];
	DWORD lcol;
	DWORD rcol;

	// Background //
	lcol = D3DCOLOR_RGBA(4, 4, 4, 128);
	rcol = D3DCOLOR_RGBA(32, 32, 32, 128);

	M3d_objectset_LVERTEX(&M3d_platedraw_PLATE, 3, 0, 0, 0, lcol, lcol, 0, 1);
	M3d_objectset_LVERTEX(&M3d_platedraw_PLATE, 2, 1, 0, 0, rcol, rcol, 1, 1);
	M3d_objectset_LVERTEX(&M3d_platedraw_PLATE, 1, 1, 1, 0, rcol, rcol, 1, 0);
	M3d_objectset_LVERTEX(&M3d_platedraw_PLATE, 0, 0, 1, 0, lcol, lcol, 0, 0);




	// Draw Level Bar Background
	M2d_platedraw(0, 0, 800, 200);

	static float xorig = 700.0f;
	static float yorig = 500.0f;
	static float counter = 0.0f;
	counter += 0.005f;

	if (texture)
	{
		M2d_sprdrawScale(texture, xorig + sinf(counter)*400.0f, yorig + cosf(counter)*400.0f, 400, 400, 0, 1.0f);
	}
	


	g_pDevice->SetTexture(0, NULL);

	g_pDevice->SetRenderState(D3DRS_ZENABLE, TRUE);
	g_pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
	g_pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);


	g_pDevice->EndScene();

	m_pSwapChain->Present(srect, drect, m_hWnd, NULL, NULL);

#endif

}


RECT MainRect;
POINT _point;
POINT _curpoint;


void CStatsWindow::MOUSEMOVE(UINT Msg, WPARAM wParam, LPARAM lParam)
{
	// TODO: Add your message handler code here and/or call default


#ifndef MAIN_SERVER	

	CPoint point;
	point.x = LOWORD(lParam);
	point.y = HIWORD(lParam);
	
	// If Left Mouse Button is down, then drag the Window



	GetCursorPos(&_curpoint);
	if (wParam == MK_LBUTTON)
	{
		::MoveWindow(m_hWnd, _curpoint.x - _point.x, _curpoint.y - _point.y,
			MainRect.right - MainRect.left, MainRect.bottom - MainRect.top,
			TRUE);
	}


#endif //MAIN_SERVER


}




void CStatsWindow::LBUTTONDOWN(UINT Msg, WPARAM wParam, LPARAM lParam)
{
	// TODO: Add your message handler code here and/or call default

	
#ifndef MAIN_SERVER	

	CPoint point;
	point.x = LOWORD(lParam);
	point.y = HIWORD(lParam);


	
	if(!m_bLButtonDown)	m_bLButtonDown = 1;
	
	m_bLButtonJustUp = 0;

	m_dragWindow = true;
	
	::SetCapture(m_hWnd);
	::GetWindowRect(m_hWnd, &MainRect);
	//save current cursor coordinate
	::GetCursorPos(&_point);
	::ScreenToClient(m_hWnd, &_point);


	// Store our Click Point.
	m_oldx = point.x;
	m_oldy = point.y;
	
#endif //!MAIN_SERVER


}

void CStatsWindow::LBUTTONUP(UINT Msg, WPARAM wParam, LPARAM lParam)
{
	// TODO: Add your message handler code here and/or call default

#ifndef MAIN_SERVER	

	CPoint point;
	point.x = LOWORD(lParam);
	point.y = HIWORD(lParam);

	
	if(m_bLButtonDown) m_bLButtonJustUp = 1;
	m_bLButtonDown = 0;
	m_bRButtonJustUp = 0;
	
	m_dragWindow = false;
	::ReleaseCapture();
#endif //!MAIN_SERVER


}




void CStatsWindow::RBUTTONDOWN(UINT Msg, WPARAM wParam, LPARAM lParam)
{
	// TODO: Add your message handler code here and/or call default

#ifndef MAIN_SERVER	

	CPoint point;
	point.x = LOWORD(lParam);
	point.y = HIWORD(lParam);

	if(!m_bRButtonDown)	m_bRButtonDown = 1;

#endif //!MAIN_SERVER
	

}



void CStatsWindow::RBUTTONUP(UINT Msg, WPARAM wParam, LPARAM lParam)
{
	// TODO: Add your message handler code here and/or call default

#ifndef MAIN_SERVER	

	CPoint point;
	point.x = LOWORD(lParam);
	point.y = HIWORD(lParam);
	

	
	m_bRButtonDown = 0;
	
	if(m_bLButtonDown) m_bRButtonJustUp = 0;
	else m_bRButtonJustUp = 1;

#endif //!MAIN_SERVER


}



