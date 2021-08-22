#pragma once
#include "afxwin.h"

#include "../direct2d/sprite3D.h"


class CStatsWindow :
	public CWnd
{

public:
	void MOUSEMOVE(UINT Msg, WPARAM wParam, LPARAM lParam);
	void LBUTTONDOWN(UINT Msg, WPARAM wParam, LPARAM lParam);
	void LBUTTONUP(UINT Msg, WPARAM wParam, LPARAM lParam);
	void RBUTTONDOWN(UINT Msg, WPARAM wParam, LPARAM lParam);
	void RBUTTONUP(UINT Msg, WPARAM wParam, LPARAM lParam);
	
	bool m_bLButtonDown;
	bool m_bLButtonJustUp;
	
	bool m_bRButtonDown;
	bool m_bRButtonJustUp;

	int m_width;
	int m_height;
	int m_xpos;
	int m_ypos;

	int m_oldx;
	int m_oldy;
	int m_newx;
	int m_newy;

	bool m_dragWindow;


public:
	CWnd *m_pParent;
	BOOL m_bPaintFirstTime;
	HWND m_hWnd;
	
	IDirect3DSwapChain9 *m_pSwapChain;
	IDirect3DSurface9 *pRenderSurface;

	// Sprites
	IDirect3DTexture9 *texture;


	LPDIRECT3DTEXTURE9 tex;
	LPD3DXSPRITE sprite;

	D3DXVECTOR3 position;
	D3DCOLOR color;
	bool initialised;
	bool _bFirstTime;


	
	Sprite3D *m_Sprite;

	Font *g_pFont1;

	LPD3DXFONT g_Font = NULL; //font object
	
	RECT m_FontPosition;
	

public:
	CStatsWindow();
	~CStatsWindow();

	bool Initialize(LPDIRECT3DDEVICE9 device, std::string file, int width, int height);
	bool IsInitialised();
	void Update();
	void Draw();

	void InitFont(LPDIRECT3DDEVICE9 device, int sWidth, int sHeight);
	void DrawFont();

	void InitD2D(HWND hWnd, int rWidth, int rHeight);
	void InitD3D(HWND hWnd, int rx, int ry, int rWidth, int rHeight);
	void Render();
	void Render3D();
	void Render2D();

};

