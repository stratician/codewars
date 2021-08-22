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

#include "advert.h"
#include "vertex.h"


Advert::Advert(	IDirect3DDevice9* device, 
			char *_texturefile, 
			char *url, 
			char *title,
			float alpha,
			D3DXVECTOR3 A,
			D3DXVECTOR3 B,
			D3DXVECTOR3 C,
			D3DXVECTOR3 D)
{

#ifndef MAIN_SERVER
	// save a ptr to the device
	_device = device;
	sprintf(_url, "%s", url);
	sprintf(_title, "%s", title);



	_device->CreateVertexBuffer(
		4 * sizeof(Vertex), 
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX,
		D3DPOOL_MANAGED,
		&_vb,
		0);

	Vertex* v;
	_vb->Lock(0, 0, (void**)&v, 0);

	// build box

	// fill in the front face vertex data
	/*
	v[0] = Vertex(-0.5f * width, -0.5f * height, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f);
	v[1] = Vertex(-0.5f * width,  0.5f * height, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f, 1.0f);
	v[2] = Vertex( 0.5f * width,  0.5f * height, 0.0f, 0.0f, 0.0f, -1.0f, 1.0f, 1.0f);
	v[3] = Vertex( 0.5f * width, -0.5f * height, 0.0f, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f);
	*/

/*
	v[0] = Vertex(A[0], A[1], A[2], 0.0f, 0.0f, 1.0f, 1.0f, 1.0f);
	v[1] = Vertex(B[0], B[1], B[2], 0.0f, 0.0f, 1.0f, 0.0f, 1.0f);
	v[2] = Vertex(C[0], C[1], C[2], 0.0f, 0.0f, 1.0f, 1.0f, 0.0f);
	v[3] = Vertex(D[0], D[1], D[2], 0.0f, 0.0f, 1.0f, 0.0f, 0.0f);
*/
	v[0] = Vertex(A[0], A[1], A[2], 0.0f, 0.0f, 1.0f, 0.0f, 0.0f);
	v[1] = Vertex(B[0], B[1], B[2], 0.0f, 0.0f, 1.0f, 1.0f, 0.0f);
	v[2] = Vertex(C[0], C[1], C[2], 0.0f, 0.0f, 1.0f, 0.0f, 1.0f);
	v[3] = Vertex(D[0], D[1], D[2], 0.0f, 0.0f, 1.0f, 1.0f, 1.0f);



	_vb->Unlock();

	_device->CreateIndexBuffer(
		6 * sizeof(WORD),
		D3DUSAGE_WRITEONLY,
		D3DFMT_INDEX16,
		D3DPOOL_MANAGED,
		&_ib,
		0);

	WORD* i = 0;
	_ib->Lock(0, 0, (void**)&i, 0);

	// fill in the front face index data
	i[0] = 0; i[1] = 1; i[2] = 2;
	i[3] = 1; i[4] = 3; i[5] = 2;

	_ib->Unlock();


	// Load Texture //
	HRESULT hr = D3DXCreateTextureFromFile(_device, _texturefile, &_tex);

	if(FAILED(hr)) _tex = NULL;

	// Set up Material //
	//device->SetMaterial(&d3d::WHITE_MTRL);
	//_mtrl-> = WHITE_MTRL;

	_mtrl.Ambient.r = 0.0f;
	_mtrl.Ambient.g = 0.0f;
	_mtrl.Ambient.b = 0.0f;
	_mtrl.Ambient.a = alpha;

	_mtrl.Emissive.r = 0.0f;
	_mtrl.Emissive.g = 0.0f;
	_mtrl.Emissive.b = 0.0f;
	_mtrl.Emissive.a = alpha;

	_mtrl.Diffuse.r = 1.0f;
	_mtrl.Diffuse.g = 1.0f;
	_mtrl.Diffuse.b = 1.0f;
	_mtrl.Diffuse.a = alpha;

	_mtrl.Specular.r = 0.0f;
	_mtrl.Specular.g = 0.0f;
	_mtrl.Specular.b = 0.0f;
	_mtrl.Specular.a = alpha;

	_mtrl.Power = 20;

	mouseover = false;
	mousehitx = 0;
	mousehity = 0;
	

	_A = A;
	_B = B;
	_C = C;
	_D = D;



	D3DXMATRIX matRotX, matRotY, matRotZ, matScale, matTrans;

	// Calculate Scale matrix
	D3DXMatrixScaling(&matScale, 1.0f, 1.0f, 1.0f);

	// Calculate rotation matrix
	D3DXMatrixRotationX( &matRotX, 0.0f );
	D3DXMatrixRotationY( &matRotY, 0.0f );
	D3DXMatrixRotationZ( &matRotZ, 0.0f );
	
	// Calculate a translation matrix
	D3DXMatrixTranslation(&matTrans, 0.0f, 0.0f, 0.0f);

	_matWorld = matScale*(matRotX*matRotY*matRotZ)*matTrans;	

#endif
}

Advert::~Advert()
{
	if(_vb){_vb->Release(); _vb = 0;}
	if(_ib){_ib->Release(); _ib = 0;}
}



bool Advert::draw(void)
{
	

	_device->SetTransform(D3DTS_WORLD, &_matWorld);
	_device->SetMaterial(&_mtrl);
	if( _tex )
		_device->SetTexture(0, _tex);

	
	_device->SetStreamSource(0, _vb, 0, sizeof(Vertex));
	_device->SetIndices(_ib);
	_device->SetFVF(FVF_VERTEX);
	_device->DrawIndexedPrimitive(
		D3DPT_TRIANGLELIST, 
		0,                  
		0,                  
		4,
		0,
		2);  
	

	return true;
}