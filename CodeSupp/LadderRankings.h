#pragma once
#include "WindowBase.h"
#include <Windows.h>
#include <tchar.h>
#include "../Code/lobby.h"
#include <vector>
#include <array>

#include "../Codedx9/font.h"




// Achiemvenet Object




typedef float vector3D[3];





class CLadderRankings : public CWindowBase 
{

public:

	void MouseDown(bool bState)
	{
		bMouseDown = bState;
		iMouseOldX = iMouseX;
		iMouseOldY = iMouseY;
	}

	void MouseMove(int x, int y)
	{
		iMouseX = x;
		iMouseY = y;
	}

private:


	CWnd *m_pParent;


	IDirect3DTexture9 *rankingsBGTex;
	IDirect3DTexture9 *rankingsProgressTex;
	LPD3DXSPRITE rankingsBGSpr;

	Font *m_pFontA;
	Font *m_pFontB;

	// Mouse Controls
	bool bMouseDown = false;
	int iMouseX = -1;
	int iMouseY = -1;
	int iMouseOldX = -1;
	int iMouseOldY = -1;
	
	bool bInitialised = false;
	

	int x, y;
	int width, height;

	// Array stores our ladder rankings data
	std::array<ladderObject, 20> ladderRanks; // (show top 20 for now)

	
	D3DXVECTOR3 position;
	D3DCOLOR color;

	HWND m_hWnd;
	
	int m_maxAchievements;

	// DWORD backgroundCol = D3DCOLOR_COLORVALUE(0.0f, 0.3f, 0.4f, 1.0f);
	DWORD backgroundCol = D3DCOLOR_COLORVALUE(0.3f, 0.3f, 0.3f, 1.0f);
	

public:
	CLadderRankings(); // Show the top 10 Ladder Rankings, and then our ladder rankings with 5 above and below us. (21 Rankings in total)
	~CLadderRankings();

	void PassParent(CWnd *pParent);
	
	static LRESULT CALLBACK WndProcBaseLadder(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

	int Create(HINSTANCE hInstance, HWND parentWnd, char *title, int x, int y, int w, int h, std::array<ladderObject, 20> ladderRanks);
	void Render2D();
	void RenderLadderRank(char *name, int ladderPoints, int ladderRank, int x, int y);
	void RenderLadderRankings();
	void UpdateLadderRankings(char *title, std::array<ladderObject, 20> ladderRanks);
	void SaveResults();
	void OnClose();
	void UpdatePlayerStats();

};


