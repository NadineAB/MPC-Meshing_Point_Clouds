/****************************************************************************
** file: trackball.cpp

****************************************************************************/
#include "trackball.h"
#include <math.h> // M_PI
#include "Vector3.h"
#include <GL/glut.h>
// size of the window
int  WINDOW_WIDTH=1069;
int  WINDOW_HEIGHT=649;
const double m_ROTSCALE = 90.0;
const double m_ZOOMSCALE = 0.008;
float fit_factor = 1.f;
Vector3 trackBallMapping(int x, int y);    // Utility routine to convert mouse locations to a virtual hemisphere
Vector3 lastPoint;                         // Keep track of the last mouse location
enum MovementType { ROTATE, ZOOM, NONE };  // Keep track of the current mode of interaction (which mouse button)
MovementType Movement;                     //    Left-mouse => ROTATE, Right-mouse => ZOOM
Vector3 mouse2D, mouse3D;
TrackBall::TrackBall() : rad2deg(180.0 / 3.14159265358979323846)
{
    velocity = 0.0;
    trackingMouse = false;  // Default  constrctor with no mouse movment  
    lastTime = QTime::currentTime(); // read the current time 
}
// track the mouse motion if it could be rotate or zoom
void TrackBall::mouseMove(const QPointF &p)
{
	
	Vector3 direction;
	double pixel_diff;
	double rot_angle, zoom_factor;
	Vector3 curPoint;
	int x =(int) p.x();
	int y = (int) p.y();
    switch (Movement) 
	{
		
	case ROTATE :  // Left-mouse button is being held down
		{
curPoint = trackBallMapping( x,y );  // Map the mouse position to a logical sphere location.
direction = curPoint - lastPoint;
double velocity = direction.Length(); 
if( velocity > 0.0001 )
{
// Rotate about the axis that is perpendicular to the great circle connecting the mouse movements.
Vector3 rotAxis;
rotAxis = lastPoint ^ curPoint ;
rotAxis.Normalize();
rot_angle = velocity * m_ROTSCALE;
// We need to apply the rotation as the last transformation.
//   1. Get the current matrix and save it.
//   2. Set the matrix to the identity matrix (clear it).
//   3. Apply the trackball rotation.
//   4. Pre-multiply it by the saved matrix.
static GLdouble m[4][4];
glGetFloatv( GL_MODELVIEW_MATRIX, (GLfloat *) m );
glLoadIdentity();
glRotatef( rot_angle, rotAxis.x, rotAxis.y, rotAxis.z );
glMultMatrixf( (GLfloat *) m );
//  If we want to see it, we need to force the system to redraw the scene.
}
break;
}
	case ZOOM :  // Right-mouse button is being held down
//
// Zoom into or away from the scene based upon how far the mouse moved in the x-direction.
//   This implementation does this by scaling the eye-space.
//   This should be the first operation performed by the GL_PROJECTION matrix.
//   1. Calculate the signed distance
//       a. movement to the left is negative (zoom out).
//       b. movement to the right is positive (zoom in).
//   2. Calculate a scale factor for the scene s = 1 + a*dx
//   3. Call glScalef to have the scale be the first transformation.
// 
pixel_diff = y - lastPoint.y; 
zoom_factor = 1.0 + pixel_diff * m_ZOOMSCALE;
glScalef( zoom_factor, zoom_factor, zoom_factor );
// Set the current point, so the lastPoint will be saved properly below.
curPoint.x = (double) x;  curPoint.y = (double) y;  (double) curPoint.z = 0;
//  If we want to see it, we need to force the system to redraw the scene.

break;
	}
// Save the location of the current point for the next movement. 
lastPoint = curPoint;	// in spherical coordinates
mouse2D = Vector3(x, WINDOW_HEIGHT - y, 0);	// in window coordinates
}
// track the mouse press if its left button  or it could be rotate or zoom or none of them
void TrackBall::mousePress(const QPointF &p , Qt::MouseButton button)
{
    int x =(int) p.x();
	int y = (int) p.y();
	mouse2D = Vector3(x, WINDOW_HEIGHT - y, 0);
 	if (button == Qt::NoButton)
	{
		// Turn-off rotations and zoom.
		Movement = NONE;
		glutPostRedisplay();
		return;
	}
	switch (button)
	{
	case (Qt::LeftButton):

		// Turn on user interactive rotations.
		// As the user moves the mouse, the scene will rotate.
		Movement = ROTATE;

		// Map the mouse position to a logical sphere location.
		// Keep it in the class variable lastPoint.
		lastPoint = trackBallMapping( x, y );

		break;

	case (Qt::MiddleButton):

		// Turn on user interactive zooming.
		// As the user moves the mouse, the scene will zoom in or out
		//   depending on the x-direction of travel.
		Movement = ZOOM;

		// Set the last point, so future mouse movements can determine
		//   the distance moved.
		lastPoint.x = (double) x;
		lastPoint.y = (double) y;

		break;

	case (Qt::RightButton):
		Movement = NONE;	
		break;
	}
}
void TrackBall::mouseRelease(const QPointF &p)
{
    mouseMove(p);
    trackingMouse = false;
}
void TrackBall::resizeViewport(int width, int height)  // resize the opengl viewport
{
    viewportWidth = static_cast<double>(width); // width 
    viewportHeight = static_cast<double>(height); // height
}
QQuaternion TrackBall::getRotation()
{
    if (trackingMouse)
        return rotation;

    QTime currentTime = QTime::currentTime();
    double angle = velocity * lastTime.msecsTo(currentTime);
    return QQuaternion::fromAxisAndAngle(axis, angle) * rotation;
}
QVector3D TrackBall::mousePosTo3D(const QPointF &p)
{
    return QVector3D(2.0 * p.x() / viewportWidth - 1.0,
                     1.0 - 2.0 * p.y() / viewportHeight,
                     0.0);
}
//
// Utility routine to calculate the 3D position of a 
// projected unit vector onto the xy-plane. Given any
// point on the xy-plane, we can think of it as the projection
// from a sphere down onto the plane. The inverse is what we
// are after.
//
Vector3 trackBallMapping(int x, int y)
{
	Vector3 v;
	double d;

	v.x = -(2.0 * x - WINDOW_WIDTH) / WINDOW_WIDTH;
	v.y = -(WINDOW_HEIGHT - 2.0 * y) / WINDOW_HEIGHT;
	v.z = 0.0;
	d = v.Length();
	d = (d < 1.0) ? d : 1.0;  // If d is > 1, then clamp it at one.
	v.z = sqrtf( 1.001 - d * d );  // project the line segment up to the surface of the sphere.

	v.Normalize();  // We forced d to be less than one, not v, so need to normalize somewhere.


	return v;
}