// CBot

#include <math.h>

#include "../CodeSupp/build.h"
#include "../CodeSupp/stdafx.h"
#include "../CodeSupp/GameAppView.h"

#include "Bot.h"


CBot::CBot(CWnd *pParent) // constructor
{
	m_parent = pParent;
	m_name = _T("Bot");
	m_rating = 1200;
	SetFlag(m_type, Flg_TYPE_NORMAL);
	m_bStatus = 0;
	m_count = 0;
}

CBot::~CBot() // destructor
{
	m_parent = 0;
}

void CBot::Init(CString name, int rating, Flags type)
{
	m_name = name;
	m_rating = rating;
	m_type = type;
	//m_bStatus = 1;	
	m_bStatus^=1;
	m_count = 0;
	m_nAngle = 0;
	m_powerPercentage = 0.0f;
	m_cueTip[0] = 0.0f;
	m_cueTip[1] = 0.0f;
	m_cueTip[2] = 0.0f;
	for(int n=0;n<180*2;n++)
		m_bDirectPots[n] = FALSE;
	m_DAngle = CVector(1.0f, 0.0f, 0.0f);

	// TODO:  will need extra code to support some WILD type settings
	#ifdef THE_POOL_CLUB
	m_PAimPocket[0] = CVector((+253*5+BALL_RADIUS*1.25)-BALL_RADIUS*2, (-126.5*5-BALL_RADIUS*1)+BALL_RADIUS*2, (0)); // BLACK-G
	m_PAimPocket[1] = CVector((+253*5+BALL_RADIUS*1.25)-BALL_RADIUS*2, (+126.5*5+BALL_RADIUS*1)-BALL_RADIUS*2, (0)); // BLACK-Y
	m_PAimPocket[2] = CVector((0),						 (-126.5*5-BALL_RADIUS*3)+BALL_RADIUS*2, (0)); // BLUE-G
	m_PAimPocket[3] = CVector((0),						 (+126.5*5+BALL_RADIUS*3)-BALL_RADIUS*2, (0)); // BLUE-Y
	m_PAimPocket[4] = CVector((-253*5-BALL_RADIUS*1.25)+BALL_RADIUS*2, (-126.5*5-BALL_RADIUS*1)+BALL_RADIUS*2, (0)); // Green
	m_PAimPocket[5] = CVector((-253*5-BALL_RADIUS*1.25)+BALL_RADIUS*2, (+126.5*5+BALL_RADIUS*1)-BALL_RADIUS*2, (0)); // Yellow		
	#else // iSnooker
	m_PAimPocket[0] = CVector((1500+300+26), (-560-300-26), (0));// BLACK-G
	m_PAimPocket[1] = CVector((1500+300+26), (+560+300+26), (0));// BLACK-Y
	m_PAimPocket[2] = CVector((0), (-560-300-36-40), (0));// BLUE-G
	m_PAimPocket[3] = CVector((0), (+560+300+36+40), (0));// BLUE-Y
	m_PAimPocket[4] = CVector((-1500-300-26), (-560-300-26));// Green
	m_PAimPocket[5] = CVector((-1500-300-26), (+560+300+26));// Yellow	
	#endif // THE_POOL_CLUB
	
	for(int n=0;n<6;n++)
	m_PocketDirectPotChance[n] = 0; // <=0 no chance, >0 good chance
	m_PocketDirectPotBest = 0; // idx of best pocket to direct-pot into
}

void CBot::Deinit(void)
{
	m_bStatus = 0;
}

void CBot::LongSearch(void)
{
	float powerPercentageInitial = 40.0f;
	float powerPercentage;
	//BOOL bPositiveShot;
	BOOL bBotTurn;
	int colourHit1st;
	int targetColour;
	M3d_vtx cueTip;
	CVector D;
	int n;
	
	GAMEAPPVIEW->m_snooker->m_bPracticeMode = FALSE;
	GAMEAPPVIEW->m_snooker->m_customTable->m_type = CTTYPE_9BALL;

	m_DAngle = CVector(1.0f, 0.0f, 0.0f);

	for(n=0;n<360*2;n++) // no of shots (number of tries)
	{
		//bPositiveShot = FALSE;

		// set power for shot
		powerPercentage = powerPercentageInitial;
		#ifdef THE_POOL_CLUB
		powerPercentage *= (1400.0f / 100.0f);
		#else
		powerPercentage *= (1600.0f / 100.0f);
		#endif //THE_POOL_CLUB

		// set angle for shot
		D = m_DAngle;
		D = D.RotateXY((n/2)*DEG2RAD);
		D = D * powerPercentage;
								
		// hit center-ball
		cueTip[0] = 0.0f;
		cueTip[1] = 0.0f;//-0.6f;
		cueTip[2] = 0.0f;

		// Strike white-ball and simulate this ball only
		GAMEAPPVIEW->m_snooker->TestBallStrike(D, cueTip, TRUE);
		
		// who's turn? before we call referee (was it a good shot?)
		bBotTurn = GAMEAPPVIEW->m_snooker->m_bTurn1P;
					
		// did we hit the ball on? (i.e. 1-ball at start of 9-ball game)
		colourHit1st = GAMEAPPVIEW->m_snooker->m_ball[GAMEAPPVIEW->m_snooker->m_dominoHit1st]->m_colour;
		targetColour = GAMEAPPVIEW->m_snooker->m_colourseq;
		if(targetColour == 0) targetColour = 1;
		if(colourHit1st == targetColour)
		{							
			// go back to previus position
			GAMEAPPVIEW->m_snooker->RestorePreviousPosition();
			for(int m=0;m<MAX_TOTALBALLS;m++)
			if(TestFlag(GAMEAPPVIEW->m_snooker->m_ball[m]->m_flags, Flg_BALL_ACTIVE|Flg_BALL_POTTED))
			{
				GAMEAPPVIEW->m_snooker->m_ball[m]->UpdateFloats();
				ClearFlag(GAMEAPPVIEW->m_snooker->m_ball[m]->m_flags, Flg_BALL_POTTED);
			}
			
			// strike at same angle & simulate all balls
			GAMEAPPVIEW->m_snooker->TestBallStrike(D, cueTip, FALSE);				

			// call refree to find to see it it's still my turn (=> good shot)
			GAMEAPPVIEW->m_snooker->m_customTable->Referee(CTMSG_SIMEND);				
		}
		else
		{
			// we didn't hit the ball-on, so swap turn (failed to hit the correct ball/in-off)
			GAMEAPPVIEW->m_snooker->m_bTurn1P^=1;
		}
		
		// if it's still my turn, then this is a good shot!
		if(bBotTurn == GAMEAPPVIEW->m_snooker->m_bTurn1P)
		{
			//bPositiveShot = TRUE;
			
			// go back to previus position
			GAMEAPPVIEW->m_snooker->RestorePreviousPosition();
			for(int m=0;m<MAX_TOTALBALLS;m++)
			if(TestFlag(GAMEAPPVIEW->m_snooker->m_ball[m]->m_flags, Flg_BALL_ACTIVE|Flg_BALL_POTTED))
			{
				GAMEAPPVIEW->m_snooker->m_ball[m]->UpdateFloats();
				ClearFlag(GAMEAPPVIEW->m_snooker->m_ball[m]->m_flags, Flg_BALL_POTTED);
			}

			break;
		}
					
		// reset turn
		GAMEAPPVIEW->m_snooker->m_bTurn1P = bBotTurn;//1
					
		// go back to previus position
		GAMEAPPVIEW->m_snooker->RestorePreviousPosition();
		for(int m=0;m<MAX_TOTALBALLS;m++)
		if(TestFlag(GAMEAPPVIEW->m_snooker->m_ball[m]->m_flags, Flg_BALL_ACTIVE|Flg_BALL_POTTED))
		{
			GAMEAPPVIEW->m_snooker->m_ball[m]->UpdateFloats();
			ClearFlag(GAMEAPPVIEW->m_snooker->m_ball[m]->m_flags, Flg_BALL_POTTED);
		}
	} // end for n

	// check if we didn't find a hit
	if(n==360*2) n = -1;

	// update bot's strike data
	m_nAngle = n;
	m_powerPercentage = powerPercentage;
	m_cueTip[0] = cueTip[0];
	m_cueTip[1] = cueTip[1];
	m_cueTip[2] = cueTip[2];
}

void CBot::Strike(void)
{
	//if(m_nAngle<0 || m_nAngle >= 360*2) return;

	/*
	// set power & angle for shot
	CVector D = m_DAngle;
	D = D.RotateXY((m_nAngle/2)*DEG2RAD);
	D = D * m_powerPercentage;
	*/
			
	// setup strike ball send data
	/*
	GAMEAPPVIEW->m_snooker->m_sPoint[0] = D.x;
	GAMEAPPVIEW->m_snooker->m_sPoint[1] = D.y;
	GAMEAPPVIEW->m_snooker->m_sPoint[2] = D.z;
	GAMEAPPVIEW->m_snooker->m_tableMousePos[0] = 0.0f;
	GAMEAPPVIEW->m_snooker->m_tableMousePos[1] = 0.0f;
	GAMEAPPVIEW->m_snooker->m_tableMousePos[2] = 0.0f;	
	*/
	GAMEAPPVIEW->m_snooker->m_cuetip[0] = m_cueTip[0];
	GAMEAPPVIEW->m_snooker->m_cuetip[1] = m_cueTip[1];
	GAMEAPPVIEW->m_snooker->m_cuetip[2] = m_cueTip[2];
	GAMEAPPVIEW->m_snooker->m_selectBall = 0; // 'WHITE' ball
	//GAMEAPPVIEW->m_snooker->m_strikeRnd = 0;
	//GAMEAPPVIEW->m_snooker->m_strikeMaxRnd = 0;
	GAMEAPPVIEW->m_snooker->m_strikeRnd = GAMEAPPVIEW->m_snooker->GetStrikeBallError(FALSE);
	GAMEAPPVIEW->m_snooker->m_strikeMaxRnd = GAMEAPPVIEW->m_snooker->GetStrikeBallError(TRUE);
	
	GAMEAPPVIEW->m_snooker->m_bPracticeMode = TRUE;
	GAMEAPPVIEW->m_snooker->m_bSeenSelectBall = TRUE;
	GAMEAPPVIEW->m_snooker->StrikeBallSend();
}
// 2503029136
// HMgHSiH
BOOL CBot::DirectHitSearch(void)
{	
	float powerPercentageInitial = 15.0f;
	float powerPercentage;
	//BOOL bPositiveShot;
	BOOL bBotTurn;
	int colourHit1st;
	int targetColour;
	M3d_vtx cueTip;
	CVector D;
	int n, m;
		
	GAMEAPPVIEW->m_snooker->m_bPracticeMode = FALSE;
	GAMEAPPVIEW->m_snooker->m_customTable->m_type = CTTYPE_9BALL;
	
	// can we hit the object ball direct? (3-points test)
	BOOL bPointTest[3];
	bPointTest[0] = FALSE;
	bPointTest[1] = FALSE;
	bPointTest[2] = FALSE;

	targetColour = GAMEAPPVIEW->m_snooker->m_colourseq;
	if(targetColour == 0) targetColour = 1;
	//n = targetColour;
			
	/////////////////////////////////////////////
	// set power for shot
	powerPercentage = powerPercentageInitial;
	#ifdef THE_POOL_CLUB
	powerPercentage *= (1400.0f / 100.0f);
	#else
	powerPercentage *= (1600.0f / 100.0f);
	#endif //THE_POOL_CLUB
	
	// hit center-ball
	cueTip[0] = 0.0f;
	cueTip[1] = 0.0f;//-0.6f;
	cueTip[2] = 0.0f;
	/////////////////////////////////////////////
	
	CVector Dir[3];

	for(m=0;m<3;m++)
	{		
		/////////////////////////////////////////////
		// set power for shot
		powerPercentage = powerPercentageInitial;
		#ifdef THE_POOL_CLUB
		powerPercentage *= (1400.0f / 100.0f);
		#else
		powerPercentage *= (1600.0f / 100.0f);
		#endif //THE_POOL_CLUB
		
		// hit center-ball
		cueTip[0] = 0.0f;
		cueTip[1] = 0.0f;//-0.6f;
		cueTip[2] = 0.0f;
		/////////////////////////////////////////////

		switch(m)
		{
			case 0:
			{
			// (1) center-ball contact
			Dir[m] = GAMEAPPVIEW->m_snooker->m_ball[targetColour]->m_P;
			Dir[m] = Dir[m] - GAMEAPPVIEW->m_snooker->m_ball[0]->m_P;
			Dir[m] = Dir[m].Unit();
			}
			break;
			
			case 1:
			{
			// (2) - edge-of-ball contact
			Dir[m] = GAMEAPPVIEW->m_snooker->m_ball[targetColour]->m_P;
			Dir[m] = Dir[m] - GAMEAPPVIEW->m_snooker->m_ball[0]->m_P;
			
			CVector N = Dir[m].Cross(CVector(0, 0, 1));
			N = N.Unit();
			float diameter = 2 * BALL_RADIUS;
			N = N * diameter*0.95f;
			Dir[m] = Dir[m] + N;
			Dir[m] = Dir[m].Unit();
			}
			break;
			
			case 2:
			{
			// (2) - edge-of-ball contact
			Dir[m] = GAMEAPPVIEW->m_snooker->m_ball[targetColour]->m_P;
			Dir[m] = Dir[m] - GAMEAPPVIEW->m_snooker->m_ball[0]->m_P;
			
			CVector N = Dir[m].Cross(CVector(0, 0, 1));
			N = N.Unit();
			float diameter = 2 * BALL_RADIUS;
			N = N * diameter*0.95f;
			Dir[m] = Dir[m] - N;
			Dir[m] = Dir[m].Unit();
			}
			break;	
		} // end switch
				

		// set angle for shot
		Dir[m] = Dir[m] * powerPercentage;		
		
		// Strike white-ball and simulate this ball only
		GAMEAPPVIEW->m_snooker->TestBallStrike(Dir[m], cueTip, FALSE);//TRUE); // [+] should be able to use TRUE???
		GAMEAPPVIEW->m_snooker->SimEndUpdateFloats();

		
		// did we hit the ball on? (i.e. 1-ball at start of 9-ball game)
		colourHit1st = GAMEAPPVIEW->m_snooker->m_ball[GAMEAPPVIEW->m_snooker->m_dominoHit1st]->m_colour;
		if(colourHit1st == targetColour)
		{
			bPointTest[m] = TRUE;
		}

		// go back to previus position
		GAMEAPPVIEW->m_snooker->RestorePreviousPosition();
		for(int n=0;n<MAX_TOTALBALLS;n++)
		if(TestFlag(GAMEAPPVIEW->m_snooker->m_ball[n]->m_flags, Flg_BALL_ACTIVE|Flg_BALL_POTTED))
		{
			GAMEAPPVIEW->m_snooker->m_ball[n]->UpdateFloats();
			ClearFlag(GAMEAPPVIEW->m_snooker->m_ball[n]->m_flags, Flg_BALL_POTTED);
		}		
	} // end for m
		
	// if we can't the center or two edges, then try other longer searches
	if(!bPointTest[0] && !bPointTest[1] && !bPointTest[2])
		return FALSE;
	
	// clear out all direct pots array 
	for(n=0;n<180*2;n++)
		m_bDirectPots[n] = FALSE;

	m_DAngle = Dir[2].Unit(); // right-edge relative to Dir[0] (center)
	
	int run;
	run = 0;
	float dot, oldDot;
	oldDot = dot = -1.0f;
		
	for(n=0;n<180*2;n++) // no of shots (number of tries)
	{
		//bPositiveShot = FALSE;

		// set power for shot
		powerPercentage = powerPercentageInitial;
		#ifdef THE_POOL_CLUB
		powerPercentage *= (1400.0f / 100.0f);
		#else
		powerPercentage *= (1600.0f / 100.0f);
		#endif //THE_POOL_CLUB

		// set angle for shot
		D = m_DAngle;
		D = D.RotateXY((n/2)*DEG2RAD);
		D = D * powerPercentage;
								
		// hit center-ball
		cueTip[0] = 0.0f;
		cueTip[1] = 0.0f;//-0.6f;
		cueTip[2] = 0.0f;

		// Strike white-ball and simulate this ball only
		GAMEAPPVIEW->m_snooker->TestBallStrike(D, cueTip, FALSE);//TRUE);
		
		// who's turn? before we call referee (was it a good shot?)
		bBotTurn = GAMEAPPVIEW->m_snooker->m_bTurn1P;
					
		// did we hit the ball on? (i.e. 1-ball at start of 9-ball game)
		//colourHit1st = GAMEAPPVIEW->m_snooker->m_ball[GAMEAPPVIEW->m_snooker->m_dominoHit1st]->m_colour;
		//targetColour = GAMEAPPVIEW->m_snooker->m_colourseq;
		//if(targetColour == 0) targetColour = 1;
		//if(colourHit1st == targetColour)
		//{							
		//	// go back to previus position
		//	GAMEAPPVIEW->m_snooker->RestorePreviousPosition();
		//	for(int m=0;m<MAX_TOTALBALLS;m++)
		//	if(TestFlag(GAMEAPPVIEW->m_snooker->m_ball[m]->m_flags, Flg_BALL_ACTIVE|Flg_BALL_POTTED))
		//	{
		//		GAMEAPPVIEW->m_snooker->m_ball[m]->UpdateFloats();
		//		ClearFlag(GAMEAPPVIEW->m_snooker->m_ball[m]->m_flags, Flg_BALL_POTTED);
		//	}
		//	
		//	// strike at same angle & simulate all balls
		//	GAMEAPPVIEW->m_snooker->TestBallStrike(D, cueTip, FALSE);				

		// call refree to find to see it it's still my turn (=> good shot)
		GAMEAPPVIEW->m_snooker->m_customTable->Referee(CTMSG_SIMEND);				
		
		//}
		//else
		//{
		//	// we didn't hit the ball-on, so swap turn (failed to hit the correct ball/in-off)
		//	GAMEAPPVIEW->m_snooker->m_bTurn1P^=1;
		//}
		
		// if it's still my turn, then this is a good shot!
		if(bBotTurn == GAMEAPPVIEW->m_snooker->m_bTurn1P)
		{
			run++;
			m_bDirectPots[n] = TRUE;
			//bPositiveShot = TRUE;
			
			// go back to previus position
			//GAMEAPPVIEW->m_snooker->RestorePreviousPosition();
			//for(int m=0;m<MAX_TOTALBALLS;m++)
			//if(TestFlag(GAMEAPPVIEW->m_snooker->m_ball[m]->m_flags, Flg_BALL_ACTIVE|Flg_BALL_POTTED))
			//{
			//	GAMEAPPVIEW->m_snooker->m_ball[m]->UpdateFloats();
			//	ClearFlag(GAMEAPPVIEW->m_snooker->m_ball[m]->m_flags, Flg_BALL_POTTED);
			//}			
		}
		else
		{
			run = 0;
		}
					
		// reset turn
		GAMEAPPVIEW->m_snooker->m_bTurn1P = bBotTurn;//1
					
		// go back to previus position
		GAMEAPPVIEW->m_snooker->RestorePreviousPosition();
		for(int m=0;m<MAX_TOTALBALLS;m++)
		if(TestFlag(GAMEAPPVIEW->m_snooker->m_ball[m]->m_flags, Flg_BALL_ACTIVE|Flg_BALL_POTTED))
		{
			GAMEAPPVIEW->m_snooker->m_ball[m]->UpdateFloats();
			ClearFlag(GAMEAPPVIEW->m_snooker->m_ball[m]->m_flags, Flg_BALL_POTTED);
		}

		// do good shot test (C)
		if(run>=5)
			break; // we've found a good pot!

		// do our dot-product test (C & O)
		oldDot = dot;
		dot = D.Dot(Dir[1]);
		if(dot < oldDot)
			break;
	} // end for n

	return TRUE;
	/*
	// check if we didn't find a hit
	if(n==180*2) n = -1;

	// update bot's strike data
	m_nAngle = n;
	m_powerPercentage = powerPercentage;
	m_cueTip[0] = cueTip[0];
	m_cueTip[1] = cueTip[1];
	m_cueTip[2] = cueTip[2];
	*/
}

BOOL CBot::DirectHitFindPot(void)
{
	BOOL bPotting;
	int bestStartAngle;
	int bestRun;
	int startAngle;
	int run;
		
	bPotting = FALSE;
	startAngle = 0;
	run = 0;
	bestStartAngle = 0;
	bestRun = 0;

	for(int n=0;n<180*2;n++)
	{
		if(!bPotting)
		{
			// check for starting pot (i.e. transition from miss to pot!)
			if(m_bDirectPots[n])
			{
				bPotting = TRUE;
				startAngle = n;
				run = 1;
			}			
		}
		else
		{
			if(m_bDirectPots[n])
			{
				run++;			
			}
			else
			{
				bPotting = FALSE;
				// update best pot-run so far
				if(run > bestRun)
				{
					bestStartAngle = startAngle;
					bestRun = run;
				}
			}
		}
	} // end for n

	// special case, if pot-run reaches end of array
	if(bPotting)
	{
		bPotting = FALSE;
		// update best pot-run so far
		if(run > bestRun)
		{
			bestStartAngle = startAngle;
			bestRun = run;
		}
	}
	
	// work out best pot within pot-run
	if(bestRun == 0) m_nAngle = -1;
	else m_nAngle = bestStartAngle + bestRun/2;

	{
		char buff[256];
		sprintf(buff,"bestStartAngle = %d, bestRun = %d", bestStartAngle, bestRun);
		ErrorMessage(m_parent, buff);
	}

	/*
	// debug
	char buff[256];
	for(int n=0;n<180*2;n++)
	{
		sprintf(buff,"m_bDirectPots[n] = %d", m_bDirectPots[n]);
		ErrorMessage(m_parent, buff);
	}
	*/
	
	if(m_nAngle == -1) return FALSE;

	return TRUE;
}

void CBot::SetStrikeVector(int selectBall, CVector PTarget, float powerPercentage)
{
	// set strike vector of shot
	CVector strikeVector;
	strikeVector = GAMEAPPVIEW->m_snooker->m_ball[selectBall]->m_P - PTarget;
	strikeVector = strikeVector.Unit();
	strikeVector = strikeVector * powerPercentage;

	// setup other dependant snooker vars.
	GAMEAPPVIEW->m_snooker->m_sPoint[0] = GAMEAPPVIEW->m_snooker->m_tableMousePos[0];
	GAMEAPPVIEW->m_snooker->m_sPoint[1] = GAMEAPPVIEW->m_snooker->m_tableMousePos[1];
	GAMEAPPVIEW->m_snooker->m_sPoint[2] = GAMEAPPVIEW->m_snooker->m_tableMousePos[2];

	GAMEAPPVIEW->m_snooker->m_tableMousePos[0] += strikeVector.x;
	GAMEAPPVIEW->m_snooker->m_tableMousePos[1] += strikeVector.y;
	GAMEAPPVIEW->m_snooker->m_tableMousePos[2] += strikeVector.z;

	//m_nAngle = 0;
	m_DAngle = CVector(GAMEAPPVIEW->m_snooker->m_sPoint[0] - GAMEAPPVIEW->m_snooker->m_tableMousePos[0],
					   GAMEAPPVIEW->m_snooker->m_sPoint[1] - GAMEAPPVIEW->m_snooker->m_tableMousePos[1],
					   GAMEAPPVIEW->m_snooker->m_sPoint[2] - GAMEAPPVIEW->m_snooker->m_tableMousePos[2]);
	
	GAMEAPPVIEW->m_snooker->m_cueAngle = atan2((double)-m_DAngle.y, (double)-m_DAngle.x); // useful for displaying 3d rendered cue orientation
}

void CBot::SetPowerPercentage(float powerPercentageInitial)
{
	// set power for shot
	float powerPercentage;
	powerPercentage = powerPercentageInitial;
	#ifdef THE_POOL_CLUB
	powerPercentage *= (1400.0f / 100.0f);
	#else
	powerPercentage *= (1600.0f / 100.0f);
	#endif //THE_POOL_CLUB
	m_powerPercentage = powerPercentage;
}

void CBot::SetSelectBall(int selectBall)
{
	CPoint pointTmp;
	pointTmp.x = 0;
	pointTmp.y = 0;
	GAMEAPPVIEW->m_snooker->m_tableMousePos[0] = GAMEAPPVIEW->m_snooker->m_ball[selectBall]->m_P.x;
	GAMEAPPVIEW->m_snooker->m_tableMousePos[1] = GAMEAPPVIEW->m_snooker->m_ball[selectBall]->m_P.y;
	GAMEAPPVIEW->m_snooker->m_tableMousePos[2] = GAMEAPPVIEW->m_snooker->m_ball[selectBall]->m_P.z;

	if(GAMEAPPVIEW->m_snooker->SelectBall(1, pointTmp) == TRUE)
	{
		GAMEAPPVIEW->m_snooker->m_bStandUpFromShot = 0;
		GAMEAPPVIEW->m_snooker->m_sightlineFade = 60;
		GAMEAPPVIEW->m_snooker->m_cuePowerPercentage = 0;
		GAMEAPPVIEW->m_snooker->m_bCuePowerJustOn = TRUE;
		//if(m_snooker->m_bPracticeMode)
			GAMEAPPVIEW->m_snooker->m_bSeenSelectBall = TRUE;
		//else
		//	m_snooker->SelectBallSend(1, point, m_snooker->m_windowWidth, m_snooker->m_windowHeight, m_snooker->m_selectBall);
	}
}

CVector CBot::FindDirectPotVector(int ballN)
{
	// calculate target point
	int n;
	CVector P;
	CVector B;
	CVector S, T;
	CVector A;
	float chance;
	float best;
	float maxTableLength;
	float objectToPocketLength;
	BOOL bNotBlocked;
			
	// score pockets direct pot chances
	best = -1;
	chance = -1;
	m_PocketDirectPotBest = rand()%6; // randomly choose any pocket initally

	P = m_PAimPocket[0] - m_PAimPocket[4];
	maxTableLength = P.Magnitude();
		
	for(n=0;n<6;n++)
	{
		// use dot product to find all viable shots
		P = GAMEAPPVIEW->m_snooker->m_ball[0]->m_P; // cue-ball
		
		B = GAMEAPPVIEW->m_snooker->m_ball[ballN]->m_P; // object-ball

		S = m_PAimPocket[n];
		B = B - S;
		objectToPocketLength = B.Magnitude();
		B = B.Unit();
		B = B * (BALL_RADIUS*2.0f);
		B = B + GAMEAPPVIEW->m_snooker->m_ball[ballN]->m_P; // ghost-ball point of contact = object-ball + adjust vector

		// check if shot can be play directly, i.e. not blocked by other object-balls
		A =  B -  GAMEAPPVIEW->m_snooker->m_ball[0]->m_P;
		bNotBlocked = FindDirectHitVector(ballN, A);
		{
		char buff[256];
		sprintf(buff, "[n=%d] FindDirectHitVector(...) = %d", n, bNotBlocked);
		ErrorMessage(m_parent, buff);
		}
		/////////////////////////////////////////////////////////////////////////////
			
		T = B - P;
		S = m_PAimPocket[n] - B;
		S = S.Unit();
		T = T.Unit();
		
		// calculate chance
		chance = -1;
		if(bNotBlocked)
		{
			chance = T.Dot(S);
			chance = chance - (objectToPocketLength/maxTableLength);
			m_PocketDirectPotChance[n] = chance;
			if(chance > best)
			{
				best = chance;
				m_PocketDirectPotBest = n;
			}
		}
		char buff[256];
		sprintf(buff, "m_PocketDirectPotChance[%d] = %f", n, chance);//m_PocketDirectPotChance[n]);
		ErrorMessage(m_parent, buff);		
	}
	
	// final chosen direct-pot pocket target
	// TODO: if we cannot find a good chance, we need to try cushion escapes, etc (not just take a random direct pot)
	if(best<=0) return CVector(0,0,0);

	P = GAMEAPPVIEW->m_snooker->m_ball[ballN]->m_P;
	B = m_PAimPocket[m_PocketDirectPotBest];
	P = P - B;
	P = P.Unit();
	P = P * (BALL_RADIUS*2.0f);
	P = P + GAMEAPPVIEW->m_snooker->m_ball[ballN]->m_P;

	return P;
}

BOOL CBot::FindDirectHitVector(int ignoreBallN, CVector V)
{	
	int n;
	int m;
	CVector A;
	CVector B;
	CVector C;
	CVector P;
	CVector T;
	float dotResult, dotResult2, dotResult3;
	float blockerTest;
	float blockerSize;
	float f;
	
	A = V;
	A = A.Unit();
	P = A.Cross(CVector(0,0,1));
	
	// calculate blockerSize
	B = V;
	C = B.Unit();
	B = B + C * (BALL_RADIUS*2.0f);
	blockerSize = B.Magnitude();

	
	// scan all object balls excluding cue-ball & ignored-ball (i.e. ball trying to pot)
	for(n=0;n<MAX_TOTALBALLS;n++)
	if(TestFlag(GAMEAPPVIEW->m_snooker->m_ball[n]->m_flags, Flg_BALL_ACTIVE))
	{
		if(n==0 || n ==ignoreBallN) continue; // skip this ball

		// check if object-ball is potential blocker (along path to hitting object-ball)
		// 1st test
		B = GAMEAPPVIEW->m_snooker->m_ball[n]->m_P - GAMEAPPVIEW->m_snooker->m_ball[0]->m_P;
		blockerTest = V.Dot(B);
		if(blockerTest<=0) continue; // skip this ball

		// 2nd test - check for vector length > magnitude |V|, skip this ball
		f = B.Magnitude();
		if(f >= blockerSize) continue; // skip this ball


		// Test1 - object-ball Direct line of center
		C = B.Unit();
		dotResult = C.Dot(P);
		
		// Test2 - object-ball edge#1 of center
		T = P*(BALL_RADIUS*2.0f);
		C = B + T;
		C = C.Unit();
		dotResult2 = C.Dot(P);
		
		// Test3 - object-ball edge#2 of center
		T = P*(BALL_RADIUS*2.0f);
		C = B - T;
		C = C.Unit();
		dotResult3 = C.Dot(P);

		// check for changing signs from the three results - Note: All successes should have same signs -> passed test!
		if( (dotResult>0 && dotResult2>0 && dotResult3>0) || // +ve
			(dotResult<0 && dotResult2<0 && dotResult3<0) ) // -ve
		{
			char buff[256];
			sprintf(buff, "[n = %d] dotResult = %f, dotResult2 = %f, dotResult3 = %f", n, dotResult, dotResult2, dotResult3);
			ErrorMessage(m_parent, buff);
		}
		// failed test!! ;(
		else
		{
			char buff[256];
			sprintf(buff, "*[n = %d] dotResult = %f, dotResult2 = %f, dotResult3 = %f", n, dotResult, dotResult2, dotResult3);
			ErrorMessage(m_parent, buff);
			
			// therefore we can't hit directly, so now need to play off cushions, etc		
			return FALSE;			
		}
	}
	
	return TRUE;
}

void CBot::Process(void)
{
	BOOL bSuccess;
		
	if(m_bStatus)
	{
		//GAMEAPPVIEW->m_snooker->m_bPracticeMode = FALSE;
		GAMEAPPVIEW->m_snooker->m_customTable->m_type = CTTYPE_9BALL;

		// set select cue-ball
		SetSelectBall(0);

		///////////////////////////////////////
		// bot processing code here!
		

		// set power for shot
		SetPowerPercentage(20.0f);
		
		// set strike vector of shot
		int objectBall = GAMEAPPVIEW->m_snooker->m_colourseq;
		if(objectBall == 0)
			objectBall = 1;

		CVector PTarget = FindDirectPotVector(objectBall);
		if(PTarget.Magnitude2() == 0)
		{
			CPoint pointTmp;
			pointTmp.x = 0;
			pointTmp.y = 0;
			GAMEAPPVIEW->m_snooker->SelectBall(0, pointTmp);
		}
		else
		{
			SetStrikeVector(0, PTarget, m_powerPercentage);
			Strike();
		}
		
		m_bStatus = FALSE;

		
		/*
		// (1) try direct hit search
		bSuccess = DirectHitSearch();
		
		if(bSuccess)
		{
			bSuccess = DirectHitFindPot();
			
			// update bot's strike data
			m_powerPercentage = 30.0f;
			#ifdef THE_POOL_CLUB
			m_powerPercentage *= (1400.0f / 100.0f);
			#else
			m_powerPercentage *= (1600.0f / 100.0f);
			#endif //THE_POOL_CLUB
		}
		
		// (2) if we don't find a direct hit pot, then try all other possiblities (i.e. off 1/2/3 cushions)
		if(!bSuccess)
			LongSearch();

		if(m_nAngle!=-1)
		{
			Strike();
		}
		
		m_bStatus = FALSE;
		*/
	}
}

void CBot::Draw(void)
{
	if(m_bStatus)
	{



	}
}
