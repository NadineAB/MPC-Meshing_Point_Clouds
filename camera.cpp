/*************************************************************************************
**  Definition of  class Camera
*************************************************************************************/
#include "camera.h"

Camera::Camera()
{
    eye = QVector3D(0.0, 0.0, 1.0);
    at = QVector3D(0.0, 0.0, 0.0);
    up = QVector3D(0.0, 1.0, 0.0);
	// define the default camera postion 
}
