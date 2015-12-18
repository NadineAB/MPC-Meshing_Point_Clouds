/****************************************************************************
** file: trackball.h
****************************************************************************/
#ifndef TRACKBALL_H
#define TRACKBALL_H
#include <QVector3D> // 3d vector 
#include <QQuaternion> // QQuaternion
#include <QTime> // Timer in order to update over the time
#include <cmath> // math lib to define M_PI

class TrackBall
{
public:
    TrackBall(); // constructor
    void mouseMove(const QPointF& p);//when mouse move
    void mousePress(const QPointF& p, Qt::MouseButton button); // when mouse press
    void mouseRelease(const QPointF& p);// when mouse release 
    void resizeViewport(int width, int height); // reshape the draw view of opengl 
    QQuaternion getRotation(); // get mouse 3d rotation 
private:
    QQuaternion rotation; // 3d orientation 
    QVector3D axis; // axis values 
    double velocity; // velocity of mouse moving 
    QVector3D lastPos3D; // last mouse postion 
    QTime lastTime; // time in order to update the over time or refersh 
    bool trackingMouse; // tracking the mouse
    double viewportWidth; // the height view port of opengl drawing part
    double viewportHeight; // the height view port of opengl drawing part 
    const double rad2deg; // from radian to degree 
    QVector3D mousePosTo3D(const QPointF& p);  // track the postion of mouse 
};
#endif
