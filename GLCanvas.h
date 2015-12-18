// ====================================================================
// GLCanvas class by Rob Jagnow.
//
// The GLCanvas can be created with a call to the 'initialize' routine,

// ====================================================================

#ifndef _GL_CANVAS_H_
#define _GL_CANVAS_H_
#include <stdlib.h>
#include "argparser.h"
#include "camera.h"
#include "mesh.h"
#include <QtOpenGL/QGLWidget>
#include <QtOpenGL>
#include <QGLWidget>
#include <time.h>
#include <QGLBuffer>
#include "trackball.h" 
// handel mouse movemet
// ====================================================================
class GLCanvas : public QGLWidget
{
	Q_OBJECT
public:
  Mesh mesh;
  int flag;
  // Constructor and destructor
  explicit GLCanvas(QWidget *parent = NULL) ;
  virtual ~GLCanvas();
  double zoom; // value of the zooming 
  // Set up the canvas and enter the rendering loop
  // Note that this function will not return but can be
  // terminated by calling 'exit(0)'
  //void initialize(ArgParser *_args, Mesh *_mesh);
  void Render();
  void LargestGapRender();
  void BoundaryRender();
  void BBRender();
  void FacesNormalRender();
  void SmoothNormalRender();
  void TangentandbiTangentRender();
  void displayInfo(); // display information
  bool displayInfoEnabled;
  TrackBall trackBall; // trackball for mouse
protected:
  void initializeGL();
  void resizeGL(int width, int height); // reshape the view port 
  void paintGL();//Render our model 
  void mouseMoveEvent(QMouseEvent *event); // handel the mouse movement
  void mousePressEvent(QMouseEvent *event); // handel the mouse pressing 
  void wheelEvent(QWheelEvent *event); // handel the mouse rolling effect
  void keyPressEvent(QKeyEvent *event); // pressing any keyboard 
  //void mouseDoubleClickEvent(QMouseEvent *event); // when the the user double click on specific point
signals:
    void statusBarMessage(QString ns);
public slots:
  void animate(); // any mouse movement required 
  void setDisplayInfo(bool enable); // display the information abut the mouse movement and fps
private:
  static ArgParser *args;
  Camera camera;
  void GLCanvas::InitLight();
  QTimer timer;// timers
  int display_list_index;
  QTime fpsTime; // time frame per second  
  double fps; // frame per second value 
  int fpsCounter; // cout frame per second of mouse movement
  clock_t begin,end;
  /////////////////////////////////////// end of main component /////////////////////////// 
  QMatrix4x4 modelViewMatrix;  // mterix model to render model 
  QMatrix4x4 projectionMatrix; // to render projection 
  QMatrix3x3 normalMatrix; // to render normal
};
// ====================================================================
int HandleGLError();
#endif
