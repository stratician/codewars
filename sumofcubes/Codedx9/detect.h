#ifndef __detectH__
#define __detectH__



#include "../CodeSupp/stdafx.h"
#include "../CodeSupp/GameApp.h"

#include "d3dUtility.h"


void TransformRay(d3d::Ray* ray, D3DXMATRIX* T);
bool RaySphereIntTest(d3d::Ray* ray, d3d::BoundingSphere* sphere);
bool RayRectangleIntTest(d3d::Ray* ray, D3DXVECTOR3 A, D3DXVECTOR3 B, D3DXVECTOR3 C, D3DXVECTOR3 D, float nearclip = 0.0f);
d3d::Ray CalcPickingRay(float x, float y);




#endif //__detectH__