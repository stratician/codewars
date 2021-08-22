#pragma once

#include "stdafx.h"
#include "../CodeSupp/GameAppView.h"

#include <string>

class Sprite
{
public:
	Sprite(void);
	virtual ~Sprite(void);

	bool loadSprite(LPDIRECT3DDEVICE9 device, std::string filename);

	void render(LPDIRECT3DDEVICE9 pDevice, int alpha);
	void setPosition(int x, int y);
	void setSize(int newWidth, int newHeight);


	int getHeight();
	int getWidth();

private:
	LPD3DXSPRITE sprite;
	LPDIRECT3DTEXTURE9 texture;
	D3DXVECTOR3 position;
	D3DXVECTOR3 scale;

	float width;
	float height;

};
