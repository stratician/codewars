// Snooker.h
//
/////////////////////////////////////////////////////////////////////////////

#ifndef __SNOOKER_H__
#define __SNOOKER_H__

#include "../CodeSupp/GameApp.h"
#include "../CodeSupp/FoulDlg.h"
//#include "../CodeSupp/BitmapEx.h"
#include "../CodeSupp/Vector.h"

#include "Ball.h"

#define MAX_AVATARS						64

#define CAMERA_MODE_PLAYER_2D			0x00
#define CAMERA_MODE_PLAYER_3D			0x01
#define CAMERA_MODE_WATCH				0x02
#define CAMERA_MODE_TV					0x03
#define CAMERA_MODE_3DFREE				0x04
#define CAMERA_MODE_DEMO				0x05
#define CAMERA_MODE_DEMO_FIXED			0x06

#define CUE_POWER_PERCENTAGE_SHIFT		0.08f
#define CUE_POWER_MAX					1600.0f	//(10.0f / BALL_STRIKEFACTOR); // 1600.0f

// [for 3d cue movement at striking time]
#define CUE_FEATHER_SPEED				0.14f//0.2f
#define CUE_FEATHER_WATCHERFACTOR		1.0f
#define CUE_STRIKEY_MAXSPEED			90.0f//120.0f

#define CUE_DELIVERY_TIME				50*2.0f // frames (2 seconds)
#define CUE_PULLBACK_TIME				65.0f	// frames

#define CUE_STRIKE MAXSPEED				120.0f //XXXX
#define CUE_STRIKE_SPEEDLOSS			0.91f//0.92f
#define CUE_STRIKE_MAXDIST				(BALL_RADIUS * 12.0f) //(BALL_RADIUS * 20.0f)
#define CUE_STRIKE_STARTFADE			50.0f	// frames

#define CUE_PULLBACK_SPEED				1.0f
#define CUE_PULLBACK_MAXSPEED			15.0f
//

#define CUE_DEFAULT_RAISEDBUTT			4.0f//8.0f//10.0f////8.0f//0.0f	// in degrees

#define MOUSE_CUE_SENSITIVY_DEFAULT		0.0125f // 0.025f - 0.25f

#define MOUSE_CAMERA_SENSITIVY_X	1.0f//0.25f
#define MOUSE_CAMERA_SENSITIVY_Y	2.0f

// snooker
enum {WHITE = 0, RED, YELLOW, GREEN, BROWN, BLUE, PINK, BLACK, MAX_BALLS,
MAX_WHITEBALLS = 1,
MAX_REDBALLS = 21+7,//15,
MAX_YELLOWBALLS = 1,
MAX_GREENBALLS = 1,
MAX_BROWNBALLS = 1,
MAX_BLUEBALLS = 1,
MAX_PINKBALLS = 1,
MAX_BLACKBALLS = 1,
MAX_TOTALBALLS = 28+7,//22,
};


#define MAX_TOUCHING_BALLS		6

extern COLORREF ballColor[MAX_BALLS];
extern COLORREF poolBallColor[1+15];


#define MAX_CUES	8
extern M3d_object m_M3d_Cue[MAX_CUES];
extern M3d_object m_M3d_Pen;
extern M3d_object m_M3d_Purchased;
extern M3d_object m_M3d_scene1;
extern M3d_object m_M3d_stframe;
extern M3d_object m_M3d_stshadow;


//extern M3d_object m_M3d_Table_fa;
extern M3d_object m_M3d_Table_fb;


extern float g_bioAverageShotTime;
extern float g_bioAveragePosition;
extern float g_bioAverageSafety;
//extern float g_bioAveragePot;
extern float g_pbioAverageBreak; // pro backups
extern float g_pbioAverageShotTime;
extern float g_pbioAveragePosition;
extern float g_pbioAverageSafety;
extern float g_sbioAverageBreak; // semi-pro backups
extern float g_sbioAverageShotTime;
extern float g_sbioAveragePosition;
extern float g_sbioAverageSafety;

//#define D3D_MATERIAL_POWER		60.0f		// sharpness of specular highlights
#define D3D_MATERIAL_POWER		60.0f		// sharpness of specular highlights

//enum {RERACK_START = 0, RERACK_LINEUP, RERACK_COLOURS = 2, RERACK_BLACK};
#define RERACK_START			0
#define RERACK_LINEUP			1
#define RERACK_COLOURS			2
#define RERACK_BLACK			3
#define RERACK_REDSSCATTERED1	4
#define RERACK_REDSSCATTERED2	5
#define RERACK_WHITEONLY		6
#define RERACK_LOAD				7
#define RERACK_BILLIARDS		8
//------------------------------- //
#define RERACK_9BALL			10
#define RERACK_8BALL			11
#define RERACK_8BALLSCATTERED	12
#define RERACK_9BALLSCATTERED	13
#define RERACK_ROTATIONBALL		14
#define RERACK_BOWLS			15
#define RERACK_6BALL			16
#define RERACK_6BALLSCATTERED	17
#define RERACK_10BALL			18
#define RERACK_10BALLSCATTERED	19
#define RERACK_UKPOOL			20
#define RERACK_BREAK			21
//------------------------------- //
#define RERACK_LEAVE			0xFE
#define RERACK_EMPTY			0xFF

// foul flags:
#define Flg_FOUL_NORMAL				0x0001
#define Flg_FOUL_WHITEPOTTED		0x0002
#define Flg_FOUL_FREEBALL			0x0004
#define Flg_FOUL_MISS				0x0008
#define Flg_FOUL_PUSH				0x0010
#define Flg_FOUL_GAMEOVER			0x0020
#define Flg_FOUL_BALLINHAND			0x0040
#define Flg_FOUL_FREEBALLSNOOKER	0x0080

#define Flg_FOUL_ALERT				0x0100
#define Flg_FOUL_ALERTPUSHOUT		0x0200
#define Flg_FOUL_ALERTPUSHOUT2		0x0400
#define Flg_FOUL_ALERTPOT8ONBREAK	0x0800

#define Flg_FOUL_NOTHITCUSHIONBREAK	0x1000
#define Flg_FOUL_NOTHITCUSHION		0x2000

#define Flg_MustPlayDownTable		0x4000

#define Flg_FOUL_BREAK				0x8000
#define Flg_FOUL_ALERTUKPOOLFOUL	0x0080		// Same as Flg_FOUL_FREEBALLSNOOKER so we must check it's ipool when using this on English Pool

// simple strcuture to hold ball plane-positions & colours
typedef struct ballPosColour_typ {
float x;
float y;
int colour;
} ballPosColour, *ballPosColour_ptr;

// [CObstacle] ///////////////////////////////////////////////////////////////////////////////////////////////////////

typedef struct {
float m_lambda;
CVector m_hitpos;
CVector m_normal;
CVector m_contactpos;
} OBSTACLE_DATASTRUCT;

typedef struct {
__fix m_lambda;
FVector m_hitpos;
FVector m_normal;
FVector m_contactpos;
} OBSTACLE_DATASTRUCTF;

#define OBSTACLE_NULL			0
#define OBSTACLE_CYLINDER		1
#define OBSTACLE_PLANE			2

// BASE CLASS //

class CObstacle {
public:
DWORD m_type;
CObstacle();
~CObstacle();
virtual BOOL Detect(void *pData, CVector &wstart, CVector &vect, float radius);
virtual BOOL Detect(void *pData, FVector &wstart, FVector &vect, __fix radius);
virtual void Draw(void);
};

// CYLINDER //

class CObstacleCYLINDER : public CObstacle {
public:
DWORD m_type;
CVector m_pos;
float m_radius;

FVector f_pos;
__fix f_radius;

BOOL m_bInterior;
CObstacleCYLINDER(CVector &pos, float radius, BOOL bInterior = FALSE);
CObstacleCYLINDER(FVector &pos, __fix radius, BOOL bInterior = FALSE);
~CObstacleCYLINDER();
BOOL Detect(void *pData, CVector &wstart, CVector &vect, float radius);
BOOL Detect(void *pData, FVector &wstart, FVector &vect, __fix radius);
void Draw(void);
};

// PLANE //

class CObstaclePLANE : public CObstacle {
public:
BOOL m_bDoubleSided;
DWORD m_type;
CVector m_start;
CVector m_vect;
CVector m_normal;
float m_length;

FVector f_start;
FVector f_vect;
FVector f_normal;
__fix f_length;

CObstaclePLANE(CVector &start, CVector &end, BOOL bDoubleSided = FALSE);
CObstaclePLANE(FVector &start, FVector &end, BOOL bDoubleSided = FALSE);
~CObstaclePLANE();
BOOL Detect(void *pData, CVector &wstart, CVector &vect, float radius);
BOOL Detect(void *pData, FVector &wstart, FVector &vect, __fix radius);
BOOL DetectSide(void *pData, CVector &wstart, CVector &vect, float radius, BOOL bSide);
BOOL DetectSide(void *pData, FVector &wstart, FVector &vect, __fix radius, BOOL bSide);
BOOL DetectPoint(void *pData, CVector &p1, CVector &wstart, CVector &vect, float radius);
BOOL DetectPoint(void *pData, FVector &p1, FVector &wstart, FVector &vect, __fix radius);
void Draw(void);
};

// [CObstacleCourse] /////////////////////////////////////////////////////////////////////////////////////////////////

class CObstacleCourse {
public:
CObstacle **m_obstaclelst;
int m_obstaclelst_n;
CObstacleCourse();
~CObstacleCourse();
int AttachObstacle(CObstacle *pObstacle);
int Detect(void *pData, CVector &wstart, CVector &vect, float radius);
int Detect(void *pData, FVector &wstart, FVector &vect, __fix radius);
void Draw(void);
};

// [CCustomTable] ///////////////////////////////////////////////////////////////////////////////////////////////////////

#define POCKET_BLACKG		0
#define POCKET_BLACKY		1
#define POCKET_BLUEG		2
#define POCKET_BLUEY		3
#define POCKET_GREEN		4
#define POCKET_YELLOW		5
#define POCKET_OBJECT		10

class CPocketProp {
public:
float m_radius;
float m_suctionRadius;
float m_suctionFactor;
};

#define CTTYPE_SNOOKER			0
#define CTTYPE_SNOOKER10		1
#define CTTYPE_SNOOKER6			2
#define CTTYPE_RESPOTBLACK		3
#define CTTYPE_BILLIARDS		4


#define CTTYPE_8BALL			5
#define CTTYPE_9BALL			6
#define CTTYPE_ROTATIONBALL		7
#define CTTYPE_BOWLS			8
#define CTTYPE_6BALL			9
#define CTTYPE_10BALL			10
#define CTTYPE_UKPOOL			11
#define CTTYPE_BREAK			12

#define CTTYPE_CUSTOM			255

#define CTMSG_MOUSEOVER1		0	// "main table view"
#define CTMSG_BALLSELECT		1
#define CTMSG_LINESET			2
#define CTMSG_MOUSEOVER2		3	// "minicam view"
#define CTMSG_STRIKE			4
#define CTMSG_SIMEND			5
#define CTMSG_POCKETSET			6

typedef struct ctmsg_LINESET_DATA_typ {
	CPoint point;
} ctmsg_LINESET_DATA, *ctmsg_LINESET_DATA_ptr;

typedef struct ctmsg_POCKETSET_DATA_typ {
	CPoint point;
} ctmsg_POCKETSET_DATA, *ctmsg_POCKETSET_DATA_ptr;


class CColorScheme {
public:
float m_tableCushionRed;
float m_tableCushionGreen;
float m_tableCushionBlue;
float m_tableSlateRed;
float m_tableSlateGreen;
float m_tableSlateBlue;
public:	
};

#define _256TO1(n) (((float) n)*(0.003922f))	// 1/255

class CCustomTable {
public:
DWORD m_type;
BYTE subtype;
CSnooker *m_pParent;
CVector *m_pocketPos;
DWORD *m_pocketType;
CPocketProp *m_pocketProp;
int m_pocket_max;
int m_pocket_n;
CObstacleCourse *m_obstacleCourse;
M3d_object m_M3d_Ball[64];
M3d_object *m_M3d_Ball_low;
M3d_object *m_M3d_Ball_med;
M3d_object *m_M3d_Ball_high;
M3d_object *m_M3d_Ball_end;
M3d_object m_M3d_Table;
M3d_object m_M3d_caromTable;
M3d_object m_M3d_proTable; // pro table for iPool
M3d_object m_M3d_ukTable; // English Pool Table for iPool
M3d_object m_M3d_bigTable; // BIG snooker Table for BIG balls
M3d_object *m_M3d_Table_ptr[9];
int m_tableType;
M3d_object m_M3d_PocketObject;
CColorScheme *m_ColorSchemeStandard;
int m_ColorSchemeStandard_n;
CColorScheme *m_pColorScheme;
int m_ColorScheme_idx;

CCustomTable(CSnooker *pParent);
~CCustomTable();

void RenewObstacleCourse(void);
void BuildObstacleCourse(M3d_object *m_M3d_Table_ptr);
void DrawObstacleCourse(void);
BOOL AddPocket(CVector v, DWORD type = 0, float radius = 0, float suctionRadius = 0, float suctionFactor = 0); // used for tagging pockets, e.g. "You must pot in this specific pocket etc."
void ClearPockets(void);
BOOL PocketProximity(int pocketIdx, CVector v, float vRadius = 0.0f);
void ExtractObstacles(M3d_mesh *pMesh);
void ChangeColorScheme(int n, M3d_object *m_M3d_Table_ptr);
void ChangeGameType(int gameType);

void SnookerInit(void);		// iSnooker init.
void SnookerInitReset(void);		// iSnooker init.
void SnookerTableModifyMaterials(M3d_object *m_M3d_Table_ptr, char type = 0);
void SnookerTableAuthenticate(M3d_object *m_M3d_Table_ptr, char type = 0);
void SnookerTableChange(M3d_object *m_M3d_Table_ptr, int type = 0);
M3d_object *SnookerTableGetType(int tableTypeIdx);

void PoolInit(void);		// iPool init.
void PoolInitReset(void);		// iPool init.
void PoolTableChange(M3d_object *m_M3d_Table_ptr, int type = 0);
void PoolTableModifyMaterials(M3d_object *m_M3d_Table_ptr);
void PoolTableModifyMaterialsUKPool(M3d_object *m_M3d_Table_ptr);
M3d_object *PoolTableGetType(int tableTypeIdx);

void ReNormalizeBalls(void);
void Referee(DWORD msg, void *pData = NULL);
	void Encode(char type = 0);
	void Decode(void);
	void RefereeSnooker(DWORD msg, void *pData = NULL);
		void RefereeSnookerLINESET(DWORD msg, void *pData);
	void RefereeBilliards(DWORD msg, void *pData);
		void RefereeBilliardsLINESET(DWORD msg, void *pData);
	void Referee8Ball(DWORD msg, void *pData = NULL);
		void Referee8BallLINESET(DWORD msg, void *pData);
		void Referee8BallPOCKETSET(DWORD msg, void *pData);
		void Referee8BallRESPOTBALL(int n);
		void Referee8BallSIMEND(void);
		void Referee8BallReSpot(void);
		BOOL Referee8BallDropBallOnTable(int n, float x, float y, BOOL bDrop = TRUE);
		void Referee8BallUpdatePots(void);
		//void Referee8BallUpdateTimer(void);
		//void Referee8BallTimeUpReceive(void);
	void RefereeUKPool(DWORD msg, void *pData = NULL);
		void RefereeUKPoolLINESET(DWORD msg, void *pData);
		void RefereeUKPoolPOCKETSET(DWORD msg, void *pData);
		void RefereeUKPoolRESPOTBALL(int n);
		void RefereeUKPoolSIMEND(void);
		void RefereeUKPoolReSpot(void);
		BOOL RefereeUKPoolDropBallOnTable(int n, float x, float y, BOOL bDrop = TRUE);
		void RefereeUKPoolUpdatePots(void);
	void Referee9Ball(DWORD msg, void *pData = NULL);
		void Referee9BallLINESET(DWORD msg, void *pData);
		void Referee9BallPOCKETSET(DWORD msg, void *pData);
		void Referee9BallRESPOTBALL(int n);
		void Referee9BallSIMEND(void);
		void Referee9BallReSpot(void);
		BOOL Referee9BallDropBallOnTable(int n, float x, float y, BOOL bDrop = TRUE);
		void Referee9BallUpdatePots(void);
	void RefereeRotationBall(DWORD msg, void *pData = NULL);
		void RefereeRotationBallLINESET(DWORD msg, void *pData);
		void RefereeRotationBallPOCKETSET(DWORD msg, void *pData);
		void RefereeRotationBallRESPOTBALL(int n);
		void RefereeRotationBallSIMEND(void);
		void RefereeRotationBallReSpot(void);
		BOOL RefereeRotationBallDropBallOnTable(int n, float x, float y, BOOL bDrop = TRUE);
		void RefereeRotationBallUpdatePots(void);
	void RefereeBreak(DWORD msg, void *pData = NULL);
		void RefereeBreakLINESET(DWORD msg, void *pData);
		void RefereeBreakPOCKETSET(DWORD msg, void *pData);
		void RefereeBreakRESPOTBALL(int n);
		void RefereeBreakSIMEND(void);
		void RefereeBreakReSpot(void);
		BOOL RefereeBreakDropBallOnTable(int n, float x, float y, BOOL bDrop = TRUE);
		void RefereeBreakUpdatePots(void);
	void RefereeBowls(DWORD msg, void *pData = NULL);
		void RefereeBowlsLINESET(DWORD msg, void *pData);
		BOOL RefereeBowlsBallOnTable(int n);
		BOOL RefereeBowlsBallOnTableNotPotted(int n);
		int RefereeBowlsNextStrikeBall(void);
		BOOL RefereeBowlsOutOfTimeNextBowl(void);
		void RefereeBowlsOutOfTime(void);
		void RefereeBowlsSIMEND(void);
		void RefereeBowlsReSpot(void);
		BOOL RefereeBowlsDropBallOnTable(int n, float x, float y, BOOL bDrop = TRUE);
		void RefereeBowlsUpdatePots(void);
	void Referee6Ball(DWORD msg, void *pData = NULL);
		void Referee6BallLINESET(DWORD msg, void *pData);
		void Referee6BallPOCKETSET(DWORD msg, void *pData);
		void Referee6BallRESPOTBALL(int n);
		void Referee6BallSIMEND(void);
		void Referee6BallReSpot(void);
		BOOL Referee6BallDropBallOnTable(int n, float x, float y, BOOL bDrop = TRUE);
		void Referee6BallUpdatePots(void);
	void Referee10Ball(DWORD msg, void *pData = NULL);
		void Referee10BallLINESET(DWORD msg, void *pData);
		void Referee10BallPOCKETSET(DWORD msg, void *pData);
		void Referee10BallRESPOTBALL(int n);
		void Referee10BallSIMEND(void);
		void Referee10BallReSpot(void);
		BOOL Referee10BallDropBallOnTable(int n, float x, float y, BOOL bDrop = TRUE);
		void Referee10BallUpdatePots(void);


		void Draw3D(void);
		void Draw3DPocketObjects(void);
		void Draw3DSnooker(void);
		void Draw3D8Ball(void);
		void Draw3D9Ball(void);
		void Draw3DRotationBall(void);
		void Draw3DBowls(void);
		void Draw3D6Ball(void);
		void Draw3D10Ball(void);

		

};

class CBall;
class CSnookerAI;

typedef struct {
float minX;
float maxX;
float minY;
float maxY;
} TableBoundTyp;


// [CSnooker] ///////////////////////////////////////////////////////////////////////////////////////////////////////////

class CSnooker
{
public:
	CSnooker(CWnd *pParent); // constructor
	~CSnooker(); // destructor

	BOOL m_firstTime;
	int m_firstFewFrames;
	CSnookerAI *m_snookerAI;
	CCustomTable *m_customTable;
	BOOL m_bMouseMoved;
	BOOL m_previousSaved; // For Undo.
	CWnd *m_parent;
	char array[1024];
	CBall *m_ball[MAX_TOTALBALLS];
	ballPosColour m_ballPosColour[MAX_TOTALBALLS]; // for storing ball position and colour (i.e. for re-rack loaded positions)
	char array2[1024];
	CBall *m_testBall;	// for exit traj. lines
	float m_testBallInitialSpeed;
	CBall *m_tmpBall;	// general purpose ball, for temp. operations
	char array3[1024];
	BOOL m_bTestBallFinished;
	int m_TestBallCollideObjColour;
	CVector m_TestBallCollideObjPos;
	CVector m_TestBallCollideVA;
	CVector m_TestBallCollideVB;
	int m_reRackType;
	BYTE m_wildRackType;
	BOOL m_bPracticeMode;
	BOOL m_bPracticeModeAlive;
	BOOL m_bComputerMode;
	BOOL m_bComputerModeAlive;
	int m_playersType;
	float m_timeDelta;

	char m_replayname1[22];
	char m_replayname2[22];
	//BOOL m_bCueError;
	int m_cueError;				// 0 = Beginner; 1 = Semi-Pro (Amateur); 2 = Pro
	BOOL m_bRated;
	BOOL m_bMissRule;
	BOOL m_bCallShot;

	

	// Bio. for 'yourself' player
	float m_bioShotTime; // from being your turn until striking ball
	float m_bioPosition; // error of each potted ball
	float m_bioSafety;	 // opponent's first shot and doesn't pot
	float m_bioPot;		 // check test-ball path to pocket and check if potted. Also did we pot ball (flukes, doubles, etc).
	
	int m_windowWidth;
	int m_windowHeight;

	int m_tableview_windowWidth;
	int m_tableview_windowHeight;

	M3d_vtx m_cameraX;
	M3d_vtx m_cameraY;
	M3d_vtx m_cameraZ;
	M3d_vtx m_cameraO;
	float m_cameraFov;
	float m_cameraAspect;

	BOOL m_cameraOn;


	int m_mwindowWidth;
	int m_mwindowHeight;
	int m_moffsetX;
	int m_moffsetY;
	M3d_vtx m_mcameraX;
	M3d_vtx m_mcameraY;
	M3d_vtx m_mcameraZ;
	M3d_vtx m_mcameraO;
	float m_mcameraFov;
	float m_mcameraAspect;

	BOOL m_mcameraOn;

	
	// window screen size width / height
	int m_cx;
	int m_cy;
	
	// 3D / 2D Toggle data //
	char m_3dtoggle_old;
	char m_3dtoggle;		
	int m_3dtoggle_status;	// 0: Not selected, 1: Mouse over (highlighted)

	// ball selection data
	int m_cueBallSelect;
	BOOL m_bStrikeBallReady;
	BOOL m_bCuePowerJustOn;
	BOOL m_bCueLineSet;
	BOOL m_bCueTipReady;
	BOOL m_bCueTipReadyFirstTime;
	int m_dominoHitLast;
	int m_dominoHit1st;
	BOOL m_bCueState;
	float m_cuePowerPercentage;
	int m_selectBall;
	int m_grabBall;
	BOOL m_bSeenSelectBall;
	CPoint m_point;
	M3d_vtx m_sPoint;
	//CPoint m_sPoint;
	CPoint m_sCPoint;
	CPoint m_sSize;
	CPoint m_deltaSum;
	float m_mouseX; // 2D pixel world.
	float m_mouseY; // 2D pixel world.
	BOOL m_tableMouseOn;
	M3d_vtx m_tableMousePos;
	M3d_vtx m_tableMousePosTmp;
	M3d_vtx m_tableMousePosStrike;
	BOOL m_ballMouseOn;
	M3d_vtx m_ballMousePos;

	M3d_vtx m_miniCameraAt;
	M3d_vtx m_miniCameraPos;
	BOOL m_miniCameraStartUpView;
	int m_CameraMode;
	int m_CameraOldMode;
	BOOL m_bCueTipOff;
	M3d_vtx m_cuetip;
	int m_targetPocket;
	int m_targetBall;
	BOOL m_bTargetSuccess;
	int m_RaiseButt; 
	int m_RaiseButtOld;
	BOOL m_bReceiveRaiseButtActive;
	BOOL m_bRaiseButtHelper;
	CVector m_cueTipStrikeDir;
	int m_sightlineFade;
	int m_cueStrikeTime;
	float m_cueStrikeSpeed;
	float m_cueStrikeDist;
	int m_rnd;
	float m_strikeRnd;
	float m_strikeMaxRnd;
	float m_strikeLine;
	float m_strikeDist;

	float m_cueAngle;	// radians
			
	// 3D table object
	M3d_object *m_M3d_Table_ptr;
	
	// 3D ball object
	M3d_object *m_M3d_Ball_ptr;
		
	M3d_object m_M3d_Ball_Shadow;

	// 3D cue object
	M3d_object *m_M3d_Cue_ptr;
	
	// HUD
	M3d_mesh m_M3dmap_plate_mesh;
	M3d_mesh m_M3dmap_cube_mesh;
	M3d_mesh m_M3dmap_tri_mesh;
	M3d_mesh m_M3dmap_utri_mesh;
	M3d_mesh m_M3dmap_uplate_mesh;

	Mgfx_spr_ptr m_M3dmap_texture;
	Mgfx_spr m_spritePN[2];
	Mgfx_spr m_spritePBall[15+2+2+4];
	Mgfx_spr m_spritePBallSmall[15+2+4];

	Mgfx_spr m_spriteViewTablesButton[2];
	//Mgfx_spr m_spritePower; // we now use vertex colors/shading
	//Mgfx_spr m_spritePower2;
	//Mgfx_spr m_spriteArrow3D;

	IDirect3DTexture9* m_spriteArrow3D;
	IDirect3DTexture9* m_TextureArrowWhite;
	IDirect3DTexture9* m_TextureDisc;
	IDirect3DTexture9* m_TextureDTop;
	IDirect3DTexture9* m_TextureDBot;
	IDirect3DTexture9* m_TextureWhite;
	IDirect3DTexture9* m_TextureRing;
	IDirect3DTexture9* m_TextureQMark;

	IDirect3DTexture9* m_spriteCameraFree;
	IDirect3DTexture9* m_spriteCameraGame;


	//Mgfx_spr m_spriteCam[10];


	#ifdef LOAD_QMARK
	Mgfx_spr m_spriteQMark3D;
	#endif //LOAD_QMARK
	IDirect3DTexture9** m_spriteCueBallTexture;
	
	BOOL m_bRefreshScores;
	
	// undo vars
	int m_score1PUndo;
	int m_break1PUndo;
	BOOL m_rednextUndo;
	int m_colourseqUndo;
	BOOL m_bBallInHandUndo;
	//

	int m_reRackRndVal;
	int m_foul3Times1P;
	int m_foul3Times2P;

	int m_score;
	int m_score1P;
	int m_score2P;
	int m_break1P;
	int m_break2P;
	int m_currentBreak;
	int m_currentBreak100s;
	int m_currentBreak10s;
	int	m_currentBreak1s;
	int	m_sfxCount;
	BOOL m_bPriorVoiceOnly;
	
	int m_voiceN;
	int m_highBreaks1P[2]; // possible 2 Breaks for regular single player games
	int m_highBreaks2P[2]; // possible 2 Breaks for regular single player games
	int m_bestScore;
	int m_gameOver;
	int m_gameOverIWon;
	BOOL m_bConcededGame;
	BOOL m_encoded;
	//BOOL m_bInvalidGame;
	BOOL m_bTurn1P;
	int m_lifes;
	int m_shotcount;    // Used to not give 2Shot Option on Break Off //
	Flags m_foul;		// foul shot (note this contains the foul type, which is passed on to foulType)
	BOOL m_bMiss;		// failed to pot ball	
	CFoulDlg m_foulDlg;
	BOOL m_bFoulDecideTurn;
	BOOL m_bFoulShowDialog;
	int m_seat;			// seat # 0/1 otherwise -1
	BOOL m_rednext;
//	BOOL m_bStrikeSend;
	
	int m_colourseq;
	
	int m_shotTime;
	int m_shotTimeOld;
	BOOL m_bShotTimeUp;

	int m_MaxShotTime;		// 2mins per shot, after that your opponent can force a forfit!
	int m_MaxShotTimeOld;

	int m_poolPots1P;		// to keep track of ball potted in succession (for iPool only)
	int m_poolPots2P;		// to keep track of ball potted in succession (for iPool only)
	
	int m_oldTouchingBall[MAX_TOUCHING_BALLS];
	int m_touchingBall[MAX_TOUCHING_BALLS];
	int m_touchingBallIdx;

	BOOL m_bBallInHand;
	BOOL m_bStandUpFromShot;
	int m_nominateBall;
	BOOL m_bShotInProgress;
	BOOL m_clientSimEnd; // end of simulation for this client
	BOOL m_SimEnd;		 // end of simulation for all clients (decided by server)
	BOOL m_SimulationEnd;
	int m_simEndCount;	 // counter for # of players that have sent sim. end messages
	int m_simEndWaitingDelay;
	int m_simEndWaitingTimeOut;
	int m_bSimJustFinished;
	BOOL m_process;
	int m_times;
	int m_cyc;
	int m_cycHitCushion;
	BOOL m_bCueBallHitCushion;
	CVector VOut[10];
	CVector VVOut[11][4];

	BOOL m_bStrikeBallReplayDrawSL; // draw sight lines flag
	
	int m_spriteAvatar_n;
	Mgfx_spr m_spriteAvatar[MAX_AVATARS];
	TableBoundTyp m_tableBound;

	Mgfx_fnt m_font,m_font2,m_font3;
	Mgfx_fnt m_snkfnt1,m_snkfnt2;


	float m_mouseCueSensitivity;

	void SetMouseSensitivity(int type);

	void LoadAvatars(void);
	void KillAvatars(void);

	void Create(void);
	void Init(int reRackType = RERACK_EMPTY, int rndval = 0, char playerTurn = 0);//RERACK_START);//RERACK_COLOURS);//
	void ClearBalls(void);
	void PullBallsApartFIXPOINT(void);
	void ReRack(int replay = 0, int _rndval = 0, int bMsgBalls = 0);
	void ReRackSTART(int rndval, int maxReds, int wildRackType = 0);
	void ReRackColourOnSpot(int n, int colour, BOOL bRndPos = FALSE);
	void ReRack8BALL_Group(int rndval, int maxBalls, BOOL bScatterBalls = FALSE, int bMsgBalls = 0);
	void ReRack8BALL_Random(int rndval, int maxBalls, BOOL bScatterBalls = TRUE, int bMsgBalls = 0);
	void ReRack9BALL_Group(int rndval, int maxBalls, BOOL bScatterBalls = FALSE, int bMsgBalls = 0);
	void ReRack9BALL_Random(int rndval, int maxBalls, BOOL bScatterBalls = TRUE, int bMsgBalls = 0);
	void ReRack6BALL_Group(int rndval, int maxBalls, BOOL bScatterBalls = FALSE, int bMsgBalls = 0);
	void ReRack6BALL_Random(int rndval, int maxBalls, BOOL bScatterBalls = TRUE, int bMsgBalls = 0);
	void ReRack10BALL_Group(int rndval, int maxBalls, BOOL bScatterBalls = FALSE, int bMsgBalls = 0);
	void ReRack10BALL_Random(int rndval, int maxBalls, BOOL bScatterBalls = TRUE, int bMsgBalls = 0);
	void Deinit(void);
	//void MouseMoveReceive(float x, float y, int w, int h);
	void MouseMoveReceive(float x, float y, int w, int h, float strikeLine, float strikeDist);
	void MouseMove(CPoint point);			// called whenever the mouse moves.
	void TableViewMouseMove(CPoint point);	// called whenever the mouse moves in main table window
	BOOL HamperedCueing(float x, float y, float w, float h);
	void MoveCueTip(float x, float y, float w, float h);
	void CueBallTipSend(float sx, float sy, float sz, float ex, float ey, float ez,
						BOOL ballMouseOn, M3d_vtx ballMousePos, M3d_vtx m_cueTip,
						int nominateBall,
						int RaiseButt, int RaiseButtOld,
						float cueAngle);
	void CueBallTipReceive(float sx, float sy, float sz, float ex, float ey, float ez,
						   BOOL ballMouseOn, M3d_vtx ballMousePos, M3d_vtx cueTip,
						   int nominateBall,
						   int RaiseButt, int RaiseButtOld,
						   float cueAngle);
	void CueBallTipReceiveCompact(void *sdc);
	void MiniCameraPlayerEyePos(M3d_vtx V);
	void MiniCameraPlayerEyePosAngle(float angle, int cueBallSelect = -1);
	void MiniCameraPlayerRaiseButt(void);
	void MiniCameraViewUpdate(void);
	BOOL MyTurn(void);
	BOOL IsSitting(void);
	void Referee(void);
	void RefereeBilliards(void);
	void RefereeScoreSfx(int score);
	int TotalPtsLeft(void);
	void PlayAgainReceive(void);
	void UpdateGameReceive(void);
	void FoulAlertReceive(char n);
	void Pot8OnBreakRerackReceive(char type = 0);
	void Pot8OnBreakRerackReceiveAll(void);
	void UKPoolBadBreakRerackReceiveAll(char type = 0);
	void BadBreakRerackReceive(BOOL bOpponentToBreak);
	void BadBreakRerackReceiveAll(BOOL bOpponentToBreak);
	void MissResetReceive(void);
	void MissResetReceiveAll(void);
	void ReSpot(void);
	void ReSpotBilliards(void);
	void FreeBallReferee(void);
	int FreeBallTestStrike(int balln, CVector StrikeDir, float maxDistance = -1, int collideColour = -1);
	BOOL FreeBallCheck(void);
	BOOL FreeBallSnookerCheck(int myFreeBallColour);
	void TableStartMessages(BOOL bPractice);
	int PocketProximity(float x, float y, float z, float extraRadius = 0.0f);
	void TestBallStrike(CVector StrikeDir, M3d_vtx cueTip, BOOL bWhiteOnly = FALSE);
	void HitCushionBallsInit(void);
	BOOL bIsStrikeBallOnTopOfAnother(int ballIdx, int excludeBallIdx);
	void ProcessBalls();	
	void Process(float timeDelta);
	//void ProcessSnookerBalls(void);
	BOOL ProcessSnookerBalls(BOOL bBotSim = FALSE, BOOL bWhiteOnly = FALSE);
	void ProcessSimEndCounters(void);
	void SimEndUpdateFloats(void);
	void SimEndReceive(int plrSockIdx, BOOL bShotEnd = TRUE);
	void PreRefereeBioCalcs(void);
	void PostRefereeBioCalcs(void);
	void DoLaggersCatchUp(void);
	void PreviousWndEditSetFocus(void);
	void SelectBallSend(int select, CPoint point, int w, int h, int selectBall);
	void SelectBallReceive(int select, CPoint point, int w, int h, int selectBall);
	BOOL SelectBall(int select, CPoint point);
	int GetSelectedBall(void);
	int GetSelectedPocket(void);
	void SetTargetBall(int n);
	void SetTargetPocket(int n);
	void ReceivePlayerTargetData(int targetPocket, int targetBall, int bTargetSuccess);
	BOOL DropBallOnTable(int n, float x, float y, BOOL bDrop = TRUE);
	BOOL NominateBall(CPoint point);
	void SavePreviousPosition(void);
	void RestorePreviousPosition(void);
	void UpdateGPottedBalls(void);
	float GetStrikeBallError(BOOL bWithRnd);			// Gets the Strike Ball Error using Type A or B, depending on which Lobby we're in.
	float GetStrikeBallErrorA(BOOL bWithRnd);			// Original Semi Pro & Pro Error System from 1.8.70
	float GetStrikeBallErrorB(BOOL bWithRnd);			// Semi Pro & Pro Error System from 2.2.43 (Tougher)
	float GetStrikeBallErrorExpert(BOOL bWithRnd);		// Tougher Error System
	void StrikeBallSend(BOOL bZeroPowerShot = FALSE);
	void StrikeBall(void);
	
	void UpdatePhysics(int type);

	void StrikeBallReplay(CReplayStrikeBall *rstrike);
	void StrikeBallReceive(float sx,float sy,float sz, float ex,float ey,float ez, M3d_vtx cuetip, int selectBall, float strikeRnd, float strikeMaxRnd,
								 BOOL ballMouseOn, M3d_vtx ballMousePos, int nominateBall, int testBallColour, DWORD crc, int replay = 0, BYTE nSimEndMsgBalls = 0);
	
	void GenerateStateReplay(void);
	void StateReplay(CReplayStrikeState *rstrike);
	void BallPosReplay(CReplayStrikeBallPos *rstrike);
	void BallPos(CVector pos, int selectBall, int replay = 0);
	void SetWindowSize(int w,int h);
	void SetTableViewWindowSize(int w,int h);
	void CameraPosAtUpFovAspect(M3d_vtx pos, M3d_vtx at, M3d_vtx up, float fov,float aspect);

	void MiniCameraPosAtUpFovAspect(M3d_vtx pos, M3d_vtx at, M3d_vtx up, float fov,float aspect);
	void SetMiniOffsetWindow(int offx,int offy, int w, int h);

	void InitMeshes(void);
	void InitPlate(void);
	void InitCube(void);
	void InitTri(void);
	void InitUTri(void);
	void InitUPlate(void);
	
	float ReLengthByBound(M3d_vtx pos, M3d_vtx rot, float length);
	void DrawTri(M3d_vtx pos, M3d_vtx rot, M3d_vtx scale, COLORREF rgba, float alpha);
	void DrawUTri(M3d_vtx pos, M3d_vtx rot, M3d_vtx scale, COLORREF rgba);
	void DrawPlate(M3d_vtx pos, M3d_vtx rot, M3d_vtx scale, COLORREF rgb);
	void DrawPlate2(M3d_vtx pos, M3d_vtx rot, M3d_vtx scale, COLORREF rgba);
	void DrawCube(long x,long y,long z,WORD c);
	void DrawSightLine(M3d_vtx pos, M3d_vtx rot, float magX, float magY, COLORREF rgba, BOOL bFade = FALSE);
	void DrawSightUTri(M3d_vtx pos, M3d_vtx rot, M3d_vtx scale, COLORREF rgba, BOOL bFade = FALSE);
	void DrawSpot(M3d_vtx pos, COLORREF rgba, float size);
	void DrawArc(M3d_vtx origin, float radius, float startAngle, float endAngle, COLORREF rgba, float thickness, unsigned int steps);
	void DrawCircle(M3d_vtx origin, float radius, COLORREF rgba, float thickness, unsigned int steps);
	void InitTexture(void);
	void LoadTexture(char *fname);
	void DeinitTexture(void);

	BOOL m_bRedrawChalkMarks;
	void InitChalkMarks(void);
	void MakeChalkMark(CBall *ball);
	void DrawChalkMarks(void);

	DWORD GenerateBallCRC(void);
	DWORD GenerateBallCRCF(void);

	void DrawBallShadow(int n);
	void DrawBall(int n, BOOL bTrans = FALSE);
	void DrawBallPos(M3d_vtx pos, float r, float g, float b, BOOL bTrans);
	void DrawBallPos2(M3d_vtx pos, float r, float g, float b, BOOL bTrans, M3d_vtx scaleMultiply);
	void DrawObstacleBall(float x, float y, float z, float r);
	void Draw3D(void); // Generic 3D Drawing
	void Draw3DDebug(void);
	void Init3DCueMaterials(void);
	BOOL PROCESS_CAMERA_TV(char mode = 0, BOOL force = FALSE);
	void PROCESS_CAMERA_POSITION(void);
};

#endif // __SNOOKER_H__
