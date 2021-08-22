#pragma once

#include <afxwin.h>


class CSprite
{

public:
	CDC _dcMem;
	HBITMAP _bitmap;
	HBITMAP _bitmap_mo;
	int _x;
	int _y;
	int _width;
	int _height;

	bool _bMouseDown;
	bool _bMouseOverPrevious;
	bool _bMouseOver;

	bool _bMouseOverSurface;
	HCURSOR _hCursor;
	bool _bMouseLButtonDown;

	int _type;		// 0: Refresh Button, 10: Find Match

	bool _bRedraw;


public:
	CSprite(int type, char *bmp, char *bmp_mo, int bw, int bh);
	~CSprite();

	void Init(CDC *pDC);
	void Draw(CDC *PDC, int x, int y);
	void Update(CDC *pDC);
	void Process();
	void ProcessMouse();
	void OnMouseLeave();
	void OnMouseHover();
	void OnLButtonDown();

};

