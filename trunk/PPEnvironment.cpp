#include "PPEnvironment.h"

void PPEnvironment::setMousePosition(const QPoint& pos){
	for(QList<PPObject*>::iterator objIter = _objects.begin(); objIter != _objects.end(); objIter++){
		(*objIter)->setMousePosition(pos);
	}
}

void PPEnvironment::draw(QPainter* painter){
	for(QList<PPObject*>::iterator objIter = _objects.begin(); objIter != _objects.end(); objIter++){
		(*objIter)->draw(painter);
	}
}

void PPEnvironment::update(void){
	for(QList<PPObject*>::iterator objIter = _objects.begin(); objIter != _objects.end(); objIter++){
		(*objIter)->update();
	}
}