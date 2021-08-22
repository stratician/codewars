#include "FindMatchCtrl.h"




#include "../CodeSupp/stdafx.h"
#include "../CodeSupp/GameAppView.h"

#include "Sprite.h"



CFindMatchCtrl::CFindMatchCtrl()
{
#ifndef MAIN_SERVER
	// Load Bitmap

#ifdef THE_POOL_CLUB
	_bgBitmap = (HBITMAP)LoadImage(0, "Graphics/skin/findmatchbg_ipool.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
#else
	_bgBitmap = (HBITMAP)LoadImage(0, "Graphics/skin/findmatchbg.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
#endif
	_x = 0;
	_y = 0;
	_width = 0;
	_height = 0;

	_bMouseOver = false;
	_bMouseOverPrevious = false;
	_bMouseOverSurface = false;
	_bMouseDown = false;
	_bMouseLButtonDown = false;


	// Find Match Button
	_findMatchBtn = new CSprite(10, "Graphics/skin/findmatchbtn.bmp", "Graphics/skin/findmatchbtn_mo.bmp", 140, 30);
	_findMatchClicked = false;

	// Find Match Sta tus
	_findMatchStatusSpr = new CSprite(11, "Graphics/skin/findmatchstatus.bmp", "Graphics/skin/findmatchstatus_mo.bmp", 152, 32);

	// Find Match Cancel
	_findMatchCancelBtn = new CSprite(12, "Graphics/skin/findmatchcancel.bmp", "Graphics/skin/findmatchcancel_mo.bmp", 132, 18);

	// Ladder Checkbox Sprite
	_raceToTwoCheckBoxSpr = new CSprite(13, "Graphics/skin/checkbox.bmp", "Graphics/skin/checkbox_mo.bmp", 20, 19);
	_raceToTwoCheckBoxOnSpr = new CSprite(13, "Graphics/skin/checkboxon.bmp", "Graphics/skin/checkboxon_mo.bmp", 20, 19);
	
	// Pro Checkbox Sprite
	_proCheckBoxSpr = new CSprite(13, "Graphics/skin/checkbox.bmp", "Graphics/skin/checkbox_mo.bmp", 20, 19);
	_proCheckBoxOnSpr = new CSprite(13, "Graphics/skin/checkboxon.bmp", "Graphics/skin/checkboxon_mo.bmp", 20, 19);

	// Exact Match Checkbox Sprite
	_exactMatchCheckBoxSpr = new CSprite(13, "Graphics/skin/checkbox.bmp", "Graphics/skin/checkbox_mo.bmp", 20, 19);
	_exactMatchCheckBoxOnSpr = new CSprite(13, "Graphics/skin/checkboxon.bmp", "Graphics/skin/checkboxon_mo.bmp", 20, 19);

			
	_findMatchStatus = 0;

	_ticker = 0;
	_tickerSeconds = 0;
	
	// Defaultly values
	
	if (g_build.gameType == GAMETYPE_SNOOKER) _bRaceToTwoChecked = true;
		else _bRaceToTwoChecked = false;
	_bProChecked = false;
	_bExactMatchChecked = false;


	_lPointsStr[0] = 0;
	_lDivisionStr[0] = 0;
	_lPoints = 12000;
	_lGames = 0;
	_lWins = 0;
	_lLosses = 0;
	_lHighBreak = 0;
	_lHighBreakPro = 0;
	_lGamesRemaining = 0;

	_bStatsReady = false;
#endif	
}


CFindMatchCtrl::~CFindMatchCtrl()
{
}

void CFindMatchCtrl::Init(CDC *pDC)
{

	static bool bFirstTime = true;
#ifndef MAIN_SERVER
	if (bFirstTime)
	{
		bFirstTime = false;
		_dcMemBG.CreateCompatibleDC(pDC);
	}
	// Select the background bitmap into the in-memory DC
	_dcMemBG.SelectObject(_bgBitmap);

	// Init Find Match Button
	_findMatchBtn->Init(pDC);

	// Init Find Match Status and Cancel
	_findMatchStatusSpr->Init(pDC);
	_findMatchCancelBtn->Init(pDC);

	_raceToTwoCheckBoxSpr->Init(pDC);
	_raceToTwoCheckBoxOnSpr->Init(pDC);

	_proCheckBoxSpr->Init(pDC);
	_proCheckBoxOnSpr->Init(pDC);

	_exactMatchCheckBoxSpr->Init(pDC);
	_exactMatchCheckBoxOnSpr->Init(pDC);


	// Create our Find Match Status Font
	// _fnt.CreateFont(18, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE, 0, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_ROMAN, "Arial Bold");
	_pOldFont = pDC->SelectObject(&_fnt);

	
	// Create stats Font
	_fntStats.CreateFont(16, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE, 0, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, ANTIALIASED_QUALITY, DEFAULT_PITCH, "Arial");
	_fntStatsBold.CreateFont(15, 0, 0, 0, FW_NORMAL, TRUE, FALSE, FALSE, 0, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, ANTIALIASED_QUALITY, DEFAULT_PITCH, "Arial Bold");


	// Create Brush
	CBrush blackBrush(RGB(0, 0, 0));
	_oldBlackBrush = pDC->SelectObject(&_blackBrush);
#endif
}

void CFindMatchCtrl::Draw(CDC *pDC)
{
#ifndef MAIN_SERVER
	if (pDC)
	{

		// Set up Font
		pDC->SetBkMode(TRANSPARENT);
		pDC->SetBkColor(RGB(0, 0, 0));

		
		pDC->SelectObject(&_blackBrush);
		pDC->SelectObject(&_fnt);


		pDC->BitBlt(_x, _y, _width, _height, &_dcMemBG, 0, 0, SRCCOPY);

		switch (_findMatchStatus)
		{
			case -1:
					break;
			case 0: // Show Find Match Button
					_findMatchBtn->Draw(pDC, _x + 11, _y + 10);
					break;

			case 1:	// Searching For Match

					_findMatchStatusSpr->Draw(pDC, _x + 3, _y + 6);
					_findMatchCancelBtn->Draw(pDC, _x + 3 + 8, _y + 6 + 32 + 2);

					// Draw Searching for Match Text
					pDC->SetTextColor(RGB(255, 255, 255));
					char buffer[80];

					pDC->SetTextColor(RGB(255, 255, 255));
					pDC->TextOut(_x + 40, _y + 14, _findMatchStatusText);

					break;
			case 2:	// Found Match
					_findMatchStatusSpr->Draw(pDC, _x + 3, _y + 6);
					// _findMatchCancelBtn->Draw(pDC, _x + 3 + 8, _y + 6 + 32 + 2);
					pDC->SetTextColor(RGB(255, 255, 255));
					pDC->TextOut(_x + 22, _y + 14, _findMatchStatusText);
					break;
		}


		// Ladder Check Box
		if (_bRaceToTwoChecked) _raceToTwoCheckBoxOnSpr->Draw(pDC, _x + 130, _y + 68);
			else _raceToTwoCheckBoxSpr->Draw(pDC, _x + 130, _y + 68);

		// Ladder Check Box
		if (_bProChecked) _proCheckBoxOnSpr->Draw(pDC, _x + 130, _y + 68 + 25);
			else _proCheckBoxSpr->Draw(pDC, _x + 130, _y + 68 + 25);

		// Exact Match Check Box
		if (_bExactMatchChecked) _exactMatchCheckBoxOnSpr->Draw(pDC, _x + 130, _y + 68 + 25 + 25);
			else _exactMatchCheckBoxSpr->Draw(pDC, _x + 130, _y + 68 + 25 + 25);


		pDC->SelectObject(_pOldFont);


		ShowLadderStats(pDC);
	}
#endif
}


void CFindMatchCtrl::SetPosition(int xpos, int ypos, int width, int height)
{
#ifndef MAIN_SERVER
	_x = xpos;
	_y = ypos;
	_width = width;
	_height = height;
#endif
}

void CFindMatchCtrl::Update(CDC *pDC)
{
#ifndef MAIN_SERVER
	if (pDC)
	{
		// Set up Font
		pDC->SetBkMode(TRANSPARENT);
		pDC->SetBkColor(RGB(0, 0, 0));

		pDC->SelectObject(&_blackBrush);
		pDC->SelectObject(&_fnt);



		// pDC->BitBlt(_x, _y, _width, _height, &_dcMemBG, 0, 0, SRCCOPY);
		switch (_findMatchStatus)
		{
			case -1:
				break;

			case 0: // Show Find Match Button
				_findMatchBtn->Update(pDC);
				break;

			case 1:	// Searching For Match
				_findMatchStatusSpr->Update(pDC);
				_findMatchCancelBtn->Update(pDC);
				
				

				// Draw Searching for Match Text
				pDC->SetTextColor(RGB(255, 255, 255));
				pDC->TextOut(_x + 40, _y + 14, _findMatchStatusText);
				

				break;
			case 2:	// Found Match
				_findMatchStatusSpr->Update(pDC);
				// _findMatchCancelBtn->Update(pDC);

				// Draw Searching for Match Text
				pDC->SetTextColor(RGB(255, 255, 255));
				pDC->TextOut(_x + 22, _y + 14, _findMatchStatusText);

				break;
		}
		
		// Ladder Check Box
		if (_bRaceToTwoChecked) _raceToTwoCheckBoxOnSpr->Draw(pDC, _x + 130, _y + 68);
			else _raceToTwoCheckBoxSpr->Draw(pDC, _x + 130, _y + 68);

		// Ladder Check Box
		if (_bProChecked) _proCheckBoxOnSpr->Draw(pDC, _x + 130, _y + 68 + 25);
			else _proCheckBoxSpr->Draw(pDC, _x + 130, _y + 68 + 25);

		// Exact Match Check Box
		if (_bExactMatchChecked) _exactMatchCheckBoxOnSpr->Draw(pDC, _x + 130, _y + 68 + 25 + 25);
			else _exactMatchCheckBoxSpr->Draw(pDC, _x + 130, _y + 68 + 25 + 25);


		pDC->SelectObject(_pOldFont);


		// ShowLadderStats(pDC);

	}
#endif
}

void CFindMatchCtrl::UpdateLadderStats()
{
#ifndef MAIN_SERVER
	// Get Ladder Wins, Losses & High Breaks

	// Get our player index
	int pIdx = g_gameAppView->m_lobby->GetPlayerIdx(g_gameAppView->m_playerSockIdx);
	if (pIdx == -1) return;



	_lLosses = (m_player[pIdx].ladderWinsLosses & 0xFFFF);
	_lWins = (m_player[pIdx].ladderWinsLosses & 0xFFFF0000) >> 16;
	_lHighBreak = (m_player[pIdx].ladderHighBreak & 0xFFFF);
	_lHighBreakPro = (m_player[pIdx].ladderHighBreak & 0xFFFF0000) >> 16;
	_lGamesRemaining = (5 - (_lWins + _lLosses));
	if (_lGamesRemaining < 0) _lGamesRemaining = 0;

	/*
	_lGamesRemaining = 0;
	_lWins = 4;
	_lLosses = 1;
	_lHighBreak = 78;
	_lHighBreakPro = 0;

	m_player[pIdx].ladderPoints = 18520;
	*/


	// Ladder Rank & Points
	// Division (i.e. 0-10999: Bronze, 11000-13999: Silver, 14000-16999: Gold, 17000-20999: Platinum, 21000+ Diamond)
	int ladderPoints = m_player[pIdx].ladderPoints;
	int rank = m_player[pIdx].ladderRank;
	// If we have played less than 5 games (change divisionStr too 'N Games needed')
	if (_lGamesRemaining > 0)
	{
		sprintf(_lDivisionStr, "after %d ladder games", _lGamesRemaining);

		sprintf(_lPointsStr, "");
	}
	else
	{
		if (ladderPoints < 11000) sprintf(_lDivisionStr, "Bronze - Rank %d", rank);
		else if (ladderPoints < 14000) sprintf(_lDivisionStr, "Silver - Rank %d", rank);
		else if (ladderPoints < 17000) sprintf(_lDivisionStr, "Gold - Rank %d", rank);
		else if (ladderPoints < 21000) sprintf(_lDivisionStr, "Platinum - Rank %d", rank);
		else sprintf(_lDivisionStr, "Diamond - Rank %d", rank);

		sprintf(_lPointsStr, "Ladder Points: %d", m_player[pIdx].ladderPoints);
	}


	_bStatsReady = true;

	// Refresh Everything
	g_gameAppView->Invalidate();
#endif
}

void CFindMatchCtrl::ShowLadderStats(CDC *pDC)
{
#ifndef MAIN_SERVER
	if (!g_gameAppView) return;
	if (!_bStatsReady) return;

	// Set up Font
	pDC->SetBkMode(TRANSPARENT);
	pDC->SetBkColor(RGB(0, 0, 0));

	pDC->SelectObject(&_blackBrush);
	

	
	

	pDC->SetTextColor(RGB(255, 255, 255));


	// Get Ladder Wins, Losses & High Breaks

	// Get our player index
	int pIdx = g_gameAppView->m_lobby->GetPlayerIdx(g_gameAppView->m_playerSockIdx);
	if (pIdx == -1) return;
	
	
	

	// Division or Games Remaining Info
	if (_lGamesRemaining > 0)
	{
		pDC->SelectObject(&_fntStats);
		pDC->TextOut(_x + 18, _y + 152, "ladder points given");
		pDC->TextOut(_x + 18, _y + 152 + 14, _lDivisionStr);
	}
	else
	{
		pDC->SelectObject(&_fntStatsBold);
		pDC->TextOut(_x + 18, _y + 152, _lDivisionStr);




		pDC->SelectObject(&_fntStats);

		// Draw Points Info
		pDC->SetTextColor(RGB(255, 255, 255));
		pDC->TextOut(_x + 18, _y + 152 + 16 + 4, _lPointsStr);

		char buf[80];


		// Draw Ladder Stats
		sprintf(buf, "Games: %d", (_lWins + _lLosses));
		pDC->SetTextColor(RGB(255, 255, 255));
		pDC->TextOut(_x + 18, _y + 152 + 32 + 4, buf);

		sprintf(buf, "Wins: %d", _lWins);
		pDC->SetTextColor(RGB(255, 255, 255));
		pDC->TextOut(_x + 18, _y + 152 + 48 + 4, buf);

		sprintf(buf, "Losses: %d", _lLosses);
		pDC->SetTextColor(RGB(255, 255, 255));
		pDC->TextOut(_x + 18, _y + 152 + 64 + 4, buf);

		sprintf(buf, "High Break: %d", _lHighBreak);
		pDC->SetTextColor(RGB(255, 255, 255));
		pDC->TextOut(_x + 18, _y + 152 + 80 + 4, buf);

		sprintf(buf, "High Break (pro): %d", _lHighBreakPro);
		pDC->SetTextColor(RGB(255, 255, 255));
		pDC->TextOut(_x + 18, _y + 152 + 96 + 4, buf);
	}


	// Restore Font / Brush
	pDC->SelectObject(_pOldFont);
	pDC->SelectObject(_oldBlackBrush);
#endif
}


void CFindMatchCtrl::OnMouseHover()
{
#ifndef MAIN_SERVER
	_bMouseOverSurface = true;

	if (_findMatchStatus == 0)
	{
		_findMatchBtn->OnMouseHover();
	}

	// If we're searching, then detections for cancel button is on
	if (_findMatchStatus == 1)
	{
		_findMatchCancelBtn->OnMouseHover();
	}

	// Only process check boxes if we're not looking for a match
	if (_findMatchStatus == 0)
	{
		if (_bRaceToTwoChecked) _raceToTwoCheckBoxOnSpr->OnMouseHover();
		else _raceToTwoCheckBoxSpr->OnMouseHover();

		if (_bProChecked) _proCheckBoxOnSpr->OnMouseHover();
		else _proCheckBoxSpr->OnMouseHover();

		if (_bExactMatchChecked) _exactMatchCheckBoxOnSpr->OnMouseHover();
		else _exactMatchCheckBoxSpr->OnMouseHover();
	}
	

#endif
}

void CFindMatchCtrl::OnMouseLeave()
{
#ifndef MAIN_SERVER
	_bMouseOverSurface = false;

	if (_findMatchStatus == 0)
	{
		_findMatchBtn->OnMouseLeave();
	}

	// If we're searching, then detections for cancel button is on
	if (_findMatchStatus == 1)
	{
		_findMatchCancelBtn->OnMouseLeave();
	}

	// Only process check boxes if we're not looking for a match
	if (_findMatchStatus == 0)
	{
		if (_bRaceToTwoChecked) _raceToTwoCheckBoxOnSpr->OnMouseLeave();
		else _raceToTwoCheckBoxSpr->OnMouseLeave();

		if (_bProChecked) _proCheckBoxOnSpr->OnMouseLeave();
		else _proCheckBoxSpr->OnMouseLeave();

		if (_bExactMatchChecked) _exactMatchCheckBoxOnSpr->OnMouseLeave();
		else _exactMatchCheckBoxSpr->OnMouseLeave();
	}
#endif
}

void CFindMatchCtrl::OnLButtonDown()
{
#ifndef MAIN_SERVER
	_bMouseLButtonDown = true;

	if (_findMatchStatus == 0)
	{
		_findMatchBtn->OnLButtonDown();
	}
	

	// If we're searching, then detections for cancel button is on
	if (_findMatchStatus == 1)
	{
		_findMatchCancelBtn->OnLButtonDown();
	}
	

	// Only process check boxes if we're not looking for a match
	if (_findMatchStatus == 0)
	{
		if (_bRaceToTwoChecked) _raceToTwoCheckBoxOnSpr->OnLButtonDown();
		else _raceToTwoCheckBoxSpr->OnLButtonDown();

		if (_bProChecked) _proCheckBoxOnSpr->OnLButtonDown();
		else _proCheckBoxSpr->OnLButtonDown();


		if (_bExactMatchChecked) _exactMatchCheckBoxOnSpr->OnLButtonDown();
		else _exactMatchCheckBoxSpr->OnLButtonDown();
	}
	
#endif
}

// www.dnb.ca
// ref: 230163498


void CFindMatchCtrl::UpdateStatus(int status)
{
#ifndef MAIN_SERVER

	_findMatchStatus = status & 0xf;	// First 4 bites represent our Find Match Status

	switch (_findMatchStatus)
	{
		case -1:// Don't show anything... (just logged in or inside a game)
				break;
		case 0:	// Not Searching.. Show the 'Find Match' Button
				sprintf(_findMatchStatusText, "");
				break;
		case 1:	// Searching for Match. Remove 'Find Match' Button and Show Searching Button & Cancel Button
				sprintf(_findMatchStatusText, "Searching.");

				break;
		case 2:	// Found Match.. Show Starting in 3,2,1 text...
				sprintf(_findMatchStatusText, "Starting Match.");
				break;

	}
#endif
	// Refresh Everything
	g_gameAppView->Invalidate();
}

void CFindMatchCtrl::Process()
{
#ifndef MAIN_SERVER
	_ticker++;

	ProcessMouse();

	_findMatchBtn->Process();
	_findMatchStatusSpr->Process();
	_findMatchCancelBtn->Process();

	if (_findMatchStatus == 0)
	{
		_raceToTwoCheckBoxSpr->Process();
		_raceToTwoCheckBoxOnSpr->Process();

		_proCheckBoxSpr->Process();
		_proCheckBoxOnSpr->Process();

		_exactMatchCheckBoxSpr->Process();
		_exactMatchCheckBoxOnSpr->Process();
	}
	

	
	// Find Match Clicked!!
	if (_findMatchBtn->_bMouseDown)
	{
		// If we are Guest or Trialist, give message that only Members may player Ladder Matches
		// Register at www.thesnookerclub.com/register.php to play Ladder Matches & Win Prizes
		// this option is not available for guests

		// Get our Player Index
		int plrIdx = g_gameAppView->m_lobby->GetPlayerIdx(g_gameAppView->m_playerSockIdx);
		if (plrIdx == -1) return;

		if (m_player[plrIdx].bIsGuest || (m_player[plrIdx].state&PLRSTATE_GUESTMEMBER))
		{
			char _bufferx[MAX_CHARS];
			sprintf(_bufferx, "*** Sorry, only Members can play Ladder Matches. Register at http://www.thesnookerclub.com/register.php and Ladder up to win prizes! ***");
			g_gameAppView->Message(_bufferx, 10); // personal message

			// Set bMouseDown status to false, so we don't do this more than once. This is really a Click action
			_findMatchBtn->_bMouseDown = false;

			return;
		}

		// If Tournament Lobby Don't allow ladder Match.
		if (g_bTournamentLobby)
		{
			char _bufferx[MAX_CHARS];
			sprintf(_bufferx, "*** Sorry, Ladder Matches can only be played in Main Lobbies! ***");
			g_gameAppView->Message(_bufferx, 10); // personal message

			// Set bMouseDown status to false, so we don't do this more than once. This is really a Click action
			_findMatchBtn->_bMouseDown = false;

			return;
		}



		// Tell Server We're looking for a Match
		if (g_gameAppView)
		{
			// Inform Server that we're looking for a Match
			g_gameAppView->SendServerFindMatch();

			// Immediately change us to Searching Status
			UpdateStatus(1);

			// Need to make sure buttons is deselected when we click it..
			_findMatchBtn->OnMouseLeave();
		}

		// Set bMouseDown status to false, so we don't do this more than once. This is really a Click action
		_findMatchBtn->_bMouseDown = false;
	}


	// Cancel Find Match Clicked!! 
	if (_findMatchCancelBtn->_bMouseDown)
	{
		// Tell Server We''ve cancelled our Search for a Match
		if (g_gameAppView)
		{
			// Inform Server that we've cancelled our search for a match
			g_gameAppView->SendServerFindMatchCancel();

			// Immediately Update Status
			UpdateStatus(0);

			// Need to make sure buttons is deselected when we click it..
			_findMatchCancelBtn->OnMouseLeave();

			// CreateLadderStatsWindow();

		}



		// Set bMouseDown status to false, so we don't do this more than once. This is really a Click action
		_findMatchCancelBtn->_bMouseDown = false;
	}


	// Ladder Check Box
	if (_bRaceToTwoChecked)
	{
		if (_raceToTwoCheckBoxOnSpr->_bMouseDown)
		{
			// Toggle Button, so unchecked button will now show only
			_bRaceToTwoChecked = false;
			_raceToTwoCheckBoxOnSpr->_bMouseDown = false;
		}
	}
	else
	{
		if (_raceToTwoCheckBoxSpr->_bMouseDown)
		{
			// Toggle Button, so unchecked button will now show only
			_bRaceToTwoChecked = true;
			_raceToTwoCheckBoxSpr->_bMouseDown = false;
		}
	}

	// Pro Check Box
	if (_bProChecked)
	{
		if (_proCheckBoxOnSpr->_bMouseDown)
		{
			// Toggle Button, so unchecked button will now show only
			_bProChecked = false;
			_proCheckBoxOnSpr->_bMouseDown = false;
		}
	}
	else
	{
		if (_proCheckBoxSpr->_bMouseDown)
		{
			// Toggle Button, so unchecked button will now show only
			_bProChecked = true;
			_proCheckBoxSpr->_bMouseDown = false;
		}
	}

	// Exact Match Check Box
	if (_bExactMatchChecked)
	{
		if (_exactMatchCheckBoxOnSpr->_bMouseDown)
		{
			// Toggle Button, so unchecked button will now show only
			_bExactMatchChecked = false;
			_exactMatchCheckBoxOnSpr->_bMouseDown = false;
		}
	}
	else
	{
		if (_exactMatchCheckBoxSpr->_bMouseDown)
		{
			// Toggle Button, so unchecked button will now show only
			_bExactMatchChecked = true;
			_exactMatchCheckBoxSpr->_bMouseDown = false;
		}
	}

#endif
}

void CFindMatchCtrl::OnOneSecondTimer()
{
	// Update Find Match Status

	switch (_findMatchStatus)
	{
	case -1:
			// Inactive (either we've just logged in or inside a game.. so Don't show anything
		break;
	case 0: // No Status Information
		break;

	case 1:	// Searching For Match 

		
		_tickerSeconds++;
		if (_tickerSeconds >= 3) _tickerSeconds = 0;

		if (_tickerSeconds == 0) sprintf(_findMatchStatusText, "Searching.");
		else if (_tickerSeconds == 1) sprintf(_findMatchStatusText, "Searching..");
		else if (_tickerSeconds == 2) sprintf(_findMatchStatusText, "Searching...");
		else;

		_findMatchStatusSpr->_bRedraw = true;
	
		break;
	case 2:	// Found Match
		_tickerSeconds++;
		if (_tickerSeconds >= 3) _tickerSeconds = 0;

		if (_tickerSeconds == 0) sprintf(_findMatchStatusText, "Starting Match.");
		else if (_tickerSeconds == 1) sprintf(_findMatchStatusText, "Starting Match..");
		else if (_tickerSeconds == 2) sprintf(_findMatchStatusText, "Starting Match...");
		else;
		_findMatchStatusSpr->_bRedraw = true;
		break;
	}
}

void CFindMatchCtrl::ProcessMouse()
{
#ifndef MAIN_SERVER
	if (!g_gameAppView) return;


	/*
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
	}
	*/

#endif

}

HWND hChildWnd;
HINSTANCE hChildInstance = NULL;

void CFindMatchCtrl::CreateLadderStatsWindow()
{
#ifndef MAIN_SERVER
	g_gameAppView->CreateStatsWindow();
#endif	
}

