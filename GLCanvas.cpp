#include "glCanvas.h"
#include <QtCore/qmath.h>
#include <QGLWidget>
// Included files for OpenGL 
//#include <GL/gl.h>
#include <GL/glut.h>
#include <windows.h>
#include <time.h>
// ========================================================
// static variables of GLCanvas class
// State of the mouse cursor
bool BB=0;
bool NN=0;
bool WW=0;
bool FF=0;
bool TT=0;
ArgParser* GLCanvas::args;
//Mesh GLCanvas::mesh;
// ========================================================
void DrawAxes(double length)
{
	/*glPushMatrix();
	//glScalef(length, length, length);
	glLineWidth(5.0f);
     glBegin(GL_LINES);
     glColor3f(1.0f, 0.0f, 0.0f);
     glVertex3f(0.0f, 0.0f, 0.0f); glVertex3f(1.0f, 0.0f, 0.0f);
     glVertex3f(1.0f, 0.1f, 0.0f); glVertex3f(1.1f, -0.1f, 0.0f);
     glVertex3f(1.1f, 0.1f, 0.0f); glVertex3f(1.0f, -0.1f, 0.0f);
     glColor3f(0.0f, 1.0f, 0.0f);
     glVertex3f(0.0f, 0.0f, 0.0f); glVertex3f(0.0f, 1.0f, 0.0f);
     glVertex3f(-0.05f, 1.25f, 0.0f); glVertex3f(0.0f, 1.15f, 0.0f);
     glVertex3f(0.05f,1.25f, 0.0f); glVertex3f(0.0f, 1.15f, 0.0f);
     glVertex3f(0.0f,1.15f, 0.0f); glVertex3f(0.0f, 1.05f, 0.0f);
     glColor3f(0.0f, 0.0f, 1.0f);
     glVertex3f(0.0f,0.0f,0.0f); glVertex3f(0.0f, 0.0f, 1.0f);
     glVertex3f(-0.05f,0.1f,1.05f); glVertex3f(0.05f, 0.1f, 1.05f);
     glVertex3f(0.05f,0.1f,1.05f); glVertex3f(-0.05f, -0.1f, 1.05f);
     glVertex3f(-0.05f,-0.1f,1.05f); glVertex3f(0.05f, -0.1f, 1.05f);
     glEnd();*/
     glLineWidth(1.0f);
     glColor3f(0.1f,0.1f,0.1f);
     glBegin(GL_LINES);
     float d = 100.0f;
     for(float i = -d; i <= d; i += 1.0f)
        {
            glVertex3f(i, 0.0f, -d);
            glVertex3f(i, 0.0f, d);
            glVertex3f(-d, 0.0f, i);
            glVertex3f(d, 0.0f, i);
        }

        glEnd();
	   glPopMatrix();
}
double getCPUtimeusage(clock_t clock1,clock_t clock2)
{
    double diffticks=clock1-clock2;
    double diffms=(diffticks*1000)/CLOCKS_PER_SEC;
    return diffms;
} 
GLCanvas::GLCanvas(QWidget *parent) :
QGLWidget(QGLFormat(QGL::SampleBuffers), parent)    
 //QGLWidget(parent)
{
setFocusPolicy( Qt::StrongFocus );
 zoom = 0.0;
 displayInfoEnabled = true;
 fpsCounter = 0;
}
GLCanvas::~GLCanvas()
{
	
}
void GLCanvas::InitLight() {
  // Set the last component of the position to 0 to indicate
  // a directional light source

  GLfloat position[4] = { 30,30,100, 1};
  GLfloat diffuse[4] = { 0.75,0.75,0.75,1};
  GLfloat specular[4] = { 0,0,0,1};
  GLfloat ambient[4] = { 0.2, 0.2, 0.2, 1.0 };

  GLfloat zero[4] = {0,0,0,0};
  glLightfv(GL_LIGHT1, GL_POSITION, position);
  glLightfv(GL_LIGHT1, GL_DIFFUSE, diffuse);
  glLightfv(GL_LIGHT1, GL_SPECULAR, specular);
  glLightfv(GL_LIGHT1, GL_AMBIENT, zero);
  glEnable(GL_LIGHT1);
  glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE);
  glEnable(GL_COLOR_MATERIAL);
  glLightModelfv(GL_LIGHT_MODEL_AMBIENT, ambient);

  GLfloat spec_mat[4] = {1,1,1,1};
  float glexponent = 30;
  glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, &glexponent);
  glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, spec_mat);

  glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
  float back_color[] = { 0.0,0.0,1.0,1};
  glMaterialfv(GL_BACK, GL_AMBIENT_AND_DIFFUSE, back_color);
  glEnable(GL_LIGHT1);
}
void GLCanvas::initializeGL()
{  cout<< flag;
    glPointSize( 9.0 );
    glEnable( GL_POINT_SMOOTH );
    glEnable( GL_BLEND );
    glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
    glRotatef(180,0.0f,1.0f,0.0f);// I rotate eveything in the scene light and objects 
    glEnable(GL_COLOR_MATERIAL);
    glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
    glClearColor( 0.7f, 0.7f, 0.7f, 1.0f ); // background color
    glClearDepth(1.0f);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
    glShadeModel(GL_SMOOTH);
    glEnable(GL_NORMALIZE);
    connect(&timer, SIGNAL(timeout()), this, SLOT(animate())); // to know
    timer.start(0);
    fpsTime.start();
}
void GLCanvas::displayInfo()
{
   if (!displayInfoEnabled)
        return;

    fpsCounter++;

    if (fpsTime.elapsed() > 1000) {
        fps = fpsCounter * 1000.0 / fpsTime.elapsed();
        fpsTime.restart();
        fpsCounter = 0;
    }
	DrawAxes(1);
	glClearColor( 0.7f, 0.7f, 0.7f, 1.0f ); // background color
	qglColor(QColor(0,0,0));
	renderText(751, 21, QString("Vertices: %1").arg(mesh.numVertices()));
     qglColor(QColor(255,255,255));
	renderText(750, 20, QString("Vertices: %1").arg(mesh.numVertices()));
	qglColor(QColor(0,0,0));
	renderText(751, 35, QString("Faces: %1").arg(mesh.numTriangles()));
     qglColor(QColor(255,255,255));
	renderText(750, 34, QString("Faces: %1").arg(mesh.numTriangles()));
	qglColor(QColor(0,0,0));
	qglColor(QColor(0,0,0));
	renderText(751, 50, QString("Edges: %1").arg(mesh.numEdges()));
     qglColor(QColor(255,255,255));
	renderText(750, 49, QString("Edges: %1").arg(mesh.numEdges()));
	qglColor(QColor(0,0,0));
	///////////////////////////////////////////////////
     renderText(11, 21, QString("FPS: %1").arg(fps));
     qglColor(QColor(255,255,255));
	renderText(10, 20, QString("FPS: %1").arg(fps));
	qglColor(QColor(0,0,0));
     renderText(11, 35, QString("Wireframe and Boundary Press: W"));
	qglColor(QColor(255,255,255));
	renderText(10, 34, QString("Wireframe and Boundary Press: W"));
	qglColor(QColor(0,0,0));
     renderText(11, 50, QString("Bounding Box Press: B"));
     qglColor(QColor(255,255,255));
	renderText(10, 49, QString("Bounding Box Press: B"));
	qglColor(QColor(0,0,0));
     renderText(11, 65, QString("Face Normal Press: F"));
	qglColor(QColor(255,255,255));
     renderText(10, 64, QString("Face Normal Press: F"));
	qglColor(QColor(0,0,0));
     renderText(11, 80, QString("Vertices Normals Press: N"));
     qglColor(QColor(255,255,255));
	renderText(10, 79, QString("Vertices Normals Press: N"));
	qglColor(QColor(0,0,0));
     renderText(11, 95, QString("Vertices Tangent Press: T"));
	qglColor(QColor(255,255,255));
	renderText(10, 94, QString("Vertices Tangent Press: T"));
	if(0)
	{
	qglColor(QColor(0,0,0));
	renderText(11, 110, QString("Time Elapsed:%1").arg(double(getCPUtimeusage(end,begin))));
	qglColor(QColor(255,255,255));
     renderText(10, 109, QString("Time Elapsed:%1").arg(double(getCPUtimeusage(end,begin))));
     qglColor(QColor(0,0,0));
	renderText(11, 125, QString("MS: : %1").arg(double(getCPUtimeusage(end,begin))/1000));
	qglColor(QColor(255,255,255));
	renderText(10, 124, QString("MS: : %1").arg(double(getCPUtimeusage(end,begin))/1000));
	qglColor(QColor(0,0,0));
	renderText(11, 135, QString("Number of Iteration: : %1").arg(double(1)));
	qglColor(QColor(255,255,255));
	renderText(10, 134, QString("Number of Iteration: : %1").arg(double(1)));
	
	
	}
}
// ========================================================
// Callback function for window resize
// ========================================================
void GLCanvas::resizeGL(int width, int height) 
{
   glViewport(0, 0, width, height);
   trackBall.resizeViewport(width, height);
  /*glMatrixMode(GL_PROJECTION);
   glLoadIdentity();
   gluPerspective(                   // Set perspective view
        65,                         // Field of view = 65 degrees
        (double)width/(double)height, // Window aspect (assumes square pixels)
        1.0,                          // Near Z clipping plane
        100.0                         // Far Z clippling plane
    );
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();	    
    gluLookAt(2,2,2,0,0,0,-1,0,-1);
    modelViewMatrix.translate(2, 2, 2);
    modelViewMatrix.rotate(trackBall.getRotation());
    normalMatrix = modelViewMatrix.normalMatrix();*/
    updateGL(); // refersh the opengl modification each time change port size
}
// ========================================================
// Callback function for mouse click or release
// ========================================================
void GLCanvas::mouseMoveEvent(QMouseEvent *event)
{ 
// Save the current state of the mouse.  This will be
// used by the 'motion' function
 trackBall.mouseMove(event->posF());
   this->updateGL();
}
// ========================================================
// Callback function for mouse drag
// ========================================================
//  event handels the mouse Click and Release Event  
void GLCanvas::mousePressEvent(QMouseEvent *event)
{
if(event->type() == QEvent::MouseButtonPress || event->type() == QEvent ::MouseButtonRelease)
 { 
	 if (event->button() & Qt::RightButton) 
	 {}
	 else
     {
	trackBall.mousePress(event->posF(),event->button());
	 //qWarning() << "leftButton";
	  this->updateGL();
	 }
this->updateGL();
}
}
void GLCanvas::wheelEvent(QWheelEvent *event)
{
if(event->type() == QEvent::Wheel)
{
zoom += 0.001 * event->delta(); // zooming
 this->updateGL();
}
}
// ========================================================
// Callback function for keyboard events
// ========================================================
void GLCanvas::keyPressEvent(QKeyEvent *e)  //QKeyEvent *e
{ // key press to quit the opengl window
	
if (flag==1)
{
if(e->type() == QEvent::KeyPress)
{
 cout << "here ";
      switch(e->key())
     {
		case Qt::Key_Escape:
		{
		close();
          qApp->quit();
		updateGL();
		}
		case Qt::Key_Q:
		{
		close();
          qApp->quit();
		updateGL();
		}
		 case Qt::Key_B:
		{
		if (BB)
		{
		BB=0;
		updateGL();
		}
		else 
		{
		BB=1;
		updateGL();
		}
		break;
		}
		case Qt::Key_N:
		{
		if (NN)
		{
		NN=0;
		updateGL();
		}
		else 
		{
		NN=1;
		updateGL();
		}
		break;
		}
		case Qt::Key_W:
		{
		if (WW)
		{
		WW=0;
		updateGL();
		}
		else 
		{
		WW=1;
		updateGL();
		}
		break;
		}
		case Qt::Key_F:
		{
		if (FF)
		{
		FF=0;
		updateGL();
		}
		else 
		{
		FF=1;
		updateGL();
		}
		break;
		}
		case Qt::Key_T:
		{
		if (TT)
		{
		TT=0;
		updateGL();
		}
		else 
		{
		TT=1;
		updateGL();
		}
		break;
		}

	 }

}
}
 this->updateGL();
}
void GLCanvas:: animate()
{
	 this->updateGL();
}   // mouse movment inside the scene
void GLCanvas::setDisplayInfo(bool enabled)
{
   displayInfoEnabled = enabled;
    this->updateGL();  // enable display information 
}
void GLCanvas::paintGL()
{
   //GLdouble eqn[4] = {0.0, 0.0, 0.0, 0.1};
   //GLdouble eqn2[4] = {0.0, 0.0, 0.0, 10000.00};
    // NADINE make sure that model view matrix work boz camera not working at all !!!
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST);
    glDepthRange(0.1,10000.00) ;
    //glMatrixMode(GL_MODELVIEW);
    DrawAxes(1);
    HandleGLError(); 
    glPointSize( 10.0 );
    glEnable( GL_POINT_SMOOTH );
    glEnable( GL_BLEND );
    glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA ); 
    /*glClipPlane (GL_CLIP_PLANE0, eqn);
    glEnable (GL_CLIP_PLANE0);
    glClipPlane (GL_CLIP_PLANE1, eqn2);
    glEnable (GL_CLIP_PLANE1);*/
    glPushMatrix();
    modelViewMatrix.setToIdentity();
    modelViewMatrix.lookAt(camera.eye, camera.at, camera.up);
    modelViewMatrix.translate(2, 2, 2);
    modelViewMatrix.rotate(-1,0,-1);
    normalMatrix = modelViewMatrix.normalMatrix();
    Render();
    displayInfo();
    if (BB)
    {
     BBRender();
    }
     if (WW)
    {
     BoundaryRender();
    }
     if (FF)
    {
     FacesNormalRender();
    }
	  if (NN)
    {
     SmoothNormalRender();
    }
	    if (TT)
    {
     TangentandbiTangentRender();
    }
    glPopMatrix();
}
void GLCanvas::Render()
{
mesh.Paint();
}
void GLCanvas::BBRender()
{
	mesh.PaintBB();
}
void GLCanvas::FacesNormalRender()
{
  mesh.DrawNormalFaces();;
}
void GLCanvas ::SmoothNormalRender()
{
mesh.DrawNormalVertices();
}
void GLCanvas ::TangentandbiTangentRender()
{
mesh.DrawTangentVertices();
}
void GLCanvas ::LargestGapRender()
{
}
void GLCanvas ::BoundaryRender()
{
	mesh.BoundaryPaint();
}
// ========================================================
// ========================================================

int HandleGLError() {
  GLenum error;
  int i = 0;
  while ((error = glGetError()) != GL_NO_ERROR) {
    printf ("GL ERROR(%d):  %s\n", i, gluErrorString(error));
    i++;
  }
  if (i == 0) return 1;
  return 0;
}
