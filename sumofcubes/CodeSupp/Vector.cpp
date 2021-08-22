// Vector.cpp


#include <stdio.h>
#include <windows.h>
#include <math.h>

#include "Vector.h"


void MultiplyMatrices(M3d_mtx &c, M3d_mtx &a, M3d_mtx &b)
{
	int x,y;
	int i;
	M3d_mtx d;

	// A * B -> C
	for(y=0;y<4;y++)
	for(x=0;x<4;x++)
	{
		
	d[y][x] = a[y][0]*b[0][x]+
			  a[y][1]*b[1][x]+
		 	  a[y][2]*b[2][x]+
			  a[y][3]*b[3][x];
	}

	// Copy to C

	for(y=0;y<4;y++)
	for(x=0;x<4;x++)
	{
	c[y][x] = d[y][x];
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////
//
//
void CVector::DisplayBox(void)
{
	char buffer[80];
	sprintf(buffer, "(%f, %f, %f)", x, y, z);
	MessageBox(0, buffer, "CVector", MB_OK);
}


///////////////////////////////////////////////////////////////////////////////////////////////////
// + operator
//
CVector CVector::operator+(CVector &A)
{
	return CVector( x+A.x, y+A.y, z+A.z );
}


///////////////////////////////////////////////////////////////////////////////////////////////////
// - operator
//
CVector CVector::operator-(CVector &A)
{
	return CVector( x-A.x, y-A.y, z-A.z );
}


///////////////////////////////////////////////////////////////////////////////////////////////////
// * operator
//
CVector CVector::operator*(float s)		// i.e. A * 5
{
	return CVector(x*s, y*s, z*s);
}


///////////////////////////////////////////////////////////////////////////////////////////////////
// * operator (friend function)
//
CVector operator*(float s, CVector &A) // i.e. 5 * A
{
	return CVector(s*A.x, s*A.y, s*A.z);
}


///////////////////////////////////////////////////////////////////////////////////////////////////
// / operator
//
CVector CVector::operator/(float s)		// i.e. A / 5
{
	return CVector(x/s, y/s, z/s);
}


///////////////////////////////////////////////////////////////////////////////////////////////////
//
//
CVector CVector::Cross(CVector &P)
{
	float f1,f2,f3,f4,f5,f6;
	float fx,fy,fz;
	// Vector Product (Cross Product)
	//
	// C = T X P
	//
	// | i  j  k  |
	// | tx ty tz | =  i(ty*pz - py*tz) - j(tx*pz - px*tz) + k(tx*py - px*ty) = C
	// | px py pz |
	//
	f1 = y*P.z;
	f2 = P.y*z;
	f3 = x*P.z;
	f4 = P.x*z;
	f5 = x*P.y;
	f6 = P.x*y;

	fx = f1-f2;
	fy = f3-f4;
	fy = -fy;
	fz = f5-f6;

	//return CVector(((y*P.z) - (P.y*z)), -((x*P.z) - (P.x*z)), ((x*P.y) - (P.x*y)));
	//return CVector((f1-f2), -(f3-f4), (f5-f6));
	return CVector(fx, fy, fz);
}


///////////////////////////////////////////////////////////////////////////////////////////////////
//
//
float CVector::Dot(CVector &P)
{
	float f1,f2,f3;
	// Scalar Product (Dot Product)
	//
	// c = T.P
	//
	// (tx*px) + (ty*py) + (tz*pz) = c
	//
	f1 = x*P.x;
	f2 = y*P.y;
	f3 = z*P.z;
	f1 = f1 +f2;
	f1 = f1 + f3;

	return f1;//f1+f2+f3;//((x*P.x) + (y*P.y) + (z*P.z));
}


///////////////////////////////////////////////////////////////////////////////////////////////////
//
//
float CVector::Magnitude(void)
{
	float f1,f2,f3;
	// Vector Magnitude for 'this'
	//
	// m = sqrt(tx*tx + ty*ty + tz*tz)
	//
	f1 = x*x;
	f2 = y*y;
	f3 = z*z;
	f1 = f1 + f2;
	f1 = f1 + f3;
	f1 = (float)sqrt(f1);

	//return (float)sqrt(x*x + y*y + z*z);
	return f1;
}


///////////////////////////////////////////////////////////////////////////////////////////////////
//
//
float CVector::Magnitude2(void)
{
	float f1,f2,f3;
	// Vector 'Magnitude Squared' for 'this'
	//
	// m = (tx*tx + ty*ty + tz*tz)
	//
	f1 = x*x;
	f2 = y*y;
	f3 = z*z;
	f1 = f1 + f2;
	f1 = f1 + f3;

	//return (float)(x*x + y*y + z*z);
	return f1;
}


///////////////////////////////////////////////////////////////////////////////////////////////////
//
//
CVector CVector::Unit(void)
{
	// Unit Vector for 'this'
	//
	// Tu = T/sqrt(tx*tx + ty*ty + tz*tz)
	//
	CVector T(x, y, z);

	float mag = Magnitude();
	if(mag == 0) return CVector(0,0,0); // dont allow div by 0
	
	return (T / mag);
	
	//return (T / Magnitude());
}


///////////////////////////////////////////////////////////////////////////////////////////////////
//
//
CVector CVector::RotateXY(float a)
{
	float f1, f2;
//	/ i' \   /  cos(a)   sin(a)  0 \ / i \
//	| j' | = | -sin(a)   cos(a)  0 | | j |
//	\ k' /   \    0        0     1 / \ k /
	
	
	CVector R;
		
	f1 = cos(a)*x;
	f2 = sin(a)*y;
	//R.x = cos(a)*x + sin(a)*y;
	R.x = f1 + f2;
	
	f1 = sin(a)*x;
	f2 = cos(a)*y;
	//R.y = -sin(a)*x + cos(a)*y;
	R.y = -f1 + f2;
	R.z = z;

	return R;
}