#include <Qt\qapplication.h>
#include <Qt\qdesktopwidget.h>

#include "PPWidget.h"
#include "PPMouseFollower.h"
#include "PPBotFollower.h"

int main(int argc, char* argv[]){
	QApplication app(argc, argv);
	QPoint desktopCenter(QApplication::desktop()->availableGeometry().center());

	PPEnvironment* environment = new PPEnvironment;

	PPMouseFollower* mouseFollower = new PPMouseFollower();
	environment->addBot(mouseFollower);

	PPBotFollower* follower = new PPBotFollower(mouseFollower);
	follower->setPosition(QPointF(800, 400));
	environment->addBot(follower);

	follower = new PPBotFollower(mouseFollower);
	follower->setPosition(QPointF(800, 300));
	environment->addBot(follower);

	follower = new PPBotFollower(mouseFollower);
	follower->setPosition(QPointF(800, 200));
	environment->addBot(follower);

	follower = new PPBotFollower(mouseFollower);
	follower->setPosition(QPointF(800, 100));
	environment->addBot(follower);

	follower = new PPBotFollower(mouseFollower);
	follower->setPosition(QPointF(0, 400));
	environment->addBot(follower);

	follower = new PPBotFollower(mouseFollower);
	follower->setPosition(QPointF(0, 300));
	environment->addBot(follower);

	follower = new PPBotFollower(mouseFollower);
	follower->setPosition(QPointF(0, 200));
	environment->addBot(follower);

	follower = new PPBotFollower(mouseFollower);
	follower->setPosition(QPointF(0, 100));
	environment->addBot(follower);

	PPWidget* widget = new PPWidget(environment);
	widget->launchAnimation();
	widget->show();

	widget->move(desktopCenter - QPoint(widget->DEFAULT_WIDTH/ 2, widget->DEFAULT_HEIGHT / 2));

	return app.exec();
}