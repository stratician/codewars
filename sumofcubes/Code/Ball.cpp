///////////////////////////////////////////////////////////////////////////////////////////////////
// CBall (Ball.cpp)
//

#include <math.h>

#include "../CodeSupp/build.h"
#include "../CodeSupp/stdafx.h"
#include "../CodeSupp/GameAppView.h"


#include "Common.h"
#include "Ball.h"


extern FILE *fptr2;

float BALL_ENERGYFACTOR;//							0.98f
float BALL_ENERGYFACTOR_N;//						0.93 // 0.835f
float BALL_ENERGYFACTOR_T;//						0.93 // 0.835f
float BALL_CUSHIONFACTOR;
float BALL_ROLLINGFRICT;
float BALL_ROLLINGFACTOR;

///////////////////////////////////////////////////////////////////////////////////////////////////
//
//
CBall::CBall(CSnooker *pParent)
{
#ifndef MAIN_SERVER

	m_parent = pParent;
	m_ghost = 0;
	m_cyc = 0;
	m_timeDelta = 0.0f;
	D3DXMatrixTranslation(&m_d3dmtx, 0.0f, 0.0f, 0.0f);
	//D3DUtil_SetTranslateMatrix(m_d3dmtx, 0,0,0);
#endif 
}


///////////////////////////////////////////////////////////////////////////////////////////////////
//
//
CBall::~CBall()
{
}

///////////////////////////////////////////////////////////////////////////////////////////////////
//
//
//void CBall::Init(float x, float y, float z, int colour, float rx, float ry, float rz)
void CBall::Init(float x, float y, float z, int colour, float rx, float ry, float rz, float spotx, float spoty)
{
#ifndef MAIN_SERVER

	m_cyc = 0;


	// physical parameters
	m_flags = 0;
	SetFlag(m_flags, Flg_BALL_ACTIVE);
//	m_diameter = 1.0625;				// 2(1/16)" +- 0.005"
//	m_weight = 5.0;						// 5.0oz - 5.5oz
	m_colour = colour;
	m_hitOtherBall = -1;
	m_hitCushion = 0;
	m_bHitCushionBeforeBall = FALSE;
	PlayedBallAroundPocketSFX = FALSE;

	// save initial position parameters
	//m_PSpot = m_POld = m_P = CVector(-x, -y, -(z-BALL_RADIUS));
	//m_PSpot = m_POld = m_P = CVector(-x, -y, -(z-BALL_RADIUS*2.8));
	float f = z - BALL_POSITIONZOFFSET;
	// no linear velocity
	m_PUndo = m_PSpot = m_POld = m_P = CVector(-x, -y, -f);
	m_V = m_VOld = CVector(0, 0, 0);

	// override respot position?
	if(spotx < 10000 && spoty < 10000)
	{
		m_PSpot.x = -spotx;
		m_PSpot.y = -spoty;
	}
					
	// no angular velocity
	m_W = CVector(0, 0, 0);
	m_wnroll = 0;

	m_WDelay = 0;//m_WDelay * 30; [NEWOAP]

	m_wSide = 0;

	
	m_FadeOut = 200;
	m_InsidePocket = 0;

	// FIXED POINT VALUES ----------- //
	f_PUndo = f_PSpot = f_POld = f_P = FVector(TOFIX(-x), TOFIX(-y), TOFIX(-f));

	// override respot position? (FIXPOINT)
	if(spotx < 10000 && spoty < 10000)
	{
		f_PSpot.x = TOFIX(-spotx);
		f_PSpot.y = TOFIX(-spoty);
	}

	// no linear velocity
	f_V = f_VOld = FVector(0, 0, 0);
					
	// no angular velocity
	f_W = FVector(0, 0, 0);

	f_wnroll = 0;

	f_wSide = 0;
	// ------------------------------ //
	
	m_bMouseOver = FALSE;

	m_justHitAnotherBall = 0;
	m_justHitCushion = 0;
	
	if(rx == 0 && ry == 0 && rz == 0)
	{ // Dont Spin! - Leave as it was
	}
	else
	{ // Spin Ball!
	//D3DUtil_SetTranslateMatrix(m_d3dmtx, 0,0,0);
	D3DXMatrixTranslation(&m_d3dmtx, 0, 0, 0);

	//D3DUtil_SetRotateXMatrix(m_d3dmtxV, rx);
	D3DXMatrixRotationX(&m_d3dmtxV, rx);
	m_d3dmtx = m_d3dmtx * m_d3dmtxV;
	//MultiplyMatrices(m_m3dmtx,m_m3dmtx, m_m3dmtxV);

	//D3DUtil_SetRotateYMatrix(m_d3dmtxV, ry);
	D3DXMatrixRotationY(&m_d3dmtxV, ry);
	m_d3dmtx = m_d3dmtx * m_d3dmtxV;
	//MultiplyMatrices(m_m3dmtx,m_m3dmtx, m_m3dmtxV);

	//D3DUtil_SetRotateZMatrix(m_d3dmtxV, rz);
	D3DXMatrixRotationZ(&m_d3dmtxV, rz);
	m_d3dmtx = m_d3dmtx * m_d3dmtxV;
	//MultiplyMatrices(m_m3dmtx,m_m3dmtx, m_m3dmtxV);
	}

#endif 
}


///////////////////////////////////////////////////////////////////////////////////////////////////
//
//
void CBall::Deinit(void)
{

}


///////////////////////////////////////////////////////////////////////////////////////////////////
//
//
BOOL CBall::Proximity(float x, float y, float z, float r)
{
	if(!TestFlag(m_flags, Flg_BALL_ACTIVE))
		return FALSE;


#ifdef FIXEDMODE
	FVector V;
	V.x = f_P.x - TOFIX(x); 
	V.y = f_P.y - TOFIX(y);
	V.z = 0;
	if(V.Magnitude2()<TOFIX(r*r)) return TRUE;
	return FALSE;
#else

	//float radius2 = (BALL_RADIUS*BALL_RADIUS); //(36)*(36); // 3.6*100 = 360
	float radius2 = r * r;

	// is point inside ball
	float d;
	float dx,dy;
	dx = (m_P.x) - x;
	dx*=dx;
	dy = (m_P.y) - y;
	dy*=dy;
	
	// ignore Z-plane for now, assume we're on the perfect plane for now!
	
	d = dx+dy;
	if(d < radius2) return TRUE;
	else return FALSE;
#endif
}

///////////////////////////////////////////////////////////////////////////////////////////////////
//
//
//#if 0
#ifdef FIXEDMODE
void CBall::Strike(float sx,float sy,float sz, 
				   float ex,float ey,float ez, 
				   M3d_vtx cuetip, float rnd,
				   BOOL bTestBall, BOOL bIsBot)
{

	
	// This must also be called up in the StrikeBallReceive function (for people receiving the shot) //
	g_bTextDoubleErrorAdded = FALSE;

	// convert all parameters to fixpoint params '_'
	UpdateFixeds();
	__fix _sx = TOFIX(sx);
	__fix _sy = TOFIX(sy);
	__fix _sz = TOFIX(sz);
	__fix _ex = TOFIX(ex);
	__fix _ey = TOFIX(ey);
	__fix _ez = TOFIX(ez);
	__fix _cuetip[3];
	_cuetip[0] = TOFIX(cuetip[0]);
	_cuetip[1] = TOFIX(cuetip[1]);
	_cuetip[2] = TOFIX(cuetip[2]);
	__fix _rnd = TOFIX(rnd);

	/////
		
	__fix f1, f2, f3;	// [#]

	__fix cmag;
	FVector C, CUnit;
	__fix maxspeed = TOFIX(BALL_MAXSPEED);

	__fix angleError = 0;


		

	// get cue strike vector
	C = FVector(_sx - _ex, _sy - _ey, _sz - _ez);


	


	// If Ball In Hand - Check if strike Vector is going down table.. from Baulk to Black Spot Dirction //
	// If so, set the flag (used for Billiards Rule for Ball In Hand) //
	g_bStrikeDownTable = TRUE;
	if(g_gameAppView->m_snooker->m_bBallInHand)
		{
		if(sx < ex ) g_bStrikeDownTable = FALSE;
		}
	
	


	#ifdef SHAKE_LINE		// debugging mode
	////////////////////////
	C = C.RotateXY(_rnd);
	////////////////////////
	#else
	// angle error for shot!!!!
	if(m_parent->m_cueError)
	if(!bTestBall) // only do if not an exit trajectory test ball
	//if(m_parent->m_cueBallSelect != 1) // only do if not an exit trajectory test ball
	{				
		//NOTE : error is calculated in snooker.cpp -> void CSnooker::StrikeBallSend(void)

		/////// Carl / Odin ///////
		/////// I Rem'ed out your Error calculation and put in mine.. 
		/////// It now works on a Normal (Gaussian Distribution) with 
		/////// S.D = 1, Mean = 0;
		/////// I take your rnd parammeter and divide by 60 to make the range of X about -0.8 to +0.8
		/////// So this is just under 1 standard deviation from mean..
		/////// I do 1/Gaussian_Value(X) & negate if the value of X is less than 0 (the mean) or Gaussian
		/////// the final error is scaled as u did before.. 

		C = C.RotateXY(_rnd);
	}
	#endif
	
	// get mag. of cue strike
	cmag = C.Magnitude();




	// get unit vector
	CUnit = C.Unit();
	


	// offset strike power distance
	// [#] cmag-=CUE_POWER_PERCENTAGE_SHIFT * CUE_POWER_MAX; // i.e. subtract 5% of initial line distance, so (0) - (d) -> (-5) - (d-5)
	f1 = FIXMUL(TOFIX(CUE_POWER_PERCENTAGE_SHIFT), TOFIX(CUE_POWER_MAX));
	

	cmag = cmag - f1;

	//if(g_build.gameType == GAMETYPE_POOL)	// force 0% to be about 1%
	cmag = cmag + FIXMUL(TOFIX(CUE_POWER_MAX), TOFIX(0.082));

	float __f1;
	float __cmag;

	if(!bTestBall)
	{
		__f1 = (float)FROMFIXSMALL(f1);
		__cmag = (float)FROMFIXSMALL(cmag);
		
	}

	

	if(cmag < TOFIX(0.0f)) return; // *TODO: should be returning FALSE!
	
	// reduce strike power, down to usable size 0.0f - 10.0f
	cmag = FIXMUL(cmag, TOFIX(0.00625f));//BALL_STRIKEFACTOR;
	if(cmag>maxspeed) cmag = maxspeed; // lock


	

	// POOL game - give exponential power growth ^3
	if(g_build.gameType == GAMETYPE_POOL)
	{
		__fix cmagUnit = cmag;
		cmagUnit = FIXDIV(cmagUnit, maxspeed);
		cmagUnit = FIXMUL(cmagUnit, cmagUnit);
		cmagUnit = FIXMUL(cmagUnit, cmagUnit);
		cmagUnit = FIXMUL(cmagUnit, maxspeed);
	}


	

	// make all exit traj. have same length
	if(bTestBall)
	//if(m_parent->m_cueBallSelect == 1) // only do if an exit trajectory test ball
	{		
		if(g_build.gameType == GAMETYPE_SNOOKER)
		{
			// OLD WAY
			m_parent->m_testBallInitialSpeed = FROMFIX(cmag);
			cmag = maxspeed; // *TODO: what abt nap for slow moving balls? /  don't show side in exit traj. when cmag = maxspeed.
			cmag = FIXDIV(cmag, TOFIX(BALL_MAXSPEEDFACTOR)); // *new* added for pool high-speed breaks 
			//cmag *= 0.5;		
		}
		else if(g_build.gameType == GAMETYPE_POOL)
		{
			m_parent->m_testBallInitialSpeed = FROMFIX(cmag);
			cmag = maxspeed; // *TODO: what abt nap for slow moving balls? /  don't show side in exit traj. when cmag = maxspeed.
			cmag = FIXDIV(cmag, TOFIX(BALL_MAXSPEEDFACTOR)); // *new* added for pool high-speed breaks 
			cmag = FIXMUL(cmag, TOFIX(0.6f));
		}
	}


	// work out power factor for cuetip -> cueball contact point:
	FVector D = FVector(_cuetip[0], _cuetip[1], TOFIX(0.0f));
	
	__fix d = D.Magnitude();
	d = FIXDIV(d, TOFIX(0.7f));
	if(d > TOFIX(1.0f)) d = TOFIX(1.0f);




	// store current cue-strike force
	C = C.Unit();
	C = C * cmag;

	/*
	if(!bTestBall)
	{
		

		char buf[256];
		sprintf(buf, "d: %f, D.x: %f, D.y: %f, cmag: %f", FROMFIXSMALL(d), FROMFIXSMALL(D.x), FROMFIXSMALL(D.y), (float)FROMFIX(cmag));
		ErrorMessage(m_parent->m_parent, buf);
	}
	*/

	// (**) scale cue-strike force
	__fix fdrop = TOFIX(BALL_MAXSPEEDFACTOR);

	/*
	if(!bTestBall)
	{
		char buf[256];
		sprintf(buf, "fdrop = %f maxspeed = %f, cmag = %f", (float)FROMFIXSMALL(fdrop), (float)FROMFIXSMALL(maxspeed), (float)FROMFIXSMALL(cmag));
		ErrorMessage(m_parent->m_parent, buf);
	}
	*/
	
	maxspeed = FIXMUL(maxspeed, fdrop);
	cmag = FIXMUL(maxspeed, fdrop);
	C = C * fdrop;

/*	
	if(bTestBall)
	{
		char buf[256];
		sprintf(buf, "d = %f, fdrop = %f, C.x = %f, C.y = %f, cmag = %f", (float)FROMFIXSMALL(d), (float)FROMFIXSMALL(fdrop), (float)FROMFIXSMALL(C.x), (float)FROMFIXSMALL(C.y), (float)FROMFIXSMALL(cmag));
		ErrorMessage(m_parent->m_parent, buf);
	}
	*/

	/////////////////////////////////////////////////////////////////////////////
	// velocity of centre of mass
	f_V = C;//CVector((float)x, (float)y, (float)z);	
	//[#] m_V = m_V * (1 - d/3);
	f1 = FIXDIV(d, TOFIX(3.0f));
	f1 = TOFIX(1.0f) - f1;
	f2 = FIXMUL(TOFIX(0.5f), _cuetip[2]); // raise butt component
	f1 = f1 - f2;
	
	/*
	if(!bTestBall)
	{
		char buf[256];
		sprintf(buf, "[S] f1 = %f, f2 = %f, f_V.x = %f, f_V.y = %f", (float)FROMFIXSMALL(f1), (float)FROMFIXSMALL(f2), (float)FROMFIXSMALL(f_V.x), (float)FROMFIXSMALL(f_V.y));
		ErrorMessage(m_parent->m_parent, buf);
	}
	*/
	
	f_V = f_V * f1;


	/////////////////////////////////////////////////////////////////////////////
	

	/////////////////////////////////////////////////////////////////////////////
	// back/top/swerve spin
	// [#] m_W = m_V * d*1.0f * (((cuetip[1] / 0.65f) * 1.3f) /*- 0.1*//*0.15*/);
	f1 = FIXDIV(_cuetip[1], TOFIX(0.65f));
	f2 = FIXDIV(_cuetip[2], TOFIX(0.90f));	// raise butt, will add a little back spin
	f1 = f1 - f2;			//
	f1 = FIXMUL(f1, TOFIX(1.3f));
	f1 = FIXMUL(f1, TOFIX(1.0f));
	f1 = FIXMUL(f1, d);
	f_W = f_V * f1;

	/*
	if(!bTestBall)
	{
		char buf[256];
		sprintf(buf, "[E] f1 = %f, f2 = %f, f_V.x = %f, f_V.y = %f", (float)FROMFIXSMALL(f1), (float)FROMFIXSMALL(f2), (float)FROMFIXSMALL(f_V.x), (float)FROMFIXSMALL(f_V.y));
		ErrorMessage(m_parent->m_parent, buf);
	}
	*/
	

	//m_W = m_W / BALL_MAXSPEEDFACTOR; // *new* added for pool high-speed breaks

	// add sverve component
	FVector Vt;	
	Vt = f_V.Cross(FVector(TOFIX(0.0f), TOFIX(0.0f), TOFIX(1.0f)));
	Vt = Vt.Unit();	
	f1 = _cuetip[2];
	f1 = FIXMUL(f1, TOFIX(1.3f));
	f1 = FIXMUL(f1, d);
	f2 = FIXDIV(_cuetip[0], TOFIX(0.65f));
	   
	//if(!m_parent->m_bCueError) // make raising butt easier too pot balls, for amateurs!
	if(g_build.gameType == GAMETYPE_SNOOKER)
	{
		if(f2<TOFIX(0.0f)) {
			f2 = FIXMUL(f2, f2); // squared
			f2 = -f2;
		}
		else {
			f2 = FIXMUL(f2, f2); // squared
		}
	}
	
	f1 = FIXMUL(f1, f2);
	f1 = FIXMUL(f1, TOFIX(6.0f));
	Vt = Vt * f1;
	//Vt = Vt / BALL_MAXSPEEDFACTOR; // *new* added for pool high-speed breaks 
	f_W = f_W + Vt;

	
	// work our delay before spin comes out
	//m_WDelay = m_W.Magnitude();
	m_WDelay = (float)FROMFIXSMALL(f_W.Magnitude());
	m_WDelay = m_WDelay * 20;
	m_WDelay = 0;
	/*
	if(!bTestBall)
	{
	char buf[256];
	sprintf(buf, "m_WDelay = %d, f_W.x = %f, f_W.y = %f, Vt.x = %f. Vt.y = %f", m_WDelay, (float)FROMFIXSMALL(f_W.x), (float)FROMFIXSMALL(f_W.y), (float)FROMFIXSMALL(Vt.x), (float)FROMFIXSMALL(Vt.y));
	ErrorMessage(m_parent->m_parent, buf);
	}
	*/
	
	


	// give ball some natural roll, if top spin is applied [FOR RollingSpin()]
	if(_cuetip[1] > TOFIX(0.0f)) {
		// [#] m_wnroll = BALL_NATURALROLLW * (cuetip[1] / 0.65f) * 10;
		f1 = FIXDIV(_cuetip[1], TOFIX(0.65f));
		f1 = FIXMUL(f1, TOFIX(3.0f));//10.0f;
		f_wnroll = FIXMUL(f1, TOFIX(BALL_NATURALROLLW));
		
		if(f_wnroll > TOFIX(BALL_NATURALROLLW))
			f_wnroll = TOFIX(BALL_NATURALROLLW);
		SetFlag(m_flags, Flg_BALL_PUREROLL); //???
	}

	// draw chalk lines, only for backspin
	if(m_parent->m_cueBallSelect != 1) // only do if not an exit trajectory test ball
		if(_cuetip[1] < TOFIX(0.0f))
		{
			m_parent->MakeChalkMark(this);
			m_parent->m_bRedrawChalkMarks = TRUE; //  now ready to do first redraw of lines & chalk marks (for update in 2D mode)
		}	
	/////////////////////////////////////////////////////////////////////////////

	
	/////////////////////////////////////////////////////////////////////////////
	// side spin
	// [#] m_wSide = - d*3 * (((cuetip[0] / 0.65f) * 1.3f)) * (2 + m_V.Magnitude() / 10);
	f1 = f_V.Magnitude();
	f1 = FIXDIV(f1, maxspeed);
	f1 = f1 + TOFIX(2.0f);
	f2 = FIXDIV(_cuetip[0], TOFIX(0.65f));
	f2 = FIXMUL(f2, TOFIX(1.3f));
	f2 = FIXMUL(f2, f1);
	f2 = FIXMUL(f2, TOFIX(3.0f));
	f2 = FIXMUL(f2, d);
	f_wSide = -f2;
	//m_wSide = m_wSide / BALL_MAXSPEEDFACTOR; // *new* added for pool high-speed breaks 

	if(f_wSide > maxspeed) f_wSide = maxspeed;
	
	#ifdef SHOW_SIDESPIN
	if(!bTestBall)
	{
		char buffy[80];
		sprintf(buffy, "SideSpin = %f", cuetip[0] / 0.65f);
		ErrorMessage(m_parent->m_parent, buffy);
	}
	#endif //SHOW_SIDESPIN
	/////////////////////////////////////////////////////////////////////////////




	// play cue2ball collision sfx
	if(g_gfxDetails.bSoundBalls)
	if(!bTestBall)
	if(!bIsBot)
	{
		float mag;
		cmag = C.Magnitude();
		mag = FROMFIXSMALL(cmag) / 15.0f;
		//cmag /= maxspeed;
		Msfx_smpvol(&((CGameAppView *)m_parent->m_parent)->m_game_smp[SFX_CUE2BALL], 255*mag);
		Msfx_smpplay(&((CGameAppView *)m_parent->m_parent)->m_game_smp[SFX_CUE2BALL]);		
	}
	//#ifdef FIXEDMODE
	//UpdateFixeds();
	//UpdateFloats();
	//#endif
}

#else // normal...
//#endif // 0
///////////////////////////////////////////////////////////////////////////////////////////////////
//
// // need to do more work on the fixed side of this function!
// Zoe Vanzuylekom
void CBall::Strike(float sx,float sy,float sz, 
				   float ex,float ey,float ez, 
				   M3d_vtx cuetip, float rnd,
				   BOOL bTestBall, BOOL bIsBot)
{
	

	
	/*
	if(!bTestBall)
	{
		bIsBot = true;
		// ez += 50;
		char buf[256];
		sprintf(buf, "(sx = %f, sy = %f, sz = %f) - (ex = %f, ey = %f, ez = %f)", sx, sy, sz, ex, ey, ez);
		// sprintf(buf, "(C.x = %f, C.y = %f, C.z = %f)", C.x, C.y, C.z);
		ErrorMessage(m_parent->m_parent, buf);
	 
	}
	*/
	
	

	//rnd = 0;
	
	// This must also be called up in the StrikeBallReceive function (for people receiving the shot) //
	g_bTextDoubleErrorAdded = FALSE;

	float f1, f2, f3;	// [#]

	float cmag;
	CVector C, CUnit;
	float maxspeed = BALL_MAXSPEED;

	float angleError = 0;

	// get cue strike vector
	C = CVector(sx - ex, sy - ey, sz - ez);

	

	if(g_gameAppView->m_replay->m_version == 4)
	{
		C.x /= 1.2f;
		C.y /= 1.2f;
	}
	else if(g_gameAppView->m_replay->m_version <= 7)
	{
		C.x /= 1.15f;
		C.y /= 1.15f;
	}
	else
	{
		C.x /= 1.15f;
		C.y /= 1.15f;
	}

	// If Ball In Hand - Check if strike Vector is going down table.. from Baulk to Black Spot Dirction //
	// If so, set the flag (used for Billiards Rule for Ball In Hand) //
	g_bStrikeDownTable = TRUE;
	if(g_gameAppView->m_snooker->m_bBallInHand)
		{
		if(sx < ex ) g_bStrikeDownTable = FALSE;
		}


	/*
	{
	char buf[256];
	//sprintf(buf, "(sx = %f, sy = %f, sz = %f) - (ex = %f, ey = %f, ez = %f)", sx, sy, sz, ex, ey, ez);
	sprintf(buf, "(C.x = %f, C.y = %f, C.z = %f)", C.x, C.y, C.z);
	ErrorMessage(m_parent->m_parent, buf);
	}
	*/

	//rnd = (rand()%100) - 50;
	
	#ifdef SHAKE_LINE		// debugging mode
	////////////////////////
	C = C.RotateXY(rnd);
	////////////////////////
	#else
	// angle error for shot!!!!
	if(m_parent->m_cueError)
	if(!bTestBall) // only do if not an exit trajectory test ball
	//if(m_parent->m_cueBallSelect != 1) // only do if not an exit trajectory test ball
	{		
		
		//NOTE : error is calculated in snooker.cpp -> void CSnooker::StrikeBallSend(void)


		/////// Carl / Odin ///////
		/////// I Rem'ed out your Error calculation and put in mine.. 
		/////// It now works on a Normal (Gaussian Distribution) with 
		/////// S.D = 1, Mean = 0;
		/////// I take your rnd parammeter and divide by 60 to make the range of X about -0.8 to +0.8
		/////// So this is just under 1 standard deviation from mean..
		/////// I do 1/Gaussian_Value(X) & negate if the value of X is less than 0 (the mean) or Gaussian
		/////// the final error is scaled as u did before.. 
/*
////////////
		float multiplier1 = 0.01f * (float)((rand()%100)+1); // 0-100% multiplier
		float multiplier2 = 0.01f * (float)((rand()%100)+1); // 0-100% multiplier

		float multiplier = multiplier2 * multiplier2;		//0.0 - 1.0
		multiplier *= 0.9;
		multiplier += 0.1;

		float newRnd = rnd * multiplier;

		char buf[80];
		sprintf(buf, "[multiplier = %f] newRnd = %f",multiplier, newRnd);
		ErrorMessage(m_parent->m_parent, buf);
///////////
*/
		
		C = C.RotateXY(rnd);
		//C = C.RotateXY(newRnd);

		/*
		char buf[80];
		sprintf(buf, "rnd = %f", rnd);
		ErrorMessage(m_parent->m_parent, buf);
		*/
	}
	#endif
	
	// get mag. of cue strike
	cmag = C.Magnitude();



	// get unit vector
	CUnit = C.Unit();
	
	// offset strike power distance
	// [#] cmag-=CUE_POWER_PERCENTAGE_SHIFT * CUE_POWER_MAX; // i.e. subtract 5% of initial line distance, so (0) - (d) -> (-5) - (d-5)
	f1 = CUE_POWER_PERCENTAGE_SHIFT * CUE_POWER_MAX;






	cmag = cmag - f1;

	//if(g_build.gameType == GAMETYPE_POOL)	// force 0% to be about 1%
	cmag = cmag + CUE_POWER_MAX*0.082;

	if(cmag < 0) return; // *TODO: should be returning FALSE!
	
	// reduce strike power, down to usable size 0.0f - 10.0f
	cmag *= 0.00625f;//BALL_STRIKEFACTOR;
	if(cmag>maxspeed) cmag = maxspeed; // lock

	/*
	if(!bTestBall)
	{
		char buf[256];
		sprintf(buf, "cmag=%f, maxspeed=%f", cmag, maxspeed);
		ErrorMessage(m_parent->m_parent, buf);
	}
	*/

	// POOL game - give exponential power growth ^3
	if(g_build.gameType == GAMETYPE_POOL)
	{
		float cmagUnit = cmag;
		cmagUnit /= maxspeed;
		cmagUnit *= cmagUnit;
		cmagUnit *= cmagUnit;
		cmagUnit *= maxspeed;
	}

	// make all exit traj. have same length
	if(bTestBall)
	//if(m_parent->m_cueBallSelect == 1) // only do if an exit trajectory test ball
	{		
		if(g_build.gameType == GAMETYPE_SNOOKER)
		{
			// OLD WAY
			m_parent->m_testBallInitialSpeed = cmag;
			cmag = maxspeed; // *TODO: what abt nap for slow moving balls? /  don't show side in exit traj. when cmag = maxspeed.
			cmag = cmag / BALL_MAXSPEEDFACTOR; // *new* added for pool high-speed breaks 
			//cmag *= 0.5;		
		}
		else if(g_build.gameType == GAMETYPE_POOL)
		{
			m_parent->m_testBallInitialSpeed = cmag;
			cmag = maxspeed; // *TODO: what abt nap for slow moving balls? /  don't show side in exit traj. when cmag = maxspeed.
			cmag = cmag / BALL_MAXSPEEDFACTOR; // *new* added for pool high-speed breaks 
			cmag *= 0.6;
		}


	}

	// work out power factor for cuetip -> cueball contact point:
	CVector D = CVector(cuetip[0], cuetip[1], 0);
	//CVector D = CVector(cuetip[0], cuetip[1], cuetip[2]);
	//D = D.Unit();
	float d = D.Magnitude();
	d = d / 0.7f;
	if(d > 1) d = 1;




	// store current cue-strike force
	C = C.Unit();
	C = C * cmag;

	// (**) scale cue-strike force
	float fdrop = BALL_MAXSPEEDFACTOR;
	maxspeed *= fdrop;
	cmag *= fdrop;
	C = C * fdrop;

	//C = C * 1.5;
	

	/////////////////////////////////////////////////////////////////////////////
	// velocity of centre of mass
	m_V = C;//CVector((float)x, (float)y, (float)z);	
	//[#] m_V = m_V * (1 - d/3);
	f1 = d/3.0f;
	f1 = 1.0f - f1;
	f2 = 0.5*cuetip[2]; // raise butt component
	f1 = f1 -  f2;
	m_V = m_V * f1;




	/////////////////////////////////////////////////////////////////////////////
	

	/////////////////////////////////////////////////////////////////////////////
	// back/top/swerve spin
	// [#] m_W = m_V * d*1.0f * (((cuetip[1] / 0.65f) * 1.3f) /*- 0.1*//*0.15*/);
	f1 = cuetip[1] / 0.65f;
	f2 = cuetip[2] * 0.90f;	// raise butt, will add a little back spin
	f1 = f1 - f2;			//
	f1 = f1 * 1.3f;
	f1 = f1 * 1.0f;
	f1 = f1 * d;
	m_W = m_V * f1;
	//m_W = m_W / BALL_MAXSPEEDFACTOR; // *new* added for pool high-speed breaks

	// add sverve component
	CVector Vt;	
	Vt = m_V.Cross(CVector(0, 0, 1));
	Vt = Vt.Unit();	
	f1 = cuetip[2];
	f1 = f1 * 1.3f;
	f1 = f1 * d;
	f2 = cuetip[0] / 0.65f;
	   
	//if(!m_parent->m_bCueError) // make raising butt easier too pot balls, for amateurs!
	if(g_build.gameType == GAMETYPE_SNOOKER)
	{
		if(f2<0) {
			f2 = f2 * f2; // squared
			f2 = -f2;
		}
		else {
			f2 = f2 * f2; // squared
		}
	}
	
	f1 = f1 * f2;
	f1 = f1 * 6.0f;
	Vt = Vt * f1;
	
	
	/*	
	if(m_colour == 0)
	if(bTestBall == false)
	{
		char buf[256];
		//sprintf(buf, "cmag=%f, fdrop=%f, C.x=%f, C.y=%f, m_V.x:%f, m_V.y:%f", cmag, fdrop, C.x, C.y, m_V.x, m_V.y);
		sprintf(buf, "Vt.x=%f, Vt.y=%f, f1:%f, f2:%f, m_W.x=%f, m_W.y=%f", Vt.x, Vt.y, f1, f2, m_W.x, m_W.y);
		ErrorMessage(m_parent->m_parent, buf);
	}
	*/

	//Vt = Vt / BALL_MAXSPEEDFACTOR; // *new* added for pool high-speed breaks 
	m_W = m_W + Vt;

	
	// work our delay before spin comes out
	m_WDelay = m_W.Magnitude();
	m_WDelay = m_WDelay * 20;
	// m_WDelay = 0;

	// give ball some natural roll, if top spin is applied [FOR RollingSpin()]
	if(cuetip[1] > 0) {
		// [#] m_wnroll = BALL_NATURALROLLW * (cuetip[1] / 0.65f) * 10;
		f1 = cuetip[1] / 0.65f;
		f1 = f1 * 3.0f;//10.0f;
		m_wnroll = f1 * BALL_NATURALROLLW;
		
		if(m_wnroll > BALL_NATURALROLLW)
			m_wnroll = BALL_NATURALROLLW;
		SetFlag(m_flags, Flg_BALL_PUREROLL); //???
	}



	// draw chalk lines, only for backspin
	if(m_parent->m_cueBallSelect != 1) // only do if not an exit trajectory test ball
		if(cuetip[1] < 0)
		{
			m_parent->MakeChalkMark(this);
			m_parent->m_bRedrawChalkMarks = TRUE; //  now ready to do first redraw of lines & chalk marks (for update in 2D mode)
		}	
	/////////////////////////////////////////////////////////////////////////////


	/////////////////////////////////////////////////////////////////////////////
	// side spin
	// [#] m_wSide = - d*3 * (((cuetip[0] / 0.65f) * 1.3f)) * (2 + m_V.Magnitude() / 10);
	f1 = m_V.Magnitude();
	f1 = f1 / maxspeed;
	f1 = f1 + 2.0f;
	f2 = cuetip[0] / 0.65f;
	f2 = f2 * 1.3f;
	f2 = f2 * f1;
	f2 = f2 * 3.0f;
	f2 = f2 * d;
	m_wSide = -f2;
	//m_wSide = m_wSide / BALL_MAXSPEEDFACTOR; // *new* added for pool high-speed breaks 

	if(m_wSide > maxspeed) m_wSide = maxspeed;
	


	/*
	if(m_colour == 0)
	if(bTestBall == false)
	{
		char buf[256];
		sprintf(buf, "Strike(%f, %f, %f, %f, %f, %f, cuetip(%f, %f, %f), rnd:%f", sx, sy, sz, ex, ey, ez, cuetip[0], cuetip[1], cuetip[2], rnd);
		ErrorMessage(m_parent->m_parent, buf);

		sprintf(buf, "m_wSide: %f, m_WDelay: %d, m_W.x: %f, m_W.y: %f, m_W.z: %f, m_V.x: %f, m_V.y: %f, m_V.z: %f", m_wSide, m_WDelay, m_W.x, m_W.y, m_W.z, m_V.x, m_V.y, m_V.z);
		ErrorMessage(m_parent->m_parent, buf);
	}
	*/
	





	#ifdef SHOW_SIDESPIN
	if(!bTestBall)
	{
		char buffy[80];
		sprintf(buffy, "SideSpin = %f", cuetip[0] / 0.65f);
		ErrorMessage(m_parent->m_parent, buffy);
	}
	#endif //SHOW_SIDESPIN
	/////////////////////////////////////////////////////////////////////////////


	// play cue2ball collision sfx
	if(g_gfxDetails.bSoundBalls)
	if(!bTestBall)
	if(!bIsBot)
	{
		cmag /= maxspeed;
		cmag = sqrt(cmag);
		cmag = sqrt(cmag);

		Msfx_smpvol(&((CGameAppView *)m_parent->m_parent)->m_game_smp[SFX_CUE2BALL], 90 + 150*cmag);
		Msfx_smpplay(&((CGameAppView *)m_parent->m_parent)->m_game_smp[SFX_CUE2BALL]);		
	}
	#ifdef FIXEDMODE
	UpdateFixeds();
	#endif
}
#endif //FIXEDMODE

#define GSURFACE_POCKETA	0
#define GSURFACE_CLOTH		-1

float g_Floor_Z;

// Check if we are inside our Global Surface for Table (hard coded for UK Pool)
char CBall::Inside_GSurface(__fix x, __fix y, __fix z)
{
	// Check Pocket Buckets (cylinders)
	FVector Pocket;

	g_Floor_Z = 0;

	for(int n=0;n<6;n++)
	{
	Pocket.x = TOFIX(m_parent->m_customTable->m_pocketPos[n].x);
	Pocket.y = TOFIX(m_parent->m_customTable->m_pocketPos[n].y);
	Pocket.z = TOFIX(m_parent->m_customTable->m_pocketPos[n].z);

	Pocket = Pocket - f_P;

	__fix sR = TOFIX(m_parent->m_customTable->m_pocketProp[n].m_suctionRadius);
	__fix sF = TOFIX(m_parent->m_customTable->m_pocketProp[n].m_suctionFactor);
	__fix r = TOFIX(m_parent->m_customTable->m_pocketProp[n].m_radius);
		
	Pocket.z = 0;
		
	__fix sR2 = FIXMUL(sR, sR);

	if(Pocket.Magnitude2() < sR2)
		{
		g_Floor_Z = sR2 - Pocket.Magnitude2();
		g_Floor_Z = g_Floor_Z / sR2; // 0 - 1 of distance from Epicentre 

		return n;
		//
		}
	}

	return -1; // No Pocket Found
}

void CBall::ProcessGravity(void)
{
	int idx;
	float Floor_Z;
	
	// If not English Pool, then return, don't do Gravity //
	//if(m_parent->m_M3d_Table_ptr != &m_parent->m_customTable->m_M3d_ukTable) return;
	
		
	
	// Gravity //
	//idx = Inside_GSurface(f_P.x, f_P.y, f_P.z);

	//Floor_Z = BALL_POSITIONZOFFSET - 80*g_Floor_Z;
	Floor_Z = BALL_POSITIONZOFFSET - 80;
	
#ifdef FIXEDMODE		
	if(f_P.z>TOFIX(Floor_Z))
		{
		f_V.z = f_V.z - 400;
		}
	else{
		f_P.z = TOFIX(Floor_Z);
		f_V.z = 0;
		}
#else
	if(m_P.z>Floor_Z)
		{
		m_V.z = m_V.z - FROMFIX(400);
		}
	else{
		m_P.z = BALL_POSITIONZOFFSET;
		m_V.z = 0;
		}
#endif


}


void CBall::AdjustOrientation(void)
{
#ifndef MAIN_SERVER

	// SPIN #1 (Standard spins/positional)
	CVector Forward;
	float Mag;
#ifdef FIXEDMODE
	FVector fV;
	fV = f_W + f_P-f_POld;
	Forward.x = FROMFIXSMALL(fV.x);
	Forward.y = FROMFIXSMALL(fV.y);
	Forward.z = FROMFIXSMALL(fV.z);
#else
	//Forward = m_W + m_P-m_POld;
	Forward = m_P-m_POld;
	Forward = Forward + m_W;
#endif

	
	Mag = Forward.Magnitude();

	if(Mag>0.01)
	{
	CVector Right;
	CVector Up(0,0,1);

	Right = Forward.Cross(Up);

	float Rads;
	D3DXVECTOR3 RightD3D;
	RightD3D.x = Right.x;
	RightD3D.y = Right.y;
	RightD3D.z = Right.z;
	//Rads = (Mag/BALL_RADIUS*6.28)*6.28;
	Rads = (Mag/BALL_RADIUS);
		
	//D3DUtil_SetRotationMatrix(m_d3dmtxV, RightD3D, Rads);
	D3DXMatrixRotationAxis(&m_d3dmtxV, &RightD3D, -Rads);
	m_d3dmtx = m_d3dmtx * m_d3dmtxV;
	//MultiplyMatrices(m_m3dmtx,m_m3dmtx, m_m3dmtxV);
	}


	// SPIN #2 (Z Axis)
	{
	D3DXVECTOR3 UpD3D;
	float Rads;
	UpD3D.x = 0;
	UpD3D.y = 0;
	UpD3D.z = 1;
	Rads = m_wSide*0.01; // on the order of 10

	//D3DUtil_SetRotationMatrix(m_d3dmtxV, UpD3D, Rads);
	D3DXMatrixRotationAxis(&m_d3dmtxV, &UpD3D, -Rads);
	m_d3dmtx = m_d3dmtx * m_d3dmtxV;
	//MultiplyMatrices(m_m3dmtx,m_m3dmtx, m_m3dmtxV);
	}
#endif
}
///////////////////////////////////////////////////////////////////////////////////////////////////
//
//

void CBall::UpdateFloats(void)
{
	m_P.x = FROMFIXSMALL(f_P.x);
	m_P.y = FROMFIXSMALL(f_P.y);
	m_P.z = FROMFIXSMALL(f_P.z);
	m_V.x = FROMFIXSMALL(f_V.x);
	m_V.y = FROMFIXSMALL(f_V.y);
	m_V.z = FROMFIXSMALL(f_V.z);
	m_W.x = FROMFIXSMALL(f_W.x);
	m_W.y = FROMFIXSMALL(f_W.y);
	m_W.z = FROMFIXSMALL(f_W.z);
	m_POld.x = FROMFIXSMALL(f_POld.x);
	m_POld.y = FROMFIXSMALL(f_POld.y);
	m_POld.z = FROMFIXSMALL(f_POld.z);
	m_VOld.x = FROMFIXSMALL(f_VOld.x);
	m_VOld.y = FROMFIXSMALL(f_VOld.y);
	m_VOld.z = FROMFIXSMALL(f_VOld.z);
	m_wnroll = FROMFIXSMALL(f_wnroll);
	m_wSide = FROMFIXSMALL(f_wSide);
}

void CBall::UpdateFixeds(void)
{
	f_P.x = TOFIX(m_P.x);
	f_P.y = TOFIX(m_P.y);
	f_P.z = TOFIX(m_P.z);
	f_V.x = TOFIX(m_V.x);
	f_V.y = TOFIX(m_V.y);
	f_V.z = TOFIX(m_V.z);
	f_W.x = TOFIX(m_W.x);
	f_W.y = TOFIX(m_W.y);
	f_W.z = TOFIX(m_W.z);
	f_POld.x = TOFIX(m_POld.x);
	f_POld.y = TOFIX(m_POld.y);
	f_POld.z = TOFIX(m_POld.z);
	f_VOld.x = TOFIX(m_VOld.x);
	f_VOld.y = TOFIX(m_VOld.y);
	f_VOld.z = TOFIX(m_VOld.z);
	f_wnroll = TOFIX(m_wnroll);
	f_wSide = TOFIX(m_wSide);
}

int tickerb = 30;

BOOL CBall::Process(BOOL bTestBall, BOOL bIsBot, int rollOff, float timeDelta)
{		

	m_timeDelta = timeDelta;


	


		

	if(!TestFlag(m_flags, Flg_BALL_ACTIVE))
	{
		m_cyc = 0;
		return FALSE;
	}



	m_cyc++; // 0..255.0..255...(cycles)
	if(m_ghost) m_ghost--;

	
	SideSpinFriction(); // Fixed //	

	

	// is this ball at rest?
	if(Motionless()) {
		
		SetFlag(m_flags, Flg_BALL_MOTIONLESS);
		AdjustOrientation();
		
		// save current position & velocity of ball
#ifdef FIXEDMODE
	f_POld = f_P;
	f_VOld = f_V;
	UpdateFloats();
#else
	m_POld = m_P;
	m_VOld = m_V;
#endif	
		m_cyc = 0;
		return FALSE;
	}
	else
		ClearFlag(m_flags, Flg_BALL_MOTIONLESS);

	if(g_gameAppView->m_replay->m_version >= 8)
	{
		rollOff *= 0.5f;
	}


	// *NEW - rollOff code
	#ifndef THE_POOL_CLUB
	if(m_parent->m_cueError <= 0) // NOT semi-pro, pro
		rollOff = 0;
	#endif
	RollOff(bTestBall, rollOff);
	////////////////////////////
	
	m_justHitAnotherBall--;
	if(m_justHitAnotherBall < 0) m_justHitAnotherBall = 0;

	m_justHitCushion--;
	if(m_justHitCushion < 0) m_justHitCushion = 0;

	// natural roll spin
	RollingSpin(); // Fixed //

	#ifdef FIXEDMODE
		f_POld = f_P;
		f_VOld = f_V;
	#else
		m_POld = m_P;
		m_VOld = m_V;
	#endif


	// spin friction
	SlippingFriction(); // Fixed //

	
	// natural roll friction
	#ifndef FIXEDMODE
		RollingFriction();
	#else
		// *NEW - do this 1 in every 16 cycles... (BUGFIX FOR iPOOL ROLLOFF)
		if(g_bBallRollOff)
		{
			if(!(m_cyc&0xf))
				RollingFriction(); // Fixed //
		}
		else
		{
			// do old way
			RollingFriction(); // Fixed //
		}
	#endif //FIXEDMODE

	

	// nap effect
	//NapEffect(); // increases error for slow played shots by too much!

	// linear ball motion
	//m_V.z-=0.01*(16/GAME_ITTR); // ball gravity

#ifdef FIXEDMODE
	FVector fV;
	fV = f_V * TOFIX(16.0f/GAME_ITTR);
	f_P = f_P + fV;
#else
	CVector V;
	V = m_V * (16.0f/GAME_ITTR);
	m_P = m_P + V;
#endif
	
	

	// do 3d world collisions (only if using old system)
	if(!g_bDetectionO)
		{
		if(g_build.gameType == GAMETYPE_SNOOKER)
			if((!bTestBall) && (CushionsProximity()))
				Collision3d(BALL_RADIUS);
		}
	
#ifdef FIXEDMODE

	//char buf[256];

	

	OBSTACLE_DATASTRUCTF obstacleData;
	if(!bTestBall)
	if(CushionsProximity(FALSE)) // NEW SEE OAP
	{
	//sprintf(buf, "iPool Detect Proximity");
	//ErrorMessage((CGameAppView *)g_wnd, buf);
	
	
	if(m_parent->m_customTable->m_obstacleCourse->Detect(&obstacleData, f_POld, fV, TOFIX(BALL_RADIUS))!=-1)
	{
	f_P = obstacleData.m_hitpos + (obstacleData.m_normal * TOFIX(0.01f));
		
	FVector V;
	__fix _eN;
	__fix _e;
	_eN = TOFIX(2.0f);
	_e = TOFIX(BALL_CUSHIONFACTOR);


	__fix fV = f_V.Dot(obstacleData.m_normal);
	fV = -fV;

	V = obstacleData.m_normal * fV;

	V = V * _eN;
	
	f_V = f_V + V;


	// If inside a Pocket, then reduce Velocity to %30
	if(m_InsidePocket==1 && m_FadeOut<170) 
		{
		m_InsidePocket=2;
		f_V.x = f_V.x >>2;
		f_V.y = f_V.y >>2;
		}


	////// * NEW - to fix balls rolling down cushions being dragged too much (cushions looked like high-friction rubber)
	if(g_bNewParallelCushionLoss) // *TODO for snooker
	{
		FVector fVN, fVT;
		fVN = obstacleData.m_normal * (f_V.Dot(obstacleData.m_normal));	
		fVT = f_V - fVN;
		fVT = fVT * TOFIX(0.85f);//_e = 0.72;
		fVN = fVN * _e;
		f_V = fVN + fVT;
	}
	else
	{
		f_V = f_V *	_e; // *OLD - 1/04/2004	
	}
	//////

		
	if(m_parent->m_dominoHit1st != m_parent->m_selectBall) // only count cushion collisions after white-ball has hit something
		m_hitCushion++;
	else
		m_bHitCushionBeforeBall = TRUE;

	SetFlag(m_flags, Flg_BALL_HITCUSHION);
	m_parent->m_bCueBallHitCushion = TRUE; // for debug purposes only!
	m_parent->m_cycHitCushion = m_parent->m_cyc;

	ApplySideSpinFixed(obstacleData.m_normal, TOFIX(1.0));

	if(g_gfxDetails.bSoundBalls)
	if(!bIsBot)
	{
		// play cue2ball collision sfx
		CVector SfxAmp;// = m_V;
		SfxAmp.x = FROMFIXSMALL(f_V.x);
		SfxAmp.y = FROMFIXSMALL(f_V.y);
		SfxAmp.z = FROMFIXSMALL(f_V.z);
		float SfxAmpMag = SfxAmp.Magnitude();
		if(SfxAmpMag > 5.0f)
			SfxAmpMag = 5.0f;
		SfxAmpMag/=5.0f;
		Msfx_smpvol(&((CGameAppView *)m_parent->m_parent)->m_game_smp[SFX_CUE2BALL], 200*SfxAmpMag);
		Msfx_smpplay(&((CGameAppView *)m_parent->m_parent)->m_game_smp[SFX_CUE2BALL]);		
	}	

	}
	}

// BAPDETECT2006
#else
	OBSTACLE_DATASTRUCT obstacleData;


	char proceed = 1;
	if(m_parent->m_customTable->m_type == CTTYPE_BILLIARDS)
		{
		
		if(m_parent->m_bTurn1P && m_colour == WHITE) proceed = 0;
		if(!m_parent->m_bTurn1P && m_colour == YELLOW) proceed = 0;
		}
	else{
		if(m_colour == WHITE) // cueball only
			proceed = 0;
		}
	

	// If we get here, then we must be the Cue Ball //
	// Tell Camera to Update if the anyball is moving beyond the blue spot + 100 //

	if(!bTestBall)
	if(m_parent->m_3dtoggle_old > 5)
	if(m_P.x < WORLD_X(591 - 100)) 
	if(m_V.x < -0.4) 
	//if((m_V.x*m_V.x) < 0.4 && (m_V.y*m_V.y) < 0.4) 
		{
		if(m_parent->PROCESS_CAMERA_TV(1));  // Force Camera to be one of the Full Table views //
			{
			
			//char buf2[256];
			//sprintf(buf2, "m_V.x: %f", m_V.x);
			//ErrorMessage(m_parent->m_parent, buf2);
			
			g_gameAppView->m_bOnSize = TRUE;
			g_gameAppView->m_bOnSizeDrawTmpSurf = TRUE;
			}
		}
	//

	if(g_bDetectionO)
	if(!bTestBall)
	if(CushionsProximity(FALSE))
	//if(m_colour == BLACK)
	if(m_parent->m_customTable->m_obstacleCourse->Detect(&obstacleData, m_POld, V, BALL_RADIUS)!=-1)
	{
	m_P = obstacleData.m_hitpos + (obstacleData.m_normal * 0.01f);
	//m_P = obstacleData.m_hitpos + (obstacleData.m_normal * 0.10f);

	CVector V;
	float _eN;
	float _e;
	_eN = 2.0f, _e = BALL_CUSHIONFACTOR;


	// If we're on version 2.2.10 or above
	// Make _e proportional to the speed. i.e. (1.0 - (speed / MaxSpeed)) * 0.3;
				
	float maxspeed = BALL_MAXSPEED;
	float cmag = m_V.Magnitude();
				
	float factor;
	factor = 1.0f;

	if(g_gameAppView->m_replay->m_version >= 8)
	{
		factor = cmag;
		factor = factor / maxspeed;
		factor = 1.0 - factor * 0.30f;
	}


	float fV = m_V.Dot(obstacleData.m_normal);
	fV = -fV;

	V = obstacleData.m_normal * fV;

	V = V * _eN;
	
	m_V = m_V + V;


	// If any ball other than the Cue Ball hits cushion, give it some error to stop Text Doubles //
	m_V = m_V * _e;

	m_V = m_V * factor;
	
	// Cushion Error for Object Balls
	// Only Do if version 1.8.00 or above (i.e. if g_bTextDoubleError is TRUE) //
	// Can only do this once per shot //
	//if(!cushion_error_added)

/*
	if(g_bTextDoubleError)
	if(!g_bTextDoubleErrorAdded)
	if(proceed)
		{
		g_bTextDoubleErrorAdded = TRUE;
		// Get a Random Value from m_strikeRnd so it is the same for all players at the table //
		float ang = g_gameAppView->m_snooker->m_strikeRnd;
		ang *= 1000000;
		ang = ((int)ang)%180;
		ang *= 0.01;

		
		//char buf2[256];
		//sprintf(buf2, "Cushion Angle: %f", ang);
		//ErrorMessage(m_parent->m_parent, buf2);
		
		// -1.8 to +1.8 range //
		  
		ang = ang * FF2RAD;
		m_V.x = m_V.x * cos(ang) - m_V.y*sin(ang);
		m_V.y = m_V.y * cos(ang) + m_V.x*sin(ang);
		
		//cushion_error_added = 1;
		}

*/	
	if(m_parent->m_dominoHit1st != m_parent->m_selectBall) // only count cushion collisions after white-ball has hit something
		m_hitCushion++;
	else
		m_bHitCushionBeforeBall = TRUE;

	SetFlag(m_flags, Flg_BALL_HITCUSHION);
	m_parent->m_bCueBallHitCushion = TRUE; // for debug purposes only!
	m_parent->m_cycHitCushion = m_parent->m_cyc;

	ApplySideSpin(obstacleData.m_normal, 1.0);

	if(g_gfxDetails.bSoundBalls)
	if(!bIsBot)
	{
		// play cue2ball collision sfx
		CVector SfxAmp = m_V;		
		float SfxAmpMag = SfxAmp.Magnitude();
		if(SfxAmpMag > 5.0f)
			SfxAmpMag = 5.0f;
		SfxAmpMag = sqrt(SfxAmpMag);
		SfxAmpMag = sqrt(SfxAmpMag);
		SfxAmpMag = 50 + SfxAmpMag * 50;

		Msfx_smpvol(&((CGameAppView *)m_parent->m_parent)->m_game_smp[SFX_BALL2POCKET], SfxAmpMag);
		Msfx_smpplay(&((CGameAppView *)m_parent->m_parent)->m_game_smp[SFX_BALL2POCKET]);		
	}	

	}

#endif

#ifdef FIXEDMODE
	{ // constrain velocity FIXED ONLY
	#define MAXBALLSPEED	10
	if(f_V.Magnitude2()>TOFIX(MAXBALLSPEED*MAXBALLSPEED))
		{
		__fix s = f_V.Magnitude();
		f_V = f_V * TOFIX(MAXBALLSPEED);
		f_V = f_V / s;
		}	
	}
#else
	{ // constrain velocity FIXED ONLY
	#define MAXBALLSPEED	10
	if(m_V.Magnitude2() > MAXBALLSPEED*MAXBALLSPEED)
		{
		__fix s = m_V.Magnitude();
		m_V = m_V * MAXBALLSPEED;
		m_V = m_V / s;
		}	
	}
#endif

	//if(!TestFlag(m_flags, Flg_BALL_POTTED))
	//{
	// stop motion in the z-axis! [*TODO - bring gravity back in]

	// Randomly Give White Ball a Z velocitiy
	
#ifdef THE_POOL_CLUB
	// Process Gravity, only if we're inside a pcoket //
	if(m_InsidePocket) ProcessGravity();


	if(m_InsidePocket)
	if(m_FadeOut) m_FadeOut--;
		
	
	//if(!m_parent->m_customTable->m_type == CTTYPE_UKPOOL) 
/*
	if(m_parent->m_M3d_Table_ptr != &m_parent->m_customTable->m_M3d_ukTable) 
		{
		#ifdef FIXEDMODE
			f_P.z = TOFIX(BALL_POSITIONZOFFSET);
			f_V.z = 0;
		#else
			m_P.z = BALL_POSITIONZOFFSET;
			m_V.z = 0;
		#endif
		}		
*/
#endif

	// Calc Delta (Rotation of ball object) ///////////////////////////////////////////////////////////////

	AdjustOrientation();

	// ////////////////////////////////////////////////////////////////////////////////////////////////////
	
 // moved to antiprocess
	
	// save current position & velocity of ball
/*	#ifdef FIXEDMODE
		f_POld = f_P;
		f_VOld = f_V;
	#else
		m_POld = m_P;
		m_VOld = m_V;
	#endif
*/	
		
	// clear ball 'hit' flags
	ClearFlag(m_flags, Flg_BALL_HITCUSHION | Flg_BALL_HITBALL);

	
	// simple cushion collision...
	//CollideWithCushions(); // UNUSED!
	

	// handle balls falling into pockects...
	if(CushionsProximity())
		CollideWithPockects(TRUE, bIsBot);

	#ifdef FIXEDMODE
		UpdateFloats();
	#endif



	
	// still processing...
	return TRUE;
}

BOOL CBall::AntiProcess(void)
{		
	return TRUE;
	// natural roll spin
	RollingSpin(); // Fixed //

	// save current position & velocity of ball
	#ifdef FIXEDMODE
		f_POld = f_P;
		f_VOld = f_V;
	#else
		m_POld = m_P;
		m_VOld = m_V;
	#endif
	return TRUE;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
//
//
	void FPRINTFX(char *str, float *x,float *y,float *z)
	{
	int *xx;
	int *yy;
	int *zz;
	xx = (int *) x;
	yy = (int *) y;
	zz = (int *) z;

	//fprintf(fptr2,"{[%s]%lx,%lx,%lx = %f,%f,%f}\r\n",str, *xx,*yy,*zz,*x,*y,*z);
	}


///////////////////////////////////////////////////////////////////////////////////////////////////
//
//
BOOL CBall::Motionless(void)
{

	

	#ifdef FIXEDMODE
	if(f_V.Magnitude2() < TOFIX(SQR(BALL_MOTIONLESSV)) )
	if(f_W.Magnitude2() < TOFIX(SQR(BALL_MOTIONLESSW)) )
	{
		//if(!TestFlag(m_flags, Flg_BALL_PUREROLL))
			
		if(TestFlag(m_flags, Flg_BALL_PUREROLL))
			ClearFlag(m_flags, Flg_BALL_PUREROLL);

		if(f_wnroll == 0)
		{
			// bring 'this' ball to a full stop!
			f_V = FVector(0,0,0);
			f_VOld = FVector(0,0,0);
			f_W = FVector(0,0,0);
			f_wnroll = 0;
			//m_wSide = 0;	// stop balls at rest, from spinning on the spot
					
			m_WDelay = 0; // NEW

			m_justHitAnotherBall = 0;
			m_justHitCushion = 0;
						  
			ClearFlag(m_flags, Flg_BALL_HITCUSHION | Flg_BALL_HITBALL);

			return TRUE;
			}

	}
	#else	

	/*
			if(m_colour == false)
			{
			char buff[256];
			sprintf(buff, "m_W.Magnitude2()=%f", m_W.Magnitude2());
			ErrorMessage(m_parent->m_parent, buff);
			}
*/

	if(m_V.Magnitude2() < SQR(BALL_MOTIONLESSV))
		if(m_W.Magnitude2() < SQR(BALL_MOTIONLESSW))
			{
			//if(!TestFlag(m_flags, Flg_BALL_PUREROLL))
			
			if(TestFlag(m_flags, Flg_BALL_PUREROLL))
				ClearFlag(m_flags, Flg_BALL_PUREROLL);

			if(m_wnroll == 0)
				{				
					// bring 'this' ball to a full stop!
					m_V = CVector(0,0,0);
					m_VOld = CVector(0,0,0);
					m_W = CVector(0,0,0);
					m_wnroll = 0;
					//m_wSide = 0;	// stop balls at rest, from spinning on the spot

					m_WDelay = 0; // NEW

					m_justHitAnotherBall = 0;
					m_justHitCushion = 0;

					ClearFlag(m_flags, Flg_BALL_HITCUSHION | Flg_BALL_HITBALL);

					return TRUE;
				}
			//else {
					//ClearFlag(m_flags, Flg_BALL_PUREROLL);
				//}
			}
	#endif
	// still in motion...
	return FALSE;
}


///////////////////////////////////////////////////////////////////////////////////////////////////
//
//
void CBall::TouchingBallStickyness(CBall *ball)
{
	// if two balls have just collided, and the total difference in velocity is very very small, then stick them together	
	
	CVector DV = m_V - ball->m_V;
	float dv = DV.Magnitude();

	if(m_colour == WHITE || ball->m_colour == WHITE)
	{
		if(m_V.Magnitude() < 0.04f && ball->m_V.Magnitude() < 0.04)
		{
			/*
			// ***these vars are negliable***
			//m_W.Magnitude()<=BALL_SLIPPINGFRICT_W)
			//m_wnroll
			char buf2[256];
			sprintf(buf2, "(m_W = %f, m_wnroll = %f) (ball->m_W = %f, ball->m_wnroll = %f)", m_W.Magnitude(), m_wnroll, ball->m_W.Magnitude(), ball->m_wnroll);
			ErrorMessage(m_parent->m_parent, buf2);
			*/

			CVector D;

			D = m_P - ball->m_P;

			if(D.Magnitude() < ((BALL_RADIUS*2) + 0.6205f))
			{
				// kill ball's velocity!
				m_V = CVector(0,0,0);
				ball->m_V = CVector(0,0,0);
			}
		}
	}
}


///////////////////////////////////////////////////////////////////////////////////////////////////
//
//
BOOL CBall::IsTouchingBall(CBall *ball)
{
	// [note we only call this when the simulation has finished!!!]
	// return TRUE is 'this' ball is touching 'ball'
	
	CVector D;

	D = m_P - ball->m_P;

	if(D.Magnitude() < ((BALL_RADIUS*2) + 0.6205f))
	{
		//m_V = CVector(0,0,0);
		//ball->m_V = CVector(0,0,0);

		return TRUE;
	}

	return FALSE;
}


///////////////////////////////////////////////////////////////////////////////////////////////////
//
//
#ifdef FIXEDMODE
void CBall::RollOff(BOOL bTestBall, int rollOff)
{
	if(!(m_cyc&0x07))
	if(g_bBallRollOff)				
	if(!bTestBall)
	{
	__fix speed;
	speed = f_V.Magnitude();
	if(speed < TOFIX(0.1f)) speed = TOFIX(0.1f);
	speed = FIXDIV(TOFIX(10), speed); // 100..1
	speed = FIXDIV(speed, TOFIX(10)); // 10..0.1
	
	speed = FIXDIV(speed, TOFIX(9.8)); ///<---
		
	FVector P;
	FVector UP = FVector(0, 0, TOFIX(1));
	P = f_V.Cross(UP);
	P = P.Unit();
	P = P * TOFIX(rollOff); // (-9..9) int range
	//P = P * TOFIX(BALL_MOTIONLESSV * 0.0040);
	P = P * TOFIX(BALL_MOTIONLESSV * 0.0080);
	P = P * speed;
		
	f_V = f_V + P;

	//if(!(m_cyc&0xff))
	//{
	//char buf[256];
	//sprintf(buf, "speed = %f, rollOff = %d, P.MAG() = %f", FROMFIXSMALL(speed), rollOff, FROMFIXSMALL(P.Magnitude()));
	//ErrorMessage(g_wnd, buf);
	//}

	//C = C.RotateXY(rnd); an idea?
	}
}
/*
 // TRY IT, JUST HIT THE PACK!!!! this is crazy!!!!!!! <-----------------------------------------------
void CBall::RollOff(BOOL bTestBall, int rollOff)
{
	if(g_bBallRollOff)				
	if(!bTestBall)
	{
	__fix speed;
	speed = f_V.Magnitude();
	if(speed < TOFIX(0.1f)) speed = TOFIX(0.1f);
	speed = FIXDIV(TOFIX(10), speed); // 100..1
	speed = FIXDIV(speed, TOFIX(10)); // 10..0.1
	
	speed = FIXDIV(speed, TOFIX(9.8)); ///<---
		
	FVector P;
	FVector UP = FVector(0, 0, TOFIX(1));
	P = f_V.Cross(UP);
	P = P.Unit();
	P = P * TOFIX(rollOff); // (-9..9) int range
	//P = P * TOFIX(BALL_MOTIONLESSV * 0.0001);
	P = P * speed;
	f_V = f_V + P;

	if(!(m_cyc&0xff))
	{
	char buf[256];
	sprintf(buf, "rollOff = %d, f_V.MAG() = %f", rollOff, FROMFIXSMALL(P.Magnitude()));
	ErrorMessage(g_wnd, buf);
	}

	//C = C.RotateXY(rnd); an idea?
	}
}
*/
#else
void CBall::RollOff(BOOL bTestBall, int rollOff)
{
	if(g_bBallRollOff)				
	if(!bTestBall)
	{
	float speed;
	speed = m_V.Magnitude();
	if(speed < 0.1f) speed = 0.1f;
	speed = 10 / speed; // 100..1
	speed /= 10; // 10..0.1
	
	speed *= 0.8; ///<---
		
	CVector P;
	CVector UP = CVector(0, 0, 1);
	P = m_V.Cross(UP);
	P = P.Unit();
	P = P * rollOff; // (-9..9) int range
	P = P * BALL_MOTIONLESSV * 0.0001;
	P = P * speed;
	m_V = m_V + P;

//	if(!(m_cyc&0xff))
//	{
//	char buf[256];
//	sprintf(buf, "m_V.MAG() = %f", m_V.Magnitude());
//	ErrorMessage(g_wnd, buf);
//	}

	//C = C.RotateXY(rnd); an idea?
	}
}
#endif


///////////////////////////////////////////////////////////////////////////////////////////////////
//
//
#ifdef FIXEDMODE
void CBall::RollingSpin(void)
{
	__fix f1, f2, f3, ff;
	//////////////////// if not in natural roll state

	// work out natural roll spin
	if(TestFlag(m_flags, Flg_BALL_PUREROLL))
		if(f_wnroll < TOFIX(BALL_NATURALROLLW))
			{
				// commence natural roll
				f_wnroll += TOFIX(BALL_NATURALROLLFORCE);
				if(f_wnroll > TOFIX(BALL_NATURALROLLW))
					f_wnroll = TOFIX(BALL_NATURALROLLW);
			}

	// if a collision has occured, see how mush spin the natral roll gives in this instance
	if(TestFlag(m_flags, Flg_BALL_HITCUSHION | Flg_BALL_HITBALL))
	{	
		// get rid of rolling 'stun', ball should carry on due to natural roll spin....
			FVector VOldUnit = f_VOld.Unit();
			
			f1 = f_V.Dot(VOldUnit);
			FVector VInOldDir = VOldUnit * f1;
			
			FVector V = (f_VOld - VInOldDir);


			V = V * TOFIX(BALL_ROLLSPINCOLLISIONFACTOR);
			
			f1 = FIXDIV(f_wnroll,TOFIX(BALL_NATURALROLLW));
			V = V * f1;

			f_W = f_W + V;

			// work our delay before spin comes out // [NEWOAP]
			//float WDelayExtra = FROMFIX(f_W.Magnitude());
			//WDelayExtra	= WDelayExtra * 2;
			//m_WDelay += WDelayExtra;

			//m_WDelay = FROMFIX(f_W.Magnitude());
			//m_WDelay = 0;//m_WDelay * 1;//30;

			ClearFlag(m_flags, Flg_BALL_PUREROLL);
	}

	if(!TestFlag(m_flags, Flg_BALL_PUREROLL))
		f_wnroll = 0;
}
#else
void CBall::RollingSpin(void)
{
	float f1, f2, f3, ff;
	//////////////////// if not in natural roll state

	// work out natural roll spin
	if(TestFlag(m_flags, Flg_BALL_PUREROLL))
		if(m_wnroll < BALL_NATURALROLLW)
			//if(m_W.Magnitude2() < SQR(BALL_MOTIONLESSW))
			{				
				// commence natural roll
				m_wnroll += BALL_NATURALROLLFORCE;
				if(m_wnroll > BALL_NATURALROLLW)
					m_wnroll = BALL_NATURALROLLW;					
			}

	// if a collision has occured, see how mush spin the natral roll gives in this instance
	//if(m_wnroll > 0)
	if(TestFlag(m_flags, Flg_BALL_HITCUSHION | Flg_BALL_HITBALL))
	{	
		// get rid of rolling 'stun', ball should carry on due to natural roll spin....
		//if(m_W.Magnitude() < BALL_MOTIONLESSW)
		//{   
			CVector VOldUnit = m_VOld.Unit();
			
			f1 = m_V.Dot(VOldUnit);
			CVector VInOldDir = VOldUnit * f1;			

			//CVector VInOldDir = (m_V.Dot(VOldUnit)) * VOldUnit;
			
			// V = RW => W = V/R
			//CVector V = m_VOld;//(m_VOld - VInOldDir);
			CVector V = (m_VOld - VInOldDir);

			//m_W = V * BALL_ROLLSPINCOLLISIONFACTOR * (m_wnroll / BALL_NATURALROLLW);
			V = V * BALL_ROLLSPINCOLLISIONFACTOR;
			
			f1 = m_wnroll / BALL_NATURALROLLW;
			V = V * f1;
			//V = V * (m_wnroll / BALL_NATURALROLLW);

			m_W = m_W + V;

			// work our delay before spin comes out // [NEWOAP]
			//float WDelayExtra = m_W.Magnitude();
			//WDelayExtra	= WDelayExtra * 2;
			//m_WDelay += WDelayExtra;

			//m_WDelay = m_W.Magnitude();
			//m_WDelay = 0;//m_WDelay * 1;//30;

			//m_W = m_W + (V * BALL_ROLLSPINCOLLISIONFACTOR * (m_wnroll / BALL_NATURALROLLW)); // SYNC. BUG!!!!!!!
			//m_W = 2;
			/*
			char buf[80];
		sprintf(buf, "%f", m_W.Magnitude());
		ErrorMessage(m_parent->m_parent, buf);
		*/
			/*
			// calc. lost spin (*TODO: NOT SURE ABT THIS??????)
			float lostspin = VInOldDir.Magnitude() / m_VOld.Magnitude();
			lostspin *= BALL_NATURALROLLW;

			m_wnroll -= lostspin;

			if(m_wnroll < 0)
				m_wnroll = 0;
			*/

			//m_wnroll = 0;
		//}
	
			ClearFlag(m_flags, Flg_BALL_PUREROLL);
	}

	if(!TestFlag(m_flags, Flg_BALL_PUREROLL))
		m_wnroll = 0;
}
#endif

/*
///////////////////////////////////////////////////////////////////////////////////////////////////
//
//
BOOL CBall::SlippingFriction(void)
{
	if(m_W.Magnitude2() != 0)
	{
		CVector Fs;
		Fs = m_W;
		Fs = Fs.Unit();
		CVector newW = m_W - (Fs * BALL_SLIPPINGFRICT_W);
		
		if( (m_W.Dot(newW)) == (m_W.Magnitude() * newW.Magnitude() * -1)) {
			m_W = CVector(0, 0, 0); // no slipping spin left, ball is in natural roll
			return FALSE;
		}
		else {
			m_V = m_V + (m_W.Unit() * BALL_SLIPPINGFRICT_V);
			m_W = newW;
			return TRUE;
		}
	}
	else {
		// no spin, so must be in pure rolling motion
		SetFlag(m_flags, Flg_BALL_PUREROLL);
	}

	return FALSE;
}
 */

///////////////////////////////////////////////////////////////////////////////////////////////////
//
//
#ifdef FIXEDMODE
BOOL CBall::SlippingFriction(void)
{
	FVector FF;

	if(f_W.Magnitude2() != 0)
	{
		if(f_W.Magnitude()<=TOFIX(BALL_SLIPPINGFRICT_W)) {
			f_W = FVector(0, 0, 0); // no slipping spin left, ball is in natural roll

			if(f_V.Magnitude2() != 0)
			{
				// no spin, so must be in pure rolling motion
				SetFlag(m_flags, Flg_BALL_PUREROLL);
			}
			return FALSE;
		}
		else {
			FVector Wu = f_W.Unit();

			if(m_WDelay) m_WDelay--;

			if(!m_WDelay)
			{
			FF = Wu * TOFIX(BALL_SLIPPINGFRICT_V);
			f_V = f_V + FF;
			}
			
			////////////////////////////////////////m_V = m_V + (m_W.Unit() * BALL_SLIPPINGFRICT_V);
			FF = Wu * TOFIX(BALL_SLIPPINGFRICT_W);

			if(m_WDelay) FF = FF * TOFIX(0.5);

			//usabap
			/*
			{
				char buf[256];
				sprintf(buf, "*m_WDelay: %d, f_W.x: %f, f_W.y %f, f_V.x: %f, f_V.y %f, f_P.x: %f, f_P.y %f", m_WDelay, (float)FROMFIXSMALL(f_W.x), (float)FROMFIXSMALL(f_W.y), (float)FROMFIXSMALL(f_W.z), (float)FROMFIXSMALL(f_V.x), (float)FROMFIXSMALL(f_V.y), (float)FROMFIXSMALL(f_V.z), (float)FROMFIXSMALL(f_P.x), (float)FROMFIXSMALL(f_P.y));
				ErrorMessage(m_parent->m_parent, buf);
			}
			*/
			

			f_W = f_W - FF;
			return TRUE; // Bad case
		}
	}	
		if(f_V.Magnitude2() != 0)
		{
			// no spin, so must be in pure rolling motion
			SetFlag(m_flags, Flg_BALL_PUREROLL);
		}
	return FALSE;
}
#else
BOOL CBall::SlippingFriction(void)
{
	CVector FF;

	
	


	if(m_W.Magnitude2() != 0)
	{
		if(m_W.Magnitude()<=BALL_SLIPPINGFRICT_W) {					
		//if( (m_W.Dot(newW)) == (m_W.Magnitude() * newW.Magnitude() * -1)) { // False
			m_W = CVector(0, 0, 0); // no slipping spin left, ball is in natural roll

			if(m_V.Magnitude2() != 0)
			{
				// no spin, so must be in pure rolling motion
				SetFlag(m_flags, Flg_BALL_PUREROLL);

				/*
				{
				char buf[256];
				sprintf(buf, "no spin, so must be in pure rolling motion");
				ErrorMessage(m_parent->m_parent, buf);
				}
				*/

			}
			return FALSE;
		}
		else {


			CVector Wu = m_W.Unit();

			
			if(m_WDelay) m_WDelay--;


			
			
			if(!m_WDelay)
			{
				FF = Wu * (BALL_SLIPPINGFRICT_V);
			/*
			////////////////////////////////
			CVector Vu = m_V.Unit();
			float spinfactor = Vu.Dot(Wu);
													// NEW!!!
			if(spinfactor < 0)
				spinfactor = -spinfactor;

			// spinfactor = spinfactor * 0.3;
			// spinfactor = spinfactor + 0.7;

			FF = FF * spinfactor;
			////////////////////////////////
			*/

			/*
			// Find the relative Angular Speed (w.r.t Velocity). nev
			
			// First find the component in direction of Motion
			float spinComponent;
			float velComponent;

			// If we're motionless, then spinComponent is simply our spin completely 
			if(m_V.Magnitude() == 0)
			{
				spinComponent = m_W.Magnitude();
			}
			else
			{
				// Find m_W component in direction of velocity
				CVector m_Vu = m_V.Unit();
				spinComponent = m_W.Dot(m_Vu);
			}
			
			// Find relative m_W component (spinComponent) w.r.t. velocity
			CVector m_Wr = m_W;


			spinComponent = 1.0f / (1.0f + m_Wr.Magnitude());
			spinComponent = pow(spinComponent, 2.0f);
			
		
			
			

			m_V = m_V + m_W * spinComponent * 0.02;
			*/

			m_V = m_V + FF;


			//m_V = m_V + (Wu * BALL_SLIPPINGFRICT_V);
			}
			
			////////////////////////////////////////m_V = m_V + (m_W.Unit() * BALL_SLIPPINGFRICT_V);
			FF = Wu * BALL_SLIPPINGFRICT_W * 1.0;
			// FF = m_W * BALL_SLIPPINGFRICT_W * 1.0;

			if(m_WDelay) FF = FF * 0.5;
			
			m_W = m_W - FF;
			

			//m_W = m_W - (Wu * BALL_SLIPPINGFRICT_W);//newW;
			return TRUE; // Bad case
		}
	}
	
	//else {
		// if ball still has linear velocity
		if(m_V.Magnitude2() != 0)
		{
			// no spin, so must be in pure rolling motion
			SetFlag(m_flags, Flg_BALL_PUREROLL);
		}
	//}

	return FALSE;
}
#endif


///////////////////////////////////////////////////////////////////////////////////////////////////
//
//
#ifdef FIXEDMODE
BOOL CBall::RollingFriction(void)
{
	__fix f1,f2,f3;
	float frict;

	frict = BALL_ROLLINGFACTOR;



	FVector Fr;
	FVector newV;
	if(f_V.Magnitude2() != 0)
	{
		Fr = f_V;
		Fr = Fr.Unit();
		if(g_bBallRollOff)
			Fr = Fr * TOFIX(frict * 16);
		else
		{
			// do old way
			Fr = Fr * TOFIX(frict);
		}	

		newV = f_V;
		newV = newV - Fr;

		f1 = -FIXMUL(f_V.Magnitude(),newV.Magnitude());
		if(f_V.Dot(newV) == f1)
		{
			f_V = FVector(0, 0, 0); // not rolling anymore
		
			/*
			{
				char buf[256];
				sprintf(buf, "not rolling anymore");
				ErrorMessage(m_parent->m_parent, buf);
			}
			*/

			return FALSE;
		}
		else 
		{
			f_V = newV;
			return TRUE;
		}
	}
	return FALSE;
}
#else
BOOL CBall::RollingFriction(void)
{
	float f1,f2;

	CVector Fr;
	CVector newV;
	/*
	// apply rolling friction...
	if(m_V.Magnitude() > BALL_DEADVELOCITY)
	{
		if(m_V.Magnitude() < BALL_ROLLINGFACTOR)
		{	
			m_V = CVector(0,0,0);
			return FALSE;
		}

		Fr = m_V;
		Fr = Fr.Unit();
		Fr = Fr*BALL_ROLLINGFACTOR;

		m_V	= m_V - Fr;
	} 
	//else
	//	return FALSE;

	return TRUE;
	*/
	if(m_V.Magnitude2() != 0)
	{
		Fr = m_V;
		Fr = Fr.Unit();
		Fr = Fr*BALL_ROLLINGFACTOR;
		//Fr = Fr*0.00020;

		//newV = m_V - Fr;
		newV = m_V;
		newV = newV - Fr;

		f1 = m_V.Magnitude() * newV.Magnitude();
		f1 = -f1;
		if(m_V.Dot(newV) == f1)
		//if( (m_V.Dot(newV)) == (m_V.Magnitude() * newV.Magnitude() * -1) )
		{
			m_V = CVector(0, 0, 0); // not rolling anymore
			
			//if(m_W.Magnitude2() == 0)
			//	if(m_wnroll == 0)
			//		ClearFlag(m_flags, Flg_BALL_PUREROLL);
			return FALSE;
		}
		else 
		{
			m_V = newV;
			return TRUE;
		}
	}


	//if(m_W.Magnitude2() == 0)
	//	if(m_wnroll == 0)
	//		ClearFlag(m_flags, Flg_BALL_PUREROLL);

	return FALSE;
}
#endif

///////////////////////////////////////////////////////////////////////////////////////////////////
//
//
#ifdef FIXEDMODE
BOOL CBall::SideSpinFriction(void)
{
	__fix fr;
	__fix newWSide;
 
	if(m_wSide != 0)
	{
	
		fr = TOFIX(0.002);//BALL_SIDESPINFRICT;
		if(m_wSide<0) fr=-fr;
		

		newWSide = f_wSide - fr;

		f_wSide = newWSide;
		return TRUE;

		/*if((newWSide*m_wSide)<=0) // Cross over
			{
			m_wSide = 0; // not side spinning anymore!
			return FALSE;
			}
		else {
			m_wSide = newWSide;
			return TRUE;
			}*/
	} 

	return FALSE;
}
#else
BOOL CBall::SideSpinFriction(void)
{
	float fr;
	float newWSide;

	if(m_wSide != 0)
	{
	
		fr = 0.002;//BALL_SIDESPINFRICT;
		if(m_wSide<0) fr=-fr;
		

		newWSide = m_wSide - fr;

			m_wSide = newWSide;
			return TRUE;

		if((newWSide*m_wSide)<=0) // Cross over
			{
			m_wSide = 0; // not side spinning anymore!
			return FALSE;
			}
		else {
			m_wSide = newWSide;
			return TRUE;
			}
	} 

	return FALSE;
}
#endif

///////////////////////////////////////////////////////////////////////////////////////////////////
//
//
// fixed
void CBall::ApplySideSpinFixed(FVector N, __fix strength)
{
	__fix f1;
 

	N = N.Unit();
	N.z = 0;
	
	// calc. force into cushion
	f1 = f_V.Dot(N);
	FVector F = N * f1;

	__fix fintocushion = F.Magnitude();
	fintocushion = FIXDIV(fintocushion,TOFIX(10.0f));

	if(fintocushion > TOFIX(1.0f)) fintocushion = TOFIX(1.0f);
	
	// work out side spin impulse-force vector, in the plane xy
	FVector Txy = N.Cross(FVector(0, 0, TOFIX(1)));
	Txy.z = 0;

	__fix f = FIXMUL(f_wSide,strength);
	f = FIXMUL(f,fintocushion);

	FVector Fwside = Txy.Unit();
	Fwside = Fwside * f;

	/*
	char buf[256];
	sprintf(buf, "f_V.x = %f, f_V.y = %f, f_V.z = %f", (float)FROMFIXSMALL(f_V.x), (float)FROMFIXSMALL(f_V.y), (float)FROMFIXSMALL(f_V.z));
	ErrorMessage(m_parent->m_parent, buf);
	*/

	// update ball's velocity
	f_V = f_V + Fwside;

	/*
	sprintf(buf, "f_wSide = %f, f_V.x = %f, f_V.y = %f, f_V.z = %f", (float)FROMFIXSMALL(f_wSide), (float)FROMFIXSMALL(f_V.x), (float)FROMFIXSMALL(f_V.y), (float)FROMFIXSMALL(f_V.z));
	ErrorMessage(m_parent->m_parent, buf);
	*/

	// lost spin due to impact!
	// [#] m_wSide = m_wSide * (1 - strength * fintocushion);
	__fix floss;
	floss = FIXMUL(fintocushion,TOFIX(BALL_LOSTSIDEFROMIMPACT));
	f = FIXMUL(strength,floss);
	f1 = TOFIX(1.0f) - f;
	f_wSide = FIXMUL(f_wSide,f1);

	

	// spin gained due to impact!
	Fwside = Txy.Unit();
	f = f_V.Dot(Fwside);
	f = FIXMUL(f,strength);
	f = FIXMUL(f,TOFIX(BALL_SIDEFROMIMPACT));
	f = FIXMUL(f,fintocushion);
	f_wSide = f_wSide + f;
	if(f_wSide > TOFIX(BALL_MAXSPEED*BALL_MAXSPEEDFACTOR))
		f_wSide = TOFIX(BALL_MAXSPEED*BALL_MAXSPEEDFACTOR);

/*	{ // constrain velocity FIXED ONLY
	#define MAXBALLSPIN		5
	if(f_wSide>TOFIX(MAXBALLSPEED))
		{
		f_wSide = TOFIX(MAXBALLSPEED);
		}
	}*/
}
// float ver.
void CBall::ApplySideSpin(CVector N, float strength)
{
	float f1;
 
	/*
	// work out side spin impulse-force vector, in the plane xy
	CVector Txy = N.Cross(CVector(0, 0, 1));
	Txy.z = 0;
	CVector Fwside = (Txy.Unit()) * m_wSide * strength;

	// update ball's velocity
	m_V = m_V + Fwside;
	*/
	
	N = N.Unit();
	N.z = 0;
	
	// calc. force into cushion
	//m_V = VFIXFLOAT(m_V);
	//N = VFIXFLOAT(N);
	f1 = m_V.Dot(N);
	CVector F = N * f1;
	//CVector F = m_V.Dot(N) * N;
	//F = VFIXFLOAT(F);

	float fintocushion = F.Magnitude();
	fintocushion = fintocushion / 10.0f;
	//fintocushion = fintocushion / (BALL_MAXSPEED*BALL_MAXSPEEDFACTOR);
	if(fintocushion > 1.0f)
		fintocushion = 1.0f;
	
	// work out side spin impulse-force vector, in the plane xy
	CVector Txy = N.Cross(CVector(0, 0, 1));
	Txy.z = 0;

	//Txy = VFIXFLOAT(Txy);
	//m_wSide = FIXFLOAT(m_wSide);
	//strength = FIXFLOAT(strength);
	//fintocushion = FIXFLOAT(fintocushion);
	float f = m_wSide * strength;
	//f = FIXFLOAT(f);
	f = f * fintocushion;
	//f = FIXFLOAT(f);

	//CVector Fwside = (Txy.Unit()) * m_wSide * strength * fintocushion;
	CVector Fwside = Txy.Unit();
	Fwside = Fwside * f;
	//Fwside = VFIXFLOAT(Fwside);

	// update ball's velocity
	m_V = m_V + Fwside;

	// lost spin due to impact!
	// [#] m_wSide = m_wSide * (1 - strength * fintocushion);
	float floss;
	floss = fintocushion * BALL_LOSTSIDEFROMIMPACT;
	f = strength * floss;
	f1 = 1.0f - f;
	m_wSide = m_wSide * f1;

	// spin gained due to impact!
	Fwside = Txy.Unit();
	f = m_V.Dot(Fwside);
	f = f * strength;
	f = f * BALL_SIDEFROMIMPACT;
	f = f * fintocushion;
	m_wSide = m_wSide + f;
	if(m_wSide > (BALL_MAXSPEED*BALL_MAXSPEEDFACTOR))
		m_wSide = BALL_MAXSPEED*BALL_MAXSPEEDFACTOR;
	
}

///////////////////////////////////////////////////////////////////////////////////////////////////
//
//
void CBall::NapEffect(void)
{
	float f;

	CVector FNap = CVector(-1, 0, 0);
	f = FNap.Dot(m_V);
	f = f * BALL_NAPFORCE;
	FNap = FNap * f;
	//FNap = FNap.Dot(m_V) * FNap * BALL_NAPFORCE;
	m_V.x += FNap.x;
}


///////////////////////////////////////////////////////////////////////////////////////////////////
//
//

extern BOOL SolveQuadraticFixed(__fix &s1, __fix &s2, __fix a, __fix b, __fix c);

#ifdef FIXEDMODE

//#if 0
BOOL CBall::CollideWithBall(CBall *ball, BOOL bTestBall, BOOL bIsBot, int ballIdx, BOOL SetFlags, BOOL bFreeBallCheck)
{
	
	if(!TestFlag(m_flags, Flg_BALL_ACTIVE))
		return FALSE;
	if(!TestFlag(ball->m_flags, Flg_BALL_ACTIVE))
		return FALSE;

	if(m_ghost) return FALSE;
	if(ball->m_ghost) return FALSE;
  
	FVector D;
	FVector C;
	FVector P;
	__fix r2,R2;
	__fix r,R;

	__fix f1, f2, f3, f4;

	// have we collided?
	D = f_P - ball->f_P;

	r2 = D.Magnitude2();
	R2 = TOFIX((BALL_RADIUS*2)*(BALL_RADIUS*2));
	
	if(r2 < R2) 
	{
		// pull balls apart to where they should have collided (* Fixes jerky exit trajectory :)
		__fix r2mag;
		
		
		for(int step = 0; step < 300; step++)
		{
			//[P = m_P - ball->m_P;]
			P = f_P;
			P = P - ball->f_P;
			
			//R2 = P.Magnitude();
			r2mag = P.Magnitude2();
	
			f1 = TOFIX(BALL_RADIUS*2.0f);
			f2 = TOFIX(1.0f - 0.001f);
			f3 = FIXMUL(f1, f2);
			f3 = FIXMUL(f3, f3);
			f1 = TOFIX(BALL_RADIUS*2.0f);
			f1 = FIXMUL(f1, TOFIX(1.001f));
			f1 = FIXMUL(f1, f1);
			if((r2mag > f3) && (r2mag < f1))
			//if((r2mag > SQR((BALL_RADIUS*2.0f)*(1-0.001f))) && (r2mag < SQR((BALL_RADIUS*2.0f)*1.001f)))
				break;
			
			//{
			//char buf[256];
			//sprintf(buf, "step#%d: ball1 colour = %d; ball2 colour = %d", step, m_colour, ball->m_colour);
			//ErrorMessage(m_parent->m_parent, buf);
			//}

			
			FVector V = f_V * TOFIX(0.01f);
			f_P = f_P - V;
			//m_P = m_P - m_V*0.01;
			V = ball->f_V * TOFIX(0.01f);
			ball->f_P = ball->f_P - V;
			//ball->m_P = ball->m_P - ball->m_V*0.01;
		}
 		
		//*NEW - fix hit wrong ball 1st bug, when red is close to another ball
		// check if white ball is on top of another ball, now that we have moved away from the current ball
		if(bTestBall)
		{
			if(m_parent->bIsStrikeBallOnTopOfAnother(-1, ballIdx))		// testball
				return FALSE; // try another ball...
		}
		else
		{
			//if(m_parent->bIsStrikeBallOnTopOfAnother(0, ballIdx))		// whiteball
			//	return FALSE; // try another ball...
		}

		//////////////////////////////////////////////////////////////////////////////////////////////

		
		if(bTestBall)
		{
			FVector V,P;
			FVector Vn,Vt;
			// *** Note: for Exit Tajectory line calc. ***
			// 'other' ball bounces off 'this' ball
			f_VOld = f_V; // save input balls velocolity for a later in strike
			m_parent->m_TestBallCollideVA.x = FROMFIXSMALL(f_V.x);
			m_parent->m_TestBallCollideVA.y = FROMFIXSMALL(f_V.y);
			m_parent->m_TestBallCollideVA.z = FROMFIXSMALL(f_V.z);

			//[V = m_V - ball->m_V;]
			V = f_V;
			V = V - ball->f_V;

			//[P = ball->m_P - m_P;]
			P = ball->f_P;
			P = P - f_P;

			P = P.Unit();
			//m_V = ((V.Dot(P)) * P * BALL_ENERGYFACTOR); // (**) - make test ball be exit traj. ball
			f_V = P;
			__fix f = V.Dot(P);
			if(f < 0) f = -f; // BUGFIX for ghost ball flipping to oppsite side [*NEW*]
			f_V = f_V * f;
			f_V = f_V * TOFIX(BALL_ENERGYFACTOR);

			// lost energy in collision
			Vn = P;
			f =  f_V.Dot(P);
			if(f < 0) f = -f; // BUGFIX for ghost ball flipping to oppsite side [*NEW*]
			Vn = Vn * f;
			
			Vt = f_V;
			Vt = Vt - Vn;

			Vt = Vt * TOFIX(BALL_ENERGYFACTOR_T);
			f_V = Vn * TOFIX(BALL_ENERGYFACTOR_N);
			f_V = f_V + Vt;

			// ------------------------------------------ //


			f_P = ball->f_P;  // (**)
			m_colour = ball->m_colour;

			m_parent->m_TestBallCollideObjColour = ball->m_colour;

			m_parent->m_TestBallCollideObjPos.x = FROMFIXSMALL(ball->f_P.x);
			m_parent->m_TestBallCollideObjPos.y = FROMFIXSMALL(ball->f_P.y);
			m_parent->m_TestBallCollideObjPos.z = FROMFIXSMALL(ball->f_P.z);

			m_parent->m_TestBallCollideVB.x = FROMFIXSMALL(f_V.x);
			m_parent->m_TestBallCollideVB.y = FROMFIXSMALL(f_V.y);
			m_parent->m_TestBallCollideVB.z = FROMFIXSMALL(f_V.z);

			return TRUE;
		}
		else {
			FVector SaveV = ball->f_V;
			FVector V,P;
			__fix fV;
			FVector Vt,Vn;
			__fix f;
// ---------------------------------------------------------------			
			// 'other' ball bounces off 'this' ball ///////////
			V = f_V - ball->f_V;	// V = this ball's speed relative to other ball.
			P = ball->f_P - f_P;	// P = other ball pos relative to this ball

			P = P.Unit(TRUE);
			fV = V.Dot(P);

			V = P * fV;

			V = V *	TOFIX(BALL_ENERGYFACTOR);
			ball->f_V = ball->f_V + V;								

			// lost energy in collision
			f = ball->f_V.Dot(P);
			Vn = P * f;
			
			Vt = ball->f_V - Vn;
			
			Vt = Vt * TOFIX(BALL_ENERGYFACTOR_T);
			ball->f_V = Vn*TOFIX(BALL_ENERGYFACTOR_N);
			ball->f_V = ball->f_V + Vt;
// ----------------------------------------------------------------
		
			/*
			{
			char buf[256];
			float vx, vy;
			vx = FROMFIXSMALL(f_V.x);
			vy = FROMFIXSMALL(f_V.y);
			sprintf(buf, "xv1 = %f, yv1 = %f", vx, vy);
			ErrorMessage(m_parent->m_parent, buf);
			}
			*/


			// 'this' ball bounces off 'other' ball /////////////
			V = SaveV - f_V;
			P = f_P - ball->f_P;

			P = P.Unit(TRUE);
			fV = V.Dot(P);


			/*
			FVector A = FVector(TOFIX(25.0f),TOFIX(25.0f),0);
			A = A.Unit();
			FVector B = FVector(TOFIX(0.707f), TOFIX(0.707f), 0);
			B = B * TOFIX(10.0f);
			__fix fixa = B.Dot(A);
			float fa = FROMFIXSMALL(fixa);
			*/
			/*
			{
			char buf[256];
			float vx, vy;
			vx = FROMFIXSMALL(P.x);
			vy = FROMFIXSMALL(P.y);
			sprintf(buf, "Px = %f, Py = %f", vx, vy);
			ErrorMessage(m_parent->m_parent, buf);

			vx = FROMFIXSMALL(fV);
			//vy = FROMFIXSMALL(fV.y);
			
			sprintf(buf, "fVx = %f, fVy = %f", vx, vx);
			ErrorMessage(m_parent->m_parent, buf);

			}
			*/
			V = P * fV;

			V = V *	TOFIX(BALL_ENERGYFACTOR);
			f_V = f_V + V;	

			// lost energy in collision			
			f = f_V.Dot(P);
			Vn = P * f;
					
			Vt = f_V - Vn;
			
			Vt = Vt * TOFIX(BALL_ENERGYFACTOR_T);
			f_V = Vn*TOFIX(BALL_ENERGYFACTOR_N);
			f_V = f_V + Vt;
			/*
			{
			char buf[256];
			float vx, vy;
			vx = FROMFIXSMALL(f_V.x);
			vy = FROMFIXSMALL(f_V.y);
			sprintf(buf, "xv2 = %f, yv2 = %f", vx, vy);
			ErrorMessage(m_parent->m_parent, buf);
			}
		   */
// -------------------------------------------------------------------
			m_ghost = 1;
			ball->m_ghost=1;


			//goto end;


			if(g_gfxDetails.bSoundBalls)
			if(!bIsBot)
			{
				// play ball2ball collision sfx
				//CVector SfxAmp = ((V.Dot(P)) * P * BALL_ENERGYFACTOR_N);
				f = V.Dot(P);
				//float ff;
				//ff = FROMFIXSMALL(f);
				FVector SfxAmp = P;
				SfxAmp = SfxAmp * f;
				SfxAmp = SfxAmp * TOFIX(BALL_ENERGYFACTOR_N);

				float SfxAmpMag = FROMFIXSMALL(SfxAmp.Magnitude());
				if(SfxAmpMag > 5.0f)
					SfxAmpMag = 5.0f;
				SfxAmpMag/=5.0f;
				
				if(!m_parent->m_firstFewFrames)
				{
				Msfx_smpvol(&((CGameAppView *)m_parent->m_parent)->m_game_smp[SFX_BALL2BALL], 255*SfxAmpMag);
				Msfx_smpplay(&((CGameAppView *)m_parent->m_parent)->m_game_smp[SFX_BALL2BALL]);		
				//Msfx_smpplayvol(&((CGameAppView *)m_parent->m_parent)->m_game_smp[SFX_BALL2BALL], 255*SfxAmpMag);
				}
			}

			
			// have we collided?
			FVector DD = f_P - ball->f_P;

			__fix r2 = DD.Magnitude2();
			__fix R2 = TOFIX((BALL_RADIUS*2)*(BALL_RADIUS*2));
/*
			// OLD WAY!!!!
			//if(r2 < R2) 
			{
			// ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
			// move balls apart (in the direction of their centres)
			//P = (m_P + ball->m_P) * 0.5f;
		 
			P = f_P + ball->f_P;
			P = P * TOFIX(0.5);
			V = f_P - ball->f_P;
			V = V.Unit();

			// check for zero unit vector (i.e.two balls perfectly on top of each other)
			if(V.Magnitude() == 0)
				V.x = TOFIX(1.0f);			

			V = V * TOFIX(BALL_RADIUS + BALL_GAS*20);//*20);
			//f_P = P + V; // messed
			//ball->f_P = P - V; // messed

			f_P.x = P.x + V.x;
			ball->f_P.x = P.x - V.x;

			// ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
			}
*/			

			
			if(r2 < R2) 
			{
			// ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
			// move balls apart (in the direction of their centres)
			V = f_P - ball->f_P;
			V = V.Unit();

			// check for zero unit vector (i.e.two balls perfectly on top of each other)
			if(V.Magnitude() == 0)
				V.x = TOFIX(1.0f);

			//V = V*TOFIX(2.0f);

			f_P = f_P + V;
			ball->f_P = ball->f_P - V;

			//{
			//char buf[256];
			//sprintf(buf, "ball1 colour = %d; ball2 colour = %d", m_colour, ball->m_colour);
			//ErrorMessage(m_parent->m_parent, buf);
			//}

			// ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
			}

			//{
			//char buf[256];
			//sprintf(buf, "ball1 colour = %d; ball2 colour = %d", m_colour, ball->m_colour);
			//ErrorMessage(m_parent->m_parent, buf);
			//}

			/////////////////////////////////////////////////////////////////

			{
			// { ball throw * spin induced by collision (m_W) }
			// ball 1: throw
			__fix fwsidetransfer = f_wSide;
			FVector Vsur_nor;
			
			//[Vsur_nor = m_P - ball->m_P;]
			Vsur_nor = f_P;
			Vsur_nor = Vsur_nor - ball->f_P;
			
			ApplySideSpinFixed(Vsur_nor, TOFIX(1.0f));

			// ball 2: throw
			fwsidetransfer = FIXMUL(fwsidetransfer,TOFIX(BALL_SIDETRANSFERFACTOR));
			ball->f_wSide = ball->f_wSide - fwsidetransfer;
			//Vsur_nor = ball->m_P - m_P;
			ball->ApplySideSpinFixed(Vsur_nor, TOFIX(1.0f));
			}

			/////////////////////////////////////////////////////////////////
			end:
			// set hit flags
			SetFlag(m_flags, Flg_BALL_HITBALL);
			SetFlag(ball->m_flags, Flg_BALL_HITBALL);
			return TRUE;
		}
	//return TRUE;
	}
	return FALSE;
}
//#endif //0

#else
BOOL CBall::CollideWithBall(CBall *ball, BOOL bTestBall, BOOL bIsBot, int ballIdx, BOOL SetFlags, BOOL bFreeBallCheck)
{
	CVector V;
	CVector P;
	CVector SaveV;
	CVector Vn, Vt;

	float f1, f2, f3, ff;

	if(!TestFlag(m_flags, Flg_BALL_ACTIVE))
		return FALSE;
	if(!TestFlag(ball->m_flags, Flg_BALL_ACTIVE))
		return FALSE;

	if(m_ghost) return FALSE;
	if(ball->m_ghost) return FALSE;

	#ifdef FIXEDMODE
	UpdateFloats();
	ball->UpdateFloats();
	#endif
  
	// have we collided?
	P = m_P - ball->m_P;

	f1 = P.Magnitude2();
	f2 = BALL_RADIUS*2;
	f2 = f2 * f2;
	if(f1 < f2) {

		// If we're checking for FREE BALL, we can exit now.. don't need anything else and don't wanna update any positions
		if(bFreeBallCheck)
		{
			return true;
		}


	//if((P.Magnitude2()) < SQR(BALL_RADIUS*2) ) {
		
		// pull balls apart to where they should have collided (* Fixes jerky exit trajectory :)
		float r2mag;
		//CVector R2;
		//for(int step = 0; step < 128; step++)
		//for(int step = 0; step < 256; step++)
		for(int step = 0; step < 300; step++)
		//for(int step = 0; step < 400; step++)
		{
			//[P = m_P - ball->m_P;]
			P = m_P;
			P = P - ball->m_P;
			
			//R2 = P.Magnitude();
			r2mag = P.Magnitude2();
			
			f1 = BALL_RADIUS*2.0f;
			f2 = 1.0f - 0.001f;
			f3 = f1 * f2;
			f3 = f3 * f3;
			f1 = BALL_RADIUS*2.0f;
			f1 = f1 * 1.001f;
			f1 = f1 * f1;
			if((r2mag > f3) && (r2mag < f1))
			//if((r2mag > SQR((BALL_RADIUS*2.0f)*(1-0.001f))) && (r2mag < SQR((BALL_RADIUS*2.0f)*1.001f)))
				break;
			CVector V = m_V * 0.01f;
			m_P = m_P - V;
			//m_P = m_P - m_V*0.01;
			V = ball->m_V * 0.01f;
			ball->m_P = ball->m_P - V;
			//ball->m_P = ball->m_P - ball->m_V*0.01;

						

		}
		
		//*NEW - fix hit wrong ball 1st bug, when red is close to another ball
		// check if white ball is on top of another ball, now that we have moved away from the current ball
		if(bTestBall)
		{
			if(m_parent->bIsStrikeBallOnTopOfAnother(-1, ballIdx))		// testball
				return FALSE; // try another ball...
		}
		else
		{
			//if(m_parent->bIsStrikeBallOnTopOfAnother(0, ballIdx))		// whiteball
			//	return FALSE; // try another ball...
		}


		// if balls have just collided before, then dont take of so much energy
		float _thisEnergyLossFactor = m_justHitAnotherBall; // from previous collision
		_thisEnergyLossFactor = _thisEnergyLossFactor / 180; //90
		float _ballEnergyLossFactor = ball->m_justHitAnotherBall; // from previous collision
		_ballEnergyLossFactor = _ballEnergyLossFactor / 180; //90	

		if(!g_bBallJustHitEnergy)
		{
			_thisEnergyLossFactor = 0;
			_ballEnergyLossFactor = 0;
		}

		#ifndef THE_POOL_CLUB
		if(m_parent->m_cueError <= 0) // NOT semi-pro, pro
		{
			_thisEnergyLossFactor = 0;
			_ballEnergyLossFactor = 0;
		}
		#endif //!POOL_CLUB

		m_justHitAnotherBall = 20;
		ball->m_justHitAnotherBall = 20;

		//float _thisJustHitCushionFactor = (m_justHitCushion*_thisEnergyLossFactor)/200 + 1;
		//float _ballJustHitCushionFactor = (ball->m_justHitCushion*_ballEnergyLossFactor)/200 + 1;
		
		//////////////////////////////////////////////////////////////////////////////////////////////


		if(bTestBall)
		{
			// *** Note: for Exit Tajectory line calc. ***
			// 'other' ball bounces off 'this' ball
			m_VOld = m_V; // save input balls velocolity for a later in strike
			m_parent->m_TestBallCollideVA = m_V;

			//[V = m_V - ball->m_V;]
			V = m_V;
			V = V - ball->m_V;

			//[P = ball->m_P - m_P;]
			P = ball->m_P;
			P = P - m_P;

			P = P.Unit();
			//m_V = ((V.Dot(P)) * P * BALL_ENERGYFACTOR); // (**) - make test ball be exit traj. ball
			m_V = P;
			float f = V.Dot(P);
			//char buff[80];
			//sprintf(buff, "f = %f", f);
			//ErrorMessage(g_wnd, buff);
			if(f < 0) f = -f; // BUGFIX for ghost ball flipping to oppsite side [*NEW*]

			m_V = m_V * f;
			m_V = m_V * BALL_ENERGYFACTOR;

			//m_V = ((V.Dot(P)) * P); // (**) - make test ball be exit traj. ball

			// lost energy in collision
			//Vn = (m_V.Dot(P)) * P;
			Vn = P;
			f =  m_V.Dot(P);
			if(f < 0) f = -f; // BUGFIX for ghost ball flipping to oppsite side [*NEW*]
			Vn = Vn * f;
			//Vt = (m_V - Vn) * BALL_ENERGYFACTOR_T;
			
			//[Vt = m_V - Vn;]
			Vt = m_V;
			Vt = Vt - Vn;

			Vt = Vt * BALL_ENERGYFACTOR_T;
			//m_V = Vn*BALL_ENERGYFACTOR_N + Vt;
			m_V = Vn*BALL_ENERGYFACTOR_N;
			m_V = m_V + Vt;

			// ------------------------------------------ //
////			CVector Vsur_nor;
////			Vsur_nor = m_P - ball->m_P; // for use later...

			m_P = ball->m_P;  // (**)
			m_colour = ball->m_colour;
			m_parent->m_TestBallCollideObjColour = ball->m_colour;
			m_parent->m_TestBallCollideObjPos = ball->m_P;
			m_parent->m_TestBallCollideVB = m_V;

////			// ball throw * spin induced by collision (m_W)
////			float fwsidetransfer = m_wSide;
////			fwsidetransfer = fwsidetransfer * BALL_SIDETRANSFERFACTOR;
////			m_wSide = -fwsidetransfer;
////			ApplySideSpin(Vsur_nor, 1.0f);

			
			//SetFlag(m_flags, Flg_BALL_HITBALL);
			//SetFlag(ball->m_flags, Flg_BALL_HITBALL);


	#ifdef FIXEDMODE
	UpdateFixeds();
	ball->UpdateFixeds();
	#endif
			return TRUE;

		}
		//////////////////////////////////////////////////////////////////////////////////////////////
		else 
		{

				




			// *** Note: ordinary ball 2 ball collision code path ***
			// save 'other' ball's velocity
			SaveV = ball->m_V;
			float fV, f, energy;

			// 'other' ball bounces off 'this' ball
			V = m_V;
			V = V - ball->m_V;

						

			P = ball->m_P;
			P = P - m_P;

			P = P.Unit();
			
			fV = V.Dot(P);


			
			

			//if(fV < 0) fV = -fV; // BUGFIX for ghost ball flipping to oppsite side [*NEW*]
			V = P;
			V = V * fV;





			//energy = _ballEnergyLossFactor;
			//energy = energy + BALL_ENERGYFACTOR;
			//f1 = _ballEnergyLossFactor + 1;
			//energy /= f1;
			V = V *	BALL_ENERGYFACTOR;//energy;
			ball->m_V = ball->m_V + V;
						
			// lost energy in collision
			Vn = P;
			f = ball->m_V.Dot(P);
			//if(f < 0) f = -f; // BUGFIX for ghost ball flipping to oppsite side [*NEW*]
			Vn = Vn * f;
			
			Vt = ball->m_V;
			Vt = Vt - Vn;
			
			//energy = _ballEnergyLossFactor;
			//energy = energy + BALL_ENERGYFACTOR_T;
			//f1 = _ballEnergyLossFactor + 1;
			//energy /= f1;
			energy = 1 - BALL_ENERGYFACTOR_T;
			energy = energy * _ballEnergyLossFactor;
			energy = energy + BALL_ENERGYFACTOR_T;
			Vt = Vt * energy;
			//Vt = Vt * _ballJustHitCushionFactor;

			//energy = _ballEnergyLossFactor;
			//energy = energy + BALL_ENERGYFACTOR_N;
			//f1 = _ballEnergyLossFactor + 1;
			//energy /= f1;
			energy = 1 - BALL_ENERGYFACTOR_N;
			energy = energy * _ballEnergyLossFactor;
			energy = energy + BALL_ENERGYFACTOR_N;
			ball->m_V = Vn;
			ball->m_V = ball->m_V * energy;
			ball->m_V = ball->m_V + Vt;

			// 'this' ball bounces off 'other' ball


			V = SaveV - m_V;
			
			P = m_P;
			P = P - ball->m_P;
						
			P = P.Unit();
			fV = V.Dot(P);
			//if(fV < 0) fV = -fV; // BUGFIX for ghost ball flipping to oppsite side [*NEW*]

			V = P;
			V = V * fV;

			//energy = _thisEnergyLossFactor;
			//energy = energy + BALL_ENERGYFACTOR;
			//f1 = _thisEnergyLossFactor + 1;
			//energy /= f1;
			V = V *	BALL_ENERGYFACTOR;//energy;
			m_V = m_V + V;

			
			// lost energy in collision
			
			Vn = P;			
			f = m_V.Dot(P);
			//if(f < 0) f = -f; // BUGFIX for ghost ball flipping to oppsite side [*NEW*]
			Vn = Vn * f;
			
		
			Vt = m_V;
			Vt = Vt - Vn;


			
			//energy = _thisEnergyLossFactor;
			//energy = energy + BALL_ENERGYFACTOR_T;
			//f1 = _thisEnergyLossFactor + 1;
			//energy /= f1;
			energy = 1 - BALL_ENERGYFACTOR_T;
			energy = energy * _thisEnergyLossFactor;
			energy = energy + BALL_ENERGYFACTOR_T;
			Vt = Vt * energy;
			//Vt = Vt * _thisJustHitCushionFactor;
			
			//energy = _thisEnergyLossFactor;
			//energy = energy + BALL_ENERGYFACTOR_N;
			//f1 = _thisEnergyLossFactor + 1;
			//energy /= f1;
			energy = 1 - BALL_ENERGYFACTOR_N;
			energy = energy * _thisEnergyLossFactor;
			energy = energy + BALL_ENERGYFACTOR_N;
			m_V = Vn;
			m_V = m_V * energy;
			m_V = m_V + Vt;

			
	
			
			
			
									
			m_ghost = 1;
			ball->m_ghost=1;
			
			
			if(g_gfxDetails.bSoundBalls)
			if(!bIsBot)
			{
				// play ball2ball collision sfx
				//CVector SfxAmp = ((V.Dot(P)) * P * BALL_ENERGYFACTOR_N);
				f = V.Dot(P) + 0.5f;
				CVector SfxAmp = P;
				SfxAmp = SfxAmp * f;
				SfxAmp = SfxAmp * BALL_ENERGYFACTOR_N;
				float SfxAmpMag = SfxAmp.Magnitude();
				if(SfxAmpMag > 5.0f)
					SfxAmpMag = 5.0f;
				//SfxAmpMag/=5.0f;
				SfxAmpMag = sqrt(SfxAmpMag);
				SfxAmpMag = sqrt(SfxAmpMag);
				
				if(!m_parent->m_firstFewFrames)
				{
				Msfx_smpvol(&((CGameAppView *)m_parent->m_parent)->m_game_smp[SFX_BALL2BALL], 50 + SfxAmpMag * 70);
				Msfx_smpplay(&((CGameAppView *)m_parent->m_parent)->m_game_smp[SFX_BALL2BALL]);		
				}
			}			
			
			// ------------------------------------------ //
			
			// move balls apart (in the direction of their centres)
			//P = (m_P + ball->m_P) * 0.5f;
			
			//[P = m_P + ball->m_P;]
			P = m_P;
			P = P + ball->m_P;
			
			P = P * 0.5f;
			
			//[V = (m_P - ball->m_P);]
			V = m_P;
			V = V - ball->m_P;

			V = V.Unit();

			// check for zero unit vector (i.e.two balls perfectly on top of each other)
			if(V.Magnitude() == 0)
				V.x = 1.0f;
			
			f1 = BALL_RADIUS + BALL_GAS;
			V = V * f1;	
			
			m_P = P;
			m_P = m_P + V;

			ball->m_P = P;
			ball->m_P = ball->m_P - V;
			
			// lost energy in collision
			
			
			/////////////////////////////////////////////////////////////////
			
			// { ball throw * spin induced by collision (m_W) }
			// ball 1: throw
			float fwsidetransfer = m_wSide;
			CVector Vsur_nor;
			
			//[Vsur_nor = m_P - ball->m_P;]
			Vsur_nor = m_P;
			Vsur_nor = Vsur_nor - ball->m_P;
			
			ApplySideSpin(Vsur_nor, 1.0f);

			// ball 2: throw
			fwsidetransfer = fwsidetransfer * BALL_SIDETRANSFERFACTOR;
			ball->m_wSide = ball->m_wSide - fwsidetransfer;
			//Vsur_nor = ball->m_P - m_P;

			ball->ApplySideSpin(Vsur_nor, 1.0f);

			/////////////////////////////////////////////////////////////////

			// *NEW*
			TouchingBallStickyness(ball);

			// set hit flags
			if(SetFlags)
				{
				SetFlag(m_flags, Flg_BALL_HITBALL);
				SetFlag(ball->m_flags, Flg_BALL_HITBALL);
				}

	#ifdef FIXEDMODE
	UpdateFixeds();
	ball->UpdateFixeds();
	#endif
			
			return TRUE;			
		}
	}

	return FALSE;
}
#endif



// This function doesn't update any variable or set any flag, it simply checks if there was a collision for this ball //
BOOL CBall::CollideWithBallSimple(CBall *ball)
{
	CVector P;
	float f1, f2;

	if(!TestFlag(m_flags, Flg_BALL_ACTIVE)) 	return FALSE;
	if(!TestFlag(ball->m_flags, Flg_BALL_ACTIVE)) return FALSE;

	#ifdef FIXEDMODE
	UpdateFloats();
	ball->UpdateFloats();
	#endif
  
	// Check if we've collided //
	P = m_P - ball->m_P;
	f1 = P.Magnitude2();
	f2 = BALL_RADIUS*2;
	f2 = f2 * f2;
	if(f1 < f2) return TRUE;	
		else return FALSE;
}

float __DOT(CVector *a, CVector *b) // NOTE: Do NOT use this function!!!!! (Because it can cause out of sync bugs!!!)
{
	float f1,f2,f3; // works ok in this manner.
	f1 = (a->x*b->x);
	f2 = (a->y*b->y);
	f3 = (a->z*b->z);
	return f1+f2+f3;//(a->x*b->x)+(a->y*b->y)+(a->z*b->z);
}

BOOL CBall::CollideWithBall2(CBall *ball, BOOL bTestBall) // NOTE: this function is not used!!!
{
	CVector V;
	CVector P;
	CVector SaveV;
	CVector Vn, Vt;
//	fprintf(fptr2,"CWB2:");

	if(!TestFlag(m_flags, Flg_BALL_ACTIVE))
		return FALSE;
	if(!TestFlag(ball->m_flags, Flg_BALL_ACTIVE))
		return FALSE;

	if(m_ghost) return FALSE;
	if(ball->m_ghost) return FALSE;
	
  
	// have we collided?
	P = m_P - ball->m_P;
	if((P.Magnitude2()) < SQR(BALL_RADIUS*2) ) {

		// pull balls apart to where they should have collided (* Fixes jerky exit trajectory :)
		float r2mag;
		//CVector R2;
		
		for(int step = 0; step < 128; step++)
		{
			P = m_P - ball->m_P;
			//R2 = P.Magnitude();
			r2mag = P.Magnitude2();
			if((r2mag > SQR((BALL_RADIUS*2.0f)*(1-0.001f))) && (r2mag < SQR((BALL_RADIUS*2.0f)*1.001f)))
				break;
			CVector V = m_V * 0.01f;
			m_P = m_P - V;
			//m_P = m_P - m_V*0.01;
			V = ball->m_V * 0.01f;			
			ball->m_P = ball->m_P - V;
			//ball->m_P = ball->m_P - ball->m_V*0.01;			
		}
		
		
		//////////////////////////////////////////////////////////////////////////////////////////////
		if(bTestBall)
		{
			
			// *** Note: for Exit Tajectory line calc. ***
			// 'other' ball bounces off 'this' ball
			V = m_V - ball->m_V;
			P = ball->m_P - m_P;
			P = P.Unit();
			m_V = ((V.Dot(P)) * P * BALL_ENERGYFACTOR); // (**) - make test ball be exit traj. ball
			//m_V = ((V.Dot(P)) * P); // (**) - make test ball be exit traj. ball
			
			// lost energy in collision
			Vn = (m_V.Dot(P)) * P;
			Vt = (m_V - Vn) * BALL_ENERGYFACTOR_T;
			m_V = Vn*BALL_ENERGYFACTOR_N + Vt;

			// ------------------------------------------ //
		
			m_P = ball->m_P;  // (**)
			m_colour = ball->m_colour;

			// *TODO - ball throw * spin induced by collision (m_W)

			SetFlag(m_flags, Flg_BALL_HITBALL);
			SetFlag(ball->m_flags, Flg_BALL_HITBALL);

			return TRUE;
		}
		//////////////////////////////////////////////////////////////////////////////////////////////
		else 
		{

			
			// *** Note: ordinary ball 2 ball collision code path ***
			// save 'other' ball's velocity
			// pink ball is the ill ball.ball 20

			SaveV = ball->m_V;
			

			// 'other' ball bounces off 'this' ball
			V = m_V - ball->m_V;
			P = ball->m_P - m_P;
			P = P.Unit();
			
			float fV = V.Dot(P);
			V = P * fV;
			V = V *	BALL_ENERGYFACTOR;
			ball->m_V = ball->m_V + V;
			//ball->m_V = ball->m_V + ((V.Dot(P)) * P * BALL_ENERGYFACTOR);
			//ball->m_V = ball->m_V + ((V.Dot(P)) * P);
						
			// lost energy in collision
			Vn = (ball->m_V.Dot(P)) * P;
			Vt = (ball->m_V - Vn) * BALL_ENERGYFACTOR_T;
			ball->m_V = Vn*BALL_ENERGYFACTOR_N + Vt;
	
			// ok 
			// 'this' ball bounces off 'other' ball
			V = SaveV - m_V;
			P = m_P - ball->m_P;
			
			P = P.Unit();

			
			fV = V.Dot(P);
			V = P * fV;
			V = V *	BALL_ENERGYFACTOR;
			
			
			m_V = m_V + V;
			//m_V = m_V + ((V.Dot(P)) * P * BALL_ENERGYFACTOR);
			//m_V = m_V + ((V.Dot(P)) * P);
			

			// lost energy in collision
			// bad
			FPRINTFX("[m_V]", &m_V.x,&m_V.y,&m_V.z); // static
			FPRINTFX("[P]", &P.x,&P.y,&P.z); // static
			// m_V ok here.

			//Vn = (m_V.Dot(P)) * P; // illness
			float s;
			float s0 = 0,s1 = 0,s2 = 0;
			float xxxx,yyyy;

			//s = (m_V.x*P.x + m_V.y*P.y + m_V.z*P.z);
			s0 = m_V.x*P.x;
			s1 = m_V.y*P.y;
			s2 = m_V.z*P.z;

			s = s0+s1+s2;			// but can do this - perhaps something to do with a stack!!!!
			xxxx = s; // Good 

			//s = (m_V.x*P.x) + (m_V.y*P.y);// + m_V.z*P.z); // can't do this
			s = (m_V.Dot(P)); // illness
			//s = __DOT(&m_V,&P);
			yyyy = s; // Bad

			FPRINTFX("[sss]", &xxxx,&yyyy,&s2); // dynamic			
			FPRINTFX("[sss]", &s,&s1,&s2); // dynamic
			
			//s = m_V.Dot(P);
			Vn = s * P;
			
			FPRINTFX("[Vn]", &Vn.x,&Vn.y,&Vn.z); // dynamic

			Vt = (m_V - Vn) * BALL_ENERGYFACTOR_T;
			//m_V = Vn*BALL_ENERGYFACTOR_N + Vt;// + Vt.x;
						
			float x = Vn.x * BALL_ENERGYFACTOR_N + Vt.x;
			float y = Vn.y * BALL_ENERGYFACTOR_N + Vt.y;
			float z = 0;
			
			m_V.x = x; // problem occurs when you set these vars with values ??? Don't know why!!!! [-]
			m_V.y = y;
			// ^^^^^^^^^^^^^^^^^^^^^^^ m_V bad here.		 

			//m_V.x = Vn.x * BALL_ENERGYFACTOR_N + Vt.x;
			//m_V.y = Vn.y * BALL_ENERGYFACTOR_N + Vt.y;


//			m_V.x = 0.0f;
//			m_V.y = 0.0f;
//			m_V.z = 0.0f;
			
					

						
			m_ghost = 1;
			ball->m_ghost=1;
			
			
			if(g_gfxDetails.bSoundBalls)
			{
				// play ball2ball collision sfx
				CVector SfxAmp = ((V.Dot(P)) * P * BALL_ENERGYFACTOR_N);
				float SfxAmpMag = SfxAmp.Magnitude();
				if(SfxAmpMag > 5.0f)
					SfxAmpMag = 5.0f;
				SfxAmpMag/=5.0f;
				
				if(!m_parent->m_firstFewFrames)
				{
				Msfx_smpvol(&((CGameAppView *)m_parent->m_parent)->m_game_smp[SFX_BALL2BALL], 255*SfxAmpMag);
				Msfx_smpplay(&((CGameAppView *)m_parent->m_parent)->m_game_smp[SFX_BALL2BALL]);		
				//Msfx_smpplayvol(&((CGameAppView *)m_parent->m_parent)->m_game_smp[SFX_BALL2BALL], 255*SfxAmpMag);
				}
			}			
			
			// ------------------------------------------ //
//#if 0									
					
			// move balls apart (in the direction of their centres)
			P = (m_P + ball->m_P) * 0.5f;
			V = (m_P - ball->m_P);
			V = V.Unit();
			V = V * (BALL_RADIUS + BALL_GAS);
						//m_P = m_P + m_V;
			m_P = P + V;
			ball->m_P = P - V;

			// lost energy in collision
			//m_V = m_V * BALL_ENERGYFACTOR2;
			//ball->m_V = ball->m_V * BALL_ENERGYFACTOR2;
			
			// *TODO - ball throw * spin induced by collision (m_W)

//#endif //0
			SetFlag(m_flags, Flg_BALL_HITBALL);
			SetFlag(ball->m_flags, Flg_BALL_HITBALL);
						
			return TRUE;
			
		}
	}
	
	return FALSE;
}



///////////////////////////////////////////////////////////////////////////////////////////////////
//
//

void CBall::CollideWithCushions(void)
{
	// simple cushion collision
	// x:
	if(m_P.x>1736-36 || m_P.x<-1736+36) {
		m_P = m_P - m_V;	// move ball back to old position

		m_V.x = -m_V.x;
		m_V = m_V*BALL_CUSHIONFACTOR;
		//m_W = m_W*BALL_CUSHIONFACTOR*0.2f;
		m_W = m_W * BALL_CUSHIONFACTOR;
		m_W = m_W * 0.2f;
		
		SetFlag(m_flags, Flg_BALL_HITCUSHION);
	}
	// y:
	if(m_P.y>837-36 || m_P.y<-841+36) {
		m_P = m_P - m_V;	// move ball back to old position

		m_V.y = -m_V.y;
		m_V = m_V*BALL_CUSHIONFACTOR;
		//m_W = m_W*BALL_CUSHIONFACTOR*0.2f;
		m_W = m_W * BALL_CUSHIONFACTOR;
		m_W = m_W * 0.2f;

		SetFlag(m_flags, Flg_BALL_HITCUSHION);
	}

	// play cue2Cushion collision sfx
	if(g_gfxDetails.bSoundBalls)
	if(TestFlag(m_flags, Flg_BALL_HITCUSHION))
	{
		CVector SfxAmp = m_V;
		float SfxAmpMag = SfxAmp.Magnitude();
		if(SfxAmpMag > 5.0f)
			SfxAmpMag = 5.0f;
		SfxAmpMag/=5.0f;
		Msfx_smpvol(&((CGameAppView *)m_parent->m_parent)->m_game_smp[SFX_BALL2POCKET], 200*SfxAmpMag);
		Msfx_smpplay(&((CGameAppView *)m_parent->m_parent)->m_game_smp[SFX_BALL2POCKET]);		
	}
}


///////////////////////////////////////////////////////////////////////////////////////////////////
//
//

BOOL CBall::CushionsProximity(BOOL bPoolForceTrue)
{
	// simple test before doing complex "M3d_Intercept()" test!
	// if (m_parent->m_cueBallSelect == 1 || m_parent->m_cueBallSelect == 5) // don't go any further if this is an exit trajectory test ball (or Fine Tuner)
	if (m_parent->m_cueBallSelect == 1) // don't go any further if this is an exit trajectory test ball
		return FALSE;


	if(g_build.gameType == GAMETYPE_SNOOKER)
	{
		//float factor = (BALL_RADIUS / BALL_RADIUS_INITIAL);
		//float offset = 0;//BALL_RADIUS - BALL_RADIUS_INITIAL;


		// simple cushion collision
		// x:
		if(m_P.x>(1500+300-36*2) || m_P.x<(-1500-300+36*2)) {	
			{
			//ErrorMessage(m_parent->m_parent, "herex!");
			return TRUE;
			}
		}
		// y:
		if(m_P.y>(560+300-36*2) || m_P.y<(-560-300+36*2)) {
			{
			//ErrorMessage(m_parent->m_parent, "herey!");
			return TRUE;
			}
		}
	} 
	else if(g_build.gameType == GAMETYPE_POOL)
	{
		if(bPoolForceTrue)
			return TRUE; // <--- ALWAYS FORCE TO BE TRUE!!!!!!!

		// English Pool Game Type //
		if(m_parent->m_M3d_Table_ptr == &m_parent->m_customTable->m_M3d_ukTable)
			{
			// simple cushion collision
			float indent;
			indent = 1 + 29*(m_V.Magnitude2()/(MAXBALLSPEED*MAXBALLSPEED));

			// Colin: extra 30 indent, due to it missing when balls blasted at high speeds! only 5 indent is required.
			if(m_P.x>(231.11*5-BALL_RADIUS*1.55-indent) || m_P.x<(-231.11*5+BALL_RADIUS*1.55+indent)) return TRUE; // x:
			if(m_P.y>(109.67*5-BALL_RADIUS*1.55-indent) || m_P.y<(-109.67*5+BALL_RADIUS*1.55+indent)) return TRUE; // y:

			}
		else{
			// simple cushion collision
			// Colin: extra 30 indent, due to it missing when balls blasted at high speeds! only 5 indent is required.
			if(m_P.x>(253*5-BALL_RADIUS*1.55-30) || m_P.x<(-253*5+BALL_RADIUS*1.55+30))	return TRUE; // x:
			if(m_P.y>(126.5*5-BALL_RADIUS*1.55-30) || m_P.y<(-126.5*5+BALL_RADIUS*1.55+30)) return TRUE; // y:
			}

	}

	return FALSE;
}


///////////////////////////////////////////////////////////////////////////////////////////////////
//
//
BOOL CBall::Collision3d(float radius)
{
	BOOL bCollision = FALSE;
	CVector t_v;

	//#if 0
	//m_P.z-=BALL_RADIUS;

	//fprintf(fptr2,"*frame = %d : vx = %f, vy = %f, vz = %f\r\n", m_parent->m_cyc/*g_cyc*/, m_V.x, m_V.y, m_V.z);

	// 3d collision
	M3d_intercept.surf_k = 0.001f;
	//M3d_intercept.radius = BALL_RADIUS;
	M3d_intercept.radius = radius;
	M3d_intercept.type=M3D_INTERCEPT_SPHERE;
	M3d_intercept.pos[0] = m_POld.x;
	M3d_intercept.pos[1] = m_POld.y;
	M3d_intercept.pos[2] = m_POld.z;
	M3d_intercept.vec[0] = m_P.x-m_POld.x;
	M3d_intercept.vec[1] = m_P.y-m_POld.y;
	M3d_intercept.vec[2] = m_P.z-m_POld.z;
	M3d_intercept.nor[0] = 0;
	M3d_intercept.nor[1] = 0;
	M3d_intercept.nor[2] = 0;

				
	if(M3d_interceptobject(m_parent->m_M3d_Table_ptr))
	{	// collision
	
	bCollision = TRUE;
	
	m_justHitCushion = 20;

	//m_P.x = M3d_intercept.sur[0]+M3d_intercept.sur_nor[0]*0.01f;
	//m_P.y = M3d_intercept.sur[1]+M3d_intercept.sur_nor[1]*0.01f;
	//m_P.z = M3d_intercept.sur[2]+M3d_intercept.sur_nor[2]*0.01f;

	m_P.x = M3d_intercept.sur_nor[0] * 0.01f;
	m_P.y = M3d_intercept.sur_nor[1] * 0.01f;
	m_P.z = M3d_intercept.sur_nor[2] * 0.01f;

	m_P.x = m_P.x + M3d_intercept.sur[0];
	m_P.y = m_P.y + M3d_intercept.sur[1];
	m_P.z = m_P.z + M3d_intercept.sur[2];
	
	CVector Vsur_nor(M3d_intercept.sur_nor[0],M3d_intercept.sur_nor[1],M3d_intercept.sur_nor[2]);
	
	//char buffer[80]; // Normal check
	//sprintf(buffer, "(%f, %f, %f)",M3d_intercept.sur_nor[0], M3d_intercept.sur_nor[1], M3d_intercept.sur_nor[2]);
	//MessageBox(0, buffer, "!", MB_OK);
	
	float _eN, _e;
	_eN = 2.0f;
	_e = BALL_CUSHIONFACTOR; //1.0f;
	
	//if(!strcmp(m_parent->m_M3d_Table.name[M3d_intercept.mesh_n].str, "Object03")) // Cloth
	if(!strcmp(m_parent->m_M3d_Table_ptr->name[M3d_intercept.mesh_n].str, "Cloth")) // Cloth
	{
	_eN = 1.25f;
	_e = 1.0f; // it rolls along the cloth so we dont take anything off!!
	
	//char buffer[80];
	//sprintf(buffer, "(%f, %f, %f)",M3d_intercept.sur_nor[0], M3d_intercept.sur_nor[1], M3d_intercept.sur_nor[2]);
	//MessageBox(0, buffer, "!", MB_OK);
							   
	}
	else {
	char *str = m_parent->m_M3d_Table_ptr->name[M3d_intercept.mesh_n].str;
	if(str[0] == 'C' && str[1] == 'o' && str[2] == 'n' && str[3] == 'e') // Net
		_eN = 1.01f, _e = 0.8f; // Net
	else 
		{
		_eN = 2.0f, _e = BALL_CUSHIONFACTOR;//0.8f; // Cushion
		
		// BIG balls iSnooker - make cushions more bouncy
		if(g_build.gameType == GAMETYPE_SNOOKER)
			if(BALL_RADIUS > (BALL_RADIUS_INITIAL*1.1f))
				_e = BALL_CUSHIONFACTOR*1.2f;
		}
		// MessageBox(0, m_parent->m_M3d_Table.name[M3d_intercept.mesh_n].str, "Collided with", MB_OK);
	}
	
	//float fV = -(m_V.Dot(Vsur_nor));
	float fV = m_V.Dot(Vsur_nor);
	fV = -fV;
	
	CVector V = 0.0f;
	
	//Vsur_nor = VFIXFLOAT(Vsur_nor);
	//fV = FIXFLOAT(fV);
	V = Vsur_nor * fV;
	//V = VFIXFLOAT(V);

	//_eN = FIXFLOAT(_eN);
		
	V = V * _eN;
	//V = VFIXFLOAT(V);
		
	//m_V = VFIXFLOAT(m_V);
	m_V = m_V + V;
	//m_V = VFIXFLOAT(m_V);
	
	//_e = FIXFLOAT(_e);
	m_V = m_V *	_e;
	//m_V = VFIXFLOAT(m_V);
	//m_V = (m_V +  ( (-m_V.Dot(Vsur_nor)) * Vsur_nor ) * _eN) *_e;//1.5; 
	
	if(m_parent->m_dominoHit1st != m_parent->m_selectBall) // only count cushion collisions after white-ball has hit something
		m_hitCushion++;
	else
		m_bHitCushionBeforeBall = TRUE;

	SetFlag(m_flags, Flg_BALL_HITCUSHION);
	m_parent->m_bCueBallHitCushion = TRUE; // for debug purposes only!
	m_parent->m_cycHitCushion = m_parent->m_cyc;

	ApplySideSpin(Vsur_nor, 1.0);	
	
	if(g_gfxDetails.bSoundBalls)
	{
		// play cue2ball collision sfx
		CVector SfxAmp = m_V;
		float SfxAmpMag = SfxAmp.Magnitude();
		if(SfxAmpMag > 5.0f)
			SfxAmpMag = 5.0f;
		SfxAmpMag/=5.0f;
		Msfx_smpvol(&((CGameAppView *)m_parent->m_parent)->m_game_smp[SFX_BALL2POCKET], 200*SfxAmpMag);
		Msfx_smpplay(&((CGameAppView *)m_parent->m_parent)->m_game_smp[SFX_BALL2POCKET]);		
	}	

	//m_P = m_P + Vsur_nor*100;

	//if(m_V.z>0) m_V.z = 0; // prevent the ball from having +ve velocity from bounces.
	//M3d_intercept.leftover = FIXFLOAT(M3d_intercept.leftover);
	t_v = m_V * M3d_intercept.leftover; // left over velocity in this frame (0-1 value).
	//t_v = VFIXFLOAT(t_v);
	// we have to make this move for the next frame, otherwise it's posible to sneak thru

// M3d_intercept.leftover	
	

	//m_V.x-= M3d_intercept.sur_nor[0];
	}
	else { // normal travelling
	m_P.x = M3d_intercept.fpos[0];
	m_P.y = M3d_intercept.fpos[1];
	m_P.z = M3d_intercept.fpos[2];
	}

	//m_P.z+=BALL_RADIUS;
	

	
	/*
	float intercept_lut[5][4] = {
	{0,0,-1, 1.5},
	{0,1,0, 1+BALL_CUSHIONFACTOR},
	{-1,0,0, 1+BALL_CUSHIONFACTOR},
	{0,-1,0, 1+BALL_CUSHIONFACTOR},
	{1,0,0, 1+BALL_CUSHIONFACTOR},
	};
	int i;
	CVector v;

	
	for(i=0;i<5;i++)
	{
	m_P.x+=intercept_lut[i][0]*BALL_RADIUS;
	m_P.y+=intercept_lut[i][1]*BALL_RADIUS;
	m_P.z+=intercept_lut[i][2]*BALL_RADIUS;

	M3d_intercept.type=M3D_INTERCEPT_LINE;
	M3d_intercept.pos[0] = m_P.x;
	M3d_intercept.pos[1] = m_P.y;
	M3d_intercept.pos[2] = m_P.z;
	CVector contactV(intercept_lut[i][0],intercept_lut[i][1],intercept_lut[i][2]);
	CVector compV;
	float f;
	f = m_V.Dot(contactV);
	//if(f<0) continue; // ignore back velocities.
	compV = f * contactV;

	if(i==0)
	{
	M3d_intercept.vec[0] = m_V.x;
	M3d_intercept.vec[1] = m_V.y;
	M3d_intercept.vec[2] = m_V.z;
	}



	M3d_intercept.vec[0] = compV.x;
	M3d_intercept.vec[1] = compV.y;
	M3d_intercept.vec[2] = compV.z;
		
	if(M3d_interceptobject(&m_parent->m_M3d_Table))
	{	// collision
	m_P.x = M3d_intercept.sur[0]+M3d_intercept.sur_nor[0]+0.001;
	m_P.y = M3d_intercept.sur[1]+M3d_intercept.sur_nor[1]+0.001;
	m_P.z = M3d_intercept.sur[2]+M3d_intercept.sur_nor[2]+0.001;

	CVector Vsur_nor(M3d_intercept.sur_nor[0],M3d_intercept.sur_nor[1],M3d_intercept.sur_nor[2]);
	m_V = m_V +  ( (-m_V.Dot(Vsur_nor)) * Vsur_nor ) * intercept_lut[i][3]; 

	//m_V = m_V + (compV.Dot(tempsV) * intercept_lut[i][3]) * compV;

	//m_V = m_V - (compV * intercept_lut[i][3]);
	}
	else { // normal travelling
	m_P = m_P + compV;
	}

	m_P.x-=intercept_lut[i][0]*BALL_RADIUS;
	m_P.y-=intercept_lut[i][1]*BALL_RADIUS;
	m_P.z-=intercept_lut[i][2]*BALL_RADIUS;
	}
	*/
	
//#endif //0		
	/*
	if(m_P.z<0) // imaginary slate //0
	{							   //0
	m_P.z = 0;					   //0
	m_V.z*=-0.5;				   //0
	}							   //0
	*/


	m_P = m_P + t_v;

	if(bCollision)
		return TRUE;
	else return FALSE;
}


/*
void CBall::Collision3d2(void) // NOTE: this function is not used!!!
{
	CVector t_v;

	//#if 0
	//m_P.z-=BALL_RADIUS;
			
	// 3d collision
	M3d_intercept.surf_k = 0.001f;
	M3d_intercept.radius = BALL_RADIUS;
	M3d_intercept.type=M3D_INTERCEPT_SPHERE;
	M3d_intercept.pos[0] = m_POld.x;
	M3d_intercept.pos[1] = m_POld.y;
	M3d_intercept.pos[2] = m_POld.z;
	M3d_intercept.vec[0] = m_P.x-m_POld.x;
	M3d_intercept.vec[1] = m_P.y-m_POld.y;
	M3d_intercept.vec[2] = m_P.z-m_POld.z;
	M3d_intercept.nor[0] = 0;
	M3d_intercept.nor[1] = 0;
	M3d_intercept.nor[2] = 0;
				
	if(M3d_interceptobject(m_parent->m_M3d_Table_ptr))
	{	// collision
	
	m_P.x = M3d_intercept.sur[0]+M3d_intercept.sur_nor[0]*0.01f;
	m_P.y = M3d_intercept.sur[1]+M3d_intercept.sur_nor[1]*0.01f;
	m_P.z = M3d_intercept.sur[2]+M3d_intercept.sur_nor[2]*0.01f;
	
	CVector Vsur_nor(M3d_intercept.sur_nor[0],M3d_intercept.sur_nor[1],M3d_intercept.sur_nor[2]);
	
	//char buffer[80]; // Normal check
	//sprintf(buffer, "(%f, %f, %f)",M3d_intercept.sur_nor[0], M3d_intercept.sur_nor[1], M3d_intercept.sur_nor[2]);
	//MessageBox(0, buffer, "!", MB_OK);
	
	float _eN, _e;
	_eN = 2.0f;
	_e = 1.0f;
	
	//if(!strcmp(m_parent->m_M3d_Table.name[M3d_intercept.mesh_n].str, "Object03")) // Cloth
	if(!strcmp(m_parent->m_M3d_Table_ptr->name[M3d_intercept.mesh_n].str, "Cloth")) // Cloth
	{
	_eN = 1.25f;
	_e = 1.0f; // it rolls along the cloth so we dont take anything off!!
	
	//char buffer[80];
	//sprintf(buffer, "(%f, %f, %f)",M3d_intercept.sur_nor[0], M3d_intercept.sur_nor[1], M3d_intercept.sur_nor[2]);
	//MessageBox(0, buffer, "!", MB_OK);
							   
	}
	else {
	char *str = m_parent->m_M3d_Table_ptr->name[M3d_intercept.mesh_n].str;
	if(str[0] == 'C' && str[1] == 'o' && str[2] == 'n' && str[3] == 'e') // Net
		_eN = 1.01f, _e = 0.8f; // Net
	else _eN = 2.0f, _e = 0.8f; // Cushion
		// MessageBox(0, m_parent->m_M3d_Table.name[M3d_intercept.mesh_n].str, "Collided with", MB_OK);
	}
	
	
	float fV = -(m_V.Dot(Vsur_nor));
		
	CVector V = 0.0f;
	
	Vsur_nor = VFIXFLOAT(Vsur_nor);
	fV = FIXFLOAT(fV);
	V = Vsur_nor * fV;
	
	V = VFIXFLOAT(V);
	
	_eN = FIXFLOAT(_eN);
	
	V = V * _eN;
	V = VFIXFLOAT(V);
	
	m_V = VFIXFLOAT(m_V);
	
	m_V = m_V + V;
	//fprintf(fptr2,"{[5]%f,%f,%f}\r\n",m_V.x,m_V.y,m_V.z);
	// ok		 
	m_V = VFIXFLOAT(m_V);
	// illed
	//fprintf(fptr2,"{[6]%f,%f,%f}\r\n",m_V.x,m_V.y,m_V.z);
	_e = FIXFLOAT(_e);
	
	m_V = m_V *	_e;
	
	m_V = VFIXFLOAT(m_V);
	//m_V = (m_V +  ( (-m_V.Dot(Vsur_nor)) * Vsur_nor ) * _eN) *_e;//1.5; 
		
	SetFlag(m_flags, Flg_BALL_HITCUSHION);
	m_parent->m_bCueBallHitCushion = TRUE; // for debug purposes only!
	m_parent->m_cycHitCushion = m_parent->m_cyc;

	//ApplySideSpin(Vsur_nor, 1.0);

	
	if(g_gfxDetails.bSoundBalls)
	{
		// play cue2ball collision sfx
		CVector SfxAmp = m_V;
		float SfxAmpMag = SfxAmp.Magnitude();
		if(SfxAmpMag > 5.0f)
			SfxAmpMag = 5.0f;
		SfxAmpMag/=5.0f;
		Msfx_smpvol(&((CGameAppView *)m_parent->m_parent)->m_game_smp[SFX_BALL2POCKET], 200*SfxAmpMag);
		Msfx_smpplay(&((CGameAppView *)m_parent->m_parent)->m_game_smp[SFX_BALL2POCKET]);		
	}
	
	//m_P = m_P + Vsur_nor*100;

	//if(m_V.z>0) m_V.z = 0; // prevent the ball from having +ve velocity from bounces.
	M3d_intercept.leftover = FIXFLOAT(M3d_intercept.leftover);
	t_v = m_V * M3d_intercept.leftover; // left over velocity in this frame (0-1 value).
	t_v = VFIXFLOAT(t_v);
	// we have to make this move for the next frame, otherwise it's posible to sneak thru

// M3d_intercept.leftover	
	

	//m_V.x-= M3d_intercept.sur_nor[0];
	}
	else { // normal travelling
	m_P.x = M3d_intercept.fpos[0];
	m_P.y = M3d_intercept.fpos[1];
	m_P.z = M3d_intercept.fpos[2];
	}

	//m_P.z+=BALL_RADIUS;
	


	m_P = m_P + t_v;
}
*/

///////////////////////////////////////////////////////////////////////////////////////////////////
//
//
#ifdef FIXEDMODE

BOOL CBall::CollideWithPockects(BOOL bPotted, BOOL bIsBot)
{
	
	
	//return CollideWithPockectsUKPool(bPotted, bIsBot);
	// If we are using Black Hole, Centre Hold or Random Holes, then use old pocket system, otherwise use new //
	if(m_parent->m_customTable->m_tableType <= 2) return CollideWithPockectsUKPool(bPotted, bIsBot);
		else return CollideWithPockectsNormal(bPotted, bIsBot);

	/*
	if(m_parent->m_M3d_Table_ptr == &m_parent->m_customTable->m_M3d_ukTable)  return CollideWithPockectsUKPool(bPotted, bIsBot);
		else return CollideWithPockectsNormal(bPotted, bIsBot);
	*/
}


BOOL CBall::CollideWithPockectsNormal(BOOL bPotted, BOOL bIsBot)
{	
	if(m_parent->m_cueBallSelect == 1) // don't go any further if this is an exit trajectory test ball
		return FALSE;
	
	if(!TestFlag(m_flags, Flg_BALL_ACTIVE))
		return FALSE;
	// ----------------------------------------
	
	FVector Pocket;
	for(int n=0;n<m_parent->m_customTable->m_pocket_n;n++)
	{
		Pocket.x = TOFIX(m_parent->m_customTable->m_pocketPos[n].x);
		Pocket.y = TOFIX(m_parent->m_customTable->m_pocketPos[n].y);
		Pocket.z = TOFIX(m_parent->m_customTable->m_pocketPos[n].z);

		Pocket = Pocket - f_P;

		__fix sR = TOFIX(m_parent->m_customTable->m_pocketProp[n].m_suctionRadius);
		__fix sF = TOFIX(m_parent->m_customTable->m_pocketProp[n].m_suctionFactor);
		__fix r = TOFIX(m_parent->m_customTable->m_pocketProp[n].m_radius);
		
		Pocket.z = 0;
		
		__fix sR2 = FIXMUL(sR, sR);

		if(Pocket.Magnitude2() < sR2)
		{
			FVector V = Pocket.Unit();
			V = V * sF;
			
			f_V = f_V + V;

			///////////////////////////////////////////////////////////
			if(g_build.gameType == GAMETYPE_POOL)
				if(m_parent->m_customTable->m_tableType == 4) // Black-Hole
					f_V = f_V * TOFIX(0.998f);
			///////////////////////////////////////////////////////////

			__fix r2 = FIXMUL(r, r);

			if(Pocket.Magnitude2() < r2) {
				SetFlag(m_flags, Flg_BALL_HITCUSHION);

				// check if this ball was potted in 'TARGET' selected pocket
				if(m_parent->m_targetBall == m_colour) // <------ should be this ball's idx
				if(m_parent->m_targetPocket == n)				
				{
					m_parent->m_bTargetSuccess = TRUE;
				}

				/*
				// 9-ball checking
				if(m_parent->m_customTable->m_type == CTTYPE_9BALL)
				{
					// check for golden break!
					if(m_parent->m_colourseq == 0) // on break
					if(m_colour == 9) // 9-ball
						Msfx_smpplayvol(&((CGameAppView *)m_parent->m_parent)->m_game_smp[SFX_CENTURY], 255);

					// check for run-out
					int _spots = 0;
					int _9ball = 0;

					// what is left?
					for(int p=0;p<MAX_TOTALBALLS;p++)
					{
						if(TestFlag(m_parent->m_ball[p]->m_flags, Flg_BALL_ACTIVE))
						{
							if(m_parent->m_ball[p]->m_colour >= 1 && m_parent->m_ball[p]->m_colour <= 8)
							{
								_spots++;
							}
							else if(m_parent->m_ball[p]->m_colour == 9)
							{
								_9ball++;
							}
						}
					}

					if(_spots == 0 && _9ball == 0) // if there are no spots left &...
					if(m_pParent->m_break2P == 1) // if we're on a run-out

				}
				// 8-ball checking
				else if(m_parent->m_customTable->m_type == CTTYPE_8BALL)
				{
				}
				*/
			}
		}
	}	
	// ----------------------------------------
	
	if(g_build.gameType == GAMETYPE_SNOOKER)
	{
		// default: i.e. normal potting
		// play ball2pocket collision sfx
		if(TestFlag(m_flags, Flg_BALL_HITCUSHION))
		{
			ClearFlag(m_flags, Flg_BALL_ACTIVE);

			SetFlag(m_flags, Flg_BALL_POTTED);

			if(g_gfxDetails.bSoundBalls)
			if(!bIsBot)
			{
				FVector SfxAmp = f_V;
				
				float SfxAmpMag = FROMFIXSMALL(SfxAmp.Magnitude());

				if(SfxAmpMag > 5.0f)
					SfxAmpMag = 5.0f;
				SfxAmpMag/=5.0f;
				Msfx_smpvol(&((CGameAppView *)m_parent->m_parent)->m_game_smp[SFX_BALL2POCKET], 255*SfxAmpMag);
				Msfx_smpplay(&((CGameAppView *)m_parent->m_parent)->m_game_smp[SFX_BALL2POCKET]);		
				//Msfx_smpplayvol(&((CGameAppView *)m_parent->m_parent)->m_game_smp[SFX_BALL2POCKET], 255*SfxAmpMag);
			}

			// bring 'this' ball to a full stop! (*TODO - remove redundant zeroing code in other places)
			f_V = FVector(0,0,0);
			f_VOld = FVector(0,0,0);
			f_W = FVector(0,0,0);
			f_wnroll = 0;
			f_wSide = 0;	// stop balls at rest, from spinning on the spot

			// yes, we have gone in a pocket!
			return TRUE;

		}
	}
	else if(g_build.gameType == GAMETYPE_POOL)
	{
		// default: i.e. normal potting
		// play ball2pocket collision sfx
		if(TestFlag(m_flags, Flg_BALL_HITCUSHION))
		{
			ClearFlag(m_flags, Flg_BALL_ACTIVE);

			SetFlag(m_flags, Flg_BALL_POTTED);

			//if(m_colour != WHITE && m_colour != 8) // OLD WAY,!!!!!
			if(m_colour != WHITE) // A TEST
			{
				m_parent->m_customTable->Decode(); // Decode Break Values //
				// *NEW* dont inc. the pot order var. if this is the 8/9 in 8-ball/9-ball (BUGFIX for the next: balls... stat.)
				if(m_parent->m_customTable->m_type == CTTYPE_8BALL && m_colour == 8);
				else if(m_parent->m_customTable->m_type == CTTYPE_9BALL && m_colour == 9);
				else if(m_parent->m_customTable->m_type == CTTYPE_6BALL && m_colour == 6);
				else if(m_parent->m_customTable->m_type == CTTYPE_10BALL && m_colour == 10);
				else m_parent->m_break1P++; // default
				
				//m_parent->m_break1P++; // (old way)

				
				if(m_parent->m_bTurn1P)
				{
					f_P.x = TOFIX(20000 + m_parent->m_break1P*100);
					f_P.y = TOFIX(20000);
				}
				else
				{
					f_P.x = TOFIX(20000);
					f_P.y = TOFIX(20000 + m_parent->m_break1P*100);
				}			
				m_parent->m_customTable->Encode(); // Encode Break Values //
				
			}

			if(g_gfxDetails.bSoundBalls)
			if(!bIsBot)
			{
				FVector SfxAmp = f_V;
				
				float SfxAmpMag = FROMFIXSMALL(SfxAmp.Magnitude());

				if(SfxAmpMag > 5.0f)
					SfxAmpMag = 5.0f;
				SfxAmpMag/=5.0f;
				Msfx_smpvol(&((CGameAppView *)m_parent->m_parent)->m_game_smp[SFX_BALL2POCKET], 255*SfxAmpMag);
				Msfx_smpplay(&((CGameAppView *)m_parent->m_parent)->m_game_smp[SFX_BALL2POCKET]);		
				//Msfx_smpplayvol(&((CGameAppView *)m_parent->m_parent)->m_game_smp[SFX_BALL2POCKET], 255*SfxAmpMag);
			}

			// bring 'this' ball to a full stop! (*TODO - remove redundant zeroing code in other places)		
			f_V = FVector(0,0,0);
			f_VOld = FVector(0,0,0);
			f_W = FVector(0,0,0);
			f_wnroll = 0;
			f_wSide = 0;	// stop balls at rest, from spinning on the spot

			// yes, we have gone in a pocket!
			return TRUE;

		}
	}

	// no collision with pocket
	return FALSE;
}


BOOL CBall::CollideWithPockectsUKPool(BOOL bPotted, BOOL bIsBot)
{	
	
	if(m_parent->m_cueBallSelect == 1) // don't go any further if this is an exit trajectory test ball
		return FALSE;
	
	if(!TestFlag(m_flags, Flg_BALL_ACTIVE))
		return FALSE;
	// ----------------------------------------
	
	FVector Pocket;
	for(int n=0;n<m_parent->m_customTable->m_pocket_n;n++)
		{
		Pocket.x = TOFIX(m_parent->m_customTable->m_pocketPos[n].x);
		Pocket.y = TOFIX(m_parent->m_customTable->m_pocketPos[n].y);
		Pocket.z = TOFIX(m_parent->m_customTable->m_pocketPos[n].z);

		Pocket = Pocket - f_P;

		__fix sR = TOFIX(m_parent->m_customTable->m_pocketProp[n].m_suctionRadius);
		__fix sF = TOFIX(m_parent->m_customTable->m_pocketProp[n].m_suctionFactor);
		__fix r = TOFIX(m_parent->m_customTable->m_pocketProp[n].m_radius);
		
		Pocket.z = 0;
		
		__fix sR2 = FIXMUL(sR, sR);


		if(!m_InsidePocket)
		if(Pocket.Magnitude2() < sR2)
			{
			// First time we get here, we need to play the BallAroundPocket SFX //
			if(PlayedBallAroundPocketSFX == FALSE)
				{
				PlayedBallAroundPocketSFX = TRUE;
				if(g_gfxDetails.bSoundBalls)
				if(!bIsBot)
					{
						FVector SfxAmp = f_V;
						float SfxAmpMag = FROMFIXSMALL(SfxAmp.Magnitude());
						if(SfxAmpMag > 5.0f)
							SfxAmpMag = 5.0f;
						SfxAmpMag/=10.0f;
						Msfx_smpvol(&((CGameAppView *)m_parent->m_parent)->m_game_smp[SFX_BALLAROUNDPOCKET], SfxAmpMag*255);
						Msfx_smpplay(&((CGameAppView *)m_parent->m_parent)->m_game_smp[SFX_BALLAROUNDPOCKET]);		
					}
				}
			


			FVector V = Pocket.Unit();
			V = V * sF;
			
			
			// If English Pool Pocket Type. we don't want to continue sucking ball, once inside pocket
				{
				//if(!m_InsidePocket) 
				if(Pocket.Magnitude2() >= FIXMUL(r, r)) V = V * TOFIX(0.1f);
					else V = V * TOFIX(0.05f);

				}
			
				
			f_V = f_V + V;

						///////////////////////////////////////////////////////////
			if(g_build.gameType == GAMETYPE_POOL)
				if(m_parent->m_customTable->m_tableType == 4) // Black-Hole
					f_V = f_V * TOFIX(0.998f);
			///////////////////////////////////////////////////////////

			//if( f_V.Magnitude2()<2)
			//	{
						
				__fix r2 = FIXMUL(r, r);

				if(Pocket.Magnitude2() < r2) {
					SetFlag(m_flags, Flg_BALL_HITCUSHION);

					if(!m_InsidePocket) m_InsidePocket = 1;
						

					// check if this ball was potted in 'TARGET' selected pocket
					if(m_parent->m_targetBall == m_colour) // <------ should be this ball's idx
					if(m_parent->m_targetPocket == n)				
						{
						m_parent->m_bTargetSuccess = TRUE;
						}

					}
			//	}
				
			}	
		}
	// ----------------------------------------
	
	

	if(g_build.gameType == GAMETYPE_SNOOKER)
	{
		// default: i.e. normal potting
		// play ball2pocket collision sfx
	if(m_InsidePocket)
		{
		PlayedBallAroundPocketSFX = FALSE;
		//if(TestFlag(m_flags, Flg_BALL_HITCUSHION))
		if(m_FadeOut==0)
			{
			ClearFlag(m_flags, Flg_BALL_ACTIVE);

			SetFlag(m_flags, Flg_BALL_POTTED);

			m_FadeOut = 200;
			m_InsidePocket = 0;
			f_P.z = BALL_POSITIONZOFFSET;
			f_V.z = BALL_POSITIONZOFFSET;

			if(g_gfxDetails.bSoundBalls)
			if(!bIsBot)
			{
				FVector SfxAmp = f_V;
				
				float SfxAmpMag = FROMFIXSMALL(SfxAmp.Magnitude());

				if(SfxAmpMag > 5.0f)
					SfxAmpMag = 5.0f;
				SfxAmpMag/=5.0f;
				Msfx_smpvol(&((CGameAppView *)m_parent->m_parent)->m_game_smp[SFX_BALL2POCKET], 100);
				Msfx_smpplay(&((CGameAppView *)m_parent->m_parent)->m_game_smp[SFX_BALL2POCKET]);		
			}

			// bring 'this' ball to a full stop! (*TODO - remove redundant zeroing code in other places)
			f_V = FVector(0,0,0);
			f_VOld = FVector(0,0,0);
			f_W = FVector(0,0,0);
			f_wnroll = 0;
			f_wSide = 0;	// stop balls at rest, from spinning on the spot

			// yes, we have gone in a pocket!
			return TRUE;
			}
		}
	}



	else if(g_build.gameType == GAMETYPE_POOL)
		{
		
		// default: i.e. normal potting
		// play ball2pocket collision sfx
		//if(TestFlag(m_flags, Flg_BALL_HITCUSHION))
		if(m_InsidePocket)
			{
			PlayedBallAroundPocketSFX = FALSE;
			//if(TestFlag(m_flags, Flg_BALL_HITCUSHION))
			if(m_FadeOut==0)
				{
				ClearFlag(m_flags, Flg_BALL_ACTIVE);
				SetFlag(m_flags, Flg_BALL_POTTED);
				//if(m_colour != WHITE && m_colour != 8) // OLD WAY,!!!!!
				if(m_colour != WHITE) // A TEST
					{
					m_parent->m_customTable->Decode(); // Decode Break Values //

					// *NEW* dont inc. the pot order var. if this is the 8/9 in 8-ball/9-ball (BUGFIX for the next: balls... stat.)
					if(m_parent->m_customTable->m_type == CTTYPE_8BALL && m_colour == 8);
					else if(m_parent->m_customTable->m_type == CTTYPE_UKPOOL && m_colour == 8);
					else if(m_parent->m_customTable->m_type == CTTYPE_9BALL && m_colour == 9);
					else if(m_parent->m_customTable->m_type == CTTYPE_6BALL && m_colour == 6);
					else if(m_parent->m_customTable->m_type == CTTYPE_10BALL && m_colour == 10);
					else m_parent->m_break1P++; // default
					
					//m_parent->m_break1P++; // (old way)
					
					
					if(m_parent->m_bTurn1P)
						{
						f_P.x = TOFIX(20000 + m_parent->m_break1P*100);
						f_P.y = TOFIX(20000);
						}
					else
						{
						f_P.x = TOFIX(20000);
						f_P.y = TOFIX(20000 + m_parent->m_break1P*100);
						}	
					m_parent->m_customTable->Encode(); // Encode Break Values //
														
					}
				m_FadeOut = 200;
				m_InsidePocket = 0;
				f_P.z = TOFIX(BALL_POSITIONZOFFSET);
				f_V.z = 0;
							



				if(g_gfxDetails.bSoundBalls)
				if(!bIsBot)
					{
					FVector SfxAmp = f_V;
					
					float SfxAmpMag = FROMFIXSMALL(SfxAmp.Magnitude());

					// SFX for pocket must be within this boundary
					if(SfxAmpMag > 5.0f) SfxAmpMag = 5.0f;
					if(SfxAmpMag < 2.0f) SfxAmpMag = 2.0f;	

					SfxAmpMag/=5.0f;
					Msfx_smpvol(&((CGameAppView *)m_parent->m_parent)->m_game_smp[SFX_BALL2POCKET], 50);
					//Msfx_smpvol(&((CGameAppView *)m_parent->m_parent)->m_game_smp[SFX_BALL2POCKET], 100);
					Msfx_smpplay(&((CGameAppView *)m_parent->m_parent)->m_game_smp[SFX_BALL2POCKET]);		
					//Msfx_smpplayvol(&((CGameAppView *)m_parent->m_parent)->m_game_smp[SFX_BALL2POCKET], 255*SfxAmpMag);
					}

				// bring 'this' ball to a full stop! (*TODO - remove redundant zeroing code in other places)		
				f_V = FVector(0,0,0);
				f_VOld = FVector(0,0,0);
				f_W = FVector(0,0,0);
				f_wnroll = 0;
				f_wSide = 0;	// stop balls at rest, from spinning on the spot

				// yes, we have gone in a pocket!
				return TRUE;
				}
			}
		}
	
	// no collision with pocket
	return FALSE;
}
#else

BOOL CBall::CollideWithPockects(BOOL bPotted, BOOL bIsBot)
{
	if(m_parent->m_cueBallSelect == 1) // don't go any further if this is an exit trajectory test ball
		return FALSE;
	
	if(!TestFlag(m_flags, Flg_BALL_ACTIVE))
		return FALSE;
	// ----------------------------------------
	
	CVector Pocket;
	for(int n=0;n<m_parent->m_customTable->m_pocket_n;n++)
	{
		Pocket = m_parent->m_customTable->m_pocketPos[n];
		Pocket = Pocket - m_P;
		float sR = m_parent->m_customTable->m_pocketProp[n].m_suctionRadius;
		float sF = m_parent->m_customTable->m_pocketProp[n].m_suctionFactor;
		float r = m_parent->m_customTable->m_pocketProp[n].m_radius;
		
		Pocket.z = 0;
		
		if(Pocket.Magnitude2() < SQR((sR)))
		{
			CVector V = Pocket.Unit();
			V = V * sF;
			
			m_V = m_V + V;

			///////////////////////////////////////////////////////////
			if(g_build.gameType == GAMETYPE_POOL)
				if(m_parent->m_customTable->m_tableType == 4) // Black-Hole
					m_V = m_V * 0.998f;
			///////////////////////////////////////////////////////////

			if(Pocket.Magnitude2() < SQR(r))
			SetFlag(m_flags, Flg_BALL_HITCUSHION);
		}
	}	
	// ----------------------------------------
	
	if(g_build.gameType == GAMETYPE_SNOOKER)
	{
		// default: i.e. normal potting
		// play ball2pocket collision sfx
		if(TestFlag(m_flags, Flg_BALL_HITCUSHION))
		{
			ClearFlag(m_flags, Flg_BALL_ACTIVE);

			SetFlag(m_flags, Flg_BALL_POTTED);

			if(g_gfxDetails.bSoundBalls)
			if(!bIsBot)
			{
				CVector SfxAmp = m_V;
				float SfxAmpMag = SfxAmp.Magnitude();
				if(SfxAmpMag > 5.0f)
					SfxAmpMag = 5.0f;
				//SfxAmpMag/=5.0f;
				SfxAmpMag = sqrt(SfxAmpMag);
				SfxAmpMag = sqrt(SfxAmpMag);
				Msfx_smpvol(&((CGameAppView *)m_parent->m_parent)->m_game_smp[SFX_BALL2POCKET], 30 + 90*SfxAmpMag);
				Msfx_smpplay(&((CGameAppView *)m_parent->m_parent)->m_game_smp[SFX_BALL2POCKET]);		
				
			}

			// bring 'this' ball to a full stop! (*TODO - remove redundant zeroing code in other places)
			m_V = CVector(0,0,0);
			m_VOld = CVector(0,0,0);
			m_W = CVector(0,0,0);
			m_wnroll = 0;
			m_wSide = 0;	// stop balls at rest, from spinning on the spot

			// yes, we have gone in a pocket!
			return TRUE;

		}
	}
	else if(g_build.gameType == GAMETYPE_POOL)
	{
		// default: i.e. normal potting
		// play ball2pocket collision sfx
		if(TestFlag(m_flags, Flg_BALL_HITCUSHION))
		{
			ClearFlag(m_flags, Flg_BALL_ACTIVE);		

			SetFlag(m_flags, Flg_BALL_POTTED);

			//if(m_colour != WHITE && m_colour != 8) // OLD WAY,!!!!!
			if(m_colour != WHITE) // A TEST
			{
				m_parent->m_customTable->Decode(); // Decode Break Values //
				// *NEW* dont inc. the pot order var. if this is the 8/9 in 8-ball/9-ball (BUGFIX for the next: balls... stat.)
				if(m_parent->m_customTable->m_type == CTTYPE_8BALL && m_colour == 8);
				else if(m_parent->m_customTable->m_type == CTTYPE_UKPOOL && m_colour == 8);
				else if(m_parent->m_customTable->m_type == CTTYPE_9BALL && m_colour == 9);
				else if(m_parent->m_customTable->m_type == CTTYPE_6BALL && m_colour == 6);
				else if(m_parent->m_customTable->m_type == CTTYPE_10BALL && m_colour == 10);
				else m_parent->m_break1P++; // default

				//m_parent->m_break1P++; // OLD WAY!

				
				if(m_parent->m_bTurn1P)
				{
					m_P.x = 20000 + m_parent->m_break1P*100;
					m_P.y = 20000;
				}
				else
				{
					m_P.x = 20000;
					m_P.y = 20000 + m_parent->m_break1P*100;
				}			
				m_parent->m_customTable->Encode(); // Encode Break Values //
			}

			if(g_gfxDetails.bSoundBalls)
			if(!bIsBot)
			{
				CVector SfxAmp = m_V;
				float SfxAmpMag = SfxAmp.Magnitude();
				if(SfxAmpMag > 5.0f)
					SfxAmpMag = 5.0f;
				SfxAmpMag/=5.0f;
				Msfx_smpvol(&((CGameAppView *)m_parent->m_parent)->m_game_smp[SFX_BALL2POCKET], 255*SfxAmpMag);
				Msfx_smpplay(&((CGameAppView *)m_parent->m_parent)->m_game_smp[SFX_BALL2POCKET]);		
				//Msfx_smpplayvol(&((CGameAppView *)m_parent->m_parent)->m_game_smp[SFX_BALL2POCKET], 255*SfxAmpMag);
			}

			// bring 'this' ball to a full stop! (*TODO - remove redundant zeroing code in other places)		
			m_V = CVector(0,0,0);
			m_VOld = CVector(0,0,0);
			m_W = CVector(0,0,0);
			m_wnroll = 0;
			m_wSide = 0;	// stop balls at rest, from spinning on the spot

			// yes, we have gone in a pocket!
			return TRUE;

		}
	}

	// no collision with pocket
	return FALSE;
}


#endif

///////////////////////////////////////////////////////////////////////////////////////////////////
//
//
BOOL CBall::TryReSpot1(CBall *ball)
{
	if(!TestFlag(m_flags, Flg_BALL_ACTIVE))
	{
		CVector P;

		// position ball on its original starting position
		m_POld = m_P = m_PSpot;
		m_W = m_V = m_VOld = CVector(0,0,0);

		// is spot occupied?
		P = m_P - ball->m_P;
		if((P.Magnitude2()) < SQR(BALL_RADIUS*2))
			return FALSE;
	}

	return TRUE;
}


///////////////////////////////////////////////////////////////////////////////////////////////////
//
//
BOOL CBall::TryReSpot2(CBall *ball, CBall *nextspotball)
{
	if(!TestFlag(m_flags, Flg_BALL_ACTIVE))
	{
		CVector P;

		// position ball on its original starting position
		m_POld = m_P = nextspotball->m_PSpot;
		m_W = m_V = m_VOld = CVector(0,0,0);

		// is spot occupied?
		P = m_P - ball->m_P;
		if((P.Magnitude2()) < SQR(BALL_RADIUS*2))
			return FALSE;
	}

	return TRUE;
}


///////////////////////////////////////////////////////////////////////////////////////////////////
//
//
BOOL CBall::TryReSpot3(CBall *ball)
{
	if(!TestFlag(m_flags, Flg_BALL_ACTIVE))
	{
		//int n;
		CVector P;

		// position ball on its original starting position
		//m_POld = m_P = m_PSpot;
		//m_W = m_V = CVector(0,0,0);
				
		// check to left side of spot (n = -1)..., then the right side (n = 1)...
		//for(n = -1; n >= 1; n+=2)
		//	do
		//	{
		//		//m_P.x += (BALL_RADIUS/20)*n;
		//		m_P.x -= BALL_RADIUS*2;
		  //
				// is pos occupied?
				P = m_P - ball->m_P;
				if((P.Magnitude2()) < SQR(BALL_RADIUS*2))
					return FALSE;
			//}
			//while((m_P.x >= (-1736+BALL_RADIUS)) || (m_P.x <= (1736-BALL_RADIUS)));   // bottom cushion + BALL_RADIUS || top cushion - BALL_RADIUS
	}

	return TRUE;
}


///////////////////////////////////////////////////////////////////////////////////////////////////
//
//
void CBall::ReSpotSuccessful(void)
{
	// turn ball on
	m_flags = 0;
	SetFlag(m_flags, Flg_BALL_ACTIVE);
}


///////////////////////////////////////////////////////////////////////////////////////////////////
//
//
#ifndef THE_POOL_CLUB
float CBall::CushionError(CVector StrikeDir, float eRange)
{
	float dx;
	float dy;
	float error = 0;
	float dxError = 0;
	float dyError = 0;
	// float eRange = BALL_RADIUS*6;

	StrikeDir.z = 0;
	StrikeDir = StrikeDir.Unit();
	//float angle2D = 90*DEG2RAD + atan2((double)StrikeDir.y, (double)StrikeDir.x);

	// check in X:
	if(m_P.x < 0)
		dx = m_P.x - (-1500-300+36*2);
	else
		dx = (1500+300-36*2) - m_P.x;

	if(dx < eRange)
	{
		dxError = dx / eRange;
		dxError = 1.0f - dxError;
		
		// are we cueing parallel to cush.
		if(StrikeDir.y < 0)
			StrikeDir.y = -StrikeDir.y;
		dxError *= (1 - StrikeDir.y*0.8);
	}
	

	// check in Y:
	if(m_P.y < 0)
		dy = m_P.y - (-560-300+36*2);
	else
		dy = (560+300-36*2) - m_P.y;

	if(dy < eRange)
	{
		dyError = dy / eRange;
		dyError = 1.0f - dyError;

		// are we cueing parallel to cush.
		if(StrikeDir.x < 0)
			StrikeDir.x = -StrikeDir.x;
		dyError *= (1 - StrikeDir.x*0.8);
	}
	
	/*
	// take biggest error
	if(dxError > dyError)
		error = dxError;
	else
		error = dyError;
	*/

	return (dxError + dyError);

	// return an error value (low)0.0f - 1.0f(high)
	// return error;
}

#else

float CBall::CushionError(CVector StrikeDir, float eRange)
{

	float dx;
	float dy;
	float error = 0;
	float dxError = 0;
	float dyError = 0;

	StrikeDir.z = 0;
	StrikeDir = StrikeDir.Unit();
	//float angle2D = 90*DEG2RAD + atan2((double)StrikeDir.y, (double)StrikeDir.x);

	// check in X:
	if(m_P.x < 0)
		dx = m_P.x - (-253*5+BALL_RADIUS*1.25);
	else
		dx = (253*5-BALL_RADIUS*1.25) - m_P.x;

	if(dx < eRange)
	{
		dxError = dx / eRange;
		dxError = 1.0f - dxError;
		
		// are we cueing parallel to cush.
		if(StrikeDir.y < 0)
			StrikeDir.y = -StrikeDir.y;
		dxError *= (1 - StrikeDir.y*0.8);
	}
	

	// check in Y:
	if(m_P.y < 0)
		dy = m_P.y - (-126.5*5+BALL_RADIUS*1.25);
	else
		dy = (126.5*5-BALL_RADIUS*1.25) - m_P.y;

	if(dy < eRange)
	{
		dyError = dy / eRange;
		dyError = 1.0f - dyError;

		// are we cueing parallel to cush.
		if(StrikeDir.x < 0)
			StrikeDir.x = -StrikeDir.x;
		dyError *= (1 - StrikeDir.x*0.8);
	}
	

	// take biggest error
	if(dxError > dyError)
		error = dxError;
	else
		error = dyError;

	//{
	//char buf[80];
	//sprintf(buf, "error = %f", error);
	//ErrorMessage(m_parent->m_parent, buf);
	//}

	// return an error value (low)0.0f - 1.0f(high)
	return error;

	
}
#endif //!THE_POOL_CLUB


///////////////////////////////////////////////////////////////////////////////////////////////////
//
//
float CBall::HamperedByBallError(CBall *strikeBall, CVector StrikeDir)
{
	float error = 0;
	float d;
	//float eRange = BALL_RADIUS*6;
	float eRange = BALL_RADIUS*12;

	StrikeDir.z = 0;
	StrikeDir = StrikeDir.Unit();
	//float angle2D = 90*DEG2RAD + atan2((double)StrikeDir.y, (double)StrikeDir.x);

	CVector P = strikeBall->m_P;
	int maxIttr = 6*10;
	BOOL hit = FALSE;
	
	// loop up to a max. of 3 balls in distance, directly behind the white...
	do
	{
		P = P - (StrikeDir * (BALL_RADIUS/10.0f));
		//hit = Proximity(P.x, P.y, 0, BALL_RADIUS*2.0f);
		hit = Proximity(P.x, P.y, 0, (BALL_RADIUS*2.0f)*0.7);
		maxIttr--;
	}
	while(!hit && maxIttr);

	if(hit)
	{
		// get distance between White-ball and Hampering-ball
		CVector D = P - strikeBall->m_P;
		d = D.Magnitude();

		if(d < eRange)
		{
			error = d / eRange;
			error = 1.0f - error;
			error = error * error;
		}

		//strikeBall->m_P = P;
	}

	// return an error value (low)0.0f - 1.0f(high)
	return (error*3);
}


///////////////////////////////////////////////////////////////////////////////////////////////////
//
//
void CBall::Draw(void)
{
}

