#include "../CodeSupp/stdafx.h"
#include "../CodeSupp/GameAppView.h"

#include "LobbyButtons.h"


CLobbyButtons::CLobbyButtons()
{
	_refreshBtn = new CSprite(0, "Graphics/skin/button_refresh.bmp", "Graphics/skin/button_refresh_mo.bmp", 24, 23);
	_refreshBtnClicked = false;
}


CLobbyButtons::~CLobbyButtons()
{
}


void CLobbyButtons::Init(CDC *pDC)
{
	_refreshBtn->Init(pDC);
}

void CLobbyButtons::OnLButtonDown()
{
	_refreshBtn->OnLButtonDown();
}

void CLobbyButtons::Process()
{
	if (!g_gameAppView) return;

	_refreshBtn->Process();


	// Check if we clicked on refresh button
	if (_refreshBtn->_bMouseDown)
	{
		if (_refreshBtnClicked == false)
		{
			_refreshBtnClicked = true;

			// Refresh the Message Board
			g_gameAppView->ButtonRefresh();
		}
	}
	else
	{
		_refreshBtnClicked = false;
	}

}


void CLobbyButtons::Update(CDC *pDC)
{
	_refreshBtn->Update(pDC);
}

void CLobbyButtons::UpdateOffset(int xbtnoffset, int tableListWidth)
{
	_xbtnoffset = xbtnoffset;
	_tableListWidth = tableListWidth;
}

void CLobbyButtons::Draw(CDC *pDC)
{
	// Draw the Buttons
	_refreshBtn->Draw(pDC, _xbtnoffset - 40 - 24, 84);
}


void CLobbyButtons::OnMouseHover()
{
	_refreshBtn->OnMouseHover();
}

void CLobbyButtons::OnMouseLeave()
{
	_refreshBtn->OnMouseLeave();
}

