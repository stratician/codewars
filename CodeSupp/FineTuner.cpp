
#include "../CodeSupp/build.h"
#include "../CodeSupp/stdafx.h"
#include "../CodeSupp/GameAppView.h"	
#include "../Codedx9/font.h"
#include <math.h>


#include "FineTuner.h"


CFineTuner::CFineTuner(int level)
{

	access_level = level;
}


float sliderScale;
float sliderScaleDelta;
float alphatab;

void CFineTuner::Init(CWnd *pParent, int x, int y)
{
#ifndef MAIN_SERVER

	m_pParentWnd = pParent;

	// Set up our Cue Names & Prices

	HRESULT hr;
	D3DSURFACE_DESC desc;

	bLevelReq = true;
	bShowLevelReq = false;

	tunerSliderMouseDown = false;
	alphatab = 0.3f;
	//public var cue_prices:Array = new Array(0, 3000, 4000, 4000, 5000, 5000, 15000, 30000);

	posx_orig = 20;
	posy_orig = 400;

	posx = 20;
	posy = 400;

	bMouseOver = false;
	HideDelay = 30;	// 30 frames before Display hides if we aren't mouse over

	tuner_x = 20;
	tuner_y = 400;
	tuner_width = 20;
	tuner_height = 220;

	tunerSlider_x = tuner_x;
	tunerSlider_y = tuner_y + tuner_height/2;

	tunerSliderBoundXmin = tunerSlider_x - 20;
	tunerSliderBoundXmax = tunerSlider_x + 40;

	tunerSliderBoundYmin = tunerSlider_y - 130;
	tunerSliderBoundYmax = tunerSlider_y + 130;

	alpha = 0.0f;

	sliderScale = 1.0;
	sliderScaleDelta = 0.0f;
	// Load our images
	
	hr = D3DXCreateTextureFromFile(g_pDevice, "Graphics/images/finetuner_color_bg.png", &fineTuner_bg_tex);
	hr = D3DXCreateTextureFromFile(g_pDevice, "Graphics/images/finetuner_new.png", &fineTuner_slider_tex);
	hr = D3DXCreateTextureFromFile(g_pDevice, "Graphics/images/finetuner_tab.png", &fineTuner_tab);
	

#endif
}


CFineTuner::~CFineTuner(void)
{
}


void CFineTuner::RenderLevelAccess(void)
{

if(bShowLevelReq == false)  return;

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

		M2d_platedraw(2, tunerSlider_y-20, 67, 20);

		// "Level Required" Text
		char buf[80];
		sprintf(buf, "lvl %d", access_level);
		g_gameAppView->g_pFont1->Render(buf, 2, tunerSlider_y - 18-20, 0.0f, 1.0f, 0.60f, 0.60f, MGFX_FNTDRAW_LEFT);

	}
#endif
}

bool CFineTuner::HasAccess(void)
{
	return true;
	if(g_gameAppView->m_playerSockIdx != -1)
	if(g_gameAppView->m_playerIdx >= 0) 
	if(m_player[g_gameAppView->m_playerIdx].level >= access_level) return true;

	return false;
}



// If Slider is Active, then make the scale increase a bit
bool bJustActive = false;
bool bAnimateReady = false;

void CFineTuner::Render(void)
{
	// Draw Fine Tuner Slider
	float width = 16*2 * sliderScale;
	float height = 25*2 * sliderScale;
	M2d_sprdrawScale(fineTuner_tab, posx_orig-12, posy_orig + tuner_height/2, width, height, 0, (1-alpha) * alphatab);
	

	RenderLevelAccess();

	if(alpha <= 0.0f) return;

	if(!g_gameAppView) return;


	char buffer[128];
	DWORD lcol;
	DWORD rcol;

	// Background //
	lcol = D3DCOLOR_RGBA(4,4,4,(int)(128 * alpha));
	rcol = D3DCOLOR_RGBA(32,32,32,(int)(128 * alpha));
	
	M3d_objectset_LVERTEX(&M3d_platedraw_PLATE,3,0,0,0,lcol,lcol,0,1);
	M3d_objectset_LVERTEX(&M3d_platedraw_PLATE,2,1,0,0,rcol,rcol,1,1);
	M3d_objectset_LVERTEX(&M3d_platedraw_PLATE,1,1,1,0,rcol,rcol,1,0);
	M3d_objectset_LVERTEX(&M3d_platedraw_PLATE,0,0,1,0,lcol,lcol,0,0);
	
	

	// Draw Fine Tuner Background
	M2d_platedraw(tuner_x - 8, tuner_y + tuner_height + 40, tuner_width + 20, tuner_height + 80);

	/*
	float spralpha = 1.0f;
	if(bLevelReq) spralpha = 0.5f;

	alpha *= spralpha;
	*/
	

	// Draw Fine Tuner
	M2d_sprdrawScale(fineTuner_bg_tex, tuner_x, tuner_y + tuner_height/2, tuner_width*2, tuner_height*2.2, 0, alpha);

	

	M2d_sprdrawScale(fineTuner_slider_tex, tunerSlider_x + 2, tunerSlider_y, width, height, 0, alpha);
	
	
	if(g_gameAppView->m_snooker->m_cueBallSelect == 0) 
	{
		bJustActive = false;
		bAnimateReady = true;
	}

	if(g_gameAppView->m_snooker->m_cueBallSelect == 5 && bJustActive == false)
	{
		bJustActive = true;
		sliderScaleDelta = 0.15;
	}

	

	// When Slider is just active make it's size increase
	if(bJustActive && bAnimateReady)
	{
		sliderScale += sliderScaleDelta;
		sliderScaleDelta -= 0.02;

		
		if(sliderScale < 1)
		{
			sliderScale = 1;
			bJustActive = false;
			bAnimateReady = false;
		}

	}


	
}


void CFineTuner::Reset(bool bLineSet)
{
	// Back up 
	m_sPointOrig[0] = g_gameAppView->m_snooker->m_sPoint[0];
	m_sPointOrig[1] = g_gameAppView->m_snooker->m_sPoint[1];

	m_tableMousePosOrig[0] = g_gameAppView->m_snooker->m_tableMousePos[0];
	m_tableMousePosOrig[1] = g_gameAppView->m_snooker->m_tableMousePos[1];

	if(bLineSet == true)
	{
		posx = -30;
		posvx = 0.0;

		// if(g_gameAppView) ErrorMessage(g_gameAppView, "Reset (LineSet)");
	}

}


void CFineTuner::Process(void)
{
	if(!g_gameAppView) return;
	
	
	bLevelReq = true;
	if(HasAccess()) bLevelReq = false;
	

	MouseDetections();

	
	if(g_gameAppView->m_snooker->m_cueBallSelect==0)
	{
		tunerSlider_y = tuner_y + tuner_height/2;
	}
	

	// If it's our Turn and cueBallSellect is 5, then increase alpha until 1.0, otherwise, decrease alpha until 0.0
	if(g_gameAppView->m_snooker->MyTurn() && g_gameAppView->m_snooker->m_cueBallSelect == 5)
	{
		// if(alpha < 1.0) alpha += 0.05;
	
		if(bLevelReq == false) alphatab = 1.0f;

		posx += posvx;
		posvx *= 0.95;

		// Update Positions

		if(bMouseOver)
		{
			if(posx < 20) posvx += 0.5;
		}
		else
		{
			if(tunerSliderMouseDown == false)
			{
				if(HideDelay) HideDelay--;
			}

			if(HideDelay == 0)
			{
				if(posx > -30) posvx -= 0.5;
			}
		}
	
		// Boundaries
		if(posx > 20)
		{
			posvx = 0.0;
			posx = 20;
			HideDelay = 60;
		}

		if(posx < -30)
		{
			posvx = 0.0;
			posx = -30.0;
		}

		alpha = (posx + 30.0f) / 50.0f;
		alpha *= alpha;

		tuner_x = posx;
		tuner_y = posy;

		tunerSlider_x = tuner_x;
		// tunerSlider_y = tuner_y + tuner_height/2;
	}
	else
	{
		if(alpha > 0.0) alpha -= 0.05;

		alphatab = 0.4f;
	}

	
}


void CFineTuner::MouseDetections(void)
{


	D3DVIEWPORT9 mainViewPort;
	g_pDevice->GetViewport(&mainViewPort);

	// Find the relative position that we clicked (1600:800) range //
	int mx = (int)(((g_gameAppView->m_snooker->m_mouseX) / (float)mainViewPort.Width) * 1600.0f);
	int my = (int)(((g_gameAppView->m_snooker->m_mouseY) / (float)mainViewPort.Height) * 800.0f);

	

	// Note if we mouse over Slider then Display Level required if we don't have access to this
	bShowLevelReq = false;
	bMouseOver = false;

	
	if(mx <= (posx_orig+30) )
	{
		if(my >= (posy_orig - 100) && my <= (posy_orig + 300) )
		{
			if(bLevelReq == true)
			{
				if(mx <= (posx_orig) && mx >= (posx_orig-20))
				if(my >= (tunerSlider_y - 20) && my <= (tunerSlider_y + 20) )
				{
					bShowLevelReq = true;
				}
			}
			else
			{
				bMouseOver = true;
			}
		}
	}
	

	if(bLevelReq == true) return;

	// Only allow this control if it's our Shot and we have set the shot..
	if(g_gameAppView->m_snooker->MyTurn() == false) return;
	if(g_gameAppView->m_snooker->m_cueBallSelect!=5) return;


	
	

	// Check if we clicked on our Slider
	
	if(g_gameAppView->m_bLButtonDown)
	{
		if(mx >= (tunerSlider_x-20) && mx<= (tunerSlider_x+40) )
		{
			if(my >= (tunerSlider_y - 20) && my <= (tunerSlider_y + 20) )
			{
				tunerSliderMouseDown = true;
			}
		}
	}
	else
	{
		tunerSliderMouseDown = false;
	}

	if(tunerSliderMouseDown)
	{
		if(my >= tunerSliderBoundYmin && my <= tunerSliderBoundYmax)
		{
			tunerSlider_y = my;


			float rx = m_tableMousePosOrig[0] - m_sPointOrig[0];
			float ry = m_tableMousePosOrig[1] - m_sPointOrig[1];

			float nx, ny;

			float deltaY = (tunerSliderBoundYmin + 0.5 * (tunerSliderBoundYmax - tunerSliderBoundYmin)) - my;

			CVector vect;
			if(g_build.gameType == GAMETYPE_SNOOKER)
			{
				vect.x = g_gameAppView->m_snooker->m_testBall->m_P.x - g_gameAppView->m_snooker->m_ball[g_gameAppView->m_snooker->m_selectBall]->m_P.x;
				vect.y = g_gameAppView->m_snooker->m_testBall->m_P.y - g_gameAppView->m_snooker->m_ball[g_gameAppView->m_snooker->m_selectBall]->m_P.y;
				vect.z = g_gameAppView->m_snooker->m_testBall->m_P.z - g_gameAppView->m_snooker->m_ball[g_gameAppView->m_snooker->m_selectBall]->m_P.z;
			}
			else
			{
				vect.x = (float)FROMFIX(g_gameAppView->m_snooker->m_testBall->f_P.x) - (float)FROMFIX(g_gameAppView->m_snooker->m_ball[g_gameAppView->m_snooker->m_selectBall]->f_P.x);
				vect.y = (float)FROMFIX(g_gameAppView->m_snooker->m_testBall->f_P.y) - (float)FROMFIX(g_gameAppView->m_snooker->m_ball[g_gameAppView->m_snooker->m_selectBall]->f_P.y);
				vect.z = (float)FROMFIX(g_gameAppView->m_snooker->m_testBall->f_P.z) - (float)FROMFIX(g_gameAppView->m_snooker->m_ball[g_gameAppView->m_snooker->m_selectBall]->f_P.z);
			}

			float dist = vect.Magnitude();

			// Scale Fine Tuner precision by the Mouse Sensitivity
			float mult = g_gfxDetails.iOptionsSensitivity;
			if(mult < 0) mult = 1.0f;
			if(mult > 10) mult = 10.0f;
			mult = (mult / 5.0f);

			deltaY *= (0.04 * mult) /dist;

			nx = rx * cosf(deltaY) - ry * sinf(deltaY);
			ny = rx * sinf(deltaY) + ry * cosf(deltaY);
			

			g_gameAppView->m_snooker->m_tableMousePos[0] = m_sPointOrig[0] + nx;
			g_gameAppView->m_snooker->m_tableMousePos[1] = m_sPointOrig[1] + ny;
			
			
			CVector m_cueTipStrikeDir = CVector(g_gameAppView->m_snooker->m_sPoint[0] - g_gameAppView->m_snooker->m_tableMousePos[0],
									g_gameAppView->m_snooker->m_sPoint[1] - g_gameAppView->m_snooker->m_tableMousePos[1],
									g_gameAppView->m_snooker->m_sPoint[2] - g_gameAppView->m_snooker->m_tableMousePos[2]);

			g_gameAppView->m_snooker->m_cueAngle = atan2((double)-m_cueTipStrikeDir.y, (double)-m_cueTipStrikeDir.x);
	

			// Update Test Ball
			


			/*
			// cue-power 0-100% boundary circle
			float rx = m_mouseX - m_sCPoint.x;
			float ry = m_mouseY - m_sCPoint.y;

			float r = rx*rx + ry*ry;
			float rTest = screenWidthFactor * 375.0f;
			if(r > rTest*rTest) 
			{
				// if outside circle - then move back to boundary edge of circle
				CVector R(rx, ry, 0);
				R = R.Unit();
				R = R * rTest;
				m_mouseX = m_sCPoint.x + R.x;
				m_mouseY = m_sCPoint.y + R.y;
			}
			*/


		}
	}

	/*
	if(mx >= tunerSliderBoundXmin && mx <= tunerSliderBoundXmax)
	{
		if(my >= tunerSliderBoundYmin && my <= tunerSliderBoundYmax)
		{
			tunerSlider_y = my;
		}
	}
	*/
}

