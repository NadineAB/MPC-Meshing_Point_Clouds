#ifndef FACEMODELING_H
#define FACEMODELING_H
#include <QtGui/QMainWindow>
#include "ui_facemodeling.h"
#include "glCanvas.h"
class FACEMODELING : public QMainWindow
{
Q_OBJECT
public:
	Mesh mesh;
	FACEMODELING(QWidget *parent = 0, Qt::WFlags flags = 0);
	~FACEMODELING();
public slots:
    void showFileOpenDialog(); // Load Scan Mesh 
    void animate(); // any mouse movement required 
    void keyPressEvent(QKeyEvent *event); // any special key like Q or esc 
private:
	Ui::FACEMODELINGClass ui;

};

#endif // FACEMODELING_H
