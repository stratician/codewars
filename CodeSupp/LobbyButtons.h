#pragma once


#include "Sprite.h"

class CLobbyButtons
{

public:
	CSprite *_refreshBtn;
	bool _refreshBtnClicked;

	
	int _xbtnoffset;
	int _tableListWidth;

public:
	CLobbyButtons();
	~CLobbyButtons();

	void OnLButtonDown();
	
	void Init(CDC *pDC);
	void Process();
	void Draw(CDC *pDC);
	void Update(CDC *pDC);
	void ProcessMouse();
	void OnMouseLeave();
	void OnMouseHover();

	void UpdateOffset(int xbtnoffset, int tableListWidth);
};

