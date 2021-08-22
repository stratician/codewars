// CSnookerAI

#include <math.h>

#include "../CodeSupp/stdafx.h"
#include "../CodeSupp/GameAppView.h"
//#include "../CodeSupp/Vector.h"

#include "Common.h"
#include "Snooker.h"
#include "SnookerAI.h"

CSnookerAI::CSnookerAI(CSnooker *pParent)
{
	m_pParent = pParent;
}

CSnookerAI::~CSnookerAI()
{

}

void CSnookerAI::Process(void)
{	
	return;
	int n;
	int p;
	CVector posObject;
	CVector posWhite;
	CVector posPocket;
	CVector vecWhite;

	posWhite = m_pParent->m_ball[WHITE]->m_P;

	// Go through all active balls (pot-able)
	for(n=0;n<MAX_TOTALBALLS;n++)
	if(TestFlag(m_pParent->m_ball[n]->m_flags, Flg_BALL_ACTIVE))
	{
		if(n!=RED) continue;
		posObject = m_pParent->m_ball[n]->m_P;
		vecWhite = posObject - posWhite;
		// Go through all pot-able pockets
		for(int p=0;p<m_pParent->m_customTable->m_pocket_n;p++)
		{
		posPocket = m_pParent->m_customTable->m_pocketPos[p];
		// Dot the vectors.
		CVector vecObject = posPocket - posObject;
		float forwardYield;
		forwardYield = vecObject.Dot(vecWhite);

		if(forwardYield>0)
			{
			CVector tempPos;
			tempPos = posObject;
			tempPos.z+=BALL_DRAWOFFSETZ+64;
			Debug(tempPos);

			tempPos = posPocket;
			tempPos.z+=BALL_DRAWOFFSETZ+64;
			Debug(tempPos);
			}
		}
	} // end entity scan	
}

void CSnookerAI::Debug(CVector &vPos)
{
/*
	M3d_vtx pos;
	M3d_vtx rot;
	M3d_vtx scale;
	int n;
	float r,g,b;

	n = WHITE;
	//float r = (float)(GetRValue(ballColor[m_ball[n]->m_colour])/255.0f);
	//float g = (float)(GetGValue(ballColor[m_ball[n]->m_colour])/255.0f);
	//float b = (float)(GetBValue(ballColor[m_ball[n]->m_colour])/255.0f);
	
	r = 1; // Magenta
	g = 0;
	b = 1;

	// draw balls GFX
	scale[0] = (BALL_RADIUS/26.09)*0.05f;
	scale[1] = (BALL_RADIUS/26.09)*0.05f;
	scale[2] = (BALL_RADIUS/26.09)*0.05f;	
	rot[0] = 0;
	rot[1] = 0;
	rot[2] = 0;

	// Set up material for ball //
	m_pParent->m_M3d_Ball_ptr->mat[0].d3dm.diffuse.r = r;
	m_pParent->m_M3d_Ball_ptr->mat[0].d3dm.diffuse.g = g;
	m_pParent->m_M3d_Ball_ptr->mat[0].d3dm.diffuse.b = b;
	m_pParent->m_M3d_Ball_ptr->mat[0].d3dm.diffuse.a = 1;

	m_pParent->m_M3d_Ball_ptr->mat[0].d3dm.specular.r = 0.9;
	m_pParent->m_M3d_Ball_ptr->mat[0].d3dm.specular.g = 0.9;
	m_pParent->m_M3d_Ball_ptr->mat[0].d3dm.specular.b = 0.9;
	m_pParent->m_M3d_Ball_ptr->mat[0].d3dm.specular.a = 1.0;
	
	m_pParent->m_M3d_Ball_ptr->mat[0].d3dm.ambient.r = 0.0;
	m_pParent->m_M3d_Ball_ptr->mat[0].d3dm.ambient.g = 0.0;
	m_pParent->m_M3d_Ball_ptr->mat[0].d3dm.ambient.b = 0.0;
	m_pParent->m_M3d_Ball_ptr->mat[0].d3dm.ambient.a = 0.0;

	m_pParent->m_M3d_Ball_ptr->mat[0].d3dm.emissive.r = 0;
	m_pParent->m_M3d_Ball_ptr->mat[0].d3dm.emissive.g = 0;
	m_pParent->m_M3d_Ball_ptr->mat[0].d3dm.emissive.b = 0;
	m_pParent->m_M3d_Ball_ptr->mat[0].d3dm.emissive.a = 1.0;

	m_pParent->m_M3d_Ball_ptr->mat[0].d3dm.power = D3D_MATERIAL_POWER;//70;

	// Set position //
	pos[0] = vPos.x; // (-)
	pos[1] = vPos.y; // (-)
	pos[2] = vPos.z;
	
	M3d_transformobject(pos, rot, scale); // For still (non keyframes) meshes.
	M3d_objectdraw(m_pParent->m_M3d_Ball_ptr);
*/
}

void CSnookerAI::Draw(void)
{	
/*
	return;
	M3d_vtx pos;
	M3d_vtx rot;
	M3d_vtx scale;
	int n;
	float r,g,b;

	n = WHITE;
	//float r = (float)(GetRValue(ballColor[m_ball[n]->m_colour])/255.0f);
	//float g = (float)(GetGValue(ballColor[m_ball[n]->m_colour])/255.0f);
	//float b = (float)(GetBValue(ballColor[m_ball[n]->m_colour])/255.0f);
	
	r = 1; // Magenta
	g = 0;
	b = 1;

	// draw balls GFX
	scale[0] = (BALL_RADIUS/26.09)*0.05f;
	scale[1] = (BALL_RADIUS/26.09)*0.05f;
	scale[2] = (BALL_RADIUS/26.09)*0.05f;	
	rot[0] = 0;
	rot[1] = 0;
	rot[2] = 0;

	// Set up material for ball //
	m_pParent->m_M3d_Ball_ptr->mat[0].d3dm.diffuse.r = r;
	m_pParent->m_M3d_Ball_ptr->mat[0].d3dm.diffuse.g = g;
	m_pParent->m_M3d_Ball_ptr->mat[0].d3dm.diffuse.b = b;
	m_pParent->m_M3d_Ball_ptr->mat[0].d3dm.diffuse.a = 1;

	m_pParent->m_M3d_Ball_ptr->mat[0].d3dm.specular.r = 0.9;
	m_pParent->m_M3d_Ball_ptr->mat[0].d3dm.specular.g = 0.9;
	m_pParent->m_M3d_Ball_ptr->mat[0].d3dm.specular.b = 0.9;
	m_pParent->m_M3d_Ball_ptr->mat[0].d3dm.specular.a = 1.0;
	
	m_pParent->m_M3d_Ball_ptr->mat[0].d3dm.ambient.r = 0.0;
	m_pParent->m_M3d_Ball_ptr->mat[0].d3dm.ambient.g = 0.0;
	m_pParent->m_M3d_Ball_ptr->mat[0].d3dm.ambient.b = 0.0;
	m_pParent->m_M3d_Ball_ptr->mat[0].d3dm.ambient.a = 0.0;

	m_pParent->m_M3d_Ball_ptr->mat[0].d3dm.emissive.r = 0;
	m_pParent->m_M3d_Ball_ptr->mat[0].d3dm.emissive.g = 0;
	m_pParent->m_M3d_Ball_ptr->mat[0].d3dm.emissive.b = 0;
	m_pParent->m_M3d_Ball_ptr->mat[0].d3dm.emissive.a = 1.0;

	m_pParent->m_M3d_Ball_ptr->mat[0].d3dm.power = D3D_MATERIAL_POWER;//70;

	// Set position //
	pos[0] = m_pParent->m_ball[n]->m_P.x; // (-)
	pos[1] = m_pParent->m_ball[n]->m_P.y; // (-)
	pos[2] = m_pParent->m_ball[n]->m_P.z + BALL_DRAWOFFSETZ+64;
	
	M3d_transformobject(pos, rot, scale); // For still (non keyframes) meshes.
	M3d_objectdraw(m_pParent->m_M3d_Ball_ptr);
*/
}

/*

1) [HIGHSPEED/EASY] Go thru all possible object balls, get vector from white to object ball,
determine angle range that object ball can take (+/-90 from that vector),
see if there is any pockets that fit in this range.

2a) Now determine whether object ball can go in pocket (i.e is it obstructed?, and at what angle cut was required for it, and distance-O), if obstructed, ignore and move on to one of the next pockets avail.

2b) Also determine whether the white ball can hit this object ball, and note the distance-W.

Generate table of "object ball, pocket, angle req'd, distance-W,distance-O, product" - (product) is for a rating the difficulty of the shot.

*3) Of current shots, plan the best shot (speed/spin) to achieve both a pot & leave white ball on a good next target ball.
*4) If really hard shot, or can't pot, then plan for a safety

Finally) Fire best shot.

*/
