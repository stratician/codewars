// Effects.h
//
/////////////////////////////////////////////////////////////////////////////

#ifndef __EFFECTS_H__
#define __EFFECTS_H__

#include "../CodeSupp/GameApp.h"
#include "../CodeSupp/FoulDlg.h"
//#include "../CodeSupp/BitmapEx.h"
#include "../CodeSupp/Vector.h"

#define ENV_LIGHTSTATE_OFF		0
#define ENV_LIGHTSTATE_ON		1
#define ENV_LIGHTSTATE_TURNOFF	2
#define ENV_LIGHTSTATE_TURNON	3

class CEnvironment {
public:
BOOL m_bUpdate;
CWnd *m_pParent;
DWORD m_lightState;
int m_lightState_seq;
BOOL m_bLightOn[3];
float m_lightInt[3];								
M3d_vtx m_lightPos[3];
public:
CEnvironment(CWnd *pParent);
~CEnvironment();
void LightState(DWORD state);
void LightPos(int n, M3d_vtx pos);
void LightPos(int n, float x,float y,float z);

float LightFunction(float t, float s);
BOOL ShadowCast(M3d_vtx pos, M3d_vtx shadowPos, float zPlane,int lightIdx, float &atten);
void Process(void);	// process lighting
void Update(void); // update lighting - call just before a new rendition
};

#endif // __EFFECTS_H__