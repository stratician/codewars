
#include ".\sprite.h"

//
//   FUNCTION: Surface(void)
//
//   PURPOSE: Constructor for the 2d Sprite class
//
Sprite::Sprite(void)
{

}
//
//   FUNCTION: ~Surface(void)
//
//   PURPOSE: deconstructor for the 2d Sprite class
//
Sprite::~Sprite(void)
{

}
//
//   FUNCTION: loadSurface(LPDIRECT3DDEVICE9 device, std::string filename, int w, int h)
//
//   PURPOSE: create a D3DXSprite and loads it with an image
//
bool Sprite::loadSprite(LPDIRECT3DDEVICE9 device, std::string filename)
{
	D3DXCreateSprite(device, &sprite);
	D3DXCreateTextureFromFileEx(device, filename.c_str(), D3DX_DEFAULT, D3DX_DEFAULT, D3DX_DEFAULT, 0, D3DFMT_UNKNOWN, D3DPOOL_DEFAULT, D3DX_FILTER_NONE, D3DX_FILTER_NONE, 0xFFFF00FF, NULL, NULL, &texture);
	
	D3DXIMAGE_INFO imageInfo;
	D3DXGetImageInfoFromFile(filename.c_str(), &imageInfo);

	height = (float)imageInfo.Height;
	width = (float)imageInfo.Width;

	return true;
}

//
//   FUNCTION: render(LPDIRECT3DDEVICE9 pDevice)
//
//   PURPOSE: draws the sprite
//
void Sprite::render(LPDIRECT3DDEVICE9 pDevice, int alpha)
{

	alpha = (int)(255*((float)alpha/100));
	D3DXMATRIX scaleMatrix; 
	D3DXMATRIX transMatrix;
	D3DXMatrixScaling(&scaleMatrix, scale.x, scale.y, scale.z);
	D3DXMatrixTranslation(&transMatrix, position.x, position.y, position.z);

	D3DXMatrixMultiply(&transMatrix, &scaleMatrix, &transMatrix);
	sprite->SetTransform(&transMatrix);

	sprite->Begin(D3DXSPRITE_ALPHABLEND);
	sprite->Draw(texture, NULL, NULL, NULL, D3DCOLOR_RGBA(255,255,255,alpha));
	sprite->End();

}

//
//   FUNCTION: setPosition(int x, int y)
//
//   PURPOSE: sets the destination position on the screen, x and y are the top left corner.
//
void Sprite::setPosition(int x, int y){
	position.x = (float)x;
	position.y = (float)y;
	position.z = 0.0f;
}

//
//   FUNCTION: setSize
//
//   PURPOSE: sets size the image width and height
//
void Sprite::setSize(int newWidth, int newHeight){
	
	scale.x = (float)newWidth/width;
	scale.y = (float)newHeight/height;
	scale.z = 0;
}


int Sprite::getWidth(){
	return (int)width;
}

int Sprite::getHeight(){
	return (int)height;
}