// CSnooker

#include <math.h>

#include "../CodeSupp/stdafx.h"
#include "../CodeSupp/GameAppView.h"
//#include "../CodeSupp/Vector.h"

#include "Common.h"
#include "Effects.h"

CEnvironment::CEnvironment(CWnd *pParent)
{
	m_pParent = pParent;
	m_bUpdate = TRUE;
	//LightState(ENV_LIGHTSTATE_OFF);
	//Process();
	//LightState(ENV_LIGHTSTATE_TURNON);
	/*int n;
	for(n=0;n<3;n++)
	{
	m_lightPos[n][0] = -1500+n*1500;
	m_lightPos[n][1] = 0;
	m_lightPos[n][2] = 800;//800;
	}
	*/

	
	LightPos(0,-1800,0,1000);
	LightPos(1,0,0,800);
	LightPos(2,+1800,0,1000);
	

	LightState(ENV_LIGHTSTATE_ON); // Turning Lights off does help!!
	//LightState(ENV_LIGHTSTATE_OFF);
	Process();
}

CEnvironment::~CEnvironment()
{

}

void CEnvironment::LightState(DWORD state)
{
	m_lightState = state;
	m_lightState_seq = 1;
}

void CEnvironment::LightPos(int n, M3d_vtx pos)
{
	LightPos(n, pos[0],pos[1],pos[2]);
}

void CEnvironment::LightPos(int n, float x,float y,float z)
{
	m_lightPos[n][0] = x;
	m_lightPos[n][1] = y;
	m_lightPos[n][2] = z;
	m_bUpdate = TRUE;
}


float CEnvironment::LightFunction(float t, float s)
{
	float u;
	u = t/s;
	if(u>1) u = 1;
	else if(u<0) u = 0;
	// u goes from 0 to 1
	if(u<0.9)
		if(!(rand()%2)) u = 0;

	return u;
}


BOOL CEnvironment::ShadowCast(M3d_vtx pos, M3d_vtx shadowPos, float zPlane,int lightIdx, float &atten)
{
	M3d_vtx lightPos;
	float fi,fo;
	if(lightIdx>=3) return FALSE;
	if(!m_bLightOn[lightIdx]) return FALSE;

	// should be tied to nice positions! - do that when we get the pool table 'lowe for now
	lightPos[0] = m_lightPos[lightIdx][0];//-1500+lightIdx*1500;
	lightPos[1] = m_lightPos[lightIdx][1];//0;
	lightPos[2] = m_lightPos[lightIdx][2];//800;
	
	
	fi = lightPos[2] - pos[2]; // z difference between light & target object.
	fo = pos[2]-zPlane;			// z difference between target object & plane.

	// fo/fi = +ve scale (%age)

	// get (fo/fi) of vector from light->object, which generates a vector pointing towards the shadow from the object
	shadowPos[0] = (pos[0] - lightPos[0])*fo/fi;
	shadowPos[1] = (pos[1] - lightPos[1])*fo/fi;
	shadowPos[2] = (pos[2] - lightPos[2])*fo/fi;

	atten =	(pos[0]-lightPos[0])*(pos[0]-lightPos[0]);
	atten+=	(pos[1]-lightPos[1])*(pos[1]-lightPos[1]);
	atten+=	(pos[2]-lightPos[2])*(pos[2]-lightPos[2]);

	atten/=((1500*1500)+(800*800))/4;

	if(g_build.gameType == GAMETYPE_SNOOKER)
	{
	shadowPos[0]*=0.45; // Adjust distance x/y scaling
	shadowPos[1]*=0.45;
	}
	else {
	atten*=1.5;
	shadowPos[0]*=0.45; // Adjust distance x/y scaling
	shadowPos[1]*=0.45;
	}


	if(atten<0.0001) atten = 1;
	else atten = 1/atten;

	atten*=atten;
			
	shadowPos[0]+=pos[0];	// add object's original position to it.
	shadowPos[1]+=pos[1];
	shadowPos[2]+=pos[2];
	
	return TRUE;
}

/*
BOOL CEnvironment::ShadowCast(M3d_vtx pos, M3d_vtx shadowPos, float zPlane,int lightIdx, float &atten)
{
	M3d_vtx lightPos;
	float fi,fo;
	if(lightIdx>=3) return FALSE;
	if(!m_bLightOn[lightIdx]) return FALSE;

	// should be tied to nice positions! - do that when we get the pool table 'lowe for now
	lightPos[0] = m_lightPos[lightIdx][0];//-1500+lightIdx*1500;
	lightPos[1] = m_lightPos[lightIdx][1];//0;
	lightPos[2] = m_lightPos[lightIdx][2];//800;
	
	
	fi = lightPos[2] - pos[2]; // z difference between light & target object.
	fo = pos[2]-zPlane;			// z difference between target object & plane.


	// fo/fi = +ve scale (%age)

	// get (fo/fi) of vector from light->object, which generates a vector pointing towards the shadow from the object
	shadowPos[0] = (pos[0] - lightPos[0])*fo/fi;
	shadowPos[1] = (pos[1] - lightPos[1])*fo/fi;
	shadowPos[2] = (pos[2] - lightPos[2])*fo/fi;

	atten =	(pos[0]-lightPos[0])*(pos[0]-lightPos[0]);
	atten+=	(pos[1]-lightPos[1])*(pos[1]-lightPos[1]);
	atten+=	(pos[2]-lightPos[2])*(pos[2]-lightPos[2]);

	atten/=((1500*1500)+(800*800))/4;

	if(g_build.gameType == GAMETYPE_SNOOKER)
	{
	shadowPos[0]*=0.35; // Adjust distance x/y scaling
	shadowPos[1]*=0.35;
	}
	else {
	//atten*=1.5;
	shadowPos[0]*=0.45; // Adjust distance x/y scaling
	shadowPos[1]*=0.45;
	}


	if(atten<0.0001) atten = 1;
	else atten = 1/sqrt(atten);

	atten*=atten*0.15f;
	//atten += 0.2f;
			
	shadowPos[0]+=pos[0];	// add object's original position to it.
	shadowPos[1]+=pos[1];
	shadowPos[2]+=pos[2];
	
	return TRUE;
}
*/


void CEnvironment::Process(void)
{
	
	// handle lighting /////////////////////////////////////////////////////////////////
	if(m_lightState_seq)
	{
	switch(m_lightState)
		{
		case ENV_LIGHTSTATE_OFF:m_bLightOn[0] = FALSE;
								m_bLightOn[1] = FALSE;
								m_bLightOn[2] = FALSE;
								m_lightInt[0] = 0;
								m_lightInt[1] = 0;
								m_lightInt[2] = 0;
								m_lightState_seq = 0;
								break;
		case ENV_LIGHTSTATE_ON:	
								if(g_build.gameType == GAMETYPE_SNOOKER)
									{
									m_bLightOn[0] = TRUE;
									m_bLightOn[1] = TRUE;
									m_bLightOn[2] = TRUE;
									}
								else{
									m_bLightOn[0] = TRUE;
									m_bLightOn[1] = TRUE;
									m_bLightOn[2] = TRUE;
									}
								m_lightInt[0] = 1;
								m_lightInt[1] = 1;
								m_lightInt[2] = 1;
								m_lightState_seq = 0;								
								break;
		case ENV_LIGHTSTATE_TURNON:
			m_lightState_seq++;
			
			#define LIGHTINTERVALP	10
			#define LIGHTINTERVAL	20
			#define LIGHTINTERVALT	25

			if(m_lightState_seq<LIGHTINTERVALP);
			else if(m_lightState_seq<LIGHTINTERVALP+LIGHTINTERVALT)
			{
			m_bLightOn[0]=TRUE;
			m_lightInt[0] = LightFunction(m_lightState_seq-LIGHTINTERVALP,LIGHTINTERVAL);
			} else if(m_lightState_seq<LIGHTINTERVALP+LIGHTINTERVALT*2)
			{
			m_bLightOn[1]=TRUE;	
			m_lightInt[1] = LightFunction(m_lightState_seq-(LIGHTINTERVALP+LIGHTINTERVALT),LIGHTINTERVAL);
			} else if(m_lightState_seq<LIGHTINTERVALP+LIGHTINTERVALT*3)
			{
			m_bLightOn[2]=TRUE;
			m_lightInt[2] = LightFunction(m_lightState_seq-(LIGHTINTERVALP+LIGHTINTERVALT*2),LIGHTINTERVAL);
			} else m_lightState_seq = 0;			
			break;
		} // end switch
	((CGameAppView *)m_pParent)->m_bUpdateTable = TRUE;
	}
}

void CEnvironment::Update(void)
{
	//return;
	M3d_lgt g_M3d_lighta;
	int n;
	float pow = 1.0f;

	if(m_bUpdate) m_bUpdate = FALSE;
	else return;
	
	/*
	////////////////////////////////////////////////////////////////////////////////////
	// 1
	M3d_lightbuild_POINT(&g_M3d_lighta,0,0,0,   0.8f,0.8f,0.8f, 10000,1,0,0);	
	g_M3d_lighta.d3dl.dltType = D3DLIGHT_DIRECTIONAL;
	g_M3d_lighta.d3dl.dvDirection.x = 1;	// Unused for Point Lights [they emit all around].
	g_M3d_lighta.d3dl.dvDirection.y = 0;
	g_M3d_lighta.d3dl.dvDirection.z = -1; // Going downwards VERTICALLY
	
	// Setting up a Light //
	M3d_setlight(0,&g_M3d_lighta);	// #0 Ambient
	M3d_light(0,1);					// (index,on/off)
	*/
	



/*
	for(n=0;n<3;n++)
	{
	pow = m_lightInt[n];
	M3d_vtx lightPos;
	
	lightPos[0] = m_lightPos[n][0];
	lightPos[1] = m_lightPos[n][1];
	lightPos[2] = m_lightPos[n][2];
	
	if(g_build.gameType == GAMETYPE_SNOOKER)
	//M3d_lightbuild_POINT(&g_M3d_lighta,lightPos[0],lightPos[1],lightPos[2],   pow,pow,pow,  4000, 0.9,0.00006,0.0000001);    //1,0.001,0);
	  M3d_lightbuild_POINT(&g_M3d_lighta,lightPos[0],lightPos[1],lightPos[2],   pow,pow,pow*0.8,  4000, 0.9,0.00006,0.00000008);    //1,0.001,0);
	else if(g_build.gameType == GAMETYPE_POOL)
	M3d_lightbuild_POINT(&g_M3d_lighta,lightPos[0],lightPos[1],lightPos[2],   pow,pow,pow,  5000, 1.2,0.0005,0);    //1,0.001,0);

	// Setting up a Light //
	M3d_setlight(n+1,&g_M3d_lighta);

	if(m_bLightOn[n]) 
	M3d_light(n+1,1);		// Switch Light on
	else M3d_light(n+1,0); // Switch Light off
	}

*/


	for(n=0;n<3;n++)
	{
		pow = m_lightInt[n];
		M3d_vtx lightPos;
		
		lightPos[0] = m_lightPos[n][0];
		lightPos[1] = m_lightPos[n][1];
		lightPos[2] = m_lightPos[n][2];
		
		if(g_build.gameType == GAMETYPE_SNOOKER)
		//M3d_lightbuild_POINT(&g_M3d_lighta,lightPos[0],lightPos[1],lightPos[2],   pow,pow,pow,      4000, 0.9,0.00006,0.0000001);    //1,0.001,0);
		//M3d_lightbuild_POINT(&g_M3d_lighta,lightPos[0],lightPos[1],lightPos[2],   pow,pow,pow*0.8,  4000, 0.9,0.00006,0.00000008);    //1,0.001,0);
		//M3d_lightbuild_POINT(&g_M3d_lighta,lightPos[0],lightPos[1],lightPos[2],   pow,pow,pow*0.8,  5000, 1.2,0.00006,0.00000008);    //1,0.001,0);
		//M3d_lightbuild_POINT(&g_M3d_lighta,lightPos[0],lightPos[1],lightPos[2],   pow,pow,pow*0.8,  5000, 1.0,0.00006,0.0000001);    //1,0.001,0);
		  M3d_lightbuild_POINT(&g_M3d_lighta,lightPos[0],lightPos[1],lightPos[2],   pow,pow,pow*0.8,  5000, 0.9,0.00006,0.00000013);    //1,0.001,0);
			
		else if(g_build.gameType == GAMETYPE_POOL)
			//M3d_lightbuild_POINT(&g_M3d_lighta,lightPos[0],lightPos[1],lightPos[2],   pow*0.4f,pow*0.4f,pow*0.4f,  4000, 0.3,0.00006,0.00000008);    //1,0.001,0);
			M3d_lightbuild_POINT(&g_M3d_lighta,lightPos[0],lightPos[1],lightPos[2],   pow,pow,pow,  5000, 1.2,0.0005,0);    //1,0.001,0);
			
			

		// Setting up a Light //
		M3d_setlight(n+1,&g_M3d_lighta);

		if(m_bLightOn[n]) 
		{
			M3d_light(n+1,1);		// Switch Light on
		}
		else 
		{
			M3d_light(n+1,0); // Switch Light off
		}
	}




/*
	// Set Additional Light for Seat Area //
	
	pow = m_lightInt[0];
	M3d_lightbuild_POINT(&g_M3d_lighta, -4000, 800, 0, pow,pow,pow,  4000, 0.9,0.00006,0.0000001);    //1,0.001,0);
	// Setting up a Light //
	M3d_setlight(0,&g_M3d_lighta);
	M3d_light(0,1);		// Switch Light on
	M3d_specular(1);
	//M3d_fog(0);
*/	
}