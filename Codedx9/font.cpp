	//////////////////////////////////////////////////////////////////////////////////////////////////
// 
// File: cube.cpp
// 
// Author: Frank Luna (C) All Rights Reserved
//
// System: AMD Athlon 1800+ XP, 512 DDR, Geforce 3, Windows XP, MSVC++ 7.0 
//
// Desc: Provides an interface to create and render a cube.
//          
//////////////////////////////////////////////////////////////////////////////////////////////////


#include "font.h"
#include <memirlib.h>
// #include "../CodeSupp/Build.h"

LPD3DXSPRITE fontSpr;

Font::Font(IDirect3DDevice9* device)
{
	// save a ptr to the device
	_device = device;
	_tex_n = 0;
	

	// Create 
	// Load the Font Image //
	//D3DXLoadTextureFromSurface(_device, sprite[sprno].surf, 0, lpddsx, &srcrect, &destrect,  D3DX_FT_POINT);
	HRESULT hr;
	hr = D3DXCreateSprite(device, &fontSpr);
	
}

Font::~Font()
{
	for(int i=0;i<_tex_n;i++)
	if(Tex[i])
	{
		Tex[i]->Release(); 
		Tex[i] = 0;
	}
}


void Font::FontALT(char *str)
{
	int i;

	
	for(i=0;i<256;i++) _alt[i] = 0xff;	// Clear everything in LUT to 0xFF

	i = 0;
	while(str[i] != 0)
	{
		_alt[str[i]] = i;
		i++;
	}

}

bool Font::Load(int img_width, int img_height, int spacing, char *fname, char *fntlst)
{

#ifndef MAIN_SERVER	
	// Get number of textures required for font //
	_tex_n = strlen(fntlst);
	if(_tex_n <= 0) return false;
	Tex = new IDirect3DTexture9 *[_tex_n];


	HRESULT hr;
	LPDIRECT3DSURFACE9 surface = NULL;
	D3DXIMAGE_INFO info;
	Mmain_log("Fnt Load [1]");
	// Set all spacing to 16 by default //
	for(int k=0;k<256;k++) _spacing[k] = spacing;
	Mmain_log("Fnt Load [1.1]");
	// _spacing = spacing;	// Width of each Character (i.e. the spacing) //

	Mgfx_fntaltgen(_alt, fntlst);
	Mmain_log("Fnt Load [1.2]");
	
	//D3DXCreateTextureFromFile(_device, "Graphics/textures/a.bmp", &Tex[0]);

	char buf[256];

	Mmain_log("Fnt Load [1.2]");
	
	hr = _device->CreateOffscreenPlainSurface(img_width, img_height, D3DFMT_A8R8G8B8, D3DPOOL_DEFAULT, &surface, NULL);
	sprintf(buf, "w:%d, h:%d, hr: %ld", img_width, img_height, hr);
	Mmain_log(buf);

	// hr = _device->CreateOffscreenPlainSurface(img_width, img_height, D3DFMT_UNKNOWN, D3DPOOL_DEFAULT, &surface, NULL);
	Mmain_log("Fnt Load [1.3]");

	//RECT destRect;
	//dRECT srcRect;
	// D3DCOLOR colorKey;
	if(surface == NULL)
	{
		Mmain_log("bad surface");
	}


	
	Mmain_log(fname);

	hr = D3DXLoadSurfaceFromFile(surface, NULL, NULL, fname, NULL, D3DX_DEFAULT, 0, &info);
	Mmain_log("Fnt Load [1.4]");

	D3DLOCKED_RECT lockedRect;
	Mmain_log("Fnt Load [1.5]");
	surface->LockRect(&lockedRect, 0, 0);	// Lock entire surface
	Mmain_log("Fnt Load [1.6]");
	DWORD* imageData = (DWORD*)lockedRect.pBits;

	Mmain_log("Fnt Load [2]");
	int x1 = 0;
	int y1 = 0;
	int w = info.Width;
	int h = info.Height;

	LONG i,j;
	LONG x,y;
	LONG l,r; // Left / Right Positions of the Current Sprite to be cut.
	LONG t,b;
	LONG r_x1,r_y1; // Exact Region where to Cut out a Sprite.
	LONG r_x2,r_y2;
	LONG row_top,row_bottom;
	LONG col_left,col_right;
	BYTE c;
	LONG sprno;
	DWORD trans;
	sprno = 0; // Sprite # counter.
	i = x1;
	j = y1;
	trans = imageData[0]; // Get value of top left hand pixel.


	RECT destRect;
	// RECT srcRect;
	Mmain_log("Fnt Load [2]");
	while(1)
	{
		// [1] Keep Scaning down each row, until we find the First Row with a solid pixel!
		for(j=j;j<(y1+h);j++)
		for(i=x1;i<(x1+w);i++)
		if(imageData[(lockedRect.Pitch>>2)*j+i]!=trans) goto break2;
		break2:
		row_top = j;
		if(row_top>=(y1+h)) break; // OFF Region, must be done!!
		// [2] Continue Scaning down each row, this time till we find the First Row with no solid Pixels!
		for(j=j;j<(y1+h);j++)
		{
			for(i=x1;i<(x1+w);i++)
			if(imageData[(lockedRect.Pitch>>2)*j+i]!=0)
			if(imageData[(lockedRect.Pitch>>2)*j+i]!=trans) break;
			if(i==(x1+w)) break; // Check if whole Line (H) completed without a solid pixel
		}
		if(j==(y1+h)) j = (y1+h)-1; // if OOB'ed. [***2k1 line]	
		row_bottom = j;
		// [3] Now we Find Col Left / Right of each Sprite Object.
		i = x1;
		while(1)
		{
			// [4] Same as Above but in Cols instead of Rows. [Start]
			for(i=i;i<(x1+w);i++)
			for(j=row_top;j<row_bottom;j++)
			if(imageData[(lockedRect.Pitch>>2)*j+i]!=0)
			if(imageData[(lockedRect.Pitch>>2)*j+i]!=trans) goto break2b;
			break2b:
			col_left = i;
			if(col_left>=(x1+w)) break; // OFF Region, must be done!!
			// [5] Same as Above but in Cols instead of Rows. [End]
			for(i=i;i<(x1+w);i++)
			{
				for(j=row_top;j<row_bottom;j++)
				if(imageData[(lockedRect.Pitch>>2)*j+i]!=0)
				if(imageData[(lockedRect.Pitch>>2)*j+i]!=trans) break;
				if(j==row_bottom) break; // Check if whole Line (V) completed without a solid pixel
			}
			if(i==(x1+w)) i = (x1+w)-1; // if OOB'ed. [***2k1 line]
			col_right = i;

			// ******** DUMP REGION into a SPRITE ******* //
			//Mgfx_sprinit(&sprite[sprno],col_right-col_left+1,row_bottom-row_top+1,style&0xff);
			//Mgfx_sprcopy(&sprite[sprno],col_left,row_top,col_right-col_left+1,row_bottom-row_top+1);

			destRect.left = 0;
			destRect.top = 0;
			destRect.right = col_right-col_left+10;
			destRect.bottom = row_bottom-row_top+10;
			/*
			srcRect.left = col_left-1;
			srcRect.top = row_top-1;
			srcRect.right = col_right+1;
			srcRect.bottom = row_bottom+1;
			*/
			_spacing[sprno] += (col_right - col_left)  * 1.5f;

			HRESULT hr;
			hr = D3DXCreateTexture(
				_device,				
				destRect.right,	destRect.bottom,	// Texture Dimensions
				1,									// create a complete mipmap chain
				0,									// usuage - none
				D3DFMT_A8R8G8B8,					// 32-bit XRGB format
				D3DPOOL_MANAGED,					// memory pool
				&Tex[sprno]);

			if(FAILED(hr))
				return false;

		

			D3DSURFACE_DESC textureDesc;
			Tex[sprno]->GetLevelDesc(0, &textureDesc);

			// Make sure we have 32-bit Format //
			if( textureDesc.Format != D3DFMT_A8R8G8B8 )
				return false;

			D3DLOCKED_RECT lockedRect2;
			Tex[sprno]->LockRect(0, &lockedRect2, 0, 0);
			
			DWORD *texData = (DWORD*)lockedRect2.pBits;

			for (int b = 0;b<(row_bottom - row_top + 10);b++)
			for (int a = 0;a < (col_right - col_left + 10);a++)
			{
				texData[(b) * lockedRect2.Pitch / 4 + (a)] = 0x00000000;
			}

			
			for(int b=0;b<(row_bottom-row_top);b++)
			for(int a=0;a<(col_right-col_left);a++)
			{
				
				// int xpos = col_left + a - 1;
				// int ypos = (row_top + b - 1) * lockedRect.Pitch / 4;
				DWORD col = imageData[(row_top + b) * (lockedRect.Pitch / 4) + col_left + a];
				// if (col == trans) col = 0x00000000;
				col &= 0xFFFFFFFF;
				texData[(b+5) * lockedRect2.Pitch / 4 + (a+5)] = col;
			}

			Tex[sprno]->UnlockRect(0);


			// fill mipmaps 
			hr = D3DXFilterTexture(
					Tex[sprno],		// texture to fill mipmap levels
					0,				// default palette
					0,				// use top level as source for lower levels
				D3DX_FILTER_NONE);	// default filter

			if(FAILED(hr))
			{
				::MessageBox(0, "D3DXFilterTexture() - FAILED", 0, 0);
				return false;
			}

			sprno++;
			// ****************************************** //
			i++;
			if(i>=(x1+w)) break; // ALL DONE on this ROW!
		}
		// ---------------------
		j++;
		if(j>=(y1+h)) break; // ALL DONE!
	}

	Mmain_log("Fnt Load [4]");

	surface->UnlockRect();
	surface->Release();

	Mmain_log("Fnt Load [5]");

#endif
	return true;

}



bool Font::Render(char *str, int xpos, int ypos, float angle, float alpha, float sx, float sy, int style)
{
	//::MessageBox(NULL, L"testing");
	// g_pDevice->SetRenderState(D3DRS_LIGHTING, true);	



	int len = strlen(str);
	float xcomp = cosf(angle);
	float ycomp = sinf(angle);

	float spacingfactor = sx * 1.0f;

	_device->SetRenderState(D3DRS_LIGHTING, true);


	M3d_sprdraw_PLATE2.mat->d3dm.Ambient.a = alpha;
	M3d_sprdraw_PLATE2.mat->d3dm.Diffuse.a = alpha;
	M3d_sprdraw_PLATE2.mat->d3dm.Emissive.a = alpha;
	M3d_sprdraw_PLATE2.mat->d3dm.Specular.a = alpha;


	M3d_sprdraw_PLATE2.mat->d3dm.Emissive.r = 1.0f;
	M3d_sprdraw_PLATE2.mat->d3dm.Emissive.g = 1.0f;
	M3d_sprdraw_PLATE2.mat->d3dm.Emissive.b = 1.0f;

	M3d_sprdraw_PLATE2.mat->d3dm.Diffuse.r = 1.0f;
	M3d_sprdraw_PLATE2.mat->d3dm.Diffuse.g = 1.0f;
	M3d_sprdraw_PLATE2.mat->d3dm.Diffuse.b = 1.0f;


	//M3d_objectbuild_PLATE(&M3d_sprdraw_PLATE2);
	/*
	M3d_objectset_LVERTEX(&M3d_platedraw_PLATE,3,0,0,0,lcol,lcol,0,1);
	M3d_objectset_LVERTEX(&M3d_platedraw_PLATE,2,1,0,0,rcol,rcol,1,1);
	M3d_objectset_LVERTEX(&M3d_platedraw_PLATE,1,1,1,0,rcol,rcol,1,0);
	M3d_objectset_LVERTEX(&M3d_platedraw_PLATE,0,0,1,0,lcol,lcol,0,0);
	*/
	M3d_objectset_VERTEX(&M3d_sprdraw_PLATE2, 3, 0, -sy, 0, 0, 1);
	M3d_objectset_VERTEX(&M3d_sprdraw_PLATE2, 2, sx*2.0f, -sy, 0, 1, 1);
	M3d_objectset_VERTEX(&M3d_sprdraw_PLATE2, 1, sx*2.0f, sy, 0, 1, 0);
	M3d_objectset_VERTEX(&M3d_sprdraw_PLATE2, 0, 0, sy, 0, 0, 0);
	M3d_normalizeobject(&M3d_sprdraw_PLATE2);


	M3d_objectset_VERTEX(&M3d_sprdraw_PLATE2, 3, 0, -sy * 2, 0, 0, 1);
	M3d_objectset_VERTEX(&M3d_sprdraw_PLATE2, 2, sx*2.0f, -sy * 2, 0, 1, 1);
	M3d_objectset_VERTEX(&M3d_sprdraw_PLATE2, 1, sx*2.0f, 0, 0, 1, 0);
	M3d_objectset_VERTEX(&M3d_sprdraw_PLATE2, 0, 0, 0, 0, 0, 0);
	M3d_normalizeobject(&M3d_sprdraw_PLATE2);






	// Get width of Text //
	float text_width = 0;
	for (int i = 0;i<strlen(str);i++)
	{
		text_width += ((float)_spacing[_alt[str[i]]])*spacingfactor;
	}

	switch (style)
	{
	case 0:	// Left Aligned Text //
		break;

	case 1:	// Centre Aligned Text //
		xpos = xpos - (int)(xcomp * text_width * 0.5f);
		ypos = ypos - (int)(ycomp * text_width * 0.5f);
		break;
	}





	for (int i = 0;i<len;i++)
	{
		// If our character is part of the LUT, then display the character //
		if (str[i] == 32)
		{
			xpos += (int)(xcomp * (_spacing['A'])*spacingfactor*1);	// Use Default Spacing //
			ypos += (int)(ycomp * (_spacing['A'])*spacingfactor*1);	// Use Default Spacing //
		}
		else if (_alt[str[i]] != 0xff)
		{
			//xpos += xcomp * (_spacing[_alt[str[i]]]>>1);
			//ypos += ycomp * (_spacing[_alt[str[i]]]>>1);
			M2d_sprdraw(Tex[_alt[str[i]]], xpos, ypos, angle, 0.3f);
			//M2d_sprdraw(Tex[_alt[str[i]]], xpos, ypos, 0.0f, alpha);

			xpos += (int)(xcomp * (_spacing[_alt[str[i]]])*spacingfactor);
			ypos += (int)(ycomp * (_spacing[_alt[str[i]]])*spacingfactor);
		}

	}

	M3d_sprdraw_PLATE2.mat->d3dm.Ambient.a = 1.0f;
	M3d_sprdraw_PLATE2.mat->d3dm.Diffuse.a = 1.0f;
	M3d_sprdraw_PLATE2.mat->d3dm.Emissive.a = 1.0f;
	M3d_sprdraw_PLATE2.mat->d3dm.Specular.a = 1.0f;

	M3d_sprdraw_PLATE2.mat->d3dm.Emissive.r = 0.0f;
	M3d_sprdraw_PLATE2.mat->d3dm.Emissive.g = 0.0f;
	M3d_sprdraw_PLATE2.mat->d3dm.Emissive.b = 0.0f;

	_device->SetRenderState(D3DRS_LIGHTING, false);

	//g_pDevice->SetRenderState(D3DRS_LIGHTING, true);

	return true;
}




bool Font::Draw(LPD3DXSPRITE surfSprite, char *str, int xpos, int ypos, float angle, float alpha, float sx, float sy, int style, D3DXCOLOR color, RECT *srcRect)
{
	//::MessageBox(NULL, L"testing");
	// g_pDevice->SetRenderState(D3DRS_LIGHTING, true);	
	
	D3DXMATRIX matWorld;
	D3DXMATRIX matScale;
	D3DXMATRIX matRotX;
	D3DXMATRIX matRotY;
	D3DXMATRIX matRotZ;
	D3DXMATRIX matTrans;


	int len = strlen(str);
	float xcomp = cosf(angle);
	float ycomp = sinf(angle);

	float spacingfactor = sx  *0.50;

	// Get width of Text //
	float text_width = 0;
	for (int i = 0;i<strlen(str);i++)
	{
		text_width += ((float)_spacing[_alt[str[i]]])*spacingfactor;
	}

	switch (style)
	{
	case 0:	// Left Aligned Text //
		break;

	case 1:	// Centre Aligned Text //
		xpos = xpos - (int)(xcomp * text_width * 0.5f);
		ypos = ypos - (int)(ycomp * text_width * 0.5f);
		break;
	}



	D3DSURFACE_DESC desc;



	
	
	for (int i = 0;i<len;i++)
	{
		// If our character is part of the LUT, then display the character //
		if (str[i] == 32)
		{
			xpos += (int)(xcomp * (_spacing['A'])*spacingfactor * 0.6);	// Use Default Spacing //
			ypos += (int)(ycomp * (_spacing['A'])*spacingfactor * 0.6);	// Use Default Spacing //
		}
		else if (_alt[str[i]] != 0xff)
		{
			//xpos += xcomp * (_spacing[_alt[str[i]]]>>1);
			//ypos += ycomp * (_spacing[_alt[str[i]]]>>1);
			//M2d_sprdraw(Tex[_alt[str[i]]], xpos, ypos, angle);

			D3DXVECTOR3 pos;
			pos.x = xpos;
			pos.y = ypos;
			pos.z = 0.0f;

			D3DXMatrixIdentity(&matTrans);
			D3DXMatrixIdentity(&matScale);
			D3DXMatrixIdentity(&matWorld);

		

			// Calculate Scale matrix
			D3DXMatrixScaling(&matScale, sx, sy, 1.0f);

			// Calculate rotation matrix
			D3DXMatrixRotationX(&matRotX, 0.0f);
			D3DXMatrixRotationY(&matRotY, 0.0f);
			D3DXMatrixRotationZ(&matRotZ, 0.0f);

			// Calculate a translation matrix
			D3DXMatrixTranslation(&matTrans, pos[0], pos[1], pos[2]);
			matWorld = matScale*(matRotX*matRotY*matRotZ)*matTrans;
			
			HRESULT hr;
			
			
			hr = surfSprite->SetTransform(&matWorld);
			hr = surfSprite->Draw(Tex[_alt[str[i]]], srcRect, 0, 0, color);
			

			// M2d_sprdraw(Tex[_alt[str[i]]], xpos, ypos, angle);

			

			xpos += (int)(xcomp * (_spacing[_alt[str[i]]])*spacingfactor);
			ypos += (int)(ycomp * (_spacing[_alt[str[i]]])*spacingfactor);
		}
		

	}
	

	return true;
}

