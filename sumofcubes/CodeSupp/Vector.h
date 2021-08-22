///////////////////////////////////////////////////////////////////////////////////////////////////
// (Vector.h)
//

#ifndef __VECTOR_H__
#define __VECTOR_H__

#include <memirlib.h>

extern void MultiplyMatrices(M3d_mtx &c, M3d_mtx &a, M3d_mtx &b);

class CVector {
public:
	float x;
	float y;
	float z;
	
	CVector(float a = 0, float b = 0, float c = 0) {
		x = a;
		y = b;
		z = c;
	}

	//~CVector();

	void DisplayBox(void);

	CVector operator+(CVector &A);
	CVector operator-(CVector &A);
	CVector operator*(float s);
	friend CVector operator*(float s, CVector &A);
	CVector operator/(float s);
	CVector Cross(CVector &P);
	float Dot(CVector &P);
	float Magnitude(void);
	float Magnitude2(void);
	CVector Unit(void);
	CVector RotateXY(float a);
};

#endif // __VECTOR_H__