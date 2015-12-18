#include "facemodeling.h"
#include <QMessageBox>
 #include <QDebug>
#include <QFileDialog>
#pragma warning(push)
#pragma warning ( disable : 4005 )
FACEMODELING::FACEMODELING(QWidget *parent, Qt::WFlags flags)
	: QMainWindow(parent, flags)
{    
	ui.MainOGFrame->flag=0;
	ui.setupUi(this); // setup the GUI
}
void FACEMODELING :: showFileOpenDialog()
{
QFileDialog dialog(this); // use Qialog to open dialog box
dialog.setNameFilter(tr("Model (*.obj)")); // type of Models that accepted to load
dialog.setViewMode(QFileDialog::Detail); 
QString fileName = QFileDialog::getOpenFileName(this, tr("Select File")
,"C:/",tr("Model (*.obj)"));
QFileInfo infoF(fileName);
if (fileName.isEmpty()) // if user select nothing check if empty 
{
qDebug() <<"No file selected"; 
QMessageBox::critical( this, tr("Load Model in Pose 1"), tr("No file selected!!!") ); // notify the user that you didnt select any file to load
ui.MainOGFrame->flag=0;
}
else
{
qDebug() << "file name is" << infoF.fileName();  //just to have the file name 
//ui.widget_1->file=infoF.fileName();  //just to have the file name 
qDebug() << "file name is -------------->" << infoF.fileName();  //just to have the file name 
ui.MainOGFrame->mesh.Load(infoF.fileName());
ui.MainOGFrame->flag=1;
update();
}
}
void FACEMODELING :: animate()
{
  ui.MainOGFrame->animate();  // animate fuction in order to print timer and infroamtion and keep updating
}
FACEMODELING::~FACEMODELING()
{
}
void FACEMODELING::keyPressEvent(QKeyEvent *event)
 {
    if(event->type() == QEvent::KeyPress)
{
 
      switch(event->key())
     {
		case Qt::Key_Escape:
			{
			close();
            qApp->quit();
			}
		 case Qt::Key_Q:
			{
			close();
            qApp->quit();
		 
			}
	     }

     event->accept();
 }
    event->accept();
}
#pragma warning( pop ) 


