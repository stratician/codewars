#include "detect.h"
#include <memirlib.h>



d3d::Ray CalcPickingRay(float x, float y)
{
	d3d::Ray ray;

#ifndef MAIN_SERVER
	float px = 0.0f;
	float py = 0.0f;

	D3DVIEWPORT9 vp;
	g_pDevice->GetViewport(&vp);

	D3DXMATRIX proj;
	g_pDevice->GetTransform(D3DTS_PROJECTION, &proj);

	px = ((( 2.0f*x) / vp.Width)  - 1.0f) / proj(0, 0);
	py = (((-2.0f*y) / vp.Height) + 1.0f) / proj(1, 1);

	
	ray._origin    = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	ray._direction = D3DXVECTOR3(px, py, 1.0f);

	return ray;
#endif

	return ray;

	
}


void TransformRay(d3d::Ray* ray, D3DXMATRIX* T)
{
#ifndef MAIN_SERVER
	// transform the ray's origin, w = 1.
	D3DXVec3TransformCoord(
		&ray->_origin,
		&ray->_origin,
		T);

	// transform the ray's direction, w = 0.
	D3DXVec3TransformNormal(
		&ray->_direction,
		&ray->_direction,
		T);

	// normalize the direction
	D3DXVec3Normalize(&ray->_direction, &ray->_direction);
#endif
}

bool RaySphereIntTest(d3d::Ray* ray, d3d::BoundingSphere* sphere)
{
#ifndef MAIN_SERVER
	D3DXVECTOR3 v = ray->_origin - sphere->_center;

	float b = 2.0f * D3DXVec3Dot(&ray->_direction, &v);
	float c = D3DXVec3Dot(&v, &v) - (sphere->_radius * sphere->_radius);

	// find the discriminant
	float discriminant = (b * b) - (4.0f * c);

	// test for imaginary number
	if( discriminant < 0.0f )
		return false;

	discriminant = sqrtf(discriminant);

	float s0 = (-b + discriminant) / 2.0f;
	float s1 = (-b - discriminant) / 2.0f;

	// if a solution is >= 0, then we intersected the sphere
	if( s0 >= 0.0f || s1 >= 0.0f )
		return true;

#endif

	return false;

}


bool SameSide(D3DXVECTOR3 p1, D3DXVECTOR3 p2, D3DXVECTOR3 a, D3DXVECTOR3 b)
{
#ifndef MAIN_SERVER
	D3DXVECTOR3 cp1;
	D3DXVECTOR3 cp2;

	D3DXVECTOR3 ba = b-a;
	D3DXVECTOR3 p1a = p1-a;
	D3DXVECTOR3 p2a = p2-a;

	D3DXVec3Cross(&cp1, &ba, &p1a);
	D3DXVec3Cross(&cp2, &ba, &p2a);
	
	if(D3DXVec3Dot(&cp1, &cp2) >= 0) return true;
		else return false;
#endif

		return false;
}

bool PointInTriangle(D3DXVECTOR3 p, D3DXVECTOR3 a, D3DXVECTOR3 b, D3DXVECTOR3 c)
{
	if(	SameSide(p, a, b, c) &&
		SameSide(p, b, a, c) &&
		SameSide(p, c, a, b) ) return true;
		else return false;
}



bool RayRectangleIntTest(d3d::Ray* ray, D3DXVECTOR3 A, D3DXVECTOR3 B, D3DXVECTOR3 C, D3DXVECTOR3 D, float nearclip )
{
#ifndef MAIN_SERVER
	// Get Normal Vector of Plane //
	D3DXVECTOR3 normal;
	D3DXVECTOR3 AB, AC;
	D3DXVECTOR3 p;


	AB = B - A;
	AC = C - A;
	D3DXVec3Cross(&normal, &AC, &AB);

	// First Check where we intersect plane //
	if( !D3DXVec3Dot(&ray->_direction, &normal) ) 
	{
		return false;
	}

	float t;
	t = (D3DXVec3Dot(&A, &normal) - D3DXVec3Dot(&ray->_origin, &normal) ) / D3DXVec3Dot(&ray->_direction, &normal);
	
	//if(t > 0)
	if(t > nearclip)	// We must make sure we aren't checking anything in front of the near clip, since we can't see this
	{
		D3DXVECTOR3 p = ray->_origin + ray->_direction * t;
		if(PointInTriangle(p, A, B, C) ||  PointInTriangle(p, B, C, D)) return true;
	}

#endif
	return false;
}

