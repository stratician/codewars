//////////////////////////////////////////////////////////////////////////////////////////////////
// 
// File: camera.h
// 
// Author: Frank Luna (C) All Rights Reserved
//
// System: AMD Athlon 1800+ XP, 512 DDR, Geforce 3, Windows XP, MSVC++ 7.0 
//
// Desc: Defines a camera's position and orientation.
//         
//////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef __cameraH__
#define __cameraH__

#include "../CodeSupp/Build.h"
#include "C:\Program Files (x86)\Microsoft DirectX SDK (June 2010)\Include\d3dx9.h"


class Camera
{
public:
	Camera(bool type);
	~Camera();

	
	void RotateZAxis(float angle);		// rotate on right vector
	void RotateHAxis(float angle);		// rotate on up vector
	void OrthoAxis(void);				// Makes all 3 axis Othogonal
	void SetDistance(float distance);				// Moves Camera

	void roll(float angle);

	void SetPosition(D3DXVECTOR3 pos); 
	void MouseUpdate(float dx, float dy);
	void Process(void);


public:
	bool _type;
	D3DXVECTOR3 _up;
	D3DXVECTOR3 _look;
	D3DXVECTOR3 _pos;
	D3DXVECTOR3 _right;


	D3DXVECTOR3 _cameraX;
	D3DXVECTOR3 _cameraY;
	D3DXVECTOR3 _cameraZ;

	float _distance;			// Magnitude of Distance Camera is from Look At point
	float _cameraFov;
	float _cameraAspect;

	float _rotaxis[2];
	float _rotaxisv[2];
	float _distancev;

	float _nearclip;
	float _farclip;

	float _maxliftangle;
	float _minliftangle;
};
#endif // __cameraH__