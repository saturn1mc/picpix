#include "PPWidget.h"

const int PPWidget::DEFAULT_WIDTH = 800;
const int PPWidget::DEFAULT_HEIGHT = 600;
const int PPWidget::DEFAULT_TIMEOUT = 5;

PPWidget::PPWidget(PPEnvironment* environment) : QWidget(0){
	_timer = new QTimer(this);
	_environment = environment;

	resize(DEFAULT_WIDTH, DEFAULT_HEIGHT);
	connect(_timer, SIGNAL(timeout()), this, SLOT(update()));
	setMouseTracking(true);
}

void PPWidget::paintEvent(QPaintEvent*){
	if(isVisible()){
		QPainter painter(this);
		painter.setBrush(Qt::white);

		_environment->update();
		_environment->draw(&painter);
	}
}

void PPWidget::mouseMoveEvent(QMouseEvent* event){
	_environment->setMousePosition(event->pos());
}

void PPWidget::update(void){
	QWidget::update();
}

void PPWidget::launchAnimation(void){
	_timer->start(DEFAULT_TIMEOUT);
}