#include <Qt\qapplication.h>
#include <Qt\qdesktopwidget.h>

#include "PPWidget.h"
#include "PPMouseFollower.h"
#include "PPBotFollower.h"
#include "PPObstacle.h"

int main(int argc, char* argv[]){
	QApplication app(argc, argv);
	QPoint desktopCenter(QApplication::desktop()->availableGeometry().center());

	PPEnvironment* environment = new PPEnvironment;

	//Obstacle 1
	PPObstacle* obstacle = new PPObstacle();
	obstacle->setShape(new QPolygon(QRect(0, 0, 375, 200), true));
	
	for(int x = 0; x < 375; x += 15){
		for(int y = 0; y < 200; y += 20){
			if(y == 0 || x == 0 || y == 180 || x == 360){
				obstacle->addBound(new QRect(x,y,15,20));
			}
		}
	}

	environment->addElement(obstacle);

	//Obstacle 2
	obstacle = new PPObstacle();
	obstacle->setShape(new QPolygon(QRect(425, 0, 375, 200), true));
	
	for(int x = 425; x < 800; x += 15){
		for(int y = 0; y < 200; y += 20){
			if(y == 0 || x == 425 || y == 180 || x == 785){
				obstacle->addBound(new QRect(x,y,15,20));
			}
		}
	}

	environment->addElement(obstacle);


	PPBot* bot = new PPBot();
	bot->setPosition(QPoint(400, 300));
	bot->setTarget(QPoint(400, 0));
	environment->addBot(bot);

	for(int i =0; i < 20; i++){
		for(int j = 0; j < 2; j++){
			PPBotFollower* follower = new PPBotFollower(bot);
			environment->addBot(follower);
			follower->setPosition(QPoint(0 + i*15*2, 500 + (j*100)));
		}
	}

	PPWidget* widget = new PPWidget(environment);
	widget->launchAnimation();
	widget->show();

	widget->move(desktopCenter - QPoint(widget->DEFAULT_WIDTH/ 2, widget->DEFAULT_HEIGHT / 2));

	return app.exec();
}