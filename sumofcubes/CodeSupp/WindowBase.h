#pragma once


#include "WindowBase.h"

// #include <d3d.h>
#include <d3d9.h>
#include "C:\Program Files (x86)\Microsoft DirectX SDK (June 2010)\Include\d3dx9.h"
// #include <d3d9types.h>



// #include <Windows.h>
#include <tchar.h>
#include <string.h>
#include "../Code/lobby.h"




struct ID3DXMesh;
struct IDirect3DTexture9;

class CWindowBase
{
	
private:

	int x, y;
	int width, height;

public:
	
	
	HWND hWnd;

	// IDirect3D9* m_pd3d9 = NULL;			// Direct3D
	IDirect3DDevice9* m_pDevice = NULL;	// Direct3D-Device
	IDirect3DSurface9 *pSurface;
	

	bool initialised;
	bool _bFirstTime;


	CWindowBase();
	virtual ~CWindowBase();

	
	virtual void Render2D() = 0;
	virtual int Create(HINSTANCE hInstance, HWND parentWnd, char *title, int x, int y, int w, int h);
	bool InitD3D();
	
	virtual void OnClose();




	void Destroy();



};

