#include <Qt\qapplication.h>
#include <Qt\qdesktopwidget.h>

#include "PPWidget.h"
#include "PPMouseFollower.h"
#include "PPBotFollower.h"

int main(int argc, char* argv[]){
	QApplication app(argc, argv);
	QPoint desktopCenter(QApplication::desktop()->availableGeometry().center());

	PPMouseFollower* mouseFollower = new PPMouseFollower();
	PPBotFollower* follower = new PPBotFollower(mouseFollower);
	follower->setPosition(QPointF(400, 400));

	PPEnvironment* environment = new PPEnvironment;
	environment->addBot(mouseFollower);
	environment->addBot(follower);

	PPWidget* widget = new PPWidget(environment);
	widget->launchAnimation();
	widget->show();

	widget->move(desktopCenter - QPoint(widget->DEFAULT_WIDTH/ 2, widget->DEFAULT_HEIGHT / 2));

	return app.exec();
}