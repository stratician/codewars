//////////////////////////////////////////////////////////////////////////////////////////////////
// 
// File: camera.cpp
// 
// Author: Frank Luna (C) All Rights Reserved
//
// System: AMD Athlon 1800+ XP, 512 DDR, Geforce 3, Windows XP, MSVC++ 7.0 
//
// Desc: Defines a camera's position and orientation.
//         
//////////////////////////////////////////////////////////////////////////////////////////////////

#include <math.h>
#include "camera.h"

Camera::Camera(bool type = 0)
{
	_pos   = D3DXVECTOR3(4000.0f, 0.0f, 1600.0f);
	_up    = D3DXVECTOR3(0.0f, 0.0f, 1.0f);
	_look  = D3DXVECTOR3(0.0f, 0.0f, 0.0f);


	_distance = D3DXVec3Length(&_pos);



	_rotaxis[0] = 0.0f;
	_rotaxis[1] = 0.0f;

	_rotaxisv[0] = 0.0f;
	_rotaxisv[1] = 0.0f;

	_distancev = 0.0f;

	_type = type;

	_nearclip = 100.0f;
	_farclip = 20000.0f;

	_maxliftangle = D3DX_PI * 0.5f * 0.9f; 
	_minliftangle = D3DX_PI * 0.5f * 0.08f;

}


Camera::~Camera()
{

}

void Camera::OrthoAxis(void)
{
#ifndef MAIN_SERVER
	if(_type == 0)
	{
		// Get Z Unit Vector
		_cameraZ = _look - _pos;
		D3DXVec3Normalize(&_cameraZ, &_cameraZ);

		// Get Right Unit Vector 'Y'
		D3DXVec3Cross(&_cameraX, &_up, &_cameraZ);
		D3DXVec3Normalize(&_cameraX, &_cameraX);

		// Finally Get our new Up Unit Vector 'X' //
		D3DXVec3Cross(&_cameraY, &_cameraZ, &_cameraX);
		D3DXVec3Normalize(&_cameraY, &_cameraY);

		_up = _cameraY;
	}
	else
	{


	// Get Z Unit Vector
	_cameraZ = _look - _pos;
	D3DXVec3Normalize(&_cameraZ, &_cameraZ);

	// Set our Up Vector //
	_up = D3DXVECTOR3(0.0f,0.0f,1.0f);


	// Get Right Unit Vector 'Y'
	D3DXVec3Cross(&_cameraX, &_up, &_cameraZ);
	D3DXVec3Normalize(&_cameraX, &_cameraX);

	}

#endif
}

void Camera::SetDistance(float distance)
{
	// Find the Vector of Camera Pos from Camera At 
	_pos = _look - distance*_cameraZ;

}

void Camera::SetPosition(D3DXVECTOR3 pos)
{
	_pos = pos;
	_distance = D3DXVec3Length(&_pos);
	OrthoAxis();

}


void Camera::RotateZAxis(float angle)
{
#ifndef MAIN_SERVER
	// Rotate around up axis (Z-Axis)

	D3DXMATRIX T;
	D3DXMatrixRotationAxis(&T, &_up, angle);
	D3DXVec3TransformCoord(&_pos,&_pos, &T);
#endif
}

void Camera::RotateHAxis(float angle)
{
#ifndef MAIN_SERVER
	// Rotation around H-Axis //
	D3DXVECTOR3 dir;
	D3DXVECTOR3 rotaxis;

	dir = _look - _pos;
	D3DXVec3Normalize(&dir, &dir);
	D3DXVec3Cross(&rotaxis, &dir, &_up);

	D3DXMATRIX T;
	D3DXMatrixRotationAxis(&T, &rotaxis, angle);
	D3DXVec3TransformCoord(&_pos,&_pos, &T);
#endif
}

void Camera::roll(float angle)
{
#ifndef MAIN_SERVER
	D3DXMATRIX T;

	D3DXMatrixRotationAxis(&T, &_look, angle);

	// rotate _up and _right around _look vector //

	D3DXVec3TransformCoord(&_right,&_right, &T);
	D3DXVec3TransformCoord(&_up,&_up, &T);
#endif

}

void Camera::MouseUpdate(float dx, float dy)
{

	_rotaxisv[0] += dx * 0.001f;
	_rotaxisv[1] -= dy * 0.001f;


}


D3DXVECTOR3 oldpos;

void Camera::Process(void)
{




	
	float distancexy;



	_rotaxisv[0] *= 0.95f;
	_rotaxisv[1] *= 0.95f;

	_distancev *= 0.95f;

	// Before we rotate around our Z Axis, lets set the HAxis rotation back to 90 Degress //
	// Make Sure we are within our Bounds //
	if(_type == 1)
	{


		oldpos = _pos;

		RotateZAxis(_rotaxisv[0]);
		RotateHAxis(_rotaxisv[1]);
	}
	// make sure our up vector is correct //

	if(_type == 1)
	{
		
		OrthoAxis();
		SetDistance(_distance);

		distancexy = sqrt( (_pos[0] * _pos[0]) + (_pos[1] * _pos[1]) );

		// Find the angle of elevation we're at.. Make sure this doesn't exceed 80 Degrees //
		// Elevation = atan(_pos[2] / _distance) //
		float e_ang = atan(_pos[2] / distancexy);
		if( e_ang > _maxliftangle )
		{
			_rotaxisv[1] = 0.0001f;
			_pos = oldpos;
		}
		else
		if( e_ang < _minliftangle )
		{
			_pos = oldpos;
			_rotaxisv[1] = -0.0001f;
		}

		_distance += _distancev;
		if(_distance > 5000.0f) _distance = 5000.0f;//, _distancev = 0.0f;
		if(_distance < 500.0f) _distance = 500.0f;//, _distancev = 0.0f;

		// If we are far away from table, make the near clip large //
		if(_distance > 4000.f) _nearclip = 1400.0f;
		else _nearclip = 100.0f;
	}		


}

