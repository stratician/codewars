//////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef __advertH__
#define __advertH__

#include "../CodeSupp/build.h"
#include "C:\Program Files (x86)\Microsoft DirectX SDK (June 2010)\Include\d3dx9.h"
#include <string>
#include "d3dUtility.h"

class Advert
{
public:
	Advert(	IDirect3DDevice9* device, 
			char *_texturefile, 
			char *url, 
			char *title, 
			float alpha,
			D3DXVECTOR3 A,
			D3DXVECTOR3 B,
			D3DXVECTOR3 C,
			D3DXVECTOR3 D);
	~Advert();

	bool draw(void);
private:
	IDirect3DDevice9*       _device;
	IDirect3DVertexBuffer9* _vb;
	IDirect3DIndexBuffer9*  _ib;
public:
	D3DXMATRIX _matWorld;
	IDirect3DTexture9*	_tex;
	
	D3DMATERIAL9 _mtrl;

	char _url[256];		// URL that image link will point to
	char _title[256];	// Not required if there's a texture specified //
	bool mouseover;		// This tell us when the mouse is over the advert
	float mousehitx;	// This is the mouse 2D X-Position that we first hit the Advert
	float mousehity;	// This is the mouse 2D Y-Position that we first hit the Advert

	D3DXVECTOR3 _A, _B, _C, _D;


};
#endif //__advertH__