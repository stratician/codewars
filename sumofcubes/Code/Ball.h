///////////////////////////////////////////////////////////////////////////////////////////////////
// CBall (Ball.h)
//

#ifndef __BALL_H__
#define __BALL_H__

#include "common.h"
#include "Snooker.h"  
#include "../CodeSupp/FVector.h"

/////////////////////////////////////////////////////////////////////////////////////////////////////
#ifndef THE_POOL_CLUB
// ***SNOOKER CONSTANTS***
// ball constants
#define BALL_RESCALEMESH						// allows for rescaling of balls rather than accepting scale factor in mesh
#define BALL_RADIUS_INITIAL						26.09f
#define BALL_RADIUS								g_ballRadius
#define BALL_GAS								0.1f
#define BALL_MAXSPEED							10.0f
#define BALL_MAXSPEEDFACTOR						1.0f //0.75 // !!IMPORTANT!! MUST ALWAYS STAY AS 1.0f [CBall::Strike(...)]

// (note: BALL_POSITIONZOFFSET + BALL_DRAWOFFSETZ =~ 2.5f)
#define BALL_POSITIONZOFFSET					(BALL_RADIUS * 1.5f)  // (BALL_RADIUS * 2.8f)
#define BALL_DRAWOFFSETZ						(-BALL_RADIUS * 0.5f) //(-BALL_RADIUS * 0.35f)//(-BALL_RADIUS * 0.3f) // set this back to 0.0f, to go back to old way
//#define BALL_POSITIONZOFFSET					(BALL_RADIUS * 2.7f)
//#define BALL_DRAWOFFSETZ						(-BALL_RADIUS * 0.2f) // set this back to 0.0f, to go back to old way

/*
#ifndef MATCH_TABLE
#define BALL_ROLLINGFRICT						0.0040f//0.0042f//0.0038//0.004f///0.0025//0.025
#else
#define BALL_ROLLINGFRICT						0.0030f
#endif
*/

// #define BALL_ROLLINGFACTOR						(float) (BALL_ROLLINGFRICT/GAME_ITTR)

#define BALL_DEADVELOCITY						(0.001f/GAME_ITTR)
#define BALL_STRIKEFORCE						0.7//0.10f
#define BALL_STRIKEFACTOR						(float) (BALL_STRIKEFORCE/GAME_ITTR)
//#define BALL_ENERGYFACTOR						0.9//0.8//0.9
//#define BALL_ENERGYFACTOR						0.9//0.85//0.8//0.9

/*
#ifndef MATCH_TABLE
///
#define BALL_ENERGYFACTOR						0.98f//0.95//0.94f//1.0f//0.9//0.85//0.8//0.9
#define BALL_ENERGYFACTOR_N						0.835f//0.825f//0.95//0.85f//0.99f//0.85//0.8//0.9
#define BALL_ENERGYFACTOR_T						0.835f//0.825f//0.85f//0.85f//0.85//0.8//0.9
///
#else
// pack splits nicely
#define BALL_ENERGYFACTOR						0.98f
#define BALL_ENERGYFACTOR_N						0.87f
#define BALL_ENERGYFACTOR_T						0.87f
///
#endif



#ifndef MATCH_TABLE
#define BALL_CUSHIONFACTOR						0.8f //0.77//0.8f//0.8//0.82f//0.8
#else
#define BALL_CUSHIONFACTOR						0.76f//0.77f
#endif
*/


#define BALL_SLIPPINGFRICT_W					((0.005f * 30.0f/*25*//*25.0f*//*30.0f*//*38*//*70*/) / GAME_ITTR) //how much spin
//#define BALL_SLIPPINGFRICT_W					((0.005f * 18.0f/*25.0f*//*30.0f*//*38*//*70*/) / GAME_ITTR)
#define BALL_SLIPPINGFRICT_V					((0.005f * 12.0f/*15*//*12*//*25*//*18*/) / GAME_ITTR)  // how much it bites
//#define BALL_SLIPPINGFRICT_V					((0.005f * 10.0f/*15*//*12*//*25*//*18*/) / GAME_ITTR)
#define BALL_SIDESPINFRICT						((0.005f * 0.85f/*0.5*//*4*/) / GAME_ITTR)
#define BALL_LOSTSIDEFROMIMPACT					2.1f
#define BALL_SIDEFROMIMPACT						2.2f
#define BALL_SIDETRANSFERFACTOR					0.2f

#define BALL_NAPFORCE							((-0.005f * 0.1f) / GAME_ITTR)

#define BALL_MOTIONLESSV						0.005f//0.02f		// ball is no longer moving
#define BALL_MOTIONLESSW						0.02f		// ball is no longer spinning

#define BALL_NATURALROLLW						10.0f
#define BALL_NATURALROLLFORCE					((0.025f * 16.0f) / GAME_ITTR)//0.01//0.02
#define BALL_ROLLSPINCOLLISIONFACTOR			((0.34f * 16.0f) / GAME_ITTR)//0.3

/////////////////////////////////////////////////////////////////////////////////////////////////////

#else
/////////////////////////////////////////////////////////////////////////////////////////////////////
// ***POOL CONSTANTS***
// ball constants
#define BALL_RESCALEMESH						// allows for rescaling of balls rather than accepting scale factor in mesh
//#define BALL_RADIUS							28.4625f//26.09f //(1.5f*28.4625f) (0.75*28.4625f)
#define BALL_RADIUS_INITIAL						28.4625f
#define BALL_RADIUS								g_ballRadius
#define BALL_GAS								0.1f
#define BALL_MAXSPEED							10.0f//10.0f//10.0f
#define BALL_MAXSPEEDFACTOR						1.5f//2.0f//1.0f//1.0f //0.75

// (note: BALL_POSITIONZOFFSET + BALL_DRAWOFFSETZ =~ 2.5f)
#define BALL_POSITIONZOFFSET					(BALL_RADIUS * 1.5f)  // (BALL_RADIUS * 2.8f)
#define BALL_DRAWOFFSETZ						(-BALL_RADIUS * 0.5f) //(-BALL_RADIUS * 0.35f)//(-BALL_RADIUS * 0.3f) // set this back to 0.0f, to go back to old way
//#define BALL_POSITIONZOFFSET					(BALL_RADIUS * 2.7f)
//#define BALL_DRAWOFFSETZ						(-BALL_RADIUS * 0.2f) // set this back to 0.0f, to go back to old way

/*
#ifndef MATCH_TABLE
#define BALL_ROLLINGFRICT						0.0040f//0.0042f//0.0038//0.004f///0.0025//0.025
#else
#define BALL_ROLLINGFRICT						0.0024f//0.0030f
#endif

#define BALL_ROLLINGFACTOR						(float) (BALL_ROLLINGFRICT/GAME_ITTR)
//#define BALL_ROLLINGFACTOR						0.00020;
*/

#define BALL_DEADVELOCITY						(0.001f/GAME_ITTR)
#define BALL_STRIKEFORCE						1.0//0.7//0.10f
#define BALL_STRIKEFACTOR						(float) (BALL_STRIKEFORCE/GAME_ITTR)
//#define BALL_ENERGYFACTOR						0.9//0.8//0.9
//#define BALL_ENERGYFACTOR						0.9//0.85//0.8//0.9

/*
#ifndef MATCH_TABLE
///
#define BALL_ENERGYFACTOR						0.98f//0.95//0.94f//1.0f//0.9//0.85//0.8//0.9
#define BALL_ENERGYFACTOR_N						0.835f//0.825f//0.95//0.85f//0.99f//0.85//0.8//0.9
#define BALL_ENERGYFACTOR_T						0.835f//0.825f//0.85f//0.85f//0.85//0.8//0.9
///
#else
*/
/*
// pack splits nicely
#define BALL_ENERGYFACTOR						0.98f
#define BALL_ENERGYFACTOR_N						0.87f
#define BALL_ENERGYFACTOR_T						0.87f
*/
///
//#endif

//#ifndef MATCH_TABLE
//#define BALL_CUSHIONFACTOR						0.8f //0.77//0.8f//0.8//0.82f//0.8
//#else
// #define BALL_CUSHIONFACTOR						0.72f//0.76f//0.77f
//#endif

#define BALL_SLIPPINGFRICT_W					((0.005f * 30.0f/*25*//*25.0f*//*30.0f*//*38*//*70*/) / GAME_ITTR) //how much spin
//#define BALL_SLIPPINGFRICT_W					((0.005f * 18.0f/*25.0f*//*30.0f*//*38*//*70*/) / GAME_ITTR)
#define BALL_SLIPPINGFRICT_V					((0.005f * 12.0f/*15*//*12*//*25*//*18*/) / GAME_ITTR)  // how much it bites
//#define BALL_SLIPPINGFRICT_V					((0.005f * 10.0f/*15*//*12*//*25*//*18*/) / GAME_ITTR)
#define BALL_SIDESPINFRICT						((0.005f * 0.85f/*0.5*//*4*/) / GAME_ITTR)
#define BALL_LOSTSIDEFROMIMPACT					2.1f
#define BALL_SIDEFROMIMPACT						2.2f
#define BALL_SIDETRANSFERFACTOR					0.2f

#define BALL_NAPFORCE							((-0.005f * 0.1f) / GAME_ITTR)

#define BALL_MOTIONLESSV						0.005f//0.02f		// ball is no longer moving
#define BALL_MOTIONLESSW						0.02f		// ball is no longer spinning

#define BALL_NATURALROLLW						10.0f
#define BALL_NATURALROLLFORCE					((0.025f * 16.0f) / GAME_ITTR)//0.01//0.02
#define BALL_ROLLSPINCOLLISIONFACTOR			((0.34f * 16.0f) / GAME_ITTR)//0.3

/////////////////////////////////////////////////////////////////////////////////////////////////////
#endif //!THE_POOL_CLUB

// flags
#define Flg_BALL_ACTIVE							0x0001
#define Flg_BALL_HITBALL						0x0002
#define Flg_BALL_HITCUSHION						0x0004
#define Flg_BALL_DOMINOHITLAST					0x0008
#define Flg_BALL_MOTIONLESS						0x0010
#define Flg_BALL_PUREROLL						0x0020
#define Flg_BALL_POTTED							0x0040
#define Flg_BALL_NOSTRIKE						0x0080


// Global Variables for Physics.. We have them here now.. as they change depending on the Replay Version

extern float BALL_ENERGYFACTOR;//							0.98f
extern float BALL_ENERGYFACTOR_N;//						0.93 // 0.835f
extern float BALL_ENERGYFACTOR_T;//						0.93 // 0.835f
extern float BALL_CUSHIONFACTOR;
extern float BALL_ROLLINGFRICT;
extern float BALL_ROLLINGFACTOR;

class CSnooker;

class CBall
{
public:
	CBall(CSnooker *pParent); // constructor
	~CBall(); // destructor

	CSnooker *m_parent;
	
	//union {
	M3d_mtx m_m3dmtx;			// Transformation Matrix. [ M3D ]
	D3DXMATRIX m_d3dmtx;		// Transformation Matrix. [ D3D ]
	//} DUMMYUNIONNAMENA;
	
	//union {
	M3d_mtx m_m3dmtxV;			// Transformation Matrix. [ M3D ]
	D3DXMATRIX m_d3dmtxV;		// Transformation Matrix. [ D3D ]
	//} DUMMYUNIONNAMENB;
	
	// ball attributes
	Flags m_flags;
	int m_colour;
//	float m_diameter;
//	float m_weight;
	int m_ghost;
	int m_hitOtherBall;					// records the last ball that was hit by 'this' ball
	int m_hitCushion;					// [NEW] - to record everytime this ball comes into contact with a cushion, during a shot
	BOOL m_bHitCushionBeforeBall;
	CVector m_P, m_POld;
	CVector m_PSpot, m_PUndo;
	CVector m_V, m_VOld;
	CVector m_W;						// back / top & sverve spin
	int m_WDelay;
	float m_wnroll;
	float m_wSide;						// side spin on ball
	int m_FadeOut;						// Fade Out Effect after ball is potted
	int m_InsidePocket;
	int m_idx;							// Hold the index of the ball //
	BOOL PlayedBallAroundPocketSFX;

	FVector f_P, f_POld, f_PSpot, f_PUndo;
	FVector f_V, f_VOld;
	FVector f_W;
	__fix f_wnroll;
	__fix f_wSide;



	BOOL m_bMouseOver;

	int m_justHitAnotherBall;
	int m_justHitCushion;

	float m_timeDelta;

	BYTE m_cyc;
			
	void Init(float x, float y, float z, int colour, float rx = 0.0f, float ry = 0.0f, float rz = 0.0f, float spotx = 10000, float spoty = 10000);
	void Deinit(void);
	BOOL Proximity(float x, float y,float z, float r = BALL_RADIUS);
	void Strike(float sx,float sy,float sz, float ex,float ey,float ez, M3d_vtx cuetip, float rnd, BOOL bTestBall, BOOL bIsBot = FALSE);
	void AdjustOrientation(void);	// FIXED
	void ProcessGravity(void);	// FIXED - For English Pool Only for now
	char Inside_GSurface(__fix x, __fix y, __fix z);  // Returns Type of Surface we are inside
	void UpdateFloats(void);
	void UpdateFixeds(void);
	BOOL Process(BOOL bTestBall = FALSE, BOOL bIsBot = FALSE, int rollOff = 0, float timeDelta = 0.0f);
	BOOL AntiProcess(void);
	//BOOL Process2(BOOL bTestBall = FALSE); // magic one for testing specific frame index.
	BOOL Motionless(void);
	void TouchingBallStickyness(CBall *ball);
	BOOL IsTouchingBall(CBall *ball);
	void RollOff(BOOL bTestBall, int rollOff);
	void RollingSpin(void);
	BOOL SlippingFriction(void); // FIXED
	BOOL RollingFriction(void); // FIXED
	BOOL SideSpinFriction(void);
	void ApplySideSpinFixed(FVector N, __fix strength);
	void ApplySideSpin(CVector N, float strength);
	void NapEffect(void);
	BOOL CollideWithBall(CBall *ball, BOOL bTestBall = FALSE, BOOL bIsBot = FALSE, int ballIdx = -1, BOOL SetFlags = TRUE,  BOOL bFreeBallCheck = false);
	BOOL CollideWithBallSimple(CBall *ball);
	BOOL CollideWithBall2(CBall *ball, BOOL bTestBall = FALSE);
	void CollideWithCushions(void);
	BOOL CushionsProximity(BOOL bPoolForceTrue = TRUE);
	BOOL CBall::Collision3d(float radius);// = BALL_RADIUS);
	void Collision3d2(void);
	BOOL CollideWithPockects(BOOL bPotted = TRUE, BOOL bIsBot = FALSE);
	BOOL CollideWithPockectsNormal(BOOL bPotted = TRUE, BOOL bIsBot = FALSE);
	BOOL CollideWithPockectsUKPool(BOOL bPotted = TRUE, BOOL bIsBot = FALSE);
	BOOL TryReSpot1(CBall *ball);
	BOOL TryReSpot2(CBall *ball, CBall *nextspotball);
	BOOL TryReSpot3(CBall *ball);
	void ReSpotSuccessful(void);
	float CushionError(CVector StrikeDir, float eRange = BALL_RADIUS*6.0f);
	float HamperedByBallError(CBall *strikeBall, CVector StrikeDir);
	void Draw(void);
};

#endif // __BALL_H__

