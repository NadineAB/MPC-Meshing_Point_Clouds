/****************************************************************************
** file: camera.h
*****************************************************************************/
#ifndef CAMERA_H
#define CAMERA_H

#include <QVector3D>

class Camera  // define the default camera in the scene by defining the three vector from qt type 
	// to track the eye and the up view and at.
{
public:
    Camera();
    QVector3D eye; // 3d vector for eye as camera
    QVector3D at; // 3d vector for at camera 
    QVector3D up;// 3d vector for up vector camera 
};

#endif // CAMERA_H
