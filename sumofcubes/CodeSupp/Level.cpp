

// #include "c:/memirlib/include/memirlib.h"
#include "../CodeSupp/build.h"
#include "../CodeSupp/stdafx.h"
#include "../CodeSupp/GameAppView.h"	
#include "../Codedx9/font.h"
#include <math.h>


#include "Level.h"
// Font *g_pFont;

CLevel::CLevel(void)
{

	
}


CLevel::~CLevel(void)
{
	//d3d::Delete<Font*>(g_pFont);
}


void CLevel::Init(CWnd *pParent)
{
	m_pParentWnd = pParent;
	// ((CGameAppView *)m_pParentWnd)->Message("test", 10); // personal message	

	// HRESULT hr = D3DXCreateTextureFromFile(g_pDevice, "Graphics/images/level.png", &bgTexture);
	//HRESULT hr = D3DXCreateTextureFromFile(g_pDevice, "crate.jpg", &bgTexture);

	level = 1;
	level_progress = 10;
	level_progress_max = pow(level, 1.5f) * 100;

	sprintf(level_xp, "%d / %d xp", level_progress, level_progress_max);
	sprintf(level_tf, "level %d", level);

	if(g_build.gameType != GAMETYPE_POOL)
	{
		xpos = 850;
		ypos = 2;
	}
	else
	{
		xpos = 850;
		ypos = 2;
	}

	// LevelProgress(33);
}


void CLevel::UpdateLevel(void)
{
	
	int myPlayerIdx;
	if(((CGameAppView *)m_pParentWnd)->m_playerIdx < 0) myPlayerIdx = 0;
		else myPlayerIdx = ((CGameAppView *)m_pParentWnd)->m_playerIdx;

	
	level = ((CGameAppView *)m_pParentWnd)->m_lobby->GetPlayerLevel(myPlayerIdx);
	level_progress = ((CGameAppView *)m_pParentWnd)->m_lobby->GetPlayerLevelProgress(myPlayerIdx);
	level_progress_max = pow(level, 1.5) * 100;

	sprintf(level_xp, "%d / %d xp", level_progress, level_progress_max);
	sprintf(level_tf, "level %d", level);
}


void CLevel::Render(void)
{

	if(!g_gameAppView) return;

	char buffer[128];
	DWORD lcol;
	DWORD rcol;

	int width = 1200;
	if(g_build.gameType != GAMETYPE_POOL)
	{
		width = 500;

		// Background //
		lcol = D3DCOLOR_RGBA(220,220,220,32);
		rcol = D3DCOLOR_RGBA(192,192,192,32);
	}
	else
	{
		width = 500;
		// Background //
		lcol = D3DCOLOR_RGBA(4,4,4,128);
		rcol = D3DCOLOR_RGBA(32,32,32,128);

	}

	
	
	M3d_objectset_LVERTEX(&M3d_platedraw_PLATE,3,0,-1,0,lcol,lcol,0,1);
	M3d_objectset_LVERTEX(&M3d_platedraw_PLATE,2,1,-1,0,rcol,rcol,1,1);
	M3d_objectset_LVERTEX(&M3d_platedraw_PLATE,1,1,0,0,rcol,rcol,1,0);
	M3d_objectset_LVERTEX(&M3d_platedraw_PLATE,0,0,0,0,lcol,lcol,0,0);
	


	// Level Bar //
	lcol = D3DCOLOR_RGBA(14,69,167,255);
	rcol = D3DCOLOR_RGBA(23,103,246,255);

	// Draw Level Bar Background
	M2d_platedraw(xpos, ypos, width, 16);
	

	
	
	M3d_objectset_LVERTEX(&M3d_platedraw_PLATE,3,0,-1,0,lcol,lcol,0,1);
	M3d_objectset_LVERTEX(&M3d_platedraw_PLATE,2,1,-1,0,rcol,rcol,1,1);
	M3d_objectset_LVERTEX(&M3d_platedraw_PLATE,1,1,0,0,rcol,rcol,1,0);
	M3d_objectset_LVERTEX(&M3d_platedraw_PLATE,0,0,0,0,lcol,lcol,0,0);
	
	
	// Draw Level Bar to represent our Level Progress
	M2d_platedraw(xpos+1, ypos+1, width * ((float)level_progress / (float)level_progress_max) - 2, 16-3);

	//g_gameAppView->g_pFont2->
	
	float aspect = 1600 / 800;


	

	// XP Stats
	g_gameAppView->g_pFont1->Render(level_xp, xpos+(width*0.5), ypos-1, 0.0f, 1.0f, 0.4f, 0.4f, MGFX_FNTDRAW_CENTER);

	// Level Stats
	g_gameAppView->g_pFont1->Render(level_tf, xpos+100, ypos-1, 0.0f, 1.0f, 0.4f, 0.4f, MGFX_FNTDRAW_CENTER);



	//M2d_fntdraw(&g_gameAppView->m_snooker->m_font, "365 1200 ", MGFX_FNTDRAW_CENTER);
	//g_gameAppView->g_pFont2->Render(m_snooker->m_replayname2, seatOffsetX, seatOffsetY, 0.0f, 1.0f, FontSize, FontSize, MGFX_FNTDRAW_CENTER);

	//g_pFont->Render("VIEW TABLES (Click To Play)", 800, 200, 0.0f, 1.0f, 1.4f/aspect, 1.4f, MGFX_FNTDRAW_CENTER);

	//D3DSURFACE_DESC desc;
	//bgTexture->GetLevelDesc(0, &desc);
	//M2d_sprdraw(bgTexture, 200, 200, 2400, 16, 0);
	//M2d_platedraw(150, 200+15, desc.Width * 0.45f, 14);
	
}



void CLevel::LevelProgress(int progress)
{
			
	level_progress += progress;
			
	// Check if we've reached a new level
	if (level_progress >= level_progress_max)
	{
		level_progress -= level_progress_max;
		level++;
		level_progress_max = pow(level, 1.5) * 100;
				
		// We've reached a new level... Update Users of this

		int myPlayerIdx = g_gameAppView->m_playerIdx;
		myPlayerIdx = 0;
		// level = g_gameAppView->m_lobby->GetPlayerLevel(myPlayerIdx);

		g_gameAppView->m_lobby->SetPlayerLevel(myPlayerIdx, level);
		char buffer[256];
		sprintf(buffer, "Congratulcations! You have reached Level %d, The Higher your level, the more features you can access (i.e. Cue Purchasing, Text Colors, Betting etc.)", level);
		g_gameAppView->Message(buffer, 10); // personal message	
		
	}
	
	sprintf(level_xp, "%d / %d xp", level_progress, level_progress_max);
	sprintf(level_tf, "level %d", level);
		
			
}