#include <Qt\qapplication.h>
#include <Qt\qdesktopwidget.h>

#include "PPWidget.h"

int main(int argc, char* argv[]){
	QApplication app(argc, argv);
	QPoint desktopCenter(QApplication::desktop()->availableGeometry().center());

	PPObject* object = new PPObject();
	
	PPEnvironment* environment = new PPEnvironment;
	environment->addObject(object);

	PPWidget* widget = new PPWidget(environment);
	widget->launchAnimation();
	widget->show();

	widget->move(desktopCenter - QPoint(widget->DEFAULT_WIDTH/ 2, widget->DEFAULT_HEIGHT / 2));

	return app.exec();
}