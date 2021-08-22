// SnookerAI.h
//
/////////////////////////////////////////////////////////////////////////////

#ifndef __SNOOKERAI_H__
#define __SNOOKERAI_H__

#include "../CodeSupp/GameApp.h"
#include "../CodeSupp/FoulDlg.h"
//#include "../CodeSupp/BitmapEx.h"
#include "../CodeSupp/Vector.h"

class CSnooker;

class CSnookerAI {
public:
CSnooker *m_pParent;
CSnookerAI(CSnooker *pParent);
~CSnookerAI();
void Process(void);
void Debug(CVector &vPos);
void Draw(void);
};

#endif // __SNOOKERAI_H__