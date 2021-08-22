// CSnooker

#include <math.h>

#include "../CodeSupp/build.h"
#include "../CodeSupp/stdafx.h"
#include "../CodeSupp/GameAppView.h"
//#include "../CodeSupp/Vector.h"

#include "Common.h"
#include "Snooker.h"
#include "SnookerAI.h"
#include "Effects.h"
#include "../Codedx9/detect.h"


M3d_object *g_M3d_Ball_ptr;
CSnooker *g_snooker;

M3d_object m_M3d_Cue[MAX_CUES];
M3d_object m_M3d_Pen;
M3d_object m_M3d_Purchased;
M3d_object m_M3d_scene1;
M3d_object m_M3d_stframe;
M3d_object m_M3d_stshadow;
M3d_object m_M3d_Table_fb;


float g_bioAverageShotTime = 15;
float g_bioAveragePosition = 50;
float g_bioAverageSafety = 50;
//float g_bioAveragePot = 50;
// pro backups
#ifdef THE_POOL_CLUB
float g_pbioAverageBreak = 3;
#else
float g_pbioAverageBreak = 25;
#endif
float g_pbioAverageShotTime = 15;
float g_pbioAveragePosition = 50;
float g_pbioAverageSafety = 50;
// semi-pro backups
#ifdef THE_POOL_CLUB
float g_sbioAverageBreak = 3;
#else
float g_sbioAverageBreak = 25;
#endif
float g_sbioAverageShotTime = 15;
float g_sbioAveragePosition = 50;
float g_sbioAverageSafety = 50;

// iSnooker
COLORREF ballColor[MAX_BALLS] = {
RGB(235,239,212),		// WHITE
RGB(162+10,0,0),		// RED
RGB(210,210,16),		// YELLOW
RGB(8,87,27),  // GREEN
RGB(123,83,33),			// BROWN
RGB(20+7,20+8,150+20),	// BLUE
RGB(240,130,130),		// PINK
RGB(40+10,40+10,40+10), // BLACK
};

// iPool
COLORREF poolBallColor[1+15] = {
RGB(235,239,212),		// WHITE
RGB(210,210,16),		// YELLOW (1-Ball)
RGB(20+7,20+8,150+20),	// BLUE   (2-Ball)
RGB(112,0,0),			// RED    (3-Ball)
RGB(255,34,143),		// PINK	  (4-Ball)
RGB(235,80,0),			// ORANGE (5-Ball)
RGB(8+10,87+10,27+10),  // GREEN  (6-Ball)
RGB(123-40,83-40,33-30),// BROWN  (7-ball)
RGB(10,10,10),			// BLACK  (8-ball)
RGB(210,210,16),		// YELLOW (9-Ball)
RGB(20+7,20+8,150+20),	// BLUE   (10-Ball)
RGB(112,0,0),			// RED    (11-Ball)
RGB(255,34,143),		// PINK   (12-Ball)
RGB(235,80,0),			// ORANGE (13-Ball)
RGB(8+10,87+10,27+10),  // GREEN  (14-Ball)
RGB(123-40,83-40,33-30),// BROWN  (15-ball)
};

char *CueBallTexturesFName[] = {
"Graphics/Balls/textures/BowlBlue.bmp",
"Graphics/Balls/textures/BowlRed.bmp",
//"Graphics/Balls/textures/woven.bmp",
//"Graphics/Balls/textures/smiley2.bmp",
//"Graphics/Balls/textures/Arrow3D.bmp",
};
	

//// Pool & Snooker Cues //////////
char *cue_lst[] =  {"house",
					"blackwhite",
					"bluelightning",
					"protonpink",
					"purplechrome",
					"wood",
					"dungeon",
					"darkknight"};




char *PNFName[] = {
"Graphics/HUD/P1.bmp",
"Graphics/HUD/P2.bmp",
};

char *PoolBallFName[] = {
"Graphics/Balls/1Ball.bmp",
"Graphics/Balls/2Ball.bmp",
"Graphics/Balls/3Ball.bmp",
"Graphics/Balls/4Ball.bmp",
"Graphics/Balls/5Ball.bmp",
"Graphics/Balls/6Ball.bmp",
"Graphics/Balls/7Ball.bmp",
"Graphics/Balls/8Ball.bmp",
"Graphics/Balls/9Ball.bmp",
"Graphics/Balls/10Ball.bmp",
"Graphics/Balls/11Ball.bmp",
"Graphics/Balls/12Ball.bmp",
"Graphics/Balls/13Ball.bmp",
"Graphics/Balls/14Ball.bmp",
"Graphics/Balls/15Ball.bmp",

"Graphics/Balls/BowlBlue.bmp",
"Graphics/Balls/BowlRed.bmp",
"Graphics/Balls/uk_y_Ball.bmp",
"Graphics/Balls/uk_r_Ball.bmp",
"Graphics/Balls/b_1_Ball.bmp",
"Graphics/Balls/b_3_Ball.bmp",
"Graphics/Balls/b_5_Ball.bmp",
"Graphics/Balls/b_7_Ball.bmp",
};

#ifdef LOAD_NEXTBALLLIST
char *PoolBall16FName[] = {
"Graphics/Balls/1Ball24.bmp",
"Graphics/Balls/2Ball24.bmp",
"Graphics/Balls/3Ball24.bmp",
"Graphics/Balls/4Ball24.bmp",
"Graphics/Balls/5Ball24.bmp",
"Graphics/Balls/6Ball24.bmp",
"Graphics/Balls/7Ball24.bmp",
"Graphics/Balls/8Ball24.bmp",
"Graphics/Balls/9Ball24.bmp",
"Graphics/Balls/10Ball24.bmp",
"Graphics/Balls/11Ball24.bmp",
"Graphics/Balls/12Ball24.bmp",
"Graphics/Balls/13Ball24.bmp",
"Graphics/Balls/14Ball24.bmp",
"Graphics/Balls/15Ball24.bmp",
"Graphics/Balls/uk_y_Ball24.bmp",
"Graphics/Balls/uk_r_Ball24.bmp",
"Graphics/Balls/b_1_Ball24.bmp",
"Graphics/Balls/b_3_Ball24.bmp",
"Graphics/Balls/b_5_Ball24.bmp",
"Graphics/Balls/b_7_Ball24.bmp",
};
#endif //LOAD_NEXTBALLLIST

// 8-ball rerack code
// 1 => stripes
// 0 => spots
BYTE rackOrder[] = {
	1,
   0,1,
  1,0,0,	
 0,1,0,1,
1,0,1,0,0,
};


CVector colourOnDefaultSpots[] = {
CVector(0,0,0),								// WHITE
CVector(0,0,0),								// RED
CVector(WORLD_X(912),WORLD_Y(199+12),0),	// YELLOW SPOT
CVector(WORLD_X(912),WORLD_Y(395-12),0),	// GREEN SPOT
CVector(WORLD_X(912),WORLD_Y(297),0),		// BROWN SPOT
CVector(WORLD_X(591),WORLD_Y(297),0),		// BLUE SPOT
CVector(WORLD_X(331+10),WORLD_Y(297),0),	// PINK SPOT
CVector(WORLD_X(175-10),WORLD_Y(297),0),	// BLACK SPOT
};

/*
int _ballRollOffValues[50] = {
	0, -4, 2, 7, 9, -1, 3, -7, -6, 2,
	-2, -1, 0, -5, 3, -9, 7, 2, -1, -3,
	8, -6, 7, 3, 2, -1, 3, 8, -4, 0,
	1, -1, 2, 9, 2, -1, 3, -5, 2, 0,
	8, -8, 3, 1, 2, -6, 8, -3, -2, 0,	
};
*/
int _ballRollOffValues[50] = {
	8, -7, 9, 7, 9, -9, 7, -7, -6, 7,
	-8, -9, 7, -5, 7, -9, 7, 9, -6, -8,
	8, -6, 7, 9, 7, -8, 9, 8, -6, 9,
	7, -8, 7, 9, 7, -9, 8, -7, 9, 7,
	8, -8, 9, 9, 7, -7, 8, -7, -9, 9,	
};


///////////////////////////////////////////////
// ->CUBE

#define CUBE_FACE_HEIGHT		100.0f

Vertex cube_vtx[20];
Vertex plate_vtx[20];
Vertex tri_lvtx[20];
Vertex utri_lvtx[20];
Vertex uplate_lvtx[20];

/*
Vertex cube_vtx[] = {
// ------------------------------------ //
// Bottom
// TRI 1
{50,50,-50,     0,0,0,			1,1, 0, 0},
{50,-50,-50,    0,0,0,			1,0, 0, 0},
{-50,-50,-50,   0,0,0,			0,0, 0, 0},
// TRI 2
{-50,-50,-50,   0,0,0,			0,0, 0, 0},
{-50,50,-50,    0,0,0,			0,1, 0, 0},
{50,50,-50,     0,0,0,			1,1, 0, 0},
// ------------------------------------ //
// Top
// TRI 3
{-50,-50,50,   0,0,0,			0,0, 0, 0},
{50,-50,50,    0,0,0,			1,0, 0, 0},
{50,50,50,     0,0,0,			1,1, 0, 0},
// TRI 4
{50,50,50,     0,0,0,			1,1, 0, 0},
{-50,50,50,    0,0,0,			0,1, 0, 0},
{-50,-50,50,   0,0,0,			0,0, 0, 0},
// ------------------------------------ //
// Left
// TRI 5
{-50,50,50,     0,0,0,			1,1, 0, 0},
{-50,50,-50,    0,0,0,			1,0, 0, 0},
{-50,-50,-50,   0,0,0,			0,0, 0, 0},
// TRI 6
{-50,-50,-50,   0,0,0,			0,0, 0, 0},
{-50,-50,50,    0,0,0,			0,1, 0, 0},
{-50,50,50,     0,0,0,			1,1, 0, 0},
// ------------------------------------ //
// Right
// TRI 7
{50,-50,-50,   0,0,0,			0,0, 0, 0},
{50,50,-50,    0,0,0,			1,0, 0, 0},
{50,50,50,     0,0,0,			1,1, 0, 0},
// TRI 8
{50,50,50,     0,0,0,			1,1, 0, 0},
{50,-50,50,    0,0,0,			0,1, 0, 0},
{50,-50,-50,   0,0,0,			0,0, 0, 0},
// ------------------------------------ //
// Up
// TRI 9
{50,-50,50,     0,0,0,			1,1, 0, 0},
{-50,-50,50,    0,0,0,			1,0, 0, 0},
{-50,-50,-50,   0,0,0,			0,0, 0, 0},
// TRI A
{-50,-50,-50,   0,0,0,			0,0, 0, 0},
{50,-50,-50,    0,0,0,			0,1, 0, 0},
{50,-50,50,     0,0,0,			1,1, 0, 0},
// ------------------------------------ //
// Down
// TRI B
{-50,50,-50,   0,0,0,			0,0, 0, 0},
{-50,50,50,    0,0,0,			1,0, 0, 0},
{50,50,50,     0,0,0,			1,1, 0, 0},
// TRI C
{50,50,50,     0,0,0,			1,1, 0, 0},
{50,50,-50,    0,0,0,			0,1, 0, 0},
{-50,50,-50,   0,0,0,			0,0, 0, 0},
// ------------------------------------ //
};

///////////////////////////////////////////////
// ->2D PLATE

Vertex plate_vtx[] = {
// Top
// TRI 3
{-50,-50,50,   0,0,1,			0,0, 0, 0},
{50,-50,50,    0,0,1,			1,0, 0, 0},
{50,50,50,     0,0,1,			1,1, 0, 0},
{-50,50,50,    0,0,1,			0,1, 0, 0},
};

*/
WORD plate_lst[] = {
0,1,2,
2,3,0,
};
/*
Vertex tri_lvtx[] = {
// Top
// TRI 3
{0,-50,50,  0.5,1,  RGBA(1,1,1,0), RGBA(1,1,1,0)},
{50,50,50,  1,0,  RGBA(1,1,1,1), RGBA(1,1,1,1)},
{-50,50,50,  0,0, RGBA(1,1,1,1), RGBA(1,1,1,1)},
};

Vertex utri_lvtx[] = {
// Top
// TRI 3
{0,0,0,  0.5,1,	   RGBA(1,1,1,1), RGBA(1,1,1,1)},
{0.5,1,0,  1,0,  RGBA(1,1,1,1), RGBA(1,1,1,1)},
{-0.5,1,0,  0,0, RGBA(1,1,1,1), RGBA(1,1,1,1)},
};



Vertex uplate_lvtx[] = {
// Top
// TRI 3
{-0.5,0,0,  0,0,  RGBA(1,1,1,1), RGBA(1,1,1,1)},
{0.5,0,0,  1,0,  RGBA(1,1,1,1), RGBA(1,1,1,1)},
{0.5,1,0,  1,1, RGBA(1,1,1,1), RGBA(1,1,1,1)},
{-0.5,1,0,  0,1, RGBA(1,1,1,1), RGBA(1,1,1,1)},
};

*/


// [CObstacle] ///////////////////////////////////////////////////////////////////////////////////////////////////////

CObstacle::CObstacle()
{
m_type = OBSTACLE_NULL;

}

CObstacle::~CObstacle()
{

}

BOOL CObstacle::Detect(void *pData, CVector &wstart, CVector &vect, float radius)
{
	return FALSE;
}

BOOL CObstacle::Detect(void *pData, FVector &wstart, FVector &vect, __fix radius)
{ // Fixed Version.
	return FALSE;
}

void CObstacle::Draw(void)
{

}

// CYLINDER //

CObstacleCYLINDER::CObstacleCYLINDER(CVector &pos, float radius, BOOL bInterior)
{
m_type = OBSTACLE_CYLINDER;
m_pos = pos;
m_radius = radius;
m_bInterior = bInterior;
}

CObstacleCYLINDER::CObstacleCYLINDER(FVector &pos, __fix radius, BOOL bInterior)
{
m_type = OBSTACLE_CYLINDER;
f_pos = pos;
f_radius = radius;
m_bInterior = bInterior;
}

CObstacleCYLINDER::~CObstacleCYLINDER()
{

}

BOOL SolveQuadratic(float &s1, float &s2, float a, float b, float c)
{	// s1 & s2 = (-b +/- sqrt(b^2 - 4ac))/2a (s1 = +ve solution, s2 = -ve solution)
	float f;
	f = b*b - 4*a*c;
	if(f<0) return FALSE; // No roots.
	if(a==0) return FALSE; // Would cause / by 0 and i.e. a NULL vector.
	f = sqrt(f);
	s1 = (-b + f)/(2*a);	// +ve root
	s2 = (-b - f)/(2*a);	// -ve root.	
	return TRUE;
}

BOOL SolveQuadraticFixed(__fix &s1, __fix &s2, __fix a, __fix b, __fix c)
{	// s1 & s2 = (-b +/- sqrt(b^2 - 4ac))/2a (s1 = +ve solution, s2 = -ve solution)
	__fix f;
	f = FIXMUL(b,b) - 4*FIXMUL(a,c);
	if(f<0) return FALSE; // No roots.
	if(a==0) return FALSE; // Would cause / by 0 and i.e. a NULL vector.
	f = TOFIX(sqrt(FROMFIXSMALL(f)));
	s1 = FIXDIV((-b + f),(2*a));	// +ve root
	s2 = FIXDIV((-b - f),(2*a));	// -ve root.
	return TRUE;
}

BOOL CObstacleCYLINDER::Detect(void *pData, CVector &wstart, CVector &vect, float radius)
{ // Detects when ball of radius collides with a cylinder of radius.
	CVector start;
	CVector m_hitpos;
	//float m_lambda;
	start = wstart-m_pos; // get relative start as opposed to world start.

	// Quadratic Equation (find lambda):-
	//	0 =	(start.x)*(start.x)+ 2*start.x*vect.x*lambda + (vect.x*vect.x)*lambda*lambda+
	//		(start.y)*(start.y)+ 2*start.y*vect.y*lambda + (vect.y*vect.y)*lambda*lambda+
	//		(start.z)*(start.z)+ 2*start.z*vect.z*lambda + (vect.z*vect.z)*lambda*lambda - R*R;
	// lambda = (-b +/- sqrt(b^2 - 4ac))/2a

	float sol1,sol2;
	float R2;
	if(m_bInterior) // Interior
	{
	R2 = (m_radius-radius)*(m_radius-radius);
	}
	else R2 = (m_radius+radius)*(m_radius+radius);
//  Sphere obstacle
//	if(!SolveQuadratic(sol1, sol2, (vect.x*vect.x)+(vect.y*vect.y)+(vect.z*vect.z),				// A
//						  (2*start.x*vect.x)+(2*start.y*vect.y)+(2*start.z*vect.z),			// B
//						  (start.x)*(start.x)+(start.y)*(start.y)+(start.z)*(start.z)) - (m_radius+radius) )		// C

// Cylinder obstacle, i.e just kill the Z component
	//if(start.Magnitude()<m_radius+radius) return TRUE;
	//else return FALSE;
	float a,b,c;
	a = (vect.x*vect.x)+(vect.y*vect.y);
	b = (2*start.x*vect.x)+(2*start.y*vect.y);
	c = (start.x*start.x)+(start.y*start.y) - R2;
	
	if(!SolveQuadratic(sol1, sol2, a,b,c))
	return FALSE;

	// Boundary check (lambda must be between 0..1)
	if(m_bInterior) // Interior
	{
	sol2 = sol1;
	}

	if(!(sol2>=0 && sol2<1)) return FALSE;
	
	OBSTACLE_DATASTRUCT *pStruct = (OBSTACLE_DATASTRUCT *)pData;

	pStruct->m_lambda = sol2;
	pStruct->m_hitpos = wstart + pStruct->m_lambda*vect;	// Exact World coordinates of ball when it hit (i.e. Center of ball)

	pStruct->m_normal = start + pStruct->m_lambda*vect;
	pStruct->m_normal.z = 0;
	pStruct->m_normal = pStruct->m_normal.Unit(); // Unit normal vector of surface of cylinder where collision occured. e.g. Impulse!

	pStruct->m_contactpos = pStruct->m_hitpos + pStruct->m_normal*radius; // Exact World coordinates of collision e.g. Spark Point! (Surface/Surface)

	return TRUE;
}



//detect_bp
BOOL CObstacleCYLINDER::Detect(void *pData, FVector &wstart, FVector &vect, __fix radius)
{ // Detects when ball of radius collides with a cylinder of radius.
	
	
	FVector start;
	FVector m_hitpos;


	__fix R2;  // Radius of Cylinder - Ball Radius (so we work with points)

	// Adjust the Working Radius, R accordingly
	if(m_bInterior) R2 = FIXMUL((f_radius-radius),(f_radius-radius));
		else R2 = FIXMUL((f_radius+radius),(f_radius+radius));
	
	// Lets find where our (wstart + Lambda*vect) intersects the Circle //
	// To do this we take the relative distance of (wstart + Lambda*vect) - (f_pos) and Find when Length equals Radius of Cylinder

	// [ (wstart + Lambda*vect) - (f_pos) ]^2  = R
	// [ (wstart-f_pos) + Lambda*vect ]^2  = R

	start = wstart-f_pos; // get relative start as opposed to world start.
	// [ (start + Lambda*vect ]^2  = raius

	

	__fix sol1,sol2;
	__fix a,b,c;

	BOOL bTiny = FALSE;
	FVector tv;
	tv = vect;


  	a = FIXMUL(tv.x,tv.x)+FIXMUL(tv.y,tv.y);
	b = FIXMUL(2*start.x,tv.x)+FIXMUL(2*start.y,tv.y);
	c = FIXMUL(start.x,start.x)+FIXMUL(start.y,start.y) - R2;



	if(!SolveQuadraticFixed(sol1, sol2, a,b,c))
	return FALSE;
	
	// Boundary check (lambda must be between 0..1)
	if(m_bInterior) // Interior
	{
	sol2 = sol1;
	}


	// Must require 0 to 1 multiples of vect to hit edge of object //
	if(sol2<0 || sol2>TOFIX(1)) return FALSE;
	



	OBSTACLE_DATASTRUCTF *pStruct = (OBSTACLE_DATASTRUCTF *)pData;

	pStruct->m_lambda = sol2;
	pStruct->m_hitpos = wstart + pStruct->m_lambda*vect;	// Exact World coordinates of ball when it hit (i.e. Center of ball)

	if(m_bInterior) pStruct->m_normal = f_pos - pStruct->m_hitpos; 
		else pStruct->m_normal = pStruct->m_hitpos - f_pos;
	pStruct->m_normal.z = 0;
	pStruct->m_normal = pStruct->m_normal.Unit(); // Unit normal vector of surface of cylinder where collision occured. e.g. Impulse!

	pStruct->m_contactpos = pStruct->m_hitpos + pStruct->m_normal*radius; // Exact World coordinates of collision e.g. Spark Point! (Surface/Surface)

	return TRUE;
}


void CObstacleCYLINDER::Draw(void)
{
return;
	M3d_vtx pos,rot,scale;

#ifdef FIXEDMODE
	pos[0] = FROMFIXSMALL(f_pos.x);
	pos[1] = FROMFIXSMALL(f_pos.y);
	pos[2] = FROMFIXSMALL(f_pos.z)-50;
	g_snooker->DrawArc(pos, FROMFIXSMALL(f_radius), 0, 360, RGBA(100,100,100, 66), 0.025f, 16);
#else
	pos[0] = m_pos.x;
	pos[1] = m_pos.y;
	pos[2] = m_pos.z-50;
	g_snooker->DrawArc(pos, m_radius, 0, 360, RGBA(100,100,100, 66), 0.025f, 16);
#endif
}

// PLANE //

CObstaclePLANE::CObstaclePLANE(CVector &start, CVector &end, BOOL bDoubleSided)
{ // R.H.S of vector/plane is solid.
m_type = OBSTACLE_PLANE;
m_start = start;
m_vect = end-start;
m_normal = m_vect.Cross(CVector(0,0,-1)); // Right side
m_normal = m_normal.Unit();
m_bDoubleSided = bDoubleSided;
m_length = m_vect.Magnitude();
m_vect = m_vect/m_length;
}

CObstaclePLANE::CObstaclePLANE(FVector &start, FVector &end, BOOL bDoubleSided)
{ // R.H.S of vector/plane is solid.
m_type = OBSTACLE_PLANE;
f_start = start;
f_vect = end-start;
f_normal = f_vect.Cross(FVector(0,0,TOFIX(-1))); // Right side
f_normal = f_normal.Unit();
m_bDoubleSided = bDoubleSided;
//f_length = f_vect.Magnitude();
float a,b,c;
a = FROMFIXSMALL(f_vect.x);
b = FROMFIXSMALL(f_vect.y);
c = FROMFIXSMALL(f_vect.z);

f_length = TOFIX(sqrt(a*a+b*b+c*c));

f_vect = f_vect/f_length;

//f_start.x-=TOFIX(400);
//f_start.y-=TOFIX(400);
}

CObstaclePLANE::~CObstaclePLANE()
{
}

BOOL CObstaclePLANE::Detect(void *pData, CVector &wstart, CVector &vect, float radius)
{ // Detects when ball of radius collides with a cylinder of radius.
	BOOL success;

	
	success = DetectSide(pData, wstart, vect, radius, FALSE); // Front side
	if(success) return TRUE;
	if(m_bDoubleSided)
	{
	success = DetectSide(pData, wstart, vect, radius, TRUE); // Back side
	if(success) return TRUE;
	}
	return FALSE;
}

BOOL CObstaclePLANE::Detect(void *pData, FVector &wstart, FVector &vect, __fix radius)
{ // Detects when ball of radius collides with a cylinder of radius.
	BOOL success;
	success = DetectSide(pData, wstart, vect, radius, FALSE); // Front side
	if(success) return TRUE;
	if(m_bDoubleSided)
	{
	success = DetectSide(pData, wstart, vect, radius, TRUE); // Back side
	if(success) return TRUE;
	}
	return FALSE;
}

BOOL CObstaclePLANE::DetectSide(void *pData, CVector &wstart, CVector &vect, float radius, BOOL bSide)
{ // Detects when ball of radius collides with a cylinder of radius.
	CVector start;
	CVector start_p;
	CVector m_hitpos;
	CVector normal;
	float inwardP;
	float inwardV;
	float inwardF;
	BOOL crossed;
	int n;


	start = wstart-m_start; // get relative start as opposed to world start.
	start_p = start;

	
	if(bSide) normal = m_normal * -1; // -ve normal
	else normal = m_normal;

	start.z = 0;
	normal.z = 0;

	
	start = start-(normal*radius); // move test point to surface of ball which is closest to plane.
	
	inwardP = start.Dot(normal); // inward position (1 dimensional)
	inwardV = vect.Dot(normal); // inward vector (1 dimensional)
	inwardF = inwardP+inwardV;


	crossed = FALSE;
	
	if(inwardP>=0 && inwardF<0) crossed = TRUE; // Crossed the plane (from +ve to -ve)
				
	
	if(!crossed){
				CVector end;
				end = (m_start+(m_vect*m_length));
				if(DetectPoint(pData, m_start, wstart, vect, radius)) return TRUE;
				if(DetectPoint(pData, end, wstart, vect, radius)) return TRUE;
				return FALSE;
				}

	// boundary checking //
	CVector hitpos;
	float lambda;
	float forwardP;
	lambda = inwardP/inwardV; // value from 0-1
	lambda = -lambda;

	hitpos = start+(vect*lambda); // <== Doesn't work due to some bug (i.e. vect * scalar)
	hitpos.z = 0;
	forwardP = m_vect.Dot(hitpos);


	
	float delta = 0.01;
	
	if(forwardP<0)	{
					if(DetectPoint(pData, m_start, wstart, vect, radius)) return TRUE;
					else return FALSE;
					}
	if(forwardP>m_length) 
					{
					//return FALSE;
					CVector end;
					end = (m_start+m_vect*m_length);
					if(DetectPoint(pData, end, wstart, vect, radius)) return TRUE;
					else return FALSE;
					}
	

/*
	sprintf(buffer,"INSIDE [%f,%f,%f] - Foward:%f, m_length:%f, g_ballRadius:%f",hitpos.x,hitpos.y,hitpos.z, forwardP, m_length, g_ballRadius);
	ErrorMessage(g_gameAppView, buffer);
*/

	// inwardP to inwardP+inwardV should cross from +ve to -ve across the plane.
	// hence the plane is crossed, then check lambda of that, and boundaries.

	OBSTACLE_DATASTRUCT *pStruct = (OBSTACLE_DATASTRUCT *)pData;

	pStruct->m_lambda = lambda; // value from 0-1
	pStruct->m_hitpos = wstart + pStruct->m_lambda*vect;	// Exact World coordinates of ball when it hit (i.e. Center of ball)
	pStruct->m_normal = normal;
	pStruct->m_contactpos = pStruct->m_hitpos + pStruct->m_normal*radius; // Exact World coordinates of collision e.g. Spark Point! (Surface/Surface)
		
	return TRUE;
}

BOOL CObstaclePLANE::DetectSide(void *pData, FVector &wstart, FVector &vect, __fix radius, BOOL bSide)
{ // Detects when ball of radius collides with a cylinder of radius.
	//return FALSE;
	
	FVector start;
	FVector m_hitpos;
	FVector normal;
	__fix inwardP;
	__fix inwardV;
	__fix inwardF;
	BOOL crossed;
	int n;

	start = wstart-f_start; // get relative start as opposed to world start.
	
	if(bSide) normal = f_normal * TOFIX(-1); // -ve normal
	else normal = f_normal;

	start.z = 0;
	normal.z = 0;

	start = start-(normal*radius); // move test point to surface of ball which is closest to plane.

	inwardP = start.Dot(normal); // inward position (1 dimensional)
	inwardV = vect.Dot(normal); // inward vector (1 dimensional)
	inwardF = inwardP+inwardV;

	crossed = FALSE;

	if(inwardP>=0 && inwardF<0) crossed = TRUE; // Crossed the plane (from +ve to -ve)
		
	if(!crossed) {		
				if(DetectPoint(pData, f_start, wstart, vect, radius)) return TRUE;
								
				FVector end;
				end = (f_start+(f_vect*f_length));
				return DetectPoint(pData, end, wstart, vect, radius);
				}

		
	// boundary checking //
	//char buffer[80];
	//float a,b,c,d,e,f;
	FVector hitpos;
	__fix lambda;
	__fix forwardP;
	lambda = FIXDIV(inwardP,inwardV); // value from 0-1
	lambda = -lambda;

	//a = FROMFIXSMALL(inwardP);
	//b = FROMFIXSMALL(inwardV);
	//c = FROMFIXSMALL(lambda);
	
	//sprintf(buffer,"{%f}%f{%f}",a,b,c);
	//ErrorMessage((CGameAppView *)g_wnd,buffer);


	hitpos = start+(vect*lambda); // <== Doesn't work due to some bug (i.e. vect * scalar)
	hitpos.z = 0;

	forwardP = f_vect.Dot(hitpos);
	

	if(forwardP<0) {
					//return FALSE;
					return DetectPoint(pData, f_start, wstart, vect, radius);
					}
	if(forwardP>=f_length) {
							//return FALSE;
							FVector end;
							end = (f_start+f_vect);
							return DetectPoint(pData, end, wstart, vect, radius);
							}


	// inwardP to inwardP+inwardV should cross from +ve to -ve across the plane.
	// hence the plane is crossed, then check lambda of that, and boundaries.

	OBSTACLE_DATASTRUCTF *pStruct = (OBSTACLE_DATASTRUCTF *)pData;
	
	pStruct->m_lambda = lambda; // value from 0-1
	pStruct->m_hitpos = wstart + pStruct->m_lambda*vect;	// Exact World coordinates of ball when it hit (i.e. Center of ball)
	pStruct->m_normal = normal;
	pStruct->m_contactpos = pStruct->m_hitpos + pStruct->m_normal*radius; // Exact World coordinates of collision e.g. Spark Point! (Surface/Surface)
		
	return TRUE;
}

BOOL CObstaclePLANE::DetectPoint(void *pData, CVector &p1, CVector &wstart, CVector &vect, float radius)
{
	CVector start;
	start = wstart-p1; // relative to p1.
	//(lambda*vect.x+start.x)*(lambda*vect.x+start.x)

	// 0 = lambda*lambda*(vect.x*vect.x) + lambda*(2*vect.x*start.x) + (start.x*start.x) - R*R
	float sol1,sol2;
	float R2;
	R2 = (radius)*(radius);
	//R2 *=0.95;
	float a,b,c;
	a = (vect.x*vect.x)+(vect.y*vect.y);
	b = (2*start.x*vect.x)+(2*start.y*vect.y);
	c = (start.x*start.x)+(start.y*start.y) - R2;
	

	if(!SolveQuadratic(sol1, sol2, a,b,c))
	return FALSE;

	// Boundary check (lambda must be between 0..1)
	if(!(sol2>=0 && sol2<=1)) return FALSE;
	
	
	OBSTACLE_DATASTRUCT *pStruct = (OBSTACLE_DATASTRUCT *)pData;

	pStruct->m_lambda = sol2;
	pStruct->m_hitpos = wstart + pStruct->m_lambda*vect;	// Exact World coordinates of ball when it hit (i.e. Center of ball)
	pStruct->m_normal = start + pStruct->m_lambda*vect;
	pStruct->m_normal.z = 0;
	pStruct->m_normal = pStruct->m_normal.Unit(); // Unit normal vector of surface of cylinder where collision occured. e.g. Impulse!

	pStruct->m_contactpos = pStruct->m_hitpos + pStruct->m_normal*radius; // Exact World coordinates of collision e.g. Spark Point! (Surface/Surface)

	return TRUE;
}

BOOL CObstaclePLANE::DetectPoint(void *pData, FVector &p1, FVector &wstart, FVector &vect, __fix radius)
{
	FVector start;
	int failed;
	start = wstart-p1; // relative to p1.

	// Quick check for OOB (defined as start & end being outside box of side radius*2 from point)
	failed = 0;
	if(start.x>radius || start.x<-radius) failed++;
	else if(start.y>radius || start.y<-radius) failed++;

	if(start.x+vect.x>radius || start.x+vect.x<-radius) failed++;
	else if(start.y+vect.y>radius || start.y+vect.y<-radius) failed++;

	if(failed>=2) return FALSE;

	//(lambda*vect.x+start.x)*(lambda*vect.x+start.x)

	// 0 = lambda*lambda*(vect.x*vect.x) + lambda*(2*vect.x*start.x) + (start.x*start.x) - R*R
	__fix sol1,sol2;
	__fix R2;
	R2 = FIXMUL(radius,radius);
	__fix a,b,c;

//	a = FIXMUL(vect.x,vect.x)+FIXMUL(vect.y,vect.y);
//	b = FIXMUL(2*start.x,vect.x)+FIXMUL(2*start.y,vect.y);
//	c = FIXMUL(start.x,start.x)+FIXMUL(start.y,start.y) - R2;

	BOOL bTiny = FALSE;
	FVector tv;
	tv = vect;

	if(tv.x<TOFIX(0.1f) && tv.x>TOFIX(-0.1f))
	if(tv.y<TOFIX(0.1f) && tv.y>TOFIX(-0.1f))
	{
	bTiny = TRUE;
	}

	if(bTiny)
	{
	tv.x<<=4;
	tv.y<<=4;
	tv.z<<=4;
	}

	a = FIXMUL(tv.x,tv.x)+FIXMUL(tv.y,tv.y);
	b = FIXMUL(2*start.x,tv.x)+FIXMUL(2*start.y,tv.y);
	c = FIXMUL(start.x,start.x)+FIXMUL(start.y,start.y) - R2;
		
	if(!SolveQuadraticFixed(sol1, sol2, a,b,c))
	return FALSE;

	if(bTiny)
	{
	sol2>>=4;
	}

	// Boundary check (lambda must be between 0..1)
	if(!(sol2>=0 && sol2<TOFIX(1))) return FALSE;

	OBSTACLE_DATASTRUCTF *pStruct = (OBSTACLE_DATASTRUCTF *)pData;

	pStruct->m_lambda = sol2;
	pStruct->m_hitpos = wstart + pStruct->m_lambda*vect;	// Exact World coordinates of ball when it hit (i.e. Center of ball)

	pStruct->m_normal = start + pStruct->m_lambda*vect;
	pStruct->m_normal.z = 0;
	pStruct->m_normal = pStruct->m_normal.Unit(); // Unit normal vector of surface of cylinder where collision occured. e.g. Impulse!

	pStruct->m_contactpos = pStruct->m_hitpos + pStruct->m_normal*radius; // Exact World coordinates of collision e.g. Spark Point! (Surface/Surface)

	return TRUE;
}

void CObstaclePLANE::Draw(void)
{
return;
	M3d_vtx pos,rot,scale;
	int n;
	float thickness = 2.0f;

#ifdef FIXEDMODE
	__fix l;
	l = FIXMUL(f_length,TOFIX(0.5));
	pos[0] = FROMFIXSMALL(f_start.x+FIXMUL(f_vect.x,l));
	pos[1] = FROMFIXSMALL(f_start.y+FIXMUL(f_vect.y,l));
	pos[2] = FROMFIXSMALL(f_start.z+FIXMUL(f_vect.z,l))+1;
		
	rot[0] = 0;
	rot[1] = 0;
	rot[2] = atan2(FROMFIXSMALL(f_vect.y),FROMFIXSMALL(f_vect.x));

	scale[0] = 0.01*FROMFIXSMALL(f_length);
	scale[1] = 0.01*thickness;
	scale[2] = 0;
#else
	pos[0] = m_start.x+m_vect.x*0.5*m_length;
	pos[1] = m_start.y+m_vect.y*0.5*m_length;
	pos[2] = m_start.z+m_vect.z*0.5*m_length+1;
		
	rot[0] = 0;
	rot[1] = 0;
	rot[2] = atan2(m_vect.y,m_vect.x);

	scale[0] = 0.01*m_length;
	scale[1] = 0.01*thickness;
	scale[2] = 0;
#endif

	//for(n=0;n<8;n++)
	//{
	g_snooker->DrawPlate(pos, rot, scale, RGBA(100,100,100, 66));
	//pos[2]+=40;
	//}
}

// [CObstacleCourse] /////////////////////////////////////////////////////////////////////////////////////////////////

CObstacleCourse::CObstacleCourse()
{
	int n;
	m_obstaclelst_n = 1024; // 4k to hold all these obstacles
	m_obstaclelst = new CObstacle*[m_obstaclelst_n];

	for(n=0;n<m_obstaclelst_n;n++)
	m_obstaclelst[n] = NULL;
}

CObstacleCourse::~CObstacleCourse()
{
	int n;
	for(n=0;n<m_obstaclelst_n;n++)
	if(m_obstaclelst[n]) delete m_obstaclelst[n];

	delete m_obstaclelst;
}

int CObstacleCourse::AttachObstacle(CObstacle *pObstacle)
{
	int n;
	for(n=0;n<m_obstaclelst_n;n++)
	if(!m_obstaclelst[n])
	{
	m_obstaclelst[n] = pObstacle;
	return n;
	}
	return -1; // couldn't add obstacle
}

int CObstacleCourse::Detect(void *pData, CVector &wstart, CVector &vect, float radius)
{
	OBSTACLE_DATASTRUCT *pTempStruct;
	float distance;
	float distance_best;
	int idx_best;
	CVector m_contactpos_best;
	CVector m_hitpos_best;
	float m_lambda_best;
	CVector m_normal_best;


	idx_best = -1;

	int n;
	for(n=0;n<m_obstaclelst_n;n++)
	if(m_obstaclelst[n])
	{
	if(m_obstaclelst[n]->Detect(pData, wstart, vect, radius)) 
		{
		// Get the Distance of the Collision //
		pTempStruct = (OBSTACLE_DATASTRUCT *)pData;
		CVector tmp = pTempStruct->m_hitpos;
		tmp = wstart - tmp;
		distance = tmp.Magnitude();
		if(idx_best == -1) 
			{
			distance_best = distance;
			m_contactpos_best = pTempStruct->m_contactpos;
			m_hitpos_best = pTempStruct->m_hitpos;
			m_lambda_best = pTempStruct->m_lambda;
			m_normal_best=  pTempStruct->m_normal;
			idx_best = n;
			}
		else{
			if(distance < distance_best)
				{
				distance_best = distance;
				m_contactpos_best = pTempStruct->m_contactpos;
				m_hitpos_best = pTempStruct->m_hitpos;
				m_lambda_best = pTempStruct->m_lambda;
				m_normal_best=  pTempStruct->m_normal;
				idx_best = n;
				}
			}	
		//return n;
		}
	}

	if(idx_best != -1)
		{
		pTempStruct = (OBSTACLE_DATASTRUCT *)pData;
		pTempStruct->m_contactpos = m_contactpos_best;
		pTempStruct->m_hitpos = m_hitpos_best;
		pTempStruct->m_lambda = m_lambda_best;
		pTempStruct->m_normal = m_normal_best;

		return idx_best;
		}

	return -1;
}


int CObstacleCourse::Detect(void *pData, FVector &wstart, FVector &vect, __fix radius)
{
	OBSTACLE_DATASTRUCTF *pTempStruct;
	__fix distance;
	__fix distance_best;
	int idx_best;
	FVector m_contactpos_best;
	FVector m_hitpos_best;
	__fix m_lambda_best;
	FVector m_normal_best;


	idx_best = -1;
	
	int n;
	for(n=0;n<m_obstaclelst_n;n++)
	if(m_obstaclelst[n])
	{
	if(m_obstaclelst[n]->Detect(pData, wstart, vect, radius)) 
	{ 
		// Get the Distance of the Collision //
		pTempStruct = (OBSTACLE_DATASTRUCTF *)pData;
		FVector tmp = pTempStruct->m_hitpos;
		tmp = wstart - tmp;
		distance = tmp.Magnitude();
		if(idx_best == -1) 
			{
			distance_best = distance;
			m_contactpos_best = pTempStruct->m_contactpos;
			m_hitpos_best = pTempStruct->m_hitpos;
			m_lambda_best = pTempStruct->m_lambda;
			m_normal_best=  pTempStruct->m_normal;
			idx_best = n;
			}
		else{
			if(distance < distance_best)
				{
				distance_best = distance;
				m_contactpos_best = pTempStruct->m_contactpos;
				m_hitpos_best = pTempStruct->m_hitpos;
				m_lambda_best = pTempStruct->m_lambda;
				m_normal_best=  pTempStruct->m_normal;
				idx_best = n;
				}
			}	
		//return n;
		}
	}

	if(idx_best != -1)
		{
		pTempStruct = (OBSTACLE_DATASTRUCTF *)pData;
		pTempStruct->m_contactpos = m_contactpos_best;
		pTempStruct->m_hitpos = m_hitpos_best;
		pTempStruct->m_lambda = m_lambda_best;
		pTempStruct->m_normal = m_normal_best;

		return idx_best;
		}

	

	return -1;
}


/*
int CObstacleCourse::Detect(void *pData, FVector &wstart, FVector &vect, __fix radius)
{
	int n;
	for(n=0;n<m_obstaclelst_n;n++)
	if(m_obstaclelst[n])
	{
	if(m_obstaclelst[n]->Detect(pData, wstart, vect, radius)) 
		{
		return n;
		}
	}
	return -1;
}
*/

void CObstacleCourse::Draw(void)
{
	//return;
	int n;
	for(n=0;n<m_obstaclelst_n;n++)
	if(m_obstaclelst[n])
	{
	m_obstaclelst[n]->Draw();
	}
}

// [CCustomTable] ///////////////////////////////////////////////////////////////////////////////////////////////////////

int g_rcyc[8];
int g_idx;
	

CCustomTable::CCustomTable(CSnooker *pParent)
{
	m_pParent = pParent;
	m_type = CTTYPE_CUSTOM;
	m_pocket_max = 16;
	m_pocketPos = new CVector[m_pocket_max];
	m_pocketType = new DWORD[m_pocket_max];
	m_pocketProp = new CPocketProp[m_pocket_max];
	m_pocket_n = 0;
	m_obstacleCourse = NULL;
	RenewObstacleCourse();
	m_ColorSchemeStandard = NULL;
			
	if(g_build.gameType == GAMETYPE_SNOOKER)
		SnookerInit();
	else if(g_build.gameType == GAMETYPE_POOL)
		PoolInit();
	Mmain_log("CCustomTable, Complete");

	g_rcyc[0] = 1; // Start off as 1, so we don't affect Scores if unmuffled //
	g_rcyc[1] = 3;
	g_rcyc[2] = 5;
	g_rcyc[3] = 2;
	g_rcyc[4] = 9;
	g_rcyc[5] = 6;
	g_rcyc[6] = 1;
	g_rcyc[7] = 2;

	g_idx = 0;
	
}

CCustomTable::~CCustomTable()
{
	if(m_pocketPos) delete m_pocketPos;
	if(m_pocketType) delete m_pocketType;
	if(m_pocketProp) delete m_pocketProp;
	if(m_obstacleCourse) delete m_obstacleCourse;
	if(m_ColorSchemeStandard) delete m_ColorSchemeStandard;
}

void CCustomTable::RenewObstacleCourse(void)
{
	if(m_obstacleCourse)
	{
		delete m_obstacleCourse;
		m_obstacleCourse = 0;
	}

	m_obstacleCourse = new CObstacleCourse;
}

void CCustomTable::BuildObstacleCourse(M3d_object *m_M3d_Table_ptr)
{
	int i, n;

#ifdef FIXEDMODE
	M3d_objectfindreset(m_M3d_Table_ptr);
	
	while((i = M3d_objectfind("detect"))!=-1) // woodtop# & frameside#
	{
	ExtractObstacles(&m_M3d_Table_ptr->mesh[i]);
	}

	for(n=0;n<m_pocket_n;n++)
	{
		FVector Pocket;
		__fix r;

		Pocket.x = TOFIX(m_pocketPos[n].x);
		Pocket.y = TOFIX(m_pocketPos[n].y);
		Pocket.z = TOFIX(m_pocketPos[n].z);
		r = TOFIX(m_pocketProp[n].m_suctionRadius);

	m_obstacleCourse->AttachObstacle(new CObstacleCYLINDER(Pocket, r, TRUE));
	}

	
	m_obstacleCourse->AttachObstacle(new CObstaclePLANE(FVector(TOFIX(235*6),TOFIX(-126.5*6),0), FVector(TOFIX(235*6), TOFIX(126.5*6),0), FALSE));
	m_obstacleCourse->AttachObstacle(new CObstaclePLANE(FVector(TOFIX(235*6),TOFIX(126.5*6),0), FVector(TOFIX(-235*6), TOFIX(126.5*6),0), FALSE));
	m_obstacleCourse->AttachObstacle(new CObstaclePLANE(FVector(TOFIX(-235*6),TOFIX(126.5*6),0), FVector(TOFIX(-235*6), TOFIX(-126.5*6),0), FALSE));
	m_obstacleCourse->AttachObstacle(new CObstaclePLANE(FVector(TOFIX(-235*6),TOFIX(-126.5*6),0), FVector(TOFIX(235*6), TOFIX(-126.5*6),0), FALSE));

#else
	M3d_objectfindreset(m_M3d_Table_ptr);
	
	while((i = M3d_objectfind("detect"))!=-1) // woodtop# & frameside#
	{
	ExtractObstacles(&m_M3d_Table_ptr->mesh[i]);					 
	}
	
	m_obstacleCourse->AttachObstacle(new CObstaclePLANE(CVector(1860,-950,0), CVector(1860, 950,0), FALSE));
	m_obstacleCourse->AttachObstacle(new CObstaclePLANE(CVector(1860, 950,0), CVector(-1860, 950,0), FALSE));
	m_obstacleCourse->AttachObstacle(new CObstaclePLANE(CVector(-1860,950,0), CVector(-1860, -950,0), FALSE));
	m_obstacleCourse->AttachObstacle(new CObstaclePLANE(CVector(-1860,-950,0), CVector(1860, -950,0), FALSE));
	
#endif
}

void CCustomTable::DrawObstacleCourse(void)
{
	/*
	//m_obstaclelst[0].m_pos.x
	//m_obstaclelst[0].m_pos.y
	//m_obstaclelst[0].m_pos.z
	float x,y,z,radius;
	x = 0;//m_obstacleCourse->m_obstaclelst[0].m_pos.x;
	y = 0;//m_obstacleCourse->m_obstaclelst[0].m_pos.y;
	z = 0;//m_obstacleCourse->m_obstaclelst[0].m_pos.z;
	radius = 1.0f;//m_obstacleCourse->m_obstaclelst[0].m_radius;
	
	m_pParent->DrawObstacleBall(x,y,z,radius);
	*/
}

BOOL CCustomTable::AddPocket(CVector v, DWORD type, float radius, float suctionRadius, float suctionFactor)
{
	if(m_pocket_n==m_pocket_max) return FALSE; // can't add no more pockets.
	m_pocketPos[m_pocket_n] = v;
	m_pocketType[m_pocket_n] = type;
	m_pocketProp[m_pocket_n].m_radius = radius;
	m_pocketProp[m_pocket_n].m_suctionRadius = suctionRadius;
	m_pocketProp[m_pocket_n].m_suctionFactor = suctionFactor;

	m_pocket_n++;
	return TRUE;
}

void CCustomTable::ClearPockets(void)
{
	m_pocket_n = 0;
	return;
}

BOOL CCustomTable::PocketProximity(int pocketIdx, CVector v, float vRadius)
{
	if(pocketIdx < 0 || pocketIdx > m_pocket_n) return FALSE;
	
	CVector D;
	float d;
	float dSuck;
	D = m_pocketPos[pocketIdx] - v;
	D.z = 0;	
	d = D.Magnitude2();
	dSuck = m_pocketProp[pocketIdx].m_suctionRadius + vRadius; 
	dSuck *= dSuck;
		
	if(d < dSuck) // collided! (i.e. this point is inside this pocket)
	{
		return TRUE;
	}

	return FALSE;
}

// 2006 //

void CCustomTable::SnookerInit(void)
{
	m_type = CTTYPE_SNOOKER;
	m_tableType = 0; // default table type mesh

	// create pockets
	AddPocket(CVector((1500+300+26), (-560-300-26), (0)),  POCKET_BLACKG, BALL_RADIUS*2.4, BALL_RADIUS*3.4, 0.035);		// BLACK-G
	AddPocket(CVector((1500+300+26), (+560+300+26), (0)),  POCKET_BLACKY, BALL_RADIUS*2.4, BALL_RADIUS*3.4, 0.035);		// BLACK-Y
	AddPocket(CVector((0), (-560-300-36-40), (0)), POCKET_BLUEG, BALL_RADIUS*2.4, BALL_RADIUS*3.4, 0.035);			// BLUE-G
	AddPocket(CVector((0), (+560+300+36+40), (0)), POCKET_BLUEY, BALL_RADIUS*2.4, BALL_RADIUS*3.4, 0.035);			// BLUE-Y
	AddPocket(CVector((-1500-300-26), (-560-300-26), (0)), POCKET_GREEN, BALL_RADIUS*2.4, BALL_RADIUS*3.4, 0.035);	// Green
	AddPocket(CVector((-1500-300-26), (+560+300+26), (0)), POCKET_YELLOW, BALL_RADIUS*2.4, BALL_RADIUS*3.4, 0.035);	// Yellow

	// Create Some Color Schemes //
	m_ColorSchemeStandard_n = 8;
	m_ColorSchemeStandard = new CColorScheme[m_ColorSchemeStandard_n];
	int i;
	int n;
	for(n=0;n<m_ColorSchemeStandard_n;n++) // Precreate color schemes
	{
	m_pColorScheme = &m_ColorSchemeStandard[n];
	switch(n)
		{
		case 0:// BLUE //
			   m_pColorScheme->m_tableCushionRed = _256TO1(44);
			   m_pColorScheme->m_tableCushionGreen = _256TO1(58);
			   m_pColorScheme->m_tableCushionBlue = _256TO1(166);
			   m_pColorScheme->m_tableSlateRed = _256TO1(55);
			   m_pColorScheme->m_tableSlateGreen = _256TO1(40);
			   m_pColorScheme->m_tableSlateBlue = _256TO1(178);
			   break;
		case 1:// RED //
			   m_pColorScheme->m_tableCushionRed = _256TO1(144);
			   m_pColorScheme->m_tableCushionGreen = _256TO1(22);
			   m_pColorScheme->m_tableCushionBlue = _256TO1(12);
			   m_pColorScheme->m_tableSlateRed = _256TO1(178);
			   m_pColorScheme->m_tableSlateGreen = _256TO1(70);
			   m_pColorScheme->m_tableSlateBlue = _256TO1(60);
			   break;
		case 2:// GREEN //
			   m_pColorScheme->m_tableCushionRed = _256TO1(4);
			   m_pColorScheme->m_tableCushionGreen = _256TO1(121);
			   m_pColorScheme->m_tableCushionBlue = _256TO1(4);
			   m_pColorScheme->m_tableSlateRed = _256TO1(4);
			   m_pColorScheme->m_tableSlateGreen = _256TO1(131);
			   m_pColorScheme->m_tableSlateBlue = _256TO1(4);
			   break;
		case 3:// BLACK //
			   m_pColorScheme->m_tableCushionRed = 0.17;
			   m_pColorScheme->m_tableCushionGreen = 0.17;
			   m_pColorScheme->m_tableCushionBlue = 0.17;
			   m_pColorScheme->m_tableSlateRed = 0.1459;
			   m_pColorScheme->m_tableSlateGreen = 0.1459;
			   m_pColorScheme->m_tableSlateBlue = 0.1459;
			   break;
		case 4:// YELLOW //
			   m_pColorScheme->m_tableCushionRed = _256TO1(142);
			   m_pColorScheme->m_tableCushionGreen = _256TO1(143);
			   m_pColorScheme->m_tableCushionBlue = _256TO1(29);
			   m_pColorScheme->m_tableSlateRed = _256TO1(126);
			   m_pColorScheme->m_tableSlateGreen = _256TO1(122);
			   m_pColorScheme->m_tableSlateBlue = _256TO1(0);
			   break;
		case 5:// PURPLE //
			   m_pColorScheme->m_tableCushionRed = _256TO1(127);
			   m_pColorScheme->m_tableCushionGreen = _256TO1(45);
			   m_pColorScheme->m_tableCushionBlue = _256TO1(130);
			   m_pColorScheme->m_tableSlateRed = _256TO1(98);
			   m_pColorScheme->m_tableSlateGreen = _256TO1(27);
			   m_pColorScheme->m_tableSlateBlue = _256TO1(104);
			   break;
		case 6:// TEAL //
			   m_pColorScheme->m_tableCushionRed = _256TO1(69);
			   m_pColorScheme->m_tableCushionGreen = _256TO1(149);
			   m_pColorScheme->m_tableCushionBlue = _256TO1(156);
			   m_pColorScheme->m_tableSlateRed = _256TO1(50);
			   m_pColorScheme->m_tableSlateGreen = _256TO1(116);
			   m_pColorScheme->m_tableSlateBlue = _256TO1(133);
			   break;
		case 7:// GREEN2 //
			   m_pColorScheme->m_tableCushionRed = _256TO1(4);
			   m_pColorScheme->m_tableCushionGreen = _256TO1(95);
			   m_pColorScheme->m_tableCushionBlue = _256TO1(4);
			   m_pColorScheme->m_tableSlateRed = _256TO1(4);
			   m_pColorScheme->m_tableSlateGreen = _256TO1(105);
			   m_pColorScheme->m_tableSlateBlue = _256TO1(4);
			   break;
		}
	}
	// ------------------------- //
	
	// Load Snooker Table //
	Mmain_log("CCustomTable, M3d_objectload (m_M3d_Table)");
	//M3d_objectload(&m_M3d_Table,"Graphics/m3d/nntable.m3d", M3D_OBJECT_STILL);

	

	// original snooker table
	#ifndef MATCH_TABLE
	M3d_objectload(&m_M3d_Table,"Graphics/m3d/stable.m3d", M3D_OBJECT_STILL);
	//M3d_objectload(&m_M3d_Table_fa,"Graphics/m3d/stable_fa.m3d", M3D_OBJECT_STILL);
	M3d_objectload(&m_M3d_Table_fb,"Graphics/m3d/stable_fb.m3d", M3D_OBJECT_STILL);
	//M3d_objectload(&m_M3d_Table,"Graphics/m3d/smtable.m3d", M3D_OBJECT_STILL);
	#else
	M3d_objectload(&m_M3d_Table,"Graphics/m3d/mtable.m3d", M3D_OBJECT_STILL);	
	#endif //!MATCH_TABLE
	SnookerTableAuthenticate(&m_M3d_Table, 0); // Check that we are using the correct table model //
	SnookerTableModifyMaterials(&m_M3d_Table);
	M3d_interceptobject_precalc(&m_M3d_Table); // pre-calc table interception
	
	//SnookerTableModifyMaterials(&m_M3d_Table_fa, 1);
	SnookerTableModifyMaterials(&m_M3d_Table_fb, 2);
	

	// BIG snooker table
	M3d_objectload(&m_M3d_bigTable,"Graphics/m3d/stableb.m3d", M3D_OBJECT_STILL);
	SnookerTableAuthenticate(&m_M3d_bigTable, 1);	// Check that we are using the correct table model //
	SnookerTableModifyMaterials(&m_M3d_bigTable);
	M3d_interceptobject_precalc(&m_M3d_bigTable); // pre-calc table interception

	// setup pointer array to tables
	m_M3d_Table_ptr[0] = &m_M3d_Table;
	m_M3d_Table_ptr[1] = &m_M3d_bigTable;
	m_M3d_Table_ptr[2] = 0;
	m_M3d_Table_ptr[3] = 0;
	m_M3d_Table_ptr[4] = 0;
	m_M3d_Table_ptr[5] = 0;
	m_M3d_Table_ptr[6] = 0;
	m_M3d_Table_ptr[7] = 0;
	m_M3d_Table_ptr[8] = 0;
		
	// Load Snooker Balls //
	Mmain_log("CCustomTable, M3d_objectload (m_M3d_Ball[0,1,2])");

	M3d_denormalizationFactor = 0.1;

	/// *NEW* DRAW BALLS 1.5% BIGGER
	M3d_denormalizationFactor *= 1.015f;
	/////////////////

	

	M3d_object *Ball_ptr;
	Ball_ptr = m_M3d_Ball; // Start from first ball
	// LOW BALLS
	m_M3d_Ball_low = Ball_ptr;	
	M3d_objectload(Ball_ptr,"Graphics/m3d/WhiteLow.m3d", M3D_OBJECT_STILL),Ball_ptr++;
	M3d_objectload(Ball_ptr, "Graphics/m3d/balls/isnk/wball_m.m3d", M3D_OBJECT_STILL), Ball_ptr++;
	M3d_objectload(Ball_ptr, "Graphics/m3d/balls/isnk/redball_m.m3d", M3D_OBJECT_STILL), Ball_ptr++;
	M3d_objectload(Ball_ptr, "Graphics/m3d/balls/isnk/yellowball_m.m3d", M3D_OBJECT_STILL), Ball_ptr++;
	M3d_objectload(Ball_ptr, "Graphics/m3d/balls/isnk/greenball_m.m3d", M3D_OBJECT_STILL), Ball_ptr++;
	M3d_objectload(Ball_ptr, "Graphics/m3d/balls/isnk/brownball_m.m3d", M3D_OBJECT_STILL), Ball_ptr++;
	M3d_objectload(Ball_ptr, "Graphics/m3d/balls/isnk/blueball_m.m3d", M3D_OBJECT_STILL), Ball_ptr++;
	M3d_objectload(Ball_ptr, "Graphics/m3d/balls/isnk/pinkball_m.m3d", M3D_OBJECT_STILL), Ball_ptr++;
	M3d_objectload(Ball_ptr, "Graphics/m3d/balls/isnk/blackball_m.m3d", M3D_OBJECT_STILL), Ball_ptr++;
	// MEDIUM BALLS
	m_M3d_Ball_med = Ball_ptr;	
	M3d_objectload(Ball_ptr,"Graphics/m3d/WhiteMed.m3d", M3D_OBJECT_STILL),Ball_ptr++;
	M3d_objectload(Ball_ptr, "Graphics/m3d/balls/isnk/wball_m.m3d", M3D_OBJECT_STILL), Ball_ptr++;
	M3d_objectload(Ball_ptr, "Graphics/m3d/balls/isnk/redball_m.m3d", M3D_OBJECT_STILL), Ball_ptr++;
	M3d_objectload(Ball_ptr, "Graphics/m3d/balls/isnk/yellowball_m.m3d", M3D_OBJECT_STILL), Ball_ptr++;
	M3d_objectload(Ball_ptr, "Graphics/m3d/balls/isnk/greenball_m.m3d", M3D_OBJECT_STILL), Ball_ptr++;
	M3d_objectload(Ball_ptr, "Graphics/m3d/balls/isnk/brownball_m.m3d", M3D_OBJECT_STILL), Ball_ptr++;
	M3d_objectload(Ball_ptr, "Graphics/m3d/balls/isnk/blueball_m.m3d", M3D_OBJECT_STILL), Ball_ptr++;
	M3d_objectload(Ball_ptr, "Graphics/m3d/balls/isnk/pinkball_m.m3d", M3D_OBJECT_STILL), Ball_ptr++;
	M3d_objectload(Ball_ptr, "Graphics/m3d/balls/isnk/blackball_m.m3d", M3D_OBJECT_STILL), Ball_ptr++;
	// HIGH BALLS
	m_M3d_Ball_high = Ball_ptr;	
	M3d_objectload(Ball_ptr,"Graphics/m3d/WhiteHigh.m3d", M3D_OBJECT_STILL),Ball_ptr++;
	M3d_objectload(Ball_ptr, "Graphics/m3d/balls/isnk/wball_l.m3d", M3D_OBJECT_STILL), Ball_ptr++;
	M3d_objectload(Ball_ptr, "Graphics/m3d/balls/isnk/redball_l.m3d", M3D_OBJECT_STILL), Ball_ptr++;
	M3d_objectload(Ball_ptr, "Graphics/m3d/balls/isnk/yellowball_l.m3d", M3D_OBJECT_STILL), Ball_ptr++;
	M3d_objectload(Ball_ptr, "Graphics/m3d/balls/isnk/greenball_l.m3d", M3D_OBJECT_STILL), Ball_ptr++;
	M3d_objectload(Ball_ptr, "Graphics/m3d/balls/isnk/brownball_l.m3d", M3D_OBJECT_STILL), Ball_ptr++;
	M3d_objectload(Ball_ptr, "Graphics/m3d/balls/isnk/blueball_l.m3d", M3D_OBJECT_STILL), Ball_ptr++;
	M3d_objectload(Ball_ptr, "Graphics/m3d/balls/isnk/pinkball_l.m3d", M3D_OBJECT_STILL), Ball_ptr++;
	M3d_objectload(Ball_ptr, "Graphics/m3d/balls/isnk/blackball_l.m3d", M3D_OBJECT_STILL), Ball_ptr++;
	
	M3d_denormalizationFactor = 1;
	m_M3d_Ball_end = Ball_ptr;

	
	// Normalize all balls using Spherical normalization!
	/*M3d_object *Object_ptr;
	Object_ptr = m_M3d_Ball;
	while(Object_ptr!=Ball_ptr)
	{
	M3d_normalizeobjectsphere(Object_ptr);
	Object_ptr++;
	}*/


	// Setup Lighting Position
	((CGameAppView *)m_pParent->m_parent)->m_environment->LightPos(0,-1600,0,800);
	((CGameAppView *)m_pParent->m_parent)->m_environment->LightPos(1,0,0,800);
	((CGameAppView *)m_pParent->m_parent)->m_environment->LightPos(2,+1600,0,800);	


	// Extract lines //
	// BAPDETECT06 //
	//g_bDetectionO = FALSE;
	BuildObstacleCourse(&m_M3d_Table); // for original snooker table (default)
	
}




// 2006 //


// Checks that we are using the Correct Table Model //
// If the user has swapped the table models, make the application crash //
// type 0: Normal Table Model
// type 1: BigPockets Table Model
void CCustomTable::SnookerTableAuthenticate(M3d_object *m_M3d_Table_ptr, char type)
{
#ifndef MAIN_SERVER
	int i;
	int n;
	char hacked;

	switch(type)
		{	
		case 0: // Must be Normal Table Model, or hacking attempt! //
				hacked = 1; // assume we are being hacked, unless Net06_Normal is found //
				for(n=0;n<m_M3d_Table_ptr->mesh_n;n++)
				if(M3d_objectstrcmp(m_M3d_Table_ptr->name[n].str, "Net06_Normal")) 
					{
					hacked = 0; // Found the tag, so this is an autentic model //
					break;
					}
				if(hacked == 1) g_hacked = TRUE;
				break;

		case 1: // Must be BigPocket Table Model, or hacking attempt! //
				hacked = 1; // assume we are being hacked, unless Net06_Normal is found //
				for(n=0;n<m_M3d_Table_ptr->mesh_n;n++)
				if(M3d_objectstrcmp(m_M3d_Table_ptr->name[n].str, "Net06_BigPockets")) 
					{
					hacked = 0;	// Found the tag, so this is an autentic model //
					break;
					}
				if(hacked == 1)	g_hacked = TRUE;	
				break;
		}

#endif //MAIN_SERVER
}





void CCustomTable::SnookerTableModifyMaterials(M3d_object *m_M3d_Table_ptr, char type)
{
	// Modify Table Mesh/Materials/Textures ############################################################################
	int i;
	int n;
	//M3d_object *m_M3d_Table_ptr;
	//m_M3d_Table_ptr = &m_M3d_Table;

	for(n=0;n<m_M3d_Table_ptr->mesh_n;n++)
	if(!M3d_objectstrcmp(m_M3d_Table_ptr->name[n].str, "Net")) // ignore pocket nets
	{
		if(m_M3d_Table_ptr->text_ref[n]>32767);
		else 
		{
			if(m_M3d_Table_ptr->text_type[m_M3d_Table_ptr->text_ref[n]]&M3D_THEAD_MAP_DIFFUSE)
			{
				m_M3d_Table_ptr->mat[n].d3dm.Diffuse.r = 0.8;
				m_M3d_Table_ptr->mat[n].d3dm.Diffuse.g = 0.0;
				m_M3d_Table_ptr->mat[n].d3dm.Diffuse.b = 0.0;
				m_M3d_Table_ptr->mat[n].d3dm.Diffuse.a = 1.0;
			}
		}

			
	m_M3d_Table_ptr->mat[n].d3dm.Specular.r = 0.0f;//3; // 0.2 orig
	m_M3d_Table_ptr->mat[n].d3dm.Specular.g = 0.0f;//3;
	m_M3d_Table_ptr->mat[n].d3dm.Specular.b = 0.0f;//3;
	m_M3d_Table_ptr->mat[n].d3dm.Specular.a = 1.0f;

	m_M3d_Table_ptr->mat[n].d3dm.Emissive.r = 0.0;
	m_M3d_Table_ptr->mat[n].d3dm.Emissive.g = 0.0;
	m_M3d_Table_ptr->mat[n].d3dm.Emissive.b = 0.0;
	m_M3d_Table_ptr->mat[n].d3dm.Emissive.a = 1.0;

	m_M3d_Table_ptr->mat[n].d3dm.Ambient.r = 0.0f;
	m_M3d_Table_ptr->mat[n].d3dm.Ambient.g = 0.0f;
	m_M3d_Table_ptr->mat[n].d3dm.Ambient.b = 0.0f;

	m_M3d_Table_ptr->mat[n].d3dm.Power = D3D_MATERIAL_POWER;
	//m_M3d_Table_ptr->mat[n].d3dm.Ambient.a*=0.2;
	}
	else
	{
		m_M3d_Table_ptr->mat[n].d3dm.Diffuse.r = 0.0;
		m_M3d_Table_ptr->mat[n].d3dm.Diffuse.g = 0.0;
		m_M3d_Table_ptr->mat[n].d3dm.Diffuse.b = 0.0;
		m_M3d_Table_ptr->mat[n].d3dm.Diffuse.a = 1.0;

		m_M3d_Table_ptr->mat[n].d3dm.Specular.r = 0.0;
		m_M3d_Table_ptr->mat[n].d3dm.Specular.g = 0.0;
		m_M3d_Table_ptr->mat[n].d3dm.Specular.b = 0.0;
		m_M3d_Table_ptr->mat[n].d3dm.Specular.a = 1.0;
	}

	
	if(type == 0) M3d_objectfindreset(&m_M3d_Table);
		//else if(type == 1) M3d_objectfindreset(&m_M3d_Table_fa);
		else if(type == 2) M3d_objectfindreset(&m_M3d_Table_fb);
	while((i = M3d_objectfind("CushionSide"))!=-1) // woodtop# & frameside#
	{
	m_M3d_Table_ptr->mat[i].d3dm.Diffuse.r = 0.23f;
	m_M3d_Table_ptr->mat[i].d3dm.Diffuse.g = 0.07f;
	m_M3d_Table_ptr->mat[i].d3dm.Diffuse.b = 0.0f;
	m_M3d_Table_ptr->mat[i].d3dm.Diffuse.a = 1.0f;

	
	m_M3d_Table_ptr->mat[i].d3dm.Ambient.r = 0.0f;
	m_M3d_Table_ptr->mat[i].d3dm.Ambient.g = 0.0f;
	m_M3d_Table_ptr->mat[i].d3dm.Ambient.b = 0.0f;
	m_M3d_Table_ptr->mat[i].d3dm.Ambient.a = 1.0f;
	


	//m_M3d_Table.mat[i].d3dm.Power = 30;
	}
	

	ChangeColorScheme(2, m_M3d_Table_ptr); // [+] Retrieve from a save file
	

	// BLUE //
    // RED //	 (+)
    // GREEN //
    // BLACK //
    // YELLOW //
    // PURPLE //
    // TEAL //

	// #################################################################################################################
}

void CCustomTable::SnookerTableChange(M3d_object *m_M3d_Table_ptr, int type)
{	
	#ifndef THE_POOL_CLUB // iSnooker

	// point snooker table mesh to chosen custom table mesh
	m_pParent->m_M3d_Table_ptr = m_M3d_Table_ptr;
	m_tableType = type;

	// remove all pockets (default)
	ClearPockets();

	// original pool table
	if(m_pParent->m_M3d_Table_ptr == &m_M3d_Table)
	{		
		// create pockets
		AddPocket(CVector((1500+300+26), (-560-300-26), (0)),  POCKET_BLACKG, BALL_RADIUS*2.4, BALL_RADIUS*3.4, 0.035);		// BLACK-G
		AddPocket(CVector((1500+300+26), (+560+300+26), (0)),  POCKET_BLACKY, BALL_RADIUS*2.4, BALL_RADIUS*3.4, 0.035);		// BLACK-Y
		AddPocket(CVector((0), (-560-300-36-40), (0)), POCKET_BLUEG, BALL_RADIUS*2.4, BALL_RADIUS*3.4, 0.035);			// BLUE-G
		AddPocket(CVector((0), (+560+300+36+40), (0)), POCKET_BLUEY, BALL_RADIUS*2.4, BALL_RADIUS*3.4, 0.035);			// BLUE-Y
		AddPocket(CVector((-1500-300-26), (-560-300-26), (0)), POCKET_GREEN, BALL_RADIUS*2.4, BALL_RADIUS*3.4, 0.035);	// Green
		AddPocket(CVector((-1500-300-26), (+560+300+26), (0)), POCKET_YELLOW, BALL_RADIUS*2.4, BALL_RADIUS*3.4, 0.035);	// Yellow
	}
	// BIG Snooker table
	else if(m_pParent->m_M3d_Table_ptr == &m_M3d_bigTable)
	{	
		float f1 = 0.65f;
		float f2 = 0.65f; 
		// create pockets
		AddPocket(CVector((1500+300+26), (-560-300-26), (0)),  POCKET_BLACKG, BALL_RADIUS*2.4*f1, BALL_RADIUS*3.4*f2, 0.035);		// BLACK-G
		AddPocket(CVector((1500+300+26), (+560+300+26), (0)),  POCKET_BLACKY, BALL_RADIUS*2.4*f1, BALL_RADIUS*3.4*f2, 0.035);		// BLACK-Y
		AddPocket(CVector((0), (-560-300-36-40), (0)), POCKET_BLUEG, BALL_RADIUS*2.4*f1, BALL_RADIUS*3.4*f2, 0.035);			// BLUE-G
		AddPocket(CVector((0), (+560+300+36+40), (0)), POCKET_BLUEY, BALL_RADIUS*2.4*f1, BALL_RADIUS*3.4*f2, 0.035);			// BLUE-Y
		AddPocket(CVector((-1500-300-26), (-560-300-26), (0)), POCKET_GREEN, BALL_RADIUS*2.4*f1, BALL_RADIUS*3.4*f2, 0.035);	// Green
		AddPocket(CVector((-1500-300-26), (+560+300+26), (0)), POCKET_YELLOW, BALL_RADIUS*2.4*f1, BALL_RADIUS*3.4*f2, 0.035);	// Yellow
	}

	// Setup Lighting Position
	//((CGameAppView *)m_pParent->m_parent)->m_environment->LightPos(0,-1250,0,600);
	//((CGameAppView *)m_pParent->m_parent)->m_environment->LightPos(1,0,0,600);
	//((CGameAppView *)m_pParent->m_parent)->m_environment->LightPos(2,+1250,0,600);	

		// re-Extract lines //
	RenewObstacleCourse();
	BuildObstacleCourse(m_pParent->m_M3d_Table_ptr);

	#endif //!THE_POOL_CLUB // iSnooker
}

M3d_object *CCustomTable::SnookerTableGetType(int tableTypeIdx)
{
	if(tableTypeIdx < 0) return 0; // error

	return m_M3d_Table_ptr[tableTypeIdx];
}

void M3dX_normalizemesh2(M3d_mesh_ptr mesh) // REF
{ // DEBUGGING FUNCTION
	int n;
	FILE *fptr;
	fptr = fopen("ddd.txt","wb");

	fprintf(fptr,"[%d:%d]",mesh->v,mesh->n);
		
	for(n=0;n<(mesh->v);n++) 
	{
	float x,y,z;
	float a,b,c;
	float dx,dy,dz;
	float f;
	x = mesh->vtx[n].x;
	y = mesh->vtx[n].y;
	z = mesh->vtx[n].z;
		
	f = x*x + y*y + z*z;
	if(f==0) {
			 fprintf(fptr,"%d:***\r\n",n);
			 continue;
			 }
	f = sqrt(1/f);
	x*=f;
	y*=f;
	z*=f;

	a = mesh->vtx[n].nx;
	b = mesh->vtx[n].ny;
	c = mesh->vtx[n].nz;

	fprintf(fptr,"%d:[%f,%f,%f]<->",n,mesh->vtx[n].x,mesh->vtx[n].y,mesh->vtx[n].z);
	fprintf(fptr,"[%f,%f,%f]<->",mesh->vtx[n].nx,mesh->vtx[n].ny,mesh->vtx[n].nz);
	fprintf(fptr,"[%f,%f,%f]",x,y,z);


	x = ((float)((int)(x*100)))*0.01;
	y = ((float)((int)(y*100)))*0.01;
	z = ((float)((int)(z*100)))*0.01;
	a = ((float)((int)(a*100)))*0.01;
	b = ((float)((int)(b*100)))*0.01;
	c = ((float)((int)(c*100)))*0.01;

	if(x==0) x=0.001;
	if(y==0) y=0.001;
	if(z==0) z=0.001;
	if(a==0) a=0.001;
	if(b==0) b=0.001;
	if(c==0) c=0.001;


//	fprintf(fptr,"%d:[%f,%f,%f]<->",n,a,b,c);
//	fprintf(fptr,"[%f,%f,%f]",x,y,z);


	dx = x/a;
	dy = y/b;
	dz = z/c;
	int xxx,yyy,zzz;
	xxx = dx*10;
	yyy = dy*10;
	zzz = dz*10;
	//fprintf(fptr,"---[%f,%f,%f]\r\n",dx,dy,dz);
	fprintf(fptr,"%d---[%d,%d,%d]\r\n",n,xxx,yyy,zzz);
	}
	fclose(fptr);
}

BOOL closeby(float x1,float y1, float z1, float x2,float y2, float z2)
{
	float f;
	f = (x2-x1)*(x2-x1)+(y2-y1)*(y2-y1)+(z2-z1)*(z2-z1);
	if(f<64) return TRUE;
	else return FALSE;
}

void CCustomTable::ExtractObstacles(M3d_mesh *pMesh)
{
	DWORD n;
	DWORD v1,v2,v3;
	DWORD a,b;
	
	M3d_vtx	v[2];
	M3d_vtx rightv;
	M3d_vtx forwardv;
	M3d_vtx upv;
	CVector va;
	CVector vb;
	float fa;
	float fb;
	int count;

	upv[0] = 0;
	upv[1] = 0;
	upv[2] = 1;
	pMesh->type|=M3D_MESH_VOID;


	// Access the Vertices //
	Vertex* vertices;
	pMesh->_vb->Lock(0, 0, (void**)&vertices, 0);




	for(n=0;n<(pMesh->n);n++) // go thru triangles. //
	{
	/// OBTAIN TWO DIRECTION VECTORS /////////////////////////////////////////////////////////
	v1 = pMesh->lst[n*3];
	v2 = pMesh->lst[(n*3)+1];
	v3 = pMesh->lst[(n*3)+2];


	v[0][0] = vertices[v2].x-vertices[v1].x;
	v[0][1] = vertices[v2].y-vertices[v1].y;
	v[0][2] = vertices[v2].z-vertices[v1].z;
	v[1][0] = vertices[v3].x-vertices[v1].x;
	v[1][1] = vertices[v3].y-vertices[v1].y;
	v[1][2] = vertices[v3].z-vertices[v1].z;
	/// COMPUTE NORMAL VECTOR ////////////////////////////////////////////////////////////////
	M3d_calcnormalu(rightv,v[0],v[1]); // pointing into the table.
	M3d_calcnormalu(forwardv,rightv,upv); // pointing along side the triangle vector.
	
	// v1 & v2 are together
	if(vertices[v1].x==vertices[v2].x && vertices[v1].y==vertices[v2].y) a = v1,b = v3; // V3 unique
	else if(vertices[v1].x==vertices[v3].x && vertices[v1].y==vertices[v3].y) a = v1,b = v2; // V2 unique
	else a = v1,b = v3; // V1 unique

	/*
	if(closeby(vertices[v1].x,vertices[v1].y,0,
			   vertices[v2].x,vertices[v2].y,0)) a = v1, b = v3;
	else if(closeby(vertices[v1].x,vertices[v1].y,0,
					vertices[v3].x,vertices[v3].y,0)) a = v1, b = v2;
	else if(closeby(vertices[v2].x,vertices[v2].y,0,
					vertices[v3].x,vertices[v3].y,0)) a = v2, b = v3;
	else {
		char buffer[1024];
		sprintf(buffer,"1:[%f,%f,%f]\r\n2:[%f,%f,%f]\r\n3:[%f,%f,%f]\r\n",vertices[v1].x,vertices[v1].y,vertices[v1].z,
			vertices[v2].x,vertices[v2].y,vertices[v2].z,
			vertices[v3].x,vertices[v3].y,vertices[v3].z);
		
		}*/

	//if(vertices[v1].x!=vertices[v2].x || vertices[v1].y==vertices[v2].y) a = v1, b = v2; // v1/v2 unique
	//else if(vertices[v2].x!=vertices[v3].x || vertices[v2].y==vertices[v3].y) a = v2, b = v3; // v1/v2 unique
	//else if(vertices[v3].x!=vertices[v1].x || vertices[v3].y==vertices[v1].y) a = v3, b = v1; // v1/v2 unique

	// ----------- //
	va.x = vertices[a].x;
	va.y = vertices[a].y;
	va.z = 0;//vertices[a].z;
	vb.x = vertices[b].x;
	vb.y = vertices[b].y;
	vb.z = 0;//vertices[b].z;

	
	fa = forwardv[0] * va.x + forwardv[1] * va.y + forwardv[2] * va.z;
	fb = forwardv[0] * vb.x + forwardv[1] * vb.y + forwardv[2] * vb.z;
	if(fa<fb);
	else { // swap order
	CVector tmpv;
	tmpv = va;
	va = vb;
	vb = tmpv;
	}

	#ifdef FIXEDMODE
	m_obstacleCourse->AttachObstacle(new CObstaclePLANE(FVector(TOFIX(va.x),TOFIX(va.y),TOFIX(va.z)),
														FVector(TOFIX(vb.x),TOFIX(vb.y),TOFIX(vb.z)), FALSE));
	#else
	m_obstacleCourse->AttachObstacle(new CObstaclePLANE(va, vb, FALSE));
	#endif	
	}


	pMesh->_vb->Unlock();
}

#ifndef THE_POOL_CLUB // iSnooker
void CCustomTable::ChangeColorScheme(int n, M3d_object *m_M3d_Table_ptr)
{
	if(!m_ColorSchemeStandard) return; // can't do color schemes!
	if(n>=m_ColorSchemeStandard_n) return; // color scheme doesn't exit
	int i;

	// use first in the list
	m_ColorScheme_idx = n;

	m_pColorScheme = &m_ColorSchemeStandard[n];
	//M3d_object *m_M3d_Table_ptr;
	//m_M3d_Table_ptr = &m_M3d_Table;

	// -- //
	M3d_objectfindreset(m_M3d_Table_ptr);

	while((i = M3d_objectfind("Cushion"))!=-1)
	{
	m_M3d_Table_ptr->mat[i].d3dm.Ambient.r = 0.0f;
	m_M3d_Table_ptr->mat[i].d3dm.Ambient.g = 0.0f;
	m_M3d_Table_ptr->mat[i].d3dm.Ambient.b = 0.0f;
	m_M3d_Table_ptr->mat[i].d3dm.Ambient.a = 1.0f;
	 
	m_M3d_Table_ptr->mat[i].d3dm.Diffuse.r = m_pColorScheme->m_tableCushionRed;
	m_M3d_Table_ptr->mat[i].d3dm.Diffuse.g = m_pColorScheme->m_tableCushionGreen;
	m_M3d_Table_ptr->mat[i].d3dm.Diffuse.b = m_pColorScheme->m_tableCushionBlue;
	m_M3d_Table_ptr->mat[i].d3dm.Diffuse.a = 1.0f;

	//m_M3d_Table_ptr->mat[i].d3dm.Power = 0;
	}
	// --- //
	M3d_objectfindreset(m_M3d_Table_ptr);
	
	while((i = M3d_objectfind("Cloth"))!=-1) // iSnooker
	{
	m_M3d_Table_ptr->mat[i].d3dm.Diffuse.r = m_pColorScheme->m_tableSlateRed;
	m_M3d_Table_ptr->mat[i].d3dm.Diffuse.g = m_pColorScheme->m_tableSlateGreen;
	m_M3d_Table_ptr->mat[i].d3dm.Diffuse.b = m_pColorScheme->m_tableSlateBlue;
	m_M3d_Table_ptr->mat[i].d3dm.Diffuse.a = 1.0f;
												 	
	//m_M3d_Table_ptr->mat[i].d3dm.Power = 0;
	}

	// Also Adjust Color for stable_fb //
	m_M3d_Table_ptr = &m_M3d_Table_fb;
	M3d_objectfindreset(m_M3d_Table_ptr);

	while((i = M3d_objectfind("Cushion"))!=-1)
	{
	m_M3d_Table_ptr->mat[i].d3dm.Ambient.r = 0.0f;
	m_M3d_Table_ptr->mat[i].d3dm.Ambient.g = 0.0f;
	m_M3d_Table_ptr->mat[i].d3dm.Ambient.b = 0.0f;
	m_M3d_Table_ptr->mat[i].d3dm.Ambient.a = 1.0f;
	 
	m_M3d_Table_ptr->mat[i].d3dm.Diffuse.r = m_pColorScheme->m_tableCushionRed;
	m_M3d_Table_ptr->mat[i].d3dm.Diffuse.g = m_pColorScheme->m_tableCushionGreen;
	m_M3d_Table_ptr->mat[i].d3dm.Diffuse.b = m_pColorScheme->m_tableCushionBlue;
	m_M3d_Table_ptr->mat[i].d3dm.Diffuse.a = 1.0f;

	//m_M3d_Table_ptr->mat[i].d3dm.Power = 0;
	}
	// --- //
	M3d_objectfindreset(m_M3d_Table_ptr);
	
	while((i = M3d_objectfind("Cloth"))!=-1) // iSnooker
	{
	m_M3d_Table_ptr->mat[i].d3dm.Diffuse.r = m_pColorScheme->m_tableSlateRed;
	m_M3d_Table_ptr->mat[i].d3dm.Diffuse.g = m_pColorScheme->m_tableSlateGreen;
	m_M3d_Table_ptr->mat[i].d3dm.Diffuse.b = m_pColorScheme->m_tableSlateBlue;
	m_M3d_Table_ptr->mat[i].d3dm.Diffuse.a = 1.0f;
												 	
	//m_M3d_Table_ptr->mat[i].d3dm.Power = 0;
	}


	
	// --- //
}
#endif //iSnooker

#ifdef THE_POOL_CLUB // iPool
void CCustomTable::ChangeColorScheme(int n, M3d_object *m_M3d_Table_ptr)
{
	if(!m_ColorSchemeStandard) return; // can't do color schemes!
	if(n>=m_ColorSchemeStandard_n) return; // color scheme doesn't exit
	int i;

	// use first in the list
	m_ColorScheme_idx = n;

	m_pColorScheme = &m_ColorSchemeStandard[n];
	//M3d_object *m_M3d_Table_ptr;
	//m_M3d_Table_ptr = &m_M3d_Table;

	// -- //
	M3d_objectfindreset(m_M3d_Table_ptr);

	while((i = M3d_objectfind("cushion"))!=-1)
	{
	m_M3d_Table_ptr->mat[i].d3dm.Specular.r=0;
	m_M3d_Table_ptr->mat[i].d3dm.Specular.g=0;
	m_M3d_Table_ptr->mat[i].d3dm.Specular.b=0;
	m_M3d_Table_ptr->mat[i].d3dm.Specular.a=0;
		
	m_M3d_Table_ptr->mat[i].d3dm.Ambient.r=0.1;
	m_M3d_Table_ptr->mat[i].d3dm.Ambient.g=0.1;
	m_M3d_Table_ptr->mat[i].d3dm.Ambient.b=0.1;
	m_M3d_Table_ptr->mat[i].d3dm.Ambient.a=1;
	
	m_M3d_Table_ptr->mat[i].d3dm.Diffuse.r=m_pColorScheme->m_tableCushionRed;
	m_M3d_Table_ptr->mat[i].d3dm.Diffuse.g=m_pColorScheme->m_tableCushionGreen;
	m_M3d_Table_ptr->mat[i].d3dm.Diffuse.b=m_pColorScheme->m_tableCushionBlue;
	m_M3d_Table_ptr->mat[i].d3dm.Diffuse.a=1;

	//m_M3d_Table_ptr->mat[i].d3dm.Power = 0;
	}
	// --- //
	M3d_objectfindreset(m_M3d_Table_ptr);
	

	while((i = M3d_objectfind("slate"))!=-1)
	{
	m_M3d_Table_ptr->mat[i].d3dm.Specular.r=0;
	m_M3d_Table_ptr->mat[i].d3dm.Specular.g=0;
	m_M3d_Table_ptr->mat[i].d3dm.Specular.b=0;
	m_M3d_Table_ptr->mat[i].d3dm.Specular.a=0;
	
	
	m_M3d_Table_ptr->mat[i].d3dm.Ambient.r=0.1;
	m_M3d_Table_ptr->mat[i].d3dm.Ambient.g=0.1;
	m_M3d_Table_ptr->mat[i].d3dm.Ambient.b=0.1;
	m_M3d_Table_ptr->mat[i].d3dm.Ambient.a=1;
	
	m_M3d_Table_ptr->mat[i].d3dm.Diffuse.r=m_pColorScheme->m_tableSlateRed;
	m_M3d_Table_ptr->mat[i].d3dm.Diffuse.g=m_pColorScheme->m_tableSlateGreen;
	m_M3d_Table_ptr->mat[i].d3dm.Diffuse.b=m_pColorScheme->m_tableSlateBlue;
	m_M3d_Table_ptr->mat[i].d3dm.Diffuse.a=1;
												 	
	//m_M3d_Table_ptr->mat[i].d3dm.Power = 0;
	}
}
#endif // iPool

void CCustomTable::ChangeGameType(int gameType)
{
	if(g_build.gameType == GAMETYPE_POOL)
	{
		if(gameType == 0)
		{
			m_type = CTTYPE_8BALL;
		}
		else if(gameType == 1)
		{
			m_type = CTTYPE_9BALL;
		}
		else if(gameType == 2)
		{
			m_type = CTTYPE_ROTATIONBALL;
		}
		else if(gameType == 3)
		{
			m_type = CTTYPE_BOWLS;
		}
		else if(gameType == 4)
		{
			m_type = CTTYPE_6BALL;
		}
		else if(gameType == 5)
		{
			m_type = CTTYPE_10BALL;
		}
		else if(gameType == 6)
		{
			m_type = CTTYPE_BREAK;
		}
		else if(gameType == 7)
		{
			m_type = CTTYPE_UKPOOL;
		}
	}
	else if(g_build.gameType == GAMETYPE_SNOOKER)
	{
		if(gameType == 0)
		{
			m_type = CTTYPE_SNOOKER;
		}
		else if(gameType == 1)
		{
			m_type = CTTYPE_SNOOKER10;
		}
		else if(gameType == 2)
		{
			m_type = CTTYPE_SNOOKER6;
		}
		else if(gameType == 3)
		{
			m_type = CTTYPE_RESPOTBLACK;
		}
		else if(gameType == 4)
		{
			m_type = CTTYPE_BILLIARDS;
			subtype = 0;
		}
		else if(gameType == 5)
		{
			m_type = CTTYPE_BILLIARDS;
			subtype = 1;
		}
		else if(gameType == 6)
		{
			m_type = CTTYPE_BILLIARDS;
			subtype = 2;
		}
	}
}

//#define _256TO1(n) (((float) n)*(0.003922f))	// 1/255

void CCustomTable::PoolInit(void)
{	
	m_type = CTTYPE_8BALL;
	m_tableType = 0; // default table type mesh
	

	// table bounds for line
	m_pParent->m_tableBound.minX = -1500;
	m_pParent->m_tableBound.maxX = 1500;
	m_pParent->m_tableBound.minY = -500;
	m_pParent->m_tableBound.maxY = 500;

	// create pockets
	AddPocket(CVector((-7+253*5+BALL_RADIUS*1.25), (+7-126.5*5-BALL_RADIUS*1),  (0)),  POCKET_BLACKG, BALL_RADIUS*1.4, BALL_RADIUS*2.4, 0.05);	// BLACK-G
	AddPocket(CVector((+253*5+BALL_RADIUS*1.25),   (+126.5*5+BALL_RADIUS*1),    (0)),  POCKET_BLACKY, BALL_RADIUS*1.4, BALL_RADIUS*2.4, 0.05);	// BLACK-Y
	AddPocket(CVector((0),						   (+10-126.5*5-BALL_RADIUS*3), (0)),  POCKET_BLUEG, BALL_RADIUS*1.4, BALL_RADIUS*2.4, 0.05 );	// BLUE-G
	AddPocket(CVector((0),						   (+126.5*5+BALL_RADIUS*3),    (0)),  POCKET_BLUEY, BALL_RADIUS*1.4, BALL_RADIUS*2.4, 0.05 );	// BLUE-Y
	AddPocket(CVector((+7-253*5-BALL_RADIUS*1.25), (+7-126.5*5-BALL_RADIUS*1),  (0)),  POCKET_GREEN, BALL_RADIUS*1.4, BALL_RADIUS*2.4, 0.05 );	// Green
	AddPocket(CVector((-253*5-BALL_RADIUS*1.25),   (+126.5*5+BALL_RADIUS*1),    (0)),  POCKET_YELLOW, BALL_RADIUS*1.4, BALL_RADIUS*2.4, 0.05 );	// Yellow	

	// Create Some Color Schemes //
	m_ColorSchemeStandard_n = 12;
	m_ColorSchemeStandard = new CColorScheme[m_ColorSchemeStandard_n];
	int i;
	int n;
	for(n=0;n<m_ColorSchemeStandard_n;n++) // Precreate color schemes
	{
	m_pColorScheme = &m_ColorSchemeStandard[n];
	switch(n)
		{
		case 0:// BLUE //40);
			   m_pColorScheme->m_tableCushionRed = _256TO1(51);
			   m_pColorScheme->m_tableCushionGreen = _256TO1(115);
			   m_pColorScheme->m_tableCushionBlue = _256TO1(168);
			   m_pColorScheme->m_tableSlateRed = _256TO1(27);
			   m_pColorScheme->m_tableSlateGreen = _256TO1(95);
			   m_pColorScheme->m_tableSlateBlue = _256TO1(151);
			   break;

		case 1:// RED //
			   m_pColorScheme->m_tableCushionRed = _256TO1(162);
			   m_pColorScheme->m_tableCushionGreen = _256TO1(31);
			   m_pColorScheme->m_tableCushionBlue = _256TO1(21);
			   m_pColorScheme->m_tableSlateRed = _256TO1(135);
			   m_pColorScheme->m_tableSlateGreen = _256TO1(14);
			   m_pColorScheme->m_tableSlateBlue = _256TO1(9);
			   break;
		case 2:// GREEN //
			   m_pColorScheme->m_tableCushionRed = _256TO1(10);
			   m_pColorScheme->m_tableCushionGreen = _256TO1(139);
			   m_pColorScheme->m_tableCushionBlue = _256TO1(10);
			   m_pColorScheme->m_tableSlateRed = _256TO1(0);
			   m_pColorScheme->m_tableSlateGreen = _256TO1(109);
			   m_pColorScheme->m_tableSlateBlue = _256TO1(0);
			   break;
		case 3:// BLACK //
			   m_pColorScheme->m_tableCushionRed = 0.27;
			   m_pColorScheme->m_tableCushionGreen = 0.27;
			   m_pColorScheme->m_tableCushionBlue = 0.27;
			   m_pColorScheme->m_tableSlateRed = 0.1059;
			   m_pColorScheme->m_tableSlateGreen = 0.1059;
			   m_pColorScheme->m_tableSlateBlue = 0.1059;
			   break;
		case 4:// YELLOW //
			   m_pColorScheme->m_tableCushionRed = _256TO1(182);
			   m_pColorScheme->m_tableCushionGreen = _256TO1(183);
			   m_pColorScheme->m_tableCushionBlue = _256TO1(69);
			   m_pColorScheme->m_tableSlateRed = _256TO1(126);
			   m_pColorScheme->m_tableSlateGreen = _256TO1(122);
			   m_pColorScheme->m_tableSlateBlue = _256TO1(0);
			   break;
		case 5:// PURPLE //
			   m_pColorScheme->m_tableCushionRed = _256TO1(147);
			   m_pColorScheme->m_tableCushionGreen = _256TO1(65);
			   m_pColorScheme->m_tableCushionBlue = _256TO1(150);
			   m_pColorScheme->m_tableSlateRed = _256TO1(98);
			   m_pColorScheme->m_tableSlateGreen = _256TO1(27);
			   m_pColorScheme->m_tableSlateBlue = _256TO1(104);
			   break;
		case 6:// TEAL //
			   m_pColorScheme->m_tableCushionRed = _256TO1(57);
			   m_pColorScheme->m_tableCushionGreen = _256TO1(179);
			   m_pColorScheme->m_tableCushionBlue = _256TO1(154);
			   m_pColorScheme->m_tableSlateRed = _256TO1(37);
			   m_pColorScheme->m_tableSlateGreen = _256TO1(157);
			   m_pColorScheme->m_tableSlateBlue = _256TO1(145);
			   break;
		case 7:// BROWN //
			   m_pColorScheme->m_tableCushionRed = _256TO1(55);
			   m_pColorScheme->m_tableCushionGreen = _256TO1(30);
			   m_pColorScheme->m_tableCushionBlue = _256TO1(15);
			   m_pColorScheme->m_tableSlateRed = _256TO1(35);
			   m_pColorScheme->m_tableSlateGreen = _256TO1(19);
			   m_pColorScheme->m_tableSlateBlue = _256TO1(8);
			   break;
		case 8:// WHITE //
			   m_pColorScheme->m_tableCushionRed = _256TO1(180);
			   m_pColorScheme->m_tableCushionGreen = _256TO1(180);
			   m_pColorScheme->m_tableCushionBlue = _256TO1(180);
			   m_pColorScheme->m_tableSlateRed = _256TO1(120);
			   m_pColorScheme->m_tableSlateGreen = _256TO1(120);
			   m_pColorScheme->m_tableSlateBlue = _256TO1(120);	   
			   break;
		case 9:// BLUE & BLACK //
			   m_pColorScheme->m_tableCushionRed = _256TO1(12);
			   m_pColorScheme->m_tableCushionGreen = _256TO1(12);
			   m_pColorScheme->m_tableCushionBlue = _256TO1(12);
			   m_pColorScheme->m_tableSlateRed = _256TO1(15);
			   m_pColorScheme->m_tableSlateGreen = _256TO1(0);
			   m_pColorScheme->m_tableSlateBlue = _256TO1(128);
			   break;
		case 10:// RED & BLACK //
			   m_pColorScheme->m_tableCushionRed = _256TO1(12);
			   m_pColorScheme->m_tableCushionGreen = _256TO1(12);
			   m_pColorScheme->m_tableCushionBlue = _256TO1(12);
			   m_pColorScheme->m_tableSlateRed = _256TO1(128);
			   m_pColorScheme->m_tableSlateGreen = _256TO1(0);
			   m_pColorScheme->m_tableSlateBlue = _256TO1(0);
			   break;
		case 11:// PURPLE & BLACK //
			   m_pColorScheme->m_tableCushionRed = _256TO1(12);
			   m_pColorScheme->m_tableCushionGreen = _256TO1(12);
			   m_pColorScheme->m_tableCushionBlue = _256TO1(12);
			   m_pColorScheme->m_tableSlateRed = _256TO1(98);
			   m_pColorScheme->m_tableSlateGreen = _256TO1(27);
			   m_pColorScheme->m_tableSlateBlue = _256TO1(104);
			   break;
		}
	}
	// ------------------------- //

	// Load Pool Tables //
	Mmain_log("CCustomTable, M3d_objectload (m_M3d_Table)");
	//M3d_objectload(&m_M3d_Table,"Graphics/m3d/nntable.m3d", M3D_OBJECT_STILL);	

	if(g_bGfxQuality==FALSE) // Low Quality
	{
	// original pool table
	M3d_objectload(&m_M3d_Table,"Graphics/m3d/ptablent.m3d", M3D_OBJECT_STILL);	
	//PoolTableModifyMaterials(&m_M3d_Table);
	//M3d_interceptobject_precalc(&m_M3d_Table); // pre-calc table interception
		
	// carom-style pool table
	M3d_objectload(&m_M3d_caromTable,"Graphics/m3d/ptablentc.m3d", M3D_OBJECT_STILL);		
	PoolTableModifyMaterials(&m_M3d_caromTable);
	M3d_interceptobject_precalc(&m_M3d_caromTable); // pre-calc table interception

	// pro pool table
	M3d_objectload(&m_M3d_proTable,"Graphics/m3d/ptablepront.m3d", M3D_OBJECT_STILL);	
	PoolTableModifyMaterials(&m_M3d_proTable);
	M3d_interceptobject_precalc(&m_M3d_proTable); // pre-calc table interception
	
	// English Pool Table
	M3d_objectload(&m_M3d_ukTable,"Graphics/m3d/uktable.m3d", M3D_OBJECT_STILL);	
	PoolTableModifyMaterialsUKPool(&m_M3d_ukTable);
	M3d_interceptobject_precalc(&m_M3d_ukTable); // pre-calc table interception
	}
	else
	{
	// original pool table
	M3d_objectload(&m_M3d_Table,"Graphics/m3d/ptable.m3d", M3D_OBJECT_STILL);	
	PoolTableModifyMaterials(&m_M3d_Table);
	M3d_interceptobject_precalc(&m_M3d_Table); // pre-calc table interception
		
	// carom-style pool table
	M3d_objectload(&m_M3d_caromTable,"Graphics/m3d/caromTable.m3d", M3D_OBJECT_STILL);		
	PoolTableModifyMaterials(&m_M3d_caromTable);
	M3d_interceptobject_precalc(&m_M3d_caromTable); // pre-calc table interception

	// pro pool table
	M3d_objectload(&m_M3d_proTable,"Graphics/m3d/ptablepro.m3d", M3D_OBJECT_STILL);	
	PoolTableModifyMaterials(&m_M3d_proTable);
	M3d_interceptobject_precalc(&m_M3d_proTable); // pre-calc table interception
	
	// English Pool table
	M3d_objectload(&m_M3d_ukTable,"Graphics/m3d/uktable.m3d", M3D_OBJECT_STILL);	
	//PoolTableModifyMaterials(&m_M3d_ukTable);
	PoolTableModifyMaterialsUKPool(&m_M3d_ukTable);
	M3d_interceptobject_precalc(&m_M3d_ukTable); // pre-calc table interception
	}

	// setup pointer array to tables
	m_M3d_Table_ptr[0] = &m_M3d_Table;		// Normal Table
	m_M3d_Table_ptr[1] = &m_M3d_proTable;   // Pro Table
	m_M3d_Table_ptr[2] = &m_M3d_ukTable;    // UK Table
	m_M3d_Table_ptr[3] = &m_M3d_caromTable; // no pockets: Center Hole
	m_M3d_Table_ptr[4] = &m_M3d_caromTable; // no pockets: Black-Hole
	m_M3d_Table_ptr[5] = &m_M3d_caromTable; // no pockets: Random 3-Holes
	m_M3d_Table_ptr[6] = &m_M3d_caromTable; // no pockets: Random 5-Holes
	m_M3d_Table_ptr[7] = &m_M3d_caromTable; // no pockets: Random 7-Holes
	m_M3d_Table_ptr[8] = &m_M3d_caromTable; // no pockets: Random 9-Holes
	
	// Load Pool Balls //
	Mmain_log("CCustomTable, M3d_objectload (m_M3d_Ball[0,1,2])");

	M3d_object *Ball_ptr;
	Ball_ptr = m_M3d_Ball; // Start from first ball

	M3d_denormalizationFactor = 0.1; // the balls are scaled to 0.1 and we want SPEED!!!

	/// *NEW* DRAW BALLS 1.5% BIGGER
	M3d_denormalizationFactor *= 1.015f;
	/////////////////
		
	char buffer[80];

	if(g_bGfxQuality==FALSE) // Low Quality
		{
		m_M3d_Ball_low = Ball_ptr;
		sprintf(buffer,"Graphics/m3d/balls/spot.m3d");
		M3d_objectload(Ball_ptr,buffer, M3D_OBJECT_STILL),Ball_ptr++;
		sprintf(buffer,"Graphics/m3d/balls/stripe.m3d");
		M3d_objectload(Ball_ptr,buffer, M3D_OBJECT_STILL),Ball_ptr++;

		m_M3d_Ball_med = m_M3d_Ball_high = m_M3d_Ball_low;
		m_M3d_Ball_end = Ball_ptr;

		ReNormalizeBalls(); // Low Quality balls are multiple objects, so we cheat
		}
	else{ // High Quality (Default)
		// LOW BALLS
		m_M3d_Ball_low = Ball_ptr;	
		
		for(n=0;n<16;n++)
			{
			if(!n) sprintf(buffer,"Graphics/m3d/balls/wball_l.m3d");
			else sprintf(buffer,"Graphics/m3d/balls/%dball_l.m3d",n);
			M3d_objectload(Ball_ptr,buffer, M3D_OBJECT_STILL),Ball_ptr++;
			}
		
		// HIGH BALLS
		m_M3d_Ball_high = Ball_ptr;
		for(n=0;n<16;n++)
			{
			if(!n) sprintf(buffer,"Graphics/m3d/balls/wball.m3d");
			else sprintf(buffer,"Graphics/m3d/balls/%dball.m3d",n);
			M3d_objectload(Ball_ptr,buffer, M3D_OBJECT_STILL),Ball_ptr++;
			}
		
		
		// English Pool Balls //
		sprintf(buffer,"Graphics/m3d/balls/uk_y_ball.m3d");
		M3d_objectload(Ball_ptr,buffer, M3D_OBJECT_STILL),Ball_ptr++;
		sprintf(buffer,"Graphics/m3d/balls/uk_r_ball.m3d");
		M3d_objectload(Ball_ptr,buffer, M3D_OBJECT_STILL),Ball_ptr++;

		// Break Pool Balls //
		sprintf(buffer,"Graphics/m3d/balls/b_1_ball.m3d");
		M3d_objectload(Ball_ptr,buffer, M3D_OBJECT_STILL),Ball_ptr++;
		sprintf(buffer,"Graphics/m3d/balls/b_3_ball.m3d");
		M3d_objectload(Ball_ptr,buffer, M3D_OBJECT_STILL),Ball_ptr++;
		sprintf(buffer,"Graphics/m3d/balls/b_5_ball.m3d");
		M3d_objectload(Ball_ptr,buffer, M3D_OBJECT_STILL),Ball_ptr++;
		sprintf(buffer,"Graphics/m3d/balls/b_7_ball.m3d");
		M3d_objectload(Ball_ptr,buffer, M3D_OBJECT_STILL),Ball_ptr++;

		m_M3d_Ball_med = m_M3d_Ball_high;// = m_M3d_Ball_low;	
		m_M3d_Ball_end = Ball_ptr;
		}
	 // end gfxqual TRUE

	M3d_denormalizationFactor = 1;

		
	// Load Pocket Object
	Mmain_log("CCustomTable, M3d_objectload (m_M3d_PocketObject)");
	sprintf(buffer,"Graphics/m3d/hole.m3d");
	M3d_objectload(&m_M3d_PocketObject, buffer, M3D_OBJECT_STILL);
		
	// Setup Lighting Position
	((CGameAppView *)m_pParent->m_parent)->m_environment->LightPos(0,-1250,0,600);
	((CGameAppView *)m_pParent->m_parent)->m_environment->LightPos(1,0,0,600);
	((CGameAppView *)m_pParent->m_parent)->m_environment->LightPos(2,+1250,0,600);	

	// Extract lines //
	BuildObstacleCourse(&m_M3d_Table); // for original pool table (default)
}


void CCustomTable::PoolInitReset(void)
{	
	/*
	int n;

	// Load Pool Tables //
	Mmain_log("CCustomTable (PoolInitReset), M3d_objectload (m_M3d_Table)");
	//M3d_objectload(&m_M3d_Table,"Graphics/m3d/nntable.m3d", M3D_OBJECT_STILL);	

	if(g_bGfxQuality==FALSE) // Low Quality
	{
	// original pool table
	M3d_objectload(&m_M3d_Table,"Graphics/m3d/ptablent.m3d", M3D_OBJECT_STILL);	
	PoolTableModifyMaterials(&m_M3d_Table);
	M3d_interceptobject_precalc(&m_M3d_Table); // pre-calc table interception
		
	// carom-style pool table
	M3d_objectload(&m_M3d_caromTable,"Graphics/m3d/ptablentc.m3d", M3D_OBJECT_STILL);		
	PoolTableModifyMaterials(&m_M3d_caromTable);
	M3d_interceptobject_precalc(&m_M3d_caromTable); // pre-calc table interception

	// pro pool table
	M3d_objectload(&m_M3d_proTable,"Graphics/m3d/ptablepront.m3d", M3D_OBJECT_STILL);	
	PoolTableModifyMaterials(&m_M3d_proTable);
	M3d_interceptobject_precalc(&m_M3d_proTable); // pre-calc table interception
	
	// English Pool Table
	M3d_objectload(&m_M3d_ukTable,"Graphics/m3d/uktable.m3d", M3D_OBJECT_STILL);	
	PoolTableModifyMaterialsUKPool(&m_M3d_ukTable);
	M3d_interceptobject_precalc(&m_M3d_ukTable); // pre-calc table interception
	}
	else
	{
	// original pool table
	M3d_objectload(&m_M3d_Table,"Graphics/m3d/ptable.m3d", M3D_OBJECT_STILL);	
	PoolTableModifyMaterials(&m_M3d_Table);
	M3d_interceptobject_precalc(&m_M3d_Table); // pre-calc table interception
		
	// carom-style pool table
	M3d_objectload(&m_M3d_caromTable,"Graphics/m3d/caromTable.m3d", M3D_OBJECT_STILL);		
	PoolTableModifyMaterials(&m_M3d_caromTable);
	M3d_interceptobject_precalc(&m_M3d_caromTable); // pre-calc table interception

	// pro pool table
	M3d_objectload(&m_M3d_proTable,"Graphics/m3d/ptablepro.m3d", M3D_OBJECT_STILL);	
	PoolTableModifyMaterials(&m_M3d_proTable);
	M3d_interceptobject_precalc(&m_M3d_proTable); // pre-calc table interception
	
	// English Pool table
	M3d_objectload(&m_M3d_ukTable,"Graphics/m3d/uktable.m3d", M3D_OBJECT_STILL);	
	//PoolTableModifyMaterials(&m_M3d_ukTable);
	PoolTableModifyMaterialsUKPool(&m_M3d_ukTable);
	M3d_interceptobject_precalc(&m_M3d_ukTable); // pre-calc table interception
	}

	// setup pointer array to tables
	m_M3d_Table_ptr[0] = &m_M3d_Table;		// Normal Table
	m_M3d_Table_ptr[1] = &m_M3d_proTable;   // Pro Table
	m_M3d_Table_ptr[2] = &m_M3d_ukTable;    // UK Table
	m_M3d_Table_ptr[3] = &m_M3d_caromTable; // no pockets: Center Hole
	m_M3d_Table_ptr[4] = &m_M3d_caromTable; // no pockets: Black-Hole
	m_M3d_Table_ptr[5] = &m_M3d_caromTable; // no pockets: Random 3-Holes
	m_M3d_Table_ptr[6] = &m_M3d_caromTable; // no pockets: Random 5-Holes
	m_M3d_Table_ptr[7] = &m_M3d_caromTable; // no pockets: Random 7-Holes
	m_M3d_Table_ptr[8] = &m_M3d_caromTable; // no pockets: Random 9-Holes
	
	
	

	// Load Pool Balls //
	Mmain_log("CCustomTable, M3d_objectload (m_M3d_Ball[0,1,2])");

	M3d_object *Ball_ptr;
	Ball_ptr = m_M3d_Ball; // Start from first ball

	M3d_denormalizationFactor = 0.1; // the balls are scaled to 0.1 and we want SPEED!!!

	/// *NEW* DRAW BALLS 1.5% BIGGER
	M3d_denormalizationFactor *= 1.015f;
	/////////////////
		
	char buffer[80];

	if(g_bGfxQuality==FALSE) // Low Quality
		{
		m_M3d_Ball_low = Ball_ptr;
		sprintf(buffer,"Graphics/m3d/balls/spot.m3d");
		M3d_objectload(Ball_ptr,buffer, M3D_OBJECT_STILL),Ball_ptr++;
		sprintf(buffer,"Graphics/m3d/balls/stripe.m3d");
		M3d_objectload(Ball_ptr,buffer, M3D_OBJECT_STILL),Ball_ptr++;

		m_M3d_Ball_med = m_M3d_Ball_high = m_M3d_Ball_low;
		m_M3d_Ball_end = Ball_ptr;

		ReNormalizeBalls(); // Low Quality balls are multiple objects, so we cheat
		}
	else{ // High Quality (Default)
		// LOW BALLS
		m_M3d_Ball_low = Ball_ptr;	
		
		for(n=0;n<16;n++)
			{
			if(!n) sprintf(buffer,"Graphics/m3d/balls/wball_l.m3d");
			else sprintf(buffer,"Graphics/m3d/balls/%dball_l.m3d",n);
			M3d_objectload(Ball_ptr,buffer, M3D_OBJECT_STILL),Ball_ptr++;
			}
		
		// HIGH BALLS
		m_M3d_Ball_high = Ball_ptr;
		for(n=0;n<16;n++)
			{
			if(!n) sprintf(buffer,"Graphics/m3d/balls/wball.m3d");
			else sprintf(buffer,"Graphics/m3d/balls/%dball.m3d",n);
			M3d_objectload(Ball_ptr,buffer, M3D_OBJECT_STILL),Ball_ptr++;
			}
		
		
		// English Pool Balls //
		sprintf(buffer,"Graphics/m3d/balls/uk_y_ball.m3d");
		M3d_objectload(Ball_ptr,buffer, M3D_OBJECT_STILL),Ball_ptr++;
		sprintf(buffer,"Graphics/m3d/balls/uk_r_ball.m3d");
		M3d_objectload(Ball_ptr,buffer, M3D_OBJECT_STILL),Ball_ptr++;

		// Break Pool Balls //
		sprintf(buffer,"Graphics/m3d/balls/b_1_ball.m3d");
		M3d_objectload(Ball_ptr,buffer, M3D_OBJECT_STILL),Ball_ptr++;
		sprintf(buffer,"Graphics/m3d/balls/b_3_ball.m3d");
		M3d_objectload(Ball_ptr,buffer, M3D_OBJECT_STILL),Ball_ptr++;
		sprintf(buffer,"Graphics/m3d/balls/b_5_ball.m3d");
		M3d_objectload(Ball_ptr,buffer, M3D_OBJECT_STILL),Ball_ptr++;
		sprintf(buffer,"Graphics/m3d/balls/b_7_ball.m3d");
		M3d_objectload(Ball_ptr,buffer, M3D_OBJECT_STILL),Ball_ptr++;

		m_M3d_Ball_med = m_M3d_Ball_high;// = m_M3d_Ball_low;	
		m_M3d_Ball_end = Ball_ptr;
		}
	 // end gfxqual TRUE

	M3d_denormalizationFactor = 1;
	
		
	// Load Pocket Object
	Mmain_log("CCustomTable, M3d_objectload (m_M3d_PocketObject)");
	sprintf(buffer,"Graphics/m3d/hole.m3d");
	M3d_objectload(&m_M3d_PocketObject, buffer, M3D_OBJECT_STILL);
		
	*/

	// Setup Lighting Position
	((CGameAppView *)m_pParent->m_parent)->m_environment->LightPos(0,-1250,0,600);
	((CGameAppView *)m_pParent->m_parent)->m_environment->LightPos(1,0,0,600);
	((CGameAppView *)m_pParent->m_parent)->m_environment->LightPos(2,+1250,0,600);	



	return;
	

	// Extract lines //
	BuildObstacleCourse(&m_M3d_Table); // for original pool table (default)

}



void STFrameModifyMaterials(M3d_object *m_M3d_ptr)
{
	// Color the Entire Thing Brown //
	int n;
	for(n=0;n<m_M3d_ptr->mesh_n;n++)
	{	
	m_M3d_ptr->mat[n].d3dm.Diffuse.r = 0.12;
	m_M3d_ptr->mat[n].d3dm.Diffuse.g = 0.04;
	m_M3d_ptr->mat[n].d3dm.Diffuse.b = 0.02;
	m_M3d_ptr->mat[n].d3dm.Diffuse.a = 1.0;

	m_M3d_ptr->mat[n].d3dm.Ambient.r = 0.00;
	m_M3d_ptr->mat[n].d3dm.Ambient.g = 0.00;
	m_M3d_ptr->mat[n].d3dm.Ambient.b = 0.00;
	m_M3d_ptr->mat[n].d3dm.Ambient.a = 1.0;

	m_M3d_ptr->mat[n].d3dm.Emissive.r = 0.00;
	m_M3d_ptr->mat[n].d3dm.Emissive.g = 0.00;
	m_M3d_ptr->mat[n].d3dm.Emissive.b = 0.00;
	m_M3d_ptr->mat[n].d3dm.Emissive.a = 1.0;
	
	m_M3d_ptr->mat[n].d3dm.Specular.r = 0.4;
	m_M3d_ptr->mat[n].d3dm.Specular.g = 0.4;
	m_M3d_ptr->mat[n].d3dm.Specular.b = 0.4;
	m_M3d_ptr->mat[n].d3dm.Specular.a = 1.0;
	m_M3d_ptr->mat[n].d3dm.Power = 30;
	}
	
}

void STShadowModifyMaterials(M3d_object *m_M3d_ptr)
{
	
	// Color the Entire Thing Dark Grey //
	int n;
	for(n=0;n<m_M3d_ptr->mesh_n;n++)
	{	
	m_M3d_ptr->mat[n].d3dm.Diffuse.r = 0.0;
	m_M3d_ptr->mat[n].d3dm.Diffuse.g = 0.0;
	m_M3d_ptr->mat[n].d3dm.Diffuse.b = 0.0;
	m_M3d_ptr->mat[n].d3dm.Diffuse.a = 0.9;

	m_M3d_ptr->mat[n].d3dm.Ambient.r = 0.0;
	m_M3d_ptr->mat[n].d3dm.Ambient.g = 0.0;
	m_M3d_ptr->mat[n].d3dm.Ambient.b = 0.0;
	m_M3d_ptr->mat[n].d3dm.Ambient.a = 0.9;

	m_M3d_ptr->mat[n].d3dm.Emissive.r = 0.0;
	m_M3d_ptr->mat[n].d3dm.Emissive.g = 0.0;
	m_M3d_ptr->mat[n].d3dm.Emissive.b = 0.0;
	m_M3d_ptr->mat[n].d3dm.Emissive.a = 0.9;
	
	m_M3d_ptr->mat[n].d3dm.Specular.r = 0.0;
	m_M3d_ptr->mat[n].d3dm.Specular.g = 0.0;
	m_M3d_ptr->mat[n].d3dm.Specular.b = 0.0;
	m_M3d_ptr->mat[n].d3dm.Specular.a = 0.9;
	}
	
	
}


void Scene1ModifyMaterials(M3d_object *m_M3d_ptr)
{
	M3d_vtx pos;
	M3d_vtx scale;
	M3d_vtx rot;
	int n;

	

	for(n=0;n<m_M3d_ptr->mesh_n;n++)
	{	// Set up material for all meshes //
	/*
	m_M3d_ptr->mat[n].d3dm.Diffuse.r = 0.40;
	m_M3d_ptr->mat[n].d3dm.Diffuse.g = 0.23;
	m_M3d_ptr->mat[n].d3dm.Diffuse.b = 0.15;
	m_M3d_ptr->mat[n].d3dm.Diffuse.a = 1.0;
	*/
	m_M3d_ptr->mat[n].d3dm.Diffuse.r = 1.0;
	m_M3d_ptr->mat[n].d3dm.Diffuse.g = 1.0;
	m_M3d_ptr->mat[n].d3dm.Diffuse.b = 1.0;
	m_M3d_ptr->mat[n].d3dm.Diffuse.a = 1.0;
	
	
	m_M3d_ptr->mat[n].d3dm.Specular.r = 0.1;
	m_M3d_ptr->mat[n].d3dm.Specular.g = 0.1;
	m_M3d_ptr->mat[n].d3dm.Specular.b = 0.1;
	m_M3d_ptr->mat[n].d3dm.Specular.a = 1.0;

	m_M3d_ptr->mat[n].d3dm.Ambient.r = 0.3;
	m_M3d_ptr->mat[n].d3dm.Ambient.g = 0.3;
	m_M3d_ptr->mat[n].d3dm.Ambient.b = 0.3;
	m_M3d_ptr->mat[n].d3dm.Ambient.a = 1.0;

	m_M3d_ptr->mat[n].d3dm.Emissive.r = 0.6;
	m_M3d_ptr->mat[n].d3dm.Emissive.g = 0.6;
	m_M3d_ptr->mat[n].d3dm.Emissive.b = 0.6;
	m_M3d_ptr->mat[n].d3dm.Emissive.a = 1.0;


	m_M3d_ptr->mat[n].d3dm.Power = 0;
	m_M3d_ptr->mat[n].d3dm.Power = D3D_MATERIAL_POWER;//70;
	}


	M3d_objectfindreset(m_M3d_ptr);
	while((n = M3d_objectfind("Gengon01"))!=-1) // Main part of ball is black
	{
	m_M3d_ptr->mat[n].d3dm.Diffuse.r = 0.10;
	m_M3d_ptr->mat[n].d3dm.Diffuse.g = 0.13;
	m_M3d_ptr->mat[n].d3dm.Diffuse.b = 0.15;
	m_M3d_ptr->mat[n].d3dm.Diffuse.a = 1.0;
	}

	M3d_objectfindreset(m_M3d_ptr);
	while((n = M3d_objectfind("arena_floor"))!=-1) // Main part of ball is black
	{
	m_M3d_ptr->mat[n].d3dm.Diffuse.r = 1.0;
	m_M3d_ptr->mat[n].d3dm.Diffuse.g = 1.0;
	m_M3d_ptr->mat[n].d3dm.Diffuse.b = 1.0;
	m_M3d_ptr->mat[n].d3dm.Diffuse.a = 1.0;

	m_M3d_ptr->mat[n].d3dm.Specular.r = 0.1;
	m_M3d_ptr->mat[n].d3dm.Specular.g = 0.1;
	m_M3d_ptr->mat[n].d3dm.Specular.b = 0.1;
	m_M3d_ptr->mat[n].d3dm.Specular.a = 1.0;
	}

	M3d_objectfindreset(m_M3d_ptr);
	while((n = M3d_objectfind("poster1"))!=-1) // Main part of ball is black
	{
	m_M3d_ptr->mat[n].d3dm.Diffuse.r = 0.9;
	m_M3d_ptr->mat[n].d3dm.Diffuse.g = 0.9;
	m_M3d_ptr->mat[n].d3dm.Diffuse.b = 0.9;
	m_M3d_ptr->mat[n].d3dm.Diffuse.a = 1.0;

	m_M3d_ptr->mat[n].d3dm.Specular.r = 0.1;
	m_M3d_ptr->mat[n].d3dm.Specular.g = 0.1;
	m_M3d_ptr->mat[n].d3dm.Specular.b = 0.1;
	m_M3d_ptr->mat[n].d3dm.Specular.a = 1.0;
	}

		M3d_objectfindreset(m_M3d_ptr);
	while((n = M3d_objectfind("curtains"))!=-1) // Main part of ball is black
	{
	m_M3d_ptr->mat[n].d3dm.Diffuse.r = 0.7;
	m_M3d_ptr->mat[n].d3dm.Diffuse.g = 0.7;
	m_M3d_ptr->mat[n].d3dm.Diffuse.b = 0.7;
	m_M3d_ptr->mat[n].d3dm.Diffuse.a = 1.0;

	m_M3d_ptr->mat[n].d3dm.Specular.r = 0.0;
	m_M3d_ptr->mat[n].d3dm.Specular.g = 0.0;
	m_M3d_ptr->mat[n].d3dm.Specular.b = 0.0;
	m_M3d_ptr->mat[n].d3dm.Specular.a = 1.0;
	}
	
	M3d_objectfindreset(m_M3d_ptr);
	while((n = M3d_objectfind("scoreboard_base"))!=-1) // Main part of ball is black
	{
	m_M3d_ptr->mat[n].d3dm.Diffuse.r = 1.0;
	m_M3d_ptr->mat[n].d3dm.Diffuse.g = 1.0;
	m_M3d_ptr->mat[n].d3dm.Diffuse.b = 1.0;
	m_M3d_ptr->mat[n].d3dm.Diffuse.a = 1.0;

	m_M3d_ptr->mat[n].d3dm.Specular.r = 0.0;
	m_M3d_ptr->mat[n].d3dm.Specular.g = 0.0;
	m_M3d_ptr->mat[n].d3dm.Specular.b = 0.0;
	m_M3d_ptr->mat[n].d3dm.Specular.a = 1.0;

		m_M3d_ptr->mat[n].d3dm.Ambient.r = 0.4;
	m_M3d_ptr->mat[n].d3dm.Ambient.g = 0.4;
	m_M3d_ptr->mat[n].d3dm.Ambient.b = 0.4;
	m_M3d_ptr->mat[n].d3dm.Ambient.a = 1.0;
	}
	
	M3d_objectfindreset(m_M3d_ptr);
	while((n = M3d_objectfind("scoreboard_metalbar"))!=-1) // Main part of ball is black
	{
	m_M3d_ptr->mat[n].d3dm.Diffuse.r = 1.0;
	m_M3d_ptr->mat[n].d3dm.Diffuse.g = 1.0;
	m_M3d_ptr->mat[n].d3dm.Diffuse.b = 0.0;
	m_M3d_ptr->mat[n].d3dm.Diffuse.a = 1.0;

	m_M3d_ptr->mat[n].d3dm.Specular.r = 1.0;
	m_M3d_ptr->mat[n].d3dm.Specular.g = 1.0;
	m_M3d_ptr->mat[n].d3dm.Specular.b = 1.0;
	m_M3d_ptr->mat[n].d3dm.Specular.a = 1.0;

	m_M3d_ptr->mat[n].d3dm.Power = 10;
	}
	
	M3d_objectfindreset(m_M3d_ptr);
	while((n = M3d_objectfind("scoreboard_text"))!=-1) // Main part of ball is black
	{
	m_M3d_ptr->mat[n].d3dm.Diffuse.r = 1.0;
	m_M3d_ptr->mat[n].d3dm.Diffuse.g = 1.0;
	m_M3d_ptr->mat[n].d3dm.Diffuse.b = 1.0;
	m_M3d_ptr->mat[n].d3dm.Diffuse.a = 1.0;
	}

	M3d_objectfindreset(m_M3d_ptr);
	while((n = M3d_objectfind("chair_frame"))!=-1) // Main part of ball is black
	{
	m_M3d_ptr->mat[n].d3dm.Diffuse.r = 0.1;
	m_M3d_ptr->mat[n].d3dm.Diffuse.g = 0.1;
	m_M3d_ptr->mat[n].d3dm.Diffuse.b = 0.1;
	m_M3d_ptr->mat[n].d3dm.Diffuse.a = 1.0;

	m_M3d_ptr->mat[n].d3dm.Specular.r = 1.0;
	m_M3d_ptr->mat[n].d3dm.Specular.g = 1.0;
	m_M3d_ptr->mat[n].d3dm.Specular.b = 1.0;
	m_M3d_ptr->mat[n].d3dm.Specular.a = 1.0;
	}
	
	M3d_objectfindreset(m_M3d_ptr);
	while((n = M3d_objectfind("CueBack"))!=-1) // Main part of ball is black
	{
	m_M3d_ptr->mat[n].d3dm.Diffuse.r = 0.1;
	m_M3d_ptr->mat[n].d3dm.Diffuse.g = 0.1;
	m_M3d_ptr->mat[n].d3dm.Diffuse.b = 0.1;
	m_M3d_ptr->mat[n].d3dm.Diffuse.a = 1.0;

	m_M3d_ptr->mat[n].d3dm.Specular.r = 0.8;
	m_M3d_ptr->mat[n].d3dm.Specular.g = 0.8;
	m_M3d_ptr->mat[n].d3dm.Specular.b = 0.8;
	m_M3d_ptr->mat[n].d3dm.Specular.a = 1.0;
	}

	M3d_objectfindreset(m_M3d_ptr);
	while((n = M3d_objectfind("Cue"))!=-1) // Main part of ball is black
	{
	m_M3d_ptr->mat[n].d3dm.Diffuse.r = 0.95;
	m_M3d_ptr->mat[n].d3dm.Diffuse.g = 0.70;
	m_M3d_ptr->mat[n].d3dm.Diffuse.b = 0.30;
	m_M3d_ptr->mat[n].d3dm.Diffuse.a = 1.0;

	m_M3d_ptr->mat[n].d3dm.Specular.r = 0.8;
	m_M3d_ptr->mat[n].d3dm.Specular.g = 0.8;
	m_M3d_ptr->mat[n].d3dm.Specular.b = 0.8;
	m_M3d_ptr->mat[n].d3dm.Specular.a = 1.0;
	}

	M3d_objectfindreset(m_M3d_ptr);
	while((n = M3d_objectfind("CueMiddle"))!=-1) // Main part of ball is black
	{
	m_M3d_ptr->mat[n].d3dm.Diffuse.r = 0.95;
	m_M3d_ptr->mat[n].d3dm.Diffuse.g = 0.70;
	m_M3d_ptr->mat[n].d3dm.Diffuse.b = 0.30;
	m_M3d_ptr->mat[n].d3dm.Diffuse.a = 1.0;

	m_M3d_ptr->mat[n].d3dm.Specular.r = 0.8;
	m_M3d_ptr->mat[n].d3dm.Specular.g = 0.8;
	m_M3d_ptr->mat[n].d3dm.Specular.b = 0.8;
	m_M3d_ptr->mat[n].d3dm.Specular.a = 1.0;
	}


	M3d_objectfindreset(m_M3d_ptr);
	while((n = M3d_objectfind("window_frame"))!=-1) // Main part of ball is black
	{
	m_M3d_ptr->mat[n].d3dm.Specular.r = 0.0;
	m_M3d_ptr->mat[n].d3dm.Specular.g = 0.0;
	m_M3d_ptr->mat[n].d3dm.Specular.b = 0.0;
	m_M3d_ptr->mat[n].d3dm.Specular.a = 1.0;
	m_M3d_ptr->mat[n].d3dm.Diffuse.r = 0.8;
	m_M3d_ptr->mat[n].d3dm.Diffuse.g = 0.8;
	m_M3d_ptr->mat[n].d3dm.Diffuse.b = 0.8;
	m_M3d_ptr->mat[n].d3dm.Diffuse.a = 1.0;
	}

	M3d_objectfindreset(m_M3d_ptr);
	while((n = M3d_objectfind("window_glass"))!=-1) // Main part of ball is black
	{
	m_M3d_ptr->mat[n].d3dm.Specular.r = 0.0;
	m_M3d_ptr->mat[n].d3dm.Specular.g = 0.0;
	m_M3d_ptr->mat[n].d3dm.Specular.b = 0.0;
	m_M3d_ptr->mat[n].d3dm.Specular.a = 1.0;
	
	m_M3d_ptr->mat[n].d3dm.Diffuse.r = 0.6;
	m_M3d_ptr->mat[n].d3dm.Diffuse.g = 0.6;
	m_M3d_ptr->mat[n].d3dm.Diffuse.b = 0.6;
	m_M3d_ptr->mat[n].d3dm.Diffuse.a = 1.0;
	
	}


	M3d_objectfindreset(m_M3d_ptr);
	while((n = M3d_objectfind("entrance_side1"))!=-1) // Main part of ball is black
	{
	m_M3d_ptr->mat[n].d3dm.Ambient.r = 0.4;
	m_M3d_ptr->mat[n].d3dm.Ambient.g = 0.4;
	m_M3d_ptr->mat[n].d3dm.Ambient.b = 0.4;
	m_M3d_ptr->mat[n].d3dm.Ambient.a = 1.0;
	}

	M3d_objectfindreset(m_M3d_ptr);
	while((n = M3d_objectfind("SnookerTableFrameSide"))!=-1) // Main part of ball is black
	{
	m_M3d_ptr->mat[n].d3dm.Diffuse.r = 0.1;
	m_M3d_ptr->mat[n].d3dm.Diffuse.g = 0.06;
	m_M3d_ptr->mat[n].d3dm.Diffuse.b = 0.01;
	m_M3d_ptr->mat[n].d3dm.Diffuse.a = 1.0;

	m_M3d_ptr->mat[n].d3dm.Specular.r = 0.0;
	m_M3d_ptr->mat[n].d3dm.Specular.g = 0.0;
	m_M3d_ptr->mat[n].d3dm.Specular.b = 0.0;
	m_M3d_ptr->mat[n].d3dm.Specular.a = 1.0;
	
	}


	
	M3d_objectfindreset(m_M3d_ptr);
	while((n = M3d_objectfind("bin_shadow"))!=-1) // Main part of ball is black
	{
	m_M3d_ptr->mat[n].d3dm.Ambient.r = 0.0;
	m_M3d_ptr->mat[n].d3dm.Ambient.g = 0.0;
	m_M3d_ptr->mat[n].d3dm.Ambient.b = 0.0;
	m_M3d_ptr->mat[n].d3dm.Ambient.a = 0.4;
	m_M3d_ptr->mat[n].d3dm.Diffuse.r = 0.0;
	m_M3d_ptr->mat[n].d3dm.Diffuse.g = 0.0;
	m_M3d_ptr->mat[n].d3dm.Diffuse.b = 0.0;
	m_M3d_ptr->mat[n].d3dm.Diffuse.a = 0.4;
	m_M3d_ptr->mat[n].d3dm.Emissive.r = 0.0;
	m_M3d_ptr->mat[n].d3dm.Emissive.g = 0.0;
	m_M3d_ptr->mat[n].d3dm.Emissive.b = 0.0;
	m_M3d_ptr->mat[n].d3dm.Emissive.a = 0.4;
	m_M3d_ptr->mat[n].d3dm.Specular.r = 0.0;
	m_M3d_ptr->mat[n].d3dm.Specular.g = 0.0;
	m_M3d_ptr->mat[n].d3dm.Specular.b = 0.0;
	m_M3d_ptr->mat[n].d3dm.Specular.a = 0.4;
	
	}


	

	// #################################################################################################################
}


void CCustomTable::PoolTableModifyMaterials(M3d_object *m_M3d_Table_ptr)
{
	// Modify Table Mesh/Materials/Textures ############################################################################
	int i;
	int n;
	//M3d_object *m_M3d_Table_ptr;
	//m_M3d_Table_ptr = &m_M3d_Table;

	for(n=0;n<m_M3d_Table_ptr->mesh_n;n++)
	{
	if(m_M3d_Table_ptr->text_ref[n]>32767);
	else {
	if(m_M3d_Table_ptr->text_type[m_M3d_Table_ptr->text_ref[n]]&M3D_THEAD_MAP_DIFFUSE)
		{
			m_M3d_Table_ptr->mat[n].d3dm.Diffuse.r = 1.0;
			m_M3d_Table_ptr->mat[n].d3dm.Diffuse.g = 1.0;
			m_M3d_Table_ptr->mat[n].d3dm.Diffuse.b = 1.0;
		}
	}

	m_M3d_Table_ptr->mat[n].d3dm.Power = 100;//D3D_MATERIAL_POWER;

	m_M3d_Table_ptr->mat[n].d3dm.Specular.r = 3; // 0.2 orig
	m_M3d_Table_ptr->mat[n].d3dm.Specular.g = 3;
	m_M3d_Table_ptr->mat[n].d3dm.Specular.b = 3;
	m_M3d_Table_ptr->mat[n].d3dm.Specular.a = 1.0f;

	m_M3d_Table_ptr->mat[n].d3dm.Emissive.r = 0.0;
	m_M3d_Table_ptr->mat[n].d3dm.Emissive.g = 0.0;
	m_M3d_Table_ptr->mat[n].d3dm.Emissive.b = 0.0;
	m_M3d_Table_ptr->mat[n].d3dm.Emissive.a = 1.0;

	m_M3d_Table_ptr->mat[n].d3dm.Ambient.r*=0.2;
	m_M3d_Table_ptr->mat[n].d3dm.Ambient.g*=0.2;
	m_M3d_Table_ptr->mat[n].d3dm.Ambient.b*=0.2;
	//m_M3d_Table_ptr->mat[n].d3dm.Ambient.a*=0.2;
	}

	ChangeColorScheme(0, m_M3d_Table_ptr); // [+] Retrieve from a save file // Default Blue tableColour = 0;

	M3d_objectfindreset(m_M3d_Table_ptr);
	while((i = M3d_objectfind("pocmet"))!=-1)
	{
	m_M3d_Table_ptr->mat[i].d3dm.Specular.r = 0.7; // 0.2 orig
	m_M3d_Table_ptr->mat[i].d3dm.Specular.g = 0.7;
	m_M3d_Table_ptr->mat[i].d3dm.Specular.b = 0.7;
	m_M3d_Table_ptr->mat[i].d3dm.Specular.a = 1.0f;
	m_M3d_Table_ptr->mat[i].d3dm.Power = 30;

	m_M3d_Table_ptr->mat[i].d3dm.Emissive.r = 0.1;
	m_M3d_Table_ptr->mat[i].d3dm.Emissive.g = 0.1;
	m_M3d_Table_ptr->mat[i].d3dm.Emissive.b = 0.1;
	m_M3d_Table_ptr->mat[i].d3dm.Emissive.a = 1.0;
	}



	M3d_objectfindreset(m_M3d_Table_ptr);
/*	
	while((i = M3d_objectfind("woodtop"))!=-1) // woodtop# & frameside#
	{
	if(g_bGfxQuality==TRUE) // High Quality.
		{
		m_M3d_Table_ptr->mat[i].d3dm.Diffuse.r = 1.0;
		m_M3d_Table_ptr->mat[i].d3dm.Diffuse.g = 1.0;
		m_M3d_Table_ptr->mat[i].d3dm.Diffuse.b = 1.0;
		}
	m_M3d_Table_ptr->mat[i].d3dm.Diffuse.a = 1;

	m_M3d_Table_ptr->mat[i].d3dm.Specular.r=0.7;
	m_M3d_Table_ptr->mat[i].d3dm.Specular.g=0.7;
	m_M3d_Table_ptr->mat[i].d3dm.Specular.b=0.7;
	m_M3d_Table_ptr->mat[i].d3dm.Specular.a=1;

	m_M3d_Table_ptr->mat[i].d3dm.Ambient.r=0;
	m_M3d_Table_ptr->mat[i].d3dm.Ambient.g=0;
	m_M3d_Table_ptr->mat[i].d3dm.Ambient.b=0;
	m_M3d_Table_ptr->mat[i].d3dm.Ambient.a=0;

	m_M3d_Table_ptr->mat[i].d3dm.Power = 11;
	}

*/
	// #################################################################################################################
}



void CCustomTable::PoolTableModifyMaterialsUKPool(M3d_object *m_M3d_Table_ptr)
{
	// Modify Table Mesh/Materials/Textures ############################################################################
	int i;
	int n;

	char spec_on = 1;

	if(g_bGfxQuality==FALSE) spec_on = 0;
	//M3d_object *m_M3d_Table_ptr;
	//m_M3d_Table_ptr = &m_M3d_Table;

	for(n=0;n<m_M3d_Table_ptr->mesh_n;n++)
	{
	if(m_M3d_Table_ptr->text_ref[n]>32767);
	else {
	if(m_M3d_Table_ptr->text_type[m_M3d_Table_ptr->text_ref[n]]&M3D_THEAD_MAP_DIFFUSE)
		{
			m_M3d_Table_ptr->mat[n].d3dm.Diffuse.r = 1.0;
			m_M3d_Table_ptr->mat[n].d3dm.Diffuse.g = 1.0;
			m_M3d_Table_ptr->mat[n].d3dm.Diffuse.b = 1.0;
		}
	}

	if(!spec_on) m_M3d_Table_ptr->mat[n].d3dm.Power = 0;//D3D_MATERIAL_POWER;
	else m_M3d_Table_ptr->mat[n].d3dm.Power = 20;//D3D_MATERIAL_POWER;

	
	m_M3d_Table_ptr->mat[n].d3dm.Specular.r = 0.0f; // 0.2 orig
	m_M3d_Table_ptr->mat[n].d3dm.Specular.g = 0.0f;
	m_M3d_Table_ptr->mat[n].d3dm.Specular.b = 0.0f;
	m_M3d_Table_ptr->mat[n].d3dm.Specular.a = 1.0f;

	m_M3d_Table_ptr->mat[n].d3dm.Emissive.r = 0;
	m_M3d_Table_ptr->mat[n].d3dm.Emissive.g = 0;
	m_M3d_Table_ptr->mat[n].d3dm.Emissive.b = 0;
	m_M3d_Table_ptr->mat[n].d3dm.Emissive.a = 0;



	m_M3d_Table_ptr->mat[n].d3dm.Ambient.r*=0.2;
	m_M3d_Table_ptr->mat[n].d3dm.Ambient.g*=0.2;
	m_M3d_Table_ptr->mat[n].d3dm.Ambient.b*=0.2;
	m_M3d_Table_ptr->mat[n].d3dm.Ambient.a*=0.2;
	}

	ChangeColorScheme(0, m_M3d_Table_ptr); // [+] Retrieve from a save file

	M3d_objectfindreset(m_M3d_Table_ptr);
	while((i = M3d_objectfind("pocmet"))!=-1)
	{
	if(!spec_on) m_M3d_Table_ptr->mat[i].d3dm.Power = 0;
		else m_M3d_Table_ptr->mat[i].d3dm.Power = 0;
	}


	M3d_objectfindreset(m_M3d_Table_ptr);

	while((i = M3d_objectfind("Pocket"))!=-1) // woodtop# & frameside#
	{
	if(!spec_on)
		{
		m_M3d_Table_ptr->mat[i].d3dm.Specular.r=0;
		m_M3d_Table_ptr->mat[i].d3dm.Specular.g=0;
		m_M3d_Table_ptr->mat[i].d3dm.Specular.b=0;
		m_M3d_Table_ptr->mat[i].d3dm.Specular.a=1;
		m_M3d_Table_ptr->mat[i].d3dm.Power = 0;
		}
	else{
		m_M3d_Table_ptr->mat[i].d3dm.Specular.r=1;
		m_M3d_Table_ptr->mat[i].d3dm.Specular.g=1;
		m_M3d_Table_ptr->mat[i].d3dm.Specular.b=1;
		m_M3d_Table_ptr->mat[i].d3dm.Specular.a=1;
		m_M3d_Table_ptr->mat[i].d3dm.Power = 40;	
		}
	
	}

	M3d_objectfindreset(m_M3d_Table_ptr);
	while((i = M3d_objectfind("Slate"))!=-1) // woodtop# & frameside#
	{
	m_M3d_Table_ptr->mat[i].d3dm.Specular.r=0;
	m_M3d_Table_ptr->mat[i].d3dm.Specular.g=0;
	m_M3d_Table_ptr->mat[i].d3dm.Specular.b=0;
	m_M3d_Table_ptr->mat[i].d3dm.Specular.a=1;

	m_M3d_Table_ptr->mat[i].d3dm.Diffuse.r*=0.8;
	m_M3d_Table_ptr->mat[i].d3dm.Diffuse.g*=0.8;
	m_M3d_Table_ptr->mat[i].d3dm.Diffuse.b*=0.8;
	m_M3d_Table_ptr->mat[i].d3dm.Diffuse.a=1;


	if(!spec_on) m_M3d_Table_ptr->mat[i].d3dm.Power = 0;
		else m_M3d_Table_ptr->mat[i].d3dm.Power = 60;
	}

		M3d_objectfindreset(m_M3d_Table_ptr);
	while((i = M3d_objectfind("CushionWood"))!=-1) // woodtop# & frameside#
	{
	if(!spec_on)
		{
		m_M3d_Table_ptr->mat[i].d3dm.Specular.r=0.0;
		m_M3d_Table_ptr->mat[i].d3dm.Specular.g=0.0;
		m_M3d_Table_ptr->mat[i].d3dm.Specular.b=0.0;
		m_M3d_Table_ptr->mat[i].d3dm.Specular.a=1.0;
		m_M3d_Table_ptr->mat[i].d3dm.Power = 0;
		}
	else{
		m_M3d_Table_ptr->mat[i].d3dm.Specular.r=0.3;
		m_M3d_Table_ptr->mat[i].d3dm.Specular.g=0.3;
		m_M3d_Table_ptr->mat[i].d3dm.Specular.b=0.3;
		m_M3d_Table_ptr->mat[i].d3dm.Specular.a=1.0;
		m_M3d_Table_ptr->mat[i].d3dm.Power = 30;
		}

	m_M3d_Table_ptr->mat[i].d3dm.Emissive.r = 0.3;
	m_M3d_Table_ptr->mat[i].d3dm.Emissive.g = 0.3;
	m_M3d_Table_ptr->mat[i].d3dm.Emissive.b = 0.3;
	m_M3d_Table_ptr->mat[i].d3dm.Emissive.a = 0.3;
	}

	M3d_objectfindreset(m_M3d_Table_ptr);
	while((i = M3d_objectfind("PocketCase"))!=-1) // woodtop# & frameside#
	{
	if(!spec_on)
		{
		m_M3d_Table_ptr->mat[i].d3dm.Specular.r=0.0;
		m_M3d_Table_ptr->mat[i].d3dm.Specular.g=0.0;
		m_M3d_Table_ptr->mat[i].d3dm.Specular.b=0.0;
		m_M3d_Table_ptr->mat[i].d3dm.Specular.a=1.0;
		m_M3d_Table_ptr->mat[i].d3dm.Power = 0;
		}
	else{
		m_M3d_Table_ptr->mat[i].d3dm.Ambient.r = 0.1;
		m_M3d_Table_ptr->mat[i].d3dm.Ambient.g = 0.1;
		m_M3d_Table_ptr->mat[i].d3dm.Ambient.b = 0.1;
		m_M3d_Table_ptr->mat[i].d3dm.Ambient.a = 1.0;

		m_M3d_Table_ptr->mat[i].d3dm.Specular.r=0.7;
		m_M3d_Table_ptr->mat[i].d3dm.Specular.g=0.7;
		m_M3d_Table_ptr->mat[i].d3dm.Specular.b=0.7;
		m_M3d_Table_ptr->mat[i].d3dm.Specular.a=1.0;
		m_M3d_Table_ptr->mat[i].d3dm.Power = 40;
		}
	}

	
	// #################################################################################################################
}


void CCustomTable::PoolTableChange(M3d_object *m_M3d_Table_ptr, int type)
{	
	#ifdef THE_POOL_CLUB // iPool

	// point snooker table mesh to chosen custom table mesh
	m_pParent->m_M3d_Table_ptr = m_M3d_Table_ptr;//&m_M3d_caromTable;//m_customTable->m_M3d_Table;
	m_tableType = type;
	
	// table bounds for line (default)
	m_pParent->m_tableBound.minX = -1500;
	m_pParent->m_tableBound.maxX = 1500;
	m_pParent->m_tableBound.minY = -500;
	m_pParent->m_tableBound.maxY = 500;

	// remove all pockets (default)
	ClearPockets();

	// original pool table
	if(m_pParent->m_M3d_Table_ptr == &m_M3d_Table)
	{
		// create pockets
		AddPocket(CVector((-7+253*5+BALL_RADIUS*1.25), (+7-126.5*5-BALL_RADIUS*1),  (0)),  POCKET_BLACKG, BALL_RADIUS*1.4, BALL_RADIUS*2.4, 0.05);	// BLACK-G
		AddPocket(CVector((+253*5+BALL_RADIUS*1.25),   (+126.5*5+BALL_RADIUS*1),    (0)),  POCKET_BLACKY, BALL_RADIUS*1.4, BALL_RADIUS*2.4, 0.05);	// BLACK-Y
		AddPocket(CVector((0),						   (+10-126.5*5-BALL_RADIUS*3), (0)),  POCKET_BLUEG, BALL_RADIUS*1.4, BALL_RADIUS*2.4, 0.05 );	// BLUE-G
		AddPocket(CVector((0),						   (+126.5*5+BALL_RADIUS*3),    (0)),  POCKET_BLUEY, BALL_RADIUS*1.4, BALL_RADIUS*2.4, 0.05 );	// BLUE-Y
		AddPocket(CVector((+7-253*5-BALL_RADIUS*1.25), (+7-126.5*5-BALL_RADIUS*1),  (0)),  POCKET_GREEN, BALL_RADIUS*1.4, BALL_RADIUS*2.4, 0.05 );	// Green
		AddPocket(CVector((-253*5-BALL_RADIUS*1.25),   (+126.5*5+BALL_RADIUS*1),    (0)),  POCKET_YELLOW, BALL_RADIUS*1.4, BALL_RADIUS*2.4, 0.05 );	// Yellow	
	}
	// carom-style pool table
	else if(m_pParent->m_M3d_Table_ptr == &m_M3d_caromTable)
	{
		if(m_tableType == 3) // center hole
		{
			// create center spot pocket
			AddPocket(CVector((0), (0), (0)), POCKET_OBJECT, BALL_RADIUS_INITIAL*1.4/*BALL_RADIUS*1.4*/, BALL_RADIUS_INITIAL*2.4/*BALL_RADIUS*2.4*//**4*/, 0.05 );
		}
		else if(m_tableType == 4) // black hole
		{
			// create black-spot pocket
			AddPocket(CVector((0), (0), (0)), POCKET_OBJECT, BALL_RADIUS_INITIAL*1.4/*BALL_RADIUS*1.4*/, BALL_RADIUS_INITIAL*2.4*4/*BALL_RADIUS*2.4*4*/, 0.05 );
		}
	}
	// pro pool table
	else if(m_pParent->m_M3d_Table_ptr == &m_M3d_proTable)
	{
				

		// create pockets
		AddPocket(CVector((-7+253*5+BALL_RADIUS*1.25), (+7-126.5*5-BALL_RADIUS*1),  (0)),  POCKET_BLACKG, BALL_RADIUS*1.4, BALL_RADIUS*2.4, 0.05);	// BLACK-G
		AddPocket(CVector((+253*5+BALL_RADIUS*1.25),   (+126.5*5+BALL_RADIUS*1),    (0)),  POCKET_BLACKY, BALL_RADIUS*1.4, BALL_RADIUS*2.4, 0.05);	// BLACK-Y
		AddPocket(CVector((0),						   (+10-126.5*5-BALL_RADIUS*3), (0)),  POCKET_BLUEG, BALL_RADIUS*1.4, BALL_RADIUS*2.4, 0.05 );	// BLUE-G
		AddPocket(CVector((0),						   (+126.5*5+BALL_RADIUS*3),    (0)),  POCKET_BLUEY, BALL_RADIUS*1.4, BALL_RADIUS*2.4, 0.05 );	// BLUE-Y
		AddPocket(CVector((+7-253*5-BALL_RADIUS*1.25), (+7-126.5*5-BALL_RADIUS*1),  (0)),  POCKET_GREEN, BALL_RADIUS*1.4, BALL_RADIUS*2.4, 0.05 );	// Green
		AddPocket(CVector((-253*5-BALL_RADIUS*1.25),   (+126.5*5+BALL_RADIUS*1),    (0)),  POCKET_YELLOW, BALL_RADIUS*1.4, BALL_RADIUS*2.4, 0.05 );	// Yellow	
	}
		// English Pool UK table
	else if(m_pParent->m_M3d_Table_ptr == &m_M3d_ukTable)
	{
		// create pockets
		AddPocket(CVector((+13+(231.11)*5+BALL_RADIUS*1.25), (-13-(109.67)*5-BALL_RADIUS*1),  (0)),  POCKET_BLACKG, BALL_RADIUS*1.7, BALL_RADIUS*2.6, 0.05);	// BLACK-G
		AddPocket(CVector((+13+(231.11)*5+BALL_RADIUS*1.25), (+13+(109.67)*5+BALL_RADIUS*1),    (0)),  POCKET_BLACKY, BALL_RADIUS*1.7, BALL_RADIUS*2.6, 0.05);	// BLACK-Y
		AddPocket(CVector((0),								(+18-(109.67)*5-BALL_RADIUS*3), (0)),  POCKET_BLUEG, BALL_RADIUS*1.4, BALL_RADIUS*2.4, 0.05 );	// BLUE-G
		AddPocket(CVector((0),								(-18+(109.67)*5+BALL_RADIUS*3),    (0)),  POCKET_BLUEY, BALL_RADIUS*1.4, BALL_RADIUS*2.4, 0.05 );	// BLUE-Y
		AddPocket(CVector((-13-(231.11)*5-BALL_RADIUS*1.25), (-13-(109.67)*5-BALL_RADIUS*1),  (0)),  POCKET_GREEN, BALL_RADIUS*1.7, BALL_RADIUS*2.6, 0.05 );	// Green
		AddPocket(CVector((-13-(231.11)*5-BALL_RADIUS*1.25), (+13+(109.67)*5+BALL_RADIUS*1),    (0)),  POCKET_YELLOW, BALL_RADIUS*1.7, BALL_RADIUS*2.6, 0.05 );	// Yellow	
	}

	// Setup Lighting Position
	//((CGameAppView *)m_pParent->m_parent)->m_environment->LightPos(0,-1250,0,600);
	//((CGameAppView *)m_pParent->m_parent)->m_environment->LightPos(1,0,0,600);
	//((CGameAppView *)m_pParent->m_parent)->m_environment->LightPos(2,+1250,0,600);	

	// re-Extract lines //
	RenewObstacleCourse();
	BuildObstacleCourse(m_pParent->m_M3d_Table_ptr);

	#endif //THE_POOL_CLUB // iPool
}

M3d_object *CCustomTable::PoolTableGetType(int tableTypeIdx)
{
	if(tableTypeIdx < 0) return 0; // error

	return m_M3d_Table_ptr[tableTypeIdx];
}

void CCustomTable::ReNormalizeBalls(void)
{
	M3d_object *Ball_ptr;
	Ball_ptr = m_M3d_Ball; // Start from first ball
	
	M3d_denormalizationFactor = 0.1 * g_ballRadiusFactor;

	/// *NEW* DRAW BALLS 1.5% BIGGER
	M3d_denormalizationFactor *= 1.015f;
	/////////////////

	//for(int n=0;n<32;n++)
	while(Ball_ptr!=m_M3d_Ball_end)
	{
		if(g_bGfxQuality==FALSE) // Low Quality.
		M3d_normalizeobjectsphere(Ball_ptr);
		else M3d_normalizeobject(Ball_ptr); // HQ
		Ball_ptr++;
	}
}



void CCustomTable::Referee(DWORD msg, void *pData)
{
	if(msg == CTMSG_LINESET) {
		m_pParent->m_targetBall = -1;	// clear target ball
		m_pParent->m_targetPocket = -1; // clear pocket select
		m_pParent->m_bTargetSuccess = FALSE;

	}

	switch(m_type)
	{
		case CTTYPE_SNOOKER:
		case CTTYPE_SNOOKER10:
		case CTTYPE_SNOOKER6:
		case CTTYPE_RESPOTBLACK:
			RefereeSnooker(msg, pData);
			break;
		case CTTYPE_BILLIARDS:
			RefereeBilliards(msg, pData);
			break;
		case CTTYPE_8BALL:			
			Referee8Ball(msg, pData);
			break;
		case CTTYPE_9BALL:
			Referee9Ball(msg, pData);
			break;
		case CTTYPE_ROTATIONBALL:
			RefereeRotationBall(msg, pData);
			break;
		case CTTYPE_BOWLS:
			RefereeBowls(msg, pData);
			break;
		case CTTYPE_6BALL:
			Referee6Ball(msg, pData);
			break;
		case CTTYPE_10BALL:
			Referee10Ball(msg, pData);
			break;
		case CTTYPE_UKPOOL:
			RefereeUKPool(msg, pData);
			break;
		case CTTYPE_BREAK:
			RefereeBreak(msg, pData);
			break;
	}
	

}
	


void CCustomTable::RefereeBilliards(DWORD msg, void *pData)
{
	if(msg == CTMSG_MOUSEOVER1)
	{
	}
	else if(msg == CTMSG_BALLSELECT)
	{
	}
	else if(msg == CTMSG_LINESET)
	{
		RefereeBilliardsLINESET(msg, pData);
	}
	else if(msg == CTMSG_MOUSEOVER2)
	{
	}
	else if(msg == CTMSG_STRIKE)
	{

	}
	else if(msg == CTMSG_SIMEND)
	{
		Decode();
		m_pParent->RefereeBilliards();	// this is the CSnooker old style code
		Encode();

	}
}




void CCustomTable::RefereeBilliardsLINESET(DWORD msg, void *pData)
{
	ctmsg_LINESET_DATA *ctmsgData;
	ctmsgData = (ctmsg_LINESET_DATA_typ *) pData;

	char SEL_YELLOW = 1;

	// -----------------------------------------------------------------------------
	// check if player has not selected the white ball & we're not in Practice mode
	// -----------------------------------------------------------------------------
	//if(!m_pParent->m_rednext)
	  //if(m_pParent->m_colourseq < YELLOW)
		//if(m_pParent->m_testBall->m_colour == WHITE)
			//if(m_pParent->m_nominateBall == WHITE)
		

		char proceed = 0;
		if(m_pParent->m_bTurn1P && m_pParent->m_selectBall != WHITE) proceed = 1;
		if(!m_pParent->m_bTurn1P && m_pParent->m_selectBall != SEL_YELLOW) proceed = 2;
		
		if(!m_pParent->m_bPracticeMode)	
		if(proceed)
			{ 
			///		
			/// [simulate R-Button down for cancel shot]
			///

			m_pParent->m_bTestBallFinished = FALSE;
			//m_snooker->m_bCueTipReady = FALSE;

			// reset foucus to table (this) window, & reset 'raise butt'
			m_pParent->m_RaiseButt = m_pParent->m_RaiseButtOld = 0;
			m_pParent->m_bReceiveRaiseButtActive = FALSE;
			m_pParent->m_bRaiseButtHelper = TRUE;
			GAMEAPPVIEW2->m_sliderRaiseButt.SetPos(90);
			GAMEAPPVIEW2->PreviousWndEditSetFocus();

			GAMEAPPVIEW2->m_point = ctmsgData->point;
				
			// Only give this window the mouse's full attention 
			GAMEAPPVIEW2->SetCapture();
			
			if(m_pParent->m_tableMouseOn)
			{
				if(m_pParent->m_bCueState)
				{
					// Now Give ,mouse's attention to all
					if(GAMEAPPVIEW2->GetCapture() == GAMEAPPVIEW2) ReleaseCapture();

					// give mouse full movement
					ClipCursor(&GAMEAPPVIEW2->m_rectCursor);

					// reset mouse cursor position back to start position
					CRect rect;
					GAMEAPPVIEW2->GetWindowRect(&rect);
					SetCursorPos(rect.left + m_pParent->m_sCPoint.x+2, rect.top + m_pParent->m_sCPoint.y+2);
				
					m_pParent->SelectBall(0, ctmsgData->point);
					
					m_pParent->m_point = ctmsgData->point;
					m_pParent->m_sightlineFade = 0;
					
					if(m_pParent->m_bPracticeMode);
					else
						m_pParent->SelectBallSend(0, ctmsgData->point, m_pParent->m_windowWidth, m_pParent->m_windowHeight, m_pParent->m_selectBall);
				}
			}

			if(!m_pParent->m_cueBallSelect)
			if(!GAMEAPPVIEW2->m_bShowCursor)
			{
				GAMEAPPVIEW2->m_bShowCursor = TRUE;
				SetCursor(GAMEAPPVIEW2->m_hCursor);
				ShowCursor(GAMEAPPVIEW2->m_bShowCursor); // show cursor
			}

			///
			///
			///

			if(proceed == 1) GAMEAPPVIEW2->AlertMessage("Warning! You can only strike the 'White' ball!\n\n(Use 'Left-Mouse-Button' to select the 'White' ball)");
			else if(proceed == 2) GAMEAPPVIEW2->AlertMessage("Warning! You can only strike the 'Yellow' ball!\n\n(Use 'Left-Mouse-Button' to select the 'Yellow' ball)");
			else;
			return;
		}

	// ---------------------------------------------
	// normal 'LineSet' here!
	// ---------------------------------------------

	// Now Give ,mouse's attention to all
	if(GAMEAPPVIEW2->GetCapture() == GAMEAPPVIEW2) ReleaseCapture();

	
	// test!!! send final cue-line pos
	GAMEAPPVIEW2->SendPlayerMouseMove(m_pParent->m_mouseX, m_pParent->m_mouseY, 
							m_pParent->m_windowWidth, m_pParent->m_windowHeight,
							m_pParent->m_strikeLine, m_pParent->m_strikeDist);

	m_pParent->m_cueBallSelect = 5; //--
	m_pParent->m_mouseX = 0; // position cuetip GFX pointer at centre of selected ball
	m_pParent->m_mouseY = 0;

	
	GAMEAPPVIEW2->m_sliderRaiseButt.EnableWindow(TRUE);
	m_pParent->m_bRaiseButtHelper = TRUE;

	// give mouse full movement
	ClipCursor(&GAMEAPPVIEW2->m_rectCursor);

	if(!GAMEAPPVIEW2->m_bShowCursor)
	{
		GAMEAPPVIEW2->m_bShowCursor = TRUE;
		SetCursor(GAMEAPPVIEW2->m_hCursor);
		ShowCursor(GAMEAPPVIEW2->m_bShowCursor); // show cursor
	}

	// reset mouse cursor position back to start position
	CRect rect;
	GAMEAPPVIEW2->GetWindowRect(&rect);
	SetCursorPos(rect.left + m_pParent->m_sCPoint.x+2, rect.top + m_pParent->m_sCPoint.y+2);

	m_pParent->m_bCueLineSet = TRUE;
	m_pParent->m_tableMousePosStrike[0] = m_pParent->m_tableMousePos[0];
	m_pParent->m_tableMousePosStrike[1] = m_pParent->m_tableMousePos[1];
	m_pParent->m_tableMousePosStrike[2] = m_pParent->m_tableMousePos[2];
}


			  
void CCustomTable::RefereeSnooker(DWORD msg, void *pData)
{
	if(msg == CTMSG_MOUSEOVER1)
	{
	}
	else if(msg == CTMSG_BALLSELECT)
	{
	}
	else if(msg == CTMSG_LINESET)
	{
		RefereeSnookerLINESET(msg, pData);
	}
	else if(msg == CTMSG_MOUSEOVER2)
	{
	}
	else if(msg == CTMSG_STRIKE)
	{

	}
	else if(msg == CTMSG_SIMEND)
	{
		Decode();
		m_pParent->Referee();	// this is the CSnooker old style code
		Encode();
	}
}

void CCustomTable::RefereeSnookerLINESET(DWORD msg, void *pData)
{
	ctmsg_LINESET_DATA *ctmsgData;
	ctmsgData = (ctmsg_LINESET_DATA_typ *) pData;

	// -----------------------------------------------------------------------------
	// check if player has not selected the white ball & we're not in Practice mode
	// -----------------------------------------------------------------------------
	//if(!m_pParent->m_rednext)
	  //if(m_pParent->m_colourseq < YELLOW)
		//if(m_pParent->m_testBall->m_colour == WHITE)
			//if(m_pParent->m_nominateBall == WHITE)
		if(!m_pParent->m_bPracticeMode)			
			if(m_pParent->m_selectBall != WHITE)
			{
				///		
				/// [simulate R-Button down for cancel shot]
				///

				m_pParent->m_bTestBallFinished = FALSE;
				//m_snooker->m_bCueTipReady = FALSE;

				// reset foucus to table (this) window, & reset 'raise butt'
				m_pParent->m_RaiseButt = m_pParent->m_RaiseButtOld = 0;
				m_pParent->m_bReceiveRaiseButtActive = FALSE;
				m_pParent->m_bRaiseButtHelper = TRUE;
				GAMEAPPVIEW2->m_sliderRaiseButt.SetPos(90);
				GAMEAPPVIEW2->PreviousWndEditSetFocus();

				GAMEAPPVIEW2->m_point = ctmsgData->point;
					
				// Only give this window the mouse's full attention 
				GAMEAPPVIEW2->SetCapture();
				
				if(m_pParent->m_tableMouseOn)
				{
					if(m_pParent->m_bCueState)
					{
						// Now Give ,mouse's attention to all
						if(GAMEAPPVIEW2->GetCapture() == GAMEAPPVIEW2) ReleaseCapture();

						// give mouse full movement
						ClipCursor(&GAMEAPPVIEW2->m_rectCursor);

						// reset mouse cursor position back to start position
						CRect rect;
						GAMEAPPVIEW2->GetWindowRect(&rect);
						SetCursorPos(rect.left + m_pParent->m_sCPoint.x+2, rect.top + m_pParent->m_sCPoint.y+2);
					
						m_pParent->SelectBall(0, ctmsgData->point);
						
						m_pParent->m_point = ctmsgData->point;
						m_pParent->m_sightlineFade = 0;
						
						if(m_pParent->m_bPracticeMode);
						else
							m_pParent->SelectBallSend(0, ctmsgData->point, m_pParent->m_windowWidth, m_pParent->m_windowHeight, m_pParent->m_selectBall);
					}
				}

				if(!m_pParent->m_cueBallSelect)
				if(!GAMEAPPVIEW2->m_bShowCursor)
				{
					GAMEAPPVIEW2->m_bShowCursor = TRUE;
					SetCursor(GAMEAPPVIEW2->m_hCursor);
					ShowCursor(GAMEAPPVIEW2->m_bShowCursor); // show cursor
				}

				///
				///
				///

				GAMEAPPVIEW2->AlertMessage("Warning! You can only strike the 'White' ball!\n\n(Use 'Left-Mouse-Button' to select the 'White' ball)");
				return;
			}

	// ------------------------------------------------------------------------------
	// check if player has not nominated a colour (only when there is no colour line)
	// ------------------------------------------------------------------------------
	if(!m_pParent->m_rednext)
	  if(m_pParent->m_colourseq < YELLOW)
		//if(m_pParent->m_testBall->m_colour == WHITE)
		if(m_pParent->m_testBall->m_colour == WHITE || m_pParent->m_testBall->m_colour == RED) //NEW
			if(m_pParent->m_nominateBall == WHITE)
			{
				///		
				/// [simulate R-Button down for cancel shot]
				///

				m_pParent->m_bTestBallFinished = FALSE;
				//m_snooker->m_bCueTipReady = FALSE;

				// reset foucus to table (this) window, & reset 'raise butt'
				m_pParent->m_RaiseButt = m_pParent->m_RaiseButtOld = 0;
				m_pParent->m_bReceiveRaiseButtActive = FALSE;
				m_pParent->m_bRaiseButtHelper = TRUE;
				GAMEAPPVIEW2->m_sliderRaiseButt.SetPos(90);
				GAMEAPPVIEW2->PreviousWndEditSetFocus();

				GAMEAPPVIEW2->m_point = ctmsgData->point;
					
				// Only give this window the mouse's full attention 
				GAMEAPPVIEW2->SetCapture();
				
				if(m_pParent->m_tableMouseOn)
				{
					if(m_pParent->m_bCueState)
					{
						// Now Give ,mouse's attention to all
						if(GAMEAPPVIEW2->GetCapture() == GAMEAPPVIEW2) ReleaseCapture();

						// give mouse full movement
						ClipCursor(&GAMEAPPVIEW2->m_rectCursor);

						// reset mouse cursor position back to start position
						CRect rect;
						GAMEAPPVIEW2->GetWindowRect(&rect);
						SetCursorPos(rect.left + m_pParent->m_sCPoint.x+2, rect.top + m_pParent->m_sCPoint.y+2);
					
						m_pParent->SelectBall(0, ctmsgData->point);
						
						m_pParent->m_point = ctmsgData->point;
						m_pParent->m_sightlineFade = 0;
						
						if(m_pParent->m_bPracticeMode);
						else
							m_pParent->SelectBallSend(0, ctmsgData->point, m_pParent->m_windowWidth, m_pParent->m_windowHeight, m_pParent->m_selectBall);
					}
				}

				if(!m_pParent->m_cueBallSelect)
				if(!GAMEAPPVIEW2->m_bShowCursor)
				{
					GAMEAPPVIEW2->m_bShowCursor = TRUE;
					SetCursor(GAMEAPPVIEW2->m_hCursor);
					ShowCursor(GAMEAPPVIEW2->m_bShowCursor); // show cursor
				}

				///
				///
				///

				if(g_build.addOnSfx && g_gfxDetails.bSoundVoices)
					Msfx_smpplayvol(&GAMEAPPVIEW2->m_game_smp[55], 212); // "please nominate!"
				
				GAMEAPPVIEW2->AlertMessage("Warning! You have not nominated a colour ball!\n\n(Use 'Right-Mouse-Button' to nominate colour)");
				return;
			}

	// ---------------------------------------------
	// normal 'LineSet' here!
	// ---------------------------------------------

	// Now Give ,mouse's attention to all
	if(GAMEAPPVIEW2->GetCapture() == GAMEAPPVIEW2) ReleaseCapture();

	
	// test!!! send final cue-line pos
	GAMEAPPVIEW2->SendPlayerMouseMove(m_pParent->m_mouseX, m_pParent->m_mouseY, 
							m_pParent->m_windowWidth, m_pParent->m_windowHeight,
							m_pParent->m_strikeLine, m_pParent->m_strikeDist);

	m_pParent->m_cueBallSelect = 5; //--
	m_pParent->m_mouseX = 0; // position cuetip GFX pointer at centre of selected ball
	m_pParent->m_mouseY = 0;

	
	GAMEAPPVIEW2->m_sliderRaiseButt.EnableWindow(TRUE);
	m_pParent->m_bRaiseButtHelper = TRUE;

	// give mouse full movement
	ClipCursor(&GAMEAPPVIEW2->m_rectCursor);

	if(!GAMEAPPVIEW2->m_bShowCursor)
	{
		GAMEAPPVIEW2->m_bShowCursor = TRUE;
		SetCursor(GAMEAPPVIEW2->m_hCursor);
		ShowCursor(GAMEAPPVIEW2->m_bShowCursor); // show cursor
	}

	// reset mouse cursor position back to start position
	CRect rect;
	GAMEAPPVIEW2->GetWindowRect(&rect);
	// ::GetWindowRect(g_hwndDX, &rect);
	
	SetCursorPos(rect.left + m_pParent->m_sCPoint.x+2, rect.top + m_pParent->m_sCPoint.y+2);

	m_pParent->m_bCueLineSet = TRUE;
	m_pParent->m_tableMousePosStrike[0] = m_pParent->m_tableMousePos[0];
	m_pParent->m_tableMousePosStrike[1] = m_pParent->m_tableMousePos[1];
	m_pParent->m_tableMousePosStrike[2] = m_pParent->m_tableMousePos[2];
}

void CCustomTable::Referee8Ball(DWORD msg, void *pData)
{
	if(msg == CTMSG_LINESET)
	{
		Referee8BallLINESET(msg, pData);
	}
	else if(msg == CTMSG_POCKETSET)
	{
		Referee8BallPOCKETSET(msg, pData);
	}
	else if(msg == CTMSG_SIMEND)
	{
		Decode();
		Referee8BallSIMEND();	// this is the CSnooker old style code
		Encode();
	}
	else
	{
		RefereeSnooker(msg, pData);
	}
}

void CCustomTable::Referee8BallLINESET(DWORD msg, void *pData)
{
	ctmsg_LINESET_DATA *ctmsgData;
	ctmsgData = (ctmsg_LINESET_DATA_typ *) pData;


	// -----------------------------------------------------------------------------
	// check if player has not selected the white ball & we're not in Practice mode
	// -----------------------------------------------------------------------------
	//if(!m_pParent->m_rednext)
	  //if(m_pParent->m_colourseq < YELLOW)
		//if(m_pParent->m_testBall->m_colour == WHITE)
			//if(m_pParent->m_nominateBall == WHITE)
		if(!m_pParent->m_bPracticeMode)			
			if(m_pParent->m_selectBall != WHITE)
			{
				///		
				/// [simulate R-Button down for cancel shot]
				///

				m_pParent->m_bTestBallFinished = FALSE;
				//m_snooker->m_bCueTipReady = FALSE;

				// reset foucus to table (this) window, & reset 'raise butt'
				m_pParent->m_RaiseButt = m_pParent->m_RaiseButtOld = 0;
				m_pParent->m_bReceiveRaiseButtActive = FALSE;
				m_pParent->m_bRaiseButtHelper = TRUE;
				GAMEAPPVIEW2->m_sliderRaiseButt.SetPos(90);
				GAMEAPPVIEW2->PreviousWndEditSetFocus();

				GAMEAPPVIEW2->m_point = ctmsgData->point;
					
				// Only give this window the mouse's full attention 
				GAMEAPPVIEW2->SetCapture();
				
				if(m_pParent->m_tableMouseOn)
				{
					if(m_pParent->m_bCueState)
					{
						// Now Give ,mouse's attention to all
						if(GAMEAPPVIEW2->GetCapture() == GAMEAPPVIEW2) ReleaseCapture();

						// give mouse full movement
						ClipCursor(&GAMEAPPVIEW2->m_rectCursor);

						// reset mouse cursor position back to start position
						CRect rect;
						GAMEAPPVIEW2->GetWindowRect(&rect);
						SetCursorPos(rect.left + m_pParent->m_sCPoint.x+2, rect.top + m_pParent->m_sCPoint.y+2);
					
						m_pParent->SelectBall(0, ctmsgData->point);
						
						m_pParent->m_point = ctmsgData->point;
						m_pParent->m_sightlineFade = 0;
						
						if(m_pParent->m_bPracticeMode);
						else
							m_pParent->SelectBallSend(0, ctmsgData->point, m_pParent->m_windowWidth, m_pParent->m_windowHeight, m_pParent->m_selectBall);
					}
				}

				if(!m_pParent->m_cueBallSelect)
				if(!GAMEAPPVIEW2->m_bShowCursor)
				{
					GAMEAPPVIEW2->m_bShowCursor = TRUE;
					SetCursor(GAMEAPPVIEW2->m_hCursor);
					ShowCursor(GAMEAPPVIEW2->m_bShowCursor); // show cursor
				}

				///
				///
				///

				GAMEAPPVIEW2->AlertMessage("Warning! You can only strike the 'White' ball!\n\n(Use 'Left-Mouse-Button' to select the 'White' ball)");
				return;
			}


	// ---------------------------------------------
	// normal 'LineSet' here!
	// ---------------------------------------------

	
//	/////////////////////////////////////////////////////////////////////////////////////////////////////
//	// set default target pocket
//	if(m_pParent->m_testBall->m_colour == 8) // if player has lined up the 8-ball
//	{
//		// initially...
//		if(m_pParent->m_targetPocket == -1)
//			m_pParent->m_targetPocket = -2; // default: point at all pockets
//		
//		// send target ball/pocket/success data to all at table
//		GAMEAPPVIEW2->SendPlayerTargetData(m_pParent->m_targetPocket, m_pParent->m_targetBall, m_pParent->m_bTargetSuccess);
//	}
//	/////////////////////////////////////////////////////////////////////////////////////////////////////

	// Now Give ,mouse's attention to all
	if(GAMEAPPVIEW2->GetCapture() == GAMEAPPVIEW2) ReleaseCapture();
			
	// test!!! send final cue-line pos
	GAMEAPPVIEW2->SendPlayerMouseMove(m_pParent->m_mouseX, m_pParent->m_mouseY, 
							m_pParent->m_windowWidth, m_pParent->m_windowHeight,
							m_pParent->m_strikeLine, m_pParent->m_strikeDist);

	m_pParent->m_cueBallSelect = 5; //--
	m_pParent->m_mouseX = 0; // position cuetip GFX pointer at centre of selected ball
	m_pParent->m_mouseY = 0;

	GAMEAPPVIEW2->m_sliderRaiseButt.EnableWindow(TRUE);
	m_pParent->m_bRaiseButtHelper = TRUE;

	// give mouse full movement
	ClipCursor(&GAMEAPPVIEW2->m_rectCursor);

	if(!GAMEAPPVIEW2->m_bShowCursor)
	{
		GAMEAPPVIEW2->m_bShowCursor = TRUE;
		SetCursor(GAMEAPPVIEW2->m_hCursor);
		ShowCursor(GAMEAPPVIEW2->m_bShowCursor); // show cursor
	}

	// reset mouse cursor position back to start position
	CRect rect;
	GAMEAPPVIEW2->GetWindowRect(&rect);
	SetCursorPos(rect.left + m_pParent->m_sCPoint.x+2, rect.top + m_pParent->m_sCPoint.y+2);

	m_pParent->m_bCueLineSet = TRUE;
	
	m_pParent->m_tableMousePosStrike[0] = m_pParent->m_tableMousePos[0];
	m_pParent->m_tableMousePosStrike[1] = m_pParent->m_tableMousePos[1];
	m_pParent->m_tableMousePosStrike[2] = m_pParent->m_tableMousePos[2];
}


void CCustomTable::Referee8BallPOCKETSET(DWORD msg, void *pData)
{	
	ctmsg_POCKETSET_DATA *ctmsgData;
	ctmsgData = (ctmsg_POCKETSET_DATA_typ *) pData;
	
	int n, m;

	if(m_pParent->m_cueBallSelect != 5) return;	// only in state '5'(after line is set!), are we allowed to do the following

	if(m_pParent->m_colourseq == 0) return; // if this is a break-off shot, dont do the following

//	// check if player has selected the black ball (8-ball) SPECIAL CASE: ALWAYS DO FOR BLACK BALL IN 8-BALL!!!
//	if(m_pParent->m_testBall->m_colour == 8)
//	{
//		//if(m_pParent->m_targetPocket == -1)
//		//	m_pParent->m_targetPocket = 2; // default: select middle pocket
//		
//		m_pParent->SetTargetBall(8);//m_targetBall = 8;
//  
//		n = m_pParent->GetSelectedPocket();
//		// MUST BE IN FIXMODE!
//		/*
//		{
//			char buf[256];
//			sprintf(buf, "n = %d", n);
//			ErrorMessage(m_pParent->m_parent, buf);
//		}
//		*/
//  
//		if(n != -1)	// if player has clicked inside a pocket
//		{
//			m_pParent->SetTargetPocket(n);
//			
//			// send target ball/pocket/success data to all at table
//			GAMEAPPVIEW2->SendPlayerTargetData(m_pParent->m_targetPocket, m_pParent->m_targetBall, m_pParent->m_bTargetSuccess);
//		}
//	}

	if(m_pParent->m_bCallShot) // default
	{
		n = m_pParent->GetSelectedPocket();
		m = m_pParent->GetSelectedBall();
		// MUST BE IN FIXMODE!
/*		
		{
			char buf[256];
			sprintf(buf, "n = %d, m = %d", n, m);
			ErrorMessage(m_pParent->m_parent, buf);
		}
*/		

		if(n != -1 || m != -1)	// if player has clicked inside a pocket OR has clicked on a ball
		{
			if(n != -1) m_pParent->SetTargetPocket(n);
			if(m != -1) m_pParent->SetTargetBall(m);

			// send target ball/pocket/success data to all at table
			GAMEAPPVIEW2->SendPlayerTargetData(m_pParent->m_targetPocket, m_pParent->m_targetBall, m_pParent->m_bTargetSuccess);
		}
	}
}


void CCustomTable::Referee8BallRESPOTBALL(int n)
{
	int m;
	int dir;
	BOOL successful;
	float f1;

	/////////////////////////////////////////////////////////////////////
	// (3) try and spot behind starting position in a direct line
	/////////////////////////////////////////////////////////////////////					
	// check to left side of spot (dir = -1)..., then the right side (dir = 1)...
	if(!TestFlag(m_pParent->m_ball[n]->m_flags, Flg_BALL_ACTIVE))
	for(dir = -1; dir <= 1; dir+=2)
	{
		CVector F = CVector(WORLD_X(591+65+120), 0, m_pParent->m_ball[n]->m_PSpot.z);
		
		// place ball on foot spot
		m_pParent->m_ball[n]->m_POld = m_pParent->m_ball[n]->m_P = F;//m_ball[n]->m_PSpot;
		m_pParent->m_ball[n]->m_W = m_pParent->m_ball[n]->m_V = m_pParent->m_ball[n]->m_VOld = CVector(0,0,0);

		do
		{
			f1 = BALL_RADIUS/20.0f;
			f1  = f1 * (float)dir;
			m_pParent->m_ball[n]->m_P.x -= f1;
			//m_ball[n]->m_P.x -= (BALL_RADIUS/20)*dir;

			successful = TRUE;
			for(m=0;m<MAX_TOTALBALLS;m++)
			if(n!=m)
			if(TestFlag(m_pParent->m_ball[m]->m_flags, Flg_BALL_ACTIVE)) // NEW: make sure this ball is active
			{
				if(!m_pParent->m_ball[n]->TryReSpot3(m_pParent->m_ball[m]))
					successful = FALSE;
			}

			// is this ball on top of a pocket
			float x = m_pParent->m_ball[n]->m_P.x;
			float y = m_pParent->m_ball[n]->m_P.y;
			for(int p=0;p<m_pocket_n;p++)
			{
				if( PocketProximity(p, CVector(x, y, 0), BALL_RADIUS) )
				{						
					successful = FALSE;
					break;
				}
			} // end for p

			if(successful) {
				m_pParent->m_ball[n]->ReSpotSuccessful();
				goto end;
			}

		}
		//while(1);
		while(!m_pParent->m_ball[n]->CushionsProximity(FALSE));
		//while((m_ball[n]->m_P.x >= (-1736+BALL_RADIUS)) ||
		//	  (m_ball[n]->m_P.x <= (1736-BALL_RADIUS)));   // bottom cushion + BALL_RADIUS || top cushion - BALL_RADIUS
		// *TODO: note: the above boundary check is incorrect!!!, it should be using the actual 3d world "Cushions"
	}
	/////////////////////////////////////////////////////////////////////

end:;

	#ifdef FIXEDMODE
	m_pParent->m_ball[n]->UpdateFixeds();
	#endif
}


void CCustomTable::Referee8BallSIMEND(void)
{
	int n;
	int colour;
	int no_colours;
	BOOL bWhiteInPocket = FALSE;
	BOOL bNextPlayer = FALSE;
	BOOL bPottedOwnGroupBall = TRUE;
	BOOL bCalledBallWasPotted = FALSE;

	int _spots = 0;
	int _stripes = 0;
	int _8ball = 0;
	int _pottedspots = 0;
	int _pottedstripes = 0;
	int _potted8ball = 0;
	

	m_pParent->m_bMiss = TRUE;												// assume no balls potted
	ClearFlag(m_pParent->m_foul, Flg_FOUL_NORMAL | Flg_FOUL_WHITEPOTTED | Flg_FOUL_GAMEOVER | Flg_FOUL_BALLINHAND);	// assume no foul to start with [keep some flags from previous shot]
	ClearFlag(m_pParent->m_foul, Flg_FOUL_ALERT | Flg_FOUL_NOTHITCUSHIONBREAK);
	m_pParent->m_bRefreshScores = TRUE;

	
	if(!m_pParent->m_bCallShot)
		bCalledBallWasPotted = TRUE; // just for safety, in case I forget to use -> "if(m_pParent->m_bCallShot)"

	/////////////////////////////////////////////////////////////
	// What Is Left?	/////////////////////////////////////////
	/////////////////////////////////////////////////////////////
	for(n=0;n<MAX_TOTALBALLS;n++)
	{
		if(TestFlag(m_pParent->m_ball[n]->m_flags, Flg_BALL_ACTIVE))
		{
			if(m_pParent->m_ball[n]->m_colour >= 1 && m_pParent->m_ball[n]->m_colour <= 7)
			{
				_spots++;
			}
			else if(m_pParent->m_ball[n]->m_colour >= 9 && m_pParent->m_ball[n]->m_colour <= 15)
			{
				_stripes++;
			}
			else if(m_pParent->m_ball[n]->m_colour == 8)
			{
				_8ball++;
			}
		}
	}

	
	/////////////////////////////////////////////////////////////
	// What Was Pottted?	/////////////////////////////////////
	/////////////////////////////////////////////////////////////
	for(n=0;n<MAX_TOTALBALLS;n++)
	{
		if(TestFlag(m_pParent->m_ball[n]->m_flags, Flg_BALL_POTTED))
		{
			// was target/called ball potted?
			if(m_pParent->m_bCallShot)
			if(m_pParent->m_bTargetSuccess)
			if(m_pParent->m_targetBall == m_pParent->m_ball[n]->m_colour)
				bCalledBallWasPotted = TRUE;

			if(m_pParent->m_ball[n]->m_colour >= 1 && m_pParent->m_ball[n]->m_colour <= 7)
			{
				_pottedspots++;
			}
			else if(m_pParent->m_ball[n]->m_colour >= 9 && m_pParent->m_ball[n]->m_colour <= 15)
			{
				_pottedstripes++;
			}
			else if(m_pParent->m_ball[n]->m_colour == 8)
			{
				_potted8ball++;
			}
		}
	}




	/////////////////////////////////////////////////////////////
	// See if White Ball has been potted (foul)
	for(n=0;n<MAX_TOTALBALLS;n++)
	{
		if(TestFlag(m_pParent->m_ball[n]->m_flags, Flg_BALL_POTTED))
			if(m_pParent->m_ball[n]->m_colour == WHITE)
			{
				bWhiteInPocket = TRUE;
				if(m_pParent->m_colourseq == 0) SetFlag(m_pParent->m_foul, Flg_FOUL_NORMAL | Flg_FOUL_WHITEPOTTED | Flg_FOUL_BREAK);
					else SetFlag(m_pParent->m_foul, Flg_FOUL_NORMAL | Flg_FOUL_WHITEPOTTED);
			}
	}
			
	/////////////////////////////////////////////////////////////


	/////////////////////////////////////////////////////////////
	// What happens if player strikes a ball other than the white
	if(m_pParent->m_selectBall != WHITE)
	{
		SetFlag(m_pParent->m_foul, Flg_FOUL_NORMAL);

		if(_potted8ball != 0)
		{
			// *TODO - FOUL!!!! DO RERACK!!!!
			SetFlag(m_pParent->m_foul, Flg_FOUL_GAMEOVER);
			goto end;
		}

		//goto end;
	}
	/////////////////////////////////////////////////////////////

	
	/////////////////////////////////////////////////////////////
	// if white doesn't hit a ball at all, foul ball on!
	//if(m_colourseq < YELLOW) // bypass if on colours at the end
	if((m_pParent->m_selectBall == WHITE) && (m_pParent->m_dominoHit1st == WHITE))
	{
			SetFlag(m_pParent->m_foul, Flg_FOUL_NORMAL);

			if(_potted8ball != 0)
			{
				// *TODO - FOUL!!!! DO RERACK!!!!
				SetFlag(m_pParent->m_foul, Flg_FOUL_GAMEOVER);
				goto end;
			}

			//goto end;
	}
	/////////////////////////////////////////////////////////////

	///
	// [NEW] if nothing was potted, did cueball or spotball touch a cushion?
	if(m_pParent->m_colourseq > 0)
	if(!bWhiteInPocket)
	//if(!bPushOutShot)
	if(_pottedspots == 0 && _pottedstripes == 0 && _potted8ball == 0) // check nothing was potted
	{
		BOOL bFailedToHitCushion = TRUE;
		
		for(n=0;n<MAX_TOTALBALLS;n++)
		{	
			if(m_pParent->m_ball[n]->m_hitCushion)
			{
				bFailedToHitCushion = FALSE; // no foul!
				break;
			}
		}

		if(bFailedToHitCushion)
		{
			SetFlag(m_pParent->m_foul, Flg_FOUL_NORMAL | Flg_FOUL_NOTHITCUSHION);
			//ErrorMessage(m_pParent->m_parent, "111");
		}

	}



	///////////////////////////////////////////////////////////////////////////
	///


	//char buf2[80];
	//sprintf(buf2, "spots potted = %d; stripes potted = %d", _pottedspots, _pottedstripes);
	//ErrorMessage(m_pParent->m_parent, buf2);

	//DEBUG!
	//char buf3[80];
	//sprintf(buf3, "1P: runOut = %d", m_pParent->m_break2P & (0x01|0x04) );
	//ErrorMessage(m_pParent->m_parent, buf3);
	//sprintf(buf3, "2P: runOut = %d", m_pParent->m_break2P & (0x02|0x08) );
	//ErrorMessage(m_pParent->m_parent, buf3);


	/////////////////////////////////////////////////////////////
	// make sure the correct ball has been potted, otherwise foul!
	//m_colourseq == 0,1 => UNDECIDED
	//m_colourseq == 2 => 1P HAS SPOTS
	//m_colourseq == 3 => 1P HAS STRIPES
	
	
		//for(n=0;n<MAX_TOTALBALLS;n++)
		//{
		//	if(TestFlag(m_pParent->m_ball[n]->m_flags, Flg_BALL_POTTED))
		//	{

	
	////
	/*
	// if the 8-ball has been potted - game-over!!!!
	colour = m_pParent->m_ball[m_pParent->m_dominoHit1st]->m_colour;
	if(_potted8ball > 0)
		if(colour != 8)
			{
			
				SetFlag(m_pParent->m_foul, Flg_FOUL_NORMAL | Flg_FOUL_GAMEOVER);
				goto end;
			}
	*/
	////
	

				if(m_pParent->m_colourseq == 0)
				{
					
					// on the break!
					//ErrorMessage(m_pParent->m_parent, "[m_colourseq == 0]");

//					// debug!
//					n = 8;
//					ClearFlag(m_pParent->m_ball[n]->m_flags, Flg_BALL_ACTIVE);
//					SetFlag(m_pParent->m_ball[n]->m_flags, Flg_BALL_POTTED);
//					_potted8ball = 1;

					if(_potted8ball != 0)
					{
						// *TODO - FOUL!!!! DO RERACK!!!!
						//SetFlag(m_pParent->m_foul, Flg_FOUL_NORMAL | Flg_FOUL_GAMEOVER);
												
						// if white ball has been potted, swap turn
						if(TestFlag(m_pParent->m_foul, Flg_FOUL_WHITEPOTTED))
						{
							if(m_pParent->m_bTurn1P)
								m_pParent->m_bTurn1P = FALSE;
							else
								m_pParent->m_bTurn1P = TRUE;
						}
						
						
						// respot black(8) ball			 						
						for(n=0;n<MAX_TOTALBALLS;n++)
						if(TestFlag(m_pParent->m_ball[n]->m_flags, Flg_BALL_POTTED))
						if(m_pParent->m_ball[n]->m_colour == 8)
						{
							Referee8BallRESPOTBALL(n);
							break;
						}
						
						// give current player choice of reracking
						SetFlag(m_pParent->m_foul, Flg_FOUL_ALERT | Flg_FOUL_ALERTPOT8ONBREAK);

						m_pParent->m_bMiss = FALSE;	// clear this, so that the 'miss' block of code below is bypassed

						goto end;
					}

					///
					// [NEW] if nothing was potted on the break, did >= 4 numbered balls touch a cushion?					
					//if(!bWhiteInPocket)
					if(_pottedspots == 0 && _pottedstripes == 0)
					{						
						int hitCushion = 0;
						
						for(n=1;n<MAX_TOTALBALLS;n++) // exclude white ball
						{	
							if(m_pParent->m_ball[n]->m_hitCushion)
							{
								hitCushion++; // (at least 4 numbered balls)
							}
						}
						
						if(hitCushion < 4)
						{
							if(!m_pParent->m_bPracticeMode || GAMEAPPVIEW2->m_bPlayOffline)// && !GAMEAPPVIEW2->m_bReplayMode)
							{
								// foul! So swap turn
								if(m_pParent->m_bTurn1P)
									m_pParent->m_bTurn1P = FALSE;
								else
									m_pParent->m_bTurn1P = TRUE;
							}

							// give current player choice of reracking
							SetFlag(m_pParent->m_foul, Flg_FOUL_NORMAL | Flg_FOUL_NOTHITCUSHIONBREAK | Flg_FOUL_ALERT);
							
							m_pParent->m_bMiss = FALSE;	// clear this, so that the 'miss' block of code below is bypassed

							goto end;
						}

						//char buf[80];
						//sprintf(buf, "hitCushion = %d", hitCushion);
						//ErrorMessage(m_pParent->m_parent, buf);
					}					
					
					m_pParent->m_colourseq = 1;					
				}
				else if(m_pParent->m_colourseq == 1)
				{
					// shots after the break : open table
					//ErrorMessage(m_pParent->m_parent, "[m_colourseq == 1]");

					if(_potted8ball != 0)
					{
						/////////////////////////////////
						// special case: [*NEW*]
						// what if no spots or stripes are left AND no spots or stripes were potted on this turn?
						if(_spots == 0 && _stripes == 0)
						if(_pottedspots == 0 && _pottedstripes == 0)
						if(!m_pParent->m_bCallShot || m_pParent->m_bTargetSuccess) // NEW* did player pot in the target pocket?
						if(!bWhiteInPocket) // so long as white wasn't potted aswell
						{
							//ErrorMessage(m_pParent->m_parent, "Target Pocket Success!!!!!");
							// this player has won!
							if(m_pParent->m_bTurn1P)
							{
								if( (m_pParent->m_break2P & 0x01) && !(m_pParent->m_break2P & 0x04) )
								{
									m_pParent->m_highBreaks1P[0] = 50; // 50 => run-out for 8-ball stat.
									Msfx_smpplayvol(&GAMEAPPVIEW2->m_game_smp[SFX_GOODSHOT], 255);
								}

								goto end;
							}
							else
							{
								if( (m_pParent->m_break2P & 0x02) && !(m_pParent->m_break2P & 0x08) )
								{
									m_pParent->m_highBreaks2P[0] = 50; // 50 => run-out for 8-ball stat.
									Msfx_smpplayvol(&GAMEAPPVIEW2->m_game_smp[SFX_GOODSHOT], 255);
								}

								goto end;
							}
						}
						/////////////////////////////////
						
						// otherwise lost!...
						// *TODO - FOUL!!!! DO RERACK!!!!
						SetFlag(m_pParent->m_foul, Flg_FOUL_NORMAL | Flg_FOUL_GAMEOVER);
						goto end;
					}

					if(m_pParent->m_bCallShot) // when player pockets the called ball, player gets that group!
					{
						// was a ball just potted?
						if(_pottedspots > 0 || _pottedstripes > 0)
						{
							// did player pot called ball in called pocket?
							if(m_pParent->m_bTargetSuccess)
							{								
								// what group was called ball from?			
								if(m_pParent->m_targetBall >= 1 && m_pParent->m_targetBall <= 7)
								{
									if(m_pParent->m_bTurn1P)
										m_pParent->m_colourseq = 2;	// 1P going for spots
									else
										m_pParent->m_colourseq = 3;	// 1P going for stripes
								}
								else if(m_pParent->m_targetBall >= 9 && m_pParent->m_targetBall <= 15)
								{
									if(m_pParent->m_bTurn1P)
										m_pParent->m_colourseq = 3;	// 1P going for stripes
									else
										m_pParent->m_colourseq = 2;	// 1P going for spots
								}
							}
							else
							{
								// foul!
								ClearFlag(m_pParent->m_foul, Flg_FOUL_NOTHITCUSHION);
								SetFlag(m_pParent->m_foul, Flg_FOUL_NORMAL);
								goto end;
							}
						}
					}
					else
					{ // default: old way, player must pocket 1 type of group only, to get that group in an open table
						if(m_pParent->m_bTurn1P)
						{
							if(_pottedspots > 0 && _pottedstripes == 0)
								m_pParent->m_colourseq = 2;	// 1P going for spots
							else if(_pottedspots == 0 && _pottedstripes > 0)
								m_pParent->m_colourseq = 3;	// 1P going for stripes
						}
						else
						{
							if(_pottedspots > 0 && _pottedstripes == 0)
								m_pParent->m_colourseq = 3;	// 2P going for spots
							else if(_pottedspots == 0 && _pottedstripes > 0)
								m_pParent->m_colourseq = 2;	// 2P going for stripes
						}
					}
				}
				else if(m_pParent->m_colourseq == 2)  // 1P going for spots...
				{
					//ErrorMessage(m_pParent->m_parent, "[m_colourseq == 2]");

					if(m_pParent->m_bTurn1P)
					{
						
						colour = m_pParent->m_ball[m_pParent->m_dominoHit1st]->m_colour;

						/// handle 8-ball /////////////////////////////////////////////////////
						if(_potted8ball > 0)
						{
							if(colour != 8 || bWhiteInPocket)
							{
								// *TODO - FOUL!!!! DO RERACK!!!!						
								SetFlag(m_pParent->m_foul, Flg_FOUL_NORMAL | Flg_FOUL_GAMEOVER);
								goto end;									
							}
							else
							{
								//ErrorMessage(m_pParent->m_parent, "[m_colourseq == 2]BLACK HIT FIRST");
								if(_pottedspots > 0 || _spots > 0)// || (_pottedstripes > 0))
								{
									// *TODO - FOUL!!!! DO RERACK!!!!
									SetFlag(m_pParent->m_foul, Flg_FOUL_NORMAL | Flg_FOUL_GAMEOVER);
									goto end;
								}
								else
								{
									// NEW* if player did NOT pot in the target pocket?
									if(m_pParent->m_bCallShot && !m_pParent->m_bTargetSuccess)
									{
										SetFlag(m_pParent->m_foul, Flg_FOUL_NORMAL | Flg_FOUL_GAMEOVER);
										goto end;
									}

									//ErrorMessage(m_pParent->m_parent, "Target Pocket Success!!!!!");
									// success - this player has won! i.e. other player has lost!
									//ErrorMessage(m_pParent->m_parent, "SUCCESS");
									m_pParent->m_gameOver = 2;

									if( (m_pParent->m_break2P & 0x01) && !(m_pParent->m_break2P & 0x04) )
									{
										m_pParent->m_highBreaks1P[0] = 50; // 50 => run-out for 8-ball stat.
										Msfx_smpplayvol(&GAMEAPPVIEW2->m_game_smp[SFX_GOODSHOT], 255);
									}
									goto end;
								}
							}
						}

						if(colour == 8)
						{
							// if we hit the 8-ball	first, and we still haven't ran out spots/stripes, then foul!
							if(_pottedspots > 0 || _spots > 0)
							{
								ClearFlag(m_pParent->m_foul, Flg_FOUL_NOTHITCUSHION);
								SetFlag(m_pParent->m_foul, Flg_FOUL_NORMAL);
								goto end;
							}
						}

						///////////////////////////////////////////////////////////////////////
						/*
						if(_pottedspots > 0 || _spots > 0)
							if(_potted8ball != 0)
							{
								// *TODO - FOUL!!!! DO RERACK!!!!						
								SetFlag(m_pParent->m_foul, Flg_FOUL_NORMAL | Flg_FOUL_GAMEOVER);
								goto end;
							}
						*/
						
						// check if player hasn't potted any of their own group
						if(_pottedspots == 0)
							bPottedOwnGroupBall = FALSE;

						// check if player hit wrong group ball first
						//if(_pottedstripes > 0 || colour >= 9)
						if(colour >= 9)
						{
							ClearFlag(m_pParent->m_foul, Flg_FOUL_NOTHITCUSHION);
							SetFlag(m_pParent->m_foul, Flg_FOUL_NORMAL);
							goto end;
						}

						// did player pot called ball
						if(m_pParent->m_bCallShot)
						if(_pottedspots > 0)
						if(!bCalledBallWasPotted)
						{  
							ClearFlag(m_pParent->m_foul, Flg_FOUL_NOTHITCUSHION);
							SetFlag(m_pParent->m_foul, Flg_FOUL_NORMAL);
							goto end;
						}
					}
					else
					{
						colour = m_pParent->m_ball[m_pParent->m_dominoHit1st]->m_colour;

						/// handle 8-ball /////////////////////////////////////////////////////
						if(_potted8ball > 0)
						{
							if(colour != 8 || bWhiteInPocket)
							{
								// *TODO - FOUL!!!! DO RERACK!!!!						
								SetFlag(m_pParent->m_foul, Flg_FOUL_NORMAL | Flg_FOUL_GAMEOVER);
								goto end;									
							}
							else
							{
								//ErrorMessage(m_pParent->m_parent, "[m_colourseq == 2]BLACK HIT FIRST2");
								if(_pottedstripes > 0 || _stripes > 0)// || (_pottedspots > 0))
								{
									// *TODO - FOUL!!!! DO RERACK!!!!						
									SetFlag(m_pParent->m_foul, Flg_FOUL_NORMAL | Flg_FOUL_GAMEOVER);
									goto end;
								}
								else
								{
									// NEW* if player did NOT pot in the target pocket?
									if(m_pParent->m_bCallShot && !m_pParent->m_bTargetSuccess)
									{
										SetFlag(m_pParent->m_foul, Flg_FOUL_NORMAL | Flg_FOUL_GAMEOVER);
										goto end;
									}

//									ErrorMessage(m_pParent->m_parent, "Target Pocket Success!!!!!");
									// success - this player has won! i.e. other player has lost!
									//ErrorMessage(m_pParent->m_parent, "SUCCESS");
									m_pParent->m_gameOver = 1;
									if( (m_pParent->m_break2P & 0x02) && !(m_pParent->m_break2P & 0x08) )
										m_pParent->m_highBreaks2P[0] = 50; // 50 => run-out for 8-ball stat.

									goto end;
								}
							}
						}

						if(colour == 8)
						{
							// if we hit the 8-ball	first, and we still haven't ran out spots/stripes, then foul!
							if(_pottedstripes > 0 || _stripes > 0)
							{
								ClearFlag(m_pParent->m_foul, Flg_FOUL_NOTHITCUSHION);
								SetFlag(m_pParent->m_foul, Flg_FOUL_NORMAL);
								goto end;
							}
						}

						///////////////////////////////////////////////////////////////////////
						/*
						if(_pottedstripes > 0 || _stripes > 0)
							if(_potted8ball != 0)
							{
								// *TODO - FOUL!!!! DO RERACK!!!!						
								SetFlag(m_pParent->m_foul, Flg_FOUL_NORMAL | Flg_FOUL_GAMEOVER);
								goto end;
							}
						  */

						// check if player hasn't potted any of their own group
						if(_pottedstripes == 0)
							bPottedOwnGroupBall = FALSE;
						
						// check for hitting wrong group ball
						//if(_pottedspots > 0 || colour <= 7)
						if(colour <= 7)
						{
							ClearFlag(m_pParent->m_foul, Flg_FOUL_NOTHITCUSHION);
							SetFlag(m_pParent->m_foul, Flg_FOUL_NORMAL);
							goto end;
						}

						// did player pot called ball
						if(m_pParent->m_bCallShot)
						if(_pottedstripes > 0)
						if(!bCalledBallWasPotted)
						{  
							ClearFlag(m_pParent->m_foul, Flg_FOUL_NOTHITCUSHION);
							SetFlag(m_pParent->m_foul, Flg_FOUL_NORMAL);
							goto end;
						}
					}
				}
				else if(m_pParent->m_colourseq == 3) // 1P going for stripes
				{
					//ErrorMessage(m_pParent->m_parent, "[m_colourseq == 3]");
					if(m_pParent->m_bTurn1P)
					{
						
						colour = m_pParent->m_ball[m_pParent->m_dominoHit1st]->m_colour;

						/// handle 8-ball /////////////////////////////////////////////////////
						if(_potted8ball > 0)
						{
							if(colour != 8 || bWhiteInPocket)
							{
								// *TODO - FOUL!!!! DO RERACK!!!!						
								SetFlag(m_pParent->m_foul, Flg_FOUL_NORMAL | Flg_FOUL_GAMEOVER);
								goto end;									
							}
							else
							{
								//ErrorMessage(m_pParent->m_parent, "[m_colourseq == 3]BLACK HIT FIRST");
								if(_pottedstripes > 0 || _stripes > 0)// || (_pottedspots > 0))
								{
									// *TODO - FOUL!!!! DO RERACK!!!!						
									SetFlag(m_pParent->m_foul, Flg_FOUL_NORMAL | Flg_FOUL_GAMEOVER);
									goto end;
								}
								else
								{
									// NEW* if player did NOT pot in the target pocket?
									if(m_pParent->m_bCallShot && !m_pParent->m_bTargetSuccess)
									{
										SetFlag(m_pParent->m_foul, Flg_FOUL_NORMAL | Flg_FOUL_GAMEOVER);
										goto end;
									}

//									ErrorMessage(m_pParent->m_parent, "Target Pocket Success!!!!!");
									//ErrorMessage(m_pParent->m_parent, "SUCCESS");
									// success - this player has won! i.e. other player has lost!
									m_pParent->m_gameOver = 2;

									if( (m_pParent->m_break2P & 0x01) && !(m_pParent->m_break2P & 0x04) )
									{
										m_pParent->m_highBreaks1P[0] = 50; // 50 => run-out for 8-ball stat.
										Msfx_smpplayvol(&GAMEAPPVIEW2->m_game_smp[SFX_GOODSHOT], 255);
									}

									goto end;
								}
							}
						}

						if(colour == 8)
						{
							// if we hit the 8-ball	first, and we still haven't ran out spots/stripes, then foul!
							if(_pottedstripes > 0 || _stripes > 0)
							{
								ClearFlag(m_pParent->m_foul, Flg_FOUL_NOTHITCUSHION);
								SetFlag(m_pParent->m_foul, Flg_FOUL_NORMAL);
								goto end;
							}
						}

						///////////////////////////////////////////////////////////////////////
						/*
						if(_pottedstripes > 0 || _stripes > 0)
							if(_potted8ball != 0)
							{
								// *TODO - FOUL!!!! DO RERACK!!!!						
								SetFlag(m_pParent->m_foul, Flg_FOUL_NORMAL | Flg_FOUL_GAMEOVER);
								goto end;
							}
						*/

						// check if player hasn't potted any of their own group balls
						if(_pottedstripes == 0)
							bPottedOwnGroupBall = FALSE;

						// check if player hit wrong group ball
						//if(_pottedspots > 0 || colour <= 7)
						if(colour <= 7)
						{
							ClearFlag(m_pParent->m_foul, Flg_FOUL_NOTHITCUSHION);
							SetFlag(m_pParent->m_foul, Flg_FOUL_NORMAL);
							goto end;
						}

						// did player pot called ball
						if(m_pParent->m_bCallShot)
						if(_pottedstripes > 0)
						if(!bCalledBallWasPotted)
						{  
							ClearFlag(m_pParent->m_foul, Flg_FOUL_NOTHITCUSHION);
							SetFlag(m_pParent->m_foul, Flg_FOUL_NORMAL);
							goto end;
						}
					}
					else
					{

						colour = m_pParent->m_ball[m_pParent->m_dominoHit1st]->m_colour;

						/// handle 8-ball /////////////////////////////////////////////////////
						if(_potted8ball > 0)
						{
							if(colour != 8 || bWhiteInPocket)
							{
								// *TODO - FOUL!!!! DO RERACK!!!!						
								SetFlag(m_pParent->m_foul, Flg_FOUL_NORMAL | Flg_FOUL_GAMEOVER);
								goto end;									
							}
							else
							{
								//ErrorMessage(m_pParent->m_parent, "[m_colourseq == 3]BLACK HIT FIRST2");
								if(_pottedspots > 0 || _spots > 0)// || (_pottedstripes > 0))
								{
									// *TODO - FOUL!!!! DO RERACK!!!!						
									SetFlag(m_pParent->m_foul, Flg_FOUL_NORMAL | Flg_FOUL_GAMEOVER);
									goto end;
								}
								else
								{
									// NEW* if player did NOT pot in the target pocket?
									if(m_pParent->m_bCallShot && !m_pParent->m_bTargetSuccess)
									{
										SetFlag(m_pParent->m_foul, Flg_FOUL_NORMAL | Flg_FOUL_GAMEOVER);
										goto end;
									}

//									ErrorMessage(m_pParent->m_parent, "Target Pocket Success!!!!!");
									//ErrorMessage(m_pParent->m_parent, "SUCCESS");
									// success - this player has won! i.e. other player has lost!
									m_pParent->m_gameOver = 1;

									if( (m_pParent->m_break2P & 0x02) && !(m_pParent->m_break2P & 0x08) )
										m_pParent->m_highBreaks2P[0] = 50; // 50 => run-out for 8-ball stat.

									goto end;
								}
							}
						}

						if(colour == 8)
						{
							// if we hit the 8-ball	first, and we still haven't ran out spots/stripes, then foul!
							if(_pottedspots > 0 || _spots > 0)
							{
								ClearFlag(m_pParent->m_foul, Flg_FOUL_NOTHITCUSHION);
								SetFlag(m_pParent->m_foul, Flg_FOUL_NORMAL);
								goto end;
							}
						}

						///////////////////////////////////////////////////////////////////////
						/*
						if(_pottedspots > 0 || _spots > 0)
							if(_potted8ball != 0)
							{
								// *TODO - FOUL!!!! DO RERACK!!!!						
								SetFlag(m_pParent->m_foul, Flg_FOUL_NORMAL | Flg_FOUL_GAMEOVER);
								goto end;
							}
						*/

						// check if player hasn't potted any of their own group balls
						if(_pottedspots == 0)
							bPottedOwnGroupBall = FALSE;

						// check if player hit wrong group ball
						//if(_pottedstripes > 0 || colour >= 9)
						if(colour >= 9)
						{
							ClearFlag(m_pParent->m_foul, Flg_FOUL_NOTHITCUSHION);
							SetFlag(m_pParent->m_foul, Flg_FOUL_NORMAL);
							goto end;
						}

						// did player pot called ball
						if(m_pParent->m_bCallShot)
						if(_pottedspots > 0)
						if(!bCalledBallWasPotted)
						{  
							ClearFlag(m_pParent->m_foul, Flg_FOUL_NOTHITCUSHION);
							SetFlag(m_pParent->m_foul, Flg_FOUL_NORMAL);
							goto end;
						}
					}

				}

					//if(m_pParent->m_bTurn1P)

	//			if(m_pParent->m_ball[n]->m_colour == WHITE)
	///			{
	//				bWhiteInPocket = TRUE;
	//				SetFlag(m_pParent->m_foul, Flg_FOUL_NORMAL | Flg_FOUL_WHITEPOTTED);
	//			}
	//		}
	//	}
	  
	/////////////////////////////////////////////////////////////
	//char buf[80];
	//sprintf(buf, "-- m_colourseq = %d ---", m_pParent->m_colourseq);
	//ErrorMessage(m_pParent->m_parent, buf);

	/////////////////////////////////////////////////////////////
	// normal pots...
	if(!bWhiteInPocket)
	{			
		//MessageBox(0, "11","11",MB_OK);
		for(n=0;n<MAX_TOTALBALLS;n++)
		{
			if(TestFlag(m_pParent->m_ball[n]->m_flags, Flg_BALL_POTTED))
			{					
				m_pParent->m_bMiss = FALSE;		// potted ball
				m_pParent->m_foul = 0;			// no foul

				if(m_pParent->m_bTurn1P) {
					// 1P just potted a ball
					// after first pot, go into runout mode
					if( !(m_pParent->m_break2P & 0x01) )
						m_pParent->m_break2P |= 0x01;					
				}
				else
				{
					// 2P just potted a ball
					// after first pot, go into runout mode
					if( !(m_pParent->m_break2P & 0x02) )
						m_pParent->m_break2P |= 0x02;
				}
				

				
				int tbl = GAMEAPPVIEW2->m_lobby->GetTableIdxFromPlayer(GAMEAPPVIEW2->m_playerSockIdx);
				if(tbl!=-1)
				{
					m_pParent->m_shotTime =  m_table[tbl].shotTime*50;
				}
									  
				/*
				if(_potted8ball >0)
				{
					// GAME_OVER! the other player losses!
					if(m_pParent->m_bTurn1P)
						m_pParent->m_gameOver = 2;
					else
						m_pParent->m_gameOver = 1;		
				}
				*/
			}
		}

		// if player failed to pot their own group ball?
		if(!bPottedOwnGroupBall)
			m_pParent->m_bMiss = TRUE;

		/*
		if(!m_colourseq) // if we are NOT on the 'colour seq.' at the end of the frame
		if(m_bMiss == FALSE)
		{
			if(m_rednext)
				m_rednext = FALSE;
			else
				m_rednext = TRUE;
		}
		*/
	}
	/////////////////////////////////////////////////////////////

end:;

	
	// if just done break! and we fouled!
	if(m_pParent->m_colourseq == 0)
		m_pParent->m_colourseq = 1;

	if(TestFlag(m_pParent->m_foul, Flg_FOUL_GAMEOVER))
	{
		// GAME_OVER! this player losses!
		if(m_pParent->m_bTurn1P)
			m_pParent->m_gameOver = 1;
		else
			m_pParent->m_gameOver = 2;		
	}

	// ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
	// how many balls have been potted in a row?
	if(!TestFlag(m_pParent->m_foul, Flg_FOUL_NORMAL | Flg_FOUL_GAMEOVER))
	{
		if(bPottedOwnGroupBall)
		{
			if(m_pParent->m_bTurn1P)
			{
				m_pParent->m_poolPots1P += (_pottedspots + _pottedstripes + _potted8ball);

				if(m_pParent->m_highBreaks1P[1] < m_pParent->m_poolPots1P)
					m_pParent->m_highBreaks1P[1] = m_pParent->m_poolPots1P;
			}
			else
			{
				m_pParent->m_poolPots2P += (_pottedspots + _pottedstripes + _potted8ball);

				if(m_pParent->m_highBreaks2P[1] < m_pParent->m_poolPots2P)
					m_pParent->m_highBreaks2P[1] = m_pParent->m_poolPots2P;
			}
		}
		
		if(!bPottedOwnGroupBall || _potted8ball > 0)
		{
			// didn't pot anything this time or gameover
			m_pParent->m_poolPots1P = 0;
			m_pParent->m_poolPots2P = 0;
		}
	}
	else
	{
		// if foul! then clear pot runs
		m_pParent->m_poolPots1P = 0;
		m_pParent->m_poolPots2P = 0;
	}

//	{
//	char buf[256];
//	sprintf(buf, "m_poolPots1P = %d (%d), m_poolPots2P = %d (%d)", m_pParent->m_poolPots1P, m_pParent->m_highBreaks1P[1], m_pParent->m_poolPots2P, m_pParent->m_highBreaks2P[1]);
//	ErrorMessage(g_wnd, buf);
//	}
	// ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

	/////////////////////////////////////////////////////////////
	// check if nothing was potted this time
//	if(!m_pParent->m_gameOver) // NOT SURE WHY WE NEEDED THIS LINE?
	if(m_pParent->m_bMiss)
	{

		// if we've already potted a ball, but now we've missed or fouled, then NO run-out!
		//if(m_pParent->m_break2P == 1)
		//	m_pParent->m_break2P = -1;
		if(m_pParent->m_bTurn1P) {
			// 1P just missed/fouled a ball
			// if already potted a ball, but now missed!
			if( (m_pParent->m_break2P & 0x01) )
				m_pParent->m_break2P |= 0x04;
		}
		else
		{
			// 2P just missed/fouled a ball			
			// if already potted a ball, but now missed!
			if( (m_pParent->m_break2P & 0x02) )
				m_pParent->m_break2P |= 0x08;
		}

		//ErrorMessage(m_pParent->m_parent, "miss!");
//		// end of break sounds
//		if(g_gfxDetails.bSoundClapping)
//		{
//			if(m_bTurn1P)
//			{
//				if(m_break1P >= 20 && m_break1P < 100)
//				{
//					float loudness = 128 + 128*(((float)m_break1P - 20)/80);
//					Msfx_smpplayvol(&GAMEAPPVIEW->m_game_smp[SFX_20PLUSBREAK], loudness);
//				}
//				
//				/*
//				if(m_break1P >= 100)
//				{
//					Msfx_smpplayvol(&GAMEAPPVIEW->m_game_smp[SFX_CENTURY], 255);
//				}
//				*/
//				
//			}
//			else
//			{
//				if(m_break2P >= 20 && m_break2P < 100)
//				{
//					float loudness = 128 + 128*(((float)m_break1P - 20)/80);
//					Msfx_smpplayvol(&GAMEAPPVIEW->m_game_smp[SFX_20PLUSBREAK], loudness);
//				}
//  
//				/*
//				if(m_break2P >= 100)
//				{
//					Msfx_smpplayvol(&GAMEAPPVIEW->m_game_smp[SFX_CENTURY], 255);
//				}
//				*/
//			}
//		}

		//MessageBox(0, "13","13",MB_OK);
		if(m_pParent->m_bPracticeMode && !GAMEAPPVIEW2->m_bReplayMode && !GAMEAPPVIEW2->m_bPlayOffline)
		{
			// force no fouls in practice
			//m_foul = 0;
			//m_break1P = 0;
		}
		else
		{
			// decide who's turn it is next?
			bNextPlayer = TRUE;
			m_pParent->m_bConcededGame = FALSE; // clear this flag (now that the turn has swapped over, we can allow gameOver to continue...)
			GAMEAPPVIEW2->m_bFlashWindow = TRUE;

			if(TestFlag(m_pParent->m_foul, Flg_FOUL_NORMAL))
				m_pParent->m_shotTime = 60*50;
			else
			{
				// if no foul, then give usual time for shot
				int tbl = GAMEAPPVIEW2->m_lobby->GetTableIdxFromPlayer(GAMEAPPVIEW2->m_playerSockIdx);
				if(tbl!=-1)
				{
					m_pParent->m_shotTime =  m_table[tbl].shotTime*50;
				}
			}
			

			//ErrorMessage(m_parent, "1. reg break!");

			if(m_pParent->m_bTurn1P) {
				m_pParent->m_bTurn1P = FALSE;
				//m_pParent->m_break2P = 0;

				//m_pParent->m_break2P = 60*50;

				/*
				// is it a high break?
				//if(m_break1P >= 50)
				{
					if(m_highBreaks1P[0] <= m_highBreaks1P[1])
					{
						if(m_break1P > m_highBreaks1P[0])
							m_highBreaks1P[0] = m_break1P;
					}
					else
					{
						if(m_break1P > m_highBreaks1P[1])
							m_highBreaks1P[1] = m_break1P;
					}
				}
				*/
			}
			else {
				m_pParent->m_bTurn1P = TRUE;
				//m_pParent->m_break1P = 0;

				//m_pParent->m_break2P = 60*50;

				/*
				// is it a high break?
				//if(m_break2P >= 50)
				{
					if(m_highBreaks2P[0] <= m_highBreaks2P[1])
					{
						if(m_break2P > m_highBreaks2P[0])
							m_highBreaks2P[0] = m_break2P;
					}
					else
					{
						if(m_break2P > m_highBreaks2P[1])
							m_highBreaks2P[1] = m_break2P;
					}
				}
				*/
			}
		}

		// if we are NOT on the 'colour seq.' at the end of the frame
		//if(!m_colourseq)
		//	m_rednext = TRUE;
	}
	/////////////////////////////////////////////////////////////


	Referee8BallUpdatePots();

	// re-spot balls
	Referee8BallReSpot();

		
		///////////
		// for practice mode : fixes "pot yellow aand white simul. bug" ***
		if(m_pParent->m_bMiss)
		{
			//MessageBox(0, "13b","13b",MB_OK);
			if(m_pParent->m_bPracticeMode && !GAMEAPPVIEW2->m_bPlayOffline)
			{
				//ErrorMessage(g_wnd, "FOUL!");
				// force no fouls in practice
				m_pParent->m_foul = 0;
				//m_pParent->m_break1P = 0;
			}
		}
		///////////

		
		if(TestFlag(m_pParent->m_foul, Flg_FOUL_NORMAL))
		{
			m_pParent->m_bBallInHand = 1;
			m_pParent->m_bStandUpFromShot = 1;
		}
}


///////////////////////////////////////////////////////////////////////////////////////////////////
//
//
void CCustomTable::Referee8BallReSpot(void)
{
	int n;
	int m;
	int nextspot;
	int dir;
	BOOL successful;
	BOOL reds;

	float f1,f2,f3;

	
	////////////////////////////////////////////////////////////////////////////////////////////////////////////
	
	//ErrorMessage(m_pParent->m_parent, "Respot:(1,2,3)");

	// for balls in pockects
	for(n=MAX_TOTALBALLS-1; n >= 0;n--)
	//if((m_ball[n]->m_colour <= BLACK && m_ball[n]->m_colour >= YELLOW) || (m_ball[n]->m_colour == WHITE))
	if((m_pParent->m_ball[n]->m_colour == WHITE)) // ***** TODO ***** -> 8 BALL
	{	
		// temp. code for WHITE ball
		if(m_pParent->m_ball[n]->m_colour == WHITE)
			//if(!TestFlag(m_pParent->m_ball[n]->m_flags, Flg_BALL_ACTIVE))
			if(TestFlag(m_pParent->m_foul, Flg_FOUL_WHITEPOTTED))
				{
					// position ball on its original starting position or...
					m_pParent->m_ball[n]->m_POld = m_pParent->m_ball[n]->m_P = m_pParent->m_ball[n]->m_PSpot;
					
					//ErrorMessage(m_pParent->m_parent, "whiteball-1");

					if( Referee8BallDropBallOnTable(WHITE, m_pParent->m_ball[n]->m_P.x, m_pParent->m_ball[n]->m_P.y ) )
					{
						//ErrorMessage(m_pParent->m_parent, "whiteball-2");
					}
					else					
					{
						//ErrorMessage(m_pParent->m_parent, "whiteball-3");
						// ...otherwise position anywhere behind the 'Head String'
						CVector Pgreen, Pyellow;
						CVector P;
						float radius;
						//Pgreen = CVector(-WORLD_X(912),-WORLD_Y(395-12), BALL_POSITIONZOFFSET);
						//Pyellow = CVector(-WORLD_X(912),-WORLD_Y(199+12), BALL_POSITIONZOFFSET);
						Pgreen = CVector(-WORLD_X(591+65+120/*912+5*/), -600, BALL_POSITIONZOFFSET);
						Pyellow = CVector(-WORLD_X(591+65+120/*912+5*/), 600, BALL_POSITIONZOFFSET);
						float w,h;
						float x,y;
						
						//[f1 = Pyellow.y - Pgreen.y;]
						f1 = Pyellow.y;
						f1 = f1 - Pgreen.y;
						
						f1 = f1 * 0.9f;
						h =	f1;
						//h =	(Pyellow.y - Pgreen.y)*0.9f;
						w = h*0.5f;
						for(x = 0.0f;x<w;x+=BALL_RADIUS)
						for(y = 0.0f;y<h;y+=BALL_RADIUS)
						{
							
							//[f1 = Pyellow.x - x;]
							f1 = Pyellow.x;
							f1 = f1 - x;
							
							//[f2 = Pyellow.y - Pgreen.y;]
							f2 = Pyellow.y;
							f2 = f2 - Pgreen.y;
							
							f2 = f2 * 0.05f;
							
							//[f3 = Pyellow.y - y;]
							f3 = Pyellow.y;
							f3 = f3 - y;
							
							f3 = f3 - f2;
							if( Referee8BallDropBallOnTable(WHITE, f1, f3) )
							//if( DropBallOnTable(WHITE, Pyellow.x - x, Pyellow.y - y - (Pyellow.y - Pgreen.y)*0.05f) )
								goto whiteSpotted;
						}
						whiteSpotted:;
					}
					
//					m_ball[n]->m_POld = m_ball[n]->m_P = m_ball[n]->m_PSpot;
					m_pParent->m_ball[n]->m_W = m_pParent->m_ball[n]->m_V = m_pParent->m_ball[n]->m_VOld = CVector(0,0,0);
	
					// turn ball on
					m_pParent->m_ball[n]->m_flags = 0;
					SetFlag(m_pParent->m_ball[n]->m_flags, Flg_BALL_ACTIVE);

					m_pParent->m_bBallInHand = 1;
					m_pParent->m_bStandUpFromShot = 1;


					continue;
				}
		
		#if 0
		///&&&&&&&&&&
		
		/////////////////////////////////////////////////////////////////////
		// (1) try and spot on starting position
		/////////////////////////////////////////////////////////////////////
		successful = TRUE;
		for(m=0;m<MAX_TOTALBALLS;m++)
		if(n!=m)
		{
			if(!m_ball[n]->TryReSpot1(m_ball[m]))
				successful = FALSE;
		}
		/////////////////////////////////////////////////////////////////////

		if(successful)
			m_ball[n]->ReSpotSuccessful();
		else
		{
			/////////////////////////////////////////////////////////////////////
			// (2) try next vacant spot, starting with highest colour spot first
			/////////////////////////////////////////////////////////////////////
			for(nextspot=MAX_TOTALBALLS-1; nextspot > 0;nextspot--)
				if(m_ball[nextspot]->m_colour <= BLACK && m_ball[nextspot]->m_colour >= YELLOW)
				{
					successful = TRUE;
					for(m=0;m<MAX_TOTALBALLS;m++)
					if(n!=m)
						{
						if(!m_ball[n]->TryReSpot2(m_ball[m], m_ball[nextspot]))
							successful = FALSE;
						}
					
					if(successful) {
						m_ball[n]->ReSpotSuccessful();
						goto end;
					}
				
				}
			/////////////////////////////////////////////////////////////////////

			/////////////////////////////////////////////////////////////////////
			// (3) try and spot behind starting position in a direct line
			/////////////////////////////////////////////////////////////////////					
			// check to left side of spot (dir = -1)..., then the right side (dir = 1)...
			if(!TestFlag(m_ball[n]->m_flags, Flg_BALL_ACTIVE))
			for(dir = -1; dir <= 1; dir+=2)
			{
				// position ball on its original starting position
				m_ball[n]->m_POld = m_ball[n]->m_P = m_ball[n]->m_PSpot;
				m_ball[n]->m_W = m_ball[n]->m_V = m_ball[n]->m_VOld = CVector(0,0,0);

				do
				{
					f1 = BALL_RADIUS/20.0f;
					f1  = f1 * dir;
					m_ball[n]->m_P.x -= f1;
					//m_ball[n]->m_P.x -= (BALL_RADIUS/20)*dir;

					successful = TRUE;
					for(m=0;m<MAX_TOTALBALLS;m++)
					if(n!=m)
					{
						if(!m_ball[n]->TryReSpot3(m_ball[m]))
							successful = FALSE;
					}

					if(successful) {
						m_ball[n]->ReSpotSuccessful();
						goto end;
					}

				}
				while((m_ball[n]->m_P.x >= (-1736+BALL_RADIUS)) ||
					  (m_ball[n]->m_P.x <= (1736-BALL_RADIUS)));   // bottom cushion + BALL_RADIUS || top cushion - BALL_RADIUS
				// *TODO: note: the above boundary check is incorrect!!!, it should be using the actual 3d world "Cushions"
			}
				/////////////////////////////////////////////////////////////////////

				// we are in trouble if it gets this far!!!

		end:;
		}		
		///&&&&&&&&&&
		#endif //0
	}
}

BOOL CCustomTable::Referee8BallDropBallOnTable(int n, float x, float y, BOOL bDrop)
{		
	
	// check if ball 'n' is on top of another ball
	int m;
	for(m=0;m<MAX_TOTALBALLS;m++)
	if(m!=n)
	if(m_pParent->m_ball[m]->Proximity(x, y, 0, BALL_RADIUS*2.0f) == TRUE)
	{
		//ErrorMessage(m_pParent->m_parent, "FALSE");
		return FALSE;
	}
	
	// check for ball n inside the D
	if(!m_pParent->m_bPracticeMode || m_type == CTTYPE_BOWLS || GAMEAPPVIEW2->m_bPlayOffline) // should be !
	{
		CVector B = CVector(x,y,BALL_POSITIONZOFFSET);
		CVector Pgreen, Pbrown;
		CVector P;
		float radius;
		Pgreen = CVector(-WORLD_X(591+65+120), -600-(28.4625f - g_ballRadius), BALL_POSITIONZOFFSET);
		Pbrown = CVector(-WORLD_X(591+65+120), 0, BALL_POSITIONZOFFSET);

		//[P = Pgreen - Pbrown;]
		P = Pgreen;
		P = P - Pbrown;

		radius = P.Magnitude2();
		
		//[P = B - Pbrown;]
		P = B;
		P = P - Pbrown;


		int xoffset = 0, yoffset = 0;

		if(m_pParent->m_M3d_Table_ptr == &m_M3d_ukTable)
			{
			// Set Different Bounding Box Regions for UK Pool Game Type //
			xoffset = -110;
			yoffset = 85;
			}



		// special
		// For 8-Ball & English Pool Ball-In-Hand is behind the line after a foul on the break //
		if(m_type == CTTYPE_8BALL || m_type == CTTYPE_UKPOOL)
			{
			if(!TestFlag(m_pParent->m_foul, Flg_FOUL_BREAK))
			if(TestFlag(m_pParent->m_foul, Flg_FOUL_WHITEPOTTED) || TestFlag(m_pParent->m_foul, Flg_FOUL_NORMAL)
				|| TestFlag(m_pParent->m_foul, Flg_FOUL_BALLINHAND))
				{
				if(m_pParent->m_M3d_Table_ptr == &m_M3d_ukTable) Pbrown.x = 1235+xoffset;	// give ball-in-hand access to whole table
					else Pbrown.x = 1235;	// give ball-in-hand access to whole table
				}
			}
		// Otherwise Ball In Hand is everywhere //
		else{
			if(TestFlag(m_pParent->m_foul, Flg_FOUL_WHITEPOTTED) || TestFlag(m_pParent->m_foul, Flg_FOUL_NORMAL)
				|| TestFlag(m_pParent->m_foul, Flg_FOUL_BALLINHAND))
				{
				if(m_pParent->m_M3d_Table_ptr == &m_M3d_ukTable) Pbrown.x = 1235+xoffset;	// give ball-in-hand access to whole table
					else Pbrown.x = 1235;	// give ball-in-hand access to whole table
				}
			}
	

		

		if((B.y < -(Pgreen.y+yoffset)) && (B.y > (Pgreen.y+yoffset)))
			{			
			//if(  (B.x < Pbrown.x) && (B.x > (-1235 - (28.4625f - g_ballRadius)*1.4f) )  )
			if(  (B.x < Pbrown.x) && (B.x > (-(1235+xoffset) - (28.4625f - g_ballRadius)*1.4f) )  )
				{
				// update the drop ball's coordinates
				if(bDrop)
					{
					m_pParent->m_ball[n]->m_P.x = x;
					m_pParent->m_ball[n]->m_P.y = y;
					#ifdef FIXEDMODE
					m_pParent->m_ball[n]->m_V.x = 0;
					m_pParent->m_ball[n]->m_V.y = 0;
					m_pParent->m_ball[n]->m_V.z = 0;
					m_pParent->m_ball[n]->UpdateFixeds();
					#endif

					m_pParent->m_ball[n]->AdjustOrientation();
					m_pParent->m_ball[n]->m_POld = m_pParent->m_ball[n]->m_P;
					#ifdef FIXEDMODE
					m_pParent->m_ball[n]->UpdateFixeds();
					#endif
					}
				return TRUE;
				}
			}

		return FALSE;
		}

	// update the drop ball's coordinates
	if(bDrop)
	{
		m_pParent->m_ball[n]->m_P.x = x;
		m_pParent->m_ball[n]->m_P.y = y;
		#ifdef FIXEDMODE
		m_pParent->m_ball[n]->m_V.x = 0;
		m_pParent->m_ball[n]->m_V.y = 0;
		m_pParent->m_ball[n]->m_V.z = 0;
		m_pParent->m_ball[n]->UpdateFixeds();
		#endif
		m_pParent->m_ball[n]->AdjustOrientation();
		m_pParent->m_ball[n]->m_POld = m_pParent->m_ball[n]->m_P;
		#ifdef FIXEDMODE
		m_pParent->m_ball[n]->UpdateFixeds();
		#endif
		//ErrorMessage(m_pParent->m_parent, "TRUE");

	}

	return TRUE;
}


void CCustomTable::Referee8BallUpdatePots(void)
{
	int ballColour;
	float fx,fy;
	int x,y;
		
	int n;
	for(n = 0; n < 16; n++)
	{
		g_pottedSpots[n] = -1;
		g_pottedStripes[n] = -1;
	}

	// scan thru all potted balls, and see who's potted what
	int m;
	for(m=0;m<MAX_TOTALBALLS;m++)
	if(!TestFlag(m_pParent->m_ball[m]->m_flags, Flg_BALL_ACTIVE))
	{
		fx = m_pParent->m_ball[m]->m_P.x;
		fy = m_pParent->m_ball[m]->m_P.y;

		
		// what has P1 potted
		if(fx > 20001)
		{
			fx-=20000;
			fx /= 100;
			if(fx>=0 && fx<= 15)
			{
				x = fx;
				y = fy;
				if((m_pParent->m_ball[m]->m_colour >= 1) &&  (m_pParent->m_ball[m]->m_colour <= 7))
					g_pottedSpots[x] = m;
				else if((m_pParent->m_ball[m]->m_colour >= 9) &&  (m_pParent->m_ball[m]->m_colour <= 15))
					g_pottedStripes[x] = m;					
				
			}
		}

		// what has P2 potted
		if(fy > 20001)
		{
			fy-=20000;
			fy /= 100;
			if(fy>=0 && fy<= 15)
			{
				x = fx;
				y = fy;
				if((m_pParent->m_ball[m]->m_colour >= 1) &&  (m_pParent->m_ball[m]->m_colour <= 7))
					g_pottedSpots[y] = m;
				else if((m_pParent->m_ball[m]->m_colour >= 9) &&  (m_pParent->m_ball[m]->m_colour <= 15))
					g_pottedStripes[y] = m;					
			}
		}
	}
	/*			
	fy = 0;
	ballColour = 0;

	// draw spots on left-hand side		
	for(m=0;m<16;m++)
	if(g_pottedSpots[m] != -1)
	{			
		ballColour = m_pParent->m_ball[g_pottedSpots[m]]->m_colour; 
		
		// *TODO: 
		//if(m_pParent->m_colourseq == 2) // 1P=SPOTS & 2P=STRIPES
		//	draw2DBall(ballColour, x = LEFT, y = fy);
		//else if(m_pParent->m_colourseq == 3) // 1P=STRIPES & 2P=SPOTS
		//	draw2DBall(ballColour, x = RIGHT, y = fy);

		fy += 32;
	}

	char bbb[80];
	sprintf(bbb, "spots potted = %f", fy / 32); 
	ErrorMessage(m_pParent->m_parent, bbb);

	// draw stripes on left-hand side		
	fy = 0;
	ballColour = 0;
	for(m=0;m<16;m++)
	if(g_pottedStripes[m] != -1)
	{			
		ballColour = m_pParent->m_ball[g_pottedStripes[m]]->m_colour; 
		
		// *TODO: 
		//if(m_pParent->m_colourseq == 2) // 1P=SPOTS & 2P=STRIPES
		//	draw2DBall(ballColour, x = RIGHT, y = fy);
		//else if(m_pParent->m_colourseq == 3) // 1P=STRIPES & 2P=SPOTS
		//	draw2DBall(ballColour, x = LEFT, y = fy);

		fy += 32;
	}		
	
	sprintf(bbb, "stripes potted = %f", fy / 32); 
	ErrorMessage(m_pParent->m_parent, bbb);
	*/
}

//void CCustomTable::Referee8BallUpdateTimer(void)
//{	
	/*
	if(!m_pParent->m_score2P)
	{
		m_pParent->m_break2P--;
		
		if(m_pParent->if( MyTurn() ))
		{
			if(m_pParent->m_break2P < 0)
			{
				// SHOT-TIME FOUL! - pass turn over to opponent
				m_pParent->m_score2P = 1;
				GAMEAPPVIEW2->SendPlayerTimeUp();
			}
		}
	}
	*/
//}

//void CCustomTable::Referee8BallTimeUpReceive(void)
//{	
	/*
	m_pParent->m_score2P = 0;
	m_pParent->m_break2P = 60*50;

	// toggle turn:
	if(m_pParent->m_bTurn1P)
		m_pParent->m_bTurn1P = FALSE;
	else
		m_pParent->m_bTurn1P = TRUE;
	*/
//}


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////




void CCustomTable::RefereeUKPool(DWORD msg, void *pData)
{
	if(msg == CTMSG_LINESET)
	{
		RefereeUKPoolLINESET(msg, pData);
	}
	else if(msg == CTMSG_POCKETSET)
	{
		RefereeUKPoolPOCKETSET(msg, pData);
	}
	else if(msg == CTMSG_SIMEND)
	{
		Decode();
		RefereeUKPoolSIMEND();	// this is the CSnooker old style code
		Encode();
	}
	else
	{
		RefereeSnooker(msg, pData);
	}
}


void CCustomTable::RefereeUKPoolLINESET(DWORD msg, void *pData)
{
	ctmsg_LINESET_DATA *ctmsgData;
	ctmsgData = (ctmsg_LINESET_DATA_typ *) pData;


	// -----------------------------------------------------------------------------
	// check if player has not selected the white ball & we're not in Practice mode
	// -----------------------------------------------------------------------------
	//if(!m_pParent->m_rednext)
	  //if(m_pParent->m_colourseq < YELLOW)
		//if(m_pParent->m_testBall->m_colour == WHITE)
			//if(m_pParent->m_nominateBall == WHITE)
		if(!m_pParent->m_bPracticeMode)			
			if(m_pParent->m_selectBall != WHITE)
			{
				///		
				/// [simulate R-Button down for cancel shot]
				///

				m_pParent->m_bTestBallFinished = FALSE;
				//m_snooker->m_bCueTipReady = FALSE;

				// reset foucus to table (this) window, & reset 'raise butt'
				m_pParent->m_RaiseButt = m_pParent->m_RaiseButtOld = 0;
				m_pParent->m_bReceiveRaiseButtActive = FALSE;
				m_pParent->m_bRaiseButtHelper = TRUE;
				GAMEAPPVIEW2->m_sliderRaiseButt.SetPos(90);
				GAMEAPPVIEW2->PreviousWndEditSetFocus();

				GAMEAPPVIEW2->m_point = ctmsgData->point;
					
				// Only give this window the mouse's full attention 
				GAMEAPPVIEW2->SetCapture();
				
				if(m_pParent->m_tableMouseOn)
				{
					if(m_pParent->m_bCueState)
					{
						// Now Give ,mouse's attention to all
						if(GAMEAPPVIEW2->GetCapture() == GAMEAPPVIEW2) ReleaseCapture();

						// give mouse full movement
						ClipCursor(&GAMEAPPVIEW2->m_rectCursor);

						// reset mouse cursor position back to start position
						CRect rect;
						GAMEAPPVIEW2->GetWindowRect(&rect);
						SetCursorPos(rect.left + m_pParent->m_sCPoint.x+2, rect.top + m_pParent->m_sCPoint.y+2);
					
						m_pParent->SelectBall(0, ctmsgData->point);
						
						m_pParent->m_point = ctmsgData->point;
						m_pParent->m_sightlineFade = 0;
						
						if(m_pParent->m_bPracticeMode);
						else
							m_pParent->SelectBallSend(0, ctmsgData->point, m_pParent->m_windowWidth, m_pParent->m_windowHeight, m_pParent->m_selectBall);
					}
				}

				if(!m_pParent->m_cueBallSelect)
				if(!GAMEAPPVIEW2->m_bShowCursor)
				{
					GAMEAPPVIEW2->m_bShowCursor = TRUE;
					SetCursor(GAMEAPPVIEW2->m_hCursor);
					ShowCursor(GAMEAPPVIEW2->m_bShowCursor); // show cursor
				}

				///
				///
				///

				GAMEAPPVIEW2->AlertMessage("Warning! You can only strike the 'White' ball!\n\n(Use 'Left-Mouse-Button' to select the 'White' ball)");
				return;
			}


	// ---------------------------------------------
	// normal 'LineSet' here!
	// ---------------------------------------------

	
//	/////////////////////////////////////////////////////////////////////////////////////////////////////
//	// set default target pocket
//	if(m_pParent->m_testBall->m_colour == 8) // if player has lined up the 8-ball
//	{
//		// initially...
//		if(m_pParent->m_targetPocket == -1)
//			m_pParent->m_targetPocket = -2; // default: point at all pockets
//		
//		// send target ball/pocket/success data to all at table
//		GAMEAPPVIEW2->SendPlayerTargetData(m_pParent->m_targetPocket, m_pParent->m_targetBall, m_pParent->m_bTargetSuccess);
//	}
//	/////////////////////////////////////////////////////////////////////////////////////////////////////

	// Now Give ,mouse's attention to all
	if(GAMEAPPVIEW2->GetCapture() == GAMEAPPVIEW2) ReleaseCapture();
			
	// test!!! send final cue-line pos
	GAMEAPPVIEW2->SendPlayerMouseMove(m_pParent->m_mouseX, m_pParent->m_mouseY, 
							m_pParent->m_windowWidth, m_pParent->m_windowHeight,
							m_pParent->m_strikeLine, m_pParent->m_strikeDist);

	m_pParent->m_cueBallSelect = 5; //--
	m_pParent->m_mouseX = 0; // position cuetip GFX pointer at centre of selected ball
	m_pParent->m_mouseY = 0;

	GAMEAPPVIEW2->m_sliderRaiseButt.EnableWindow(TRUE);
	m_pParent->m_bRaiseButtHelper = TRUE;

	// give mouse full movement
	ClipCursor(&GAMEAPPVIEW2->m_rectCursor);

	if(!GAMEAPPVIEW2->m_bShowCursor)
	{
		GAMEAPPVIEW2->m_bShowCursor = TRUE;
		SetCursor(GAMEAPPVIEW2->m_hCursor);
		ShowCursor(GAMEAPPVIEW2->m_bShowCursor); // show cursor
	}

	// reset mouse cursor position back to start position
	CRect rect;
	GAMEAPPVIEW2->GetWindowRect(&rect);
	SetCursorPos(rect.left + m_pParent->m_sCPoint.x+2, rect.top + m_pParent->m_sCPoint.y+2);

	m_pParent->m_bCueLineSet = TRUE;
	
	m_pParent->m_tableMousePosStrike[0] = m_pParent->m_tableMousePos[0];
	m_pParent->m_tableMousePosStrike[1] = m_pParent->m_tableMousePos[1];
	m_pParent->m_tableMousePosStrike[2] = m_pParent->m_tableMousePos[2];
}


void CCustomTable::RefereeUKPoolPOCKETSET(DWORD msg, void *pData)
{	
	ctmsg_POCKETSET_DATA *ctmsgData;
	ctmsgData = (ctmsg_POCKETSET_DATA_typ *) pData;
	
	int n, m;

	if(m_pParent->m_cueBallSelect != 5) return;	// only in state '5'(after line is set!), are we allowed to do the following

	if(m_pParent->m_colourseq == 0) return; // if this is a break-off shot, dont do the following

//	// check if player has selected the black ball (8-ball) SPECIAL CASE: ALWAYS DO FOR BLACK BALL IN 8-BALL!!!
//	if(m_pParent->m_testBall->m_colour == 8)
//	{
//		//if(m_pParent->m_targetPocket == -1)
//		//	m_pParent->m_targetPocket = 2; // default: select middle pocket
//		
//		m_pParent->SetTargetBall(8);//m_targetBall = 8;
//  
//		n = m_pParent->GetSelectedPocket();
//		// MUST BE IN FIXMODE!
//		/*
//		{
//			char buf[256];
//			sprintf(buf, "n = %d", n);
//			ErrorMessage(m_pParent->m_parent, buf);
//		}
//		*/
//  
//		if(n != -1)	// if player has clicked inside a pocket
//		{
//			m_pParent->SetTargetPocket(n);
//			
//			// send target ball/pocket/success data to all at table
//			GAMEAPPVIEW2->SendPlayerTargetData(m_pParent->m_targetPocket, m_pParent->m_targetBall, m_pParent->m_bTargetSuccess);
//		}
//	}

	if(m_pParent->m_bCallShot) // default
	{
		n = m_pParent->GetSelectedPocket();
		m = m_pParent->GetSelectedBall();
		// MUST BE IN FIXMODE!
/*		
		{
			char buf[256];
			sprintf(buf, "n = %d, m = %d", n, m);
			ErrorMessage(m_pParent->m_parent, buf);
		}
*/		

		if(n != -1 || m != -1)	// if player has clicked inside a pocket OR has clicked on a ball
		{
			if(n != -1) m_pParent->SetTargetPocket(n);
			if(m != -1) m_pParent->SetTargetBall(m);

			// send target ball/pocket/success data to all at table
			GAMEAPPVIEW2->SendPlayerTargetData(m_pParent->m_targetPocket, m_pParent->m_targetBall, m_pParent->m_bTargetSuccess);
		}
	}
}


void CCustomTable::RefereeUKPoolRESPOTBALL(int n)
{
	int m;
	int dir;
	BOOL successful;
	float f1;

	/////////////////////////////////////////////////////////////////////
	// (3) try and spot behind starting position in a direct line
	/////////////////////////////////////////////////////////////////////					
	// check to left side of spot (dir = -1)..., then the right side (dir = 1)...
	if(!TestFlag(m_pParent->m_ball[n]->m_flags, Flg_BALL_ACTIVE))
	for(dir = -1; dir <= 1; dir+=2)
	{
		CVector F = CVector(WORLD_X(591+65+120), 0, m_pParent->m_ball[n]->m_PSpot.z);
		
		// place ball on foot spot
		m_pParent->m_ball[n]->m_POld = m_pParent->m_ball[n]->m_P = F;//m_ball[n]->m_PSpot;
		m_pParent->m_ball[n]->m_W = m_pParent->m_ball[n]->m_V = m_pParent->m_ball[n]->m_VOld = CVector(0,0,0);

		do
		{
			f1 = BALL_RADIUS/20.0f;
			f1  = f1 * (float)dir;
			m_pParent->m_ball[n]->m_P.x -= f1;
			//m_ball[n]->m_P.x -= (BALL_RADIUS/20)*dir;

			successful = TRUE;
			for(m=0;m<MAX_TOTALBALLS;m++)
			if(n!=m)
			if(TestFlag(m_pParent->m_ball[m]->m_flags, Flg_BALL_ACTIVE)) // NEW: make sure this ball is active
			{
				if(!m_pParent->m_ball[n]->TryReSpot3(m_pParent->m_ball[m]))
					successful = FALSE;
			}

			// is this ball on top of a pocket
			float x = m_pParent->m_ball[n]->m_P.x;
			float y = m_pParent->m_ball[n]->m_P.y;
			for(int p=0;p<m_pocket_n;p++)
			{
				if( PocketProximity(p, CVector(x, y, 0), BALL_RADIUS) )
				{						
					successful = FALSE;
					break;
				}
			} // end for p

			if(successful) {
				m_pParent->m_ball[n]->ReSpotSuccessful();
				goto end;
			}

		}
		//while(1);
		while(!m_pParent->m_ball[n]->CushionsProximity(FALSE));
		//while((m_ball[n]->m_P.x >= (-1736+BALL_RADIUS)) ||
		//	  (m_ball[n]->m_P.x <= (1736-BALL_RADIUS)));   // bottom cushion + BALL_RADIUS || top cushion - BALL_RADIUS
		// *TODO: note: the above boundary check is incorrect!!!, it should be using the actual 3d world "Cushions"
	}
	/////////////////////////////////////////////////////////////////////

end:;

	#ifdef FIXEDMODE
	m_pParent->m_ball[n]->UpdateFixeds();
	#endif
}


void CCustomTable::RefereeUKPoolSIMEND(void)
{
	int n;
	int colour;
	int no_colours;
	BOOL bWhiteInPocket = FALSE;
	BOOL bNextPlayer = FALSE;
	BOOL bPottedOwnGroupBall = TRUE;
	BOOL bCalledBallWasPotted = FALSE;

	int _spots = 0;
	int _stripes = 0;
	int _8ball = 0;
	int _pottedspots = 0;
	int _pottedstripes = 0;
	int _potted8ball = 0;

	m_pParent->m_shotcount++;

	m_pParent->m_bMiss = TRUE;												// assume no balls potted
	ClearFlag(m_pParent->m_foul, Flg_FOUL_NORMAL | Flg_FOUL_WHITEPOTTED | Flg_FOUL_GAMEOVER | Flg_FOUL_BALLINHAND);	// assume no foul to start with [keep some flags from previous shot]
	ClearFlag(m_pParent->m_foul, Flg_FOUL_ALERT | Flg_FOUL_NOTHITCUSHIONBREAK);
	m_pParent->m_bRefreshScores = TRUE;

	
	if(!m_pParent->m_bCallShot)
		bCalledBallWasPotted = TRUE; // just for safety, in case I forget to use -> "if(m_pParent->m_bCallShot)"

	

	for(n=0;n<MAX_TOTALBALLS;n++)
	{
		if(TestFlag(m_pParent->m_ball[n]->m_flags, Flg_BALL_ACTIVE))
		{
			if(m_pParent->m_ball[n]->m_colour >= 1 && m_pParent->m_ball[n]->m_colour <= 7)
			{
				_spots++;
			}
			else if(m_pParent->m_ball[n]->m_colour >= 9 && m_pParent->m_ball[n]->m_colour <= 15)
			{
				_stripes++;
			}
			else if(m_pParent->m_ball[n]->m_colour == 8)
			{
				_8ball++;
			}
		}
	}

	//char buf3[80];
	//sprintf(buf3, "spots = %d; stripes = %d", _spots, _stripes);
	//ErrorMessage(m_pParent->m_parent, buf3);

	/////////////////////////////////////////////////////////////
	// what was pottted?
	

	for(n=0;n<MAX_TOTALBALLS;n++)
	{
		if(TestFlag(m_pParent->m_ball[n]->m_flags, Flg_BALL_POTTED))
		{
			// was target/called ball potted?
			if(m_pParent->m_bCallShot)
			if(m_pParent->m_bTargetSuccess)
			if(m_pParent->m_targetBall == m_pParent->m_ball[n]->m_colour)
				bCalledBallWasPotted = TRUE;

			if(m_pParent->m_ball[n]->m_colour >= 1 && m_pParent->m_ball[n]->m_colour <= 7)
			{
				_pottedspots++;
			}
			else if(m_pParent->m_ball[n]->m_colour >= 9 && m_pParent->m_ball[n]->m_colour <= 15)
			{
				_pottedstripes++;
			}
			else if(m_pParent->m_ball[n]->m_colour == 8)
			{
				_potted8ball++;
			}
		}
	}

	/////////////////////////////////////////////////////////////
	// see if white ball has been potted (foul)
	for(n=0;n<MAX_TOTALBALLS;n++)
	{
		if(TestFlag(m_pParent->m_ball[n]->m_flags, Flg_BALL_POTTED))
			if(m_pParent->m_ball[n]->m_colour == WHITE)
			{
				bWhiteInPocket = TRUE;
				if(m_pParent->m_colourseq == 0) SetFlag(m_pParent->m_foul, Flg_FOUL_NORMAL | Flg_FOUL_WHITEPOTTED | Flg_FOUL_BREAK);
				else{
					m_pParent->UpdateGameReceive();
					SetFlag(m_pParent->m_foul, Flg_FOUL_NORMAL);
					//goto end;
					}
						
					//
				//m_pParent->m_bBallInHand = 1;
				//goto end;
			}
	}
	/////////////////////////////////////////////////////////////


	/////////////////////////////////////////////////////////////
	// what happens if player strikes a ball other than the white
	if(m_pParent->m_selectBall != WHITE)
	{
		SetFlag(m_pParent->m_foul, Flg_FOUL_NORMAL | Flg_FOUL_ALERTUKPOOLFOUL);
		
		if(_potted8ball != 0)
		{
			// *TODO - FOUL!!!! DO RERACK!!!!
			SetFlag(m_pParent->m_foul, Flg_FOUL_GAMEOVER);
			goto end;
		}

		//goto end;
	}
	/////////////////////////////////////////////////////////////

	
	
	/////////////////////////////////////////////////////////////
	// if white doesn't hit a ball at all, foul ball on!
	// if(m_colourseq < YELLOW) // bypass if on colours at the end
	// if white was potted, then we already have handled foul code
	if((m_pParent->m_selectBall == WHITE) && (m_pParent->m_dominoHit1st == WHITE))
	if(!TestFlag(m_pParent->m_foul, Flg_FOUL_WHITEPOTTED))
	{
			m_pParent->m_foul = 0;
			SetFlag(m_pParent->m_foul, Flg_FOUL_NORMAL | Flg_FOUL_ALERTUKPOOLFOUL);

			if(_potted8ball != 0)
			{
				// *TODO - FOUL!!!! DO RERACK!!!!
				SetFlag(m_pParent->m_foul, Flg_FOUL_GAMEOVER);
				goto end;
			}

			//goto end;
	}
	/////////////////////////////////////////////////////////////

	///
	// [NEW] if nothing was potted, did cueball or spotball touch a cushion?
	if(m_pParent->m_colourseq > 0)
	if(!bWhiteInPocket)
	//if(!bPushOutShot)
	if(_pottedspots == 0 && _pottedstripes == 0 && _potted8ball == 0) // check nothing was potted
	{
		BOOL bFailedToHitCushion = TRUE;
		
		for(n=0;n<MAX_TOTALBALLS;n++)
		{	
			if(m_pParent->m_ball[n]->m_hitCushion)
			{
				bFailedToHitCushion = FALSE; // no foul!
				break;
			}
		}

		if(bFailedToHitCushion)
		{
			SetFlag(m_pParent->m_foul, Flg_FOUL_NORMAL | Flg_FOUL_NOTHITCUSHION | Flg_FOUL_ALERTUKPOOLFOUL);
		}
	}
	///////////////////////////////////////////////////////////////////////////
	///


				if(m_pParent->m_colourseq == 0)
					{
					// on the break!
					// If the Eight-Ball is potted on any break, the balls are re-racked and the same player
					// will break again. When the Eight-Ball is potted on the break, all other aspects of 
					// the shot are ignored.
					if(_potted8ball != 0)
						{
						// Re-Rack the Game Auto-Matically //
						m_pParent->m_foul = 0;
						// usa
						m_pParent->Pot8OnBreakRerackReceive(1);
						m_pParent->m_bMiss = FALSE;	// clear this, so that the 'miss' block of code below is bypassed
						m_pParent->m_bBallInHand = 1;
						goto End_BadBreak;
						}
					

					///
					// [NEW] if nothing was potted on the break, did >= 4 numbered balls touch a cushion?					
					//if(!bWhiteInPocket)
					if(_pottedspots == 0 && _pottedstripes == 0)
						{						
						int hitCushion = 0;
						for(n=1;n<MAX_TOTALBALLS;n++) // exclude white ball
						if(m_pParent->m_ball[n]->m_hitCushion) hitCushion++; // (at least 4 numbered balls)
								
						if(hitCushion < 4)
							{
							// The opponent is awarded two visits.
							// The balls are re-racked.
							// The opponent re-starts the game and is under the same obligation to achieve a Fair Break.

							if(!m_pParent->m_bPracticeMode || GAMEAPPVIEW2->m_bPlayOffline)// && !GAMEAPPVIEW2->m_bReplayMode)
								{

								// Re-Rack the Game Auto-Matically //
								m_pParent->m_foul = 0;
								m_pParent->Pot8OnBreakRerackReceive();
								m_pParent->m_bMiss = FALSE;	// clear this, so that the 'miss' block of code below is bypassed
								m_pParent->m_bBallInHand = 1;


								// foul! So swap turn
								if(m_pParent->m_bTurn1P)
									m_pParent->m_bTurn1P = FALSE;
								else
									m_pParent->m_bTurn1P = TRUE;

								// Next Player at the Table is allowed 2 Shots, unless he/she fouls //
								// No point in giving 2 lifes if the game's already over //
								if(!TestFlag(m_pParent->m_foul, Flg_FOUL_GAMEOVER))
									{
									m_pParent->m_lifes = 1;
									sprintf(g_gameAppView->m_sponsortxt, "First Visit (2 Shots)");
									g_gameAppView->m_sponsoralpha = 0.5f;
									}
								}


							//SetFlag(m_pParent->m_foul, Flg_FOUL_NORMAL | Flg_FOUL_NOTHITCUSHIONBREAK | Flg_FOUL_ALERT | Flg_FOUL_ALERTUKPOOLFOUL);
							goto End_BadBreak;
							}

						//char buf[80];
						//sprintf(buf, "hitCushion = %d", hitCushion);
						//ErrorMessage(m_pParent->m_parent, buf);
						}					
					m_pParent->m_colourseq = 1;	
					

				}
				else if(m_pParent->m_colourseq == 1)
				{
					// shots after the break : open table
					//ErrorMessage(m_pParent->m_parent, "[m_colourseq == 1]");
					if(_potted8ball != 0)
					{
						/////////////////////////////////
						// special case: [*NEW*]
						// what if no spots or stripes are left AND no spots or stripes were potted on this turn?
						if(_spots == 0 && _stripes == 0)
						if(_pottedspots == 0 && _pottedstripes == 0)
						if(!m_pParent->m_bCallShot || m_pParent->m_bTargetSuccess) // NEW* did player pot in the target pocket?
						if(!bWhiteInPocket) // so long as white wasn't potted aswell
						{
							//ErrorMessage(m_pParent->m_parent, "Target Pocket Success!!!!!");
							// this player has won!
							if(m_pParent->m_bTurn1P)
							{
								m_pParent->m_gameOver = 2;

								if( (m_pParent->m_break2P & 0x01) && !(m_pParent->m_break2P & 0x04) )
								{
									m_pParent->m_highBreaks1P[0] = 70; // run-out for UK Pool stat.
									Msfx_smpplayvol(&GAMEAPPVIEW2->m_game_smp[SFX_GOODSHOT], 255);
								}

								goto end;
							}
							else
							{
								m_pParent->m_gameOver = 1;

								if( (m_pParent->m_break2P & 0x02) && !(m_pParent->m_break2P & 0x08) )
								{
									m_pParent->m_highBreaks2P[0] = 70; // run-out for UK Pool stat.
									Msfx_smpplayvol(&GAMEAPPVIEW2->m_game_smp[SFX_GOODSHOT], 255);
								}

								goto end;
							}
						}
						/////////////////////////////////
						
						// otherwise lost!...
						// *TODO - FOUL!!!! DO RERACK!!!!
						SetFlag(m_pParent->m_foul, Flg_FOUL_NORMAL | Flg_FOUL_GAMEOVER);
						goto end;
					}

					if(m_pParent->m_bCallShot) // when player pockets the called ball, player gets that group!
					{
						// was a ball just potted?
						if(_pottedspots > 0 || _pottedstripes > 0)
						{
							// did player pot called ball in called pocket?
							if(m_pParent->m_bTargetSuccess)
							{								
								// what group was called ball from?			
								if(m_pParent->m_targetBall >= 1 && m_pParent->m_targetBall <= 7)
								{
									if(m_pParent->m_bTurn1P)
										m_pParent->m_colourseq = 2;	// 1P going for spots
									else
										m_pParent->m_colourseq = 3;	// 1P going for stripes
								}
								else if(m_pParent->m_targetBall >= 9 && m_pParent->m_targetBall <= 15)
								{
									if(m_pParent->m_bTurn1P)
										m_pParent->m_colourseq = 3;	// 1P going for stripes
									else
										m_pParent->m_colourseq = 2;	// 1P going for spots
								}
							}
							else
							{
								// foul!
								ClearFlag(m_pParent->m_foul, Flg_FOUL_NOTHITCUSHION);
								SetFlag(m_pParent->m_foul, Flg_FOUL_NORMAL | Flg_FOUL_ALERTUKPOOLFOUL);
								goto end;
							}
						}
					}
					else
					{ // default: old way, player must pocket 1 type of group only, to get that group in an open table
						if(m_pParent->m_bTurn1P)
						{
							if(_pottedspots > 0 && _pottedstripes == 0)
								m_pParent->m_colourseq = 2;	// 1P going for spots
							else if(_pottedspots == 0 && _pottedstripes > 0)
								m_pParent->m_colourseq = 3;	// 1P going for stripes
						}
						else
						{
							if(_pottedspots > 0 && _pottedstripes == 0)
								m_pParent->m_colourseq = 3;	// 2P going for spots
							else if(_pottedspots == 0 && _pottedstripes > 0)
								m_pParent->m_colourseq = 2;	// 2P going for stripes
						}
					}
				}
				else if(m_pParent->m_colourseq == 2)  // 1P going for spots...
				{
					//ErrorMessage(m_pParent->m_parent, "[m_colourseq == 2]");

					if(m_pParent->m_bTurn1P)
					{
						
						colour = m_pParent->m_ball[m_pParent->m_dominoHit1st]->m_colour;

						/// handle 8-ball /////////////////////////////////////////////////////
						if(_potted8ball > 0)
						{
							if(colour != 8 || bWhiteInPocket)
							{
								// *TODO - FOUL!!!! DO RERACK!!!!						
								SetFlag(m_pParent->m_foul, Flg_FOUL_NORMAL | Flg_FOUL_GAMEOVER);
								goto end;									
							}
							else
							{
		
								if(_pottedspots > 0 || _spots > 0)// || (_pottedstripes > 0))
								{
									// *TODO - FOUL!!!! DO RERACK!!!!
									SetFlag(m_pParent->m_foul, Flg_FOUL_NORMAL | Flg_FOUL_GAMEOVER);
									goto end;
								}
								else
								{
									// NEW* if player did NOT pot in the target pocket?
									if(m_pParent->m_bCallShot && !m_pParent->m_bTargetSuccess)
									{
										SetFlag(m_pParent->m_foul, Flg_FOUL_NORMAL | Flg_FOUL_GAMEOVER);
										goto end;
									}

									//ErrorMessage(m_pParent->m_parent, "Target Pocket Success!!!!!");
									// success - this player has won! i.e. other player has lost!
									//ErrorMessage(m_pParent->m_parent, "SUCCESS");
									m_pParent->m_gameOver = 2;

									if( (m_pParent->m_break2P & 0x01) && !(m_pParent->m_break2P & 0x04) )
									{
										m_pParent->m_highBreaks1P[0] = 70; // run-out for UK Pool stat.
										Msfx_smpplayvol(&GAMEAPPVIEW2->m_game_smp[SFX_GOODSHOT], 255);
									}
									goto end;
								}
							}
						}

						if(colour == 8)
						{
							// if we hit the 8-ball	first, and we still haven't ran out spots/stripes, then foul!
							// In UK Pool, we are allowed to hit the 8-Ball if it's the shot after a Foul //
							if(m_pParent->m_lifes == 1 && m_pParent->m_poolPots1P == 0);
							else if(_pottedspots > 0 || _spots > 0)
							{
								ClearFlag(m_pParent->m_foul, Flg_FOUL_NOTHITCUSHION);
								SetFlag(m_pParent->m_foul, Flg_FOUL_NORMAL | Flg_FOUL_ALERTUKPOOLFOUL);
								goto end;
							}
						}

						///////////////////////////////////////////////////////////////////////
						/*
						if(_pottedspots > 0 || _spots > 0)
							if(_potted8ball != 0)
							{
								// *TODO - FOUL!!!! DO RERACK!!!!						
								SetFlag(m_pParent->m_foul, Flg_FOUL_NORMAL | Flg_FOUL_GAMEOVER);
								goto end;
							}
						*/
						


						// check if player hasn't potted any of their own group
						if(_pottedspots == 0)
							bPottedOwnGroupBall = FALSE;

						

						// check for hitting / potting wrong group ball
						// Allow this if it's a shot after a foul //						
						if(m_pParent->m_lifes == 1 && m_pParent->m_poolPots1P == 0);
						else{
							// If we hit an opponent's ball first or potted one, then set the Foul Flag //
							if(colour >= 9 || _pottedstripes > 0)
								{
								ClearFlag(m_pParent->m_foul, Flg_FOUL_NOTHITCUSHION);
								SetFlag(m_pParent->m_foul, Flg_FOUL_NORMAL | Flg_FOUL_ALERTUKPOOLFOUL);
								goto end;
								}	
							}

						// did player pot called ball
						if(m_pParent->m_bCallShot)
						if(_pottedspots > 0)
						if(!bCalledBallWasPotted)
						{  
							ClearFlag(m_pParent->m_foul, Flg_FOUL_NOTHITCUSHION);
							SetFlag(m_pParent->m_foul, Flg_FOUL_NORMAL | Flg_FOUL_ALERTUKPOOLFOUL);
							goto end;
						}
					}
					else
					{
						colour = m_pParent->m_ball[m_pParent->m_dominoHit1st]->m_colour;

						/// handle 8-ball /////////////////////////////////////////////////////
						if(_potted8ball > 0)
						{
							if(colour != 8 || bWhiteInPocket)
							{
								// *TODO - FOUL!!!! DO RERACK!!!!						
								SetFlag(m_pParent->m_foul, Flg_FOUL_NORMAL | Flg_FOUL_GAMEOVER);
								goto end;									
							}
							else
							{
								//ErrorMessage(m_pParent->m_parent, "[m_colourseq == 2]BLACK HIT FIRST2");
								if(_pottedstripes > 0 || _stripes > 0)// || (_pottedspots > 0))
								{
									// *TODO - FOUL!!!! DO RERACK!!!!						
									SetFlag(m_pParent->m_foul, Flg_FOUL_NORMAL | Flg_FOUL_GAMEOVER);
									goto end;
								}
								else
								{
									// NEW* if player did NOT pot in the target pocket?
									if(m_pParent->m_bCallShot && !m_pParent->m_bTargetSuccess)
									{
										SetFlag(m_pParent->m_foul, Flg_FOUL_NORMAL | Flg_FOUL_GAMEOVER);
										goto end;
									}

//									ErrorMessage(m_pParent->m_parent, "Target Pocket Success!!!!!");
									// success - this player has won! i.e. other player has lost!
									//ErrorMessage(m_pParent->m_parent, "SUCCESS");
									m_pParent->m_gameOver = 1;

									if( (m_pParent->m_break2P & 0x02) && !(m_pParent->m_break2P & 0x08) )
										m_pParent->m_highBreaks2P[0] = 70; // 80 => run-out for UKPool stat.

									goto end;
								}
							}
						}

						if(colour == 8)
						{
							// if we hit the 8-ball	first, and we still haven't ran out spots/stripes, then foul!
							// In UK Pool, we are allowed to hit the 8-Ball if it's the shot after a Foul //
							if(m_pParent->m_lifes == 1 && m_pParent->m_poolPots1P == 0);
							else if(_pottedstripes > 0 || _stripes > 0)
							{
								ClearFlag(m_pParent->m_foul, Flg_FOUL_NOTHITCUSHION);
								SetFlag(m_pParent->m_foul, Flg_FOUL_NORMAL | Flg_FOUL_BALLINHAND);
								goto end;
							}
						}

						///////////////////////////////////////////////////////////////////////
						/*
						if(_pottedstripes > 0 || _stripes > 0)
							if(_potted8ball != 0)
							{
								// *TODO - FOUL!!!! DO RERACK!!!!						
								SetFlag(m_pParent->m_foul, Flg_FOUL_NORMAL | Flg_FOUL_GAMEOVER);
								goto end;
							}
						  */

						// check if player hasn't potted any of their own group
						if(_pottedstripes == 0)
							bPottedOwnGroupBall = FALSE;
						
						// check for hitting / potting wrong group ball
						// Allow this if it's a shot after a foul //						
						if(m_pParent->m_lifes == 1 && m_pParent->m_poolPots1P == 0);
						else{
							if(colour <= 7 || _pottedspots > 0)
								{
								// If we hit an opponent's ball first or potted one, then set the Foul Flag //
								ClearFlag(m_pParent->m_foul, Flg_FOUL_NOTHITCUSHION);
								SetFlag(m_pParent->m_foul, Flg_FOUL_NORMAL | Flg_FOUL_ALERTUKPOOLFOUL);
								goto end;
								}
							}

						// did player pot called ball
						if(m_pParent->m_bCallShot)
						if(_pottedstripes > 0)
						if(!bCalledBallWasPotted)
						{  
							ClearFlag(m_pParent->m_foul, Flg_FOUL_NOTHITCUSHION);
							SetFlag(m_pParent->m_foul, Flg_FOUL_NORMAL | Flg_FOUL_ALERTUKPOOLFOUL);
							goto end;
						}
					}
				}
				else if(m_pParent->m_colourseq == 3) // 1P going for stripes
				{
					//ErrorMessage(m_pParent->m_parent, "[m_colourseq == 3]");
					if(m_pParent->m_bTurn1P)
					{
						
						colour = m_pParent->m_ball[m_pParent->m_dominoHit1st]->m_colour;

						/// handle 8-ball /////////////////////////////////////////////////////
						if(_potted8ball > 0)
						{
							if(colour != 8 || bWhiteInPocket)
							{
								// *TODO - FOUL!!!! DO RERACK!!!!						
								SetFlag(m_pParent->m_foul, Flg_FOUL_NORMAL | Flg_FOUL_GAMEOVER);
								goto end;									
							}
							else
							{
								//ErrorMessage(m_pParent->m_parent, "[m_colourseq == 3]BLACK HIT FIRST");
								if(_pottedstripes > 0 || _stripes > 0)// || (_pottedspots > 0))
								{
									// *TODO - FOUL!!!! DO RERACK!!!!						
									SetFlag(m_pParent->m_foul, Flg_FOUL_NORMAL | Flg_FOUL_GAMEOVER);
									goto end;
								}
								else
								{
									// NEW* if player did NOT pot in the target pocket?
									if(m_pParent->m_bCallShot && !m_pParent->m_bTargetSuccess)
									{
										SetFlag(m_pParent->m_foul, Flg_FOUL_NORMAL | Flg_FOUL_GAMEOVER);
										goto end;
									}

//									ErrorMessage(m_pParent->m_parent, "Target Pocket Success!!!!!");
									//ErrorMessage(m_pParent->m_parent, "SUCCESS");
									// success - this player has won! i.e. other player has lost!
									m_pParent->m_gameOver = 2;

									if( (m_pParent->m_break2P & 0x01) && !(m_pParent->m_break2P & 0x04) )
									{
										m_pParent->m_highBreaks1P[0] = 70; // => run-out for UK Pool stat.
										Msfx_smpplayvol(&GAMEAPPVIEW2->m_game_smp[SFX_GOODSHOT], 255);
									}

									goto end;
								}
							}
						}

						if(colour == 8)
						{
							// if we hit the 8-ball	first, and we still haven't ran out spots/stripes, then foul!
							// allow it, if this is a shot after a foul //
							if(m_pParent->m_lifes == 1 && m_pParent->m_poolPots1P == 0);
							else if(_pottedstripes > 0 || _stripes > 0)
							{
								ClearFlag(m_pParent->m_foul, Flg_FOUL_NOTHITCUSHION);
								SetFlag(m_pParent->m_foul, Flg_FOUL_NORMAL | Flg_FOUL_BALLINHAND);
								goto end;
							}
						}

						///////////////////////////////////////////////////////////////////////
						/*
						if(_pottedstripes > 0 || _stripes > 0)
							if(_potted8ball != 0)
							{
								// *TODO - FOUL!!!! DO RERACK!!!!						
								SetFlag(m_pParent->m_foul, Flg_FOUL_NORMAL | Flg_FOUL_GAMEOVER);
								goto end;
							}
						*/

						// check if player hasn't potted any of their own group balls
						if(_pottedstripes == 0)
							bPottedOwnGroupBall = FALSE;

						// check for hitting / potting wrong group ball
						// Allow this if it's a shot after a foul //						
						if(m_pParent->m_lifes == 1 && m_pParent->m_poolPots1P == 0);
						else{
							// If we hit an opponent's ball first or potted one, then set the Foul Flag //
							if(colour <= 7 || _pottedspots > 0)
								{
								ClearFlag(m_pParent->m_foul, Flg_FOUL_NOTHITCUSHION);
								SetFlag(m_pParent->m_foul, Flg_FOUL_NORMAL | Flg_FOUL_ALERTUKPOOLFOUL);
								goto end;
								}
							}

						// did player pot called ball
						if(m_pParent->m_bCallShot)
						if(_pottedstripes > 0)
						if(!bCalledBallWasPotted)
						{  
							ClearFlag(m_pParent->m_foul, Flg_FOUL_NOTHITCUSHION);
							SetFlag(m_pParent->m_foul, Flg_FOUL_NORMAL | Flg_FOUL_ALERTUKPOOLFOUL);
							goto end;
						}
					}
					else
					{

						colour = m_pParent->m_ball[m_pParent->m_dominoHit1st]->m_colour;

						/// handle 8-ball /////////////////////////////////////////////////////
						if(_potted8ball > 0)
						{
							if(colour != 8 || bWhiteInPocket)
							{
								// *TODO - FOUL!!!! DO RERACK!!!!						
								SetFlag(m_pParent->m_foul, Flg_FOUL_NORMAL | Flg_FOUL_GAMEOVER);
								goto end;									
							}
							else
							{
								//ErrorMessage(m_pParent->m_parent, "[m_colourseq == 3]BLACK HIT FIRST2");
								if(_pottedspots > 0 || _spots > 0)// || (_pottedstripes > 0))
								{
									// *TODO - FOUL!!!! DO RERACK!!!!						
									SetFlag(m_pParent->m_foul, Flg_FOUL_NORMAL | Flg_FOUL_GAMEOVER);
									goto end;
								}
								else
								{
									// NEW* if player did NOT pot in the target pocket?
									if(m_pParent->m_bCallShot && !m_pParent->m_bTargetSuccess)
									{
										SetFlag(m_pParent->m_foul, Flg_FOUL_NORMAL | Flg_FOUL_GAMEOVER);
										goto end;
									}

//									ErrorMessage(m_pParent->m_parent, "Target Pocket Success!!!!!");
									//ErrorMessage(m_pParent->m_parent, "SUCCESS");
									// success - this player has won! i.e. other player has lost!
									m_pParent->m_gameOver = 1;

									if( (m_pParent->m_break2P & 0x02) && !(m_pParent->m_break2P & 0x08) )
										m_pParent->m_highBreaks2P[0] = 70; // 50 => run-out for UK Pool stat.

									goto end;
								}
							}
						}

						if(colour == 8)
						{
							// if we hit the 8-ball	first, and we still haven't ran out spots/stripes, then foul!
							// allow it, if it's a shot after the foul 
							if(m_pParent->m_lifes == 1 && m_pParent->m_poolPots1P == 0);
							else if(_pottedspots > 0 || _spots > 0)
							{
								ClearFlag(m_pParent->m_foul, Flg_FOUL_NOTHITCUSHION);
								SetFlag(m_pParent->m_foul, Flg_FOUL_NORMAL | Flg_FOUL_BALLINHAND);
								goto end;
							}
						}

						///////////////////////////////////////////////////////////////////////
						/*
						if(_pottedspots > 0 || _spots > 0)
							if(_potted8ball != 0)
							{
								// *TODO - FOUL!!!! DO RERACK!!!!						
								SetFlag(m_pParent->m_foul, Flg_FOUL_NORMAL | Flg_FOUL_GAMEOVER);
								goto end;
							}
						*/

						// check if player hasn't potted any of their own group balls
						if(_pottedspots == 0)
							bPottedOwnGroupBall = FALSE;

						// check for hitting / potting wrong group ball
						// Allow this if it's a shot after a foul //
						if(m_pParent->m_lifes == 1 && m_pParent->m_poolPots1P == 0);
						else{
							// If we hit an opponent's ball first or potted one, then set the Foul Flag //
							if(colour >= 9 || _pottedstripes > 0)
								{
								ClearFlag(m_pParent->m_foul, Flg_FOUL_NOTHITCUSHION);
								SetFlag(m_pParent->m_foul, Flg_FOUL_NORMAL | Flg_FOUL_ALERTUKPOOLFOUL);
								goto end;
								}
							}

						// did player pot called ball
						if(m_pParent->m_bCallShot)
						if(_pottedspots > 0)
						if(!bCalledBallWasPotted)
						{  
							ClearFlag(m_pParent->m_foul, Flg_FOUL_NOTHITCUSHION);
							SetFlag(m_pParent->m_foul, Flg_FOUL_NORMAL | Flg_FOUL_ALERTUKPOOLFOUL);
							goto end;
						}
					}

				}

					//if(m_pParent->m_bTurn1P)

	//			if(m_pParent->m_ball[n]->m_colour == WHITE)
	///			{
	//				bWhiteInPocket = TRUE;
	//				SetFlag(m_pParent->m_foul, Flg_FOUL_NORMAL | Flg_FOUL_WHITEPOTTED);
	//			}
	//		}
	//	}
	  
	/////////////////////////////////////////////////////////////
	//char buf[80];
	//sprintf(buf, "-- m_colourseq = %d ---", m_pParent->m_colourseq);
	//ErrorMessage(m_pParent->m_parent, buf);

	/////////////////////////////////////////////////////////////
	// normal pots...
	if(!bWhiteInPocket)
	{			
		//MessageBox(0, "11","11",MB_OK);
		for(n=0;n<MAX_TOTALBALLS;n++)
		{
			if(TestFlag(m_pParent->m_ball[n]->m_flags, Flg_BALL_POTTED))
			{					
				m_pParent->m_bMiss = FALSE;		// potted ball
				m_pParent->m_foul = 0;			// no foul

				if(m_pParent->m_bTurn1P) {
					// 1P just potted a ball
					// after first pot, go into runout mode
					if( !(m_pParent->m_break2P & 0x01) )
						m_pParent->m_break2P |= 0x01;					
				}
				else
				{
					// 2P just potted a ball
					// after first pot, go into runout mode
					if( !(m_pParent->m_break2P & 0x02) )
						m_pParent->m_break2P |= 0x02;
				}
				

				
				int tbl = GAMEAPPVIEW2->m_lobby->GetTableIdxFromPlayer(GAMEAPPVIEW2->m_playerSockIdx);
				if(tbl!=-1)
				{
					m_pParent->m_shotTime =  m_table[tbl].shotTime*50;
				}
									  
				/*
				if(_potted8ball >0)
				{
					// GAME_OVER! the other player losses!
					if(m_pParent->m_bTurn1P)
						m_pParent->m_gameOver = 2;
					else
						m_pParent->m_gameOver = 1;		
				}
				*/
			}
		}

		// if player failed to pot their own group ball?
		if(!bPottedOwnGroupBall)
			m_pParent->m_bMiss = TRUE;

		/*
		if(!m_colourseq) // if we are NOT on the 'colour seq.' at the end of the frame
		if(m_bMiss == FALSE)
		{
			if(m_rednext)
				m_rednext = FALSE;
			else
				m_rednext = TRUE;
		}
		*/
	}
	/////////////////////////////////////////////////////////////

end:;

	



	if(TestFlag(m_pParent->m_foul, Flg_FOUL_GAMEOVER))
	{
		// GAME_OVER! this player losses!
		if(m_pParent->m_bTurn1P)
			m_pParent->m_gameOver = 1;
		else
			m_pParent->m_gameOver = 2;	
		
		// Set this back to default values if it's Game Over!! //
		m_pParent->m_lifes = 0;
		m_pParent->m_bBallInHand = 0;
		//

	}

	// ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
	// how many balls have been potted in a row?
	if(!TestFlag(m_pParent->m_foul, Flg_FOUL_NORMAL | Flg_FOUL_GAMEOVER))
	{
		if(bPottedOwnGroupBall)
		{
			if(m_pParent->m_bTurn1P)
			{
				m_pParent->m_poolPots1P += (_pottedspots + _pottedstripes + _potted8ball);

				if(m_pParent->m_highBreaks1P[1] < m_pParent->m_poolPots1P)
					m_pParent->m_highBreaks1P[1] = m_pParent->m_poolPots1P;
			}
			else
			{
				m_pParent->m_poolPots2P += (_pottedspots + _pottedstripes + _potted8ball);

				if(m_pParent->m_highBreaks2P[1] < m_pParent->m_poolPots2P)
					m_pParent->m_highBreaks2P[1] = m_pParent->m_poolPots2P;
			}
		}
		
		if(!bPottedOwnGroupBall || _potted8ball > 0)
		{
			// didn't pot anything this time or gameover
			m_pParent->m_poolPots1P = 0;
			m_pParent->m_poolPots2P = 0;
		}
	}
	else
	{
		// if foul! then clear pot runs
		m_pParent->m_poolPots1P = 0;
		m_pParent->m_poolPots2P = 0;
	}

//	{
//	char buf[256];
//	sprintf(buf, "m_poolPots1P = %d (%d), m_poolPots2P = %d (%d)", m_pParent->m_poolPots1P, m_pParent->m_highBreaks1P[1], m_pParent->m_poolPots2P, m_pParent->m_highBreaks2P[1]);
//	ErrorMessage(g_wnd, buf);
//	}
	// ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

	/////////////////////////////////////////////////////////////
	// check if nothing was potted this time
//	if(!m_pParent->m_gameOver) // NOT SURE WHY WE NEEDED THIS LINE?
	if(m_pParent->m_bMiss)
	{

		// if we've already potted a ball, but now we've missed or fouled, then NO run-out!
		//if(m_pParent->m_break2P == 1)
		//	m_pParent->m_break2P = -1;
		
		
		if(m_pParent->m_bTurn1P) {
			// 1P just missed/fouled a ball
			// if already potted a ball, but now missed!
			if( (m_pParent->m_break2P & 0x01) )
				m_pParent->m_break2P |= 0x04;
		}
		else
		{
			// 2P just missed/fouled a ball			
			// if already potted a ball, but now missed!
			if( (m_pParent->m_break2P & 0x02) )
				m_pParent->m_break2P |= 0x08;
		}


		//goto end;

		//MessageBox(0, "13","13",MB_OK);
		if(m_pParent->m_bPracticeMode && !GAMEAPPVIEW2->m_bReplayMode && !GAMEAPPVIEW2->m_bPlayOffline)
		{

		}
		else
		{
			// decide who's turn it is next?
			bNextPlayer = TRUE;
			m_pParent->m_bConcededGame = FALSE; // clear this flag (now that the turn has swapped over, we can allow gameOver to continue...)
			GAMEAPPVIEW2->m_bFlashWindow = TRUE;

			if(TestFlag(m_pParent->m_foul, Flg_FOUL_NORMAL))
				m_pParent->m_shotTime = 60*50;
			else
			{
				// if no foul, then give usual time for shot
				int tbl = GAMEAPPVIEW2->m_lobby->GetTableIdxFromPlayer(GAMEAPPVIEW2->m_playerSockIdx);
				if(tbl!=-1)
				{
					m_pParent->m_shotTime =  m_table[tbl].shotTime*50;
				}
			}
			

			//ErrorMessage(m_parent, "1. reg break!");

			// Only Change Turn if  m_pParent->m_lifes == 0;
			// Next Player at the Table is allowed 2 Shots, unless he/she fouls //


			// If a Foul, then give other player 2 Shots and Change Turn //
			if(TestFlag(m_pParent->m_foul, Flg_FOUL_NORMAL)) 
				{
				if(m_pParent->m_bTurn1P) m_pParent->m_bTurn1P = FALSE;
					else m_pParent->m_bTurn1P = TRUE;
				
				if(m_pParent->m_colourseq > 0) 
					{
					m_pParent->m_lifes = 1;
					//m_pParent->m_bBallInHand = 1;
					sprintf(g_gameAppView->m_sponsortxt, "First Visit (2 Shots)");
					g_gameAppView->m_sponsoralpha = 0.5f;
					}
				}
			// Otherwise, Change Turn only if m_lifes is 0 //
			else{
				if(m_pParent->m_lifes==0)
					{
					if(m_pParent->m_bTurn1P) m_pParent->m_bTurn1P = FALSE;
						else m_pParent->m_bTurn1P = TRUE;
					
					g_gameAppView->m_sponsortxt[0] = 0;
					g_gameAppView->m_sponsoralpha = 0;
					}
				else{
					m_pParent->m_lifes--;
					sprintf(g_gameAppView->m_sponsortxt, "Second Visit");
					g_gameAppView->m_sponsoralpha = 0.5f;
					}
				}
			
			
		}

		// if we are NOT on the 'colour seq.' at the end of the frame
		//if(!m_colourseq)
		//	m_rednext = TRUE;
	}


				
	/////////////////////////////////////////////////////////////
	//SetFlag(m_pParent->m_foul, Flg_FOUL_NORMAL);

	// if just done break! and we fouled!
	if(m_pParent->m_colourseq == 0)
		m_pParent->m_colourseq = 1;


	//AfxMessageBox("!");
	End_BadBreak:;

	RefereeUKPoolUpdatePots();

	// re-spot balls
	RefereeUKPoolReSpot();

	


		///////////
		// for practice mode : fixes "pot yellow aand white simul. bug" ***
		if(m_pParent->m_bMiss)
		{
			//MessageBox(0, "13b","13b",MB_OK);
			if(m_pParent->m_bPracticeMode && !GAMEAPPVIEW2->m_bPlayOffline)
			{
				//ErrorMessage(g_wnd, "FOUL!");
				// force no fouls in practice
				//m_pParent->m_foul = 0;
				//m_pParent->m_break1P = 0;
			}
		}
		///////////

		
		
		if(TestFlag(m_pParent->m_foul, Flg_FOUL_NORMAL))
		{
			//m_pParent->m_bBallInHand = 1;
			m_pParent->m_bStandUpFromShot = 1;
		}



}


///////////////////////////////////////////////////////////////////////////////////////////////////
//
//
void CCustomTable::RefereeUKPoolReSpot(void)
{
	int n;
	int m;
	int nextspot;
	int dir;
	BOOL successful;
	BOOL reds;

	float f1,f2,f3;

	
	////////////////////////////////////////////////////////////////////////////////////////////////////////////
	
	//ErrorMessage(m_pParent->m_parent, "Respot:(1,2,3)");

	// for balls in pockects
	for(n=MAX_TOTALBALLS-1; n >= 0;n--)
	//if((m_ball[n]->m_colour <= BLACK && m_ball[n]->m_colour >= YELLOW) || (m_ball[n]->m_colour == WHITE))
	if((m_pParent->m_ball[n]->m_colour == WHITE)) // ***** TODO ***** -> 8 BALL
	{	
						
		// temp. code for WHITE ball
		if(m_pParent->m_ball[n]->m_colour == WHITE)
			//if(!TestFlag(m_pParent->m_ball[n]->m_flags, Flg_BALL_ACTIVE))
			if(TestFlag(m_pParent->m_foul, Flg_FOUL_WHITEPOTTED))
				{
					// position ball on its original starting position or...
					m_pParent->m_ball[n]->m_POld = m_pParent->m_ball[n]->m_P = m_pParent->m_ball[n]->m_PSpot;
					
					//ErrorMessage(m_pParent->m_parent, "whiteball-1");

					if( RefereeUKPoolDropBallOnTable(WHITE, m_pParent->m_ball[n]->m_P.x, m_pParent->m_ball[n]->m_P.y ) )
					{
						//ErrorMessage(m_pParent->m_parent, "whiteball-2");
					}
					else					
					{
						//ErrorMessage(m_pParent->m_parent, "whiteball-3");
						// ...otherwise position anywhere behind the 'Head String'
						CVector Pgreen, Pyellow;
						CVector P;
						float radius;
						//Pgreen = CVector(-WORLD_X(912),-WORLD_Y(395-12), BALL_POSITIONZOFFSET);
						//Pyellow = CVector(-WORLD_X(912),-WORLD_Y(199+12), BALL_POSITIONZOFFSET);
						Pgreen = CVector(-WORLD_X(591+65+120/*912+5*/), -600, BALL_POSITIONZOFFSET);
						Pyellow = CVector(-WORLD_X(591+65+120/*912+5*/), 600, BALL_POSITIONZOFFSET);
						float w,h;
						float x,y;
						
						//[f1 = Pyellow.y - Pgreen.y;]
						f1 = Pyellow.y;
						f1 = f1 - Pgreen.y;
						
						f1 = f1 * 0.9f;
						h =	f1;
						//h =	(Pyellow.y - Pgreen.y)*0.9f;
						w = h*0.5f;
						for(x = 0.0f;x<w;x+=BALL_RADIUS)
						for(y = 0.0f;y<h;y+=BALL_RADIUS)
						{
							
							//[f1 = Pyellow.x - x;]
							f1 = Pyellow.x;
							f1 = f1 - x;
							
							//[f2 = Pyellow.y - Pgreen.y;]
							f2 = Pyellow.y;
							f2 = f2 - Pgreen.y;
							
							f2 = f2 * 0.05f;
							
							//[f3 = Pyellow.y - y;]
							f3 = Pyellow.y;
							f3 = f3 - y;
							
							f3 = f3 - f2;
							if( RefereeUKPoolDropBallOnTable(WHITE, f1, f3) )
							//if( DropBallOnTable(WHITE, Pyellow.x - x, Pyellow.y - y - (Pyellow.y - Pgreen.y)*0.05f) )
								goto whiteSpotted;
						}
						whiteSpotted:;
					}
					
//					m_ball[n]->m_POld = m_ball[n]->m_P = m_ball[n]->m_PSpot;
					m_pParent->m_ball[n]->m_W = m_pParent->m_ball[n]->m_V = m_pParent->m_ball[n]->m_VOld = CVector(0,0,0);
	
					// turn ball on
					m_pParent->m_ball[n]->m_flags = 0;
					SetFlag(m_pParent->m_ball[n]->m_flags, Flg_BALL_ACTIVE);

					//m_pParent->m_bBallInHand = 1;
					m_pParent->m_bStandUpFromShot = 1;


					continue;
				}
		
		#if 0
		///&&&&&&&&&&
		
		/////////////////////////////////////////////////////////////////////
		// (1) try and spot on starting position
		/////////////////////////////////////////////////////////////////////
		successful = TRUE;
		for(m=0;m<MAX_TOTALBALLS;m++)
		if(n!=m)
		{
			if(!m_ball[n]->TryReSpot1(m_ball[m]))
				successful = FALSE;
		}
		/////////////////////////////////////////////////////////////////////

		if(successful)
			m_ball[n]->ReSpotSuccessful();
		else
		{
			/////////////////////////////////////////////////////////////////////
			// (2) try next vacant spot, starting with highest colour spot first
			/////////////////////////////////////////////////////////////////////
			for(nextspot=MAX_TOTALBALLS-1; nextspot > 0;nextspot--)
				if(m_ball[nextspot]->m_colour <= BLACK && m_ball[nextspot]->m_colour >= YELLOW)
				{
					successful = TRUE;
					for(m=0;m<MAX_TOTALBALLS;m++)
					if(n!=m)
						{
						if(!m_ball[n]->TryReSpot2(m_ball[m], m_ball[nextspot]))
							successful = FALSE;
						}
					
					if(successful) {
						m_ball[n]->ReSpotSuccessful();
						goto end;
					}
				
				}
			/////////////////////////////////////////////////////////////////////

			/////////////////////////////////////////////////////////////////////
			// (3) try and spot behind starting position in a direct line
			/////////////////////////////////////////////////////////////////////					
			// check to left side of spot (dir = -1)..., then the right side (dir = 1)...
			if(!TestFlag(m_ball[n]->m_flags, Flg_BALL_ACTIVE))
			for(dir = -1; dir <= 1; dir+=2)
			{
				// position ball on its original starting position
				m_ball[n]->m_POld = m_ball[n]->m_P = m_ball[n]->m_PSpot;
				m_ball[n]->m_W = m_ball[n]->m_V = m_ball[n]->m_VOld = CVector(0,0,0);

				do
				{
					f1 = BALL_RADIUS/20.0f;
					f1  = f1 * dir;
					m_ball[n]->m_P.x -= f1;
					//m_ball[n]->m_P.x -= (BALL_RADIUS/20)*dir;

					successful = TRUE;
					for(m=0;m<MAX_TOTALBALLS;m++)
					if(n!=m)
					{
						if(!m_ball[n]->TryReSpot3(m_ball[m]))
							successful = FALSE;
					}

					if(successful) {
						m_ball[n]->ReSpotSuccessful();
						goto end;
					}

				}
				while((m_ball[n]->m_P.x >= (-1736+BALL_RADIUS)) ||
					  (m_ball[n]->m_P.x <= (1736-BALL_RADIUS)));   // bottom cushion + BALL_RADIUS || top cushion - BALL_RADIUS
				// *TODO: note: the above boundary check is incorrect!!!, it should be using the actual 3d world "Cushions"
			}
				/////////////////////////////////////////////////////////////////////

				// we are in trouble if it gets this far!!!

		end:;
		}		
		///&&&&&&&&&&
		#endif //0
	}
}

BOOL CCustomTable::RefereeUKPoolDropBallOnTable(int n, float x, float y, BOOL bDrop)
{										 
	// check if ball 'n' is on top of another ball
	int m;
	for(m=0;m<MAX_TOTALBALLS;m++)
	if(m!=n)
	if(m_pParent->m_ball[m]->Proximity(x, y, 0, BALL_RADIUS*2.0f) == TRUE)
	{
		//ErrorMessage(m_pParent->m_parent, "FALSE");
		return FALSE;
	}
	
	// check for ball n inside the D
	//if(!m_pParent->m_bPracticeMode || m_type == CTTYPE_BOWLS || GAMEAPPVIEW2->m_bPlayOffline) // should be !
	{
		CVector B = CVector(x,y,BALL_POSITIONZOFFSET);
		CVector Pgreen, Pbrown;
		CVector P;
		float radius;
		//Pgreen = CVector(-WORLD_X(912),-WORLD_Y(395-12), BALL_POSITIONZOFFSET);
		//Pbrown = CVector(-WORLD_X(912),-WORLD_Y(297),BALL_POSITIONZOFFSET);
		Pgreen = CVector(-WORLD_X(591+65+120/*912+5*/), -600-(28.4625f - g_ballRadius), BALL_POSITIONZOFFSET);
		Pbrown = CVector(-WORLD_X(591+65+120/*912+5*/)/*-(28.4625f - g_ballRadius)*/, 0, BALL_POSITIONZOFFSET);

		//[P = Pgreen - Pbrown;]
		P = Pgreen;
		P = P - Pbrown;

		radius = P.Magnitude2();
		
		//[P = B - Pbrown;]
		P = B;
		P = P - Pbrown;

		int xoffset = 0, yoffset = 0;

		if(m_pParent->m_M3d_Table_ptr == &m_M3d_ukTable)
			{
			// Set Different Bounding Box Regions for UK Pool Game Type //
			xoffset = -110;
			yoffset = 85;
			}
	

		if(TestFlag(m_pParent->m_foul, Flg_FOUL_WHITEPOTTED) || TestFlag(m_pParent->m_foul, Flg_FOUL_NORMAL)
			|| TestFlag(m_pParent->m_foul, Flg_FOUL_BALLINHAND))
			{
			if(m_pParent->m_M3d_Table_ptr == &m_M3d_ukTable) Pbrown.x = 1235+xoffset;	// give ball-in-hand access to whole table
			else Pbrown.x = 1235;	// give ball-in-hand access to whole table	
			}


		//if( P.Magnitude2() < radius)
		if((B.y < -(Pgreen.y+yoffset)) && (B.y > (Pgreen.y+yoffset)))
		{			
			if(  (B.x < Pbrown.x) && (B.x > (-(1235+xoffset) - (28.4625f - g_ballRadius)*1.4f) )  )
			{
				// update the drop ball's coordinates
				if(bDrop)
				{
					m_pParent->m_ball[n]->m_P.x = x;
					m_pParent->m_ball[n]->m_P.y = y;
					#ifdef FIXEDMODE
					m_pParent->m_ball[n]->m_V.x = 0;
					m_pParent->m_ball[n]->m_V.y = 0;
					m_pParent->m_ball[n]->m_V.z = 0;
					m_pParent->m_ball[n]->UpdateFixeds();
					#endif

					m_pParent->m_ball[n]->AdjustOrientation();
					m_pParent->m_ball[n]->m_POld = m_pParent->m_ball[n]->m_P;
					#ifdef FIXEDMODE
					m_pParent->m_ball[n]->UpdateFixeds();
					#endif
				}
				return TRUE;
			}
		}
		return FALSE;
	}
	
	// update the drop ball's coordinates
	if(bDrop)
	{
		m_pParent->m_ball[n]->m_P.x = x;
		m_pParent->m_ball[n]->m_P.y = y;
		#ifdef FIXEDMODE
		m_pParent->m_ball[n]->m_V.x = 0;
		m_pParent->m_ball[n]->m_V.y = 0;
		m_pParent->m_ball[n]->m_V.z = 0;
		m_pParent->m_ball[n]->UpdateFixeds();
		#endif
		m_pParent->m_ball[n]->AdjustOrientation();
		m_pParent->m_ball[n]->m_POld = m_pParent->m_ball[n]->m_P;
		#ifdef FIXEDMODE
		m_pParent->m_ball[n]->UpdateFixeds();
		#endif
		//ErrorMessage(m_pParent->m_parent, "TRUE");

	}
	return TRUE;
}


void CCustomTable::RefereeUKPoolUpdatePots(void)
{
	int ballColour;
	float fx,fy;
	int x,y;
		
	int n;
	for(n = 0; n < 16; n++)
	{
		g_pottedSpots[n] = -1;
		g_pottedStripes[n] = -1;
	}

	// scan thru all potted balls, and see who's potted what
	int m;
	for(m=0;m<MAX_TOTALBALLS;m++)
	if(!TestFlag(m_pParent->m_ball[m]->m_flags, Flg_BALL_ACTIVE))
	{
		fx = m_pParent->m_ball[m]->m_P.x;
		fy = m_pParent->m_ball[m]->m_P.y;

		
		// what has P1 potted
		if(fx > 20001)
		{
			fx-=20000;
			fx /= 100;
			if(fx>=0 && fx<= 15)
			{
				x = fx;
				y = fy;
				if((m_pParent->m_ball[m]->m_colour >= 1) &&  (m_pParent->m_ball[m]->m_colour <= 7))
					g_pottedSpots[x] = m;
				else if((m_pParent->m_ball[m]->m_colour >= 9) &&  (m_pParent->m_ball[m]->m_colour <= 15))
					g_pottedStripes[x] = m;					
				
			}
		}

		// what has P2 potted
		if(fy > 20001)
		{
			fy-=20000;
			fy /= 100;
			if(fy>=0 && fy<= 15)
			{
				x = fx;
				y = fy;
				if((m_pParent->m_ball[m]->m_colour >= 1) &&  (m_pParent->m_ball[m]->m_colour <= 7))
					g_pottedSpots[y] = m;
				else if((m_pParent->m_ball[m]->m_colour >= 9) &&  (m_pParent->m_ball[m]->m_colour <= 15))
					g_pottedStripes[y] = m;					
			}
		}
	}
	/*			
	fy = 0;
	ballColour = 0;

	// draw spots on left-hand side		
	for(m=0;m<16;m++)
	if(g_pottedSpots[m] != -1)
	{			
		ballColour = m_pParent->m_ball[g_pottedSpots[m]]->m_colour; 
		
		// *TODO: 
		//if(m_pParent->m_colourseq == 2) // 1P=SPOTS & 2P=STRIPES
		//	draw2DBall(ballColour, x = LEFT, y = fy);
		//else if(m_pParent->m_colourseq == 3) // 1P=STRIPES & 2P=SPOTS
		//	draw2DBall(ballColour, x = RIGHT, y = fy);

		fy += 32;
	}

	char bbb[80];
	sprintf(bbb, "spots potted = %f", fy / 32); 
	ErrorMessage(m_pParent->m_parent, bbb);

	// draw stripes on left-hand side		
	fy = 0;
	ballColour = 0;
	for(m=0;m<16;m++)
	if(g_pottedStripes[m] != -1)
	{			
		ballColour = m_pParent->m_ball[g_pottedStripes[m]]->m_colour; 
		
		// *TODO: 
		//if(m_pParent->m_colourseq == 2) // 1P=SPOTS & 2P=STRIPES
		//	draw2DBall(ballColour, x = RIGHT, y = fy);
		//else if(m_pParent->m_colourseq == 3) // 1P=STRIPES & 2P=SPOTS
		//	draw2DBall(ballColour, x = LEFT, y = fy);

		fy += 32;
	}		
	
	sprintf(bbb, "stripes potted = %f", fy / 32); 
	ErrorMessage(m_pParent->m_parent, bbb);
	*/
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////



void CCustomTable::Referee9Ball(DWORD msg, void *pData)
{
	if(msg == CTMSG_LINESET)
	{
		Referee9BallLINESET(msg, pData);
	}
	else if(msg == CTMSG_POCKETSET)
	{
		Referee9BallPOCKETSET(msg, pData);
	}
	else if(msg == CTMSG_SIMEND)
	{
		Decode();
		Referee9BallSIMEND();
		Encode();
	}
	else
	{
		RefereeSnooker(msg, pData);
	}
}

void CCustomTable::Referee9BallLINESET(DWORD msg, void *pData)
{
	ctmsg_LINESET_DATA *ctmsgData;
	ctmsgData = (ctmsg_LINESET_DATA_typ *) pData;

	// -----------------------------------------------------------------------------
	// check if player has not selected the white ball & we're not in Practice mode
	// -----------------------------------------------------------------------------
	//if(!m_pParent->m_rednext)
	  //if(m_pParent->m_colourseq < YELLOW)
		//if(m_pParent->m_testBall->m_colour == WHITE)
			//if(m_pParent->m_nominateBall == WHITE)
		if(!m_pParent->m_bPracticeMode)			
			if(m_pParent->m_selectBall != WHITE)
			{
				///		
				/// [simulate R-Button down for cancel shot]
				///

				m_pParent->m_bTestBallFinished = FALSE;
				//m_snooker->m_bCueTipReady = FALSE;

				// reset foucus to table (this) window, & reset 'raise butt'
				m_pParent->m_RaiseButt = m_pParent->m_RaiseButtOld = 0;
				m_pParent->m_bReceiveRaiseButtActive = FALSE;
				m_pParent->m_bRaiseButtHelper = TRUE;
				GAMEAPPVIEW2->m_sliderRaiseButt.SetPos(90);
				GAMEAPPVIEW2->PreviousWndEditSetFocus();

				GAMEAPPVIEW2->m_point = ctmsgData->point;
					
				// Only give this window the mouse's full attention 
				GAMEAPPVIEW2->SetCapture();
				
				if(m_pParent->m_tableMouseOn)
				{
					if(m_pParent->m_bCueState)
					{
						// Now Give ,mouse's attention to all
						if(GAMEAPPVIEW2->GetCapture() == GAMEAPPVIEW2) ReleaseCapture();

						// give mouse full movement
						ClipCursor(&GAMEAPPVIEW2->m_rectCursor);

						// reset mouse cursor position back to start position
						CRect rect;
						GAMEAPPVIEW2->GetWindowRect(&rect);
						SetCursorPos(rect.left + m_pParent->m_sCPoint.x+2, rect.top + m_pParent->m_sCPoint.y+2);
					
						m_pParent->SelectBall(0, ctmsgData->point);
						
						m_pParent->m_point = ctmsgData->point;
						m_pParent->m_sightlineFade = 0;
						
						if(m_pParent->m_bPracticeMode);
						else
							m_pParent->SelectBallSend(0, ctmsgData->point, m_pParent->m_windowWidth, m_pParent->m_windowHeight, m_pParent->m_selectBall);
					}
				}

				if(!m_pParent->m_cueBallSelect)
				if(!GAMEAPPVIEW2->m_bShowCursor)
				{
					GAMEAPPVIEW2->m_bShowCursor = TRUE;
					SetCursor(GAMEAPPVIEW2->m_hCursor);
					ShowCursor(GAMEAPPVIEW2->m_bShowCursor); // show cursor
				}

				///
				///
				///

				GAMEAPPVIEW2->AlertMessage("Warning! You can only strike the 'White' ball!\n\n(Use 'Left-Mouse-Button' to select the 'White' ball)");
				return;
			}

	// ---------------------------------------------
	// normal 'LineSet' here!
	// ---------------------------------------------

	// Now Give ,mouse's attention to all
	if(GAMEAPPVIEW2->GetCapture() == GAMEAPPVIEW2) ReleaseCapture();
			
	// test!!! send final cue-line pos
	GAMEAPPVIEW2->SendPlayerMouseMove(m_pParent->m_mouseX, m_pParent->m_mouseY, 
							m_pParent->m_windowWidth, m_pParent->m_windowHeight,
							m_pParent->m_strikeLine, m_pParent->m_strikeDist);

	m_pParent->m_cueBallSelect = 5; //--
	m_pParent->m_mouseX = 0; // position cuetip GFX pointer at centre of selected ball
	m_pParent->m_mouseY = 0;

	GAMEAPPVIEW2->m_sliderRaiseButt.EnableWindow(TRUE);
	m_pParent->m_bRaiseButtHelper = TRUE;

	// give mouse full movement
	ClipCursor(&GAMEAPPVIEW2->m_rectCursor);

	if(!GAMEAPPVIEW2->m_bShowCursor)
	{
		GAMEAPPVIEW2->m_bShowCursor = TRUE;
		SetCursor(GAMEAPPVIEW2->m_hCursor);
		ShowCursor(GAMEAPPVIEW2->m_bShowCursor); // show cursor
	}

	// reset mouse cursor position back to start position
	CRect rect;
	GAMEAPPVIEW2->GetWindowRect(&rect);
	SetCursorPos(rect.left + m_pParent->m_sCPoint.x+2, rect.top + m_pParent->m_sCPoint.y+2);

	m_pParent->m_bCueLineSet = TRUE;
	
	m_pParent->m_tableMousePosStrike[0] = m_pParent->m_tableMousePos[0];
	m_pParent->m_tableMousePosStrike[1] = m_pParent->m_tableMousePos[1];
	m_pParent->m_tableMousePosStrike[2] = m_pParent->m_tableMousePos[2];
}


void CCustomTable::Referee9BallPOCKETSET(DWORD msg, void *pData)
{
	ctmsg_POCKETSET_DATA *ctmsgData;
	ctmsgData = (ctmsg_POCKETSET_DATA_typ *) pData;
	
	int n, m;

	if(m_pParent->m_cueBallSelect != 5) return;	// only in state '5'(after line is set!), are we allowed to do the following

	if(m_pParent->m_colourseq == 0) return; // if this is a break-off shot, dont do the following

		
	if(m_pParent->m_bCallShot) // default
	{
		n = m_pParent->GetSelectedPocket();
		m = m_pParent->GetSelectedBall();
		// MUST BE IN FIXMODE!
/*		
		{
			char buf[256];
			sprintf(buf, "n = %d, m = %d", n, m);
			ErrorMessage(m_pParent->m_parent, buf);
		}
*/		

		if(n != -1 || m != -1)	// if player has clicked inside a pocket OR has clicked on a ball
		{
			if(n != -1) m_pParent->SetTargetPocket(n);			
			if(m != -1) m_pParent->SetTargetBall(m);

			// send target ball/pocket/success data to all at table
			GAMEAPPVIEW2->SendPlayerTargetData(m_pParent->m_targetPocket, m_pParent->m_targetBall, m_pParent->m_bTargetSuccess);
		}
	}
}


void CCustomTable::Referee9BallRESPOTBALL(int n)
{
	int m;
	int dir;
	BOOL successful;
	float f1;

	/////////////////////////////////////////////////////////////////////
	// (3) try and spot behind starting position in a direct line
	/////////////////////////////////////////////////////////////////////					
	// check to left side of spot (dir = -1)..., then the right side (dir = 1)...
	if(!TestFlag(m_pParent->m_ball[n]->m_flags, Flg_BALL_ACTIVE))
	for(dir = -1; dir <= 1; dir+=2)
	{
		//If Break, then ReSport Black, Blue, Brown in their correct positions //
		CVector F = CVector(WORLD_X(591+65+120), 0, m_pParent->m_ball[n]->m_PSpot.z);
		if(m_type == CTTYPE_BREAK)
			{
			if(n==11) F = CVector(WORLD_X(331+10-2+65),WORLD_Y(297), m_pParent->m_ball[n]->m_PSpot.z); // Brown
			else if(n==12) F = CVector(WORLD_X(591),WORLD_Y(297), m_pParent->m_ball[n]->m_PSpot.z); // Blue 
			else if(n==13) F = CVector(WORLD_X(776+100),WORLD_Y(297), m_pParent->m_ball[n]->m_PSpot.z); // Black
			}
		
		
		// place ball on foot spot
		m_pParent->m_ball[n]->m_POld = m_pParent->m_ball[n]->m_P = F;//m_ball[n]->m_PSpot;
		m_pParent->m_ball[n]->m_W = m_pParent->m_ball[n]->m_V = m_pParent->m_ball[n]->m_VOld = CVector(0,0,0);

		do
		{
			f1 = BALL_RADIUS/20.0f;
			f1  = f1 * (float)dir;
			m_pParent->m_ball[n]->m_P.x -= f1;
			//m_ball[n]->m_P.x -= (BALL_RADIUS/20)*dir;

			successful = TRUE;
			for(m=0;m<MAX_TOTALBALLS;m++)
			if(n!=m)
			if(TestFlag(m_pParent->m_ball[m]->m_flags, Flg_BALL_ACTIVE)) // NEW: make sure this ball is active
			{
				if(!m_pParent->m_ball[n]->TryReSpot3(m_pParent->m_ball[m]))
					successful = FALSE;
			}

			// is this ball on top of a pocket
			float x = m_pParent->m_ball[n]->m_P.x;
			float y = m_pParent->m_ball[n]->m_P.y;
			for(int p=0;p<m_pocket_n;p++)
			{
				if( PocketProximity(p, CVector(x, y, 0), BALL_RADIUS) )
				{						
					successful = FALSE;
					break;
				}
			} // end for p

			if(successful) {
				m_pParent->m_ball[n]->ReSpotSuccessful();
				goto end;
			}

		}
		while(!m_pParent->m_ball[n]->CushionsProximity(FALSE)); // NEW
		//while((m_ball[n]->m_P.x >= (-1736+BALL_RADIUS)) ||
		//	  (m_ball[n]->m_P.x <= (1736-BALL_RADIUS)));   // bottom cushion + BALL_RADIUS || top cushion - BALL_RADIUS
		// *TODO: note: the above boundary check is incorrect!!!, it should be using the actual 3d world "Cushions"
	}
	/////////////////////////////////////////////////////////////////////

end:;

	#ifdef FIXEDMODE
	m_pParent->m_ball[n]->UpdateFixeds();
	#endif
}


void CCustomTable::Referee9BallSIMEND(void)
{
	int n;
	int colour;
	int no_colours;
	BOOL bWhiteInPocket = FALSE;
	BOOL bNextPlayer = FALSE;
	BOOL bCalledBallWasPotted = FALSE;

	int _spots = 0;
	//int _stripes = 0;
	int _9ball = 0;
	int _pottedspots = 0;
	//int _pottedstripes = 0;
	int _potted9ball = 0;

	int colourHit1st = 0; // points to white

	BOOL bPushOutShot = FALSE;

	m_pParent->m_bMiss = TRUE;												// assume no balls potted
	ClearFlag(m_pParent->m_foul, Flg_FOUL_NORMAL | Flg_FOUL_WHITEPOTTED | Flg_FOUL_GAMEOVER | Flg_FOUL_BALLINHAND);	// assume no foul to start with [keep some flags from previous shot]
	ClearFlag(m_pParent->m_foul, Flg_FOUL_ALERT);
	ClearFlag(m_pParent->m_foul, Flg_FOUL_NOTHITCUSHION | Flg_FOUL_NOTHITCUSHIONBREAK);
	m_pParent->m_bRefreshScores = TRUE;
	
	if(!m_pParent->m_bCallShot)
		bCalledBallWasPotted = TRUE; // just for safety, in case I forget to use -> "if(m_pParent->m_bCallShot)"

	// handle push-out stuff (2:2)
	if(TestFlag(m_pParent->m_foul, Flg_FOUL_ALERTPUSHOUT))
	{
		if(m_pParent->m_colourseq > 0)
		{
			if(TestFlag(m_pParent->m_foul, Flg_FOUL_ALERTPUSHOUT))
				ClearFlag(m_pParent->m_foul, Flg_FOUL_ALERTPUSHOUT);
			
			if(TestFlag(m_pParent->m_foul, Flg_FOUL_ALERTPUSHOUT2))
			{
				SetFlag(m_pParent->m_foul, Flg_FOUL_ALERT);
				bPushOutShot = TRUE;
			}
		}
	}
	else
		ClearFlag(m_pParent->m_foul, Flg_FOUL_ALERTPUSHOUT2);


	// just in case :)
	//if(m_colourseq)
	//	m_rednext = FALSE;

	// [freeball mode]
	//if(TestFlag(m_foul, Flg_FOUL_FREEBALL))
	//{
	//	//MessageBox(0, "0","0",MB_OK);
//		FreeBallReferee();
//		ClearFlag(m_foul, Flg_FOUL_FREEBALL);
//		goto end;
//	}

	// [default mode]

	/////////////////////////////////////////////////////////////
	// what is left?
	

	for(n=0;n<MAX_TOTALBALLS;n++)
	{
		if(TestFlag(m_pParent->m_ball[n]->m_flags, Flg_BALL_ACTIVE))
		{
			if(m_pParent->m_ball[n]->m_colour >= 1 && m_pParent->m_ball[n]->m_colour <= 8)
			{
				_spots++;
			}
			/*
			else if(m_pParent->m_ball[n]->m_colour >= 9 && m_pParent->m_ball[n]->m_colour <= 15)
			{
				_stripes++;
			}
			*/
			else if(m_pParent->m_ball[n]->m_colour == 9)
			{
				_9ball++;
			}
		}
	}

	//char buf3[80];
	//sprintf(buf3, "spots = %d; 9-ball = %d", _spots, _9ball);
	//ErrorMessage(m_pParent->m_parent, buf3);

	//DEBUG!
	//char buf3[80];
	//sprintf(buf3, "PRE: runOut = %d", m_pParent->m_break2P);
	//ErrorMessage(m_pParent->m_parent, buf3);

	/////////////////////////////////////////////////////////////
	// what was pottted?
											

	for(n=0;n<MAX_TOTALBALLS;n++)
	{
		if(TestFlag(m_pParent->m_ball[n]->m_flags, Flg_BALL_POTTED))
		{
			// was target/called ball potted?
			if(m_pParent->m_bCallShot)
			if(m_pParent->m_bTargetSuccess)
			if(m_pParent->m_targetBall == m_pParent->m_ball[n]->m_colour)
				bCalledBallWasPotted = TRUE;

			if(m_pParent->m_ball[n]->m_colour >= 1 && m_pParent->m_ball[n]->m_colour <= 8)
			{
				_pottedspots++;
			}
			/*
			else if(m_pParent->m_ball[n]->m_colour >= 9 && m_pParent->m_ball[n]->m_colour <= 15)
			{
				_pottedstripes++;
			}
			*/
			else if(m_pParent->m_ball[n]->m_colour == 9)
			{
				_potted9ball++;
			}
		}
	}

	/////////////////////////////////////////////////////////////
	// see if white ball has been potted (foul)
	for(n=0;n<MAX_TOTALBALLS;n++)
	{
		if(TestFlag(m_pParent->m_ball[n]->m_flags, Flg_BALL_POTTED))
			if(m_pParent->m_ball[n]->m_colour == WHITE)
			{
				bWhiteInPocket = TRUE;
				if(m_pParent->m_colourseq == 0) SetFlag(m_pParent->m_foul, Flg_FOUL_NORMAL | Flg_FOUL_WHITEPOTTED | Flg_FOUL_BREAK);
					else SetFlag(m_pParent->m_foul, Flg_FOUL_NORMAL | Flg_FOUL_WHITEPOTTED);

				if(_potted9ball != 0)
				{
					// *TODO - FOUL!!!! DO RERACK!!!!
					//SetFlag(m_pParent->m_foul, Flg_FOUL_GAMEOVER);
					
					// respot 9-ball
					for(n=0;n<MAX_TOTALBALLS;n++)
					if(TestFlag(m_pParent->m_ball[n]->m_flags, Flg_BALL_POTTED))
					if(m_pParent->m_ball[n]->m_colour == 9)
					{
						Referee9BallRESPOTBALL(n);
						break;
					}					
				}
			}
	}
	/////////////////////////////////////////////////////////////

	/////////////////////////////////////////////////////////////
	// what happens if player strikes a ball other than the white
	if(m_pParent->m_selectBall != WHITE)
	{
		SetFlag(m_pParent->m_foul, Flg_FOUL_NORMAL);

		if(_potted9ball != 0)
		{
			// *TODO - FOUL!!!! DO RERACK!!!!
			//SetFlag(m_pParent->m_foul, Flg_FOUL_GAMEOVER);
			
			// respot 9-ball
			for(n=0;n<MAX_TOTALBALLS;n++)
			if(TestFlag(m_pParent->m_ball[n]->m_flags, Flg_BALL_POTTED))
			if(m_pParent->m_ball[n]->m_colour == 9)
			{
				Referee9BallRESPOTBALL(n);
				break;
			}
		}

//		goto end;
	}
	/////////////////////////////////////////////////////////////
	
	/////////////////////////////////////////////////////////////
	// if white doesn't hit a ball at all, foul ball on!
	//if(m_colourseq < YELLOW) // bypass if on colours at the end
	if(!bPushOutShot)
	if((m_pParent->m_selectBall == WHITE) && (m_pParent->m_dominoHit1st == WHITE))
	{
			SetFlag(m_pParent->m_foul, Flg_FOUL_NORMAL);

			if(_potted9ball != 0)
			{
				// *TODO - FOUL!!!! DO RERACK!!!!
				//SetFlag(m_pParent->m_foul, Flg_FOUL_GAMEOVER);

				// respot 9-ball
				for(n=0;n<MAX_TOTALBALLS;n++)
				if(TestFlag(m_pParent->m_ball[n]->m_flags, Flg_BALL_POTTED))
				if(m_pParent->m_ball[n]->m_colour == 9)
				{
					Referee9BallRESPOTBALL(n);
					break;
				}				
			}

//			goto end;
	}
	/////////////////////////////////////////////////////////////

	///
	// [NEW] if nothing was potted, did cueball or spotball touch a cushion?
	if(m_pParent->m_colourseq > 0)
	if(!bWhiteInPocket)
	if(!bPushOutShot)
	if(_pottedspots == 0 && _potted9ball == 0)
	{
		BOOL bFailedToHitCushion = TRUE;
		
		for(n=0;n<MAX_TOTALBALLS;n++)
		{	
			if(m_pParent->m_ball[n]->m_hitCushion)
			{
				bFailedToHitCushion = FALSE; // no foul!
				break;
			}
		}

		if(bFailedToHitCushion)
		{
			SetFlag(m_pParent->m_foul, Flg_FOUL_NORMAL | Flg_FOUL_NOTHITCUSHION);
			//ErrorMessage(m_pParent->m_parent, "111");
		}
	}
	///////////////////////////////////////////////////////////////////////////
	///




	colourHit1st = m_pParent->m_ball[m_pParent->m_dominoHit1st]->m_colour;




				if(m_pParent->m_colourseq == 0)
				{					
					///
					// [NEW] if nothing was potted on the break, did >= 4 numbered balls touch a cushion?					
					if(!bWhiteInPocket)
					if(!bPushOutShot)
					if(_pottedspots == 0 && _potted9ball == 0)
					{						
						int hitCushion = 0;
						
						for(n=1;n<MAX_TOTALBALLS;n++) // exclude white ball
						{	
							if(m_pParent->m_ball[n]->m_hitCushion)
							{
								hitCushion++; // (at least 4 numbered balls)
							}
						}
						
						if(hitCushion < 4)
						{
							SetFlag(m_pParent->m_foul, Flg_FOUL_NORMAL | Flg_FOUL_NOTHITCUSHIONBREAK);
						}

						//char buf[80];
						//sprintf(buf, "hitCushion = %d", hitCushion);
						//ErrorMessage(m_pParent->m_parent, buf);
					}
					///////////////////////////////////////////////////////////////////////////
					///
					
					// on the break!
					m_pParent->m_colourseq = 1;
					
					if(_potted9ball != 0)
					{
						if( (colourHit1st == 1) && !TestFlag(m_pParent->m_foul, Flg_FOUL_NORMAL) ) // 1-ball
						{
							// Golden break!!!!
							if(m_pParent->m_bTurn1P)
							{
								m_pParent->m_gameOver = 2;
								Msfx_smpplayvol(&GAMEAPPVIEW2->m_game_smp[SFX_GOODSHOT], 255);
							}
							else
							{
								m_pParent->m_gameOver = 1;
								Msfx_smpplayvol(&GAMEAPPVIEW2->m_game_smp[SFX_GOODSHOT], 255);
							}
						}
						else
						{
							// this player losses for now!
							//SetFlag(m_pParent->m_foul, Flg_FOUL_NORMAL | Flg_FOUL_GAMEOVER); //--
							
							SetFlag(m_pParent->m_foul, Flg_FOUL_NORMAL);

							// respot 9-ball
							for(n=0;n<MAX_TOTALBALLS;n++)
							if(TestFlag(m_pParent->m_ball[n]->m_flags, Flg_BALL_POTTED))
							if(m_pParent->m_ball[n]->m_colour == 9)
							{
								Referee9BallRESPOTBALL(n);
								break;
							}
							//goto end; //--
						}						
					}
					
					// what is the lowest value ball left on the table?
					int lowestball = 9;
					for(n=0;n<MAX_TOTALBALLS;n++)
					if(TestFlag(m_pParent->m_ball[n]->m_flags, Flg_BALL_ACTIVE))
					if(!TestFlag(m_pParent->m_ball[n]->m_flags, Flg_BALL_POTTED))
					{
						if(m_pParent->m_ball[n]->m_colour >= 1 && m_pParent->m_ball[n]->m_colour <= 9)
							if(m_pParent->m_ball[n]->m_colour < lowestball)
								lowestball = m_pParent->m_ball[n]->m_colour;			
					}

					// point to next ball to pot
					m_pParent->m_colourseq = lowestball;

					if(colourHit1st != 1) // if 1-ball wasn't hit first on the break
					{
						SetFlag(m_pParent->m_foul, Flg_FOUL_NORMAL);
						goto end;
					}

					if(TestFlag(m_pParent->m_foul, Flg_FOUL_NORMAL))
					{
						goto end;
					}

					// allow Push-Out now ( handle push-out stuff (1:2) )
					SetFlag(m_pParent->m_foul, Flg_FOUL_ALERT | Flg_FOUL_ALERTPUSHOUT);
				}
				else if(m_pParent->m_colourseq > 0)
				{
					// shots after the break
					BOOL bHitWrongBall = FALSE;
					if(colourHit1st != m_pParent->m_colourseq) // if ball-on wasn't hit first
						bHitWrongBall = TRUE;
					
					if(_potted9ball != 0)
					{
						if(bPushOutShot)
						{
							//// this player losses for now!
							//SetFlag(m_pParent->m_foul, Flg_FOUL_NORMAL | Flg_FOUL_GAMEOVER);
							//goto end;

							// respot 9-ball
							for(n=0;n<MAX_TOTALBALLS;n++)
							if(TestFlag(m_pParent->m_ball[n]->m_flags, Flg_BALL_POTTED))
							if(m_pParent->m_ball[n]->m_colour == 9)
							{
								Referee9BallRESPOTBALL(n);
								break;
							}

							goto _skipPotted9Ball;
						}

						// if this is a called shot type game
						if(m_pParent->m_bCallShot && !bCalledBallWasPotted)
						{
							SetFlag(m_pParent->m_foul, Flg_FOUL_NORMAL);

							// respot 9-ball
							for(n=0;n<MAX_TOTALBALLS;n++)
							if(TestFlag(m_pParent->m_ball[n]->m_flags, Flg_BALL_POTTED))
							if(m_pParent->m_ball[n]->m_colour == 9)
							{
								Referee9BallRESPOTBALL(n);
								break;
							}							
						}
						else
						{
							// default : old way
							if(!bHitWrongBall && !TestFlag(m_pParent->m_foul, Flg_FOUL_NORMAL) ) // ball-on (i.e. lowest ball that was on)
								//if( (colourHit1st == 1) &&  ) // 1-ball
							{							
								// Combination on the 9!!!! / Or finally potted the 9! (Check this OAP)
								if(m_pParent->m_bTurn1P)
								{
									m_pParent->m_gameOver = 2;
									
									if(_spots == 0) // if there are no spots left &...
									if(m_pParent->m_break2P == 1) // if we're on a run-out
									{
										m_pParent->m_highBreaks1P[0] = 60; // 60 => run-out for 9-ball stat.
										Msfx_smpplayvol(&GAMEAPPVIEW2->m_game_smp[SFX_GOODSHOT], 255);
									}
								}
								else
								{
									m_pParent->m_gameOver = 1;

									if(_spots == 0) // if there are no spots left &...
									if(m_pParent->m_break2P == 1) // if we're on a run-out
									{
										m_pParent->m_highBreaks2P[0] = 60; // 60 => run-out for 9-ball stat.
										Msfx_smpplayvol(&GAMEAPPVIEW2->m_game_smp[SFX_GOODSHOT], 255);
									}
								}
							}
							else
							{
								// this player losses for now!
								//SetFlag(m_pParent->m_foul, Flg_FOUL_NORMAL | Flg_FOUL_GAMEOVER); //--

								SetFlag(m_pParent->m_foul, Flg_FOUL_NORMAL);

								// respot 9-ball
								for(n=0;n<MAX_TOTALBALLS;n++)
								if(TestFlag(m_pParent->m_ball[n]->m_flags, Flg_BALL_POTTED))
								if(m_pParent->m_ball[n]->m_colour == 9)
								{
									Referee9BallRESPOTBALL(n);
									break;
								}
														
								//goto end; //--
							}
						}
					}
				_skipPotted9Ball:;
					// what is the lowest value ball left on the table?
					int lowestball = 9;
					for(n=0;n<MAX_TOTALBALLS;n++)
					if(TestFlag(m_pParent->m_ball[n]->m_flags, Flg_BALL_ACTIVE))
					if(!TestFlag(m_pParent->m_ball[n]->m_flags, Flg_BALL_POTTED))
					{
						if(m_pParent->m_ball[n]->m_colour >= 1 && m_pParent->m_ball[n]->m_colour <= 9)
							if(m_pParent->m_ball[n]->m_colour < lowestball)
								lowestball = m_pParent->m_ball[n]->m_colour;
						
// wrong place!!!! (**)
//						if(!bHitWrongBall && !TestFlag(m_pParent->m_foul, Flg_FOUL_NORMAL) )
//						if(m_pParent->m_bCallShot && !bCalledBallWasPotted)
//							SetFlag(m_pParent->m_foul, Flg_FOUL_NORMAL);
						
					}

					// point to next ball to pot
					m_pParent->m_colourseq = lowestball;

					if(!bPushOutShot)
					if(bHitWrongBall) // if ball-on wasn't hit first
					{
						ClearFlag(m_pParent->m_foul, Flg_FOUL_NOTHITCUSHION); // if this person hit the wrong ball, dont bother to tell them abt the cushion rule
						SetFlag(m_pParent->m_foul, Flg_FOUL_NORMAL);
						goto end;
					}

					// did player pot the wrong ball, instead of the called one (**)
					for(n=0;n<MAX_TOTALBALLS;n++)
					if(TestFlag(m_pParent->m_ball[n]->m_flags, Flg_BALL_POTTED))
					if(m_pParent->m_ball[n]->m_colour >= 1 && m_pParent->m_ball[n]->m_colour <= 15)
					{
						if(!bHitWrongBall && !TestFlag(m_pParent->m_foul, Flg_FOUL_NORMAL) )
						if(m_pParent->m_bCallShot && !bCalledBallWasPotted)
						{	
							SetFlag(m_pParent->m_foul, Flg_FOUL_NORMAL);
							break;
						}
					}
					/////////////////////////////////////////////////////////////////

					if(TestFlag(m_pParent->m_foul, Flg_FOUL_NORMAL))
						goto end;
				}
				

					//if(m_pParent->m_bTurn1P)

	//			if(m_pParent->m_ball[n]->m_colour == WHITE)
	///			{
	//				bWhiteInPocket = TRUE;
	//				SetFlag(m_pParent->m_foul, Flg_FOUL_NORMAL | Flg_FOUL_WHITEPOTTED);
	//			}
	//		}
	//	}
	  
	/////////////////////////////////////////////////////////////
	//char buf[80];
	//sprintf(buf, "-- m_colourseq = %d ---", m_pParent->m_colourseq);
	//ErrorMessage(m_pParent->m_parent, buf);

	/////////////////////////////////////////////////////////////
	// normal pots...
	if(!bPushOutShot)
	if(!bWhiteInPocket)
	{			
		//MessageBox(0, "11","11",MB_OK);
		for(n=0;n<MAX_TOTALBALLS;n++)
		{
			if(TestFlag(m_pParent->m_ball[n]->m_flags, Flg_BALL_POTTED))
			{					
				

				if(m_pParent->m_bTurn1P) {
					//m_pParent->m_score1P += 1;//m_pParent->m_ball[n]->m_colour;
					//m_pParent->m_break1P += 1;//m_ball[n]->m_colour;
				}
				else {
					//m_pParent->m_score2P += 1;//m_pParent->m_ball[n]->m_colour;
					//m_pParent->m_break2P += 1;//m_ball[n]->m_colour;
				}
				
				m_pParent->m_bMiss = FALSE;		// potted ball

				// after first pot, go into runout mode
				if(m_pParent->m_break2P == 0)
					m_pParent->m_break2P = 1;
				
				//m_pParent->m_foul = 0;				// no foul
				ClearFlag(m_pParent->m_foul, (~(Flg_FOUL_ALERT | Flg_FOUL_ALERTPUSHOUT | Flg_FOUL_ALERTPUSHOUT2)) );

				
				int tbl = GAMEAPPVIEW2->m_lobby->GetTableIdxFromPlayer(GAMEAPPVIEW2->m_playerSockIdx);
				if(tbl!=-1)
				{
					m_pParent->m_shotTime =  m_table[tbl].shotTime*50;
				}

				/*
				if(_potted8ball >0)
				{
					// GAME_OVER! the other player losses!
					if(m_pParent->m_bTurn1P)
						m_pParent->m_gameOver = 2;
					else
						m_pParent->m_gameOver = 1;		
				}
				*/
			}
		}

		/*
		if(!m_colourseq) // if we are NOT on the 'colour seq.' at the end of the frame
		if(m_bMiss == FALSE)
		{
			if(m_rednext)
				m_rednext = FALSE;
			else
				m_rednext = TRUE;
		}
		*/
	}
	/////////////////////////////////////////////////////////////
	
	/////////////////////////////////////
	// *NEW: this shot was a good one, so clear the foul3times counter
	if(!TestFlag(m_pParent->m_foul, Flg_FOUL_NORMAL))
	{
		if(m_pParent->m_bTurn1P)
			m_pParent->m_foul3Times1P = 0;
		else
			m_pParent->m_foul3Times2P = 0;
	}
	/////////////////////////////////////

end:;

	/////////////////////////////////////
	// *NEW: this shot was a bad one
	if(TestFlag(m_pParent->m_foul, Flg_FOUL_NORMAL))
	{
		if(m_pParent->m_bTurn1P)
			m_pParent->m_foul3Times1P++;
		else
			m_pParent->m_foul3Times2P++;
	}

	// 3 times => gameover!
	if(m_pParent->m_foul3Times1P >= 3)
	{
		m_pParent->m_gameOver = 1;
	}
	else if(m_pParent->m_foul3Times2P >= 3)
	{
		m_pParent->m_gameOver = 2;
	}
	/////////////////////////////////////

	if(TestFlag(m_pParent->m_foul, Flg_FOUL_NORMAL))
	{
		ClearFlag(m_pParent->m_foul, Flg_FOUL_ALERT);
	}

	//char buf[80];
	//sprintf(buf, "-- m_colourseq = %d ---", m_pParent->m_colourseq);
	//ErrorMessage(m_pParent->m_parent, buf);


	// if just done break! and we fouled!
	if(m_pParent->m_colourseq == 0)
		m_pParent->m_colourseq = 1;

	if(TestFlag(m_pParent->m_foul, Flg_FOUL_GAMEOVER))
	{
		// GAME_OVER! this player losses!
		if(m_pParent->m_bTurn1P)
			m_pParent->m_gameOver = 1;
		else
			m_pParent->m_gameOver = 2;		
	}

	// ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
	// how many balls have been potted in a row?
	if(!TestFlag(m_pParent->m_foul, Flg_FOUL_NORMAL | Flg_FOUL_GAMEOVER))
	{
		if(_pottedspots > 0 || _potted9ball > 0)
		{
			if(m_pParent->m_bTurn1P)
			{
				m_pParent->m_poolPots1P += (_pottedspots + _potted9ball);
				
				if(m_pParent->m_highBreaks1P[1] < m_pParent->m_poolPots1P)
					m_pParent->m_highBreaks1P[1] = m_pParent->m_poolPots1P;
			}
			else
			{
				m_pParent->m_poolPots2P += (_pottedspots + _potted9ball);

				if(m_pParent->m_highBreaks2P[1] < m_pParent->m_poolPots2P)
					m_pParent->m_highBreaks2P[1] = m_pParent->m_poolPots2P;
			}
		}
		
		if( (_pottedspots == 0 && _potted9ball == 0) || _potted9ball > 0)
		{
			// didn't pot anything this time or gameover
			m_pParent->m_poolPots1P = 0;
			m_pParent->m_poolPots2P = 0;
		}
	}
	else
	{
		// if foul! then clear pot runs
		m_pParent->m_poolPots1P = 0;
		m_pParent->m_poolPots2P = 0;
	}

//	{
//	char buf[256];
//	sprintf(buf, "m_poolPots1P = %d (%d), m_poolPots2P = %d (%d)", m_pParent->m_poolPots1P, m_pParent->m_highBreaks1P[1], m_pParent->m_poolPots2P, m_pParent->m_highBreaks2P[1]);
//	ErrorMessage(g_wnd, buf);
//	}

	// ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

	/////////////////////////////////////////////////////////////
	// check if nothing was potted this time
	//if(!m_pParent->m_gameOver) // NOT SURE WHY WE NEEDED THIS LINE?

	if(m_pParent->m_bMiss)
	{
		// if we've already potted a ball, but now we've missed or fouled, then NO run-out!
		if(m_pParent->m_break2P == 1)
			m_pParent->m_break2P = -1;

		//ErrorMessage(m_pParent->m_parent, "miss!");
//		// end of break sounds
//		if(g_gfxDetails.bSoundClapping)
//		{
//			if(m_bTurn1P)
//			{
//				if(m_break1P >= 20 && m_break1P < 100)
//				{
//					float loudness = 128 + 128*(((float)m_break1P - 20)/80);
//					Msfx_smpplayvol(&GAMEAPPVIEW->m_game_smp[SFX_20PLUSBREAK], loudness);
//				}
//				
//				/*
//				if(m_break1P >= 100)
//				{
//					Msfx_smpplayvol(&GAMEAPPVIEW->m_game_smp[SFX_CENTURY], 255);
//				}
//				*/
//				
//			}
//			else
//			{
//				if(m_break2P >= 20 && m_break2P < 100)
//				{
//					float loudness = 128 + 128*(((float)m_break1P - 20)/80);
//					Msfx_smpplayvol(&GAMEAPPVIEW->m_game_smp[SFX_20PLUSBREAK], loudness);
//				}
//  
//				/*
//				if(m_break2P >= 100)
//				{
//					Msfx_smpplayvol(&GAMEAPPVIEW->m_game_smp[SFX_CENTURY], 255);
//				}
//				*/
//			}
//		}

		//MessageBox(0, "13","13",MB_OK);
		if(m_pParent->m_bPracticeMode && !GAMEAPPVIEW2->m_bReplayMode && !GAMEAPPVIEW2->m_bPlayOffline)
		{
			// force no fouls in practice
			//m_foul = 0;
			//m_break1P = 0;
		}
		else
		{
			// decide who's turn it is next?
			bNextPlayer = TRUE;
			m_pParent->m_bConcededGame = FALSE; // clear this flag (now that the turn has swapped over, we can allow gameOver to continue...)
			GAMEAPPVIEW2->m_bFlashWindow = TRUE;

			//m_pParent->m_shotTime = 60*50;
			if(TestFlag(m_pParent->m_foul, Flg_FOUL_NORMAL))
				m_pParent->m_shotTime = 60*50;
			else
			{
				// if no foul, then give usual time for shot
				int tbl = GAMEAPPVIEW2->m_lobby->GetTableIdxFromPlayer(GAMEAPPVIEW2->m_playerSockIdx);
				if(tbl!=-1)
				{
					m_pParent->m_shotTime =  m_table[tbl].shotTime*50;
				}
			}

			//ErrorMessage(m_parent, "1. reg break!");

			if(m_pParent->m_bTurn1P) {
				m_pParent->m_bTurn1P = FALSE;
				//m_pParent->m_break2P = 0;

				//m_pParent->m_break2P = 60*50;

				/*
				// is it a high break?
				//if(m_break1P >= 50)
				{
					if(m_highBreaks1P[0] <= m_highBreaks1P[1])
					{
						if(m_break1P > m_highBreaks1P[0])
							m_highBreaks1P[0] = m_break1P;
					}
					else
					{
						if(m_break1P > m_highBreaks1P[1])
							m_highBreaks1P[1] = m_break1P;
					}
				}
				*/
			}
			else {
				m_pParent->m_bTurn1P = TRUE;
				//m_pParent->m_break1P = 0;

				//m_pParent->m_break2P = 60*50;

				/*
				// is it a high break?
				//if(m_break2P >= 50)
				{
					if(m_highBreaks2P[0] <= m_highBreaks2P[1])
					{
						if(m_break2P > m_highBreaks2P[0])
							m_highBreaks2P[0] = m_break2P;
					}
					else
					{
						if(m_break2P > m_highBreaks2P[1])
							m_highBreaks2P[1] = m_break2P;
					}
				}
				*/
			}
		}

		// if we are NOT on the 'colour seq.' at the end of the frame
		//if(!m_colourseq)
		//	m_rednext = TRUE;
	}
	/////////////////////////////////////////////////////////////	

	Referee9BallUpdatePots();

	// re-spot balls
	Referee9BallReSpot();

	//char buf33[80];
	//sprintf(buf33, "POST: runOut = %d", m_pParent->m_break2P);
	//ErrorMessage(m_pParent->m_parent, buf33);

		
		///////////
		// for practice mode : fixes "pot yellow aand white simul. bug" ***
		if(m_pParent->m_bMiss)
		{
			//MessageBox(0, "13b","13b",MB_OK);
			if(m_pParent->m_bPracticeMode && !GAMEAPPVIEW2->m_bPlayOffline)
			{
				// force no fouls in practice
				m_pParent->m_foul = 0;
				//m_pParent->m_break1P = 0;
			}
		}
		///////////

		
		if(TestFlag(m_pParent->m_foul, Flg_FOUL_NORMAL))
		{
			m_pParent->m_bBallInHand = 1;
			m_pParent->m_bStandUpFromShot = 1;
		}
}


///////////////////////////////////////////////////////////////////////////////////////////////////
//
//
void CCustomTable::Referee9BallReSpot(void)
{
	int n;
	int m;
	int nextspot;
	int dir;
	BOOL successful;
	BOOL reds;

	float f1,f2,f3;

	
	////////////////////////////////////////////////////////////////////////////////////////////////////////////
	
	//ErrorMessage(m_pParent->m_parent, "Respot:(1,2,3)");

	// for balls in pockects
	for(n=MAX_TOTALBALLS-1; n >= 0;n--)
	//if((m_ball[n]->m_colour <= BLACK && m_ball[n]->m_colour >= YELLOW) || (m_ball[n]->m_colour == WHITE))
	if((m_pParent->m_ball[n]->m_colour == WHITE)) // ***** TODO ***** -> 8 BALL
	{	
		// temp. code for WHITE ball
		if(m_pParent->m_ball[n]->m_colour == WHITE)
			//if(!TestFlag(m_pParent->m_ball[n]->m_flags, Flg_BALL_ACTIVE))
			if(TestFlag(m_pParent->m_foul, Flg_FOUL_WHITEPOTTED))
				{
					// position ball on its original starting position or...
					m_pParent->m_ball[n]->m_POld = m_pParent->m_ball[n]->m_P = m_pParent->m_ball[n]->m_PSpot;
					
					//ErrorMessage(m_pParent->m_parent, "whiteball-1");

					if( Referee9BallDropBallOnTable(WHITE, m_pParent->m_ball[n]->m_P.x, m_pParent->m_ball[n]->m_P.y ) )
					{
						//ErrorMessage(m_pParent->m_parent, "whiteball-2");
					}
					else					
					{
						//ErrorMessage(m_pParent->m_parent, "whiteball-3");
						// ...otherwise position anywhere behind the 'Head String'
						CVector Pgreen, Pyellow;
						CVector P;
						float radius;
						//Pgreen = CVector(-WORLD_X(912),-WORLD_Y(395-12), BALL_POSITIONZOFFSET);
						//Pyellow = CVector(-WORLD_X(912),-WORLD_Y(199+12), BALL_POSITIONZOFFSET);
						Pgreen = CVector(-WORLD_X(591+65+120/*912+5*/), -600, BALL_POSITIONZOFFSET);
						Pyellow = CVector(-WORLD_X(591+65+120/*912+5*/), 600, BALL_POSITIONZOFFSET);
						float w,h;
						float x,y;
						
						//[f1 = Pyellow.y - Pgreen.y;]
						f1 = Pyellow.y;
						f1 = f1 - Pgreen.y;
						
						f1 = f1 * 0.9f;
						h =	f1;
						//h =	(Pyellow.y - Pgreen.y)*0.9f;
						w = h*0.5f;
						for(x = 0.0f;x<w;x+=BALL_RADIUS)
						for(y = 0.0f;y<h;y+=BALL_RADIUS)
						{
							
							//[f1 = Pyellow.x - x;]
							f1 = Pyellow.x;
							f1 = f1 - x;
							
							//[f2 = Pyellow.y - Pgreen.y;]
							f2 = Pyellow.y;
							f2 = f2 - Pgreen.y;
							
							f2 = f2 * 0.05f;
							
							//[f3 = Pyellow.y - y;]
							f3 = Pyellow.y;
							f3 = f3 - y;
							
							f3 = f3 - f2;
							if( Referee9BallDropBallOnTable(WHITE, f1, f3) )
							//if( DropBallOnTable(WHITE, Pyellow.x - x, Pyellow.y - y - (Pyellow.y - Pgreen.y)*0.05f) )
								goto whiteSpotted;
						}
						whiteSpotted:;
					}
					
//					m_ball[n]->m_POld = m_ball[n]->m_P = m_ball[n]->m_PSpot;
					m_pParent->m_ball[n]->m_W = m_pParent->m_ball[n]->m_V = m_pParent->m_ball[n]->m_VOld = CVector(0,0,0);
	
					// turn ball on
					m_pParent->m_ball[n]->m_flags = 0;
					SetFlag(m_pParent->m_ball[n]->m_flags, Flg_BALL_ACTIVE);

					m_pParent->m_bBallInHand = 1;
					m_pParent->m_bStandUpFromShot = 1;


					continue;
				}
		
		#if 0
		///&&&&&&&&&&
		
		/////////////////////////////////////////////////////////////////////
		// (1) try and spot on starting position
		/////////////////////////////////////////////////////////////////////
		successful = TRUE;
		for(m=0;m<MAX_TOTALBALLS;m++)
		if(n!=m)
		{
			if(!m_ball[n]->TryReSpot1(m_ball[m]))
				successful = FALSE;
		}
		/////////////////////////////////////////////////////////////////////

		if(successful)
			m_ball[n]->ReSpotSuccessful();
		else
		{
			/////////////////////////////////////////////////////////////////////
			// (2) try next vacant spot, starting with highest colour spot first
			/////////////////////////////////////////////////////////////////////
			for(nextspot=MAX_TOTALBALLS-1; nextspot > 0;nextspot--)
				if(m_ball[nextspot]->m_colour <= BLACK && m_ball[nextspot]->m_colour >= YELLOW)
				{
					successful = TRUE;
					for(m=0;m<MAX_TOTALBALLS;m++)
					if(n!=m)
						{
						if(!m_ball[n]->TryReSpot2(m_ball[m], m_ball[nextspot]))
							successful = FALSE;
						}
					
					if(successful) {
						m_ball[n]->ReSpotSuccessful();
						goto end;
					}
				
				}
			/////////////////////////////////////////////////////////////////////

			/////////////////////////////////////////////////////////////////////
			// (3) try and spot behind starting position in a direct line
			/////////////////////////////////////////////////////////////////////					
			// check to left side of spot (dir = -1)..., then the right side (dir = 1)...
			if(!TestFlag(m_ball[n]->m_flags, Flg_BALL_ACTIVE))
			for(dir = -1; dir <= 1; dir+=2)
			{
				// position ball on its original starting position
				m_ball[n]->m_POld = m_ball[n]->m_P = m_ball[n]->m_PSpot;
				m_ball[n]->m_W = m_ball[n]->m_V = m_ball[n]->m_VOld = CVector(0,0,0);

				do
				{
					f1 = BALL_RADIUS/20.0f;
					f1  = f1 * dir;
					m_ball[n]->m_P.x -= f1;
					//m_ball[n]->m_P.x -= (BALL_RADIUS/20)*dir;

					successful = TRUE;
					for(m=0;m<MAX_TOTALBALLS;m++)
					if(n!=m)
					{
						if(!m_ball[n]->TryReSpot3(m_ball[m]))
							successful = FALSE;
					}

					if(successful) {
						m_ball[n]->ReSpotSuccessful();
						goto end;
					}

				}
				while((m_ball[n]->m_P.x >= (-1736+BALL_RADIUS)) ||
					  (m_ball[n]->m_P.x <= (1736-BALL_RADIUS)));   // bottom cushion + BALL_RADIUS || top cushion - BALL_RADIUS
				// *TODO: note: the above boundary check is incorrect!!!, it should be using the actual 3d world "Cushions"
			}
				/////////////////////////////////////////////////////////////////////

				// we are in trouble if it gets this far!!!

		end:;
		}		
		///&&&&&&&&&&
		#endif //0
	}
}

BOOL CCustomTable::Referee9BallDropBallOnTable(int n, float x, float y, BOOL bDrop)
{
	// check if ball 'n' is on top of another ball
	int m;
	for(m=0;m<MAX_TOTALBALLS;m++)
	if(m!=n)
	if(m_pParent->m_ball[m]->Proximity(x, y, 0, BALL_RADIUS*2.0f) == TRUE)
	{
		//ErrorMessage(m_pParent->m_parent, "FALSE");
		return FALSE;
	}
	
	// check for ball n inside the D
	if(!m_pParent->m_bPracticeMode) // should be !
	{
		CVector B = CVector(x,y,BALL_POSITIONZOFFSET);
		CVector Pgreen, Pbrown;
		CVector P;
		float radius;
		//Pgreen = CVector(-WORLD_X(912),-WORLD_Y(395-12), BALL_POSITIONZOFFSET);
		//Pbrown = CVector(-WORLD_X(912),-WORLD_Y(297),BALL_POSITIONZOFFSET);
		Pgreen = CVector(-WORLD_X(591+65+120/*912+5*/), -600, BALL_POSITIONZOFFSET);
		Pbrown = CVector(-WORLD_X(591+65+120/*912+5*/), 0, BALL_POSITIONZOFFSET);

		//[P = Pgreen - Pbrown;]
		P = Pgreen;
		P = P - Pbrown;

		radius = P.Magnitude2();
		
		//[P = B - Pbrown;]
		P = B;
		P = P - Pbrown;

		if(TestFlag(m_pParent->m_foul, Flg_FOUL_WHITEPOTTED) || TestFlag(m_pParent->m_foul, Flg_FOUL_NORMAL)
			|| TestFlag(m_pParent->m_foul, Flg_FOUL_BALLINHAND))
			Pbrown.x = 1235;	// give ball-in-hand access to whole table

		//if( P.Magnitude2() < radius)
		if((B.y < -Pgreen.y) && (B.y > Pgreen.y))
		{			
			if((B.x < Pbrown.x) && (B.x > -1235))
			{
				// update the drop ball's coordinates
				if(bDrop)
				{
					//m_pParent->m_ball[n]->m_P.x = x;
					//m_pParent->m_ball[n]->m_P.y = y;
					//m_pParent->m_ball[n]->AdjustOrientation();
					//m_pParent->m_ball[n]->m_POld = m_pParent->m_ball[n]->m_P;
				m_pParent->m_ball[n]->m_P.x = x;
				m_pParent->m_ball[n]->m_P.y = y;
				#ifdef FIXEDMODE
				m_pParent->m_ball[n]->m_V.x = 0;
				m_pParent->m_ball[n]->m_V.y = 0;
				m_pParent->m_ball[n]->m_V.z = 0;
				m_pParent->m_ball[n]->UpdateFixeds();
				#endif
				m_pParent->m_ball[n]->AdjustOrientation();
				m_pParent->m_ball[n]->m_POld = m_pParent->m_ball[n]->m_P;
				#ifdef FIXEDMODE
				m_pParent->m_ball[n]->UpdateFixeds();
				#endif

				}
				return TRUE;
			}
		}
		return FALSE;
	}
	
	// update the drop ball's coordinates
	if(bDrop)
	{
		//m_pParent->m_ball[n]->m_P.x = x;
		//m_pParent->m_ball[n]->m_P.y = y;
		//m_pParent->m_ball[n]->AdjustOrientation();
		//m_pParent->m_ball[n]->m_POld = m_pParent->m_ball[n]->m_P;
		//ErrorMessage(m_pParent->m_parent, "TRUE");

		m_pParent->m_ball[n]->m_P.x = x;
		m_pParent->m_ball[n]->m_P.y = y;
		#ifdef FIXEDMODE
		m_pParent->m_ball[n]->m_V.x = 0;
		m_pParent->m_ball[n]->m_V.y = 0;
		m_pParent->m_ball[n]->m_V.z = 0;
		m_pParent->m_ball[n]->UpdateFixeds();
		#endif
		m_pParent->m_ball[n]->AdjustOrientation();
		m_pParent->m_ball[n]->m_POld = m_pParent->m_ball[n]->m_P;
		#ifdef FIXEDMODE
		m_pParent->m_ball[n]->UpdateFixeds();
		#endif

	}
	return TRUE;
}


void CCustomTable::Referee9BallUpdatePots(void)
{
	int ballColour;
	float fx,fy;
	int x,y;
		
	int n;
	for(n = 0; n < 16; n++)
	{
		g_pottedSpots[n] = -1;
		g_pottedStripes[n] = -1;
	}

	// scan thru all potted balls, and see who's potted what
	int m;
	for(m=0;m<MAX_TOTALBALLS;m++)
	if(!TestFlag(m_pParent->m_ball[m]->m_flags, Flg_BALL_ACTIVE))
	{
		fx = m_pParent->m_ball[m]->m_P.x;
		fy = m_pParent->m_ball[m]->m_P.y;

		
		// what has P1 potted
		if(fx > 20001)
		{
			fx-=20000;
			fx /= 100;
			if(fx>=0 && fx<= 15)
			{
				x = fx;
				y = fy;
				if((m_pParent->m_ball[m]->m_colour >= 1) &&  (m_pParent->m_ball[m]->m_colour <= 8))
					g_pottedSpots[x] = m;
				/*
				else if((m_pParent->m_ball[m]->m_colour >= 9) &&  (m_pParent->m_ball[m]->m_colour <= 15))
					g_pottedStripes[x] = m;
				*/				
			}
		}

		// what has P2 potted
		if(fy > 20001)
		{
			fy-=20000;
			fy /= 100;
			if(fy>=0 && fy<= 15)
			{
				x = fx;
				y = fy;
				if((m_pParent->m_ball[m]->m_colour >= 1) &&  (m_pParent->m_ball[m]->m_colour <= 8))
					g_pottedSpots[y] = m;
				/*
				else if((m_pParent->m_ball[m]->m_colour >= 9) &&  (m_pParent->m_ball[m]->m_colour <= 15))
					g_pottedStripes[y] = m;
				*/
			}
		}
	}
	/*			
	fy = 0;
	ballColour = 0;

	// draw spots on left-hand side		
	for(m=0;m<16;m++)
	if(g_pottedSpots[m] != -1)
	{			
		ballColour = m_pParent->m_ball[g_pottedSpots[m]]->m_colour; 
		
		// *TODO: 
		//if(m_pParent->m_colourseq == 2) // 1P=SPOTS & 2P=STRIPES
		//	draw2DBall(ballColour, x = LEFT, y = fy);
		//else if(m_pParent->m_colourseq == 3) // 1P=STRIPES & 2P=SPOTS
		//	draw2DBall(ballColour, x = RIGHT, y = fy);

		fy += 32;
	}

	char bbb[80];
	sprintf(bbb, "spots potted = %f", fy / 32); 
	ErrorMessage(m_pParent->m_parent, bbb);

	// draw stripes on left-hand side		
	fy = 0;
	ballColour = 0;
	for(m=0;m<16;m++)
	if(g_pottedStripes[m] != -1)
	{			
		ballColour = m_pParent->m_ball[g_pottedStripes[m]]->m_colour; 
		
		// *TODO: 
		//if(m_pParent->m_colourseq == 2) // 1P=SPOTS & 2P=STRIPES
		//	draw2DBall(ballColour, x = RIGHT, y = fy);
		//else if(m_pParent->m_colourseq == 3) // 1P=STRIPES & 2P=SPOTS
		//	draw2DBall(ballColour, x = LEFT, y = fy);

		fy += 32;
	}		
	
	sprintf(bbb, "stripes potted = %f", fy / 32); 
	ErrorMessage(m_pParent->m_parent, bbb);
	*/
}

//**

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CCustomTable::Referee6Ball(DWORD msg, void *pData)
{
	if(msg == CTMSG_LINESET)
	{
		Referee6BallLINESET(msg, pData);
	}
	else if(msg == CTMSG_POCKETSET)
	{
		Referee6BallPOCKETSET(msg, pData);
	}
	else if(msg == CTMSG_SIMEND)
	{
		Decode();
		Referee6BallSIMEND();
		Encode();
	}
	else
	{
		RefereeSnooker(msg, pData);
	}
}

void CCustomTable::Referee6BallLINESET(DWORD msg, void *pData)
{
	ctmsg_LINESET_DATA *ctmsgData;
	ctmsgData = (ctmsg_LINESET_DATA_typ *) pData;

	// -----------------------------------------------------------------------------
	// check if player has not selected the white ball & we're not in Practice mode
	// -----------------------------------------------------------------------------
	//if(!m_pParent->m_rednext)
	  //if(m_pParent->m_colourseq < YELLOW)
		//if(m_pParent->m_testBall->m_colour == WHITE)
			//if(m_pParent->m_nominateBall == WHITE)
		if(!m_pParent->m_bPracticeMode)			
			if(m_pParent->m_selectBall != WHITE)
			{
				///		
				/// [simulate R-Button down for cancel shot]
				///

				m_pParent->m_bTestBallFinished = FALSE;
				//m_snooker->m_bCueTipReady = FALSE;

				// reset foucus to table (this) window, & reset 'raise butt'
				m_pParent->m_RaiseButt = m_pParent->m_RaiseButtOld = 0;
				m_pParent->m_bReceiveRaiseButtActive = FALSE;
				m_pParent->m_bRaiseButtHelper = TRUE;
				GAMEAPPVIEW2->m_sliderRaiseButt.SetPos(90);
				GAMEAPPVIEW2->PreviousWndEditSetFocus();

				GAMEAPPVIEW2->m_point = ctmsgData->point;
					
				// Only give this window the mouse's full attention 
				GAMEAPPVIEW2->SetCapture();
				
				if(m_pParent->m_tableMouseOn)
				{
					if(m_pParent->m_bCueState)
					{
						// Now Give ,mouse's attention to all
						if(GAMEAPPVIEW2->GetCapture() == GAMEAPPVIEW2) ReleaseCapture();

						// give mouse full movement
						ClipCursor(&GAMEAPPVIEW2->m_rectCursor);

						// reset mouse cursor position back to start position
						CRect rect;
						GAMEAPPVIEW2->GetWindowRect(&rect);
						SetCursorPos(rect.left + m_pParent->m_sCPoint.x+2, rect.top + m_pParent->m_sCPoint.y+2);
					
						m_pParent->SelectBall(0, ctmsgData->point);
						
						m_pParent->m_point = ctmsgData->point;
						m_pParent->m_sightlineFade = 0;
						
						if(m_pParent->m_bPracticeMode);
						else
							m_pParent->SelectBallSend(0, ctmsgData->point, m_pParent->m_windowWidth, m_pParent->m_windowHeight, m_pParent->m_selectBall);
					}
				}

				if(!m_pParent->m_cueBallSelect)
				if(!GAMEAPPVIEW2->m_bShowCursor)
				{
					GAMEAPPVIEW2->m_bShowCursor = TRUE;
					SetCursor(GAMEAPPVIEW2->m_hCursor);
					ShowCursor(GAMEAPPVIEW2->m_bShowCursor); // show cursor
				}

				///
				///
				///

				GAMEAPPVIEW2->AlertMessage("Warning! You can only strike the 'White' ball!\n\n(Use 'Left-Mouse-Button' to select the 'White' ball)");
				return;
			}

	// ---------------------------------------------
	// normal 'LineSet' here!
	// ---------------------------------------------

	// Now Give ,mouse's attention to all
	if(GAMEAPPVIEW2->GetCapture() == GAMEAPPVIEW2) ReleaseCapture();
			

	// test!!! send final cue-line pos
	GAMEAPPVIEW2->SendPlayerMouseMove(m_pParent->m_mouseX, m_pParent->m_mouseY, 
							m_pParent->m_windowWidth, m_pParent->m_windowHeight,
							m_pParent->m_strikeLine, m_pParent->m_strikeDist);

	m_pParent->m_cueBallSelect = 5; //--
	m_pParent->m_mouseX = 0; // position cuetip GFX pointer at centre of selected ball
	m_pParent->m_mouseY = 0;

	GAMEAPPVIEW2->m_sliderRaiseButt.EnableWindow(TRUE);
	m_pParent->m_bRaiseButtHelper = TRUE;

	// give mouse full movement
	ClipCursor(&GAMEAPPVIEW2->m_rectCursor);

	if(!GAMEAPPVIEW2->m_bShowCursor)
	{
		GAMEAPPVIEW2->m_bShowCursor = TRUE;
		SetCursor(GAMEAPPVIEW2->m_hCursor);
		ShowCursor(GAMEAPPVIEW2->m_bShowCursor); // show cursor
	}

	// reset mouse cursor position back to start position
	CRect rect;
	GAMEAPPVIEW2->GetWindowRect(&rect);
	SetCursorPos(rect.left + m_pParent->m_sCPoint.x+2, rect.top + m_pParent->m_sCPoint.y+2);

	m_pParent->m_bCueLineSet = TRUE;
	
	m_pParent->m_tableMousePosStrike[0] = m_pParent->m_tableMousePos[0];
	m_pParent->m_tableMousePosStrike[1] = m_pParent->m_tableMousePos[1];
	m_pParent->m_tableMousePosStrike[2] = m_pParent->m_tableMousePos[2];
}


void CCustomTable::Referee6BallPOCKETSET(DWORD msg, void *pData)
{
	ctmsg_POCKETSET_DATA *ctmsgData;
	ctmsgData = (ctmsg_POCKETSET_DATA_typ *) pData;
	
	int n, m;

	if(m_pParent->m_cueBallSelect != 5) return;	// only in state '5'(after line is set!), are we allowed to do the following

	if(m_pParent->m_colourseq == 0) return; // if this is a break-off shot, dont do the following

		
	if(m_pParent->m_bCallShot) // default
	{
		n = m_pParent->GetSelectedPocket();
		m = m_pParent->GetSelectedBall();
		// MUST BE IN FIXMODE!
/*		
		{
			char buf[256];
			sprintf(buf, "n = %d, m = %d", n, m);
			ErrorMessage(m_pParent->m_parent, buf);
		}
*/		

		if(n != -1 || m != -1)	// if player has clicked inside a pocket OR has clicked on a ball
		{
			if(n != -1) m_pParent->SetTargetPocket(n);			
			if(m != -1) m_pParent->SetTargetBall(m);

			// send target ball/pocket/success data to all at table
			GAMEAPPVIEW2->SendPlayerTargetData(m_pParent->m_targetPocket, m_pParent->m_targetBall, m_pParent->m_bTargetSuccess);
		}
	}
}


void CCustomTable::Referee6BallRESPOTBALL(int n)
{
	int m;
	int dir;
	BOOL successful;
	float f1;

	/////////////////////////////////////////////////////////////////////
	// (3) try and spot behind starting position in a direct line
	/////////////////////////////////////////////////////////////////////					
	// check to left side of spot (dir = -1)..., then the right side (dir = 1)...
	if(!TestFlag(m_pParent->m_ball[n]->m_flags, Flg_BALL_ACTIVE))
	for(dir = -1; dir <= 1; dir+=2)
	{
		CVector F = CVector(WORLD_X(591+65+120), 0, m_pParent->m_ball[n]->m_PSpot.z);
		
		// place ball on foot spot
		m_pParent->m_ball[n]->m_POld = m_pParent->m_ball[n]->m_P = F;//m_ball[n]->m_PSpot;
		m_pParent->m_ball[n]->m_W = m_pParent->m_ball[n]->m_V = m_pParent->m_ball[n]->m_VOld = CVector(0,0,0);

		do
		{
			f1 = BALL_RADIUS/20.0f;
			f1  = f1 * (float)dir;
			m_pParent->m_ball[n]->m_P.x -= f1;
			//m_ball[n]->m_P.x -= (BALL_RADIUS/20)*dir;

			successful = TRUE;
			for(m=0;m<MAX_TOTALBALLS;m++)
			if(n!=m)
			if(TestFlag(m_pParent->m_ball[m]->m_flags, Flg_BALL_ACTIVE)) // NEW: make sure this ball is active
			{
				if(!m_pParent->m_ball[n]->TryReSpot3(m_pParent->m_ball[m]))
					successful = FALSE;
			}

			// is this ball on top of a pocket
			float x = m_pParent->m_ball[n]->m_P.x;
			float y = m_pParent->m_ball[n]->m_P.y;
			for(int p=0;p<m_pocket_n;p++)
			{
				if( PocketProximity(p, CVector(x, y, 0), BALL_RADIUS) )
				{						
					successful = FALSE;
					break;
				}
			} // end for p

			if(successful) {
				m_pParent->m_ball[n]->ReSpotSuccessful();
				goto end;
			}

		}
		while(!m_pParent->m_ball[n]->CushionsProximity(FALSE)); // NEW
		//while((m_ball[n]->m_P.x >= (-1736+BALL_RADIUS)) ||
		//	  (m_ball[n]->m_P.x <= (1736-BALL_RADIUS)));   // bottom cushion + BALL_RADIUS || top cushion - BALL_RADIUS
		// *TODO: note: the above boundary check is incorrect!!!, it should be using the actual 3d world "Cushions"
	}
	/////////////////////////////////////////////////////////////////////

end:;

	#ifdef FIXEDMODE
	m_pParent->m_ball[n]->UpdateFixeds();
	#endif
}


void CCustomTable::Referee6BallSIMEND(void)
{
	int n;
	int colour;
	int no_colours;
	BOOL bWhiteInPocket = FALSE;
	BOOL bNextPlayer = FALSE;
	BOOL bCalledBallWasPotted = FALSE;

	int _spots = 0;
	//int _stripes = 0;
	int _6ball = 0;
	int _pottedspots = 0;
	//int _pottedstripes = 0;
	int _potted6ball = 0;

	int colourHit1st = 0; // points to white

	BOOL bPushOutShot = FALSE;

	m_pParent->m_bMiss = TRUE;												// assume no balls potted
	ClearFlag(m_pParent->m_foul, Flg_FOUL_NORMAL | Flg_FOUL_WHITEPOTTED | Flg_FOUL_GAMEOVER | Flg_FOUL_BALLINHAND);	// assume no foul to start with [keep some flags from previous shot]
	ClearFlag(m_pParent->m_foul, Flg_FOUL_ALERT);
	ClearFlag(m_pParent->m_foul, Flg_FOUL_NOTHITCUSHION | Flg_FOUL_NOTHITCUSHIONBREAK);
	m_pParent->m_bRefreshScores = TRUE;
	
	if(!m_pParent->m_bCallShot)
		bCalledBallWasPotted = TRUE; // just for safety, in case I forget to use -> "if(m_pParent->m_bCallShot)"

	// handle push-out stuff (2:2)
	if(TestFlag(m_pParent->m_foul, Flg_FOUL_ALERTPUSHOUT))
	{
		if(m_pParent->m_colourseq > 0)
		{
			if(TestFlag(m_pParent->m_foul, Flg_FOUL_ALERTPUSHOUT))
				ClearFlag(m_pParent->m_foul, Flg_FOUL_ALERTPUSHOUT);
			
			if(TestFlag(m_pParent->m_foul, Flg_FOUL_ALERTPUSHOUT2))
			{
				SetFlag(m_pParent->m_foul, Flg_FOUL_ALERT);
				bPushOutShot = TRUE;
			}
		}
	}
	else
		ClearFlag(m_pParent->m_foul, Flg_FOUL_ALERTPUSHOUT2);


	// just in case :)
	//if(m_colourseq)
	//	m_rednext = FALSE;

	// [freeball mode]
	//if(TestFlag(m_foul, Flg_FOUL_FREEBALL))
	//{
	//	//MessageBox(0, "0","0",MB_OK);
//		FreeBallReferee();
//		ClearFlag(m_foul, Flg_FOUL_FREEBALL);
//		goto end;
//	}

	// [default mode]

	/////////////////////////////////////////////////////////////
	// what is left?
	

	for(n=0;n<MAX_TOTALBALLS;n++)
	{
		if(TestFlag(m_pParent->m_ball[n]->m_flags, Flg_BALL_ACTIVE))
		{
			if(m_pParent->m_ball[n]->m_colour >= 1 && m_pParent->m_ball[n]->m_colour <= 5)
			{
				_spots++;
			}
			/*
			else if(m_pParent->m_ball[n]->m_colour >= 9 && m_pParent->m_ball[n]->m_colour <= 15)
			{
				_stripes++;
			}
			*/
			else if(m_pParent->m_ball[n]->m_colour == 6)
			{
				_6ball++;
			}
		}
	}

	//char buf3[80];
	//sprintf(buf3, "spots = %d; 9-ball = %d", _spots, _9ball);
	//ErrorMessage(m_pParent->m_parent, buf3);

	//DEBUG!
	//char buf3[80];
	//sprintf(buf3, "PRE: runOut = %d", m_pParent->m_break2P);
	//ErrorMessage(m_pParent->m_parent, buf3);

	/////////////////////////////////////////////////////////////
	// what was pottted?
											

	for(n=0;n<MAX_TOTALBALLS;n++)
	{
		if(TestFlag(m_pParent->m_ball[n]->m_flags, Flg_BALL_POTTED))
		{
			// was target/called ball potted?
			if(m_pParent->m_bCallShot)
			if(m_pParent->m_bTargetSuccess)
			if(m_pParent->m_targetBall == m_pParent->m_ball[n]->m_colour)
				bCalledBallWasPotted = TRUE;

			if(m_pParent->m_ball[n]->m_colour >= 1 && m_pParent->m_ball[n]->m_colour <= 5)
			{
				_pottedspots++;
			}
			/*
			else if(m_pParent->m_ball[n]->m_colour >= 9 && m_pParent->m_ball[n]->m_colour <= 15)
			{
				_pottedstripes++;
			}
			*/
			else if(m_pParent->m_ball[n]->m_colour == 6)
			{
				_potted6ball++;
			}
		}
	}

	/////////////////////////////////////////////////////////////
	// see if white ball has been potted (foul)
	for(n=0;n<MAX_TOTALBALLS;n++)
	{
		if(TestFlag(m_pParent->m_ball[n]->m_flags, Flg_BALL_POTTED))
			if(m_pParent->m_ball[n]->m_colour == WHITE)
			{
				bWhiteInPocket = TRUE;
				if(m_pParent->m_colourseq == 0) SetFlag(m_pParent->m_foul, Flg_FOUL_NORMAL | Flg_FOUL_WHITEPOTTED | Flg_FOUL_BREAK);
					else SetFlag(m_pParent->m_foul, Flg_FOUL_NORMAL | Flg_FOUL_WHITEPOTTED);


				if(_potted6ball != 0)
				{
					// *TODO - FOUL!!!! DO RERACK!!!!
					//SetFlag(m_pParent->m_foul, Flg_FOUL_GAMEOVER);
					
					// respot 6-ball
					for(n=0;n<MAX_TOTALBALLS;n++)
					if(TestFlag(m_pParent->m_ball[n]->m_flags, Flg_BALL_POTTED))
					if(m_pParent->m_ball[n]->m_colour == 6)
					{
						Referee6BallRESPOTBALL(n);
						break;
					}					
				}
			}
	}
	/////////////////////////////////////////////////////////////

	/////////////////////////////////////////////////////////////
	// what happens if player strikes a ball other than the white
	if(m_pParent->m_selectBall != WHITE)
	{
		SetFlag(m_pParent->m_foul, Flg_FOUL_NORMAL);

		if(_potted6ball != 0)
		{
			// *TODO - FOUL!!!! DO RERACK!!!!
			//SetFlag(m_pParent->m_foul, Flg_FOUL_GAMEOVER);
			
			// respot 6-ball
			for(n=0;n<MAX_TOTALBALLS;n++)
			if(TestFlag(m_pParent->m_ball[n]->m_flags, Flg_BALL_POTTED))
			if(m_pParent->m_ball[n]->m_colour == 6)
			{
				Referee6BallRESPOTBALL(n);
				break;
			}
		}

//		goto end;
	}
	/////////////////////////////////////////////////////////////
	
	/////////////////////////////////////////////////////////////
	// if white doesn't hit a ball at all, foul ball on!
	//if(m_colourseq < YELLOW) // bypass if on colours at the end
	if(!bPushOutShot)
	if((m_pParent->m_selectBall == WHITE) && (m_pParent->m_dominoHit1st == WHITE))
	{
			SetFlag(m_pParent->m_foul, Flg_FOUL_NORMAL);

			if(_potted6ball != 0)
			{
				// *TODO - FOUL!!!! DO RERACK!!!!
				//SetFlag(m_pParent->m_foul, Flg_FOUL_GAMEOVER);

				// respot 6-ball
				for(n=0;n<MAX_TOTALBALLS;n++)
				if(TestFlag(m_pParent->m_ball[n]->m_flags, Flg_BALL_POTTED))
				if(m_pParent->m_ball[n]->m_colour == 6)
				{
					Referee6BallRESPOTBALL(n);
					break;
				}				
			}

//			goto end;
	}
	/////////////////////////////////////////////////////////////

	///
	// [NEW] if nothing was potted, did cueball or spotball touch a cushion?
	if(m_pParent->m_colourseq > 0)
	if(!bWhiteInPocket)
	if(!bPushOutShot)
	if(_pottedspots == 0 && _potted6ball == 0)
	{
		BOOL bFailedToHitCushion = TRUE;
		
		for(n=0;n<MAX_TOTALBALLS;n++)
		{	
			if(m_pParent->m_ball[n]->m_hitCushion)
			{
				bFailedToHitCushion = FALSE; // no foul!
				break;
			}
		}

		if(bFailedToHitCushion)
		{
			SetFlag(m_pParent->m_foul, Flg_FOUL_NORMAL | Flg_FOUL_NOTHITCUSHION);
			//ErrorMessage(m_pParent->m_parent, "111");
		}
	}
	///////////////////////////////////////////////////////////////////////////
	///




	colourHit1st = m_pParent->m_ball[m_pParent->m_dominoHit1st]->m_colour;



				if(m_pParent->m_colourseq == 0)
				{
					///
					// [NEW] if nothing was potted on the break, did >= 4 numbered balls touch a cushion?					
					if(!bWhiteInPocket)
					if(!bPushOutShot)
					if(_pottedspots == 0 && _potted6ball == 0)
					{						
						int hitCushion = 0;
						
						for(n=1;n<MAX_TOTALBALLS;n++) // exclude white ball
						{	
							if(m_pParent->m_ball[n]->m_hitCushion)
							{
								hitCushion++; // (at least 4 numbered balls)
							}
						}
						
						if(hitCushion < 4)
						{
							SetFlag(m_pParent->m_foul, Flg_FOUL_NORMAL | Flg_FOUL_NOTHITCUSHIONBREAK);
						}

						//char buf[80];
						//sprintf(buf, "hitCushion = %d", hitCushion);
						//ErrorMessage(m_pParent->m_parent, buf);
					}
					///////////////////////////////////////////////////////////////////////////
					///
					
					// on the break!
					m_pParent->m_colourseq = 1;

					if(_potted6ball != 0)
					{
						if( (colourHit1st == 1) && !TestFlag(m_pParent->m_foul, Flg_FOUL_NORMAL) ) // 1-ball
						{
							// Golden break!!!!
							if(m_pParent->m_bTurn1P)
							{
								m_pParent->m_gameOver = 2;
								Msfx_smpplayvol(&GAMEAPPVIEW2->m_game_smp[SFX_GOODSHOT], 255);
							}
							else
							{
								m_pParent->m_gameOver = 1;
								Msfx_smpplayvol(&GAMEAPPVIEW2->m_game_smp[SFX_GOODSHOT], 255);
							}
						}
						else
						{
							// this player losses for now!
							//SetFlag(m_pParent->m_foul, Flg_FOUL_NORMAL | Flg_FOUL_GAMEOVER); //--
							
							SetFlag(m_pParent->m_foul, Flg_FOUL_NORMAL);

							// respot 6-ball
							for(n=0;n<MAX_TOTALBALLS;n++)
							if(TestFlag(m_pParent->m_ball[n]->m_flags, Flg_BALL_POTTED))
							if(m_pParent->m_ball[n]->m_colour == 6)
							{
								Referee6BallRESPOTBALL(n);
								break;
							}
							//goto end; //--
						}						
					}
					
					// what is the lowest value ball left on the table?
					int lowestball = 6;
					for(n=0;n<MAX_TOTALBALLS;n++)
					if(TestFlag(m_pParent->m_ball[n]->m_flags, Flg_BALL_ACTIVE))
					if(!TestFlag(m_pParent->m_ball[n]->m_flags, Flg_BALL_POTTED))
					{
						if(m_pParent->m_ball[n]->m_colour >= 1 && m_pParent->m_ball[n]->m_colour <= 6)
							if(m_pParent->m_ball[n]->m_colour < lowestball)
								lowestball = m_pParent->m_ball[n]->m_colour;			
					}

					// point to next ball to pot
					m_pParent->m_colourseq = lowestball;

					if(colourHit1st != 1) // if 1-ball wasn't hit first on the break
					{
						SetFlag(m_pParent->m_foul, Flg_FOUL_NORMAL);
						goto end;
					}

					if(TestFlag(m_pParent->m_foul, Flg_FOUL_NORMAL))
					{
						goto end;
					}

					// allow Push-Out now ( handle push-out stuff (1:2) )
					SetFlag(m_pParent->m_foul, Flg_FOUL_ALERT | Flg_FOUL_ALERTPUSHOUT);
				}
				else if(m_pParent->m_colourseq > 0)
				{
					// shots after the break
					BOOL bHitWrongBall = FALSE;
					if(colourHit1st != m_pParent->m_colourseq) // if ball-on wasn't hit first
						bHitWrongBall = TRUE;
					
					if(_potted6ball != 0)
					{
						if(bPushOutShot)
						{
							//// this player losses for now!
							//SetFlag(m_pParent->m_foul, Flg_FOUL_NORMAL | Flg_FOUL_GAMEOVER);
							//goto end;

							// respot 6-ball
							for(n=0;n<MAX_TOTALBALLS;n++)
							if(TestFlag(m_pParent->m_ball[n]->m_flags, Flg_BALL_POTTED))
							if(m_pParent->m_ball[n]->m_colour == 6)
							{
								Referee6BallRESPOTBALL(n);
								break;
							}

							goto _skipPotted6Ball;
						}

						// if this is a called shot type game
						if(m_pParent->m_bCallShot && !bCalledBallWasPotted)
						{
							SetFlag(m_pParent->m_foul, Flg_FOUL_NORMAL);

							// respot 6-ball
							for(n=0;n<MAX_TOTALBALLS;n++)
							if(TestFlag(m_pParent->m_ball[n]->m_flags, Flg_BALL_POTTED))
							if(m_pParent->m_ball[n]->m_colour == 6)
							{
								Referee6BallRESPOTBALL(n);
								break;
							}							
						}
						else
						{
							// default : old way
							if(!bHitWrongBall && !TestFlag(m_pParent->m_foul, Flg_FOUL_NORMAL) ) // ball-on (i.e. lowest ball that was on)
								//if( (colourHit1st == 1) &&  ) // 1-ball
							{							
								// Combination on the 6!!!! / Or finally potted the 6! (Check this OAP)
								if(m_pParent->m_bTurn1P)
								{
									m_pParent->m_gameOver = 2;
									
									if(_spots == 0) // if there are no spots left &...
									if(m_pParent->m_break2P == 1) // if we're on a run-out
									{
//										m_pParent->m_highBreaks1P[0] = 60; // 60 => run-out for 9-ball stat. // *TODO - 6ball runout!
										Msfx_smpplayvol(&GAMEAPPVIEW2->m_game_smp[SFX_GOODSHOT], 255);
									}
								}
								else
								{
									m_pParent->m_gameOver = 1;

									if(_spots == 0) // if there are no spots left &...
									if(m_pParent->m_break2P == 1) // if we're on a run-out
									{
//										m_pParent->m_highBreaks2P[0] = 60; // 60 => run-out for 9-ball stat. // *TODO - 6ball runout!
										Msfx_smpplayvol(&GAMEAPPVIEW2->m_game_smp[SFX_GOODSHOT], 255);
									}
								}
							}
							else
							{
								// this player losses for now!
								//SetFlag(m_pParent->m_foul, Flg_FOUL_NORMAL | Flg_FOUL_GAMEOVER); //--

								SetFlag(m_pParent->m_foul, Flg_FOUL_NORMAL);

								// respot 6-ball
								for(n=0;n<MAX_TOTALBALLS;n++)
								if(TestFlag(m_pParent->m_ball[n]->m_flags, Flg_BALL_POTTED))
								if(m_pParent->m_ball[n]->m_colour == 6)
								{
									Referee6BallRESPOTBALL(n);
									break;
								}
														
								//goto end; //--
							}
						}
					}
				_skipPotted6Ball:;
					// what is the lowest value ball left on the table?
					int lowestball = 6;
					for(n=0;n<MAX_TOTALBALLS;n++)
					if(TestFlag(m_pParent->m_ball[n]->m_flags, Flg_BALL_ACTIVE))
					if(!TestFlag(m_pParent->m_ball[n]->m_flags, Flg_BALL_POTTED))
					{
						if(m_pParent->m_ball[n]->m_colour >= 1 && m_pParent->m_ball[n]->m_colour <= 6)
							if(m_pParent->m_ball[n]->m_colour < lowestball)
								lowestball = m_pParent->m_ball[n]->m_colour;
						
// wrong place!!!! (**)
//						if(!bHitWrongBall && !TestFlag(m_pParent->m_foul, Flg_FOUL_NORMAL) )
//						if(m_pParent->m_bCallShot && !bCalledBallWasPotted)
//							SetFlag(m_pParent->m_foul, Flg_FOUL_NORMAL);
						
					}

					// point to next ball to pot
					m_pParent->m_colourseq = lowestball;

					if(!bPushOutShot)
					if(bHitWrongBall) // if ball-on wasn't hit first
					{
						ClearFlag(m_pParent->m_foul, Flg_FOUL_NOTHITCUSHION); // if this person hit the wrong ball, dont bother to tell them abt the cushion rule
						SetFlag(m_pParent->m_foul, Flg_FOUL_NORMAL);
						goto end;
					}

					// did player pot the wrong ball, instead of the called one (**)
					for(n=0;n<MAX_TOTALBALLS;n++)
					if(TestFlag(m_pParent->m_ball[n]->m_flags, Flg_BALL_POTTED))
					if(m_pParent->m_ball[n]->m_colour >= 1 && m_pParent->m_ball[n]->m_colour <= 15)
					{
						if(!bHitWrongBall && !TestFlag(m_pParent->m_foul, Flg_FOUL_NORMAL) )
						if(m_pParent->m_bCallShot && !bCalledBallWasPotted)
						{
							SetFlag(m_pParent->m_foul, Flg_FOUL_NORMAL);
							break;
						}
					}
					/////////////////////////////////////////////////////////////////

					if(TestFlag(m_pParent->m_foul, Flg_FOUL_NORMAL))
						goto end;
				}
				

					//if(m_pParent->m_bTurn1P)

	//			if(m_pParent->m_ball[n]->m_colour == WHITE)
	///			{
	//				bWhiteInPocket = TRUE;
	//				SetFlag(m_pParent->m_foul, Flg_FOUL_NORMAL | Flg_FOUL_WHITEPOTTED);
	//			}
	//		}
	//	}
	  
	/////////////////////////////////////////////////////////////
	//char buf[80];
	//sprintf(buf, "-- m_colourseq = %d ---", m_pParent->m_colourseq);
	//ErrorMessage(m_pParent->m_parent, buf);

	/////////////////////////////////////////////////////////////
	// normal pots...
	if(!bPushOutShot)
	if(!bWhiteInPocket)
	{			
		//MessageBox(0, "11","11",MB_OK);
		for(n=0;n<MAX_TOTALBALLS;n++)
		{
			if(TestFlag(m_pParent->m_ball[n]->m_flags, Flg_BALL_POTTED))
			{					
				

				if(m_pParent->m_bTurn1P) {
					//m_pParent->m_score1P += 1;//m_pParent->m_ball[n]->m_colour;
					//m_pParent->m_break1P += 1;//m_ball[n]->m_colour;
				}
				else {
					//m_pParent->m_score2P += 1;//m_pParent->m_ball[n]->m_colour;
					//m_pParent->m_break2P += 1;//m_ball[n]->m_colour;
				}
				
				m_pParent->m_bMiss = FALSE;		// potted ball

				// after first pot, go into runout mode
				if(m_pParent->m_break2P == 0)
					m_pParent->m_break2P = 1;
				
				//m_pParent->m_foul = 0;				// no foul
				ClearFlag(m_pParent->m_foul, (~(Flg_FOUL_ALERT | Flg_FOUL_ALERTPUSHOUT | Flg_FOUL_ALERTPUSHOUT2)) );

				
				int tbl = GAMEAPPVIEW2->m_lobby->GetTableIdxFromPlayer(GAMEAPPVIEW2->m_playerSockIdx);
				if(tbl!=-1)
				{
					m_pParent->m_shotTime =  m_table[tbl].shotTime*50;
				}

				/*
				if(_potted8ball >0)
				{
					// GAME_OVER! the other player losses!
					if(m_pParent->m_bTurn1P)
						m_pParent->m_gameOver = 2;
					else
						m_pParent->m_gameOver = 1;		
				}
				*/
			}
		}

		/*
		if(!m_colourseq) // if we are NOT on the 'colour seq.' at the end of the frame
		if(m_bMiss == FALSE)
		{
			if(m_rednext)
				m_rednext = FALSE;
			else
				m_rednext = TRUE;
		}
		*/
	}
	/////////////////////////////////////////////////////////////

	/////////////////////////////////////
	// *NEW: this shot was a good one, so clear the foul3times counter
	if(!TestFlag(m_pParent->m_foul, Flg_FOUL_NORMAL))
	{
		if(m_pParent->m_bTurn1P)
			m_pParent->m_foul3Times1P = 0;
		else
			m_pParent->m_foul3Times2P = 0;
	}
	/////////////////////////////////////

end:;

	/////////////////////////////////////
	// *NEW: this shot was a bad one
	if(TestFlag(m_pParent->m_foul, Flg_FOUL_NORMAL))
	{
		if(m_pParent->m_bTurn1P)
			m_pParent->m_foul3Times1P++;
		else
			m_pParent->m_foul3Times2P++;
	}

	// 3 times => gameover!
	if(m_pParent->m_foul3Times1P >= 3)
	{
		m_pParent->m_gameOver = 1;
	}
	else if(m_pParent->m_foul3Times2P >= 3)
	{
		m_pParent->m_gameOver = 2;
	}
	/////////////////////////////////////


	if(TestFlag(m_pParent->m_foul, Flg_FOUL_NORMAL))
	{
		ClearFlag(m_pParent->m_foul, Flg_FOUL_ALERT);
	}

	//char buf[80];
	//sprintf(buf, "-- m_colourseq = %d ---", m_pParent->m_colourseq);
	//ErrorMessage(m_pParent->m_parent, buf);


	// if just done break! and we fouled!
	if(m_pParent->m_colourseq == 0)
		m_pParent->m_colourseq = 1;

	if(TestFlag(m_pParent->m_foul, Flg_FOUL_GAMEOVER))
	{
		// GAME_OVER! this player losses!
		if(m_pParent->m_bTurn1P)
			m_pParent->m_gameOver = 1;
		else
			m_pParent->m_gameOver = 2;		
	}

	/////////////////////////////////////////////////////////////
	// check if nothing was potted this time
	//if(!m_pParent->m_gameOver) // NOT SURE WHY WE NEEDED THIS LINE?

	if(m_pParent->m_bMiss)
	{
		// if we've already potted a ball, but now we've missed or fouled, then NO run-out!
		if(m_pParent->m_break2P == 1)
			m_pParent->m_break2P = -1;

		//ErrorMessage(m_pParent->m_parent, "miss!");
//		// end of break sounds
//		if(g_gfxDetails.bSoundClapping)
//		{
//			if(m_bTurn1P)
//			{
//				if(m_break1P >= 20 && m_break1P < 100)
//				{
//					float loudness = 128 + 128*(((float)m_break1P - 20)/80);
//					Msfx_smpplayvol(&GAMEAPPVIEW->m_game_smp[SFX_20PLUSBREAK], loudness);
//				}
//				
//				/*
//				if(m_break1P >= 100)
//				{
//					Msfx_smpplayvol(&GAMEAPPVIEW->m_game_smp[SFX_CENTURY], 255);
//				}
//				*/
//				
//			}
//			else
//			{
//				if(m_break2P >= 20 && m_break2P < 100)
//				{
//					float loudness = 128 + 128*(((float)m_break1P - 20)/80);
//					Msfx_smpplayvol(&GAMEAPPVIEW->m_game_smp[SFX_20PLUSBREAK], loudness);
//				}
//  
//				/*
//				if(m_break2P >= 100)
//				{
//					Msfx_smpplayvol(&GAMEAPPVIEW->m_game_smp[SFX_CENTURY], 255);
//				}
//				*/
//			}
//		}

		//MessageBox(0, "13","13",MB_OK);
		if(m_pParent->m_bPracticeMode && !GAMEAPPVIEW2->m_bReplayMode && !GAMEAPPVIEW2->m_bPlayOffline)
		{
			// force no fouls in practice
			//m_foul = 0;
			//m_break1P = 0;
		}
		else
		{
			// decide who's turn it is next?
			bNextPlayer = TRUE;
			m_pParent->m_bConcededGame = FALSE; // clear this flag (now that the turn has swapped over, we can allow gameOver to continue...)
			GAMEAPPVIEW2->m_bFlashWindow = TRUE;

			//m_pParent->m_shotTime = 60*50;
			if(TestFlag(m_pParent->m_foul, Flg_FOUL_NORMAL))
				m_pParent->m_shotTime = 60*50;
			else
			{
				// if no foul, then give usual time for shot
				int tbl = GAMEAPPVIEW2->m_lobby->GetTableIdxFromPlayer(GAMEAPPVIEW2->m_playerSockIdx);
				if(tbl!=-1)
				{
					m_pParent->m_shotTime =  m_table[tbl].shotTime*50;
				}
			}

			//ErrorMessage(m_parent, "1. reg break!");

			if(m_pParent->m_bTurn1P) {
				m_pParent->m_bTurn1P = FALSE;

			}
			else {
				m_pParent->m_bTurn1P = TRUE;
			}
		}

		// if we are NOT on the 'colour seq.' at the end of the frame
		//if(!m_colourseq)
		//	m_rednext = TRUE;
	}
	/////////////////////////////////////////////////////////////	

	Referee6BallUpdatePots();

	// re-spot balls
	Referee6BallReSpot();

	//char buf33[80];
	//sprintf(buf33, "POST: runOut = %d", m_pParent->m_break2P);
	//ErrorMessage(m_pParent->m_parent, buf33);

		
		///////////
		// for practice mode : fixes "pot yellow aand white simul. bug" ***
		if(m_pParent->m_bMiss)
		{
			//MessageBox(0, "13b","13b",MB_OK);
			if(m_pParent->m_bPracticeMode && !GAMEAPPVIEW2->m_bPlayOffline)
			{
				// force no fouls in practice
				m_pParent->m_foul = 0;
				//m_pParent->m_break1P = 0;
			}
		}
		///////////

		
		if(TestFlag(m_pParent->m_foul, Flg_FOUL_NORMAL))
		{
			m_pParent->m_bBallInHand = 1;
			m_pParent->m_bStandUpFromShot = 1;
		}
}


///////////////////////////////////////////////////////////////////////////////////////////////////
//
//
void CCustomTable::Referee6BallReSpot(void)
{
	int n;
	int m;
	int nextspot;
	int dir;
	BOOL successful;
	BOOL reds;

	float f1,f2,f3;

	
	////////////////////////////////////////////////////////////////////////////////////////////////////////////
	
	//ErrorMessage(m_pParent->m_parent, "Respot:(1,2,3)");

	// for balls in pockects
	for(n=MAX_TOTALBALLS-1; n >= 0;n--)
	//if((m_ball[n]->m_colour <= BLACK && m_ball[n]->m_colour >= YELLOW) || (m_ball[n]->m_colour == WHITE))
	if((m_pParent->m_ball[n]->m_colour == WHITE)) // ***** TODO ***** -> 8 BALL
	{	
		// temp. code for WHITE ball
		if(m_pParent->m_ball[n]->m_colour == WHITE)
			//if(!TestFlag(m_pParent->m_ball[n]->m_flags, Flg_BALL_ACTIVE))
			if(TestFlag(m_pParent->m_foul, Flg_FOUL_WHITEPOTTED))
				{
					// position ball on its original starting position or...
					m_pParent->m_ball[n]->m_POld = m_pParent->m_ball[n]->m_P = m_pParent->m_ball[n]->m_PSpot;
					
					//ErrorMessage(m_pParent->m_parent, "whiteball-1");

					if( Referee6BallDropBallOnTable(WHITE, m_pParent->m_ball[n]->m_P.x, m_pParent->m_ball[n]->m_P.y ) )
					{
						//ErrorMessage(m_pParent->m_parent, "whiteball-2");
					}
					else					
					{
						//ErrorMessage(m_pParent->m_parent, "whiteball-3");
						// ...otherwise position anywhere behind the 'Head String'
						CVector Pgreen, Pyellow;
						CVector P;
						float radius;
						//Pgreen = CVector(-WORLD_X(912),-WORLD_Y(395-12), BALL_POSITIONZOFFSET);
						//Pyellow = CVector(-WORLD_X(912),-WORLD_Y(199+12), BALL_POSITIONZOFFSET);
						Pgreen = CVector(-WORLD_X(591+65+120/*912+5*/), -600, BALL_POSITIONZOFFSET);
						Pyellow = CVector(-WORLD_X(591+65+120/*912+5*/), 600, BALL_POSITIONZOFFSET);
						float w,h;
						float x,y;
						
						//[f1 = Pyellow.y - Pgreen.y;]
						f1 = Pyellow.y;
						f1 = f1 - Pgreen.y;
						
						f1 = f1 * 0.9f;
						h =	f1;
						//h =	(Pyellow.y - Pgreen.y)*0.9f;
						w = h*0.5f;
						for(x = 0.0f;x<w;x+=BALL_RADIUS)
						for(y = 0.0f;y<h;y+=BALL_RADIUS)
						{
							
							//[f1 = Pyellow.x - x;]
							f1 = Pyellow.x;
							f1 = f1 - x;
							
							//[f2 = Pyellow.y - Pgreen.y;]
							f2 = Pyellow.y;
							f2 = f2 - Pgreen.y;
							
							f2 = f2 * 0.05f;
							
							//[f3 = Pyellow.y - y;]
							f3 = Pyellow.y;
							f3 = f3 - y;
							
							f3 = f3 - f2;
							if( Referee6BallDropBallOnTable(WHITE, f1, f3) )
							//if( DropBallOnTable(WHITE, Pyellow.x - x, Pyellow.y - y - (Pyellow.y - Pgreen.y)*0.05f) )
								goto whiteSpotted;
						}
						whiteSpotted:;
					}
					
//					m_ball[n]->m_POld = m_ball[n]->m_P = m_ball[n]->m_PSpot;
					m_pParent->m_ball[n]->m_W = m_pParent->m_ball[n]->m_V = m_pParent->m_ball[n]->m_VOld = CVector(0,0,0);
	
					// turn ball on
					m_pParent->m_ball[n]->m_flags = 0;
					SetFlag(m_pParent->m_ball[n]->m_flags, Flg_BALL_ACTIVE);

					m_pParent->m_bBallInHand = 1;
					m_pParent->m_bStandUpFromShot = 1;


					continue;
				}
	}
}

BOOL CCustomTable::Referee6BallDropBallOnTable(int n, float x, float y, BOOL bDrop)
{
	// check if ball 'n' is on top of another ball
	int m;
	for(m=0;m<MAX_TOTALBALLS;m++)
	if(m!=n)
	if(m_pParent->m_ball[m]->Proximity(x, y, 0, BALL_RADIUS*2.0f) == TRUE)
	{
		//ErrorMessage(m_pParent->m_parent, "FALSE");
		return FALSE;
	}
	
	// check for ball n inside the D
	if(!m_pParent->m_bPracticeMode) // should be !
	{
		CVector B = CVector(x,y,BALL_POSITIONZOFFSET);
		CVector Pgreen, Pbrown;
		CVector P;
		float radius;
		//Pgreen = CVector(-WORLD_X(912),-WORLD_Y(395-12), BALL_POSITIONZOFFSET);
		//Pbrown = CVector(-WORLD_X(912),-WORLD_Y(297),BALL_POSITIONZOFFSET);
		Pgreen = CVector(-WORLD_X(591+65+120/*912+5*/), -600, BALL_POSITIONZOFFSET);
		Pbrown = CVector(-WORLD_X(591+65+120/*912+5*/), 0, BALL_POSITIONZOFFSET);

		//[P = Pgreen - Pbrown;]
		P = Pgreen;
		P = P - Pbrown;

		radius = P.Magnitude2();
		
		//[P = B - Pbrown;]
		P = B;
		P = P - Pbrown;

		if(TestFlag(m_pParent->m_foul, Flg_FOUL_WHITEPOTTED) || TestFlag(m_pParent->m_foul, Flg_FOUL_NORMAL)
			|| TestFlag(m_pParent->m_foul, Flg_FOUL_BALLINHAND))
			Pbrown.x = 1235;	// give ball-in-hand access to whole table

		//if( P.Magnitude2() < radius)
		if((B.y < -Pgreen.y) && (B.y > Pgreen.y))
		{			
			if((B.x < Pbrown.x) && (B.x > -1235))
			{
				// update the drop ball's coordinates
				if(bDrop)
				{
					//m_pParent->m_ball[n]->m_P.x = x;
					//m_pParent->m_ball[n]->m_P.y = y;
					//m_pParent->m_ball[n]->AdjustOrientation();
					//m_pParent->m_ball[n]->m_POld = m_pParent->m_ball[n]->m_P;
				m_pParent->m_ball[n]->m_P.x = x;
				m_pParent->m_ball[n]->m_P.y = y;
				#ifdef FIXEDMODE
				m_pParent->m_ball[n]->m_V.x = 0;
				m_pParent->m_ball[n]->m_V.y = 0;
				m_pParent->m_ball[n]->m_V.z = 0;
				m_pParent->m_ball[n]->UpdateFixeds();
				#endif
				m_pParent->m_ball[n]->AdjustOrientation();
				m_pParent->m_ball[n]->m_POld = m_pParent->m_ball[n]->m_P;
				#ifdef FIXEDMODE
				m_pParent->m_ball[n]->UpdateFixeds();
				#endif

				}
				return TRUE;
			}
		}
		return FALSE;
	}
	
	// update the drop ball's coordinates
	if(bDrop)
	{
		//m_pParent->m_ball[n]->m_P.x = x;
		//m_pParent->m_ball[n]->m_P.y = y;
		//m_pParent->m_ball[n]->AdjustOrientation();
		//m_pParent->m_ball[n]->m_POld = m_pParent->m_ball[n]->m_P;
		//ErrorMessage(m_pParent->m_parent, "TRUE");

		m_pParent->m_ball[n]->m_P.x = x;
		m_pParent->m_ball[n]->m_P.y = y;
		#ifdef FIXEDMODE
		m_pParent->m_ball[n]->m_V.x = 0;
		m_pParent->m_ball[n]->m_V.y = 0;
		m_pParent->m_ball[n]->m_V.z = 0;
		m_pParent->m_ball[n]->UpdateFixeds();
		#endif
		m_pParent->m_ball[n]->AdjustOrientation();
		m_pParent->m_ball[n]->m_POld = m_pParent->m_ball[n]->m_P;
		#ifdef FIXEDMODE
		m_pParent->m_ball[n]->UpdateFixeds();
		#endif

	}
	return TRUE;
}

void CCustomTable::Referee6BallUpdatePots(void)
{
	int ballColour;
	float fx,fy;
	int x,y;
		
	int n;
	for(n = 0; n < 16; n++)
	{
		g_pottedSpots[n] = -1;
		g_pottedStripes[n] = -1;
	}

	// scan thru all potted balls, and see who's potted what
	int m;
	for(m=0;m<MAX_TOTALBALLS;m++)
	if(!TestFlag(m_pParent->m_ball[m]->m_flags, Flg_BALL_ACTIVE))
	{
		fx = m_pParent->m_ball[m]->m_P.x;
		fy = m_pParent->m_ball[m]->m_P.y;

		
		// what has P1 potted
		if(fx > 20001)
		{
			fx-=20000;
			fx /= 100;
			if(fx>=0 && fx<= 15)
			{
				x = fx;
				y = fy;
				if((m_pParent->m_ball[m]->m_colour >= 1) &&  (m_pParent->m_ball[m]->m_colour <= 5))
					g_pottedSpots[x] = m;
				/*
				else if((m_pParent->m_ball[m]->m_colour >= 9) &&  (m_pParent->m_ball[m]->m_colour <= 15))
					g_pottedStripes[x] = m;
				*/				
			}
		}

		// what has P2 potted
		if(fy > 20001)
		{
			fy-=20000;
			fy /= 100;
			if(fy>=0 && fy<= 15)
			{
				x = fx;
				y = fy;
				if((m_pParent->m_ball[m]->m_colour >= 1) &&  (m_pParent->m_ball[m]->m_colour <= 5))
					g_pottedSpots[y] = m;
				/*
				else if((m_pParent->m_ball[m]->m_colour >= 9) &&  (m_pParent->m_ball[m]->m_colour <= 15))
					g_pottedStripes[y] = m;
				*/
			}
		}
	}
	/*			
	fy = 0;
	ballColour = 0;

	// draw spots on left-hand side		
	for(m=0;m<16;m++)
	if(g_pottedSpots[m] != -1)
	{			
		ballColour = m_pParent->m_ball[g_pottedSpots[m]]->m_colour; 
		
		// *TODO: 
		//if(m_pParent->m_colourseq == 2) // 1P=SPOTS & 2P=STRIPES
		//	draw2DBall(ballColour, x = LEFT, y = fy);
		//else if(m_pParent->m_colourseq == 3) // 1P=STRIPES & 2P=SPOTS
		//	draw2DBall(ballColour, x = RIGHT, y = fy);

		fy += 32;
	}

	char bbb[80];
	sprintf(bbb, "spots potted = %f", fy / 32); 
	ErrorMessage(m_pParent->m_parent, bbb);

	// draw stripes on left-hand side		
	fy = 0;
	ballColour = 0;
	for(m=0;m<16;m++)
	if(g_pottedStripes[m] != -1)
	{			
		ballColour = m_pParent->m_ball[g_pottedStripes[m]]->m_colour; 
		
		// *TODO: 
		//if(m_pParent->m_colourseq == 2) // 1P=SPOTS & 2P=STRIPES
		//	draw2DBall(ballColour, x = RIGHT, y = fy);
		//else if(m_pParent->m_colourseq == 3) // 1P=STRIPES & 2P=SPOTS
		//	draw2DBall(ballColour, x = LEFT, y = fy);

		fy += 32;
	}		
	
	sprintf(bbb, "stripes potted = %f", fy / 32); 
	ErrorMessage(m_pParent->m_parent, bbb);
	*/
}

//**

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CCustomTable::Referee10Ball(DWORD msg, void *pData)
{
	if(msg == CTMSG_LINESET)
	{
		Referee10BallLINESET(msg, pData);
	}
	else if(msg == CTMSG_POCKETSET)
	{
		Referee10BallPOCKETSET(msg, pData);
	}
	else if(msg == CTMSG_SIMEND)
	{
		Decode();
		Referee10BallSIMEND();
		Encode();
	}
	else
	{
		RefereeSnooker(msg, pData);
	}
}

void CCustomTable::Referee10BallLINESET(DWORD msg, void *pData)
{
	ctmsg_LINESET_DATA *ctmsgData;
	ctmsgData = (ctmsg_LINESET_DATA_typ *) pData;

	// -----------------------------------------------------------------------------
	// check if player has not selected the white ball & we're not in Practice mode
	// -----------------------------------------------------------------------------
	//if(!m_pParent->m_rednext)
	  //if(m_pParent->m_colourseq < YELLOW)
		//if(m_pParent->m_testBall->m_colour == WHITE)
			//if(m_pParent->m_nominateBall == WHITE)
		if(!m_pParent->m_bPracticeMode)			
			if(m_pParent->m_selectBall != WHITE)
			{
				///		
				/// [simulate R-Button down for cancel shot]
				///

				m_pParent->m_bTestBallFinished = FALSE;
				//m_snooker->m_bCueTipReady = FALSE;

				// reset foucus to table (this) window, & reset 'raise butt'
				m_pParent->m_RaiseButt = m_pParent->m_RaiseButtOld = 0;
				m_pParent->m_bReceiveRaiseButtActive = FALSE;
				m_pParent->m_bRaiseButtHelper = TRUE;
				GAMEAPPVIEW2->m_sliderRaiseButt.SetPos(90);
				GAMEAPPVIEW2->PreviousWndEditSetFocus();

				GAMEAPPVIEW2->m_point = ctmsgData->point;
					
				// Only give this window the mouse's full attention 
				GAMEAPPVIEW2->SetCapture();
				
				if(m_pParent->m_tableMouseOn)
				{
					if(m_pParent->m_bCueState)
					{
						// Now Give ,mouse's attention to all
						if(GAMEAPPVIEW2->GetCapture() == GAMEAPPVIEW2) ReleaseCapture();

						// give mouse full movement
						ClipCursor(&GAMEAPPVIEW2->m_rectCursor);

						// reset mouse cursor position back to start position
						CRect rect;
						GAMEAPPVIEW2->GetWindowRect(&rect);
						SetCursorPos(rect.left + m_pParent->m_sCPoint.x+2, rect.top + m_pParent->m_sCPoint.y+2);
					
						m_pParent->SelectBall(0, ctmsgData->point);
						
						m_pParent->m_point = ctmsgData->point;
						m_pParent->m_sightlineFade = 0;
						
						if(m_pParent->m_bPracticeMode);
						else
							m_pParent->SelectBallSend(0, ctmsgData->point, m_pParent->m_windowWidth, m_pParent->m_windowHeight, m_pParent->m_selectBall);
					}
				}

				if(!m_pParent->m_cueBallSelect)
				if(!GAMEAPPVIEW2->m_bShowCursor)
				{
					GAMEAPPVIEW2->m_bShowCursor = TRUE;
					SetCursor(GAMEAPPVIEW2->m_hCursor);
					ShowCursor(GAMEAPPVIEW2->m_bShowCursor); // show cursor
				}

				///
				///
				///

				GAMEAPPVIEW2->AlertMessage("Warning! You can only strike the 'White' ball!\n\n(Use 'Left-Mouse-Button' to select the 'White' ball)");
				return;
			}

	// ---------------------------------------------
	// normal 'LineSet' here!
	// ---------------------------------------------

	// Now Give ,mouse's attention to all
	if(GAMEAPPVIEW2->GetCapture() == GAMEAPPVIEW2) ReleaseCapture();
			
	// test!!! send final cue-line pos
	GAMEAPPVIEW2->SendPlayerMouseMove(m_pParent->m_mouseX, m_pParent->m_mouseY, 
							m_pParent->m_windowWidth, m_pParent->m_windowHeight,
							m_pParent->m_strikeLine, m_pParent->m_strikeDist);

	m_pParent->m_cueBallSelect = 5; //--
	m_pParent->m_mouseX = 0; // position cuetip GFX pointer at centre of selected ball
	m_pParent->m_mouseY = 0;

	GAMEAPPVIEW2->m_sliderRaiseButt.EnableWindow(TRUE);
	m_pParent->m_bRaiseButtHelper = TRUE;

	// give mouse full movement
	ClipCursor(&GAMEAPPVIEW2->m_rectCursor);

	if(!GAMEAPPVIEW2->m_bShowCursor)
	{
		GAMEAPPVIEW2->m_bShowCursor = TRUE;
		SetCursor(GAMEAPPVIEW2->m_hCursor);
		ShowCursor(GAMEAPPVIEW2->m_bShowCursor); // show cursor
	}

	// reset mouse cursor position back to start position
	CRect rect;
	GAMEAPPVIEW2->GetWindowRect(&rect);
	SetCursorPos(rect.left + m_pParent->m_sCPoint.x+2, rect.top + m_pParent->m_sCPoint.y+2);

	m_pParent->m_bCueLineSet = TRUE;
	
	m_pParent->m_tableMousePosStrike[0] = m_pParent->m_tableMousePos[0];
	m_pParent->m_tableMousePosStrike[1] = m_pParent->m_tableMousePos[1];
	m_pParent->m_tableMousePosStrike[2] = m_pParent->m_tableMousePos[2];
}


void CCustomTable::Referee10BallPOCKETSET(DWORD msg, void *pData)
{
	ctmsg_POCKETSET_DATA *ctmsgData;
	ctmsgData = (ctmsg_POCKETSET_DATA_typ *) pData;
	
	int n, m;

	if(m_pParent->m_cueBallSelect != 5) return;	// only in state '5'(after line is set!), are we allowed to do the following

	if(m_pParent->m_colourseq == 0) return; // if this is a break-off shot, dont do the following

		
	if(m_pParent->m_bCallShot) // default
	{
		n = m_pParent->GetSelectedPocket();
		m = m_pParent->GetSelectedBall();
		// MUST BE IN FIXMODE!
/*		
		{
			char buf[256];
			sprintf(buf, "n = %d, m = %d", n, m);
			ErrorMessage(m_pParent->m_parent, buf);
		}
*/		

		if(n != -1 || m != -1)	// if player has clicked inside a pocket OR has clicked on a ball
		{
			if(n != -1) m_pParent->SetTargetPocket(n);			
			if(m != -1) m_pParent->SetTargetBall(m);

			// send target ball/pocket/success data to all at table
			GAMEAPPVIEW2->SendPlayerTargetData(m_pParent->m_targetPocket, m_pParent->m_targetBall, m_pParent->m_bTargetSuccess);
		}
	}
}


void CCustomTable::Referee10BallRESPOTBALL(int n)
{
	int m;
	int dir;
	BOOL successful;
	float f1;

	/////////////////////////////////////////////////////////////////////
	// (3) try and spot behind starting position in a direct line
	/////////////////////////////////////////////////////////////////////					
	// check to left side of spot (dir = -1)..., then the right side (dir = 1)...
	if(!TestFlag(m_pParent->m_ball[n]->m_flags, Flg_BALL_ACTIVE))
	for(dir = -1; dir <= 1; dir+=2)
	{
		CVector F = CVector(WORLD_X(591+65+120), 0, m_pParent->m_ball[n]->m_PSpot.z);
		
		// place ball on foot spot
		m_pParent->m_ball[n]->m_POld = m_pParent->m_ball[n]->m_P = F;//m_ball[n]->m_PSpot;
		m_pParent->m_ball[n]->m_W = m_pParent->m_ball[n]->m_V = m_pParent->m_ball[n]->m_VOld = CVector(0,0,0);

		do
		{
			f1 = BALL_RADIUS/20.0f;
			f1  = f1 * (float)dir;
			m_pParent->m_ball[n]->m_P.x -= f1;
			//m_ball[n]->m_P.x -= (BALL_RADIUS/20)*dir;

			successful = TRUE;
			for(m=0;m<MAX_TOTALBALLS;m++)
			if(n!=m)
			if(TestFlag(m_pParent->m_ball[m]->m_flags, Flg_BALL_ACTIVE)) // NEW: make sure this ball is active
			{
				if(!m_pParent->m_ball[n]->TryReSpot3(m_pParent->m_ball[m]))
					successful = FALSE;
			}

			// is this ball on top of a pocket
			float x = m_pParent->m_ball[n]->m_P.x;
			float y = m_pParent->m_ball[n]->m_P.y;
			for(int p=0;p<m_pocket_n;p++)
			{
				if( PocketProximity(p, CVector(x, y, 0), BALL_RADIUS) )
				{						
					successful = FALSE;
					break;
				}
			} // end for p

			if(successful) {
				m_pParent->m_ball[n]->ReSpotSuccessful();
				goto end;
			}

		}
		while(!m_pParent->m_ball[n]->CushionsProximity(FALSE)); // NEW
		//while((m_ball[n]->m_P.x >= (-1736+BALL_RADIUS)) ||
		//	  (m_ball[n]->m_P.x <= (1736-BALL_RADIUS)));   // bottom cushion + BALL_RADIUS || top cushion - BALL_RADIUS
		// *TODO: note: the above boundary check is incorrect!!!, it should be using the actual 3d world "Cushions"
	}
	/////////////////////////////////////////////////////////////////////

end:;

	#ifdef FIXEDMODE
	m_pParent->m_ball[n]->UpdateFixeds();
	#endif
}


void CCustomTable::Referee10BallSIMEND(void)
{
	int n;
	int colour;
	int no_colours;
	BOOL bWhiteInPocket = FALSE;
	BOOL bNextPlayer = FALSE;
	BOOL bCalledBallWasPotted = FALSE;

	int _spots = 0;
	//int _stripes = 0;
	int _10ball = 0;
	int _pottedspots = 0;
	//int _pottedstripes = 0;
	int _potted10ball = 0;

	int colourHit1st = 0; // points to white

	BOOL bPushOutShot = FALSE;

	m_pParent->m_bMiss = TRUE;												// assume no balls potted
	ClearFlag(m_pParent->m_foul, Flg_FOUL_NORMAL | Flg_FOUL_WHITEPOTTED | Flg_FOUL_GAMEOVER | Flg_FOUL_BALLINHAND);	// assume no foul to start with [keep some flags from previous shot]
	ClearFlag(m_pParent->m_foul, Flg_FOUL_ALERT);
	ClearFlag(m_pParent->m_foul, Flg_FOUL_NOTHITCUSHION | Flg_FOUL_NOTHITCUSHIONBREAK);
	m_pParent->m_bRefreshScores = TRUE;
	
	if(!m_pParent->m_bCallShot)
		bCalledBallWasPotted = TRUE; // just for safety, in case I forget to use -> "if(m_pParent->m_bCallShot)"

	// handle push-out stuff (2:2)
	if(TestFlag(m_pParent->m_foul, Flg_FOUL_ALERTPUSHOUT))
	{
		if(m_pParent->m_colourseq > 0)
		{
			if(TestFlag(m_pParent->m_foul, Flg_FOUL_ALERTPUSHOUT))
				ClearFlag(m_pParent->m_foul, Flg_FOUL_ALERTPUSHOUT);
			
			if(TestFlag(m_pParent->m_foul, Flg_FOUL_ALERTPUSHOUT2))
			{
				SetFlag(m_pParent->m_foul, Flg_FOUL_ALERT);
				bPushOutShot = TRUE;
			}
		}
	}
	else
		ClearFlag(m_pParent->m_foul, Flg_FOUL_ALERTPUSHOUT2);


	// just in case :)
	//if(m_colourseq)
	//	m_rednext = FALSE;

	// [freeball mode]
	//if(TestFlag(m_foul, Flg_FOUL_FREEBALL))
	//{
	//	//MessageBox(0, "0","0",MB_OK);
//		FreeBallReferee();
//		ClearFlag(m_foul, Flg_FOUL_FREEBALL);
//		goto end;
//	}

	// [default mode]

	/////////////////////////////////////////////////////////////
	// what is left?
	

	for(n=0;n<MAX_TOTALBALLS;n++)
	{
		if(TestFlag(m_pParent->m_ball[n]->m_flags, Flg_BALL_ACTIVE))
		{
			if(m_pParent->m_ball[n]->m_colour >= 1 && m_pParent->m_ball[n]->m_colour <= 9)
			{
				_spots++;
			}
			/*
			else if(m_pParent->m_ball[n]->m_colour >= 9 && m_pParent->m_ball[n]->m_colour <= 15)
			{
				_stripes++;
			}
			*/
			else if(m_pParent->m_ball[n]->m_colour == 10)
			{
				_10ball++;
			}
		}
	}

	//char buf3[80];
	//sprintf(buf3, "spots = %d; 9-ball = %d", _spots, _9ball);
	//ErrorMessage(m_pParent->m_parent, buf3);

	//DEBUG!
	//char buf3[80];
	//sprintf(buf3, "PRE: runOut = %d", m_pParent->m_break2P);
	//ErrorMessage(m_pParent->m_parent, buf3);

	/////////////////////////////////////////////////////////////
	// what was pottted?
											

	for(n=0;n<MAX_TOTALBALLS;n++)
	{
		if(TestFlag(m_pParent->m_ball[n]->m_flags, Flg_BALL_POTTED))
		{
			// was target/called ball potted?
			if(m_pParent->m_bCallShot)
			if(m_pParent->m_bTargetSuccess)
			if(m_pParent->m_targetBall == m_pParent->m_ball[n]->m_colour)
				bCalledBallWasPotted = TRUE;

			if(m_pParent->m_ball[n]->m_colour >= 1 && m_pParent->m_ball[n]->m_colour <= 9)
			{
				_pottedspots++;
			}
			/*
			else if(m_pParent->m_ball[n]->m_colour >= 9 && m_pParent->m_ball[n]->m_colour <= 15)
			{
				_pottedstripes++;
			}
			*/
			else if(m_pParent->m_ball[n]->m_colour == 10)
			{
				_potted10ball++;
			}
		}
	}

	/////////////////////////////////////////////////////////////
	// see if white ball has been potted (foul)
	for(n=0;n<MAX_TOTALBALLS;n++)
	{
		if(TestFlag(m_pParent->m_ball[n]->m_flags, Flg_BALL_POTTED))
			if(m_pParent->m_ball[n]->m_colour == WHITE)
			{
				bWhiteInPocket = TRUE;
				SetFlag(m_pParent->m_foul, Flg_FOUL_NORMAL | Flg_FOUL_WHITEPOTTED);

				if(_potted10ball != 0)
				{
					// *TODO - FOUL!!!! DO RERACK!!!!
					//SetFlag(m_pParent->m_foul, Flg_FOUL_GAMEOVER);
					
					// respot 10-ball
					for(n=0;n<MAX_TOTALBALLS;n++)
					if(TestFlag(m_pParent->m_ball[n]->m_flags, Flg_BALL_POTTED))
					if(m_pParent->m_ball[n]->m_colour == 10)
					{
						Referee10BallRESPOTBALL(n);
						break;
					}					
				}
			}
	}
	/////////////////////////////////////////////////////////////

	/////////////////////////////////////////////////////////////
	// what happens if player strikes a ball other than the white
	if(m_pParent->m_selectBall != WHITE)
	{
		SetFlag(m_pParent->m_foul, Flg_FOUL_NORMAL);

		if(_potted10ball != 0)
		{
			// *TODO - FOUL!!!! DO RERACK!!!!
			//SetFlag(m_pParent->m_foul, Flg_FOUL_GAMEOVER);
			
			// respot 10-ball
			for(n=0;n<MAX_TOTALBALLS;n++)
			if(TestFlag(m_pParent->m_ball[n]->m_flags, Flg_BALL_POTTED))
			if(m_pParent->m_ball[n]->m_colour == 10)
			{
				Referee10BallRESPOTBALL(n);
				break;
			}
		}

//		goto end;
	}
	/////////////////////////////////////////////////////////////
	
	/////////////////////////////////////////////////////////////
	// if white doesn't hit a ball at all, foul ball on!
	//if(m_colourseq < YELLOW) // bypass if on colours at the end
	if(!bPushOutShot)
	if((m_pParent->m_selectBall == WHITE) && (m_pParent->m_dominoHit1st == WHITE))
	{
			SetFlag(m_pParent->m_foul, Flg_FOUL_NORMAL);

			if(_potted10ball != 0)
			{
				// *TODO - FOUL!!!! DO RERACK!!!!
				//SetFlag(m_pParent->m_foul, Flg_FOUL_GAMEOVER);

				// respot 10-ball
				for(n=0;n<MAX_TOTALBALLS;n++)
				if(TestFlag(m_pParent->m_ball[n]->m_flags, Flg_BALL_POTTED))
				if(m_pParent->m_ball[n]->m_colour == 10)
				{
					Referee10BallRESPOTBALL(n);
					break;
				}				
			}

//			goto end;
	}
	/////////////////////////////////////////////////////////////

	///
	// [NEW] if nothing was potted, did cueball or spotball touch a cushion?
	if(m_pParent->m_colourseq > 0)
	if(!bWhiteInPocket)
	if(!bPushOutShot)
	if(_pottedspots == 0 && _potted10ball == 0)
	{
		BOOL bFailedToHitCushion = TRUE;
		
		for(n=0;n<MAX_TOTALBALLS;n++)
		{	
			if(m_pParent->m_ball[n]->m_hitCushion)
			{
				bFailedToHitCushion = FALSE; // no foul!
				break;
			}
		}

		if(bFailedToHitCushion)
		{
			SetFlag(m_pParent->m_foul, Flg_FOUL_NORMAL | Flg_FOUL_NOTHITCUSHION);
			//ErrorMessage(m_pParent->m_parent, "111");
		}
	}
	///////////////////////////////////////////////////////////////////////////
	///




	colourHit1st = m_pParent->m_ball[m_pParent->m_dominoHit1st]->m_colour;



				if(m_pParent->m_colourseq == 0)
				{
					///
					// [NEW] if nothing was potted on the break, did >= 4 numbered balls touch a cushion?					
					if(!bWhiteInPocket)
					if(!bPushOutShot)
					if(_pottedspots == 0 && _potted10ball == 0)
					{						
						int hitCushion = 0;
						
						for(n=1;n<MAX_TOTALBALLS;n++) // exclude white ball
						{	
							if(m_pParent->m_ball[n]->m_hitCushion)
							{
								hitCushion++; // (at least 4 numbered balls)
							}
						}
						
						if(hitCushion < 4)
						{
							SetFlag(m_pParent->m_foul, Flg_FOUL_NORMAL | Flg_FOUL_NOTHITCUSHIONBREAK);
						}

						//char buf[80];
						//sprintf(buf, "hitCushion = %d", hitCushion);
						//ErrorMessage(m_pParent->m_parent, buf);
					}
					///////////////////////////////////////////////////////////////////////////
					///
					
					// on the break!
					m_pParent->m_colourseq = 1;

					if(_potted10ball != 0)
					{
						if( (colourHit1st == 1) && !TestFlag(m_pParent->m_foul, Flg_FOUL_NORMAL) ) // 1-ball
						{
							// Golden break!!!!
							if(m_pParent->m_bTurn1P)
							{
								m_pParent->m_gameOver = 2;
								Msfx_smpplayvol(&GAMEAPPVIEW2->m_game_smp[SFX_GOODSHOT], 255);
							}
							else
							{
								m_pParent->m_gameOver = 1;
								Msfx_smpplayvol(&GAMEAPPVIEW2->m_game_smp[SFX_GOODSHOT], 255);
							}
						}
						else
						{
							// this player losses for now!
							//SetFlag(m_pParent->m_foul, Flg_FOUL_NORMAL | Flg_FOUL_GAMEOVER); //--
							
							SetFlag(m_pParent->m_foul, Flg_FOUL_NORMAL);

							// respot 10-ball
							for(n=0;n<MAX_TOTALBALLS;n++)
							if(TestFlag(m_pParent->m_ball[n]->m_flags, Flg_BALL_POTTED))
							if(m_pParent->m_ball[n]->m_colour == 10)
							{
								Referee10BallRESPOTBALL(n);
								break;
							}
							//goto end; //--
						}						
					}
					
					// what is the lowest value ball left on the table?
					int lowestball = 10;
					for(n=0;n<MAX_TOTALBALLS;n++)
					if(TestFlag(m_pParent->m_ball[n]->m_flags, Flg_BALL_ACTIVE))
					if(!TestFlag(m_pParent->m_ball[n]->m_flags, Flg_BALL_POTTED))
					{
						if(m_pParent->m_ball[n]->m_colour >= 1 && m_pParent->m_ball[n]->m_colour <= 10)
							if(m_pParent->m_ball[n]->m_colour < lowestball)
								lowestball = m_pParent->m_ball[n]->m_colour;			
					}

					// point to next ball to pot
					m_pParent->m_colourseq = lowestball;

					if(colourHit1st != 1) // if 1-ball wasn't hit first on the break
					{
						SetFlag(m_pParent->m_foul, Flg_FOUL_NORMAL);
						goto end;
					}

					if(TestFlag(m_pParent->m_foul, Flg_FOUL_NORMAL))
					{
						goto end;
					}

					// allow Push-Out now ( handle push-out stuff (1:2) )
					SetFlag(m_pParent->m_foul, Flg_FOUL_ALERT | Flg_FOUL_ALERTPUSHOUT);
				}
				else if(m_pParent->m_colourseq > 0)
				{
					// shots after the break
					BOOL bHitWrongBall = FALSE;
					if(colourHit1st != m_pParent->m_colourseq) // if ball-on wasn't hit first
						bHitWrongBall = TRUE;
					
					if(_potted10ball != 0)
					{
						if(bPushOutShot)
						{
							//// this player losses for now!
							//SetFlag(m_pParent->m_foul, Flg_FOUL_NORMAL | Flg_FOUL_GAMEOVER);
							//goto end;

							// respot 10-ball
							for(n=0;n<MAX_TOTALBALLS;n++)
							if(TestFlag(m_pParent->m_ball[n]->m_flags, Flg_BALL_POTTED))
							if(m_pParent->m_ball[n]->m_colour == 10)
							{
								Referee10BallRESPOTBALL(n);
								break;
							}

							goto _skipPotted10Ball;
						}

						// if this is a called shot type game
						if(m_pParent->m_bCallShot && !bCalledBallWasPotted)
						{
							SetFlag(m_pParent->m_foul, Flg_FOUL_NORMAL);

							// respot 10-ball
							for(n=0;n<MAX_TOTALBALLS;n++)
							if(TestFlag(m_pParent->m_ball[n]->m_flags, Flg_BALL_POTTED))
							if(m_pParent->m_ball[n]->m_colour == 10)
							{
								Referee10BallRESPOTBALL(n);
								break;
							}							
						}
						else
						{
							// default : old way
							if(!bHitWrongBall && !TestFlag(m_pParent->m_foul, Flg_FOUL_NORMAL) ) // ball-on (i.e. lowest ball that was on)
								//if( (colourHit1st == 1) &&  ) // 1-ball
							{							
								// Combination on the 9!!!! / Or finally potted the 9! (Check this OAP)
								if(m_pParent->m_bTurn1P)
								{
									m_pParent->m_gameOver = 2;
									
									if(_spots == 0) // if there are no spots left &...
									if(m_pParent->m_break2P == 1) // if we're on a run-out
									{
//										m_pParent->m_highBreaks1P[0] = 60; // 60 => run-out for 9-ball stat. // *TODO - 10ball runout!
										Msfx_smpplayvol(&GAMEAPPVIEW2->m_game_smp[SFX_GOODSHOT], 255);
									}
								}
								else
								{
									m_pParent->m_gameOver = 1;

									if(_spots == 0) // if there are no spots left &...
									if(m_pParent->m_break2P == 1) // if we're on a run-out
									{
//										m_pParent->m_highBreaks2P[0] = 60; // 60 => run-out for 9-ball stat. // *TODO - 10ball runout!
										Msfx_smpplayvol(&GAMEAPPVIEW2->m_game_smp[SFX_GOODSHOT], 255);
									}
								}
							}
							else
							{
								// this player losses for now!
								//SetFlag(m_pParent->m_foul, Flg_FOUL_NORMAL | Flg_FOUL_GAMEOVER); //--

								SetFlag(m_pParent->m_foul, Flg_FOUL_NORMAL);

								// respot 10-ball
								for(n=0;n<MAX_TOTALBALLS;n++)
								if(TestFlag(m_pParent->m_ball[n]->m_flags, Flg_BALL_POTTED))
								if(m_pParent->m_ball[n]->m_colour == 10)
								{
									Referee10BallRESPOTBALL(n);
									break;
								}
														
								//goto end; //--
							}
						}
					}
				_skipPotted10Ball:;
					// what is the lowest value ball left on the table?
					int lowestball = 10;
					for(n=0;n<MAX_TOTALBALLS;n++)
					if(TestFlag(m_pParent->m_ball[n]->m_flags, Flg_BALL_ACTIVE))
					if(!TestFlag(m_pParent->m_ball[n]->m_flags, Flg_BALL_POTTED))
					{
						if(m_pParent->m_ball[n]->m_colour >= 1 && m_pParent->m_ball[n]->m_colour <= 10)
							if(m_pParent->m_ball[n]->m_colour < lowestball)
								lowestball = m_pParent->m_ball[n]->m_colour;
						
// wrong place!!!! (**)
//						if(!bHitWrongBall && !TestFlag(m_pParent->m_foul, Flg_FOUL_NORMAL) )
//						if(m_pParent->m_bCallShot && !bCalledBallWasPotted)
//							SetFlag(m_pParent->m_foul, Flg_FOUL_NORMAL);
						
					}

					// point to next ball to pot
					m_pParent->m_colourseq = lowestball;

					if(!bPushOutShot)
					if(bHitWrongBall) // if ball-on wasn't hit first
					{
						ClearFlag(m_pParent->m_foul, Flg_FOUL_NOTHITCUSHION); // if this person hit the wrong ball, dont bother to tell them abt the cushion rule
						SetFlag(m_pParent->m_foul, Flg_FOUL_NORMAL);
						goto end;
					}

					// did player pot the wrong ball, instead of the called one (**)
					for(n=0;n<MAX_TOTALBALLS;n++)
					if(TestFlag(m_pParent->m_ball[n]->m_flags, Flg_BALL_POTTED))
					if(m_pParent->m_ball[n]->m_colour >= 1 && m_pParent->m_ball[n]->m_colour <= 15)
					{
						if(!bHitWrongBall && !TestFlag(m_pParent->m_foul, Flg_FOUL_NORMAL) )
						if(m_pParent->m_bCallShot && !bCalledBallWasPotted)
						{
							SetFlag(m_pParent->m_foul, Flg_FOUL_NORMAL);
							break;
						}
					}
					/////////////////////////////////////////////////////////////////

					if(TestFlag(m_pParent->m_foul, Flg_FOUL_NORMAL))
						goto end;
				}
				

					//if(m_pParent->m_bTurn1P)

	//			if(m_pParent->m_ball[n]->m_colour == WHITE)
	///			{
	//				bWhiteInPocket = TRUE;
	//				SetFlag(m_pParent->m_foul, Flg_FOUL_NORMAL | Flg_FOUL_WHITEPOTTED);
	//			}
	//		}
	//	}
	  
	/////////////////////////////////////////////////////////////
	//char buf[80];
	//sprintf(buf, "-- m_colourseq = %d ---", m_pParent->m_colourseq);
	//ErrorMessage(m_pParent->m_parent, buf);

	/////////////////////////////////////////////////////////////
	// normal pots...
	if(!bPushOutShot)
	if(!bWhiteInPocket)
	{			
		//MessageBox(0, "11","11",MB_OK);
		for(n=0;n<MAX_TOTALBALLS;n++)
		{
			if(TestFlag(m_pParent->m_ball[n]->m_flags, Flg_BALL_POTTED))
			{					
				

				if(m_pParent->m_bTurn1P) {
					//m_pParent->m_score1P += 1;//m_pParent->m_ball[n]->m_colour;
					//m_pParent->m_break1P += 1;//m_ball[n]->m_colour;
				}
				else {
					//m_pParent->m_score2P += 1;//m_pParent->m_ball[n]->m_colour;
					//m_pParent->m_break2P += 1;//m_ball[n]->m_colour;
				}
				
				m_pParent->m_bMiss = FALSE;		// potted ball

				// after first pot, go into runout mode
				if(m_pParent->m_break2P == 0)
					m_pParent->m_break2P = 1;
				
				//m_pParent->m_foul = 0;				// no foul
				ClearFlag(m_pParent->m_foul, (~(Flg_FOUL_ALERT | Flg_FOUL_ALERTPUSHOUT | Flg_FOUL_ALERTPUSHOUT2)) );

				
				int tbl = GAMEAPPVIEW2->m_lobby->GetTableIdxFromPlayer(GAMEAPPVIEW2->m_playerSockIdx);
				if(tbl!=-1)
				{
					m_pParent->m_shotTime =  m_table[tbl].shotTime*50;
				}

				/*
				if(_potted8ball >0)
				{
					// GAME_OVER! the other player losses!
					if(m_pParent->m_bTurn1P)
						m_pParent->m_gameOver = 2;
					else
						m_pParent->m_gameOver = 1;		
				}
				*/
			}
		}

		/*
		if(!m_colourseq) // if we are NOT on the 'colour seq.' at the end of the frame
		if(m_bMiss == FALSE)
		{
			if(m_rednext)
				m_rednext = FALSE;
			else
				m_rednext = TRUE;
		}
		*/
	}
	/////////////////////////////////////////////////////////////


	/////////////////////////////////////
	// *NEW: this shot was a good one, so clear the foul3times counter
	if(!TestFlag(m_pParent->m_foul, Flg_FOUL_NORMAL))
	{
		if(m_pParent->m_bTurn1P)
			m_pParent->m_foul3Times1P = 0;
		else
			m_pParent->m_foul3Times2P = 0;
	}
	/////////////////////////////////////

end:;

	/////////////////////////////////////
	// *NEW: this shot was a bad one
	if(TestFlag(m_pParent->m_foul, Flg_FOUL_NORMAL))
	{
		if(m_pParent->m_bTurn1P)
			m_pParent->m_foul3Times1P++;
		else
			m_pParent->m_foul3Times2P++;
	}

	// 3 times => gameover!
	if(m_pParent->m_foul3Times1P >= 3)
	{
		m_pParent->m_gameOver = 1;
	}
	else if(m_pParent->m_foul3Times2P >= 3)
	{
		m_pParent->m_gameOver = 2;
	}
	/////////////////////////////////////


	if(TestFlag(m_pParent->m_foul, Flg_FOUL_NORMAL))
	{
		ClearFlag(m_pParent->m_foul, Flg_FOUL_ALERT);
	}

	//char buf[80];
	//sprintf(buf, "-- m_colourseq = %d ---", m_pParent->m_colourseq);
	//ErrorMessage(m_pParent->m_parent, buf);


	// if just done break! and we fouled!
	if(m_pParent->m_colourseq == 0)
		m_pParent->m_colourseq = 1;

	if(TestFlag(m_pParent->m_foul, Flg_FOUL_GAMEOVER))
	{
		// GAME_OVER! this player losses!
		if(m_pParent->m_bTurn1P)
			m_pParent->m_gameOver = 1;
		else
			m_pParent->m_gameOver = 2;		
	}

	/////////////////////////////////////////////////////////////
	// check if nothing was potted this time
	//if(!m_pParent->m_gameOver) // NOT SURE WHY WE NEEDED THIS LINE?

	if(m_pParent->m_bMiss)
	{
		// if we've already potted a ball, but now we've missed or fouled, then NO run-out!
		if(m_pParent->m_break2P == 1)
			m_pParent->m_break2P = -1;

		//ErrorMessage(m_pParent->m_parent, "miss!");
//		// end of break sounds
//		if(g_gfxDetails.bSoundClapping)
//		{
//			if(m_bTurn1P)
//			{
//				if(m_break1P >= 20 && m_break1P < 100)
//				{
//					float loudness = 128 + 128*(((float)m_break1P - 20)/80);
//					Msfx_smpplayvol(&GAMEAPPVIEW->m_game_smp[SFX_20PLUSBREAK], loudness);
//				}
//				
//				/*
//				if(m_break1P >= 100)
//				{
//					Msfx_smpplayvol(&GAMEAPPVIEW->m_game_smp[SFX_CENTURY], 255);
//				}
//				*/
//				
//			}
//			else
//			{
//				if(m_break2P >= 20 && m_break2P < 100)
//				{
//					float loudness = 128 + 128*(((float)m_break1P - 20)/80);
//					Msfx_smpplayvol(&GAMEAPPVIEW->m_game_smp[SFX_20PLUSBREAK], loudness);
//				}
//  
//				/*
//				if(m_break2P >= 100)
//				{
//					Msfx_smpplayvol(&GAMEAPPVIEW->m_game_smp[SFX_CENTURY], 255);
//				}
//				*/
//			}
//		}

		//MessageBox(0, "13","13",MB_OK);
		if(m_pParent->m_bPracticeMode && !GAMEAPPVIEW2->m_bReplayMode && !GAMEAPPVIEW2->m_bPlayOffline)
		{
			// force no fouls in practice
			//m_foul = 0;
			//m_break1P = 0;
		}
		else
		{
			// decide who's turn it is next?
			bNextPlayer = TRUE;
			m_pParent->m_bConcededGame = FALSE; // clear this flag (now that the turn has swapped over, we can allow gameOver to continue...)
			GAMEAPPVIEW2->m_bFlashWindow = TRUE;

			//m_pParent->m_shotTime = 60*50;
			if(TestFlag(m_pParent->m_foul, Flg_FOUL_NORMAL))
				m_pParent->m_shotTime = 60*50;
			else
			{
				// if no foul, then give usual time for shot
				int tbl = GAMEAPPVIEW2->m_lobby->GetTableIdxFromPlayer(GAMEAPPVIEW2->m_playerSockIdx);
				if(tbl!=-1)
				{
					m_pParent->m_shotTime =  m_table[tbl].shotTime*50;
				}
			}

			//ErrorMessage(m_parent, "1. reg break!");

			if(m_pParent->m_bTurn1P) {
				m_pParent->m_bTurn1P = FALSE;

			}
			else {
				m_pParent->m_bTurn1P = TRUE;

			}
		}

		// if we are NOT on the 'colour seq.' at the end of the frame
		//if(!m_colourseq)
		//	m_rednext = TRUE;
	}
	/////////////////////////////////////////////////////////////	

	Referee10BallUpdatePots();

	// re-spot balls
	Referee10BallReSpot();

	//char buf33[80];
	//sprintf(buf33, "POST: runOut = %d", m_pParent->m_break2P);
	//ErrorMessage(m_pParent->m_parent, buf33);

		
		///////////
		// for practice mode : fixes "pot yellow aand white simul. bug" ***
		if(m_pParent->m_bMiss)
		{
			//MessageBox(0, "13b","13b",MB_OK);
			if(m_pParent->m_bPracticeMode && !GAMEAPPVIEW2->m_bPlayOffline)
			{
				// force no fouls in practice
				m_pParent->m_foul = 0;
				//m_pParent->m_break1P = 0;
			}
		}
		///////////

		
		if(TestFlag(m_pParent->m_foul, Flg_FOUL_NORMAL))
		{
			m_pParent->m_bBallInHand = 1;
			m_pParent->m_bStandUpFromShot = 1;
		}
}


///////////////////////////////////////////////////////////////////////////////////////////////////
//
//
void CCustomTable::Referee10BallReSpot(void)
{
	int n;
	int m;
	int nextspot;
	int dir;
	BOOL successful;
	BOOL reds;

	float f1,f2,f3;

	
	////////////////////////////////////////////////////////////////////////////////////////////////////////////
	
	//ErrorMessage(m_pParent->m_parent, "Respot:(1,2,3)");

	// for balls in pockects
	for(n=MAX_TOTALBALLS-1; n >= 0;n--)
	//if((m_ball[n]->m_colour <= BLACK && m_ball[n]->m_colour >= YELLOW) || (m_ball[n]->m_colour == WHITE))
	if((m_pParent->m_ball[n]->m_colour == WHITE)) // ***** TODO ***** -> 8 BALL
	{	
		// temp. code for WHITE ball
		if(m_pParent->m_ball[n]->m_colour == WHITE)
			//if(!TestFlag(m_pParent->m_ball[n]->m_flags, Flg_BALL_ACTIVE))
			if(TestFlag(m_pParent->m_foul, Flg_FOUL_WHITEPOTTED))
				{
					// position ball on its original starting position or...
					m_pParent->m_ball[n]->m_POld = m_pParent->m_ball[n]->m_P = m_pParent->m_ball[n]->m_PSpot;
					
					//ErrorMessage(m_pParent->m_parent, "whiteball-1");

					if( Referee10BallDropBallOnTable(WHITE, m_pParent->m_ball[n]->m_P.x, m_pParent->m_ball[n]->m_P.y ) )
					{
						//ErrorMessage(m_pParent->m_parent, "whiteball-2");
					}
					else					
					{
						//ErrorMessage(m_pParent->m_parent, "whiteball-3");
						// ...otherwise position anywhere behind the 'Head String'
						CVector Pgreen, Pyellow;
						CVector P;
						float radius;
						//Pgreen = CVector(-WORLD_X(912),-WORLD_Y(395-12), BALL_POSITIONZOFFSET);
						//Pyellow = CVector(-WORLD_X(912),-WORLD_Y(199+12), BALL_POSITIONZOFFSET);
						Pgreen = CVector(-WORLD_X(591+65+120/*912+5*/), -600, BALL_POSITIONZOFFSET);
						Pyellow = CVector(-WORLD_X(591+65+120/*912+5*/), 600, BALL_POSITIONZOFFSET);
						float w,h;
						float x,y;
						
						//[f1 = Pyellow.y - Pgreen.y;]
						f1 = Pyellow.y;
						f1 = f1 - Pgreen.y;
						
						f1 = f1 * 0.9f;
						h =	f1;
						//h =	(Pyellow.y - Pgreen.y)*0.9f;
						w = h*0.5f;
						for(x = 0.0f;x<w;x+=BALL_RADIUS)
						for(y = 0.0f;y<h;y+=BALL_RADIUS)
						{
							
							//[f1 = Pyellow.x - x;]
							f1 = Pyellow.x;
							f1 = f1 - x;
							
							//[f2 = Pyellow.y - Pgreen.y;]
							f2 = Pyellow.y;
							f2 = f2 - Pgreen.y;
							
							f2 = f2 * 0.05f;
							
							//[f3 = Pyellow.y - y;]
							f3 = Pyellow.y;
							f3 = f3 - y;
							
							f3 = f3 - f2;
							if( Referee10BallDropBallOnTable(WHITE, f1, f3) )
							//if( DropBallOnTable(WHITE, Pyellow.x - x, Pyellow.y - y - (Pyellow.y - Pgreen.y)*0.05f) )
								goto whiteSpotted;
						}
						whiteSpotted:;
					}
					
//					m_ball[n]->m_POld = m_ball[n]->m_P = m_ball[n]->m_PSpot;
					m_pParent->m_ball[n]->m_W = m_pParent->m_ball[n]->m_V = m_pParent->m_ball[n]->m_VOld = CVector(0,0,0);
	
					// turn ball on
					m_pParent->m_ball[n]->m_flags = 0;
					SetFlag(m_pParent->m_ball[n]->m_flags, Flg_BALL_ACTIVE);

					m_pParent->m_bBallInHand = 1;
					m_pParent->m_bStandUpFromShot = 1;


					continue;
				}		
	}
}

BOOL CCustomTable::Referee10BallDropBallOnTable(int n, float x, float y, BOOL bDrop)
{
	// check if ball 'n' is on top of another ball
	int m;
	for(m=0;m<MAX_TOTALBALLS;m++)
	if(m!=n)
	if(m_pParent->m_ball[m]->Proximity(x, y, 0, BALL_RADIUS*2.0f) == TRUE)
	{
		//ErrorMessage(m_pParent->m_parent, "FALSE");
		return FALSE;
	}
	
	// check for ball n inside the D
	if(!m_pParent->m_bPracticeMode) // should be !
	{
		CVector B = CVector(x,y,BALL_POSITIONZOFFSET);
		CVector Pgreen, Pbrown;
		CVector P;
		float radius;
		//Pgreen = CVector(-WORLD_X(912),-WORLD_Y(395-12), BALL_POSITIONZOFFSET);
		//Pbrown = CVector(-WORLD_X(912),-WORLD_Y(297),BALL_POSITIONZOFFSET);
		Pgreen = CVector(-WORLD_X(591+65+120/*912+5*/), -600, BALL_POSITIONZOFFSET);
		Pbrown = CVector(-WORLD_X(591+65+120/*912+5*/), 0, BALL_POSITIONZOFFSET);

		//[P = Pgreen - Pbrown;]
		P = Pgreen;
		P = P - Pbrown;

		radius = P.Magnitude2();
		
		//[P = B - Pbrown;]
		P = B;
		P = P - Pbrown;

		if(TestFlag(m_pParent->m_foul, Flg_FOUL_WHITEPOTTED) || TestFlag(m_pParent->m_foul, Flg_FOUL_NORMAL)
			|| TestFlag(m_pParent->m_foul, Flg_FOUL_BALLINHAND))
			Pbrown.x = 1235;	// give ball-in-hand access to whole table

		//if( P.Magnitude2() < radius)
		if((B.y < -Pgreen.y) && (B.y > Pgreen.y))
		{			
			if((B.x < Pbrown.x) && (B.x > -1235))
			{
				// update the drop ball's coordinates
				if(bDrop)
				{
					//m_pParent->m_ball[n]->m_P.x = x;
					//m_pParent->m_ball[n]->m_P.y = y;
					//m_pParent->m_ball[n]->AdjustOrientation();
					//m_pParent->m_ball[n]->m_POld = m_pParent->m_ball[n]->m_P;
				m_pParent->m_ball[n]->m_P.x = x;
				m_pParent->m_ball[n]->m_P.y = y;
				#ifdef FIXEDMODE
				m_pParent->m_ball[n]->m_V.x = 0;
				m_pParent->m_ball[n]->m_V.y = 0;
				m_pParent->m_ball[n]->m_V.z = 0;
				m_pParent->m_ball[n]->UpdateFixeds();
				#endif
				m_pParent->m_ball[n]->AdjustOrientation();
				m_pParent->m_ball[n]->m_POld = m_pParent->m_ball[n]->m_P;
				#ifdef FIXEDMODE
				m_pParent->m_ball[n]->UpdateFixeds();
				#endif

				}
				return TRUE;
			}
		}
		return FALSE;
	}
	
	// update the drop ball's coordinates
	if(bDrop)
	{
		//m_pParent->m_ball[n]->m_P.x = x;
		//m_pParent->m_ball[n]->m_P.y = y;
		//m_pParent->m_ball[n]->AdjustOrientation();
		//m_pParent->m_ball[n]->m_POld = m_pParent->m_ball[n]->m_P;
		//ErrorMessage(m_pParent->m_parent, "TRUE");

		m_pParent->m_ball[n]->m_P.x = x;
		m_pParent->m_ball[n]->m_P.y = y;
		#ifdef FIXEDMODE
		m_pParent->m_ball[n]->m_V.x = 0;
		m_pParent->m_ball[n]->m_V.y = 0;
		m_pParent->m_ball[n]->m_V.z = 0;
		m_pParent->m_ball[n]->UpdateFixeds();
		#endif
		m_pParent->m_ball[n]->AdjustOrientation();
		m_pParent->m_ball[n]->m_POld = m_pParent->m_ball[n]->m_P;
		#ifdef FIXEDMODE
		m_pParent->m_ball[n]->UpdateFixeds();
		#endif

	}
	return TRUE;
}

void CCustomTable::Referee10BallUpdatePots(void)
{
	int ballColour;
	float fx,fy;
	int x,y;
		
	int n;
	for(n = 0; n < 16; n++)
	{
		g_pottedSpots[n] = -1;
		g_pottedStripes[n] = -1;
	}

	// scan thru all potted balls, and see who's potted what
	int m;
	for(m=0;m<MAX_TOTALBALLS;m++)
	if(!TestFlag(m_pParent->m_ball[m]->m_flags, Flg_BALL_ACTIVE))
	{
		fx = m_pParent->m_ball[m]->m_P.x;
		fy = m_pParent->m_ball[m]->m_P.y;

		
		// what has P1 potted
		if(fx > 20001)
		{
			fx-=20000;
			fx /= 100;
			if(fx>=0 && fx<= 15)
			{
				x = fx;
				y = fy;
				if((m_pParent->m_ball[m]->m_colour >= 1) &&  (m_pParent->m_ball[m]->m_colour <= 9))
					g_pottedSpots[x] = m;
				/*
				else if((m_pParent->m_ball[m]->m_colour >= 9) &&  (m_pParent->m_ball[m]->m_colour <= 15))
					g_pottedStripes[x] = m;
				*/				
			}
		}

		// what has P2 potted
		if(fy > 20001)
		{
			fy-=20000;
			fy /= 100;
			if(fy>=0 && fy<= 15)
			{
				x = fx;
				y = fy;
				if((m_pParent->m_ball[m]->m_colour >= 1) &&  (m_pParent->m_ball[m]->m_colour <= 9))
					g_pottedSpots[y] = m;
				/*
				else if((m_pParent->m_ball[m]->m_colour >= 9) &&  (m_pParent->m_ball[m]->m_colour <= 15))
					g_pottedStripes[y] = m;
				*/
			}
		}
	}
	/*			
	fy = 0;
	ballColour = 0;

	// draw spots on left-hand side		
	for(m=0;m<16;m++)
	if(g_pottedSpots[m] != -1)
	{			
		ballColour = m_pParent->m_ball[g_pottedSpots[m]]->m_colour; 
		
		// *TODO: 
		//if(m_pParent->m_colourseq == 2) // 1P=SPOTS & 2P=STRIPES
		//	draw2DBall(ballColour, x = LEFT, y = fy);
		//else if(m_pParent->m_colourseq == 3) // 1P=STRIPES & 2P=SPOTS
		//	draw2DBall(ballColour, x = RIGHT, y = fy);

		fy += 32;
	}

	char bbb[80];
	sprintf(bbb, "spots potted = %f", fy / 32); 
	ErrorMessage(m_pParent->m_parent, bbb);

	// draw stripes on left-hand side		
	fy = 0;
	ballColour = 0;
	for(m=0;m<16;m++)
	if(g_pottedStripes[m] != -1)
	{			
		ballColour = m_pParent->m_ball[g_pottedStripes[m]]->m_colour; 
		
		// *TODO: 
		//if(m_pParent->m_colourseq == 2) // 1P=SPOTS & 2P=STRIPES
		//	draw2DBall(ballColour, x = RIGHT, y = fy);
		//else if(m_pParent->m_colourseq == 3) // 1P=STRIPES & 2P=SPOTS
		//	draw2DBall(ballColour, x = LEFT, y = fy);

		fy += 32;
	}		
	
	sprintf(bbb, "stripes potted = %f", fy / 32); 
	ErrorMessage(m_pParent->m_parent, bbb);
	*/
}



//**

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CCustomTable::RefereeRotationBall(DWORD msg, void *pData)
{
	if(msg == CTMSG_LINESET)
	{
		RefereeRotationBallLINESET(msg, pData);
	}
	else if(msg == CTMSG_POCKETSET)
	{
		RefereeRotationBallPOCKETSET(msg, pData);
	}
	else if(msg == CTMSG_SIMEND)
	{
		Decode();
		RefereeRotationBallSIMEND();
		Encode();
	}
	else
	{
		RefereeSnooker(msg, pData);	
	}
}

void CCustomTable::RefereeRotationBallLINESET(DWORD msg, void *pData)
{
	ctmsg_LINESET_DATA *ctmsgData;
	ctmsgData = (ctmsg_LINESET_DATA_typ *) pData;


	// -----------------------------------------------------------------------------
	// check if player has not selected the white ball & we're not in Practice mode
	// -----------------------------------------------------------------------------
	//if(!m_pParent->m_rednext)
	  //if(m_pParent->m_colourseq < YELLOW)
		//if(m_pParent->m_testBall->m_colour == WHITE)
			//if(m_pParent->m_nominateBall == WHITE)
		if(!m_pParent->m_bPracticeMode)			
			if(m_pParent->m_selectBall != WHITE)
			{
				///		
				/// [simulate R-Button down for cancel shot]
				///

				m_pParent->m_bTestBallFinished = FALSE;
				//m_snooker->m_bCueTipReady = FALSE;

				// reset foucus to table (this) window, & reset 'raise butt'
				m_pParent->m_RaiseButt = m_pParent->m_RaiseButtOld = 0;
				m_pParent->m_bReceiveRaiseButtActive = FALSE;
				m_pParent->m_bRaiseButtHelper = TRUE;
				GAMEAPPVIEW2->m_sliderRaiseButt.SetPos(90);
				GAMEAPPVIEW2->PreviousWndEditSetFocus();

				GAMEAPPVIEW2->m_point = ctmsgData->point;
					
				// Only give this window the mouse's full attention 
				GAMEAPPVIEW2->SetCapture();
				
				if(m_pParent->m_tableMouseOn)
				{
					if(m_pParent->m_bCueState)
					{
						// Now Give ,mouse's attention to all
						if(GAMEAPPVIEW2->GetCapture() == GAMEAPPVIEW2) ReleaseCapture();

						// give mouse full movement
						ClipCursor(&GAMEAPPVIEW2->m_rectCursor);

						// reset mouse cursor position back to start position
						CRect rect;
						GAMEAPPVIEW2->GetWindowRect(&rect);
						SetCursorPos(rect.left + m_pParent->m_sCPoint.x+2, rect.top + m_pParent->m_sCPoint.y+2);
					
						m_pParent->SelectBall(0, ctmsgData->point);
						
						m_pParent->m_point = ctmsgData->point;
						m_pParent->m_sightlineFade = 0;
						
						if(m_pParent->m_bPracticeMode);
						else
							m_pParent->SelectBallSend(0, ctmsgData->point, m_pParent->m_windowWidth, m_pParent->m_windowHeight, m_pParent->m_selectBall);
					}
				}

				if(!m_pParent->m_cueBallSelect)
				if(!GAMEAPPVIEW2->m_bShowCursor)
				{
					GAMEAPPVIEW2->m_bShowCursor = TRUE;
					SetCursor(GAMEAPPVIEW2->m_hCursor);
					ShowCursor(GAMEAPPVIEW2->m_bShowCursor); // show cursor
				}

				///
				///
				///

				GAMEAPPVIEW2->AlertMessage("Warning! You can only strike the 'White' ball!\n\n(Use 'Left-Mouse-Button' to select the 'White' ball)");
				return;
			}

	// ---------------------------------------------
	// normal 'LineSet' here!
	// ---------------------------------------------

	// Now Give ,mouse's attention to all
	if(GAMEAPPVIEW2->GetCapture() == GAMEAPPVIEW2) ReleaseCapture();
			
	// test!!! send final cue-line pos
	GAMEAPPVIEW2->SendPlayerMouseMove(m_pParent->m_mouseX, m_pParent->m_mouseY, 
							m_pParent->m_windowWidth, m_pParent->m_windowHeight,
							m_pParent->m_strikeLine, m_pParent->m_strikeDist);

	m_pParent->m_cueBallSelect = 5; //--
	m_pParent->m_mouseX = 0; // position cuetip GFX pointer at centre of selected ball
	m_pParent->m_mouseY = 0;

	GAMEAPPVIEW2->m_sliderRaiseButt.EnableWindow(TRUE);
	m_pParent->m_bRaiseButtHelper = TRUE;

	// give mouse full movement
	ClipCursor(&GAMEAPPVIEW2->m_rectCursor);

	if(!GAMEAPPVIEW2->m_bShowCursor)
	{
		GAMEAPPVIEW2->m_bShowCursor = TRUE;
		SetCursor(GAMEAPPVIEW2->m_hCursor);
		ShowCursor(GAMEAPPVIEW2->m_bShowCursor); // show cursor
	}

	// reset mouse cursor position back to start position
	CRect rect;
	GAMEAPPVIEW2->GetWindowRect(&rect);
	SetCursorPos(rect.left + m_pParent->m_sCPoint.x+2, rect.top + m_pParent->m_sCPoint.y+2);

	m_pParent->m_bCueLineSet = TRUE;
	
	m_pParent->m_tableMousePosStrike[0] = m_pParent->m_tableMousePos[0];
	m_pParent->m_tableMousePosStrike[1] = m_pParent->m_tableMousePos[1];
	m_pParent->m_tableMousePosStrike[2] = m_pParent->m_tableMousePos[2];
}


void CCustomTable::RefereeRotationBallPOCKETSET(DWORD msg, void *pData)
{
	ctmsg_POCKETSET_DATA *ctmsgData;
	ctmsgData = (ctmsg_POCKETSET_DATA_typ *) pData;
	
	int n, m;

	if(m_pParent->m_cueBallSelect != 5) return;	// only in state '5'(after line is set!), are we allowed to do the following

	if(m_pParent->m_colourseq == 0) return; // if this is a break-off shot, dont do the following

		
	if(m_pParent->m_bCallShot) // default
	{
		n = m_pParent->GetSelectedPocket();
		m = m_pParent->GetSelectedBall();
		// MUST BE IN FIXMODE!
/*		
		{
			char buf[256];
			sprintf(buf, "n = %d, m = %d", n, m);
			ErrorMessage(m_pParent->m_parent, buf);
		}
*/		

		if(n != -1 || m != -1)	// if player has clicked inside a pocket OR has clicked on a ball
		{
			if(n != -1) m_pParent->SetTargetPocket(n);			
			if(m != -1) m_pParent->SetTargetBall(m);

			// send target ball/pocket/success data to all at table
			GAMEAPPVIEW2->SendPlayerTargetData(m_pParent->m_targetPocket, m_pParent->m_targetBall, m_pParent->m_bTargetSuccess);
		}
	}
}

void CCustomTable::RefereeRotationBallRESPOTBALL(int n)
{
	int m;
	int dir;
	BOOL successful;
	float f1;

	/////////////////////////////////////////////////////////////////////
	// (3) try and spot behind starting position in a direct line
	/////////////////////////////////////////////////////////////////////					
	// check to left side of spot (dir = -1)..., then the right side (dir = 1)...
	if(!TestFlag(m_pParent->m_ball[n]->m_flags, Flg_BALL_ACTIVE))
	for(dir = -1; dir <= 1; dir+=2)
	{
		CVector F = CVector(WORLD_X(591+65+120), 0, m_pParent->m_ball[n]->m_PSpot.z);
		
		// place ball on foot spot
		m_pParent->m_ball[n]->m_POld = m_pParent->m_ball[n]->m_P = F;//m_ball[n]->m_PSpot;
		m_pParent->m_ball[n]->m_W = m_pParent->m_ball[n]->m_V = m_pParent->m_ball[n]->m_VOld = CVector(0,0,0);

		do
		{
			f1 = BALL_RADIUS/20.0f;
			f1  = f1 * (float)dir;
			m_pParent->m_ball[n]->m_P.x -= f1;
			//m_ball[n]->m_P.x -= (BALL_RADIUS/20)*dir;

			successful = TRUE;
			for(m=0;m<MAX_TOTALBALLS;m++)
			if(n!=m)
			if(TestFlag(m_pParent->m_ball[m]->m_flags, Flg_BALL_ACTIVE)) // NEW: make sure this ball is active
			{
				if(!m_pParent->m_ball[n]->TryReSpot3(m_pParent->m_ball[m]))
					successful = FALSE;
			}

			// is this ball on top of a pocket
			float x = m_pParent->m_ball[n]->m_P.x;
			float y = m_pParent->m_ball[n]->m_P.y;
			for(int p=0;p<m_pocket_n;p++)
			{
				if( PocketProximity(p, CVector(x, y, 0), BALL_RADIUS) )
				{						
					successful = FALSE;
					break;
				}
			} // end for p

			if(successful) {
				m_pParent->m_ball[n]->ReSpotSuccessful();
				goto end;
			}

		}
		while(!m_pParent->m_ball[n]->CushionsProximity(FALSE)); // NEW
		//while((m_ball[n]->m_P.x >= (-1736+BALL_RADIUS)) ||
		//	  (m_ball[n]->m_P.x <= (1736-BALL_RADIUS)));   // bottom cushion + BALL_RADIUS || top cushion - BALL_RADIUS
		// *TODO: note: the above boundary check is incorrect!!!, it should be using the actual 3d world "Cushions"
	}
	/////////////////////////////////////////////////////////////////////

end:;

	#ifdef FIXEDMODE
	m_pParent->m_ball[n]->UpdateFixeds();
	#endif
}

void CCustomTable::RefereeRotationBallSIMEND(void)
{
	int n;
	int colour;
	int no_colours;
	BOOL bWhiteInPocket = FALSE;
	BOOL bNextPlayer = FALSE;
	BOOL bCalledBallWasPotted = FALSE;

	int _balls = 0;
	int _pottedballs = 0;

	int colourHit1st = 0; // points to white

	//BOOL bPushOutShot = FALSE;

	m_pParent->m_bMiss = TRUE;												// assume no balls potted
	ClearFlag(m_pParent->m_foul, Flg_FOUL_NORMAL | Flg_FOUL_WHITEPOTTED | Flg_FOUL_GAMEOVER | Flg_FOUL_BALLINHAND);	// assume no foul to start with [keep some flags from previous shot]
	//ClearFlag(m_pParent->m_foul, Flg_FOUL_ALERT);
	m_pParent->m_bRefreshScores = TRUE;

	if(!m_pParent->m_bCallShot)
		bCalledBallWasPotted = TRUE; // just for safety, in case I forget to use -> "if(m_pParent->m_bCallShot)"

	/*
	// handle push-out stuff (2:2)
	if(TestFlag(m_pParent->m_foul, Flg_FOUL_ALERTPUSHOUT))
	{
		if(m_pParent->m_colourseq > 0)
		{
			if(TestFlag(m_pParent->m_foul, Flg_FOUL_ALERTPUSHOUT))
				ClearFlag(m_pParent->m_foul, Flg_FOUL_ALERTPUSHOUT);
			
			if(TestFlag(m_pParent->m_foul, Flg_FOUL_ALERTPUSHOUT2))
			{
				SetFlag(m_pParent->m_foul, Flg_FOUL_ALERT);
				bPushOutShot = TRUE;
			}
		}
	}
	else
		ClearFlag(m_pParent->m_foul, Flg_FOUL_ALERTPUSHOUT2);
	*/

	// just in case :)
	//if(m_colourseq)
	//	m_rednext = FALSE;

	// [freeball mode]
	//if(TestFlag(m_foul, Flg_FOUL_FREEBALL))
	//{
	//	//MessageBox(0, "0","0",MB_OK);
//		FreeBallReferee();
//		ClearFlag(m_foul, Flg_FOUL_FREEBALL);
//		goto end;
//	}

	// [default mode]

	/////////////////////////////////////////////////////////////
	// what is left?
	

	for(n=0;n<MAX_TOTALBALLS;n++)
	{
		if(TestFlag(m_pParent->m_ball[n]->m_flags, Flg_BALL_ACTIVE))
		{
			if(m_pParent->m_ball[n]->m_colour >= 1 && m_pParent->m_ball[n]->m_colour <= 15)
			{
				_balls++;
			}
		}
	}

	//char buf3[80];
	//sprintf(buf3, "balls = %d", _balls);
	//ErrorMessage(m_pParent->m_parent, buf3);

	/////////////////////////////////////////////////////////////
	// what was pottted?
	for(n=0;n<MAX_TOTALBALLS;n++)
	{
		if(TestFlag(m_pParent->m_ball[n]->m_flags, Flg_BALL_POTTED))
		{
			// was target/called ball potted?
			if(m_pParent->m_bCallShot)
			if(m_pParent->m_bTargetSuccess)
			if(m_pParent->m_targetBall == m_pParent->m_ball[n]->m_colour)
				bCalledBallWasPotted = TRUE;

			if(m_pParent->m_ball[n]->m_colour >= 1 && m_pParent->m_ball[n]->m_colour <= 15)
			{
				_pottedballs++;
			}
		}
	}

	//char buf2[80];
	//sprintf(buf2, "balls potted = %d", _pottedballs);
	//ErrorMessage(m_pParent->m_parent, buf2);

	/////////////////////////////////////////////////////////////
	// see if white ball has been potted (foul)
	for(n=0;n<MAX_TOTALBALLS;n++)
	{
		if(TestFlag(m_pParent->m_ball[n]->m_flags, Flg_BALL_POTTED))
			if(m_pParent->m_ball[n]->m_colour == WHITE)
			{
				bWhiteInPocket = TRUE;
				SetFlag(m_pParent->m_foul, Flg_FOUL_NORMAL | Flg_FOUL_WHITEPOTTED);

				/*
				if(_potted9ball != 0)
				{
					// *TODO - FOUL!!!! DO RERACK!!!!
					SetFlag(m_pParent->m_foul, Flg_FOUL_GAMEOVER);
				}
				*/
			}
	}
	/////////////////////////////////////////////////////////////

	/////////////////////////////////////////////////////////////
	// what happens if player strikes a ball other than the white
	if(m_pParent->m_selectBall != WHITE)
	{
		SetFlag(m_pParent->m_foul, Flg_FOUL_NORMAL);

		/*
		if(_potted9ball != 0)
		{
			// *TODO - FOUL!!!! DO RERACK!!!!
			SetFlag(m_pParent->m_foul, Flg_FOUL_GAMEOVER);
		}
		*/

		//goto end;
	}
	/////////////////////////////////////////////////////////////

	
	/////////////////////////////////////////////////////////////
	// if white doesn't hit a ball at all, foul ball on!
	//if(m_colourseq < YELLOW) // bypass if on colours at the end
	//if(!bPushOutShot)
	if((m_pParent->m_selectBall == WHITE) && (m_pParent->m_dominoHit1st == WHITE))
	{
			SetFlag(m_pParent->m_foul, Flg_FOUL_NORMAL);

			/*
			if(_potted9ball != 0)
			{
				// *TODO - FOUL!!!! DO RERACK!!!!
				SetFlag(m_pParent->m_foul, Flg_FOUL_GAMEOVER);
			}
			*/

			//goto end;
	}
	/////////////////////////////////////////////////////////////


	///
	// [NEW] if nothing was potted, did cueball or spotball touch a cushion?
	if(m_pParent->m_colourseq > 0)
	if(!bWhiteInPocket)
	//if(!bPushOutShot)
	if(_pottedballs == 0) // check nothing was potted
	{
		BOOL bFailedToHitCushion = TRUE;
		
		for(n=0;n<MAX_TOTALBALLS;n++)
		{	
			if(m_pParent->m_ball[n]->m_hitCushion)
			{
				bFailedToHitCushion = FALSE; // no foul!
				break;
			}
		}

		if(bFailedToHitCushion)
		{
			SetFlag(m_pParent->m_foul, Flg_FOUL_NORMAL | Flg_FOUL_NOTHITCUSHION);
			//ErrorMessage(m_pParent->m_parent, "111");
		}
	}
	///////////////////////////////////////////////////////////////////////////
	///

	colourHit1st = m_pParent->m_ball[m_pParent->m_dominoHit1st]->m_colour;
	
	
	
				if(m_pParent->m_colourseq == 0)
				{
					// on the break!
					m_pParent->m_colourseq = 1;

					/*
					if(_potted9ball != 0)
					{
						if(colourHit1st == 1) // 1-ball
						{
							// Golden break!!!!
							if(m_pParent->m_bTurn1P)
								m_pParent->m_gameOver = 2;
							else
								m_pParent->m_gameOver = 1;
						}
						else
						{
							// this player losses for now!
							SetFlag(m_pParent->m_foul, Flg_FOUL_NORMAL | Flg_FOUL_GAMEOVER);
							goto end;
						}						
					}
					*/
					
					// what is the lowest value ball left on the table?
					int lowestball = 15;
					for(n=0;n<MAX_TOTALBALLS;n++)
					if(TestFlag(m_pParent->m_ball[n]->m_flags, Flg_BALL_ACTIVE))
					if(!TestFlag(m_pParent->m_ball[n]->m_flags, Flg_BALL_POTTED))
					{
						if(m_pParent->m_ball[n]->m_colour >= 1 && m_pParent->m_ball[n]->m_colour <= 15)
							if(m_pParent->m_ball[n]->m_colour < lowestball)
								lowestball = m_pParent->m_ball[n]->m_colour;			
					}

					// point to next ball to pot
					m_pParent->m_colourseq = lowestball;

					if(colourHit1st != 1) // if 1-ball wasn't hit first on the break
					{
						SetFlag(m_pParent->m_foul, Flg_FOUL_NORMAL);
						//goto end;
					}

					///
					// [NEW] if nothing was potted on the break, did >= 4 numbered balls touch a cushion?					
					//if(!bWhiteInPocket)
					if(_pottedballs == 0)
					{						
						int hitCushion = 0;
						
						for(n=1;n<MAX_TOTALBALLS;n++) // exclude white ball
						{	
							if(m_pParent->m_ball[n]->m_hitCushion)
							{
								hitCushion++; // (at least 4 numbered balls)
							}
						}
						
						if(hitCushion < 4)
						{
							if(!m_pParent->m_bPracticeMode || GAMEAPPVIEW2->m_bPlayOffline)// && !GAMEAPPVIEW2->m_bReplayMode)
							{
								// foul! So swap turn
								if(m_pParent->m_bTurn1P)
									m_pParent->m_bTurn1P = FALSE;
								else
									m_pParent->m_bTurn1P = TRUE;
							}

							// give current player choice of reracking
							SetFlag(m_pParent->m_foul, Flg_FOUL_NORMAL | Flg_FOUL_NOTHITCUSHIONBREAK | Flg_FOUL_ALERT);
							
							m_pParent->m_bMiss = FALSE;	// clear this, so that the 'miss' block of code below is bypassed

							goto end;
						}

						//char buf[80];
						//sprintf(buf, "hitCushion = %d", hitCushion);
						//ErrorMessage(m_pParent->m_parent, buf);
					}

					if(TestFlag(m_pParent->m_foul, Flg_FOUL_NORMAL))
					{
						goto end;
					}

					// allow Push-Out now ( handle push-out stuff (1:2) )
					//SetFlag(m_pParent->m_foul, Flg_FOUL_ALERT | Flg_FOUL_ALERTPUSHOUT);
				}
				else if(m_pParent->m_colourseq > 0)
				{
					// shots after the break
					BOOL bHitWrongBall = FALSE;
					if(colourHit1st != m_pParent->m_colourseq) // if ball-on wasn't hit first
						bHitWrongBall = TRUE;
					
					/*
					if(_potted9ball != 0)
					{
						// *
						if(bPushOutShot)
						{
							// this player losses for now!
							SetFlag(m_pParent->m_foul, Flg_FOUL_NORMAL | Flg_FOUL_GAMEOVER);
							goto end;
						}
						* //

						if(!bHitWrongBall) // ball-on (i.e. lowest ball that was on)
						{
							// Combination on the 9!!!!
							if(m_pParent->m_bTurn1P)
								m_pParent->m_gameOver = 2;
							else
								m_pParent->m_gameOver = 1;
						}
						else
						{
							// this player losses for now!
							SetFlag(m_pParent->m_foul, Flg_FOUL_NORMAL | Flg_FOUL_GAMEOVER);
							goto end;
						}						
					}
					*/
					
					// what is the lowest value ball left on the table?
					int lowestball = 15;
					for(n=0;n<MAX_TOTALBALLS;n++)
					if(TestFlag(m_pParent->m_ball[n]->m_flags, Flg_BALL_ACTIVE))
					if(!TestFlag(m_pParent->m_ball[n]->m_flags, Flg_BALL_POTTED))
					{
						if(m_pParent->m_ball[n]->m_colour >= 1 && m_pParent->m_ball[n]->m_colour <= 15)
							if(m_pParent->m_ball[n]->m_colour < lowestball)
								lowestball = m_pParent->m_ball[n]->m_colour;

// WRONG!!! (**)
//						if(!bHitWrongBall && !TestFlag(m_pParent->m_foul, Flg_FOUL_NORMAL) )
//						if(m_pParent->m_bCallShot && !bCalledBallWasPotted)
//							SetFlag(m_pParent->m_foul, Flg_FOUL_NORMAL); //// <---------------
					}

					// point to next ball to pot
					m_pParent->m_colourseq = lowestball;

					//if(!bPushOutShot)
					if(bHitWrongBall) // if ball-on wasn't hit first
					{
						ClearFlag(m_pParent->m_foul, Flg_FOUL_NOTHITCUSHION); // if this person hit thw wrong ball, dont bother to tell them abt the cushion rule						
						SetFlag(m_pParent->m_foul, Flg_FOUL_NORMAL);
						goto end;
					}

					// did player pot the wrong ball, instead of the called one (**)
					for(n=0;n<MAX_TOTALBALLS;n++)
					if(TestFlag(m_pParent->m_ball[n]->m_flags, Flg_BALL_POTTED))
					if(m_pParent->m_ball[n]->m_colour >= 1 && m_pParent->m_ball[n]->m_colour <= 15)
					{
						if(!bHitWrongBall && !TestFlag(m_pParent->m_foul, Flg_FOUL_NORMAL) )
						if(m_pParent->m_bCallShot && !bCalledBallWasPotted)
						{
							SetFlag(m_pParent->m_foul, Flg_FOUL_NORMAL);
							break;
						}
					}
					/////////////////////////////////////////////////////////////////

					if(TestFlag(m_pParent->m_foul, Flg_FOUL_NORMAL))
					{
						goto end;
					}
				}
				
	/////////////////////////////////////////////////////////////
	//char buf[80];
	//sprintf(buf, "-- m_colourseq = %d ---", m_pParent->m_colourseq);
	//ErrorMessage(m_pParent->m_parent, buf);

	/////////////////////////////////////////////////////////////
	// normal pots...
	//if(!bPushOutShot)
	if(!bWhiteInPocket)
	{			
		//MessageBox(0, "11","11",MB_OK);
		for(n=0;n<MAX_TOTALBALLS;n++)
		{
			if(TestFlag(m_pParent->m_ball[n]->m_flags, Flg_BALL_POTTED))
			{					
				

				if(m_pParent->m_bTurn1P) {
					m_pParent->m_score1P += m_pParent->m_ball[n]->m_colour;
					//m_pParent->m_break1P += 1;//m_ball[n]->m_colour;

				}
				else {
					m_pParent->m_score2P += m_pParent->m_ball[n]->m_colour;
					//m_pParent->m_break2P += 1;//m_ball[n]->m_colour;
				}
				
				

				m_pParent->m_bMiss = FALSE;		// potted ball
				
				m_pParent->m_foul = 0;				// no foul
				//ClearFlag(m_pParent->m_foul, (~(Flg_FOUL_ALERT | Flg_FOUL_ALERTPUSHOUT | Flg_FOUL_ALERTPUSHOUT2)) );

				
				int tbl = GAMEAPPVIEW2->m_lobby->GetTableIdxFromPlayer(GAMEAPPVIEW2->m_playerSockIdx);
				if(tbl!=-1)
				{
					m_pParent->m_shotTime =  m_table[tbl].shotTime*50;
				}

				/*
				if(_potted8ball >0)
				{
					// GAME_OVER! the other player losses!
					if(m_pParent->m_bTurn1P)
						m_pParent->m_gameOver = 2;
					else
						m_pParent->m_gameOver = 1;		
				}
				*/
			}
		}

		/*
		if(!m_colourseq) // if we are NOT on the 'colour seq.' at the end of the frame
		if(m_bMiss == FALSE)
		{
			if(m_rednext)
				m_rednext = FALSE;
			else
				m_rednext = TRUE;
		}
		*/
	}
	/////////////////////////////////////////////////////////////

	/////////////////////////////////////
	// *NEW: this shot was a good one, so clear the foul3times counter
	if(!TestFlag(m_pParent->m_foul, Flg_FOUL_NORMAL))
	{
		if(m_pParent->m_bTurn1P)
			m_pParent->m_foul3Times1P = 0;
		else
			m_pParent->m_foul3Times2P = 0;
	}
	/////////////////////////////////////

end:;

	/////////////////////////////////////
	// *NEW: this shot was a bad one
	if(TestFlag(m_pParent->m_foul, Flg_FOUL_NORMAL))
	{
		if(m_pParent->m_bTurn1P)
			m_pParent->m_foul3Times1P++;
		else
			m_pParent->m_foul3Times2P++;
	}

	// 3 times => gameover!
	if(m_pParent->m_foul3Times1P >= 3)
	{
		m_pParent->m_gameOver = 1;
	}
	else if(m_pParent->m_foul3Times2P >= 3)
	{
		m_pParent->m_gameOver = 2;
	}
	/////////////////////////////////////

	//char buf[80];
	//sprintf(buf, "-- m_colourseq = %d ---", m_pParent->m_colourseq);
	//ErrorMessage(m_pParent->m_parent, buf);

	// ***IF FOUL! RESPOT ANY ILLEGALLY POTTED BALLS***
	if(TestFlag(m_pParent->m_foul, Flg_FOUL_NORMAL))
	{
		for(n=0;n<MAX_TOTALBALLS;n++)
		if(TestFlag(m_pParent->m_ball[n]->m_flags, Flg_BALL_POTTED))
		if(m_pParent->m_ball[n]->m_colour >= 1 && m_pParent->m_ball[n]->m_colour <= 15)
		{
			Referee9BallRESPOTBALL(n);
		}
	
		// what is the lowest value ball left on the table?
		int lowestball = 15;
		for(n=0;n<MAX_TOTALBALLS;n++)
		if(TestFlag(m_pParent->m_ball[n]->m_flags, Flg_BALL_ACTIVE))
		if(!TestFlag(m_pParent->m_ball[n]->m_flags, Flg_BALL_POTTED))
		{
			if(m_pParent->m_ball[n]->m_colour >= 1 && m_pParent->m_ball[n]->m_colour <= 15)
				if(m_pParent->m_ball[n]->m_colour < lowestball)
					lowestball = m_pParent->m_ball[n]->m_colour;			
		}

		// point to next ball to pot
		m_pParent->m_colourseq = lowestball;
	}

	// how many balls left now?
	for(n=0;n<MAX_TOTALBALLS;n++)
	{
		if(TestFlag(m_pParent->m_ball[n]->m_flags, Flg_BALL_ACTIVE))
		{
			if(m_pParent->m_ball[n]->m_colour >= 1 && m_pParent->m_ball[n]->m_colour <= 15)
			{
				_balls++;
			}
		}
	}


	// if there are no balls left on table, except for the white-ball
	if(_balls == 0)
	{
		// END OF GAME!
		//m_bConcededGame = FALSE; // clear this flag (now that the turn has finished, we can allow gameOver to continue...)
		// ??????? ^ CHECK THIS OUT ODIN!!!

		if(m_pParent->m_score1P < m_pParent->m_score2P)
			m_pParent->m_gameOver = 1;
		else if(m_pParent->m_score2P < m_pParent->m_score1P)
			m_pParent->m_gameOver = 2;
		else
		{
			/*
			// draw game!
			if(m_pParent->m_foul == 0)
			{
				// the last ball was potted legally
				if(m_pParent->m_bTurn1P)
					m_pParent->m_gameOver = 2;
				else
					m_pParent->m_gameOver = 1;
			}
			else
			{
				// there has been a foul!
				if(m_pParent->m_bTurn1P)
					m_pParent->m_gameOver = 1;
				else
					m_pParent->m_gameOver = 2;
			}
			*/
			// draw game!
			if(m_pParent->m_bTurn1P)
				m_pParent->m_score1P++;
			else
				m_pParent->m_score2P++;
		}
	}

	if(m_pParent->m_score1P >= 61) // player 1P wins! (2P losses)
	{
		m_pParent->m_gameOver = 2;
	}
	else if(m_pParent->m_score2P >= 61) // player 2P wins! (1P losses)
	{
		m_pParent->m_gameOver = 1;
	}

	// if just done break! and we fouled!
	if(m_pParent->m_colourseq == 0)
		m_pParent->m_colourseq = 1;

	/*
	if(TestFlag(m_pParent->m_foul, Flg_FOUL_GAMEOVER))
	{
		// GAME_OVER! this player losses!
		if(m_pParent->m_bTurn1P)
			m_pParent->m_gameOver = 1;
		else
			m_pParent->m_gameOver = 2;		
	}
	*/

	/////////////////////////////////////////////////////////////
	// check if nothing was potted this time
	//if(!m_pParent->m_gameOver) // NOT SURE WHY WE NEEDED THIS LINE?
	if(m_pParent->m_bMiss)
	{
		//ErrorMessage(m_pParent->m_parent, "miss!");
//		// end of break sounds
//		if(g_gfxDetails.bSoundClapping)
//		{
//			if(m_bTurn1P)
//			{
//				if(m_break1P >= 20 && m_break1P < 100)
//				{
//					float loudness = 128 + 128*(((float)m_break1P - 20)/80);
//					Msfx_smpplayvol(&GAMEAPPVIEW->m_game_smp[SFX_20PLUSBREAK], loudness);
//				}
//				
//				/*
//				if(m_break1P >= 100)
//				{
//					Msfx_smpplayvol(&GAMEAPPVIEW->m_game_smp[SFX_CENTURY], 255);
//				}
//				*/
//				
//			}
//			else
//			{
//				if(m_break2P >= 20 && m_break2P < 100)
//				{
//					float loudness = 128 + 128*(((float)m_break1P - 20)/80);
//					Msfx_smpplayvol(&GAMEAPPVIEW->m_game_smp[SFX_20PLUSBREAK], loudness);
//				}
//  
//				/*
//				if(m_break2P >= 100)
//				{
//					Msfx_smpplayvol(&GAMEAPPVIEW->m_game_smp[SFX_CENTURY], 255);
//				}
//				*/
//			}
//		}

		//MessageBox(0, "13","13",MB_OK);
		if(m_pParent->m_bPracticeMode && !GAMEAPPVIEW2->m_bReplayMode && !GAMEAPPVIEW2->m_bPlayOffline)
		{
			// force no fouls in practice
			//m_foul = 0;
			//m_break1P = 0;
		}
		else
		{
			// decide who's turn it is next?
			bNextPlayer = TRUE;
			m_pParent->m_bConcededGame = FALSE; // clear this flag (now that the turn has swapped over, we can allow gameOver to continue...)
			GAMEAPPVIEW2->m_bFlashWindow = TRUE;

			//m_pParent->m_shotTime = 60*50;
			if(TestFlag(m_pParent->m_foul, Flg_FOUL_NORMAL))
				m_pParent->m_shotTime = 60*50;
			else
			{
				// if no foul, then give usual time for shot
				int tbl = GAMEAPPVIEW2->m_lobby->GetTableIdxFromPlayer(GAMEAPPVIEW2->m_playerSockIdx);
				if(tbl!=-1)
				{
					m_pParent->m_shotTime =  m_table[tbl].shotTime*50;
				}
			}

			//ErrorMessage(m_parent, "1. reg break!");

			if(m_pParent->m_bTurn1P) {
				m_pParent->m_bTurn1P = FALSE;
				//m_pParent->m_break2P = 0;

				//m_pParent->m_break2P = 60*50;

				/*
				// is it a high break?
				//if(m_break1P >= 50)
				{
					if(m_highBreaks1P[0] <= m_highBreaks1P[1])
					{
						if(m_break1P > m_highBreaks1P[0])
							m_highBreaks1P[0] = m_break1P;
					}
					else
					{
						if(m_break1P > m_highBreaks1P[1])
							m_highBreaks1P[1] = m_break1P;
					}
				}
				*/
			}
			else {
				m_pParent->m_bTurn1P = TRUE;
				//m_pParent->m_break1P = 0;

				//m_pParent->m_break2P = 60*50;

				/*
				// is it a high break?
				//if(m_break2P >= 50)
				{
					if(m_highBreaks2P[0] <= m_highBreaks2P[1])
					{
						if(m_break2P > m_highBreaks2P[0])
							m_highBreaks2P[0] = m_break2P;
					}
					else
					{
						if(m_break2P > m_highBreaks2P[1])
							m_highBreaks2P[1] = m_break2P;
					}
				}
				*/
			}
		}

		// if we are NOT on the 'colour seq.' at the end of the frame
		//if(!m_colourseq)
		//	m_rednext = TRUE;
	}
	/////////////////////////////////////////////////////////////

		
	// update global potted balls arrays
	RefereeRotationBallUpdatePots();

	// re-spot balls
	RefereeRotationBallReSpot();

		
		///////////
		// for practice mode : fixes "pot yellow aand white simul. bug" ***
		if(m_pParent->m_bMiss)
		{
			//MessageBox(0, "13b","13b",MB_OK);
			if(m_pParent->m_bPracticeMode && !GAMEAPPVIEW2->m_bPlayOffline)
			{
				// force no fouls in practice
				m_pParent->m_foul = 0;
				//m_pParent->m_break1P = 0;
			}
		}
		///////////

		
		if(TestFlag(m_pParent->m_foul, Flg_FOUL_NORMAL))
		{
//			m_pParent->m_bBallInHand = 1;
//			m_pParent->m_bStandUpFromShot = 1;
		}
}


///////////////////////////////////////////////////////////////////////////////////////////////////
//
//
void CCustomTable::RefereeRotationBallReSpot(void)
{
	int n;
	int m;
	int nextspot;
	int dir;
	BOOL successful;
	BOOL reds;

	float f1,f2,f3;

	
	////////////////////////////////////////////////////////////////////////////////////////////////////////////
	
	//ErrorMessage(m_pParent->m_parent, "Respot:(1,2,3)");

	// for balls in pockects
	for(n=MAX_TOTALBALLS-1; n >= 0;n--)
	//if((m_ball[n]->m_colour <= BLACK && m_ball[n]->m_colour >= YELLOW) || (m_ball[n]->m_colour == WHITE))
	if((m_pParent->m_ball[n]->m_colour == WHITE)) // ***** TODO ***** -> 8 BALL
	{	
		// temp. code for WHITE ball
		if(m_pParent->m_ball[n]->m_colour == WHITE)
			//if(!TestFlag(m_pParent->m_ball[n]->m_flags, Flg_BALL_ACTIVE))
			if(TestFlag(m_pParent->m_foul, Flg_FOUL_WHITEPOTTED))
				{
					// position ball on its original starting position or...
					m_pParent->m_ball[n]->m_POld = m_pParent->m_ball[n]->m_P = m_pParent->m_ball[n]->m_PSpot;
					
					//ErrorMessage(m_pParent->m_parent, "whiteball-1");

					if( RefereeRotationBallDropBallOnTable(WHITE, m_pParent->m_ball[n]->m_P.x, m_pParent->m_ball[n]->m_P.y ) )
					{
						//ErrorMessage(m_pParent->m_parent, "whiteball-2");
					}
					else					
					{
						//ErrorMessage(m_pParent->m_parent, "whiteball-3");
						// ...otherwise position anywhere behind the 'Head String'
						CVector Pgreen, Pyellow;
						CVector P;
						float radius;
						//Pgreen = CVector(-WORLD_X(912),-WORLD_Y(395-12), BALL_POSITIONZOFFSET);
						//Pyellow = CVector(-WORLD_X(912),-WORLD_Y(199+12), BALL_POSITIONZOFFSET);
						Pgreen = CVector(-WORLD_X(591+65+120/*912+5*/), -600, BALL_POSITIONZOFFSET);
						Pyellow = CVector(-WORLD_X(591+65+120/*912+5*/), 600, BALL_POSITIONZOFFSET);
						float w,h;
						float x,y;
						
						//[f1 = Pyellow.y - Pgreen.y;]
						f1 = Pyellow.y;
						f1 = f1 - Pgreen.y;
						
						f1 = f1 * 0.9f;
						h =	f1;
						//h =	(Pyellow.y - Pgreen.y)*0.9f;
						w = h*0.5f;
						for(x = 0.0f;x<w;x+=BALL_RADIUS)
						for(y = 0.0f;y<h;y+=BALL_RADIUS)
						{
							
							//[f1 = Pyellow.x - x;]
							f1 = Pyellow.x;
							f1 = f1 - x;
							
							//[f2 = Pyellow.y - Pgreen.y;]
							f2 = Pyellow.y;
							f2 = f2 - Pgreen.y;
							
							f2 = f2 * 0.05f;
							
							//[f3 = Pyellow.y - y;]
							f3 = Pyellow.y;
							f3 = f3 - y;
							
							f3 = f3 - f2;
							if( RefereeRotationBallDropBallOnTable(WHITE, f1, f3) )
							//if( DropBallOnTable(WHITE, Pyellow.x - x, Pyellow.y - y - (Pyellow.y - Pgreen.y)*0.05f) )
								goto whiteSpotted;
						}
						whiteSpotted:;
					}
					
//					m_ball[n]->m_POld = m_ball[n]->m_P = m_ball[n]->m_PSpot;
					m_pParent->m_ball[n]->m_W = m_pParent->m_ball[n]->m_V = m_pParent->m_ball[n]->m_VOld = CVector(0,0,0);
	
					// turn ball on
					m_pParent->m_ball[n]->m_flags = 0;
					SetFlag(m_pParent->m_ball[n]->m_flags, Flg_BALL_ACTIVE);

					m_pParent->m_bBallInHand = 1;
					m_pParent->m_bStandUpFromShot = 1;


					continue;
				}		
	}
}

BOOL CCustomTable::RefereeRotationBallDropBallOnTable(int n, float x, float y, BOOL bDrop)
{
	// check if ball 'n' is on top of another ball
	int m;
	for(m=0;m<MAX_TOTALBALLS;m++)
	if(m!=n)
	if(m_pParent->m_ball[m]->Proximity(x, y, 0, BALL_RADIUS*2.0f) == TRUE)
	{
		//ErrorMessage(m_pParent->m_parent, "FALSE");
		return FALSE;
	}
	
	// check for ball n inside the D
	if(!m_pParent->m_bPracticeMode) // should be !
	{
		CVector B = CVector(x,y,BALL_POSITIONZOFFSET);
		CVector Pgreen, Pbrown;
		CVector P;
		float radius;
		//Pgreen = CVector(-WORLD_X(912),-WORLD_Y(395-12), BALL_POSITIONZOFFSET);
		//Pbrown = CVector(-WORLD_X(912),-WORLD_Y(297),BALL_POSITIONZOFFSET);
		Pgreen = CVector(-WORLD_X(591+65+120/*912+5*/), -600, BALL_POSITIONZOFFSET);
		Pbrown = CVector(-WORLD_X(591+65+120/*912+5*/), 0, BALL_POSITIONZOFFSET);

		//[P = Pgreen - Pbrown;]
		P = Pgreen;
		P = P - Pbrown;

		radius = P.Magnitude2();
		
		//[P = B - Pbrown;]
		P = B;
		P = P - Pbrown;

		if(TestFlag(m_pParent->m_foul, Flg_FOUL_WHITEPOTTED) || TestFlag(m_pParent->m_foul, Flg_FOUL_NORMAL)
			|| TestFlag(m_pParent->m_foul, Flg_FOUL_BALLINHAND))
			Pbrown.x = 1235;	// give ball-in-hand access to whole table

		//if( P.Magnitude2() < radius)
		if((B.y < -Pgreen.y) && (B.y > Pgreen.y))
		{			
			if((B.x < Pbrown.x) && (B.x > -1235))
			{
				// update the drop ball's coordinates
				if(bDrop)
				{
					//m_pParent->m_ball[n]->m_P.x = x;
					//m_pParent->m_ball[n]->m_P.y = y;
					//m_pParent->m_ball[n]->AdjustOrientation();
					//m_pParent->m_ball[n]->m_POld = m_pParent->m_ball[n]->m_P;
				m_pParent->m_ball[n]->m_P.x = x;
				m_pParent->m_ball[n]->m_P.y = y;
				#ifdef FIXEDMODE
				m_pParent->m_ball[n]->m_V.x = 0;
				m_pParent->m_ball[n]->m_V.y = 0;
				m_pParent->m_ball[n]->m_V.z = 0;
				m_pParent->m_ball[n]->UpdateFixeds();
				#endif
				m_pParent->m_ball[n]->AdjustOrientation();
				m_pParent->m_ball[n]->m_POld = m_pParent->m_ball[n]->m_P;
				#ifdef FIXEDMODE
				m_pParent->m_ball[n]->UpdateFixeds();
				#endif
				}
				return TRUE;
			}
		}
		return FALSE;
	}
	
	// update the drop ball's coordinates
	if(bDrop)
	{
		//m_pParent->m_ball[n]->m_P.x = x;
		//m_pParent->m_ball[n]->m_P.y = y;
		//m_pParent->m_ball[n]->AdjustOrientation();
		//m_pParent->m_ball[n]->m_POld = m_pParent->m_ball[n]->m_P;
		//ErrorMessage(m_pParent->m_parent, "TRUE");
		m_pParent->m_ball[n]->m_P.x = x;
		m_pParent->m_ball[n]->m_P.y = y;
		#ifdef FIXEDMODE
		m_pParent->m_ball[n]->m_V.x = 0;
		m_pParent->m_ball[n]->m_V.y = 0;
		m_pParent->m_ball[n]->m_V.z = 0;
		m_pParent->m_ball[n]->UpdateFixeds();
		#endif
		m_pParent->m_ball[n]->AdjustOrientation();
		m_pParent->m_ball[n]->m_POld = m_pParent->m_ball[n]->m_P;
		#ifdef FIXEDMODE
		m_pParent->m_ball[n]->UpdateFixeds();
		#endif

	}
	return TRUE;
}

void CCustomTable::RefereeRotationBallUpdatePots(void)
{
	int ballColour;
	float fx,fy;
	int x,y;
		
	int n;
	for(n = 0; n < 16; n++)
	{
		g_pottedSpots[n] = -1;
		g_pottedStripes[n] = -1;
	}

	// scan thru all potted balls, and see who's potted what
	int m;
	for(m=0;m<MAX_TOTALBALLS;m++)
	if(!TestFlag(m_pParent->m_ball[m]->m_flags, Flg_BALL_ACTIVE))
	{
		// what has been potted? (for both P1 & P2)
		if(m>=0 && m<=15)
		{
			g_pottedSpots[m] = m;
		}
		
		/*
		fx = m_pParent->m_ball[m]->m_P.x;
		fy = m_pParent->m_ball[m]->m_P.y;

		
		// what has P1 potted
		if(fx > 20001)
		{
			fx-=20000;
			fx /= 100;
			if(fx>=0 && fx<= 15)
			{
				x = fx;
				y = fy;
				if((m_pParent->m_ball[m]->m_colour >= 1) &&  (m_pParent->m_ball[m]->m_colour <= 8))
					g_pottedSpots[x] = m;				
				else if((m_pParent->m_ball[m]->m_colour >= 9) &&  (m_pParent->m_ball[m]->m_colour <= 15))
					g_pottedStripes[x] = m;								
			}
		}

		// what has P2 potted
		if(fy > 20001)
		{
			fy-=20000;
			fy /= 100;
			if(fy>=0 && fy<= 15)
			{
				x = fx;
				y = fy;
				if((m_pParent->m_ball[m]->m_colour >= 1) &&  (m_pParent->m_ball[m]->m_colour <= 8))
					g_pottedSpots[y] = m;				
				else if((m_pParent->m_ball[m]->m_colour >= 9) &&  (m_pParent->m_ball[m]->m_colour <= 15))
					g_pottedStripes[y] = m;				
			}
		}
		*/
	}
	
}

//////
//////

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////



// UnMuffle the Score, to to Memory Hacks //
void CCustomTable::Decode(void)
{
	if(m_pParent->m_encoded == TRUE)
		{
		m_pParent->m_encoded = FALSE;
		m_pParent->m_score1P -= 12;
		m_pParent->m_score2P -= 12;
		m_pParent->m_break1P -= 12;
		m_pParent->m_break2P -= 12;
		m_pParent->m_highBreaks1P[0] -= 12;
		m_pParent->m_highBreaks1P[1] -= 12;
		m_pParent->m_highBreaks2P[0] -= 12;
		m_pParent->m_highBreaks2P[1] -= 12;
		m_pParent->m_bestScore -= 12;
		//m_pParent->m_currentBreak -= 12;
		
		}
}


//m_snooker->m_break1P
// set the bit flag, and add something.. constant.

// Change the Score, to stop Memory Hacks //
void CCustomTable::Encode(char type)
{
	//Encode Mask - 16th Bit is turned on //
	//1000 0000 0000 0000
	if(m_pParent->m_encoded == FALSE)
		{
		m_pParent->m_encoded = TRUE;
		m_pParent->m_score1P += 12;
		m_pParent->m_score2P += 12;
		m_pParent->m_break1P += 12;
		m_pParent->m_break2P += 12;
		m_pParent->m_highBreaks1P[0] += 12;
		m_pParent->m_highBreaks1P[1] += 12;
		m_pParent->m_highBreaks2P[0] += 12;
		m_pParent->m_highBreaks2P[1] += 12;
		m_pParent->m_bestScore += 12;
		//m_pParent->m_currentBreak += 12;
		}
}


void CCustomTable::RefereeBreak(DWORD msg, void *pData)
{
	if(msg == CTMSG_LINESET)
	{
		RefereeBreakLINESET(msg, pData);
	}
	else if(msg == CTMSG_POCKETSET)
	{
		RefereeBreakPOCKETSET(msg, pData);
	}
	else if(msg == CTMSG_SIMEND)
	{
		Decode();
		RefereeBreakSIMEND();
		Encode();
		
	}
	else
	{
		RefereeSnooker(msg, pData);	
	}
}

void CCustomTable::RefereeBreakLINESET(DWORD msg, void *pData)
{
	ctmsg_LINESET_DATA *ctmsgData;
	ctmsgData = (ctmsg_LINESET_DATA_typ *) pData;


	// -----------------------------------------------------------------------------
	// check if player has not selected the white ball & we're not in Practice mode
	// -----------------------------------------------------------------------------
	//if(!m_pParent->m_rednext)
	  //if(m_pParent->m_colourseq < YELLOW)
		//if(m_pParent->m_testBall->m_colour == WHITE)
			//if(m_pParent->m_nominateBall == WHITE)
		if(!m_pParent->m_bPracticeMode)			
			if(m_pParent->m_selectBall != WHITE)
			{
				///		
				/// [simulate R-Button down for cancel shot]
				///

				m_pParent->m_bTestBallFinished = FALSE;
				//m_snooker->m_bCueTipReady = FALSE;

				// reset foucus to table (this) window, & reset 'raise butt'
				m_pParent->m_RaiseButt = m_pParent->m_RaiseButtOld = 0;
				m_pParent->m_bReceiveRaiseButtActive = FALSE;
				m_pParent->m_bRaiseButtHelper = TRUE;
				GAMEAPPVIEW2->m_sliderRaiseButt.SetPos(90);
				GAMEAPPVIEW2->PreviousWndEditSetFocus();

				GAMEAPPVIEW2->m_point = ctmsgData->point;
					
				// Only give this window the mouse's full attention 
				GAMEAPPVIEW2->SetCapture();
				
				if(m_pParent->m_tableMouseOn)
				{
					if(m_pParent->m_bCueState)
					{
						// Now Give ,mouse's attention to all
						if(GAMEAPPVIEW2->GetCapture() == GAMEAPPVIEW2) ReleaseCapture();

						// give mouse full movement
						ClipCursor(&GAMEAPPVIEW2->m_rectCursor);

						// reset mouse cursor position back to start position
						CRect rect;
						GAMEAPPVIEW2->GetWindowRect(&rect);
						SetCursorPos(rect.left + m_pParent->m_sCPoint.x+2, rect.top + m_pParent->m_sCPoint.y+2);
					
						m_pParent->SelectBall(0, ctmsgData->point);
						
						m_pParent->m_point = ctmsgData->point;
						m_pParent->m_sightlineFade = 0;
						
						if(m_pParent->m_bPracticeMode);
						else
							m_pParent->SelectBallSend(0, ctmsgData->point, m_pParent->m_windowWidth, m_pParent->m_windowHeight, m_pParent->m_selectBall);
					}
				}

				if(!m_pParent->m_cueBallSelect)
				if(!GAMEAPPVIEW2->m_bShowCursor)
				{
					GAMEAPPVIEW2->m_bShowCursor = TRUE;
					SetCursor(GAMEAPPVIEW2->m_hCursor);
					ShowCursor(GAMEAPPVIEW2->m_bShowCursor); // show cursor
				}

				///
				///
				///

				GAMEAPPVIEW2->AlertMessage("Warning! You can only strike the 'White' ball!\n\n(Use 'Left-Mouse-Button' to select the 'White' ball)");
				return;
			}

	// ---------------------------------------------
	// normal 'LineSet' here!
	// ---------------------------------------------

	// Now Give ,mouse's attention to all
	if(GAMEAPPVIEW2->GetCapture() == GAMEAPPVIEW2) ReleaseCapture();
			

	// test!!! send final cue-line pos
	GAMEAPPVIEW2->SendPlayerMouseMove(m_pParent->m_mouseX, m_pParent->m_mouseY, 
							m_pParent->m_windowWidth, m_pParent->m_windowHeight,
							m_pParent->m_strikeLine, m_pParent->m_strikeDist);

	m_pParent->m_cueBallSelect = 5; //--
	m_pParent->m_mouseX = 0; // position cuetip GFX pointer at centre of selected ball
	m_pParent->m_mouseY = 0;

	GAMEAPPVIEW2->m_sliderRaiseButt.EnableWindow(TRUE);
	m_pParent->m_bRaiseButtHelper = TRUE;

	// give mouse full movement
	ClipCursor(&GAMEAPPVIEW2->m_rectCursor);

	if(!GAMEAPPVIEW2->m_bShowCursor)
	{
		GAMEAPPVIEW2->m_bShowCursor = TRUE;
		SetCursor(GAMEAPPVIEW2->m_hCursor);
		ShowCursor(GAMEAPPVIEW2->m_bShowCursor); // show cursor
	}

	// reset mouse cursor position back to start position
	CRect rect;
	GAMEAPPVIEW2->GetWindowRect(&rect);
	SetCursorPos(rect.left + m_pParent->m_sCPoint.x+2, rect.top + m_pParent->m_sCPoint.y+2);


	m_pParent->m_bCueLineSet = TRUE;
	
	m_pParent->m_tableMousePosStrike[0] = m_pParent->m_tableMousePos[0];
	m_pParent->m_tableMousePosStrike[1] = m_pParent->m_tableMousePos[1];
	m_pParent->m_tableMousePosStrike[2] = m_pParent->m_tableMousePos[2];
}


void CCustomTable::RefereeBreakPOCKETSET(DWORD msg, void *pData)
{
	ctmsg_POCKETSET_DATA *ctmsgData;
	ctmsgData = (ctmsg_POCKETSET_DATA_typ *) pData;
	
	int n, m;

	if(m_pParent->m_cueBallSelect != 5) return;	// only in state '5'(after line is set!), are we allowed to do the following

	if(m_pParent->m_colourseq == 0) return; // if this is a break-off shot, dont do the following

		
	if(m_pParent->m_bCallShot) // default
	{
		n = m_pParent->GetSelectedPocket();
		m = m_pParent->GetSelectedBall();
		// MUST BE IN FIXMODE!
/*		
		{
			char buf[256];
			sprintf(buf, "n = %d, m = %d", n, m);
			ErrorMessage(m_pParent->m_parent, buf);
		}
*/		

		if(n != -1 || m != -1)	// if player has clicked inside a pocket OR has clicked on a ball
		{
			if(n != -1) m_pParent->SetTargetPocket(n);			
			if(m != -1) m_pParent->SetTargetBall(m);

			// send target ball/pocket/success data to all at table
			GAMEAPPVIEW2->SendPlayerTargetData(m_pParent->m_targetPocket, m_pParent->m_targetBall, m_pParent->m_bTargetSuccess);
		}
	}
}

void CCustomTable::RefereeBreakRESPOTBALL(int n)
{
	int m;
	int dir;
	BOOL successful;
	float f1;

	/////////////////////////////////////////////////////////////////////
	// (3) try and spot behind starting position in a direct line
	/////////////////////////////////////////////////////////////////////					
	// check to left side of spot (dir = -1)..., then the right side (dir = 1)...
	if(!TestFlag(m_pParent->m_ball[n]->m_flags, Flg_BALL_ACTIVE))
	for(dir = -1; dir <= 1; dir+=2)
	{
		CVector F = CVector(WORLD_X(591+65+120), 0, m_pParent->m_ball[n]->m_PSpot.z);
		
		// place ball on foot spot
		m_pParent->m_ball[n]->m_POld = m_pParent->m_ball[n]->m_P = F;//m_ball[n]->m_PSpot;
		m_pParent->m_ball[n]->m_W = m_pParent->m_ball[n]->m_V = m_pParent->m_ball[n]->m_VOld = CVector(0,0,0);

		do
		{
			f1 = BALL_RADIUS/20.0f;
			f1  = f1 * (float)dir;
			m_pParent->m_ball[n]->m_P.x -= f1;
			//m_ball[n]->m_P.x -= (BALL_RADIUS/20)*dir;

			successful = TRUE;
			for(m=0;m<MAX_TOTALBALLS;m++)
			if(n!=m)
			if(TestFlag(m_pParent->m_ball[m]->m_flags, Flg_BALL_ACTIVE)) // NEW: make sure this ball is active
			{
				if(!m_pParent->m_ball[n]->TryReSpot3(m_pParent->m_ball[m]))
					successful = FALSE;
			}

			// is this ball on top of a pocket
			float x = m_pParent->m_ball[n]->m_P.x;
			float y = m_pParent->m_ball[n]->m_P.y;
			for(int p=0;p<m_pocket_n;p++)
			{
				if( PocketProximity(p, CVector(x, y, 0), BALL_RADIUS) )
				{						
					successful = FALSE;
					break;
				}
			} // end for p

			if(successful) {
				m_pParent->m_ball[n]->ReSpotSuccessful();
				goto end;
			}

		}
		while(!m_pParent->m_ball[n]->CushionsProximity(FALSE)); // NEW
		//while((m_ball[n]->m_P.x >= (-1736+BALL_RADIUS)) ||
		//	  (m_ball[n]->m_P.x <= (1736-BALL_RADIUS)));   // bottom cushion + BALL_RADIUS || top cushion - BALL_RADIUS
		// *TODO: note: the above boundary check is incorrect!!!, it should be using the actual 3d world "Cushions"
	}
	/////////////////////////////////////////////////////////////////////

end:;

	#ifdef FIXEDMODE
	m_pParent->m_ball[n]->UpdateFixeds();
	#endif
}

void CCustomTable::RefereeBreakSIMEND(void)
{
	
	int n;
	int colour;
	int no_colours;
	BOOL bWhiteInPocket = FALSE;
	BOOL bNextPlayer = FALSE;
	BOOL bCalledBallWasPotted = FALSE;

	int _balls = 0;
	int _pottedballs = 0;

	int colourHit1st = 0; // points to white

	//BOOL bPushOutShot = FALSE;

	m_pParent->m_bMiss = TRUE;												// assume no balls potted
	ClearFlag(m_pParent->m_foul, Flg_FOUL_NORMAL | Flg_FOUL_WHITEPOTTED | Flg_FOUL_GAMEOVER | Flg_FOUL_BALLINHAND);	// assume no foul to start with [keep some flags from previous shot]
	//ClearFlag(m_pParent->m_foul, Flg_FOUL_ALERT);
	m_pParent->m_bRefreshScores = TRUE;

	if(!m_pParent->m_bCallShot)	bCalledBallWasPotted = TRUE; // just for safety, in case I forget to use -> "if(m_pParent->m_bCallShot)"


	for(n=0;n<MAX_TOTALBALLS;n++)
	if(TestFlag(m_pParent->m_ball[n]->m_flags, Flg_BALL_ACTIVE))
	if(m_pParent->m_ball[n]->m_colour >= 1 && m_pParent->m_ball[n]->m_colour <= 13) _balls++;
			

	/////////////////////////////////////////////////////////////
	// what was pottted?
	for(n=0;n<MAX_TOTALBALLS;n++)
	if(TestFlag(m_pParent->m_ball[n]->m_flags, Flg_BALL_POTTED))
		{
		// was target/called ball potted?
		if(m_pParent->m_bCallShot)
		if(m_pParent->m_bTargetSuccess)
		if(m_pParent->m_targetBall == m_pParent->m_ball[n]->m_colour)
			bCalledBallWasPotted = TRUE;
		if(m_pParent->m_ball[n]->m_colour >= 1 && m_pParent->m_ball[n]->m_colour <= 13) _pottedballs++;
		}

	/////////////////////////////////////////////////////////////
	// see if white ball has been potted (foul)
	for(n=0;n<MAX_TOTALBALLS;n++)
	if(TestFlag(m_pParent->m_ball[n]->m_flags, Flg_BALL_POTTED))
	if(m_pParent->m_ball[n]->m_colour == WHITE)
		{
		bWhiteInPocket = TRUE;
		SetFlag(m_pParent->m_foul, Flg_FOUL_NORMAL | Flg_FOUL_WHITEPOTTED);
		}
	/////////////////////////////////////////////////////////////

	/////////////////////////////////////////////////////////////
	// what happens if player strikes a ball other than the white
	if(m_pParent->m_selectBall != WHITE) SetFlag(m_pParent->m_foul, Flg_FOUL_NORMAL);
	/////////////////////////////////////////////////////////////

	
	/////////////////////////////////////////////////////////////
	// if white doesn't hit a ball at all, foul ball on!
	//if(m_colourseq < YELLOW) // bypass if on colours at the end
	//if(!bPushOutShot)
	if((m_pParent->m_selectBall == WHITE) && (m_pParent->m_dominoHit1st == WHITE)) SetFlag(m_pParent->m_foul, Flg_FOUL_NORMAL);
		
	/////////////////////////////////////////////////////////////




	colourHit1st = m_pParent->m_ball[m_pParent->m_dominoHit1st]->m_colour;
	
	
	// 
	BOOL Legal_Pot = TRUE;

	if(m_pParent->m_colourseq == 0)
		{
		// on the break!
		m_pParent->m_colourseq = 1;

		// If Potted a Black, Blue or Green (then Foul)
		if(TestFlag(m_pParent->m_ball[11]->m_flags, Flg_BALL_POTTED)) Legal_Pot = FALSE;
		if(TestFlag(m_pParent->m_ball[12]->m_flags, Flg_BALL_POTTED)) Legal_Pot = FALSE;
		if(TestFlag(m_pParent->m_ball[13]->m_flags, Flg_BALL_POTTED)) Legal_Pot = FALSE;

	
		// If Legal Pot on Red, Point to Colour for Next Ball
		if(_pottedballs) // Make Sure something was potted 
			{
			if(Legal_Pot) m_pParent->m_colourseq = 15; // Special, Means Any Colour, Black, Blue or Brown //
				else 
					{
					m_pParent->m_colourseq = 1; // Pot a Red Next 
					ClearFlag(m_pParent->m_foul, Flg_FOUL_NOTHITCUSHION); // if this person hit thw wrong ball, dont bother to tell them abt the cushion rule						
					SetFlag(m_pParent->m_foul, Flg_FOUL_NORMAL);
					goto end;
					}
			}
		


		if(TestFlag(m_pParent->m_foul, Flg_FOUL_NORMAL))
			{
			goto end;
			}
		}


	else if(m_pParent->m_colourseq > 0)
		{
		// shots after the break
		BOOL bHitWrongBall = FALSE;
		if(m_pParent->m_colourseq==1)
			{
			// if ball-on wasn't hit first
			if(!(colourHit1st>=1 && colourHit1st<=10)) bHitWrongBall = TRUE;
			}
		else if(m_pParent->m_colourseq==11) // Green
			{
			if(colourHit1st!=11) bHitWrongBall = TRUE;
			}	
		else if(m_pParent->m_colourseq==12) // Blue
			{
			if(colourHit1st!=12) bHitWrongBall = TRUE;
			}	
		else if(m_pParent->m_colourseq==13) // Black
			{
			if(colourHit1st!=13) bHitWrongBall = TRUE;
			}	
		else{  // Any Colour //
			// if ball-on wasn't hit first
			if(colourHit1st<10) bHitWrongBall = TRUE;
			}

		// Check that First Ball Hit = Ball Potted (if Colours) //
		// For Reds, we just need a red to be hit first, if a red is potted //
		if(!bWhiteInPocket)
		if(m_pParent->m_foul==0)
		for(n=0;n<MAX_TOTALBALLS;n++)
		if(TestFlag(m_pParent->m_ball[n]->m_flags, Flg_BALL_POTTED))
			{
			if(n>=1 && n<=10) if(colourHit1st > 10) bHitWrongBall = TRUE;
			if(n==11) if(colourHit1st != 11) bHitWrongBall = TRUE;
			if(n==12) if(colourHit1st != 12) bHitWrongBall = TRUE;
			if(n==13) if(colourHit1st != 13) bHitWrongBall = TRUE;
			}

		
		//if(!bPushOutShot)
		if(bHitWrongBall) // if ball-on wasn't hit first
		{
			ClearFlag(m_pParent->m_foul, Flg_FOUL_NOTHITCUSHION); // if this person hit thw wrong ball, dont bother to tell them abt the cushion rule						
			SetFlag(m_pParent->m_foul, Flg_FOUL_NORMAL);
			goto end;
		}

		/////////////////////////////////////////////////////////////////
		if(TestFlag(m_pParent->m_foul, Flg_FOUL_NORMAL)) goto end;
		
	}






				
	// Normal Pots //
	if(!bWhiteInPocket)
	if(m_pParent->m_foul==0)
	for(n=0;n<MAX_TOTALBALLS;n++)
	if(TestFlag(m_pParent->m_ball[n]->m_flags, Flg_BALL_POTTED))
		{
		int pts=0;
		if(m_pParent->m_ball[n]->m_colour>=1 && m_pParent->m_ball[n]->m_colour<=10) pts=1;
			else if(m_pParent->m_ball[n]->m_colour==11) pts=3;
			else if(m_pParent->m_ball[n]->m_colour==12) pts=5;
			else pts=7;

		if(m_pParent->m_bTurn1P) m_pParent->m_score1P += pts, m_pParent->m_highBreaks1P[0]+=pts;
			else m_pParent->m_score2P += pts, m_pParent->m_highBreaks2P[0]+=pts;
		
		m_pParent->m_bMiss = FALSE;		// potted ball
		m_pParent->m_foul = 0;				// no foul
		int tbl = GAMEAPPVIEW2->m_lobby->GetTableIdxFromPlayer(GAMEAPPVIEW2->m_playerSockIdx);
		if(tbl!=-1)	m_pParent->m_shotTime =  m_table[tbl].shotTime*50;
		}
		
	/////////////////////////////////////////////////////////////

	/////////////////////////////////////
	// *NEW: this shot was a good one, so clear the foul3times counter
	if(!TestFlag(m_pParent->m_foul, Flg_FOUL_NORMAL))
		{
		if(m_pParent->m_bTurn1P) m_pParent->m_foul3Times1P = 0;
		else m_pParent->m_foul3Times2P = 0;
		}
	/////////////////////////////////////

end:;

	////////////////////////////m_pParent->m_lifests/////////
	// *NEW: this shot was a bad one
	if(TestFlag(m_pParent->m_foul, Flg_FOUL_NORMAL))
		{
		if(m_pParent->m_bTurn1P) m_pParent->m_foul3Times1P++;
			else m_pParent->m_foul3Times2P++;
		}

	// 3 times => gameover!
	if(m_pParent->m_foul3Times1P >= 3) m_pParent->m_gameOver = 1;
	else if(m_pParent->m_foul3Times2P >= 3) m_pParent->m_gameOver = 2;
	


	if(m_pParent->m_colourseq == 0 || m_pParent->m_colourseq == 1 || m_pParent->m_colourseq == 15)
		{  
		// ReSpot Colours Normally when not at End Stage (i.e. m_colourseq == 0 || m_colourseq == 1 || m_colourseq == 15
		if(TestFlag(m_pParent->m_ball[11]->m_flags, Flg_BALL_POTTED)) Referee9BallRESPOTBALL(11); // Green 
		if(TestFlag(m_pParent->m_ball[12]->m_flags, Flg_BALL_POTTED)) Referee9BallRESPOTBALL(12); // Blue
		if(TestFlag(m_pParent->m_ball[13]->m_flags, Flg_BALL_POTTED)) Referee9BallRESPOTBALL(13); // Black
		}
	else{
		// Only ReSpot when at End Stage if a Foul //
		if(TestFlag(m_pParent->m_foul, Flg_FOUL_NORMAL))
			{
			if(m_pParent->m_colourseq == 11) Referee9BallRESPOTBALL(11), Referee9BallRESPOTBALL(12), Referee9BallRESPOTBALL(13); // Make Sure all Colours are ReSpotted
			else if(m_pParent->m_colourseq == 12) Referee9BallRESPOTBALL(12), Referee9BallRESPOTBALL(13); // Make Sure Blue and Black are ReSpotted
			else if(m_pParent->m_colourseq == 13) Referee9BallRESPOTBALL(13); // Make Sure Black is ReSpotted
			}
		}




	// how many balls left now?
	_balls = 0;
	for(n=0;n<MAX_TOTALBALLS;n++)
	if(TestFlag(m_pParent->m_ball[n]->m_flags, Flg_BALL_ACTIVE))
	if(m_pParent->m_ball[n]->m_colour >= 1 && m_pParent->m_ball[n]->m_colour <= 13)	_balls++;



	// if there are no balls left on table, except for the white-ball
	if(_balls == 0)
		{
		// END OF GAME!
		if(m_pParent->m_score1P < m_pParent->m_score2P)	m_pParent->m_gameOver = 1;
		else if(m_pParent->m_score2P < m_pParent->m_score1P) m_pParent->m_gameOver = 2;
		else Referee9BallRESPOTBALL(13); // ReSpot the Black
		}

	// if just done break! and we fouled!
	if(m_pParent->m_colourseq == 0) m_pParent->m_colourseq = 1;

	

	// how many reds left now?
	int _rballs = 0;
	for(n=0;n<MAX_TOTALBALLS;n++)
	if(TestFlag(m_pParent->m_ball[n]->m_flags, Flg_BALL_ACTIVE))
	if(m_pParent->m_ball[n]->m_colour >= 1 && m_pParent->m_ball[n]->m_colour <= 10)	_rballs++;


	// if just done break! and we fouled!
	if(TestFlag(m_pParent->m_foul, Flg_FOUL_NORMAL)) if(_rballs) m_pParent->m_colourseq = 1;

	
	/////////////////////////////////////////////////////////////
	// check if nothing was potted this time
	if(m_pParent->m_bMiss)
		{
		// Only Set Next Ball to Red, if there's reds on the Table, after a foul, otherwise leave the sequence alone
		if(_rballs) m_pParent->m_colourseq = 1;
			else if(m_pParent->m_colourseq == 1 || m_pParent->m_colourseq == 15) m_pParent->m_colourseq = 11;
			

		if(m_pParent->m_bPracticeMode && !GAMEAPPVIEW2->m_bReplayMode && !GAMEAPPVIEW2->m_bPlayOffline)
			{
			// no fouls in practice
			m_pParent->m_highBreaks1P[0] = 0;
			//m_pParent->m_colourseq = 1;
			}
		else
			{
			// decide who's turn it is next?
			bNextPlayer = TRUE;
			m_pParent->m_bConcededGame = FALSE; // clear this flag (now that the turn has swapped over, we can allow gameOver to continue...)
			GAMEAPPVIEW2->m_bFlashWindow = TRUE;
			//m_pParent->m_colourseq = 1;

			
			if(TestFlag(m_pParent->m_foul, Flg_FOUL_NORMAL)) m_pParent->m_shotTime = 60*50;
			else
				{
				// if no foul, then give usual time for shot
				int tbl = GAMEAPPVIEW2->m_lobby->GetTableIdxFromPlayer(GAMEAPPVIEW2->m_playerSockIdx);
				if(tbl!=-1)	m_pParent->m_shotTime =  m_table[tbl].shotTime*50;
				}

			if(m_pParent->m_bTurn1P) m_pParent->m_bTurn1P = FALSE, m_pParent->m_highBreaks2P[0] = 0;
				else m_pParent->m_bTurn1P = TRUE, m_pParent->m_highBreaks1P[0] = 0;

			}
		}
	else{
		if(_rballs) 
			{
			if(m_pParent->m_colourseq == 1) m_pParent->m_colourseq = 15;
			else if(m_pParent->m_colourseq == 15) m_pParent->m_colourseq = 1;
			}
		else{
			if(m_pParent->m_colourseq == 1) m_pParent->m_colourseq = 15;
			else if(m_pParent->m_colourseq == 15) m_pParent->m_colourseq = 11;
			else if(m_pParent->m_colourseq == 11) m_pParent->m_colourseq = 12;
			else if(m_pParent->m_colourseq == 12) m_pParent->m_colourseq = 13;		
			}
		}


	/////////////////////////////////////////////////////////////
		
	// update global potted balls arrays
	RefereeBreakUpdatePots();

	// re-spot balls
	RefereeBreakReSpot();

	///////////
	// for practice mode : fixes "pot yellow aand white simul. bug" ***
	if(m_pParent->m_bMiss)
	if(m_pParent->m_bPracticeMode && !GAMEAPPVIEW2->m_bPlayOffline)	m_pParent->m_foul = 0;
		
}


///////////////////////////////////////////////////////////////////////////////////////////////////
//
//
void CCustomTable::RefereeBreakReSpot(void)
{
	int n;
	int m;
	int nextspot;
	int dir;
	BOOL successful;
	BOOL reds;

	float f1,f2,f3;

	
	////////////////////////////////////////////////////////////////////////////////////////////////////////////
	
	//ErrorMessage(m_pParent->m_parent, "Respot:(1,2,3)");

	// for balls in pockects
	for(n=MAX_TOTALBALLS-1; n >= 0;n--)
	//if((m_ball[n]->m_colour <= BLACK && m_ball[n]->m_colour >= YELLOW) || (m_ball[n]->m_colour == WHITE))
	if((m_pParent->m_ball[n]->m_colour == WHITE)) // ***** TODO ***** -> 8 BALL
	{	
		// temp. code for WHITE ball
		if(m_pParent->m_ball[n]->m_colour == WHITE)
			//if(!TestFlag(m_pParent->m_ball[n]->m_flags, Flg_BALL_ACTIVE))
			if(TestFlag(m_pParent->m_foul, Flg_FOUL_WHITEPOTTED))
				{
					// position ball on its original starting position or...
					m_pParent->m_ball[n]->m_POld = m_pParent->m_ball[n]->m_P = m_pParent->m_ball[n]->m_PSpot;
					
					//ErrorMessage(m_pParent->m_parent, "whiteball-1");

					if( RefereeBreakDropBallOnTable(WHITE, m_pParent->m_ball[n]->m_P.x, m_pParent->m_ball[n]->m_P.y ) )
					{
						//ErrorMessage(m_pParent->m_parent, "whiteball-2");
					}
					else					
					{
						//ErrorMessage(m_pParent->m_parent, "whiteball-3");
						// ...otherwise position anywhere behind the 'Head String'
						CVector Pgreen, Pyellow;
						CVector P;
						float radius;
						//Pgreen = CVector(-WORLD_X(912),-WORLD_Y(395-12), BALL_POSITIONZOFFSET);
						//Pyellow = CVector(-WORLD_X(912),-WORLD_Y(199+12), BALL_POSITIONZOFFSET);
						Pgreen = CVector(-WORLD_X(591+65+120/*912+5*/), -600, BALL_POSITIONZOFFSET);
						Pyellow = CVector(-WORLD_X(591+65+120/*912+5*/), 600, BALL_POSITIONZOFFSET);
						float w,h;
						float x,y;
						
						//[f1 = Pyellow.y - Pgreen.y;]
						f1 = Pyellow.y;
						f1 = f1 - Pgreen.y;
						
						f1 = f1 * 0.9f;
						h =	f1;
						//h =	(Pyellow.y - Pgreen.y)*0.9f;
						w = h*0.5f;
						for(x = 0.0f;x<w;x+=BALL_RADIUS)
						for(y = 0.0f;y<h;y+=BALL_RADIUS)
						{
							
							//[f1 = Pyellow.x - x;]
							f1 = Pyellow.x;
							f1 = f1 - x;
							
							//[f2 = Pyellow.y - Pgreen.y;]
							f2 = Pyellow.y;
							f2 = f2 - Pgreen.y;
							
							f2 = f2 * 0.05f;
							
							//[f3 = Pyellow.y - y;]
							f3 = Pyellow.y;
							f3 = f3 - y;
							
							f3 = f3 - f2;
							if( RefereeBreakDropBallOnTable(WHITE, f1, f3) )
							//if( DropBallOnTable(WHITE, Pyellow.x - x, Pyellow.y - y - (Pyellow.y - Pgreen.y)*0.05f) )
								goto whiteSpotted;
						}
						whiteSpotted:;
					}
					
//					m_ball[n]->m_POld = m_ball[n]->m_P = m_ball[n]->m_PSpot;
					m_pParent->m_ball[n]->m_W = m_pParent->m_ball[n]->m_V = m_pParent->m_ball[n]->m_VOld = CVector(0,0,0);
	
					// turn ball on
					m_pParent->m_ball[n]->m_flags = 0;
					SetFlag(m_pParent->m_ball[n]->m_flags, Flg_BALL_ACTIVE);

					m_pParent->m_bBallInHand = 1;
					m_pParent->m_bStandUpFromShot = 1;


					continue;
				}		
	}
}

BOOL CCustomTable::RefereeBreakDropBallOnTable(int n, float x, float y, BOOL bDrop)
{
	// check if ball 'n' is on top of another ball
	int m;
	for(m=0;m<MAX_TOTALBALLS;m++)
	if(m!=n)
	if(m_pParent->m_ball[m]->Proximity(x, y, 0, BALL_RADIUS*2.0f) == TRUE)
	{
		//ErrorMessage(m_pParent->m_parent, "FALSE");
		return FALSE;
	}
	
	// check for ball n inside the D
	if(!m_pParent->m_bPracticeMode) // should be !
	{
		CVector B = CVector(x,y,BALL_POSITIONZOFFSET);
		CVector Pgreen, Pbrown;
		CVector P;
		float radius;
		//Pgreen = CVector(-WORLD_X(912),-WORLD_Y(395-12), BALL_POSITIONZOFFSET);
		//Pbrown = CVector(-WORLD_X(912),-WORLD_Y(297),BALL_POSITIONZOFFSET);
		Pgreen = CVector(-WORLD_X(591+65+120/*912+5*/), -600, BALL_POSITIONZOFFSET);
		Pbrown = CVector(-WORLD_X(591+65+120/*912+5*/), 0, BALL_POSITIONZOFFSET);

		//[P = Pgreen - Pbrown;]
		P = Pgreen;
		P = P - Pbrown;

		radius = P.Magnitude2();
		
		//[P = B - Pbrown;]
		P = B;
		P = P - Pbrown;


		int xoffset = 0, yoffset = 0;

		if(m_pParent->m_M3d_Table_ptr == &m_M3d_ukTable)
			{
			// Set Different Bounding Box Regions for UK Pool Game Type //
			xoffset = -110;
			yoffset = 85;
			}
		

		
		
		if(TestFlag(m_pParent->m_foul, Flg_FOUL_WHITEPOTTED) || TestFlag(m_pParent->m_foul, Flg_FOUL_NORMAL)
			|| TestFlag(m_pParent->m_foul, Flg_FOUL_BALLINHAND))
			{
			if(m_pParent->m_M3d_Table_ptr == &m_M3d_ukTable) Pbrown.x = 1235+xoffset;	// give ball-in-hand access to whole table
				else Pbrown.x = 1235;	// give ball-in-hand access to whole table
			}	


		//if( P.Magnitude2() < radius)
		if((B.y < -(Pgreen.y+xoffset)) && (B.y > (Pgreen.y+yoffset)))
		{			
			if(((B.x < Pbrown.x)) && (B.x > -(1235+xoffset)))
			{
				// update the drop ball's coordinates
				if(bDrop)
				{
					//m_pParent->m_ball[n]->m_P.x = x;
					//m_pParent->m_ball[n]->m_P.y = y;
					//m_pParent->m_ball[n]->AdjustOrientation();
					//m_pParent->m_ball[n]->m_POld = m_pParent->m_ball[n]->m_P;
				m_pParent->m_ball[n]->m_P.x = x;
				m_pParent->m_ball[n]->m_P.y = y;
				#ifdef FIXEDMODE
				m_pParent->m_ball[n]->m_V.x = 0;
				m_pParent->m_ball[n]->m_V.y = 0;
				m_pParent->m_ball[n]->m_V.z = 0;
				m_pParent->m_ball[n]->UpdateFixeds();
				#endif
				m_pParent->m_ball[n]->AdjustOrientation();
				m_pParent->m_ball[n]->m_POld = m_pParent->m_ball[n]->m_P;
				#ifdef FIXEDMODE
				m_pParent->m_ball[n]->UpdateFixeds();
				#endif
				}
				return TRUE;
			}
		}
		return FALSE;
	}
	
	// update the drop ball's coordinates
	if(bDrop)
	{
		//m_pParent->m_ball[n]->m_P.x = x;
		//m_pParent->m_ball[n]->m_P.y = y;
		//m_pParent->m_ball[n]->AdjustOrientation();
		//m_pParent->m_ball[n]->m_POld = m_pParent->m_ball[n]->m_P;
		//ErrorMessage(m_pParent->m_parent, "TRUE");
		m_pParent->m_ball[n]->m_P.x = x;
		m_pParent->m_ball[n]->m_P.y = y;
		#ifdef FIXEDMODE
		m_pParent->m_ball[n]->m_V.x = 0;
		m_pParent->m_ball[n]->m_V.y = 0;
		m_pParent->m_ball[n]->m_V.z = 0;
		m_pParent->m_ball[n]->UpdateFixeds();
		#endif
		m_pParent->m_ball[n]->AdjustOrientation();
		m_pParent->m_ball[n]->m_POld = m_pParent->m_ball[n]->m_P;
		#ifdef FIXEDMODE
		m_pParent->m_ball[n]->UpdateFixeds();
		#endif

	}
	return TRUE;
}

void CCustomTable::RefereeBreakUpdatePots(void)
{
	int ballColour;
	float fx,fy;
	int x,y;
		
	int n;
	for(n = 0; n < 16; n++)
	{
		g_pottedSpots[n] = -1;
		g_pottedStripes[n] = -1;
	}

	// scan thru all potted balls, and see who's potted what
	int m;
	for(m=0;m<MAX_TOTALBALLS;m++)
	if(!TestFlag(m_pParent->m_ball[m]->m_flags, Flg_BALL_ACTIVE))
	{
		// what has been potted? (for both P1 & P2)
		if(m>=0 && m<=15)
		{
			g_pottedSpots[m] = m;
		}
		
		/*
		fx = m_pParent->m_ball[m]->m_P.x;
		fy = m_pParent->m_ball[m]->m_P.y;

		
		// what has P1 potted
		if(fx > 20001)
		{
			fx-=20000;
			fx /= 100;
			if(fx>=0 && fx<= 15)
			{
				x = fx;
				y = fy;
				if((m_pParent->m_ball[m]->m_colour >= 1) &&  (m_pParent->m_ball[m]->m_colour <= 8))
					g_pottedSpots[x] = m;				
				else if((m_pParent->m_ball[m]->m_colour >= 9) &&  (m_pParent->m_ball[m]->m_colour <= 15))
					g_pottedStripes[x] = m;								
			}
		}

		// what has P2 potted
		if(fy > 20001)
		{
			fy-=20000;
			fy /= 100;
			if(fy>=0 && fy<= 15)
			{
				x = fx;
				y = fy;
				if((m_pParent->m_ball[m]->m_colour >= 1) &&  (m_pParent->m_ball[m]->m_colour <= 8))
					g_pottedSpots[y] = m;				
				else if((m_pParent->m_ball[m]->m_colour >= 9) &&  (m_pParent->m_ball[m]->m_colour <= 15))
					g_pottedStripes[y] = m;				
			}
		}
		*/
	}
	
}

//////
//////

void CCustomTable::RefereeBowls(DWORD msg, void *pData)
{
	if(msg == CTMSG_LINESET)
	{
		RefereeBowlsLINESET(msg, pData);
	}
	else if(msg == CTMSG_SIMEND)
	{
		Decode();
		RefereeBowlsSIMEND();				   
		Encode();
	}
	else
	{
		RefereeSnooker(msg, pData);
	}
}

void CCustomTable::RefereeBowlsLINESET(DWORD msg, void *pData)
{
	ctmsg_LINESET_DATA *ctmsgData;
	ctmsgData = (ctmsg_LINESET_DATA_typ *) pData;
	
	// ---------------------------------------------------------------------------
	// if not in state 0 : check if player has selected the white ball by mistake
	// ---------------------------------------------------------------------------
	//if(!m_pParent->m_rednext)
	  //if(m_pParent->m_colourseq < YELLOW)
		//if(m_pParent->m_testBall->m_colour == WHITE)
			//if(m_pParent->m_nominateBall == WHITE)
	if(m_pParent->m_colourseq > 0)
		//if(!m_pParent->m_bPracticeMode)
			if(m_pParent->m_selectBall == WHITE)
			{
				///		
				/// [simulate R-Button down for cancel shot]
				///

				m_pParent->m_bTestBallFinished = FALSE;
				//m_snooker->m_bCueTipReady = FALSE;

				// reset foucus to table (this) window, & reset 'raise butt'
				m_pParent->m_RaiseButt = m_pParent->m_RaiseButtOld = 0;
				m_pParent->m_bReceiveRaiseButtActive = FALSE;
				m_pParent->m_bRaiseButtHelper = TRUE;
				GAMEAPPVIEW2->m_sliderRaiseButt.SetPos(90);
				GAMEAPPVIEW2->PreviousWndEditSetFocus();

				GAMEAPPVIEW2->m_point = ctmsgData->point;
					
				// Only give this window the mouse's full attention 
				GAMEAPPVIEW2->SetCapture();
				
				if(m_pParent->m_tableMouseOn)
				{
					if(m_pParent->m_bCueState)
					{
						// Now Give ,mouse's attention to all
						if(GAMEAPPVIEW2->GetCapture() == GAMEAPPVIEW2) ReleaseCapture();

						// give mouse full movement
						ClipCursor(&GAMEAPPVIEW2->m_rectCursor);

						// reset mouse cursor position back to start position
						CRect rect;
						GAMEAPPVIEW2->GetWindowRect(&rect);
						SetCursorPos(rect.left + m_pParent->m_sCPoint.x+2, rect.top + m_pParent->m_sCPoint.y+2);
					
						m_pParent->SelectBall(0, ctmsgData->point);
						
						m_pParent->m_point = ctmsgData->point;
						m_pParent->m_sightlineFade = 0;
						
						if(m_pParent->m_bPracticeMode);
						else
							m_pParent->SelectBallSend(0, ctmsgData->point, m_pParent->m_windowWidth, m_pParent->m_windowHeight, m_pParent->m_selectBall);
					}
				}

				if(!m_pParent->m_cueBallSelect)
				if(!GAMEAPPVIEW2->m_bShowCursor)
				{
					GAMEAPPVIEW2->m_bShowCursor = TRUE;
					SetCursor(GAMEAPPVIEW2->m_hCursor);
					ShowCursor(GAMEAPPVIEW2->m_bShowCursor); // show cursor
				}

				///
				///
				///

				GAMEAPPVIEW2->AlertMessage("Warning! You can't strike the 'White' ball!\n\n(Use 'Left-Mouse-Button' to select an alternative ball");
				return;
			}


	// -----------------------------------------------------------------------------------------
	// if in state 1 or 2 : check if player has selected a ball that has already been played out
	// -----------------------------------------------------------------------------------------
/*
	BOOL bCorrectColour = FALSE;

	if(m_pParent->m_colourseq == 1)
	{
		if(m_pParent->m_selectBall >= 1 && m_pParent->m_selectBall <= 5)
			bCorrectColour = TRUE;
	}
	else if(m_pParent->m_colourseq == 2)
	{
		if(m_pParent->m_selectBall >= 6 && m_pParent->m_selectBall <= 10)
			bCorrectColour = TRUE;
	}
  */
	
	//if(!m_pParent->m_rednext)
	  //if(m_pParent->m_colourseq < YELLOW)
		//if(m_pParent->m_testBall->m_colour == WHITE)
			//if(m_pParent->m_nominateBall == WHITE)
	if(m_pParent->m_colourseq > 0)
		//if(RefereeBowlsBallOnTable(m_pParent->m_selectBall))
		//if(bCorrectColour)
			if(m_pParent->m_ball[m_pParent->m_selectBall]->m_P.x > -WORLD_X(591+65+120))
		//if(TestFlag(m_pParent->m_ball[m_pParent->m_selectBall]->m_flags, Flg_BALL_NOSTRIKE))
			//if(m_pParent->m_selectBall == WHITE)
			{
				///		
				/// [simulate R-Button down for cancel shot]
				///

				m_pParent->m_bTestBallFinished = FALSE;
				//m_snooker->m_bCueTipReady = FALSE;

				// reset foucus to table (this) window, & reset 'raise butt'
				m_pParent->m_RaiseButt = m_pParent->m_RaiseButtOld = 0;
				m_pParent->m_bReceiveRaiseButtActive = FALSE;
				m_pParent->m_bRaiseButtHelper = TRUE;
				GAMEAPPVIEW2->m_sliderRaiseButt.SetPos(90);
				GAMEAPPVIEW2->PreviousWndEditSetFocus();

				GAMEAPPVIEW2->m_point = ctmsgData->point;
					
				// Only give this window the mouse's full attention 
				GAMEAPPVIEW2->SetCapture();
				
				if(m_pParent->m_tableMouseOn)
				{
					if(m_pParent->m_bCueState)
					{
						// Now Give ,mouse's attention to all
						if(GAMEAPPVIEW2->GetCapture() == GAMEAPPVIEW2) ReleaseCapture();

						// give mouse full movement
						ClipCursor(&GAMEAPPVIEW2->m_rectCursor);

						// reset mouse cursor position back to start position
						CRect rect;
						GAMEAPPVIEW2->GetWindowRect(&rect);
						SetCursorPos(rect.left + m_pParent->m_sCPoint.x+2, rect.top + m_pParent->m_sCPoint.y+2);
					
						m_pParent->SelectBall(0, ctmsgData->point);
						
						m_pParent->m_point = ctmsgData->point;
						m_pParent->m_sightlineFade = 0;
						
						if(m_pParent->m_bPracticeMode);
						else
							m_pParent->SelectBallSend(0, ctmsgData->point, m_pParent->m_windowWidth, m_pParent->m_windowHeight, m_pParent->m_selectBall);
					}
				}

				if(!m_pParent->m_cueBallSelect)
				if(!GAMEAPPVIEW2->m_bShowCursor)
				{
					GAMEAPPVIEW2->m_bShowCursor = TRUE;
					SetCursor(GAMEAPPVIEW2->m_hCursor);
					ShowCursor(GAMEAPPVIEW2->m_bShowCursor); // show cursor
				}

				///
				///
				///

				GAMEAPPVIEW2->AlertMessage("Warning! You can't strike this ball!\n\n(Use 'Left-Mouse-Button' to select an alternative ball");
				return;
			}

	// ---------------------------------------------
	// normal 'LineSet' here!
	// ---------------------------------------------

	// Now Give ,mouse's attention to all
	if(GAMEAPPVIEW2->GetCapture() == GAMEAPPVIEW2) ReleaseCapture();
			
	// test!!! send final cue-line pos
	GAMEAPPVIEW2->SendPlayerMouseMove(m_pParent->m_mouseX, m_pParent->m_mouseY, 
							m_pParent->m_windowWidth, m_pParent->m_windowHeight,
							m_pParent->m_strikeLine, m_pParent->m_strikeDist);

	m_pParent->m_cueBallSelect = 5; //--
	m_pParent->m_mouseX = 0; // position cuetip GFX pointer at centre of selected ball
	m_pParent->m_mouseY = 0;

	GAMEAPPVIEW2->m_sliderRaiseButt.EnableWindow(TRUE);
	m_pParent->m_bRaiseButtHelper = TRUE;

	// give mouse full movement
	ClipCursor(&GAMEAPPVIEW2->m_rectCursor);

	if(!GAMEAPPVIEW2->m_bShowCursor)
	{
		GAMEAPPVIEW2->m_bShowCursor = TRUE;
		SetCursor(GAMEAPPVIEW2->m_hCursor);
		ShowCursor(GAMEAPPVIEW2->m_bShowCursor); // show cursor
	}

	// reset mouse cursor position back to start position
	CRect rect;
	GAMEAPPVIEW2->GetWindowRect(&rect);
	SetCursorPos(rect.left + m_pParent->m_sCPoint.x+2, rect.top + m_pParent->m_sCPoint.y+2);


	m_pParent->m_bCueLineSet = TRUE;
	
	m_pParent->m_tableMousePosStrike[0] = m_pParent->m_tableMousePos[0];
	m_pParent->m_tableMousePosStrike[1] = m_pParent->m_tableMousePos[1];
	m_pParent->m_tableMousePosStrike[2] = m_pParent->m_tableMousePos[2];
}

BOOL CCustomTable::RefereeBowlsBallOnTable(int n)
{
	//note: this include balls that have been potted. (Use the ! of this function to see how many bowls are remaining)
	if(TestFlag(m_pParent->m_ball[n]->m_flags, Flg_BALL_ACTIVE) || TestFlag(m_pParent->m_ball[n]->m_flags, Flg_BALL_POTTED))
		return TRUE;

	return FALSE;
}

BOOL CCustomTable::RefereeBowlsBallOnTableNotPotted(int n)
{
	if(TestFlag(m_pParent->m_ball[n]->m_flags, Flg_BALL_ACTIVE))
		return TRUE;

	return FALSE;
}


int CCustomTable::RefereeBowlsNextStrikeBall(void)
{

	if(m_pParent->m_colourseq == 0) return 0; // Jack (white-ball)
	else if(m_pParent->m_colourseq == 1) // 1P
	{
		int n;
		for(n=1;n<=5;n++)
		if(TestFlag(m_pParent->m_ball[n]->m_flags, Flg_BALL_ACTIVE))
		if(m_pParent->m_ball[n]->m_P.x <= -WORLD_X(591+65+120)) // check behind head-string
		{
			return n; //  blue bowls (spot-balls)
		}
	}
	else if(m_pParent->m_colourseq == 2) // 2P
	{
		int n;
		for(n=6;n<=10;n++)
		if(TestFlag(m_pParent->m_ball[n]->m_flags, Flg_BALL_ACTIVE))
		if(m_pParent->m_ball[n]->m_P.x <= -WORLD_X(591+65+120)) // check behind head-string
		{
			return n; // red bowls (stripe-balls)
		}
	}

	return 0;
}


BOOL CCustomTable::RefereeBowlsOutOfTimeNextBowl(void)
{
	int m;

	if(m_pParent->m_bTurn1P)
	{
		// find a vacant 'blue' bowl to throw
		for(m=1;m<=5;m++)			
		if(!RefereeBowlsBallOnTable(m))
		{					
			m_pParent->m_ball[m]->Init(WORLD_X(591+65+120 + 10),WORLD_Y(297),0, m);
			
			//char bbb[80];
			//sprintf(bbb, "Next ball =  %d", m);
			//ErrorMessage(m_pParent->m_parent, bbb);
			
			return TRUE;
		}
	}
	else
	{
		// find a vacant 'red' bowl to throw
		for(m=6;m<=10;m++)			
		if(!RefereeBowlsBallOnTable(m))
		{					
			m_pParent->m_ball[m]->Init(WORLD_X(591+65+120 + 10),WORLD_Y(297),0, m);

			//char bbb[80];
			//sprintf(bbb, "Next ball =  %d", m);
			//ErrorMessage(m_pParent->m_parent, bbb);

			return TRUE;
		}			
	}

	return FALSE;
}


#if 0

void CCustomTable::RefereeBowlsOutOfTime(void)
{
	int n;

	m_pParent->m_bBallInHand = 1;
	m_pParent->m_bStandUpFromShot = 1;

	// remove the ball that is behind the baulk-line
	for(n=1;n<=10;n++)
	//if(RefereeBowlsBallOnTable(n))
	//if(!TestFlag(m_pParent->m_ball[n]->m_flags, Flg_BALL_POTTED)) // make sure this isn't a potted ball
	if(RefereeBowlsBallOnTableNotPotted(n))
	if(m_pParent->m_ball[n]->m_P.x < -WORLD_X(591+65+120))
	{
		//char bbb[80];
		//sprintf(bbb, "Out of time, so lose %d ball", n);
		//ErrorMessage(m_pParent->m_parent, bbb);

		// treat this ball as potted.
		ClearFlag(m_pParent->m_ball[n]->m_flags, Flg_BALL_ACTIVE);
		SetFlag(m_pParent->m_ball[n]->m_flags, Flg_BALL_POTTED);

		// give next player his/her ball, if they have any remaining:
		// -------------------------------------
		if(RefereeBowlsOutOfTimeNextBowl())
			goto end;

		// ...if get this far, this player has ran-out of ammo, so give other player a chance to continue (i.e. repeat above code, but swap player turn)
		if(m_pParent->m_bTurn1P) m_pParent->m_bTurn1P = FALSE;
		else m_pParent->m_bTurn1P = TRUE;

		if(RefereeBowlsOutOfTimeNextBowl())
			goto end;

		// ...if get this far(2), this player has also ran-out of ammo. So gameover, who is closest?
		if(m_pParent->m_bTurn1P) m_pParent->m_bTurn1P = FALSE;
		else m_pParent->m_bTurn1P = TRUE;

		////////////////////////////////////////////////////////////////////
		// work out which ball is closest to white (Jack)
		float closestDist = 10000;
		int closestBall = 0;
		CVector D;
		float d;
		int p;

		if(TestFlag(m_pParent->m_ball[0]->m_flags, Flg_BALL_ACTIVE)) // only do if white is still on table
		for(p=1;p<=10;p++)
		//if(p != ignoreBall)
		//if(RefereeBowlsBallOnTable(p))
		if(RefereeBowlsBallOnTableNotPotted(n))
		{
			D = m_pParent->m_ball[p]->m_P - m_pParent->m_ball[0]->m_P;
			d = D.Magnitude();
			if(d < closestDist)
			{
				closestDist = d;
				closestBall = p;
			}
		}
		m_pParent->m_bestScore = closestBall; // keep track of closest ball to white
		////////////////////////////////////////////////////////////////////
				
		// no balls left to play
		//if(_blues == 0 && _reds == 0)
		//{					
			// who is closest?
			if(closestBall >= 1 && closestBall <= 5) // 1P wins!
				m_pParent->m_gameOver = 2;
			else if(closestBall >= 6 && closestBall <= 10) // 2P wins!
				m_pParent->m_gameOver = 1;
			else
			{
				// draw! i.e both players have wasted all their bowls, only the white remains
				// lets make them start again! :)				
				for(int nn=1;nn<=10;nn++)				
				{						
					// reset this ball, so we can use it again
					ClearFlag(m_pParent->m_ball[nn]->m_flags, Flg_BALL_ACTIVE | Flg_BALL_POTTED);
					m_pParent->m_ball[nn]->m_P.x = 10000;
					m_pParent->m_ball[nn]->m_P.y = 10000;
				}

				if(RefereeBowlsOutOfTimeNextBowl())
					goto end;				
			}
		//}
		break;
	}

	end:;

	if(m_pParent->m_bTurn1P)
		m_pParent->m_colourseq = 1; // P1: blue bowls
	else
		m_pParent->m_colourseq = 2; // P2: blue bowls
}

#endif // 0

void CCustomTable::RefereeBowlsOutOfTime(void)
{
	int n;

//	m_pParent->m_bBallInHand = 1;
//	m_pParent->m_bStandUpFromShot = 1;

	// remove any balls that are behind the baulk-line
	for(n=0;n<=10;n++)
	//if(RefereeBowlsBallOnTable(n))
	//if(!TestFlag(m_pParent->m_ball[n]->m_flags, Flg_BALL_POTTED)) // make sure this isn't a potted ball
	if(RefereeBowlsBallOnTableNotPotted(n))
	if(m_pParent->m_ball[n]->m_P.x < -WORLD_X(591+65+120))
	{
		char bbb[80];
		sprintf(bbb, "Out of time, so lose %d ball", n);
		ErrorMessage(m_pParent->m_parent, bbb);

		// treat this ball as potted.
		ClearFlag(m_pParent->m_ball[n]->m_flags, Flg_BALL_ACTIVE);
		SetFlag(m_pParent->m_ball[n]->m_flags, Flg_BALL_POTTED);
	}
}


void CCustomTable::RefereeBowlsSIMEND(void)
{
	int n;
	int colour;
	int no_colours;
	BOOL bWhiteInPocket = FALSE;
	BOOL bWhiteInBaulk = FALSE;
	BOOL bWhiteLessThanFootSpot = FALSE;
	BOOL bNextPlayer = FALSE;
	BOOL bEndOfGame = FALSE;

	int ignoreBall = 0;

	int _spots = 0;
	int _stripes = 0;
	int _8ball = 0;
	int _pottedspots = 0;
	int _pottedstripes = 0;
	int _potted8ball = 0;
		 
	int _blues = 0;
	int _reds = 0;



	m_pParent->m_bMiss = TRUE;												// assume no balls potted
	ClearFlag(m_pParent->m_foul, Flg_FOUL_NORMAL | Flg_FOUL_WHITEPOTTED | Flg_FOUL_GAMEOVER | Flg_FOUL_BALLINHAND);	// assume no foul to start with [keep some flags from previous shot]
	m_pParent->m_bRefreshScores = TRUE;
	

	// just in case :)
	//if(m_colourseq)
	//	m_rednext = FALSE;

	// [freeball mode]
	//if(TestFlag(m_foul, Flg_FOUL_FREEBALL))
	//{
	//	//MessageBox(0, "0","0",MB_OK);
//		FreeBallReferee();
//		ClearFlag(m_foul, Flg_FOUL_FREEBALL);
//		goto end;
//	}

	// [default mode]

	/////////////////////////////////////////////////////////////
	// see if any balls have made it back past the baulk line, if so u get them back, except if target ball doesn't collide with anything
	
	//char bbb[80];
	//sprintf(bbb, "m_selectBall = %d; m_dominoHit1st = %d", m_pParent->m_selectBall, m_pParent->m_dominoHit1st);
	//ErrorMessage(m_pParent->m_parent, bbb);

	/*
	// DEBUG CODE -------------
	char bbbb[80];
	for(n=0;n<=10;n++)
	if(RefereeBowlsBallOnTable(n))
	{
		if(TestFlag(m_pParent->m_ball[n]->m_flags, Flg_BALL_DOMINOHITLAST))
		{
			sprintf(bbbb, "%d ball domino-hit-last flag is set", n);
			ErrorMessage(m_pParent->m_parent, bbbb);
		}

	}
	// -----------------------
	*/


//	char bbb2[80];
//	sprintf(bbb2, "m_pParent->m_selectBall = %d, m_pParent->m_dominoHit1st = %d", m_pParent->m_selectBall, m_pParent->m_dominoHit1st);
//	ErrorMessage(m_pParent->m_parent, bbb2);
	
	if(m_pParent->m_selectBall == m_pParent->m_dominoHit1st)
	{
//		char bbb1[80];
//		sprintf(bbb1, "%d ball didn't hit anything, so lose it 1", m_pParent->m_selectBall);
//		ErrorMessage(m_pParent->m_parent, bbb1);

		// this ball didn't hit anything, so dont give it back to player, if it passed the baulk line
		if(m_pParent->m_selectBall != 0) // exclude the white ball (Jack)
		if(m_pParent->m_ball[m_pParent->m_selectBall]->m_P.x < -WORLD_X(591+65+120))
		{
//			char bbb[80];
//			sprintf(bbb, "%d ball didn't hit anything, so lose it", m_pParent->m_selectBall);
//			ErrorMessage(m_pParent->m_parent, bbb);

			// treat this ball as potted.
			ClearFlag(m_pParent->m_ball[m_pParent->m_selectBall]->m_flags, Flg_BALL_ACTIVE);
			SetFlag(m_pParent->m_ball[m_pParent->m_selectBall]->m_flags, Flg_BALL_POTTED);
			//m_pParent->m_ball[n]->m_P.x = 10000;
			//m_pParent->m_ball[n]->m_P.y = 10000;
		}
	}
	else
	{		
//		char bbb1[80];
//		sprintf(bbb1, "%d is back in play 1", n);
//		ErrorMessage(m_pParent->m_parent, bbb1);

		// give balls back to player(s) if need be
		for(n=1;n<=10;n++)
		//if(RefereeBowlsBallOnTable(n))
		if(RefereeBowlsBallOnTableNotPotted(n))
		{						
			if(m_pParent->m_ball[n]->m_P.x < -WORLD_X(591+65+120))
			{
//				char bbb[80];
//				sprintf(bbb, "%d is back in play", n);
//				ErrorMessage(m_pParent->m_parent, bbb);

				//////////////////////////////////////////////////////
				// if this ball was the ball just played out & it hit the white or opponents ball first, then lose it!				
				if(m_pParent->m_selectBall == n) // ball that was just bowled out
				if(m_pParent->m_colourseq == 1) // if P1 just rolled out a ball
				if(m_pParent->m_dominoHit1st == 0 || m_pParent->m_dominoHit1st >= 6)
				{
					// treat this ball as potted.
					ClearFlag(m_pParent->m_ball[n]->m_flags, Flg_BALL_ACTIVE);
					SetFlag(m_pParent->m_ball[n]->m_flags, Flg_BALL_POTTED);
					continue;
				}

				if(m_pParent->m_selectBall == n) // ball that was just bowled out
				if(m_pParent->m_colourseq == 2) // if P1 just rolled out a ball
				if(m_pParent->m_dominoHit1st <= 5) // white incl. aswell
				{
					// treat this ball as potted.
					ClearFlag(m_pParent->m_ball[n]->m_flags, Flg_BALL_ACTIVE);
					SetFlag(m_pParent->m_ball[n]->m_flags, Flg_BALL_POTTED);
					continue;
				}
				//////////////////////////////////////////////////////

				// reset this ball, so we can use it again
				ClearFlag(m_pParent->m_ball[n]->m_flags, Flg_BALL_ACTIVE | Flg_BALL_POTTED);
				m_pParent->m_ball[n]->m_P.x = 10000;
				m_pParent->m_ball[n]->m_P.y = 10000;
			}
		}
	}


	// check if white is past the baulk-line, and it's not an initial jack roll-out, ...then game-over
	if(m_pParent->m_colourseq != 0)
	if(m_pParent->m_ball[0]->m_P.x < -WORLD_X(591+65+120))
	{
		// gameover, whoever push white back past the baulk line! ul :)
		if(m_pParent->m_bTurn1P)
			m_pParent->m_gameOver = 1;
		else
			m_pParent->m_gameOver = 2;

		return;
	}

	/////////////////////////////////////////////////////////////
_start:;
	/////////////////////////////////////////////////////////////
	// what is left?
	for(n=1;n<=5;n++)
	{
		if(!RefereeBowlsBallOnTable(n))
			_blues++;		
	}

	for(n=6;n<=10;n++)
	{
		if(!RefereeBowlsBallOnTable(n))
			_reds++;		
	}			

	//char buf3[80];
	//sprintf(buf3, "blues = %d; reds = %d", _blues, _reds);
	//ErrorMessage(m_pParent->m_parent, buf3);

	/////////////////////////////////////////////////////////////

	// what was pottted?
	
/*
	for(n=0;n<MAX_TOTALBALLS;n++)
	{
		if(TestFlag(m_pParent->m_ball[n]->m_flags, Flg_BALL_POTTED))
		{
			if(m_pParent->m_ball[n]->m_colour >= 1 && m_pParent->m_ball[n]->m_colour <= 7)
			{
				_pottedspots++;
			}
			else if(m_pParent->m_ball[n]->m_colour >= 9 && m_pParent->m_ball[n]->m_colour <= 15)
			{
				_pottedstripes++;
			}
			else if(m_pParent->m_ball[n]->m_colour == 8)
			{
				_potted8ball++;
			}
		}
	}
  */
	/////////////////////////////////////////////////////////////
	// see if white ball has been potted (foul)
	//for(n=0;n<MAX_TOTALBALLS;n++)
	{
		if(TestFlag(m_pParent->m_ball[0]->m_flags, Flg_BALL_POTTED))
			if(m_pParent->m_ball[0]->m_colour == WHITE)
			{
				bWhiteInPocket = TRUE;

				// check if someone has lost
				if(m_pParent->m_colourseq != 0) // if not at the begining of a game, i.e. Jack roll-out
				{
					// gameover, whoever potted the white loses!
					if(m_pParent->m_bTurn1P)
						m_pParent->m_gameOver = 1;
					else
						m_pParent->m_gameOver = 2;

					return;
				}

				//break;
				//SetFlag(m_pParent->m_foul, Flg_FOUL_NORMAL | Flg_FOUL_WHITEPOTTED);

				//if(_potted8ball != 0)
				//{
				//	// *TODO - FOUL!!!! DO RERACK!!!!
				//	SetFlag(m_pParent->m_foul, Flg_FOUL_GAMEOVER);
				//}

			}
	}
	/////////////////////////////////////////////////////////////

	/////////////////////////////////////////////////////////////
	// see if white ball has ended up in the baulk area
	if(m_pParent->m_ball[0]->m_colour == WHITE)
	{
				//-WORLD_X(591+65+120), -600
//				char bbb[80];
//				sprintf(bbb, "%f < %f", m_pParent->m_ball[0]->m_P.x, -WORLD_X(591+65+120));
//				ErrorMessage(m_pParent->m_parent, bbb);

				if(m_pParent->m_ball[0]->m_P.x < -WORLD_X(591+65+120))
				{
					bWhiteInBaulk = TRUE;
//					ErrorMessage(m_pParent->m_parent, "> BAULKLINE");
				}

				//break;


				//SetFlag(m_pParent->m_foul, Flg_FOUL_NORMAL | Flg_FOUL_WHITEPOTTED);

				//if(_potted8ball != 0)
				//{
				//	// *TODO - FOUL!!!! DO RERACK!!!!
				//	SetFlag(m_pParent->m_foul, Flg_FOUL_GAMEOVER);
				//}

	}
	/////////////////////////////////////////////////////////////

	/////////////////////////////////////////////////////////////
	// see if white ball hasn't reached default starting dist
	if(m_pParent->m_ball[0]->m_colour == WHITE)
	{
				//-WORLD_X(591+65+120), -600
//				char bbb[80];
//				sprintf(bbb, "%f < %f", m_pParent->m_ball[0]->m_P.x, -WORLD_X(591+65+120));
//				ErrorMessage(m_pParent->m_parent, bbb);

				if(m_pParent->m_ball[0]->m_P.x < WORLD_X(591+65+120))
				{
					bWhiteLessThanFootSpot = TRUE;
//					ErrorMessage(m_pParent->m_parent, "> START SPOT");
				}

				//break;


				//SetFlag(m_pParent->m_foul, Flg_FOUL_NORMAL | Flg_FOUL_WHITEPOTTED);

				//if(_potted8ball != 0)
				//{
				//	// *TODO - FOUL!!!! DO RERACK!!!!
				//	SetFlag(m_pParent->m_foul, Flg_FOUL_GAMEOVER);
				//}

	}
	/////////////////////////////////////////////////////////////
	

	/*
	/////////////////////////////////////////////////////////////
	// what happens if player strikes a ball other than the white
	if(m_pParent->m_selectBall != WHITE)
	{
		SetFlag(m_pParent->m_foul, Flg_FOUL_NORMAL);

		if(_potted8ball != 0)
		{
			// *TODO - FOUL!!!! DO RERACK!!!!
			SetFlag(m_pParent->m_foul, Flg_FOUL_GAMEOVER);
		}

		goto end;
	}
	/////////////////////////////////////////////////////////////
	*/

	/*
	/////////////////////////////////////////////////////////////
	// if white doesn't hit a ball at all, foul ball on!
	//if(m_colourseq < YELLOW) // bypass if on colours at the end
	if((m_pParent->m_selectBall == WHITE) && (m_pParent->m_dominoHit1st == WHITE))
	{
			SetFlag(m_pParent->m_foul, Flg_FOUL_NORMAL);

			if(_potted8ball != 0)
			{
				// *TODO - FOUL!!!! DO RERACK!!!!
				SetFlag(m_pParent->m_foul, Flg_FOUL_GAMEOVER);
			}

			goto end;
	}
	/////////////////////////////////////////////////////////////
	*/

	

	//char buf2[80];
	//sprintf(buf2, "spots potted = %d; stripes potted = %d", _pottedspots, _pottedstripes);
	//ErrorMessage(m_pParent->m_parent, buf2);
	

	/////////////////////////////////////////////////////////////
	// make sure the correct ball has been potted, otherwise foul!
	//m_colourseq == 0,1 => UNDECIDED
	//m_colourseq == 2 => 1P HAS SPOTS
	//m_colourseq == 3 => 1P HAS STRIPES
	
	
		//for(n=0;n<MAX_TOTALBALLS;n++)
		//{
		//	if(TestFlag(m_pParent->m_ball[n]->m_flags, Flg_BALL_POTTED))
		//	{

	
	////
	/*
	// if the 8-ball has been potted - game-over!!!!
	colour = m_pParent->m_ball[m_pParent->m_dominoHit1st]->m_colour;
	if(_potted8ball > 0)
		if(colour != 8)
			{
			
				SetFlag(m_pParent->m_foul, Flg_FOUL_NORMAL | Flg_FOUL_GAMEOVER);
				goto end;
			}
	*/
	////
				// for every shot
				m_pParent->m_bBallInHand = 1;
				m_pParent->m_bStandUpFromShot = 1;
				
				int tbl = GAMEAPPVIEW2->m_lobby->GetTableIdxFromPlayer(GAMEAPPVIEW2->m_playerSockIdx);
				if(tbl!=-1)
				{
					m_pParent->m_shotTime =  m_table[tbl].shotTime*50;
//					ErrorMessage(m_pParent->m_parent, "TIMER");
				}

				_top:;
				if(m_pParent->m_colourseq == 0)
				{
					// playing the 'Jack' (white-ball) out state

					// check if player potted the white or hasn't reached the foot-spot //left it in the baulk area
					if(bWhiteInPocket || bWhiteLessThanFootSpot)//bWhiteInBaulk)
					{
						// reposition white at foot-spot
						m_pParent->m_ball[0]->Init(WORLD_X(331 + 10 - 2 + 65), WORLD_Y(297), 0, WHITE);						
					}
					else
					{
						// move white to centre of table y - component only
						float x = m_pParent->m_ball[0]->m_P.x; // keep x component
						m_pParent->m_ball[0]->Init(-x, WORLD_Y(297), 0, WHITE);
					}

					if(m_pParent->m_bTurn1P)
					{
						m_pParent->m_colourseq = 1; // P1: blue bowls

						/*
						// find a vacant 'blue' bowl to throw
						for(n=1;n<=5;n++)
						{
							if(TestFlag(m_pParent->m_ball[n]->m_flags, Flg_BALL_ACTIVE))
							if(!RefereeBowlsBallOnTable(n))
							{
								//if(m_pParent->m_ball[n]->m_colour >= 1 && m_pParent->m_ball[n]->m_colour <= 5)
								{								  
									char bb[80];
									sprintf(bb, "n = %d", n);
									ErrorMessage(m_pParent->m_parent, bb);

									m_pParent->m_ball[n]->Init(WORLD_X(591+65+120 + 10),WORLD_Y(297),0, n);
									break;
								}
							}
						}
						*/
						m_pParent->m_ball[1]->Init(WORLD_X(591+65+120 + 10),WORLD_Y(297),0, 1);
						ignoreBall = 1;
					}											  
					else
					{
						m_pParent->m_colourseq = 2;  // P2: red bowls
						/*
						// find a vacant 'red' bowl to throw
						for(n=6;n<=10;n++)
						{
							if(TestFlag(m_pParent->m_ball[n]->m_flags, Flg_BALL_ACTIVE))
							if(!RefereeBowlsBallOnTable(n))
							{
								//if(m_pParent->m_ball[n]->m_colour >= 6 && m_pParent->m_ball[n]->m_colour <= 10)
								{								
									m_pParent->m_ball[n]->Init(WORLD_X(591+65+120 + 10),WORLD_Y(297),0, n);
									break;
								}
							}
						}
						*/
						m_pParent->m_ball[6]->Init(WORLD_X(591+65+120 + 10),WORLD_Y(297),0, 6);
						ignoreBall = 6;
					}
				}
				else if(m_pParent->m_colourseq == 1)
				{					
					// opponents turn
					if(_reds > 0) {
						m_pParent->m_colourseq = 2;
						m_pParent->m_bTurn1P = FALSE;
						//bNextPlayer = TRUE;
						//m_pParent->m_bConcededGame = FALSE; // clear this flag (now that the turn has swapped over, we can allow gameOver to continue...)
						GAMEAPPVIEW2->m_bFlashWindow = TRUE;

						//ErrorMessage(m_pParent->m_parent, "A");

						// find a vacant 'red' bowl to throw
						for(n=6;n<=10;n++)
						{
							//if(TestFlag(m_pParent->m_ball[n]->m_flags, Flg_BALL_ACTIVE))
							if(!RefereeBowlsBallOnTable(n))
							{
								//ErrorMessage(m_pParent->m_parent, "B");
								//if(m_pParent->m_ball[n]->m_colour >= 6 && m_pParent->m_ball[n]->m_colour <= 10)
								{								
									m_pParent->m_ball[n]->Init(WORLD_X(591+65+120 + 10),WORLD_Y(297),0, n);
									ignoreBall = n;
									break;
								}
							}
						}
					}
					else
					{
						if(_blues > 0)
						{
							m_pParent->m_colourseq = 2;
							goto _top;
						}
					}
				}
				else if(m_pParent->m_colourseq == 2)
				{					
					// opponents turn 
					if(_blues > 0) {
						m_pParent->m_colourseq = 1;
						m_pParent->m_bTurn1P = TRUE;
						//bNextPlayer = TRUE;
						//m_pParent->m_bConcededGame = FALSE; // clear this flag (now that the turn has swapped over, we can allow gameOver to continue...)
						GAMEAPPVIEW2->m_bFlashWindow = TRUE;

						//ErrorMessage(m_pParent->m_parent, "C");

						// find a vacant 'blue' bowl to throw
						for(n=1;n<=5;n++)
						{
							//if(TestFlag(m_pParent->m_ball[n]->m_flags, Flg_BALL_ACTIVE))
							if(!RefereeBowlsBallOnTable(n))
							{
								//ErrorMessage(m_pParent->m_parent, "D");
								//if(m_pParent->m_ball[n]->m_colour >= 1 && m_pParent->m_ball[n]->m_colour <= 5)
								{								
									m_pParent->m_ball[n]->Init(WORLD_X(591+65+120 + 10),WORLD_Y(297),0, n);
									ignoreBall = n;
									break;
								}
							}
						}
					}
					else
					{
						if(_reds > 0)
						{
							m_pParent->m_colourseq = 1;
							goto _top;
						}
					}
				}


				////////////////////////////////////////////////////////////////////
				// work out which ball is closest to white (Jack)
				float closestDist = 10000;
				int closestBall = 0;
				CVector D;
				float d;

				if(TestFlag(m_pParent->m_ball[0]->m_flags, Flg_BALL_ACTIVE)) // only do if white is still on table
				for(n=1;n<=10;n++)
				if(n != ignoreBall)
				//if(RefereeBowlsBallOnTable(n))
				if(RefereeBowlsBallOnTableNotPotted(n))
				{
					D = m_pParent->m_ball[n]->m_P - m_pParent->m_ball[0]->m_P;
					d = D.Magnitude();
					if(d < closestDist)
					{
						closestDist = d;
						closestBall = n;
					}
				}
				m_pParent->m_bestScore = closestBall; // keep track of closest ball to white
				////////////////////////////////////////////////////////////////////

				
				// no balls left to play
				if(_blues == 0 && _reds == 0)
				{					
					// who is closest?
					if(closestBall >= 1 && closestBall <= 5) // 1P wins!
						m_pParent->m_gameOver = 2;
					else if(closestBall >= 6 && closestBall <= 10) // 2P wins!
						m_pParent->m_gameOver = 1;
					else
					{
						// draw! i.e both players have wasted all their bowls, only the white remains
						// lets make them start again! :)
						///////
						// give balls back to player(s)
						for(n=1;n<=10;n++)
						//if(RefereeBowlsBallOnTable(n))
						{						
							//if(m_pParent->m_ball[n]->m_P.x < -WORLD_X(591+65+120))
							//{
							//	char bbb[80];
							//	sprintf(bbb, "%d is back in play", n);
							//	ErrorMessage(m_pParent->m_parent, bbb);

								// reset this ball, so we can use it again
								ClearFlag(m_pParent->m_ball[n]->m_flags, Flg_BALL_ACTIVE | Flg_BALL_POTTED);
								m_pParent->m_ball[n]->m_P.x = 10000;
								m_pParent->m_ball[n]->m_P.y = 10000;
							//}
						}
						///////

						goto _start;
					}
				}

				return;
				

					/*
					if(_potted8ball != 0)
					{
						// *TODO - FOUL!!!! DO RERACK!!!!
						SetFlag(m_pParent->m_foul, Flg_FOUL_NORMAL | Flg_FOUL_GAMEOVER);
						goto end;
					}

					if(m_pParent->m_bTurn1P)
					{
						if(_pottedspots > 0 && _pottedstripes == 0)
							m_pParent->m_colourseq = 2;	// 1P going for spots
						else if(_pottedspots == 0 && _pottedstripes > 0)
							m_pParent->m_colourseq = 3;	// 1P going for stripes
					}
					else
					{
						if(_pottedspots > 0 && _pottedstripes == 0)
							m_pParent->m_colourseq = 3;	// 2P going for spots
						else if(_pottedspots == 0 && _pottedstripes > 0)
							m_pParent->m_colourseq = 2;	// 2P going for stripes
					}
						
					*/
					
			
				
	/////////////////////////////////////////////////////////////
	//char buf[80];
	//sprintf(buf, "-- m_colourseq = %d ---", m_pParent->m_colourseq);
	//ErrorMessage(m_pParent->m_parent, buf);

	/////////////////////////////////////////////////////////////
	// normal pots...
	if(!bWhiteInPocket)
	{			
		//MessageBox(0, "11","11",MB_OK);
		for(n=0;n<MAX_TOTALBALLS;n++)
		{
			if(TestFlag(m_pParent->m_ball[n]->m_flags, Flg_BALL_POTTED))
			{					
				

				if(m_pParent->m_bTurn1P) {
					//m_pParent->m_score1P += 1;//m_pParent->m_ball[n]->m_colour;
					//m_pParent->m_break1P += 1;//m_ball[n]->m_colour;
				}
				else {
					//m_pParent->m_score2P += 1;//m_pParent->m_ball[n]->m_colour;
					//m_pParent->m_break2P += 1;//m_ball[n]->m_colour;
				}
				
				

				m_pParent->m_bMiss = FALSE;		// potted ball
				m_pParent->m_foul = 0;				// no foul

				if(m_pParent->m_bTurn1P) {
					// 1P just potted a ball
					// after first pot, go into runout mode
					if( !(m_pParent->m_break2P & 0x01) )
						m_pParent->m_break2P |= 0x01;
				}
				else
				{
					// 2P just potted a ball
					// after first pot, go into runout mode
					if( !(m_pParent->m_break2P & 0x02) )
						m_pParent->m_break2P |= 0x02;
				}
				

				
				int tbl = GAMEAPPVIEW2->m_lobby->GetTableIdxFromPlayer(GAMEAPPVIEW2->m_playerSockIdx);
				if(tbl!=-1)
				{
					m_pParent->m_shotTime =  m_table[tbl].shotTime*50;
				}
									  
				/*
				if(_potted8ball >0)
				{
					// GAME_OVER! the other player losses!
					if(m_pParent->m_bTurn1P)
						m_pParent->m_gameOver = 2;
					else
						m_pParent->m_gameOver = 1;		
				}
				*/
			}
		}

		/*
		if(!m_colourseq) // if we are NOT on the 'colour seq.' at the end of the frame
		if(m_bMiss == FALSE)
		{
			if(m_rednext)
				m_rednext = FALSE;
			else
				m_rednext = TRUE;
		}
		*/
	}
	/////////////////////////////////////////////////////////////

end:;

	
	// if just done break! and we fouled!
	if(m_pParent->m_colourseq == 0)
		m_pParent->m_colourseq = 1;

	if(TestFlag(m_pParent->m_foul, Flg_FOUL_GAMEOVER))
	{
		// GAME_OVER! this player losses!
		if(m_pParent->m_bTurn1P)
			m_pParent->m_gameOver = 1;
		else
			m_pParent->m_gameOver = 2;		
	}

	/////////////////////////////////////////////////////////////
	// check if nothing was potted this time
	if(!m_pParent->m_gameOver)
	if(m_pParent->m_bMiss)
	{

		// if we've already potted a ball, but now we've missed or fouled, then NO run-out!
		//if(m_pParent->m_break2P == 1)
		//	m_pParent->m_break2P = -1;
		if(m_pParent->m_bTurn1P) {
			// 1P just missed/fouled a ball
			// if already potted a ball, but now missed!
			if( (m_pParent->m_break2P & 0x01) )
				m_pParent->m_break2P |= 0x04;
		}
		else
		{
			// 2P just missed/fouled a ball			
			// if already potted a ball, but now missed!
			if( (m_pParent->m_break2P & 0x02) )
				m_pParent->m_break2P |= 0x08;
		}

		//ErrorMessage(m_pParent->m_parent, "miss!");
//		// end of break sounds
//		if(g_gfxDetails.bSoundClapping)
//		{
//			if(m_bTurn1P)
//			{
//				if(m_break1P >= 20 && m_break1P < 100)
//				{
//					float loudness = 128 + 128*(((float)m_break1P - 20)/80);
//					Msfx_smpplayvol(&GAMEAPPVIEW->m_game_smp[SFX_20PLUSBREAK], loudness);
//				}
//				
//				/*
//				if(m_break1P >= 100)
//				{
//					Msfx_smpplayvol(&GAMEAPPVIEW->m_game_smp[SFX_CENTURY], 255);
//				}
//				*/
//				
//			}
//			else
//			{
//				if(m_break2P >= 20 && m_break2P < 100)
//				{
//					float loudness = 128 + 128*(((float)m_break1P - 20)/80);
//					Msfx_smpplayvol(&GAMEAPPVIEW->m_game_smp[SFX_20PLUSBREAK], loudness);
//				}
//  
//				/*
//				if(m_break2P >= 100)
//				{
//					Msfx_smpplayvol(&GAMEAPPVIEW->m_game_smp[SFX_CENTURY], 255);
//				}
//				*/
//			}
//		}
			

		//MessageBox(0, "13","13",MB_OK);
		//if(m_pParent->m_bPracticeMode && !GAMEAPPVIEW2->m_bReplayMode && !GAMEAPPVIEW->m_bPlayOffline)
		if(m_pParent->m_bPracticeMode 
			#ifdef THE_POOL_CLUB
			&& m_type != CTTYPE_BOWLS
			#endif //THE_POOL_CLUB
			)
		{
			// force no fouls in practice
			//m_foul = 0;
			//m_break1P = 0;
		}
		else
		{
			// decide who's turn it is next?
			bNextPlayer = TRUE;
			m_pParent->m_bConcededGame = FALSE; // clear this flag (now that the turn has swapped over, we can allow gameOver to continue...)
			GAMEAPPVIEW2->m_bFlashWindow = TRUE;

			if(TestFlag(m_pParent->m_foul, Flg_FOUL_NORMAL))
				m_pParent->m_shotTime = 60*50;
			else
			{
				// if no foul, then give usual time for shot
				int tbl = GAMEAPPVIEW2->m_lobby->GetTableIdxFromPlayer(GAMEAPPVIEW2->m_playerSockIdx);
				if(tbl!=-1)
				{
					m_pParent->m_shotTime =  m_table[tbl].shotTime*50;
				}
			}
			

			//ErrorMessage(m_parent, "1. reg break!");

			if(m_pParent->m_bTurn1P) {
				m_pParent->m_bTurn1P = FALSE;
				//m_pParent->m_break2P = 0;

				//m_pParent->m_break2P = 60*50;

				/*
				// is it a high break?
				//if(m_break1P >= 50)
				{
					if(m_highBreaks1P[0] <= m_highBreaks1P[1])
					{
						if(m_break1P > m_highBreaks1P[0])
							m_highBreaks1P[0] = m_break1P;
					}
					else
					{
						if(m_break1P > m_highBreaks1P[1])
							m_highBreaks1P[1] = m_break1P;
					}
				}
				*/
			}
			else {
				m_pParent->m_bTurn1P = TRUE;
				//m_pParent->m_break1P = 0;

				//m_pParent->m_break2P = 60*50;

				/*
				// is it a high break?
				//if(m_break2P >= 50)
				{
					if(m_highBreaks2P[0] <= m_highBreaks2P[1])
					{
						if(m_break2P > m_highBreaks2P[0])
							m_highBreaks2P[0] = m_break2P;
					}
					else
					{
						if(m_break2P > m_highBreaks2P[1])
							m_highBreaks2P[1] = m_break2P;
					}
				}
				*/
			}
		}

		// if we are NOT on the 'colour seq.' at the end of the frame
		//if(!m_colourseq)
		//	m_rednext = TRUE;
	}
	/////////////////////////////////////////////////////////////


	RefereeBowlsUpdatePots();


#if 0
	/////////////////////////////////////////////////////////////
	// clear all 'POTTED' balls
	for(n=0;n<MAX_TOTALBALLS;n++)
		if(TestFlag(m_pParent->m_ball[n]->m_flags, Flg_BALL_POTTED))
		{
			ClearFlag(m_pParent->m_ball[n]->m_flags, Flg_BALL_POTTED);
			
			///////////////////////////////////////////////////////////
			///////////////////////////////////////////////////////////
			// position potted ball on the side of the screen
			
			if(m_pParent->m_ball[n]->m_colour == WHITE) continue;

			/*
			if(m_pParent->m_ball[n]->m_colour == 8)
			{
					
			}
			*/

			//SetFlag(m_pParent->m_ball[n]->m_flags, Flg_BALL_ACTIVE);
			if(m_pParent->m_colourseq == 2)
			{
				//ErrorMessage(m_pParent->m_parent, "pot [colourseq = 2]");
				
				if(m_pParent->m_ball[n]->m_colour <= 7)
				{																													 
					m_pParent->m_ball[n]->Init(-1500, (m_pParent->m_score1P*BALL_RADIUS*3), 0, m_pParent->m_ball[n]->m_colour, -90.0f*DEG2RAD, 110.0f*DEG2RAD, 0.0f*DEG2RAD);
					m_pParent->m_score1P++;
				}
				else if(m_pParent->m_ball[n]->m_colour >= 9)
				{
					m_pParent->m_ball[n]->Init(1500, (m_pParent->m_score2P*BALL_RADIUS*3), 0, m_pParent->m_ball[n]->m_colour, -90.0f*DEG2RAD, 70.0f*DEG2RAD, 0.0f*DEG2RAD);
					m_pParent->m_score2P++;
				}
			}
			else if(m_pParent->m_colourseq == 3)
			{
				//ErrorMessage(m_pParent->m_parent, "pot [colourseq = 3]");

				if(m_pParent->m_ball[n]->m_colour <= 7)
				{
					m_pParent->m_ball[n]->Init(1500, (m_pParent->m_score2P*BALL_RADIUS*3), 0, m_pParent->m_ball[n]->m_colour, -90.0f*DEG2RAD, 110.0f*DEG2RAD, 0.0f*DEG2RAD);
					m_pParent->m_score2P++;
				}
				else  if(m_pParent->m_ball[n]->m_colour >= 9)
				{
					m_pParent->m_ball[n]->Init(-1500, (m_pParent->m_score1P*BALL_RADIUS*3), 0, m_pParent->m_ball[n]->m_colour,  -90.0f*DEG2RAD, 70.0f*DEG2RAD, 0.0f*DEG2RAD);
					m_pParent->m_score1P++;
				}
				
			}
			///////////////////////////////////////////////////////////
			///////////////////////////////////////////////////////////
			
			
		}
	/////////////////////////////////////////////////////////////
#endif //0


	// re-spot balls
	RefereeBowlsReSpot();

		
		///////////
		// for practice mode : fixes "pot yellow aand white simul. bug" ***
		if(m_pParent->m_bMiss)
		{
			//MessageBox(0, "13b","13b",MB_OK);
			if(m_pParent->m_bPracticeMode && !GAMEAPPVIEW2->m_bPlayOffline)
			{
				// force no fouls in practice
				m_pParent->m_foul = 0;
				//m_pParent->m_break1P = 0;
			}
		}
		///////////

		
		if(TestFlag(m_pParent->m_foul, Flg_FOUL_NORMAL))
		{
			m_pParent->m_bBallInHand = 1;
			m_pParent->m_bStandUpFromShot = 1;
		}

}


///////////////////////////////////////////////////////////////////////////////////////////////////
//
//
void CCustomTable::RefereeBowlsReSpot(void)
{
	int n;
	int m;
	int nextspot;
	int dir;
	BOOL successful;
	BOOL reds;

	float f1,f2,f3;

	
	////////////////////////////////////////////////////////////////////////////////////////////////////////////
	
	//ErrorMessage(m_pParent->m_parent, "Respot:(1,2,3)");

	// for balls in pockects
	for(n=MAX_TOTALBALLS-1; n >= 0;n--)
	//if((m_ball[n]->m_colour <= BLACK && m_ball[n]->m_colour >= YELLOW) || (m_ball[n]->m_colour == WHITE))
	if((m_pParent->m_ball[n]->m_colour == WHITE)) // ***** TODO ***** -> 8 BALL
	{	
		// temp. code for WHITE ball
		if(m_pParent->m_ball[n]->m_colour == WHITE)
			//if(!TestFlag(m_pParent->m_ball[n]->m_flags, Flg_BALL_ACTIVE))
			if(TestFlag(m_pParent->m_foul, Flg_FOUL_WHITEPOTTED))
				{
					// position ball on its original starting position or...
					m_pParent->m_ball[n]->m_POld = m_pParent->m_ball[n]->m_P = m_pParent->m_ball[n]->m_PSpot;
					
					//ErrorMessage(m_pParent->m_parent, "whiteball-1");

					if( Referee8BallDropBallOnTable(WHITE, m_pParent->m_ball[n]->m_P.x, m_pParent->m_ball[n]->m_P.y ) )
					{
						//ErrorMessage(m_pParent->m_parent, "whiteball-2");
					}
					else					
					{
						//ErrorMessage(m_pParent->m_parent, "whiteball-3");
						// ...otherwise position anywhere behind the 'Head String'
						CVector Pgreen, Pyellow;
						CVector P;
						float radius;
						//Pgreen = CVector(-WORLD_X(912),-WORLD_Y(395-12), BALL_POSITIONZOFFSET);
						//Pyellow = CVector(-WORLD_X(912),-WORLD_Y(199+12), BALL_POSITIONZOFFSET);
						Pgreen = CVector(-WORLD_X(591+65+120/*912+5*/), -600, BALL_POSITIONZOFFSET);
						Pyellow = CVector(-WORLD_X(591+65+120/*912+5*/), 600, BALL_POSITIONZOFFSET);
						float w,h;
						float x,y;
						
						//[f1 = Pyellow.y - Pgreen.y;]
						f1 = Pyellow.y;
						f1 = f1 - Pgreen.y;
						
						f1 = f1 * 0.9f;
						h =	f1;
						//h =	(Pyellow.y - Pgreen.y)*0.9f;
						w = h*0.5f;
						for(x = 0.0f;x<w;x+=BALL_RADIUS)
						for(y = 0.0f;y<h;y+=BALL_RADIUS)
						{
							
							//[f1 = Pyellow.x - x;]
							f1 = Pyellow.x;
							f1 = f1 - x;
							
							//[f2 = Pyellow.y - Pgreen.y;]
							f2 = Pyellow.y;
							f2 = f2 - Pgreen.y;
							
							f2 = f2 * 0.05f;
							
							//[f3 = Pyellow.y - y;]
							f3 = Pyellow.y;
							f3 = f3 - y;
							
							f3 = f3 - f2;
							if( Referee8BallDropBallOnTable(WHITE, f1, f3) )
							//if( DropBallOnTable(WHITE, Pyellow.x - x, Pyellow.y - y - (Pyellow.y - Pgreen.y)*0.05f) )
								goto whiteSpotted;
						}
						whiteSpotted:;
					}
					
//					m_ball[n]->m_POld = m_ball[n]->m_P = m_ball[n]->m_PSpot;
					m_pParent->m_ball[n]->m_W = m_pParent->m_ball[n]->m_V = m_pParent->m_ball[n]->m_VOld = CVector(0,0,0);
	
					// turn ball on
					m_pParent->m_ball[n]->m_flags = 0;
					SetFlag(m_pParent->m_ball[n]->m_flags, Flg_BALL_ACTIVE);

					m_pParent->m_bBallInHand = 1;
					m_pParent->m_bStandUpFromShot = 1;


					continue;
				}
		
		#if 0
		///&&&&&&&&&&
		
		/////////////////////////////////////////////////////////////////////
		// (1) try and spot on starting position
		/////////////////////////////////////////////////////////////////////
		successful = TRUE;
		for(m=0;m<MAX_TOTALBALLS;m++)
		if(n!=m)
		{
			if(!m_ball[n]->TryReSpot1(m_ball[m]))
				successful = FALSE;
		}
		/////////////////////////////////////////////////////////////////////

		if(successful)
			m_ball[n]->ReSpotSuccessful();
		else
		{
			/////////////////////////////////////////////////////////////////////
			// (2) try next vacant spot, starting with highest colour spot first
			/////////////////////////////////////////////////////////////////////
			for(nextspot=MAX_TOTALBALLS-1; nextspot > 0;nextspot--)
				if(m_ball[nextspot]->m_colour <= BLACK && m_ball[nextspot]->m_colour >= YELLOW)
				{
					successful = TRUE;
					for(m=0;m<MAX_TOTALBALLS;m++)
					if(n!=m)
						{
						if(!m_ball[n]->TryReSpot2(m_ball[m], m_ball[nextspot]))
							successful = FALSE;
						}
					
					if(successful) {
						m_ball[n]->ReSpotSuccessful();
						goto end;
					}
				
				}
			/////////////////////////////////////////////////////////////////////

			/////////////////////////////////////////////////////////////////////
			// (3) try and spot behind starting position in a direct line
			/////////////////////////////////////////////////////////////////////					
			// check to left side of spot (dir = -1)..., then the right side (dir = 1)...
			if(!TestFlag(m_ball[n]->m_flags, Flg_BALL_ACTIVE))
			for(dir = -1; dir <= 1; dir+=2)
			{
				// position ball on its original starting position
				m_ball[n]->m_POld = m_ball[n]->m_P = m_ball[n]->m_PSpot;
				m_ball[n]->m_W = m_ball[n]->m_V = m_ball[n]->m_VOld = CVector(0,0,0);

				do
				{
					f1 = BALL_RADIUS/20.0f;
					f1  = f1 * dir;
					m_ball[n]->m_P.x -= f1;
					//m_ball[n]->m_P.x -= (BALL_RADIUS/20)*dir;

					successful = TRUE;
					for(m=0;m<MAX_TOTALBALLS;m++)
					if(n!=m)
					{
						if(!m_ball[n]->TryReSpot3(m_ball[m]))
							successful = FALSE;
					}

					if(successful) {
						m_ball[n]->ReSpotSuccessful();
						goto end;
					}

				}
				while((m_ball[n]->m_P.x >= (-1736+BALL_RADIUS)) ||
					  (m_ball[n]->m_P.x <= (1736-BALL_RADIUS)));   // bottom cushion + BALL_RADIUS || top cushion - BALL_RADIUS
				// *TODO: note: the above boundary check is incorrect!!!, it should be using the actual 3d world "Cushions"
			}
				/////////////////////////////////////////////////////////////////////

				// we are in trouble if it gets this far!!!

		end:;
		}		
		///&&&&&&&&&&
		#endif //0
	}
}

BOOL CCustomTable::RefereeBowlsDropBallOnTable(int n, float x, float y, BOOL bDrop)
{
	// check if ball 'n' is on top of another ball
	int m;
	for(m=0;m<MAX_TOTALBALLS;m++)
	if(m!=n)
	if(m_pParent->m_ball[m]->Proximity(x, y, 0, BALL_RADIUS*2.0f) == TRUE)
	{
		//ErrorMessage(m_pParent->m_parent, "FALSE");
		return FALSE;
	}
	
	// check for ball n inside the D
	if(!m_pParent->m_bPracticeMode) // should be !
	{
		CVector B = CVector(x,y,BALL_POSITIONZOFFSET);
		CVector Pgreen, Pbrown;
		CVector P;
		float radius;
		//Pgreen = CVector(-WORLD_X(912),-WORLD_Y(395-12), BALL_POSITIONZOFFSET);
		//Pbrown = CVector(-WORLD_X(912),-WORLD_Y(297),BALL_POSITIONZOFFSET);
		Pgreen = CVector(-WORLD_X(591+65+120/*912+5*/), -600, BALL_POSITIONZOFFSET);
		Pbrown = CVector(-WORLD_X(591+65+120/*912+5*/), 0, BALL_POSITIONZOFFSET);

		//[P = Pgreen - Pbrown;]
		P = Pgreen;
		P = P - Pbrown;

		radius = P.Magnitude2();
		
		//[P = B - Pbrown;]
		P = B;
		P = P - Pbrown;

		if(TestFlag(m_pParent->m_foul, Flg_FOUL_WHITEPOTTED) || TestFlag(m_pParent->m_foul, Flg_FOUL_NORMAL)
			|| TestFlag(m_pParent->m_foul, Flg_FOUL_BALLINHAND))
			Pbrown.x = 1235;	// give ball-in-hand access to whole table

		//if( P.Magnitude2() < radius)
		if((B.y < -Pgreen.y) && (B.y > Pgreen.y))
		{			
			if((B.x < Pbrown.x) && (B.x > -1235))
			{
				// update the drop ball's coordinates
				if(bDrop)
				{
					m_pParent->m_ball[n]->m_P.x = x;
					m_pParent->m_ball[n]->m_P.y = y;
					#ifdef FIXEDMODE
					m_pParent->m_ball[n]->m_V.x = 0;
					m_pParent->m_ball[n]->m_V.y = 0;
					m_pParent->m_ball[n]->m_V.z = 0;
					m_pParent->m_ball[n]->UpdateFixeds();
					#endif

					m_pParent->m_ball[n]->AdjustOrientation();
					m_pParent->m_ball[n]->m_POld = m_pParent->m_ball[n]->m_P;
					#ifdef FIXEDMODE
					m_pParent->m_ball[n]->UpdateFixeds();
					#endif
				}
				return TRUE;
			}
		}
		return FALSE;
	}
	
	// update the drop ball's coordinates
	if(bDrop)
	{
		m_pParent->m_ball[n]->m_P.x = x;
		m_pParent->m_ball[n]->m_P.y = y;
		#ifdef FIXEDMODE
		m_pParent->m_ball[n]->m_V.x = 0;
		m_pParent->m_ball[n]->m_V.y = 0;
		m_pParent->m_ball[n]->m_V.z = 0;
		m_pParent->m_ball[n]->UpdateFixeds();
		#endif
		m_pParent->m_ball[n]->AdjustOrientation();
		m_pParent->m_ball[n]->m_POld = m_pParent->m_ball[n]->m_P;
		#ifdef FIXEDMODE
		m_pParent->m_ball[n]->UpdateFixeds();
		#endif
		//ErrorMessage(m_pParent->m_parent, "TRUE");

	}
	return TRUE;
}


void CCustomTable::RefereeBowlsUpdatePots(void)
{
	int ballColour;
	float fx,fy;
	int x,y;
	
	int n;
	for(n = 0; n < 16; n++)
	{
		g_pottedSpots[n] = -1;
		g_pottedStripes[n] = -1;
	}

	// scan thru all potted balls, and see who's potted what
	int m;
	for(m=0;m<MAX_TOTALBALLS;m++)
	if(!TestFlag(m_pParent->m_ball[m]->m_flags, Flg_BALL_ACTIVE))
	{
		fx = m_pParent->m_ball[m]->m_P.x;
		fy = m_pParent->m_ball[m]->m_P.y;

		
		// what has P1 potted
		if(fx > 20001)
		{
			fx-=20000;
			fx /= 100;
			if(fx>=0 && fx<= 15)
			{
				x = fx;
				y = fy;
				if((m_pParent->m_ball[m]->m_colour >= 1) &&  (m_pParent->m_ball[m]->m_colour <= 7))
					g_pottedSpots[x] = m;
				else if((m_pParent->m_ball[m]->m_colour >= 9) &&  (m_pParent->m_ball[m]->m_colour <= 15))
					g_pottedStripes[x] = m;					
				
			}
		}

		// what has P2 potted
		if(fy > 20001)
		{
			fy-=20000;
			fy /= 100;
			if(fy>=0 && fy<= 15)
			{
				x = fx;
				y = fy;
				if((m_pParent->m_ball[m]->m_colour >= 1) &&  (m_pParent->m_ball[m]->m_colour <= 7))
					g_pottedSpots[y] = m;
				else if((m_pParent->m_ball[m]->m_colour >= 9) &&  (m_pParent->m_ball[m]->m_colour <= 15))
					g_pottedStripes[y] = m;					
			}
		}
	}
	/*			
	fy = 0;
	ballColour = 0;

	// draw spots on left-hand side		
	for(m=0;m<16;m++)
	if(g_pottedSpots[m] != -1)
	{			
		ballColour = m_pParent->m_ball[g_pottedSpots[m]]->m_colour; 
		
		// *TODO: 
		//if(m_pParent->m_colourseq == 2) // 1P=SPOTS & 2P=STRIPES
		//	draw2DBall(ballColour, x = LEFT, y = fy);
		//else if(m_pParent->m_colourseq == 3) // 1P=STRIPES & 2P=SPOTS
		//	draw2DBall(ballColour, x = RIGHT, y = fy);

		fy += 32;
	}

	char bbb[80];
	sprintf(bbb, "spots potted = %f", fy / 32); 
	ErrorMessage(m_pParent->m_parent, bbb);

	// draw stripes on left-hand side		
	fy = 0;
	ballColour = 0;
	for(m=0;m<16;m++)
	if(g_pottedStripes[m] != -1)
	{			
		ballColour = m_pParent->m_ball[g_pottedStripes[m]]->m_colour; 
		
		// *TODO: 
		//if(m_pParent->m_colourseq == 2) // 1P=SPOTS & 2P=STRIPES
		//	draw2DBall(ballColour, x = RIGHT, y = fy);
		//else if(m_pParent->m_colourseq == 3) // 1P=STRIPES & 2P=SPOTS
		//	draw2DBall(ballColour, x = LEFT, y = fy);

		fy += 32;
	}		
	
	sprintf(bbb, "stripes potted = %f", fy / 32); 
	ErrorMessage(m_pParent->m_parent, bbb);
	*/
}


//////
//////


void CCustomTable::Draw3D(void)
{
	switch(m_type)
	{
		case CTTYPE_SNOOKER:
		case CTTYPE_SNOOKER10:
		case CTTYPE_SNOOKER6:
		case CTTYPE_RESPOTBLACK:
		case CTTYPE_BILLIARDS:
			Draw3DSnooker();
			break;
		case CTTYPE_8BALL:
			Draw3D8Ball();
			break;
		case CTTYPE_9BALL: 
			Draw3D9Ball();
			break;
		case CTTYPE_ROTATIONBALL: 
			Draw3DRotationBall();
			break;
		case CTTYPE_BOWLS: 
			Draw3DBowls();
			break;
		case CTTYPE_6BALL: 
			Draw3D6Ball();
			break;
		case CTTYPE_10BALL: 
			Draw3D10Ball();
			break;
		case CTTYPE_BREAK: 
			Draw3DRotationBall();
			break;
	}

	#ifdef THE_POOL_CLUB // iPool
	Draw3DPocketObjects();
	#endif //THE_POOL_CLUB // iPool
}

void CCustomTable::Draw3DPocketObjects(void)
{
	// draw pockets mesh objects for custom table
	M3d_object *M3d_pocketObject_ptr;
	M3d_pocketObject_ptr = &m_M3d_PocketObject;
	M3d_vtx pos,rot,scale;
		
	///*
	// Set up material for ball //
	M3d_pocketObject_ptr->mat[0].d3dm.Diffuse.r = 0.0f;
	M3d_pocketObject_ptr->mat[0].d3dm.Diffuse.g = 0.0f;
	M3d_pocketObject_ptr->mat[0].d3dm.Diffuse.b = 0.0f;
	M3d_pocketObject_ptr->mat[0].d3dm.Diffuse.a = 1.0f;	

	M3d_pocketObject_ptr->mat[0].d3dm.Specular.r = 0.1f;
	M3d_pocketObject_ptr->mat[0].d3dm.Specular.g = 0.1f;
	M3d_pocketObject_ptr->mat[0].d3dm.Specular.b = 0.1f;
	M3d_pocketObject_ptr->mat[0].d3dm.Specular.a = 1.0f;
	
	M3d_pocketObject_ptr->mat[0].d3dm.Ambient.r = 0.0f;
	M3d_pocketObject_ptr->mat[0].d3dm.Ambient.g = 0.0f;
	M3d_pocketObject_ptr->mat[0].d3dm.Ambient.b = 0.0f;
	M3d_pocketObject_ptr->mat[0].d3dm.Ambient.a = 0.0f;

	M3d_pocketObject_ptr->mat[0].d3dm.Emissive.r = 0;//0.05f;
	M3d_pocketObject_ptr->mat[0].d3dm.Emissive.g = 0;//0.05f;
	M3d_pocketObject_ptr->mat[0].d3dm.Emissive.b = 0;//0.05f;
	M3d_pocketObject_ptr->mat[0].d3dm.Emissive.a = 1.0f;

	M3d_pocketObject_ptr->mat[0].d3dm.Power = D3D_MATERIAL_POWER;//70;
	//*/
		
	// only render pocker objects (i.e. pockets that are not part of the 3D table mesh)
	for(int n=0;n<m_pocket_n;n++)
	if(m_pocketType[n] == POCKET_OBJECT)
	{
		CVector Pocket;
		Pocket = m_pocketPos[n];
		scale[0] = 0.1f;
		scale[1] = 0.1f;
		scale[2] = 0.1f;
				
		scale[0] = 1.0f;
		scale[1] = 1.0f;
		scale[2] = 1.0f;

		pos[0] = Pocket.x;
		pos[1] = Pocket.y;
		pos[2] = Pocket.z;
		
		rot[0] = 0;
		rot[1] = 0;
		rot[2] = 0;

		//int tableTypeIdx = 0;
		//int tblIdx = GAMEAPPVIEW2->m_lobby->GetTableIdxFromPlayer(GAMEAPPVIEW2->m_playerSockIdx);
		//if(tblIdx!=-1)
		//{
		//	tableTypeIdx = m_table[tblIdx].tableType;
		//}
		
		M3d_transformobject(pos, rot, scale); // For still (non keyframes) meshes.
		M3d_objectdraw(M3d_pocketObject_ptr);


		if(m_tableType == 4) // Black-Hole
		{					
			M3d_pocketObject_ptr->mat[0].d3dm.Diffuse.r = 1.0f;
			M3d_pocketObject_ptr->mat[0].d3dm.Diffuse.g = 0.0f;
			M3d_pocketObject_ptr->mat[0].d3dm.Diffuse.b = 0.0f;
			M3d_pocketObject_ptr->mat[0].d3dm.Diffuse.a = 0.25f;
			
			scale[0] = 1.0f*4;
			scale[1] = 1.0f*4;
			scale[2] = 1.0f*4;

			pos[2] = Pocket.z + BALL_RADIUS*0.125f;
			
			M3d_transformobject(pos, rot, scale); // For still (non keyframes) meshes.
			M3d_objectdraw(M3d_pocketObject_ptr);
		}
	}
}

void CCustomTable::Draw3DSnooker(void)
{
}

void CCustomTable::Draw3D8Ball(void)
{
}

void CCustomTable::Draw3D9Ball(void)
{
}

void CCustomTable::Draw3DRotationBall(void)
{
}

void CCustomTable::Draw3DBowls(void)
{
}

void CCustomTable::Draw3D6Ball(void)
{
}

void CCustomTable::Draw3D10Ball(void)
{
}


// [CSnooker] ///////////////////////////////////////////////////////////////////////////////////////////////////////////

CSnooker::CSnooker(CWnd *pParent)
{
#ifndef MAIN_SERVER

	int n;

	g_snooker = this;

	m_parent = pParent;
	m_firstTime = TRUE;
	m_firstFewFrames = 1;

	m_timeDelta = 0.0f;
	
	m_customTable = new CCustomTable(this);	 // Sets up custom table (loads balls etc.)

	m_M3d_Ball_ptr = m_customTable->m_M3d_Ball_med; // default pointer to medium detail balls		
	m_M3d_Table_ptr = &m_customTable->m_M3d_Table;	

	// Load the balls Shadow.
	Mmain_log("CSnooker, M3d_objectload (m_M3d_Ball_Shadow)");
	M3d_objectload(&m_M3d_Ball_Shadow,"Graphics/m3d/Shadow.m3d", M3D_OBJECT_STILL);
	
	// Load 3d cue object

	
	char fname[80];
	Mmain_log("CSnooker, M3d_objectload (Loading Cues)");
	
	for(n=0;n<MAX_CUES;n++)
	{
	sprintf(fname,"Graphics/m3d/Cues/%s.m3d", cue_lst[n]);
	M3d_objectload(&m_M3d_Cue[n], fname, M3D_OBJECT_STILL);
	}
	
	// Load the Pen Mesh //
	M3d_objectload(&m_M3d_Pen, "Graphics/m3d/pen.m3d", M3D_OBJECT_STILL);
	// Load the Purchased Text Mesh //
	M3d_objectload(&m_M3d_Purchased, "Graphics/m3d/purchased.m3d", M3D_OBJECT_STILL);

	/*
	// load the arrow sprites		
	Mmain_log("CSnooker, Mgfx_sprload (m_spriteViewTablesButton)");
	
	Mgfx_sprload(&m_spriteViewTablesButton[0],"Graphics/skin/button_viewtables.bmp",MGFX_SPR_VRAM);
	Mgfx_sprload(&m_spriteViewTablesButton[1],"Graphics/skin/button_viewmsgb.bmp",MGFX_SPR_VRAM);
	 	
	
	if(g_build.gameType == GAMETYPE_POOL)
	{			
			Mmain_log("CSnooker, Mgfx_sprload (m_spritePBall)");
		for(n=0;n<(15+2+2+4);n++)
		{
			Mgfx_sprload(&m_spritePBall[n],PoolBallFName[n],MGFX_SPR_VRAM|MGFX_SPR_TRANS);
			//Mgfx_sprcolorkey(&m_spritePBall[n], Mgfx_rgbcolor(128,0,255));
			Mgfx_sprsettrans(&m_spritePBall[n], 0,0); // Transparancy seems to be a slightly different color (D3DXLoadTextureFromFile...)
		}

	}

	#ifdef LOAD_NEXTBALLLIST
	// small pool balls [16x16]
	if(g_build.gameType == GAMETYPE_POOL)
	{			
			Mmain_log("CSnooker, Mgfx_sprload (m_spritePBallSmall");
		for(n=0;n<(15+2+4);n++)
		{
			Mgfx_sprload(&m_spritePBallSmall[n],PoolBall16FName[n],MGFX_SPR_VRAM|MGFX_SPR_TRANS);
			//Mgfx_sprcolorkey(&m_spritePBallSmall[n], Mgfx_rgbcolor(128,0,255));
			Mgfx_sprsettrans(&m_spritePBallSmall[n], 0,0);
		}

	}
	#endif //LOAD_NEXTBALLLIST
	*/		
	if(g_build.gameType == GAMETYPE_POOL)
	{
		// load the cue-ball sprites		
		Mmain_log("CSnooker, Mgfx_sprload (m_spriteCueBallTexture)");
		m_spriteCueBallTexture = new IDirect3DTexture9*[2]; 
		for(n=0;n<2;n++)
		{										  
			D3DXCreateTextureFromFile(g_pDevice, CueBallTexturesFName[n], &m_spriteCueBallTexture[n]);
		}
	}
		
	Mmain_log("CSnooker, Mgfx_sprload (m_spritePower)");



	m_spriteArrow3D = 0;
	m_TextureArrowWhite = 0;
	m_TextureDisc = 0;
	m_TextureDTop = 0;
	m_TextureDBot = 0;
	m_TextureWhite = 0;
	m_TextureRing = 0;
	m_TextureQMark = 0;
	m_spriteCameraFree = 0;
	m_spriteCameraGame = 0;

	D3DXCreateTextureFromFile(g_pDevice, "Graphics/textures/arrowwhite.png", &m_spriteArrow3D);
	D3DXCreateTextureFromFile(g_pDevice, "Graphics/textures/arrowwhite.png", &m_TextureArrowWhite);
	D3DXCreateTextureFromFile(g_pDevice, "Graphics/textures/qmark.png", &m_TextureQMark);
	D3DXCreateTextureFromFile(g_pDevice, "Graphics/textures/ring.png", &m_TextureRing);
	D3DXCreateTextureFromFile(g_pDevice, "Graphics/textures/disc.png", &m_TextureDisc);
	D3DXCreateTextureFromFile(g_pDevice, "Graphics/textures/dtop.png", &m_TextureDTop);
	D3DXCreateTextureFromFile(g_pDevice, "Graphics/textures/dbot.png", &m_TextureDBot);
	D3DXCreateTextureFromFile(g_pDevice, "Graphics/textures/baulkline.png", &m_TextureWhite);
	D3DXCreateTextureFromFile(g_pDevice, "Graphics/textures/camerafree.png", &m_spriteCameraFree);
	D3DXCreateTextureFromFile(g_pDevice, "Graphics/textures/camerafree.png", &m_spriteCameraGame);

	
/*
	#ifdef LOAD_QMARK
	Mgfx_sprload(&m_spriteQMark3D,"Graphics/HUD/questionmark.bmp",MGFX_SPR_VRAM | MGFX_SPR_TEXTURE);
	#endif //LOAD_QMARK
*/
	// create snooker ball objects
	Mmain_log("CSnooker, Creating snooker balls");
	for(n=0;n<MAX_TOTALBALLS;n++)
	{
		m_ball[n] = new CBall(this);
	}
	m_testBall = new CBall(this);
	m_tmpBall = new CBall(this);	

	
	// setup a 2D plate & cube primitives
	Mmain_log("CSnooker, Creating snooker GFX primitives");
	InitMeshes();
	

	m_seat = -1;
	m_bPracticeMode = FALSE;
	m_bPracticeModeAlive = 0;
	m_bComputerMode = FALSE;//TRUE;
	m_bComputerModeAlive = 0;
	m_playersType = -1;//PLAYERS_1PVSCPU;
	m_wildRackType = 0; // no WILD table rack types (default)

	#ifdef CUEERROR_DEFAULTON
	//m_bCueError = TRUE;
	m_cueError = 1;		// semi-pro
	#else
	//m_bCueError = FALSE;
	m_cueError = 0;		// beginner
	#endif

	m_bRated = TRUE;
	m_bMissRule = FALSE;
	m_bCallShot = FALSE;

	m_snookerAI = new CSnookerAI(this);	

	g_M3d_Ball_ptr = m_M3d_Ball_ptr;
	m_replayname1[0] = 0;
	m_replayname2[0] = 0;
	m_cameraOn = 0;

	m_customTable->PoolTableChange(&m_customTable->m_M3d_proTable);

#endif // END OF NOT MAIN_SERVER

	///////////////////////////
}

CSnooker::~CSnooker()
{
	int n;
	delete m_snookerAI;
	delete m_customTable;

	KillAvatars();
	// deinit cube texture
	//DeinitTexture();
	
	for(n=0;n<MAX_TOTALBALLS;n++)
		delete m_ball[n];
	delete m_testBall;
	delete m_tmpBall;

	d3d::Release<IDirect3DTexture9*>(m_spriteArrow3D);
	d3d::Release<IDirect3DTexture9*>(m_TextureArrowWhite);
	d3d::Release<IDirect3DTexture9*>(m_TextureQMark);
	d3d::Release<IDirect3DTexture9*>(m_TextureRing);
	d3d::Release<IDirect3DTexture9*>(m_TextureDisc);
	d3d::Release<IDirect3DTexture9*>(m_TextureDTop);
	d3d::Release<IDirect3DTexture9*>(m_TextureDBot);
	d3d::Release<IDirect3DTexture9*>(m_TextureWhite);
	d3d::Release<IDirect3DTexture9*>(m_spriteCameraFree);
	d3d::Release<IDirect3DTexture9*>(m_spriteCameraGame);

	if(g_build.gameType == GAMETYPE_POOL)
	{
		d3d::Release<IDirect3DTexture9*>(m_spriteCueBallTexture[0]);
		d3d::Release<IDirect3DTexture9*>(m_spriteCueBallTexture[1]);

	}
}

void CSnooker::LoadAvatars(void)
{
	char fnamestr[80];
	int n;
	FILE *fptr;
	m_spriteAvatar_n=0;
	for(n=0;n<MAX_AVATARS;n++)
	{
	sprintf(fnamestr,"Avatars/Generic/av%d.bmp",n+1);
	if( (fptr = fopen(fnamestr,"rb")) == NULL) break;
	fclose(fptr);

	Mgfx_sprload(&m_spriteAvatar[n],fnamestr,MGFX_SPR_VRAM);//|MGFX_SPR_TRANS);
	m_spriteAvatar_n++;
	}

}

void CSnooker::KillAvatars(void)
{
	
}


void CSnooker::SetMouseSensitivity(int type)
{


	g_gfxDetails.iOptionsSensitivity = type;

	// 5 is default
	float mult = type;
	mult = (mult / 5.0f);

	m_mouseCueSensitivity = MOUSE_CUE_SENSITIVY_DEFAULT * mult;

	// Check Check Mark on menu item

	CMenu *menu = AfxGetMainWnd()->GetMenu();
	CMenu *subMenu;
	CMenu *subSubMenu;
	
	if(menu)
	{
		subMenu = menu->GetSubMenu(OPTIONS_MENU);
		if(subMenu)
		{
			subSubMenu = subMenu->GetSubMenu(OPTIONS_MOUSE_SENSITIVITY);
			if(subSubMenu)
			{
				// Uncheck everything
				for(int i=0;i<9;i++)
				{
					subSubMenu->CheckMenuItem(i, MF_UNCHECKED | MF_BYPOSITION);
				}

				// Check item we clicked
				subSubMenu->CheckMenuItem((type-1), MF_CHECKED | MF_BYPOSITION);
			}
		}
	}

}

void CSnooker::Create(void)
{
	SetMouseSensitivity(g_gfxDetails.iOptionsSensitivity);
	// m_mouseCueSensitivity = MOUSE_CUE_SENSITIVY_DEFAULT;
	
// menus
	
	// set menu 'players' - '1P Vs CPU'
	CMenu *menu = AfxGetMainWnd()->GetMenu();
	CMenu *subMenu;



	if(menu)
	{
		m_playersType = -1;//PLAYERS_1PVS2P;//PLAYERS_1PVSCPU;
		subMenu = menu->GetSubMenu(PLAYERS_MENU);
		if(subMenu)
		{
			//subMenu->CheckMenuItem(PLAYERS_1PVS2P, MF_CHECKED | MF_BYPOSITION);
			//subMenu->CheckMenuItem(PLAYERS_1PVSCPU, MF_CHECKED | MF_BYPOSITION);
			//subMenu->CheckMenuItem(PLAYERS_CPUVS1P, MF_UNCHECKED | MF_BYPOSITION);
		}
	}


	// set menu option performance
	if(g_gfxDetails.bHighPerformance)
	{
		g_gfxDetails.bHighPerformance = TRUE;
		if(menu)
		{
			subMenu = menu->GetSubMenu(OPTIONS_MENU);
			if(subMenu)
				subMenu->CheckMenuItem(OPTIONS_HIGH_PERFORMANCE, MF_CHECKED | MF_BYPOSITION);
		}


	}
	else
	{
		g_gfxDetails.bHighPerformance = FALSE;
		if(menu)
		{
			subMenu = menu->GetSubMenu(OPTIONS_MENU);
			if(subMenu)
				subMenu->CheckMenuItem(OPTIONS_HIGH_PERFORMANCE, MF_UNCHECKED | MF_BYPOSITION);
		}
	}

	g_bPerformanceChanged = true;

	// set menu option refresh rate
	{
		int n;
		CMenu *subSubMenu;
		if(menu)
		{
			subMenu = menu->GetSubMenu(OPTIONS_MENU);
			if(subMenu)
			{
				subSubMenu = subMenu->GetSubMenu(OPTIONS_SET_REFRESH_RATE);
				if(subSubMenu)
				{	
					for(n=0;n<13;n++)
					{					
						if(n == g_gfxDetails.optionsSetRefreshRate)				
							subSubMenu->CheckMenuItem(n, MF_CHECKED | MF_BYPOSITION);
						else
							subSubMenu->CheckMenuItem(n, MF_UNCHECKED | MF_BYPOSITION);
					}
				}
			}
		}
	}
	
	// High Performance
	if(g_gfxDetails.bHighPerformance)
	{
		if(menu)
		{
			subMenu = menu->GetSubMenu(OPTIONS_MENU);
			if(subMenu)
			{
				subMenu->CheckMenuItem(OPTIONS_HIGH_PERFORMANCE, MF_CHECKED | MF_BYPOSITION);
			}
		}
	}


	//////////////////////////////////////////////////
	// decide which level-of-detail ball to use
		Mmain_log("CSnooker, choosing GFX LOD");
	if(g_gfxDetails.bOptionsLowgfx)
	{
		g_gfxDetails.bOptionsLowgfx = TRUE;
		g_gfxDetails.bOptionsMediumgfx = FALSE;
		g_gfxDetails.bOptionsHighgfx = FALSE;
		if(menu)
		{
			subMenu = menu->GetSubMenu(OPTIONS_MENU);
			if(subMenu)
			{
				subMenu->CheckMenuItem(OPTIONS_LOW_GRAPHICS, MF_CHECKED | MF_BYPOSITION);
				subMenu->CheckMenuItem(OPTIONS_MEDIUM_GRAPHICS, MF_UNCHECKED | MF_BYPOSITION);
				subMenu->CheckMenuItem(OPTIONS_HIGH_GRAPHICS, MF_UNCHECKED | MF_BYPOSITION);
			}
		}
	}
	else if(g_gfxDetails.bOptionsMediumgfx)
	{
		g_gfxDetails.bOptionsLowgfx = FALSE;
		g_gfxDetails.bOptionsMediumgfx = TRUE;
		g_gfxDetails.bOptionsHighgfx = FALSE;
		if(menu)
		{
			subMenu = menu->GetSubMenu(OPTIONS_MENU);
			if(subMenu)
			{
				subMenu->CheckMenuItem(OPTIONS_LOW_GRAPHICS, MF_UNCHECKED | MF_BYPOSITION);
				subMenu->CheckMenuItem(OPTIONS_MEDIUM_GRAPHICS, MF_CHECKED | MF_BYPOSITION);
				subMenu->CheckMenuItem(OPTIONS_HIGH_GRAPHICS, MF_UNCHECKED | MF_BYPOSITION);
			}
		}
	}
	else if(g_gfxDetails.bOptionsHighgfx)
	{
		g_gfxDetails.bOptionsLowgfx = FALSE;
		g_gfxDetails.bOptionsMediumgfx = FALSE;
		g_gfxDetails.bOptionsHighgfx = TRUE;
		if(menu)
		{
			subMenu = menu->GetSubMenu(OPTIONS_MENU);
			if(subMenu)
			{
				subMenu->CheckMenuItem(OPTIONS_LOW_GRAPHICS, MF_UNCHECKED | MF_BYPOSITION);
				subMenu->CheckMenuItem(OPTIONS_MEDIUM_GRAPHICS, MF_UNCHECKED | MF_BYPOSITION);
				subMenu->CheckMenuItem(OPTIONS_HIGH_GRAPHICS, MF_CHECKED | MF_BYPOSITION);
			}
		}
	}

	if(g_gfxDetails.bOptionsLowgfx) m_M3d_Ball_ptr = m_customTable->m_M3d_Ball_low;
	if(g_gfxDetails.bOptionsMediumgfx) m_M3d_Ball_ptr = m_customTable->m_M3d_Ball_med;
	if(g_gfxDetails.bOptionsHighgfx) m_M3d_Ball_ptr = m_customTable->m_M3d_Ball_high;
	//////////////////////////////////////////////////

	// set menu option sensitivity
	int type = g_gfxDetails.iOptionsSensitivity;
	if(type < 1) type = 1;
	if(type > 9) type = 9;


	if(menu)
	{
		subMenu = menu->GetSubMenu(OPTIONS_MENU);
		if(subMenu)
		{
			CMenu *subSubMenu;
			subSubMenu = subMenu->GetSubMenu(OPTIONS_MOUSE_SENSITIVITY);
			if(subSubMenu)
			{
				// Uncheck everything
				for(int i=0;i<9;i++)
				{
					subSubMenu->CheckMenuItem(i, MF_UNCHECKED | MF_BYPOSITION);
				}

				// Check item we clicked
				subSubMenu->CheckMenuItem((type-1), MF_CHECKED | MF_BYPOSITION);
			}
		}
	}

	/*
	// set menu option wireframe
	if(g_gfxDetails.bOptionsWireframe)
	{
		g_gfxDetails.bOptionsWireframe = TRUE;
		if(menu)
		{
			subMenu = menu->GetSubMenu(OPTIONS_MENU);
			if(subMenu)
				subMenu->CheckMenuItem(OPTIONS_WIREFRAME, MF_CHECKED | MF_BYPOSITION);
		}
	}
	else
	{
		g_gfxDetails.bOptionsWireframe = FALSE;
		if(menu)
		{
			subMenu = menu->GetSubMenu(OPTIONS_MENU);
			if(subMenu)
				subMenu->CheckMenuItem(OPTIONS_WIREFRAME, MF_UNCHECKED | MF_BYPOSITION);
		}
	}
	*/
			
	// set menu option GFX mode
	if(g_gfxDetails.bOptions2DMode)
	{
		GAMEAPPVIEW->m_GFXMode = 0;			// 2D
		g_gfxDetails.bOptions2DMode = TRUE;
		g_gfxDetails.bOptions2D3DMode = FALSE;
		g_gfxDetails.bOptions2DTVAutoMode = FALSE;
		g_gfxDetails.bOptions2D3DFreeMode = FALSE;
		
		if(menu)
		{
			subMenu = menu->GetSubMenu(OPTIONS_MENU);
			if(subMenu)
			{
				subMenu->CheckMenuItem(OPTIONS_2D_MODE, MF_CHECKED | MF_BYPOSITION);
				subMenu->CheckMenuItem(OPTIONS_2D3D_MODE, MF_UNCHECKED | MF_BYPOSITION);
				subMenu->CheckMenuItem(OPTIONS_2DTVAUTO_MODE, MF_UNCHECKED | MF_BYPOSITION);
				subMenu->CheckMenuItem(OPTIONS_2D3DFREE_MODE, MF_UNCHECKED | MF_BYPOSITION);
			}
		}
	}
	else if(g_gfxDetails.bOptions2D3DMode)
	{
		GAMEAPPVIEW->m_GFXMode = 1;			// 2D3D
		g_gfxDetails.bOptions2DMode = FALSE;
		g_gfxDetails.bOptions2D3DMode = TRUE;
		g_gfxDetails.bOptions2DTVAutoMode = FALSE;
		g_gfxDetails.bOptions2D3DFreeMode = FALSE;
		if(menu)
		{
			subMenu = menu->GetSubMenu(OPTIONS_MENU);
			if(subMenu)
			{
				subMenu->CheckMenuItem(OPTIONS_2D_MODE, MF_UNCHECKED | MF_BYPOSITION);
				subMenu->CheckMenuItem(OPTIONS_2D3D_MODE, MF_CHECKED | MF_BYPOSITION);
				subMenu->CheckMenuItem(OPTIONS_2DTVAUTO_MODE, MF_UNCHECKED | MF_BYPOSITION);
				subMenu->CheckMenuItem(OPTIONS_2D3DFREE_MODE, MF_UNCHECKED | MF_BYPOSITION);
			}
		}
	}
	else if(g_gfxDetails.bOptions2DTVAutoMode)
	{
		GAMEAPPVIEW->m_GFXMode = 2;			// 2DTVAuto
		g_gfxDetails.bOptions2DMode = FALSE;
		g_gfxDetails.bOptions2D3DMode = FALSE;
		g_gfxDetails.bOptions2DTVAutoMode = TRUE;
		g_gfxDetails.bOptions2D3DFreeMode = FALSE;
		if(menu)
		{
			subMenu = menu->GetSubMenu(OPTIONS_MENU);
			if(subMenu)
			{
				subMenu->CheckMenuItem(OPTIONS_2D_MODE, MF_UNCHECKED | MF_BYPOSITION);
				subMenu->CheckMenuItem(OPTIONS_2D3D_MODE, MF_UNCHECKED | MF_BYPOSITION);
				subMenu->CheckMenuItem(OPTIONS_2DTVAUTO_MODE, MF_CHECKED | MF_BYPOSITION);
				subMenu->CheckMenuItem(OPTIONS_2D3DFREE_MODE, MF_UNCHECKED | MF_BYPOSITION);

			}
		}
	}
	else if(g_gfxDetails.bOptions2D3DFreeMode)
	{
		GAMEAPPVIEW->m_GFXMode = 3;			// 2D3DFree
		g_gfxDetails.bOptions2DMode = FALSE;
		g_gfxDetails.bOptions2D3DMode = FALSE;
		g_gfxDetails.bOptions2DTVAutoMode = FALSE;
		g_gfxDetails.bOptions2D3DFreeMode = TRUE;
		if(menu)
		{
			subMenu = menu->GetSubMenu(OPTIONS_MENU);
			if(subMenu)
			{
				subMenu->CheckMenuItem(OPTIONS_2D_MODE, MF_UNCHECKED | MF_BYPOSITION);
				subMenu->CheckMenuItem(OPTIONS_2D3D_MODE, MF_UNCHECKED | MF_BYPOSITION);
				subMenu->CheckMenuItem(OPTIONS_2DTVAUTO_MODE, MF_UNCHECKED | MF_BYPOSITION);
				subMenu->CheckMenuItem(OPTIONS_2D3DFREE_MODE, MF_CHECKED | MF_BYPOSITION);
			}
		}
	}
	else;


	// set menu option complex shadows
	if(g_gfxDetails.bOptionsComplexShadows)
	{
		g_gfxDetails.bOptionsComplexShadows = TRUE;
		if(menu)
		{
			subMenu = menu->GetSubMenu(OPTIONS_MENU);
			if(subMenu)
				subMenu->CheckMenuItem(OPTIONS_COMPLEX_SHADOWS, MF_CHECKED | MF_BYPOSITION);
		}
	}
	else
	{
		g_gfxDetails.bOptionsComplexShadows = FALSE;
		if(menu)
		{
			subMenu = menu->GetSubMenu(OPTIONS_MENU);
			if(subMenu)
				subMenu->CheckMenuItem(OPTIONS_COMPLEX_SHADOWS, MF_UNCHECKED | MF_BYPOSITION);
		}
	}

	// set menu option show mini-cam on shot
	if(g_gfxDetails.bOptionsShowMiniCam)
	{
		g_gfxDetails.bOptionsShowMiniCam = TRUE;
		if(menu)
		{
			subMenu = menu->GetSubMenu(OPTIONS_MENU);
			if(subMenu)
				subMenu->CheckMenuItem(OPTIONS_SHOW_MINICAM, MF_CHECKED | MF_BYPOSITION);
		}
	}
	else
	{
		g_gfxDetails.bOptionsShowMiniCam = FALSE;
		if(menu)
		{
			subMenu = menu->GetSubMenu(OPTIONS_MENU);
			if(subMenu)
				subMenu->CheckMenuItem(OPTIONS_SHOW_MINICAM, MF_UNCHECKED | MF_BYPOSITION);
		}
	}


	// set menu option colour blind
	if (g_gfxDetails.bOptionsColourBlind)
	{
		g_gfxDetails.bOptionsColourBlind = TRUE;
		if (menu)
		{
			subMenu = menu->GetSubMenu(OPTIONS_MENU);
			if (subMenu)
				subMenu->CheckMenuItem(OPTIONS_COLOUR_BLIND, MF_CHECKED | MF_BYPOSITION);
		}
	}
	else
	{
		g_gfxDetails.bOptionsColourBlind = FALSE;
		if (menu)
		{
			subMenu = menu->GetSubMenu(OPTIONS_MENU);
			if (subMenu)
				subMenu->CheckMenuItem(OPTIONS_COLOUR_BLIND, MF_UNCHECKED | MF_BYPOSITION);
		}
	}

	// set menu option dotted cue ball
	if (g_gfxDetails.bOptionsDottedCueBall)
	{
		g_gfxDetails.bOptionsDottedCueBall = TRUE;
		if (menu)
		{
			subMenu = menu->GetSubMenu(OPTIONS_MENU);
			if (subMenu)
				subMenu->CheckMenuItem(OPTIONS_DOTTEDCUEBALL, MF_CHECKED | MF_BYPOSITION);
		}
	}
	else
	{
		g_gfxDetails.bOptionsDottedCueBall = FALSE;
		if (menu)
		{
			subMenu = menu->GetSubMenu(OPTIONS_MENU);
			if (subMenu)
				subMenu->CheckMenuItem(OPTIONS_DOTTEDCUEBALL, MF_UNCHECKED | MF_BYPOSITION);
		}
	}


	// set menu option shot in lobby TV
	if (g_gfxDetails.bShowShotInLobbyTV)
	{
		g_gfxDetails.bShowShotInLobbyTV = TRUE;
		if (menu)
		{
			subMenu = menu->GetSubMenu(OPTIONS_MENU);
			if (subMenu)
				subMenu->CheckMenuItem(OPTIONS_SHOW_LOBBYTV, MF_CHECKED | MF_BYPOSITION);
		}
	}
	else
	{
		g_gfxDetails.bShowShotInLobbyTV = FALSE;
		if (menu)
		{
			subMenu = menu->GetSubMenu(OPTIONS_MENU);
			if (subMenu)
				subMenu->CheckMenuItem(OPTIONS_SHOW_LOBBYTV, MF_UNCHECKED | MF_BYPOSITION);
		}
	}


	// set menu option 'Sound'
	if(menu)
	{
		subMenu = menu->GetSubMenu(OPTIONS_MENU);
		if(subMenu)
		{
			CMenu *subSubMenu = subMenu->GetSubMenu(OPTIONS_SOUND);
			if(subSubMenu)
			{
				if(g_gfxDetails.bSoundBalls)
				{
					subSubMenu->CheckMenuItem(OPTIONS_SOUND_BALL_SFX, MF_CHECKED | MF_BYPOSITION);
					g_gfxDetails.bSoundBalls = 1;
				}
				else
				{
					subSubMenu->CheckMenuItem(OPTIONS_SOUND_BALL_SFX, MF_UNCHECKED | MF_BYPOSITION);
					g_gfxDetails.bSoundBalls = 0;
				}
				////
				if(g_gfxDetails.bSoundClapping)
				{
					subSubMenu->CheckMenuItem(OPTIONS_SOUND_CLAPPING_SFX, MF_CHECKED | MF_BYPOSITION);
					g_gfxDetails.bSoundClapping = 1;
				}
				else
				{
					subSubMenu->CheckMenuItem(OPTIONS_SOUND_CLAPPING_SFX, MF_UNCHECKED | MF_BYPOSITION);
					g_gfxDetails.bSoundClapping = 0;
				}
				////
				if(g_gfxDetails.bSoundVoices)
				{
					subSubMenu->CheckMenuItem(OPTIONS_SOUND_VOICES, MF_CHECKED | MF_BYPOSITION);
					g_gfxDetails.bSoundVoices = 1;
				}
				else
				{
					subSubMenu->CheckMenuItem(OPTIONS_SOUND_VOICES, MF_UNCHECKED | MF_BYPOSITION);
					g_gfxDetails.bSoundVoices = 0;
				}
			}
		}
	}
	// set menu option 'Sound'
	if(menu)
	{
		subMenu = menu->GetSubMenu(OPTIONS_MENU);
		if(subMenu)
		{
			CMenu *subSubMenu = subMenu->GetSubMenu(OPTIONS_DRAW_EYE_CAM_CUE);
			if(subSubMenu)
			{
				if(g_gfxDetails.bEyeCamDrawCueTip)
				{
					g_gfxDetails.bEyeCamDrawCueTip = 1;
					g_gfxDetails.bEyeCamDrawCueTransparent = 0;
					g_gfxDetails.bEyeCamDrawCueSolid = 0;
					subSubMenu->CheckMenuItem(OPTIONS_DRAW_EYE_CAM_CUE_TIP, MF_CHECKED | MF_BYPOSITION);
					subSubMenu->CheckMenuItem(OPTIONS_DRAW_EYE_CAM_CUE_TRANSPARENT, MF_UNCHECKED | MF_BYPOSITION);
					subSubMenu->CheckMenuItem(OPTIONS_DRAW_EYE_CAM_CUE_SOLID, MF_UNCHECKED | MF_BYPOSITION);
				}
				else if(g_gfxDetails.bEyeCamDrawCueTransparent)
				{
					g_gfxDetails.bEyeCamDrawCueTip = 0;
					g_gfxDetails.bEyeCamDrawCueTransparent = 1;
					g_gfxDetails.bEyeCamDrawCueSolid = 0;
					subSubMenu->CheckMenuItem(OPTIONS_DRAW_EYE_CAM_CUE_TIP, MF_UNCHECKED | MF_BYPOSITION);
					subSubMenu->CheckMenuItem(OPTIONS_DRAW_EYE_CAM_CUE_TRANSPARENT, MF_CHECKED | MF_BYPOSITION);
					subSubMenu->CheckMenuItem(OPTIONS_DRAW_EYE_CAM_CUE_SOLID, MF_UNCHECKED | MF_BYPOSITION);
				}
				else if(g_gfxDetails.bEyeCamDrawCueSolid)
				{
					g_gfxDetails.bEyeCamDrawCueTip = 0;
					g_gfxDetails.bEyeCamDrawCueTransparent = 0;
					g_gfxDetails.bEyeCamDrawCueSolid = 1;
					subSubMenu->CheckMenuItem(OPTIONS_DRAW_EYE_CAM_CUE_TIP, MF_UNCHECKED | MF_BYPOSITION);
					subSubMenu->CheckMenuItem(OPTIONS_DRAW_EYE_CAM_CUE_TRANSPARENT, MF_UNCHECKED | MF_BYPOSITION);
					subSubMenu->CheckMenuItem(OPTIONS_DRAW_EYE_CAM_CUE_SOLID, MF_CHECKED | MF_BYPOSITION);
				}
			}
		}
	}
		
	// set menu option show login wizard
	if(g_gfxDetails.bShowLoginWizard)
	{
		g_gfxDetails.bShowLoginWizard = TRUE;
		if(menu)
		{
			subMenu = menu->GetSubMenu(OPTIONS_MENU);
			if(subMenu)
				subMenu->CheckMenuItem(OPTIONS_SHOW_LOGIN_WIZARD, MF_CHECKED | MF_BYPOSITION);
		}
	}
	else
	{
		g_gfxDetails.bShowLoginWizard = FALSE;
		if(menu)
		{
			subMenu = menu->GetSubMenu(OPTIONS_MENU);
			if(subMenu)
				subMenu->CheckMenuItem(OPTIONS_SHOW_LOGIN_WIZARD, MF_UNCHECKED | MF_BYPOSITION);
		}
	}


	// set menu option Default Scheme
	if(g_gfxDetails.bDefaultColorScheme)
	{
		if(menu)
		{
			subMenu = menu->GetSubMenu(OPTIONS_MENU);
			if(subMenu)
				subMenu->CheckMenuItem(OPTIONS_DEFAULT_SCHEME, MF_CHECKED | MF_BYPOSITION);
		}
	}
	else
	{
		if(menu)
		{
			subMenu = menu->GetSubMenu(OPTIONS_MENU);
			if(subMenu)
				subMenu->CheckMenuItem(OPTIONS_DEFAULT_SCHEME, MF_UNCHECKED | MF_BYPOSITION);
		}
	}


	
	// set menu option Show Shot In Lobby TV
	if(g_gfxDetails.bShowShotInLobbyTV)
	{
		if(menu)
		{
			subMenu = menu->GetSubMenu(OPTIONS_MENU);
			if(subMenu)
				subMenu->CheckMenuItem(OPTIONS_SHOW_LOBBYTV, MF_CHECKED | MF_BYPOSITION);
		}
	}
	else
	{
		if(menu)
		{
			subMenu = menu->GetSubMenu(OPTIONS_MENU);
			if(subMenu)
				subMenu->CheckMenuItem(OPTIONS_SHOW_LOBBYTV, MF_UNCHECKED | MF_BYPOSITION);
		}
	}



	// set menu option draw colour sight line
	if(g_gfxDetails.bOptionsDrawColourSL)
	{
		g_gfxDetails.bOptionsDrawColourSL = TRUE;
		if(menu)
		{
			subMenu = menu->GetSubMenu(OPTIONS_MENU);
			if(subMenu)
				subMenu->CheckMenuItem(OPTIONS_DRAW_COLOUR_SL, MF_CHECKED | MF_BYPOSITION);
		}
	}
	else
	{
		g_gfxDetails.bOptionsDrawColourSL = FALSE;
		if(menu)
		{
			subMenu = menu->GetSubMenu(OPTIONS_MENU);
			if(subMenu)
				subMenu->CheckMenuItem(OPTIONS_DRAW_COLOUR_SL, MF_UNCHECKED | MF_BYPOSITION);
		}
	}

	// set menu option draw error sight line
	if(g_gfxDetails.bOptionsDrawErrorSL)
	{
		g_gfxDetails.bOptionsDrawErrorSL = TRUE;
		if(menu)
		{
			subMenu = menu->GetSubMenu(OPTIONS_MENU);
			if(subMenu)
				subMenu->CheckMenuItem(OPTIONS_DRAW_ERROR_SL, MF_CHECKED | MF_BYPOSITION);
		}
	}
	else
	{
		g_gfxDetails.bOptionsDrawErrorSL = FALSE;
		if(menu)
		{
			subMenu = menu->GetSubMenu(OPTIONS_MENU);
			if(subMenu)
				subMenu->CheckMenuItem(OPTIONS_DRAW_ERROR_SL, MF_UNCHECKED | MF_BYPOSITION);
		}
	}

	// set menu option draw ghost cue-ball
	if(g_gfxDetails.bOptionsDrawGhostCueBall)
	{
		g_gfxDetails.bOptionsDrawGhostCueBall = TRUE;
		if(menu)
		{
			subMenu = menu->GetSubMenu(OPTIONS_MENU);
			if(subMenu)
				subMenu->CheckMenuItem(OPTIONS_DRAW_GHOST_CUE_BALL, MF_CHECKED | MF_BYPOSITION);
		}
	}
	else
	{
		g_gfxDetails.bOptionsDrawGhostCueBall = FALSE;
		if(menu)
		{
			subMenu = menu->GetSubMenu(OPTIONS_MENU);
			if(subMenu)
				subMenu->CheckMenuItem(OPTIONS_DRAW_GHOST_CUE_BALL, MF_UNCHECKED | MF_BYPOSITION);
		}
	}

	// set menu option draw cue-ball line extend
	if(g_gfxDetails.bOptionsDrawCueBallLE)
	{
		
		g_gfxDetails.bOptionsDrawCueBallLE = TRUE;
		if(menu)
		{
			subMenu = menu->GetSubMenu(OPTIONS_MENU);
			if(subMenu)
				subMenu->CheckMenuItem(OPTIONS_DRAW_CUEBALL_LE, MF_CHECKED | MF_BYPOSITION);
		}
	}
	else
	{
		g_gfxDetails.bOptionsDrawCueBallLE = FALSE;
		if(menu)
		{
			subMenu = menu->GetSubMenu(OPTIONS_MENU);
			if(subMenu)
				subMenu->CheckMenuItem(OPTIONS_DRAW_CUEBALL_LE, MF_UNCHECKED | MF_BYPOSITION);
		}
	}
	
	
	// set menu option draw static power %
	if(g_gfxDetails.bOptionsDrawStaticPower)
	{
		g_gfxDetails.bOptionsDrawStaticPower = TRUE;
		if(menu)
		{
			subMenu = menu->GetSubMenu(OPTIONS_MENU);
			if(subMenu)
				subMenu->CheckMenuItem(OPTIONS_DRAW_STATIC_POWER, MF_CHECKED | MF_BYPOSITION);
		}
	}
	else
	{
		g_gfxDetails.bOptionsDrawStaticPower = FALSE;
		if(menu)
		{
			subMenu = menu->GetSubMenu(OPTIONS_MENU);
			if(subMenu)
				subMenu->CheckMenuItem(OPTIONS_DRAW_STATIC_POWER, MF_UNCHECKED | MF_BYPOSITION);
		}
	}

	
	// set menu file -> 'auto save frames'
	if(g_gfxDetails.bAutoSaveFrames)
	{
		g_gfxDetails.bAutoSaveFrames = TRUE;
		if(menu)
		{
			subMenu = menu->GetSubMenu(FILE_MENU);
			if(subMenu)
				subMenu->CheckMenuItem(FILE_AUTOSAVEFRAMES, MF_CHECKED | MF_BYPOSITION);
		}
	}
	else
	{
		g_gfxDetails.bAutoSaveFrames = FALSE;
		if(menu)
		{
			subMenu = menu->GetSubMenu(FILE_MENU);
			if(subMenu)
				subMenu->CheckMenuItem(FILE_AUTOSAVEFRAMES, MF_UNCHECKED | MF_BYPOSITION);
		}
	}
	

	

	// set menu option Mini Cam Show Helpers
	if (g_gfxDetails.bOptionsMiniCamShowHelpers)
	{
		g_gfxDetails.bOptionsMiniCamShowHelpers = TRUE;
		if (menu)
		{
			subMenu = menu->GetSubMenu(OPTIONS_MENU);
			if (subMenu)
				subMenu->CheckMenuItem(OPTIONS_MINICAM_SHOWHELPERS, MF_CHECKED | MF_BYPOSITION);
		}
	}
	else
	{
		g_gfxDetails.bOptionsMiniCamShowHelpers = FALSE;
		if (menu)
		{
			subMenu = menu->GetSubMenu(OPTIONS_MENU);
			if (subMenu)
				subMenu->CheckMenuItem(OPTIONS_MINICAM_SHOWHELPERS, MF_UNCHECKED | MF_BYPOSITION);
		}
	}


	// set menu option Mini Cam Lock Mouse
	if (g_gfxDetails.bOptionsMiniCamLockMouse)
	{
		g_gfxDetails.bOptionsMiniCamLockMouse = TRUE;
		if (menu)
		{
			subMenu = menu->GetSubMenu(OPTIONS_MENU);
			if (subMenu)
				subMenu->CheckMenuItem(OPTIONS_MINICAM_LOCKMOUSE, MF_CHECKED | MF_BYPOSITION);
		}
	}
	else
	{
		g_gfxDetails.bOptionsMiniCamLockMouse = FALSE;
		if (menu)
		{
			subMenu = menu->GetSubMenu(OPTIONS_MENU);
			if (subMenu)
				subMenu->CheckMenuItem(OPTIONS_MINICAM_LOCKMOUSE, MF_UNCHECKED | MF_BYPOSITION);
		}
	}



	// Replace some Code here // TODO
	

	// create foul dialog box
	//m_foulDlg.Create(IDD_FOULDLG);
	//m_foulDlg.GetParent(m_parent);
				
	// make sure the main view window has focus
	((CGameAppView *)m_parent)->SetFocus();

	// init 3D snooker cue materials
	Init3DCueMaterials();
}

void CSnooker::Init(int reRackType, int rndval, char playerTurn)
{

	UpdatePhysics(5);

	// create foul dialog box
	m_foulDlg.DestroyWindow();
	m_foulDlg.Create(IDD_FOULDLG);
	m_foulDlg.GetParent(m_parent);

	int n;
	// initialise vars
	g_cycOld = g_cyc;
	for(n=0;n<16;n++)
	{
		g_pottedSpots[n] = -1;
		g_pottedStripes[n] = -1;
	}
	m_firstTime = TRUE;
	m_firstFewFrames = 10;		// Make sure physics processes for 10 frames when we rerack, so balls will separate if needed
	m_cueBallSelect = 0;	
	m_3dtoggle = 3;				// Default View is 3
	m_3dtoggle_old = 3;


	///// Set Default Camera Position //////////
	((CGameAppView *)m_parent)->m_TVcameraX_pos[0] = 5000;
	((CGameAppView *)m_parent)->m_TVcameraY_pos[0] = 0;
	((CGameAppView *)m_parent)->m_TVcameraZ_pos[0] = 1600;
	((CGameAppView *)m_parent)->m_TVcameraX_at[0] = 500;
	((CGameAppView *)m_parent)->m_TVcameraY_at[0] = 0;
	((CGameAppView *)m_parent)->m_TVcameraZ_at[0] = 0;
	((CGameAppView *)m_parent)->m_TVClipType = 0;
	if(g_gameAppView) PROCESS_CAMERA_TV(0, TRUE);
	////////////////////////////////////////////

	m_3dtoggle_status = -1;		// 0: Not selected, 1: Mouse over (highlighted)
	m_bStrikeBallReady = FALSE; // NEW! OAP & CAP
	m_bCuePowerJustOn = FALSE;
	GAMEAPPVIEW->m_oldCBS = -1;
	m_bCueLineSet = FALSE;
	m_bCueTipReady = FALSE;	
	m_bCueTipReadyFirstTime = TRUE;
	m_dominoHitLast = 0;
	m_dominoHit1st = 0;
	m_bCueState = FALSE;
	m_cuePowerPercentage = 0;
	m_bTestBallFinished = FALSE; // <-oap this var wasn't being inited before!!??
	m_TestBallCollideObjColour = 0;
	m_TestBallCollideObjPos = CVector(0,0,0);
	m_TestBallCollideVA = CVector(0,0,0);
	m_TestBallCollideVB = CVector(0,0,0);
	m_sCPoint.x = 0;
	m_sCPoint.y = 0;
	m_sPoint[0] = 0;
	m_sPoint[1] = 0;
	m_sPoint[2] = 0;
	//m_cx = 788;
	//m_cy = 396;
	m_sSize.x = 0;//m_cx;
	m_sSize.y = 0;//m_cy;
		
	m_selectBall = WHITE;
	m_grabBall = WHITE;
	m_bSeenSelectBall = FALSE;
	m_mouseX = 0.0f;
	m_mouseY = 0.0f;

	m_bMouseMoved = 1;

	m_bFoulDecideTurn = FALSE;
	m_bFoulShowDialog = FALSE;
	m_foulDlg.SetWindowPos(&((CGameAppView *)m_parent)->wndTop, 0, 0, 32, 32, SWP_NOACTIVATE | SWP_HIDEWINDOW);

	
	m_mcameraOn = 0;
	m_tableMouseOn = 0;
	m_ballMouseOn = 0;
	m_ballMousePos[0] = 0;
	m_ballMousePos[1] = 0;
	m_ballMousePos[2] = -20;
	m_bCueTipOff = TRUE;
	m_cuetip[0] = 0;
	m_cuetip[1] = 0;
	//m_cuetip[2] = 1;
	m_cuetip[2] = 0;
	m_targetPocket = -1;
	m_targetBall = -1;
	m_bTargetSuccess = FALSE;
	m_RaiseButt = m_RaiseButtOld = 0;
	m_bReceiveRaiseButtActive = FALSE;
	m_bRaiseButtHelper = TRUE;
	((CGameAppView *)m_parent)->m_sliderRaiseButt.SetPos(90);
	((CGameAppView *)m_parent)->m_sliderRaiseButt.EnableWindow(FALSE);
	((CGameAppView *)m_parent)->PreviousWndEditSetFocus();
	m_cueTipStrikeDir = CVector(1, 0, 0);
	m_sightlineFade = 0;
	m_cueStrikeTime = 0;
	m_cueStrikeSpeed = 0;
	m_cueStrikeDist = 0;
	m_rnd = 0;
	m_strikeRnd = 0;
	m_strikeMaxRnd = 0;
	m_strikeLine = 0;
	m_strikeDist = 0;

	m_shotTime = 60*50;
	m_shotTimeOld = m_shotTime;
	m_bShotTimeUp = FALSE;

	m_MaxShotTime = 120*50;		// 2 mins per shot, after that your opponent can force a forfit!
	m_MaxShotTimeOld = m_MaxShotTime;

	m_poolPots1P = 0;
	m_poolPots2P = 0;
	
	m_touchingBallIdx = 0;
	for(int t=0; t<MAX_TOUCHING_BALLS; t++)
		m_oldTouchingBall[t] = m_touchingBall[t] = 0;


	m_CameraMode = CAMERA_MODE_PLAYER_2D;
	m_CameraOldMode = m_CameraMode;
	g_pCameraMode = false;
	
	int tblIdx = GAMEAPPVIEW->m_lobby->GetTableIdxFromPlayer(GAMEAPPVIEW->m_playerSockIdx);
	

	if(!m_firstTime)
	{
		if(GAMEAPPVIEW->m_GFXMode == 1)
		{
			if( !MyTurn() || m_cueBallSelect == 2) m_CameraMode = CAMERA_MODE_WATCH;
				else m_CameraMode = CAMERA_MODE_PLAYER_2D;
		}
		else if(GAMEAPPVIEW->m_GFXMode == 2) 
		{
			if( !MyTurn() || m_cueBallSelect == 2) m_CameraMode = CAMERA_MODE_TV;				// 2DTV
				else m_CameraMode = CAMERA_MODE_PLAYER_2D;
		}
		else if(GAMEAPPVIEW->m_GFXMode == 3) 
		{
			if( !MyTurn() )
			{
				m_CameraMode = CAMERA_MODE_3DFREE;
				if(tblIdx != -1) g_pCameraMode = true;		// 2D3DFree
			}
			else
			{
				m_CameraMode = CAMERA_MODE_PLAYER_2D;
				if(tblIdx != -1) g_pCameraMode = false;

			}
		}
		else
		{
			m_CameraMode = CAMERA_MODE_PLAYER_2D;
			if(tblIdx != -1) g_pCameraMode = false;
		}

		m_CameraOldMode = m_CameraMode;
	}
	
	//CAMERA_MODE_PLAYER_3D;//CAMERA_MODE_WATCH;
	
	m_miniCameraStartUpView = TRUE;

	m_reRackRndVal = 0;
	m_foul3Times1P = 0;
	m_foul3Times2P = 0;

	m_bRefreshScores = TRUE;
	m_lifes = 0;
	m_shotcount = 0;
	m_score = 0;
	m_score1P = 0;
	m_score2P = 0;
	m_break1P = 0;
	m_break2P = 0;
	m_currentBreak  = 0;
	m_currentBreak100s = 0;
	m_currentBreak10s = 0;
	m_currentBreak1s = 0;
	m_encoded = FALSE;
	m_sfxCount = 0;
	m_bPriorVoiceOnly = FALSE;
	m_voiceN = 0;
	m_highBreaks1P[0] = 0;
	m_highBreaks1P[1] = 0;
	m_highBreaks2P[0] = 0;
	m_highBreaks2P[1] = 0;
	m_bestScore = 0;
	m_gameOver = 0;

	
	//m_gameOverIWon = -1; // NEW (if opponent has already conceeded, then i've won! (I cant lose anymore)) CANT PUT THIS HERE!
	m_bConcededGame = FALSE;
	//m_bInvalidGame = FALSE; // NEW - if a player concedes with >=10reds left, and opponent clears up table, we should still
	m_bTurn1P = TRUE;



	#ifdef THE_POOL_CLUB
	if(g_gameAppView)
		{
		// Make sure the sponsor text is off //
		// This is for the 2 Shot / 1 Shot msg for English Pool //
		g_gameAppView->m_sponsortxt[0] = 0;
		g_gameAppView->m_sponsoralpha = 0;
		}
	#endif	


	//

	// overrides...
	//if(g_build.gameType == GAMETYPE_POOL)
	if(!m_bPracticeMode)
	{
		int tblIdx = GAMEAPPVIEW->m_lobby->GetTableIdxFromPlayer(GAMEAPPVIEW->m_playerSockIdx);
		if(tblIdx != -1)
		{
			// WHO TO BREAK?...
			if(m_table[tblIdx].whoBreaks == 0)
			{
				// 1P Always
				m_bTurn1P = TRUE;
			}
			else if(m_table[tblIdx].whoBreaks == 1)
			{
				// 2P Always
				m_bTurn1P = FALSE;
			}
			else if(m_table[tblIdx].whoBreaks == 2)
			{
				// Alternate				
				m_bTurn1P = m_table[tblIdx].bSwitchBreakTo1P;
			}
			else if(m_table[tblIdx].whoBreaks == 3)
			{
				// Winner
				m_bTurn1P = m_table[tblIdx].bSwitchBreakTo1P;
			}
			else if(m_table[tblIdx].whoBreaks == 4)
			{
				// Loser
				m_bTurn1P = m_table[tblIdx].bSwitchBreakTo1P;
			}
			else if(m_table[tblIdx].whoBreaks == 5)
			{
				// Player Trailing
				m_bTurn1P = m_table[tblIdx].bSwitchBreakTo1P;
			}
			//////////////////////////////////////////////////////////
		

			// START/HANDICAP SCORES?...
			m_score1P = m_table[tblIdx].score1P;
			m_score2P = m_table[tblIdx].score2P;
		}
	}
	//

	// final override (if playerTurn is non-zero)
	if(playerTurn == 1)
		m_bTurn1P = TRUE;
	else if(playerTurn == 2)
		m_bTurn1P = FALSE;


	g_bLastTurn = m_bTurn1P;

	
	// "player 1/2 to break"

	if(tblIdx != -1)
	{
		// We must be seated in Player1, Player2, Player1b or Player2b to hear the 'Player 1 / Player 2 to break' sfx
		if(GAMEAPPVIEW->m_lobby->SeatedAtTable(GAMEAPPVIEW->m_playerSockIdx))
		if(m_table[tblIdx].bGameStarted)
		//if(reRackType == RERACK_START)
		{
			if(m_bTurn1P)
			{
				//m_bPriorVoiceOnly = TRUE;
				if(g_build.addOnSfx && g_gfxDetails.bSoundVoices)
					Msfx_smpplayvol(&GAMEAPPVIEW->m_game_smp[67], 200);
			}
			else
			{
				//m_bPriorVoiceOnly = TRUE;
				if(g_build.addOnSfx && g_gfxDetails.bSoundVoices)
					Msfx_smpplayvol(&GAMEAPPVIEW->m_game_smp[68], 200);
			}
		}
	}

	m_foul = 0;				// foul shot
	m_bMiss = TRUE;			// no ball potted
	m_rednext = TRUE;		// red ball to be potted first
//	m_bStrikeSend = FALSE;

	
	m_colourseq = 0;

	m_bBallInHand = 1;


	m_bStandUpFromShot = 1;
	m_nominateBall = 0; // WHITE

	m_clientSimEnd = FALSE;
	m_bShotInProgress = FALSE;
	m_simEndWaitingDelay = GAMEAPPCLIENT_FPS*2;
	m_simEndWaitingTimeOut = GAMEAPPCLIENT_FPS*20;//15;
	m_SimEnd = FALSE;
	m_SimulationEnd = TRUE;
	m_bSimJustFinished = FALSE;
	m_simEndCount = 0;
	m_process = FALSE;
	m_times = 0;
	m_cyc = 0;
	m_cycHitCushion = -1;
	m_bCueBallHitCushion = FALSE;

	// Bio. for 'yourself' player
	m_bioShotTime = 0; // from being your turn until striking ball
	m_bioPosition = 0; // error of each potted ball
	m_bioSafety = 0;   // opponent's first shot and doesn't pot
	m_bioPot = 0;	   // check test-ball path to pocket and check if potted. Also did we pot ball (flukes, doubles, etc).
	

	for(n=0;n<10;n++) VOut[n] = CVector(0.0f, 0.0f, 0.0f);

	// init test ball
	m_testBall->Init(WORLD_X(912),WORLD_Y(250),0, WHITE);
	m_testBallInitialSpeed = 0;
	
	// init tmp ball
	m_tmpBall->Init(WORLD_X(912),WORLD_Y(250),0, WHITE);

	// Setup snooker table
	ClearBalls();

	
	//m_reRackType = reRackType; //RERACK_START; //reRackType;

	

#ifdef THE_POOL_CLUB
	if(reRackType == RERACK_EMPTY) m_reRackType = RERACK_8BALL;
		else m_reRackType = reRackType;
#else
	if(reRackType == RERACK_EMPTY) m_reRackType = RERACK_START;
		else m_reRackType = reRackType;
#endif
	

	((CGameAppView *)m_parent)->m_replay->ResetStrike();
	ReRack(0, rndval);
	GAMEAPPVIEW->m_first_time = 0;



	// init chalks
	// for undo
	m_previousSaved = FALSE;
	//SavePreviousPosition();
	
	InitChalkMarks();
	GAMEAPPVIEW->GamePingInit();
	
}

void CSnooker::ClearBalls(void)
{
	int m;
	for(m=0;m<MAX_TOTALBALLS;m++)
	{
		m_ball[m]->m_flags = 0;		
		//ClearFlag(m_ball[m]->m_flags, Flg_BALL_ACTIVE);
		m_ball[m]->m_colour = 100; // trick code below, so this ball doesn't get respotted!!!!

		// keep them away from the blue spot (fixes the blue respot bug in practice mode 'colours')
		m_ball[m]->m_P.x = 10000;
		m_ball[m]->m_P.y = 10000;
	}
}

void CSnooker::PullBallsApartFIXPOINT(void)
{
	int n, m;
	FVector P, V;
	FVector D;
	__fix r2,R2;

	BOOL bNoChange = TRUE;

	//while(bNoChange) // CRASHES!
	for(int t=0;t<100;t++)
	for(n=0;n<MAX_TOTALBALLS;n++)
	if(TestFlag(m_ball[n]->m_flags, Flg_BALL_ACTIVE))		
	for(m=0;m<MAX_TOTALBALLS;m++)
	if(TestFlag(m_ball[m]->m_flags, Flg_BALL_ACTIVE))	
	if(n!=m)
	{
		/////////////////////
		D = m_ball[n]->f_P - m_ball[m]->f_P;
		r2 = D.Magnitude2();
		R2 = TOFIX((BALL_RADIUS*2)*(BALL_RADIUS*2));
		if(r2 < R2)
		{			
			bNoChange = FALSE;
			// move balls apart (in the direction of their centres)
			//P = (m_P + ball->m_P) * 0.5f;
		 
			P = m_ball[n]->f_P + m_ball[m]->f_P;
			P = P * TOFIX(0.5);
			V = m_ball[n]->f_P - m_ball[m]->f_P;
			V = V.Unit();

			// check for zero unit vector (i.e.two balls perfectly on top of each other)
			if(V.Magnitude() == 0)
				V.x = TOFIX(1.0f);			

			V = V * TOFIX(BALL_RADIUS + BALL_GAS*20);
			m_ball[n]->f_P = P + V; // messed
			m_ball[m]->f_P = P - V; // messed			

			//m_ball[n]->f_POld = m_ball[n]->f_P;		// ?
			//m_ball[m]->f_VOld = m_ball[m]->f_V;		// ?
			//UpdateFloats();		// ?
		}
		/////////////////////
	}
}

void CSnooker::ReRack(int replay, int _rndval, int bMsgBalls)
{

	float f1,f2,f3,f4;
	int rndval;
	float fn;

	//_rndval = 1;
	if(!_rndval) rndval = rand();
	else rndval = _rndval;

	//if(!rndval) rndval = rand(); // 2nd chance just in case

	if(!bMsgBalls) // only save/update this var when we are starting a new rack
		m_reRackRndVal = rndval;

	// set current version back
	if(!replay)
	{
		((CGameAppView *)m_parent)->m_replay->SetCurrentVersion();
	}

	// replay code: Adds a StrikeReRack to the replay list /////////////////////////////////////////
	if(!bMsgBalls) // if online and someone is sending you the balls updater, then we ignore this packet.
	if(!replay)
	if(m_reRackType!=RERACK_EMPTY) // don't record rerack empty.
	{
		if(! ((CGameAppView *)m_parent)->m_replay->IsFull() )
		{
			((CGameAppView *)m_parent)->m_replay->AddTimeStamp();

			CReplayStrikeReRack *rstrike = new CReplayStrikeReRack;
			((CGameAppView *)m_parent)->m_replay->AddStrike(rstrike);

			if(rstrike==NULL)
			{
				// Error can't add strike to list (list full).
			}
			else
			{
				rstrike->m_type = STRIKETYPE_RERACK;
				rstrike->m_reRackType = m_reRackType;
				rstrike->m_randseed = rndval;
				#ifdef THE_POOL_CLUB
				rstrike->m_customTableType = m_customTable->m_type;
				#endif //THE_POOL_CLUB
			}
		}
	} // end !replay
	// end replay code ///////////////////////////////////////////////////////////////////////

	srand(rndval);

	switch(m_reRackType)
	{
		// Starting Position	
		case RERACK_START:
		{
			// set all the ball positions & colours
			m_ball[MAX_WHITEBALLS-1]->Init(WORLD_X(912+5),WORLD_Y(250+5),0, WHITE);
			
			int maxReds;

			if(m_customTable->m_type == CTTYPE_SNOOKER10)
				maxReds = 10;
			else if(m_customTable->m_type == CTTYPE_SNOOKER6)
				maxReds = 6;
			else
				maxReds = 15;//MAX_REDBALLS; // i.e. 15 reds

			ReRackSTART(rndval, maxReds, m_wildRackType);
		}
		break;
		// The Line-Up
		case RERACK_LINEUP:
		{
			int n;
			int R = (BALL_RADIUS+1);

			// set all the ball positions & colours
			m_ball[MAX_WHITEBALLS-1]->Init(WORLD_X(912+5),WORLD_Y(250+5),0, WHITE);

			// relative to snooker x & y (not screen x & Y)
			float x, y;
			float yShift;
			yShift = 0;

			// build triangle of reds!
			//for(n=0;n<MAX_REDBALLS;n++)
			for(n=0;n<15;n++)
			{
				
				x = (float)(127 - (int)(rand()&255))*0.05f;
				y = (float)(127 - (int)(rand()&255))*0.03f;
				if(n==0) m_ball[MAX_WHITEBALLS+n]->Init( WORLD_X(135+15+15-10) -3*R + yShift*(/*3.32f*/3.5f*R) + x, WORLD_Y(297) + y,0, RED);
				else m_ball[MAX_WHITEBALLS+n]->Init( WORLD_X(135+15+15) -3*R + yShift*(/*3.32f*/3.5f*R) + x, WORLD_Y(297) + y,0, RED);
				yShift++;
				if(n==0 || n==5 || n==13)
					yShift+=1.2f;//0.8f;
			}
				
			// position the colours
			m_ball[MAX_WHITEBALLS+MAX_REDBALLS]->Init(WORLD_X(912),WORLD_Y(199+12),0, YELLOW);
			m_ball[MAX_WHITEBALLS+MAX_REDBALLS+1]->Init(WORLD_X(912),WORLD_Y(395-12),0, GREEN);
			m_ball[MAX_WHITEBALLS+MAX_REDBALLS+2]->Init(WORLD_X(912),WORLD_Y(297),0, BROWN);
			m_ball[MAX_WHITEBALLS+MAX_REDBALLS+3]->Init(WORLD_X(591),WORLD_Y(297),0, BLUE);
			m_ball[MAX_WHITEBALLS+MAX_REDBALLS+4]->Init(WORLD_X(331+10),WORLD_Y(297),0, PINK);
			m_ball[MAX_WHITEBALLS+MAX_REDBALLS+5]->Init(WORLD_X(175-10),WORLD_Y(297),0, BLACK);
		}
		break;
			
		// The Colours
		case RERACK_COLOURS:
		{
			m_colourseq = 2; // if yellow is potted, it stays in. And colourseq++ ready for the green ball etc...
			m_rednext = FALSE;

			// set all the ball positions & colours
			m_ball[MAX_WHITEBALLS-1]->Init(WORLD_X(912+5),WORLD_Y(250+5),0, WHITE);
				
			// position the colours
			m_ball[MAX_WHITEBALLS+MAX_REDBALLS]->Init(WORLD_X(912),WORLD_Y(199+12),0, YELLOW);
			m_ball[MAX_WHITEBALLS+MAX_REDBALLS+1]->Init(WORLD_X(912),WORLD_Y(395-12),0, GREEN);
			m_ball[MAX_WHITEBALLS+MAX_REDBALLS+2]->Init(WORLD_X(912),WORLD_Y(297),0, BROWN);
			m_ball[MAX_WHITEBALLS+MAX_REDBALLS+3]->Init(WORLD_X(591),WORLD_Y(297),0, BLUE);
			m_ball[MAX_WHITEBALLS+MAX_REDBALLS+4]->Init(WORLD_X(331+10),WORLD_Y(297),0, PINK);
			m_ball[MAX_WHITEBALLS+MAX_REDBALLS+5]->Init(WORLD_X(175-10),WORLD_Y(297),0, BLACK);
		}
		break;
			
		// Re-Spotted Black
		case RERACK_BLACK:
		{
			m_colourseq = 7; // if black is potted, it stays in
			m_rednext = FALSE;

			// set all the ball positions & colours
			m_ball[MAX_WHITEBALLS-1]->Init(WORLD_X(912+5),WORLD_Y(250+5),0, WHITE);

			m_ball[MAX_WHITEBALLS+MAX_REDBALLS+5]->Init(WORLD_X(175-10),WORLD_Y(297),0, BLACK);
		}
		break;

		// Re-Spotted Black
		case RERACK_BILLIARDS:
		{
			m_rednext = TRUE;
			
			// set all the ball positions & colours
			m_ball[MAX_WHITEBALLS-1]->Init(WORLD_X(912+5),WORLD_Y(250+5),0, WHITE);	// Player1's Ball
			m_ball[MAX_WHITEBALLS]->Init(WORLD_X(912+5),WORLD_Y(270+5),0, YELLOW);	// Player2's Ball
			m_ball[MAX_WHITEBALLS+1]->Init(WORLD_X(175-10),WORLD_Y(297),0, RED);	// We'll use this as our Red ball
			

			m_customTable->m_type = CTTYPE_BILLIARDS;
			
			// Billiards only starts with the Player's Ball who's turn it is  //
			if(m_bTurn1P) ClearFlag(m_ball[MAX_WHITEBALLS]->m_flags, Flg_BALL_ACTIVE);
				else ClearFlag(m_ball[MAX_WHITEBALLS-1]->m_flags, Flg_BALL_ACTIVE);

		}
		break;

		case RERACK_REDSSCATTERED1:
		{
			int n;
			int R = (BALL_RADIUS+1);

			// set all the ball positions & colours
			m_ball[MAX_WHITEBALLS-1]->Init(WORLD_X(912+5),WORLD_Y(250+5),0, WHITE);

			// position the colours
			m_ball[MAX_WHITEBALLS+MAX_REDBALLS]->Init(WORLD_X(912),WORLD_Y(199+12),0, YELLOW);
			m_ball[MAX_WHITEBALLS+MAX_REDBALLS+1]->Init(WORLD_X(912),WORLD_Y(395-12),0, GREEN);
			m_ball[MAX_WHITEBALLS+MAX_REDBALLS+2]->Init(WORLD_X(912),WORLD_Y(297),0, BROWN);
			m_ball[MAX_WHITEBALLS+MAX_REDBALLS+3]->Init(WORLD_X(591),WORLD_Y(297),0, BLUE);
			m_ball[MAX_WHITEBALLS+MAX_REDBALLS+4]->Init(WORLD_X(331+10),WORLD_Y(297),0, PINK);
			m_ball[MAX_WHITEBALLS+MAX_REDBALLS+5]->Init(WORLD_X(175-10),WORLD_Y(297),0, BLACK);
			
			// relative to snooker x & y (not screen x & Y)
			int m;
			float x, y;
			BOOL bValidPos;
			BOOL bFoundSpot;
		
			// spread reds everywhere!
			//for(n=0;n<MAX_REDBALLS;n++)
			for(n=0;n<15;n++)
			{
				bValidPos = FALSE;
				while(!bValidPos)
				{
					x = (175-75) + rand()%(912-175+150 + 75);
					y = (199-125) + rand()%(395-199+250);

					// check if ball 'n' is on top of another ball m
					bFoundSpot = TRUE;
					for(m=0;m<MAX_TOTALBALLS;m++)
					if(m!=(MAX_WHITEBALLS+n))
					if(TestFlag(m_ball[m]->m_flags, Flg_BALL_ACTIVE))
					if(m_ball[m]->Proximity(-WORLD_X(x), -WORLD_Y(y), 0, BALL_RADIUS*4.0f) == TRUE)
					{
						bFoundSpot = FALSE;
						break;					
					}

					// check if we have found a valid spot
					if(bFoundSpot)
					{
						m_ball[MAX_WHITEBALLS+n]->Init( WORLD_X(x), WORLD_Y(y),0, RED);
						bValidPos = TRUE;
					}
				}
			}
		}
		break;

		case RERACK_REDSSCATTERED2:
		{
			int n;
			int R = (BALL_RADIUS+1);

			// set all the ball positions & colours
			m_ball[MAX_WHITEBALLS-1]->Init(WORLD_X(912+5),WORLD_Y(250+5),0, WHITE);

			// position the colours
			m_ball[MAX_WHITEBALLS+MAX_REDBALLS]->Init(WORLD_X(912),WORLD_Y(199+12),0, YELLOW);
			m_ball[MAX_WHITEBALLS+MAX_REDBALLS+1]->Init(WORLD_X(912),WORLD_Y(395-12),0, GREEN);
			m_ball[MAX_WHITEBALLS+MAX_REDBALLS+2]->Init(WORLD_X(912),WORLD_Y(297),0, BROWN);
			m_ball[MAX_WHITEBALLS+MAX_REDBALLS+3]->Init(WORLD_X(591),WORLD_Y(297),0, BLUE);
			m_ball[MAX_WHITEBALLS+MAX_REDBALLS+4]->Init(WORLD_X(331+10),WORLD_Y(297),0, PINK);
			m_ball[MAX_WHITEBALLS+MAX_REDBALLS+5]->Init(WORLD_X(175-10),WORLD_Y(297),0, BLACK);
			
			// relative to snooker x & y (not screen x & Y)
			int m;
			float x, y;
			BOOL bValidPos;
			BOOL bFoundSpot;
		
			// spread reds everywhere!
			//for(n=0;n<MAX_REDBALLS;n++)
			for(n=0;n<15;n++)
			{
				bValidPos = FALSE;
				while(!bValidPos)
				{
					x = (175-75) + rand()%(591/*912*/-175+150);
					y = (199-75) + rand()%(395-199+150);

					// check if ball 'n' is on top of another ball m
					bFoundSpot = TRUE;
					for(m=0;m<MAX_TOTALBALLS;m++)
					if(m!=(MAX_WHITEBALLS+n))
					if(TestFlag(m_ball[m]->m_flags, Flg_BALL_ACTIVE))
					if(m_ball[m]->Proximity(-WORLD_X(x), -WORLD_Y(y), 0, BALL_RADIUS*2.0f) == TRUE)
					{
						bFoundSpot = FALSE;
						break;					
					}

					// check if we have found a valid spot
					if(bFoundSpot)
					{
						m_ball[MAX_WHITEBALLS+n]->Init( WORLD_X(x), WORLD_Y(y),0, RED);
						bValidPos = TRUE;
					}
				}
			}
		}
		break;

		case RERACK_WHITEONLY:
		{
			m_colourseq = 0; // if black is potted, it stays in
			m_rednext = TRUE;

			// set all the ball positions & colours
			m_ball[MAX_WHITEBALLS-1]->Init(WORLD_X(912+5),WORLD_Y(250+5),0, WHITE);
		}
		break;

		// Loaded position
		case RERACK_LOAD:
		{
			int n;
			
			// setup all ball positions
			for(n=0;n<MAX_TOTALBALLS;n++)
			if(m_ballPosColour[n].colour != -1)
			{
				// init a snooker ball
				if(g_build.gameType == GAMETYPE_SNOOKER)
					m_ball[n]->Init(m_ballPosColour[n].x, m_ballPosColour[n].y, 0, m_ballPosColour[n].colour);
				if(g_build.gameType == GAMETYPE_POOL)
					m_ball[n]->Init(m_ballPosColour[n].x, m_ballPosColour[n].y, 0, m_ballPosColour[n].colour,
									(rand()%360)*DEG2RAD, (rand()%360)*DEG2RAD, (rand()%360)*DEG2RAD);
				GAMEAPPVIEW->PracticeEditRefreshBallSpots(n);
			}
		}
		break;
		
		// 9-ball pool starting postition
		case RERACK_9BALL:
		{
			//m_colourseq = 2; // if yellow is potted, it stays in. And colourseq++ ready for the green ball etc...
			//m_rednext = FALSE;
						
			// set all the ball positions & colours
			m_ball[MAX_WHITEBALLS-1]->Init(WORLD_X(591+65+120/*912+5*/),WORLD_Y(297),0, WHITE);
			
			if(m_wildRackType == 0)
			{
				int n;
				int m;
				int i;
				BOOL bRackRnd;
				//int R = (BALL_RADIUS+1);
				//float R = (BALL_RADIUS+0.4);
				//float R = (BALL_RADIUS + 0.1f);
				//float R = (BALL_RADIUS - 0.7f); // (NEW FROM BRANDON)
				float R = (BALL_RADIUS+0.4f); // (NEW FROM BRANDON)				
				float noisex, noisey;

				// relative to snooker x & y (not screen x & Y)
				BOOL diamondBottom = 0;
				int xShift, yShift, xEnd;
				xShift = 0;
				yShift = 0;
				xEnd = 0;
				
				for(m=0;m<9/*MAX_REDBALLS*/;m++)
				{

					if(m == 0 || m == 4) bRackRnd = 0;
					else  bRackRnd = 1;

					if(bRackRnd)
						n = rand()%9;
					else n = 0;

					// build triangle of reds!
					for(i=0;i<9/*MAX_REDBALLS*/;i++)
					{
						//			'?-BALL'				'1-BALL'			'9-BALL'
						if( (bRackRnd && n!=0 && n!=8) || (m==0 && n==0) || (m == 4 && n == 8) )
						if(!TestFlag(m_ball[MAX_WHITEBALLS+n]->m_flags, Flg_BALL_ACTIVE))
						{
							fn = n;
							//noisex = 0;//((rand()%200) - 100)*0.01f;
							//noisey = 0;//((rand()%200) - 100)*0.01f;
							noisex = ((rand()%200) - 100)*0.01f;
							noisey = ((rand()%200) - 100)*0.01f;

							f1 = WORLD_X(331+10-2+65 + 18/*331+10*/);
							f2 = R*2;
							f1 = f1 - f2;
							f1 = f1 - 1;
							f3 = R*SQUAREROOT3;
							f3 = f3*((float)yShift);
							f3 = f3 + ((float)noisex);
							f1 = f1 + f3;
							
							f2 = R*2;
							f2 = f2*((float)xShift);
							f2 = f2 + WORLD_Y(297);
							f3 = ((float)xEnd)*R;
							f3 = -f3;
							f3 = f3 + ((float)noisey);
							f2 = f2 + f3;
				  
							
							float rx = (rand()%360)*DEG2RAD;
							float ry = (rand()%360)*DEG2RAD;
							float rz = (rand()%360)*DEG2RAD;
							if(bMsgBalls) rx = ry = rz = 0.0f;
							
							m_ball[MAX_WHITEBALLS+n]->Init( f1, f2, 0, MAX_WHITEBALLS+n, rx, ry, rz);															
														
							if(xShift==xEnd) 
							{
								xShift = 0;
								yShift--;

								if(xEnd>=2) diamondBottom = 1;
								
								if(!diamondBottom)
									xEnd++;
								else
									xEnd--;
							}
							else xShift++;
						
							break;						
						} // if
						n++;
						if(n == 9)
							n = 0;
					} // end for i
				} // end for m
			}
			else if(m_wildRackType == 1) // Groups
				ReRack9BALL_Group(rndval, 9, FALSE, bMsgBalls);
			else if(m_wildRackType == 2) // Random
				ReRack9BALL_Random(rndval, 9, TRUE, bMsgBalls);
				
			// position the colours
			//m_ball[MAX_WHITEBALLS+MAX_REDBALLS]->Init(WORLD_X(912),WORLD_Y(199+12),0, YELLOW);
			//m_ball[MAX_WHITEBALLS+MAX_REDBALLS+1]->Init(WORLD_X(912),WORLD_Y(395-12),0, GREEN);
			//m_ball[MAX_WHITEBALLS+MAX_REDBALLS+2]->Init(WORLD_X(912),WORLD_Y(297),0, BROWN);
			//m_ball[MAX_WHITEBALLS+MAX_REDBALLS+3]->Init(WORLD_X(591),WORLD_Y(297),0, BLUE);
			//m_ball[MAX_WHITEBALLS+MAX_REDBALLS+4]->Init(WORLD_X(331+10),WORLD_Y(297),0, PINK);
			//m_ball[MAX_WHITEBALLS+MAX_REDBALLS+5]->Init(WORLD_X(175-10),WORLD_Y(297),0, BLACK);

			//m_ball[MAX_WHITEBALLS+0]->Init(0, 0, 0, MAX_WHITEBALLS+0,
			//										(rand()%360)*DEG2RAD, (rand()%360)*DEG2RAD, (rand()%360)*DEG2RAD);
		}
		break;

		// 6-ball pool starting postition
		case RERACK_6BALL:
		{
			//m_colourseq = 2; // if yellow is potted, it stays in. And colourseq++ ready for the green ball etc...
			//m_rednext = FALSE;
						
			// set all the ball positions & colours
			m_ball[MAX_WHITEBALLS-1]->Init(WORLD_X(591+65+120/*912+5*/),WORLD_Y(297),0, WHITE);
			
			if(m_wildRackType == 0)
			{
				int n;
				int m;
				int i;
				BOOL bRackRnd;
				//int R = (BALL_RADIUS+1);
				//float R = (BALL_RADIUS+0.4);
				//float R = (BALL_RADIUS + 0.1f);
				//float R = (BALL_RADIUS - 0.7f); // (NEW FROM BRANDON)
				float R = (BALL_RADIUS+0.4f); // (NEW FROM BRANDON)				
				float noisex, noisey;

				// relative to snooker x & y (not screen x & Y)
				BOOL diamondBottom = 0;
				int xShift, yShift, xEnd;
				xShift = 0;
				yShift = 0;
				xEnd = 0;
				
				for(m=0;m<6/*MAX_REDBALLS*/;m++)
				{

					if(m == 0 || m == 4) bRackRnd = 0;
					else  bRackRnd = 1;

					if(bRackRnd)
						n = rand()%6;
					else n = 0;

					// build triangle of reds!
					for(i=0;i<6/*MAX_REDBALLS*/;i++)
					{
						//			'?-BALL'				'1-BALL'			'6-BALL'
						if( (bRackRnd && n!=0 && n!=5) || (m==0 && n==0) || (m == 4 && n == 5) )
						if(!TestFlag(m_ball[MAX_WHITEBALLS+n]->m_flags, Flg_BALL_ACTIVE))
						{
							fn = n;
							//noisex = 0;//((rand()%200) - 100)*0.01f;
							//noisey = 0;//((rand()%200) - 100)*0.01f;
							noisex = ((rand()%200) - 100)*0.01f;
							noisey = ((rand()%200) - 100)*0.01f;

							f1 = WORLD_X(331+10-2+65 + 18/*331+10*/);
							f2 = R*2;
							f1 = f1 - f2;
							f1 = f1 - 1;
							f3 = R*SQUAREROOT3;
							f3 = f3*((float)yShift);
							f3 = f3 + ((float)noisex);
							f1 = f1 + f3;
							
							f2 = R*2;
							f2 = f2*((float)xShift);
							f2 = f2 + WORLD_Y(297);
							f3 = ((float)xEnd)*R;
							f3 = -f3;
							f3 = f3 + ((float)noisey);
							f2 = f2 + f3;
							
							float rx = (rand()%360)*DEG2RAD;
							float ry = (rand()%360)*DEG2RAD;
							float rz = (rand()%360)*DEG2RAD;
							if(bMsgBalls) rx = ry = rz = 0.0f;
							
							m_ball[MAX_WHITEBALLS+n]->Init( f1, f2, 0, MAX_WHITEBALLS+n, rx, ry, rz);
							
							//m_ball[MAX_WHITEBALLS+n]->Init( WORLD_X(331+10) - (R*2) - 1 + yShift*(R*SQUAREROOT3) + noisex,
							//								WORLD_Y(297) + xShift*(R*2) - xEnd*(R) + noisey, 0, RED);
							if(xShift==xEnd) 
							{
								xShift = 0;
								yShift--;

								//if(xEnd>=2) diamondBottom = 1;
								
								if(!diamondBottom)
									xEnd++;
								else
									xEnd--;
							}
							else xShift++;
						
							break;						
						} // if
						n++;
						if(n == 6)
							n = 0;
					} // end for i
				} // end for m
			}
			else if(m_wildRackType == 1) // Groups
				ReRack6BALL_Group(rndval, 6, FALSE, bMsgBalls);
			else if(m_wildRackType == 2) // Random
				ReRack6BALL_Random(rndval, 6, TRUE, bMsgBalls);
				
			// position the colours
			//m_ball[MAX_WHITEBALLS+MAX_REDBALLS]->Init(WORLD_X(912),WORLD_Y(199+12),0, YELLOW);
			//m_ball[MAX_WHITEBALLS+MAX_REDBALLS+1]->Init(WORLD_X(912),WORLD_Y(395-12),0, GREEN);
			//m_ball[MAX_WHITEBALLS+MAX_REDBALLS+2]->Init(WORLD_X(912),WORLD_Y(297),0, BROWN);
			//m_ball[MAX_WHITEBALLS+MAX_REDBALLS+3]->Init(WORLD_X(591),WORLD_Y(297),0, BLUE);
			//m_ball[MAX_WHITEBALLS+MAX_REDBALLS+4]->Init(WORLD_X(331+10),WORLD_Y(297),0, PINK);
			//m_ball[MAX_WHITEBALLS+MAX_REDBALLS+5]->Init(WORLD_X(175-10),WORLD_Y(297),0, BLACK);

			//m_ball[MAX_WHITEBALLS+0]->Init(0, 0, 0, MAX_WHITEBALLS+0,
			//										(rand()%360)*DEG2RAD, (rand()%360)*DEG2RAD, (rand()%360)*DEG2RAD);

		}
		break;

		// 10-ball pool starting postition
		case RERACK_10BALL:
		{
			//m_colourseq = 2; // if yellow is potted, it stays in. And colourseq++ ready for the green ball etc...
			//m_rednext = FALSE;
						
			// set all the ball positions & colours
			m_ball[MAX_WHITEBALLS-1]->Init(WORLD_X(591+65+120/*912+5*/),WORLD_Y(297),0, WHITE);
			
			if(m_wildRackType == 0)
			{
				int n;
				int m;
				int i;
				BOOL bRackRnd;
				//int R = (BALL_RADIUS+1);
				//float R = (BALL_RADIUS+0.4);
				//float R = (BALL_RADIUS + 0.1f);
				//float R = (BALL_RADIUS - 0.7f); // (NEW FROM BRANDON)
				float R = (BALL_RADIUS+0.4f); // (NEW FROM BRANDON)				
				float noisex, noisey;

				// relative to snooker x & y (not screen x & Y)
				BOOL diamondBottom = 0;
				int xShift, yShift, xEnd;
				xShift = 0;
				yShift = 0;
				xEnd = 0;
				
				for(m=0;m<10/*MAX_REDBALLS*/;m++)
				{

					if(m == 0 || m == 4) bRackRnd = 0;
					else  bRackRnd = 1;

					if(bRackRnd)
						n = rand()%10;
					else n = 0;

					// build triangle of reds!
					for(i=0;i<10/*MAX_REDBALLS*/;i++)
					{
						//			'?-BALL'				'1-BALL'			'10-BALL'
						if( (bRackRnd && n!=0 && n!=9) || (m==0 && n==0) || (m == 4 && n == 9) )
						if(!TestFlag(m_ball[MAX_WHITEBALLS+n]->m_flags, Flg_BALL_ACTIVE))
						{
							fn = n;
							//noisex = 0;//((rand()%200) - 100)*0.01f;
							//noisey = 0;//((rand()%200) - 100)*0.01f;
							noisex = ((rand()%200) - 100)*0.01f;
							noisey = ((rand()%200) - 100)*0.01f;

							f1 = WORLD_X(331+10-2+65 + 18/*331+10*/);
							f2 = R*2;
							f1 = f1 - f2;
							f1 = f1 - 1;
							f3 = R*SQUAREROOT3;
							f3 = f3*((float)yShift);
							f3 = f3 + ((float)noisex);
							f1 = f1 + f3;
							
							f2 = R*2;
							f2 = f2*((float)xShift);
							f2 = f2 + WORLD_Y(297);
							f3 = ((float)xEnd)*R;
							f3 = -f3;
							f3 = f3 + ((float)noisey);
							f2 = f2 + f3;

							
							float rx = (rand()%360)*DEG2RAD;
							float ry = (rand()%360)*DEG2RAD;
							float rz = (rand()%360)*DEG2RAD;
							if(bMsgBalls) rx = ry = rz = 0.0f;

							m_ball[MAX_WHITEBALLS+n]->Init( f1, f2, 0, MAX_WHITEBALLS+n, rx, ry, rz);
							
							//m_ball[MAX_WHITEBALLS+n]->Init( WORLD_X(331+10) - (R*2) - 1 + yShift*(R*SQUAREROOT3) + noisex,
							//								WORLD_Y(297) + xShift*(R*2) - xEnd*(R) + noisey, 0, RED);
							if(xShift==xEnd) 
							{
								xShift = 0;
								yShift--;

								//if(xEnd>=2) diamondBottom = 1;
								
								if(!diamondBottom)
									xEnd++;
								else
									xEnd--;
							}
							else xShift++;
						
							break;						
						} // if
						n++;
						if(n == 10)
							n = 0;
					} // end for i
				} // end for m
			}
			else if(m_wildRackType == 1) // Groups
				ReRack10BALL_Group(rndval, 10, FALSE, bMsgBalls);
			else if(m_wildRackType == 2) // Random
				ReRack10BALL_Random(rndval, 10, TRUE, bMsgBalls);
				
			// position the colours
			//m_ball[MAX_WHITEBALLS+MAX_REDBALLS]->Init(WORLD_X(912),WORLD_Y(199+12),0, YELLOW);
			//m_ball[MAX_WHITEBALLS+MAX_REDBALLS+1]->Init(WORLD_X(912),WORLD_Y(395-12),0, GREEN);
			//m_ball[MAX_WHITEBALLS+MAX_REDBALLS+2]->Init(WORLD_X(912),WORLD_Y(297),0, BROWN);
			//m_ball[MAX_WHITEBALLS+MAX_REDBALLS+3]->Init(WORLD_X(591),WORLD_Y(297),0, BLUE);
			//m_ball[MAX_WHITEBALLS+MAX_REDBALLS+4]->Init(WORLD_X(331+10),WORLD_Y(297),0, PINK);
			//m_ball[MAX_WHITEBALLS+MAX_REDBALLS+5]->Init(WORLD_X(175-10),WORLD_Y(297),0, BLACK);

			//m_ball[MAX_WHITEBALLS+0]->Init(0, 0, 0, MAX_WHITEBALLS+0,
			//										(rand()%360)*DEG2RAD, (rand()%360)*DEG2RAD, (rand()%360)*DEG2RAD);

		}
		break;

		// Rotation-ball pool starting postition
		case RERACK_ROTATIONBALL:
		{
			//m_colourseq = 2; // if yellow is potted, it stays in. And colourseq++ ready for the green ball etc...
			//m_rednext = FALSE;
						
			//int R = (BALL_RADIUS+1);
			//float R = (BALL_RADIUS+0.4);
			//float R = (BALL_RADIUS + 0.1f);
			//float R = (BALL_RADIUS - 0.7f); // (NEW FROM BRANDON)
						
			// set all the ball positions & colours
			m_ball[MAX_WHITEBALLS-1]->Init(WORLD_X(591+65+120/*912+5*/),WORLD_Y(297),0, WHITE);
			
			if(m_wildRackType == 0)
			{
				int n;
				int m;
				int i;
				BOOL bRackRnd;
				float R = (BALL_RADIUS+0.4f); // (NEW FROM BRANDON)
				float noisex, noisey;

				// relative to snooker x & y (not screen x & Y)
				int xShift, yShift, xEnd;
				xShift = 0;
				yShift = 0;
				xEnd = 0;
				
				for(m=0;m<15/*MAX_REDBALLS*/;m++)
				{

					if(m == 0 || m == 4 || m == 10 || m == 14) bRackRnd = 0;
					else  bRackRnd = 1;

					if(bRackRnd)
						n = rand()%15;
					else n = 0;

					// build triangle of reds!
					for(i=0;i<15/*MAX_REDBALLS*/;i++)
					{
						//			'?-BALL'								'1-BALL'			'15-BALL'				'2-BALL'				'3-BALL'
						if( (bRackRnd && n!=0 && n!=14 && n!=1 && n!=2) || (m==0 && n==0) || (m == 4 && n == 14) || (m == 10 && n == 1) || (m == 14 && n == 2) )
						if(!TestFlag(m_ball[MAX_WHITEBALLS+n]->m_flags, Flg_BALL_ACTIVE))
						{
							fn = n;
							//noisex = 0;//((rand()%200) - 100)*0.01f;
							//noisey = 0;//((rand()%200) - 100)*0.01f;
							noisex = ((rand()%200) - 100)*0.01f;
							noisey = ((rand()%200) - 100)*0.01f;

							f1 = WORLD_X(331+10-2+65 + 18/*331+10*/);
							f2 = R*2;
							f1 = f1 - f2;
							f1 = f1 - 1;
							f3 = R*SQUAREROOT3;
							f3 = f3*((float)yShift);
							f3 = f3 + ((float)noisex);
							f1 = f1 + f3;
							
							f2 = R*2;
							f2 = f2*((float)xShift);
							f2 = f2 + WORLD_Y(297);
							f3 = ((float)xEnd)*R;
							f3 = -f3;
							f3 = f3 + ((float)noisey);
							f2 = f2 + f3;
				  

							float rx = (rand()%360)*DEG2RAD;
							float ry = (rand()%360)*DEG2RAD;
							float rz = (rand()%360)*DEG2RAD;
							if(bMsgBalls) rx = ry = rz = 0.0f;

							m_ball[MAX_WHITEBALLS+n]->Init( f1, f2, 0, MAX_WHITEBALLS+n, rx, ry, rz);

							//m_ball[MAX_WHITEBALLS+n]->Init( WORLD_X(331+10) - (R*2) - 1 + yShift*(R*SQUAREROOT3) + noisex,
							//								WORLD_Y(297) + xShift*(R*2) - xEnd*(R) + noisey, 0, RED);
							if(xShift==xEnd) 
							{
								xShift = 0;
								yShift--;
								xEnd++;
							}
							else xShift++;
						
							break;						
						} // if
						n++;
						if(n == 15)
							n = 0;
					} // end for i
				} // end for m
			}
			else if(m_wildRackType == 1) // Groups
				ReRack8BALL_Group(rndval, 15, FALSE, bMsgBalls);
			else if(m_wildRackType == 2) // Random
				ReRack8BALL_Random(rndval, 15, TRUE, bMsgBalls);
			
			// position the colours
			//m_ball[MAX_WHITEBALLS+MAX_REDBALLS]->Init(WORLD_X(912),WORLD_Y(199+12),0, YELLOW);
			//m_ball[MAX_WHITEBALLS+MAX_REDBALLS+1]->Init(WORLD_X(912),WORLD_Y(395-12),0, GREEN);
			//m_ball[MAX_WHITEBALLS+MAX_REDBALLS+2]->Init(WORLD_X(912),WORLD_Y(297),0, BROWN);
			//m_ball[MAX_WHITEBALLS+MAX_REDBALLS+3]->Init(WORLD_X(591),WORLD_Y(297),0, BLUE);
			//m_ball[MAX_WHITEBALLS+MAX_REDBALLS+4]->Init(WORLD_X(331+10),WORLD_Y(297),0, PINK);
			//m_ball[MAX_WHITEBALLS+MAX_REDBALLS+5]->Init(WORLD_X(175-10),WORLD_Y(297),0, BLACK);
		}
		break;


		// Break starting postition
		case RERACK_BREAK:
		{
			// set all the ball positions & colours
			m_ball[MAX_WHITEBALLS-1]->Init(WORLD_X(776),WORLD_Y(297+50),0, WHITE);
			
			if(m_wildRackType == 0)
				{
				int n;
				int m;
				int i;
				BOOL bRackRnd;
				float R = (BALL_RADIUS+0.4f); // (NEW FROM BRANDON)
				float noisex, noisey;

				// relative to snooker x & y (not screen x & Y)
				int xShift, yShift, xEnd;
				xShift = 0;
				yShift = 0;
				xEnd = 0;
				n = 0;
				
				// build triangle of reds!
				for(i=0;i<10;i++)
				if(!TestFlag(m_ball[MAX_WHITEBALLS+i]->m_flags, Flg_BALL_ACTIVE))
					{
					fn = n;
					noisex = ((rand()%200) - 100)*0.01f;
					noisey = ((rand()%200) - 100)*0.01f;

					f1 = WORLD_X(331+10-2+65 + 18/*331+10*/);
					f2 = R*2;
					f1 = f1 - f2;
					f1 = f1 - 1;
					f3 = R*SQUAREROOT3;
					f3 = f3*((float)yShift);
					f3 = f3 + ((float)noisex);
					f1 = f1 + f3;
							
					f2 = R*2;
					f2 = f2*((float)xShift);
					f2 = f2 + WORLD_Y(297);
					f3 = ((float)xEnd)*R;
					f3 = -f3;
					f3 = f3 + ((float)noisey);
					f2 = f2 + f3;
				  
					float rx = (rand()%360)*DEG2RAD;
					float ry = (rand()%360)*DEG2RAD;
					float rz = (rand()%360)*DEG2RAD;
					if(bMsgBalls) rx = ry = rz = 0.0f;

					m_ball[MAX_WHITEBALLS+i]->Init( f1, f2, 0, MAX_WHITEBALLS+i, rx, ry, rz); // Reds

					if(xShift==xEnd) 
						{
						xShift = 0;
						yShift--;
						xEnd++;
						}
					else 
						{
						xShift++;
						} // if
					n++;
					} // end for i
				}
			else if(m_wildRackType == 1) // Groups
				ReRack8BALL_Group(rndval, 15, FALSE, bMsgBalls);
			else if(m_wildRackType == 2) // Random
				ReRack8BALL_Random(rndval, 15, TRUE, bMsgBalls);
			
			// position the colours
			m_ball[MAX_WHITEBALLS+10]->Init(WORLD_X(776),WORLD_Y(297),0, MAX_WHITEBALLS+10);     // Brown 
			m_ball[MAX_WHITEBALLS+10+1]->Init(WORLD_X(591),WORLD_Y(297),0, MAX_WHITEBALLS+10+1); // Blue
			m_ball[MAX_WHITEBALLS+10+2]->Init(WORLD_X(305),WORLD_Y(297),0, MAX_WHITEBALLS+10+2); // Black
		}
		break;
		// 8-ball pool starting postition
		case RERACK_8BALL:
		{
			//m_colourseq = 2; // if yellow is potted, it stays in. And colourseq++ ready for the green ball etc...
			//m_rednext = FALSE;

			//m_score2P = 0;
			//m_break2P = 60*50;	// our shot-timer
				
						
			// set all the ball positions & colours
			m_ball[MAX_WHITEBALLS-1]->Init(WORLD_X(591+65+120/*912+5*/),WORLD_Y(297),0, WHITE);
			
			if(m_wildRackType == 0)
			{
			int n;
			//float R = (BALL_RADIUS+0.4f);
			float R = (BALL_RADIUS+0.1f);
			float noisex, noisey;

			// relative to snooker x & y (not screen x & Y)
			int xShift, yShift, xEnd;
			xShift = 0;
			yShift = 0;
			xEnd = 0;
			
			
				// build triangle of balls!
				
				// now randomly pick any stripe/spot ball
				int i = 0;
				int pick = rand()%15;
				
				BOOL bStripe = TRUE;
				int m;
				for(m=0;m<15;m++)
				{
					bStripe = rackOrder[m]; // stripe or spot ball?
					//n = 0;
					n = pick;
					for(i=0;i<15;i++)
					{
						//  'STRIPES'			 'SPOTS'						 '8-BALL'
						if( (bStripe && n>=8) || (!bStripe && n<=6 && m != 4) || (m == 4 && n == 7))
						if(!TestFlag(m_ball[MAX_WHITEBALLS+n]->m_flags, Flg_BALL_ACTIVE))
						{
							fn = n;
							//noisex = 0;//((rand()%200) - 100)*0.01f;
							//noisey = 0;//((rand()%200) - 100)*0.01f;
							noisex = ((rand()%200) - 100)*0.01f;
							noisey = ((rand()%200) - 100)*0.01f;
										 
							f1 = WORLD_X(331+10-2+65 + 18);
							f2 = R*2;
							f1 = f1 - f2;
							f1 = f1 - 1;
							f3 = R*SQUAREROOT3;
							f3 = f3*((float)yShift);
							f3 = f3 + ((float)noisex);
							f1 = f1 + f3;
							
							f2 = R*2;
							f2 = f2*((float)xShift);
							f2 = f2 + WORLD_Y(297);
							f3 = ((float)xEnd)*R;
							f3 = -f3;
							f3 = f3 + ((float)noisey);
							f2 = f2 + f3;

							if(n == 7) // '8-ball'
							{
								if(!bMsgBalls)
									m_ball[MAX_WHITEBALLS+n]->Init( f1, f2, 0, MAX_WHITEBALLS+n, 0.0f*DEG2RAD, 90.0f*DEG2RAD, 0.0f*DEG2RAD);
								else
									m_ball[MAX_WHITEBALLS+n]->Init( f1, f2, 0, MAX_WHITEBALLS+n);
							}
							else // other balls
							{
								float rx = (rand()%360)*DEG2RAD;
								float ry = (rand()%360)*DEG2RAD;
								float rz = (rand()%360)*DEG2RAD;
								if(bMsgBalls) rx = ry = rz = 0.0f;

								m_ball[MAX_WHITEBALLS+n]->Init( f1, f2, 0, MAX_WHITEBALLS+n, rx, ry, rz);
							}
							
							if(xShift==xEnd) 
							{
								xShift = 0;
								yShift--;
								xEnd++;
							}
							else xShift++;

							break;
						} // if
						n++;
						if(n == 15)
							n = 0;
					} // end for i
				} // end for m

			}
			else if(m_wildRackType == 1) // Groups
				ReRack8BALL_Group(rndval, 15, FALSE, bMsgBalls);
			else if(m_wildRackType == 2) // Random
				ReRack8BALL_Random(rndval, 15, TRUE, bMsgBalls);
		}
		break;
// UK-POOL starting postition
		case RERACK_UKPOOL:
		{
			//m_colourseq = 2; // if yellow is potted, it stays in. And colourseq++ ready for the green ball etc...
			//m_rednext = FALSE;

			//m_score2P = 0;
			//m_break2P = 60*50;	// our shot-timer
				
						
			// set all the ball positions & colours
			m_ball[MAX_WHITEBALLS-1]->Init(WORLD_X(591+65+120/*912+5*/),WORLD_Y(297),0, WHITE);
			
			if(m_wildRackType == 0)
			{
			int n;
			float R = (BALL_RADIUS+0.4f);
			float noisex, noisey;

			// relative to snooker x & y (not screen x & Y)
			int xShift, yShift, xEnd;
			xShift = 0;
			yShift = 0;
			xEnd = 0;
			
			
				// build triangle of balls!
				
				// now randomly pick any Red / Yellow ball
				int i = 0;
				int pick = rand()%15;
				
				BOOL bStripe = TRUE;
				int m;
				for(m=0;m<15;m++)
				{
					bStripe = rackOrder[m]; // stripe or spot ball?
					//n = 0;
					n = pick;
					for(i=0;i<15;i++)
					{
						//  'STRIPES'			 'SPOTS'						 '8-BALL'
						if( (bStripe && n>=8) || (!bStripe && n<=6 && m != 4) || (m == 4 && n == 7))
						if(!TestFlag(m_ball[MAX_WHITEBALLS+n]->m_flags, Flg_BALL_ACTIVE))
						{
							fn = n;
							//noisex = 0;//((rand()%200) - 100)*0.01f;
							//noisey = 0;//((rand()%200) - 100)*0.01f;
							noisex = ((rand()%200) - 100)*0.01f;
							noisey = ((rand()%200) - 100)*0.01f;
										 
							f1 = WORLD_X(331+10-2+65 + 18 + 30);
							f2 = R*2;
							f1 = f1 - f2;
							f1 = f1 - 1;
							f3 = R*SQUAREROOT3;
							f3 = f3*((float)yShift);
							f3 = f3 + ((float)noisex);
							f1 = f1 + f3;
							
							f2 = R*2;
							f2 = f2*((float)xShift);
							f2 = f2 + WORLD_Y(297);
							f3 = ((float)xEnd)*R;
							f3 = -f3;
							f3 = f3 + ((float)noisey);
							f2 = f2 + f3;

							if(n == 7) // '8-ball'
							{
								if(!bMsgBalls)
									m_ball[MAX_WHITEBALLS+n]->Init( f1, f2, 0, MAX_WHITEBALLS+n, 0.0f*DEG2RAD, 90.0f*DEG2RAD, 0.0f*DEG2RAD);
								else
									m_ball[MAX_WHITEBALLS+n]->Init( f1, f2, 0, MAX_WHITEBALLS+n);
							}
							else // other balls
							{
								float rx = (rand()%360)*DEG2RAD;
								float ry = (rand()%360)*DEG2RAD;
								float rz = (rand()%360)*DEG2RAD;
								if(bMsgBalls) rx = ry = rz = 0.0f;
								if(n>7) m_ball[MAX_WHITEBALLS+n]->Init( f1, f2, 0, MAX_WHITEBALLS+n, rx, ry, rz);
									else m_ball[MAX_WHITEBALLS+n]->Init( f1, f2, 0, MAX_WHITEBALLS+n, rx, ry, rz);
							}
							
							if(xShift==xEnd) 
							{
								xShift = 0;
								yShift--;
								xEnd++;
							}
							else xShift++;

							break;
						} // if
						n++;
						if(n == 15)
							n = 0;
					} // end for i
				} // end for m

			}
			else if(m_wildRackType == 1) // Groups
				ReRack8BALL_Group(rndval, 15, FALSE, bMsgBalls);
			else if(m_wildRackType == 2) // Random
				ReRack8BALL_Random(rndval, 15, TRUE, bMsgBalls);
		}
		break;

		case RERACK_8BALLSCATTERED:
		{
			int n;
			float R = (BALL_RADIUS+0.4f);

			// set all the ball positions & colours
			m_ball[MAX_WHITEBALLS-1]->Init(WORLD_X(591+65+120/*912+5*/),WORLD_Y(297),0, WHITE);
			
			// relative to snooker x & y (not screen x & Y)
			int m;
			float x, y;
			BOOL bValidPos;
			BOOL bFoundSpot;
		
			// spread reds everywhere!
			for(n=0;n<15/*MAX_REDBALLS*/;n++)
			{
				bValidPos = FALSE;
				while(!bValidPos)
				{
					//x = (175-75)*2.5f + rand()%(912-175+150 + 75)*0.75f;
					//y = (199-125)*1.5f + rand()%(395-199+250)*0.75f;
					x = (235 + BALL_RADIUS*0.5) + rand()%(710-(int)BALL_RADIUS*1);
					y = (125 + BALL_RADIUS*0.5) + rand()%(340-(int)BALL_RADIUS*1);
					
					// check if ball 'n' is on top of another ball m
					bFoundSpot = TRUE;
					for(m=0;m<MAX_TOTALBALLS;m++)
					if(m!=(MAX_WHITEBALLS+n))
					if(TestFlag(m_ball[m]->m_flags, Flg_BALL_ACTIVE))
					if(m_ball[m]->Proximity(-WORLD_X(x), -WORLD_Y(y), 0, BALL_RADIUS*4.0f) == TRUE)
					{
						bFoundSpot = FALSE;
						break;					
					}

					// check if we have found a valid spot
					if(bFoundSpot)
					{
						m_ball[MAX_WHITEBALLS+n]->Init( WORLD_X(x), WORLD_Y(y),0, MAX_WHITEBALLS+n,
														(rand()%360)*DEG2RAD, (rand()%360)*DEG2RAD, (rand()%360)*DEG2RAD);
						bValidPos = TRUE;
					}
				}
			}
		}
		break;

		case RERACK_9BALLSCATTERED:
		{
			int n;
			float R = (BALL_RADIUS+0.4f);

			// set all the ball positions & colours
			m_ball[MAX_WHITEBALLS-1]->Init(WORLD_X(591+65+120/*912+5*/),WORLD_Y(297),0, WHITE);
			
			// relative to snooker x & y (not screen x & Y)
			int m;
			float x, y;
			BOOL bValidPos;
			BOOL bFoundSpot;
		
			// spread reds everywhere!
			for(n=0;n<9/*MAX_REDBALLS*/;n++)
			{
				bValidPos = FALSE;
				while(!bValidPos)
				{
					//x = (175-75)*2.5f + rand()%(912-175+150 + 75)*0.75f;
					//y = (199-125)*1.5f + rand()%(395-199+250)*0.75f;
					x = (235 + BALL_RADIUS*0.5) + rand()%(710-(int)BALL_RADIUS*1);
					y = (125 + BALL_RADIUS*0.5) + rand()%(340-(int)BALL_RADIUS*1);
					
					// check if ball 'n' is on top of another ball m
					bFoundSpot = TRUE;
					for(m=0;m<MAX_TOTALBALLS;m++)
					if(m!=(MAX_WHITEBALLS+n))
					if(TestFlag(m_ball[m]->m_flags, Flg_BALL_ACTIVE))
					if(m_ball[m]->Proximity(-WORLD_X(x), -WORLD_Y(y), 0, BALL_RADIUS*4.0f) == TRUE)
					{
						bFoundSpot = FALSE;
						break;					
					}

					// check if we have found a valid spot
					if(bFoundSpot)
					{
						m_ball[MAX_WHITEBALLS+n]->Init( WORLD_X(x), WORLD_Y(y),0, MAX_WHITEBALLS+n,
														(rand()%360)*DEG2RAD, (rand()%360)*DEG2RAD, (rand()%360)*DEG2RAD);
						bValidPos = TRUE;
					}
				}
			}
		}
		break;		

		case RERACK_6BALLSCATTERED:
		{
			int n;
			float R = (BALL_RADIUS+0.4f);

			// set all the ball positions & colours
			m_ball[MAX_WHITEBALLS-1]->Init(WORLD_X(591+65+120/*912+5*/),WORLD_Y(297),0, WHITE);
			
			// relative to snooker x & y (not screen x & Y)
			int m;
			float x, y;
			BOOL bValidPos;
			BOOL bFoundSpot;
		
			// spread reds everywhere!
			for(n=0;n<6/*MAX_REDBALLS*/;n++)
			{
				bValidPos = FALSE;
				while(!bValidPos)
				{
					//x = (175-75)*2.5f + rand()%(912-175+150 + 75)*0.75f;
					//y = (199-125)*1.5f + rand()%(395-199+250)*0.75f;
					x = (235 + BALL_RADIUS*0.5) + rand()%(710-(int)BALL_RADIUS*1);
					y = (125 + BALL_RADIUS*0.5) + rand()%(340-(int)BALL_RADIUS*1);
					
					// check if ball 'n' is on top of another ball m
					bFoundSpot = TRUE;
					for(m=0;m<MAX_TOTALBALLS;m++)
					if(m!=(MAX_WHITEBALLS+n))
					if(TestFlag(m_ball[m]->m_flags, Flg_BALL_ACTIVE))
					if(m_ball[m]->Proximity(-WORLD_X(x), -WORLD_Y(y), 0, BALL_RADIUS*4.0f) == TRUE)
					{
						bFoundSpot = FALSE;
						break;					
					}

					// check if we have found a valid spot
					if(bFoundSpot)
					{
						m_ball[MAX_WHITEBALLS+n]->Init( WORLD_X(x), WORLD_Y(y),0, MAX_WHITEBALLS+n,
														(rand()%360)*DEG2RAD, (rand()%360)*DEG2RAD, (rand()%360)*DEG2RAD);
						bValidPos = TRUE;
					}
				}
			}
		}
		break;		

		case RERACK_10BALLSCATTERED:
		{
			int n;
			float R = (BALL_RADIUS+0.4f);

			// set all the ball positions & colours
			m_ball[MAX_WHITEBALLS-1]->Init(WORLD_X(591+65+120/*912+5*/),WORLD_Y(297),0, WHITE);
			
			// relative to snooker x & y (not screen x & Y)
			int m;
			float x, y;
			BOOL bValidPos;
			BOOL bFoundSpot;
		
			// spread reds everywhere!
			for(n=0;n<10/*MAX_REDBALLS*/;n++)
			{
				bValidPos = FALSE;
				while(!bValidPos)
				{
					//x = (175-75)*2.5f + rand()%(912-175+150 + 75)*0.75f;
					//y = (199-125)*1.5f + rand()%(395-199+250)*0.75f;
					x = (235 + BALL_RADIUS*0.5) + rand()%(710-(int)BALL_RADIUS*1);
					y = (125 + BALL_RADIUS*0.5) + rand()%(340-(int)BALL_RADIUS*1);
					
					// check if ball 'n' is on top of another ball m
					bFoundSpot = TRUE;
					for(m=0;m<MAX_TOTALBALLS;m++)
					if(m!=(MAX_WHITEBALLS+n))
					if(TestFlag(m_ball[m]->m_flags, Flg_BALL_ACTIVE))
					if(m_ball[m]->Proximity(-WORLD_X(x), -WORLD_Y(y), 0, BALL_RADIUS*4.0f) == TRUE)
					{
						bFoundSpot = FALSE;
						break;					
					}

					// check if we have found a valid spot
					if(bFoundSpot)
					{
						m_ball[MAX_WHITEBALLS+n]->Init( WORLD_X(x), WORLD_Y(y),0, MAX_WHITEBALLS+n,
														(rand()%360)*DEG2RAD, (rand()%360)*DEG2RAD, (rand()%360)*DEG2RAD);
						bValidPos = TRUE;
					}
				}
			}
		}
		break;

		// 8-ball pool starting postition
		case RERACK_BOWLS:
		{						
			// set all the ball positions & colours:

			// white-ball is the "Jack"
			m_ball[MAX_WHITEBALLS-1]->Init(WORLD_X(591+65+120/*912+5*/),WORLD_Y(297),0, WHITE);
			
			
			// 5 blue bowls...
			//int n;
			//for(n = 0; n < 5; n++)
			//{
			//	m_ball[MAX_WHITEBALLS+n]->Init(0 - WORLD_X(591+65+120/*912+5*/) - (BALL_RADIUS*2*2) + n*(BALL_RADIUS*2) - 1000/* - (50000)*/, WORLD_Y(297) - BALL_RADIUS*2*13, 0, MAX_WHITEBALLS+n/*, 
			//								   (rand()%360)*DEG2RAD, (rand()%360)*DEG2RAD, (rand()%360)*DEG2RAD*/);
			//}

			// 5 red bowls...
			//for(n = 5; n < 10; n++)
			//{
			//	m_ball[MAX_WHITEBALLS+n]->Init(0 + WORLD_X(591+65+120/*912+5*/) - (BALL_RADIUS*2*2) + (n - 5)*(BALL_RADIUS*2) - 1000/* - (50000)*/, WORLD_Y(297) - BALL_RADIUS*2*13, 0, MAX_WHITEBALLS+n/*, 
			//								   (rand()%360)*DEG2RAD, (rand()%360)*DEG2RAD, (rand()%360)*DEG2RAD*/);
			//}

				
		}
		break;

		case RERACK_EMPTY:
		// set white ball position (this is only to help mini camera at ONLY!)
		m_ball[MAX_WHITEBALLS-1]->Init(WORLD_X(-100/*912-300*/),WORLD_Y(250),0, WHITE);
		break;
	} // end switch


	////////////////////////////////////////////////////////////////
	// Add pocket objects WILD type setting
	#ifdef THE_POOL_CLUB	
	if(m_customTable->m_tableType >= 5 && m_customTable->m_tableType <= 8) // Random Multiple Holes table types
	{
		// remove all pockets
		m_customTable->ClearPockets();

		//srand(rndval); // wont work, cause balls could be in diff positions

		int xLimit = 0;
		float x = 100.0f;
		int yLimit = 0;
		float y = 0.0f;
		int holes = (m_customTable->m_tableType-4)*2+1; // 3,5,7,9 holes
		int maxTries = 0;
		for(int h=0;h<holes;h++)
		{			
			for(maxTries=0;maxTries<20;maxTries++)
			{			
				xLimit = (1500+300+26) - 650;
				x = xLimit - rand()%(xLimit*2);
				yLimit = (560+300+26) - 400;
				y = yLimit - rand()%(yLimit*2);
				
				// TODO: check this pocket is not on any balls or pockets!
				BOOL bSuccess = TRUE;
				for(int p=0;p<m_customTable->m_pocket_n;p++)
				{
					if( m_customTable->PocketProximity(p, CVector(x, y, 0), m_customTable->m_pocketProp[p].m_suctionRadius) )
					{						
						bSuccess = FALSE;
						break;
					}
				} // end for p

				if(bSuccess)
				for(int b=0;b<MAX_TOTALBALLS;b++)
				if(TestFlag(m_ball[b]->m_flags, Flg_BALL_ACTIVE))
				if( m_ball[b]->Proximity(x, y, 0, BALL_RADIUS+(BALL_RADIUS_INITIAL*2.4)) )
				{
					bSuccess = FALSE;
					break;
				}

				if(bSuccess) goto _success; // there is no pockets - this is our first pocket!
			} // end maxTries
			
			if(maxTries == 20) continue; // can't create this hole, so skip! unlucky this time

			_success:;

			// create center spot pocket
			m_customTable->AddPocket(CVector(x, y, (0)), POCKET_OBJECT, BALL_RADIUS_INITIAL*1.4, BALL_RADIUS_INITIAL*2.4/* *4 */, 0.05 );
			_end:;
		} // end for h
	}
	#endif //THE_POOL_CLUB
	////////////////////////////////////////////////////////////////

	//#ifdef FIXEDMODE
	//PullBallsApartFIXPOINT();
	//#endif // FIXEDMODE
}

void CSnooker::ReRackSTART(int rndval, int maxReds, int wildRackType)
{
	//wildRackType = 8;
	//m_wildRackType = 8;
	//maxReds = 1;

	if(maxReds <= 0) return;

	float f1,f2,f3,f4;
	float fn;
	int n;
	//int R = (BALL_RADIUS+1);
	//float R = (BALL_RADIUS+0.4);
	//float R = (BALL_RADIUS + 0.1f);
	//float R = (BALL_RADIUS - 0.7f); // (NEW FROM BRANDON)
	float R = (BALL_RADIUS+0.4f); // (NEW FROM BRANDON)
	
	float noisex, noisey;
	
	BOOL bMixColoursToReds = FALSE;
	int mixColoursToReds[15+6];
	
	// relative to snooker x & y (not screen x & Y)
	int xShift, yShift, xEnd;
	xShift = 0;
	yShift = 0;
	xEnd = 0;

	float RedsXOffset = 0.0f;
	if(wildRackType == 4) // Reds Behind Blue
		RedsXOffset = ( WORLD_X(591) - WORLD_X(331+10) );
	else if(wildRackType == 5) // Reds Behind Brown
		RedsXOffset = ( WORLD_X(912) - WORLD_X(331+10) );
	
	if(wildRackType == 6) // 21-Reds
	{
		maxReds = 21;
	}
	else if(wildRackType == 7) // 28-Reds
	{
		maxReds = 21+7;
	}
	else if(wildRackType == 3) // Triangle Mix
	{
		maxReds = 15+6;
		bMixColoursToReds = TRUE;

		for(n=0;n<(15+6);n++) // clear
			mixColoursToReds[n] = 0;

		int colour;
		int rndSlot;
		for(colour = YELLOW;colour <= BLACK;colour++)
		while(1)
		{
			rndSlot	= rand()%(15+6);
			if(!mixColoursToReds[rndSlot])
			{
				mixColoursToReds[rndSlot] = colour;
				break;
			}
		}
	}

	if(wildRackType == 9) // Scrap Heap
	{
		// relative to snooker x & y (not screen x & Y)
		int m;
		float x, y;
		BOOL bValidPos;
		BOOL bFoundSpot;
	
		// spread reds everywhere!
		for(n=0;n<(15+6);n++)
		{
			bValidPos = FALSE;
			while(!bValidPos)
			{
				x = (175-75+100-45) + rand()%(591/*912*/-175+150);
				y = (199-75+870) + rand()%(395-199+150);
				x*=0.65f;
				y*=0.25f;
				
				// check if ball 'n' is on top of another ball m
				bFoundSpot = TRUE;
				for(m=0;m<MAX_TOTALBALLS;m++)
				if(m!=(MAX_WHITEBALLS+n))
				if(TestFlag(m_ball[m]->m_flags, Flg_BALL_ACTIVE))
				if(m_ball[m]->Proximity(-WORLD_X(x), -WORLD_Y(y), 0, BALL_RADIUS*2.0f) == TRUE)
				{
					bFoundSpot = FALSE;
					break;					
				}

				// check if we have found a valid spot
				if(bFoundSpot)
				{
					if(n>=15) m_ball[MAX_WHITEBALLS+MAX_REDBALLS+(n-15)]->Init( WORLD_X(x), WORLD_Y(y),0, YELLOW+(n-15), 0, 0, 0, colourOnDefaultSpots[YELLOW+(n-15)].x, colourOnDefaultSpots[YELLOW+(n-15)].y);
					else m_ball[MAX_WHITEBALLS+n]->Init( WORLD_X(x), WORLD_Y(y),0, RED);
					bValidPos = TRUE;
				}
			}
		}
	}
	else
	{
		// build triangle of reds!
		for(n=0;n<maxReds/*MAX_REDBALLS*/;n++)
		{
			fn = n;
			//noisex = 0;//((rand()%200) - 100)*0.01f;
			//noisey = 0;//((rand()%200) - 100)*0.01f;
			noisex = ((rand()%200) - 100)*0.01f;
			noisey = ((rand()%200) - 100)*0.01f;

			f1 = WORLD_X(331+10);
			f2 = R*2;
			f1 = f1 - f2;
			f1 = f1 - 1;
			f3 = R*SQUAREROOT3;
			f3 = f3*((float)yShift) * 1.005f;
			f3 = f3 + ((float)noisex);
			f1 = f1 + f3;
			
			f2 = R*2;
			f2 = f2*((float)xShift) * 0.995f;
			f2 = f2 + WORLD_Y(297);
			f3 = ((float)xEnd)*R;
			f3 = -f3;
			f3 = f3 + ((float)noisey);
			f2 = f2 + f3;

			f1 = f1 + RedsXOffset;

			if(bMixColoursToReds && mixColoursToReds[n]>=YELLOW)
			{
				m_ball[MAX_WHITEBALLS+MAX_REDBALLS+(mixColoursToReds[n]-YELLOW)]->Init( f1, f2, 0, mixColoursToReds[n], 0, 0, 0, colourOnDefaultSpots[mixColoursToReds[n]].x, colourOnDefaultSpots[mixColoursToReds[n]].y);
			}
			else		
			{
				m_ball[MAX_WHITEBALLS+n]->Init( f1, f2, 0, RED);
				//m_ball[MAX_WHITEBALLS+n]->Init( WORLD_X(331+10) - (R*2) - 1 + yShift*(R*SQUAREROOT3) + noisex,
				//								WORLD_Y(297) + xShift*(R*2) - xEnd*(R) + noisey, 0, RED);
			}
			if(xShift==xEnd) 
			{
				xShift = 0;
				yShift--;
				xEnd++;
			}
			else xShift++;
		}
	} // end else

	// position the colours
	if(wildRackType == 1) // Randomly Spotted Colours
	{
		int n,m,i;
		n = 0;

		for(m=0;m<6;m++)
		{
			int rndPick = rand()%6;
			n = rndPick;
			
			for(i=0;i<6;i++)
			{
				if(!TestFlag(m_ball[MAX_WHITEBALLS+MAX_REDBALLS+n]->m_flags, Flg_BALL_ACTIVE))
				{
					ReRackColourOnSpot(m, YELLOW+n);
					break;
				}

				n++;
				if(n==6) n = 0;
			}
		}
	}
	else if(wildRackType == 2) // Randomly Placed Colours
	{
		int n,m,i;
		n = 0;

		for(m=0;m<6;m++)
		{
			int rndPick = rand()%6;
			n = rndPick;
			
			for(i=0;i<6;i++)
			{
				if(!TestFlag(m_ball[MAX_WHITEBALLS+MAX_REDBALLS+n]->m_flags, Flg_BALL_ACTIVE))
				{
					ReRackColourOnSpot(m, YELLOW+n, TRUE);
					break;
				}

				n++;
				if(n==6) n = 0;
			}
		}
	}
	else if(wildRackType == 8) // All Blacks
	{
		m_ball[MAX_WHITEBALLS+MAX_REDBALLS]->Init(WORLD_X(912),WORLD_Y(199+12),0, BLACK);//, 0, 0, 0, colourOnDefaultSpots[BLACK].x, colourOnDefaultSpots[BLACK].y);
		m_ball[MAX_WHITEBALLS+MAX_REDBALLS+1]->Init(WORLD_X(912),WORLD_Y(395-12),0, BLACK);//, 0, 0, 0, colourOnDefaultSpots[BLACK].x, colourOnDefaultSpots[BLACK].y);
		m_ball[MAX_WHITEBALLS+MAX_REDBALLS+2]->Init(WORLD_X(912),WORLD_Y(297),0, BLACK);//, 0, 0, 0, colourOnDefaultSpots[BLACK].x, colourOnDefaultSpots[BLACK].y);
		m_ball[MAX_WHITEBALLS+MAX_REDBALLS+3]->Init(WORLD_X(591),WORLD_Y(297),0, BLACK);//, 0, 0, 0, colourOnDefaultSpots[BLACK].x, colourOnDefaultSpots[BLACK].y);
		m_ball[MAX_WHITEBALLS+MAX_REDBALLS+4]->Init(WORLD_X(331+10),WORLD_Y(297),0, BLACK);//, 0, 0, 0, colourOnDefaultSpots[BLACK].x, colourOnDefaultSpots[BLACK].y);
		m_ball[MAX_WHITEBALLS+MAX_REDBALLS+5]->Init(WORLD_X(175-10),WORLD_Y(297),0, BLACK);//, 0, 0, 0, colourOnDefaultSpots[BLACK].x, colourOnDefaultSpots[BLACK].y);
	}
	else if(wildRackType == 3); // Triangle Mix
	else if(wildRackType == 9); // Scrap Heap
	else // Normal racking of colours
	{
		m_ball[MAX_WHITEBALLS+MAX_REDBALLS]->Init(WORLD_X(912),WORLD_Y(199+12),0, YELLOW);
		m_ball[MAX_WHITEBALLS+MAX_REDBALLS+1]->Init(WORLD_X(912),WORLD_Y(395-12),0, GREEN);
		m_ball[MAX_WHITEBALLS+MAX_REDBALLS+2]->Init(WORLD_X(912),WORLD_Y(297),0, BROWN);
		m_ball[MAX_WHITEBALLS+MAX_REDBALLS+3]->Init(WORLD_X(591),WORLD_Y(297),0, BLUE);
		m_ball[MAX_WHITEBALLS+MAX_REDBALLS+4]->Init(WORLD_X(331+10),WORLD_Y(297),0, PINK);
		m_ball[MAX_WHITEBALLS+MAX_REDBALLS+5]->Init(WORLD_X(175-10),WORLD_Y(297),0, BLACK);
	}
}

void CSnooker::ReRackColourOnSpot(int n, int colour, BOOL bRndPos)
{	
	float x = 0.0f;
	float y = 0.0f;
	
	if(!bRndPos)
	{
		switch(n)
		{
		case 0:
		x = WORLD_X(912); y = WORLD_Y(199+12);
		break;
		case 1:
		x = WORLD_X(912); y = WORLD_Y(395-12);
		break;
		case 2:
		x = WORLD_X(912); y = WORLD_Y(297);
		break;
		case 3:
		x = WORLD_X(591); y = WORLD_Y(297);
		break;
		case 4:
		x = WORLD_X(331+10); y = WORLD_Y(297);
		break;
		case 5:
		x = WORLD_X(175-10); y = WORLD_Y(297);
		break;
		}
	}
	else
	{
		// relative to snooker x & y (not screen x & Y)
		int m;
		BOOL bValidPos;
		BOOL bFoundSpot;
	
		// place red anywhere on table
		bValidPos = FALSE;
		while(!bValidPos)
		{
			x = (175-75) + rand()%(912-175+150 + 75);
			y = (199-125) + rand()%(395-199+250);

			// check if ball 'n' is on top of another ball m
			bFoundSpot = TRUE;
			for(m=0;m<MAX_TOTALBALLS;m++)
			if(m!=(MAX_WHITEBALLS+n))
			if(TestFlag(m_ball[m]->m_flags, Flg_BALL_ACTIVE))
			if(m_ball[m]->Proximity(-WORLD_X(x), -WORLD_Y(y), 0, BALL_RADIUS*4.0f) == TRUE)
			{
				bFoundSpot = FALSE;
				break;					
			}

			// check if we have found a valid spot
			if(bFoundSpot)
			{
				x = WORLD_X(x); y = WORLD_Y(y);
				bValidPos = TRUE;
			}
		}
	}
		
	m_ball[MAX_WHITEBALLS+MAX_REDBALLS+(colour-YELLOW)]->Init(x,y,0, colour, 0, 0, 0, colourOnDefaultSpots[colour].x, colourOnDefaultSpots[colour].y);
}

void CSnooker::ReRack8BALL_Group(int rndval, int maxBalls, BOOL bScatterBalls, int bMsgBalls)
{
	if(maxBalls <= 0) return;

	float f1,f2,f3,f4;
	float fn;
	int n;
	float R = (BALL_RADIUS+0.4f);
	float SAFE_RADIUS_DISTANCE = BALL_RADIUS*2.4*4;
	float noisex, noisey;
	float offsetX, offsetY;
	
	int randRangeX = rand()%int(710-BALL_RADIUS*1*3);
	int randRangeY = rand()%int(340-BALL_RADIUS*1*3);
	int randomOffseX = (235 + BALL_RADIUS*0.5*3) + randRangeX;
	int randomOffseY = (125 + BALL_RADIUS*0.5*3) + randRangeY;
			
	// relative to snooker x & y (not screen x & Y)
	int xShift, yShift, xEnd;
	xShift = 0;
	yShift = 0;
	xEnd = 0;

	// build triangle of balls!
	
	// now randomly pick any stripe/spot ball
	int i = 0;
	int pick = rand()%maxBalls;
	
	BOOL bStripe = TRUE;
	int m;
	int count = 0;
	for(m=0;m<maxBalls;m++)
	{
		//bStripe = rackOrder[m]; // stripe or spot ball?
		//n = 0;
		n = pick;
		for(i=0;i<maxBalls;i++)
		{
			//  'STRIPES'			 'SPOTS'						 '8-BALL'
			//if( (bStripe && n>=8) || (!bStripe && n<=6 && m != 4) || (m == 4 && n == 7))
			if(!TestFlag(m_ball[MAX_WHITEBALLS+n]->m_flags, Flg_BALL_ACTIVE))
			{
				fn = n;
				//noisex = 0;//((rand()%200) - 100)*0.01f;
				//noisey = 0;//((rand()%200) - 100)*0.01f;
				noisex = ((rand()%200) - 100)*0.01f;
				noisey = ((rand()%200) - 100)*0.01f;
				
				/////////////////////////////////////////
				offsetX = 0.0f;
				offsetY = 0.0f;

				if(bScatterBalls)
				{
					xShift = 0;
					yShift = 0;
					xEnd = 0;
					randRangeX = rand()%int(710-BALL_RADIUS*1*3);
					randRangeY = rand()%int(340-BALL_RADIUS*1*3);
					randomOffseX = (235 + BALL_RADIUS*0.5*3) + randRangeX;
					randomOffseY = (125 + BALL_RADIUS*0.5*3) + randRangeY;		
					offsetY = WORLD_Y(randomOffseY);
					offsetX = WORLD_X(randomOffseX);
 
					// check for ball location inside hole region
					if(m_customTable->m_tableType == 3 || m_customTable->m_tableType == 4) // center/black-hole
					{		
						if(offsetX < (SAFE_RADIUS_DISTANCE) && offsetX >= 0)
							offsetX = (SAFE_RADIUS_DISTANCE);

						if(offsetX > -(SAFE_RADIUS_DISTANCE) && offsetX <= 0)
							offsetX = -(SAFE_RADIUS_DISTANCE);

						if(offsetY < (SAFE_RADIUS_DISTANCE) && offsetY >= 0)
							offsetY = (SAFE_RADIUS_DISTANCE);

						if(offsetY > -(SAFE_RADIUS_DISTANCE) && offsetY <= 0)
							offsetY = -(SAFE_RADIUS_DISTANCE);			
					}
				}
				else
				{
					if(count==6) // reset position
					{
						xShift = 0;
						yShift = 0;
						xEnd = 0;
						randRangeX = rand()%int(710-BALL_RADIUS*1*3);
						randRangeY = rand()%int(340-BALL_RADIUS*1*3);
						randomOffseX = (235 + BALL_RADIUS*0.5*3) + randRangeX;
						randomOffseY = (125 + BALL_RADIUS*0.5*3) + randRangeY;		
					}
					if(count==(6+6)) // reset position
					{
						xShift = 0;
						yShift = 0;
						xEnd = 0;
						randRangeX = rand()%int(710-BALL_RADIUS*1*3);
						randRangeY = rand()%int(340-BALL_RADIUS*1*3);
						randomOffseX = (235 + BALL_RADIUS*0.5*3) + randRangeX;
						randomOffseY = (125 + BALL_RADIUS*0.5*3) + randRangeY;							
					}
					if(count<6)
					{
						offsetY = WORLD_Y(randomOffseY);//-BALL_RADIUS*2*1.5; // move to right of foot-spot
						offsetX = WORLD_X(randomOffseX);//-BALL_RADIUS*2*1.5; // move to right of foot-spot

						// check for ball location inside hole region
						if(m_customTable->m_tableType == 3 || m_customTable->m_tableType == 4) // center/black-hole
						{
							if(offsetX < (SAFE_RADIUS_DISTANCE) && offsetX >= 0)
								offsetX = (SAFE_RADIUS_DISTANCE);

							if(offsetX > -(SAFE_RADIUS_DISTANCE) && offsetX <= 0)
								offsetX = -(SAFE_RADIUS_DISTANCE);

							if(offsetY < (SAFE_RADIUS_DISTANCE) && offsetY >= 0)
								offsetY = (SAFE_RADIUS_DISTANCE);

							if(offsetY > -(SAFE_RADIUS_DISTANCE) && offsetY <= 0)
								offsetY = -(SAFE_RADIUS_DISTANCE);										
						}
					}
					else if(count<(6+6))
					{
						offsetY = WORLD_Y(randomOffseY);//-BALL_RADIUS*2*1.5; // move to right of foot-spot
						offsetX = WORLD_X(randomOffseX);//-BALL_RADIUS*2*1.5; // move to right of foot-spot
						//offsetY = 0;//BALL_RADIUS*2*1.5; // move to left of foot-spot

						// check for ball location inside hole region
						if(m_customTable->m_tableType == 3 || m_customTable->m_tableType == 4) // center/black-hole
						{	
							if(offsetX < (SAFE_RADIUS_DISTANCE) && offsetX >= 0)
								offsetX = (SAFE_RADIUS_DISTANCE);

							if(offsetX > -(SAFE_RADIUS_DISTANCE) && offsetX <= 0)
								offsetX = -(SAFE_RADIUS_DISTANCE);

							if(offsetY < (SAFE_RADIUS_DISTANCE) && offsetY >= 0)
								offsetY = (SAFE_RADIUS_DISTANCE);

							if(offsetY > -(SAFE_RADIUS_DISTANCE) && offsetY <= 0)
								offsetY = -(SAFE_RADIUS_DISTANCE);																	
						}
					}
					else if(count>=(6+6))
					{
						offsetY = WORLD_Y(randomOffseY);//-BALL_RADIUS*2*1.5; // move to right of foot-spot
						offsetX = WORLD_X(randomOffseX);//-BALL_RADIUS*2*1.5; // move to right of foot-spot
						//offsetY = 0;
						//offsetX = BALL_RADIUS*2*16; // move to foot-rail of table

						// check for ball location inside hole region
						if(m_customTable->m_tableType == 3 || m_customTable->m_tableType == 4) // center/black-hole
						{		
							if(offsetX < (SAFE_RADIUS_DISTANCE) && offsetX >= 0)
								offsetX = (SAFE_RADIUS_DISTANCE);

							if(offsetX > -(SAFE_RADIUS_DISTANCE) && offsetX <= 0)
								offsetX = -(SAFE_RADIUS_DISTANCE);

							if(offsetY < (SAFE_RADIUS_DISTANCE) && offsetY >= 0)
								offsetY = (SAFE_RADIUS_DISTANCE);

							if(offsetY > -(SAFE_RADIUS_DISTANCE) && offsetY <= 0)
								offsetY = -(SAFE_RADIUS_DISTANCE);										
						}
					}
				}
				/////////////////////////////////////////
				
				f1 = BALL_RADIUS*2*2;//WORLD_X(331+10-2+65 + 18);
				f2 = R*2;
				f1 = f1 - f2;
				f1 = f1 - 1;
				f3 = R*SQUAREROOT3;
				f3 = f3*((float)yShift);
				f3 = f3 + ((float)noisex);
				f1 = f1 + f3;
				
				f2 = R*2;
				f2 = f2*((float)xShift);
				f2 = f2 + WORLD_Y(297);
				f3 = ((float)xEnd)*R;
				f3 = -f3;
				f3 = f3 + ((float)noisey);
				f2 = f2 + f3;
				
				if(n == 7) // '8-ball'
				{
					if(!bMsgBalls)
						m_ball[MAX_WHITEBALLS+n]->Init( f1+offsetX, f2+offsetY, 0, MAX_WHITEBALLS+n, 0.0f*DEG2RAD, 90.0f*DEG2RAD, 0.0f*DEG2RAD);
					else
						m_ball[MAX_WHITEBALLS+n]->Init( f1+offsetX, f2+offsetY, 0, MAX_WHITEBALLS+n);
				}
				else // other balls
				{
					float rx = (rand()%360)*DEG2RAD;
					float ry = (rand()%360)*DEG2RAD;
					float rz = (rand()%360)*DEG2RAD;
					if(bMsgBalls) rx = ry = rz = 0.0f;

					m_ball[MAX_WHITEBALLS+n]->Init( f1+offsetX, f2+offsetY, 0, MAX_WHITEBALLS+n, rx, ry, rz);
				}
				
				if(xShift==xEnd) 
				{
					xShift = 0;
					yShift--;
					xEnd++;
				}
				else xShift++;

				count++;

				break;
			} // if
			n++;
			if(n == maxBalls)
				n = 0;
		} // end for i
	} // end for m
}

void CSnooker::ReRack8BALL_Random(int rndval, int maxBalls, BOOL bScatterBalls, int bMsgBalls)
{
	if(maxBalls <= 0 || maxBalls > 15) return;
	
	ReRack8BALL_Group(rndval, maxBalls, bScatterBalls, bMsgBalls);
}

void CSnooker::ReRack9BALL_Group(int rndval, int maxBalls, BOOL bScatterBalls, int bMsgBalls)
{
	if(maxBalls <= 0 || maxBalls > 9) return;

	ReRack8BALL_Group(rndval, maxBalls, bScatterBalls, bMsgBalls);
}

void CSnooker::ReRack9BALL_Random(int rndval, int maxBalls, BOOL bScatterBalls, int bMsgBalls)
{
	if(maxBalls <= 0 || maxBalls > 9) return;

	ReRack8BALL_Group(rndval, maxBalls, bScatterBalls, bMsgBalls);
}

void CSnooker::ReRack6BALL_Group(int rndval, int maxBalls, BOOL bScatterBalls, int bMsgBalls)
{
	if(maxBalls <= 0 || maxBalls > 6) return;

	ReRack8BALL_Group(rndval, maxBalls, bScatterBalls, bMsgBalls);
}

void CSnooker::ReRack6BALL_Random(int rndval, int maxBalls, BOOL bScatterBalls, int bMsgBalls)
{
	if(maxBalls <= 0 || maxBalls > 6) return;

	ReRack8BALL_Group(rndval, maxBalls, bScatterBalls, bMsgBalls);
}

void CSnooker::ReRack10BALL_Group(int rndval, int maxBalls, BOOL bScatterBalls, int bMsgBalls)
{
	if(maxBalls <= 0 || maxBalls > 10) return;

	ReRack8BALL_Group(rndval, maxBalls, bScatterBalls, bMsgBalls);
}

void CSnooker::ReRack10BALL_Random(int rndval, int maxBalls, BOOL bScatterBalls, int bMsgBalls)
{
	if(maxBalls <= 0 || maxBalls > 10) return;

	ReRack8BALL_Group(rndval, maxBalls, bScatterBalls, bMsgBalls);
}

void CSnooker::Deinit(void)			
{
	
}

void CSnooker::SetWindowSize(int w,int h)
{
	m_windowWidth = w;
	m_windowHeight = h;
}

void CSnooker::SetTableViewWindowSize(int w,int h)
{
	m_tableview_windowWidth = w;
	m_tableview_windowHeight = h;
}


void CSnooker::MouseMoveReceive(float x, float y, int w, int h, float strikeLine, float strikeDist)
{
	//return;

	if (MyTurn()) return;

	// update vars for short lines
	m_strikeLine = strikeLine;
	m_strikeDist = strikeDist;

	float fx, fy;
	fx = x / (float)w;
	fy = y / (float)h;
	fx *= (float)m_windowWidth;
	fy *= (float)m_windowHeight;

	m_mouseX = fx;
	m_mouseY = fy;

	CPoint point;
	point.x = 0;
	point.y = 0;
	MouseMove(point);
}

void CSnooker::MouseMove(CPoint point)
{
#ifndef MAIN_SERVER


	if (point.x < 0)
	{
		g_gameAppView->m_bMButtonDown = 0;
	}
	//return;
	float x, y;
	float w, h;
	float k;
	M3d_vtx pos;
	M3d_vtx vec;
	float d1, d2, dx, dy;

	D3DVIEWPORT9 mainViewPort;
	d3d::BoundingSphere BSphere;
	D3DXMATRIX view;
	D3DXMATRIX viewInverse;
	d3d::Ray ray;


	m_bMouseMoved = 1;
	m_tableMouseOn = 0; // default is not pointing at the table.
	m_ballMouseOn = 0; // default is not pointing at the table.




	if (!MyTurn()) m_ballMouseOn = 1; // default is not pointing at the table.

	// mouse cue movement
	if (m_cueBallSelect == 1)
	{
		if (MyTurn())
		{


			// scale mouse relative coords (by squaring values)
			float x, y, tmp;
			float screenWidthFactor = m_cx;
			int sign = 1;
			screenWidthFactor /= 800.0f;
			x = Mio_mouse.lX;


			tmp = x*x*0.5f;
			if (tmp > 2000) tmp = 2000;
			if (x<0) x = -tmp;
			else x = tmp;
			// -------------- //
			y = Mio_mouse.lY;

			tmp = y*y*0.5f;
			if (tmp > 2000) tmp = 2000;
			if (y<0) y = -tmp;
			else y = tmp;
			// -------------- //	//
			m_mouseX += ((float)x *  m_mouseCueSensitivity * screenWidthFactor);
			m_mouseY += ((float)y *  m_mouseCueSensitivity * screenWidthFactor);


			// char buf[80];
			// sprintf(buf, "m_mouseX = %f, m_mouseY = %f, x = %f, y = %f, lX = %ld, lY = %ld", m_mouseX, m_mouseY, x, y, Mio_mouse.lX, Mio_mouse.lY);
			// ErrorMessage(m_parent, buf);


			// cue-power 0-100% boundary circle
			float rx = m_mouseX - m_sCPoint.x;
			float ry = m_mouseY - m_sCPoint.y;

			float r = rx*rx + ry*ry;
			float rTest = screenWidthFactor * 375.0f;
			if (r > rTest*rTest)
			{
				// if outside circle - then move back to boundary edge of circle
				CVector R(rx, ry, 0);
				R = R.Unit();
				R = R * rTest;
				m_mouseX = m_sCPoint.x + R.x;
				m_mouseY = m_sCPoint.y + R.y;
			}
		}
		else
		{
			// If it't not our turn, then still do Default Mouse Movement
			m_mouseX = point.x;
			m_mouseY = point.y;
		}
	}


	// mouse camera mode orient/offset
	else if (m_CameraMode == CAMERA_MODE_TV && ((CGameAppView *)m_parent)->m_bRButtonDown)
	{

		CGameAppView *p = (CGameAppView *)m_parent;

		// use mouse X-axis for camera orientation
		p->m_cameraAngle += (float)(Mio_mouse.lX)*MOUSE_CAMERA_SENSITIVY_X;

		// use mouse Y-axis for camera magnitude (offset)
		p->m_cameraMag += (float)(Mio_mouse.lY) *2.0f * MOUSE_CAMERA_SENSITIVY_Y;
		float ang = ((CGameAppView *)m_parent)->m_cameraAngle*DEG2RAD;

	}

	// mouse default movement
	else
	{
		m_mouseX = point.x;
		m_mouseY = point.y;
	}




	//if(m_3dtoggle == 1) return;


	x = m_mouseX;
	y = m_mouseY;
	w = m_windowWidth;
	h = m_windowHeight;


	// if strike is in progress - don't update mouse data
	if (m_cueBallSelect == 10 ||
		m_cueBallSelect == 9 ||
		m_cueBallSelect == 2 ||
		m_cueBallSelect >= 20); // return;



	if (m_cueBallSelect == 0) // just browsing, so we do bounds
		//--	if(m_cueBallSelect == 0 || m_cueBallSelect == 5) // just browsing, so we do bounds
	{
		if (x >= w) goto eyecam;
		if (x<0) goto eyecam;
		if (y >= h) goto eyecam;
		if (y<0) goto eyecam;

		// check if player is in idle mouse movement (special case - when player holds down multiple L & R buttons)
		if (!((CGameAppView *)m_parent)->m_bLButtonDown &&
			!((CGameAppView *)m_parent)->m_bRButtonDown)
			((CGameAppView *)m_parent)->m_bRButtonJustUp = 1;

		// check if player has stood up from shot - and has a ball-in-hand!!
		if (!((CGameAppView *)m_parent)->m_bPracticeEditPlay)//m_snooker->m_bPracticeMode) // Edit mode
		{
			if (((CGameAppView *)m_parent)->m_bRButtonDown)
			if (!((CGameAppView *)m_parent)->m_bLButtonDown)
			if (((CGameAppView *)m_parent)->m_bRButtonJustUp)
			{
				// find the ball we want to grab
				for (int n = 0; n<MAX_TOTALBALLS; n++)
				{
					if (m_ball[n]->Proximity(m_tableMousePos[0], m_tableMousePos[1], m_tableMousePos[2]) == TRUE)
					{
#ifdef FIXEDMODE
						m_sPoint[0] = FROMFIXSMALL(m_ball[n]->f_P.x);
						m_sPoint[1] = FROMFIXSMALL(m_ball[n]->f_P.y);
						m_sPoint[2] = FROMFIXSMALL(m_ball[n]->f_P.z);
#else
						m_sPoint[0] = m_ball[n]->m_P.x;
						m_sPoint[1] = m_ball[n]->m_P.y;
						m_sPoint[2] = m_ball[n]->m_P.z;
#endif
						m_grabBall = n;
						break;
					}
				}
				DropBallOnTable(m_grabBall, m_tableMousePos[0], m_tableMousePos[1]);
			}
		}
		else // normal game mode (Play mode)
		{
			// Make sure it's our turn

			if (MyTurn())
			{

				// check if in practice mode & have ball-in-hand ticked!
				if (m_bPracticeMode)
				if (g_practiceDetails.bOptionsBallInHandAlways)
					m_bBallInHand = 1;
				// ----------------- //

				if (m_bStandUpFromShot)
				if (m_bBallInHand)
				if (((CGameAppView *)m_parent)->m_bRButtonDown)
				if (!((CGameAppView *)m_parent)->m_bLButtonDown)
				if (((CGameAppView *)m_parent)->m_bRButtonJustUp)
				{
					int cueBallIdx = 0;

					if (g_build.gameType == GAMETYPE_POOL)
					if (m_customTable->m_type == CTTYPE_BOWLS)
						cueBallIdx = m_customTable->RefereeBowlsNextStrikeBall();

					// If we're playing Billiards, then cueBallIdx holds 2 possible values, depending on the turn //
					if (g_build.gameType == GAMETYPE_SNOOKER && m_customTable->m_type == CTTYPE_BILLIARDS)
					if (!m_bTurn1P) cueBallIdx = 1;

					DropBallOnTable(cueBallIdx, m_tableMousePos[0], m_tableMousePos[1]);
					/*CVector P;
					P.x = m_tableMousePos[0];
					P.y = m_tableMousePos[1];
					P.z = m_ball[0]->m_P.z;
					((CGameAppView *)m_parent)->SendPlayerBallPos(P, WHITE);*/
				}
			}
		}
	}

	if (!g_pCamera) return; // can't calculate 3d position due to no camera being on.



	//
	// Find Ray from Camera Orgin to Mouse Pointer
	//

	// compute the ray in view space given the clicked screen point





	ray = CalcPickingRay(x, y);
	// transform the ray to world space
	g_pDevice->GetTransform(D3DTS_VIEW, &view);
	D3DXMatrixInverse(&viewInverse, 0, &view);
	TransformRay(&ray, &viewInverse);


	// Check If Our Mouse is over any adverts //
	// Only do if our Mouse Middle Button isn't Pressed //
	g_gameAppView->Process3DAdverts(ray, x, y, w, h);


	// if strike is in progress - don't update mouse data
	if (m_cueBallSelect == 10 ||
		m_cueBallSelect == 9 ||
		m_cueBallSelect == 2 ||
		m_cueBallSelect >= 20) return;


	// If we reached this point of code, then our mouse must be on the table
	m_tableMouseOn = 1; // pointing at the table

	// Let see what Ball (if Any we have our Mouse Pointer on

	if (m_cueBallSelect != 5) //--
	{
		// Find out where our mouse is on the table //
		// ray._origin.z + ray._direction.z * k = 0
		// k = -ray.orgin.z / ray._direction.z
		k = -ray._origin.z / ray._direction.z;

		m_tableMousePos[0] = ray._origin.x + ray._direction.x * k;
		m_tableMousePos[1] = ray._origin.y + ray._direction.y * k;
		m_tableMousePos[2] = BALL_POSITIONZOFFSET + BALL_DRAWOFFSETZ;


		m_tableMousePosTmp[0] = m_tableMousePos[0];
		m_tableMousePosTmp[1] = m_tableMousePos[1];
		m_tableMousePosTmp[2] = m_tableMousePos[2];


		// Check if our Mouse is on the Table or Minicam //
		g_pDevice->GetViewport(&mainViewPort);
		if (y > mainViewPort.Height)
		{
			int p;
			p = 10;
			goto eyecam;
		}



		// check mouse over a ball
		for (int n = 0; n<MAX_TOTALBALLS; n++)
		if (TestFlag(m_ball[n]->m_flags, Flg_BALL_ACTIVE))
		{

			BSphere._center = D3DXVECTOR3(m_ball[n]->m_P.x, m_ball[n]->m_P.y, m_ball[n]->m_P.z);
			BSphere._radius = BALL_RADIUS;


			float dx, dy;
			dx = m_ball[n]->m_P.x - m_tableMousePos[0];
			dy = m_ball[n]->m_P.y - m_tableMousePos[1];
			if (sqrt(dx*dx + dy*dy) <= BALL_RADIUS)
				//if( RaySphereIntTest(&ray, &BSphere) )
			{
				//::MessageBox(0, "Hit!", "HIT", 0);

				if (!m_ball[n]->m_bMouseOver) // Mouse on
				{
					//m_ball[n]->MouseOn();
					g_ballmouseoncyc = g_cyc;
					// subtract this shit (g_ballmouseoncyc) from g_cyc at the alpha bit
					// i.e. sin((float)g_cya-g_ballmouseoncyc)
				}
				m_ball[n]->m_bMouseOver = TRUE;
			}
			else
			{
				m_ball[n]->m_bMouseOver = FALSE;
			}
		}
		////////////////////////////////////////////////////////
	}
	else
	{
		k = -ray._origin.z / ray._direction.z;
		m_tableMousePosTmp[0] = ray._origin.x + ray._direction.x * k;
		m_tableMousePosTmp[1] = ray._origin.y + ray._direction.y * k;
		m_tableMousePosTmp[2] = BALL_POSITIONZOFFSET + BALL_DRAWOFFSETZ;

	}


	m_tableMouseOn = 1; // pointing at the table



	/*
	// Project this vector from m_cameraO till it hits the table's surface Z = 0, that plane.
	// (m_cameraO + vec*k) scalar* (0,0,1) = 0

	// hence: m_cameraO[2] + vec[2] * k = 0;

	if(vec[2]>-0.001f) goto eyecam;//return; // pointing away from table

	k = -(g_pCamera->_pos[2])/vec[2]; // k should be a +ve value to indicate hitting the table.

	if(k<0) goto eyecam; //return; // failed to intercept on the +ve direction

	m_tableMouseOn = 1; // pointing at the table

	if(m_cueBallSelect != 5) //--
	{

	m_tableMousePos[0] = g_pCamera->_pos[0] + vec[0] * k;
	m_tableMousePos[1] = g_pCamera->_pos[1] + vec[1] * k;
	//	m_tableMousePos[2] = BALL_RADIUS*2.8f;//0; //	m_tableMousePos[2] = m_cameraO[2] + vec[2] * k; // should equal zero.
	//m_tableMousePos[2] = BALL_POSITIONZOFFSET;
	m_tableMousePos[2] = BALL_POSITIONZOFFSET + BALL_DRAWOFFSETZ;

	///
	m_tableMousePosTmp[0] = g_pCamera->_pos[0] + vec[0] * k;
	m_tableMousePosTmp[1] = g_pCamera->_pos[1] + vec[1] * k;
	m_tableMousePosTmp[2] = BALL_POSITIONZOFFSET + BALL_DRAWOFFSETZ;
	///

	////////////////////////////////////////////////////////
	// check mouse over a ball
	for(int n=0;n<MAX_TOTALBALLS;n++)
	{
	if( m_ball[n]->Proximity(m_tableMousePos[0], m_tableMousePos[1], m_tableMousePos[2]) == TRUE)
	{
	if(!m_ball[n]->m_bMouseOver) // Mouse on
	{
	//m_ball[n]->MouseOn();
	g_ballmouseoncyc = g_cyc;
	// subtract this shit (g_ballmouseoncyc) from g_cyc at the alpha bit
	// i.e. sin((float)g_cya-g_ballmouseoncyc)
	}
	m_ball[n]->m_bMouseOver = TRUE;
	} else m_ball[n]->m_bMouseOver = FALSE;
	}
	////////////////////////////////////////////////////////
	}
	else
	{
	///
	m_tableMousePosTmp[0] = g_pCamera->_pos[0] + vec[0] * k;
	m_tableMousePosTmp[1] = g_pCamera->_pos[1] + vec[1] * k;
	m_tableMousePosTmp[2] = BALL_POSITIONZOFFSET + BALL_DRAWOFFSETZ;
	///
	}
	//return;
	*/


eyecam:;

	

	// strike cue-ball finally!
	if(g_mouseSector == SECTOR_MINI)
	if (m_cueBallSelect == 5) //--
	if (((CGameAppView *)m_parent)->m_bLButtonJustUp)
	if (m_bCueTipReady)
	if (!m_bCueTipReadyFirstTime)
	if (!m_bShotTimeUp) // NEW 29/JAN/04
	{
		//m_bCueTipReady = FALSE;
		x = g_gameAppView->m_mousePoint.x - m_moffsetX;
		y = g_gameAppView->m_mousePoint.y - m_moffsetY;
		w = m_mwindowWidth;
		h = m_mwindowHeight;

		// see if tip is inside mini-camera's box
		if (x >= w);
		else if (x<0);
		else if (y >= h);
		else if (y<0);
		else
		{
			m_bCueTipReady = FALSE;
			m_bCueTipReadyFirstTime = TRUE;

			//m_bCueState = FALSE;
			m_ballMouseOn = 1;
			((CGameAppView *)m_parent)->m_sliderRaiseButt.EnableWindow(FALSE);
			((CGameAppView *)m_parent)->PreviousWndEditSetFocus();

			StrikeBallSend();

			// player decided to play the shot! - so loses opportunity to put opponent back in!
			if (m_bFoulDecideTurn)
			{
				m_bFoulDecideTurn = FALSE;

				//CRect origin;
				//((CGameAppView *)m_parent)->GetWindowRect(&origin);
				//m_foulDlg.SetWindowPos(&((CGameAppView *)m_parent)->wndTop, origin.left, origin.top, 32, 32, SWP_NOACTIVATE | SWP_HIDEWINDOW);
				m_foulDlg.SetWindowPos(&((CGameAppView *)m_parent)->wndTop, 0, 0, 32, 32, SWP_NOACTIVATE | SWP_HIDEWINDOW);
				//m_foulDlg.RedrawWindow(NULL, NULL, RDW_INVALIDATE | RDW_UPDATENOW | RDW_ERASE | RDW_FRAME);
			}

			return;
		}
	}






	// align cue tip on cue-ball
	//--    if(m_cueBallSelect == 0)
	if (g_mouseSector == SECTOR_MINI)
	if (m_cueBallSelect == 5) //--
		//if(((CGameAppView *)m_parent)->m_bLButtonDown)
	{
		//		if( MyTurn() )
		//			m_CameraMode = CAMERA_MODE_WATCH;




		// Use the true Window (Global) mouse coordinates for the Mini Cam Stuff

		((CGameAppView *)m_parent)->m_bLButtonJustUp = 0;

		x = g_gameAppView->m_mousePoint.x - m_moffsetX;
		y = g_gameAppView->m_mousePoint.y - m_moffsetY;
		w = m_mwindowWidth;
		h = m_mwindowHeight;

		m_bCueTipReady = TRUE;

		// see if tip is inside mini-camera's box
		if (x >= w);
		else if (x < 0);
		else if (y >= h);
		else if (y<0);
		else {
			if (!HamperedCueing(x, y, w, h))
			{
				//m_bCueTipReady = TRUE;
				m_bCueTipReadyFirstTime = FALSE; // change state, now that player has moved the mouse into the mini-cam area!

				MoveCueTip(x, y, w, h);
			}
		}
	}
	// ---------------------- //

	//skipcuetipmove:
	pos[0] = m_cuetip[0];
	pos[1] = m_cuetip[1];
	//pos[2] = m_cuetip[2];
	//pos[2] = 1;//m_cuetip[2];
	if (m_bCueTipReady) pos[2] = 1;
	else pos[2] = 0;

	// ------------------------------------------- //

	if (!m_mcameraOn) return; // can't calculate 3d position due to no camera being on.


	pos[0] *= m_mcameraFov;
	pos[1] *= m_mcameraFov;

	pos[0] *= 1 / m_mcameraAspect;




	vec[0] = pos[0] * m_mcameraX[0] + pos[1] * m_mcameraY[0] + pos[2] * m_mcameraZ[0];
	vec[1] = pos[0] * m_mcameraX[1] + pos[1] * m_mcameraY[1] + pos[2] * m_mcameraZ[1];
	vec[2] = pos[0] * m_mcameraX[2] + pos[1] * m_mcameraY[2] + pos[2] * m_mcameraZ[2];

	// Project this vector from m_cameraO till it hits the table's surface Z = 0, that plane.
	// (m_cameraO + vec*k) scalar* (0,0,1) = 0

	// hence: m_cameraO[2] + vec[2] * k = 0;



	if (MyTurn())
	{
		m_ballMouseOn = 1; // pointing at the table

		k = (BALL_RADIUS * 6.5);


		m_ballMousePos[0] = m_mcameraO[0] + vec[0] * k;
		m_ballMousePos[1] = m_mcameraO[1] + vec[1] * k;
		m_ballMousePos[2] = m_mcameraO[2] + vec[2] * k; //	m_tableMousePos[2] = m_cameraO[2] + vec[2] * k; // should equal zero.	


		//if(m_bCueTipReady == FALSE) {
		//	m_ballMousePos[0] = 0;
		//	m_ballMousePos[1] = 0;
		//	m_ballMousePos[2] = 0;
		///}
	}




	return;
nextcam:;

#endif

}





#ifndef THE_POOL_CLUB
BOOL CSnooker::HamperedCueing(float x, float y, float w, float h)
{
	// check for hampered cueing first:
	int n, m;
	CVector S = m_cueTipStrikeDir;

	S.z = 0;

	CVector P = m_ball[m_selectBall]->m_P;
	CVector P2 = m_ball[m_selectBall]->m_P;
	CVector D;
	CVector T;
	float d2, cueoffsetx, cueoffsety, miniballr;
	float r;

	miniballr = (w*0.204f);

	S = S.Unit();
	S.z -= 0.35;//-tan((CUE_DEFAULT_RAISEDBUTT) * DEG2RAD);

	T = S.Cross(CVector(0, 0, 1));
	T = T.Unit();

	// xy plane --------------
	// lock it lotto, rip it!
	r = x - (w / 2);
	if (r < -miniballr)
		r = -miniballr;
	if (r > miniballr)
		r = miniballr;

	cueoffsetx = -((r / miniballr) * BALL_RADIUS);

	P = P + (T * cueoffsetx);
	P2 = P2 + (T * cueoffsetx);
	// -----------------------

	// z plane ---------------
	// lock it lotto, rip it!
	r = y - (w / 2);
	if (r < -miniballr)
		r = -miniballr;
	if (r > miniballr)
		r = miniballr;

	cueoffsety = -((r / miniballr) * BALL_RADIUS);

	P.z += cueoffsety;
	P2.z += cueoffsety;
	// -----------------------

	P.z += m_cuetip[2] * BALL_RADIUS*3.0f;
	P2.z += m_cuetip[2] * BALL_RADIUS*3.0f;

	for (n = 0; n<1200/*500*/; n++) // not sure how many stepes are required? 500 is good for now (1*)
	{
		P = P - (S * BALL_RADIUS * 0.01);
		P2 = P2 - (S * BALL_RADIUS * 0.01);

		//if(n < 500)
		for (m = 0; m<MAX_TOTALBALLS; m++)
		if (TestFlag(m_ball[m]->m_flags, Flg_BALL_ACTIVE))
		if (m_selectBall != m)
		{
			// (1) r we hampered by another ball
			D = P2 - m_ball[m]->m_P;
			d2 = D.Magnitude2();
			//if(d2 < SQR(BALL_RADIUS*1.7)) // (1.5) 1.3
			if (d2 < SQR(BALL_RADIUS*2.0)) // (1.5) 1.3
			{
				// hampered cueing!
				return TRUE;
			}
		}

		// simple cushion boundary checking:
		// x:
		if (P.x>(1500 + 300 - 36 * 2 + BALL_RADIUS + 15) || P.x<(-1500 - 300 + 36 * 2 - BALL_RADIUS - 15)) {
			if (P.z < (BALL_RADIUS * 3.3))
				return TRUE;
		}
		// y:
		if (P.y>(560 + 300 - 36 * 2 + BALL_RADIUS + 40) || P.y<(-560 - 300 + 36 * 2 - BALL_RADIUS - 40)) {
			if (P.z < (BALL_RADIUS * 3.3))
				return TRUE;
		}

	}

	// we r not hampered here
	return FALSE;
}

#else

BOOL CSnooker::HamperedCueing(float x, float y, float w, float h)
{
	// check for hampered cueing first:
	int n, m;
	CVector S = m_cueTipStrikeDir;

	S.z = 0;

	//CVector P = m_ball[m_selectBall]->m_P;
	//CVector P2 = m_ball[m_selectBall]->m_P;

	CVector P, P2;
	P.x = FROMFIX(m_ball[m_selectBall]->f_P.x);
	P.y = FROMFIX(m_ball[m_selectBall]->f_P.y);
	P.z = FROMFIX(m_ball[m_selectBall]->f_P.z);

	P2 = P;



	CVector D;
	CVector T;
	float d2, cueoffsetx, cueoffsety, miniballr;
	float r;

	miniballr = (w*0.204f);

	S = S.Unit();

	S.z -= 0.35;//-tan((CUE_DEFAULT_RAISEDBUTT) * DEG2RAD);

	T = S.Cross(CVector(0, 0, 1));
	T = T.Unit();

	// xy plane --------------
	// lock it lotto, rip it!
	r = x - (w / 2);
	if (r < -miniballr)
		r = -miniballr;
	if (r > miniballr)
		r = miniballr;

	cueoffsetx = -((r / miniballr) * BALL_RADIUS);

	P = P + (T * cueoffsetx);
	P2 = P2 + (T * cueoffsetx);
	// -----------------------

	// z plane ---------------
	// lock it lotto, rip it!
	r = y - (w / 2);
	if (r < -miniballr)
		r = -miniballr;
	if (r > miniballr)
		r = miniballr;

	cueoffsety = -((r / miniballr) * BALL_RADIUS);

	P.z += cueoffsety;
	P2.z += cueoffsety;
	// -----------------------

	P.z += m_cuetip[2] * BALL_RADIUS*3.0f;
	P2.z += m_cuetip[2] * BALL_RADIUS*3.0f;

	for (n = 0; n<1200/*500*/; n++) // not sure how many stepes are required? 500 is good for now (1*)
	{
		P = P - (S * BALL_RADIUS * 0.01);
		P2 = P2 - (S * BALL_RADIUS * 0.01);

		//if(n < 500)
		for (m = 0; m<MAX_TOTALBALLS; m++)
		if (TestFlag(m_ball[m]->m_flags, Flg_BALL_ACTIVE))
		if (m_selectBall != m)
		{
			// (1) r we hampered by another ball
			D = P2 - m_ball[m]->m_P;
			d2 = D.Magnitude2();
			//if(d2 < SQR(BALL_RADIUS*1.7)) // (1.5) 1.3
			if (d2 < SQR(BALL_RADIUS*2.0)) // (1.5) 1.3
			{
				// hampered cueing!
				return TRUE;
			}
		}



		// simple cushion boundary checking:

		// English Pool Game Type //
		if (m_M3d_Table_ptr == &m_customTable->m_M3d_ukTable)
		{
			if (P.x>(231.11 * 5 - 20) || P.x<(-231.11 * 5 + 20)) if (P.z < (BALL_RADIUS * 3.1)) return TRUE; // x:
			if (P.y>(109.67 * 5 - 20) || P.y<(-109.67 * 5 + 20)) if (P.z < (BALL_RADIUS * 3.1)) return TRUE; // y:
		}
		else
		{
			if (P.x>((253 * 5 - BALL_RADIUS*1.25) + BALL_RADIUS + 15) || P.x<((-253 * 5 + BALL_RADIUS*1.25) - BALL_RADIUS - 15)) if (P.z < (BALL_RADIUS * 3.4)) return TRUE; // x:
			if (P.y>((125.5 * 5 - BALL_RADIUS*1.25) + BALL_RADIUS + 40) || P.y<((-125.5 * 5 + BALL_RADIUS*1.25) - BALL_RADIUS - 40)) if (P.z < (BALL_RADIUS * 3.4)) return TRUE; // y:
		}

	}

	// we r not hampered here
	return FALSE;
}
#endif //!THE_POOL_CLUB


void CSnooker::MoveCueTip(float x, float y, float w, float h)
{
	// Only proceed is mouse left button is up
	if (g_gfxDetails.bOptionsMiniCamLockMouse)
	{
		if (m_cueBallSelect == 5)
		{
			if (((CGameAppView *)m_parent)->m_bLButtonDown) return;
		}
	}

	float d1, d2, dx, dy;
	float w2, h2;

	w2 = (w / 2);
	h2 = (h / 2);

	// see if tip is inside mini-camera's box & inside cueball's perimeter
	if (x >= w);
	else if (x<0);
	else if (y >= h);
	else if (y<0);
	else
	{
		dx = x - w2;
		dy = y - h2;
		//dy = y - (h2 - (BALL_DRAWOFFSETZ*(h/140)));
		d1 = (dx*dx) + (dy*dy);
		d2 = SQR(w*0.204f) + SQR(h*0.204f);
		if (d1 < d2)
		{	// if inside circle
			m_cuetip[0] = -(x / w2 - 1); // 1 / aspect ( 0.75 now 0.525f)
			m_cuetip[1] = -(y / h2 - 1);
			//m_cuetip[2] = 1;
		}
		else
		{	// if outside circle (ensures smooth movement)
			CVector R(dx, dy, 0);
			R = R.Unit();
			R = R * (w*(0.204f + 0.09f));
			x = R.x + w2;
			y = R.y + h2;
			//y = R.y + (h2 - (BALL_DRAWOFFSETZ*(h/140)));
			m_cuetip[0] = -(x / w2 - 1); // 1 / aspect ( 0.75 now 0.525f)
			m_cuetip[1] = -(y / h2 - 1);
			//m_cuetip[2] = 1;
		}
	}
}



void CSnooker::CueBallTipSend(float sx, float sy, float sz, float ex, float ey, float ez,
							  BOOL ballMouseOn, M3d_vtx ballMousePos, M3d_vtx cueTip,
							  int nominateBall,
							  int RaiseButt, int RaiseButtOld,
							  float cueAngle)
{
	((CGameAppView *)m_parent)->SendPlayerCueBallTip(sx, sy, sz, ex, ey, ez,
													 ballMouseOn, ballMousePos, cueTip,
													 nominateBall,
													 RaiseButt, RaiseButtOld,
													 cueAngle);
}

void CSnooker::CueBallTipReceiveCompact(void *sdc_ptr)
{
	CueBallTipSendDataC *sdc;
	CueBallTipSendData sd_data;
	CueBallTipSendData *sd;
	int cueTipMash_x,cueTipMash_y;
	float power;

	sdc = (CueBallTipSendDataC *) sdc_ptr;
	sd = &sd_data;

	// Build up the Vector again //
	sd->m_sx = m_ball[sdc->selectBall]->m_P.x;
	sd->m_sy = m_ball[sdc->selectBall]->m_P.y;
	sd->m_sz = m_ball[sdc->selectBall]->m_P.z;
	power = sdc->power*10;
	sd->m_ex = sin(((float)sdc->angle)/256.0f * FF2RAD);
	sd->m_ey = cos(((float)sdc->angle)/256.0f * FF2RAD);
	sd->m_ez = 0;
	sd->m_ex*= power;
	sd->m_ey*= power;
	sd->m_ex+=sd->m_sx;
	sd->m_ey+=sd->m_sy;
	sd->m_ez+=sd->m_sz;
	
	sd->m_ballMouseOn = TRUE;
	sd->m_ballMousePos[0] = 0; // Huh?
	sd->m_ballMousePos[1] = 0; // |
	sd->m_ballMousePos[2] = 0; // |

	// Build CueTip	
	cueTipMash_x = sdc->buffer[3]&0xf;
	cueTipMash_y = sdc->buffer[3]>>4;
	cueTipMash_x-=8;
	cueTipMash_y-=8;
	sd->m_cueTip[0] = ((float)cueTipMash_x)*0.125;
	sd->m_cueTip[1] = ((float)cueTipMash_y)*0.125;
	sd->m_cueTip[2] = 0;

	// Copy Nominate/Raise
	sd->m_nominateBall = sdc->buffer[2]; // nominate
	sd->m_RaiseButt = sdc->raise;
	sd->m_RaiseButtOld = sdc->raise; // Huh?
	//sd->m_cueAngle = (64-((float)sdc->angle))*FF2RAD; // Huh?
	sd->m_cueAngle = ((float)sdc->angle)/256.0f * FF2RAD;

	CueBallTipReceive(sd->m_sx, sd->m_sy, sd->m_sz, sd->m_ex, sd->m_ey, sd->m_ez,
																		 sd->m_ballMouseOn, sd->m_ballMousePos, sd->m_cueTip,
																		 sd->m_nominateBall,
																		 sd->m_RaiseButt, sd->m_RaiseButtOld,
																		 sd->m_cueAngle);	
}

void CSnooker::CueBallTipReceive(float sx, float sy, float sz, float ex, float ey, float ez,
								 BOOL ballMouseOn, M3d_vtx ballMousePos, M3d_vtx cueTip,
								 int nominateBall,
								 int RaiseButt, int RaiseButtOld,
								 float cueAngle)
{
	if( MyTurn() ) return;

	m_sPoint[0] = sx;
	m_sPoint[1] = sy;
	m_sPoint[2] = sz;

	m_tableMousePos[0] = ex;
	m_tableMousePos[1] = ey;
	m_tableMousePos[2] = ez;

	m_tableMousePosStrike[0] = m_tableMousePos[0];
	m_tableMousePosStrike[1] = m_tableMousePos[1];
	m_tableMousePosStrike[2] = m_tableMousePos[2];

	m_cueAngle = cueAngle; // note cueAngle is always +ve (0..2Pi, should be ok), but sender may have a -ve(-pi..pi)

	//char buf[80];
	//sprintf(buf, "Receive: cueAngle = %f", cueAngle);
	//ErrorMessage(m_parent, buf);

	m_ballMouseOn = ballMouseOn; // pointing at the table (don't need to send this data [-])
	m_ballMousePos[0] = ballMousePos[0];
	m_ballMousePos[1] = ballMousePos[1];
	m_ballMousePos[2] = ballMousePos[2];

	m_cuetip[0] = cueTip[0];
	m_cuetip[1] = cueTip[1];
	m_cuetip[2] = cueTip[2];

	m_nominateBall = nominateBall;
	m_RaiseButt = RaiseButt;
	//m_RaiseButtOld = RaiseButtOld;

	if(m_RaiseButtOld != m_RaiseButt)
	{
		m_bReceiveRaiseButtActive = TRUE;
		MiniCameraPlayerRaiseButt();
		m_RaiseButtOld = m_RaiseButt;
	}
}

void CSnooker::CameraPosAtUpFovAspect(M3d_vtx pos, M3d_vtx at, M3d_vtx up, float fov,float aspect)
{
	m_cameraOn = 1;
	m_cameraO[0] = pos[0];
	m_cameraO[1] = pos[1];
	m_cameraO[2] = pos[2];

	m_cameraZ[0] = at[0]-pos[0];
	m_cameraZ[1] = at[1]-pos[1];
	m_cameraZ[2] = at[2]-pos[2];

	m_cameraY[0] = up[0];
	m_cameraY[1] = up[1];
	m_cameraY[2] = up[2];

	m_cameraFov = tan(fov*0.5f); // 180' fov = inf, 90' fov = 1 (45'),  0' fov = 0 (0')
	m_cameraAspect = aspect;
	
	M3d_unit(m_cameraZ); // unit of the long At vector
	M3d_unit(m_cameraY); // unit of the Up vector

	M3d_calcnormal(m_cameraX,m_cameraZ,m_cameraY);
	M3d_unit(m_cameraX); // unit of the new Right vector

	M3d_calcnormal(m_cameraY,m_cameraX,m_cameraZ); // generate new Up Vector (correct right angled one)
}

void CSnooker::MiniCameraPlayerEyePos(M3d_vtx V)
{
	// POS - is at a fixed radius from the centre of the ball
	// get position of mouse cursor in 3d
	CVector C(V[0], V[1], V[2]);

	// get position of ball
	#ifdef FIXEDMODE
	CVector B;
	B.x = FROMFIXSMALL(m_ball[m_selectBall]->f_P.x);
	B.y = FROMFIXSMALL(m_ball[m_selectBall]->f_P.y);
	B.z = FROMFIXSMALL(m_ball[m_selectBall]->f_P.z);
	#else
	CVector B = m_ball[m_selectBall]->m_P;
	#endif
	//B.z += BALL_RADIUS*0.25f; // ??? above centre
	B.z += BALL_DRAWOFFSETZ;

	// calc new direction vector (ball -> camera)
	C = C - B;
	C = C.Unit() * (BALL_RADIUS * 8);
	
	// calc new position of camera
	#ifdef FIXEDMODE
	m_miniCameraPos[0] = FROMFIXSMALL(m_ball[m_selectBall]->f_P.x) + C.x;
	m_miniCameraPos[1] = FROMFIXSMALL(m_ball[m_selectBall]->f_P.y) + C.y;
	m_miniCameraPos[2] = FROMFIXSMALL(m_ball[m_selectBall]->f_P.z) + (BALL_RADIUS * 1.35f) + BALL_DRAWOFFSETZ;
	#else
	m_miniCameraPos[0] = m_ball[m_selectBall]->m_P.x + C.x;
	m_miniCameraPos[1] = m_ball[m_selectBall]->m_P.y + C.y;
	m_miniCameraPos[2] = m_ball[m_selectBall]->m_P.z + (BALL_RADIUS * 1.35f) + BALL_DRAWOFFSETZ;
	#endif
}


#ifdef FIXEDMODE
void CSnooker::MiniCameraPlayerEyePosAngle(float angle, int cueBallSelect)
{	
	if(g_build.gameType == GAMETYPE_POOL)
	{
		
		if(cueBallSelect == 5)
		{
			// POS - is at a fixed radius from the centre of the ball
			CVector C;
			
			// calc new direction vector (ball -> camera)
			// Rotate eye angle in 2d plane by 'angle' radians
			float dx, dy;
			
			dx = (BALL_RADIUS * 8) * cos(angle);
			dy = (BALL_RADIUS * 8) * sin(angle);
			C = CVector(dx, dy, 0);
			
			// calc new position of camera
			m_miniCameraPos[0] = FROMFIXSMALL(m_ball[m_selectBall]->f_P.x) + C.x;
			m_miniCameraPos[1] = FROMFIXSMALL(m_ball[m_selectBall]->f_P.y) + C.y;	
			m_miniCameraPos[2] = FROMFIXSMALL(m_ball[m_selectBall]->f_P.z) + (BALL_RADIUS * 1.35f) + BALL_DRAWOFFSETZ;
		}
		else
		{
			
			

			// POS - is at a fixed radius from the centre of the ball
			CVector C;
			
			// calc new direction vector (ball -> camera)
			// Rotate eye angle in 2d plane by 'angle' radians
			float dx, dy;
			
			dx = 2 * (BALL_RADIUS * 8) * cos(angle);
			dy = 2 * (BALL_RADIUS * 8) * sin(angle);
			C = CVector(dx, dy, 0);
			
			// calc new position of camera
			m_miniCameraPos[0] = FROMFIXSMALL(m_ball[m_selectBall]->f_P.x) + C.x;
			m_miniCameraPos[1] = FROMFIXSMALL(m_ball[m_selectBall]->f_P.y) + C.y;	
			m_miniCameraPos[2] = FROMFIXSMALL(m_ball[m_selectBall]->f_P.z) + (BALL_RADIUS * 1.35f) + BALL_DRAWOFFSETZ + (BALL_RADIUS * 2.0f);
			//m_miniCameraPos[0] = m_testBall->m_P.x;// + C.x;
			//m_miniCameraPos[1] = m_testBall->m_P.y;// + C.y;	
			//m_miniCameraPos[2] = m_testBall->m_P.z + (BALL_RADIUS * 1.35f) + BALL_DRAWOFFSETZ + (BALL_RADIUS * 12.0f);
		}
	}
	else
	{
		// POS - is at a fixed radius from the centre of the ball
		CVector C;
		
		// calc new direction vector (ball -> camera)
		// Rotate eye angle in 2d plane by 'angle' radians
		float dx, dy;
		
		dx = (BALL_RADIUS * 8) * cos(angle);
		dy = (BALL_RADIUS * 8) * sin(angle);
		C = CVector(dx, dy, 0);
		
		// calc new position of camera
		m_miniCameraPos[0] = FROMFIXSMALL(m_ball[m_selectBall]->f_P.x) + C.x;
		m_miniCameraPos[1] = FROMFIXSMALL(m_ball[m_selectBall]->f_P.y) + C.y;	
		m_miniCameraPos[2] = FROMFIXSMALL(m_ball[m_selectBall]->f_P.z) + (BALL_RADIUS * 1.35f) + BALL_DRAWOFFSETZ;
	}
}
#else
void CSnooker::MiniCameraPlayerEyePosAngle(float angle, int cueBallSelect)
{	
	if(g_build.gameType == GAMETYPE_POOL)
	{
		
		if(cueBallSelect == 5)
		{
			// POS - is at a fixed radius from the centre of the ball
			CVector C;
			
			// calc new direction vector (ball -> camera)
			// Rotate eye angle in 2d plane by 'angle' radians
			float dx, dy;
			
			dx = (BALL_RADIUS * 8) * cos(angle);
			dy = (BALL_RADIUS * 8) * sin(angle);
			C = CVector(dx, dy, 0);
			
			// calc new position of camera
			m_miniCameraPos[0] = m_ball[m_selectBall]->m_P.x + C.x;
			m_miniCameraPos[1] = m_ball[m_selectBall]->m_P.y + C.y;	
			m_miniCameraPos[2] = m_ball[m_selectBall]->m_P.z + (BALL_RADIUS * 1.35f) + BALL_DRAWOFFSETZ;
		}
		else
		{
			
			

			// POS - is at a fixed radius from the centre of the ball
			CVector C;
			
			// calc new direction vector (ball -> camera)
			// Rotate eye angle in 2d plane by 'angle' radians
			float dx, dy;
			
			dx = 2 * (BALL_RADIUS * 8) * cos(angle);
			dy = 2 * (BALL_RADIUS * 8) * sin(angle);
			C = CVector(dx, dy, 0);
			
			// calc new position of camera
			m_miniCameraPos[0] = m_ball[m_selectBall]->m_P.x + C.x;
			m_miniCameraPos[1] = m_ball[m_selectBall]->m_P.y + C.y;	
			m_miniCameraPos[2] = m_ball[m_selectBall]->m_P.z + (BALL_RADIUS * 1.35f) + BALL_DRAWOFFSETZ + (BALL_RADIUS * 2.0f);
			//m_miniCameraPos[0] = m_testBall->m_P.x;// + C.x;
			//m_miniCameraPos[1] = m_testBall->m_P.y;// + C.y;	
			//m_miniCameraPos[2] = m_testBall->m_P.z + (BALL_RADIUS * 1.35f) + BALL_DRAWOFFSETZ + (BALL_RADIUS * 12.0f);
		}
	}
	else
	{
		// POS - is at a fixed radius from the centre of the ball
		CVector C;
		
		// calc new direction vector (ball -> camera)
		// Rotate eye angle in 2d plane by 'angle' radians
		float dx, dy;
		
		dx = (BALL_RADIUS * 8) * cos(angle);
		dy = (BALL_RADIUS * 8) * sin(angle);
		C = CVector(dx, dy, 0);
		
		// calc new position of camera
		m_miniCameraPos[0] = m_ball[m_selectBall]->m_P.x + C.x;
		m_miniCameraPos[1] = m_ball[m_selectBall]->m_P.y + C.y;	
		m_miniCameraPos[2] = m_ball[m_selectBall]->m_P.z + (BALL_RADIUS * 1.35f) + BALL_DRAWOFFSETZ;
	}
}
#endif

void CSnooker::MiniCameraPlayerRaiseButt(void)
{
	//float angleDelta = (float)(m_RaiseButt - m_RaiseButtOld) / 90.0f;
	//m_miniCameraPos[2] += angleDelta * (BALL_RADIUS*2.0f);
	//m_cuetip[2] = (float)m_RaiseButt / 90.0f;

	// get unit vector in dir. eyepos -> selected ball
	#ifdef FIXEDMODE
	CVector TempVec;
	TempVec.x = FROMFIXSMALL(m_ball[m_selectBall]->f_P.x);
	TempVec.y = FROMFIXSMALL(m_ball[m_selectBall]->f_P.y);
	TempVec.z = FROMFIXSMALL(m_ball[m_selectBall]->f_P.z);
	
	CVector UToBall = TempVec - CVector(m_miniCameraPos[0], m_miniCameraPos[1], m_miniCameraPos[2]);
	#else
	CVector UToBall = m_ball[m_selectBall]->m_P - CVector(m_miniCameraPos[0], m_miniCameraPos[1], m_miniCameraPos[2]);
	#endif
	UToBall.z = 0;
	UToBall = UToBall.Unit();	

	// rotate eyepos about 'selected ballpos' 0->90 degress
	float fRaiseButtNew = (float)m_RaiseButt / 90.0f;
	float fRaiseButtOld = (float)m_RaiseButtOld / 90.0f;
	float znew = sin(fRaiseButtNew*1.5f);
	float zold = sin(fRaiseButtOld*1.5f);
	float xynew = cos(fRaiseButtNew*1.5f);
	float xyold = cos(fRaiseButtOld*1.5f);

	// get position change
	float z = znew - zold;
	float xy = -(xynew - xyold);
	
	// update mini 3d cam view (move pos):
	float pivotRadius = BALL_RADIUS * 7.0f;
	//float pivotRadius = (BALL_RADIUS * (7.0f - fRaiseButtNew*0.8f)); //(BALL_DRAWOFFSETZ*(h/140)));
	m_miniCameraPos[0] += UToBall.x * xy * pivotRadius;
	m_miniCameraPos[1] += UToBall.y * xy * pivotRadius;
	m_miniCameraPos[2] += z * pivotRadius;

	//m_miniCameraPos[2] += BALL_DRAWOFFSETZ;
	
	// update cuetip pos
	m_bCueTipReady = FALSE;
	m_bCueTipReadyFirstTime = TRUE;
	m_cuetip[0] = 0;
	m_cuetip[1] = 0;
	m_cuetip[2] = (float)m_RaiseButt / 90.0f; // save raisebutt info (0.0f - 1.0f)
	m_ballMousePos[0] = 0;
	m_ballMousePos[1] = 0;
	m_ballMousePos[2] = -20;
}

void CSnooker::MiniCameraPosAtUpFovAspect(M3d_vtx pos, M3d_vtx at, M3d_vtx up, float fov,float aspect)
{
	m_mcameraOn = 1;
	m_mcameraO[0] = pos[0];
	m_mcameraO[1] = pos[1];
	m_mcameraO[2] = pos[2];

	m_mcameraZ[0] = at[0]-pos[0];
	m_mcameraZ[1] = at[1]-pos[1];
	m_mcameraZ[2] = at[2]-pos[2];

	m_mcameraY[0] = up[0];
	m_mcameraY[1] = up[1];
	m_mcameraY[2] = up[2];

	m_mcameraFov = tan(fov*0.5f); // 180' fov = inf, 90' fov = 1 (45'),  0' fov = 0 (0')
	m_mcameraAspect = aspect;
	
	M3d_unit(m_mcameraZ); // unit of the long At vector
	M3d_unit(m_mcameraY); // unit of the Up vector

	M3d_calcnormal(m_mcameraX,m_mcameraZ,m_mcameraY);
	M3d_unit(m_mcameraX); // unit of the new Right vector

	M3d_calcnormal(m_mcameraY,m_mcameraX,m_mcameraZ); // generate new Up Vector (correct right angled one)
}

void CSnooker::SetMiniOffsetWindow(int offx,int offy, int w, int h)
{			   
	m_moffsetX = offx;
	m_moffsetY = offy;
	m_mwindowWidth = w;
	m_mwindowHeight = h;
}

#ifdef FIXEDMODE
void CSnooker::MiniCameraViewUpdate(void)
{
	// only happens at startup, before a ball is selected (*)
	if(m_miniCameraStartUpView == TRUE)
	{
		m_miniCameraStartUpView = FALSE;

		// AT
		m_miniCameraAt[0] = FROMFIXSMALL(m_ball[m_dominoHitLast]->f_P.x);
		m_miniCameraAt[1] = FROMFIXSMALL(m_ball[m_dominoHitLast]->f_P.y);
		m_miniCameraAt[2] = FROMFIXSMALL(m_ball[m_dominoHitLast]->f_P.z);

		// POS - initial break-off 'eye' position
		M3d_vtx V;

		if(g_build.gameType == GAMETYPE_SNOOKER)
		{
			V[0] = -1550;
			V[1] = 160;
			//V[2] = BALL_RADIUS*2.8f;//0;//BALL_POSITIONZOFFSET - BALL_RADIUS * 1;
			V[2] = BALL_POSITIONZOFFSET + BALL_DRAWOFFSETZ;
		}
		else if(g_build.gameType == GAMETYPE_POOL)
		{
			V[0] = -1550;
			V[1] = 0;
			V[2] = BALL_POSITIONZOFFSET + BALL_DRAWOFFSETZ;
		}
		
		MiniCameraPlayerEyePos(V);
	}

	// update mini 3d view camera pos
	if(m_cueBallSelect == 0 || m_bReceiveRaiseButtActive)
	{
		m_miniCameraAt[0] = FROMFIXSMALL(m_ball[m_selectBall]->f_P.x);
		m_miniCameraAt[1] = FROMFIXSMALL(m_ball[m_selectBall]->f_P.y);
		m_miniCameraAt[2] = FROMFIXSMALL(m_ball[m_selectBall]->f_P.z) + BALL_DRAWOFFSETZ;

		// if WHITE ball, leave camera POS where it is, otherwise...
		if(m_ball[m_selectBall]->m_colour != WHITE)
		{
		}
	}
	else if(m_cueBallSelect == 1 || m_cueBallSelect == 5)
	{
		m_miniCameraStartUpView = FALSE; // now, clear startupview flag (see (*) above)

		// AT  - should now be the centre of the ball
		{
			m_miniCameraAt[0] = FROMFIXSMALL(m_ball[m_selectBall]->f_P.x);
			m_miniCameraAt[1] = FROMFIXSMALL(m_ball[m_selectBall]->f_P.y);
			m_miniCameraAt[2] = FROMFIXSMALL(m_ball[m_selectBall]->f_P.z) + BALL_DRAWOFFSETZ;		
		}
		
		
		if(m_ball[m_selectBall]->m_colour == WHITE )
		{
			// move with eye (calc new position of camera) 
			MiniCameraPlayerEyePosAngle(m_cueAngle, m_cueBallSelect);
		}
		else {
			
			// move with eye (calc new position of camera) 
			MiniCameraPlayerEyePosAngle(m_cueAngle, m_cueBallSelect);
		}
	}
	else if (m_cueBallSelect == 2)
	{
		// while sim. is running...
		if(m_ball[m_selectBall]->m_colour == WHITE)
		{
			m_miniCameraAt[0] = FROMFIXSMALL(m_ball[m_dominoHitLast]->f_P.x);
			m_miniCameraAt[1] = FROMFIXSMALL(m_ball[m_dominoHitLast]->f_P.y);
			m_miniCameraAt[2] = FROMFIXSMALL(m_ball[m_dominoHitLast]->f_P.z) + BALL_DRAWOFFSETZ;
		}
		else
		{
			m_miniCameraAt[0] = FROMFIXSMALL(m_ball[m_dominoHitLast]->f_P.x);
			m_miniCameraAt[1] = FROMFIXSMALL(m_ball[m_dominoHitLast]->f_P.y);
			m_miniCameraAt[2] = FROMFIXSMALL(m_ball[m_dominoHitLast]->f_P.z) + BALL_DRAWOFFSETZ;
		}
	}
	else if (m_cueBallSelect == 3)
	{
		// after sim, look at white ball
		m_cueBallSelect = 0;
		m_bCuePowerJustOn = FALSE;
		m_bSimJustFinished = GAMEAPP_FPS;

		m_miniCameraAt[0] = FROMFIXSMALL(m_ball[0]->f_P.x);
		m_miniCameraAt[1] = FROMFIXSMALL(m_ball[0]->f_P.y);
		m_miniCameraAt[2] = FROMFIXSMALL(m_ball[0]->f_P.z) + BALL_DRAWOFFSETZ;

		// POS - similar to initial break-off 'eye' position
		M3d_vtx V;
		
		if(g_build.gameType == GAMETYPE_SNOOKER)
		{	
			V[0] = -1550;
			V[1] = 160;
			//V[2] = 0;
			V[2] = BALL_POSITIONZOFFSET + BALL_DRAWOFFSETZ;
		}
		else if(g_build.gameType == GAMETYPE_POOL)
		{
			V[0] = -1550;
			V[1] = 0;
			V[2] = BALL_POSITIONZOFFSET + BALL_DRAWOFFSETZ;
		}

		MiniCameraPlayerEyePos(V);
	}
}
#else
void CSnooker::MiniCameraViewUpdate(void)
{
	//char buff[80];
	//sprintf(buff, "CBS = %d, m_dominoHitLast = %d", m_cueBallSelect, m_dominoHitLast);
	//ErrorMessage(m_parent, buff);

	// only happens at startup, before a ball is selected (*)
	if(m_miniCameraStartUpView == TRUE)
	{
		m_miniCameraStartUpView = FALSE;

		// AT
		m_miniCameraAt[0] = m_ball[m_dominoHitLast]->m_P.x;
		m_miniCameraAt[1] = m_ball[m_dominoHitLast]->m_P.y;
		m_miniCameraAt[2] = m_ball[m_dominoHitLast]->m_P.z;

		// POS - initial break-off 'eye' position
		M3d_vtx V;

		if(g_build.gameType == GAMETYPE_SNOOKER)
		{
			V[0] = -1550;
			V[1] = 160;
			//V[2] = BALL_RADIUS*2.8f;//0;//BALL_POSITIONZOFFSET - BALL_RADIUS * 1;
			V[2] = BALL_POSITIONZOFFSET + BALL_DRAWOFFSETZ;
		}
		else if(g_build.gameType == GAMETYPE_POOL)
		{
			V[0] = -1550;
			V[1] = 0;
			V[2] = BALL_POSITIONZOFFSET + BALL_DRAWOFFSETZ;
		}
		
		MiniCameraPlayerEyePos(V);
	}

	// update mini 3d view camera pos
	if(m_cueBallSelect == 0 || (m_bReceiveRaiseButtActive && m_cueBallSelect != 2))
	{
		//m_miniCameraAt[0] = m_ball[m_dominoHitLast]->m_P.x;
		//m_miniCameraAt[1] = m_ball[m_dominoHitLast]->m_P.y;
		//m_miniCameraAt[2] = m_ball[m_dominoHitLast]->m_P.z + BALL_DRAWOFFSETZ;
		m_miniCameraAt[0] = m_ball[m_selectBall]->m_P.x;
		m_miniCameraAt[1] = m_ball[m_selectBall]->m_P.y;
		m_miniCameraAt[2] = m_ball[m_selectBall]->m_P.z + BALL_DRAWOFFSETZ;

		// if WHITE ball, leave camera POS where it is, otherwise...
		if(m_ball[m_selectBall]->m_colour != WHITE)
		{
			/*
			m_miniCameraPos[0] = m_ball[m_dominoHitLast]->m_P.x;
			m_miniCameraPos[1] = m_ball[m_dominoHitLast]->m_P.y;
			m_miniCameraPos[2] = m_ball[m_dominoHitLast]->m_P.z + 400;
			*/
		}
	}
	else if(m_cueBallSelect == 1 || m_cueBallSelect == 5)
	{
		m_miniCameraStartUpView = FALSE; // now, clear startupview flag (see (*) above)

		// AT  - should now be the centre of the ball
		//if(m_cueBallSelect == 5)
		{
			m_miniCameraAt[0] = m_ball[m_selectBall]->m_P.x;
			m_miniCameraAt[1] = m_ball[m_selectBall]->m_P.y;
			m_miniCameraAt[2] = m_ball[m_selectBall]->m_P.z + BALL_DRAWOFFSETZ;		
		}
		//else
		//{
		//	m_miniCameraAt[0] = m_testBall->m_P.x;
		//	m_miniCameraAt[1] = m_testBall->m_P.y;
		//	m_miniCameraAt[2] = m_testBall->m_P.z + BALL_DRAWOFFSETZ;		
		//}
		
		
		if(m_ball[m_selectBall]->m_colour == WHITE )
		{
			// move with eye (calc new position of camera) 
//			MiniCameraPlayerEyePos(m_tableMousePos);			
			MiniCameraPlayerEyePosAngle(m_cueAngle, m_cueBallSelect);
		}
		else {
			/*
			// move with ball (calc new position of camera)
			m_miniCameraPos[0] = m_ball[m_selectBall]->m_P.x;
			m_miniCameraPos[1] = m_ball[m_selectBall]->m_P.y;
			m_miniCameraPos[2] = m_ball[m_selectBall]->m_P.z + 400;
			*/
			
			// move with eye (calc new position of camera) 
//			MiniCameraPlayerEyePos(m_tableMousePos);
			MiniCameraPlayerEyePosAngle(m_cueAngle, m_cueBallSelect);
		}
	}
	else if (m_cueBallSelect == 2)
	{
		// while sim. is running...
		
		if(TestFlag(m_ball[m_dominoHitLast]->m_flags, Flg_BALL_ACTIVE)) //Flg_BALL_POTTED
		{
		

			if(m_ball[m_selectBall]->m_colour == WHITE)
			{
				m_miniCameraAt[0] = m_ball[m_dominoHitLast]->m_P.x;
				m_miniCameraAt[1] = m_ball[m_dominoHitLast]->m_P.y;
				m_miniCameraAt[2] = m_ball[m_dominoHitLast]->m_P.z + BALL_DRAWOFFSETZ;
			}
			else
			{
				/*
				m_miniCameraPos[0] = m_ball[m_dominoHitLast]->m_P.x;
				m_miniCameraPos[1] = m_ball[m_dominoHitLast]->m_P.y;
				m_miniCameraPos[2] = m_ball[m_dominoHitLast]->m_P.z + 400;
			
				m_miniCameraAt[0] = m_ball[m_dominoHitLast]->m_P.x;
				m_miniCameraAt[1] = m_ball[m_dominoHitLast]->m_P.y;
				m_miniCameraAt[2] = m_ball[m_dominoHitLast]->m_P.z;
				*/

				m_miniCameraAt[0] = m_ball[m_dominoHitLast]->m_P.x;
				m_miniCameraAt[1] = m_ball[m_dominoHitLast]->m_P.y;
				m_miniCameraAt[2] = m_ball[m_dominoHitLast]->m_P.z + BALL_DRAWOFFSETZ;
			}
		}
	}
	else if (m_cueBallSelect == 3)
	{
		// after sim, look at white ball

		// If the turn just changed, Update Main Camera View if we were in one of the black ball area views //
		if(!MyTurn())
		if(m_3dtoggle_old > 5)
		if(PROCESS_CAMERA_TV(1));  // Force Camera to be one of the Full Table views //
			{
			g_gameAppView->m_bOnSize = TRUE;
			g_gameAppView->m_bOnSizeDrawTmpSurf = TRUE;
			}

		m_cueBallSelect = 0;
		m_bCuePowerJustOn = FALSE;
		m_bSimJustFinished = GAMEAPP_FPS;

		m_miniCameraAt[0] = m_ball[0]->m_P.x;
		m_miniCameraAt[1] = m_ball[0]->m_P.y;
		m_miniCameraAt[2] = m_ball[0]->m_P.z + BALL_DRAWOFFSETZ;

		// POS - similar to initial break-off 'eye' position
		M3d_vtx V;
		
		if(g_build.gameType == GAMETYPE_SNOOKER)
		{	
			V[0] = -1550;
			V[1] = 160;
			//V[2] = 0;
			V[2] = BALL_POSITIONZOFFSET + BALL_DRAWOFFSETZ;
		}
		else if(g_build.gameType == GAMETYPE_POOL)
		{
			V[0] = -1550;
			V[1] = 0;
			V[2] = BALL_POSITIONZOFFSET + BALL_DRAWOFFSETZ;
		}

		MiniCameraPlayerEyePos(V);
	}
}
#endif

///////////////////////////////////////////////////////////////////////////////////////////////////
//
//
// check if it's this player's go
BOOL CSnooker::MyTurn(void)
{
	//if(m_seat == -1) return FALSE;
	if(m_bPracticeMode || GAMEAPPVIEW->m_lobby->GameStarted(GAMEAPPVIEW->m_playerSockIdx))
	{
		if((m_seat == 0) && m_bTurn1P) return TRUE;
		if((m_seat == 1) && !m_bTurn1P) return TRUE;

		if(GAMEAPPVIEW->m_bReplayMode || GAMEAPPVIEW->m_bPlayOffline) return TRUE;

		#ifdef THE_POOL_CLUB
		if(m_bPracticeMode && m_customTable->m_type == CTTYPE_BOWLS) return TRUE;
		#endif //THE_POOL_CLUB
	}

	return FALSE;
}


///////////////////////////////////////////////////////////////////////////////////////////////////
//
//
// check if this player is seated at table
BOOL CSnooker::IsSitting(void)
{
	if(m_seat == -1) return FALSE;
	
	return TRUE;
}


///////////////////////////////////////////////////////////////////////////////////////////////////
//
//
// enforce rules & keep scores
void CSnooker::Referee(void)
{
		int n;
		int colour;
		int no_colours;
		BOOL bWhiteInPocket = FALSE;
		BOOL bNextPlayer = FALSE;
		BOOL bHitTouchingBall = FALSE;

		m_bMiss = TRUE;												// assume no balls potted
		ClearFlag(m_foul, Flg_FOUL_NORMAL | Flg_FOUL_WHITEPOTTED);	// assume no foul to start with [keep some flags from previous shot]
		ClearFlag(m_foul, Flg_FOUL_FREEBALLSNOOKER); // *NEW*
		ClearFlag(m_foul, Flg_FOUL_MISS);
		ClearFlag(m_foul, Flg_FOUL_PUSH);
		m_bRefreshScores = TRUE;
		
		// just in case :)
		if(m_colourseq)
			m_rednext = FALSE;

		// check if player has hit a touching ball?
		for(n=0;n<MAX_TOUCHING_BALLS;n++)
		if(!m_ball[0]->m_bHitCushionBeforeBall)
		if(m_oldTouchingBall[n])
		//if(m_ball[m_dominoHit1st]->m_colour == m_ball[m_oldTouchingBall[n]]->m_colour)
		if(m_dominoHit1st == m_oldTouchingBall[n]) // check for actual ball being hit first (^ BUG above)
		{
			bHitTouchingBall = TRUE;
		}
		//

		//////////////////////////////////////////////////////////////////////////////////
		// if before this shot was played, there was a touching ball situation, work out what ball player intended to play:
		if(!bHitTouchingBall)
		{
			BOOL bWasTouchingARedBall = FALSE;
			BOOL bWasTouchingAColourBall = FALSE;
			for(n=0;n<MAX_TOUCHING_BALLS;n++)
			if(m_oldTouchingBall[n])
			{
				if(m_ball[m_oldTouchingBall[n]]->m_colour == RED)
				{
					bWasTouchingARedBall = TRUE;
				}
				else if((m_ball[m_oldTouchingBall[n]]->m_colour >= YELLOW) && (m_ball[m_oldTouchingBall[n]]->m_colour <= BLACK))
				{
					bWasTouchingAColourBall = TRUE;
				}
			}

			if(m_rednext) // was we just on a RED
			{
				if(bWasTouchingARedBall)
				{
					for(n=0;n<MAX_TOUCHING_BALLS;n++)
					if(m_oldTouchingBall[n])
					if(m_ball[m_oldTouchingBall[n]]->m_colour == RED)
					{
						m_dominoHit1st = m_oldTouchingBall[n]; // let's say that player played the touching ball as a red
						break;
					}
				}
			}
			else // we were just on a colour
			{
				if(bWasTouchingAColourBall)
				{
					for(n=0;n<MAX_TOUCHING_BALLS;n++)
					if(m_oldTouchingBall[n])
					if((m_ball[m_oldTouchingBall[n]]->m_colour >= YELLOW) && (m_ball[m_oldTouchingBall[n]]->m_colour <= BLACK))
					{
						// now check that player nominated this colour ball, or we were already on the colours at the end of the frame
						if((m_ball[m_nominateBall]->m_colour == m_ball[m_oldTouchingBall[n]]->m_colour) ||
						   (m_colourseq == m_ball[m_oldTouchingBall[n]]->m_colour))
						{
							m_dominoHit1st = m_oldTouchingBall[n]; // let's say that player played the touching ball as a colour
							break;
						}
					}
				}
			}
		}
		///////////////////////////////////////////////////////////////////////////


		// [freeball mode]
		if(TestFlag(m_foul, Flg_FOUL_FREEBALL))
		{
			//MessageBox(0, "0","0",MB_OK);
			FreeBallReferee();
			ClearFlag(m_foul, Flg_FOUL_FREEBALL);
			goto end;
		}

		// [default mode]

		/////////////////////////////////////////////////////////////
		// see if white ball has been potted (foul)
		//MessageBox(0, "3","3",MB_OK);
		for(n=0;n<MAX_TOTALBALLS;n++)
		{
			if(TestFlag(m_ball[n]->m_flags, Flg_BALL_POTTED))
				if(m_ball[n]->m_colour == WHITE)
				{
					bWhiteInPocket = TRUE;
					SetFlag(m_foul, Flg_FOUL_NORMAL | Flg_FOUL_WHITEPOTTED);
					//if(TestFlag(m_foul, Flg_FOUL_WHITEPOTTED))
					//	MessageBox(0, "---","---",MB_OK);
				}
		}
		/////////////////////////////////////////////////////////////

		/////////////////////////////////////////////////////////////
		// what happens if player strikes a ball other than the white
		if(m_selectBall != WHITE)
		{
			//MessageBox(0, "1","1",MB_OK);
			colour = m_ball[m_selectBall]->m_colour;

			// foul! Give 4 pts to oppenent
			if(colour <= BROWN)
				if(m_bTurn1P) m_score2P += 4;
					else m_score1P += 4;

			// foul! Give 5,6 or 7 pts to oppenent
			if(colour >= BLUE)
				if(m_bTurn1P) m_score2P += colour;
					else m_score1P += colour;

			SetFlag(m_foul, Flg_FOUL_NORMAL);

			goto end;
		}
		/////////////////////////////////////////////////////////////

		//
		
		/////////////////////////////////////////////////////////////
		// if white doesn't hit a ball at all, foul ball on!
		if(m_colourseq < YELLOW) // bypass if on colours at the end
		if((m_selectBall == WHITE) && (m_dominoHit1st == WHITE))
		{
			//MessageBox(0, "2","2",MB_OK);
			// find ball-on
			colour = m_ball[m_nominateBall]->m_colour;
			if(colour >= YELLOW)
			{
				// foul! Give 4 pts to oppenent
				if(colour <= BROWN)
					if(m_bTurn1P) m_score2P += 4;
					else m_score1P += 4;

				// foul! Give 5,6 or 7 pts to oppenent
				if(colour >= BLUE)
					if(m_bTurn1P) m_score2P += colour;
					else m_score1P += colour;

				SetFlag(m_foul, Flg_FOUL_NORMAL);
				goto end;
			}
			else  {
				if(m_bTurn1P) m_score2P += 4;
					else m_score1P += 4;

				SetFlag(m_foul, Flg_FOUL_NORMAL);
				goto end;
			}
		}
		/////////////////////////////////////////////////////////////


		/////////////////////////////////////////////////////////////
		// if white ball has been potted, did any other colour balls go in also?
		if(bWhiteInPocket)
		{
			//MessageBox(0, "4","4",MB_OK);
			colour = RED;
			for(n=0;n<MAX_TOTALBALLS;n++)
				if(TestFlag(m_ball[n]->m_flags, Flg_BALL_POTTED))
					if((m_ball[n]->m_colour <= BLACK) && (m_ball[n]->m_colour >= YELLOW))
						if(colour < m_ball[n]->m_colour)
							colour = m_ball[n]->m_colour;

			
			//// potted colour!
			//if(colour >= YELLOW)
			if(m_ball[m_dominoHit1st]->m_colour <= colour) // if fail this test, then do (**) code below or (***)
			{
				// foul! Give 4 pts to oppenent
				if(colour <= BROWN)
					if(m_bTurn1P) m_score2P += 4;
					else m_score1P += 4;

				// foul! Give 5,6 or 7 pts to oppenent
				if(colour >= BLUE)
					if(m_bTurn1P) m_score2P += colour;
					else m_score1P += colour;

				SetFlag(m_foul, Flg_FOUL_NORMAL);
				goto end;
			}
		}
		/////////////////////////////////////////////////////////////

		/////////////////////////////////////////////////////////////
		// check if player hit a touching ball first
		if(bHitTouchingBall)
		for(n=0;n<MAX_TOUCHING_BALLS;n++)
		if(!m_ball[0]->m_bHitCushionBeforeBall)
		if(m_oldTouchingBall[n])
		if(m_ball[m_dominoHit1st]->m_colour == m_ball[m_oldTouchingBall[n]]->m_colour)
		{
			// foul! - cannot hit directly a touching ball
			colour = m_ball[m_dominoHit1st]->m_colour;

			// check if a higher colour was potted
			for(n=0;n<MAX_TOTALBALLS;n++)
				if(TestFlag(m_ball[n]->m_flags, Flg_BALL_POTTED))
					if((m_ball[n]->m_colour <= BLACK) && (m_ball[n]->m_colour >= YELLOW))
						if(colour < m_ball[n]->m_colour)
							colour = m_ball[n]->m_colour;

			// foul! Give 4 pts to oppenent
			if(colour <= BROWN)
				if(m_bTurn1P) m_score2P += 4;
				else m_score1P += 4;

			// foul! Give 5,6 or 7 pts to oppenent
			if(colour >= BLUE)
				if(m_bTurn1P) m_score2P += colour;
				else m_score1P += colour;
		
			//////////////////////////////////
			char _buffer[MAX_CHARS];
			char sColour[MAX_CHARS];

			switch(colour)
			{
				case RED:
					sprintf(sColour, "a RED");
					break;
				case YELLOW:
					sprintf(sColour, "the YELLOW");
					break;
				case GREEN:
					sprintf(sColour, "the GREEN");
					break;
				case BROWN:
					sprintf(sColour, "the BROWN");
					break;
				case BLUE:
					sprintf(sColour, "the BLUE");
					break;
				case PINK:
					sprintf(sColour, "the PINK");
					break;
				case BLACK:
					sprintf(sColour, "the BLACK");
					break;
				default:
					sprintf(sColour, "[Error!]");
					break;
			}

			/*
			if(m_bPracticeMode)
			{
				if(m_bTurn1P)
					sprintf(_buffer,"(??? played a 'Push Shot' on %s ball)", sColour);
				else
					sprintf(_buffer,"(??? played a 'Push Shot' on %s ball)", sColour);
				GAMEAPPVIEW->Message(_buffer,10); // system message
			}
			else
			*/
			{
				int tbl = GAMEAPPVIEW->m_lobby->GetTableIdxFromPlayer(GAMEAPPVIEW->m_playerSockIdx);
				if(tbl!=-1)
				{
					int plr1Idx = GAMEAPPVIEW->m_lobby->GetPlayerIdx(m_table[tbl].player1SockIdx);
					if(plr1Idx == -1) goto endsystemmsg;
					int plr2Idx = GAMEAPPVIEW->m_lobby->GetPlayerIdx(m_table[tbl].player2SockIdx);
					if(plr2Idx == -1) goto endsystemmsg;

					if(m_bTurn1P)
						sprintf(_buffer,"(%s played a 'Push Shot' on %s ball)", m_player[plr1Idx].name, sColour);
					else
						sprintf(_buffer,"(%s played a 'Push Shot' on %s ball)", m_player[plr2Idx].name, sColour);
					GAMEAPPVIEW->Message(_buffer,10); // system message
				}
			}
endsystemmsg:;
			//////////////////////////////////

			SetFlag(m_foul, Flg_FOUL_NORMAL | Flg_FOUL_PUSH);
			goto end;
		}
		/////////////////////////////////////////////////////////////


		/////////////////////////////////////////////////////////////
		// check if more than 1 colour ball has been potted
		if(m_wildRackType != 8) // if 'ALL_BLACK' wild type, we are allowed to pot more than 1 black simul.
		if(!bWhiteInPocket)
		{
			//MessageBox(0, "5","5",MB_OK);
			no_colours = 0;
			colour = 0;
			
			for(n=0;n<MAX_TOTALBALLS;n++)
				if(TestFlag(m_ball[n]->m_flags, Flg_BALL_POTTED))
					if((m_ball[n]->m_colour <= BLACK) && (m_ball[n]->m_colour >= YELLOW)) {
						no_colours++;		
						if(colour < m_ball[n]->m_colour)
							colour = m_ball[n]->m_colour;
					}

			if(no_colours >= 2)
			{
				// foul! Give 4 pts to oppenent
				if(colour <= BROWN)
					if(m_bTurn1P) m_score2P += 4;
					else m_score1P += 4;

				// foul! Give 5,6 or 7 pts to oppenent
				if(colour >= BLUE)
					if(m_bTurn1P) m_score2P += colour;
					else m_score1P += colour;
			
				SetFlag(m_foul, Flg_FOUL_NORMAL);
				goto end;
			}		
		}
		/////////////////////////////////////////////////////////////

		
		/////////////////////////////////////////////////////////////
		// check if 'red' is next but a 'colour' is potted OR vice-versa
		if(!bWhiteInPocket)
		{
			if(m_rednext)
			{
				//MessageBox(0, "6","6",MB_OK);
				colour = 0;			
				for(n=0;n<MAX_TOTALBALLS;n++)
					if(TestFlag(m_ball[n]->m_flags, Flg_BALL_POTTED))
						if((m_ball[n]->m_colour <= BLACK) && (m_ball[n]->m_colour >= YELLOW))
							if(colour < m_ball[n]->m_colour)
								colour = m_ball[n]->m_colour;
				
				// potted colour!
				if(colour >= YELLOW)
				{
					// foul! Give 4 pts to oppenent
					if(colour <= BROWN)
						if(m_bTurn1P) m_score2P += 4;
						else m_score1P += 4;

					// foul! Give 5,6 or 7 pts to oppenent
					if(colour >= BLUE)
						if(m_bTurn1P) m_score2P += colour;
						else m_score1P += colour;

					SetFlag(m_foul, Flg_FOUL_NORMAL);
					goto end;
				}
			}
			else
			{
				//MessageBox(0, "7","7",MB_OK);
				colour = 0;			
				for(n=0;n<MAX_TOTALBALLS;n++)
					if(TestFlag(m_ball[n]->m_flags, Flg_BALL_POTTED))
						if((m_ball[n]->m_colour == RED))
							colour++;

				// if a red has been potted
				if(colour >= 1) {
					// find out what colour was nominated, a give these points to opponenet
					//colour = BROWN;	// test!!!
					//if(m_selectBall != WHITE)
					//if(m_dominoHit1st == 0)
					colour = m_ball[m_dominoHit1st]->m_colour;

					// foul! Give 4 pts to oppenent
					if(colour <= BROWN)
						if(m_bTurn1P) m_score2P += 4;
						else m_score1P += 4;

					// foul! Give 5,6 or 7 pts to oppenent
					if(colour >= BLUE)
						if(m_bTurn1P) m_score2P += colour;
						else m_score1P += colour;

					SetFlag(m_foul, Flg_FOUL_NORMAL);
					goto end;
				}
			}
		}
		/////////////////////////////////////////////////////////////


		/////////////////////////////////////////////////////////////
		// colour seq. at end of frame - check if they r taken in the correct order
		/////////////////////////////////////////////////////////////
		if(m_colourseq >= YELLOW)
		{
			colour = 0;
			//MessageBox(0, "8","8",MB_OK);
			// look for highest value colour that was potted
			for(n=0;n<MAX_TOTALBALLS;n++)
			{
				if(TestFlag(m_ball[n]->m_flags, Flg_BALL_POTTED))
				{
					// higher or lower?
					if(colour < m_ball[n]->m_colour)
					colour = m_ball[n]->m_colour;
				}
			}
			
			// did player hit the wrong colour ball?
			if(m_ball[m_dominoHit1st]->m_colour != m_colourseq)
			{
				// higher or lower?
				if(colour < m_ball[m_dominoHit1st]->m_colour)
					colour = m_ball[m_dominoHit1st]->m_colour;

				// higher or lower?
				if(colour <  m_colourseq)
					colour =  m_colourseq;

				// foul! Give 4 pts to oppenent
				if(colour <= BROWN)
					if(m_bTurn1P) m_score2P += 4;
					else m_score1P += 4;

				// foul! Give 5,6 or 7 pts to oppenent
				if(colour >= BLUE)
					if(m_bTurn1P) m_score2P += colour;
					else m_score1P += colour;

				SetFlag(m_foul, Flg_FOUL_NORMAL);
				goto end;
			}
		}
		/////////////////////////////////////////////////////////////


		/////////////////////////////////////////////////////////////
		// check if 'red' is next but a 'colour' is hit OR vice-versa (**)
		//if(!bWhiteInPocket)
		//{
			if(m_rednext)
			{			
				//MessageBox(0, "9","9",MB_OK);
				// get colour of ball that was hit first
				colour = m_ball[m_dominoHit1st]->m_colour;
				if(colour != RED)
				{	
					// potted colour!
					if(colour >= YELLOW)
					{
						// foul! Give 4 pts to oppenent
						if(colour <= BROWN)
							if(m_bTurn1P) m_score2P += 4;
							else m_score1P += 4;

						// foul! Give 5,6 or 7 pts to oppenent
						if(colour >= BLUE)
							if(m_bTurn1P) m_score2P += colour;
							else m_score1P += colour;

						SetFlag(m_foul, Flg_FOUL_NORMAL);
						goto end;
					}
				}
			}
			else
			{	
				//MessageBox(0, "10","10",MB_OK);
				colour = m_testBall->m_colour;			// BUG: CHECK HERE!!!!! what if i aim a green ball, and swerve around it to hit the yellow????????
				if(colour == WHITE || colour == RED)
				{
					colour = m_ball[m_nominateBall]->m_colour;
				}
				
				// override colour, if on colours at the end of a frame
				if(m_colourseq >= YELLOW)
					colour = m_colourseq;

				if(colour == WHITE)
					colour = -1;
				
				

				if(colour != m_ball[m_dominoHit1st]->m_colour)
				{
					// FOUL!!!! - ball that is hit is the foul
					if(colour < m_ball[m_dominoHit1st]->m_colour)
						colour = m_ball[m_dominoHit1st]->m_colour;

					///if(colour >= YELLOW)
					//{
						// foul! Give 4 pts to oppenent
						if(colour <= BROWN)
							if(m_bTurn1P) m_score2P += 4;
							else m_score1P += 4;

						// foul! Give 5,6 or 7 pts to oppenent
						if(colour >= BLUE)
							if(m_bTurn1P) m_score2P += colour;
							else m_score1P += colour;

						SetFlag(m_foul, Flg_FOUL_NORMAL);
						goto end;
					//}

				}

				// also check if 'm_dominoHit1st' colour hits another colour in
				
				// look for highest value colour that was potted
				colour = 0;
				for(n=0;n<MAX_TOTALBALLS;n++)
				{
					if(TestFlag(m_ball[n]->m_flags, Flg_BALL_POTTED))
					{
						// higher or lower?
						if(colour < m_ball[n]->m_colour)
						colour = m_ball[n]->m_colour;
					}
				}
			
				if(colour >= YELLOW)
					if(colour != m_ball[m_dominoHit1st]->m_colour)
					{
						
						// higher or lower
						if(colour < m_ball[m_dominoHit1st]->m_colour)
							colour = m_ball[m_dominoHit1st]->m_colour;

						// foul! Give 4 pts to oppenent
						if(colour <= BROWN)
							if(m_bTurn1P) m_score2P += 4;
							else m_score1P += 4;

						// foul! Give 5,6 or 7 pts to oppenent
						if(colour >= BLUE)
							if(m_bTurn1P) m_score2P += colour;
							else m_score1P += colour;

						SetFlag(m_foul, Flg_FOUL_NORMAL);
						goto end;
					}
					
				
			}
		//}
		/////////////////////////////////////////////////////////////


		/////////////////////////////////////////////////////////////
		// normal pots...
		if(!bWhiteInPocket)
		{			
			//MessageBox(0, "11","11",MB_OK);

			for(n=0;n<MAX_TOTALBALLS;n++)
			{
				if(TestFlag(m_ball[n]->m_flags, Flg_BALL_POTTED))
				{					
					int prev_brk1P = m_break1P;
					int prev_brk2P = m_break2P;

					// was it a good pot?
					if(g_gfxDetails.bSoundClapping)
					//if(m_bCueError == TRUE)
					if(m_cueError)
					{
						
							if(m_strikeMaxRnd < 0) m_strikeMaxRnd = -m_strikeMaxRnd;
							float goodpot = m_strikeMaxRnd;
							goodpot += (0.00008) * (m_strikeDist / (BALL_RADIUS*20));
							
							/*
							if(m_cueError == 1)
							{
								if(goodpot > (3.8 * 0.00008 * 3.5))
									Msfx_smpplayvol(&GAMEAPPVIEW->m_game_smp[SFX_GOODSHOT], 180);
							}
							else if(m_cueError == 2)
							{
								//if(goodpot > (3.8 * 0.00008 * 5.5))
								//if(goodpot > (3.8 * 0.00008 * 5.65))
								
								if(goodpot > (3.8 * 0.00008 * 5.75))
									Msfx_smpplayvol(&GAMEAPPVIEW->m_game_smp[SFX_GOODSHOT], 180);
							}
							*/

							//if(m_strikeMaxRnd < 0) m_strikeMaxRnd = -m_strikeMaxRnd;
							//if(m_strikeMaxRnd > (3.0 * 0.00008 * 3.5))
								//Msfx_smpplayvol(&GAMEAPPVIEW->m_game_smp[SFX_GOODSHOT], 160);
					}

					if(m_bTurn1P) {
						m_score1P += m_ball[n]->m_colour;
						m_break1P += m_ball[n]->m_colour;
					}
					else {
						m_score2P += m_ball[n]->m_colour;
						m_break2P += m_ball[n]->m_colour;
					}
					
					/*
					// simple test only
					char buff[80];
					sprintf(buff, "m_break1P = %d", m_break1P);
					ErrorMessage(g_wnd,buff);
					RefereeScoreSfx(m_break1P);
					*/
					
					// store current players 1/2 break
					if(m_bTurn1P) m_currentBreak = m_break1P;
					else m_currentBreak = m_break2P;

					// has a 100+ break been reached yet
					if(m_bTurn1P)
					{
						if(prev_brk1P < 100 && m_break1P >= 100)
							Msfx_smpplayvol(&GAMEAPPVIEW->m_game_smp[SFX_CENTURY], 255);							
					}
					else
					{
						if(prev_brk1P < 100 && m_break2P >= 100)
							Msfx_smpplayvol(&GAMEAPPVIEW->m_game_smp[SFX_CENTURY], 255);
					}

					//if(m_rednext)
					//	m_rednext = FALSE;
					//else
					//	m_rednext = TRUE;




					m_bMiss = FALSE;		// potted ball
					m_foul = 0;				// no foul

					// timer
					int tbl = GAMEAPPVIEW->m_lobby->GetTableIdxFromPlayer(GAMEAPPVIEW->m_playerSockIdx);
					if(tbl!=-1)
					{
						m_shotTime =  m_table[tbl].shotTime*50;
					}
					

				}
			}

			if(!m_colourseq) // if we are NOT on the 'colour seq.' at the end of the frame
			if(m_bMiss == FALSE)
			{
				if(m_rednext)
					m_rednext = FALSE;
				else
					m_rednext = TRUE;
			}
		}
		/////////////////////////////////////////////////////////////
																
		
		/////////////////////////////////////////////////////////////
		// if white went in off & no other ball was potted (***)
		if(bWhiteInPocket)
		{
			//MessageBox(0, "12","12",MB_OK);
			colour = m_ball[m_dominoHit1st]->m_colour;

			// foul! Give 4 pts to oppenent
			if(colour <= BROWN)
				if(m_bTurn1P) m_score2P += 4;
				else m_score1P += 4;

			// foul! Give 5,6 or 7 pts to oppenent
			if(colour >= BLUE)
				if(m_bTurn1P) m_score2P += colour;
				else m_score1P += colour;

			goto end;
		}
		/////////////////////////////////////////////////////////////


end:

		// normal foul sfx
		if(TestFlag(m_foul, Flg_FOUL_NORMAL))
		{
			m_bPriorVoiceOnly = TRUE;
			if(g_build.addOnSfx && g_gfxDetails.bSoundVoices)
				Msfx_smpplayvol(&GAMEAPPVIEW->m_game_smp[56], 200); // "Foul!"
		}

		// check for touching ball sfx
		int nn;
		for(nn = 0; nn<MAX_TOUCHING_BALLS; nn++)
		{
			if(m_touchingBall[m_touchingBallIdx++] != 0)
			{
				if(!m_bPriorVoiceOnly)
				{
					if(g_build.addOnSfx && g_gfxDetails.bSoundVoices)
						Msfx_smpplayvol(&GAMEAPPVIEW->m_game_smp[61], 200); // "TouchingBall!"
				}
				break;
			}
		}

		/////////////////////////////////////////////////////////////
		// check if nothing was potted this time
		if(m_bMiss)
		{
			// end of break sounds
			if(g_gfxDetails.bSoundClapping)
			{
				if(m_bTurn1P)
				{
					if(m_break1P >= 20 && m_break1P < 100)
					{
						float loudness = 128 + 128*(((float)m_break1P - 20)/80);
						Msfx_smpplayvol(&GAMEAPPVIEW->m_game_smp[SFX_20PLUSBREAK], loudness);
					}
					
					/*
					if(m_break1P >= 100)
					{
						Msfx_smpplayvol(&GAMEAPPVIEW->m_game_smp[SFX_CENTURY], 255);
					}
					*/
					
				}
				else
				{
					if(m_break2P >= 20 && m_break2P < 100)
					{
						float loudness = 128 + 128*(((float)m_break1P - 20)/80);
						Msfx_smpplayvol(&GAMEAPPVIEW->m_game_smp[SFX_20PLUSBREAK], loudness);
					}

					/*
					if(m_break2P >= 100)
					{
						Msfx_smpplayvol(&GAMEAPPVIEW->m_game_smp[SFX_CENTURY], 255);
					}
					*/
				}
			}

			//MessageBox(0, "13","13",MB_OK);
			if(m_bPracticeMode && !GAMEAPPVIEW->m_bReplayMode && !GAMEAPPVIEW->m_bPlayOffline)
			{
				// force no fouls in practice
				//m_foul = 0;
				//m_break1P = 0;
			}
			else
			{
				// decide who's turn it is next?
				bNextPlayer = TRUE;
				m_bConcededGame = FALSE; // clear this flag (now that the turn has swapped over, we can allow gameOver to continue...)
				GAMEAPPVIEW->m_bFlashWindow = TRUE;

				// timer
				if(TestFlag(m_foul, Flg_FOUL_NORMAL))
					m_shotTime = 60*50;
				else
				{
					// if no foul, then give usual time for shot
					int tbl = GAMEAPPVIEW->m_lobby->GetTableIdxFromPlayer(GAMEAPPVIEW->m_playerSockIdx);
					if(tbl!=-1)
					{
						m_shotTime =  m_table[tbl].shotTime*50;
					}
				}

				//ErrorMessage(m_parent, "1. reg break!");

				if(m_bTurn1P) {
					m_bTurn1P = FALSE;
					m_break2P = 0;

					// is it a high break?
					//if(m_break1P >= 50)
					{
						if(m_highBreaks1P[0] <= m_highBreaks1P[1])
						{
							if(m_break1P > m_highBreaks1P[0])
								m_highBreaks1P[0] = m_break1P;
						}
						else
						{
							if(m_break1P > m_highBreaks1P[1])
								m_highBreaks1P[1] = m_break1P;
						}
					}
				}
				else{
					m_bTurn1P = TRUE;
					m_break1P = 0;

					// is it a high break?
					//if(m_break2P >= 50)
					{
						if(m_highBreaks2P[0] <= m_highBreaks2P[1])
						{
							if(m_break2P > m_highBreaks2P[0])
								m_highBreaks2P[0] = m_break2P;
						}
						else
						{
							if(m_break2P > m_highBreaks2P[1])
								m_highBreaks2P[1] = m_break2P;
						}
					}
				}
			}

			// if we are NOT on the 'colour seq.' at the end of the frame
			if(!m_colourseq)
				m_rednext = TRUE;
		}
		/////////////////////////////////////////////////////////////


		/////////////////////////////////////////////////////////////
		// clear all 'POTTED' balls
		for(n=0;n<MAX_TOTALBALLS;n++)
			if(TestFlag(m_ball[n]->m_flags, Flg_BALL_POTTED))
				ClearFlag(m_ball[n]->m_flags, Flg_BALL_POTTED);
		/////////////////////////////////////////////////////////////


		// re-spot balls
		ReSpot();

		
		///////////
		// for practice mode : fixes "pot yellow aand white simul. bug" ***
		if(m_bMiss)
		{
			//MessageBox(0, "13b","13b",MB_OK);
			if(m_bPracticeMode && (!GAMEAPPVIEW->m_bPlayOffline || GAMEAPPVIEW->m_bReplayMode))
			{
				// force no fouls in practice
				m_foul = 0;
				m_break1P = 0;
			}
		}
		///////////

		
		/////////////////////////////////////////////////////////////
		// "FOUL and a MISS" rule
		if(m_bMissRule) // <-- doesn't get set offline in practice mode
		if(TestFlag(m_foul, Flg_FOUL_NORMAL))
		{									
			BOOL bDoMissRule = TRUE;

			// check scores
			m_bestScore = 0;

			int totalPtsLeft = TotalPtsLeft();
			///////////////////////////////////////////////////////
			// how many pts does this player require in snookers
			int bestScore = 0;
			int diffPts;

			diffPts = m_score1P - m_score2P;			
			if(diffPts < 0)
			{
				// 1P
				bestScore = diffPts + totalPtsLeft;

				if(bestScore <= 0)
					bDoMissRule = FALSE;
			}
			else
			{
				// 2P
				diffPts = m_score2P - m_score1P;
				bestScore = diffPts + totalPtsLeft;

				if(bestScore <= 0)
					bDoMissRule = FALSE;
			}
			///////////////////////////////////////////////////////
			
			if(bDoMissRule)
				SetFlag(m_foul, Flg_FOUL_MISS);
		}
		/////////////////////////////////////////////////////////////
//		/////////////////////////////////////////////////////////////
//		// "FOUL and a MISS" rule
//		if(m_bMissRule) // <-- doesn't get set offline in practice mode
//		if(TestFlag(m_foul, Flg_FOUL_NORMAL))
//		{						
//			// what was playing going for?
//			int targetColour = m_ball[m_nominateBall]->m_colour;
//			if(targetColour <= RED)
//			{
//				targetColour = m_testBall->m_colour;
//				if(targetColour == WHITE)
//				{
//					// this player must be going for a red ball
//					targetColour = RED;
//				}				
//			}
//
//			if(m_rednext)
//				targetColour = RED;
//
//			// if player missed their target ball
//			if(targetColour != m_ball[m_dominoHit1st]->m_colour)
//			{
//				SetFlag(m_foul, Flg_FOUL_MISS);
//				
//				{
//				char buf[256];
//				sprintf(buf, "MISS-RULE: targetColour = %d, m_dominoHit1st = %d", targetColour, m_ball[m_dominoHit1st]->m_colour);
//				ErrorMessage(m_parent, buf);
//				}
//
//				/*
//				// foul! Give 4 pts to oppenent
//				if(targetColour <= BROWN)
//					if(m_bTurn1P) m_score2P += 4;
//						else m_score1P += 4;
//  
//				// foul! Give 5,6 or 7 pts to oppenent
//				if(targetColour >= BLUE)
//					if(m_bTurn1P) m_score2P += targetColour;
//						else m_score1P += targetColour;
//				
//				goto end;
//				*/
//			}
//		}
//		/////////////////////////////////////////////////////////////

		// check for free - ball
		if(TestFlag(m_foul, Flg_FOUL_NORMAL))
		{			
			// in-off freeball check
			// if(TestFlag(m_foul, Flg_FOUL_WHITEPOTTED))
			if(bWhiteInPocket)
			{
					// [this is NOT a 100% full test for a freeball after an in-off]
				float f1,f2,f3;

				// save WHITE ball's current position
				CVector PWhite = m_ball[0]->m_P;		

				// position ball on its original starting position or...
				if( DropBallOnTable(WHITE, m_ball[0]->m_PSpot.x, m_ball[0]->m_PSpot.y) )
				{
					
					if(!FreeBallCheck())
					{
						goto _endInOffFreeBallChecking;
					}
				}
				//else					
				{
					// ...otherwise position anywhere in the 'D'
					CVector Pgreen, Pyellow;
					CVector P;
					float radius;
					Pgreen = CVector(-WORLD_X(912),-WORLD_Y(395-12), BALL_POSITIONZOFFSET);
					Pyellow = CVector(-WORLD_X(912),-WORLD_Y(199+12), BALL_POSITIONZOFFSET);
					float w,h;
					float x,y;
					
					//[f1 = Pyellow.y - Pgreen.y;]
					f1 = Pyellow.y;
					f1 = f1 - Pgreen.y;
					
					f1 = f1 * 0.9f;
					h =	f1;
					//h =	(Pyellow.y - Pgreen.y)*0.9f;
					w = h*0.5f;

					x = 0.0f;
					y = 0.0f;
					for(x = 0.0f;x<w;x+=BALL_RADIUS)
					for(y = 0.0f;y<h;y+=BALL_RADIUS)
					{
						//[f1 = Pyellow.x - x;]
						f1 = Pyellow.x;
						f1 = f1 - x;

						//[f2 = Pyellow.y - Pgreen.y;]
						f2 = Pyellow.y;
						f2 = f2 - Pgreen.y;

						f2 = f2 * 0.05f;
						
						//[f3 = Pyellow.y - y;]
						f3 = Pyellow.y;
						f3 = f3 - y;

						f3 = f3 - f2;

						// Free Ball Not Here
						// f3 = 110.49002;
						// f1 = -1159.6301;

						if( DropBallOnTable(WHITE, f1, f3) )
						{
							if(!FreeBallCheck())
							{
								goto _endInOffFreeBallChecking;							
							}
						//if( DropBallOnTable(WHITE, Pyellow.x - x, Pyellow.y - y - (Pyellow.y - Pgreen.y)*0.05f) )
						//	goto _whiteSpotted;
						}
					}
					//_whiteSpotted:; 
				}
			
				// if we get here, then must be a freeball situation
				SetFlag(m_foul, Flg_FOUL_FREEBALL);

				_endInOffFreeBallChecking:;			
				// restore WHITE ball's position
				m_ball[0]->m_POld = m_ball[0]->m_P = PWhite;
			}
			else
			{
				if(FreeBallCheck())
				{
					SetFlag(m_foul, Flg_FOUL_FREEBALL);
				}
			}
			
			
		}

/*
		// check if opponent fouled!
		if(m_bFoul)
		if(MyTurn())
		{
			MessageBox(0, "14","14",MB_OK);
			m_bFoulDecideTurn = TRUE;
			m_bFoulShowDialog = TRUE;
		}
*/

		// check for gameover!
		// 1) if require >=10pts in snookers it's gameover
		// 2) if on black ball and cannot draw or win it's gameover
		if(!m_bPracticeMode || GAMEAPPVIEW->m_bPlayOffline)
		{
			m_bestScore = 0;

			int totalPtsLeft = TotalPtsLeft();
			
			///////////////////////////////////////////////////////
			// how many pts does this player require in snookers
			int bestScore = 0;
			int diffPts;

			diffPts = m_score1P - m_score2P;			
			if(diffPts < 0)
			{
				// 1P
				bestScore = diffPts + totalPtsLeft;
				if(bestScore < 0)
					m_bestScore = bestScore;
				
				if(bestScore <= -10)
					if(bNextPlayer)
						m_gameOver = 1;					
			}
			else
			{
				// 2P
				diffPts = m_score2P - m_score1P;
				bestScore = diffPts + totalPtsLeft;

				if(bestScore < 0)
					m_bestScore = bestScore;
				
				if(bestScore <= -10)
					if(bNextPlayer)
						m_gameOver = 2;
			}
			///////////////////////////////////////////////////////

			if(totalPtsLeft == 0) // if table is empty
			{
				m_bConcededGame = FALSE; // Clear this flag (now that the turn has finished, we can allow gameOver to continue...)

				if(m_score1P < m_score2P)
					m_gameOver = 1;
				else if(m_score2P < m_score1P)
					m_gameOver = 2;
				else
				{
					// *TODO: draw game : display a msg & toss of a coin
					m_gameOver = 3; // respotted black
					//bNextPlayer = TRUE; // trick to get into (****) for recording current high break
					
					// ***RESPOTTED BLACK!***
					// Setup snooker table
					ClearBalls();
					m_reRackType = RERACK_BLACK;
					ReRack();
					
					// give ball in hand
					m_bBallInHand = 1;
					m_bStandUpFromShot = 1;
				}
			}
			else if(totalPtsLeft == 7) // if only black ball is left
			{
				// if foul on the black!
				if(TestFlag(m_foul, Flg_FOUL_NORMAL))
				{
					if(m_score1P == m_score2P) // now drawing after the foul on the black
					{
						; // so carry on playing...
					}
					else
					{						
						// gameover!
						if(m_score1P < m_score2P)
							m_gameOver = 1; // 2P wins
						else if(m_score2P < m_score1P)
							m_gameOver = 2; // 1P wins
					}
				}
				else // default, no foul...
				{
					if( (m_score1P + 7) < m_score2P)
					{
						if(bNextPlayer)
							m_gameOver = 1;
					}
					else if( (m_score2P + 7) < m_score1P)
					{
						if(bNextPlayer)
							m_gameOver = 2;
					}
				}

				/* SEE ABOVE ^
				// if someone does a foul on the respot black (NEW!!!!!)
				if(m_reRackType == RERACK_BLACK)
				if(TestFlag(m_foul, Flg_FOUL_NORMAL))				
				{
					if(m_bTurn1P)
						m_gameOver = 2; // i.e. now player 1's turn, player 2 had fouled before-hand, so P2 loses.
					else
						m_gameOver = 1; // i.e. now player 2's turn, player 1 had fouled before-hand, so P1 loses.
				}
				*/
			}
			/*
			else // how many pts does this player require in snookers
			{
				int bestScore = 0;
				int diffPts;

				diffPts = m_score1P - m_score2P;
				if(diffPts < 0)
				{
					// 1P
					bestScore = diffPts + totalPtsLeft;
					
					if(bestScore < 0)
						m_bestScore = bestScore;
					
					if(bestScore <= -10)
						if(bNextPlayer)
							m_gameOver = 1;					
				}
				else
				{
					// 2P
					diffPts = m_score2P - m_score1P;
					bestScore = diffPts + totalPtsLeft;

					if(bestScore < 0)
						m_bestScore = bestScore;
					
					if(bestScore <= -10)
						if(bNextPlayer)
							m_gameOver = 2;
				}
			}
			*/

			// game over!, is this player still on a big break!? (****)
			if(!m_bConcededGame)
			if(m_gameOver)
			if(!bNextPlayer)
			{
				//ErrorMessage(m_parent, "2. reg break!");
				// end of break sounds
				if(g_gfxDetails.bSoundClapping)
				{
					if(m_bTurn1P)
					{
						if(m_break1P >= 20 && m_break1P < 100)
						{
							float loudness = 128 + 128*(((float)m_break1P - 20)/80);
							Msfx_smpplayvol(&GAMEAPPVIEW->m_game_smp[SFX_20PLUSBREAK], loudness);
						}

						if(m_break1P >= 100)
						{
							Msfx_smpplayvol(&GAMEAPPVIEW->m_game_smp[SFX_CENTURY], 255);
						}
					}
					else
					{
						if(m_break2P >= 20 && m_break2P < 100)
						{
							float loudness = 128 + 128*(((float)m_break1P - 20)/80);
							Msfx_smpplayvol(&GAMEAPPVIEW->m_game_smp[SFX_20PLUSBREAK], loudness);
						}

						if(m_break2P >= 100)
						{
							Msfx_smpplayvol(&GAMEAPPVIEW->m_game_smp[SFX_CENTURY], 255);
						}
					}
				}

				// high breaks?...
				if(m_bTurn1P)
				{
					// is it a high break?
					//if(m_break1P >= 50)
					{
						if(m_highBreaks1P[0] <= m_highBreaks1P[1])
						{
							if(m_break1P > m_highBreaks1P[0])
								m_highBreaks1P[0] = m_break1P;
						}
						else
						{
							if(m_break1P > m_highBreaks1P[1])
								m_highBreaks1P[1] = m_break1P;
						}
					}
				}				
				else
				{
					// is it a high break?
					//if(m_break2P >= 50)
					{
						if(m_highBreaks2P[0] <= m_highBreaks2P[1])
						{
							if(m_break2P > m_highBreaks2P[0])
								m_highBreaks2P[0] = m_break2P;
						}
						else
						{
							if(m_break2P > m_highBreaks2P[1])
								m_highBreaks2P[1] = m_break2P;
						}
					}
				}

				if(m_gameOver == 3) // special case for draw game (respotted black)
				{
					m_gameOver = 0;

					// reset breaks
					m_break1P = 0;
					m_break2P = 0;
				}
			}
		}

}


///////////////////////////////////////////////////////////////////////////////////////////////////
//
//
// enforce rules & keep scores
void CSnooker::RefereeBilliards(void)
{
		int n;
		int colour;
		int no_colours;
		BOOL bWhiteInPocket = FALSE;
		BOOL bNextPlayer = FALSE;
		BOOL bHitTouchingBall = FALSE;
		BOOL m_bLastTurn1P;


		m_bLastTurn1P = m_bTurn1P;	// Store the turn it was before this shot was played //
		
		int _pottedballs = 0;
		BOOL Valid;
		char SEL_YELLOW = 1;

		m_bMiss = TRUE;												// assume no balls potted
		ClearFlag(m_foul, Flg_FOUL_NORMAL | Flg_FOUL_WHITEPOTTED);	// assume no foul to start with [keep some flags from previous shot]
		ClearFlag(m_foul, Flg_FOUL_FREEBALLSNOOKER); // *NEW*
		ClearFlag(m_foul, Flg_FOUL_MISS);
		ClearFlag(m_foul, Flg_FOUL_PUSH);
		m_bRefreshScores = TRUE;
		
		int tbl = GAMEAPPVIEW->m_lobby->GetTableIdxFromPlayer(GAMEAPPVIEW->m_playerSockIdx);

		// check if player has hit a touching ball?
		for(n=0;n<MAX_TOUCHING_BALLS;n++)
		if(!m_ball[0]->m_bHitCushionBeforeBall)
		if(m_oldTouchingBall[n])
		if(m_dominoHit1st == m_oldTouchingBall[n]) // check for actual ball being hit first (^ BUG above)
		{
			bHitTouchingBall = TRUE;
		}


		/////////////////////////////////////////////////////////////
		// Calculated _pottedballs
		for(n=0;n<MAX_TOTALBALLS;n++)
		{
			if(TestFlag(m_ball[n]->m_flags, Flg_BALL_POTTED))
			{
				if(m_ball[n]->m_colour >= 1 && m_ball[n]->m_colour <= 15)
				{
					_pottedballs++;
				}
			}
		}

		/////////////////////////////////////////////////////////////
		// see if white ball has been potted 
		for(n=0;n<MAX_TOTALBALLS;n++)
		{
			if(TestFlag(m_ball[n]->m_flags, Flg_BALL_POTTED))
				if(m_ball[n]->m_colour == WHITE)
				{
					bWhiteInPocket = TRUE;
				}
		}




		// Rule 1 - A Ball Must Hit the Cushion after each shot or pot a ball //
		// See if the white ball doesn't hit Cushion after shot // 
		BOOL bFailedToHitCushion = TRUE;
		
		
		for(n=0;n<MAX_TOTALBALLS;n++)
		if(m_ball[n]->m_hitCushion) bFailedToHitCushion = FALSE;

		

		// Foul if we didn't hit a Cushion on this shot with a ball //
		if(!bWhiteInPocket)
		if(_pottedballs == 0) // check nothing was potted
		if(bFailedToHitCushion == TRUE) 
			{
			SetFlag(m_foul, Flg_FOUL_NORMAL | Flg_FOUL_NOTHITCUSHION);
			if(m_bTurn1P) m_score2P += 1;
				else m_score1P += 1;
			goto end;
			}

		// Foul if we didn't shot down the table, when having ball in hand //
		if(g_bStrikeDownTable == FALSE)
			{
 			SetFlag(m_foul, Flg_FOUL_NORMAL | Flg_FOUL_NOTHITCUSHION);
			if(m_bTurn1P) m_score2P += 1;
				else m_score1P += 1;
			goto end;
			g_bStrikeDownTable = TRUE;
			}
		

		/////////////////////////////////////////////////////////////
		// What happens if Player1 strikes a ball other than the WHITE
		// or if Player2 strikes a ball other than the YELLOW
		if( (m_bTurn1P && m_selectBall != WHITE) || (!m_bTurn1P && m_selectBall != SEL_YELLOW) )
		{
			// foul, End of Turn and 1pt to opponent // ! 
			SetFlag(m_foul, Flg_FOUL_NORMAL);

			if(m_bTurn1P) m_score2P += 1;
				else m_score1P += 1;

			goto end;
		}

		

		/////////////////////////////////////////////////////////////
		
		/////////////////////////////////////////////////////////////
		// If Player doesn't hit a ball, End of Turn

		// Player1 //
		if((m_selectBall == WHITE) && (m_dominoHit1st == WHITE))
		{
			// foul, End of Turn and 1pt to opponent // ! 
			SetFlag(m_foul, Flg_FOUL_NORMAL);
			if(m_bTurn1P) m_score2P += 1;
				else m_score1P += 1;
			goto end;
		}

		// Player2
		if((m_selectBall == SEL_YELLOW) && (m_dominoHit1st == SEL_YELLOW))
		{
			// foul, End of Turn and 1pt to opponent // ! 
			SetFlag(m_foul, Flg_FOUL_NORMAL);
			if(m_bTurn1P) m_score2P += 1;
				else m_score1P += 1;
			goto end;
		}

		/////////////////////////////////////////////////////////////

	
		
		/////////////////////////////////////////////////////////////
		// normal pots...
		
		// 3 points for potting the our own ball.
		// 3 points for potting the red ball.
		// 2 points for potting the oppnent's ball. (don't respot this)
		// 2 points for a cannon

			
		Valid = FALSE;
		// If player1,then we need White to hit both the yellow and red //
		if( m_lifes == 3)
			{
			// 2pts for a cannon //
			if(m_bTurn1P)
				{
				m_score1P += 2;
				m_break1P += 2;
				}
			else{
				m_score2P += 2;
				m_break2P += 2;
				}
			
			Valid = TRUE;
			m_bMiss = FALSE;		// scored something
			m_poolPots1P = 0;		// Reset the counter if a cannon was made //

			// Set the Timer back to it's start since a Cannon was made //
			if(tbl!=-1)
				{
				m_shotTime =  m_table[tbl].shotTime*50;
				}
			}

		
				
		for(n=0;n<MAX_TOTALBALLS;n++)
		{
			if(TestFlag(m_ball[n]->m_flags, Flg_BALL_POTTED))
			{					
				int prev_brk1P = m_break1P;
				int prev_brk2P = m_break2P;

			
				if(m_bTurn1P)
					{
					// Red is worth 3 pts //
					if(m_ball[n]->m_colour == RED) 
						{
						m_score1P += 3;
						m_break1P += 3;
						m_poolPots1P++;	// Keeps track of how many RED's we've potted in a row //
						}
					// Potting Oppnent's Ball is 2 pts //
					if(m_ball[n]->m_colour == YELLOW)
						{
						m_score1P += 2;
						m_break1P += 2;
						}
					// Potting White (in off shot) is 2 pts //
					if(m_ball[n]->m_colour == WHITE)
						{
						if(m_dominoHit1st == 1)
							{
							m_score1P += 2;
							m_break1P += 2;
							}
						if(m_dominoHit1st == 2)
							{
							m_score1P += 3;
							m_break1P += 3;
							}
						}

					}
				else 
					{
					// Red is worth 3 pts //
					if(m_ball[n]->m_colour == RED) 
						{
						m_score2P += 3;
						m_break2P += 3;
						m_poolPots1P++;	// Keeps track of how many RED's we've potted in a row //
						}
					// Potting Oppnent's Ball is 2 pts //
					if(m_ball[n]->m_colour == WHITE)
						{
						m_score2P += 2;
						m_break2P += 2;
						}
					}
					
					// Potting Yellow (in off shot) is 2 pts //
					if(m_ball[n]->m_colour == YELLOW)
						{
						if(m_dominoHit1st == 0)
							{
							m_score2P += 2;
							m_break2P += 2;
							}
						else if(m_dominoHit1st == 2)
							{
							m_score2P += 3;
							m_break2P += 3;
							}
						}
				
				// store current players 1/2 break
				if(m_bTurn1P) m_currentBreak = m_break1P;
				else m_currentBreak = m_break2P;

				// has a 100+ break been reached yet
				if(m_bTurn1P)
				{
					if(prev_brk1P < 100 && m_break1P >= 100)
						Msfx_smpplayvol(&GAMEAPPVIEW->m_game_smp[SFX_CENTURY], 255);							
				}
				else
				{
					if(prev_brk1P < 100 && m_break2P >= 100)
						Msfx_smpplayvol(&GAMEAPPVIEW->m_game_smp[SFX_CENTURY], 255);
				}

				m_bMiss = FALSE;		// potted ball
				m_foul = 0;				// no foul

				// timer
				if(tbl!=-1)
				{
					m_shotTime =  m_table[tbl].shotTime*50;
				}
				

			}
		}


		// If this is the shot after our 10th Consecutive Pot of a Red and we didn't play a Cannon, then end break //
		if(m_poolPots1P == 11) 
			{
			if(Valid == FALSE) m_bMiss = TRUE;
				else m_poolPots1P = 0;
			}

		
		
		
		/////////////////////////////////////////////////////////////
end:

		// normal foul sfx
		if(TestFlag(m_foul, Flg_FOUL_NORMAL))
		{
			m_bPriorVoiceOnly = TRUE;
			if(g_build.addOnSfx && g_gfxDetails.bSoundVoices)
				Msfx_smpplayvol(&GAMEAPPVIEW->m_game_smp[56], 200); // "Foul!"
		}

		// check for touching ball sfx
		int nn;
		for(nn = 0; nn<MAX_TOUCHING_BALLS; nn++)
		{
			if(m_touchingBall[m_touchingBallIdx++] != 0)
			{
				if(!m_bPriorVoiceOnly)
				{
					if(g_build.addOnSfx && g_gfxDetails.bSoundVoices)
						Msfx_smpplayvol(&GAMEAPPVIEW->m_game_smp[61], 200); // "TouchingBall!"
				}
				break;
			}
		}

		/////////////////////////////////////////////////////////////
		// check if nothing was scored //
		if(m_bMiss)
		{
			m_poolPots1P = 0;	// Reset our counter for REDs potted in a row //

			// end of break sounds
			if(g_gfxDetails.bSoundClapping)
			{
				if(m_bTurn1P)
				{
					if(m_break1P >= 20 && m_break1P < 100)
					{
						float loudness = 128 + 128*(((float)m_break1P - 20)/80);
						Msfx_smpplayvol(&GAMEAPPVIEW->m_game_smp[SFX_20PLUSBREAK], loudness);
					}
					
					
				}
				else
				{
					if(m_break2P >= 20 && m_break2P < 100)
					{
						float loudness = 128 + 128*(((float)m_break1P - 20)/80);
						Msfx_smpplayvol(&GAMEAPPVIEW->m_game_smp[SFX_20PLUSBREAK], loudness);
					}

				}
			}

			//MessageBox(0, "13","13",MB_OK);
			if(m_bPracticeMode && !GAMEAPPVIEW->m_bReplayMode && !GAMEAPPVIEW->m_bPlayOffline)
			{
				// force no fouls in practice
				//m_foul = 0;
				//m_break1P = 0;
			}
			else
			{
				// decide who's turn it is next?
				bNextPlayer = TRUE;
				//m_bConcededGame = FALSE; // clear this flag (now that the turn has swapped over, we can allow gameOver to continue...)
				GAMEAPPVIEW->m_bFlashWindow = TRUE;

				// timer
				if(TestFlag(m_foul, Flg_FOUL_NORMAL))
					m_shotTime = 60*50;
				else
				{
					// if no foul, then give usual time for shot
					int tbl = GAMEAPPVIEW->m_lobby->GetTableIdxFromPlayer(GAMEAPPVIEW->m_playerSockIdx);
					if(tbl!=-1)
					{
						m_shotTime =  m_table[tbl].shotTime*50;
					}
				}

				//ErrorMessage(m_parent, "1. reg break!");

				if(m_bTurn1P) {
					m_bTurn1P = FALSE;
					m_break2P = 0;
					m_selectBall = 1;

					// is it a high break?
					//if(m_break1P >= 50)
					{

						
						if(m_highBreaks1P[0] <= m_highBreaks1P[1])
						{
							if(m_break1P > m_highBreaks1P[0])
								m_highBreaks1P[0] = m_break1P;
						}
						else
						{
							if(m_break1P > m_highBreaks1P[1])
								m_highBreaks1P[1] = m_break1P;
						}
					}
				}
				else {
					m_bTurn1P = TRUE;
					m_break1P = 0;
					m_selectBall = 0;

					// is it a high break?
					//if(m_break2P >= 50)
					{
						if(m_highBreaks2P[0] <= m_highBreaks2P[1])
						{
							if(m_break2P > m_highBreaks2P[0])
								m_highBreaks2P[0] = m_break2P;
						}
						else
						{
							if(m_break2P > m_highBreaks2P[1])
								m_highBreaks2P[1] = m_break2P;
						}
					}
				}
			}


			// Red is always next //
			m_rednext = TRUE;
			
		}
		
		/////////////////////////////////////////////////////////////


		

		// Use m_poolPots1P to keep track of what we can pot next //
		m_colourseq = 1;	// Can Pot Red Only
		if(!TestFlag(m_ball[1]->m_flags, Flg_BALL_POTTED))
		if(m_bTurn1P)
			{
			m_colourseq = 2;
			}

		if(!TestFlag(m_ball[0]->m_flags, Flg_BALL_POTTED))
		if(!m_bTurn1P)
			{
			m_colourseq = 3;
			}



		/////////////////////////////////////////////////////////////
		// clear all 'POTTED' balls
		for(n=0;n<MAX_TOTALBALLS;n++)
			if(TestFlag(m_ball[n]->m_flags, Flg_BALL_POTTED))
				ClearFlag(m_ball[n]->m_flags, Flg_BALL_POTTED);
		/////////////////////////////////////////////////////////////


		// re-spot balls
		// ReSpot Red Ball and our Ball only.. Don't ReSpot opponent's ball

		
		ReSpotBilliards();
		if(m_bBallInHand != 1) ClearFlag(m_foul, Flg_MustPlayDownTable);


		
		
		///////////
		// for practice mode : fixes "pot yellow aand white simul. bug" ***
		if(m_bMiss)
		{
			//MessageBox(0, "13b","13b",MB_OK);
			if(m_bPracticeMode && (!GAMEAPPVIEW->m_bPlayOffline || GAMEAPPVIEW->m_bReplayMode))
			{
				// force no fouls in practice
				m_foul = 0;
				m_break1P = 0;
			}
		}
		///////////
		m_lifes = 0;



		// check for gameover!
		// 1) if we have reached our target of 50pts, 100pts or 200 pts, end the game the next time we miss //
		// 2) if on black ball and cannot draw or win it's gameover
		if(m_bMiss)
		{
		if(!m_bConcededGame)
		if(!m_bPracticeMode || GAMEAPPVIEW->m_bPlayOffline)
			{
			m_gameOver = 0;
			int target;
			if(m_table[tbl].gameType == 6) target = 200;
			else if(m_table[tbl].gameType == 5) target = 100;
			else if(m_table[tbl].gameType == 4) target = 50;

			if(m_bLastTurn1P == 1)
				{
				// It was Player1's Turn last, so check if enough points were scored to Win Match //
				if(m_score1P >= target) m_gameOver = 2;		// Player1 Won //
				}
			else{
				// It was Player2's Turn last, so check if enough points were scored to Win Match //
				if(m_score2P >= target) m_gameOver = 1;		// Player1 Won //
				}
			}		

		m_bConcededGame = FALSE; // clear this flag (now that the turn has finished, we can allow gameOver to continue...)
		}
		
}



void CSnooker::RefereeScoreSfx(int score)
{
	int hundreds = 0;
	int tens = 0;
	int units = 0;
	int teens = 0;

	hundreds = score / 100;
	tens = (score - hundreds*100) / 10;
	units = (score - hundreds*100 - tens*10);
	teens = tens + units; // 11-19

	if(hundreds) m_currentBreak = tens + units;
	//else if(tens>=1 && tens<=2);
	else if(tens>1) m_currentBreak = units;
	else m_currentBreak = 0;

	//if(hundreds) Msfx_smpplayvol(&GAMEAPPVIEW2->m_game_smp[], 255);
	//if(tens) Msfx_smpplayvol(&GAMEAPPVIEW2->m_game_smp[20+tens], 255);

	if(hundreds==0)
	{
		if(tens==0) // units 0-9
		{
			m_voiceN = 10+units;
			if(units) 
			{
				if(g_build.addOnSfx && g_gfxDetails.bSoundVoices)
					Msfx_smpplayvol(&GAMEAPPVIEW->m_game_smp[m_voiceN], 167);
			}
		}
		else if(tens==1) // teens 11-19
		{
			if(units)
			{
				if(g_build.addOnSfx && g_gfxDetails.bSoundVoices)
					Msfx_smpplayvol(&GAMEAPPVIEW->m_game_smp[20+units], 167);
			}

			//m_voiceN = 19+teens;
			//Msfx_smpplayvol(&GAMEAPPVIEW->m_game_smp[m_voiceN], 127);
		}
		else if(tens>1) // 20's 30's, .... , 100's
		{
			if(g_build.addOnSfx && g_gfxDetails.bSoundVoices)
				Msfx_smpplayvol(&GAMEAPPVIEW->m_game_smp[30+tens-2], 167);

			// 30+0 = 20, 30+8 = 100	
			//if(units == 0) 
			//{
			//	m_voiceN = 28+tens;
			//	Msfx_smpplayvol(&GAMEAPPVIEW->m_game_smp[m_voiceN], 127);
			//}
			//else 
			//{
			//	m_voiceN = 37+tens;
			//	Msfx_smpplayvol(&GAMEAPPVIEW->m_game_smp[m_voiceN], 127);
			//}
		}
	}
	else // hundreds
	{
		if(tens==0) // units 0-9
		{
			m_voiceN = units;
			if(units)
			{
				if(g_build.addOnSfx && g_gfxDetails.bSoundVoices)
					Msfx_smpplayvol(&GAMEAPPVIEW->m_game_smp[10+units], 167);
			}
		}
		else if(tens==1) // teens 11-19
		{
			m_voiceN = tens;
			if(g_build.addOnSfx && g_gfxDetails.bSoundVoices)
				Msfx_smpplayvol(&GAMEAPPVIEW->m_game_smp[10+tens], 167);
		}
		else if(tens>1) // 20's 30's
		{
			m_voiceN = tens;
			if(g_build.addOnSfx && g_gfxDetails.bSoundVoices)
				Msfx_smpplayvol(&GAMEAPPVIEW->m_game_smp[10+tens], 167);
		}
	}

	// 1 2 3 4 5 6 7 8 9 10 11 12 13 14 15 16 17 18 19 20 30 40 50 60 70 80 90 100
}

int CSnooker::TotalPtsLeft(void)
{
	// NOTE: ensure this is called after the referee() & respot() calls
	int m;
	int reds = 0;
	int colours = 0;
	int totalPts = 0;

	// count reds...
	for(m=0;m<MAX_TOTALBALLS;m++)
		{
			if(TestFlag(m_ball[m]->m_flags, Flg_BALL_ACTIVE))
				if(m_ball[m]->m_colour == RED)
					reds++;
		}
	
	// add-up colours
	for(m=0;m<MAX_TOTALBALLS;m++)
	{
		if(TestFlag(m_ball[m]->m_flags, Flg_BALL_ACTIVE))
			if(m_ball[m]->m_colour >= YELLOW)
				colours += m_ball[m]->m_colour;
	}

	totalPts = (reds * 8) + colours;

	// if just potted a 'red' then add 7pts (i.e. assume player can pot a black) (BUGFIX)
	if(m_colourseq <= 1)
		if(!m_rednext) totalPts += 7;

	return totalPts;
}

void CSnooker::PlayAgainReceive(void)
{
	GAMEAPPVIEW->m_bFlashWindow = TRUE;

	// Encode Values //
	// Used to Stop Memory Hacks //
	g_gameAppView->m_snooker->m_customTable->Decode();

	//if(m_bFoul)
	//{
		if(m_bTurn1P) {
			m_bTurn1P = FALSE;
	
			if(g_build.gameType == GAMETYPE_SNOOKER)
				m_break2P = 0;
		}
		else {
			m_bTurn1P = TRUE;
			
			if(g_build.gameType == GAMETYPE_SNOOKER)
				m_break1P = 0;
		}
	//}


	// Encode Values //
	// Used to Stop Memory Hacks //
	g_gameAppView->m_snooker->m_customTable->Encode();
		
	///////
	// cancel mouse select (i.e. as if player right-clicked!)

	m_bStandUpFromShot = 1;
	m_cueBallSelect = 0;
	m_bCuePowerJustOn = FALSE;
	m_bCueState = FALSE;
	GAMEAPPVIEW->FreeMouse();
	///////

	m_foul = 0;

	m_bRefreshScores = TRUE;
		
	int tbl = GAMEAPPVIEW->m_lobby->GetTableIdxFromPlayer(GAMEAPPVIEW->m_playerSockIdx);
	if(tbl!=-1)
	{
		int plr1Idx = GAMEAPPVIEW->m_lobby->GetPlayerIdx(m_table[tbl].player1SockIdx);
		if(plr1Idx == -1) return;
		int plr2Idx = GAMEAPPVIEW->m_lobby->GetPlayerIdx(m_table[tbl].player2SockIdx);
		if(plr2Idx == -1) return;

		char _buffer[MAX_CHARS];
		if(m_bTurn1P)
			sprintf(_buffer,"*** %s has put %s back in play ***", m_player[plr2Idx].name, m_player[plr1Idx].name);
		else
			sprintf(_buffer,"*** %s has put %s back in play ***", m_player[plr1Idx].name, m_player[plr2Idx].name);
		GAMEAPPVIEW->Message(_buffer,10); // system message

		// reset shot time if needed
		m_shotTime =  m_table[tbl].shotTime*50; // BUGFIX: for when the other player runs down the timer, then puts this player back in with 1 second left!
	}
	else if(GAMEAPPVIEW->m_bPlayOffline)
	{
		char _buffer[MAX_CHARS];
		if(m_bTurn1P)
			sprintf(_buffer,"*** Player2 has put Player1 back in play ***");
		else
			sprintf(_buffer,"*** Player1 has put Player2 back in play ***");
		GAMEAPPVIEW->Message(_buffer,10); // system message

		// reset shot time if needed
		m_shotTime =  m_table[tbl].shotTime*50; // BUGFIX: for when the other player runs down the timer, then puts this player back in with 1 second left!
	}
}



void CSnooker::UpdateGameReceive(void)
{

	int n;
	int m;
	int nextspot;
	int dir;
	BOOL successful;
	BOOL reds;

	float f1,f2,f3;

	// This is special for UK Pool Only //
	// If a player selects the Option to take Ball-In-Hand from behind the Baulk Line, we must move the 
	// Cue-Ball to behind the Baulk Area //
	// In this situation the player is allowed to Nominate / Pot any ball on the first shot (m_bBallInHand = 2)
		
	m_lifes = 1;
	m_bBallInHand = 1;
	// Position White Behind The Baulk Line //

	// position ball on its original starting position or...
	m_ball[0]->m_POld = m_ball[0]->m_P = m_ball[0]->m_PSpot;
	m_ball[0]->m_P.x -= 1;

	
	if( m_customTable->RefereeUKPoolDropBallOnTable(WHITE, m_ball[0]->m_P.x, m_ball[0]->m_P.y ) );
	else{
		// ...otherwise position anywhere behind the 'Head String'
		CVector Pgreen, Pyellow;
		CVector P;
		float radius;
		Pgreen = CVector(-WORLD_X(591+65+120/*912+5*/), -600, BALL_POSITIONZOFFSET);
		Pyellow = CVector(-WORLD_X(591+65+120/*912+5*/), 600, BALL_POSITIONZOFFSET);
		float w,h;
		float x,y;
		
		//[f1 = Pyellow.y - Pgreen.y;]
		f1 = Pyellow.y;
		f1 = f1 - Pgreen.y;
		
		f1 = f1 * 0.9f;
		h =	f1;
		//h =	(Pyellow.y - Pgreen.y)*0.9f;
		w = h*0.5f;
		for(x = 0.0f;x<w;x+=BALL_RADIUS)
		for(y = 0.0f;y<h;y+=BALL_RADIUS)
		{
			
			//[f1 = Pyellow.x - x;]
			f1 = Pyellow.x;
			f1 = f1 - x;
			
			//[f2 = Pyellow.y - Pgreen.y;]
			f2 = Pyellow.y;
			f2 = f2 - Pgreen.y;
			
			f2 = f2 * 0.05f;
			
			//[f3 = Pyellow.y - y;]
			f3 = Pyellow.y;
			f3 = f3 - y;
			
			f3 = f3 - f2;
			if( m_customTable->RefereeUKPoolDropBallOnTable(WHITE, f1, f3) )
			//if( DropBallOnTable(WHITE, Pyellow.x - x, Pyellow.y - y - (Pyellow.y - Pgreen.y)*0.05f) )
				goto whiteSpotted;
		}
		whiteSpotted:;
	}
	
	m_ball[0]->m_W = m_ball[0]->m_V = m_ball[0]->m_VOld = CVector(0,0,0);

	// Turn Ball On
	m_ball[0]->m_flags = 0;
	SetFlag(m_ball[0]->m_flags, Flg_BALL_ACTIVE);
	m_bStandUpFromShot = 1;


}


void CSnooker::FoulAlertReceive(char n)
{
	if(n == 1)
	{
		// Push - out shot in 9-ball (Flg_FOUL_ALERTPUSHOUT)
		//SetFlag(m_foul, Flg_FOUL_ALERT | Flg_FOUL_ALERTPUSHOUT2);
		SetFlag(m_foul, Flg_FOUL_ALERTPUSHOUT2);


		m_bRefreshScores = TRUE; // might not need this??????
		
		int tbl = GAMEAPPVIEW->m_lobby->GetTableIdxFromPlayer(GAMEAPPVIEW->m_playerSockIdx);
		if(tbl!=-1)
		{
			int plr1Idx = GAMEAPPVIEW->m_lobby->GetPlayerIdx(m_table[tbl].player1SockIdx);
			if(plr1Idx == -1) return;
			int plr2Idx = GAMEAPPVIEW->m_lobby->GetPlayerIdx(m_table[tbl].player2SockIdx);
			if(plr2Idx == -1) return;

			char _buffer[MAX_CHARS];
			if(m_bTurn1P)
				sprintf(_buffer,"*** %s has decided to play a 'Push-Out' shot! ***", m_player[plr1Idx].name);
			else
				sprintf(_buffer,"*** %s has decided to play a 'Push-Out' shot! ***", m_player[plr2Idx].name);
			GAMEAPPVIEW->Message(_buffer,10); // system message
		}
		else if(GAMEAPPVIEW->m_bPlayOffline)
		{
			char _buffer[MAX_CHARS];
			if(m_bTurn1P)
				sprintf(_buffer,"*** Player1 has decided to play a 'Push-Out' shot! ***");
			else
				sprintf(_buffer,"*** Player2 has decided to play a 'Push-Out' shot! ***");
			GAMEAPPVIEW->Message(_buffer,10); // system message
		}
	}
}


void CSnooker::Pot8OnBreakRerackReceive(char type)
{
	// for all to do

	int tbl = GAMEAPPVIEW->m_lobby->GetTableIdxFromPlayer(GAMEAPPVIEW->m_playerSockIdx);

	/*
	if( (g_build.gameType == GAMETYPE_POOL) && (m_table[tbl].gameType == 7) ) UKPoolBadBreakRerackReceiveAll();
		else Pot8OnBreakRerackReceiveAll();
	*/

	if( (g_build.gameType == GAMETYPE_POOL) && (m_customTable->m_type == CTTYPE_UKPOOL) ) UKPoolBadBreakRerackReceiveAll(type);
		else Pot8OnBreakRerackReceiveAll();
	

	// Offline or English Pool //
	if(GAMEAPPVIEW->m_bPlayOffline)
	{
		int rndval = rand();
		Init(m_reRackType, rndval, 0);
	}
	else if( ((g_build.gameType == GAMETYPE_POOL) && (m_customTable->m_type == CTTYPE_UKPOOL)) )
	{
		BOOL m_bTurn1P_save = m_bTurn1P;
		int rndval = rand();
		Init(m_reRackType, rndval, 0);
		m_bTurn1P = m_bTurn1P_save;

	}
	else // online
	{
		// get host of table to send 'Reset' msg to all at table
		if(GAMEAPPVIEW->m_lobby->HostOfTable(GAMEAPPVIEW->m_playerSockIdx))
		{
			// the receive side, calls snooker->init(...
			GAMEAPPVIEW->SendReset(); // network game only!	
		}
	}
}

void CSnooker::Pot8OnBreakRerackReceiveAll(void)
{
	GAMEAPPVIEW->m_bFlashWindow = TRUE;	

	///////
	// cancel mouse select (i.e. as if player right-clicked!)
	m_bStandUpFromShot = 1;
	m_cueBallSelect = 0;
	m_bCuePowerJustOn = FALSE;
	m_bCueState = FALSE;
	GAMEAPPVIEW->FreeMouse();
	///////

	m_foul = 0;

	m_bRefreshScores = TRUE;
		
	int tbl = GAMEAPPVIEW->m_lobby->GetTableIdxFromPlayer(GAMEAPPVIEW->m_playerSockIdx);
	if(tbl!=-1)
	{
		int plr1Idx = GAMEAPPVIEW->m_lobby->GetPlayerIdx(m_table[tbl].player1SockIdx);
		if(plr1Idx == -1) return;
		int plr2Idx = GAMEAPPVIEW->m_lobby->GetPlayerIdx(m_table[tbl].player2SockIdx);
		if(plr2Idx == -1) return;

		char _buffer[MAX_CHARS];
		if(m_bTurn1P)
			sprintf(_buffer,"*** %s has chosen to Rerack! ***", m_player[plr1Idx].name);
		else
			sprintf(_buffer,"*** %s has chosen to Rerack! ***", m_player[plr2Idx].name);
		GAMEAPPVIEW->Message(_buffer,10); // system message

		// reset shot time if needed
		m_shotTime =  m_table[tbl].shotTime*50;
	}
	else if(GAMEAPPVIEW->m_bPlayOffline)
	{
		char _buffer[MAX_CHARS];
		if(m_bTurn1P)
			sprintf(_buffer,"*** Player1 has chosen to Rerack! ***");
		else
			sprintf(_buffer,"*** Player2 has chosen to Rerack! ***");
		GAMEAPPVIEW->Message(_buffer,10); // system message

		// reset shot time if needed
		m_shotTime =  m_table[tbl].shotTime*50;
	}
}


void CSnooker::UKPoolBadBreakRerackReceiveAll(char type)
{
	GAMEAPPVIEW->m_bFlashWindow = TRUE;	

	///////
	// cancel mouse select (i.e. as if player right-clicked!)
	m_bStandUpFromShot = 1;
	m_cueBallSelect = 0;
	m_bCuePowerJustOn = FALSE;
	m_bCueState = FALSE;
	GAMEAPPVIEW->FreeMouse();
	///////

	m_foul = 0;

	m_bRefreshScores = TRUE;
		
	int tbl = GAMEAPPVIEW->m_lobby->GetTableIdxFromPlayer(GAMEAPPVIEW->m_playerSockIdx);
	if(tbl!=-1)
	{
		int plr1Idx = GAMEAPPVIEW->m_lobby->GetPlayerIdx(m_table[tbl].player1SockIdx);
		if(plr1Idx == -1) return;
		int plr2Idx = GAMEAPPVIEW->m_lobby->GetPlayerIdx(m_table[tbl].player2SockIdx);
		if(plr2Idx == -1) return;

		char _buffer[MAX_CHARS];
		if(type == 0)
			{
			if(m_bTurn1P) sprintf(_buffer,"*** %s has made an illegal break. %s receives 2 shots plus the break! ***", m_player[plr1Idx].name, m_player[plr2Idx].name);
				else sprintf(_buffer,"*** %s has made an illegal break. %s receives 2 shots plus the break! ***", m_player[plr2Idx].name, m_player[plr1Idx].name);
			}
		else if(type == 1)
			{
			if(m_bTurn1P) sprintf(_buffer,"*** %s has potted the 8-Ball. %s must break again! ***", m_player[plr1Idx].name, m_player[plr1Idx].name);
				else sprintf(_buffer,"*** %s has potted the 8-Ball. %s must break again! ***", m_player[plr2Idx].name, m_player[plr2Idx].name);
			}
		GAMEAPPVIEW->Message(_buffer,40); // system message

		// reset shot time if needed
		m_shotTime =  m_table[tbl].shotTime*50;
	}
	else if(GAMEAPPVIEW->m_bPlayOffline)
	{
		char _buffer[MAX_CHARS];
		if(type == 0)
			{
			if(m_bTurn1P) sprintf(_buffer,"*** Player1 has made an illegal break. Player2 recieves 2 shots plus the break! ***");
				else sprintf(_buffer,"*** Player2 has made an illegal break. Player1 recieves 2 shots plus the break! ***");
			}
		else if(type == 1)
			{
			if(m_bTurn1P) sprintf(_buffer,"*** Player1 has potted the 8-Ball. Player1 must break again! ***");
				else sprintf(_buffer,"*** Player2 has potted the 8-Ball. Player2 must break again! ***");
			}
		GAMEAPPVIEW->Message(_buffer,40); // system message

		// reset shot time if needed
		m_shotTime =  m_table[tbl].shotTime*50;
	}
}



void CSnooker::BadBreakRerackReceive(BOOL bOpponentToBreak)
{
	// for all to do
	BadBreakRerackReceiveAll(bOpponentToBreak);

	// get host of table to send 'Reset' msg to all at table
	if(GAMEAPPVIEW->m_lobby->HostOfTable(GAMEAPPVIEW->m_playerSockIdx))
	{
		// the receive side, calls snooker->init(...
		if(m_bTurn1P)
			GAMEAPPVIEW->SendReset(1); // network game only!	
		else
			GAMEAPPVIEW->SendReset(2); // network game only!	
	}
	else if(GAMEAPPVIEW->m_bPlayOffline)
	{
		int rndval = rand();
		// the receive side, calls snooker->init(...
		if(m_bTurn1P)
			Init(m_reRackType, rndval, 1);
		else
			Init(m_reRackType, rndval, 2);
	}
}

void CSnooker::BadBreakRerackReceiveAll(BOOL bOpponentToBreak)
{
	GAMEAPPVIEW->m_bFlashWindow = TRUE;	

	///////
	// cancel mouse select (i.e. as if player right-clicked!)
	m_bStandUpFromShot = 1;
	m_cueBallSelect = 0;
	m_bCuePowerJustOn = FALSE;
	m_bCueState = FALSE;
	GAMEAPPVIEW->FreeMouse();
	///////

	m_foul = 0;

	m_bRefreshScores = TRUE;
		
	int tbl = GAMEAPPVIEW->m_lobby->GetTableIdxFromPlayer(GAMEAPPVIEW->m_playerSockIdx);
	if(tbl!=-1)
	{
		int plr1Idx = GAMEAPPVIEW->m_lobby->GetPlayerIdx(m_table[tbl].player1SockIdx);
		if(plr1Idx == -1) return;
		int plr2Idx = GAMEAPPVIEW->m_lobby->GetPlayerIdx(m_table[tbl].player2SockIdx);
		if(plr2Idx == -1) return;

		char _extra[MAX_CHARS];
		if(bOpponentToBreak)
			sprintf(_extra, "and give opponent Break!");
		else
			sprintf(_extra, "and have the Break!");

		char _buffer[MAX_CHARS];
		if(m_bTurn1P)
			sprintf(_buffer,"*** %s has chosen to Rerack %s ***", m_player[plr1Idx].name, _extra);
		else
			sprintf(_buffer,"*** %s has chosen to Rerack %s ***", m_player[plr2Idx].name, _extra);
		GAMEAPPVIEW->Message(_buffer,10); // system message

		// reset shot time if needed
		m_shotTime =  m_table[tbl].shotTime*50;

		// do we need to swap turn?
		if(bOpponentToBreak)
		{
			if(m_bTurn1P)
				m_bTurn1P = FALSE;
			else
				m_bTurn1P = TRUE;
		}
	}
	else if(GAMEAPPVIEW->m_bPlayOffline)
	{
		char _extra[MAX_CHARS];
		if(bOpponentToBreak)
			sprintf(_extra, "and give opponent Break!");
		else
			sprintf(_extra, "and have the Break!");

		char _buffer[MAX_CHARS];
		if(m_bTurn1P)
			sprintf(_buffer,"*** Player1 has chosen to Rerack %s ***", _extra);
		else
			sprintf(_buffer,"*** Player2 has chosen to Rerack %s ***", _extra);
		GAMEAPPVIEW->Message(_buffer,10); // system message

		// reset shot time if needed
		m_shotTime =  m_table[tbl].shotTime*50;

		// do we need to swap turn?
		if(bOpponentToBreak)
		{
			if(m_bTurn1P)
				m_bTurn1P = FALSE;
			else
				m_bTurn1P = TRUE;
		}
	}
}



void CSnooker::MissResetReceive(void)
{
	// Encode Values //
	// Used to Stop Memory Hacks //
	g_gameAppView->m_snooker->m_customTable->Decode();

	
	// for all to do
	MissResetReceiveAll();

	// get host of table to send "undo" data to all at tabl
	if(GAMEAPPVIEW->m_lobby->HostOfTable(GAMEAPPVIEW->m_playerSockIdx)) // *TODO - DONT LET HOST DO THIS, LET P1 or P2 DO THIS AS THIS COULD HOLD UP GAME, OR MISS OUT DOING THIS CODE
	{
		/////////////////////////////////////////////////////////////////////////////////
		// call undo function to reset back to the previous table position:
		int tmpscore1P = m_score1P;
		int tmpscore2P = m_score2P;

		RestorePreviousPosition();

		m_score1P = tmpscore1P;
		m_score2P = tmpscore2P;
		
		m_bBallInHand = 0;
		m_break1P = 0;
		m_break2P = 0;

		// send the previous(undo) position to all at table
		GAMEAPPVIEW->SendBalls(FALSE, FALSE, -1, TRUE); //GAMEAPPVIEW->SendBalls(TRUE, TRUE);	

		// ??? may need something like this...
		//for(int m=0;m<MAX_TOTALBALLS;m++)
		//if(TestFlag(m_ball[m]->m_flags, Flg_BALL_ACTIVE|Flg_BALL_POTTED))
		//{
		//	GAMEAPPVIEW->m_snooker->m_ball[m]->UpdateFloats();
		//	ClearFlag(GAMEAPPVIEW->m_snooker->m_ball[m]->m_flags, Flg_BALL_POTTED);
		//}
		/////////////////////////////////////////////////////////////////////////////////
	}
	else if( GAMEAPPVIEW->m_snooker->m_bPracticeMode && GAMEAPPVIEW->m_bPlayOffline )
	{
		/////////////////////////////////////////////////////////////////////////////////
		// call undo function to reset back to the previous table position:
		int tmpscore1P = m_score1P;
		int tmpscore2P = m_score2P;

		RestorePreviousPosition();

		m_score1P = tmpscore1P;
		m_score2P = tmpscore2P;
		
		m_bBallInHand = 0;
		m_break1P = 0;
		m_break2P = 0;
	}

	// Encode Values //
	// Used to Stop Memory Hacks //
	g_gameAppView->m_snooker->m_customTable->Encode();

}


void CSnooker::MissResetReceiveAll(void)
{
	GAMEAPPVIEW->m_bFlashWindow = TRUE;	

	//if(m_bFoul)
	//{
		if(m_bTurn1P) {
			m_bTurn1P = FALSE;
	
			if(g_build.gameType == GAMETYPE_SNOOKER)
				m_break2P = 0;
		}
		else {
			m_bTurn1P = TRUE;
			
			if(g_build.gameType == GAMETYPE_SNOOKER)
				m_break1P = 0;
		}	
	//}

	///////
	// cancel mouse select (i.e. as if player right-clicked!)
	m_bStandUpFromShot = 1;
	m_cueBallSelect = 0;
	m_bCuePowerJustOn = FALSE;
	m_bCueState = FALSE;
	GAMEAPPVIEW->FreeMouse();
	///////

	m_foul = 0;

	m_bRefreshScores = TRUE;
		
	int tbl = GAMEAPPVIEW->m_lobby->GetTableIdxFromPlayer(GAMEAPPVIEW->m_playerSockIdx);
	if(tbl!=-1)
	{
		int plr1Idx = GAMEAPPVIEW->m_lobby->GetPlayerIdx(m_table[tbl].player1SockIdx); if(plr1Idx == -1) return;
		int plr2Idx = GAMEAPPVIEW->m_lobby->GetPlayerIdx(m_table[tbl].player2SockIdx); if(plr2Idx == -1) return;
			
		int plr1bIdx = -1;
		int plr2bIdx = -1;
		if(m_table[tbl].doubles)
			{
			plr1bIdx = GAMEAPPVIEW->m_lobby->GetPlayerIdx(m_table[tbl].player1bSockIdx); if(plr1bIdx == -1) return;
			plr2bIdx = GAMEAPPVIEW->m_lobby->GetPlayerIdx(m_table[tbl].player2bSockIdx); if(plr2bIdx == -1) return;
			}
		
				

		char _buffer[MAX_CHARS];
		if(m_bTurn1P)
			{
			if(m_table[tbl].doubles) sprintf(_buffer,"*** %s / %s has had the balls replaced & put %s / %s back in play ***", m_player[plr2Idx].name, m_player[plr2bIdx].name, m_player[plr1Idx].name, m_player[plr1bIdx].name);
				else sprintf(_buffer,"*** %s has had the balls replaced & put %s back in play ***", m_player[plr2Idx].name, m_player[plr1Idx].name);
			}
		else{
			if(m_table[tbl].doubles) sprintf(_buffer,"*** %s / %s has had the balls replaced & put %s / %s back in play ***", m_player[plr1Idx].name, m_player[plr1bIdx].name, m_player[plr2Idx].name, m_player[plr2bIdx].name);
				else sprintf(_buffer,"*** %s has had the balls replaced & put %s back in play ***", m_player[plr1Idx].name, m_player[plr2Idx].name);
			}
		GAMEAPPVIEW->Message(_buffer,10); // system message

		// reset shot time if needed
		m_shotTime =  m_table[tbl].shotTime*50; // BUGFIX: for when the other player runs down the timer, then puts this player back in with 1 second left!
	}
	else if( GAMEAPPVIEW->m_snooker->m_bPracticeMode && GAMEAPPVIEW->m_bPlayOffline )
	{
		char _buffer[MAX_CHARS];
		if(m_bTurn1P)
			sprintf(_buffer,"*** Player2 has had the balls replaced & put Player1 back in play ***");
		else
			sprintf(_buffer,"*** Player1 has had the balls replaced & put Player2 back in play ***");
		GAMEAPPVIEW->Message(_buffer,10); // system message

		// reset shot time if needed
		m_shotTime =  m_table[tbl].shotTime*50; // BUGFIX: for when the other player runs down the timer, then puts this player back in with 1 second left!
	}
}


///////////////////////////////////////////////////////////////////////////////////////////////////
//
//
void CSnooker::ReSpot(void)
{
	int n;
	int m;
	int nextspot;
	int dir;
	BOOL successful;
	BOOL reds;

	float f1,f2,f3;



	// special case for potting colours at the end of game. (Note: this is not a full-proof test)
	////////////////////////////////////////////////////////////////////////////////////////////////////////////
	if(!TestFlag(m_foul, Flg_FOUL_NORMAL)) // only do if NO foul has occured [//if(!m_foul) // only do if NO foul has occured]
		if(m_colourseq >= 2)
			for(n=MAX_TOTALBALLS-1; n > 0;n--)
			{
				if(!TestFlag(m_ball[n]->m_flags, Flg_BALL_ACTIVE))
					if(m_ball[n]->m_colour == m_colourseq)
					{	
						m_ball[n]->m_colour = 100; // trick code below, so this ball doesn't get respotted!!!!
						m_colourseq++;
						
						if(m_wildRackType == 8) // if 'ALL_BLACK' wild type and on colours, force on black now for two or more blacks going in
							m_colourseq = 7;

						//MessageBox(0, "a","a",MB_OK);
					}
			}
 
	////////////////////////////////
	if(m_colourseq == 1)
		m_colourseq = 2; // now, if yellow is potted, it stays in. And colourseq++ ready for the green ball etc...
	////////////////////////////////

	if(!m_colourseq)
	{
		//MessageBox(0, "b","b",MB_OK);
		// no reds left check
		reds = FALSE;
		for(m=0;m<MAX_TOTALBALLS;m++)
		{
			if(TestFlag(m_ball[m]->m_flags, Flg_BALL_ACTIVE))
				if(m_ball[m]->m_colour == RED)
					reds = TRUE;
		}

		if(!reds) {
			m_colourseq = 1; // next colour that is potted, comes out, only on next turn
		
			// only do if foul has occured
			if(TestFlag(m_foul, Flg_FOUL_NORMAL))
				m_colourseq = 2; // now we r on the colour seq. early
		}
	}

	////////////////////////
	if(m_wildRackType == 8) // if 'ALL_BLACK' wild type and on colours, force on black now
	{
		if(m_colourseq >= 2)
			m_colourseq = 7;
	}
	////////////////////////

	////////////////////////////////////////////////////////////////////////////////////////////////////////////


	// for balls in pockects
	for(n=MAX_TOTALBALLS-1; n >= 0;n--)
	if((m_ball[n]->m_colour <= BLACK && m_ball[n]->m_colour >= YELLOW) || (m_ball[n]->m_colour == WHITE))
	{	
		// temp. code for WHITE ball
		if(m_ball[n]->m_colour == WHITE)
			if(!TestFlag(m_ball[n]->m_flags, Flg_BALL_ACTIVE))
				{
					// position ball on its original starting position or...
					m_ball[n]->m_POld = m_ball[n]->m_P = m_ball[n]->m_PSpot;
					
					if( DropBallOnTable(WHITE, m_ball[n]->m_P.x, m_ball[n]->m_P.y ) );
					else					
					{
						// ...otherwise position anywhere in the 'D'
						CVector Pgreen, Pyellow;
						CVector P;
						float radius;
						Pgreen = CVector(-WORLD_X(912),-WORLD_Y(395-12), BALL_POSITIONZOFFSET);
						Pyellow = CVector(-WORLD_X(912),-WORLD_Y(199+12), BALL_POSITIONZOFFSET);
						float w,h;
						float x,y;
						
						//[f1 = Pyellow.y - Pgreen.y;]
						f1 = Pyellow.y;
						f1 = f1 - Pgreen.y;
						
						f1 = f1 * 0.9f;
						h =	f1;
						//h =	(Pyellow.y - Pgreen.y)*0.9f;
						w = h*0.5f;
						for(x = 0.0f;x<w;x+=BALL_RADIUS)
						for(y = 0.0f;y<h;y+=BALL_RADIUS)
						{
							
							//[f1 = Pyellow.x - x;]
							f1 = Pyellow.x;
							f1 = f1 - x;
							
							//[f2 = Pyellow.y - Pgreen.y;]
							f2 = Pyellow.y;
							f2 = f2 - Pgreen.y;
							
							f2 = f2 * 0.05f;
							
							//[f3 = Pyellow.y - y;]
							f3 = Pyellow.y;
							f3 = f3 - y;
							
							f3 = f3 - f2;
							if( DropBallOnTable(WHITE, f1, f3) )
							//if( DropBallOnTable(WHITE, Pyellow.x - x, Pyellow.y - y - (Pyellow.y - Pgreen.y)*0.05f) )
								goto whiteSpotted;
						}
						whiteSpotted:;
					}
					
//					m_ball[n]->m_POld = m_ball[n]->m_P = m_ball[n]->m_PSpot;
					m_ball[n]->m_W = m_ball[n]->m_V = m_ball[n]->m_VOld = CVector(0,0,0);
	
					// turn ball on
					m_ball[n]->m_flags = 0;
					SetFlag(m_ball[n]->m_flags, Flg_BALL_ACTIVE);

					m_bBallInHand = 1;
					m_bStandUpFromShot = 1;

					continue;
				}
		
		////////////////////////
		if(m_wildRackType == 8) // if 'ALL_BLACK' wild type, then skip test (1) and goto (2)
		{
			successful = FALSE;
			goto _skipTest1;
		}
		////////////////////////

		/////////////////////////////////////////////////////////////////////
		// (1) try and spot on starting position
		/////////////////////////////////////////////////////////////////////
		successful = TRUE;
		for(m=0;m<MAX_TOTALBALLS;m++)
		if(n!=m)
		if(TestFlag(m_ball[m]->m_flags, Flg_BALL_ACTIVE)) // NEW: make sure this ball is active
		{
			if(!m_ball[n]->TryReSpot1(m_ball[m]))
				successful = FALSE;
		}
		/////////////////////////////////////////////////////////////////////
_skipTest1:;
		if(successful)
			m_ball[n]->ReSpotSuccessful();
		else
		{
			/////////////////////////////////////////////////////////////////////
			// (2) try next vacant spot, starting with highest colour spot first
			/////////////////////////////////////////////////////////////////////
			for(nextspot=MAX_TOTALBALLS-1; nextspot > 0;nextspot--)
				if(m_ball[nextspot]->m_colour <= BLACK && m_ball[nextspot]->m_colour >= YELLOW)
				{
					successful = TRUE;
					for(m=0;m<MAX_TOTALBALLS;m++)
					if(n!=m)
					if(TestFlag(m_ball[m]->m_flags, Flg_BALL_ACTIVE)) // NEW: make sure this ball is active
						{
							if(!m_ball[n]->TryReSpot2(m_ball[m], m_ball[nextspot]))
								successful = FALSE;
						}
					
					if(successful) {
						m_ball[n]->ReSpotSuccessful();
						goto end;
					}
				
				}
			/////////////////////////////////////////////////////////////////////

			/////////////////////////////////////////////////////////////////////
			// (3) try and spot behind starting position in a direct line
			/////////////////////////////////////////////////////////////////////					
			// check to left side of spot (dir = -1)..., then the right side (dir = 1)...
			if(!TestFlag(m_ball[n]->m_flags, Flg_BALL_ACTIVE))
			for(dir = -1; dir <= 1; dir+=2)
			{
				// position ball on its original starting position
				m_ball[n]->m_POld = m_ball[n]->m_P = m_ball[n]->m_PSpot;
				m_ball[n]->m_W = m_ball[n]->m_V = m_ball[n]->m_VOld = CVector(0,0,0);

				do
				{
					f1 = BALL_RADIUS/20.0f;
					f1  = f1 * (float)dir;
					m_ball[n]->m_P.x -= f1;
					//m_ball[n]->m_P.x -= (BALL_RADIUS/20)*dir;

					successful = TRUE;
					for(m=0;m<MAX_TOTALBALLS;m++)
					if(n!=m)
					if(TestFlag(m_ball[m]->m_flags, Flg_BALL_ACTIVE)) // NEW: make sure this ball is active
					{
						if(!m_ball[n]->TryReSpot3(m_ball[m]))
							successful = FALSE;
					}

					if(successful) {
						m_ball[n]->ReSpotSuccessful();
						goto end;
					}

				}
				while(!m_ball[n]->CushionsProximity()); // NEW
				//while((m_ball[n]->m_P.x >= (-1736+BALL_RADIUS)) ||
				//	  (m_ball[n]->m_P.x <= (1736-BALL_RADIUS)));   // bottom cushion + BALL_RADIUS || top cushion - BALL_RADIUS
				// *TODO: note: the above boundary check is incorrect!!!, it should be using the actual 3d world "Cushions"
			}
				/////////////////////////////////////////////////////////////////////

				// we are in trouble if it gets this far!!!

		end:;
		}		
	}
}


void CSnooker::ReSpotBilliards(void)
{
	int n;
	int m;
	int nextspot;
	int dir;
	BOOL successful;
	BOOL reds;
	
	float f1,f2,f3;



	// ReSpot The Red and Our Ball Only //
	for(n=3; n >= 0;n--)
	//if((m_ball[n]->m_colour == RED) || (m_ball[n]->m_colour == YELLOW && !m_bTurn1P) || (m_ball[n]->m_colour == WHITE && m_bTurn1P))
	{	

		// code for WHITE / YELLOW ball
		int COLOR;
		
		if((m_ball[n]->m_colour == YELLOW && !m_bTurn1P) || (m_ball[n]->m_colour == WHITE && m_bTurn1P) )
			{
			if(!TestFlag(m_ball[n]->m_flags, Flg_BALL_ACTIVE))
				{
					
					if(m_ball[n]->m_colour == WHITE) COLOR = WHITE;
						else COLOR = YELLOW;
					// position ball on its original starting position or...
					m_ball[n]->m_POld = m_ball[n]->m_P = m_ball[n]->m_PSpot;


					
					if( DropBallOnTable(n, m_ball[n]->m_P.x, m_ball[n]->m_P.y ) );
					else					
					{
						// ...otherwise position anywhere in the 'D'
						CVector Pgreen, Pyellow;
						CVector P;
						float radius;
						Pgreen = CVector(-WORLD_X(912),-WORLD_Y(395-12), BALL_POSITIONZOFFSET);
						Pyellow = CVector(-WORLD_X(912),-WORLD_Y(199+12), BALL_POSITIONZOFFSET);
						float w,h;
						float x,y;
						
						//[f1 = Pyellow.y - Pgreen.y;]
						f1 = Pyellow.y;
						f1 = f1 - Pgreen.y;
						
						f1 = f1 * 0.9f;
						h =	f1;
						//h =	(Pyellow.y - Pgreen.y)*0.9f;
						w = h*0.5f;
						for(x = 0.0f;x<w;x+=BALL_RADIUS)
						for(y = 0.0f;y<h;y+=BALL_RADIUS)
						{
							
							//[f1 = Pyellow.x - x;]
							f1 = Pyellow.x;
							f1 = f1 - x;
							
							//[f2 = Pyellow.y - Pgreen.y;]
							f2 = Pyellow.y;
							f2 = f2 - Pgreen.y;
							
							f2 = f2 * 0.05f;
							
							//[f3 = Pyellow.y - y;]
							f3 = Pyellow.y;
							f3 = f3 - y;
							
							f3 = f3 - f2;
							if( DropBallOnTable(n, f1, f3) )
								goto whiteSpotted;
						}
						whiteSpotted:;
					}
					

					m_ball[n]->m_W = m_ball[n]->m_V = m_ball[n]->m_VOld = CVector(0,0,0);
	
					// turn ball on
					m_ball[n]->m_flags = 0;
					SetFlag(m_ball[n]->m_flags, Flg_BALL_ACTIVE);

		
					m_bBallInHand = 1;
					m_bStandUpFromShot = 1;

					if(TestFlag(m_foul, Flg_FOUL_NORMAL)) SetFlag(m_foul, Flg_FOUL_NORMAL | Flg_MustPlayDownTable);
					else{
						char _buffer[80];
						sprintf(_buffer, "Ball-In-Hand. You must play down the table for a Legal Shot");
						GAMEAPPVIEW->Message(_buffer, 40);
						}

					continue;
				}
			}


		// If a player has just fouled, then we need to make sure their ball is respoted also, to the Blue Spot //
		if(TestFlag(m_foul, Flg_FOUL_NORMAL))
		if(m_ball[n]->m_colour == WHITE || m_ball[n]->m_colour == YELLOW)
		if(!TestFlag(m_ball[n]->m_flags, Flg_BALL_ACTIVE))
		{
			for(dir = -1; dir <= 1; dir+=2)
			{
				// position ball on its original starting position
				
				// If we potted a red ball two times, then it must be respoted in middle //
				// 
				m_ball[n]->m_POld = m_ball[n]->m_P = m_ball[n]->m_PSpot;
				// Move Cue Ball to BLUE spot //
				m_ball[n]->m_P.x = WORLD_X(591);
				m_ball[n]->m_P.y = 0;
				

				m_ball[n]->m_W = m_ball[n]->m_V = m_ball[n]->m_VOld = CVector(0,0,0);

				do
				{
					f1 = BALL_RADIUS/20.0f;
					f1  = f1 * (float)dir;
					m_ball[n]->m_P.x -= f1;
					//m_ball[n]->m_P.x -= (BALL_RADIUS/20)*dir;

					successful = TRUE;
					for(m=0;m<MAX_TOTALBALLS;m++)
					if(n!=m)
					if(TestFlag(m_ball[m]->m_flags, Flg_BALL_ACTIVE)) // NEW: make sure this ball is active
					{
						if(!m_ball[n]->TryReSpot3(m_ball[m]))
							successful = FALSE;
					}

					if(successful)
					{
						m_ball[n]->ReSpotSuccessful();
						// If we respotted a ball, turn on Ball In Hand //
						// m_bBallInHand = 1;
						goto end_1;
					}

				}
				while(!m_ball[n]->CushionsProximity()); // NEW
				//while((m_ball[n]->m_P.x >= (-1736+BALL_RADIUS)) ||
				//	  (m_ball[n]->m_P.x <= (1736-BALL_RADIUS)));   // bottom cushion + BALL_RADIUS || top cushion - BALL_RADIUS
				// *TODO: note: the above boundary check is incorrect!!!, it should be using the actual 3d world "Cushions"
			}


		end_1:;


		}
		
		
		/////////////////////////////////////////////////////////////////////
		// (3) try and spot behind starting position in a direct line
		/////////////////////////////////////////////////////////////////////					
		// check to left side of spot (dir = -1)..., then the right side (dir = 1)...
		if(n==2)
		if(m_ball[n]->m_colour == RED)
		if(!TestFlag(m_ball[n]->m_flags, Flg_BALL_ACTIVE))
		for(dir = -1; dir <= 1; dir+=2)
		{
			// position ball on its original starting position
			
			// If we potted a red ball two times, then it must be respoted in middle //
			// 
			if(m_ball[n]->m_colour == RED)
				{
				if(m_poolPots1P&2) 
					{
					m_ball[n]->m_POld = m_ball[n]->m_P = m_ball[n]->m_PSpot;
					// Move Red to BLUE spot //
					m_ball[n]->m_P.x = WORLD_X(591); 
					}
				else{ 
					m_ball[n]->m_POld = m_ball[n]->m_P = m_ball[n]->m_PSpot;
					}
				}


			m_ball[n]->m_W = m_ball[n]->m_V = m_ball[n]->m_VOld = CVector(0,0,0);

			do
			{
				f1 = BALL_RADIUS/20.0f;
				f1  = f1 * (float)dir;
				m_ball[n]->m_P.x -= f1;
				//m_ball[n]->m_P.x -= (BALL_RADIUS/20)*dir;

				successful = TRUE;
				for(m=0;m<MAX_TOTALBALLS;m++)
				if(n!=m)
				if(TestFlag(m_ball[m]->m_flags, Flg_BALL_ACTIVE)) // NEW: make sure this ball is active
				{
					if(!m_ball[n]->TryReSpot3(m_ball[m]))
						successful = FALSE;
				}

				if(successful) 
				{
					m_ball[n]->ReSpotSuccessful();
					// If we respotted a ball, turn on Ball In Hand //
					// m_bBallInHand = 1;
					goto end;
				}

			}
			while(!m_ball[n]->CushionsProximity()); // NEW
			//while((m_ball[n]->m_P.x >= (-1736+BALL_RADIUS)) ||
			//	  (m_ball[n]->m_P.x <= (1736-BALL_RADIUS)));   // bottom cushion + BALL_RADIUS || top cushion - BALL_RADIUS
			// *TODO: note: the above boundary check is incorrect!!!, it should be using the actual 3d world "Cushions"
		}


	end:;
	}		
	
}

///////////////////////////////////////////////////////////////////////////////////////////////////
//
//
void CSnooker::FreeBallReferee(void)
{
	// when a freeball has been taken
	int n;
	int colour;
	int no_colours;
	BOOL bWhiteInPocket = FALSE;
	BOOL bNextPlayer = FALSE;
	BOOL bHitTouchingBall = FALSE;

	int freeBallColour = m_colourseq;
	if(freeBallColour == 0) 
		freeBallColour = 1;
	int myFreeBallColour = 0;

	// check if player has hit a touching ball?
	for(n=0;n<MAX_TOUCHING_BALLS;n++)
	if(!m_ball[0]->m_bHitCushionBeforeBall)
	if(m_oldTouchingBall[n])
	//if(m_ball[m_dominoHit1st]->m_colour == m_ball[m_oldTouchingBall[n]]->m_colour)
	if(m_dominoHit1st == m_oldTouchingBall[n]) // check for actual ball being hit first (^ BUG above)
	{
		bHitTouchingBall = TRUE;
	}
	//

	/////////////////////////////////////////////////////////////
	// see if white ball has been potted (foul)
	//MessageBox(0, "3","3",MB_OK);
	for(n=0;n<MAX_TOTALBALLS;n++)
	{
		if(TestFlag(m_ball[n]->m_flags, Flg_BALL_POTTED))
			if(m_ball[n]->m_colour == WHITE)
			{
				bWhiteInPocket = TRUE;
				SetFlag(m_foul, Flg_FOUL_NORMAL | Flg_FOUL_WHITEPOTTED);
			}
	}
	/////////////////////////////////////////////////////////////

	/////////////////////////////////////////////////////////////
	// what happens if player strikes a ball other than the white
	if(m_selectBall != WHITE)
	{
		//MessageBox(0, "1","1",MB_OK);
		colour = m_ball[m_selectBall]->m_colour;

		// foul! Give 4 pts to oppenent
		if(colour <= BROWN)
			if(m_bTurn1P) m_score2P += 4;
				else m_score1P += 4;

		// foul! Give 5,6 or 7 pts to oppenent
		if(colour >= BLUE)
			if(m_bTurn1P) m_score2P += colour;
				else m_score1P += colour;

		SetFlag(m_foul, Flg_FOUL_NORMAL);

		goto end;
	}
	/////////////////////////////////////////////////////////////

	// get freeBall that we're going for
	myFreeBallColour = m_testBall->m_colour;	
	if(myFreeBallColour == WHITE)
		myFreeBallColour = m_ball[m_nominateBall]->m_colour;
	//int freeBallColour = m_ball[m_dominoHit1st]->m_colour;

	// NEW* if player hasn't nominated a colour & hasn't targeted a ball :  then assume the following
	// (otherwise when on Blue, Pink and Black at the end of a frame, a player may play cushion first to hit the ball on without nominating)
	if(myFreeBallColour == WHITE)
		myFreeBallColour = freeBallColour;

/*
	/////////////////////////////////////////////////////////////
	// "FOUL and a MISS" rule
	if(m_bMissRule) // <-- doesn't get set offline in practice mode
	if(!TestFlag(m_foul, Flg_FOUL_NORMAL))
	{			
		// what was playing going for?
		int targetColour = myFreeBallColour;
		
		// if player missed their target ball
		if(targetColour != m_ball[m_dominoHit1st]->m_colour)
		{			
			{
			char buf[256];
			sprintf(buf, "FREEBALL - MISS-RULE: targetColour = %d, m_dominoHit1st = %d", targetColour, m_ball[m_dominoHit1st]->m_colour);
			ErrorMessage(m_parent, buf);
			}

			colour = freeBallColour;
			
			if(colour >= YELLOW)
			{
				// foul! Give 4 pts to oppenent
				if(colour <= BROWN)
					if(m_bTurn1P) m_score2P += 4;
					else m_score1P += 4;

				// foul! Give 5,6 or 7 pts to oppenent
				if(colour >= BLUE)
					if(m_bTurn1P) m_score2P += colour;
					else m_score1P += colour;

				SetFlag(m_foul, Flg_FOUL_NORMAL | Flg_FOUL_MISS);
				goto end;
			}
			else  {
				if(m_bTurn1P) m_score2P += 4;
					else m_score1P += 4;

				SetFlag(m_foul, Flg_FOUL_NORMAL | Flg_FOUL_MISS);
				goto end;
			}
		}	
	}
	/////////////////////////////////////////////////////////////
*/
	
	/////////////////////////////////////////////////////////////
	// if white doesn't hit a ball at all, foul ball on!
	if(m_colourseq < YELLOW) // bypass if on colours at the end
	if((m_selectBall == WHITE) && (m_dominoHit1st == WHITE))
	{
		//MessageBox(0, "2","2",MB_OK);
		// find ball-on
		colour = freeBallColour;
		//colour = m_ball[m_nominateBall]->m_colour;
		
		if(colour >= YELLOW)
		{
			// foul! Give 4 pts to oppenent
			if(colour <= BROWN)
				if(m_bTurn1P) m_score2P += 4;
				else m_score1P += 4;

			// foul! Give 5,6 or 7 pts to oppenent
			if(colour >= BLUE)
				if(m_bTurn1P) m_score2P += colour;
				else m_score1P += colour;

			SetFlag(m_foul, Flg_FOUL_NORMAL);
			goto end;
		}
		else  {
			if(m_bTurn1P) m_score2P += 4;
				else m_score1P += 4;

			SetFlag(m_foul, Flg_FOUL_NORMAL);
			goto end;
		}
	}
	/////////////////////////////////////////////////////////////


	/////////////////////////////////////////////////////////////
	// if white ball has been potted, did any other colour balls go in also?
	if(bWhiteInPocket)
	{
		//MessageBox(0, "4","4",MB_OK);
		colour = RED;
		for(n=0;n<MAX_TOTALBALLS;n++)
			if(TestFlag(m_ball[n]->m_flags, Flg_BALL_POTTED))
				if((m_ball[n]->m_colour <= BLACK) && (m_ball[n]->m_colour >= YELLOW))
					if(colour < m_ball[n]->m_colour)
					{
						if(m_ball[n]->m_colour == myFreeBallColour)
						{
							if(colour < freeBallColour)
								colour = freeBallColour;
						}
						else
						{
							// default
							colour = m_ball[n]->m_colour;
						}
					}

		
		//// potted colour!
		//if(colour >= YELLOW)
		if(m_ball[m_dominoHit1st]->m_colour <= colour) // if fail this test, then do (**) code below or (***) below
		{
			// foul! Give 4 pts to oppenent
			if(colour <= BROWN)
				if(m_bTurn1P) m_score2P += 4;
				else m_score1P += 4;

			// foul! Give 5,6 or 7 pts to oppenent
			if(colour >= BLUE)
				if(m_bTurn1P) m_score2P += colour;
				else m_score1P += colour;

			SetFlag(m_foul, Flg_FOUL_NORMAL);
			goto end;
		}
	}
	/////////////////////////////////////////////////////////////


	/////////////////////////////////////////////////////////////
	// check if player hit a touching ball first
	if(bHitTouchingBall)
	for(n=0;n<MAX_TOUCHING_BALLS;n++)
	if(!m_ball[0]->m_bHitCushionBeforeBall)
	if(m_oldTouchingBall[n])
	if(m_ball[m_dominoHit1st]->m_colour == m_ball[m_oldTouchingBall[n]]->m_colour)
	{
		// foul! - cannot hit directly a touching ball
		colour = m_ball[m_dominoHit1st]->m_colour;

		// check if a higher colour was potted
		for(n=0;n<MAX_TOTALBALLS;n++)
			if(TestFlag(m_ball[n]->m_flags, Flg_BALL_POTTED))
				if((m_ball[n]->m_colour <= BLACK) && (m_ball[n]->m_colour >= YELLOW))
					if(colour < m_ball[n]->m_colour)
						colour = m_ball[n]->m_colour;

		// foul! Give 4 pts to oppenent
		if(colour <= BROWN)
			if(m_bTurn1P) m_score2P += 4;
			else m_score1P += 4;

		// foul! Give 5,6 or 7 pts to oppenent
		if(colour >= BLUE)
			if(m_bTurn1P) m_score2P += colour;
			else m_score1P += colour;
	
		//////////////////////////////////
		char _buffer[MAX_CHARS];
		char sColour[MAX_CHARS];

		switch(colour)
		{
			case RED:
				sprintf(sColour, "a RED");
				break;
			case YELLOW:
				sprintf(sColour, "the YELLOW");
				break;
			case GREEN:
				sprintf(sColour, "the GREEN");
				break;
			case BROWN:
				sprintf(sColour, "the BROWN");
				break;
			case BLUE:
				sprintf(sColour, "the BLUE");
				break;
			case PINK:
				sprintf(sColour, "the PINK");
				break;
			case BLACK:
				sprintf(sColour, "the BLACK");
				break;
			default:
				sprintf(sColour, "[Error!]");
				break;
		}
		

		/*
		if(m_bPracticeMode)
		{
			if(m_bTurn1P)
				sprintf(_buffer,"(??? played a 'Push Shot' on %s ball)", sColour);
			else
				sprintf(_buffer,"(??? played a 'Push Shot' on %s ball)", sColour);
			GAMEAPPVIEW->Message(_buffer,10); // system message
		}
		else
		*/
		{
			int tbl = GAMEAPPVIEW->m_lobby->GetTableIdxFromPlayer(GAMEAPPVIEW->m_playerSockIdx);
			if(tbl!=-1)
			{
				int plr1Idx = GAMEAPPVIEW->m_lobby->GetPlayerIdx(m_table[tbl].player1SockIdx);
				if(plr1Idx == -1) goto endsystemmsg;
				int plr2Idx = GAMEAPPVIEW->m_lobby->GetPlayerIdx(m_table[tbl].player2SockIdx);
				if(plr2Idx == -1) goto endsystemmsg;

				if(m_bTurn1P)
					sprintf(_buffer,"(%s played a 'Push Shot' on %s ball)", m_player[plr1Idx].name, sColour);
				else
					sprintf(_buffer,"(%s played a 'Push Shot' on %s ball)", m_player[plr2Idx].name, sColour);
				GAMEAPPVIEW->Message(_buffer,10); // system message
			}
		}
endsystemmsg:;
		//////////////////////////////////

		SetFlag(m_foul, Flg_FOUL_NORMAL | Flg_FOUL_PUSH);
		goto end;
	}
	/////////////////////////////////////////////////////////////

	/////////////////////////////////////////////////////////////
	// check if more than 1 colour ball has been potted
	if(m_wildRackType != 8) // if 'ALL_BLACK' wild type, we are allowed to pot more than 1 black simul.
	if(!bWhiteInPocket)
	{
		//MessageBox(0, "5","5",MB_OK);
		no_colours = 0;
		colour = 0;
		BOOL bDiffColours = FALSE;
		
		for(n=0;n<MAX_TOTALBALLS;n++)
			if(TestFlag(m_ball[n]->m_flags, Flg_BALL_POTTED))
				if((m_ball[n]->m_colour <= BLACK) && (m_ball[n]->m_colour >= YELLOW)) {
					no_colours++;		
					//if(colour < m_ball[n]->m_colour)
					//{
						if(m_ball[n]->m_colour == myFreeBallColour)
						{							
							if(colour != 0)
								if(colour != freeBallColour)	// at least 2 of the potted balls are different
									bDiffColours = TRUE;

							if(colour < freeBallColour)
								colour = freeBallColour;
						}
						else
						{
							if(colour != 0)
								if(colour != m_ball[n]->m_colour)	// at least 2 of the potted balls are different
									bDiffColours = TRUE;

							// default
							if(colour < m_ball[n]->m_colour)
								colour = m_ball[n]->m_colour;
						}
					//}
				}

		if(bDiffColours)
		if(no_colours >= 2)
		{
			// foul! Give 4 pts to oppenent
			if(colour <= BROWN)
				if(m_bTurn1P) m_score2P += 4;
				else m_score1P += 4;

			// foul! Give 5,6 or 7 pts to oppenent
			if(colour >= BLUE)
				if(m_bTurn1P) m_score2P += colour;
				else m_score1P += colour;
		
			SetFlag(m_foul, Flg_FOUL_NORMAL);
			goto end;
		}		
	}
	/////////////////////////////////////////////////////////////

	
	/////////////////////////////////////////////////////////////
	// check if 'red' is next but a 'colour' is potted OR vice-versa
	if(!bWhiteInPocket)
	{
		if(m_rednext)
		{
			//MessageBox(0, "6","6",MB_OK);
			colour = 0;			
			for(n=0;n<MAX_TOTALBALLS;n++)
				if(TestFlag(m_ball[n]->m_flags, Flg_BALL_POTTED))
					if((m_ball[n]->m_colour <= BLACK) && (m_ball[n]->m_colour >= YELLOW))
						if(colour < m_ball[n]->m_colour)
							if(m_ball[n]->m_colour != myFreeBallColour)
								colour = m_ball[n]->m_colour;
			
			// potted colour!
			if(colour >= YELLOW)
			{
				// foul! Give 4 pts to oppenent
				if(colour <= BROWN)
					if(m_bTurn1P) m_score2P += 4;
					else m_score1P += 4;

				// foul! Give 5,6 or 7 pts to oppenent
				if(colour >= BLUE)
					if(m_bTurn1P) m_score2P += colour;
					else m_score1P += colour;

				SetFlag(m_foul, Flg_FOUL_NORMAL);
				goto end;
			}
		}
		else
		{
			// NOT NEEDED FOR FREEBALL
			/*
			//MessageBox(0, "7","7",MB_OK);
			colour = 0;			
			for(n=0;n<MAX_TOTALBALLS;n++)
				if(TestFlag(m_ball[n]->m_flags, Flg_BALL_POTTED))
					if((m_ball[n]->m_colour == RED))
						colour++;

			// if a red has been potted
			if(colour >= 1) {
				// find out what colour was nominated, a give these points to opponenet
				//colour = BROWN;	// test!!!
				//if(m_selectBall != WHITE)
				//if(m_dominoHit1st == 0)
				colour = m_ball[m_dominoHit1st]->m_colour;

				// foul! Give 4 pts to oppenent
				if(colour <= BROWN)
					if(m_bTurn1P) m_score2P += 4;
					else m_score1P += 4;

				// foul! Give 5,6 or 7 pts to oppenent
				if(colour >= BLUE)
					if(m_bTurn1P) m_score2P += colour;
					else m_score1P += colour;

				SetFlag(m_foul, Flg_FOUL_NORMAL);
				goto end;
			}
			*/
		}
	}
	/////////////////////////////////////////////////////////////


	/////////////////////////////////////////////////////////////
	// colour seq. at end of frame - check if they r taken in the correct order
	/////////////////////////////////////////////////////////////
	if(m_colourseq >= YELLOW)
	{
		colour = 0;
		//MessageBox(0, "8","8",MB_OK);
		// look for highest value colour that was potted
		for(n=0;n<MAX_TOTALBALLS;n++)
		{
			if(TestFlag(m_ball[n]->m_flags, Flg_BALL_POTTED))
			{
				// higher or lower?
				if(colour < m_ball[n]->m_colour)
				colour = m_ball[n]->m_colour;
			}
		}
		
		// did player hit the wrong colour ball?
		if(m_ball[m_dominoHit1st]->m_colour != m_colourseq)
		{
			if(m_ball[m_dominoHit1st]->m_colour == myFreeBallColour)
				if(freeBallColour == m_colourseq)
					goto _skip1;

			// higher or lower?
			if(colour < m_ball[m_dominoHit1st]->m_colour)
				colour = m_ball[m_dominoHit1st]->m_colour;

			// higher or lower?
			if(colour <  m_colourseq)
				colour =  m_colourseq;

			// foul! Give 4 pts to oppenent
			if(colour <= BROWN)
				if(m_bTurn1P) m_score2P += 4;
				else m_score1P += 4;

			// foul! Give 5,6 or 7 pts to oppenent
			if(colour >= BLUE)
				if(m_bTurn1P) m_score2P += colour;
				else m_score1P += colour;

			SetFlag(m_foul, Flg_FOUL_NORMAL);
			goto end;
		}
	}
	_skip1:
	/////////////////////////////////////////////////////////////


	/////////////////////////////////////////////////////////////
	// check if 'red' is next but a 'colour' is hit OR vice-versa (**)
	//if(!bWhiteInPocket)
	//{
		if(m_rednext)
		{			
			//MessageBox(0, "9","9",MB_OK);
			// get colour of ball that was hit first
			colour = m_ball[m_dominoHit1st]->m_colour;
			if(colour != RED)
			{	
				if(colour == myFreeBallColour);
				else
				{
					// potted colour!
					if(colour >= YELLOW)
					{
						// foul! Give 4 pts to oppenent
						if(colour <= BROWN)
							if(m_bTurn1P) m_score2P += 4;
							else m_score1P += 4;

						// foul! Give 5,6 or 7 pts to oppenent
						if(colour >= BLUE)
							if(m_bTurn1P) m_score2P += colour;
							else m_score1P += colour;

						SetFlag(m_foul, Flg_FOUL_NORMAL);
						goto end;
					}
				}
			}
		}
		else
		{	
			//MessageBox(0, "10","10",MB_OK);
			colour = m_testBall->m_colour;
			if(colour == WHITE || colour == RED)
			{
				colour = m_ball[m_nominateBall]->m_colour;
			}
			
			// override colour, if on colours at the end of a frame
			if(m_colourseq >= YELLOW)
				colour = m_colourseq;

			if(colour == WHITE)
				colour = -1;
			
			
			if(myFreeBallColour != m_ball[m_dominoHit1st]->m_colour)
			if(colour != m_ball[m_dominoHit1st]->m_colour)
			{
				// FOUL!!!! - ball that is hit is the foul
				if(colour < m_ball[m_dominoHit1st]->m_colour)
					colour = m_ball[m_dominoHit1st]->m_colour;

				///if(colour >= YELLOW)
				//{
					// foul! Give 4 pts to oppenent
					if(colour <= BROWN)
						if(m_bTurn1P) m_score2P += 4;
						else m_score1P += 4;

					// foul! Give 5,6 or 7 pts to oppenent
					if(colour >= BLUE)
						if(m_bTurn1P) m_score2P += colour;
						else m_score1P += colour;

					SetFlag(m_foul, Flg_FOUL_NORMAL);
					goto end;
				//}

			}

			// also check if 'm_dominoHit1st' colour hits another colour in
			
			// look for highest value colour that was potted
			colour = 0;
			for(n=0;n<MAX_TOTALBALLS;n++)
			{
				if(TestFlag(m_ball[n]->m_flags, Flg_BALL_POTTED))
				{					
					// check if freeBall has been potted
					if(m_ball[n]->m_colour == myFreeBallColour)
					{
					
						// check if freeBall was struck first
						if(m_ball[m_dominoHit1st]->m_colour == myFreeBallColour)
						{	
							// ok, no foul here
						}
						else
						{
							// cannot hit the colourseq-ball into the free-ball, and pot the freeball!
							// higher or lower?
							if(colour < freeBallColour)
							colour = freeBallColour;	
						}
					}
					else
					{
						// higher or lower?
						if(colour < m_ball[n]->m_colour)
						colour = m_ball[n]->m_colour;
					}
				}
			}
		
			if(colour >= YELLOW)
				//if(colour != m_ball[m_dominoHit1st]->m_colour) // plant blue onto yellow pot bug!
				if(colour != freeBallColour)
				{
					
					// higher or lower
					if(colour < m_ball[m_dominoHit1st]->m_colour)
						colour = m_ball[m_dominoHit1st]->m_colour;

					// foul! Give 4 pts to oppenent
					if(colour <= BROWN)
						if(m_bTurn1P) m_score2P += 4;
						else m_score1P += 4;

					// foul! Give 5,6 or 7 pts to oppenent
					if(colour >= BLUE)
						if(m_bTurn1P) m_score2P += colour;
						else m_score1P += colour;

					SetFlag(m_foul, Flg_FOUL_NORMAL);
					goto end;
				}
				
			
		}
	//}
	/////////////////////////////////////////////////////////////


	/////////////////////////////////////////////////////////////
	// check if player has done a freeball-snooker, which is a foul unless only pink & black remain
	if(!bWhiteInPocket)
		if(FreeBallSnookerCheck(myFreeBallColour))
		{
			colour = freeBallColour;

			// foul! Give 4 pts to oppenent
			if(colour <= BROWN)
				if(m_bTurn1P) m_score2P += 4;
				else m_score1P += 4;

			// foul! Give 5,6 or 7 pts to oppenent
			if(colour >= BLUE)
				if(m_bTurn1P) m_score2P += colour;
				else m_score1P += colour;

			SetFlag(m_foul, Flg_FOUL_NORMAL | Flg_FOUL_FREEBALLSNOOKER);
			goto end;
		}
	/////////////////////////////////////////////////////////////


	/////////////////////////////////////////////////////////////
	// normal pots...
	if(!bWhiteInPocket)
	{
		BOOL bFirstColourPotted = FALSE;
		//MessageBox(0, "11","11",MB_OK);
		for(n=0;n<MAX_TOTALBALLS;n++)
		{
			if(TestFlag(m_ball[n]->m_flags, Flg_BALL_POTTED))
			{
				colour = m_ball[n]->m_colour;
				
				/*
				//////
				// colours only...
				if(colour >= YELLOW)
				{
					// if this is the free-ball, then get it's value
					if(colour == myFreeBallColour)
					{
						if(!bFirstColourPotted)
						{
							bFirstColourPotted = TRUE;
							colour = freeBallColour;
						}
						else
						{
							// don't give any extra points for 'double' colour pots. i.e. two greens => only 3pts						
							colour = 0;
						}
					}
				}
				//////
				*/

				//////
				// colours only... [NEW: the above code looks wrong]
				if(m_wildRackType != 8) // if 'ALL_BLACK' wild type, we get multiple pot-black points
				if(colour >= YELLOW)
				{
					if(!bFirstColourPotted) // get in here once only
					{
						bFirstColourPotted = TRUE;

						// if this is the free-ball, then get it's value (otherwise colour value is correct)
						if(colour == myFreeBallColour)
							colour = freeBallColour;						
					}
					else
					{
						// don't give any extra points for 'double' colour pots. i.e. two greens => only 3pts						
						colour = 0;
					}
				}
				//////

				if(m_bTurn1P) {					
					m_score1P += colour;
					m_break1P += colour;
				}
				else {
					m_score2P += colour;
					m_break2P += colour;
				}
				
				//if(m_rednext)
				//	m_rednext = FALSE;
				//else
				//	m_rednext = TRUE;

				m_bMiss = FALSE;		// potted ball
				m_foul = 0;				// no foul
			}
		}

		if(!m_colourseq) // if we are NOT on the 'colour seq.' at the end of the frame
		if(m_bMiss == FALSE)
		{
			if(m_rednext)
				m_rednext = FALSE;
			else
				m_rednext = TRUE;
		}
	}
	/////////////////////////////////////////////////////////////

	
	/////////////////////////////////////////////////////////////
	// if white went in off & no other ball was potted (***)
	if(bWhiteInPocket)
	{
		//MessageBox(0, "12","12",MB_OK);
		colour = m_ball[m_dominoHit1st]->m_colour;

		// if white hits a freeball
		if(colour == myFreeBallColour)
		{
			colour = freeBallColour;
		}

		// foul! Give 4 pts to oppenent
		if(colour <= BROWN)
			if(m_bTurn1P) m_score2P += 4;
			else m_score1P += 4;

		// foul! Give 5,6 or 7 pts to oppenent
		if(colour >= BLUE)
			if(m_bTurn1P) m_score2P += colour;
			else m_score1P += colour;

		goto end;
	}
	/////////////////////////////////////////////////////////////


end:;
	

}


///////////////////////////////////////////////////////////////////////////////////////////////////
//
//
int CSnooker::FreeBallTestStrike(int balln, CVector StrikeDir, float maxDistance, int collideColour)
{
	int m;
	int ittr;
	float closestDist = 0;
	int closestBallIdx = -1;


	// copy selected ball attributes into 'testball' attributes
	m_testBall->m_parent = m_ball[0]->m_parent;
	m_testBall->m_flags = m_ball[0]->m_flags;
	m_testBall->m_colour = m_ball[0]->m_colour;
	m_testBall->m_ghost = m_ball[0]->m_ghost;
	m_testBall->m_hitOtherBall = m_ball[0]->m_hitOtherBall;
	m_testBall->m_P = m_ball[0]->m_P;
	m_testBall->m_POld = m_ball[0]->m_POld;
	m_testBall->m_V = m_ball[0]->m_V;
	m_testBall->m_VOld = m_ball[0]->m_VOld;
	m_testBall->m_W = m_ball[0]->m_W;
	m_testBall->m_idx = balln;

	m_testBall->Strike(StrikeDir.x, StrikeDir.y, StrikeDir.z,
						0, 0, 0,
						m_cuetip, 0,
						TRUE);

	m_bTestBallFinished = FALSE;


	//************************************************
	if(!m_bTestBallFinished)
	{
		//for(ittr = 0;ittr < ((GAME_ITTR)*60*3);ittr++)	// perform multiple processes per frame	// was v1.6
		for(ittr = 0;ittr < ((GAME_ITTR)*60*1);ittr++)	// perform multiple processes per frame	
		{
			///////////////////////////////////////////////////////////////////
			// (0) have we gone far enough?
			if(maxDistance > 0)
			{
				//[CVector D = m_testBall->m_P - m_ball[0]->m_P;]
				CVector D = m_testBall->m_P;
				D = D - m_ball[0]->m_P;

				float d = D.Magnitude();
				if(d >= maxDistance)
					return -1;
			}

			// (1) process individual ball, that is selected
			m_testBall->Process(TRUE);
			//if(m_testBall->Process(TRUE) == TRUE)
				//bActive = TRUE;

			// (2) ball2ball collisions
			for(m=1;m<MAX_TOTALBALLS;m++)
			{
				int ballOnColour = m_colourseq;
				if(ballOnColour == 0) ballOnColour = RED;

				if(balln != m)
				{
				// ignore RED balls (bugfix: getting FREEBALL when the reds are all blocking each other. i.e. starting pos.)
				if(m_ball[m]->m_colour == RED)
					continue;
				}
				
				// ignore colour that is ball-on
				//if(m_ball[m]->m_colour == ballOnColour)
				//	continue;

				// this is for freeball-snooker situations
				// do collisions...
				if((collideColour >= YELLOW) && (m_ball[m]->m_colour != collideColour) && (m_ball[m]->m_colour != ballOnColour))
				{
					int p;
					p = 0;
					// don't do collide with ball
				}
				else
				{
					// default
					// if(m_ball[m]->m_colour == PINK)
					if(m_testBall->CollideWithBall(m_ball[m], TRUE, 0, -1, 1, TRUE))
					{
						// Get Distance of this Ball, and update closestDist if it's closer
						// Probably Don't need this.....
						/* 
						//[CVector D = m_testBall->m_P - m_ball[0]->m_P;]
						CVector D = m_testBall->m_P;
						D = D - m_ball[0]->m_P;

						float d = D.Magnitude();
						
						// If closestDist isn't set, then set it
						if(closestDist == 0)
						{
							closestDist = d;
							closestBallIdx = m;
						}
						// If we already have a closestDist, then check if this new collision is closer
						else
						{
							if(d < closestDist) 
							{
								closestDist = d;
								closestBallIdx = m;
							}
						}

						*/

						m_bTestBallFinished = TRUE;
						break; // m
					}
				}
			}
			m_testBall->AntiProcess(); // update old coords

			
			// (3) end of test sim.
			if(m_bTestBallFinished)
			{
				break; // ittr
			}
			//-END-
			
			////////////////////////////////////////////////////////////////////
		}
	}
	//************************************************	

	// did we hit something?
	if(m_bTestBallFinished)
	{
		// return closestBallIdx;
		return m;
	}

	return -1;
}	   


///////////////////////////////////////////////////////////////////////////////////////////////////
//
//
BOOL CSnooker::FreeBallCheck(void)
{
	int n;	
	int colour = 0;
	int ballIdx = 0;
	
	// what is ball-on's colour?
	if(m_colourseq == 0) colour = RED;
	else colour = m_colourseq;

	// scan thru all balls on table, except the white
	for(n=1;n<MAX_TOTALBALLS;n++)
	if(TestFlag(m_ball[n]->m_flags, Flg_BALL_ACTIVE))
	if(m_ball[n]->m_colour == colour)
	{		
		// (1) - simple full-ball collision
		//[CVector D = m_ball[n]->m_P - m_ball[0]->m_P;]
		CVector D = m_ball[n]->m_P;
		D = D - m_ball[0]->m_P;
		
		ballIdx = FreeBallTestStrike(n, D, D.Magnitude() + 100);
		//if(ballIdx != n)

		if((ballIdx != n) && (ballIdx != -1))
		//if(ballIdx != -1)
			continue; // failed! Go to next ball...

		// (2) - edge-of-ball contact
		CVector N = D.Cross(CVector(0, 0, 1));
		N = N.Unit();
		float diameter = 2 * BALL_RADIUS; // Bug - Edge of ball is actually just BALL_RADIUS distance from Centre
		
		N = N * diameter;
		
		//[CVector D2 = D + N;]
		CVector D2 = D;
		D2 = D2 + N;
		
		ballIdx = FreeBallTestStrike(n, D2, D2.Magnitude() + 100);


		if((ballIdx != n) && (ballIdx != -1))
		//if(ballIdx != -1)
			continue; // failed! Go to next ball...

		// (3) - other edge-of-ball contact
		//[D2 = D - N;]
		D2 = D;
		D2 = D2 - N;
		
		ballIdx = FreeBallTestStrike(n, D2, D2.Magnitude() + 100);

		if((ballIdx != n) && (ballIdx != -1))
		//if(ballIdx != -1)
			continue; // failed! Go to next ball...

		// if we get this far, then we can hit the ball-on completely. So FreeBallCheck has failed 
		return FALSE;

	}

	// give player FreeBall option
	return TRUE;
}



///////////////////////////////////////////////////////////////////////////////////////////////////
//
// check if someone has played a snooker behind the freeball
BOOL CSnooker::FreeBallSnookerCheck(int myFreeBallColour)
{
	int n;	
	int colour = 0;
	int ballIdx = 0;
	BOOL bColourNotOnTable = TRUE;

	// what is ball-on's colour?
	if(m_colourseq == 0) colour = RED;
	else colour = m_colourseq;

	// if ball-on is a red and i just played and hit a red, cant be a freeball snooker! // BUGFIX!
	if(colour == RED && myFreeBallColour == RED)
		return FALSE;

	// scan thru all balls on table
	for(n=1;n<MAX_TOTALBALLS;n++)
	if(TestFlag(m_ball[n]->m_flags, Flg_BALL_ACTIVE))
	if(m_ball[n]->m_colour == colour)
	{		
		bColourNotOnTable = FALSE;

		// (1) - simple full-ball collision
		//[CVector D = m_ball[n]->m_P - m_ball[0]->m_P;]
		CVector D = m_ball[n]->m_P;
		D = D - m_ball[0]->m_P;
		
		ballIdx = FreeBallTestStrike(n, D, D.Magnitude(), myFreeBallColour);  // D.Mag.() was -1
		//if(ballIdx == n)
		if((ballIdx == n) || (ballIdx == -1))
		//if(ballIdx == -1)
			return FALSE;//fail++;//continue; // failed! Go to next ball...

		// (2) - edge-of-ball contact
		CVector N = D.Cross(CVector(0, 0, 1));
		N = N.Unit();
		float diameter = 2 * BALL_RADIUS;
		N = N * diameter;
		
		//[CVector D2 = D + N;]
		CVector D2 = D;
		D2 = D2 + N;

		ballIdx = FreeBallTestStrike(n, D2, D2.Magnitude(), myFreeBallColour);
		//if((ballIdx == n) || (ballIdx == -1))
		if(ballIdx == -1)
			return FALSE;//fail++;//continue; // failed! Go to next ball...

		// (3) - other edge-of-ball contact
		//[D2 = D - N;]
		D2 = D;
		D2 = D2 - N;
		
		ballIdx = FreeBallTestStrike(n, D2, D2.Magnitude(), myFreeBallColour);
		if((ballIdx == n) || (ballIdx == -1))
		//if(ballIdx == -1)
			return FALSE;///fail++;//continue; // failed! Go to next ball...
	}

	// if ball-On is no longer on the table, cannot be a FreeBall Snooker!  // BUGFIX!
	if(bColourNotOnTable)
		return FALSE;

	// we are snookered! Check if just the pink and black remain
	if((m_colourseq == PINK) && (myFreeBallColour == BLACK))
		return FALSE;
	
	// we are snookered by the freeball
	return TRUE;
}





///////////////////////////////////////////////////////////////////////////////////////////////////
//
// not used!
void CSnooker::TableStartMessages(BOOL bPractice)
{
	
	if(!bPractice)
	{
		if(m_reRackType == RERACK_EMPTY) // only do the following when the table is empty at the start
		{
			int msg = GAMEAPPVIEW->m_lobby->GetTableStartMessageForPlayer(GAMEAPPVIEW->m_playerSockIdx);

			// -1 = <display no message>
			//  0 = "click 'Sit' button to join game"
			//  1 = "waiting for opponent to click 'Sit' button..."
			//  2 = "click 'Start' to begin game"
			//  3 = "waiting for opponent to click 'Start' button..."
			switch(msg)
			{
				case -1:
					//m_score1P = 999;
					//m_bRefreshScores = TRUE;
					break;

				case 0:
					m_score1P = 990;
					m_bRefreshScores = TRUE;
					break;

				case 1:
					m_score1P = 991;
					m_bRefreshScores = TRUE;
					break;

				case 2:
					m_score1P = 992;
					m_bRefreshScores = TRUE;
					break;

				case 3:
					m_score1P = 993;
					m_bRefreshScores = TRUE;
					break;
			}
		}
	}
	else
	{
		// practice mode messages!
		m_score1P = 888;
		m_bRefreshScores = TRUE;
		// i.e. if player isn't sitting down
		//"click 'Sit' button to start practice session"
	}
}




///////////////////////////////////////////////////////////////////////////////////////////////////
//
//

extern FILE *fptr2;

extern void FPRINTFX(char *str, float *x,float *y,float *z);
/*	{
	int *xx;
	int *yy;
	int *zz;
	xx = (int *) x;
	yy = (int *) y;
	zz = (int *) z;

	fprintf(fptr2,"\r\n{[%s]%lx,%lx,%lx = %f,%f,%f}",str, *xx,*yy,*zz,*x,*y,*z);
	}*/

/*
///////////////////////////////////////////////////////////////////////////////////////////////////
//
//
#ifdef FIXEDMODE
void CSnooker::PocketProximity(float x, float y, float z, float r)
{
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
				if(m_parent->m_customTable->m_tableType == 4) // black hole
					f_V = f_V * TOFIX(0.998f);
			///////////////////////////////////////////////////////////

			__fix r2 = FIXMUL(r, r);

			if(Pocket.Magnitude2() < r2) {
				SetFlag(m_flags, Flg_BALL_HITCUSHION);

				// check if this ball was potted in 'TARGET' selected pocket
				if(m_parent->m_targetBall == m_colour)
				if(m_parent->m_targetPocket == n)				
				{
					m_parent->m_bTargetSuccess = TRUE;
				}
			}
		}
	}	
	// ----------------------------------------
}
  */
///////////////////////////////////////////////////////////////////////////////////////////////////
//
//							 
//#else
int CSnooker::PocketProximity(float x, float y, float z, float extraRadius)
{
	CVector Pos = CVector(x, y, z);
		
	// [code below is taking from Ball.cpp]
	// ----------------------------------------
	
	CVector Pocket;
	for(int n=0;n<m_customTable->m_pocket_n;n++)
	{
		Pocket = m_customTable->m_pocketPos[n];
		Pocket = Pocket - Pos;
		float sR = m_customTable->m_pocketProp[n].m_suctionRadius;
		float sF = m_customTable->m_pocketProp[n].m_suctionFactor;
		//float r = m_parent->m_customTable->m_pocketProp[n].m_radius;
		
		Pocket.z = 0;
		
		if(Pocket.Magnitude2() < SQR((sR + extraRadius)) )
		{
			return n;
			/*
			CVector V = Pocket.Unit();
			V = V * sF;
			
			m_V = m_V + V;

			///////////////////////////////////////////////////////////
			if(g_build.gameType == GAMETYPE_POOL)
				if(m_parent->m_customTable->m_tableType == 4) // black hole
					m_V = m_V * 0.998f;
			///////////////////////////////////////////////////////////

			if(Pocket.Magnitude2() < SQR(r))
			SetFlag(m_flags, Flg_BALL_HITCUSHION);
			*/
		}
	}	
	// ----------------------------------------

	return -1;
}
//#endif //FIXEDMODE


///////////////////////////////////////////////////////////////////////////////////////////////////
//
//							 
void CSnooker::TestBallStrike(CVector StrikeDir, M3d_vtx cueTip, BOOL bWhiteOnly)
{
	int m;
	int ittr;

	///////////////////////////////////////////////////////////////////////////////////
	// for undo
	SavePreviousPosition();
	///////////////////////////////////////////////////////////////////////////////////

	HitCushionBallsInit();

	///////////////////////////////////////////////////////////////////////////////////
	m_ball[0]->Strike(StrikeDir.x, StrikeDir.y, StrikeDir.z,
						0, 0, 0,
						cueTip, 0/*m_strikeRnd*/,
						FALSE, TRUE);		
	///////////////////////////////////////////////////////////////////////////////////

	///////////////////////////////////////////////////////////////////////////////////
	// init domino ball to ball hit chain (start off chain with ball to be hit by cue)
	//ClearFlag(m_ball[m_dominoHitLast]->m_flags, Flg_BALL_DOMINOHITLAST); // MAYBE WE SHOULD CLEAR ALL BALLS, JUST TO BE SAFE!!!
	int n;
	for(n=0;n<MAX_TOTALBALLS;n++)
		ClearFlag(m_ball[n]->m_flags, Flg_BALL_DOMINOHITLAST);

	m_dominoHitLast = m_selectBall;//0;
	m_dominoHit1st = m_selectBall;// was 0;	
	SetFlag(m_ball[m_dominoHitLast]->m_flags, Flg_BALL_DOMINOHITLAST);
	///////////////////////////////////////////////////////////////////////////////////

	BOOL bEndOfSim = TRUE;
	while(bEndOfSim)
	{
		bEndOfSim = ProcessSnookerBalls(TRUE, bWhiteOnly);
	}

	return; 

#if 0
	m_bTestBallFinished = FALSE;


	//************************************************
	if(!m_bTestBallFinished)
	{
		//for(ittr = 0;ittr < ((GAME_ITTR)*60*3);ittr++)	// perform multiple processes per frame	// was v1.6
		for(ittr = 0;ittr < ((GAME_ITTR)*60*1);ittr++)	// perform multiple processes per frame	
		{
			// (1) process individual ball, that is selected
			m_testBall->Process(TRUE);

			// (2) ball2ball collisions
			for(m=1;m<MAX_TOTALBALLS;m++)
			{
				/*
				int ballOnColour = m_colourseq;
				if(ballOnColour == 0) ballOnColour = RED;

				if(balln != m)
				{
				// ignore RED balls (bugfix: getting FREEBALL when the reds are all blocking each other. i.e. starting pos.)
				if(m_ball[m]->m_colour == RED)
					continue;
				}
								
				// ignore colour that is ball-on
				//if(m_ball[m]->m_colour == ballOnColour)
				//	continue;
				
				// this is for freeball-snooker situations
				// do collisions...
				if((collideColour >= YELLOW) && (m_ball[m]->m_colour != collideColour) && (m_ball[m]->m_colour != ballOnColour))
				{
					// don't do collide with ball
				}
				else
				{
				*/
					// default
					if(m_testBall->CollideWithBall(m_ball[m], TRUE))
					{
						m_bTestBallFinished = TRUE;
						break; // m
					}
				//}
			}
			m_testBall->AntiProcess(); // update old coords

			// (3) end of test sim.
			if(m_bTestBallFinished)
			{
				break; // ittr
			}
			//-END-

			////////////////////////////////////////////////////////////////////
		}
	}
	//************************************************	

	// did we hit something?
	if(m_bTestBallFinished)
	{
		return m;
	}

	return -1;

#endif //0
}	   


void CSnooker::HitCushionBallsInit(void)
{
	// clear all the hitCushion flags for all the balls
	for(int n=0;n<MAX_TOTALBALLS;n++)
	{
		m_ball[n]->m_hitCushion = 0;
		m_ball[n]->m_bHitCushionBeforeBall = FALSE;
		m_ball[n]->m_cyc = 0; // *NEW - garantee all balls have their cyc reset to 0, before the next strike takes place
	}
}

#ifdef FIXEDMODE
BOOL CSnooker::bIsStrikeBallOnTopOfAnother(int ballIdx, int excludeBallIdx)
{
	CBall *ballptr = NULL;

	if(ballIdx == -1) ballptr = (CBall *)m_testBall;
	else ballptr = (CBall *)m_ball[ballIdx];
		
	__fix f1, f2;
	FVector P;
	int m;
	for(m=1;m<MAX_TOTALBALLS;m++)
	if(TestFlag(m_ball[m]->m_flags, Flg_BALL_ACTIVE))
	if(m!=ballIdx)
	if(m!=excludeBallIdx)
	{
		// have we collided?
		P = ballptr->f_P - m_ball[m]->f_P;

		f1 = P.Magnitude2();
		f2 = TOFIX(BALL_RADIUS*2);
		f2 = FIXMUL(f2, f2);
		if(f1 < f2)
		{
		//	char buf[256];
		//	sprintf(buf, "ballIdx = %d, m = %d", ballIdx, m);
		//	ErrorMessage(g_wnd, buf);
			return TRUE;		
		}
	}

	return FALSE;
}
#else
BOOL CSnooker::bIsStrikeBallOnTopOfAnother(int ballIdx, int excludeBallIdx)
{
	CBall *ballptr = NULL;
	char omit;

	if(ballIdx == -1) ballptr = (CBall *)m_testBall;
	else ballptr = (CBall *)m_ball[ballIdx];

	
	if(g_build.gameType == GAMETYPE_SNOOKER && m_customTable->m_type == CTTYPE_BILLIARDS)
		{
		if(m_bTurn1P) omit = 0;	// SKIP the WHITE //
			else omit = 1;	// SKIP the YELLOW //
		}
	else omit = 0;	// SKIP the WHITE


	float f1, f2;
	CVector P;
	int m;
	for(m=0;m<MAX_TOTALBALLS;m++)
	if(m!=omit)
	if(TestFlag(m_ball[m]->m_flags, Flg_BALL_ACTIVE))
	if(m!=ballIdx)
	if(m!=excludeBallIdx)
	{
		// have we collided?
		P = ballptr->m_P - m_ball[m]->m_P;

		f1 = P.Magnitude2();
		f2 = BALL_RADIUS*2;
		f2 = f2 * f2;
		if(f1 < f2)
		{
		//	char buf[256];
		//	sprintf(buf, "ballIdx = %d, m = %d", ballIdx, m);
		//	ErrorMessage(g_wnd, buf);
			return TRUE;		
		}
	}

	return FALSE;
}
#endif // FIXEDMODE


void CSnooker::ProcessBalls()
{
	
	if(g_gameAppView == NULL) return;

	BOOL bActive = FALSE;
	VOut[0] = m_ball[0]->m_V;


	if(m_firstTime)
	{
		g_gameAppView->m_elCycle = EL_REST|EL_BALLS;
	}

	if(g_gameAppView->m_elCycle & EL_REST);
	else
	{
		
		if(g_gameAppView->m_elCycle & EL_BALLS) 
		{
			ProcessSnookerBalls();
		}


		m_firstTime = false;
		if(m_firstFewFrames) m_firstFewFrames--;
		return;
	}

	if(m_cueBallSelect == 2 || m_firstFewFrames > 0)
	{
		if(((CGameAppView *)m_parent)->m_elCycle&EL_BALLS) 
		{
			ProcessSnookerBalls(); // mode 2 is everything but balls
		}
	}
	
}

void CSnooker::Process(float timeDelta)
{
	if(g_gameAppView == NULL) return;

	m_timeDelta = timeDelta;

	m_rnd = rand();
	
	int n;
	int m;
	int ittr;
	bool bActive = false;
	
	/////////////////////////////////////////////////////////////////////////////////////////
	// check if this current game is over	
	if(g_build.gameType == GAMETYPE_SNOOKER)
	{
		// check if it's conceder's turn
		if(m_cueBallSelect != 4) // stop gameover being displayed 'twice'
		if(m_bConcededGame)
		if(m_gameOver)
		{

			if(m_bTurn1P)
			{
				if(m_gameOver == 1)	
					{
					m_bConcededGame = FALSE;
					}
			}
			else
			{
				if(m_gameOver == 2)	
					{
					m_bConcededGame = FALSE;
					}
			}
		}
	}
	else if(g_build.gameType == GAMETYPE_POOL)
	{
		// check if it's conceder's turn before allowing gameover to continue
		if(m_cueBallSelect != 4) // stop gameover being displayed 'twice'
		if(m_bConcededGame)
		if(m_gameOver)
		{
			
			if(m_bTurn1P)
			{
				if(m_gameOver == 1)
					m_bConcededGame = FALSE;
			}
			else
			{
				if(m_gameOver == 2)
					m_bConcededGame = FALSE;
			}

			if(m_customTable->m_type == CTTYPE_BOWLS) // BUGFIX: clear 'm_bConcededGame' flag if this is a BOWLS type game
				m_bConcededGame = FALSE;

		}
	}


	// make a note the first time gameover goes non-zero (to stop a player losing a won/conceeded game)
	if(m_gameOver)
	if(m_gameOverIWon == 0)
	{
		// check to see if 'I' have won? (note: should only ever get in here once per frame/rack)
		if(GAMEAPPVIEW->m_lobby->IsPlayerWinner(GAMEAPPVIEW->m_playerSockIdx))
			m_gameOverIWon = 1; // i'm the winner :)
		else
			m_gameOverIWon = -1; // i'm the loser :(
	}
 
	
	// GAME OVER!!!...
	if(m_cueBallSelect != 4) // stop gameover being displayed 'twice'
	if(!m_bConcededGame)
	if(m_gameOver)
	{
		int tblIdx = GAMEAPPVIEW->m_lobby->GetTableIdxFromPlayer(GAMEAPPVIEW->m_playerSockIdx);
		if(tblIdx!=-1)
		{
			// [If got his far, we assume that this is our TRUE winner]

			// make sure the first gameOver winner wins! (Not the the later gameOvers)
			int plrIdx = GAMEAPPVIEW->m_lobby->GetPlayerIdx(GAMEAPPVIEW->m_playerSockIdx);
			if(plrIdx==-1) return;

			if(!strcmpi(m_player[plrIdx].name, m_table[tblIdx].P1StartName)) m_gameOver = 2; // 1P wins
				else m_gameOver = 1; // 2P wins

			// If i'm the winner, send gameover msg to all
			if(GAMEAPPVIEW->m_lobby->IsPlayerWinner(GAMEAPPVIEW->m_playerSockIdx))
				{
				GAMEAPPVIEW->SendGameOverForAll(tblIdx, m_gameOver);
				}
		}
		else if(GAMEAPPVIEW->m_bPlayOffline)
		{
			char _buffer[MAX_CHARS];
			sprintf(_buffer,"*** Game Over! ***");
			GAMEAPPVIEW->Message(_buffer,10); // personal message
	
			if(m_gameOver == 1)
			{
				sprintf(_buffer,"*** Player2 wins! ***");
				GAMEAPPVIEW->Message(_buffer,10); // personal message
			}
			else if(m_gameOver == 2)
			{
				sprintf(_buffer,"*** Player1 wins! ***");
				GAMEAPPVIEW->Message(_buffer,10); // personal message
			}
		}

		#ifdef THE_POOL_CLUB
		// Make sure the sponsor text is off //
		// This is for the 2 Shot / 1 Shot msg for English Pool //
		g_gameAppView->m_sponsortxt[0] = 0;
		g_gameAppView->m_sponsoralpha = 0;
		#endif	

		m_gameOver = 0;
		m_gameOverIWon = 0;//-1;
	}
	/////////////////////////////////////////////////////////////////////////////////////////

	// keep an eye on table start messages
	//TableStartMessages(m_bPracticeMode);	

	// check if opponent fouled!
#ifndef THE_POOL_CLUB
	if(m_cueBallSelect != 4) // stop foul msgs being displayed 'twice'
	if(TestFlag(m_foul, Flg_FOUL_NORMAL))
		//if(IsSitting()) // ensure player is sitting at table
		{
			ClearFlag(m_foul, Flg_FOUL_NORMAL);

			///////////////////////////////////
			// display system msg
			int tbl = GAMEAPPVIEW->m_lobby->GetTableIdxFromPlayer(GAMEAPPVIEW->m_playerSockIdx);
			if(tbl!=-1)
			{
				int plr1Idx = GAMEAPPVIEW->m_lobby->GetPlayerIdx(m_table[tbl].player1SockIdx);
				if(plr1Idx == -1) goto endsystemmsg;
				int plr2Idx = GAMEAPPVIEW->m_lobby->GetPlayerIdx(m_table[tbl].player2SockIdx);
				if(plr2Idx == -1) goto endsystemmsg;
				
				char sfoulType[MAX_CHARS];
				if(TestFlag(m_foul, Flg_FOUL_FREEBALL) && TestFlag(m_foul, Flg_FOUL_MISS))
					sprintf(sfoulType," [ Free-Ball / Miss-Rule]");
				else if(TestFlag(m_foul, Flg_FOUL_FREEBALL))
					sprintf(sfoulType," [ Free-Ball ]");
				else if(TestFlag(m_foul, Flg_FOUL_MISS))
					sprintf(sfoulType," [ Miss-Rule ]");
				else
					sprintf(sfoulType,"");				

				char _buffer[MAX_CHARS];
				if(m_table[tbl].doubles)
					{
					if(m_bTurn1P) sprintf(_buffer,"*** Team %s / %s have fouled!", m_table[tbl].P2StartName, m_table[tbl].P2bStartName);
						else sprintf(_buffer,"*** Team %s / %s have fouled!", m_table[tbl].P1StartName, m_table[tbl].P1bStartName);
					}
				else{
					if(m_bTurn1P) sprintf(_buffer,"*** %s has fouled!", m_player[plr2Idx].name);
						else sprintf(_buffer,"*** %s has fouled!", m_player[plr1Idx].name);
					}
				
					GAMEAPPVIEW->Message(_buffer,-10); // system message
				sprintf(_buffer,"%s", sfoulType);
				GAMEAPPVIEW->Message(_buffer,-80); // system message
				sprintf(_buffer," ***");
				GAMEAPPVIEW->Message(_buffer,10); // system message

				if(TestFlag(m_foul, Flg_FOUL_FREEBALLSNOOKER)) // *NEW*
				{
					sprintf(_buffer,"NOTE:- not allowed to play a snooker behind a 'freeball', unless only Pink and Black remain.");
					GAMEAPPVIEW->Message(_buffer,10); // system message
				}

				if(TestFlag(m_foul, Flg_MustPlayDownTable)) // *NEW*
				{
				sprintf(_buffer, "Ball-In-Hand. You must play down the table for a Legal Shot");
				GAMEAPPVIEW->Message(_buffer, 40);
				}

				/*
				else if(TestFlag(m_foul, Flg_FOUL_PUSH))
				{
					if(m_bTurn1P)
						sprintf(_buffer,"(%s played a "Push Shot" on the %s ball)", m_player[plr2Idx].name, sColour);
					else
						sprintf(_buffer,"(%s played a "Push Shot" on the %s ball)", m_player[plr1Idx].name, sColour);
					GAMEAPPVIEW->Message(_buffer,10); // system message
				}
				*/
			}
			else if(GAMEAPPVIEW->m_bPlayOffline)
			{
				char sfoulType[MAX_CHARS];
				if(TestFlag(m_foul, Flg_FOUL_FREEBALL) && TestFlag(m_foul, Flg_FOUL_MISS))
					sprintf(sfoulType," [ Free-Ball / Miss-Rule]");
				else if(TestFlag(m_foul, Flg_FOUL_FREEBALL))
					sprintf(sfoulType," [ Free-Ball ]");
				else if(TestFlag(m_foul, Flg_FOUL_MISS))
					sprintf(sfoulType," [ Miss-Rule ]");
				else
					sprintf(sfoulType,"");				

				char _buffer[MAX_CHARS];
	
				if(m_bTurn1P) sprintf(_buffer,"*** Player2 has fouled!");
				else sprintf(_buffer,"*** Player1 has fouled!");
				
				GAMEAPPVIEW->Message(_buffer,-10); // system message
				sprintf(_buffer,"%s", sfoulType);
				GAMEAPPVIEW->Message(_buffer,-80); // system message
				sprintf(_buffer," ***");
				GAMEAPPVIEW->Message(_buffer,10); // system message
				
				if(TestFlag(m_foul, Flg_FOUL_FREEBALLSNOOKER)) // *NEW*
				{
					sprintf(_buffer,"NOTE:- not allowed to play a snooker behind a 'freeball', unless only Pink and Black remain.");
					GAMEAPPVIEW->Message(_buffer,10); // system message
				}

				if(TestFlag(m_foul, Flg_MustPlayDownTable)) // *NEW*
				{
				sprintf(_buffer, "Ball-In-Hand. You must play down the table for a Legal Shot");
				GAMEAPPVIEW->Message(_buffer, 40);
				}
			}

			endsystemmsg:;
			///////////////////////////////////

			if(MyTurn())
			{
				m_foulDlg.DestroyWindow();
				
				if(TestFlag(m_foul, Flg_FOUL_MISS))
				{
					m_foulDlg.Create(IDD_FOULMISSDLG);
					//m_foulDlg.ShowWindow(SW_SHOW);
					//m_foulEditDlg.ShowWindow(SW_SHOW);

					// "Foul and a Miss!"
					m_foulDlg.m_staticText.SetWindowText("     Foul and a Miss!");
				}
				else
				{
					m_foulDlg.Create(IDD_FOULDLG);	
					//m_foulDlg.ShowWindow(SW_SHOW);
					//m_foulEditDlg.ShowWindow(SW_SHOW);

					if(TestFlag(m_foul, Flg_FOUL_FREEBALL))
					{
						// "Foul and a Free-Ball!"
						m_foulDlg.m_staticText.SetWindowText("Foul and a Free-Ball!");		
					}				
					else
					{	
						// "Foul Shot!"
						m_foulDlg.m_staticText.SetWindowText("          Foul Shot!");
					}
				}
				
				

				//m_bFoul = FALSE;
				//MessageBox(0, "14","14",MB_OK);
				m_bFoulDecideTurn = TRUE;
				m_bFoulShowDialog = TRUE;
			}
			
		}
#else
	// THE_POOL_CLUB:
	if(m_cueBallSelect != 4) // stop foul msgs being displayed 'twice'
	{
		if(TestFlag(m_foul, Flg_FOUL_NORMAL))
			{
			ClearFlag(m_foul, Flg_FOUL_NORMAL);
			if(m_customTable->m_type != CTTYPE_ROTATIONBALL && m_customTable->m_type != CTTYPE_BREAK && m_customTable->m_type != CTTYPE_UKPOOL)
				{
				SetFlag(m_foul, Flg_FOUL_BALLINHAND);
				}
			
			
			// if there's been a foul!, dont allow push-out anymore
			ClearFlag(m_foul, Flg_FOUL_ALERTPUSHOUT);
			ClearFlag(m_foul, Flg_FOUL_ALERTPUSHOUT2);

			///////////////////////////////////
			// display system msg
			int tbl = GAMEAPPVIEW->m_lobby->GetTableIdxFromPlayer(GAMEAPPVIEW->m_playerSockIdx);
			if(tbl!=-1)
				{
				int plr1Idx = GAMEAPPVIEW->m_lobby->GetPlayerIdx(m_table[tbl].player1SockIdx);
				if(plr1Idx == -1) goto endsystemmsg;
				int plr2Idx = GAMEAPPVIEW->m_lobby->GetPlayerIdx(m_table[tbl].player2SockIdx);
				if(plr2Idx == -1) goto endsystemmsg;
				
				char sfoulType[MAX_CHARS];

				if((m_customTable->m_type == CTTYPE_ROTATIONBALL) || (m_customTable->m_type == CTTYPE_BREAK) )
					sprintf(sfoulType," [ Play-Or-Skip ]");
				else if(!TestFlag(m_foul, Flg_FOUL_GAMEOVER))	// Only Add Info about receiving playing's turn if not Game Over //
					{
					// If it's iPool, English Pool, then award 2 shots + ball in hand //
					if( (g_build.gameType == GAMETYPE_POOL) && (m_customTable->m_type == CTTYPE_UKPOOL) )
						{
						if(!TestFlag(m_foul, Flg_FOUL_BREAK)) sprintf(sfoulType," [ Opponent receives 2 Shots ]");
							else sprintf(sfoulType," [ Opponent receives 2 Shots behind the Break-Line ]");
						}
					else if( (g_build.gameType == GAMETYPE_POOL) && (m_customTable->m_type == CTTYPE_8BALL) )
						{
						if(!TestFlag(m_foul, Flg_FOUL_BREAK)) sprintf(sfoulType," [ Opponent receives Ball-In-Hand Anywhere ]");
							else sprintf(sfoulType," [ Opponent receives Ball-In-Hand behind the Break-Line ]");
						}
					else{
						sprintf(sfoulType," [ Opponent receives Ball-In-Hand Anywhere ]");
						}
					}
				else sfoulType[0] = 0;

				char _buffer[MAX_CHARS];
				if(m_table[tbl].doubles)
					{
					if(m_bTurn1P) sprintf(_buffer,"*** Team %s / %s have fouled!%s", m_table[tbl].P2StartName, m_table[tbl].P2bStartName, sfoulType);
						else sprintf(_buffer,"*** Team %s / %s have fouled!%s", m_table[tbl].P1StartName, m_table[tbl].P1bStartName, sfoulType);
					}
				else{
					if(m_bTurn1P) sprintf(_buffer,"*** %s has fouled!%s", m_player[plr2Idx].name, sfoulType);
						else sprintf(_buffer,"*** %s has fouled!%s", m_player[plr1Idx].name, sfoulType);
					}

				GAMEAPPVIEW->Message(_buffer,40); // system message

				
				if(TestFlag(m_foul, Flg_FOUL_NOTHITCUSHIONBREAK)) // *NEW*
					{
					sprintf(_buffer,"NOTE:- if you fail to pocket a ball on the break, you must drive at least four numbered balls to the rail.");
					GAMEAPPVIEW->Message(_buffer,10); // system message					
					}

				if(TestFlag(m_foul, Flg_FOUL_NOTHITCUSHION)) // *NEW*
					{
					sprintf(_buffer,"NOTE:- if you fail to pocket a ball, you must drive the cue ball or any numbered ball to a rail after the cue ball contacts the object ball.");
					GAMEAPPVIEW->Message(_buffer,10); // system message					
					}
	
				if((m_customTable->m_type == CTTYPE_ROTATIONBALL) || (m_customTable->m_type == CTTYPE_BREAK) )
					{
					sprintf(_buffer,"(play from this position OR put opponent back in)");
					GAMEAPPVIEW->Message(_buffer,10); // system message
					}
				else if((m_customTable->m_type != CTTYPE_UKPOOL))
					{
					sprintf(_buffer,"(click 'Right-Mouse' button to place white-ball)");
					GAMEAPPVIEW->Message(_buffer,10); // system message
					}

				/*
				if(m_customTable->m_type == CTTYPE_UKPOOL)
				if(TestFlag(m_foul, Flg_FOUL_ALERTUKPOOLFOUL) )
				if(m_shotcount>1)
					{
					ClearFlag(m_foul, Flg_FOUL_BALLINHAND); // Don't Set the Ball In Hand Yet //
					m_bBallInHand = 0;
					if(m_bTurn1P) sprintf(_buffer,"%s has fouled, %s is awarded with 2 Turns / Ball-In-Hand", m_player[plr2Idx].name, m_player[plr1Idx].name);
						else sprintf(_buffer,"%s has fouled, %s is awarded with 2 Turns / Ball-In-Hand", m_player[plr1Idx].name, m_player[plr2Idx].name);
					GAMEAPPVIEW->Message(_buffer,10); // system message
					}
				*/

				if( (m_customTable->m_type == CTTYPE_ROTATIONBALL) || (m_customTable->m_type == CTTYPE_9BALL) ||
					(m_customTable->m_type == CTTYPE_6BALL) || (m_customTable->m_type == CTTYPE_10BALL) )
					{
					

					char _buffer[MAX_CHARS];
					if(m_bTurn1P)
						{
						if(m_foul3Times2P == 2)
							{
							sprintf(_buffer,"WARNING! %s has fouled 2 consecutive times! (3 = loss)", m_player[plr2Idx].name);
							GAMEAPPVIEW->Message(_buffer,10); // system message
							}
						}
					else
					{
						if(m_foul3Times1P == 2)
						{
							sprintf(_buffer,"WARNING! %s has fouled 2 consecutive times! (3 = loss)", m_player[plr1Idx].name);
							GAMEAPPVIEW->Message(_buffer,10); // system message
						}
					}
				}
			}
			else if(GAMEAPPVIEW->m_bPlayOffline)
			{
				char sfoulType[MAX_CHARS];
				
				//if(TestFlag(m_foul, Flg_FOUL_FREEBALL))
				//	sprintf(sfoulType," [ Free-Ball ]");
				//else
				//	sprintf(sfoulType,"");
				if((m_customTable->m_type == CTTYPE_ROTATIONBALL) || (m_customTable->m_type == CTTYPE_BREAK) )
					sprintf(sfoulType," [ Play-Or-Skip ]");
				else if(!TestFlag(m_foul, Flg_FOUL_GAMEOVER))	// Only Add Info about receiving playing's turn if not Game Over //
					{
					// If it's iPool, English Pool, then award 2 shots + ball in hand //
					if( (g_build.gameType == GAMETYPE_POOL) && (m_customTable->m_type == CTTYPE_UKPOOL) )
						{
						if(!TestFlag(m_foul, Flg_FOUL_BREAK)) sprintf(sfoulType," [ Opponent receives 2 Shots ]");
							else sprintf(sfoulType," [ Opponent receives 2 Shots behind the Break-Line ]");
						}
					else if( (g_build.gameType == GAMETYPE_POOL) && (m_customTable->m_type == CTTYPE_8BALL) )
						{
						if(!TestFlag(m_foul, Flg_FOUL_BREAK)) sprintf(sfoulType," [ Ball-In-Hand Anywhere ]");
							else sprintf(sfoulType," [ Ball-In-Hand behind the Break-Line ]");
						}
					else{
						sprintf(sfoulType," [ Ball-In-Hand Anywhere ]");
						}
					}
				else sfoulType[0] = 0;

				char _buffer[MAX_CHARS];
				if(m_bTurn1P)
					sprintf(_buffer,"*** Player2 has fouled!%s ***", sfoulType);
				else
					sprintf(_buffer,"*** Player1 has fouled!%s ***", sfoulType);

				GAMEAPPVIEW->Message(_buffer,40); // system message

				if(TestFlag(m_foul, Flg_FOUL_NOTHITCUSHIONBREAK)) // *NEW*
				{
					sprintf(_buffer,"NOTE:- if you fail to pocket a ball on the break, you must drive at least four numbered balls to the rail.");
					GAMEAPPVIEW->Message(_buffer,10); // system message					
				}

				if(TestFlag(m_foul, Flg_FOUL_NOTHITCUSHION)) // *NEW*
				{
					sprintf(_buffer,"NOTE:- if you fail to pocket a ball, you must drive the cue ball or any numbered ball to a rail after the cue ball contacts the object ball.");
					GAMEAPPVIEW->Message(_buffer,10); // system message					
				}

				if((m_customTable->m_type == CTTYPE_ROTATIONBALL) || (m_customTable->m_type == CTTYPE_BREAK) )
				{
					sprintf(_buffer,"(play from this position OR put opponent back in)");
					GAMEAPPVIEW->Message(_buffer,10); // system message
				}
				else if((m_customTable->m_type != CTTYPE_UKPOOL))
				{
					sprintf(_buffer,"(click 'Right-Mouse' button to place white-ball)");
					GAMEAPPVIEW->Message(_buffer,10); // system message
				}

		
				if( (m_customTable->m_type == CTTYPE_ROTATIONBALL) || (m_customTable->m_type == CTTYPE_9BALL) ||
					(m_customTable->m_type == CTTYPE_6BALL) || (m_customTable->m_type == CTTYPE_10BALL) )
				{
					

					char _buffer[MAX_CHARS];
					if(m_bTurn1P)
					{
						if(m_foul3Times2P == 2)
						{
							sprintf(_buffer,"WARNING! Player2 has fouled 2 consecutive times! (3 = loss)");
							GAMEAPPVIEW->Message(_buffer,10); // system message
						}
					}
					else
					{
						if(m_foul3Times1P == 2)
						{
							sprintf(_buffer,"WARNING! Player1 has fouled 2 consecutive times! (3 = loss)");
							GAMEAPPVIEW->Message(_buffer,10); // system message
						}
					}
				}
			}

			endsystemmsg:;
			///////////////////////////////////
		
				
		 if(MyTurn())
			{				
				if((m_customTable->m_type == CTTYPE_ROTATIONBALL) || (m_customTable->m_type == CTTYPE_BREAK) )
				{
					m_foulDlg.DestroyWindow();
					m_foulDlg.Create(IDD_FOULDLG);					
					// "Foul Shot!"
					m_foulDlg.m_staticText.SetWindowText("          Foul Shot!");

					goto _endAlert;													
				}								
				else if((m_customTable->m_type == CTTYPE_UKPOOL))
				{
					
					m_foulDlg.DestroyWindow();
					if(TestFlag(m_foul, Flg_FOUL_ALERTUKPOOLFOUL))
						{
						m_foulDlg.DestroyWindow();
						m_foulDlg.Create(IDD_FOULUKDLG);					
						
						// "Foul Shot!"
						m_foulDlg.m_staticText.SetWindowText("          Foul Shot!");
						goto _endAlert;													
						}
					
					
					
				}		
		 }
			
		}

	// NEW CODE //
		
		if(TestFlag(m_foul, Flg_FOUL_ALERT))
		{
			ClearFlag(m_foul, Flg_FOUL_ALERT);

			if(MyTurn())
			{
				if(TestFlag(m_foul, Flg_FOUL_NOTHITCUSHIONBREAK))
				{
					m_foulDlg.DestroyWindow();
					m_foulDlg.Create(IDD_FOULBADBREAKDLG);
					
					m_foulDlg.m_staticText.SetWindowText("");

					goto _endAlert;
				}				

				m_foulDlg.DestroyWindow();
				m_foulDlg.Create(IDD_FOULALERTDLG);
				//m_foulEditDlg.ShowWindow(SW_SHOW);

				if(TestFlag(m_foul, Flg_FOUL_ALERTPUSHOUT))
				{
					// "Push-out after break"
					//m_foulDlg.m_staticText.SetWindowText("Foul and a Free-Ball!");
					m_foulDlg.m_staticText.SetWindowText("    Push-Out Shot?");					
				}
				else if(TestFlag(m_foul, Flg_FOUL_ALERTPUSHOUT2))
				{
					// "Push-out put back-in"
					m_foulDlg.m_staticText.SetWindowText("Put opponent back in?");					
				}
				else if(TestFlag(m_foul, Flg_FOUL_ALERTPOT8ONBREAK))
				{
					// "8-Ball Potted on Break! Rerack?"
					m_foulDlg.m_staticText.SetWindowText("8-Ball Potted on Break! \nRerack?");
				}
				
				else if(TestFlag(m_foul, Flg_FOUL_ALERTUKPOOLFOUL))
				{
					// "UK Pool Foul"
					m_foulDlg.m_staticText.SetWindowText("You have been awarded with 2 Turns\nDo you wish to play from Behind-The-Baulk line?");
				}
				

_endAlert:;
				//m_bFoul = FALSE;
				//MessageBox(0, "14","14",MB_OK);
				m_bFoulDecideTurn = TRUE;
				m_bFoulShowDialog = TRUE;
				
			}

			//ClearFlag(m_foul, Flg_FOUL_ALERTPUSHOUT);
			//SetFlag(m_foul, Flg_FOUL_ALERTPUSHOUT2);
		}
	}
#endif //!THE_POOL_CLUB

	
//#ifdef THE_POOL_CLUB
	// *** shot timer:	
	if(m_cueBallSelect == 0 || m_cueBallSelect == 1 || m_cueBallSelect == 5)
	{
		int table = GAMEAPPVIEW->m_lobby->GetTableIdxFromPlayer(GAMEAPPVIEW->m_playerSockIdx);
		if(table!=-1)
		{
			// setup game timer:
			if(m_table[table].addTimer)
			if(!m_bPracticeMode && GAMEAPPVIEW->m_lobby->GameStarted(GAMEAPPVIEW->m_playerSockIdx))
			{
				if( MyTurn() )
				{			// -ve            // +ve                  //-ve             -ve
					if( (m_shotTime <= 0 && m_shotTimeOld > 0) || (m_shotTime < 0 && m_shotTimeOld < 0) )
					if(!m_bShotTimeUp)
					{
						m_bShotTimeUp = TRUE;
						GAMEAPPVIEW->SendPlayerTimeUp(TRUE);
					}
				}
				//m_shotTime--;
				m_shotTimeOld = m_shotTime;
				m_shotTime -= (g_cyc - g_cycOld)/20; //(/1000*500)				
			}

			////
			// if player takes too long over shot, opponent can force a forfit
			if(!m_gameOver)
			if(!m_bPracticeMode && GAMEAPPVIEW->m_lobby->GameStarted(GAMEAPPVIEW->m_playerSockIdx))
			{			
				int tblIdx = GAMEAPPVIEW->m_lobby->GetTableIdxFromPlayer(GAMEAPPVIEW->m_playerSockIdx);

				// if i'm waiting for oppoent to play their shot AND my opponent hasn't lost connection (*)
				if(GAMEAPPVIEW->m_lobby->PlayingAGame(GAMEAPPVIEW->m_playerSockIdx))
				if( !MyTurn() )
				if(!m_table[tblIdx].bLockSeats) // ^(*)
				{
					// if time is up!
					if(m_MaxShotTime <= 0 && m_MaxShotTimeOld > 0)
					{
						// tell all at this table, time's up!
						GAMEAPPVIEW->SendPlayerMaxTimeUp(GAMEAPPVIEW->m_playerSockIdx);

						// bring-up force forfeit dialog!
						GAMEAPPVIEW->m_forfeitDlg.m_forfeitType = 1;
						GAMEAPPVIEW->m_forfeitDlg.DestroyWindow();
						GAMEAPPVIEW->m_forfeitDlg.Create(IDD_FORFEIT2DLG);
						GAMEAPPVIEW->m_forfeitDlg.ShowWindow(SW_SHOW);
						//GAMEAPPVIEW->SetFocus();
						GAMEAPPVIEW->PreviousWndEditSetFocus();
					}

					m_MaxShotTimeOld = m_MaxShotTime;
					m_MaxShotTime -= (g_cyc - g_cycOld)/20;			
				}
			}
			////
			//g_cycOld = g_cyc; <-- was here

		}

		if(!m_bPracticeMode)
			if( MyTurn() )
				//if(m_cueBallSelect != 0)
					m_bioShotTime += (g_cyc - g_cycOld)/20;

		g_cycOld = g_cyc;
	}
	// *** //////////
//#endif //THE_POOL_CLUB



	// delay to allow other players to see the cuetip data update - 20 frames to see it!
//	if( !MyTurn() )
	if((m_cueBallSelect >= 20) || m_bStrikeBallReady)
	{
//				
		// just before strike, destroy the force forfit MaxTimeUp dlg!
		if(GAMEAPPVIEW->m_forfeitDlg.m_hWnd)
		if(GAMEAPPVIEW->m_forfeitDlg.IsWindowEnabled())
		if(GAMEAPPVIEW->m_forfeitDlg.m_forfeitType == 1)
		{
			GAMEAPPVIEW->m_forfeitDlg.DestroyWindow();
		}
		
		m_bStrikeBallReady = FALSE;



		// MSGGAME_SHOT FIRED
		m_cueBallSelect = 40; 
		
		if(m_cueBallSelect == 40)
		{
			m_cueBallSelect = 2;
			//if( !MyTurn() )
			//{
			m_bCueState = FALSE;	
			
			HitCushionBallsInit();
			StrikeBall();

			m_sightlineFade = 60;
			m_cueStrikeTime = CUE_DELIVERY_TIME;
			m_cueStrikeSpeed = CUE_STRIKEY_MAXSPEED * (m_cuePowerPercentage/100.0f);
			m_cueStrikeDist = 0.0f;

			// If TV Modes, Then PROCESS CAMERA VIEWS //
			if(g_gameAppView->m_GFXMode >= 2) 
				{
				PROCESS_CAMERA_TV();
				g_gameAppView->m_bOnSize = TRUE;
				g_gameAppView->m_bOnSizeDrawTmpSurf = TRUE;
				}

		}
	}

	// Do RaiseButt Helper *************************************************
	if(MyTurn())
	if(m_cueBallSelect == 5)
	{
		MiniCameraViewUpdate(); // needed to fix zoom-in bug in pool game
		m_bReceiveRaiseButtActive = TRUE; // **		

		if(m_bRaiseButtHelper)
		{	
			m_bRaiseButtHelper = FALSE;

			////// raise the butt if player is completely blocked [see ODIN]
			int loop = 35;//25;//18;
			m_RaiseButt = 0;
			m_RaiseButtOld = 0;
			GAMEAPPVIEW->m_sliderRaiseButt.SetPos(90 - m_RaiseButt);

			while(loop > 0)
			{
				if(!HamperedCueing(m_mwindowWidth / 2, 7, m_mwindowWidth, m_mwindowHeight))
				//if(!HamperedCueing(0, 10, m_mwindowWidth, m_mwindowHeight))
					break;
				
				m_RaiseButt += 1;//+= 5;
				GAMEAPPVIEW->m_sliderRaiseButt.SetPos(90 - m_RaiseButt);

				//if(m_RaiseButtOld != m_RaiseButt)
				//m_bReceiveRaiseButtActive = TRUE; // ** moved this to above
				MiniCameraPlayerRaiseButt();
				m_RaiseButtOld = m_RaiseButt;

				loop--;
			}
			//////
		}
	}
	// *********************************************************************

	//m_targetBall = -1;	// clear target ball
	//m_targetPocket = -1; // clear pocket select

	//////////////////////////////////////////////////////////////////////////////////////////////////
	// do Smart-Pocket Target code... [*NEW*]
	if(m_bCallShot)
	if(MyTurn())
	if(m_cueBallSelect == 5)
	if(m_customTable->m_type != CTTYPE_BOWLS) // dont do for BOWLS!
	{		
		// if we dont already have a target pocket:
		if(m_targetPocket == -1)
		{
			int newTargetPocket = -1;
			int newTargetPocketLast = -1;
			int newTargetBall = -1;

			// get object ball Pos, that testBall(White) has hit
			CVector P = m_TestBallCollideObjPos;
			
			// get object ball's velocity, that testBall(White) has collided with
			CVector V = m_TestBallCollideVB.Unit() * (BALL_RADIUS*2);

			int safetyCount = 0;
			BOOL bHitBallOrPocket = FALSE;
			do {

//				{
//					char buf[256];
//					sprintf(buf, "V.x = %f, V.y = %f, V.z = %f", V.x, V.y, V.z);
//					ErrorMessage(m_parent, buf);
//				}			 

				// 1 - check if we are on top of another ball (except WhiteBall)
				for(int m=1;m<MAX_TOTALBALLS;m++)
				if(m_ball[m]->m_colour != m_TestBallCollideObjColour) // dont include object ball that testBall collided with.
				if(m_ball[m]->Proximity(P.x, P.y, 0, BALL_RADIUS*2.0f))
				{
					bHitBallOrPocket = TRUE;
					newTargetBall = m;
				}
								
				// 2 - check if we are on top of the pocket
				//newTargetPocket = PocketProximity(P.x, P.y, P.z, 0/*BALL_RADIUS*/);
				newTargetPocket = PocketProximity(P.x, P.y, P.z, BALL_RADIUS*2);
				//if(newTargetPocket != -1)
				//{
				//	bHitBallOrPocket = TRUE;
				//}
				if(newTargetPocket != -1) // keep going... we may hit another pocket i.e. shots along the rails past the middle pocket to far corner pocket
					newTargetPocketLast = newTargetPocket;

				// move on to next position...
				if(!bHitBallOrPocket)				
					P = P + V;

//				m_ball[0]->m_P = m_ball[0]->m_POld = P;
//				m_ball[0]->UpdateFixeds();
//				bHitBallOrPocket = TRUE;
			}
			while(!bHitBallOrPocket && safetyCount++ < 100);

			newTargetPocket = newTargetPocketLast;

			// auto - update target pocket indicator			
			if(newTargetPocket != -1)
				m_targetPocket = newTargetPocket;
			else
				m_targetPocket = -2;
			
			if(m_colourseq == 0) // dont do if on break
				m_targetPocket = -1;
			

			// auto - update target ball indicator
			if(newTargetBall != -1)
				m_targetBall = newTargetBall;
			else
			{				
				// find the testball that white first hit!
				for(int m=1;m<MAX_TOTALBALLS;m++)
				if(m_ball[m]->m_colour == m_TestBallCollideObjColour)
				{
					m_targetBall = m;
					break;
				}				
			}

			if(m_colourseq == 0) // dont do if on break
					m_targetBall = -1;
			
			if(m_testBall->m_colour == WHITE) // dont do if white-ball did not hit another ball
					m_targetBall = -1;


			if(m_targetPocket != -1 || m_targetBall != -1)
			{
				// send target ball/pocket/success data to all at table
				GAMEAPPVIEW->SendPlayerTargetData(m_targetPocket, m_targetBall, m_bTargetSuccess);
			}
		}
	}
	//////////////////////////////////////////////////////////////////////////////////////////////////


	int tblIdx = GAMEAPPVIEW->m_lobby->GetTableIdxFromPlayer(GAMEAPPVIEW->m_playerSockIdx);

	BOOL force_redraw = FALSE;


	// If the old Mode was just false, then we have just changed //
	if(g_pCameraMode == 0) 
	if(GAMEAPPVIEW->m_GFXMode == 3)	g_bCameraModeJustChanged = true;

	// Deafault make Camera Mode in 2D // 
	// Default is 2D //
	
	if(tblIdx != -1) g_pCameraMode = false;					// Fixed Camera by Default (when online)

	// TV mode - only for players that watcher (not playing the shot)
	
	//if(m_cueBallSelect == 2)	// During the Shot Only //
	
	if(GAMEAPPVIEW->m_GFXMode == 1)
	{
		if( !MyTurn() || m_cueBallSelect == 2) m_CameraMode = CAMERA_MODE_WATCH;
			else m_CameraMode = CAMERA_MODE_PLAYER_2D;
	}
	else if(GAMEAPPVIEW->m_GFXMode == 2) 
	{
		if( !MyTurn() || m_cueBallSelect == 2) m_CameraMode = CAMERA_MODE_TV;				// 2DTV
			else m_CameraMode = CAMERA_MODE_PLAYER_2D;
	}
	else if(GAMEAPPVIEW->m_GFXMode == 3) 
	{
		if( !MyTurn() )
		{
			m_CameraMode = CAMERA_MODE_3DFREE;
			if(tblIdx != -1) g_pCameraMode = true;		// 2D3DFree
		}
		else
		{
			m_CameraMode = CAMERA_MODE_PLAYER_2D;
			if(tblIdx != -1) g_pCameraMode = false;

		}
	}
	else
	{
		m_CameraMode = CAMERA_MODE_PLAYER_2D;
		if(tblIdx != -1) g_pCameraMode = false;
	}
		
	// Force ReDraw on next Render, if we are changing View //
	//if(m_CameraMode == CAMERA_MODE_PLAYER_2D) force_redraw = TRUE;

	//Force ReDraw is next render if we are changing view //
	if(m_CameraOldMode != m_CameraMode)
	{
		GAMEAPPVIEW->m_bUpdateTable = TRUE;
	}
	
	m_CameraOldMode = m_CameraMode;



	// If the game hasn't started, show the Camera in Demo Mode //
	// int tblIdx = GAMEAPPVIEW->m_lobby->GetTableIdxFromPlayer(GAMEAPPVIEW->m_playerSockIdx);
	if(tblIdx != -1)
	{
		if(!m_table[tblIdx].bGameStarted)
		{
			//m_CameraMode = CAMERA_MODE_DEMO;
			
			// We will pick a Random Position to View Table, which is changed each time we join the table //
			m_CameraMode = CAMERA_MODE_DEMO_FIXED;
		
		}
	}

	// move cuetip away, in mini3dcam
	if( MyTurn() )
	if((m_cueBallSelect == 0) || (m_cueBallSelect == 1))// || (m_cueBallSelect == 2))
	{
		m_cuetip[0] = 0;
		m_cuetip[1] = 0;
		m_cuetip[2] = 0;//1; ???? maybe get rid of this ???
		m_ballMousePos[0] = 0;
		m_ballMousePos[1] = 0;
		m_ballMousePos[2] = -20;

		if(m_CameraMode != CAMERA_MODE_PLAYER_2D)
		{
			m_CameraMode = CAMERA_MODE_PLAYER_2D;  // Should always be in 2D When my turn to play the shot //
			g_pCameraMode = false;
			g_gameAppView->m_bOnSize = TRUE;
			g_gameAppView->m_bOnSizeDrawTmpSurf = TRUE;
		}

	}
	if(g_gameAppView)
	if(m_cueBallSelect == 1 || g_gameAppView->m_fineTuner->tunerSliderMouseDown == true)
	{
		m_bCueTipReady = FALSE; // *NEW* - clear this everytime we set the line
		m_bCueTipReadyFirstTime = TRUE;
	
		// save dir. of cue strike				
		m_cueTipStrikeDir = CVector(m_sPoint[0] - m_tableMousePos[0],
									m_sPoint[1] - m_tableMousePos[1],
									m_sPoint[2] - m_tableMousePos[2]);

		/*

		char buf[80];
		sprintf(buf, "m_tableMousePos[0]: %f, m_tableMousePos[1]:%f", m_tableMousePos[0], m_tableMousePos[1]);
		ErrorMessage(m_parent, buf);

	*/



		float _strkRnd = 0;
		if( MyTurn() ) {

			/*
			char buf[80];
			sprintf(buf, "m_cueTipStrikeDir.x:%f, m_cueTipStrikeDir.y: %f", m_cueTipStrikeDir.x, m_cueTipStrikeDir.y);
			ErrorMessage(m_parent, buf);
*/
			m_cueAngle = atan2((double)-m_cueTipStrikeDir.y, (double)-m_cueTipStrikeDir.x);
			
			#ifdef SHAKE_LINE
			_strkRnd = GetStrikeBallError(TRUE);
			#endif
		}



		char proceed = 0;
		if(g_build.gameType == GAMETYPE_SNOOKER && m_customTable->m_type == CTTYPE_BILLIARDS)
			{
			if(m_ball[m_selectBall]->m_colour == WHITE || m_ball[m_selectBall]->m_colour == YELLOW) // Billiards Cue Ball
				proceed = 1;
			}
		else{
			if(m_ball[m_selectBall]->m_colour == WHITE) // cueball only
				proceed = 1;
			}
			

		if(proceed) 
		{
			// copy selected ball attributes into 'testball' attributes
			m_testBall->m_parent = m_ball[m_selectBall]->m_parent;
			m_testBall->m_flags = m_ball[m_selectBall]->m_flags;
			m_testBall->m_colour = m_ball[m_selectBall]->m_colour;
			m_testBall->m_idx = m_selectBall;
			m_testBall->m_ghost = m_ball[m_selectBall]->m_ghost;
			m_testBall->m_hitOtherBall = m_ball[m_selectBall]->m_hitOtherBall;
			m_testBall->m_P = m_ball[m_selectBall]->m_P;
			m_testBall->m_POld = m_ball[m_selectBall]->m_POld;
			m_testBall->m_V = m_ball[m_selectBall]->m_V;
			m_testBall->m_VOld = m_ball[m_selectBall]->m_VOld;
			m_testBall->m_W = m_ball[m_selectBall]->m_W;
			m_testBall->m_cyc = 0;

			HitCushionBallsInit();

			m_testBall->Strike(m_sPoint[0], m_sPoint[1], m_sPoint[2], 
								m_tableMousePos[0], m_tableMousePos[1], m_sPoint[2],
								m_cuetip, _strkRnd,
								TRUE);
			
			
			
			

			m_bTestBallFinished = FALSE;

			m_TestBallCollideObjColour = 0;

			#ifdef FIXEDMODE
			FVector StartPos, EndPos; // for recording the dist. between cueball and obj. ball
			EndPos = StartPos = m_testBall->f_P;

			// remember testball's initial strike velocity
			m_TestBallCollideVA.x = FROMFIXSMALL(m_testBall->f_V.x);
			m_TestBallCollideVA.y = FROMFIXSMALL(m_testBall->f_V.y);
			m_TestBallCollideVA.z = FROMFIXSMALL(m_testBall->f_V.z);

			#else
			CVector StartPos, EndPos; // for recording the dist. between cueball and obj. ball
			EndPos = StartPos = m_testBall->m_P;

			// remember testball's initial strike velocity
			m_TestBallCollideVA = m_testBall->m_V;

			#endif //FIXEDMODE

			if(!m_bTestBallFinished)
			{
				for(ittr = 0;ittr < ((GAME_ITTR)*30*1);ittr++)	// perform multiple processes per frame	
				{
					////////////////////////////////////////////////////////////////////
					// -START-
					// (1) process individual ball,` that is selected
					//for(n=0;n<MAX_TOTALBALLS;n++)
					//{
					  	if(m_testBall->Process(TRUE) == TRUE)
							bActive = TRUE;
					  
					//}

					// Actual Ball positions for rendering
					// (2) ball2ball collisions
						for(m=0;m<MAX_TOTALBALLS;m++)
							if(m_selectBall!=m)
							{
								//m_testBall->m_colour = 10;
								//if(m_testBall->CollideWithTestBall(m_ball[m]))
								if(m_testBall->CollideWithBall(m_ball[m], TRUE, FALSE, m))
								{
									//1 - m_testBall->m_colour;
									// collided!!!
									m_bTestBallFinished = TRUE;
									
									#ifdef FIXEDMODE
									EndPos = m_testBall->f_P;
									#else
									EndPos = m_testBall->m_P;
									#endif //FIXEDMODE
									
									break;
								}
							}
						m_testBall->AntiProcess(); // update old coords

					if(m_bTestBallFinished)
					{						
						if( MyTurn() )
						{
							// these 2 vars are used for calculating the length of the exit traj. line
							//m_strikeLine = GetStrikeBallError();							
							//m_strikeRnd = GetStrikeBallError(TRUE);
							m_strikeMaxRnd = GetStrikeBallError(FALSE); // [*NEW FOR POOL THICKNESS OF TRI-LINES]
							
							//char buff[80];
							//sprintf(buff, "Error = %f", m_testBallInitialSpeed);//m_strikeMaxRnd);
							//ErrorMessage(m_parent, buff);

							#ifdef FIXEDMODE
							FVector FDist = EndPos - StartPos;
							CVector Dist;
							Dist.x = FROMFIXSMALL(FDist.x);
							Dist.y = FROMFIXSMALL(FDist.y);
							Dist.z = FROMFIXSMALL(FDist.z);
							m_strikeDist = Dist.Magnitude();
							
							//{							
							//char buf[256];
							//sprintf(buf, "[FIX]m_strikeDist = %f", m_strikeDist);
							//ErrorMessage(m_parent, buf);
							//}

							#else
							CVector Dist = EndPos - StartPos;
							m_strikeDist = Dist.Magnitude();
							#endif // FIXEDMODE

						}

						break;
					}
					//-END-

					////////////////////////////////////////////////////////////////////
				}
			}

			if(!m_bTestBallFinished)
			{
				// reset E_Rnd input data, if testBall didn't hit an object ball
				m_strikeDist = 0;
				//m_TestBallCollideVA = CVector(0,0,0); // wrong, cos this is always present, as it's the white's velocity
				m_TestBallCollideVB = CVector(0,0,0);
			}
		}
	}
	else if(m_cueBallSelect == 2)//
	{
		// fix player's control to ONLY the view window
		//GAMEAPPVIEW->m_sendEdit.SetCapture();


		m_cyc++;
		
		//////////////////////////////////////////////////////////////////////////////////////
		// process cue-delivery

		if(m_cueStrikeTime)
		{
			m_cueStrikeTime--;
			if(m_cueStrikeTime < CUE_PULLBACK_TIME) //if(m_cueStrikeTime<65)
			{
				m_cueStrikeSpeed-=CUE_PULLBACK_SPEED; //m_cueStrikeSpeed-=1.0f;
				if(m_cueStrikeSpeed < -CUE_PULLBACK_MAXSPEED) //if(m_cueStrikeSpeed < -15.0f)
					m_cueStrikeSpeed = -CUE_PULLBACK_MAXSPEED; //m_cueStrikeSpeed = -15.0f;
			}
			else
			{
				m_cueStrikeSpeed *= CUE_STRIKE_SPEEDLOSS; //m_cueStrikeSpeed *= 0.92f;
			}
			
			m_cueStrikeDist += m_cueStrikeSpeed;
			if(  m_cueStrikeDist > (m_cuePowerPercentage/100.0f) * CUE_STRIKE_MAXDIST) //if(  m_cueStrikeDist > (m_cuePowerPercentage/100.0f) * (BALL_RADIUS*20.0f)  )
			{
				m_cueStrikeDist = (m_cuePowerPercentage/100.0f) * CUE_STRIKE_MAXDIST;
				m_cueStrikeSpeed = 0.0f;
			}
		}
	
		//////////////////////////////////////////////////////////////////////////////////////

		m_bCueBallHitCushion = FALSE; // for debug purposes only!
		
		//////////////////////////////////////////////////////////////////////////////////////////////////////
		//////////////////////////////////////////////////////////////////////////////////////////////////////
		// if(((CGameAppView *)m_parent)->m_elCycle&EL_BALLS) ProcessSnookerBalls(); // mode 2 is everything but balls
		//////////////////////////////////////////////////////////////////////////////////////////////////////
		//////////////////////////////////////////////////////////////////////////////////////////////////////
	}


	PROCESS_CAMERA_POSITION();
	

	// update AT & POS for minicamera view
	MiniCameraViewUpdate();

	// reset the mouse R button flag
	//((CGameAppView *)m_parent)->m_bRButtonDown = 0;

	//////////////////////////////////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////////////////////////////////
	// Snooker Environment SFX
	
	// play crowd cough!! sfx
	//if(!(rand()&255))
	//	Msfx_smpplay(&((CGameAppView *)m_parent)->m_game_smp[SFX_COUGH1]);
	
	// play tap on table sound option
	//if( ((CGameAppView *)m_parent)->m_bLButtonDown )
	//	Msfx_smpplay(&((CGameAppView *)m_parent)->m_game_smp[SFX_BALL2BALL]);

	//////////////////////////////////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////////////////////////////////
	m_process = FALSE;
	
	VOut[3] = m_ball[0]->m_V;//++

	m_firstTime=FALSE;

	
}

BOOL CSnooker::ProcessSnookerBalls(BOOL bBotSim, BOOL bWhiteOnly)
{
/*
	static float s_fCounter = 0.0f;
	s_fCounter += 0.07f;

	m_ball[5]->m_P.x = 1000.0f*sinf(s_fCounter * 0.1f);

	return true;
*/
	int ittr;
	int n;
	int m;
	int rollOff;
	WORD rollOffIdx = (double)(m_strikeRnd * 100000);
	BOOL bActive = FALSE;


		
//	if(rollOffIdx != 0)
//	{
//		char buf[256];
//		sprintf(buf, "rollOffIdx = %d; (m_strikeRnd = %f)", rollOffIdx, m_strikeRnd);
//		ErrorMessage(g_wnd, buf);
//	}
	


		//static float s_fCounter = 0.0f;
		//s_fCounter += 0.07f;


		//for(ittr = 0;ittr < (GAME_ITTR);ittr++)	// perform multiple processes per frame
		for(ittr = 0;ittr <1;ittr++)	// perform multiple processes per frame
		//for(ittr = 0;ittr <4;ittr++)	// perform multiple processes per frame
		{
			////////////////////////////////////////////////////////////////////
			// -START-
			// (1) process individual balls
			// Problem is in here on 5th frame: //
			
			float start = (float)timeGetTime();


			//for(int it = 0;it <8;it++)
			for(n=0;n<MAX_TOTALBALLS;n++)
			{
				//rollOffIdx+=3;				
				//rollOff = _ballRollOffValues[(rollOffIdx%50)]; //[1] => -4
				rollOff = _ballRollOffValues[((rollOffIdx+n)%50)]; //[1] => -4


				//m_ball[5]->m_P.x = 1000.0*sinf(s_fCounter * 0.1f);
				
				if(m_ball[n]->Process(FALSE, bBotSim, rollOff, m_timeDelta) == TRUE)
				{
					bActive = TRUE;
					m_times++;
				}
				

				if(bWhiteOnly) break;
			}


			float end = (float)timeGetTime();

			static float taverage = 0.0f;
			static float tcount = 0.0f;
			taverage += (float)(end-start);
			tcount += 1.0f;
			if(tcount == 60) 
			{
				g_fProbe = taverage  / tcount;
				tcount = 0;
				taverage = 0.0f;
			}




			// ------------------- //			

			// after this


			 // Billiards, Work of if we made a cannon //
			// For Billiards we will use "m_lifes" as our variable to tell us
			// If the cueball has hit both Red and Opponent's ball //
			
			if(g_build.gameType == GAMETYPE_SNOOKER && m_customTable->m_type == CTTYPE_BILLIARDS)
				{
				// Player 1 //
				if(m_bTurn1P) 
				{
					// White ball must hit both the Yellow and Red (any order) //
					if(m_ball[0]->CollideWithBallSimple(m_ball[1]))
						{
						m_lifes |= 1;
						}
					if(m_ball[0]->CollideWithBallSimple(m_ball[2])) 
						{
						m_lifes |= 2;
						}
					}
				else{
					// Yellow ball must hit both the White and Red (any orer) //
					if(m_ball[1]->CollideWithBallSimple(m_ball[0]))
						{
						m_lifes |= 1;
						}
					if(m_ball[1]->CollideWithBallSimple(m_ball[2])) 
						{
						m_lifes |= 2;
						}
					}


				}



//#if 0
			// Actual Ball positions for rendering
			BYTE movedballs[MAX_TOTALBALLS];
			for(n=0;n<MAX_TOTALBALLS;n++)
			movedballs[n] = 0;
			// (2) ball2ball collisions
			for(n=0;n<MAX_TOTALBALLS;n++)
			{
			//if(!TestFlag(m_ball[n]->m_flags, Flg_BALL_MOTIONLESS));
				for(m=0;m<MAX_TOTALBALLS;m++)
					if(n!=m)
					{

						if(m_ball[n]->CollideWithBall(m_ball[m], FALSE, bBotSim))
						{
							movedballs[n] = 1;
							movedballs[m] = 1;
							bActive = TRUE;

							m_ball[n]->m_hitOtherBall = m;
							m_ball[m]->m_hitOtherBall = n;

							// keep track of last ball hit, (like dominos! :)
							if(TestFlag(m_ball[n]->m_flags, Flg_BALL_DOMINOHITLAST) ||
							   TestFlag(m_ball[m]->m_flags, Flg_BALL_DOMINOHITLAST))
							{
								ToggleFlag(m_ball[n]->m_flags, Flg_BALL_DOMINOHITLAST);
								ToggleFlag(m_ball[m]->m_flags, Flg_BALL_DOMINOHITLAST);
								
								if(TestFlag(m_ball[n]->m_flags, Flg_BALL_DOMINOHITLAST))
									m_dominoHitLast = n;
								else
									m_dominoHitLast = m;

								// only update this on first hit!
								//if(!m_dominoHit1st)					// OLD WAY
								//	m_dominoHit1st = m_dominoHitLast;								
								if(m_dominoHit1st == m_selectBall)		// SHOULD BE OK (OAP: 18/10/2003)								
									m_dominoHit1st = m_dominoHitLast;

							} // end if
						} // end if(m_ball
				
					} // end if n!=m
				if(bWhiteOnly)
				{
					if(m_dominoHit1st != 0) return FALSE;
					break;
				}
			}

			// don't know if this helps...
			
			#ifdef FIXEDMODE
			OBSTACLE_DATASTRUCTF obstacleData;
			for(n=0;n<MAX_TOTALBALLS;n++)
			if(movedballs[n])
			{ 
			FVector V;
			V = m_ball[n]->f_P-m_ball[n]->f_POld;
							
				if(m_customTable->m_obstacleCourse->Detect(&obstacleData, m_ball[n]->f_POld, V, TOFIX(BALL_RADIUS))!=-1)
				{
				m_ball[n]->f_P = obstacleData.m_hitpos + (obstacleData.m_normal * TOFIX(0.01f));
				} // 				
			}
			#else
			OBSTACLE_DATASTRUCT obstacleData;
			for(n=0;n<MAX_TOTALBALLS;n++)
			if(movedballs[n])
			{ 
			CVector V;
			V = m_ball[n]->m_P-m_ball[n]->m_POld;
							
				if(m_customTable->m_obstacleCourse->Detect(&obstacleData, m_ball[n]->m_POld, V, BALL_RADIUS)!=-1)
				{
				m_ball[n]->m_P = obstacleData.m_hitpos + (obstacleData.m_normal * 0.01f);
				} // 				
			}
			#endif //FIXEDMODE
			
			//-END-
			// Messed up here!!!
			 
			// current simulation has finished.
			if(!bActive && !bBotSim)
			//if(((CGameAppView *)m_parent)->m_elCounter == 0)
			{				
				//((CGameAppView *)m_parent)->MessageBox("got here!!!","!",MB_OK);
				if(m_cueBallSelect == 2)
				{					
					SimEndReceive(-1);
					m_process = FALSE;
					m_firstTime=FALSE;
					if(m_firstFewFrames) m_firstFewFrames--;

					if(((CGameAppView *)m_parent)->m_elCycle&EL_BALLS)
						((CGameAppView *)m_parent)->m_elCycle^=EL_BALLS;

					for(n=0;n<MAX_TOTALBALLS;n++) m_ball[n]->AntiProcess();
					return FALSE;
				}
			} // end if !bActive
			////////////////////////////////////////////////////////////////////	
		}	 // end ittr

		m_firstTime=FALSE;
		if(m_firstFewFrames) m_firstFewFrames--;
		//n = 0;
		for(n=0;n<MAX_TOTALBALLS;n++) m_ball[n]->AntiProcess();		

		// current simulation has finished.
		if(!bActive && bBotSim) return FALSE;

		return TRUE;
}

void CSnooker::ProcessSimEndCounters(void)
{
	///
	// *** waiting for players that have still to complete their sim.
	if(m_simEndWaitingDelay)
		m_simEndWaitingDelay--;

	//
	// *** time-out count
	if(!m_simEndWaitingTimeOut)
	if(!m_simEndWaitingDelay)
	{
		//m_simEndWaitingTimeOut = GAMEAPPCLIENT_FPS/8;//*15; // should be *15
		
		if(GAMEAPPVIEW->m_lobby->HostOfTable(GAMEAPPVIEW->m_playerSockIdx))
			m_simEndWaitingTimeOut = GAMEAPPCLIENT_FPS * 25; // GAMEAPPCLIENT_FPS/8
		else
			m_simEndWaitingTimeOut = GAMEAPPCLIENT_FPS * 25; // GAMEAPPCLIENT_FPS/8
		
	}


	if(m_simEndWaitingTimeOut > 1)
		m_simEndWaitingTimeOut--;
	//
}

void CSnooker::SimEndUpdateFloats(void)
{
	///////////////////////////////////////
	// stop balls from spinning on the spot...
	for(int n=0;n<MAX_TOTALBALLS;n++)
	if(TestFlag(m_ball[n]->m_flags, Flg_BALL_ACTIVE))
		{
			#ifdef FIXEDMODE
				m_ball[n]->f_wSide = 0;	// stop balls at rest, from spinning on the spot	
				SetFlag(m_ball[n]->m_flags, Flg_BALL_MOTIONLESS);				
				m_ball[n]->AdjustOrientation();
				// save current position & velocity of ball
				m_ball[n]->f_POld = m_ball[n]->f_P;
				m_ball[n]->f_VOld = m_ball[n]->f_V;
				m_ball[n]->UpdateFloats();
			#else
				m_ball[n]->m_wSide = 0;	// stop balls at rest, from spinning on the spot	
				SetFlag(m_ball[n]->m_flags, Flg_BALL_MOTIONLESS);				
				m_ball[n]->AdjustOrientation();
				// save current position & velocity of ball
				m_ball[n]->m_POld = m_ball[n]->m_P;
				m_ball[n]->m_VOld = m_ball[n]->m_V;
			#endif // FIXEDMODE
		}
	///////////////////////////////////////
}

void CSnooker::SimEndReceive(int plrSockIdx, BOOL bShotEnd)
{

	g_cycOld = g_cyc;
	

	// entire sim. has finished for all
	if(m_cueBallSelect == 2)
	{
		PreRefereeBioCalcs();

		///////////////////////////////////////
		// stop balls from spinning on the spot... & check if WHITE is touching another ball
		BOOL bTouchingBall = FALSE;

		for(int n=0;n<MAX_TOTALBALLS;n++)
		if(TestFlag(m_ball[n]->m_flags, Flg_BALL_ACTIVE))
			{
				#ifdef FIXEDMODE
					m_ball[n]->f_wSide = 0;	// stop balls at rest, from spinning on the spot
					SetFlag(m_ball[n]->m_flags, Flg_BALL_MOTIONLESS);				
					m_ball[n]->AdjustOrientation();
					// save current position & velocity of ball
					m_ball[n]->f_POld = m_ball[n]->f_P;
					m_ball[n]->f_VOld = m_ball[n]->f_V;
					m_ball[n]->UpdateFloats();
				#else
					m_ball[n]->m_wSide = 0;	// stop balls at rest, from spinning on the spot	
					SetFlag(m_ball[n]->m_flags, Flg_BALL_MOTIONLESS);				
					m_ball[n]->AdjustOrientation();
					// save current position & velocity of ball
					m_ball[n]->m_POld = m_ball[n]->m_P;
					m_ball[n]->m_VOld = m_ball[n]->m_V;
				#endif // FIXEDMODE


				
				// for now, just do this for iSnooker, *TODO* - always do, update pool ref. code
				#ifndef THE_POOL_CLUB
				// is WHITE ball touching another ball?
				// Skip this code for Billiards //
				if(m_customTable->m_type != CTTYPE_BILLIARDS)
				if(n)
				if(m_ball[0]->IsTouchingBall(m_ball[n]))
				{
					m_touchingBall[m_touchingBallIdx++] = n;
					bTouchingBall = TRUE;					
					
					 // [BUGFIX: when it is a touching ball, but the white doesn't hit]
					// only update if this is first hit for cueball!
					if(m_dominoHit1st == m_selectBall)
						m_dominoHit1st = n;
				}
				#endif //!THE_POOL_CLUB
			}

		if( !GAMEAPPVIEW->m_lobby->m_bInLobby)
		if(!m_bPracticeMode || GAMEAPPVIEW->m_bPlayOffline)
		if(bTouchingBall)
		{			
			char _buffer[MAX_CHARS];
			sprintf(_buffer,"*** Touching ball(s) declared! ***");
			GAMEAPPVIEW->Message(_buffer,10); // system message
		}

		///////////////////////////////////////		


		GAMEAPPVIEW->m_lobby->TableClearReRackRequest(GAMEAPPVIEW->m_playerSockIdx);


		// Now Give ,mouse's attention to all
		//if(GetCapture() == GAMEAPPVIEW->m_sendEdit) 
		//{
			//AfxGetMainWnd()->EnableWindow(TRUE);
			ReleaseCapture();
			GAMEAPPVIEW->PreviousWndEditSetFocus();
		//}




			m_SimEnd = FALSE;
			m_clientSimEnd = FALSE;
			m_bShotInProgress = FALSE;


			m_bPriorVoiceOnly = FALSE;
			m_cueBallSelect = 4; // SendBalls recieve will put us back into state 3			for resync

			if(m_bPracticeMode) // i.e. in practice mode
				m_cueBallSelect = 3;

			m_bCueState = FALSE;
//						m_bStrikeSend = FALSE;
			m_bBallInHand = 0;
			m_cyc = 0;
			
			// re-position cuetip at centre of ball (*TODO - (0, 1) some of the following may be redundant now)
			m_cuetip[0] = 0;
			m_cuetip[1] = 0;
			m_cuetip[2] = 0;
			
			m_RaiseButt = m_RaiseButtOld = 0;
			m_bReceiveRaiseButtActive = FALSE;
			GAMEAPPVIEW->m_sliderRaiseButt.SetPos(90);
			GAMEAPPVIEW->m_bRButtonDown = 0; // [*NEW* CP]
			GAMEAPPVIEW->m_bLButtonDown = 0; // [*NEW* CP]
			GAMEAPPVIEW->m_bRButtonJustUp = 1; // [*NEW* CP]
			GAMEAPPVIEW->m_bLButtonJustUp = 1; // [*NEW* CP]

			// update AT & POS for minicamera view, point back at white ball
			m_dominoHitLast = m_selectBall;	// the initail 'selected' ball is the last thing we want to see
			MiniCameraViewUpdate();

			
			// enforce rules & keep scores
			if(m_bSeenSelectBall) {
				//MessageBox(0, "Referee!", "Referee!" ,MB_OK);
				//Referee();
				m_customTable->Referee(CTMSG_SIMEND); // usabap
			}
			//else 
			//	ReSpot();

			PostRefereeBioCalcs();

			/////
			//clear testBall
			//m_testBall->Init(WORLD_X(912),WORLD_Y(250),0, WHITE);
			#ifndef THE_POOL_CLUB
			m_testBall->m_colour = 0;
			#endif
			/////
			
			m_nominateBall = 0; // WHITE

			/////////////////////////////////////////////
			m_targetBall = -1;	// clear target ball
			m_targetPocket = -1; // clear pocket select
			m_bTargetSuccess = FALSE;
			/////////////////////////////////////////////

			if( MyTurn() )
			if(m_CameraMode == CAMERA_MODE_PLAYER_3D);
			else 
			{
				if(!g_gameAppView->m_bReplayMode)			
				m_CameraMode = CAMERA_MODE_PLAYER_2D;
				g_pCameraMode = false;
			}
			/*
			// TV mode - only for players that watcher (not playing the shot)
			if( !MyTurn() )
			if(GAMEAPPVIEW->m_GFXMode == 3)
				m_CameraMode = CAMERA_MODE_WATCH;
			*/

			m_cueStrikeTime = 0;
			m_cuePowerPercentage = 0;

			// reset 2mins countdown timer, ready for next shot
			m_MaxShotTime = 120*50;		// 2mins per shot, after that your opponent can force a forfit!
			m_MaxShotTimeOld = m_MaxShotTime;

			
			/////////////////////////////////////////
			// move Beetle-point to 0,0f
			CPoint point;
			point.x = point.y = 0;
			MouseMove(point);//GAMEAPPVIEW->m_point);
			/////////////////////////////////////////
			
			m_bRedrawChalkMarks = TRUE; //  now ready to do final redraw of lines & chalk marks (for update in 2D mode)
			
			


			// if i'm sitting at a table and it's 'MyTurn' then send over balls to everyone else at table
			//if( MyTurn() )
			// both players to send balls over at end of shot...
			// Only Send balls to the server (with rule data etc) if we saw the Shot Selected
			if( m_bSeenSelectBall)
			if( GAMEAPPVIEW->m_lobby->SeatedAtTable(GAMEAPPVIEW->m_playerSockIdx) || 
				GAMEAPPVIEW->m_lobby->PressedStartAtTable(GAMEAPPVIEW->m_playerSockIdx) )				
			{
				GAMEAPPVIEW->SendBalls(FALSE, TRUE, -1, FALSE); //GAMEAPPVIEW->SendBalls(TRUE, TRUE);
			}


			// clear out ball potted flags - need for 8-ball iPool Practice - this is done above SendBalls Rec. fn. when online
			if(m_bPracticeMode)
			if(m_customTable->m_type != CTTYPE_BOWLS) // override possibly for Bowls
			{
				for(int m=0;m<MAX_TOTALBALLS;m++)
				{					
					/////////////////////////////////////////////////////////////
					// clear all 'POTTED' balls
					ClearFlag(m_ball[m]->m_flags, Flg_BALL_POTTED);
					/////////////////////////////////////////////////////////////
				}
			}		

			//((CGameAppView *)m_parent)->SendSimEnd(((CGameAppView *)m_parent)->m_playerSockIdx);

	////////////////////////////////////////////////////////////////////
	// LAGGERS #1:2
	////////////////////////////////////////////////////////////////////
	if(!m_bPracticeMode)
		DoLaggersCatchUp();
	////////////////////////////////////////////////////////////////////

	/////////////////////////////////////////////////////////////
	//char buf[80];
	//sprintf(buf, "-- (START) m_snooker->m_colourseq = %d ---", m_colourseq);
	//ErrorMessage(m_parent, buf);
	/////////////////////////////////////////////////////////////

	/*if(GAMEAPPVIEW->m_bReplayMode) // in replay mode, therefore continue the play cycle.
		{
		((CGameAppView *)m_pParent)->FinishReplayCommand();
		}*/
	}


}

void CSnooker::PreRefereeBioCalcs(void)
{
	m_break1P -=12;
	m_break2P -=12;

	if(!m_bPracticeMode)
	{
		if( MyTurn() )
		{
			// Bio. for 'yourself' player
			//m_bioShotTime // from being your turn until striking ball
			//m_bioPosition // error of each potted ball
			//m_bioSafety   // opponent's first shot and doesn't pot
			//m_bioPot	    // TODO: check test-ball path to pocket and check if potted. Also did we pot ball (flukes, doubles, etc).

			// calculate average shot time for player
			if(g_bioAverageShotTime < 1)
				g_bioAverageShotTime = m_bioShotTime/50;
			else
				g_bioAverageShotTime = g_bioAverageShotTime*0.9f + (m_bioShotTime/50)*0.1f;

			if(g_bioAverageShotTime < 1) g_bioAverageShotTime = 1;
			if(g_bioAverageShotTime > 120) g_bioAverageShotTime = 120;

//			char buff[128];
//			sprintf(buff, "g_bioAverageShotTime = %f", g_bioAverageShotTime);
//			ErrorMessage(g_wnd, buff);			
			/////////////////////////////////////////

			// calculate positional play of player
			if( (m_bTurn1P && m_break1P>=1) ||
				(!m_bTurn1P && m_break2P>=1) )
			{
				float strikeMaxRnd = m_strikeMaxRnd;
				if(strikeMaxRnd < 0) strikeMaxRnd = -strikeMaxRnd;
				float goodpot = strikeMaxRnd;
				
				#ifndef THE_POOL_CLUB
				goodpot += (0.0018) * (m_strikeDist / (BALL_RADIUS*20));			
				m_bioPosition = goodpot*100*100;
				m_bioPosition-=20;
				#else
				goodpot += (0.0218) * (m_strikeDist / (BALL_RADIUS*25));			
				m_bioPosition = goodpot*1000;				
				#endif

				if(m_bioPosition < 0) m_bioPosition = 0;
				else if(m_bioPosition > 100) m_bioPosition = 100;

				m_bioPosition = 100 - m_bioPosition;

				g_bioAveragePosition = g_bioAveragePosition*0.9f + m_bioPosition*0.1f;

				//char buff[128];
				//sprintf(buff, "m_bioPosition = %f, g_bioAveragePosition = %f", m_bioPosition, g_bioAveragePosition);
				//ErrorMessage(g_wnd, buff);
			}
			/////////////////////////////////////////
		}

		m_bioShotTime = 0; // reset counter for next strike
	}

	m_break1P +=12;
	m_break2P +=12;
}

void CSnooker::PostRefereeBioCalcs(void)
{
	// calculate safety play of player
	m_break1P -=12;
	m_break2P -=12;

	if(!m_bPracticeMode)
	{
		if( MyTurn() )
		{
			if( (!m_bTurn1P && m_break1P<1 && m_break2P<1) ||
				(m_bTurn1P && m_break2P<1 && m_break1P<1) )
			{
			//	if( (m_bTurn1P && m_break1P<1) ||
			//		(!m_bTurn1P && m_break2P<1) )
				
				m_bioSafety = 100;

				g_bioAverageSafety = g_bioAverageSafety*0.9f + m_bioSafety*0.1f;
/*
				char buff[128];
				sprintf(buff, "m_bioSafety = %f, g_bioAverageSafety = %f", m_bioSafety, g_bioAverageSafety);
				ErrorMessage(g_wnd, buff);
*/
			}
		}
		else
		{
			if( (m_bTurn1P && (m_break1P>=1 && m_break1P<=2)) ||
				(!m_bTurn1P && (m_break2P>=1 && m_break2P<=2)) )
			{
				m_bioSafety = 0;
				g_bioAverageSafety = g_bioAverageSafety*0.9f + m_bioSafety*0.1f;
/*
				char buff[128];
				sprintf(buff, "m_bioSafety = %f, g_bioAverageSafety = %f", m_bioSafety, g_bioAverageSafety);
				ErrorMessage(g_wnd, buff);
*/
			}
		}
	}

	m_break1P +=12;
	m_break2P +=12;
}

void CSnooker::DoLaggersCatchUp(void)
{
	// ErrorMessage(g_wnd, "DoLaggersCatchUp");
	if (g_ballsSendDataBk.buffer[0]) // make sure this is valid data [*(!)*]
	{
		// ErrorMessage(g_wnd, "DoLaggersCatchUp - 1");
		m_cueBallSelect = 4;
		GAMEAPPVIEW->SendBallsReceive(&g_ballsSendDataBk);
		memset(&g_ballsSendDataBk, 0, sizeof(BallsSendData_typ));
		MiniCameraViewUpdate(); // do we need this!? <---------------


		if (g_ResetbufferBk[0])
		{
			// ErrorMessage(g_wnd, "DoLaggersCatchUp - Reset");
			char playerTurn = g_ResetbufferBk[3];
			int *rndval = (int *)&g_ResetbufferBk[4];
			Init(m_reRackType, *rndval, playerTurn);
			memset(&g_ResetbufferBk, 0, sizeof(char)* 16);
		}

		if (g_PlayAgainbufferBk[0])
		{
			PlayAgainReceive();
			memset(&g_PlayAgainbufferBk, 0, sizeof(char)* 4);
		}

		if (g_MissResetbufferBk[0])
		{
			MissResetReceive();
			memset(&g_MissResetbufferBk, 0, sizeof(char)* 4);
		}

		if (g_Pot8OnBreakRerackbufferBk[0])
		{
			Pot8OnBreakRerackReceive();
			memset(&g_Pot8OnBreakRerackbufferBk, 0, sizeof(char)* 4);
		}

		if (g_BadBreakRerackbufferBk[0])
		{
			BOOL bOpponentToBreak = FALSE;
			if (g_BadBreakRerackbufferBk[3] == 1)
				bOpponentToBreak = TRUE;

			BadBreakRerackReceive(bOpponentToBreak);
			memset(&g_BadBreakRerackbufferBk, 0, sizeof(char)* 4);
		}

		if (g_FoulAlertbufferBk[0])
		{
			FoulAlertReceive((char)g_FoulAlertbufferBk[3]);
			memset(&g_FoulAlertbufferBk, 0, sizeof(char)* 4);
		}

		if (g_MaxTimeUpDataBk.buffer[0])
		{
			GAMEAPPVIEW->ReceivePlayerMaxTimeUp(g_MaxTimeUpDataBk.m_playerSockIdx);
			memset(&g_MaxTimeUpDataBk, 0, sizeof(MaxTimeUpSendData_typ));
		}

		if (g_ballPosDataBk.buffer[0])
		{
			BallPos(g_ballPosDataBk.m_P, g_ballPosDataBk.m_selectBall);
			memset(&g_ballPosDataBk, 0, sizeof(ballPosSendData_typ));
		}

		if (g_selectBallDataBk.buffer[0])
		{
			SelectBallReceive(g_selectBallDataBk.m_cueBallSelect, g_selectBallDataBk.m_point, g_selectBallDataBk.w, g_selectBallDataBk.h, g_selectBallDataBk.m_selectBall);
			memset(&g_selectBallDataBk, 0, sizeof(selectBallSendData_typ));
		}

		if (g_mouseMoveDataBk.buffer[0])
		{
			MouseMoveReceive(g_mouseMoveDataBk.x, g_mouseMoveDataBk.y, g_mouseMoveDataBk.w, g_mouseMoveDataBk.h, g_mouseMoveDataBk.strikeLine, g_mouseMoveDataBk.strikeDist);
			memset(&g_mouseMoveDataBk, 0, sizeof(mouseMoveSendData_typ));
		}

		if (g_playerTargetDataBk.buffer[0])
		{
			ReceivePlayerTargetData(g_playerTargetDataBk.m_targetPocket, g_playerTargetDataBk.m_targetBall, g_playerTargetDataBk.m_bTargetSuccess);
			memset(&g_playerTargetDataBk, 0, sizeof(PlayerTargetSendData_typ));
		}

		if (g_CueBallTipDataCBk.buffer[0])
		{
			CueBallTipReceiveCompact(&g_CueBallTipDataCBk);
			memset(&g_CueBallTipDataCBk, 0, sizeof(CueBallTipSendDataC_typ));
		}
	}
	else
	{
		// *new - 29/3/2004 - if we have not saved the sendballs, clear all else!
		//memset(&g_ballsSendDataBk, 0, sizeof(BallsSendData_typ));
		memset(&g_CueBallTipDataCBk, 0, sizeof(CueBallTipSendDataC_typ));
		memset(&g_ballPosDataBk, 0, sizeof(ballPosSendData_typ));
		memset(&g_selectBallDataBk, 0, sizeof(selectBallSendData_typ));
		// memset(&g_ResetbufferBk, 0, sizeof(char)* 16);
		memset(&g_mouseMoveDataBk, 0, sizeof(mouseMoveSendData_typ));
		memset(&g_PlayAgainbufferBk, 0, sizeof(char)* 4);
		memset(&g_FoulAlertbufferBk, 0, sizeof(char)* 4);
		memset(&g_MissResetbufferBk, 0, sizeof(char)* 4);
		memset(&g_MaxTimeUpDataBk, 0, sizeof(MaxTimeUpSendData_typ));
		memset(&g_playerTargetDataBk, 0, sizeof(PlayerTargetSendData_typ));
		memset(&g_Pot8OnBreakRerackbufferBk, 0, sizeof(char)* 4);
		memset(&g_BadBreakRerackbufferBk, 0, sizeof(char)* 4);
	}
}



void CSnooker::PreviousWndEditSetFocus(void)
{
	if(g_previousWndNo == 1) // statsDlg(2) statsTabCtrlDlg
	{
		//////////////////////////////////////////
		if(g_build.gameType == GAMETYPE_SNOOKER)
		{
			if( !GAMEAPPVIEW->m_statsDlg.IsWindowVisible() )
			{
				if(!GAMEAPPVIEW->m_statsDlg.m_bActive)
					g_previousWndNo = 0;
			}
			else
			{
				GAMEAPPVIEW->m_statsDlg.SetFocus();
				GAMEAPPVIEW->m_statsDlg.GetMSGFocusBack();
			}
		}
		//////////////////////////////////////////
		else // ipool
		{
			if( !GAMEAPPVIEW->m_statsDlg2.IsWindowVisible() )
			{
				if(!GAMEAPPVIEW->m_statsDlg2.m_bActive)
				{
					g_previousWndNo = 0;
				}
			}
			else
			{
				GAMEAPPVIEW->m_statsDlg2.SetFocus();
				GAMEAPPVIEW->m_statsDlg2.GetMSGFocusBack();
			}
		}
		//////////////////////////////////////////
	}
	else if(g_previousWndNo == 2) // statsChatsDlg
	{
		if( !GAMEAPPVIEW->m_statsChatDlg.IsWindowVisible() )
		{
			if(!GAMEAPPVIEW->m_statsChatDlg.m_bActive)
				g_previousWndNo = 0;
		}
		else
		{
			GAMEAPPVIEW->m_statsChatDlg.SetFocus();					
			GAMEAPPVIEW->m_statsChatDlg.GetMSGFocusBack();
		}
	}

	if(g_previousWndNo == 0) // sendEdit
	{
		GAMEAPPVIEW->m_sendEdit.SetFocus();
	}

	return;
}

void CSnooker::SelectBallSend(int select, CPoint point, int w, int h, int selectBall)
{
	((CGameAppView *)m_parent)->SendPlayerSelectBall(select, point, w, h, selectBall);
}

void CSnooker::SelectBallReceive(int select, CPoint point, int w, int h, int selectBall)
{
	// all players at table have seen the selection lines
	m_bSeenSelectBall = TRUE;
	
	if( MyTurn() ) return;

	float x, y;
	x = point.x / (float)w;
	y = point.y / (float)h;
	x *= m_windowWidth;
	y *= m_windowHeight;

	CPoint pointTmp;
	pointTmp.x = (int)x;
	pointTmp.y = (int)y;
	MouseMove(pointTmp); // update mouse's (3D table plane) coordinates to the exact click point!

	if(select == 1) // safety net - garentees we select a ball (compensates for small errors for oppenent selections)
	{
		m_tableMousePos[0] = m_ball[selectBall]->m_P.x;
		m_tableMousePos[1] = m_ball[selectBall]->m_P.y;
		m_tableMousePos[2] = m_ball[selectBall]->m_P.z;
	}

	if(SelectBall(select, pointTmp) == TRUE)
	{
		if(select == 1)
		{
			m_bStandUpFromShot = 0;
			m_sightlineFade = 60;
		}
		else if(select == 0)
		{
			m_point = pointTmp;
			m_sightlineFade = 0;
		}
	}
}

BOOL CSnooker::SelectBall(int select, CPoint point)
{ // we disregard the point, as we've calced the 3D points.


	if(g_gameAppView->m_chalkChanger->bShow) return false;
	if(g_gameAppView->m_cueChanger->bShow) return false;



	m_cueBallSelect = select;
	m_point = point;

	/////////////////////////////////////////////
	m_targetBall = -1;	// clear target ball
	m_targetPocket = -1; // clear pocket select
	m_bTargetSuccess = FALSE;
	/////////////////////////////////////////////

	if(m_cueBallSelect == 1)
	{



		m_bCueState = TRUE;
		m_sCPoint = point;
		m_sSize.x = m_cx;
		m_sSize.y = m_cy;
		m_sPoint[0] = m_tableMousePos[0];
		m_sPoint[1] = m_tableMousePos[1];
		m_sPoint[2] = m_tableMousePos[2];

		// moves cue tip to ball center
		int n;
		float fX = ((CGameAppView *)m_parent)->m_fX;
		float fY = ((CGameAppView *)m_parent)->m_fY;

		m_selectBall = 0;

		// check mouse over a ball
		for(n=0;n<MAX_TOTALBALLS;n++) 
		if(m_ball[n]->m_bMouseOver)
		{
			float dx, dy;
			dx = m_ball[n]->m_P.x - m_sPoint[0];
			dy = m_ball[n]->m_P.y - m_sPoint[1];
			if( sqrt(dx*dx + dy*dy) <= BALL_RADIUS)
			{
				#ifdef FIXEDMODE
				m_sPoint[0] = FROMFIXSMALL(m_ball[n]->f_P.x);
				m_sPoint[1] = FROMFIXSMALL(m_ball[n]->f_P.y);
				m_sPoint[2] = FROMFIXSMALL(m_ball[n]->f_P.z);
				#else//
				m_sPoint[0] = m_ball[n]->m_P.x;
				m_sPoint[1] = m_ball[n]->m_P.y;
				m_sPoint[2] = m_ball[n]->m_P.z;
				#endif
				m_selectBall = n;
				break;
			}
		}

		// If we didn't find a ball //
		if(n == MAX_TOTALBALLS)
		for(n=0;n<MAX_TOTALBALLS;n++)
		{
			if( m_ball[n]->Proximity(m_tableMousePos[0], m_tableMousePos[1], m_tableMousePos[2]) == TRUE)
			{
				#ifdef FIXEDMODE
				m_sPoint[0] = FROMFIXSMALL(m_ball[n]->f_P.x);
				m_sPoint[1] = FROMFIXSMALL(m_ball[n]->f_P.y);
				m_sPoint[2] = FROMFIXSMALL(m_ball[n]->f_P.z);
				#else//
				m_sPoint[0] = m_ball[n]->m_P.x;
				m_sPoint[1] = m_ball[n]->m_P.y;
				m_sPoint[2] = m_ball[n]->m_P.z;
				#endif
				m_selectBall = n;
				break;
			}
		}
		

		// check if user didn't select a ball
		if(n == MAX_TOTALBALLS)
		{
			m_cueBallSelect = 0;
			m_bCuePowerJustOn = FALSE;
			m_bCueState = FALSE;
			return FALSE;
		}

		m_bStrikeBallReplayDrawSL = 0; // draw sight lines flag
	}
	else{
		m_bCueState = FALSE;
		ClearFlag(m_ball[m_dominoHitLast]->m_flags, Flg_BALL_DOMINOHITLAST);
		m_dominoHitLast = m_selectBall;
	}
		   
	return TRUE;
}


#ifdef FIXEDMODE // TODO* - make non-fixed version of this function
int CSnooker::GetSelectedBall(void)
{
	//char buf[256];
	//sprintf(buf, "m_tableMousePosTmp[0] = %f. m_tableMousePosTmp[1] = %f", m_tableMousePosTmp[0], m_tableMousePosTmp[0]);
	//ErrorMessage(m_parent, buf);

	// ----------------------------------------

	//FVector MouseClickPos = FVector(TOFIX(m_tableMousePosTmp[0]), TOFIX(m_tableMousePosTmp[1]), TOFIX(m_tableMousePosTmp[2]));
	CVector MouseClickPos = CVector(m_tableMousePosTmp[0], m_tableMousePosTmp[1], m_tableMousePosTmp[2]);
	
	// 1 - check if we are on top of another ball (except WhiteBall)
	for(int m=1;m<MAX_TOTALBALLS;m++)
	//if(m_ball[m]->m_colour != m_TestBallCollideObjColour) // dont include object ball that testBall collided with.
	if(m_ball[m]->Proximity(MouseClickPos.x, MouseClickPos.y, 0, BALL_RADIUS))
	{
		return m;
	}

	// ----------------------------------------

	// ball was not selected
	return -1;
}

#else

int CSnooker::GetSelectedBall(void)
{
	return -1;
}
#endif //FIXEDMODE


#ifdef FIXEDMODE
int CSnooker::GetSelectedPocket(void)
{
	//char buf[256];
	//sprintf(buf, "m_tableMousePosTmp[0] = %f. m_tableMousePosTmp[1] = %f", m_tableMousePosTmp[0], m_tableMousePosTmp[0]);
	//ErrorMessage(m_parent, buf);

	// ----------------------------------------

	FVector MouseClickPos = FVector(TOFIX(m_tableMousePosTmp[0]), TOFIX(m_tableMousePosTmp[1]), TOFIX(m_tableMousePosTmp[2]));
	FVector Pocket;
	for(int n=0;n<m_customTable->m_pocket_n;n++)
	{
		Pocket.x = TOFIX(m_customTable->m_pocketPos[n].x);
		Pocket.y = TOFIX(m_customTable->m_pocketPos[n].y);
		Pocket.z = TOFIX(m_customTable->m_pocketPos[n].z);

		Pocket = Pocket - MouseClickPos;

		__fix sR = TOFIX(m_customTable->m_pocketProp[n].m_suctionRadius);
		__fix sF = TOFIX(m_customTable->m_pocketProp[n].m_suctionFactor);
		__fix r = TOFIX(m_customTable->m_pocketProp[n].m_radius);
		
		Pocket.z = 0;
		
		__fix sR2 = FIXMUL(sR, sR);

		if(Pocket.Magnitude2() < sR2)
		{
			
			//__fix r2 = FIXMUL(r, r);
			//if(Pocket.Magnitude2() < r2)

			//sprintf(buf, "n = %d", n);
			//ErrorMessage(m_parent, buf);			

			return n;
		}
	}	
	// ----------------------------------------

	// pocket was not selected
	return -1;
}

#else

int CSnooker::GetSelectedPocket(void)
{
	/*
	// ----------------------------------------
	
	CVector Pocket;
	for(int n=0;n<m_customTable->m_pocket_n;n++)
	{
		Pocket = m_customTable->m_pocketPos[n];
		Pocket = Pocket - m_P;
		float sR = m_customTable->m_pocketProp[n].m_suctionRadius;
		float sF = m_customTable->m_pocketProp[n].m_suctionFactor;
		float r = m_customTable->m_pocketProp[n].m_radius;
		
		Pocket.z = 0;
		
		if(Pocket.Magnitude2() < SQR((sR)))
		{
			//if(Pocket.Magnitude2() < SQR(r))
			return n;

		}
	}	
	// ----------------------------------------
	*/

	return -1;

}
#endif // FIXEDMODE


void CSnooker::SetTargetBall(int n)
{	
	m_targetBall = n;
}

void CSnooker::SetTargetPocket(int n)
{
	m_targetPocket = n;
			
	//m_customTable->m_pocketPos[n].x
	//m_customTable->m_pocketPos[n].y
	//m_customTable->m_pocketPos[n].z
}

void CSnooker::ReceivePlayerTargetData(int targetPocket, int targetBall, int bTargetSuccess)
{
	m_targetPocket = targetPocket;
	m_targetBall = targetBall;
	m_bTargetSuccess = bTargetSuccess;
}

#ifndef THE_POOL_CLUB
BOOL CSnooker::DropBallOnTable(int n, float x, float y, BOOL bDrop)
{
	// check if ball 'n' is on top of another ball
	int m;
	for(m=0;m<MAX_TOTALBALLS;m++)
	if(m!=n)
	if(m_ball[m]->Proximity(x, y, 0, BALL_RADIUS*2.0f) == TRUE)
	{
		return FALSE;
	}
	
	// check for ball n inside the D
	if(!m_bPracticeMode || GAMEAPPVIEW->m_bPlayOffline) // should be !
	{
		CVector B = CVector(x,y,BALL_POSITIONZOFFSET);
		CVector Pgreen, Pbrown;
		CVector P;
		float radius;
		Pgreen = CVector(-WORLD_X(912),-WORLD_Y(395-12), BALL_POSITIONZOFFSET);
		Pbrown = CVector(-WORLD_X(912),-WORLD_Y(297),BALL_POSITIONZOFFSET);

		//[P = Pgreen - Pbrown;]
		P = Pgreen;
		P = P - Pbrown;

		radius = P.Magnitude2();
		
		//[P = B - Pbrown;]
		P = B;
		P = P - Pbrown;

		if( P.Magnitude2() < radius)
		{
			if(B.x < Pbrown.x)
			{
				// update the drop ball's coordinates
				if(bDrop)
				{
					//m_ball[n]->m_P.x = x;
					//m_ball[n]->m_P.y = y;
					//m_ball[n]->m_POld = m_ball[n]->m_P;
				m_ball[n]->m_P.x = x;
				m_ball[n]->m_P.y = y;
				#ifdef FIXEDMODE
				m_ball[n]->m_V.x = 0;
				m_ball[n]->m_V.y = 0;
				m_ball[n]->m_V.z = 0;
				m_ball[n]->UpdateFixeds();
				#endif
				m_ball[n]->AdjustOrientation();
				m_ball[n]->m_POld = m_ball[n]->m_P;
				#ifdef FIXEDMODE
				m_ball[n]->UpdateFixeds();
				#endif
				}
				return TRUE;
			}
		}
		return FALSE;
	}
	
	// update the drop ball's coordinates
	if(bDrop)
	{
		//m_ball[n]->m_P.x = x;
		//m_ball[n]->m_P.y = y;
		//m_ball[n]->AdjustOrientation();
		//m_ball[n]->m_POld = m_ball[n]->m_P;

		m_ball[n]->m_P.x = x;
		m_ball[n]->m_P.y = y;
		#ifdef FIXEDMODE
		m_ball[n]->m_V.x = 0;
		m_ball[n]->m_V.y = 0;
		m_ball[n]->m_V.z = 0;
		m_ball[n]->UpdateFixeds();
		#endif
		m_ball[n]->AdjustOrientation();
		m_ball[n]->m_POld = m_ball[n]->m_P;
		#ifdef FIXEDMODE
		m_ball[n]->UpdateFixeds();
		#endif

	}
	return TRUE;	
}
#else
BOOL CSnooker::DropBallOnTable(int n, float x, float y, BOOL bDrop)
{
	// *TODO - switch other pool game types in future.
	//return m_customTable->Referee8BallDropBallOnTable(WHITE, x, y, bDrop);
	return m_customTable->Referee8BallDropBallOnTable(n, x, y, bDrop);
}
#endif //!THE_POOL_CLUB

BOOL CSnooker::NominateBall(CPoint point)
{ // we disregard the point, as we've calced the 3D points.

		int n;
		for(n=0;n<MAX_TOTALBALLS;n++)
		{
			if( m_ball[n]->Proximity(m_tableMousePos[0], m_tableMousePos[1], m_tableMousePos[2]) == TRUE)
			{
				/*// these ball cannot be nominated
				if(m_ball[n]->m_colour == WHITE);
				else if(m_ball[n]->m_colour == RED);

				// these ball can be nominated
				else*/
				if(m_ball[n]->m_colour >= YELLOW)
				{					
					int idx = 48+(m_ball[n]->m_colour-YELLOW);
					if(idx >= 48 && idx <= 53)
					{
						if(g_build.addOnSfx && g_gfxDetails.bSoundVoices)
							Msfx_smpplayvol(&GAMEAPPVIEW->m_game_smp[idx], 212);
					}

					m_nominateBall = n;
					break;
				}
			}
		}
		   
	return TRUE;
}


void CSnooker::SavePreviousPosition(void)
{
	// save undo position of balls
#ifdef FIXEDMODE
	for(int m=0;m<MAX_TOTALBALLS;m++)
	if(TestFlag(m_ball[m]->m_flags, Flg_BALL_ACTIVE))
	{
		m_ball[m]->f_PUndo = m_ball[m]->f_P;
	}
	else
	{
		// note: this ball is not active
		m_ball[m]->f_PUndo.x = TOFIX(20000);
		m_ball[m]->f_PUndo.y = TOFIX(20000);
	}
#else
	for(int m=0;m<MAX_TOTALBALLS;m++)
	if(TestFlag(m_ball[m]->m_flags, Flg_BALL_ACTIVE))
	{
		m_ball[m]->m_PUndo = m_ball[m]->m_P;
	}
	else
	{
		// note: this ball is not active
		m_ball[m]->m_PUndo.x = 20000;
		m_ball[m]->m_PUndo.y = 20000;
	}
#endif
	///////

	m_score1PUndo = m_score1P;
	m_break1PUndo = m_break1P;					
	
	m_rednextUndo = m_rednext;
	m_colourseqUndo = m_colourseq;
	m_bBallInHandUndo = m_bBallInHand;

	
	for(int t=0; t<MAX_TOUCHING_BALLS; t++)
		m_oldTouchingBall[t] = m_touchingBall[t];
	
	m_previousSaved = TRUE;
}

void CSnooker::RestorePreviousPosition(void)
{
	if(m_previousSaved == FALSE) return;
	m_previousSaved = FALSE;
		
	// NOTE: only call this if (m_cueBallSelect == 0) <-----
	
	// replay code: Adds a Strike to the replay list /////////////////////////////////////////
	//	((CGameAppView *)m_parent)->m_replay->DeleteStrike(); // NOTE: THIS LINE USED TO BE IN ver.1.6.7


	// end replay code ///////////////////////////////////////////////////////////////////////

	for(int t=0; t<MAX_TOUCHING_BALLS; t++)
		m_touchingBall[t] = m_oldTouchingBall[t];

	// save undo position of balls
	#ifdef FIXEDMODE
	for(int m=0;m<MAX_TOTALBALLS;m++)
	if(m_ball[m]->f_PUndo.x > TOFIX(19999) && m_ball[m]->f_PUndo.y > TOFIX(19999))
	{
		ClearFlag(m_ball[m]->m_flags, Flg_BALL_ACTIVE);
	}
	else
	{
		SetFlag(m_ball[m]->m_flags, Flg_BALL_ACTIVE);
		m_ball[m]->f_P = m_ball[m]->f_POld = m_ball[m]->f_PUndo;

		m_ball[m]->m_P.x = FROMFIXSMALL(m_ball[m]->f_P.x);
		m_ball[m]->m_P.y = FROMFIXSMALL(m_ball[m]->f_P.y);
		m_ball[m]->m_P.z = FROMFIXSMALL(m_ball[m]->f_P.z);

		// *** note: taken from Motionless() in ball.cpp ***
		ClearFlag(m_ball[m]->m_flags, Flg_BALL_PUREROLL);
						
		// bring 'this' ball to a full stop!
		m_ball[m]->f_V = FVector(0,0,0);
		m_ball[m]->f_VOld = FVector(0,0,0);
		m_ball[m]->f_W = FVector(0,0,0);
		m_ball[m]->f_wnroll = 0;
		m_ball[m]->f_wSide = 0;	// stop balls at rest, from spinning on the spot

		m_ball[m]->m_WDelay = 0; // NEW

		ClearFlag(m_ball[m]->m_flags, Flg_BALL_HITCUSHION | Flg_BALL_HITBALL);
		// **************************************************

		// *TODO* ??? WHAT IF BALL WAS JUST POTTED IN SNOOKER/POOL, WHAT HAPPENS TO 'FLG_BALL_POTTED' (WE SHOULD CLEAR IT) ???
		
	}
	#else
	for(int m=0;m<MAX_TOTALBALLS;m++)
	if(m_ball[m]->m_PUndo.x > 19999 && m_ball[m]->m_PUndo.y > 19999)
	{
		ClearFlag(m_ball[m]->m_flags, Flg_BALL_ACTIVE);
	}
	else
	{
		SetFlag(m_ball[m]->m_flags, Flg_BALL_ACTIVE);
		m_ball[m]->m_P = m_ball[m]->m_POld = m_ball[m]->m_PUndo;
		
		// *** note: taken from Motionless() in ball.cpp ***
		ClearFlag(m_ball[m]->m_flags, Flg_BALL_PUREROLL);
						
		// bring 'this' ball to a full stop!
		
		m_ball[m]->m_V = CVector(0,0,0);
		m_ball[m]->m_VOld = CVector(0,0,0);
		m_ball[m]->m_W = CVector(0,0,0);
		m_ball[m]->m_wnroll = 0;
		m_ball[m]->m_wSide = 0;	// stop balls at rest, from spinning on the spot
		
		m_ball[m]->m_WDelay = 0; // NEW

		ClearFlag(m_ball[m]->m_flags, Flg_BALL_HITCUSHION | Flg_BALL_HITBALL);
		// **************************************************

		// *TODO* ??? WHAT IF BALL WAS JUST POTTED IN SNOOKER/POOL, WHAT HAPPENS TO 'FLG_BALL_POTTED' (WE SHOULD CLEAR IT) ???

	}
	#endif
	///////

	m_score1P = m_score1PUndo;
	m_break1P = m_break1PUndo;
	
	m_rednext = m_rednextUndo;
	m_colourseq = m_colourseqUndo;

	#ifndef THE_POOL_CLUB // isnooker
	if(m_colourseq >= YELLOW && m_colourseq <= BLACK)
	{
		m_ball[MAX_WHITEBALLS+MAX_REDBALLS+m_colourseq-2]->m_colour = m_colourseq;
	}
	#else // ipool
	UpdateGPottedBalls();	
	#endif //THE_POOL_CLUB
	
	m_bBallInHand = m_bBallInHandUndo;

	m_bRefreshScores = TRUE;

	m_bRaiseButtHelper = TRUE;
}

void CSnooker::UpdateGPottedBalls(void)
{
	// re-update the g_pottedspots &g_pottedstripes arrays 
	int type = m_customTable->m_type;
	switch(type)
	{
		case CTTYPE_8BALL:
			m_customTable->Referee8BallUpdatePots();
			break;
		case CTTYPE_9BALL:
			m_customTable->Referee9BallUpdatePots();
			break;
		case CTTYPE_ROTATIONBALL:
			m_customTable->RefereeRotationBallUpdatePots();
			break;
		//case CTTYPE_BOWLS:
		//	m_customTable->RefereeBowlsBallUpdatePots();
		//	break;
		case CTTYPE_6BALL:
			m_customTable->Referee6BallUpdatePots();
			break;
		case CTTYPE_10BALL:
			m_customTable->Referee10BallUpdatePots();
			break;
		case CTTYPE_UKPOOL:
			m_customTable->RefereeUKPoolUpdatePots();
			break;
		case CTTYPE_BREAK:
			m_customTable->RefereeBreakUpdatePots();
			break;
	}
}


float CSnooker::GetStrikeBallError(BOOL bWithRnd)
{
	// g_bOriginalVersion = true;

	if (g_bOriginalVersion)
	{
		// Use the Error Code from version 1.8.70
		return GetStrikeBallErrorA(bWithRnd);
	}
	else
	{
		// Use the latest Error Code (version 2.2.43 and onwards)
		return GetStrikeBallErrorB(bWithRnd);
	}

}


#ifndef THE_POOL_CLUB

// Error Code from version 1.8.70
float CSnooker::GetStrikeBallErrorA(BOOL bWithRnd) // NOTE: this function is not used!!!
{
	// calc striking error value
	srand(m_rnd);
	int rnd = (rand() % 100) - 50;
	int neg;

	// ***************** override it!
	if (!bWithRnd)
		rnd = 50;
	// ******************************

	rnd = 50;

	///////////////////////////////////////////////////////////////////////
	// find out whether initial random value is 'negative' or 'positive'
	if (rnd == 0) {
		neg = rand() % 2;
		if (!neg) neg = -1;
	}
	else if (rnd < 0) neg = -1;
	else neg = 1;
	///////////////////////////////////////////////////////////////////////

	float fRnd;
	////////////////////////////////////////////////////

	m_testBall->m_VOld = m_testBall->m_V;

	/////////////////////////////////////////////////////////////////
	// angleError for thin cuts (***)
	//CVector Adir = m_testBall->m_VOld.Unit();//m_V.Unit(); <- not calced. yet
	//CVector Bdir = m_testBall->m_V.Unit();

	CVector Adir = m_TestBallCollideVA.Unit(); // white balls initial velocity
	CVector Bdir = m_TestBallCollideVB.Unit(); // object balls velocity just after collision

	float angleError;
	angleError = Adir.Dot(Bdir);
	angleError = 1.0f - angleError;

	// ------- [*NEW - ver V1.6.7d] ---------	
	float softCloseThinShotError = 0.0f;

	if (m_cueError >= 1) // semi-pro, pro
	{
		float dd = m_strikeDist / (BALL_RADIUS*2.0f); // in no. of balls
		float angErr = angleError;
		float powErr = (float)(m_testBallInitialSpeed / BALL_MAXSPEED);

		if (dd < 0.1f || powErr < 0.01f)
			softCloseThinShotError = 0.0f;
		else
			softCloseThinShotError = (1 / powErr) * angErr * (1 / dd);

		if (softCloseThinShotError < 0.0f) softCloseThinShotError = 0.0f;
		if (softCloseThinShotError > 50.0f) softCloseThinShotError = 50.0f;

		softCloseThinShotError /= 10;
		softCloseThinShotError += 1.0f;
	}

	float longMiddleAngleErrorFactor = 1.0f;
	if (m_cueError >= 1) // semi-pro, pro
	{
		float val = angleError - 0.3;
		if (val < 0) val = -val;

		val = 1 - val; // 1..0.3
		val -= 0.4;

		float longdist = m_strikeDist / (BALL_RADIUS * 2.0f * 15.0f); // 15 balls distance
		longdist /= 3;
		val *= longdist;

		val *= 0.75;

		val *= 0.87; ///<---

		val += 1.0;

		longMiddleAngleErrorFactor = val;

	}
	// --------------------------------------

	float longAngleErrorFactor = m_strikeDist / (BALL_RADIUS * 2.0f * 15.0f); // 15 balls distance
	longAngleErrorFactor *= angleError;
	if (longAngleErrorFactor < 1) longAngleErrorFactor = 1;
	if (longAngleErrorFactor > 3) longAngleErrorFactor = 3;

	
	// include dist into angle Error (i.e. close cuts are harder now)
	float dd = m_strikeDist;
	
	if (dd == 0) angleError = 0; // no error if they are super close (what abt touching ball!? OAP)
	else
	{
		dd = (BALL_RADIUS * 14) / dd;
		
		angleError *= dd;
		angleError *= 1.6;//1.8;

		if (angleError > 1.0)
			angleError *= angleError;

	
		if (m_cueError >= 1)
		if (angleError > 5) angleError = 5;
	}


	if (m_cueError >= 1)
		angleError *= 1.5;


	// ------- [*NEW - ver V1.6.7d] ---------
	if (m_cueError >= 1) // semi-pro, pro
	{
		angleError *= longMiddleAngleErrorFactor;//(1.03*longAngleErrorFactor);

		// *NEW
		angleError /= softCloseThinShotError;
	}
	// --------------------------------------


	/////// Rotate Angle using Gaussian Distrition so more concentrated error at small deviation
	/////// Less frequent large error deviations from 0.
	float E_Mean = 0.0f;
	float E_Sigma = 1.0f;
	float E_Var;
	float E_X;
	float E_Rnd;
	float a, b, c;


	E_X = rnd;
	E_X = E_X / 60.0f;	   ////(-1 to 1 S.)

	/// Note at 4 Sigma the Gaussian value will be very low - les than 0.5 percent

	E_Rnd = (float)rnd;
	E_Var = E_Sigma * E_Sigma;


	////// First part of Gaussian Equation //////
	a = 2.0f;
	a = a*3.1415f;
	a = sqrt(a);
	a = a * E_Sigma;
	a = 1 / a;

	////// Second part of Gaussian Equation //////
	b = E_X - E_Mean;
	b = b*b;//-(b*b);
	b = -b;// "
	b = b / 2;
	b = b / E_Var;

	////// Evaluate Gaussian ///////////
	E_Rnd = exp(b);
	E_Rnd = E_Rnd * a;

	if (E_X<0) E_Rnd = -E_Rnd;
	E_Rnd = 1 / E_Rnd;

	//E_Rnd *= 0.00020f;    ///// Scaling of Error.. You might want to increase this.
	//E_Rnd *= 0.00014f;    ///// Scaling of Error.. You might want to increase this.

	//if(m_cueError == 3)
	//	E_Rnd *= 0.00018f;//E_Rnd *= 0.00018f;    ///// Scaling of Error.. You might want to increase this.
	if (m_cueError == 2)
		E_Rnd *= 0.00018f;//E_Rnd *= 0.00018f;    ///// Scaling of Error.. You might want to increase this.
	else if (m_cueError == 1)
		E_Rnd *= 0.00018f;//E_Rnd *= 0.00018f;    ///// Scaling of Error.. You might want to increase this.
	else
		E_Rnd = 0;

	//E_Rnd *= 0.8;

	/////////////////////////////////////////////////////////////////		
	// for thin cuts, see (***) above.
	////	float cutError = E_Rnd * angleError; // OLD WAY!
	//	fRnd = ((rand()%100)-50) * 0.00008;
	fRnd = (float)((rand() % 50)*neg) * 0.00008f;
	if (!bWithRnd) fRnd = (float)(50 * neg) * 0.00008f;
	float cutError = fRnd * angleError;

	//if(m_cueError == 3)
	//	cutError = cutError * 1.2f;//1.5f;//0.7f;//2.2f;//2.4f; //2.0f//2.8f;//2.5f;
	if (m_cueError == 2)
		cutError = cutError * 1.2f;//1.5f;//0.7f;//2.2f;//2.4f; //2.0f//2.8f;//2.5f;
	else if (m_cueError == 1)
		cutError = cutError * 1.2f;////1.25f;//2.8f;//2.5f;
	else
		cutError = 0;


	/////////////////////////////////////////////////////////////////		
	// for shots near cushions
	CVector StrikekDir = m_testBall->m_VOld.Unit();
	float cushionError = m_ball[m_selectBall]->CushionError(StrikekDir);
	////	cushionError = cushionError * E_Rnd; // OLD WAY!
	//	fRnd = ((rand()%100)-50) * 0.00008;
	fRnd = (float)((rand() % 50)*neg) * 0.00008f;
	if (!bWithRnd) fRnd = (float)(50 * neg) * 0.00008f;
	cushionError = cushionError * fRnd;

	// ------- [*NEW - ver V1.6.7d] ---------
	if (m_cueError >= 1) // semi-pro, pro
	{
		cushionError = cushionError * 1.2; //*NEW since Ver.1.6.7c[beta]
	}
	// --------------------------------------

	//if(m_cueError == 3)	
	//	cushionError = cushionError * 0.6f;//0.8f;//1.68f; //1.4;//2.0f;
	if (m_cueError == 2)
		cushionError = cushionError * 0.6f;//0.8f;//1.68f; //1.4;//2.0f;
	else if (m_cueError == 1)
		cushionError = cushionError * 0.6f;//2.0f;
	else
		cushionError = 0;




	/////////////////////////////////////////////////////////////////		
	// for shots where white is hampered by other balls (Direct-line)
	//	CVector StrikekDir = m_testBall->m_VOld.Unit();
	float hError = 0;
	float hamperedByBallError = 0;

	int t;
	for (t = 0; t<MAX_TOTALBALLS; t++)
	if (t != m_selectBall)
	{
		//hError = m_ball[t]->HamperedByBallError(m_ball[m_selectBall], StrikekDir);
		hError = m_ball[t]->HamperedByBallError(m_ball[m_selectBall], m_TestBallCollideVA);
		if (hError > hamperedByBallError)
			hamperedByBallError = hError;
	}

	fRnd = (float)((rand() % 50)*neg) * 0.00008f;
	if (!bWithRnd) fRnd = (float)(50 * neg) * 0.00008f;
	hamperedByBallError = hamperedByBallError * fRnd;

	//hamperedByBallError *= 1.25f;//1.5f; // OLD ver.1.6.7c[beta]
	// ------- [*NEW - ver V1.6.7d] ---------
	hamperedByBallError *= 1.40f;//1.5f;
	// --------------------------------------

	if (m_cueError == 2)
		hamperedByBallError = hamperedByBallError * 0.6f;//0.8f;//1.68f; //1.4;//2.0f;
	else if (m_cueError == 1)
		hamperedByBallError = hamperedByBallError * 0.6f;//2.0f;
	else
		hamperedByBallError = 0;


#ifndef THE_POOL_CLUB 

	// SNOOKER ONLY: Side Spin Error. The More Side spin we apply, the more error needs to be added to the shot //
	float SideSpinError = 0.0f;

	SideSpinError = m_cuetip[0] / 1.10f;
	SideSpinError = SideSpinError * m_cuetip[0] / 1.10f;
	SideSpinError = SideSpinError * SideSpinError;

	float PowerFactor = (float)(m_testBallInitialSpeed / BALL_MAXSPEED);
	PowerFactor *= 0.02;
	SideSpinError = SideSpinError * PowerFactor;

#endif

	/////////////////////////////////////////////////////////////////		
	// for raising the butt of the cue
	fRnd = (float)((rand() % 50)*neg) * 0.00008f;
	if (!bWithRnd) fRnd = (float)(50 * neg) * 0.00008f;
	float raisedButtError = m_cuetip[2] * fRnd * 2.5f;//3.5f;

	/////////////////////////////////////////////////////////////////

	// ------- [*NEW - ver V1.6.7d] ---------
	float powerErrorFactor = 1.0f;
	if (m_cueError == 2) {
		powerErrorFactor = 1.40 + (float)(m_testBallInitialSpeed / BALL_MAXSPEED)*0.4; // range (140% - 180% * OverallError )	

		float upper = (float)(m_testBallInitialSpeed - (BALL_MAXSPEED*0.5));
		if (upper < 0) upper = 0;
		powerErrorFactor += (float)(upper / BALL_MAXSPEED)*1.0; // (50%power and above) add 0.0 - 0.5 more error

		float upper2 = (float)(m_testBallInitialSpeed - (BALL_MAXSPEED*0.75));
		if (upper2 < 0) upper2 = 0;
		powerErrorFactor += (float)(upper2 / BALL_MAXSPEED)*1.0; // (75%power and above) add 0.0 - 0.25 more error
	}
	else if (m_cueError == 1)
	{
		//powerErrorFactor = 0.80 + (float)(m_testBallInitialSpeed / BALL_MAXSPEED)*0.8; // range (80% - 160% * OverallError )	
		powerErrorFactor = 1.40 + (float)(m_testBallInitialSpeed / BALL_MAXSPEED)*0.4; // range (80% - 160% * OverallError )	

		float upper = (float)(m_testBallInitialSpeed - (BALL_MAXSPEED*0.5));
		if (upper < 0) upper = 0;
		powerErrorFactor += (float)(upper / BALL_MAXSPEED)*1.0; // (50%power and above) add 0.0 - 0.5 more error

		float upper2 = (float)(m_testBallInitialSpeed - (BALL_MAXSPEED*0.75));
		if (upper2 < 0) upper2 = 0;
		powerErrorFactor += (float)(upper2 / BALL_MAXSPEED)*1.0; // (75%power and above) add 0.0 - 0.25 more error
	}
	else
		powerErrorFactor = 0;
	// --------------------------------------

	/////////////////////////////////////////////////////////////////

	/////////////////////////////////////////////////////////////////	
	// add on more extra
	if (m_cueError == 2)
	{
		E_Rnd = E_Rnd * powerErrorFactor * 0.8f;
		E_Rnd = E_Rnd + cutError * powerErrorFactor * 0.6 * 2.0 * 2.0 * 0.75 * 0.9 * 0.8; // <--- NEW 0.75 * 0.8
		//E_Rnd = E_Rnd + cutError * powerErrorFactor * 0.6 * 2.0 * 2.0 * 0.75 * 0.9;

		E_Rnd = E_Rnd * 1.2;
		E_Rnd = E_Rnd / longAngleErrorFactor;

		E_Rnd = E_Rnd + raisedButtError * powerErrorFactor * 0.85;
		E_Rnd = E_Rnd + cushionError * powerErrorFactor * 1.2;

		E_Rnd = E_Rnd + hamperedByBallError * powerErrorFactor * 1.2;

		E_Rnd = E_Rnd + SideSpinError;			// New Error for Side Spin (Added 6th March 07)


		E_Rnd *= 0.80;//0.70;//0.77;//0.88;//0.93; // -5% from 93%

		if (bWithRnd)
		{
			float multiplier1 = 0.01f * (float)((rand() % 100) + 1); // 1-100% multiplier
			
			float multiplier = 0;
			multiplier = multiplier1;

			if (multiplier < 0.25)
				multiplier *= 2;

			int sign = rand() % 2;
			if (!sign) E_Rnd = -E_Rnd;

			E_Rnd = E_Rnd * multiplier;
		}
	}
	else if (m_cueError == 1)
	{
		E_Rnd = E_Rnd * powerErrorFactor * 0.8f;
		//E_Rnd = E_Rnd + cutError * powerErrorFactor * 0.6 * 2.0 * 2.0;
		E_Rnd = E_Rnd + cutError * powerErrorFactor * 0.6 * 2.0 * 2.0 * 0.75 * 0.9; // <--- NEW 0.75 * 0.8

		E_Rnd = E_Rnd * 1.2;
		E_Rnd = E_Rnd / longAngleErrorFactor;

		E_Rnd = E_Rnd + raisedButtError * powerErrorFactor * 0.85;
		E_Rnd = E_Rnd + cushionError * powerErrorFactor * 1.2;

		E_Rnd = E_Rnd + hamperedByBallError * powerErrorFactor * 1.2;

		E_Rnd = E_Rnd + SideSpinError;			// New Error for Side Spin (Added 6th March 07)

		E_Rnd *= 0.55;

		if (bWithRnd)
		{
			float multiplier1 = 0.01f * (float)((rand() % 100) + 1); // 1-100% multiplier
			float multiplier = 0;
			multiplier = multiplier1;
			
			if (multiplier < 0.25)
				multiplier *= 2;


			int sign = rand() % 2;
			if (!sign) E_Rnd = -E_Rnd;
			E_Rnd = E_Rnd * multiplier;
		}
	}

	else
	{
		E_Rnd = 0;
	}
	
#ifdef SHOW_ERRORVALUE
	if (bWithRnd)
	{
		char bbuffer[80];
		sprintf(bbuffer, "E_Rnd = %f", E_Rnd);
		ErrorMessage(m_parent, bbuffer);
	}
#endif //SHOW_ERRORVALUE

	return E_Rnd;
	//////////////////////////////////////////////////////
}

#else //THE_POOL_CLUB

float CSnooker::GetStrikeBallErrorA(BOOL bWithRnd) // NOTE: this function is not used!!!
{
	// calc striking error value
	//	int rnd = (m_rnd%100)-50;  // <-- OLD WAY!!!!!
	srand(m_rnd);
	int rnd = (rand() % 100) - 50;
	int neg;


	// ***************** override it!
	if (!bWithRnd)
		rnd = 50;
	// ******************************

	rnd = 50;

	///////////////////////////////////////////////////////////////////////
	// find out whether initial random value is 'negative' or 'positive'
	if (rnd == 0) {
		neg = rand() % 2;
		if (!neg) neg = -1;
	}
	else if (rnd < 0) neg = -1;
	else neg = 1;
	///////////////////////////////////////////////////////////////////////



	float fRnd;
	////////////////////////////////////////////////////

	m_testBall->m_VOld = m_testBall->m_V;

	/////////////////////////////////////////////////////////////////
	// angleError for thin cuts (***)
	//CVector Adir = m_testBall->m_VOld.Unit();//m_V.Unit(); <- not calced. yet
	//CVector Bdir = m_testBall->m_V.Unit();

	CVector Adir = m_TestBallCollideVA.Unit(); // white balls initial velocity
	CVector Bdir = m_TestBallCollideVB.Unit(); // object balls velocity just after collision

	float angleError;
	angleError = Adir.Dot(Bdir);
	angleError = 1.0f - angleError;
	angleError = angleError*1.2f; // *NEW - added 20% more at 25/9/2003

	float longAngleErrorFactor = m_strikeDist / (BALL_RADIUS * 2.0f * 9.0f); // 15 balls distance
	longAngleErrorFactor *= angleError;
	if (longAngleErrorFactor < 1) longAngleErrorFactor = 1;
	if (longAngleErrorFactor > 3) longAngleErrorFactor = 3;


	// include dist into angle Error (i.e. close cuts are harder now)
	//CVector DD = m_testBall->m_P - m_testBall->m_POld; // dist between object-ball and cue-ball
	//float dd = DD.Magnitude();
	float dd = m_strikeDist;
	//char buffer[180];
	//sprintf(buffer, "angleError = %f : %f, (BALL_RADIUS*2 = %f)", angleError, m_strikeDist, BALL_RADIUS*2);


	if (dd == 0) angleError = 0; // no error if they are super close (what abt touching ball!? OAP)
	else
	{
		//dd = (BALL_RADIUS*14) / dd;
		dd = (BALL_RADIUS * 7) / dd;
		angleError *= dd;
		angleError *= 1.6;//1.8;

		if (angleError > 1.0)
			angleError *= angleError;

		//if(angleError > 20) angleError = 20; //*OLD FROM 25/9/2003
		//if(angleError > 10) angleError = 10;

		//if(angleError > 12) angleError = 12; // <-WAS
		if (angleError > 6) angleError = 6; // *NEW* 22/12/2003
	}

	

	/////// Rotate Angle using Gaussian Distrition so more concentrated error at small deviation
	/////// Less frequent large error deviations from 0.
	float E_Mean = 0.0f;
	float E_Sigma = 1.0f;
	float E_Var;
	float E_X;
	float E_Rnd;
	float a, b, c;


	E_X = rnd;
	E_X = E_X / 60.0f;	   ////(-1 to 1 S.)

	/// Note at 4 Sigma the Gaussian value will be very low - les than 0.5 percent

	E_Rnd = (float)rnd;
	E_Var = E_Sigma * E_Sigma;


	////// First part of Gaussian Equation //////
	a = 2.0f;
	a = a*3.1415f;
	a = sqrt(a);
	a = a * E_Sigma;
	a = 1 / a;

	////// Second part of Gaussian Equation //////
	b = E_X - E_Mean;
	b = b*b;//-(b*b);
	b = -b;// "
	b = b / 2;
	b = b / E_Var;

	////// Evaluate Gaussian ///////////
	E_Rnd = exp(b);
	E_Rnd = E_Rnd * a;

	if (E_X<0) E_Rnd = -E_Rnd;
	E_Rnd = 1 / E_Rnd;

	//E_Rnd *= 0.00020f;    ///// Scaling of Error.. You might want to increase this.
	//E_Rnd *= 0.00014f;    ///// Scaling of Error.. You might want to increase this.

	//	if(m_bCueError == TRUE)
	//		E_Rnd *= 0.00018f;    ///// Scaling of Error.. You might want to increase this.
	//	else
	E_Rnd *= 0.00012f; //0.00018f;    ///// Scaling of Error.. You might want to increase this.

	E_Rnd *= 1.5f;

	E_Rnd *= 0.6f;//0.75f;

	/////////////////////////////////////////////////////////////////		
	// for thin cuts, see (***) above.
	fRnd = (float)((rand() % 50)*neg) * 0.00008f;
	if (!bWithRnd) fRnd = (float)(50 * neg) * 0.00008f;
	float cutError = fRnd * angleError;

	cutError = cutError * 1.2f;////1.25f;//2.8f;//2.5f;


	/////////////////////////////////////////////////////////////////		
	// for shots near cushions
	CVector StrikekDir = m_testBall->m_VOld.Unit();
	float cushionError = m_ball[m_selectBall]->CushionError(StrikekDir);
	////	cushionError = cushionError * E_Rnd; // OLD WAY!
	//	fRnd = ((rand()%100)-50) * 0.00008;
	fRnd = (float)((rand() % 50)*neg) * 0.00008f;
	if (!bWithRnd) fRnd = (float)(50 * neg) * 0.00008f;
	//if(!bWithRnd) fRnd = (float)((rand()%50)*neg) * 0.00008f;
	cushionError = cushionError * fRnd;

	cushionError = cushionError * 0.6f;//2.0f;


	/////////////////////////////////////////////////////////////////		
	// for shots where white is hampered by other balls (Direct-line)
	//	CVector StrikekDir = m_testBall->m_VOld.Unit();
	float hError = 0;
	float hamperedByBallError = 0;

	int t;
	for (t = 0; t<MAX_TOTALBALLS; t++)
	if (t != m_selectBall)
	{
		//hError = m_ball[t]->HamperedByBallError(m_ball[m_selectBall], StrikekDir);
		hError = m_ball[t]->HamperedByBallError(m_ball[m_selectBall], m_TestBallCollideVA);
		if (hError > hamperedByBallError)
			hamperedByBallError = hError;
	}

	fRnd = (float)((rand() % 50)*neg) * 0.00008f;
	if (!bWithRnd) fRnd = (float)(50 * neg) * 0.00008f;
	hamperedByBallError = hamperedByBallError * fRnd;

	hamperedByBallError *= 0.1f;//0.75f;//1.5f;

	if (m_cueError == 2)
		hamperedByBallError = hamperedByBallError * 0.6f;//0.8f;//1.68f; //1.4;//2.0f;
	else if (m_cueError == 1)
		hamperedByBallError = hamperedByBallError * 0.6f;//2.0f;
	else
		hamperedByBallError = 0;


	/////////////////////////////////////////////////////////////////		
	// for raising the butt of the cue
	//	//float raisedButtError = E_Rnd * m_cuetip[2] * 3.5f; // OLD WAY! (extra error)
	//	fRnd = ((rand()%100)-50) * 0.00008;
	fRnd = (float)((rand() % 50)*neg) * 0.00008f;
	if (!bWithRnd) fRnd = (float)(50 * neg) * 0.00008f;
	float raisedButtError = m_cuetip[2] * fRnd * 2.5f;//3.5f;

	/////////////////////////////////////////////////////////////////		
	// power error factor

	float powerErrorFactor = 1.0f;
	//	if(m_bCueError == TRUE)
	powerErrorFactor = 1.40 + (float)(m_testBallInitialSpeed / BALL_MAXSPEED)*0.4; // range (140% - 180% * OverallError )	
	//	else
	//		powerErrorFactor = 0.80 + (float)(m_testBallInitialSpeed / BALL_MAXSPEED)*0.8; // range (80% - 160% * OverallError )	
	/////////////////////////////////////////////////////////////////

	/////////////////////////////////////////////////////////////////	
	// add on more extra
	//	if(m_bCueError == TRUE)
	{

		E_Rnd = E_Rnd * powerErrorFactor * 0.8f;//0.7f;
		//E_Rnd = E_Rnd + cutError * powerErrorFactor * 0.6; // v1.6.2
		E_Rnd = E_Rnd + cutError * powerErrorFactor * 0.6 * 1.4 * 1.82 * 1.20; // +20% ver 1.6.6b

		E_Rnd = E_Rnd / longAngleErrorFactor; // [NEW]

		//float longCushErrorFactor = m_strikeDist / (BALL_RADIUS * 2.0f * 9.0f); // 15 balls distance		
		float longCushErrorFactor = (m_strikeDist / (BALL_RADIUS * 2.0f * 18.0f)); // 15 balls distance		
		if (longCushErrorFactor < 1) longCushErrorFactor = 1;
		if (longCushErrorFactor > 3) longCushErrorFactor = 3;

		E_Rnd = E_Rnd + cushionError * powerErrorFactor * 0.7f * 0.75 * 0.5 / longCushErrorFactor;//0.5;//1.2f; // *2.0f
		E_Rnd = E_Rnd + raisedButtError * powerErrorFactor * 0.75 / longCushErrorFactor;// * 0.8;	

		E_Rnd = E_Rnd + hamperedByBallError * powerErrorFactor * 1.2;

		E_Rnd *= 1.9;//1.6;
		E_Rnd *= 1.5;

		E_Rnd *= 1.6;//1.2;

	}

	if (bWithRnd)
	{
		float multiplier1 = 0.01f * (float)((rand() % 100) + 1); // 1-100% multiplier
		//float multiplier2 = 0.01f * (float)((rand()%100)+1); // 1-100% multiplier

		float multiplier = 0;
	
		multiplier = multiplier1;

		//if((rand()%2))
		if (multiplier < 0.25)
			multiplier *= 2;


		//multiplier  = 1;

		int sign = rand() % 2;
		if (!sign) E_Rnd = -E_Rnd;

		E_Rnd = E_Rnd * multiplier;

	
	}

#ifdef SHOW_ERRORVALUE	
	char buf[80];
	sprintf(buf, "E_Rnd = %f", E_Rnd);
	ErrorMessage(m_parent, buf);
#endif //SHOW_ERRORVALUE


	return E_Rnd;
	//////////////////////////////////////////////////////
}
#endif // !THE_POOL_CLUB



#ifndef THE_POOL_CLUB


// Latest Error Code (2.2.43 and after)
float CSnooker::GetStrikeBallErrorB(BOOL bWithRnd) // NOTE: this function is not used!!!
{
	bool bRandom = false;
	if (bWithRnd) bRandom = true;

	// From now on, process maximum error always.. and bRandom will tell use what percentage to take for the error
	bWithRnd = false;

	// Calculate Striking Error value
	srand(m_rnd);
	int rnd = (rand() % 100) - 50;
	int neg;

	// ***************** override it!
	if (!bWithRnd)
		rnd = 50;
	// ******************************

	rnd = 50;

	///////////////////////////////////////////////////////////////////////
	// find out whether initial random value is 'negative' or 'positive'
	if (rnd == 0) {
		neg = rand() % 2;
		if (!neg) neg = -1;
	}
	else if (rnd < 0) neg = -1;
	else neg = 1;
	///////////////////////////////////////////////////////////////////////

	float fRnd;
	////////////////////////////////////////////////////

	m_testBall->m_VOld = m_testBall->m_V;


	CVector Adir = m_TestBallCollideVA.Unit(); // white balls initial velocity
	CVector Bdir = m_TestBallCollideVB.Unit(); // object balls velocity just after collision

	float angleError;
	angleError = Adir.Dot(Bdir);
	angleError = 1.0f - angleError;

	
	

	// ------- [*NEW - ver V1.6.7d] ---------	
	float softCloseThinShotError = 0.0f;

	if (m_cueError >= 1) // semi-pro, pro
	{
		float dd = m_strikeDist / (BALL_RADIUS*2.0f); // in no. of balls
		float angErr = angleError;
		float powErr = (float)(m_testBallInitialSpeed / BALL_MAXSPEED);

		if (dd < 0.1f || powErr < 0.01f)
			softCloseThinShotError = 0.0f;
		else
			softCloseThinShotError = (1 / powErr) * angErr * (1 / dd);

		if (softCloseThinShotError < 0.0f) softCloseThinShotError = 0.0f;
		if (softCloseThinShotError > 50.0f) softCloseThinShotError = 50.0f;

		softCloseThinShotError /= 10;
		softCloseThinShotError += 1.0f;

	}

	float longMiddleAngleErrorFactor = 1.0f;
	if (m_cueError >= 1) // semi-pro, pro
	{
		float val = angleError - 0.3;
		if (val < 0) val = -val;

		val = 1 - val; // 1..0.3
		val -= 0.4;

		float longdist = m_strikeDist / (BALL_RADIUS * 2.0f * 15.0f); // 15 balls distance
		longdist /= 3;
		val *= longdist;

		val *= 0.75;

		// val *= 0.87; ///<---

		val += 1.0;

		longMiddleAngleErrorFactor = val;



		//longMiddleAngleErrorFactor = //(1.03*longAngleErrorFactor);
	}
	// --------------------------------------

	/*
	char buffer[180];
	sprintf(buffer, "m_strikeDist: %f, angleErro: %f", m_strikeDist, angleError);
	ErrorMessage(m_parent, buffer);
	*/



	float longAngleErrorFactor = m_strikeDist / (BALL_RADIUS * 2.0f * 15.0f); // 15 balls distance
	longAngleErrorFactor *= angleError;
	
	// Safety Check, as we don't want to raise a negative number to the power of 1.2...
	if (longAngleErrorFactor < 0) longAngleErrorFactor = 0;

	longAngleErrorFactor = pow(longAngleErrorFactor, 1.2f);

	if (longAngleErrorFactor < 1) longAngleErrorFactor = 1;
	if (longAngleErrorFactor > 5) longAngleErrorFactor = 5;
	float distanceError = 1.0f;
	
	
	// Near Shots have less error..

	float nearDistanceReduction = 1.0f;
	float distance;
	
	distance = m_strikeDist;
	if (distance < (BALL_RADIUS * 2.0f)) distance = BALL_RADIUS * 2.0f;
	
	if (distance < 1000)
	{
		nearDistanceReduction = (BALL_RADIUS * 2.0f) / distance; // aprox Range of 0.01 to 1.0f
		if (nearDistanceReduction < 0) nearDistanceReduction = 0;
		else if (nearDistanceReduction > 1.0) nearDistanceReduction = 1.0;
		else;

		nearDistanceReduction += 1.0f; // range of 1.1 to 2.0;
		// nearDistanceReduction = pow(nearDistanceReduction, 0.5);
		if (nearDistanceReduction > 1.2f) nearDistanceReduction = 1.2f;
	}

	

	// For near shots, don't limit the distance Error more than 0.9

	// if (distanceError < 0.9) distanceError = 0.9;
	


	
	float dd = m_strikeDist;
	/*
	char buffer[180];
	sprintf(buffer, "nearDistanceReduction: %f, longAngleErrorFactor: %f", nearDistanceReduction, longAngleErrorFactor);
	ErrorMessage(m_parent, buffer);
	*/

	if (dd == 0) angleError = 0; // no error if they are super close (what abt touching ball!? OAP)
	else
	{
		dd = (BALL_RADIUS * 14) / dd;


		//if(m_cueError == 2) // pro-level
		//	if(dd > 1)
		//		dd *= 5.0;

		angleError *= dd;
		angleError *= 1.6;//1.8;

		if (angleError > 1.0)
			angleError *= angleError;

		if (m_cueError >= 1)
		if (angleError > 5) angleError = 5;
	}


	if (m_cueError >= 1)
		angleError *= 1.5;


	// ------- [*NEW - ver V1.6.7d] ---------
	if (m_cueError >= 1) // semi-pro, pro
	{
		angleError *= longMiddleAngleErrorFactor;//(1.03*longAngleErrorFactor);

		// *NEW
		angleError /= softCloseThinShotError;
	}
	// --------------------------------------

	

	//char buffer[80];
	//sprintf(buffer, "End: angleError = %f", angleError);
	//ErrorMessage(m_parent, buffer);


	/////////////////////////////////////////////////////////////////

	// angle error for shot!!!!

	/////// Carl / Odin ///////
	/////// I Rem'ed out your Error calculation and put in mine.. 
	/////// It now works on a Normal (Gaussian Distribution) with 
	/////// S.D = 1, Mean = 0;
	/////// I take your rnd parammeter and divide by 60 to make the range of X about -0.8 to +0.8
	/////// So this is just under 1 standard deviation from mean..
	/////// I do 1/Gaussian_Value(X) & negate if the value of X is less than 0 (the mean) or Gaussian
	/////// the final error is scaled as u did before.. 



	// add an error to the cue strike C
	// [#] C = C.RotateXY(((float)rnd)*0.000035);


	/////// Rotate Angle using Gaussian Distrition so more concentrated error at small deviation
	/////// Less frequent large error deviations from 0.
	float E_Mean = 0.0f;
	float E_Sigma = 1.0f;
	float E_Var;
	float E_X;
	float E_Rnd;
	float a, b, c;


	E_X = rnd;
	E_X = E_X / 60.0f;	   ////(-1 to 1 S.)

	/// Note at 4 Sigma the Gaussian value will be very low - les than 0.5 percent

	E_Rnd = (float)rnd;
	E_Var = E_Sigma * E_Sigma;


	////// First part of Gaussian Equation //////
	a = 5.0f;
	a = a*3.1415f;
	a = sqrt(a);
	a = a * E_Sigma;
	a = 1 / a;

	////// Second part of Gaussian Equation //////
	b = E_X - E_Mean;
	b = b*b;//-(b*b);
	b = -b;// "
	b = b / 2;
	b = b / E_Var;

	////// Evaluate Gaussian ///////////
	E_Rnd = exp(b);
	E_Rnd = E_Rnd * a;

	if (E_X<0) E_Rnd = -E_Rnd;
	E_Rnd = 1 / E_Rnd;



	//if(m_cueError == 3)
	//	E_Rnd *= 0.00018f;//E_Rnd *= 0.00018f;    ///// Scaling of Error.. You might want to increase this.
	if (m_cueError == 2)
		E_Rnd *= 0.00018f;//E_Rnd *= 0.00018f;    ///// Scaling of Error.. You might want to increase this.
	else if (m_cueError == 1)
		E_Rnd *= 0.00018f;//E_Rnd *= 0.00018f;    ///// Scaling of Error.. You might want to increase this.
	else
		E_Rnd = 0;


	



	/////////////////////////////////////////////////////////////////		
	// for thin cuts, see (***) above.
	fRnd = (float)((rand() % 50)*neg) * 0.00008f;
	if (!bWithRnd) fRnd = (float)(50 * neg) * 0.00008f;
	float cutError = fRnd * angleError;


	if (m_cueError == 2)
		cutError = cutError * 1.2f;
	else if (m_cueError == 1)
		cutError = cutError * 1.2f;
	else
		cutError = 0;




	/////////////////////////////////////////////////////////////////		
	// for shots near cushions
	CVector StrikekDir = m_testBall->m_VOld.Unit();
	float cushionError = m_ball[m_selectBall]->CushionError(StrikekDir, BALL_RADIUS*12.0f);


	fRnd = (float)((rand() % 50)*neg) * 0.00008f;
	if (!bWithRnd) fRnd = (float)(50 * neg) * 0.00008f;
	cushionError = cushionError * fRnd;

	// ------- [*NEW - ver V1.6.7d] ---------
	if (m_cueError >= 1) // semi-pro, pro
	{
		cushionError = cushionError * 0.6; //*NEW since Ver.1.6.7c[beta]
	}
	// --------------------------------------

	//if(m_cueError == 3)	
	//	cushionError = cushionError * 0.6f;//0.8f;//1.68f; //1.4;//2.0f;
	if (m_cueError == 2)
		cushionError = cushionError * 0.6f; // 0.6f
	else if (m_cueError == 1)
		cushionError = cushionError * 0.6f;	// 0.6f;
	else
		cushionError = 0;




	/////////////////////////////////////////////////////////////////		
	// for shots where white is hampered by other balls (Direct-line)
	//	CVector StrikekDir = m_testBall->m_VOld.Unit();
	float hError = 0;
	float hamperedByBallError = 0;

	int t;
	for (t = 0; t<MAX_TOTALBALLS; t++)
	if (t != m_selectBall)
	{
		//hError = m_ball[t]->HamperedByBallError(m_ball[m_selectBall], StrikekDir);
		hError = m_ball[t]->HamperedByBallError(m_ball[m_selectBall], m_TestBallCollideVA);
		if (hError > hamperedByBallError)
			hamperedByBallError = hError;
	}

	fRnd = (float)((rand() % 50)*neg) * 0.00008f;
	if (!bWithRnd) fRnd = (float)(50 * neg) * 0.00008f;
	hamperedByBallError = hamperedByBallError * fRnd;



		//if(m_cueError == 3)	
		//	hamperedByBallError = hamperedByBallError * 0.6f;//0.8f;//1.68f; //1.4;//2.0f;

	if (m_cueError == 2)
		hamperedByBallError = hamperedByBallError * 0.8f;//0.8f;//1.68f; //1.4;//2.0f;
	else if (m_cueError == 1)
		hamperedByBallError = hamperedByBallError * 1.1f;//2.0f;
	else
		hamperedByBallError = 0;
	
	//	{
	//		char buf[256];
	//		sprintf(buf, "hamperedByBallError = %f", hamperedByBallError);
	//		ErrorMessage(m_parent, buf);
	//	}



#ifndef THE_POOL_CLUB 

	// SNOOKER ONLY: Side Spin Error. The More Side spin we apply, the more error needs to be added to the shot //
	float SideSpinError = 0.0f;

	SideSpinError = m_cuetip[0] / 1.10f;
	SideSpinError = SideSpinError * m_cuetip[0] / 1.10f;
	SideSpinError = SideSpinError * SideSpinError;

	float PowerFactor = (float)(m_testBallInitialSpeed / BALL_MAXSPEED);
	PowerFactor *= 0.04; // 0.02;
	SideSpinError = SideSpinError * PowerFactor;



#endif

	/////////////////////////////////////////////////////////////////		
	// for raising the butt of the cue
	//	//float raisedButtError = E_Rnd * m_cuetip[2] * 3.5f; // OLD WAY! (extra error)
	//	fRnd = ((rand()%100)-50) * 0.00008;
	fRnd = (float)((rand() % 50)*neg) * 0.00008f;
	if (!bWithRnd) fRnd = (float)(50 * neg) * 0.00008f;
	float raisedButtError = m_cuetip[2] * fRnd * 2.5f;//3.5f;

	/////////////////////////////////////////////////////////////////

	
	// ------- [*NEW - ver V1.6.7d] ---------
	float powerErrorFactor = 1.0f;
	if (m_cueError == 2) {
		powerErrorFactor = 2.00 + (float)(m_testBallInitialSpeed / BALL_MAXSPEED)*0.2; // range (140% - 180% * OverallError )	

		float upper = (float)(m_testBallInitialSpeed - (BALL_MAXSPEED*0.5));
		if (upper < 0) upper = 0;
		powerErrorFactor += (float)(upper / BALL_MAXSPEED)*0.3; // (50%power and above) add 0.0 - 0.5 more error

		float upper2 = (float)(m_testBallInitialSpeed - (BALL_MAXSPEED*0.75));
		if (upper2 < 0) upper2 = 0;
		powerErrorFactor += (float)(upper2 / BALL_MAXSPEED)*0.3; // (75%power and above) add 0.0 - 0.25 more error
	}
	else if (m_cueError == 1)
	{
		//powerErrorFactor = 0.80 + (float)(m_testBallInitialSpeed / BALL_MAXSPEED)*0.8; // range (80% - 160% * OverallError )	
		powerErrorFactor = 2.00 + (float)(m_testBallInitialSpeed / BALL_MAXSPEED)*0.2; // range (80% - 160% * OverallError )	

		float upper = (float)(m_testBallInitialSpeed - (BALL_MAXSPEED*0.5));
		if (upper < 0) upper = 0;
		powerErrorFactor += (float)(upper / BALL_MAXSPEED)*0.3; // (50%power and above) add 0.0 - 0.5 more error

		float upper2 = (float)(m_testBallInitialSpeed - (BALL_MAXSPEED*0.75));
		if (upper2 < 0) upper2 = 0;
		powerErrorFactor += (float)(upper2 / BALL_MAXSPEED)*0.3; // (75%power and above) add 0.0 - 0.25 more error
	}
	/*
	else if(m_cueError == 1)
	{ // Amateur (old Semi-PRO)
	powerErrorFactor = 1.40 + (float)(m_testBallInitialSpeed / BALL_MAXSPEED)*0.4; // range (140% - 180% * OverallError )
	}
	*/
	else
		powerErrorFactor = 0;
	// --------------------------------------

	/////////////////////////////////////////////////////////////////

	/////////////////////////////////////////////////////////////////	
	// add on more extra
	if (m_cueError == 2)
	{
		E_Rnd = E_Rnd * powerErrorFactor * 0.4f;
		E_Rnd = E_Rnd + cutError * powerErrorFactor * 1.2;
		//E_Rnd = E_Rnd + cutError * powerErrorFactor * 0.6 * 2.0 * 2.0 * 0.75 * 0.9;

		E_Rnd = E_Rnd * 1.2;
		E_Rnd = E_Rnd / longAngleErrorFactor;

		E_Rnd = E_Rnd + raisedButtError * powerErrorFactor * 0.85;
		E_Rnd = E_Rnd + cushionError * powerErrorFactor * 1.2;

		E_Rnd = E_Rnd + hamperedByBallError * powerErrorFactor * 0.4;

		E_Rnd = E_Rnd + SideSpinError * 0.5;			// New Error for Side Spin (Added 6th March 07)


		E_Rnd *= 0.72;//0.70;//0.77;//0.88;//0.93; // -5% from 93%

		E_Rnd /= nearDistanceReduction;

		if (bRandom)
		{
			// if longAngleErrorFactor > 1.0 (this is for long thin cuts, lets make sure our accuracy is 0 - 0.5
			float multiplier1;

			if (longAngleErrorFactor > 1.1) multiplier1 = 0.01f * (float)((rand() % 50) + 1); // 1-50% multiplier
				else multiplier1 = 0.01f * (float)((rand() % 100) + 1); // 1-100% multiplier

			//float multiplier2 = 0.01f * (float)((rand()%100)+1); // 1-100% multiplier

			float multiplier = 0;
			/*
			if(!(rand()%3)) // 33% of the time...
			{
			// ...take the lowest of two random error values (between 1-100)
			if(multiplier1 < multiplier2)
			multiplier = multiplier1;
			else
			multiplier = multiplier2;
			}
			else
			{
			multiplier = multiplier1; // 67% of the time just take the first random error 1-100
			}
			*/

			multiplier = multiplier1;

			//if((rand()%2))
			if (multiplier < 0.25)
				multiplier *= 2;


			int sign = rand() % 2;
			if (!sign) E_Rnd = -E_Rnd;

			E_Rnd = E_Rnd * multiplier;

			//char buf[80];
			//sprintf(buf, "[multiplier = %f]",multiplier);
			//ErrorMessage(m_parent, buf);
		}
	}
	else if (m_cueError == 1)
	{
		E_Rnd = E_Rnd * powerErrorFactor * 0.4f;
		//E_Rnd = E_Rnd + cutError * powerErrorFactor * 0.6 * 2.0 * 2.0;
		E_Rnd = E_Rnd + cutError * powerErrorFactor * 1.2; // 1.62

		E_Rnd = E_Rnd * 1.2;
		E_Rnd = E_Rnd / longAngleErrorFactor;

		E_Rnd = E_Rnd + raisedButtError * powerErrorFactor * 0.85;
		E_Rnd = E_Rnd + cushionError * powerErrorFactor * 1.2;

		E_Rnd = E_Rnd + hamperedByBallError * powerErrorFactor * 0.4;

		E_Rnd = E_Rnd + SideSpinError * 0.5;			// New Error for Side Spin (Added 6th March 07)

		E_Rnd *= 0.55;

		E_Rnd /= nearDistanceReduction;


		if (bRandom)
		{
			// if longAngleErrorFactor > 1.0 (this is for long thin cuts, lets make sure our accuracy is 0 - 0.5
			float multiplier1;

			if (longAngleErrorFactor > 1.0) multiplier1 = 0.01f * (float)((rand() % 50) + 1); // 1-50% multiplier
			else multiplier1 = 0.01f * (float)((rand() % 100) + 1); // 1-100% multiplier

			
			//float multiplier2 = 0.01f * (float)((rand()%100)+1); // 1-100% multiplier

			float multiplier = 0;
			/*
			if(!(rand()%3)) // 33% of the time...
			{
			// ...take the lowest of two random error values (between 1-100)
			if(multiplier1 < multiplier2)
			multiplier = multiplier1;
			else
			multiplier = multiplier2;
			}
			else
			{
			multiplier = multiplier1; // 67% of the time just take the first random error 1-100
			}
			*/

			multiplier = multiplier1;

			//if((rand()%2))
			if (multiplier < 0.25)
				multiplier *= 2;


			int sign = rand() % 2;
			if (!sign) E_Rnd = -E_Rnd;

			E_Rnd = E_Rnd * multiplier;

			/*
			// dirty bug 
			// This gives a NaN value.. so when:P the strike ball uses this to rotate the x and y coords of cueball, it turns those float values to NaN also
			E_Rnd = sqrt(-1);
			// This was caused by code using pow('val', 1.2') when val is negative, you get NaN (can't do x^1.2 when x<0)...
			
			if (E_Rnd != E_Rnd)
			{
				ErrorMessage(m_parent, "Euston we have a problem!!!");
			}
			*/

			//char buf[80];
			//sprintf(buf, "[multiplier = %f]",multiplier);
			//ErrorMessage(m_parent, buf);
		}
	}

	/*
	else if(m_cueError == 1)
	{
	E_Rnd = E_Rnd * powerErrorFactor * 0.8f;
	//E_Rnd = E_Rnd + cutError * powerErrorFactor * 0.6 * 2.0 * 2.0;
	E_Rnd = E_Rnd + cutError * powerErrorFactor * 0.6 * 2.0 * 2.0 * 0.75 * 0.9; // <--- NEW 0.75 * 0.8

	E_Rnd = E_Rnd * 1.2;
	E_Rnd = E_Rnd / longAngleErrorFactor;

	E_Rnd = E_Rnd + raisedButtError * powerErrorFactor * 0.85;
	E_Rnd = E_Rnd + cushionError * powerErrorFactor * 1.2;

	E_Rnd = E_Rnd + hamperedByBallError * powerErrorFactor * 1.2;

	E_Rnd *= 0.55;

	if(bWithRnd)
	{
	float multiplier1 = 0.01f * (float)((rand()%100)+1); // 1-100% multiplier
	//float multiplier2 = 0.01f * (float)((rand()%100)+1); // 1-100% multiplier

	float multiplier = 0;
	/ *
	if(!(rand()%3)) // 33% of the time...
	{
	// ...take the lowest of two random error values (between 1-100)
	if(multiplier1 < multiplier2)
	multiplier = multiplier1;
	else
	multiplier = multiplier2;
	}
	else
	{
	multiplier = multiplier1; // 67% of the time just take the first random error 1-100
	}
	* /

	multiplier = multiplier1;

	//if((rand()%2))
	if(multiplier < 0.25)
	multiplier *= 2;


	int sign = rand()%2;
	if(!sign) E_Rnd = -E_Rnd;

	E_Rnd = E_Rnd * multiplier;

	//char buf[80];
	//sprintf(buf, "[multiplier = %f]",multiplier);
	//ErrorMessage(m_parent, buf);
	}
	}
	*/
	else
	{
		E_Rnd = 0;
	}
	/*

	{
		char buffer[180];
		sprintf(buffer, "E_Rnd = %f", E_Rnd);
		ErrorMessage(m_parent, buffer);
	}
	*/

	//m_strikeRnd = E_Rnd;

	//////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////	
	// add on more extra
	//E_Rnd = E_Rnd + cutError;
	//E_Rnd = E_Rnd + cushionError;

	//m_strikeRnd = E_Rnd;


#ifdef SHOW_ERRORVALUE
	if (bWithRnd)
	{
		char bbuffer[80];
		sprintf(bbuffer, "E_Rnd = %f", E_Rnd);
		ErrorMessage(m_parent, bbuffer);
	}
#endif //SHOW_ERRORVALUE

	return E_Rnd;
	//////////////////////////////////////////////////////
}


#else //THE_POOL_CLUB

float CSnooker::GetStrikeBallErrorB(BOOL bWithRnd) // NOTE: this function is not used!!!
{
	// calc striking error value
	//	int rnd = (m_rnd%100)-50;  // <-- OLD WAY!!!!!
	srand(m_rnd);
	int rnd = (rand() % 100) - 50;
	int neg;


	// ***************** override it!
	if (!bWithRnd)
		rnd = 50;
	// ******************************

	rnd = 50;

	///////////////////////////////////////////////////////////////////////
	// find out whether initial random value is 'negative' or 'positive'
	if (rnd == 0) {
		neg = rand() % 2;
		if (!neg) neg = -1;
	}
	else if (rnd < 0) neg = -1;
	else neg = 1;
	///////////////////////////////////////////////////////////////////////



	float fRnd;
	////////////////////////////////////////////////////

	m_testBall->m_VOld = m_testBall->m_V;

	/////////////////////////////////////////////////////////////////
	// angleError for thin cuts (***)
	//CVector Adir = m_testBall->m_VOld.Unit();//m_V.Unit(); <- not calced. yet
	//CVector Bdir = m_testBall->m_V.Unit();

	CVector Adir = m_TestBallCollideVA.Unit(); // white balls initial velocity
	CVector Bdir = m_TestBallCollideVB.Unit(); // object balls velocity just after collision

	float angleError;
	angleError = Adir.Dot(Bdir);
	angleError = 1.0f - angleError;
	angleError = angleError*1.0f; // *NEW - added 20% more at 25/9/2003

	float longAngleErrorFactor = m_strikeDist / (BALL_RADIUS * 2.0f * 9.0f); // 15 balls distance
	longAngleErrorFactor *= angleError;
	if (longAngleErrorFactor < 1) longAngleErrorFactor = 1;
	if (longAngleErrorFactor > 3) longAngleErrorFactor = 3;

	//char bbuffer[80];
	//sprintf(bbuffer, "Before: angleError = %f", angleError);
	//ErrorMessage(m_parent, bbuffer);


	/*
	char buffer1[80];
	sprintf(buffer1, "m_VOldx =  %f, m_VOldy =  %f",Adir.x, Adir.y);
	ErrorMessage(m_parent, buffer1);
	sprintf(buffer1, "m_Vx =  %f, m_Vy =  %f",Bdir.x, Bdir.y);
	ErrorMessage(m_parent, buffer1);
	*/

	// include dist into angle Error (i.e. close cuts are harder now)
	//CVector DD = m_testBall->m_P - m_testBall->m_POld; // dist between object-ball and cue-ball
	//float dd = DD.Magnitude();
	float dd = m_strikeDist;
	//char buffer[180];
	//sprintf(buffer, "angleError = %f : %f, (BALL_RADIUS*2 = %f)", angleError, m_strikeDist, BALL_RADIUS*2);


	if (dd == 0)
	{
		angleError = 0; // no error if they are super close (what abt touching ball!? OAP)
	}
	else
	{
		//dd = (BALL_RADIUS*14) / dd;
		dd = (BALL_RADIUS * 7) / dd;
		angleError *= dd;
		angleError *= 1.6;//1.8;

		if (angleError > 1.0)
			angleError *= angleError;

		//if(angleError > 20) angleError = 20; //*OLD FROM 25/9/2003
		//if(angleError > 10) angleError = 10;

		//if(angleError > 12) angleError = 12; // <-WAS
		if (angleError > 6) angleError = 6; // *NEW* 22/12/2003
	}

	//angleError *= 0.6; // 1st 1.6.1
	//	if(m_bCueError == FALSE)
	//	{
	//		angleError *= 0.25; // latest v1.6.2
	//	}
	//angleError = 0;  //v1.6

	/*
	if(dd==0) angleError = 0; // no error if they are super close (what abt touching ball!? OAP)
	else
	{
	//dd = (BALL_RADIUS*14) / dd;
	dd = (BALL_RADIUS*14) / dd;

	if(dd > 1.0f)
	{
	dd*=2.5;
	angleError *= dd;
	//angleError = 10;
	}
	//else angleError = 0;

	}

	//angleError *= 1.35;//1.5;

	angleError *= 0.1f;
	*/

	//char buffer[80];
	//sprintf(buffer, "End: angleError = %f", angleError);
	//ErrorMessage(m_parent, buffer);


	/////////////////////////////////////////////////////////////////

	// angle error for shot!!!!

	/////// Carl / Odin ///////
	/////// I Rem'ed out your Error calculation and put in mine.. 
	/////// It now works on a Normal (Gaussian Distribution) with 
	/////// S.D = 1, Mean = 0;
	/////// I take your rnd parammeter and divide by 60 to make the range of X about -0.8 to +0.8
	/////// So this is just under 1 standard deviation from mean..
	/////// I do 1/Gaussian_Value(X) & negate if the value of X is less than 0 (the mean) or Gaussian
	/////// the final error is scaled as u did before.. 



	// add an error to the cue strike C
	// [#] C = C.RotateXY(((float)rnd)*0.000035);
	/*
	float fRnd;
	fRnd = (float)rnd;
	fRnd *= 0.0000030f;//0.000050f;//0.000038f;
	C = C.RotateXY(fRnd);
	*/

	/////// Rotate Angle using Gaussian Distrition so more concentrated error at small deviation
	/////// Less frequent large error deviations from 0.
	float E_Mean = 0.0f;
	float E_Sigma = 1.0f;
	float E_Var;
	float E_X;
	float E_Rnd;
	float a, b, c;


	E_X = rnd;
	E_X = E_X / 60.0f;	   ////(-1 to 1 S.)

	/// Note at 4 Sigma the Gaussian value will be very low - les than 0.5 percent

	E_Rnd = (float)rnd;
	E_Var = E_Sigma * E_Sigma;


	////// First part of Gaussian Equation //////
	a = 2.0f;
	a = a*3.1415f;
	a = sqrt(a);
	a = a * E_Sigma;
	a = 1 / a;

	////// Second part of Gaussian Equation //////
	b = E_X - E_Mean;
	b = b*b;//-(b*b);
	b = -b;// "
	b = b / 2;
	b = b / E_Var;

	////// Evaluate Gaussian ///////////
	E_Rnd = exp(b);
	E_Rnd = E_Rnd * a;

	if (E_X<0) E_Rnd = -E_Rnd;
	E_Rnd = 1 / E_Rnd;

	//E_Rnd *= 0.00020f;    ///// Scaling of Error.. You might want to increase this.
	//E_Rnd *= 0.00014f;    ///// Scaling of Error.. You might want to increase this.

	//	if(m_bCueError == TRUE)
	//		E_Rnd *= 0.00018f;    ///// Scaling of Error.. You might want to increase this.
	//	else
	E_Rnd *= 0.00012f; //0.00018f;    ///// Scaling of Error.. You might want to increase this.

	E_Rnd *= 1.5f;

	E_Rnd *= 0.6f;//0.75f;

	/////////////////////////////////////////////////////////////////		
	// for thin cuts, see (***) above.
	////	float cutError = E_Rnd * angleError; // OLD WAY!
	//	fRnd = ((rand()%100)-50) * 0.00008;
	fRnd = (float)((rand() % 50)*neg) * 0.00008f;
	if (!bWithRnd) fRnd = (float)(50 * neg) * 0.00008f;
	float cutError = fRnd * angleError;

	//	if(m_bCueError == TRUE)
	//		cutError = cutError * 1.2f;//1.5f;//0.7f;//2.2f;//2.4f; //2.0f//2.8f;//2.5f;
	//	else
	cutError = cutError * 1.2f;////1.25f;//2.8f;//2.5f;


	/////////////////////////////////////////////////////////////////


	/////////////////////////////////////////////////////////////////		
	// for shots near cushions
	CVector StrikekDir = m_testBall->m_VOld.Unit();
	float cushionError = m_ball[m_selectBall]->CushionError(StrikekDir);
	////	cushionError = cushionError * E_Rnd; // OLD WAY!
	//	fRnd = ((rand()%100)-50) * 0.00008;
	fRnd = (float)((rand() % 50)*neg) * 0.00008f;
	if (!bWithRnd) fRnd = (float)(50 * neg) * 0.00008f;
	//if(!bWithRnd) fRnd = (float)((rand()%50)*neg) * 0.00008f;
	cushionError = cushionError * fRnd;

	//	if(m_bCueError == TRUE)	
	//		cushionError = cushionError * 0.6f;//0.8f;//1.68f; //1.4;//2.0f;
	//	else
	cushionError = cushionError * 0.6f;//2.0f;


	/////////////////////////////////////////////////////////////////		
	// for shots where white is hampered by other balls (Direct-line)
	//	CVector StrikekDir = m_testBall->m_VOld.Unit();
	float hError = 0;
	float hamperedByBallError = 0;

	int t;
	for (t = 0; t<MAX_TOTALBALLS; t++)
	if (t != m_selectBall)
	{
		//hError = m_ball[t]->HamperedByBallError(m_ball[m_selectBall], StrikekDir);
		hError = m_ball[t]->HamperedByBallError(m_ball[m_selectBall], m_TestBallCollideVA);
		if (hError > hamperedByBallError)
			hamperedByBallError = hError;
	}

	fRnd = (float)((rand() % 50)*neg) * 0.00008f;
	if (!bWithRnd) fRnd = (float)(50 * neg) * 0.00008f;
	hamperedByBallError = hamperedByBallError * fRnd;

	hamperedByBallError *= 0.1f;//0.75f;//1.5f;

	if (m_cueError == 2)
		hamperedByBallError = hamperedByBallError * 0.6f;//0.8f;//1.68f; //1.4;//2.0f;
	else if (m_cueError == 1)
		hamperedByBallError = hamperedByBallError * 0.6f;//2.0f;
	else
		hamperedByBallError = 0;

	//	{
	//		char buf[256];
	//		sprintf(buf, "hamperedByBallError = %f", hamperedByBallError);
	//		ErrorMessage(m_parent, buf);
	//	}


	/////////////////////////////////////////////////////////////////		
	// for raising the butt of the cue
	//	//float raisedButtError = E_Rnd * m_cuetip[2] * 3.5f; // OLD WAY! (extra error)
	//	fRnd = ((rand()%100)-50) * 0.00008;
	fRnd = (float)((rand() % 50)*neg) * 0.00008f;
	if (!bWithRnd) fRnd = (float)(50 * neg) * 0.00008f;
	float raisedButtError = m_cuetip[2] * fRnd * 2.5f;//3.5f;

	/////////////////////////////////////////////////////////////////

	/////////////////////////////////////////////////////////////////		
	// power error factor
	//float powerErrorFactor = 0.50 + (float)(m_testBallInitialSpeed / BALL_MAXSPEED)*1.4; // range (50% - 190% * OverallError )
	//float powerErrorFactor = 0.70 + (float)(m_testBallInitialSpeed / BALL_MAXSPEED)*1.0; // range (70% - 170% * OverallError )	
	//float powerErrorFactor = 0.70 + (float)(m_testBallInitialSpeed / BALL_MAXSPEED)*0.9; // range (70% - 160% * OverallError ) ***OLD v1.6***
	//float powerErrorFactor = 0.80 + (float)(m_testBallInitialSpeed / BALL_MAXSPEED)*0.8; // range (80% - 160% * OverallError )	
	////float powerErrorFactor = 0.9 + (float)(m_testBallInitialSpeed / BALL_MAXSPEED)*0.7; // range (90% - 160% * OverallError )	
	//float powerErrorFactor = 1.40 + (float)(m_testBallInitialSpeed / BALL_MAXSPEED)*0.4; // range (140% - 180% * OverallError )	// NEW EXPERIMENT (SEPT. 2003)
	//float powerErrorFactor = 1.0f;

	float powerErrorFactor = 1.0f;
	//	if(m_bCueError == TRUE)
	powerErrorFactor = 1.40 + (float)(m_testBallInitialSpeed / BALL_MAXSPEED)*0.4; // range (140% - 180% * OverallError )	
	//	else
	//		powerErrorFactor = 0.80 + (float)(m_testBallInitialSpeed / BALL_MAXSPEED)*0.8; // range (80% - 160% * OverallError )	
	/////////////////////////////////////////////////////////////////

	/////////////////////////////////////////////////////////////////	
	// add on more extra
	//	if(m_bCueError == TRUE)
	{
		/* OLD WAY:  15th OCT 2003
		E_Rnd = E_Rnd * powerErrorFactor * 0.8f;//0.7f;
		//E_Rnd = E_Rnd + cutError * powerErrorFactor * 0.6; // v1.6.2
		E_Rnd = E_Rnd + cutError * powerErrorFactor * 0.6 * 1.4 * 1.25;

		E_Rnd = E_Rnd / longAngleErrorFactor; // [NEW]

		E_Rnd = E_Rnd + cushionError * powerErrorFactor * 0.7f;//1.2f; // *2.0f
		E_Rnd = E_Rnd + raisedButtError * powerErrorFactor;// * 0.8;

		E_Rnd *= 1.5;

		E_Rnd *= 1.9;//1.6;

		*/


		E_Rnd = E_Rnd * powerErrorFactor * 0.8f;//0.7f;
		//E_Rnd = E_Rnd + cutError * powerErrorFactor * 0.6; // v1.6.2
		E_Rnd = E_Rnd + cutError * powerErrorFactor * 0.6 * 1.4 * 1.82 * 1.20; // +20% ver 1.6.6b

		E_Rnd = E_Rnd / longAngleErrorFactor; // [NEW]

		//float longCushErrorFactor = m_strikeDist / (BALL_RADIUS * 2.0f * 9.0f); // 15 balls distance		
		float longCushErrorFactor = (m_strikeDist / (BALL_RADIUS * 2.0f * 18.0f)); // 15 balls distance		
		if (longCushErrorFactor < 1) longCushErrorFactor = 1;
		if (longCushErrorFactor > 3) longCushErrorFactor = 3;

		E_Rnd = E_Rnd + cushionError * powerErrorFactor * 0.7f * 0.75 * 0.5 / longCushErrorFactor;//0.5;//1.2f; // *2.0f
		E_Rnd = E_Rnd + raisedButtError * powerErrorFactor * 0.75 / longCushErrorFactor;// * 0.8;	

		E_Rnd = E_Rnd + hamperedByBallError * powerErrorFactor * 1.2;

		E_Rnd *= 1.9;//1.6;
		E_Rnd *= 1.5;

		// E_Rnd *= 1.6; // [14th Apr 2012]
		E_Rnd *= 1.5; // 

		/*
		if(bWithRnd)
		{
		char buf[80];
		sprintf(buf, "longCushErrorFactor = %f", longCushErrorFactor);
		ErrorMessage(m_parent, buf);
		}
		*/


		//if(E_Rnd > 0.02512) E_Rnd = 0.02512;

	}
	//	else
	//	{
	//		E_Rnd = E_Rnd * powerErrorFactor * 0.8f;//0.7f;
	//		E_Rnd = E_Rnd + cutError * powerErrorFactor * 0.6;
	//		E_Rnd = E_Rnd + cushionError * powerErrorFactor * 0.7f;//1.2f; // *2.0f
	//		E_Rnd = E_Rnd + raisedButtError * powerErrorFactor;// * 0.8;	
	//	}
	//m_strikeRnd = E_Rnd;

	//////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////	
	// add on more extra
	//E_Rnd = E_Rnd + cutError;
	//E_Rnd = E_Rnd + cushionError;

	//m_strikeRnd = E_Rnd;


	if (bWithRnd)
	{
		float multiplier1 = 0.01f * (float)((rand() % 100) + 1); // 1-100% multiplier
		//float multiplier2 = 0.01f * (float)((rand()%100)+1); // 1-100% multiplier

		float multiplier = 0;
		/*
		if(!(rand()%3)) // 33% of the time...
		{
		// ...take the lowest of two random error values (between 1-100)
		if(multiplier1 < multiplier2)
		multiplier = multiplier1;
		else
		multiplier = multiplier2;
		}
		else
		{
		multiplier = multiplier1; // 67% of the time just take the first random error 1-100
		}
		*/

		multiplier = multiplier1;

		//if((rand()%2))
		if (multiplier < 0.25)
			multiplier *= 2;


		//multiplier  = 1;

		int sign = rand() % 2;
		if (!sign) E_Rnd = -E_Rnd;

		E_Rnd = E_Rnd * multiplier;

		/*
		char buf[80];
		if(E_Rnd < 0)
		sprintf(buf, "[multiplier = (-)%f]",multiplier);
		else
		sprintf(buf, "[multiplier = (+)%f]",multiplier);
		ErrorMessage(m_parent, buf);
		*/
	}

#ifdef SHOW_ERRORVALUE	
	char buf[80];
	sprintf(buf, "E_Rnd = %f", E_Rnd);
	ErrorMessage(m_parent, buf);
#endif //SHOW_ERRORVALUE


	return E_Rnd;
	//////////////////////////////////////////////////////
}
#endif // !THE_POOL_CLUB


void CSnooker::StrikeBallSend(BOOL bZeroPowerShot)
{
	
	// calc striking error value
	//m_strikeRnd = (m_rnd%100)-50;

	
	m_strikeMaxRnd = GetStrikeBallError(FALSE);
	m_strikeRnd = GetStrikeBallError(TRUE);

	//m_strikeMaxRnd = 0;//--
	//m_strikeRnd = 0;//--
		
	CVector U = CVector(0, 0, 0);

	// ------- [*NEW - ver V1.6.7d] ---------
	///////////////////////////////////////////////////////////////////////
	if(g_strikePowerError)
	if((g_build.gameType != GAMETYPE_POOL && m_cueError >= 1) || (g_build.gameType == GAMETYPE_POOL))
	{
		float fAddRandomStrikePower = (rand()%((g_strikePowerError*2000)+1000)) - g_strikePowerError*1000;
		fAddRandomStrikePower *=0.001;

		if(m_cuePowerPercentage <= 2)
		{
			if(fAddRandomStrikePower < 0) fAddRandomStrikePower = -fAddRandomStrikePower;
		}


		
		fAddRandomStrikePower /= 2; // do -0.5% - 0.5%
		if(g_build.gameType == GAMETYPE_POOL) 
			{
			// do -0.25% - 0.25% for iPool
			fAddRandomStrikePower /= 2;
			}
		else{
			// ------- [*NEW - ver V1.6.96 (8th Mar 2006)] ---------
			// iSnooker Power Error adjustments //
			// Shots in range of 0-20% , do normal Power error //
			// Shots in range of 20-35%, do normal Power error *1.6 //
			// Shots over 35% (Break Off Range) do normal Power error *2.2 //
			// if(m_cuePowerPercentage >= 20 && m_cuePowerPercentage < 35) fAddRandomStrikePower *= 1.6;
			// else if(m_cuePowerPercentage >= 35) fAddRandomStrikePower *= 2.2;
			}

		U.x = m_tableMousePos[0] - m_sPoint[0];
		U.y = m_tableMousePos[1] - m_sPoint[1];
		U.z = 0; //m_tableMousePos[2] - m_sPoint[2];
		U = U.Unit();
		U = U * (fAddRandomStrikePower/100);
		
		if(g_build.gameType == GAMETYPE_POOL)
			U = U * 1400.0f;
		else
			U = U * 1600.0f;

//		{
//			char buf[256];
//			sprintf(buf, "RandomPower = %f; U = (%f, %f, %f)", fAddRandomStrikePower, U.x, U.y, U.z);
//			ErrorMessage(g_wnd, buf);
//		}
	}
	///////////////////////////////////////////////////////////////////////
	// --------------------------------------

	// store values for later use:
	((CGameAppView *)m_parent)->m_strikeBallSendTmp.m_sPoint[0] = m_sPoint[0];
	((CGameAppView *)m_parent)->m_strikeBallSendTmp.m_sPoint[1]	= m_sPoint[1];
	((CGameAppView *)m_parent)->m_strikeBallSendTmp.m_sPoint[2] = m_sPoint[2];
	((CGameAppView *)m_parent)->m_strikeBallSendTmp.m_tableMousePos[0] = m_tableMousePos[0];
	((CGameAppView *)m_parent)->m_strikeBallSendTmp.m_tableMousePos[1] = m_tableMousePos[1];
	((CGameAppView *)m_parent)->m_strikeBallSendTmp.m_tableMousePos[2] = m_tableMousePos[2];

	((CGameAppView *)m_parent)->m_strikeBallSendTmp.m_tableMousePos[0] += U.x;
	((CGameAppView *)m_parent)->m_strikeBallSendTmp.m_tableMousePos[1] += U.y;
	((CGameAppView *)m_parent)->m_strikeBallSendTmp.m_tableMousePos[2] += U.z;

	((CGameAppView *)m_parent)->m_strikeBallSendTmp.m_cuetip[0] = m_cuetip[0];
	((CGameAppView *)m_parent)->m_strikeBallSendTmp.m_cuetip[1] = m_cuetip[1];
	((CGameAppView *)m_parent)->m_strikeBallSendTmp.m_cuetip[2] = m_cuetip[2];
	((CGameAppView *)m_parent)->m_strikeBallSendTmp.m_selectBall = m_selectBall;
	((CGameAppView *)m_parent)->m_strikeBallSendTmp.m_strikeRnd = m_strikeRnd;
	((CGameAppView *)m_parent)->m_strikeBallSendTmp.m_strikeMaxRnd = m_strikeMaxRnd;

	if(bZeroPowerShot)
	{
	((CGameAppView *)m_parent)->m_strikeBallSendTmp.m_sPoint[0] = 0;
	((CGameAppView *)m_parent)->m_strikeBallSendTmp.m_sPoint[1]	= 0;
	((CGameAppView *)m_parent)->m_strikeBallSendTmp.m_sPoint[2] = 0;
	((CGameAppView *)m_parent)->m_strikeBallSendTmp.m_tableMousePos[0] = 0;
	((CGameAppView *)m_parent)->m_strikeBallSendTmp.m_tableMousePos[1] = 0;
	((CGameAppView *)m_parent)->m_strikeBallSendTmp.m_tableMousePos[2] = 0;
	}

	m_cueBallSelect = 10;
	//	m_bStrikeSend = TRUE;
}


void CSnooker::UpdatePhysics(int type)
{
	if(g_build.gameType == GAMETYPE_SNOOKER)
	{
		switch(type)
		{
			case 0:
				// Old
				BALL_ENERGYFACTOR =	0.98f;
				BALL_ENERGYFACTOR_N = 0.835f;
				BALL_ENERGYFACTOR_T = 0.835f;
				BALL_CUSHIONFACTOR = 0.8f;
				BALL_ROLLINGFRICT = 0.0040f;
				BALL_ROLLINGFACTOR = BALL_ROLLINGFRICT / GAME_ITTR;
				break;

			case 1:
				// v2.2.02
				BALL_ENERGYFACTOR =	0.98f;
				BALL_ENERGYFACTOR_N = 0.91f;
				BALL_ENERGYFACTOR_T = 0.91f;
				BALL_CUSHIONFACTOR = 0.83;
				BALL_ROLLINGFRICT = 0.0025;
				BALL_ROLLINGFACTOR = BALL_ROLLINGFRICT / GAME_ITTR;
				break;

			case 2:
				// v2.2.00
				BALL_ENERGYFACTOR =	0.98f;
				BALL_ENERGYFACTOR_N = 0.88f;
				BALL_ENERGYFACTOR_T = 0.88f;
				BALL_CUSHIONFACTOR = 0.80f;
				BALL_ROLLINGFRICT = 0.0033f;
				BALL_ROLLINGFACTOR = BALL_ROLLINGFRICT / GAME_ITTR;
				break;

			case 3:
				// v2.2.10
				BALL_ENERGYFACTOR =	0.98f;
				BALL_ENERGYFACTOR_N = 0.85f;
				BALL_ENERGYFACTOR_T = 0.85f;
				BALL_CUSHIONFACTOR = 0.80f;
				BALL_ROLLINGFRICT = 0.0035f;
				BALL_ROLLINGFACTOR = BALL_ROLLINGFRICT / GAME_ITTR;
				break;

			case 4:
				// v2.3.01
				BALL_ENERGYFACTOR =	0.98f;
				BALL_ENERGYFACTOR_N = 0.85f;
				BALL_ENERGYFACTOR_T = 0.85f;
				BALL_CUSHIONFACTOR = 0.80f;
				BALL_ROLLINGFRICT = 0.0037f;
				BALL_ROLLINGFACTOR = BALL_ROLLINGFRICT / GAME_ITTR;
				break;
			case 5:
				// v2.2.40
				BALL_ENERGYFACTOR =	0.98f;
				BALL_ENERGYFACTOR_N = 0.88f;
				BALL_ENERGYFACTOR_T = 0.88f;
				BALL_CUSHIONFACTOR = 0.80f;
				BALL_ROLLINGFRICT = 0.00365f;// 0.0034f;// 0.00365f;
				BALL_ROLLINGFACTOR = BALL_ROLLINGFRICT / GAME_ITTR;
				break;
		}
	}
	else
	{
		switch(type)
		{
			case 0:
				BALL_ENERGYFACTOR =	0.98f;
				BALL_ENERGYFACTOR_N = 0.87f;
				BALL_ENERGYFACTOR_T = 0.87f;
				BALL_CUSHIONFACTOR = 0.72;
				BALL_ROLLINGFRICT = 0.0024;
				BALL_ROLLINGFACTOR = BALL_ROLLINGFRICT / GAME_ITTR;
				break;

			case 1:
			case 2:
			case 3:
			case 4:
				BALL_ENERGYFACTOR = 0.98f;
				BALL_ENERGYFACTOR_N = 0.90f;
				BALL_ENERGYFACTOR_T = 0.90f;
				BALL_CUSHIONFACTOR = 0.80;
				BALL_ROLLINGFRICT = 0.0026;
				BALL_ROLLINGFACTOR = BALL_ROLLINGFRICT / GAME_ITTR;
				break;
			case 5:
				BALL_ENERGYFACTOR =	0.98f;
				BALL_ENERGYFACTOR_N = 0.92f;
				BALL_ENERGYFACTOR_T = 0.92f;
				BALL_CUSHIONFACTOR = 0.72;
				BALL_ROLLINGFRICT = 0.0028;
				BALL_ROLLINGFACTOR = BALL_ROLLINGFRICT / GAME_ITTR;
				break;
		}
	}
}

void CSnooker::StrikeBall(void)
{
	

	if(!m_bStrikeBallReplayDrawSL)
	{
		// use current physics version settings
		GAMEAPPVIEW->m_replay->VersionSettings(FALSE);
	}

	// for undo
	SavePreviousPosition();

	// carry-out strike...
	VOut[4] = m_ball[0]->m_V;//++
	m_ball[m_selectBall]->Strike(m_sPoint[0], m_sPoint[1], m_sPoint[2], 
								m_tableMousePos[0], m_tableMousePos[1], m_sPoint[2],
								m_cuetip, m_strikeRnd, //
								FALSE);
	VOut[5] = m_ball[0]->m_V;//++

	// clear touching balls flags
	m_touchingBallIdx = 0;
	for(int t=0; t<MAX_TOUCHING_BALLS; t++)
		m_touchingBall[t] = 0;			

	// init domino ball to ball hit chain (start off chain with ball to be hit by cue)
	//ClearFlag(m_ball[m_dominoHitLast]->m_flags, Flg_BALL_DOMINOHITLAST); // MAYBE WE SHOULD CLEAR ALL BALLS, JUST TO BE SAFE!!!
	int n;
	for(n=0;n<MAX_TOTALBALLS;n++)
		ClearFlag(m_ball[n]->m_flags, Flg_BALL_DOMINOHITLAST);

	m_dominoHitLast = m_selectBall;//0;
	m_dominoHit1st = m_selectBall;// was 0;	
	SetFlag(m_ball[m_dominoHitLast]->m_flags, Flg_BALL_DOMINOHITLAST);
}

void CSnooker::StrikeBallReplay(CReplayStrikeBall *rstrike) 
{
	
	g_bTextDoubleErrorAdded = FALSE;

	// use current replay physics version settings
	GAMEAPPVIEW->m_replay->VersionSettings(TRUE);

	// cue angle in replay is always wrong! BUGFIX!
	// *TODO - note also: the exit traj. colour lines are messed up too
	m_cueAngle = atan2((double)(rstrike->m_e[1] - rstrike->m_s[1]), (double)(rstrike->m_e[0] - rstrike->m_s[0]));

	m_bStrikeBallReplayDrawSL = 1;

	StrikeBallReceive(rstrike->m_s[0],rstrike->m_s[1],rstrike->m_s[2],
					  rstrike->m_e[0],rstrike->m_e[1],rstrike->m_e[2],
					  rstrike->m_cuetip,rstrike->m_selectBall,rstrike->m_strikeRnd, 0,
					  rstrike->m_ballMouseOn,rstrike->m_ballMousePos,rstrike->m_nominateBall,
					  rstrike->m_testBallColour, 0, 1);
}

void CSnooker::StrikeBallReceive(float sx,float sy,float sz, float ex,float ey,float ez, M3d_vtx cuetip, int selectBall, float strikeRnd, float strikeMaxRnd,
								 BOOL ballMouseOn, M3d_vtx ballMousePos, int nominateBall, int testBallColour, DWORD crc, int replay, BYTE nSimEndMsgBalls)
{	

	
	#ifdef MAIN_SERVER
		return;
	#endif //MAIN_SERVER

	//Mmain_fsreset(); // reset frameskipper

	////////////////////////////////////////////////////////////////////
	// LAGGERS #2:2
	////////////////////////////////////////////////////////////////////
	if(!m_bPracticeMode)
	if(m_cueBallSelect == 2 || m_cueBallSelect == 4 || m_cueBallSelect >= 20) // if still in sim. or just finished simEndRec.
		DoLaggersCatchUp();	
	////////////////////////////////////////////////////////////////////

	// put us in sim mode now!
	if( MyTurn() ) 
		//m_cueBallSelect = 2;
		m_cueBallSelect = 20;
	else
		m_cueBallSelect = 20; // go into countdown mode, wait for x frame before strike() is called


	m_bStrikeBallReady = TRUE;
	m_bShotInProgress = TRUE;

	// replay code: Adds a StrikeBall to the replay list /////////////////////////////////////////
	if(!replay) // only save replay if we're not in playback mode
	{
		if(! ((CGameAppView *)m_parent)->m_replay->IsFull() )
		{
		GenerateStateReplay();

		((CGameAppView *)m_parent)->m_replay->AddTimeStamp();

		CReplayStrikeBall *rstrike = new CReplayStrikeBall;
		((CGameAppView *)m_parent)->m_replay->AddStrike(rstrike);

		if(rstrike==NULL)
		{
		// Error can't add strike to list (list full).
		}
		else {
		rstrike->m_type = STRIKETYPE_BALL;
		rstrike->SetStart(sx,sy,sz);
		rstrike->SetEnd(ex,ey,ez);
		rstrike->SetCuetip(cuetip[0],cuetip[1],cuetip[2]);
		rstrike->m_selectBall = selectBall;
		rstrike->m_strikeRnd = strikeRnd;
		rstrike->m_ballMouseOn = ballMouseOn;
		rstrike->SetBallMousePos(ballMousePos[0],ballMousePos[1],ballMousePos[2]);
		rstrike->m_nominateBall = nominateBall;
		rstrike->m_testBallColour = testBallColour;
		}

		} // end if IsFull()


	DWORD crc_now;
#ifdef FIXEDMODE
	crc_now = GenerateBallCRCF();
#else
	crc_now = GenerateBallCRC();
#endif

	if(crc_now != crc)
		{
		//ErrorMessage(m_parent, "Out Of Sync!"); // de-rem this line to display this error!
		//ErrorMessage(m_parent, "ReSync!"); // de-rem this line to display this error!
		
		//char buffer[80];
		//sprintf(buffer,"***  ***");
		//((CGameAppView *)pParent)->Message(buffer,10); // important error message (personal)
				
		//char buffer[80];
		//sprintf(buffer,"%lx\r\n%lx",crc,crc_now);
		}
	} // end !replay
	// end replay code ///////////////////////////////////////////////////////////////////////

	// fix player's control to ONLY the sendEdit window

/*
	if(GAMEAPPVIEW->m_lobby->m_bInTable)
	if(!m_bPracticeMode) // online only! Note: helps to prevent ppl slowing down the game! May need to rethink this?
		{
		GAMEAPPVIEW->m_sendEdit.SetCapture();
		}
*/

	// setup player's SIM flag for 'SIMEND'
	if(!m_bPracticeMode)
	{		
		m_SimulationEnd = FALSE;
	}
	
	// prepare strikeball data:
	m_selectBall = selectBall;
	//m_cueBallSelect = 2;
	//m_bCueState = FALSE;

	m_sPoint[0] = sx;
	m_sPoint[1] = sy;
	m_sPoint[2] = sz;
	//m_tableMousePos[0] = ex;
	//m_tableMousePos[1] = ey;
	//m_tableMousePos[2] = ez;
	m_tableMousePos[0] = ex;//sx-800;//ex;
	m_tableMousePos[1] = ey;//sy+65;//ey;
	m_tableMousePos[2] = ez;//sz;//ez;
	m_cuetip[0] = cuetip[0];
	m_cuetip[1] = cuetip[1];
	m_cuetip[2] = cuetip[2];

	m_strikeRnd = strikeRnd;
	m_strikeMaxRnd = strikeMaxRnd;

	m_tableMousePosStrike[0] = m_tableMousePos[0];
	m_tableMousePosStrike[1] = m_tableMousePos[1];
	m_tableMousePosStrike[2] = m_tableMousePos[2];
	
	m_testBall->m_colour = testBallColour;
	m_testBall->m_idx = selectBall;
	
	// receive new cuetip data (this is the last time it was changed before the strike):
	m_ballMouseOn = ballMouseOn; // pointing at the table (don't need to send this data [-])
	m_ballMousePos[0] = ballMousePos[0];
	m_ballMousePos[1] = ballMousePos[1];
	m_ballMousePos[2] = ballMousePos[2];
	m_nominateBall = nominateBall;

	// *NEW - remember what strike idx this is, for later when we do sendballs
	int tbl = GAMEAPPVIEW->m_lobby->GetTableIdxFromPlayer(GAMEAPPVIEW->m_playerSockIdx);
	if(tbl!=-1)
	{
		m_table[tbl].nSimEndMsgBalls = nSimEndMsgBalls;
	}

	//m_cueBallSelect = 20; // go into countdown mode, wait for x frame before strike() is called
}

void CSnooker::GenerateStateReplay(void) 
{   // SAVING // 

	// Used to Stop Memory Hacks, so we have to Decode all the Scores, High Breaks //
	g_gameAppView->m_snooker->m_customTable->Decode();


	if(((CGameAppView *)m_parent)->m_replay->IsFull() ) return;
	
	// Snooker uses floating point & 10,000 to represent inactive ball.
	CReplayStrikeState *rstrike = new CReplayStrikeState;
	int n;
	rstrike->m_type = STRIKETYPE_STATE;
	for(n=0;n<MAX_TOTALBALLS;n++)
		{
			#ifndef THE_POOL_CLUB
			if(TestFlag(m_ball[n]->m_flags, Flg_BALL_ACTIVE))
				{
				rstrike->m_ballX[n] = m_ball[n]->m_P.x;
				rstrike->m_ballY[n] = m_ball[n]->m_P.y;
				}
			else {
				rstrike->m_ballX[n] = 10000;
				rstrike->m_ballY[n] = 10000;
				}
			#else // iPool
			rstrike->m_ballX[n] = m_ball[n]->m_P.x;
			rstrike->m_ballY[n] = m_ball[n]->m_P.y;

			// override possibly for Bowls
			if(m_customTable->m_type == CTTYPE_BOWLS)
			if(TestFlag(m_ball[n]->m_flags, Flg_BALL_POTTED))
			{
				rstrike->m_ballX[n] = 20000;
				rstrike->m_ballX[n] = 20000;
			}
			#endif // !THE_POOL_CLUB
		}


	// Make sure data is decoded when saved for replay //
	m_customTable->Decode();
	
	rstrike->m_score1P = m_score1P;
	rstrike->m_score2P = m_score2P;
	rstrike->m_break1P = m_break1P;
	rstrike->m_break2P = m_break2P;
	rstrike->m_bTurn1P = m_bTurn1P;
	rstrike->m_bestScore = m_bestScore;
	rstrike->m_rednext = m_rednext;
	rstrike->m_colourseq = m_colourseq;

	// Encode data again after we've finished saving it //
	m_customTable->Encode();

	#ifdef THE_POOL_CLUB
	//char buf[256];
	for(n=0;n<16;n++)
	{
		rstrike->m_pottedSpots[n] = g_pottedSpots[n];
		rstrike->m_pottedStripes[n] = g_pottedStripes[n];	
		//sprintf(buf, "SAVE: rstrike->m_pottedSpots[n] = %d - (%d)", rstrike->m_pottedSpots[n], n);
		//ErrorMessage(m_parent, buf);
		//sprintf(buf, "SAVE: rstrike->m_pottedStripes[n] = %d - (%d)", rstrike->m_pottedStripes[n], n);
		//ErrorMessage(m_parent, buf);		
	}
	#endif //THE_POOL_CLUB

	

	int tbl;
	tbl = ((CGameAppView *)m_parent)->m_lobby->GetTableIdxFromPlayer(((CGameAppView *)m_parent)->m_playerSockIdx);

	rstrike->m_name1[0] = 0;
	rstrike->m_name2[0] = 0;

	if(tbl!=-1) // Not Offline/or in lobby (logically impossible though)
	{
	int plr1Idx;
	int plr2Idx;
	plr1Idx = ((CGameAppView *)m_parent)->m_lobby->GetPlayerIdx(m_table[tbl].player1SockIdx);
	plr2Idx = ((CGameAppView *)m_parent)->m_lobby->GetPlayerIdx(m_table[tbl].player2SockIdx);

	if(plr1Idx!=-1) strcpy(rstrike->m_name1, m_player[plr1Idx].name);
	if(plr2Idx!=-1) strcpy(rstrike->m_name2, m_player[plr2Idx].name);
	}
	else if(GAMEAPPVIEW->m_bPlayOffline)
	{
		strcpy(rstrike->m_name1, "Player1");
		strcpy(rstrike->m_name2, "Player2");
	}

	((CGameAppView *)m_parent)->m_replay->AddStrike(rstrike);


	// Used to Stop Memory Hacks, so we have to Encode all the Scores, High Breaks //
	g_gameAppView->m_snooker->m_customTable->Encode();
}

void CSnooker::StateReplay(CReplayStrikeState *rstrike) 
{   // LOADING // 

	// Snooker uses floating point & 10,000 to represent inactive ball.
	int n;
	for(n=0;n<MAX_TOTALBALLS;n++)
	{
		#ifndef THE_POOL_CLUB

		if(rstrike->m_ballX[n]>=9999 && rstrike->m_ballY[n]>=9999) // check if balls are not active.
		{
		ClearFlag(m_ball[n]->m_flags,Flg_BALL_ACTIVE); 
		}
		else {
		SetFlag(m_ball[n]->m_flags,Flg_BALL_ACTIVE); 
		m_ball[n]->m_P.x = rstrike->m_ballX[n];
		m_ball[n]->m_P.y = rstrike->m_ballY[n];		
		}

		#else // iPool

		// off-screen case i.e. not active/visible
		if(rstrike->m_ballX[n] >= 9999 && rstrike->m_ballY[n] >= 9999)
		{
			// this ball is out of play
			if(rstrike->m_ballX[n] >= 19999 && rstrike->m_ballY[n] >= 19999) // check if balls are not active.
			if(m_customTable->m_type == CTTYPE_BOWLS)
			{								
				SetFlag(m_ball[n]->m_flags, Flg_BALL_POTTED);
			}
			
			m_ball[n]->m_P.x = rstrike->m_ballX[n];
			m_ball[n]->m_P.y = rstrike->m_ballY[n];
			
			// not active
			ClearFlag(m_ball[n]->m_flags, Flg_BALL_ACTIVE);
		}
		// on-screen case i.e. active/visible
		else
		{
			// active
			SetFlag(m_ball[n]->m_flags, Flg_BALL_ACTIVE);
			
			m_ball[n]->m_P.x = rstrike->m_ballX[n];
			m_ball[n]->m_P.y = rstrike->m_ballY[n];
		}

			#ifdef FIXEDMODE
			m_ball[n]->UpdateFixeds();
			#endif

		#endif //!THE_POOL_CLUB
	}

	// Replay Data is always in Decoded State //
	g_gameAppView->m_snooker->m_customTable->Decode();

	m_score1P = rstrike->m_score1P;
	m_score2P = rstrike->m_score2P;
	m_break1P = rstrike->m_break1P;
	m_break2P = rstrike->m_break2P;
	m_bTurn1P = rstrike->m_bTurn1P;
	m_bestScore = rstrike->m_bestScore;
	m_rednext = rstrike->m_rednext;
	m_colourseq = rstrike->m_colourseq;
	strcpy(m_replayname1, rstrike->m_name1);
	strcpy(m_replayname2, rstrike->m_name2);
	
	
	#ifdef THE_POOL_CLUB
	//char buf[256];
	for(n=0;n<16;n++)
	{
		//sprintf(buf, "rstrike->m_pottedSpots[n] = %d - (%d)", rstrike->m_pottedSpots[n], n);
		//ErrorMessage(m_parent, buf);
		//sprintf(buf, "rstrike->m_pottedStripes[n] = %d - (%d)", rstrike->m_pottedStripes[n], n);
		//ErrorMessage(m_parent, buf);

		g_pottedSpots[n] = rstrike->m_pottedSpots[n];
		g_pottedStripes[n] = rstrike->m_pottedStripes[n];
	}
	#endif //THE_POOL_CLUB

	// Used to Stop Memory Hacks, so we have to Encode all the Scores, High Breaks //
	g_gameAppView->m_snooker->m_customTable->Encode();
	
}

void CSnooker::BallPosReplay(CReplayStrikeBallPos *rstrike) 
{
	CVector p;
	p.x = rstrike->m_x;
	p.y = rstrike->m_y;
	p.z = rstrike->m_z;
	BallPos(p, rstrike->m_selectBall, 1);
}

void CSnooker::BallPos(CVector pos, int selectBall, int replay)
{
	//if( MyTurn() ) return;
	// replay code: Adds a StrikeBall to the replay list /////////////////////////////////////////
	/*if(!replay) // only save replay if we're not in playback mode
	{
	if(! ((CGameAppView *)m_parent)->m_replay->IsFull() )
		{
		
		((CGameAppView *)m_parent)->m_replay->AddTimeStamp();

		CReplayStrikeBallPos *rstrike = new CReplayStrikeBallPos;
		((CGameAppView *)m_parent)->m_replay->AddStrike(rstrike);

		if(rstrike==NULL)
		{
		// Error can't add strike to list (list full).
		}
		else {
		rstrike->m_type = STRIKETYPE_BALLPOS;
		rstrike->m_x = pos.x;

		rstrike->m_y = pos.y;
		rstrike->m_z = pos.z;
		rstrike->m_selectBall = selectBall;
		}

		}

	} // end !replay
	// end replay code ///////////////////////////////////////////////////////////////////////
	*/
	
	m_ball[selectBall]->m_P = pos;
	#ifdef FIXEDMODE
	m_ball[selectBall]->UpdateFixeds();
	#endif
}

void CSnooker::InitMeshes(void)
{
	InitPlate();
	InitCube();
	
	// Setup Mesh Struct.
	m_M3dmap_tri_mesh.type = M3D_MESH_UNI|M3D_MESH_LVERTEX; // Lit Vertex (has color specifiers)
	m_M3dmap_tri_mesh.n = 1;
	m_M3dmap_tri_mesh.vtx = (Vertex *)tri_lvtx;
	m_M3dmap_tri_mesh.bVB = false;
	m_M3dmap_tri_mesh._vb = NULL;


	InitUTri();
	InitUPlate();
/*
	// unit tri
	m_M3dmap_utri_mesh.type = M3D_MESH_UNI|M3D_MESH_LVERTEX; // Lit Vertex (has color specifiers)
	m_M3dmap_utri_mesh.n = 1;
	m_M3dmap_utri_mesh.vtx = (Vertex *)utri_lvtx;
	m_M3dmap_utri_mesh.bVB = false;
	m_M3dmap_utri_mesh._vb = NULL;
*/
/*
	// unit plate
	m_M3dmap_uplate_mesh.type = M3D_MESH_REF|M3D_MESH_LVERTEX; // Lit Vertex (has color specifiers)
	m_M3dmap_uplate_mesh.v = 4;
	m_M3dmap_uplate_mesh.n = 2;
	m_M3dmap_uplate_mesh.vtx = (Vertex *)uplate_lvtx;
	m_M3dmap_uplate_mesh.lst = plate_lst;
	m_M3dmap_uplate_mesh.bVB = false;
	m_M3dmap_uplate_mesh._vb = NULL;
*/
}


void CSnooker::InitUPlate(void)
{
	// unit plate
	m_M3dmap_uplate_mesh.type = M3D_MESH_V3;
	m_M3dmap_uplate_mesh.v = 4;
	m_M3dmap_uplate_mesh.n = 2;
	m_M3dmap_uplate_mesh.vtx = NULL;
	m_M3dmap_uplate_mesh.lst = NULL;
	m_M3dmap_uplate_mesh.bVB = true;
		
	// Create Vertex and Index Buffers
	g_pDevice->CreateVertexBuffer(
		4 * sizeof(Vertex3),
		D3DUSAGE_WRITEONLY,
		Vertex3::FVF,
		D3DPOOL_MANAGED,
		&m_M3dmap_uplate_mesh._vb,
		0);

	g_pDevice->CreateIndexBuffer(
		(2*3) * sizeof(WORD),
		D3DUSAGE_WRITEONLY,
		D3DFMT_INDEX16,
		D3DPOOL_MANAGED,
		&m_M3dmap_uplate_mesh._ib,
		0);


	//
	// Fill the buffers with the cube data.
	//

	// define unique vertices:
	Vertex3* vertices;
	m_M3dmap_uplate_mesh._vb->Lock(0, 0, (void**)&vertices, 0);
	// vertices of a unit cube
	vertices[0] = Vertex3(-0.5f, 0.0f, 0.0f, D3DCOLOR_RGBA(255,255,255,255));
	vertices[1] = Vertex3(0.5f, 0.0f, 0.0f, D3DCOLOR_RGBA(255,255,255,255));
	vertices[2] = Vertex3(-0.5f, 1.0f, 0.0f, D3DCOLOR_RGBA(255,255,255,255));
	vertices[3] = Vertex3(0.5f, 1.0f, 0.0f, D3DCOLOR_RGBA(255,255,255,255));
	/*
	vertices[0] = Vertex2(-0.5f, 0.0f, 0.0f, 0.0f, 0.0f, 0xFFFFFFFF);
	vertices[1] = Vertex2(0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 0xFFFFFFFF);
	vertices[2] = Vertex2(0.5f, 1.0f, 0.0f, 1.0f, 1.0f, 0xFFFFFFFFF);
	vertices[3] = Vertex2(-0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 0xFFFFFFFF);
	*/
		
	
	m_M3dmap_uplate_mesh._vb->Unlock();

	// define the triangles of the cube:
	WORD* indices = 0;
	m_M3dmap_uplate_mesh._ib->Lock(0, 0, (void**)&indices, 0);

	// front side
	indices[0]  = 1; indices[1]  = 2; indices[2]  = 0;
	indices[3]  = 1; indices[4]  = 3; indices[5]  = 2;
	m_M3dmap_uplate_mesh._ib->Unlock();
}


void CSnooker::InitUTri(void)
{
	// unit tri
	m_M3dmap_utri_mesh.type = M3D_MESH_V3;
	m_M3dmap_utri_mesh.n = 1;
	m_M3dmap_utri_mesh.v = 3;
	m_M3dmap_utri_mesh.vtx = NULL;
	m_M3dmap_utri_mesh.lst = NULL;
	m_M3dmap_utri_mesh.bVB = true;
	
	// Create Vertex and Index Buffers
	g_pDevice->CreateVertexBuffer(
		3 * sizeof(Vertex3),
		D3DUSAGE_WRITEONLY,
		Vertex3::FVF,
		D3DPOOL_MANAGED,
		&m_M3dmap_utri_mesh._vb,
		0);

	g_pDevice->CreateIndexBuffer(
		(1*3) * sizeof(WORD),
		D3DUSAGE_WRITEONLY,
		D3DFMT_INDEX16,
		D3DPOOL_MANAGED,
		&m_M3dmap_utri_mesh._ib,
		0);


	//
	// Fill the buffers with the data.
	//

	// define unique vertices:
	Vertex3* vertices;
	m_M3dmap_utri_mesh._vb->Lock(0, 0, (void**)&vertices, 0);
	// vertices of a unit cube

	vertices[0] = Vertex3(0.0f, 0.0f, 0.0f, D3DCOLOR_RGBA(255,0,0,90));
	vertices[1] = Vertex3(0.5f, 1.0f, 0.0f, D3DCOLOR_RGBA(0,255,0,90));
	vertices[2] = Vertex3(-0.5f, 1.0f, 0.0f, D3DCOLOR_RGBA(0,0,255,90));
	m_M3dmap_utri_mesh._vb->Unlock();

	// define the triangles of the cube:
	WORD* indices = 0;
	m_M3dmap_utri_mesh._ib->Lock(0, 0, (void**)&indices, 0);

	// front side
	indices[0]  = 0; indices[1]  = 1; indices[2]  = 2;
	m_M3dmap_utri_mesh._ib->Unlock();
}

void CSnooker::InitPlate(void)
{
	// Setup Mesh Struct.
	m_M3dmap_plate_mesh.type = M3D_MESH_REF;//UNI;
	m_M3dmap_plate_mesh.v = 4;
	m_M3dmap_plate_mesh.n = 2;
	m_M3dmap_plate_mesh.vtx = plate_vtx;
	m_M3dmap_plate_mesh.lst = plate_lst;
	m_M3dmap_plate_mesh.bVB = false;
	m_M3dmap_plate_mesh._vb = NULL;
	
	// Create Vertex and Index Buffers
	g_pDevice->CreateVertexBuffer(
		4 * sizeof(Vertex),
		D3DUSAGE_WRITEONLY,
		Vertex2::FVF,
		D3DPOOL_MANAGED,
		&m_M3dmap_plate_mesh._vb,
		0);

	g_pDevice->CreateIndexBuffer(
		(2*3) * sizeof(WORD),
		D3DUSAGE_WRITEONLY,
		D3DFMT_INDEX16,
		D3DPOOL_MANAGED,
		&m_M3dmap_plate_mesh._ib,
		0);



	// define unique vertices:
	Vertex* vertices;
	m_M3dmap_plate_mesh._vb->Lock(0, 0, (void**)&vertices, 0);
	// vertices of a unit cube
	vertices[0] = Vertex(-50.0f, -50.0f, 50.0f,	0.0f, 0.0f,	1.0f,	0.0f, 0.0f);
	vertices[1] = Vertex(50.0f, -50.0f, 50.0f,	0.0f, 0.0f,	1.0f,	1.0f, 0.0f);
	vertices[2] = Vertex(50.0f, 50.0f, 50.0f,	0.0f, 0.0f,	1.0f,	1.0f, 1.0f);
	vertices[3] = Vertex(-50.0f, 50.0f, 50.0f,	0.0f, 0.0f,	1.0f,	0.0f, 1.0f);
	m_M3dmap_plate_mesh._vb->Unlock();

	// define the triangles of the cube:
	WORD* indices = 0;
	m_M3dmap_plate_mesh._ib->Lock(0, 0, (void**)&indices, 0);
	// front side
	indices[0]  = 0; indices[1]  = 1; indices[2]  = 2;
	indices[3]  = 2; indices[4]  = 3; indices[5]  = 0;
	m_M3dmap_plate_mesh._ib->Unlock();

	M3d_normalizemesh(&m_M3dmap_plate_mesh); // calc normals.
	// Erase 'em.
}

void CSnooker::InitCube(void)
{
	// Setup Mesh Struct.
	m_M3dmap_cube_mesh.type = M3D_MESH_REF;
	m_M3dmap_cube_mesh.n = 12;
	m_M3dmap_cube_mesh.v = 8;
	m_M3dmap_cube_mesh.vtx = cube_vtx;
	m_M3dmap_cube_mesh.bVB = true;
	
	// Create Vertex and Index Buffers
	g_pDevice->CreateVertexBuffer(
		8 * sizeof(Vertex),
		D3DUSAGE_WRITEONLY,
		Vertex::FVF,
		D3DPOOL_MANAGED,
		&m_M3dmap_cube_mesh._vb,
		0);

	g_pDevice->CreateIndexBuffer(
		36 * sizeof(WORD),
		D3DUSAGE_WRITEONLY,
		D3DFMT_INDEX16,
		D3DPOOL_MANAGED,
		&m_M3dmap_cube_mesh._ib,
		0);


	//
	// Fill the buffers with the cube data.
	//

	// define unique vertices:
	Vertex* vertices;
	m_M3dmap_cube_mesh._vb->Lock(0, 0, (void**)&vertices, 0);

	// vertices of a unit cube
	vertices[0] = Vertex(-1.0f, -1.0f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f);
	vertices[1] = Vertex(-1.0f,  1.0f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f);
	vertices[2] = Vertex( 1.0f,  1.0f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f);
	vertices[3] = Vertex( 1.0f, -1.0f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f);
	vertices[4] = Vertex(-1.0f, -1.0f,  1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f);
	vertices[5] = Vertex(-1.0f,  1.0f,  1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f);
	vertices[6] = Vertex( 1.0f,  1.0f,  1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f);
	vertices[7] = Vertex( 1.0f, -1.0f,  1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f);

	m_M3dmap_cube_mesh._vb->Unlock();

	// define the triangles of the cube:
	WORD* indices = 0;
	m_M3dmap_cube_mesh._ib->Lock(0, 0, (void**)&indices, 0);

	// front side
	indices[0]  = 0; indices[1]  = 1; indices[2]  = 2;
	indices[3]  = 0; indices[4]  = 2; indices[5]  = 3;

	// back side
	indices[6]  = 4; indices[7]  = 6; indices[8]  = 5;
	indices[9]  = 4; indices[10] = 7; indices[11] = 6;

	// left side
	indices[12] = 4; indices[13] = 5; indices[14] = 1;
	indices[15] = 4; indices[16] = 1; indices[17] = 0;

	// right side
	indices[18] = 3; indices[19] = 2; indices[20] = 6;
	indices[21] = 3; indices[22] = 6; indices[23] = 7;

	// top
	indices[24] = 1; indices[25] = 5; indices[26] = 6;
	indices[27] = 1; indices[28] = 6; indices[29] = 2;

	// bottom
	indices[30] = 4; indices[31] = 0; indices[32] = 3;
	indices[33] = 4; indices[34] = 3; indices[35] = 7;

	m_M3dmap_cube_mesh._ib->Unlock();




	M3d_normalizemesh(&m_M3dmap_cube_mesh); // calc normals.
	// Erase 'em.
}

void CSnooker::InitTexture(void)
{
	m_M3dmap_texture = new Mgfx_spr;
}

void CSnooker::LoadTexture(char *fname)
{
	// Load 3D Textures.
	M3d_bmpload16(m_M3dmap_texture,fname);
}

void CSnooker::DeinitTexture(void)
{
	delete m_M3dmap_texture;	
}

float CSnooker::ReLengthByBound(M3d_vtx pos, M3d_vtx rot, float length)
{
	CVector vect;
	float mag_x,mag_y;
	float newlen;
	vect.x = sin(rot[2]);
	vect.y = cos(rot[2]);
	vect.z = 0;
	// find what quadrant.
	if(vect.x>0) mag_x = vect.x;	
	else mag_x = -vect.x;

	if(vect.y>0) mag_y = vect.y;
	else mag_y = -vect.y;

	if(mag_y>mag_x) // Y gradient
	{
	if(vect.y>0) newlen = (m_tableBound.maxY-pos[1])/mag_y;
	else newlen = (pos[1]-m_tableBound.minY)/mag_y;
	} else { // X gradient
	if(vect.x>0) newlen = (m_tableBound.maxX-pos[0])/mag_x;
	else newlen = (pos[0]-m_tableBound.minX)/mag_x;
	}

	if(newlen>length) newlen = length;
	return newlen;
}

void CSnooker::DrawTri(M3d_vtx pos, M3d_vtx rot, M3d_vtx scale, COLORREF rgba, float gAlpha)
{
	int ri = (int)(GetBValue(rgba));	// Note color is encoded RGB not BGR
	int gi = (int)(GetGValue(rgba));
	int bi = (int)(GetRValue(rgba));
	
	
	float r = (float)(GetRValue(rgba)/255.0f);
	float g = (float)(GetGValue(rgba)/255.0f);
	float b = (float)(GetBValue(rgba)/255.0f);
	float a = (float)(GetAValue(rgba)/255.0f);
	
	a = 1.0f;

	M3d_mat temp_mat;

    temp_mat.d3dm.Diffuse.r = r;
	temp_mat.d3dm.Diffuse.g = g;
	temp_mat.d3dm.Diffuse.b = b;
	temp_mat.d3dm.Diffuse.a = a;

	temp_mat.d3dm.Ambient.r = r;
	temp_mat.d3dm.Ambient.g = g;
	temp_mat.d3dm.Ambient.b = b;
	temp_mat.d3dm.Ambient.a = a;	

	temp_mat.d3dm.Specular.r = r;
	temp_mat.d3dm.Specular.g = g;
	temp_mat.d3dm.Specular.b = b;
	temp_mat.d3dm.Specular.a = a;
	
	temp_mat.d3dm.Emissive.r = r;
	temp_mat.d3dm.Emissive.g = g;
	temp_mat.d3dm.Emissive.b = b;
	temp_mat.d3dm.Emissive.a = a;

	temp_mat.d3dm.Power = D3D_MATERIAL_POWER;//1;//70;
						  
	//M3d_texture(m_M3dmap_texture,0);	// Set Texture.
	//M3d_material(&temp_mat);			// Set Material.
	M3d_texture_kill();


	Vertex3* pVtx;
		
	// Access the Vertices //
	m_M3dmap_utri_mesh._vb->Lock(0, 0, (void**)&pVtx, 0);
	int alpha = (float)(a*255)*0.2f;

	pVtx[0].color = D3DCOLOR_RGBA(ri, gi, bi, 0);
	pVtx[1].color = D3DCOLOR_RGBA(ri, gi, bi, (int)(gAlpha*80.0f));
	pVtx[2].color = D3DCOLOR_RGBA(ri, gi, bi, (int)(gAlpha*80.0f));

	m_M3dmap_utri_mesh._vb->Unlock();
		


/*

	// Adjust coloring of vertices
	Vertex *pVtx = (Vertex *) m_M3dmap_tri_mesh.vtx;
	pVtx[0].color = RGBA(1,1,1,0);
	pVtx[0].specular = RGBA(1,1,1,0);

	pVtx[1].color = RGBA(1,1,1,a);
	pVtx[1].specular = RGBA(1,1,1,a);
	pVtx[2].color = RGBA(1,1,1,a);
	pVtx[2].specular = RGBA(1,1,1,a);
*/
	M3d_texture_kill();
	M3d_transformobject(pos,rot,scale);
	M3d_drawmesh(&m_M3dmap_utri_mesh);
}

void CSnooker::DrawUTri(M3d_vtx pos, M3d_vtx rot, M3d_vtx scale, COLORREF rgba)
{

#ifndef MAIN_SERVER

	float r = (float)(GetRValue(rgba)/255.0f);
	float g = (float)(GetGValue(rgba)/255.0f);
	float b = (float)(GetBValue(rgba)/255.0f);
	float a = (float)(GetAValue(rgba)/255.0f);
	
	M3d_mat temp_mat;

    temp_mat.d3dm.Diffuse.r = r;
	temp_mat.d3dm.Diffuse.g = g;
	temp_mat.d3dm.Diffuse.b = b;
	temp_mat.d3dm.Diffuse.a = a;

	temp_mat.d3dm.Ambient.r = r;
	temp_mat.d3dm.Ambient.g = g;
	temp_mat.d3dm.Ambient.b = b;
	temp_mat.d3dm.Ambient.a = a;	

	temp_mat.d3dm.Specular.r = r;
	temp_mat.d3dm.Specular.g = g;
	temp_mat.d3dm.Specular.b = b;
	temp_mat.d3dm.Specular.a = a;
	
	temp_mat.d3dm.Emissive.r = r;
	temp_mat.d3dm.Emissive.g = g;
	temp_mat.d3dm.Emissive.b = b;
	temp_mat.d3dm.Emissive.a = a;

	temp_mat.d3dm.Power = D3D_MATERIAL_POWER;//1;//70;
						  
	//M3d_texture(m_M3dmap_texture,0);	// Set Texture.
	M3d_material(&temp_mat);			// Set Material.
	M3d_texture_kill();



	// Adjust coloring of vertices
	//Vertex *pVtx = (Vertex *) m_M3dmap_utri_mesh.vtx;

	Vertex3* pVtx;
	m_M3dmap_utri_mesh._vb->Lock(0, 0, (void**)&pVtx, 0);


	pVtx[0].color = RGBA(1,1,1,a);
	//pVtx[0].specular = RGBA(1,1,1,a);

	pVtx[1].color = RGBA(1,1,1,0);
	//pVtx[1].specular = RGBA(1,1,1,0);
	pVtx[2].color = RGBA(1,1,1,0);
	//pVtx[2].specular = RGBA(1,1,1,0);

	m_M3dmap_utri_mesh._vb->Unlock();

	//M3d_transformobject(pos,rot,scale);

	D3DXMATRIX matRotX, matRotY, matRotZ, matTrans, matScale;
	D3DXMATRIX matWorld;
	// Calculate Scale matrix
	D3DXMatrixScaling(&matScale, scale[0], scale[1], scale[2]);

	// Calculate rotation matrix
	D3DXMatrixRotationX( &matRotX, rot[0] );
	D3DXMatrixRotationY( &matRotY, rot[1] );
	D3DXMatrixRotationZ( &matRotZ, rot[2] );
	
	// Calculate a translation matrix
	D3DXMatrixTranslation(&matTrans, pos[0], pos[1], pos[2]);

	matWorld = matScale*(matRotX*matRotY*matRotZ)*matTrans;
	g_pDevice->SetTransform(D3DTS_WORLD, &matWorld);		


	
	M3d_drawmesh(&m_M3dmap_utri_mesh);
#endif
}

void CSnooker::DrawPlate(M3d_vtx pos, M3d_vtx rot, M3d_vtx scale, COLORREF rgba)
{
#ifndef MAIN_SERVER
	float r = (float)(GetRValue(rgba)/255.0f);
	float g = (float)(GetGValue(rgba)/255.0f);
	float b = (float)(GetBValue(rgba)/255.0f);
	float a = (float)(GetAValue(rgba)/255.0f);
	
	M3d_mat temp_mat;

    temp_mat.d3dm.Diffuse.r = r;
	temp_mat.d3dm.Diffuse.g = g;
	temp_mat.d3dm.Diffuse.b = b;
	temp_mat.d3dm.Diffuse.a = a;

	temp_mat.d3dm.Ambient.r = r;
	temp_mat.d3dm.Ambient.g = g;
	temp_mat.d3dm.Ambient.b = b;
	temp_mat.d3dm.Ambient.a = a;	

	temp_mat.d3dm.Specular.r = r;
	temp_mat.d3dm.Specular.g = g;
	temp_mat.d3dm.Specular.b = b;
	temp_mat.d3dm.Specular.a = a;
	
	temp_mat.d3dm.Emissive.r = r;
	temp_mat.d3dm.Emissive.g = g;
	temp_mat.d3dm.Emissive.b = b;
	temp_mat.d3dm.Emissive.a = a;

	temp_mat.d3dm.Power = D3D_MATERIAL_POWER;//1;//70;
						  
	//M3d_texture(m_M3dmap_texture,0);	// Set Texture.
	M3d_material(&temp_mat);			// Set Material.
	M3d_texture_kill();

	M3d_transformobject(pos,rot,scale);
	M3d_drawmesh(&m_M3dmap_plate_mesh);
#endif
}

void CSnooker::DrawPlate2(M3d_vtx pos, M3d_vtx rot, M3d_vtx scale, COLORREF rgba)
{
#ifndef MAIN_SERVER
	float r = (float)(GetRValue(rgba)/255.0f);
	float g = (float)(GetGValue(rgba)/255.0f);
	float b = (float)(GetBValue(rgba)/255.0f);
	float a = (float)(GetAValue(rgba)/255.0f);
	
	M3d_mat temp_mat;
	
	temp_mat.d3dm.Diffuse.r = 0;
	temp_mat.d3dm.Diffuse.g = 0;
	temp_mat.d3dm.Diffuse.b = 0;
	temp_mat.d3dm.Diffuse.a = a;

	temp_mat.d3dm.Ambient.r = 0;
	temp_mat.d3dm.Ambient.g = 0;
	temp_mat.d3dm.Ambient.b = 0;
	temp_mat.d3dm.Ambient.a = a;

	temp_mat.d3dm.Specular.r = r;
	temp_mat.d3dm.Specular.g = g;
	temp_mat.d3dm.Specular.b = b;
	temp_mat.d3dm.Specular.a = a;
	
	temp_mat.d3dm.Emissive.r = r;
	temp_mat.d3dm.Emissive.g = g;
	temp_mat.d3dm.Emissive.b = b;  
	temp_mat.d3dm.Emissive.a = a;

	temp_mat.d3dm.Power = D3D_MATERIAL_POWER;//1;
						  
	//M3d_texture(m_M3dmap_texture,0);	// Set Texture.
	M3d_texture_kill();
	M3d_material(&temp_mat);			// Set Material.
	

	M3d_transformobject(pos,rot,scale);
	M3d_drawmesh(&m_M3dmap_plate_mesh);
#endif
}

void CSnooker::DrawCube(long x,long y,long z,WORD c)
{
#ifndef MAIN_SERVER
	M3d_vtx scale;
	M3d_vtx rot;
	M3d_vtx pos;
	M3d_mat temp_mat;

  	scale[0] = 10; // x
	scale[1] = 10;//0.1f; // y
	scale[2] = 10; // z

	rot[0] = 0;
	rot[1] = 0;
	rot[2] = 90*DEG2RAD;

	pos[0] = 0;
	pos[1] = 0;
	pos[2] = 500;
	
    temp_mat.d3dm.Diffuse.r = 0;
	temp_mat.d3dm.Diffuse.g = 0;
	temp_mat.d3dm.Diffuse.b = 0;
	temp_mat.d3dm.Diffuse.a = 0.5f;

	temp_mat.d3dm.Ambient.r = 0;
	temp_mat.d3dm.Ambient.g = 0;
	temp_mat.d3dm.Ambient.b = 0;
	temp_mat.d3dm.Ambient.a = 0.5f;	

	temp_mat.d3dm.Specular.r = 1;
	temp_mat.d3dm.Specular.g = 1;
	temp_mat.d3dm.Specular.b = 1;
	temp_mat.d3dm.Specular.a = 0.5f;
	
	temp_mat.d3dm.Emissive.r = 1;
	temp_mat.d3dm.Emissive.g = 1;
	temp_mat.d3dm.Emissive.b = 1;
	temp_mat.d3dm.Emissive.a = 0.5f;

	temp_mat.d3dm.Power = D3D_MATERIAL_POWER;//1;
						  
	//M3d_texture(m_M3dmap_texture,0);	// Set Texture.
	M3d_texture_kill();
	M3d_material(&temp_mat);			// Set Material.

	M3d_transformobject(pos,rot,scale);
	M3d_drawmesh(&m_M3dmap_cube_mesh);
#endif
}

void CSnooker::DrawSightLine(M3d_vtx pos, M3d_vtx rot, float magX, float magY, COLORREF rgba, BOOL bFade)
{
#ifndef MAIN_SERVER
	M3d_vtx scale;
//	M3d_vtx rot;
//	M3d_vtx pos;
	M3d_mat temp_mat;
	M3d_mat oldTemp_mat;
	float factor;

	float r = (float)(GetRValue(rgba)/255.0f);
	float g = (float)(GetGValue(rgba)/255.0f);
	float b = (float)(GetBValue(rgba)/255.0f);
	float a = (float)(GetAValue(rgba)/255.0f);


	if(g_build.gameType == GAMETYPE_SNOOKER)
	{
		r*=0.42f;
		g*=0.42f;
		b*=0.42f;
	}
	
	if(bFade)
	{
		r*=0.95f;
		g*=0.95f;
		b*=0.95f;
		a = 0.1f;
		if(g_build.gameType == GAMETYPE_POOL) a = 0.05f;
	}
	
	factor = 1.0f;
	if(g_build.gameType == GAMETYPE_POOL)
		factor = 0.4f;
	

  	scale[0] = magY*0.01f;//10; // x
	scale[1] = magX*0.1f; // y
	scale[2] = 0.1f; // z

  	scale[0] *= 50.0f;
	scale[1] *= 50.0f;
	scale[2] *= 50.0f;

//	rot[0] = 0;
//	rot[1] = 0;
//	rot[2] = 90*DEG2RAD;

//	pos[0]-=(mag*0.01f)*cos(rot[2])*0.5f;//0;
//	pos[1]-=(mag*0.01f)*sin(rot[2])*0.5f; //0;
	//pos[2] = //500;
	
    temp_mat.d3dm.Diffuse.r = 0.0f;
	temp_mat.d3dm.Diffuse.g = 0.0f;
	temp_mat.d3dm.Diffuse.b = 0.0f;
	if(bFade)
		temp_mat.d3dm.Diffuse.a = a + 0.01f * (float)m_sightlineFade * factor;
	else
		temp_mat.d3dm.Diffuse.a = a;

	temp_mat.d3dm.Ambient.r = 0;
	temp_mat.d3dm.Ambient.g = 0;
	temp_mat.d3dm.Ambient.b = 0;
	if(bFade)
		temp_mat.d3dm.Ambient.a = a + 0.01f * (float)m_sightlineFade * factor;	
	else
		temp_mat.d3dm.Ambient.a = a;

	temp_mat.d3dm.Specular.r = r;
	temp_mat.d3dm.Specular.g = g;
	temp_mat.d3dm.Specular.b = b;
	if(bFade)
		temp_mat.d3dm.Specular.a = a + 0.01f * (float)m_sightlineFade * factor;
	else
		temp_mat.d3dm.Specular.a = a;
	
	temp_mat.d3dm.Emissive.r = r;
	temp_mat.d3dm.Emissive.g = g;
	temp_mat.d3dm.Emissive.b = b;  
	if(bFade)
		temp_mat.d3dm.Emissive.a = a + 0.01f * (float)m_sightlineFade * factor;
	else
		temp_mat.d3dm.Emissive.a = a;

	temp_mat.d3dm.Power = D3D_MATERIAL_POWER;//1;
						  
	//M3d_texture(m_M3dmap_texture,0);	// Set Texture.
	M3d_texture_kill();
	M3d_material(&temp_mat);			// Set Material.

	//M3d_transformobject(pos,rot,scale);

   

	/*
	pos[0] = 0.0f;
	pos[1] = 0.0f;
	pos[2] = 0.0f;

	rot[0] = 0.0f;
	rot[1] = 0.0f;
	rot[2] = 0.0f;

	scale[0] = 100.0f;
	scale[1] = 100.0f;
	scale[2] = 100.0f;
	*/


	D3DXMATRIX matRotX, matRotY, matRotZ, matTrans, matScale;

	// Calculate Scale matrix
	D3DXMatrixScaling(&matScale, scale[0], scale[1], scale[2]);

	// Calculate rotation matrix
	D3DXMatrixRotationX( &matRotX, rot[0] );
	D3DXMatrixRotationY( &matRotY, rot[1] );
	D3DXMatrixRotationZ( &matRotZ, rot[2] );
	
	// Calculate a translation matrix
	D3DXMatrixTranslation(&matTrans, pos[0], pos[1], pos[2]);

	D3DXMATRIX matWorld = matScale*(matRotX*matRotY*matRotZ)*matTrans;
	g_pDevice->SetTransform(D3DTS_WORLD, &matWorld);

	//M3d_objectdraw(&m_M3d_Ball_Shadow);
	//M3d_objectdraw(m_M3dmap_cube_mesh);
	M3d_drawmesh(&m_M3dmap_cube_mesh);
#endif
}




void CSnooker::DrawSightUTri(M3d_vtx pos, M3d_vtx rot, M3d_vtx scale, COLORREF rgba, BOOL bFade)
{
#ifndef MAIN_SERVER
	D3DXMATRIX matRotX, matRotY, matRotZ, matTrans, matScale;
	D3DXMATRIX matWorld;


	int r = GetRValue(rgba);
	int g = GetGValue(rgba);
	int b = GetBValue(rgba);
	int a = GetAValue(rgba);

	if(bFade)
		a = m_sightlineFade;// * factor;


	////////////////////////////////////////////////
	// Tri-line (represents error)
	//a = 255;

	M3d_mat temp_mat;

    temp_mat.d3dm.Diffuse.r = r;
	temp_mat.d3dm.Diffuse.g = g;
	temp_mat.d3dm.Diffuse.b = b;
	temp_mat.d3dm.Diffuse.a = a;

	temp_mat.d3dm.Ambient.r = r;
	temp_mat.d3dm.Ambient.g = g;
	temp_mat.d3dm.Ambient.b = b;
	temp_mat.d3dm.Ambient.a = a;

	temp_mat.d3dm.Specular.r = r;
	temp_mat.d3dm.Specular.g = g;
	temp_mat.d3dm.Specular.b = b;
	temp_mat.d3dm.Specular.a = a;
	
	temp_mat.d3dm.Emissive.r = r;
	temp_mat.d3dm.Emissive.g = g;
	temp_mat.d3dm.Emissive.b = b;
	temp_mat.d3dm.Emissive.a = a;

	temp_mat.d3dm.Power = D3D_MATERIAL_POWER;//1;//70;
						  
	//M3d_texture(m_M3dmap_texture,0);	// Set Texture.
	M3d_material(&temp_mat);			// Set Material.
	M3d_texture_kill();

	//M3d_texture_kill();
	// Adjust coloring of vertices
	Vertex3* pVtx;
	//Vertex *pVtx = (Vertex *) m_M3dmap_utri_mesh.vtx;
	

	//if(m_cueError == 1) // No Tri-Lines Error Helpers for BEGINNER & PRO levels ;)
	if(g_gfxDetails.bOptionsDrawErrorSL)
	{
		
		// Access the Vertices //
		m_M3dmap_utri_mesh._vb->Lock(0, 0, (void**)&pVtx, 0);

		pVtx[0].color = D3DCOLOR_RGBA(r,g,b,a);
		pVtx[1].color = D3DCOLOR_RGBA(r,g,b,a);
		pVtx[2].color = D3DCOLOR_RGBA(r,g,b,a);
	
		m_M3dmap_utri_mesh._vb->Unlock();
		

		// Calculate Scale matrix
		D3DXMatrixScaling(&matScale, scale[0], scale[1], scale[2]);

		// Calculate rotation matrix
		D3DXMatrixRotationX( &matRotX, rot[0] );
		D3DXMatrixRotationY( &matRotY, rot[1] );
		D3DXMatrixRotationZ( &matRotZ, rot[2] );
		
		// Calculate a translation matrix
		D3DXMatrixTranslation(&matTrans, pos[0], pos[1], pos[2]);

		matWorld = matScale*(matRotX*matRotY*matRotZ)*matTrans;
		g_pDevice->SetTransform(D3DTS_WORLD, &matWorld);		

		
		//M3d_drawmesh(&m_M3dmap_cube_mesh);
		
		M3d_texture_kill();
		M3d_drawmesh(&m_M3dmap_utri_mesh);
	}



	//pVtx = (Vertex *) m_M3dmap_uplate_mesh.vtx;
	// Access the Vertices //
	m_M3dmap_uplate_mesh._vb->Lock(0, 0, (void**)&pVtx, 0);


	////////////////////////////////////////////////
	// line (represents ideal line)
	if(g_build.gameType == GAMETYPE_POOL) a*=2;
	else a*=2.25f; // iSnooker



	pVtx[0].color = D3DCOLOR_RGBA(r,g,b,a);
	pVtx[1].color = D3DCOLOR_RGBA(r,g,b,a);
	pVtx[2].color = D3DCOLOR_RGBA(r,g,b,a);
	pVtx[3].color = D3DCOLOR_RGBA(r,g,b,a);
	

	m_M3dmap_uplate_mesh._vb->Unlock();



	if(g_build.gameType == GAMETYPE_SNOOKER) scale[0] = 10.0f;//7.0f;
	else if(g_build.gameType == GAMETYPE_POOL) scale[0] = 5.5f;
	
	
	// Calculate Scale matrix
	D3DXMatrixScaling(&matScale, scale[0], scale[1], scale[2]);

	// Calculate rotation matrix
	D3DXMatrixRotationX( &matRotX, rot[0] );
	D3DXMatrixRotationY( &matRotY, rot[1] );
	D3DXMatrixRotationZ( &matRotZ, rot[2] );
	
	// Calculate a translation matrix
	D3DXMatrixTranslation(&matTrans, pos[0], pos[1], pos[2]);

	matWorld = matScale*(matRotX*matRotY*matRotZ)*matTrans;
	g_pDevice->SetTransform(D3DTS_WORLD, &matWorld);		




	M3d_drawmesh(&m_M3dmap_uplate_mesh);


	//M3d_drawmesh(&m_M3dmap_cube_mesh);

	////////////////////////////////////////////////
#endif
}

void CSnooker::DrawSpot(M3d_vtx pos, COLORREF rgba, float size)
{
	M3d_vtx rot;
	M3d_vtx scale;
	float thickness = 2.0f;
	
	rot[0] = 0;
	rot[1] = 0;
	rot[2] = 0;
	scale[0] = 0.1f*size;
	scale[1] = 0.025f*size*thickness;
	scale[2] = 0;
	DrawPlate(pos, rot, scale, rgba);
	scale[0] = 0.025f*size*thickness;
	scale[1] = 0.1f*size;
	scale[2] = 0;
	DrawPlate(pos, rot, scale, rgba);
}

void CSnooker::DrawArc(M3d_vtx origin, float radius, float startAngle, float endAngle, COLORREF rgba, float thickness, unsigned int steps)
{
	M3d_vtx pos;
	M3d_vtx rot;
	M3d_vtx scale;
	int n;
	float angleStep;
	float angle;
	float angleSpread;
	float stripLength;
	angleSpread = endAngle-startAngle;
	stripLength = radius*(angleSpread*DEG2RAD)/((float)steps);

	
	angleStep = (endAngle - startAngle) / (float)steps;
	pos[2] = origin[2];

	for(n=0;n<steps+1;n++)
	{
		angle = (startAngle + ((float)n) * (float)angleStep) * DEG2RAD;
		pos[0] = origin[0]-radius*sin(angle);
		pos[1] = origin[1]+radius*cos(angle);
		
		rot[0] = 0;
		rot[1] = 0;
		rot[2] = angle;
		//scale[0] = 1.0f / steps*10;//11;////(radius/**0.5f*/)/CUBE_FACE_HEIGHT;
		scale[0] = stripLength * 0.01;//(1.0f / 100);
		scale[1] = thickness;
		scale[2] = 0;
		DrawPlate(pos, rot, scale, rgba);
	}
}

void CSnooker::DrawCircle(M3d_vtx origin, float radius, COLORREF rgba, float thickness, unsigned int steps)
{

	


	/*
	M3d_vtx pos;
	M3d_vtx rot;
	M3d_vtx scale;
	int n;
	float angleStep;
	float angle;
		
	angleStep = (360 - 0) / (float)steps;
	pos[2] = origin[2];

	for(n=0;n<steps+1;n++)
	{
		angle = (0 + ((float)n) * angleStep) * DEG2RAD;
		pos[0] = origin[0]-radius*sin(angle);
		pos[1] = origin[1]+radius*cos(angle);
		
		rot[0] = 0;
		rot[1] = 0;
		rot[2] = angle;
		scale[0] = 1.0f / steps*11;////(radius)/CUBE_FACE_HEIGHT;
		scale[1] = thickness;
		scale[2] = 0;
		DrawPlate2(pos, rot, scale, rgba);
	}
	*/
}

//////////////////////////////////
// chalk marks engine
#define MAX_CHALKMARKS		32

typedef struct {
BOOL status;
float alpha;
M3d_vtx pos;
M3d_vtx rot;
M3d_vtx scale;
DWORD col;
} chalk;

int chalk_idx;

chalk chalks[MAX_CHALKMARKS];

void CSnooker::InitChalkMarks(void)
{	
	for(int n = 0; n < MAX_CHALKMARKS; n++)
		chalks[n].status = FALSE;

	chalk_idx = 0;

	m_bRedrawChalkMarks = TRUE;//FALSE; // false chalk marks to be re-drawn (helps to clean out marks on player restart!!)
}


void CSnooker::MakeChalkMark(CBall *ball)
{
	M3d_vtx pos;
	M3d_vtx rot;
	M3d_vtx scale;
	float angle2D;
	float mag;	
	
	if(m_cuetip[1] < 0)
	{
		//m_bRedrawChalkMarks = TRUE;

		if(g_build.gameType == GAMETYPE_SNOOKER) 
			{
			mag = - ball->m_V.Magnitude() * (m_cuetip[1] / 0.65f);	
			mag*=2; // scale length of chalk mark

			chalks[chalk_idx].alpha = mag / 10;
			chalks[chalk_idx].alpha *= (m_cuetip[1] / 0.65f);


			if(chalks[chalk_idx].alpha < 0) chalks[chalk_idx].alpha = -chalks[chalk_idx].alpha;
			if(chalks[chalk_idx].alpha > 1.0) chalks[chalk_idx].alpha = 1.0;
	
			chalks[chalk_idx].status = TRUE;
			//angle2D = 90*DEG2RAD + atan2((double)ball->m_V.y, (double)ball->m_V.x);
			float a = ball->m_V.x;
			float b = ball->m_V.y;
			if(a > -0.001 && a < 0.001) a = 0.001;
			float ang = atan(b/a);
			angle2D = (float)(90.0f*DEG2RAD) + ang;

			//angle2D = 90*DEG2RAD + atan2((double)ball->m_V.y, (double)ball->m_V.x);
			
			chalks[chalk_idx].rot[0] = 0;
			chalks[chalk_idx].rot[1] = 0;
			chalks[chalk_idx].rot[2] = angle2D;
			
			chalks[chalk_idx].pos[0] = ball->m_P.x + sin(angle2D) * 150*(mag * 0.2f)/2;
			chalks[chalk_idx].pos[1] = ball->m_P.y - cos(angle2D) * 150*(mag * 0.2f)/2;
			//chalks[chalk_idx].pos[2] = BALL_RADIUS*(1.8f);//ball->m_P.z + BALL_RADIUS;
			chalks[chalk_idx].pos[2] = 0.1;//(BALL_POSITIONZOFFSET + BALL_DRAWOFFSETZ) - 0.45f*BALL_RADIUS;
		
			chalks[chalk_idx].scale[0] = 0.10f * 100.0f;
			chalks[chalk_idx].scale[1] = mag * 0.20f * 100.0f;
			chalks[chalk_idx].scale[2] = 0;
			}
		else{
			mag = - FROMFIX(ball->f_V.Magnitude()) * (m_cuetip[1] / 0.65f);	
			mag*=2; // scale length of chalk mark
	
			chalks[chalk_idx].alpha = mag / 10;
			chalks[chalk_idx].alpha *= (m_cuetip[1] / 0.65f);
			
			if(chalks[chalk_idx].alpha < 0) chalks[chalk_idx].alpha = -chalks[chalk_idx].alpha;
			if(chalks[chalk_idx].alpha > 1.0) chalks[chalk_idx].alpha = 1.0;
	

			chalks[chalk_idx].status = TRUE;
			float a = ball->f_V.x;
			float b = ball->f_V.y;

			if(a > -0.001 && a < 0.001) a = 0.001;
			float ang = atan(b/a);

			angle2D = (float)(90.0f*DEG2RAD) + ang;
			chalks[chalk_idx].rot[0] = 0;
			chalks[chalk_idx].rot[1] = 0;
			chalks[chalk_idx].rot[2] = angle2D;
			
			chalks[chalk_idx].pos[0] = FROMFIX(ball->f_P.x) + sin(angle2D) * 200*(mag * 0.2f)/2;
			chalks[chalk_idx].pos[1] = FROMFIX(ball->f_P.y) - cos(angle2D) * 200*(mag * 0.2f)/2;
			//chalks[chalk_idx].pos[2] = BALL_RADIUS*(1.8f);//ball->m_P.z + BALL_RADIUS;
			chalks[chalk_idx].pos[2] = 0.1;//(BALL_POSITIONZOFFSET + BALL_DRAWOFFSETZ) - 0.45f*BALL_RADIUS;
			
			chalks[chalk_idx].scale[0] = 0.12f * 100.0f;
			chalks[chalk_idx].scale[1] = mag * 0.25f * 100.0f;
			chalks[chalk_idx].scale[2] = 0;

			
			}

		/*
		 * Set up Chalk Color
		 */
		/////// Find Player Indexes /////////
		int plr1Idx =-1;
		int plr2Idx =-1;


		int tbl = g_gameAppView->m_lobby->GetTableIdxFromPlayer(g_gameAppView->m_playerSockIdx);
		if(tbl!=-1)
		{
			plr1Idx = g_gameAppView->m_lobby->GetPlayerIdx(m_table[tbl].player1SockIdx);
			plr2Idx = g_gameAppView->m_lobby->GetPlayerIdx(m_table[tbl].player2SockIdx);
		}

		// Default Blue Chalk
		chalks[chalk_idx].col = chalk_col[0];
		int invent_idx = 0;
			
		if(m_bTurn1P) 
		{
			if(plr1Idx==-1);
			else if(m_player[plr1Idx].bIsGuest);
			else
			{
				invent_idx = (m_player[plr1Idx].chalktype) & 0xff;
				chalks[chalk_idx].col = chalk_col[invent_idx];
			}
		}
		else
		{
			if(plr2Idx==-1);
			else if(m_player[plr2Idx].bIsGuest);
			else
			{
				invent_idx = (m_player[plr2Idx].chalktype) & 0xff;
				chalks[chalk_idx].col = chalk_col[invent_idx];
			}
		}


		// circular chalk buffer wrap...
		if(++chalk_idx == MAX_CHALKMARKS)
			chalk_idx = 0;
	}
}

void CSnooker::DrawChalkMarks(void)
{	// [!] Weird bug with chalk marks flashing gotta sort that out eventually!!! change to old color and you'll get the bug!
	// Bug is caused due to balls rotating, rem out ball's Draw_mesh's final rendering line and it's ok.
	// something due to shared resources/pipelining shit!
	for(int n = 0; n < MAX_CHALKMARKS; n++)
	if(chalks[n].status)
	{
		DrawTri(chalks[n].pos, chalks[n].rot, chalks[n].scale, chalks[n].col, chalks[n].alpha);//RGBA(0,60,100,35));
	}
}
//////////////////////////////////

DWORD CSnooker::GenerateBallCRC(void)
{
	int m;
	DWORD crc;
	DWORD *t;

	crc = 0;

	for(m=0;m<MAX_TOTALBALLS;m++)
	if(TestFlag(m_ball[m]->m_flags, Flg_BALL_ACTIVE))
	{
		t = (DWORD *)&m_ball[m]->m_P.x;
		crc+= *t;

		t = (DWORD *)&m_ball[m]->m_P.y;
		crc+= *t;

		t = (DWORD *)&m_ball[m]->m_P.z;
		crc+= *t;
	}
	return crc;
}

DWORD CSnooker::GenerateBallCRCF(void)
{
	int m;
	DWORD crc;
	DWORD *t;

	crc = 0;

	for(m=0;m<MAX_TOTALBALLS;m++)
	if(TestFlag(m_ball[m]->m_flags, Flg_BALL_ACTIVE))
	{
		t = (DWORD *)&m_ball[m]->f_P.x;
		crc+= *t;
		crc+= t[1];

		t = (DWORD *)&m_ball[m]->f_P.y;
		crc+= *t;
		crc+= t[1];

		t = (DWORD *)&m_ball[m]->f_P.z;
		crc+= *t;
		crc+= t[1];
	}
	return crc;
}

void CSnooker::DrawBallShadow(int n)
{
#ifndef MAIN_SERVER


	if(!TestFlag(m_ball[n]->m_flags, Flg_BALL_ACTIVE)) return;
	M3d_vtx pos;
	M3d_vtx scale;
	// complex shadows
	M3d_vtx shadow_pos;

#ifdef FIXEDMODE
	pos[0] = FROMFIXSMALL(m_ball[n]->f_P.x); // (-)
	pos[1] = FROMFIXSMALL(m_ball[n]->f_P.y); // (-)
	pos[2] = FROMFIXSMALL(m_ball[n]->f_P.z) + BALL_DRAWOFFSETZ;
#else
	pos[0] = m_ball[n]->m_P.x; // (-)
	pos[1] = m_ball[n]->m_P.y; // (-)
	pos[2] = m_ball[n]->m_P.z + BALL_DRAWOFFSETZ+2;
#endif

#ifdef BALL_RESCALEMESH
	{
	float f;
	if(g_build.gameType == GAMETYPE_SNOOKER) f = 26.09f;
	else  f = 28.4625f;
	scale[0] = (BALL_RADIUS/f)*0.115f;
	scale[1] = (BALL_RADIUS/f)*0.115f;
	scale[2] = (BALL_RADIUS/f)*0.1f;
	}
#else
scale[0] = 0.1f;
scale[1] = 0.1f;
scale[2] = 0.1f;
#endif

	BOOL success;
	D3DXMATRIX matRotX, matRotY, matRotZ, matTrans, matScale;

	////////// Draw The Ball's Shadow //////////////////////////////
	if(!g_gfxDetails.bOptionsComplexShadows) // normal shadows
	{
		int l;

		//g_pDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE); // Disable Z-Buf. Writing

		float atten;
		atten = 0.8f;

		shadow_pos[0] = pos[0];
		shadow_pos[1] = pos[1];
		shadow_pos[2] = pos[2];

		// Calculate Scale matrix
		float ratiox = pos[0] - shadow_pos[0];
		if(ratiox < 0) ratiox = -ratiox;
		ratiox /= BALL_RADIUS * 2.0f;
		ratiox += 0.9f;

		float ratioy = pos[1] - shadow_pos[1];
		if(ratioy < 0) ratioy = -ratioy;
		ratioy /= BALL_RADIUS * 2.0f;
		ratioy += 0.9f;


		D3DXMatrixScaling(&matScale, scale[0]*ratiox, scale[1]*ratioy, scale[2]);

		// Calculate rotation matrix
		D3DXMatrixRotationX( &matRotX, I_rot[0] );
		D3DXMatrixRotationY( &matRotY, I_rot[1] );
		D3DXMatrixRotationZ( &matRotZ, I_rot[2] );
		
		// Calculate a translation matrix
		D3DXMatrixTranslation(&matTrans, shadow_pos[0], shadow_pos[1], shadow_pos[2]);

		D3DXMATRIX matWorld = matScale*(matRotX*matRotY*matRotZ)*matTrans;
		g_pDevice->SetTransform(D3DTS_WORLD, &matWorld);

		m_M3d_Ball_Shadow.mat[0].d3dm.Diffuse.r = 0;
		m_M3d_Ball_Shadow.mat[0].d3dm.Diffuse.g = 0;
		m_M3d_Ball_Shadow.mat[0].d3dm.Diffuse.b = 0;
		if(m_customTable->m_type == CTTYPE_UKPOOL)	m_M3d_Ball_Shadow.mat[0].d3dm.Diffuse.a = 0.7*atten;
		else m_M3d_Ball_Shadow.mat[0].d3dm.Diffuse.a = 0.5*atten;

		m_M3d_Ball_Shadow.mat[0].d3dm.Ambient.r = 0;
		m_M3d_Ball_Shadow.mat[0].d3dm.Ambient.g = 0;
		m_M3d_Ball_Shadow.mat[0].d3dm.Ambient.b = 0;
		if(m_customTable->m_type == CTTYPE_UKPOOL) m_M3d_Ball_Shadow.mat[0].d3dm.Ambient.a = 0.7*atten;
		else m_M3d_Ball_Shadow.mat[0].d3dm.Ambient.a = atten;
		
		m_M3d_Ball_Shadow.mat[0].d3dm.Emissive.r = 0;
		m_M3d_Ball_Shadow.mat[0].d3dm.Emissive.g = 0;
		m_M3d_Ball_Shadow.mat[0].d3dm.Emissive.b = 0;
		m_M3d_Ball_Shadow.mat[0].d3dm.Emissive.a = 1*atten;
		
		m_M3d_Ball_Shadow.mat[0].d3dm.Specular.r = 0;
		m_M3d_Ball_Shadow.mat[0].d3dm.Specular.g = 0;
		m_M3d_Ball_Shadow.mat[0].d3dm.Specular.b = 0;
		m_M3d_Ball_Shadow.mat[0].d3dm.Specular.a = 1*atten;

		M3d_objectdraw(&m_M3d_Ball_Shadow);

		//g_pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE); // Disable Z-Buf. Writing

		return;
	}

	int l;
	
	
	g_pDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE); // Disable Z-Buf. Writing

for(l=0;l<3;l++)
{
	float atten;
	atten = 1;

	success = ((CGameAppView *)m_parent)->m_environment->ShadowCast(pos, shadow_pos, pos[2] - BALL_RADIUS,l,atten);
	if(!success) continue;

	// Shadows work better when not on top of each each in z axis, so offset them by 1 unit, seems to fix this flicker bug on some Graphics Cards..
	//shadow_pos[2]+=BALL_RADIUS-((float)l)*0.1; // get shadow back to zero
	shadow_pos[2]+=BALL_RADIUS;
	//BOOL ShadowCast(M3d_vtx pos, M3d_vtx shadowPos, float zPlane,int lightIdx);

	//M3d_transformobject(shadow_pos, I_rot, scale); // For still (non keyframes) meshes.



	// Calculate Scale matrix
	float ratiox = pos[0] - shadow_pos[0];
	if(ratiox < 0) ratiox = -ratiox;
	ratiox /= BALL_RADIUS * 2.0f;
	ratiox += 1.0f;

	float ratioy = pos[1] - shadow_pos[1];
	if(ratioy < 0) ratioy = -ratioy;
	ratioy /= BALL_RADIUS * 2.0f;
	ratioy += 1.0f;

	D3DXMatrixScaling(&matScale, scale[0]*ratiox, scale[1]*ratioy, scale[2]);

	// Calculate rotation matrix
	D3DXMatrixRotationX( &matRotX, I_rot[0] );
	D3DXMatrixRotationY( &matRotY, I_rot[1] );
	D3DXMatrixRotationZ( &matRotZ, I_rot[2] );
	
	// Calculate a translation matrix
	D3DXMatrixTranslation(&matTrans, shadow_pos[0], shadow_pos[1], shadow_pos[2]);

	D3DXMATRIX matWorld = matScale*(matRotX*matRotY*matRotZ)*matTrans;
	g_pDevice->SetTransform(D3DTS_WORLD, &matWorld);

	m_M3d_Ball_Shadow.mat[0].d3dm.Diffuse.r = 0;
	m_M3d_Ball_Shadow.mat[0].d3dm.Diffuse.g = 0;
	m_M3d_Ball_Shadow.mat[0].d3dm.Diffuse.b = 0;
	if(m_customTable->m_type == CTTYPE_UKPOOL)	m_M3d_Ball_Shadow.mat[0].d3dm.Diffuse.a = 0.7*atten;
	else m_M3d_Ball_Shadow.mat[0].d3dm.Diffuse.a = 0.5*atten;

	m_M3d_Ball_Shadow.mat[0].d3dm.Ambient.r = 0;
	m_M3d_Ball_Shadow.mat[0].d3dm.Ambient.g = 0;
	m_M3d_Ball_Shadow.mat[0].d3dm.Ambient.b = 0;
	if(m_customTable->m_type == CTTYPE_UKPOOL) m_M3d_Ball_Shadow.mat[0].d3dm.Ambient.a = 0.7*atten;
	else m_M3d_Ball_Shadow.mat[0].d3dm.Ambient.a = atten;
	
	m_M3d_Ball_Shadow.mat[0].d3dm.Emissive.r = 0;
	m_M3d_Ball_Shadow.mat[0].d3dm.Emissive.g = 0;
	m_M3d_Ball_Shadow.mat[0].d3dm.Emissive.b = 0;
	m_M3d_Ball_Shadow.mat[0].d3dm.Emissive.a = 1*atten;
	
	m_M3d_Ball_Shadow.mat[0].d3dm.Specular.r = 0;
	m_M3d_Ball_Shadow.mat[0].d3dm.Specular.g = 0;
	m_M3d_Ball_Shadow.mat[0].d3dm.Specular.b = 0;
	m_M3d_Ball_Shadow.mat[0].d3dm.Specular.a = 1*atten;

	M3d_objectdraw(&m_M3d_Ball_Shadow);
}

	g_pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE); // Disable Z-Buf. Writing

#endif
}
 
float KpoolColorMap[][3] = {
{_256TO1(255),_256TO1(255),_256TO1(255)}, // White Ball
{_256TO1(218),_256TO1(234),_256TO1(39)}, // 1 Ball
{_256TO1(0),_256TO1(34),_256TO1(161)}, // 2 Ball
{_256TO1(188),_256TO1(0),_256TO1(0)}, // 3 Ball
{_256TO1(99),_256TO1(0),_256TO1(114)}, // 4 Ball
{_256TO1(255),_256TO1(132),_256TO1(0)}, // 5 Ball
{_256TO1(10),_256TO1(103),_256TO1(0)}, // 6 Ball
{_256TO1(76),_256TO1(27),_256TO1(0)}, // 7 Ball
{_256TO1(0),_256TO1(0),_256TO1(0)}, // 8 Ball
};

void ModifyBallMats(M3d_object *m_M3d_ptr, int _n)
{
	int i;
	int n;
	n = _n;
	if(n>8) n-=8; // 9 -> 1

	if(n>8) n = 8; // safety lock


	M3d_objectfindreset(m_M3d_ptr);

	while((i = M3d_objectfind("white"))!=-1)
	//i = 0;
	{
	m_M3d_ptr->mat[i].d3dm.Diffuse.r=1;
	m_M3d_ptr->mat[i].d3dm.Diffuse.g=1;
	m_M3d_ptr->mat[i].d3dm.Diffuse.b=1;
	}
	// --- //

	M3d_objectfindreset(m_M3d_ptr);
	while((i = M3d_objectfind("ball"))!=-1)
	//i = 1;
	{
	m_M3d_ptr->mat[i].d3dm.Diffuse.r=KpoolColorMap[n][0];
	m_M3d_ptr->mat[i].d3dm.Diffuse.g=KpoolColorMap[n][1];
	m_M3d_ptr->mat[i].d3dm.Diffuse.b=KpoolColorMap[n][2];	
	}
}


void ModifyBallMatsUKPool(M3d_object *m_M3d_ptr, int n)
{
	int i;

	M3d_objectfindreset(m_M3d_ptr);
		
	for(i=0;i<m_M3d_ptr->mesh_n;i++)
		{
		if(n==0) // White Ball
			{
			m_M3d_ptr->mat[i].d3dm.Diffuse.r=1;
			m_M3d_ptr->mat[i].d3dm.Diffuse.g=1;
			m_M3d_ptr->mat[i].d3dm.Diffuse.b=1;
			}
		else if(n==8) { // Black - 8Ball
			m_M3d_ptr->mat[i].d3dm.Diffuse.r=0.1;
			m_M3d_ptr->mat[i].d3dm.Diffuse.g=0.1;
			m_M3d_ptr->mat[i].d3dm.Diffuse.b=0.1;
			}
		else if(n<=7) // Yellow Balls
			{
			m_M3d_ptr->mat[i].d3dm.Diffuse.r=1;
			m_M3d_ptr->mat[i].d3dm.Diffuse.g=0;
			m_M3d_ptr->mat[i].d3dm.Diffuse.b=0;
			}
		else { // Red Balls
			m_M3d_ptr->mat[i].d3dm.Diffuse.r=1;
			m_M3d_ptr->mat[i].d3dm.Diffuse.g=1;
			m_M3d_ptr->mat[i].d3dm.Diffuse.b=0;
			}
		}
	/*M3d_objectfindreset(m_M3d_ptr);
		
	while((i = M3d_objectfind("white"))!=-1)
		{
		m_M3d_ptr->mat[i].d3dm.Diffuse.r=1;
		m_M3d_ptr->mat[i].d3dm.Diffuse.g=1;
		m_M3d_ptr->mat[i].d3dm.Diffuse.b=1;
		}*/
}


void ModifyBallMatsBreak(M3d_object *m_M3d_ptr, int n)
{
	int i;

	M3d_objectfindreset(m_M3d_ptr);
		
	for(i=0;i<m_M3d_ptr->mesh_n;i++)
		{
		if(n==0) // White Ball
			{
			m_M3d_ptr->mat[i].d3dm.Diffuse.r=1;
			m_M3d_ptr->mat[i].d3dm.Diffuse.g=1;
			m_M3d_ptr->mat[i].d3dm.Diffuse.b=1;
			}
		else if(n<=10) { // Reds
			m_M3d_ptr->mat[i].d3dm.Diffuse.r=0.8;
			m_M3d_ptr->mat[i].d3dm.Diffuse.g=0;
			m_M3d_ptr->mat[i].d3dm.Diffuse.b=0;
			}
		else if(n==11) // Green Ball
			{
			m_M3d_ptr->mat[i].d3dm.Diffuse.r=0;
			m_M3d_ptr->mat[i].d3dm.Diffuse.g=0.8;
			m_M3d_ptr->mat[i].d3dm.Diffuse.b=0;
			}
		else if(n==12) // Blue Ball
			{
			m_M3d_ptr->mat[i].d3dm.Diffuse.r=0;
			m_M3d_ptr->mat[i].d3dm.Diffuse.g=0;
			m_M3d_ptr->mat[i].d3dm.Diffuse.b=0.8;
			}
		else { // Black Balls
			m_M3d_ptr->mat[i].d3dm.Diffuse.r=0.1;
			m_M3d_ptr->mat[i].d3dm.Diffuse.g=0.1;
			m_M3d_ptr->mat[i].d3dm.Diffuse.b=0.1;
			}
		}
/*
	M3d_objectfindreset(m_M3d_ptr);
		
	while((i = M3d_objectfind("white"))!=-1)
		{
		m_M3d_ptr->mat[i].d3dm.Diffuse.r=1;
		m_M3d_ptr->mat[i].d3dm.Diffuse.g=1;
		m_M3d_ptr->mat[i].d3dm.Diffuse.b=1;
		}*/
	
}


void ModifyBallMatsBowls(M3d_object *m_M3d_ptr, int n)
{
	int i;

	M3d_objectfindreset(m_M3d_ptr);

	while((i = M3d_objectfind("white"))!=-1)
	//i = 0;
	{
	if(n==0) // Jack
		{
		m_M3d_ptr->mat[i].d3dm.Diffuse.r=1;
		m_M3d_ptr->mat[i].d3dm.Diffuse.g=1;
		m_M3d_ptr->mat[i].d3dm.Diffuse.b=1;
		}
	else if(n<=5) // Blue Bowls
		{
		m_M3d_ptr->mat[i].d3dm.Diffuse.r=0;
		m_M3d_ptr->mat[i].d3dm.Diffuse.g=0;
		m_M3d_ptr->mat[i].d3dm.Diffuse.b=1;
		}
	else if(n<=10) { // Red Bowls
		m_M3d_ptr->mat[i].d3dm.Diffuse.r=1;
		m_M3d_ptr->mat[i].d3dm.Diffuse.g=0;
		m_M3d_ptr->mat[i].d3dm.Diffuse.b=0;
		}
	else { // unused bowls
		m_M3d_ptr->mat[i].d3dm.Diffuse.r=1;
		m_M3d_ptr->mat[i].d3dm.Diffuse.g=1;
		m_M3d_ptr->mat[i].d3dm.Diffuse.b=1;
		}
	}
	// --- //

	M3d_objectfindreset(m_M3d_ptr);
	while((i = M3d_objectfind("ball"))!=-1) // Main part of ball is black
	//i = 1;
	{
	if(n==0) // Jack
		{
		m_M3d_ptr->mat[i].d3dm.Diffuse.r=1;
		m_M3d_ptr->mat[i].d3dm.Diffuse.g=1;
		m_M3d_ptr->mat[i].d3dm.Diffuse.b=1;
		}
	else if(n<=10)
		{
		m_M3d_ptr->mat[i].d3dm.Diffuse.r=0;
		m_M3d_ptr->mat[i].d3dm.Diffuse.g=0;
		m_M3d_ptr->mat[i].d3dm.Diffuse.b=0;	
		}
	else { // unused bowls.
		m_M3d_ptr->mat[i].d3dm.Diffuse.r=1;
		m_M3d_ptr->mat[i].d3dm.Diffuse.g=1;
		m_M3d_ptr->mat[i].d3dm.Diffuse.b=1;	
		}
	}
}

void UpdateBallMats(M3d_object *m_M3d_ptr, int n)
{
	int i;
	for(i=1;i<m_M3d_ptr->mesh_n;i++)
	{
	m_M3d_ptr->mat[i].d3dm.Diffuse.a = m_M3d_ptr->mat[0].d3dm.Diffuse.a;

	m_M3d_ptr->mat[i].d3dm.Specular.r = m_M3d_ptr->mat[0].d3dm.Specular.r;
	m_M3d_ptr->mat[i].d3dm.Specular.g = m_M3d_ptr->mat[0].d3dm.Specular.g;
	m_M3d_ptr->mat[i].d3dm.Specular.b = m_M3d_ptr->mat[0].d3dm.Specular.b;
	m_M3d_ptr->mat[i].d3dm.Specular.a = m_M3d_ptr->mat[0].d3dm.Specular.a;

	m_M3d_ptr->mat[i].d3dm.Ambient.r = m_M3d_ptr->mat[0].d3dm.Ambient.r;
	m_M3d_ptr->mat[i].d3dm.Ambient.g = m_M3d_ptr->mat[0].d3dm.Ambient.g;
	m_M3d_ptr->mat[i].d3dm.Ambient.b = m_M3d_ptr->mat[0].d3dm.Ambient.b;
	m_M3d_ptr->mat[i].d3dm.Ambient.a = m_M3d_ptr->mat[0].d3dm.Ambient.a;

	m_M3d_ptr->mat[i].d3dm.Emissive.r = m_M3d_ptr->mat[0].d3dm.Emissive.r;
	m_M3d_ptr->mat[i].d3dm.Emissive.g = m_M3d_ptr->mat[0].d3dm.Emissive.g;
	m_M3d_ptr->mat[i].d3dm.Emissive.b = m_M3d_ptr->mat[0].d3dm.Emissive.b;
	m_M3d_ptr->mat[i].d3dm.Emissive.a = m_M3d_ptr->mat[0].d3dm.Emissive.a;

	m_M3d_ptr->mat[i].d3dm.Power = m_M3d_ptr->mat[0].d3dm.Power;	
	}
}


void CSnooker::DrawBall(int n, BOOL bTrans)
{
#ifndef MAIN_SERVER

	/*
	if ((rand()%1000) == 1)
	if (n == 0)
	{
		float px = sqrt(-1);
		m_ball[n]->m_P.x = px;
		m_ball[n]->m_P.y = px;
	}
	*/
	// g_pDevice->SetRenderState(D3DRS_ZENABLE, false);
	//g_pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, true);
	DrawBallShadow(n);
	// g_pDevice->SetRenderState(D3DRS_ZENABLE, true);


	if(!TestFlag(m_ball[n]->m_flags, Flg_BALL_ACTIVE)) return;
	//if(!TestFlag(m_ball[n]->m_flags, Flg_BALL_ACTIVE)) 
	//if(n<10) n = 2; 
	//else return;

	M3d_vtx pos;
	M3d_vtx rot;
	// complex shadows
	M3d_vtx shadow_pos;

	M3d_vtx scale;
	M3d_object *Ball_ptr;
	float r = (float)(GetRValue(ballColor[m_ball[n]->m_colour])/255.0f);
	float g = (float)(GetGValue(ballColor[m_ball[n]->m_colour])/255.0f);
	float b = (float)(GetBValue(ballColor[m_ball[n]->m_colour])/255.0f);

	

	if (g_build.gameType == GAMETYPE_SNOOKER)
	{
		// Cue Ball
		if (m_ball[n]->m_colour == 0)
		{
			
			if (g_gfxDetails.bOptionsDottedCueBall)
			{
				Ball_ptr = &m_M3d_Ball_ptr[1];
			}
			else
			{
				Ball_ptr = &m_M3d_Ball_ptr[0];
			}
			
		}
		else
		{
			// If colour blind is off, just use one ball mesh, otherwise we need to use meshes with numbers on them
			if (g_gfxDetails.bOptionsColourBlind == false)
			{
				Ball_ptr = &m_M3d_Ball_ptr[0]; // snooker uses one ball mesh
 			}
			else
			{
				if (m_ball[n]->m_colour == 1) Ball_ptr = &m_M3d_Ball_ptr[2];
				else if (m_ball[n]->m_colour == 2) Ball_ptr = &m_M3d_Ball_ptr[3];
				else if (m_ball[n]->m_colour == 3) Ball_ptr = &m_M3d_Ball_ptr[4];
				else if (m_ball[n]->m_colour == 4) Ball_ptr = &m_M3d_Ball_ptr[5];
				else if (m_ball[n]->m_colour == 5) Ball_ptr = &m_M3d_Ball_ptr[6];
				else if (m_ball[n]->m_colour == 6) Ball_ptr = &m_M3d_Ball_ptr[7];
				else if (m_ball[n]->m_colour == 7) Ball_ptr = &m_M3d_Ball_ptr[8];
				else Ball_ptr = &m_M3d_Ball_ptr[0]; // Solid ball by default
			}
			
		}
	}
	else {
			if(g_bGfxQuality==FALSE) // Low Quality.
			{
			if(m_customTable->m_type == CTTYPE_BOWLS)
				{
				if(n==0) Ball_ptr = &m_M3d_Ball_ptr[0];	// Solid for jack, less likely to notice the artifacts.
				else Ball_ptr = &m_M3d_Ball_ptr[1];	// Stripes
				}
			else if(m_customTable->m_type == CTTYPE_UKPOOL) Ball_ptr = &m_M3d_Ball_ptr[0];
			else if(m_customTable->m_type == CTTYPE_BREAK) Ball_ptr = &m_M3d_Ball_ptr[0];
			else{
				if(n<9) Ball_ptr = &m_M3d_Ball_ptr[0]; // Solids (and white ball)
				else Ball_ptr = &m_M3d_Ball_ptr[1];	// Stripes
				}
			}
			else { // HQ
			//If UK Pool, We use Different Balls //
			if(m_customTable->m_type == CTTYPE_UKPOOL)
				{
				if(n>=MAX_WHITEBALLS)
					{
					if((n-MAX_WHITEBALLS) <=6 ) Ball_ptr = &m_M3d_Ball_ptr[17]; // Red 
					else if((n-MAX_WHITEBALLS) >=8 ) Ball_ptr = &m_M3d_Ball_ptr[16]; // Yellow Ball
					else Ball_ptr = &m_M3d_Ball_ptr[n]; // 8-Ball
					}
				else Ball_ptr = &m_M3d_Ball_ptr[n];	// White Ball
				}
			//If Break, We use Different Balls //
			else if(m_customTable->m_type == CTTYPE_BREAK)
				{
				if(n>=MAX_WHITEBALLS)
					{
					if(n>=1  && n<=10) Ball_ptr = &m_M3d_Ball_ptr[18]; // Red 
					else if(n==11) Ball_ptr = &m_M3d_Ball_ptr[19]; // Green 
					else if(n==12) Ball_ptr = &m_M3d_Ball_ptr[20]; // Blue 
					else Ball_ptr = &m_M3d_Ball_ptr[21]; // Black 
					}
				else Ball_ptr = &m_M3d_Ball_ptr[n];	// White Ball
				}
			else Ball_ptr = &m_M3d_Ball_ptr[n];	// the rest of the games have custom ball meshes
			}
		}

	// draw balls GFX

#ifdef BALL_RESCALEMESH
	{
	float f;
	if(g_build.gameType == GAMETYPE_SNOOKER) f = 26.09f;
	else  f = 28.4625f;
	scale[0] = (BALL_RADIUS/f)*0.1f;
	scale[1] = (BALL_RADIUS/f)*0.1f;
	scale[2] = (BALL_RADIUS/f)*0.1f;
	}
#else
scale[0] = 0.1f;
scale[1] = 0.1f;
scale[2] = 0.1f;
#endif

/// *NEW* DRAW BALLS 1.5% BIGGER
scale[0] *= 1.015f;
scale[1] *= 1.015f;
scale[2] *= 1.015f;
/////////////////

	rot[0] = 0;
	rot[1] = 0;
	rot[2] = 0;

	// Set up material for ball //
	if(g_build.gameType == GAMETYPE_SNOOKER)
	{
		if (g_gfxDetails.bOptionsColourBlind == false)
		{
			Ball_ptr->mat[0].d3dm.Diffuse.r = r;
			Ball_ptr->mat[0].d3dm.Diffuse.g = g;
			Ball_ptr->mat[0].d3dm.Diffuse.b = b;
		}
		else
		{
			Ball_ptr->mat[0].d3dm.Diffuse.r = 1;
			Ball_ptr->mat[0].d3dm.Diffuse.g = 1;
			Ball_ptr->mat[0].d3dm.Diffuse.b = 1;
		}


	} 
	else 
	{ 
		// Texture
		Ball_ptr->mat[0].d3dm.Diffuse.r = 1;
		Ball_ptr->mat[0].d3dm.Diffuse.g = 1;
		Ball_ptr->mat[0].d3dm.Diffuse.b = 1;
	}

	if(g_bGfxQuality==FALSE) // Low Quality.
	{
	if(m_customTable->m_type == CTTYPE_BOWLS) ModifyBallMatsBowls(Ball_ptr,n);
	else if(m_customTable->m_type == CTTYPE_UKPOOL) ModifyBallMatsUKPool(Ball_ptr,n);
	else if(m_customTable->m_type == CTTYPE_BREAK) ModifyBallMatsBreak(Ball_ptr,n);
	else ModifyBallMats(Ball_ptr,n);
	}

	
	// Safety //
	Ball_ptr->mat[0].d3dm.Diffuse.a = 1;

	float fade;
	fade = m_ball[n]->m_FadeOut;
	fade = fade / 200.0f;
	
	if(bTrans)	Ball_ptr->mat[0].d3dm.Diffuse.a = 0.7f;
	else Ball_ptr->mat[0].d3dm.Diffuse.a = fade;

	if (g_build.gameType == GAMETYPE_SNOOKER)
	{
		Ball_ptr->mat[0].d3dm.Specular.r = 0.9f;
		Ball_ptr->mat[0].d3dm.Specular.g = 0.9f;
		Ball_ptr->mat[0].d3dm.Specular.b = 0.9f;
		Ball_ptr->mat[0].d3dm.Specular.a = 1.0f;
	}
	else
	{
		Ball_ptr->mat[0].d3dm.Specular.r = 0.8f;
		Ball_ptr->mat[0].d3dm.Specular.g = 0.8f;
		Ball_ptr->mat[0].d3dm.Specular.b = 0.8f;
		Ball_ptr->mat[0].d3dm.Specular.a = 1.0f;
	}
	
	Ball_ptr->mat[0].d3dm.Ambient.r = 0.0f;
	Ball_ptr->mat[0].d3dm.Ambient.g = 0.0f;
	Ball_ptr->mat[0].d3dm.Ambient.b = 0.0f;
	Ball_ptr->mat[0].d3dm.Ambient.a = 1.0f;

	Ball_ptr->mat[0].d3dm.Emissive.r = 0.0f;
	Ball_ptr->mat[0].d3dm.Emissive.g = 0.0f;
	Ball_ptr->mat[0].d3dm.Emissive.b = 0.0f;
	Ball_ptr->mat[0].d3dm.Emissive.a = 1.0f;


	
	if (g_build.gameType == GAMETYPE_SNOOKER)
	{
		Ball_ptr->mat[0].d3dm.Power = g_fPower; //D3D_MATERIAL_POWER;//70;
	}
	else
	{
		Ball_ptr->mat[0].d3dm.Power = g_fPower * 1.5f; //D3D_MATERIAL_POWER;//70;
	}

	if(!m_cueBallSelect)
	if(m_ball[n]->m_bMouseOver && !bTrans)
	{
	int cyc = g_cyc - g_ballmouseoncyc;
	float f = sin(((float)cyc)*0.005)*0.25+0.5;
	//Ball_ptr->mat[0].d3dm.Specular.r+= f;
	//Ball_ptr->mat[0].d3dm.Specular.g+= f;
	//Ball_ptr->mat[0].d3dm.Specular.b+= f;
	//Ball_ptr->mat[0].d3dm.Diffuse.r+= f;
	//Ball_ptr->mat[0].d3dm.Diffuse.g+= f;
	//Ball_ptr->mat[0].d3dm.Diffuse.b+= f;

	Ball_ptr->mat[0].d3dm.Specular.a = f;
	Ball_ptr->mat[0].d3dm.Diffuse.a = f;
	Ball_ptr->mat[0].d3dm.Ambient.a = f;
	Ball_ptr->mat[0].d3dm.Emissive.a = f;
	}

	if(g_bGfxQuality==FALSE) // Low Quality.
	{
	UpdateBallMats(Ball_ptr,n);
	}

	// Set position //
#ifdef FIXEDMODE 
	if (n == 0)
	{
		n = n;

	}
	pos[0] = FROMFIXSMALL(m_ball[n]->f_P.x); // (-)
	pos[1] = FROMFIXSMALL(m_ball[n]->f_P.y); // (-)
	pos[2] = FROMFIXSMALL(m_ball[n]->f_P.z) + BALL_DRAWOFFSETZ;
#else
	pos[0] = m_ball[n]->m_P.x; // (-)
	pos[1] = m_ball[n]->m_P.y; // (-)
	pos[2] = m_ball[n]->m_P.z + BALL_DRAWOFFSETZ;
#endif
	

	//M3d_transformobject(pos, rot, scale); // For still (non keyframes) meshes.

	D3DXMATRIX matRotX, matRotY, matRotZ, matTrans, matScale;

	// Calculate Scale matrix
	D3DXMatrixScaling(&matScale, scale[0], scale[1], scale[2]);

	// Calculate rotation matrix
	D3DXMatrixRotationX( &matRotX, rot[0] );
	D3DXMatrixRotationY( &matRotY, rot[1] );
	D3DXMatrixRotationZ( &matRotZ, rot[2] );
	
	// Calculate a translation matrix
	D3DXMatrixTranslation(&matTrans, pos[0], pos[1], pos[2]);

	D3DXMATRIX matWorld = matScale*(matRotX*matRotY*matRotZ)*matTrans;
	g_pDevice->SetTransform(D3DTS_WORLD, &matWorld);


	g_pDevice->MultiplyTransform(D3DTS_WORLD, &m_ball[n]->m_d3dmtx);
		
	if(g_build.gameType == GAMETYPE_SNOOKER)
	{
		M3d_objectdraw(Ball_ptr); // materials dictate appearence in snooker
	}
	else if(g_build.gameType == GAMETYPE_POOL)
	{
		
		if(m_customTable->m_type == CTTYPE_BOWLS)
		{
			
			if(n != 0 && g_bGfxQuality==TRUE) // Bowls (object-balls) + high quality
			{
				//int n = 0;
				M3d_object_ptr object = Ball_ptr;
				M3d_material(&object->mat[0]);		// Set Material.
				 
				if(object->text_ref[0]>32767) M3d_texture_kill();
				else {
						 if(object->text_type[object->text_ref[0]]&M3D_THEAD_MAP_DIFFUSE)
						 {
							if((n<=5)) // Blue Bowls
								g_pDevice->SetTexture(0, m_spriteCueBallTexture[0]); //M3d_texture(&m_spriteCueBallTexture[0], 0);	// Set Texture.
							else if((n<=10)) // Red Bowls
								g_pDevice->SetTexture(0, m_spriteCueBallTexture[1]); // M3d_texture(&m_spriteCueBallTexture[1], 0);	// Set Texture.
							else M3d_texture_kill(); // (un-used)
						 }
						 else M3d_texture_kill();
					 }			
				
				/// VECTOR/MATRIX ///

				/// MESH/RENDERER ///
				M3d_drawmesh(&object->mesh[0]);		// Draw Mesh.
			
			}
			else // Jack (white-ball)
			{
				M3d_objectdraw(Ball_ptr); // materials dictate appearence in snooker
			}
		
		}
		else // default Pool balls (8-ball / 9-ball / Rotation-ball)		
		{
			M3d_objectdraw(Ball_ptr); // materials dictate appearence in snooker

		/*
			if(n != 0) // (object-balls)
			{
				M3d_object_ptr object = Ball_ptr;

				// UK style Reds/Yellows pool ;) (doesn't require textures)
				if(n<=7)
				{
					Ball_ptr->mat[0].d3dm.Diffuse.r = 1;
					Ball_ptr->mat[0].d3dm.Diffuse.g = 1;
					Ball_ptr->mat[0].d3dm.Diffuse.b = 1;
				}
				else if(n>=9)
				{
					Ball_ptr->mat[0].d3dm.Diffuse.r = 1;
					Ball_ptr->mat[0].d3dm.Diffuse.g = 1;
					Ball_ptr->mat[0].d3dm.Diffuse.b = 1;
				}
				
				M3d_material(&object->mat[0]);		// Set Material.
				 
				if(object->text_ref[0]>32767) M3d_texture_kill();
				else {
						 //if(object->text_type[object->text_ref[0]]&M3D_THEAD_MAP_DIFFUSE)
						 //{
						//	if((n<=5)) // Blue Bowls
						//		M3d_texture(&m_spriteCueBallTexture[0], 0);	// Set Texture.
						//	else if((n<=10)) // Red Bowls
						//		M3d_texture(&m_spriteCueBallTexture[1], 0);	// Set Texture.
						//	else M3d_texture_kill(); // (un-used)
						// }
						// else 
						M3d_texture_kill();
					 }			
				
				/// VECTOR/MATRIX ///

				/// MESH/RENDERER ///
				M3d_drawmesh(&object->mesh[0]);		// Draw Mesh.
			}
			else
			{
				M3d_objectdraw(Ball_ptr); // materials dictate appearence in snooker
			}
			*/

		}


	}

#endif
}


void CSnooker::DrawBallPos(M3d_vtx pos, float r, float g, float b, BOOL bTrans)
{
#ifndef MAIN_SERVER
	//M3d_vtx pos;
	M3d_vtx rot;
	M3d_vtx scale;
	//float r = (float)(GetRValue(ballColor[m_ball[n]->m_colour])/255.0f);
	//float g = (float)(GetGValue(ballColor[m_ball[n]->m_colour])/255.0f);
	//float b = (float)(GetBValue(ballColor[m_ball[n]->m_colour])/255.0f);

	// draw balls GFX
	/*scale[0] = (BALL_RADIUS/26.09)*0.1f;
	scale[1] = (BALL_RADIUS/26.09)*0.1f;
	scale[2] = (BALL_RADIUS/26.09)*0.1f;
	*/

#ifdef BALL_RESCALEMESH
	{
	float f;
	if(g_build.gameType == GAMETYPE_SNOOKER) f = 26.09f;
	else  f = 28.4625f;
	scale[0] = (BALL_RADIUS/f)*0.1f;
	scale[1] = (BALL_RADIUS/f)*0.1f;
	scale[2] = (BALL_RADIUS/f)*0.1f;
	}
#else
scale[0] = 0.1f;
scale[1] = 0.1f;
scale[2] = 0.1f;
#endif

	rot[0] = 0;
	rot[1] = 0;
	rot[2] = 0;

	// Set up material for ball //
	m_M3d_Ball_ptr->mat[0].d3dm.Diffuse.r = r;
	m_M3d_Ball_ptr->mat[0].d3dm.Diffuse.g = g;
	m_M3d_Ball_ptr->mat[0].d3dm.Diffuse.b = b;

	if(bTrans)	m_M3d_Ball_ptr->mat[0].d3dm.Diffuse.a = 0.7f;
	else m_M3d_Ball_ptr->mat[0].d3dm.Diffuse.a = 1;	

	m_M3d_Ball_ptr->mat[0].d3dm.Specular.r = 0.9;
	m_M3d_Ball_ptr->mat[0].d3dm.Specular.g = 0.9;
	m_M3d_Ball_ptr->mat[0].d3dm.Specular.b = 0.9;
	m_M3d_Ball_ptr->mat[0].d3dm.Specular.a = 1.0;
	
	m_M3d_Ball_ptr->mat[0].d3dm.Ambient.r = 0.0;
	m_M3d_Ball_ptr->mat[0].d3dm.Ambient.g = 0.0;
	m_M3d_Ball_ptr->mat[0].d3dm.Ambient.b = 0.0;
	m_M3d_Ball_ptr->mat[0].d3dm.Ambient.a = 0.0;


	m_M3d_Ball_ptr->mat[0].d3dm.Emissive.r = 0;
	m_M3d_Ball_ptr->mat[0].d3dm.Emissive.g = 0;
	m_M3d_Ball_ptr->mat[0].d3dm.Emissive.b = 0;
	m_M3d_Ball_ptr->mat[0].d3dm.Emissive.a = 1.0;

	m_M3d_Ball_ptr->mat[0].d3dm.Power = D3D_MATERIAL_POWER;//70;


	// Set position //
	//pos[0] = m_ball[n]->m_P.x; // (-)
	//pos[1] = m_ball[n]->m_P.y; // (-)
	//pos[2] = m_ball[n]->m_P.z + BALL_DRAWOFFSETZ;
			
	//M3d_transformobject(pos, rot, scale); // For still (non keyframes) meshes.

	D3DXMATRIX matRotX, matRotY, matRotZ, matTrans, matScale;

	// Calculate Scale matrix
	D3DXMatrixScaling(&matScale, scale[0], scale[1], scale[2]);

	// Calculate rotation matrix
	D3DXMatrixRotationX( &matRotX, rot[0] );
	D3DXMatrixRotationY( &matRotY, rot[1] );
	D3DXMatrixRotationZ( &matRotZ, rot[2] );
	
	// Calculate a translation matrix
	D3DXMatrixTranslation(&matTrans, pos[0], pos[1], pos[2]);

	D3DXMATRIX matWorld = matScale*(matRotX*matRotY*matRotZ)*matTrans;
	g_pDevice->SetTransform(D3DTS_WORLD, &matWorld);


	M3d_objectdraw(m_M3d_Ball_ptr);
#endif
}

void CSnooker::DrawBallPos2(M3d_vtx pos, float r, float g, float b, BOOL bTrans, M3d_vtx scaleMultiply)
{
#ifndef MAIN_SERVER
	//M3d_vtx pos;
	M3d_vtx rot;
	M3d_vtx scale;
	//float r = (float)(GetRValue(ballColor[m_ball[n]->m_colour])/255.0f);
	//float g = (float)(GetGValue(ballColor[m_ball[n]->m_colour])/255.0f);
	//float b = (float)(GetBValue(ballColor[m_ball[n]->m_colour])/255.0f);

	// draw balls GFX
	/*scale[0] = (BALL_RADIUS/26.09)*0.1f;
	scale[1] = (BALL_RADIUS/26.09)*0.1f;
	scale[2] = (BALL_RADIUS/26.09)*0.1f;
	*/

#ifdef BALL_RESCALEMESH
	{
	float f;
	if(g_build.gameType == GAMETYPE_SNOOKER) f = 26.09f;
	else  f = 28.4625f;
	scale[0] = (BALL_RADIUS/f)*0.1f;
	scale[1] = (BALL_RADIUS/f)*0.1f;
	scale[2] = (BALL_RADIUS/f)*0.1f;
	}
#else
scale[0] = 0.1f;
scale[1] = 0.1f;
scale[2] = 0.1f;
#endif

	scale[0] *= scaleMultiply[0];
	scale[1] *= scaleMultiply[1];
	scale[2] *= scaleMultiply[2];

	rot[0] = 0;
	rot[1] = 0;
	rot[2] = 0;

	// Set up material for ball //
	m_M3d_Ball_ptr->mat[0].d3dm.Diffuse.r = r;
	m_M3d_Ball_ptr->mat[0].d3dm.Diffuse.g = g;
	m_M3d_Ball_ptr->mat[0].d3dm.Diffuse.b = b;

	if(bTrans)	m_M3d_Ball_ptr->mat[0].d3dm.Diffuse.a = 0.7f;
	else m_M3d_Ball_ptr->mat[0].d3dm.Diffuse.a = 1;	

	m_M3d_Ball_ptr->mat[0].d3dm.Diffuse.a = 1.0f;

	m_M3d_Ball_ptr->mat[0].d3dm.Specular.r = 0;//0.9;
	m_M3d_Ball_ptr->mat[0].d3dm.Specular.g = 0;//0.9;
	m_M3d_Ball_ptr->mat[0].d3dm.Specular.b = 0;//0.9;
	m_M3d_Ball_ptr->mat[0].d3dm.Specular.a = 0;//1.0;
	
	m_M3d_Ball_ptr->mat[0].d3dm.Ambient.r = 0.0;
	m_M3d_Ball_ptr->mat[0].d3dm.Ambient.g = 0.0;
	m_M3d_Ball_ptr->mat[0].d3dm.Ambient.b = 0.0;
	m_M3d_Ball_ptr->mat[0].d3dm.Ambient.a = 0.0;


	m_M3d_Ball_ptr->mat[0].d3dm.Emissive.r = 0;
	m_M3d_Ball_ptr->mat[0].d3dm.Emissive.g = 0;
	m_M3d_Ball_ptr->mat[0].d3dm.Emissive.b = 0;
	m_M3d_Ball_ptr->mat[0].d3dm.Emissive.a = 1.0;

	m_M3d_Ball_ptr->mat[0].d3dm.Power = D3D_MATERIAL_POWER;//70;


	// Set position //
	//pos[0] = m_ball[n]->m_P.x; // (-)
	//pos[1] = m_ball[n]->m_P.y; // (-)
	//pos[2] = m_ball[n]->m_P.z + BALL_DRAWOFFSETZ;
			
	M3d_transformobject(pos, rot, scale); // For still (non keyframes) meshes.
	M3d_objectdraw(m_M3d_Ball_ptr);
#endif
}

void CSnooker::DrawObstacleBall(float x, float y, float z, float radius)
{
/*
	M3d_vtx pos;
	M3d_vtx rot;
	// complex shadows
	M3d_vtx shadow_pos;

	M3d_vtx scale;
	M3d_object *Ball_ptr;
	float r = 1.0f;
	float g = 1.0f;
	float b = 1.0f;

	Ball_ptr = &m_M3d_Ball_ptr[0]; // point to 1st ball mesh (i.e. cue-ball)
	
	// draw ball GFX
	//scale[0] = radius;
	//scale[1] = radius;
	//scale[2] = radius;

	float f = 28.4625f;
	scale[0] = (BALL_RADIUS/f)*0.0035f*radius;
	scale[1] = (BALL_RADIUS/f)*0.0035f*radius;
	scale[2] = (BALL_RADIUS/f)*0.0035f*radius;

	rot[0] = 0*DEG2RAD;
	rot[1] = 0;
	rot[2] = 0;

	// Set up material for ball //
	//if(g_build.gameType == GAMETYPE_SNOOKER)
	{
	Ball_ptr->mat[0].d3dm.Diffuse.r = r;
	Ball_ptr->mat[0].d3dm.Diffuse.g = g;
	Ball_ptr->mat[0].d3dm.Diffuse.b = b;
	}// else { // Texture
	//Ball_ptr->mat[0].d3dm.Diffuse.r = 1;
	//Ball_ptr->mat[0].d3dm.Diffuse.g = 1;
	//Ball_ptr->mat[0].d3dm.Diffuse.b = 1;
	//}


	Ball_ptr->mat[0].d3dm.Diffuse.a = 1;	

	Ball_ptr->mat[0].d3dm.Specular.r = 0.9;
	Ball_ptr->mat[0].d3dm.Specular.g = 0.9;
	Ball_ptr->mat[0].d3dm.Specular.b = 0.9;
	Ball_ptr->mat[0].d3dm.Specular.a = 1.0;
	
	Ball_ptr->mat[0].d3dm.Ambient.r = 0;//0.05;
	Ball_ptr->mat[0].d3dm.Ambient.g = 0;//0.05;
	Ball_ptr->mat[0].d3dm.Ambient.b = 0;//0.05;
	Ball_ptr->mat[0].d3dm.Ambient.a = 1.0;

	Ball_ptr->mat[0].d3dm.Emissive.r = 0;
	Ball_ptr->mat[0].d3dm.Emissive.g = 0;
	Ball_ptr->mat[0].d3dm.Emissive.b = 0;
	Ball_ptr->mat[0].d3dm.Emissive.a = 1.0;

	Ball_ptr->mat[0].d3dm.Power = D3D_MATERIAL_POWER;//70;

	// Set position //
//#ifdef FIXEDMODE
//	pos[0] = FROMFIXSMALL(m_ball[n]->f_P.x); // (-)
//	pos[1] = FROMFIXSMALL(m_ball[n]->f_P.y); // (-)
//	pos[2] = FROMFIXSMALL(m_ball[n]->f_P.z) + BALL_DRAWOFFSETZ;
//#else
	pos[0] = x;//m_ball[n]->m_P.x; // (-)
	pos[1] = y;//m_ball[n]->m_P.y; // (-)
	pos[2] = z;//m_ball[n]->m_P.z + BALL_DRAWOFFSETZ;
//#endif
	
	M3d_transformobject(pos, rot, scale); // For still (non keyframes) meshes.


	//lpd3dd->MultiplyTransform(D3DTRANSFORMSTATE_WORLD,&m_ball[0]->m_d3dmtx);
	
	
	// cue-ball
	{
		int n = 0;
		M3d_object_ptr object = Ball_ptr;
		M3d_material(&object->mat[n]);		// Set Material.
		 
		if(object->text_ref[n]>32767) M3d_texture_kill();
		else 
		{
			if(object->text_type[object->text_ref[n]]&M3D_THEAD_MAP_DIFFUSE)
			{
				// Set the Texture //
				g_pDevice->SetTexture(0, &m_spriteCueBallTexture[0]);
				//M3d_texture(&m_spriteCueBallTexture[0], 0);	// Set Texture.
			}
			else
			{
				M3d_texture_kill();
			}			
		}
		/// VECTOR/MATRIX ///

		/// MESH/RENDERER ///
		M3d_drawmesh(&object->mesh[n]);		// Draw Mesh.
	}
	//else // other balls
	//{
	//	M3d_objectdraw(Ball_ptr); // materials dictate appearence in snooker
	//}
*/
}

void CSnooker::Draw3D(void) // Generic 3D Drawing
{
	//Draw3DDebug();
	m_customTable->Draw3D();
}

extern void M3d_objectdrawspecial(M3d_object_ptr object);
extern void M3d_drawmeshspecial(M3d_mesh_ptr mesh);

void CSnooker::Draw3DDebug(void) // Generic 3D Drawing
{
	M3d_vtx pos,rot,scale;

	scale[0] = 0.1f;
	scale[1] = 0.1f;
	scale[2] = 0.1f;
	rot[0] = 0;
	rot[1] = 0;
	rot[2] = 0;

	// Set up material for ball //
	m_M3d_Ball_ptr->mat[0].d3dm.Diffuse.r = 1;
	m_M3d_Ball_ptr->mat[0].d3dm.Diffuse.g = 1;
	m_M3d_Ball_ptr->mat[0].d3dm.Diffuse.b = 1;

	m_M3d_Ball_ptr->mat[0].d3dm.Diffuse.a = 1;	

	m_M3d_Ball_ptr->mat[0].d3dm.Specular.r = 0.9;
	m_M3d_Ball_ptr->mat[0].d3dm.Specular.g = 0.9;
	m_M3d_Ball_ptr->mat[0].d3dm.Specular.b = 0.9;
	m_M3d_Ball_ptr->mat[0].d3dm.Specular.a = 1.0;
	
	m_M3d_Ball_ptr->mat[0].d3dm.Ambient.r = 0.0;
	m_M3d_Ball_ptr->mat[0].d3dm.Ambient.g = 0.0;
	m_M3d_Ball_ptr->mat[0].d3dm.Ambient.b = 0.0;
	m_M3d_Ball_ptr->mat[0].d3dm.Ambient.a = 0.0;


	m_M3d_Ball_ptr->mat[0].d3dm.Emissive.r = 0;
	m_M3d_Ball_ptr->mat[0].d3dm.Emissive.g = 0;
	m_M3d_Ball_ptr->mat[0].d3dm.Emissive.b = 0;
	m_M3d_Ball_ptr->mat[0].d3dm.Emissive.a = 1.0;

	m_M3d_Ball_ptr->mat[0].d3dm.Power = D3D_MATERIAL_POWER;//70;

/* 
 	// Set position //
	for(int lightIdx=0;lightIdx<3;lightIdx++)
		{
		pos[0] = ((CGameAppView *)m_parent)->m_environment->m_lightPos[lightIdx][0];
		pos[1] = ((CGameAppView *)m_parent)->m_environment->m_lightPos[lightIdx][1];
		pos[2] = ((CGameAppView *)m_parent)->m_environment->m_lightPos[lightIdx][2];

		
		M3d_transformobject(pos, rot, scale); // For still (non keyframes) meshes.
		M3d_objectdraw(m_M3d_Ball_ptr);
		}
*/
	
	for(int n=0;n<m_customTable->m_pocket_n;n++)
	{
		CVector Pocket;
		Pocket = m_customTable->m_pocketPos[n];
		scale[0] = 0.1f;
		scale[1] = 0.1f;
		scale[2] = 0.1f;

		if((g_cyc&1))
		{
		scale[0]*=m_customTable->m_pocketProp[n].m_radius;
		scale[1]*=m_customTable->m_pocketProp[n].m_radius;
		scale[2]*=m_customTable->m_pocketProp[n].m_radius;
		}
		else {
		scale[0]*=m_customTable->m_pocketProp[n].m_suctionRadius;
		scale[1]*=m_customTable->m_pocketProp[n].m_suctionRadius;
		scale[2]*=m_customTable->m_pocketProp[n].m_suctionRadius;
		}

		scale[0]/=26.09;
		scale[1]/=26.09;
		scale[2]/=26.09;

		pos[0] = Pocket.x;
		pos[1] = Pocket.y;
		pos[2] = Pocket.z;
		
		M3d_transformobject(pos, rot, scale); // For still (non keyframes) meshes.
		M3d_objectdraw(m_M3d_Ball_ptr);	
	}

}

void CSnooker::Init3DCueMaterials(void)
{	
	int n,i;
	for(i=0;i<MAX_CUES;i++)
		{
		m_M3d_Cue_ptr = &m_M3d_Cue[i];
		for(n=0;n<m_M3d_Cue_ptr->mesh_n;n++)
			{
			//m_M3d_Cue_ptr->mat[n].d3dm.Diffuse.r *= 0.5;
			//m_M3d_Cue_ptr->mat[n].d3dm.Diffuse.g *= 0.5;
			//m_M3d_Cue_ptr->mat[n].d3dm.Diffuse.b *= 0.5;
			m_M3d_Cue_ptr->mat[n].d3dm.Diffuse.a = 1;

			//m_M3d_Cue_ptr->mat[n].d3dm.Specular.r = 0.9;
			//m_M3d_Cue_ptr->mat[n].d3dm.Specular.g = 0.9;
			//m_M3d_Cue_ptr->mat[n].d3dm.Specular.b = 0.9;
			m_M3d_Cue_ptr->mat[n].d3dm.Specular.a = 1.0;
			
			//m_M3d_Cue_ptr->mat[n].d3dm.Ambient.r *= 0.5;
			//m_M3d_Cue_ptr->mat[n].d3dm.Ambient.g *= 0.5;
			//m_M3d_Cue_ptr->mat[n].d3dm.Ambient.b *= 0.5;
			m_M3d_Cue_ptr->mat[n].d3dm.Ambient.a = 1.0;

			//m_M3d_Cue_ptr->mat[n].d3dm.Emissive.r *= 0.85;
			//m_M3d_Cue_ptr->mat[n].d3dm.Emissive.g *= 0.85;
			//m_M3d_Cue_ptr->mat[n].d3dm.Emissive.b *= 0.85;  
			m_M3d_Cue_ptr->mat[n].d3dm.Emissive.a = 1.0;

			m_M3d_Cue_ptr->mat[n].d3dm.Power = 30;//D3D_MATERIAL_POWER;//10;


			m_M3d_Cue_ptr->mat[n].d3dm.Diffuse.r = m_M3d_Cue_ptr->mat[n].d3dm.Emissive.r;
			m_M3d_Cue_ptr->mat[n].d3dm.Diffuse.g = m_M3d_Cue_ptr->mat[n].d3dm.Emissive.g;
			m_M3d_Cue_ptr->mat[n].d3dm.Diffuse.b = m_M3d_Cue_ptr->mat[n].d3dm.Emissive.b;
			
			

			}
		}
}


BOOL lastturn;

BOOL CSnooker::PROCESS_CAMERA_TV(char mode, BOOL force)
	{
	float dir[3];
	float mag;	

	static char first_time = TRUE;
	if(first_time)
		{
		lastturn = m_bTurn1P;
		first_time = FALSE;
		}

	char view = m_3dtoggle;

	// If TV Mode Auto, then we need the Camera to Pick it's Own Angles //
	// 1. If it's myturn, then force the view to 3 (CENTRE VIEW) //
	//

	if(GAMEAPPVIEW->m_GFXMode >= 2)
		{
		/*
		if( MyTurn() && GAMEAPPVIEW->m_GFXMode == 3)	// If we're in Manual Mode, then force the Camera to the fixed view when our Shot, otherwise do Auto Mode 
			{
			view = m_3dtoggle;
			}
		*/
	/*
		BOOL proceed = FALSE;
		if(GAMEAPPVIEW->m_GFXMode == 3)	
			{
			// If we're in Fixed Mode, then make the camera only change once per turn //
			if(lastturn != m_bTurn1P) proceed = TRUE;
			}
		else proceed = TRUE;
	*/
		BOOL proceed = TRUE;

		lastturn = m_bTurn1P;

		if(proceed && force == FALSE)
		{
			// On Opponent's Turn, let the Camerea behave like it does on TV //

			// Find the Direction that our Object ball is going to Travel in and work out the Sector (1-4) //

			if(g_ACamAngle < 45*DEG2RAD) view = 1;
			else if(g_ACamAngle < 90*DEG2RAD) view = 1;
			else if(g_ACamAngle < 135*DEG2RAD) view = 5;
			else if(g_ACamAngle < 180*DEG2RAD) view = 5;
			else if(g_ACamAngle < 225*DEG2RAD) view = 4;
			else if(g_ACamAngle < 270*DEG2RAD) view = 4;
			else if(g_ACamAngle < 315*DEG2RAD) view = 2;
			else if(g_ACamAngle <= 360*DEG2RAD) view = 2;

#ifndef THE_POOL_CLUB
			// For iSnooker Only //
			// If we're in the black ball half of table, then Camera should be at Side of Table (where Camera's are on TV)
			// Only do Close range vies if Mode is Default - 0

			if(mode == 0)
			{
			if(m_ball[0]->m_P.x > WORLD_X(591 + 100)) 
				{
				if(m_ball[0]->m_P.x > WORLD_X(340))	
					{
					// White is lower than Pink Spot (Ronnie Style Break Building Zone)
					if(g_ACamAngle >= 80*DEG2RAD && g_ACamAngle <= 190*DEG2RAD) view = 8;
					else if(g_ACamAngle >= 170*DEG2RAD && g_ACamAngle <= 280*DEG2RAD) view = 6;
					}
				else{
					// White is between Pink and Blue spot, more Camera Views
					if(g_ACamAngle >= 80*DEG2RAD && g_ACamAngle <= 190*DEG2RAD) view = 7;
					else if(g_ACamAngle >= 170*DEG2RAD && g_ACamAngle <= 280*DEG2RAD) view = 9;
					}
				}
			}
#endif		
			

		}

		}
	// If we haven't changed view (and in Ball.cpp code), return FALSE //
	
	if(mode == 1)
	if(m_3dtoggle_old == view) return FALSE;
	

	// If we game is not started, then set the mode to Default 3 //
	int tblIdx = g_gameAppView->m_lobby->GetTableIdxFromPlayer(g_gameAppView->m_playerSockIdx);
	if(tblIdx != -1)
	if(!m_table[tblIdx].doubles)
		{
		if(m_table[tblIdx].P1StartAtTable == -1 || m_table[tblIdx].P2StartAtTable == -1) view = 3;
		}
	else{
		if(m_table[tblIdx].P1StartAtTable == -1 || m_table[tblIdx].P2StartAtTable == -1 || m_table[tblIdx].P1bStartAtTable == -1 || m_table[tblIdx].P2bStartAtTable == -1) view = 3;
		}

	m_3dtoggle_old = view;		// Use this Variable to store our view in TV Auto
	m_3dtoggle = view;

	//if((rand()%100) <= 30)
	//if(1)
	{
	//switch(rand()%9)

	float zoom = 0.75f; 
#ifdef THE_POOL_CLUB
	zoom = 0.55;
#endif

	switch(view)
		{
		case 0:	// View 1 //
				g_gameAppView->m_TVcameraX_pos[1] = 4000*zoom;
				g_gameAppView->m_TVcameraY_pos[1] = 3000*zoom;
				g_gameAppView->m_TVcameraZ_pos[1] = 1900*zoom;
				g_gameAppView->m_TVcameraX_at[1] = 500*zoom;
				g_gameAppView->m_TVcameraY_at[1] = 0;
				g_gameAppView->m_TVcameraZ_at[1] = 0;
				g_gameAppView->m_TVClipType = 0;
				break;
		
		case 1:	// View 2 //
				g_gameAppView->m_TVcameraX_pos[1] = 5000*zoom;
				g_gameAppView->m_TVcameraY_pos[1] = 2500*zoom;
				g_gameAppView->m_TVcameraZ_pos[1] = 1900*zoom;
				g_gameAppView->m_TVcameraX_at[1] = 500*zoom;
				g_gameAppView->m_TVcameraY_at[1] = 0;
				g_gameAppView->m_TVcameraZ_at[1] = 0;
				g_gameAppView->m_TVClipType = 0;
				break;
	
		case 2:	// View 3 //
				g_gameAppView->m_TVcameraX_pos[1] = 5000*zoom;
				g_gameAppView->m_TVcameraY_pos[1] = -2500*zoom;
				g_gameAppView->m_TVcameraZ_pos[1] = 2500*zoom;
				g_gameAppView->m_TVcameraX_at[1] = 500*zoom;
				g_gameAppView->m_TVcameraY_at[1] = 0;
				g_gameAppView->m_TVcameraZ_at[1] = 0;
				g_gameAppView->m_TVClipType = 0;
				break;

		case 3:	// View 4 //
				g_gameAppView->m_TVcameraX_pos[1] = 5000*zoom;
				g_gameAppView->m_TVcameraY_pos[1] = 0;
				g_gameAppView->m_TVcameraZ_pos[1] = 1600*zoom;
				g_gameAppView->m_TVcameraX_at[1] = 500*zoom;
				g_gameAppView->m_TVcameraY_at[1] = 0;
				g_gameAppView->m_TVcameraZ_at[1] = 0;
				g_gameAppView->m_TVClipType = 0;
				break;
		
		case 4:	// View 1 //
				g_gameAppView->m_TVcameraX_pos[1] = -5000*zoom;
				g_gameAppView->m_TVcameraY_pos[1] = -2800*zoom;
				g_gameAppView->m_TVcameraZ_pos[1] = 2200*zoom;
				g_gameAppView->m_TVcameraX_at[1] = -500*zoom;
				g_gameAppView->m_TVcameraY_at[1] = 0;
				g_gameAppView->m_TVcameraZ_at[1] = 0;
				g_gameAppView->m_TVClipType = 0;
				break;
		
		case 5:	// View 2 //
				g_gameAppView->m_TVcameraX_pos[1] = -5000*zoom;
				g_gameAppView->m_TVcameraY_pos[1] = 2800*zoom;
				g_gameAppView->m_TVcameraZ_pos[1] = 2200*zoom;
				g_gameAppView->m_TVcameraX_at[1] = -500*zoom;
				g_gameAppView->m_TVcameraY_at[1] = 0;
				g_gameAppView->m_TVcameraZ_at[1] = 0;
				g_gameAppView->m_TVClipType = 0;
				break;
	
		// Break Building Camera Views, like on TV //
		case 6:	// View 6 //
				g_gameAppView->m_TVcameraX_pos[1] = 2800*zoom;
				g_gameAppView->m_TVcameraY_pos[1] = 4000*zoom;
				g_gameAppView->m_TVcameraZ_pos[1] = 2000*zoom;
				g_gameAppView->m_TVcameraX_at[1] = 800*zoom;
				g_gameAppView->m_TVcameraY_at[1] = 0;
				g_gameAppView->m_TVcameraZ_at[1] = 0;
				g_gameAppView->m_TVClipType = 0;
				break;

		case 7:	// View 7 //
				g_gameAppView->m_TVcameraX_pos[1] = 3000*zoom;
				g_gameAppView->m_TVcameraY_pos[1] = 3000*zoom;
				g_gameAppView->m_TVcameraZ_pos[1] = 2000*zoom;
				g_gameAppView->m_TVcameraX_at[1] = 1000*zoom;
				g_gameAppView->m_TVcameraY_at[1] = 0;
				g_gameAppView->m_TVcameraZ_at[1] = 0;
				g_gameAppView->m_TVClipType = 0;
				break;
		
		case 8:	// View 8 //
				g_gameAppView->m_TVcameraX_pos[1] = 2800*zoom;
				g_gameAppView->m_TVcameraY_pos[1] = -4000*zoom;
				g_gameAppView->m_TVcameraZ_pos[1] = 2000*zoom;
				g_gameAppView->m_TVcameraX_at[1] = 800*zoom;
				g_gameAppView->m_TVcameraY_at[1] = 0;
				g_gameAppView->m_TVcameraZ_at[1] = 0;
				g_gameAppView->m_TVClipType = 0;
				break;

		case 9:	// View 9 //
				g_gameAppView->m_TVcameraX_pos[1] = 3000*zoom;
				g_gameAppView->m_TVcameraY_pos[1] = -3000*zoom;
				g_gameAppView->m_TVcameraZ_pos[1] = 2000*zoom;
				g_gameAppView->m_TVcameraX_at[1] = 1000*zoom;
				g_gameAppView->m_TVcameraY_at[1] = 0;
				g_gameAppView->m_TVcameraZ_at[1] = 0;
				g_gameAppView->m_TVClipType = 0;
				break;
		
		case 10: // Player View //
			
				g_gameAppView->m_TVcameraX_at[1] = 0;//m_ball[0]->m_P.x;
				g_gameAppView->m_TVcameraY_at[1] = 0;//m_ball[0]->m_P.y;
				g_gameAppView->m_TVcameraZ_at[1] = 0;//m_ball[0]->m_P.z;

				dir[0] = m_ball[0]->m_V.x;
				dir[1] = m_ball[0]->m_V.y;
				dir[2] = m_ball[0]->m_V.z;
				
				mag = sqrt(dir[0]*dir[0] + dir[1]*dir[1]);
				if(mag<0.1) mag = 0.1;
				dir[0] /= mag;
				dir[1] /= mag;
				dir[2] /= mag;

				g_gameAppView->m_TVcameraX_pos[1] = 0 - (dir[0])*5200*zoom;
				g_gameAppView->m_TVcameraY_pos[1] = 0 - (dir[1])*4900*zoom;
				g_gameAppView->m_TVcameraZ_pos[1] = 1900*zoom;
				
				g_gameAppView->m_TVClipType = 1;
				break;
		case 11: // Birds Eye View //
			
				g_gameAppView->m_TVcameraX_at[1] = 0;//m_ball[0]->m_P.x;
				g_gameAppView->m_TVcameraY_at[1] = 0;//m_ball[0]->m_P.y;
				g_gameAppView->m_TVcameraZ_at[1] = 0;//m_ball[0]->m_P.z;

				dir[0] = m_ball[0]->m_V.x;
				dir[1] = m_ball[0]->m_V.y;
				dir[2] = m_ball[0]->m_V.z;
				
				mag = sqrt(dir[0]*dir[0] + dir[1]*dir[1]);
				if(mag<0.1) mag = 0.1;
				dir[0] /= mag;
				dir[1] /= mag;
				dir[2] /= mag;

				g_gameAppView->m_TVcameraX_pos[1] = 0 - (dir[0])*7000*zoom;
				g_gameAppView->m_TVcameraY_pos[1] = 0 - (dir[1])*5500*zoom;
				g_gameAppView->m_TVcameraZ_pos[1] = 5000*zoom;
				
				g_gameAppView->m_TVClipType = 2;
				break;
				
		}
	


	GAMEAPPVIEW->m_TVcameraX_pos[0] = GAMEAPPVIEW->m_TVcameraX_pos[1];
	GAMEAPPVIEW->m_TVcameraY_pos[0] = GAMEAPPVIEW->m_TVcameraY_pos[1];
	GAMEAPPVIEW->m_TVcameraZ_pos[0] = GAMEAPPVIEW->m_TVcameraZ_pos[1];
	GAMEAPPVIEW->m_TVcameraX_at[0] = GAMEAPPVIEW->m_TVcameraX_at[1];
	GAMEAPPVIEW->m_TVcameraY_at[0] = GAMEAPPVIEW->m_TVcameraY_at[1];
	GAMEAPPVIEW->m_TVcameraZ_at[0] = GAMEAPPVIEW->m_TVcameraZ_at[1];
	g_gameAppView->m_TVcameraChange = 0;
	}

	return TRUE;
}


void CSnooker::PROCESS_CAMERA_POSITION(void)
{
	
	if(GAMEAPPVIEW->m_TVcameraChange >0) (GAMEAPPVIEW->m_TVcameraChange) -= 1;
	
	
	if(GAMEAPPVIEW->m_TVcameraChange == 0)
		{
		GAMEAPPVIEW->m_TVcameraX_pos[0] = GAMEAPPVIEW->m_TVcameraX_pos[1];
		GAMEAPPVIEW->m_TVcameraY_pos[0] = GAMEAPPVIEW->m_TVcameraY_pos[1];
		GAMEAPPVIEW->m_TVcameraZ_pos[0] = GAMEAPPVIEW->m_TVcameraZ_pos[1];
		GAMEAPPVIEW->m_TVcameraX_at[0] = GAMEAPPVIEW->m_TVcameraX_at[1];
		GAMEAPPVIEW->m_TVcameraY_at[0] = GAMEAPPVIEW->m_TVcameraY_at[1];
		GAMEAPPVIEW->m_TVcameraZ_at[0] = GAMEAPPVIEW->m_TVcameraZ_at[1];
		}
	
}



// This Processes Mouse Move only for MainView Window //

void CSnooker::TableViewMouseMove(CPoint point)
{

#ifndef MAIN_SERVER

	float x, y;
	float w, h;
	float k;
	M3d_vtx pos;
	M3d_vtx vec;
	float d1, d2, dx, dy;

	D3DVIEWPORT9 mainViewPort;
	d3d::BoundingSphere BSphere;
	D3DXMATRIX view;
	D3DXMATRIX viewInverse;
	d3d::Ray ray;


	m_bMouseMoved = 1;
	m_tableMouseOn = 0; // default is not pointing at the table.
	m_ballMouseOn = 0; // default is not pointing at the table.


	if (g_gameAppView)
	{
		CRect mainrect;
		CRect clientrect;
		g_gameAppView->GetWindowRect(&mainrect);
		::GetWindowRect(g_hwndDX, clientrect);

		int diff = clientrect.left - mainrect.left;
		if (diff > 2)
		{
			point.x -= (diff - 2);
		}
	}


	if (!MyTurn()) m_ballMouseOn = 1; // default is not pointing at the table.

	// mouse cue movement
	if (m_cueBallSelect == 1)
	{
		if (MyTurn())
		{

			// scale mouse relative coords (by squaring values)
			float x, y, tmp;
			float screenWidthFactor = m_cx;
			screenWidthFactor /= 800.0f;
			x = Mio_mouse.lX;
			tmp = x*x;
			if (tmp > 2000) tmp = 2000;
			if (x<0) x = -tmp;
			else x = tmp;
			// -------------- //
			y = Mio_mouse.lY;
			tmp = y*y;
			if (tmp > 2000) tmp = 2000;
			if (y<0) y = -tmp;
			else y = tmp;
			// -------------- //	
			m_mouseX += ((float)x * m_mouseCueSensitivity * screenWidthFactor);
			m_mouseY += ((float)y * m_mouseCueSensitivity * screenWidthFactor);

			// cue-power 0-100% boundary circle
			float rx = m_mouseX - m_sCPoint.x;
			float ry = m_mouseY - m_sCPoint.y;

			float r = rx*rx + ry*ry;
			float rTest = screenWidthFactor * 375.0f;
			if (r > rTest*rTest)
			{
				// if outside circle - then move back to boundary edge of circle
				CVector R(rx, ry, 0);
				R = R.Unit();
				R = R * rTest;
				m_mouseX = m_sCPoint.x + R.x;
				m_mouseY = m_sCPoint.y + R.y;
			}
		}
	}


	// mouse camera mode orient/offset
	else if (m_CameraMode == CAMERA_MODE_TV && ((CGameAppView *)m_parent)->m_bRButtonDown)
	{

		CGameAppView *p = (CGameAppView *)m_parent;

		// use mouse X-axis for camera orientation
		p->m_cameraAngle += (float)(Mio_mouse.lX)*MOUSE_CAMERA_SENSITIVY_X;

		// use mouse Y-axis for camera magnitude (offset)
		p->m_cameraMag += (float)(Mio_mouse.lY) *2.0f * MOUSE_CAMERA_SENSITIVY_Y;
		float ang = ((CGameAppView *)m_parent)->m_cameraAngle*DEG2RAD;

	}

	// mouse default movement
	else
	{
		m_mouseX = point.x;
		m_mouseY = point.y;
	}


	// if strike is in progress - don't update mouse data
	if (m_cueBallSelect == 10 ||
		m_cueBallSelect == 9 ||
		m_cueBallSelect == 2 ||
		m_cueBallSelect >= 20) return;

	//if(m_3dtoggle == 1) return;


	x = m_mouseX;
	y = m_mouseY;
	w = m_windowWidth;
	h = m_windowHeight;


	if (m_cueBallSelect == 0) // just browsing, so we do bounds
		//--	if(m_cueBallSelect == 0 || m_cueBallSelect == 5) // just browsing, so we do bounds
	{
		if (x >= w) goto eyecam;
		if (x<0) goto eyecam;
		if (y >= h) goto eyecam;
		if (y<0) goto eyecam;

		// check if player is in idle mouse movement (special case - when player holds down multiple L & R buttons)
		if (!((CGameAppView *)m_parent)->m_bLButtonDown &&
			!((CGameAppView *)m_parent)->m_bRButtonDown)
			((CGameAppView *)m_parent)->m_bRButtonJustUp = 1;

		// check if player has stood up from shot - and has a ball-in-hand!!
		if (!((CGameAppView *)m_parent)->m_bPracticeEditPlay)//m_snooker->m_bPracticeMode) // Edit mode
		{
			if (((CGameAppView *)m_parent)->m_bRButtonDown)
			if (!((CGameAppView *)m_parent)->m_bLButtonDown)
			if (((CGameAppView *)m_parent)->m_bRButtonJustUp)
			{
				// find the ball we want to grab
				for (int n = 0; n<MAX_TOTALBALLS; n++)
				{
					if (m_ball[n]->Proximity(m_tableMousePos[0], m_tableMousePos[1], m_tableMousePos[2]) == TRUE)
					{
#ifdef FIXEDMODE
						m_sPoint[0] = FROMFIXSMALL(m_ball[n]->f_P.x);
						m_sPoint[1] = FROMFIXSMALL(m_ball[n]->f_P.y);
						m_sPoint[2] = FROMFIXSMALL(m_ball[n]->f_P.z);
#else
						m_sPoint[0] = m_ball[n]->m_P.x;
						m_sPoint[1] = m_ball[n]->m_P.y;
						m_sPoint[2] = m_ball[n]->m_P.z;
#endif
						m_grabBall = n;
						break;
					}
				}
				DropBallOnTable(m_grabBall, m_tableMousePos[0], m_tableMousePos[1]);
			}
		}
		else // normal game mode (Play mode)
		{
			// check if in practice mode & have ball-in-hand ticked!
			if (m_bPracticeMode)
			if (g_practiceDetails.bOptionsBallInHandAlways)
				m_bBallInHand = 1;
			// ----------------- //

			if (m_bStandUpFromShot)
			if (m_bBallInHand)
			if (((CGameAppView *)m_parent)->m_bRButtonDown)
			if (!((CGameAppView *)m_parent)->m_bLButtonDown)
			if (((CGameAppView *)m_parent)->m_bRButtonJustUp)
			{
				int cueBallIdx = 0;

				if (g_build.gameType == GAMETYPE_POOL)
				if (m_customTable->m_type == CTTYPE_BOWLS)
					cueBallIdx = m_customTable->RefereeBowlsNextStrikeBall();

				// If we're playing Billiards, then cueBallIdx holds 2 possible values, depending on the turn //
				if (g_build.gameType == GAMETYPE_SNOOKER && m_customTable->m_type == CTTYPE_BILLIARDS)
				if (!m_bTurn1P) cueBallIdx = 1;

				DropBallOnTable(cueBallIdx, m_tableMousePos[0], m_tableMousePos[1]);
				/*CVector P;
				P.x = m_tableMousePos[0];
				P.y = m_tableMousePos[1];
				P.z = m_ball[0]->m_P.z;
				((CGameAppView *)m_parent)->SendPlayerBallPos(P, WHITE);*/
			}
		}
	}

	if (!g_pCamera) return; // can't calculate 3d position due to no camera being on.



	//
	// Find Ray from Camera Orgin to Mouse Pointer
	//

	// compute the ray in view space given the clicked screen point




	ray = CalcPickingRay(x, y);
	// transform the ray to world space
	g_pDevice->GetTransform(D3DTS_VIEW, &view);
	D3DXMatrixInverse(&viewInverse, 0, &view);
	TransformRay(&ray, &viewInverse);


	// If we reached this point of code, then our mouse must be on the table
	m_tableMouseOn = 1; // pointing at the table

	// Let see what Ball (if Any we have our Mouse Pointer on

	if (m_cueBallSelect != 5) //--
	{
		// Find out where our mouse is on the table //
		// ray._origin.z + ray._direction.z * k = 0
		// k = -ray.orgin.z / ray._direction.z
		k = -ray._origin.z / ray._direction.z;

		m_tableMousePos[0] = ray._origin.x + ray._direction.x * k;
		m_tableMousePos[1] = ray._origin.y + ray._direction.y * k;
		m_tableMousePos[2] = BALL_POSITIONZOFFSET + BALL_DRAWOFFSETZ;
		/*
		char buf[256];
		sprintf(buf, "m_tableMousePos[1]: %f, ray._origin.y: %f, ray._direction.y: %f, k: %f", m_tableMousePos[1], ray._origin.y, ray._direction.y, k);
		ErrorMessage(m_parent, buf);
		*/

		m_tableMousePosTmp[0] = m_tableMousePos[0];
		m_tableMousePosTmp[1] = m_tableMousePos[1];
		m_tableMousePosTmp[2] = m_tableMousePos[2];


		// Check if our Mouse is on the Table or Minicam //
		g_pDevice->GetViewport(&mainViewPort);
		if (y > mainViewPort.Height)
		{
			int p;
			p = 10;
			goto eyecam;
		}



		// check mouse over a ball
		for (int n = 0; n<MAX_TOTALBALLS; n++)
		{

			BSphere._center = D3DXVECTOR3(m_ball[n]->m_P.x, m_ball[n]->m_P.y, m_ball[n]->m_P.z);
			BSphere._radius = BALL_RADIUS;


			float dx, dy;
			dx = m_ball[n]->m_P.x - m_tableMousePos[0];
			dy = m_ball[n]->m_P.y - m_tableMousePos[1];
			if (sqrt(dx*dx + dy*dy) <= BALL_RADIUS)
				//if( RaySphereIntTest(&ray, &BSphere) )
			{
				//::MessageBox(0, "Hit!", "HIT", 0);

				if (!m_ball[n]->m_bMouseOver) // Mouse on
				{
					//m_ball[n]->MouseOn();
					g_ballmouseoncyc = g_cyc;
					// subtract this shit (g_ballmouseoncyc) from g_cyc at the alpha bit
					// i.e. sin((float)g_cya-g_ballmouseoncyc)
				}
				m_ball[n]->m_bMouseOver = TRUE;
			}
			else
			{
				m_ball[n]->m_bMouseOver = FALSE;
			}
		}
		////////////////////////////////////////////////////////
	}


	m_tableMouseOn = 1; // pointing at the table



	/*
	// Project this vector from m_cameraO till it hits the table's surface Z = 0, that plane.
	// (m_cameraO + vec*k) scalar* (0,0,1) = 0

	// hence: m_cameraO[2] + vec[2] * k = 0;

	if(vec[2]>-0.001f) goto eyecam;//return; // pointing away from table

	k = -(g_pCamera->_pos[2])/vec[2]; // k should be a +ve value to indicate hitting the table.

	if(k<0) goto eyecam; //return; // failed to intercept on the +ve direction

	m_tableMouseOn = 1; // pointing at the table

	if(m_cueBallSelect != 5) //--
	{

	m_tableMousePos[0] = g_pCamera->_pos[0] + vec[0] * k;
	m_tableMousePos[1] = g_pCamera->_pos[1] + vec[1] * k;
	//	m_tableMousePos[2] = BALL_RADIUS*2.8f;//0; //	m_tableMousePos[2] = m_cameraO[2] + vec[2] * k; // should equal zero.
	//m_tableMousePos[2] = BALL_POSITIONZOFFSET;
	m_tableMousePos[2] = BALL_POSITIONZOFFSET + BALL_DRAWOFFSETZ;

	///
	m_tableMousePosTmp[0] = g_pCamera->_pos[0] + vec[0] * k;
	m_tableMousePosTmp[1] = g_pCamera->_pos[1] + vec[1] * k;
	m_tableMousePosTmp[2] = BALL_POSITIONZOFFSET + BALL_DRAWOFFSETZ;
	///

	////////////////////////////////////////////////////////
	// check mouse over a ball
	for(int n=0;n<MAX_TOTALBALLS;n++)
	{
	if( m_ball[n]->Proximity(m_tableMousePos[0], m_tableMousePos[1], m_tableMousePos[2]) == TRUE)
	{
	if(!m_ball[n]->m_bMouseOver) // Mouse on
	{
	//m_ball[n]->MouseOn();
	g_ballmouseoncyc = g_cyc;
	// subtract this shit (g_ballmouseoncyc) from g_cyc at the alpha bit
	// i.e. sin((float)g_cya-g_ballmouseoncyc)
	}
	m_ball[n]->m_bMouseOver = TRUE;
	} else m_ball[n]->m_bMouseOver = FALSE;
	}
	////////////////////////////////////////////////////////
	}
	else
	{
	///
	m_tableMousePosTmp[0] = g_pCamera->_pos[0] + vec[0] * k;
	m_tableMousePosTmp[1] = g_pCamera->_pos[1] + vec[1] * k;
	m_tableMousePosTmp[2] = BALL_POSITIONZOFFSET + BALL_DRAWOFFSETZ;
	///
	}
	//return;
	*/

eyecam:;



	// strike cue-ball finally!
	if (m_cueBallSelect == 5) //--
	if (((CGameAppView *)m_parent)->m_bLButtonJustUp)
	if (m_bCueTipReady)
	if (!m_bCueTipReadyFirstTime)
	if (!m_bShotTimeUp) // NEW 29/JAN/04
	{
		//m_bCueTipReady = FALSE;
		x = m_mouseX - m_moffsetX;
		y = m_mouseY - m_moffsetY;
		w = m_mwindowWidth;
		h = m_mwindowHeight;

		// see if tip is inside mini-camera's box
		if (x >= w);
		else if (x<0);
		else if (y >= h);
		else if (y<0);
		else
		{
			m_bCueTipReady = FALSE;
			m_bCueTipReadyFirstTime = TRUE;

			//m_bCueState = FALSE;
			m_ballMouseOn = 1;
			((CGameAppView *)m_parent)->m_sliderRaiseButt.EnableWindow(FALSE);
			((CGameAppView *)m_parent)->PreviousWndEditSetFocus();

			StrikeBallSend();

			// player decided to play the shot! - so loses opportunity to put opponent back in!
			if (m_bFoulDecideTurn)
			{
				m_bFoulDecideTurn = FALSE;

				//CRect origin;
				//((CGameAppView *)m_parent)->GetWindowRect(&origin);
				//m_foulDlg.SetWindowPos(&((CGameAppView *)m_parent)->wndTop, origin.left, origin.top, 32, 32, SWP_NOACTIVATE | SWP_HIDEWINDOW);
				m_foulDlg.SetWindowPos(&((CGameAppView *)m_parent)->wndTop, 0, 0, 32, 32, SWP_NOACTIVATE | SWP_HIDEWINDOW);
				//m_foulDlg.RedrawWindow(NULL, NULL, RDW_INVALIDATE | RDW_UPDATENOW | RDW_ERASE | RDW_FRAME);
			}

			return;
		}
	}


	// align cue tip on cue-ball
	//--    if(m_cueBallSelect == 0)
	if (m_cueBallSelect == 5) //--
		//if(((CGameAppView *)m_parent)->m_bLButtonDown)
	{
		//		if( MyTurn() )
		//			m_CameraMode = CAMERA_MODE_WATCH;

		((CGameAppView *)m_parent)->m_bLButtonJustUp = 0;

		x = m_mouseX - m_moffsetX;
		y = m_mouseY - m_moffsetY;
		w = m_mwindowWidth;
		h = m_mwindowHeight;

		m_bCueTipReady = TRUE;

		// see if tip is inside mini-camera's box
		if (x >= w);
		else if (x<0);
		else if (y >= h);
		else if (y<0);
		else {
			if (!HamperedCueing(x, y, w, h))
			{
				//m_bCueTipReady = TRUE;
				m_bCueTipReadyFirstTime = FALSE; // change state, now that player has moved the mouse into the mini-cam area!

				MoveCueTip(x, y, w, h);
			}
		}
	}
	// ---------------------- //

	//skipcuetipmove:
	pos[0] = m_cuetip[0];
	pos[1] = m_cuetip[1];
	//pos[2] = m_cuetip[2];
	//pos[2] = 1;//m_cuetip[2];
	if (m_bCueTipReady) pos[2] = 1;
	else pos[2] = 0;

	// ------------------------------------------- //

	if (!m_mcameraOn) return; // can't calculate 3d position due to no camera being on.


	pos[0] *= m_mcameraFov;
	pos[1] *= m_mcameraFov;

	pos[0] *= 1 / m_mcameraAspect;




	vec[0] = pos[0] * m_mcameraX[0] + pos[1] * m_mcameraY[0] + pos[2] * m_mcameraZ[0];
	vec[1] = pos[0] * m_mcameraX[1] + pos[1] * m_mcameraY[1] + pos[2] * m_mcameraZ[1];
	vec[2] = pos[0] * m_mcameraX[2] + pos[1] * m_mcameraY[2] + pos[2] * m_mcameraZ[2];

	// Project this vector from m_cameraO till it hits the table's surface Z = 0, that plane.
	// (m_cameraO + vec*k) scalar* (0,0,1) = 0

	// hence: m_cameraO[2] + vec[2] * k = 0;



	if (MyTurn())
	{
		m_ballMouseOn = 1; // pointing at the table

		k = (BALL_RADIUS * 6.5);


		m_ballMousePos[0] = m_mcameraO[0] + vec[0] * k;
		m_ballMousePos[1] = m_mcameraO[1] + vec[1] * k;
		m_ballMousePos[2] = m_mcameraO[2] + vec[2] * k; //	m_tableMousePos[2] = m_cameraO[2] + vec[2] * k; // should equal zero.	


		//if(m_bCueTipReady == FALSE) {
		//	m_ballMousePos[0] = 0;
		//	m_ballMousePos[1] = 0;
		//	m_ballMousePos[2] = 0;
		///}
	}

	return;
nextcam:;

#endif	
}