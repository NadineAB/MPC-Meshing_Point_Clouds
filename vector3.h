#ifndef __VECTOR3__
#define __VECTOR3__

#include <math.h>


// represents a vector in 3D
class Vector3
{
public:
	double x, y, z;		// coordinates

	Vector3()
	{
		x = 0; 
		y = 0; 
		z = 0;
	}

	Vector3(double _x, double _y, double _z)
	{ 
		x = _x; 
		y = _y; 
		z = _z;
	}


	// returns the euclidean norm of the vector
	inline double Length() const
	{
	    return sqrt( x * x + y * y + z * z );
	}
		
	// returns the squared euclidean norm of the vector
	inline double SquaredLength() const
	{
	    return x * x + y * y + z * z;
	}
		
	inline Vector3 & operator =( Vector3 const & v )
	{
			x = v.x; 
			y = v.y; 
			z = v.z;
			return *this;
	}

	// addition among vectors
	inline Vector3 operator + ( Vector3 const & v) const
	{
		return Vector3( x + v.x, y + v.y, z + v.z );
	}

	// addition among vectors
	inline Vector3 & operator += ( Vector3 const & v)
	{
		x += v.x;
		y += v.y;
		z += v.z;

		return *this;
	}

	// difference among vectors
	inline Vector3 & operator -= ( Vector3 const & v)
	{
		x -= v.x;
		y -= v.y;
		z -= v.z;

		return *this;
	}

	// difference among vectors
	inline Vector3 operator - ( Vector3 const & v) const
	{
		return Vector3( x - v.x, y - v.y, z - v.z );
	}

	// multiplication by a scalar (double)
	inline Vector3 operator * ( const double s ) const
	{
		return Vector3( x * s, y * s, z * s);
	}

	// division by a scalar (double)
	inline Vector3 operator / ( const double s ) const
	{
		return Vector3( x / s, y / s, z / s);
	}

	// division by a scalar (double)
	inline Vector3 & operator /= ( const double s)
	{
		x /= s;
		y /= s;
		z /= s;

		return *this;
	}

	// Cross product
	inline Vector3 operator ^ ( Vector3 const & p ) const
	{
		return Vector3
		(
			y * p.z - z * p.y,
			z * p.x - x * p.z,
			x * p.y - y * p.x
		);
	}

	// Dot product
	inline double operator * ( Vector3 const & p ) const
	{
		return x * p.x + y * p.y + z * p.z;
	}

	// normalizes the vector lenght to unity
	inline Vector3 Normalize()
	{
		double length = Length();

		x /= length;
		y /= length;
		z /= length;

		return *this;
	}
};


#endif //__VECTOR3__