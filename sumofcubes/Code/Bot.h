// Bot.h
//
/////////////////////////////////////////////////////////////////////////////

#ifndef __BOT_H__
#define __BOT_H__

#include "../CodeSupp/GameApp.h"
#include "../CodeSupp/Vector.h"

#include "Ball.h" // include snooker.h / fvector.h / common.h

// type flags:
#define Flg_TYPE_NORMAL				0x0001
#define Flg_TYPE_ATTACKING			0x0002
#define Flg_TYPE_DEFENSIVE			0x0004
#define Flg_TYPE_FAST				0x0008
#define Flg_TYPE_SLOW				0x0010

// [CBot] ///////////////////////////////////////////////////////////////////////////////////////////////////////////

class CBot
{
public:
	CBot(CWnd *pParent); // constructor
	~CBot(); // destructor

	CWnd *m_parent;

	CString m_name;
	int m_rating;
	Flags m_type;
	BOOL m_bStatus;
	int m_count;
	BOOL m_bDirectPots[180*2];
	CVector m_PAimPocket[6];
	float m_PocketDirectPotChance[6];
	int m_PocketDirectPotBest;
	
	// strike data
	CVector m_DAngle;
	int m_nAngle;
	float m_powerPercentage;
	M3d_vtx m_cueTip;

	void Init(CString name, int rating, Flags type);
	void Deinit(void);
	void LongSearch(void);
	void Strike(void);
	BOOL DirectHitSearch(void);
	BOOL DirectHitFindPot(void);
	void SetStrikeVector(int selectBall, CVector PTarget, float powerPercentage);
	void SetPowerPercentage(float powerPercentageInitial = 40.0f);
	void SetSelectBall(int selectBall = 0);
	CVector FindDirectPotVector(int ballN);
	BOOL FindDirectHitVector(int ignoreBallN, CVector V);
	void Process(void);
	void Draw(void);
};

#endif // __BOT_H__
