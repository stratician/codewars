
#include "../CodeSupp/build.h"
#include "../CodeSupp/stdafx.h"
#include "../CodeSupp/GameAppView.h"	
#include "../Codedx9/font.h"
#include <math.h>

#include "ChalkChanger.h"


char *chalk_names[] = {"Blue", "Pink", "Brown", "Gray", "Green", "Orange", "Black", "Purple", "Red", "Tan", "White"};
char *chalk_names_id[] = {"chalk_blue", "chalk_pink", "chalk_brown", "chalk_gray", "chalk_green", "chalk_orange", "chalk_black", "chalk_purple", "chalk_red", "chalk_tan", "chalk_white"};

#ifndef THE_POOL_CLUB
	int chalk_prices[] = {0, 1000, 1000, 1100, 1100, 1200, 1200, 1300, 1300, 1400, 1500}; // iSnooker Cue Prices
#else
	int chalk_prices[] = {0, 300, 300, 350, 350, 400, 400, 450, 450, 500, 500};	// iPool Prices
#endif

int chalk_levels[] = {1, 8, 10, 12, 14, 16, 18, 22, 26, 32, 36};

DWORD chalk_col[] = {0x4848FF, 0xFF77BA, 0x673E0D, 0x656565, 0x51EC72, 0xFF7E00, 0x666666, 0x9F48CC, 0xFB3939, 0xDF9742, 0xFFFFFF};


CChalkChanger::CChalkChanger(int level)
{
	access_level = level;
}


void CChalkChanger::Init(CWnd *pParent)
{
#ifndef MAIN_SERVER

	m_pParentWnd = pParent;

	// Set up our Chalk Names & Prices

	HRESULT hr;
	D3DSURFACE_DESC desc;

	bInActive = false;
	pos_indent = 20;
	bShow = false;
	bLevelReq = true;
	bShowLevelReq = false;

	bLButton = false;


	width = 450;
	height = 330;

	pos_xorig = 50;
	pos_yorig = 0;

	pos_x = pos_xorig;
	pos_y = pos_yorig;

	// Load our Chalk images
	chalk_type = 0;
	chalks_tex = new IDirect3DTexture9 *[MAX_CHALKS];

	hr = D3DXCreateTextureFromFile(g_pDevice, "Graphics/images/chalks/Chalk_Blue.png", &chalks_tex[0]);
	hr = D3DXCreateTextureFromFile(g_pDevice, "Graphics/images/chalks/Chalk_Pink.png", &chalks_tex[1]);
	hr = D3DXCreateTextureFromFile(g_pDevice, "Graphics/images/chalks/Chalk_Brown.png", &chalks_tex[2]);
	hr = D3DXCreateTextureFromFile(g_pDevice, "Graphics/images/chalks/Chalk_Gray.png", &chalks_tex[3]);
	hr = D3DXCreateTextureFromFile(g_pDevice, "Graphics/images/chalks/Chalk_LightGreen.png", &chalks_tex[4]);
	hr = D3DXCreateTextureFromFile(g_pDevice, "Graphics/images/chalks/Chalk_Orange.png", &chalks_tex[5]);
	hr = D3DXCreateTextureFromFile(g_pDevice, "Graphics/images/chalks/Chalk_Black.png", &chalks_tex[6]);
	hr = D3DXCreateTextureFromFile(g_pDevice, "Graphics/images/chalks/Chalk_Purple.png", &chalks_tex[7]);
	hr = D3DXCreateTextureFromFile(g_pDevice, "Graphics/images/chalks/Chalk_Red.png", &chalks_tex[8]);
	hr = D3DXCreateTextureFromFile(g_pDevice, "Graphics/images/chalks/Chalk_Tan.png", &chalks_tex[9]);
	hr = D3DXCreateTextureFromFile(g_pDevice, "Graphics/images/chalks/Chalk_White.png", &chalks_tex[10]);

	hr = D3DXCreateTextureFromFile(g_pDevice, "Graphics/images/chalks/Chalk_Glow.png", &chalks_glow_tex);
	hr = D3DXCreateTextureFromFile(g_pDevice, "Graphics/images/chalks/Chalk_void.png", &chalk_void_tex);
	hr = D3DXCreateTextureFromFile(g_pDevice, "Graphics/images/chalks/Chalk_select.png", &chalk_select_tex);
	
	
	chalks_tex[0]->GetLevelDesc(0, &desc);
	chalk_width = desc.Width * 2.0;
	chalk_height = desc.Height * 1.7;

	chalk_index = 0;
	chalk_index_selected = 0;
	chalk_select = 0;
	chalk_select_target = 0;
		
	chalk_select_y = 0;
	chalk_select_vy = 0;



	chalk_posyorig = pos_y - height + 130;

	chalk_posx = pos_x + 130;
	chalk_posy = chalk_posyorig;
	chalk_alpha = 1.0;

	

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

	// CHalk List Texture
	hr = D3DXCreateTextureFromFile(g_pDevice, "Graphics/images/chalks/Chalk_list.png", &chalks_list_tex);


	// Load our ChalkChanger tab images 
	
	hr = D3DXCreateTextureFromFile(g_pDevice, "Graphics/images/chalk_tab.png", &chalks_tab_tex);
	hr = D3DXCreateTextureFromFile(g_pDevice, "Graphics/images/chalk_tab_mo.png", &chalks_tab_mo_tex);
	hr = D3DXCreateTextureFromFile(g_pDevice, "Graphics/images/chalk_tab_off.png", &chalks_tab_off_tex);
	
	
	chalks_tab_tex->GetLevelDesc(0, &desc);
	
	chalks_tab_width = desc.Width * 2;
	chalks_tab_height = desc.Height * 2;

	chalks_tab_x = pos_x + width/2 - chalks_tab_width / 2.6;
	chalks_tab_y = pos_y + 12;

	bMouseOver_chalks_tab = false;
	bMouseOver_uparrow = false;
	bMouseOver_downarrow = false;
	bMouseOver_select = false;
	bMouseOver_buy = false;

	bMouseCursorHand = false;
	chalk_new_y = chalk_posyorig;
	chalk_new_vy = 0.0;
	ticker = 0.0;

	col = chalk_col[0];

	sel_x = pos_x + 10;
	sel_y = pos_y - 28;
	sel_vx = 0.0;
	sel_vy = 0.0;

#endif
}


CChalkChanger::~CChalkChanger(void)
{
}


void CChalkChanger::RenderLevelAccess(void)
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

		M2d_platedraw(chalks_tab_x-4, chalks_tab_y + 30, 106, 26);

		// "Level Required" Text
		char buf[80];
		sprintf(buf, "Level %d", access_level);
		g_gameAppView->g_pFont1->Render(buf, chalks_tab_x + 2, chalks_tab_y + 6, 0.0f, 1.0f, 0.3f, 0.3f, MGFX_FNTDRAW_LEFT);

	}
#endif
}

bool CChalkChanger::HasAccessTab(void)
{

	if(g_gameAppView->m_playerSockIdx != -1)
	if(g_gameAppView->m_playerIdx >= 0) 
	if(m_player[g_gameAppView->m_playerIdx].level >= access_level) return true;

	return false;
}


bool CChalkChanger::HasAccessChalk(int idx)
{
	if(HasAccessTab())
	{
		if(idx == 0) return true;

		if(g_gameAppView->m_playerSockIdx != -1)
		if(g_gameAppView->m_playerIdx >= 0) 
		if(m_player[g_gameAppView->m_playerIdx].level >= chalk_levels[idx]) return true;
	}
	
	return false;
}



void CChalkChanger::Render(void)
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


		

		// Draw Chalk
		chalk_posyorig = pos_y - height + 120;

		if(HasAccessChalk(chalk_type) == false)
		{
			M2d_sprdrawScale(chalks_tex[chalk_type], chalk_posx, chalk_posy, chalk_width, chalk_height, 0, chalk_alpha * 0.5);
		}
		else
		{
			M2d_sprdrawScale(chalks_tex[chalk_type], chalk_posx, chalk_posy, chalk_width, chalk_height, 0, chalk_alpha);
		}


		
	

		upArrow_posx = pos_x + chalk_width + 40;
		upArrow_posy = pos_y - height + 115 - 25;

		downArrow_posx = pos_x + chalk_width + 40;
		downArrow_posy = pos_y - height + 115 + 40;



		// Draw Up Arrow
		M2d_sprdrawScale(upArrow_tex[bMouseOver_uparrow], upArrow_posx, upArrow_posy, 26*3, 14*2.0, 0);

		// Draw Down Arrow
		M2d_sprdrawScale(downArrow_tex[bMouseOver_downarrow], downArrow_posx, downArrow_posy, 26*3, 14*2.0, 0);
	
		
		// Render Chalk List
		M2d_sprdrawScale(chalks_list_tex, pos_x + 10, pos_y - 28, 352*2, 32*2, 0);

		// Draw Plate Over Chalks that we don't yet have
		for(int i=0;i<MAX_CHALKS;i++)
		if (PurchasedChalk(i) == false || (HasAccessChalk(i) == false) )
		{
			M2d_sprdrawScale(chalk_void_tex, pos_x + 10 + i * 38.5, pos_y - 28, 31*2, 32.5*2, 0.0, 0.7);
		}
		
		// Draw our selection cursor
		
		
		if(sel_x != (pos_x + 10 + chalk_type * 38.5) )
		{
			sel_vx = ((pos_x + 10 + chalk_type * 38.5) - sel_x);
			sel_vx *= 0.1;
			sel_x += sel_vx;

			if(sel_vx < 0.01 && sel_vx > 0.01) 
			{
				sel_x = (pos_x + 10 + chalk_type * 38.5);
				sel_vx = 0;
			}
		}
		
		
		// sel_x = pos_x + 10;
		sel_y = pos_y - 28;

		M2d_sprdrawScale(chalk_select_tex, sel_x, sel_y, 31*2, 32.5*2, 0.0, 0.6);
		
	



		// RenderChalkList(upArrow_posx + 20, upArrow_posy - 20);

		// Draw CHalk Info Text
		if (PurchasedChalk(chalk_index) == false)
		{
			sprintf(info_tf, "%d. %s - $%.2f %s", (chalk_index+1), chalk_names[chalk_index], ((float)(chalk_prices[chalk_index]) / 100.0f), "vCash");
		}
		else
		{
			sprintf(info_tf, "%d. %s", (chalk_index+1), chalk_names[chalk_index]);
		}

		g_gameAppView->g_pFont2->Render(info_tf, pos_x + 205, chalk_posyorig + 75, 0.0f, chalk_alpha, 0.30f, 0.30f, MGFX_FNTDRAW_CENTER);


		// Draw Select or Buy Button (depending on if we have Purchased the Cue or Selected it or not
		if(chalk_index == chalk_index_selected)
		{
			// Draw Cue Glow (if we have chalk selected
			M2d_sprdrawScale(chalks_glow_tex, chalk_posx-4, chalk_posy, chalk_width+16, chalk_height+12, 0, (0.5 + 0.4*sinf(ticker)) * chalk_alpha);
		}
		else
		{

			select_posx = pos_x + 200 - 35;
			select_posy = chalk_posyorig + 120;
	
			buy_posx = pos_x + 200 - 35;
			buy_posy = chalk_posyorig + 120;

			// If we don't have the required level yet, then Display Level Required Text
			if(HasAccessChalk(chalk_type) == false)
			{	
				sprintf(info_tf, "Level %d Required", chalk_levels[chalk_index]);
				g_gameAppView->g_pFont2->Render(info_tf, pos_x + 210, chalk_posyorig + 110, 0.0f, chalk_alpha * 0.5, 0.25f, 0.25f, MGFX_FNTDRAW_CENTER);
			}
			else
			{
				// If we have Purchased, show the Select Button
				if(PurchasedChalk(chalk_index))
				{
					M2d_sprdrawScale(select_tex[bMouseOver_select], select_posx, select_posy, select_width, select_height, 0);
				}
				else
				{
					M2d_sprdrawScale(buy_tex[bMouseOver_buy], buy_posx, buy_posy, buy_width, buy_height, 0);
				}
			}
		}

	}
	
	float pAlpha = 1.0f;
	if(bInActive) pAlpha = 0.75f;

	// Draw ChalkChanger Tab
	if(bLevelReq == true)
	{
		M2d_sprdrawScale(chalks_tab_off_tex, chalks_tab_x, chalks_tab_y, chalks_tab_width, chalks_tab_height, 0, 1.0f * pAlpha);

		if(bShowLevelReq)
		{
			// Draw Level Access Overlay
			RenderLevelAccess();
		}
	}
	else
	{
		if(bMouseOver_chalks_tab == false)
		{
			M2d_sprdrawScale(chalks_tab_tex, chalks_tab_x, chalks_tab_y, chalks_tab_width, chalks_tab_height, 0, 1.0f * pAlpha);
		}
		else
		{
			M2d_sprdrawScale(chalks_tab_mo_tex, chalks_tab_x, chalks_tab_y, chalks_tab_width, chalks_tab_height, 0, 1.0f * pAlpha);
		}
	}
	


}


/*
 * Render a vertical list of all Chalks
 */

/*
void CChalkChanger::RenderChalkList(int x, int y)
{
	int i;
	for(i=0;i<MAX_CHALKS;i++)
	{
		//M2d_sprdraw(chalks_tex[i], x, y + i*22, 20, 20, 0.0, 0.0);
		M2d_sprdraw(chalks_tex[0], x, y + i*70, 64, 64, 0);
	}
}
*/


bool CChalkChanger::PurchasedChalk(int chalkIdx)
{

	if(chalkIdx==0) return true;

	// Remember our own Idx is always 0
	// Find our playerIdx
	if(g_gameAppView->m_playerSockIdx != -1)
	if(g_gameAppView->m_playerIdx >= 0) 
	{
		return (m_player[g_gameAppView->m_playerIdx].chalktype&(1<<(16+chalkIdx) ));
	}

	/*
	// debug code
	if( chalkIdx == 0) return true;
	else if( chalkIdx == 2) return true;
	else if( chalkIdx == 4) return true;
	else return false;
	*/
	
	return false;
}


void CChalkChanger::Process(void)
{
	if(!g_gameAppView) return;
	
	bLevelReq = true;
	if(HasAccessTab()) bLevelReq = false;
	

	MouseDetections();


	// Contrl the ChalkChanger Sliding On and Off

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
	if (chalk_select < chalk_select_target)
	{
		if (chalk_new_y == chalk_posyorig) 
		{
			SetChalk(chalk_select, chalk_select + 1);
		}
	}
	else if (chalk_select > chalk_select_target)
	{	
		if (chalk_new_y == chalk_posyorig) 
		{
			SetChalk(chalk_select, chalk_select - 1);
		}
	}	
			
			
			
	// Move Cue to new position if changed
	if (chalk_new_y < chalk_posyorig && chalk_new_vy >= 0) 
	{
		if (chalk_select == chalk_select_target)
		{
			chalk_new_vy = (chalk_posyorig - chalk_new_y) * 0.1;
			if (chalk_new_vy < 0.1) chalk_new_vy = 0.1;
		}
		else
		{
			chalk_new_vy = (60) * 0.1;
		}
				
		chalk_new_vy *= 0.9;
		chalk_new_y += chalk_new_vy;
	}
	else if (chalk_new_y > chalk_posyorig && chalk_new_vy <= 0) 
	{
		if (chalk_select == chalk_select_target)
		{
			chalk_new_vy = -(chalk_new_y - chalk_posyorig) * 0.1;
			if (chalk_new_vy > -0.1) chalk_new_vy = -0.1;
		}
		else
		{
			chalk_new_vy = -(60) * 0.1;
		}
				
		chalk_new_vy *= 0.9;
		chalk_new_y += chalk_new_vy;
	}
	else
	{
		chalk_new_y = chalk_posyorig;
		chalk_new_vy = 0.0;
	}
			
	chalk_posy = chalk_new_y;
	float dist = (chalk_posyorig - 120);
	if(dist < 0.0) dist = -dist;

	chalk_alpha = 1.0 - (dist) / 120.0;
			
	ticker += 0.06;

}


void CChalkChanger::InitChalk(int index)
{

	chalk_index = index;
	chalk_select = chalk_index;
	chalk_select_target = chalk_select;
	SelectChalk(chalk_index);
	chalk_type = chalk_select;
	

}




void CChalkChanger::MouseDetections(void)
{
	// Only Process this if we aren't in process of our shot (i.e. if we're not aiming)

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
	
	
	if(ChalkTabDetections(mx, my)) 
	{
		// If we have Clicked on the Tab, then make sure other tabes are hidden
		g_gameAppView->m_cueChanger->bShow = false;
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

	// Note if we clicked anywhere outside of the ChalkChanger, then Hide it
	
	if(bShow)
	if(g_gameAppView->m_bLButtonDown)
	if(MouseOver(mx, my) == false)
	{
		bShow = false;
	}

	
}


bool CChalkChanger::MouseOver(int mx, int my)
{
	// Note if we clicked anywhere outside of the ChalkChanger, then Hide it
	if(bShow)
	{
		if(mx < (pos_x + width) && mx > (pos_x) && my > (pos_y-height) && my < (pos_y) )
		{
			return true;
		}
	}

	return false;
}

bool CChalkChanger::ChalkTabDetections(int mx, int my)
{
	
	bShowLevelReq = false;

	// Mouse Detections for chalks_tab
	bMouseOver_chalks_tab = false;

	if(g_gameAppView->m_lobby->m_bInLobby == false)
	{
		if(my > (chalks_tab_y - 14) && my < (chalks_tab_y+14)) 
		{
			if(mx > (chalks_tab_x) && mx < (chalks_tab_x +chalks_tab_width/1.6) ) 
			{

				// Display Access Level Required Text by default if we mouse over, unless we are online and meet the level requirements
				bShowLevelReq = true;

				if(HasAccessTab())
				{
					bShowLevelReq = false;
				}
				else
				{
					return false;
				}

				bMouseOver_chalks_tab = true;

				
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


void CChalkChanger::UpArrowDetections(int mx, int my)
{
	// Mouse Detections for chalks_tab
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

void CChalkChanger::DownArrowDetections(int mx, int my)
{
	// Mouse Detections for chalks_tab
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



void CChalkChanger::SelectDetections(int mx, int my)
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


void CChalkChanger::BuyDetections(int mx, int my)
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

void CChalkChanger::SelectChalk(int idx)
{
	chalk_index_selected = idx;
	col = chalk_col[idx];
}

void CChalkChanger::OnSelectClick(void)
{
	/*
	SelectChalk(chalk_index);
	m_player[0].chalktype = chalk_index;
	return;
	
	*/

	char msg[128];

	
	if (PurchasedChalk(chalk_index) == true)
	{
		// SelectCue(chalk_index);
		sprintf(msg, "/chalk %s", chalk_names_id[chalk_index]);
		mainShell->ChatSend(msg);
	}

	// Hide Tab after we've selected Chalk
	bShow = false;
}

void CChalkChanger::OnBuyClick(void)
{
	if(!g_gameAppView) return;

	char msg[128];

	if (PurchasedChalk(chalk_index) == false)
	{
		// Check that we have enough Virtual Cash first
		if (m_player[g_gameAppView->m_playerIdx].cash < chalk_prices[chalk_index])
		{
			sprintf(msg, "You don't have enough Virtual Cash ($%.2f vCash is required to purchase %s chalk", (float)(chalk_prices[chalk_index]) / 100.0f, chalk_names[chalk_index]);
			g_gameAppView->Message(msg, 90); // personal message	
			return;
		}
		else
		{
			// Request Purchase & Change at Server
			sprintf(msg, "/buy %s", chalk_names_id[chalk_index]);
			mainShell->ChatSend(msg);

			
		}

	}
}	





void CChalkChanger::OnUpClick(void)
{
	chalk_select_target--;
}
		

void CChalkChanger::OnDownClick(void)
{
	chalk_select_target++;
}


void CChalkChanger::SetChalk(int oldIdx, int newIdx)
{
	
			
	int mod_idx = newIdx;
			
	while (mod_idx < 0) 
	{
		mod_idx += MAX_CHALKS;
	}
			
	while (mod_idx >= MAX_CHALKS) 
	{
		mod_idx -= MAX_CHALKS;
	}
			
	chalk_index = mod_idx;
			
	chalk_type = chalk_index;
	
		
	if (newIdx > oldIdx)
	{
		chalk_new_y = chalk_posyorig - 60;
	}
	else
	{
		chalk_new_y = chalk_posyorig + 60;
	}
			
	/*		
	// Change Button
	while (button_spr.numChildren)
	{
		button_spr.removeChildAt(0);
	}
	if (CMain.clientShell.m_users.PurchasedCue(0, chalk_index) == true)
	{
		button_spr.addChild(select_bmp);
	}
	else
	{
		button_spr.addChild(buynow_bmp);
	}
			
	// If we have this chalk selected, then hide button mode
	if (chalk_index == chalk_index_selected)
	{
		button_spr.visible = false;
	}
	else
	{
		button_spr.visible = true;
	}
		
	*/
			
	chalk_select = newIdx;
			
	/*
	info_tf.text = chalk_names[chalk_index];
	if (CMain.clientShell.m_users.PurchasedCue(0, chalk_index) == false)
	{
		info_tf.appendText(" - $" + (chalk_prices[chalk_index] / 100).toFixed(2) + " vCash");
	}

	info_tf.x = (surface.width / 2) - info_tf.width / 2;
	button_spr.x = info_tf.x + info_tf.width + 10;
	*/		
			
							
}