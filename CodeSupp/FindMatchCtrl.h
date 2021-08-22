#pragma once

#include "Sprite.h"
#include <afxwin.h>

class CFindMatchCtrl
{

public:
	CDC _dcMemBG;
	CDC *_pDC;

	HBITMAP _bgBitmap;

	CSprite *_findMatchBtn;
	CSprite *_findMatchStatusSpr;
	CSprite *_findMatchCancelBtn;
	CSprite *_raceToTwoCheckBoxSpr;
	CSprite *_raceToTwoCheckBoxOnSpr;
	CSprite *_proCheckBoxSpr;
	CSprite *_proCheckBoxOnSpr;

	CSprite *_exactMatchCheckBoxSpr;
	CSprite *_exactMatchCheckBoxOnSpr;

	char _lPointsStr[80];
	char _lDivisionStr[80];
	int _lPoints;
	int _lGames;
	int _lWins;
	int _lLosses;
	int _lHighBreak;
	int _lHighBreakPro;
	int _lGamesRemaining;
	bool _bStatsReady;
	

	bool _bRaceToTwoChecked;
	bool _bProChecked;
	bool _bExactMatchChecked;

	bool _findMatchClicked;

	char _findMatchStatusText[80];
	int _findMatchStatus;		// 0: Idle, 1: Searching For Match, 2: Found Match (Starting in 3,2,1 etc..)

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

	CFont _fnt;
	CFont _fntStats;
	CFont _fntStatsBold;
	CFont *_pOldFont;
	CBrush _blackBrush;
	CBrush *_oldBlackBrush;

	int _ticker;
	int _tickerSeconds;

public:
	CFindMatchCtrl();
	~CFindMatchCtrl();

	void SetPosition(int xpos, int ypos, int width, int height);
	void Init(CDC *pDC);
	void Draw(CDC *PDC);
	void Update(CDC *pDC);
	void Process();
	void ProcessMouse();
	void OnMouseLeave();
	void OnMouseHover();
	void OnLButtonDown();
	void UpdateStatus(int status);
	void OnOneSecondTimer();

	void UpdateLadderStats();
	void ShowLadderStats(CDC *pDC);


	void CreateLadderStatsWindow();

};

