#pragma once


// #include "C:\Program Files (x86)\Microsoft DirectX SDK (June 2010)\Include\d3dx9.h"
#include <string>

class CLevel
{

public: 
	
	int level;
	int level_progress;
	int level_progress_max;
	
	char level_tf[64];
	char level_xp[64];

	int ypos;
	int xpos;

	CWnd *m_pParentWnd;
	

	IDirect3DTexture9 *bgTexture;

public:
	CLevel();
	~CLevel(void);

	//void PassParent(CWnd *pParent);

	void Init(CWnd *pParent);
	void UpdateLevel(void);
	void Render(void);
	void LevelProgress(int progress);
};

