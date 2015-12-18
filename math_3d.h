#ifndef MATH_3D_H
#define	MATH_3D_H
#include <iostream>
#include <vector>
#include <list>
#include <QDebug>
#include <stdio.h>
#include <stdio.h>
#ifdef WIN32
#define _USE_MATH_DEFINES 
#include <cmath>
#else
#include <math.h>
#endif

#define ToRadian(x) (float)(((x) * M_PI / 180.0f))
//#define ToDegree(x) (float)(((x) * 180.0f / M_PI))

float RandomFloat();

struct Vector2i
{
    int x;
    int y;
};

struct Vector2f
{
    float x;
    float y;

    Vector2f()
    {
    }
   
     Vector2f& operator-=(const Vector2f& r)
    {
        x -= r.x;
        y -= r.y;
     
        return *this;
    }
    Vector2f(float _x, float _y)
    {
        x = _x;
        y = _y;
    }
};
struct Vector3i
{
    int x;
    int y;
    int z;
	int getX() { return x;}
	int getY() { return y;}
	float getZ() { return z;}
Vector3i()
    {
    }
Vector3i(int _x, int _y, int _z)
    {
        x = _x;
        y = _y;
        z = _z;
    }
};

struct Vector3f
{
    float x;
    float y;
    float z;
	float getX() { return x;}
	float getY() { return y;}
	float getZ() { return z;}
     bool Equal(const Vector3f &v,const Vector3f &v1 );
    Vector3f()
    {
    }

    Vector3f(float _x, float _y, float _z)
    {
        x = _x;
        y = _y;
        z = _z;
    }

    Vector3f& operator+=(const Vector3f& r)
    {
        x += r.x;
        y += r.y;
        z += r.z;

        return *this;
    }
	  
     Vector3f & operator /= ( const double s)
	{
		x /= s;
		y /= s;
		z /= s;

		return *this;
	}
    Vector3f& operator-=(const Vector3f& r)
    {
        x -= r.x;
        y -= r.y;
        z -= r.z;

        return *this;
    }

    Vector3f& operator*=(float f)
    {
        x *= f;
        y *= f;
        z *= f;

        return *this;
    }

    Vector3f Cross(const Vector3f& v) const;
	float Dot(const Vector3f& v);
    Vector3f Normalize();
    Vector3f Normalize(Vector3f);
    Vector3f Normalize(double);
    void Rotate(float Angle, const Vector3f& Axis);
    float Magnitude( );
	float Distance( const Vector3f &V1 );
    void Print() const
    {
        printf("(%.02f, %.02f, %.02f)", x, y, z);
    }
};


struct Vector4f
{
    float x;
    float y;
    float z;
    float w;

    Vector4f()
    {        
    }
    
    Vector4f(float _x, float _y, float _z, float _w)
    {
        x = _x;
        y = _y;
        z = _z;
        w = _w;
    }
    
    void Print() const
    {
        printf("(%.02f, %.02f, %.02f, %.02f)", x, y, z, w);
    }       
};

inline Vector3f operator+(const Vector3f& l, const Vector3f& r)
{
    Vector3f Ret(l.x + r.x,
                 l.y + r.y,
                 l.z + r.z);

    return Ret;
}

inline Vector3f operator-(const Vector3f& l, const Vector3f& r)
{
    Vector3f Ret(l.x - r.x,
                 l.y - r.y,
                 l.z - r.z);

    return Ret;
}

inline Vector3f operator*(const Vector3f& l, float f)
{
    Vector3f Ret(l.x * f,
                 l.y * f,
                 l.z * f);

    return Ret;
}

struct PersProjInfo
{
    float FOV;
    float Width; 
    float Height;
    float zNear;
    float zFar;
};

class Matrix4f
{
public:
    float m[4][4];

    Matrix4f()
    {        
    }


    inline void InitIdentity()
    {
        m[0][0] = 1.0f; m[0][1] = 0.0f; m[0][2] = 0.0f; m[0][3] = 0.0f;
        m[1][0] = 0.0f; m[1][1] = 1.0f; m[1][2] = 0.0f; m[1][3] = 0.0f;
        m[2][0] = 0.0f; m[2][1] = 0.0f; m[2][2] = 1.0f; m[2][3] = 0.0f;
        m[3][0] = 0.0f; m[3][1] = 0.0f; m[3][2] = 0.0f; m[3][3] = 1.0f;
    }

    inline Matrix4f operator*(const Matrix4f& Right) const
    {
        Matrix4f Ret;

        for (unsigned int i = 0 ; i < 4 ; i++) {
            for (unsigned int j = 0 ; j < 4 ; j++) {
                Ret.m[i][j] = m[i][0] * Right.m[0][j] +
                              m[i][1] * Right.m[1][j] +
                              m[i][2] * Right.m[2][j] +
                              m[i][3] * Right.m[3][j];
            }
        }

        return Ret;
    }
    
    Vector4f operator*(const Vector4f& v) const
    {
        Vector4f r;
        
        r.x = m[0][0]* v.x + m[0][1]* v.y + m[0][2]* v.z + m[0][3]* v.w;
        r.y = m[1][0]* v.x + m[1][1]* v.y + m[1][2]* v.z + m[1][3]* v.w;
        r.z = m[2][0]* v.x + m[2][1]* v.y + m[2][2]* v.z + m[2][3]* v.w;
        r.w = m[3][0]* v.x + m[3][1]* v.y + m[3][2]* v.z + m[3][3]* v.w;
        
        return r;
    }
    
    void Print()
    {
        for (int i = 0 ; i < 4 ; i++) {
            printf("%f %f %f %f\n", m[i][0], m[i][1], m[i][2], m[i][3]);
        }       
    }

    void InitScaleTransform(float ScaleX, float ScaleY, float ScaleZ);
    void InitRotateTransform(float RotateX, float RotateY, float RotateZ);
    void InitTranslationTransform(float x, float y, float z);
    void InitCameraTransform(const Vector3f& Target, const Vector3f& Up);
    void InitPersProjTransform(const PersProjInfo& p);
};


struct Quaternion
{
    float x, y, z, w;

    Quaternion(float _x, float _y, float _z, float _w);

    void Normalize();

    Quaternion Conjugate();  
 };

Quaternion operator*(const Quaternion& l, const Quaternion& r);

Quaternion operator*(const Quaternion& q, const Vector3f& v);
class Matrix3f
{
public:
std::vector <Vector3f> m[10][10][10];
Matrix3f()
{       
}
~Matrix3f()
{ 

}
void Matrix3f_Display()
{  int FullSize=0;
for( int j=0; j <10;j++)
for( int k=0; k <10;k++)
for( int f=0; f <10;f++)
{
//qDebug () << "X" <<j<< "Y"<< k<<"Z" << f;
 FullSize+=m[j][k][f].size();
}
qDebug () <<" Over All Martrix Size " << FullSize;
}
void Matrix3f_PushBack(int _x, int _y, int _z,
Vector3f v, int index )
{   
m[_x][_y][_z].push_back(Vector3f (v));    
}
int Matrix3f_size(int _x, int _y, int _z)
    { 
	    return  m[_x][_y][_z].size();    
    }
std::vector<Vector3f>  Matrix3f_Neighers(int _x, int _y, int _z,
	Vector3f q, 
	std::vector <Vector3f> query)
 {  
    
    int qsize =query.size();
    std::vector<Vector3f> Neighers;
    for ( int i=0 ; i<m[_x][_y][_z].size(); i++)
    {
     if ((m[_x][_y][_z].at(i).getX()!=q.getX() &&
	m[_x][_y][_z].at(i).getX()!=q.getY() &&
	m[_x][_y][_z].at(i).getX()!=q.getZ()))
	{
	if (query.size()==0)	
	Neighers.push_back(m[_x][_y][_z].at(i));
	else if(qsize<25)
	{
	for(int k=0; k<query.size(); k++)	 
	{
	if (m[_x][_y][_z].at(i).getX()!=query.at(k).getX()
		&& m[_x][_y][_z].at(i).getY()!=query.at(k).getY()
		&& m[_x][_y][_z].at(i).getZ()!=query.at(k).getZ()&& qsize<25)
          Neighers.push_back(m[_x][_y][_z].at(i));
			qsize++;
	 }
			// qDebug () << "in 3d math" << qsize;
	if(qsize=25)
	break;
	}
	}
    }
	 return Neighers;
};
};
#endif	/* MATH_3D_H */

