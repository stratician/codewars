#include "../CodeSupp/stdafx.h"
#include "../CodeSupp/GameAppView.h"

#include "Sprite.h"



CSprite::CSprite(int type, char *bmp, char *bmp_mo, int bw, int bh)
{
	// Load Bitmap
	_bitmap = (HBITMAP)LoadImage(0, bmp, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
	_bitmap_mo = (HBITMAP)LoadImage(0, bmp_mo, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
	_x = 0;
	_y = 0;
	_width = bw;
	_height = bh;

	_bMouseOver = false;
	_bMouseOverPrevious = false;
	_bMouseOverSurface = false;
	_bMouseDown = false;
	_bMouseLButtonDown = false;

	_type = type;
	_bRedraw = true;
}


CSprite::~CSprite()
{
}

void CSprite::Init(CDC *pDC)
{
	if (_dcMem == NULL)
	{
		_dcMem.CreateCompatibleDC(pDC);
	}
	// Select the bitmap into the in-memory DC
	_dcMem.SelectObject(_bitmap);
}

void CSprite::Draw(CDC *pDC, int x, int y)
{
	if (pDC)
	{
		_x = x;
		_y = y;


		pDC->BitBlt(_x, _y, _width, _height, &_dcMem, 0, 0, SRCCOPY);

		// Draw a red rectangle for now.
		// CBrush brRed(RGB(255, 50, 50));  //Creates a red brush
		// pDC->FillRect(CRect(0, 0, 755, 755), &brRed);
	}
}


void CSprite::Update(CDC *pDC)
{
	if (_bRedraw)
	{
		_bRedraw = false;

		if (pDC)
		{
			pDC->BitBlt(_x, _y, _width, _height, &_dcMem, 0, 0, SRCCOPY);
		}
	}
}


void CSprite::OnMouseHover()
{
	_bMouseOverSurface = true;
}

void CSprite::OnMouseLeave()
{
	_bMouseOverSurface = false;
}

void CSprite::OnLButtonDown()
{
	_bMouseLButtonDown = true;
}

void CSprite::Process()
{
	ProcessMouse();
}

void CSprite::ProcessMouse()
{
	if (!g_gameAppView) return;

	

	// Get Mouse Over Status
	_bMouseOver = false;
	_bMouseDown = false;

	if (_bMouseOverSurface)
	{
		if (g_gameAppView->m_mousePoint.x >= _x && g_gameAppView->m_mousePoint.x <= (_x + _width))
		if (g_gameAppView->m_mousePoint.y >= _y && g_gameAppView->m_mousePoint.y <= (_y + _height))
		{
			_bMouseOver = true;

			// Check if we clicked on the button
			if (_bMouseLButtonDown)
			{
				_type = _type;
				_bMouseLButtonDown = false;
				_bMouseDown = true;
			}
		}
	}

	_bMouseLButtonDown = false;

	// If there was a change of state with Mouse Over, then we need to update the sprite
	if (_bMouseOver != _bMouseOverPrevious)
	{
		_bMouseOverPrevious = _bMouseOver;
		if (_bMouseOver)
		{
			_dcMem.SelectObject(_bitmap_mo);
			
			// Update our Cursor
			g_gameAppView->m_bUpdateCursor = false;
			SetCursor(g_gameAppView->m_hCursorHand);
			
		}

		else
		{
			_dcMem.SelectObject(_bitmap);

			// Update our Cursor
			g_gameAppView->m_bUpdateCursor = true;
			SetCursor(g_gameAppView->m_hCursor);

			
		}

		// Need to redraw, since we changed the sprite bitmap
		_bRedraw = true;

	}

	

	
}
