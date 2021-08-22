
#include "../CodeSupp/build.h"
#include "../CodeSupp/stdafx.h"
#include "../CodeSupp/GameAppView.h"	
#include "../Codedx9/font.h"

#include "WindowBase.h"


#include "acgfx_graphics.h"
#include "acutil_log.h"
#include "acgfx_font.h"
#include "acgfx_camera.h"
#include "acgfx_dynrender.h"
#include "acutil_timer.h"
#include "acutil_unicode.h"



// #include "../CodeSupp/stdafx.h"
// #include "../CodeSupp/GameAppView.h"


CWindowBase::CWindowBase()
{
}


CWindowBase::~CWindowBase()
{
	/*
	if (pSurface)
	{
		pSurface->Release();
	}
	*/
	m_pDevice->Release();
	if (hWnd) DestroyWindow(hWnd);
}




int CWindowBase::Create(HINSTANCE hInstance, HWND parentWnd, char *title, int x, int y, int w, int h)
{

	// Create a windows dialog (basic)

	this->x = x;
	this->y = y;
	this->width = w;
	this->height = h;

	return 1;
}




void CWindowBase::OnClose()
{
	// Window is closing
}


void CWindowBase::Destroy()
{
	
}


bool CWindowBase::InitD3D()
{
	Mmain_log("WinBase Init[1.0]");
	_bFirstTime = true;

	



	HRESULT hr;


	
	// Acquire an IDirect3D9 Interface //
	//_pd3d9 = Direct3DCreate9(D3D_SDK_VERSION);



	Mmain_log("Init WindowBase [1.1]");

	
	// Check for Hardware Vertex Processing //
	D3DCAPS9 caps;
	g_pd3d9->GetDeviceCaps(
		D3DADAPTER_DEFAULT, // Denotes primary display adapter
		D3DDEVTYPE_HAL,
		&caps);


	



	



	Mmain_log("WinBase Init[1.2]");

	
	D3DPRESENT_PARAMETERS d3dpp;
	ZeroMemory(&d3dpp, sizeof(D3DPRESENT_PARAMETERS));
	// d3dpp.BackBufferWidth = this->width;
	// d3dpp.BackBufferHeight = this->height;
	d3dpp.BackBufferFormat = D3DFMT_A8R8G8B8; // D3DFMT_UNKNOWN; //D3DFMT_A8R8G8B8;
	d3dpp.BackBufferCount = 1;
	d3dpp.MultiSampleType = D3DMULTISAMPLE_NONE;
	d3dpp.MultiSampleQuality = 0;
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
	d3dpp.hDeviceWindow = hWnd;
	d3dpp.Windowed = true;
	d3dpp.EnableAutoDepthStencil = false;
	d3dpp.AutoDepthStencilFormat = D3DFMT_D24S8;	// Depth Format
	d3dpp.Flags = 0;//D3DPRESENTFLAG_DISCARD_DEPTHSTENCIL;
	d3dpp.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;
	d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;

	

	// Can we use Hardware Processing //
	int vp = 0;
	if (caps.DevCaps & D3DDEVCAPS_HWTRANSFORMANDLIGHT)
	{
		// Hardware Vertex Processing supported //
		vp = D3DCREATE_HARDWARE_VERTEXPROCESSING;
	}
	else
	{
		// Only Soft3ware Vertex Processing supported //
		vp = D3DCREATE_SOFTWARE_VERTEXPROCESSING;
	}
	Mmain_log("WinBase Init[1.3]");

	// Create IDirect3DDevice9 Interface //
	m_pDevice = 0;
	hr = g_pd3d9->CreateDevice(
		D3DADAPTER_DEFAULT,		// Primary adapter
		D3DDEVTYPE_HAL,			// Device Type
		hWnd,					// Window associated with device
		vp,						// Vertex Processing Type (found from DEVCAPS)
		&d3dpp,					// Present Params
		&m_pDevice);			// returned created device

								// Create our Swap Chain, so we can render to our Window's surface

	if(hr == S_OK)	Mmain_log("WinBase Init[1.3.1] - S_OK");
		else Mmain_log("WinBase Init[1.3.1] - S_ERROR");

	// Create Render Surface



	// hr = m_pDevice->GetRenderTarget(0, &pSurface);
	/*
	Mmain_log("WinBase Init[1.4]");
	D3DSURFACE_DESC desc;
	pSurface->GetDesc(&desc);

	hr = m_pDevice->CreateRenderTarget(desc.Width, desc.Height, desc.Format, desc.MultiSampleType, desc.MultiSampleQuality, true, &pSurface, NULL);
	*/
	// m_pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, true);
	// m_pDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
	// m_pDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
	
	Mmain_log("WinBase Init[1.5]");

	// m_pDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
	// m_pDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);

	//g_pDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1);

	// Set blending factors so that alpha component determines transparancy //
	// m_pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	// m_pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
	

	// m_pDevice->SetRenderState(D3DRS_CLIPPING, false);


	/*
	m_pDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
	m_pDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_ANISOTROPIC);
	m_pDevice->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_LINEAR);
	m_pDevice->SetSamplerState(0, D3DSAMP_MAXANISOTROPY, 16);
	*/

	// m_pDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
	// m_pDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
	// m_pDevice->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_LINEAR);

	m_pDevice->SetRenderState(D3DRS_ANTIALIASEDLINEENABLE, TRUE);
	m_pDevice->SetRenderState(D3DRS_MULTISAMPLEANTIALIAS, TRUE);

	/*
	g_pFont1 = new Font(g_pDevice);
	g_pFont1->Load(640, 80, 10, "Graphics/HUD/font2.png", "ABCDEFGHIJKLMNOPQRSTUVWXYZ,.<>/\\?;:1234567890!@#$%^&*()-_=+[]{}°abcdefghijklmnopqrstuvwxyz");
	color = D3DCOLOR_ARGB(255, 255, 255, 255);

	initialised = false;

	// Init Sprite
	Initialize(g_pDevice, "Graphics/images/ladder/bg1.png", 600, 400);

	// Init Font
	InitFont(g_pDevice, rWidth, rHeight);
	*/

	Mmain_log("WinBase Init[1.6]");

	return true;
}

void CWindowBase::Render2D()
{



}


