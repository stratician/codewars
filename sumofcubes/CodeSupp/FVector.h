///////////////////////////////////////////////////////////////////////////////////////////////////
// (FVector.h)
//

#ifndef __FVECTOR_H__
#define __FVECTOR_H__

#include <memirlib.h>

typedef __int64 __fix;

#define VFIX_PRECBIT	16		// 48:16
#define VFIX_PREC		(1<<VFIX_PRECBIT)
#define VFIX_PRECBIT2	(VFIX_PRECBIT/2)

#define TOFIX(v) ((__fix)((v)*VFIX_PREC))
#define FROMFIX(v) ((v)>>VFIX_PRECBIT)
#define FROMFIXSMALL(v) (((float)v)/VFIX_PREC)

#define FIXMULSMALL(u,v) (((u)*(v))>>VFIX_PRECBIT)				// 48:16*48:16 = 96:32 [32:32] = 16:16
#define FIXMULBIG(u,v) (((u)>>VFIX_PRECBIT2)*((v)>>VFIX_PRECBIT2)) // 48:16*48:16 = [40:8] = [80:16] = 32:16

#define FIXMUL(u,v) FIXMULSMALL(u,v)			// Defaultly works best for small numbers 16:16 * 16:16 i.e. 1/64k accurate upto 64k
												// Big means works for 24:8 * 24:8 i.e. 1/256 accurate upto 16.7M
#define FIXDIV(u,v) (((u)<<VFIX_PRECBIT)/(v))


class FVector {
public:
	__fix x;
	__fix y;
	__fix z;
	
	/*FVector(float a = 0, float b = 0, float c = 0) {
		x = TOFIX(a);
		y = TOFIX(b);
		z = TOFIX(c);
	}*/

	FVector(__fix a = 0, __fix b = 0, __fix c = 0) {
		x = a;
		y = b;
		z = c;
	}

	//~FVector();

	void DisplayBox(void);

	FVector operator+(FVector &A);
	FVector operator-(FVector &A);
	FVector operator*(__fix s);
	friend FVector operator*(__fix s, FVector &A);
	FVector operator/(__fix s);
	FVector Cross(FVector &P);
	__fix Dot(FVector &P);
	__fix Magnitude(BOOL bAccurate = FALSE);
	__fix Magnitude2(void);
	FVector Unit(BOOL bAccurate = FALSE);
	FVector RotateXY(__fix a);
};

__fix sqrtBabylonian(__fix xx, __fix xStart = 0, int steps = 20);

#endif // __FVECTOR_H__