// FVector.cpp


#include <stdio.h>
#include <windows.h>
#include <math.h>

#include "FVector.h"

///////////////////////////////////////////////////////////////////////////////////////////////////
//
//
void FVector::DisplayBox(void)
{
	char buffer[80];
	sprintf(buffer, "(%f, %f, %f)", FROMFIXSMALL(x), FROMFIXSMALL(y), FROMFIXSMALL(z));
	MessageBox(0, buffer, "FVector", MB_OK);
}


///////////////////////////////////////////////////////////////////////////////////////////////////
// + operator
//
FVector FVector::operator+(FVector &A)
{
	return FVector( x+A.x, y+A.y, z+A.z );
}


///////////////////////////////////////////////////////////////////////////////////////////////////
// - operator
//
FVector FVector::operator-(FVector &A)
{
	return FVector( x-A.x, y-A.y, z-A.z );
}


///////////////////////////////////////////////////////////////////////////////////////////////////
// * operator
//
FVector FVector::operator*(__fix s)		// i.e. A * 5
{
	return FVector(FIXMUL(x,s), FIXMUL(y,s), FIXMUL(z,s));
}


///////////////////////////////////////////////////////////////////////////////////////////////////
// * operator (friend function)
//
FVector operator*(__fix s, FVector &A) // i.e. 5 * A
{
	return FVector(FIXMUL(s,A.x), FIXMUL(s,A.y), FIXMUL(s,A.z));
}


///////////////////////////////////////////////////////////////////////////////////////////////////
// / operator
//
FVector FVector::operator/(__fix s)		// i.e. A / 5
{
	return FVector(FIXDIV(x,s), FIXDIV(y,s), FIXDIV(z,s));
}


///////////////////////////////////////////////////////////////////////////////////////////////////
//
//
FVector FVector::Cross(FVector &P)
{
	/*
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

	//return FVector(((y*P.z) - (P.y*z)), -((x*P.z) - (P.x*z)), ((x*P.y) - (P.x*y)));
	//return FVector((f1-f2), -(f3-f4), (f5-f6));
	return FVector(fx, fy, fz);
	*/
	__fix f1,f2,f3,f4,f5,f6;
	__fix fx,fy,fz;
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

	return FVector(fx>>VFIX_PRECBIT, fy>>VFIX_PRECBIT, fz>>VFIX_PRECBIT);
}


///////////////////////////////////////////////////////////////////////////////////////////////////
//
//
__fix FVector::Dot(FVector &P)
{
	return (x*P.x + y*P.y + z*P.z)>>VFIX_PRECBIT;
	/*float f1,f2,f3;
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
	

	//return f1;//f1+f2+f3;//((x*P.x) + (y*P.y) + (z*P.z));
	*/
}

__fix fastdist(__fix x, __fix y)
{// Computes Distance between (0,0) & (x,y) - FAST but 3.5% error!!!!
// Got this from André Lamothe - Very Impressive!
// first compute the absolute value of x,y
if(x<0) x = -x;
if(y<0) y = -y;

// compute the minimum of x,y
__fix mn = min(x,y);

// return the distance
return(x+y-(mn>>1)-(mn>>2)+(mn>>4));
} // end Fast_Distance

__fix fastdist(__fix x, __fix y, __fix z)
{// Computes Distance between (0,0) & (x,y) - FAST but 3.5% error!!!!
	return fastdist(fastdist(x,y),z);
}

__fix sqrtBabylonian(__fix xx, __fix xStart, int steps)
{
//	float fxx = FROMFIXSMALL(xx);
//	fxx = sqrt(fxx);
//	return TOFIX(fxx);
	

	if(xStart == 0) xStart = FIXDIV(xx, 2.0f);
	if(xStart == 0) return 0;

	int n;
	__fix r, rold;
	__fix a;
	rold = r = xStart;
			
	for(n=0;n<steps;n++)
	{
		if(r == 0) return 0;//rold;
		//Babylonian method to approximate sqrt of xx  ->  r = (r + xx/r) * 0.5f;
		rold = r;
		a =  FIXDIV(xx, r);
		a = a + r;
		r = FIXMUL(a, TOFIX(0.5f));
	}		
	
	return r;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
//
//
__fix FVector::Magnitude(BOOL bAccurate)
{
	/*
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
	*/
	/*
	__fix tx,ty,tz;
	__fix mn;
	if(x<0) tx = -x;
	else tx = x;
	if(y<0) ty = -y;
	else ty = y;
	if(z<0) tz = -z;
	else tz = z;

	mn = min(min(tx,ty),tz);

	return (tx+ty+tz)-(mn>>1)-(mn>>2)+(mn>>4)); // x+y+z - 0.707*mn
	*/

  /*
int Mgame_fastdist(int x, int y)
{// Computes Distance between (0,0) & (x,y) - FAST but 3.5% error!!!!
// Got this from André Lamothe - Very Impressive!
// first compute the absolute value of x,y
x = abs(x);
y = abs(y);

// compute the minimum of x,y
int mn = min(x,y);

// return the distance
return(x+y-(mn>>1)-(mn>>2)+(mn>>4));
} // end Fast_Distance
*/

	//if(!bAccurate)
	//{
	//	return fastdist(x,y,z);
	//}
	//else
	{
		////----
		BOOL bBIGX = FALSE;
		BOOL bBIGY = FALSE;
		BOOL bBIGZ = FALSE;
		BOOL bSMALLX = FALSE;
		BOOL bSMALLY = FALSE;
		BOOL bSMALLZ = FALSE;
		BOOL bBIG = FALSE;
		BOOL bSMALL = FALSE;
		float x1, y1, z1;
		////----



		// first compute the absolute value of x,y
		
		__fix fx;
		__fix fy;
		__fix fz;

		// get absolute values (no -ves)
		if(x<0) fx = -x;
		else fx = x;
		if(y<0) fy = -y;
		else fy = y;
		if(z<0) fz = -z;
		else fz = z;

		////----
		x1 = FROMFIXSMALL(fx);
		y1 = FROMFIXSMALL(fy);
		z1 = FROMFIXSMALL(fz);

		if(x1 > 1000.0f) bBIGX = TRUE;
		if(y1 > 1000.0f) bBIGY = TRUE;
		if(z1 > 1000.0f) bBIGZ = TRUE;

		if(x1 < 0.001f) bSMALLX = TRUE;
		if(y1 < 0.001f) bSMALLY = TRUE;
		if(z1 < 0.001f) bSMALLZ = TRUE;

		if(bBIGX || bBIGY || bBIGZ)
		{
			bBIG = TRUE;
		}
		else if(bSMALLX && bSMALLY && bSMALLY)
		{
			bSMALL = TRUE;
		}
		   
		if(bBIG)
		{
			fx = FIXMUL(fx, TOFIX(0.001f));
			fy = FIXMUL(fy, TOFIX(0.001f));
			fz = FIXMUL(fz, TOFIX(0.001f));
		}

		if(bSMALL)
		{
			fx = FIXMUL(fx, TOFIX(1000.0f));
			fy = FIXMUL(fy, TOFIX(1000.0f));
			fz = FIXMUL(fz, TOFIX(1000.0f));
		}
		
		////----

		__fix guess;
		if( min(fx, fy) ) guess = fy;
		else  guess = fx;
		if( min(guess, fz) ) guess = fz;
		
		// f = fx*fx + fy*fy + fz*fz
		fx = FIXMUL(fx, fx);
		fy = FIXMUL(fy, fy);
		fz = FIXMUL(fz, fz);
		__fix f = fx+fy+fz;
		
		// find the sqrt of f
		//return sqrtBabylonian(f, guess, 10);

		f = sqrtBabylonian(f, guess, 20);
		
		if(bBIG)
		{
			f = FIXMUL(f, TOFIX(1000.0f));
		}

		if(bSMALL)
		{
			f = FIXMUL(f, TOFIX(0.001f));
		}
	  
		
		///////
		// make sure magnitude we get from the sqrt is not smaller than x, y, z
		if(x<0) fx = -x;
		else fx = x;
		if(y<0) fy = -y;
		else fy = y;
		if(z<0) fz = -z;
		else fz = z;
		
		
		//if(f < fx) f = fx;
		//if(f < fy) f = fy;
		//if(f < fz) f = fz;
		

		if(f < fx || f < fy || f <fz) // use old system if we have an incorrect answer
			return fastdist(x,y,z);
		else
			return f; // default
		///////

		//return f;
		

	}
}


///////////////////////////////////////////////////////////////////////////////////////////////////
//
//
__fix FVector::Magnitude2(void)
{
	/*float f1,f2,f3;
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
	*/
	return (x*x + y*y + z*z)>>VFIX_PRECBIT;
}


///////////////////////////////////////////////////////////////////////////////////////////////////
//
//
FVector FVector::Unit(BOOL bAccurate)
{
	// Unit Vector for 'this'
	//
	// Tu = T/sqrt(tx*tx + ty*ty + tz*tz)
	//
	FVector T(x, y, z);

	__fix mag = Magnitude(bAccurate);

	if(mag == 0) return FVector(0,0,0); // dont allow div by 0
	
	return (T / mag);

	//return (T / Magnitude());
}


///////////////////////////////////////////////////////////////////////////////////////////////////
//
//
FVector FVector::RotateXY(__fix a) // ** NOT FIXED
{
	double f1, f2;
//	/ i' \   /  cos(a)   sin(a)  0 \ / i \
//	| j' | = | -sin(a)   cos(a)  0 | | j |
//	\ k' /   \    0        0     1 / \ k /
	
	
	FVector R;
		
	f1 = cos(FROMFIXSMALL(a))*x;
	f2 = sin(FROMFIXSMALL(a))*y;
	//R.x = cos(a)*x + sin(a)*y;
	R.x = f1 + f2;
	
	f1 = sin(FROMFIXSMALL(a))*x;
	f2 = cos(FROMFIXSMALL(a))*y;
	//R.y = -sin(a)*x + cos(a)*y;
	R.y = -f1 + f2;
	R.z = z;

	//R.x>>=VFIX_PRECBIT;
	//R.y>>=VFIX_PRECBIT;

	return R;
}
