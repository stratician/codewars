#pragma once
#include "WindowBase.h"
#include <Windows.h>
#include <tchar.h>
#include "../Code/lobby.h"


typedef float vector3D[3];



// Sprites
extern IDirect3DTexture9 *achievementsBGTex;
extern LPD3DXSPRITE achievementsBGSpr;


class CAObject
{

public: 

	

	D3DXVECTOR3 pos;
	char *name;
	char *desc;
	int percentComplete;
	int targetPercent;
	int achievementIdx;
	int level;		// Achievement level. (some achievements have multiple levels. 

public:



	CAObject(char *name, char *desc, int idx, int level) 
	{
		percentComplete = 0;

		pos.x = 0.0f;
		pos.y = 0.0f;
		pos.z = 0.0f;

		this->level = level;

		this->name = new char[strlen(name) + 1];
		strcpy(this->name, name);

		this->desc = new char[strlen(desc) + 1];
		strcpy(this->desc, desc);

		this->achievementIdx = idx;

	};
	~CAObject() {};

	
	void SetPosition(float x, float y)
	{
		pos.x = x;
		pos.y = y;
	}


	void Render();
	void RenderStars();
	void RenderProgressBar();
	void SetPercent(int percent);
	void Process();
	
	int GetStars() 
	{
		int stars = 0;
		if (percentComplete > 0)
		{
			stars = (percentComplete - 1) / 33;
		}

		return stars;
	}
	

	int GetHeight()
	{
		D3DSURFACE_DESC desc;

		achievementsBGTex->GetLevelDesc(0, &desc);
		return desc.Height;
	}

	int GetWidth()
	{
		D3DSURFACE_DESC desc;

		achievementsBGTex->GetLevelDesc(0, &desc);
		return desc.Width;
	}


};




class CAchievements : public CWindowBase 
{

public:

	void MouseDown(bool bState)
	{
		bMouseDown = bState;

		vscrollOldPos = vscrollPos;
		iMouseOldX = iMouseX;
		iMouseOldY = iMouseY;
	}

	void MouseMove(int x, int y)
	{
		iMouseX = x;
		iMouseY = y;
	}

private:

	// Mouse Controls
	bool bMouseDown = false;
	int iMouseX = -1;
	int iMouseY = -1;
	int iMouseOldX = -1;
	int iMouseOldY = -1;
	
	bool bInitialised = false;


	int x, y;
	int width, height;

	const int maxAchievementsOnPage = 8;

	D3DXVECTOR3 position;
	D3DCOLOR color;

	HWND m_hWnd;
	
	int m_maxAchievements;
	CAObject **m_obj;

	int acheivementObjectGap = 10;
	int achievementObjectHeight;	// This is the height of the achievement + the gap. i.e. 64 + 10

	int vscrollOldPos;
	int vscrollPos;
	int vscrollHeight;
	int vscrollMaxHeight;
	int vscrollWidth = 4;


	DWORD backgroundCol = D3DCOLOR_COLORVALUE(0.9f, 0.9f, 0.9f, 1.0f);

	// Vertical Scroll Bar texture
	IDirect3DTexture9 *vScrollBarTex;

	// Clipping image (Rectangle the same color as background.. ) we place this over achivements object to give the effect of clipping
	IDirect3DTexture9 *achievementClipTex;

	// footer background
	IDirect3DTexture9 *achievementFooterTex;
	

public:
	CAchievements(int maxAchievements);
	~CAchievements();

	
	static LRESULT CALLBACK WndProcBase(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

	int Create(HINSTANCE hInstance, HWND parentWnd, char *title, int x, int y, int w, int h, Player plr);
	int CreateAchievement(char *title, char *desc, int level);

	int GetTotalAchievements();			// All achievemnts (on all levels) - some achievements have multiple levels...
	int GetTotalAchievementsCompleted();	// Get all achievement stars we have done all on levels
	
	int GetTotalAchievementsActive();	// All achievements that are currently active i.e number of achievement objects
	int GetTotalAchievementsCompletedActive();
	void ProcessVScrollBar();

	void Render2D();
	void RenderAchievements();
	void RenderVScrollBar();
	void RenderStatsSummary();
	void Process();
	void OnClose();
	

	void RemoveAllAchievements();
	void CreateAchievements(Player plr);
	void UpdateAchievements(char *title, Player plr);
	void SetAchievementPercent(int achievementIdx, int percent);
	


	


};


