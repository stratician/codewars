//////////////////////////////////////////////////////////////////////////////////////////////////
// 
// File: Font.h
// 
// Author: Memir Games (C) All Rights Reserved
//
//          
// This Class loads an image (of fonts) and extracts the characters //
//////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef __fontH__
#define __fontH__


#include "C:\Program Files (x86)\Microsoft DirectX SDK (June 2010)\Include\d3dx9.h"
#include <string>

class Font
{
public:
	Font(IDirect3DDevice9* device);
	~Font();

	bool Load(int img_width, int img_height, int spacing, char *fname, char *fntlst);
	void FontALT(char *alt);
	bool Render(char *str, int xpos, int ypos, float angle = 0.0f, float alpha = 1.0f, float sx = 1.0f, float sy = 1.0f, int style = 0 );
	bool Draw(LPD3DXSPRITE surfSprite, char *str, int xpos, int ypos, float angle = 0.0f, float alpha = 1.0f, float sx = 1.0f, float sy = 1.0f, int style = 0, D3DXCOLOR color = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), RECT *srcRect = NULL);


private:
	
	IDirect3DDevice9*       _device;
	IDirect3DVertexBuffer9* _vb;
	IDirect3DIndexBuffer9*  _ib;
	IDirect3DTexture9**	Tex;

	int _tex_n;
	BYTE _alt[256];
	int _spacing[256];

};


#endif //__FontH__