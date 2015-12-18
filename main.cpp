#include "facemodeling.h"
#include <QtGui/QApplication>
int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	FACEMODELING w;
	w.show();
	w.update();
	return a.exec();
}
