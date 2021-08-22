
#include "../CodeSupp/build.h"
#include "../CodeSupp/stdafx.h"
#include "../CodeSupp/GameAppView.h"	
#include "../Codedx9/font.h"
#include <math.h>

#include "CueChanger.h"


char *cue_names[] = {"House", "Black & White", "Blue Lightning", "Proton Pink", "Purple Chrome", "Wood", "Dungeon", "Dark Knight"};
char *cue_names_id[] = {"house", "blackwhite", "bluelightning", "protonpink", "purplechrome", "wood", "dungeon", "darkknight"};

#ifndef THE_POOL_CLUB
	int cue_prices[] = {0, 3000, 4000, 4000, 5000, 5000, 15000, 30000}; // iSnooker Cue Prices
#else
	int cue_prices[] = {0, 1000, 1200, 1200, 1200, 1500, 2000, 5000};  // iPool Cue Prices
#endif

CCueChanger::CCueChanger(int level)
{
	access_level = level;
}


void CCueChanger::Init(CWnd *pParent)
{
#ifndef MAIN_SERVER

	m_pParentWnd = pParent;

	// Set up our Cue Names & Prices

	HRESULT hr;
	D3DSURFACE_DESC desc;

	bInActive = false;
	pos_indent = 20;
	bShow = false;
	bLevelReq = true;
	bShowLevelReq = false;

	//public var cue_prices:Array = new Array(0, 3000, 4000, 4000, 5000, 5000, 15000, 30000);

	width = 670;
	height = 300;

	pos_xorig = 140;
	pos_yorig = 300-height;

	pos_x = pos_xorig;
	pos_y = pos_yorig;

	// Load our Cue images
	cue_type = 0;
	cues_tex = new IDirect3DTexture9 *[MAX_CUES];

	hr = D3DXCreateTextureFromFile(g_pDevice, "Graphics/images/cues/cue_house.png", &cues_tex[0]);
	hr = D3DXCreateTextureFromFile(g_pDevice, "Graphics/images/cues/cue_blackwhite.png", &cues_tex[1]);
	hr = D3DXCreateTextureFromFile(g_pDevice, "Graphics/images/cues/cue_bluelightning.png", &cues_tex[2]);
	hr = D3DXCreateTextureFromFile(g_pDevice, "Graphics/images/cues/cue_protonpink.png", &cues_tex[3]);
	hr = D3DXCreateTextureFromFile(g_pDevice, "Graphics/images/cues/cue_purplechrome.png", &cues_tex[4]);
	hr = D3DXCreateTextureFromFile(g_pDevice, "Graphics/images/cues/cue_wood.png", &cues_tex[5]);
	hr = D3DXCreateTextureFromFile(g_pDevice, "Graphics/images/cues/cue_dungeon.png", &cues_tex[6]);
	hr = D3DXCreateTextureFromFile(g_pDevice, "Graphics/images/cues/cue_darkknight.png", &cues_tex[7]);

	hr = D3DXCreateTextureFromFile(g_pDevice, "Graphics/images/cues/cue_glow.png", &cue_glow_tex);
	
	cues_tex[0]->GetLevelDesc(0, &desc);
	cue_width = desc.Width * 2.0;
	cue_height = desc.Height * 1.7;

	cue_index = 0;
	cue_index_selected = 0;
	cue_select = 0;
	cue_select_target = 0;
		
	cue_select_y = 0;
	cue_select_vy = 0;



	cue_posyorig = pos_y - height + 120;

	cue_posx = pos_x + 20;
	cue_posy = cue_posyorig;
	cue_alpha = 1.0;

	

	// UpArrow (for Cue Changer)
	
	upArrow_tex = new IDirect3DTexture9 *[2];
	hr = D3DXCreateTextureFromFile(g_pDevice, "Graphics/images/upsize.png", &upArrow_tex[0]);
	hr = D3DXCreateTextureFromFile(g_pDevice, "Graphics/images/upsize_mo.png", &upArrow_tex[1]);
	
	downArrow_tex = new IDirect3DTexture9 *[2];
	hr = D3DXCreateTextureFromFile(g_pDevice, "Graphics/images/downsize.png", &downArrow_tex[0]);
	hr = D3DXCreateTextureFromFile(g_pDevice, "Graphics/images/downsize_mo.png", &downArrow_tex[1]);


	// Select Button
	select_tex = new IDirect3DTexture9 *[2];
	hr = D3DXCreateTextureFromFile(g_pDevice, "Graphics/images/select.png", &select_tex[0]);
	hr = D3DXCreateTextureFromFile(g_pDevice, "Graphics/images/select_mo.png", &select_tex[1]);
	select_width = 70*2.0;
	select_height = 20*2.0;

	// Buy Button
	buy_tex = new IDirect3DTexture9 *[2];
	hr = D3DXCreateTextureFromFile(g_pDevice, "Graphics/images/buy.png", &buy_tex[0]);
	hr = D3DXCreateTextureFromFile(g_pDevice, "Graphics/images/buy_mo.png", &buy_tex[1]);
	buy_width = 70*2.0;
	buy_height = 20*2.0;


	// Load our CueChanger tab images 
	
	hr = D3DXCreateTextureFromFile(g_pDevice, "Graphics/images/cues_tab.png", &cues_tab_tex);
	hr = D3DXCreateTextureFromFile(g_pDevice, "Graphics/images/cues_tab_mo.png", &cues_tab_mo_tex);
	hr = D3DXCreateTextureFromFile(g_pDevice, "Graphics/images/cues_tab_off.png", &cues_tab_off_tex);
	
	
	cues_tab_tex->GetLevelDesc(0, &desc);
	
	cues_tab_width = desc.Width * 2;
	cues_tab_height = desc.Height * 2;

	cues_tab_x = pos_x + width/2 - cues_tab_width / 2.6;
	cues_tab_y = pos_y + 12;

	bMouseOver_cues_tab = false;
	bMouseOver_uparrow = false;
	bMouseOver_downarrow = false;
	bMouseOver_select = false;
	bMouseOver_buy = false;

	bMouseCursorHand = false;
	cue_new_y = cue_posyorig;
	cue_new_vy = 0.0;
	ticker = 0.0;
#endif
}


CCueChanger::~CCueChanger(void)
{
}


void CCueChanger::RenderLevelAccess(void)
{
#ifndef MAIN_SERVER
	if(!g_gameAppView) return;
	{
	
		// Render Text on Background stating Level Required to access this tab
		char buffer[128];
		DWORD lcol;
		DWORD rcol;

		// Background //
		lcol = D3DCOLOR_RGBA(4,4,4,192);
		rcol = D3DCOLOR_RGBA(32,32,32,192);
	
		M3d_objectset_LVERTEX(&M3d_platedraw_PLATE,3,0,0,0,lcol,lcol,0,1);
		M3d_objectset_LVERTEX(&M3d_platedraw_PLATE,2,1,0,0,rcol,rcol,1,1);
		M3d_objectset_LVERTEX(&M3d_platedraw_PLATE,1,1,1,0,rcol,rcol,1,0);
		M3d_objectset_LVERTEX(&M3d_platedraw_PLATE,0,0,1,0,lcol,lcol,0,0);

		M2d_platedraw(cues_tab_x, cues_tab_y + 30, 96, 26);

		// "Level Required" Text
		char buf[80];
		sprintf(buf, "Level %d", access_level);
		g_gameAppView->g_pFont1->Render(buf, cues_tab_x + 7, cues_tab_y + 5, 0.0f, 1.0f, 0.3f, 0.3f, MGFX_FNTDRAW_LEFT);

	}
#endif
}

bool CCueChanger::HasAccess(void)
{

	if(g_gameAppView->m_playerSockIdx != -1)
	if(g_gameAppView->m_playerIdx >= 0) 
	if(m_player[g_gameAppView->m_playerIdx].level >= access_level) return true;

	return false;
}



void CCueChanger::Render(void)
{

	if(!g_gameAppView) return;
	
	
	

	if(pos_y > pos_yorig)
	{



		char buffer[128];
		DWORD lcol;
		DWORD rcol;

		// Background //
		lcol = D3DCOLOR_RGBA(4,4,4,128);
		rcol = D3DCOLOR_RGBA(32,32,32,128);
	
		M3d_objectset_LVERTEX(&M3d_platedraw_PLATE,3,0,0,0,lcol,lcol,0,1);
		M3d_objectset_LVERTEX(&M3d_platedraw_PLATE,2,1,0,0,rcol,rcol,1,1);
		M3d_objectset_LVERTEX(&M3d_platedraw_PLATE,1,1,1,0,rcol,rcol,1,0);
		M3d_objectset_LVERTEX(&M3d_platedraw_PLATE,0,0,1,0,lcol,lcol,0,0);
	
	
	

		// Draw Level Bar Background
		M2d_platedraw(pos_x, pos_y, width, height);


		

		// Draw Cues
		cue_posyorig = pos_y - height + 120;

		M2d_sprdrawScale(cues_tex[cue_type], cue_posx, cue_posy, cue_width, cue_height, 0, cue_alpha);

	

		upArrow_posx = pos_x + width/2 - 26*1.25;
		upArrow_posy = pos_y - height + 120-45;

		downArrow_posx = pos_x + width/2 - 26*1.25;
		downArrow_posy = pos_y - height + 120 + 60;

		// Draw Up Arrow
		M2d_sprdrawScale(upArrow_tex[bMouseOver_uparrow], upArrow_posx, upArrow_posy, 26*3, 14*2.0, 0);

		// Draw Down Arrow
		M2d_sprdrawScale(downArrow_tex[bMouseOver_downarrow], downArrow_posx, downArrow_posy, 26*3, 14*2.0, 0);
	


		// Draw Cue Info Text
		if (PurchasedCue(cue_index) == false)
		{
			sprintf(info_tf, "%d. %s - $%.2f %s", (cue_index+1), cue_names[cue_index], ((float)(cue_prices[cue_index]) / 100.0f), "vCash");
		}
		else
		{
			sprintf(info_tf, "%d. %s", (cue_index+1), cue_names[cue_index]);
		}

		g_gameAppView->g_pFont2->Render(info_tf, pos_x + width/2 - 20, cue_posyorig + 15, 0.0f, cue_alpha, 0.30f, 0.30f, MGFX_FNTDRAW_CENTER);


		// Draw Select or Buy Button (depending on if we have Purchased the Cue or Selected it or not
		if(cue_index == cue_index_selected)
		{
			// Draw Cue Glow (if we have cue selected
			M2d_sprdrawScale(cue_glow_tex, cue_posx-4, cue_posy, cue_width+16, cue_height+12, 0, 0.5 + 0.4*sinf(ticker));
		}
		else
		{

			select_posx = pos_x + width - 140;
			select_posy = pos_y - height + 200;
	
			buy_posx = pos_x + width - 140;
			buy_posy = pos_y - height + 200;


			// If we have Purchased, show the Select Button
			if(PurchasedCue(cue_index))
			{
				M2d_sprdrawScale(select_tex[bMouseOver_select], select_posx, select_posy, select_width, select_height, 0);
			}
			else
			{
				M2d_sprdrawScale(buy_tex[bMouseOver_buy], buy_posx, buy_posy, buy_width, buy_height, 0);
			}
		}

	}
	
	float pAlpha = 1.0f;
	if(bInActive) pAlpha = 0.75f;

	// Draw CueChanger Tab
	if(bLevelReq == true)
	{
		M2d_sprdrawScale(cues_tab_off_tex, cues_tab_x, cues_tab_y, cues_tab_width, cues_tab_height, 0, 1.0f * pAlpha);

		if(bShowLevelReq)
		{
			// Draw Level Access Overlay
			RenderLevelAccess();
		}
	}
	else
	{
		if(bMouseOver_cues_tab == false)
		{
			M2d_sprdrawScale(cues_tab_tex, cues_tab_x, cues_tab_y, cues_tab_width, cues_tab_height, 0, 1.0f * pAlpha);
		}
		else
		{
			M2d_sprdrawScale(cues_tab_mo_tex, cues_tab_x, cues_tab_y, cues_tab_width, cues_tab_height, 0, 1.0f * pAlpha);
		}
	}
	


}


bool CCueChanger::PurchasedCue(int cueIdx)
{
	// return true;

	// Remember our own Idx is always 0
	// Find our playerIdx
	if(g_gameAppView->m_playerSockIdx != -1)
	if(g_gameAppView->m_playerIdx >= 0) 
	{
		if(cueIdx==0) return true;
		else return (m_player[g_gameAppView->m_playerIdx].inventory&(1<<(16+cueIdx) ));
	}

	/*
	// debug code
	if( cueIdx == 0) return true;
	else if( cueIdx == 2) return true;
	else if( cueIdx == 4) return true;
	else return false;
	*/
	
	return false;
}


void CCueChanger::Process(void)
{
	if(!g_gameAppView) return;
	
	bLevelReq = true;
	if(HasAccess()) bLevelReq = false;
	

	MouseDetections();


	// Contrl the CueChanger Sliding On and Off

	// Tab Control
	float vel;
			
	// If Game isn't started, then Slide on invitefriends object
	if (bShow == false )
	{
		if (pos_y > pos_yorig)
		{
			// Velocity if proportional to the distance from stop point
			vel = pos_y - pos_yorig;
			vel /= (height);
			vel *= 3.0;
					
			pos_vy += vel;
			pos_vy *= 0.88;
			pos_y -= pos_vy;
		}
		else
		{
			pos_vy = 0;
			pos_y = pos_yorig;
			// surface.visible = false;
		}
	}
	else
	{
		if (pos_y < (pos_yorig+height))
		{
			// surface.visible = true;
			// Velocity if proportional to the distance from stop point
			vel = (pos_yorig+height - pos_y);
			vel /= (height);
			vel *= 3.0;
					
					
			pos_vy += vel;
			pos_vy *= 0.88;
			pos_y += pos_vy;
		}
		else
		{
			pos_vy = 0;
			pos_y = pos_yorig+height;
		}	
				
	}
			
	// surface.x = pos_x;
	// surface.y = pos_y;
			
		
	// Change Target Cue
	if (cue_select < cue_select_target)
	{
		if (cue_new_y == cue_posyorig) 
		{
			SetCue(cue_select, cue_select + 1);
		}
	}
	else if (cue_select > cue_select_target)
	{	
		if (cue_new_y == cue_posyorig) 
		{
			SetCue(cue_select, cue_select - 1);
		}
	}	
			
			
			
	// Move Cue to new position if changed
	if (cue_new_y < cue_posyorig && cue_new_vy >= 0) 
	{
		if (cue_select == cue_select_target)
		{
			cue_new_vy = (cue_posyorig - cue_new_y) * 0.1;
			if (cue_new_vy < 0.1) cue_new_vy = 0.1;
		}
		else
		{
			cue_new_vy = (60) * 0.1;
		}
				
		cue_new_vy *= 0.9;
		cue_new_y += cue_new_vy;
	}
	else if (cue_new_y > cue_posyorig && cue_new_vy <= 0) 
	{
		if (cue_select == cue_select_target)
		{
			cue_new_vy = -(cue_new_y - cue_posyorig) * 0.1;
			if (cue_new_vy > -0.1) cue_new_vy = -0.1;
		}
		else
		{
			cue_new_vy = -(60) * 0.1;
		}
				
		cue_new_vy *= 0.9;
		cue_new_y += cue_new_vy;
	}
	else
	{
		cue_new_y = cue_posyorig;
		cue_new_vy = 0.0;
	}
			
	cue_posy = cue_new_y;
	float dist = (cue_posyorig - cue_posy);
	if(dist < 0.0) dist = -dist;

	cue_alpha = 1.0 - (dist) / 60.0;
			
	ticker += 0.06;

}


void CCueChanger::InitCue(int index)
{
	cue_index = index;
	cue_select = cue_index;
	cue_select_target = cue_select;
	SelectCue(cue_index);
	cue_type = cue_select;
	

}

bool bLButton = false;


void CCueChanger::MouseDetections(void)
{

	if(g_gameAppView->m_snooker->m_cueBallSelect == 1 || g_gameAppView->m_snooker->m_cueBallSelect == 5) 
	{
		if(g_gameAppView->m_snooker->MyTurn())
		{
			bInActive = true;
			return;
		}
	}
	else;

	bInActive = false;

	D3DVIEWPORT9 mainViewPort;
	g_pDevice->GetViewport(&mainViewPort);

	// Find the relative position that we clicked (1600:800) range //
	int mx = (int)(((g_gameAppView->m_snooker->m_mouseX) / (float)mainViewPort.Width) * 1600.0f);
	int my = (int)(((g_gameAppView->m_snooker->m_mouseY) / (float)mainViewPort.Height) * 800.0f);

	bLButton = false;
	
	if(CueTabDetections(mx, my)) 
	{
		// If we have Clicked on the Tab, then make sure other tabes are hidden
		g_gameAppView->m_chalkChanger->bShow = false;
	}

	if(bShow)
	if(pos_y == pos_yorig+height)
	{
		UpArrowDetections(mx, my);
		DownArrowDetections(mx, my);
		SelectDetections(mx, my);
		BuyDetections(mx, my);
	}

	if(bLButton) g_gameAppView->m_bLButtonDown = false;

	// Note if we clicked anywhere outside of the CueChanger, then Hide it
	
	if(bShow)
	if(g_gameAppView->m_bLButtonDown)
	if(MouseOver(mx, my) == false)
	{
		bShow = false;
	}
}


bool CCueChanger::MouseOver(int mx, int my)
{
	// Note if we clicked anywhere outside of the CueChanger, then Hide it
	if(bShow)
	{
		if(mx < (pos_x + width) && mx > (pos_x) && my > (pos_y-height) && my < (pos_y) )
		{
			return true;
		}
	}

	return false;
}

bool CCueChanger::CueTabDetections(int mx, int my)
{
	
	bShowLevelReq = false;

	// Mouse Detections for cues_tab
	bMouseOver_cues_tab = false;

	if(g_gameAppView->m_lobby->m_bInLobby == false)
	{
		if(my > (cues_tab_y - 14) && my < (cues_tab_y+14)) 
		{
			if(mx > (cues_tab_x) && mx < (cues_tab_x +cues_tab_width/1.6) ) 
			{

				// Display Access Level Required Text by default if we mouse over, unless we are online and meet the level requirements
				bShowLevelReq = true;

				if(HasAccess())
				{
					bShowLevelReq = false;
				}
				else
				{
					return false;
				}
				
	

				bMouseOver_cues_tab = true;


				if(g_gameAppView->m_bLButtonDown)
				{
					if(bShow == false) bShow = true;
						else bShow = false;

					bLButton = true;
					return true;
					
				}



			}
				
		}
	}

	return false;
}


void CCueChanger::UpArrowDetections(int mx, int my)
{
	// Mouse Detections for cues_tab
	bMouseOver_uparrow = false;

	if(g_gameAppView->m_lobby->m_bInLobby == false)
	{
		if(my > (upArrow_posy - 14) && my < (upArrow_posy+14)) 
		{
			if(mx > (upArrow_posx-2) && mx < (upArrow_posx + 26*2 - 2)) 
			{
				bMouseOver_uparrow = true;

				if(g_gameAppView->m_bLButtonDown)
				{
					bLButton = true;

					OnUpClick();
				}
			}
		}
	}
}

void CCueChanger::DownArrowDetections(int mx, int my)
{
	// Mouse Detections for cues_tab
	bMouseOver_downarrow = false;

	if(g_gameAppView->m_lobby->m_bInLobby == false)
	{
		if(my > (downArrow_posy - 14) && my < (downArrow_posy+14)) 
		{
			if(mx > (downArrow_posx-2) && mx < (downArrow_posx + 26*2 - 2)) 
			{
				bMouseOver_downarrow = true;

				if(g_gameAppView->m_bLButtonDown)
				{
					bLButton = true;

					OnDownClick();
				}
			}
		}
	}
}



void CCueChanger::SelectDetections(int mx, int my)
{
	// Mouse Detections for Select Button
	bMouseOver_select = false;

	if(g_gameAppView->m_lobby->m_bInLobby == false)
	{
		if(my > (select_posy - select_height/2.4) && my < (select_posy + select_height/2.4)) 
		{
			if(mx > (select_posx) && mx < (select_posx + select_width/1.6)) 
			{
				bMouseOver_select = true;

				if(g_gameAppView->m_bLButtonDown)
				{
					bLButton = true;

					OnSelectClick();
				}
			}
		}
	}
}


void CCueChanger::BuyDetections(int mx, int my)
{


	// Mouse Detections for Select Button
	bMouseOver_buy = false;

	if(g_gameAppView->m_lobby->m_bInLobby == false)
	{
		if(my > (buy_posy - buy_height/2.4) && my < (buy_posy + buy_height/2.4)) 
		{
			if(mx > (buy_posx) && mx < (buy_posx + buy_width/1.6)) 
			{
				bMouseOver_buy = true;

				if(g_gameAppView->m_bLButtonDown)
				{
					bLButton = true;

					OnBuyClick();
				}
			}
		}
	}
}

void CCueChanger::SelectCue(int idx)
{
	cue_index_selected = idx;
}

void CCueChanger::OnSelectClick(void)
{
	/*
	SelectCue(cue_index);
	m_player[0].cuetype = cue_index+1;
	return;
	*/

	char msg[128];

	
	if (PurchasedCue(cue_index) == true)
	{
		// SelectCue(cue_index);

		sprintf(msg, "/cue %s", cue_names_id[cue_index]);
		mainShell->ChatSend(msg);
	}

	// Hide Tab after we've selected Chalk
	bShow = false;
}

void CCueChanger::OnBuyClick(void)
{
	
	char msg[128];

	if (PurchasedCue(cue_index) == false)
	{
		// Check that we have enough Virtual Cash first
		if (m_player[g_gameAppView->m_playerIdx].cash < cue_prices[cue_index])
		{
			sprintf(msg, "You don't have enough Virtual Cash ($%.2f vCash is required to purchase %s cue", (float)(cue_prices[cue_index]) / 100.0f, cue_names[cue_index]);
			g_gameAppView->Message(msg, 90); // personal message	
			return;
		}
		else
		{
			// Request Purchase & Change at Server
			sprintf(msg, "/buy %s", cue_names_id[cue_index]);
			mainShell->ChatSend(msg);

			
		}

	}
}	





void CCueChanger::OnUpClick(void)
{
	cue_select_target--;
}
		

void CCueChanger::OnDownClick(void)
{
	cue_select_target++;
}


void CCueChanger::SetCue(int oldIdx, int newIdx)
{
	
			
	int mod_idx = newIdx;
			
	while (mod_idx < 0) 
	{
		mod_idx += MAX_CUES;
	}
			
	while (mod_idx >= MAX_CUES) 
	{
		mod_idx -= MAX_CUES;
	}
			
	cue_index = mod_idx;
			
	cue_type = cue_index;
	
		
	if (newIdx > oldIdx)
	{
		cue_new_y = cue_posyorig - 60;
	}
	else
	{
		cue_new_y = cue_posyorig + 60;
	}
			
	/*		
	// Change Button
	while (button_spr.numChildren)
	{
		button_spr.removeChildAt(0);
	}
	if (CMain.clientShell.m_users.PurchasedCue(0, cue_index) == true)
	{
		button_spr.addChild(select_bmp);
	}
	else
	{
		button_spr.addChild(buynow_bmp);
	}
			
	// If we have this cue selected, then hide button mode
	if (cue_index == cue_index_selected)
	{
		button_spr.visible = false;
	}
	else
	{
		button_spr.visible = true;
	}
		
	*/
			
	cue_select = newIdx;
			
	/*
	info_tf.text = cue_names[cue_index];
	if (CMain.clientShell.m_users.PurchasedCue(0, cue_index) == false)
	{
		info_tf.appendText(" - $" + (cue_prices[cue_index] / 100).toFixed(2) + " vCash");
	}

	info_tf.x = (surface.width / 2) - info_tf.width / 2;
	button_spr.x = info_tf.x + info_tf.width + 10;
	*/		
			
							
}