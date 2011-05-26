#include "PPEnvironment.h"

void PPEnvironment::setMousePosition(const QPoint& pos){
	for(QList<PPBot*>::iterator botIter = _bots->begin(); botIter != _bots->end(); botIter++){
		(*botIter)->setMousePosition(pos);
	}
}

void PPEnvironment::draw(QPainter* painter){
	for(QList<PPBot*>::iterator botIter = _bots->begin(); botIter != _bots->end(); botIter++){
		(*botIter)->draw(painter);
	}

	for(QList<PPElement*>::iterator elemIter = _elements->begin(); elemIter != _elements->end(); elemIter++){
		(*elemIter)->draw(painter);
	}
}

void PPEnvironment::update(void){
	for(QList<PPBot*>::iterator botIter = _bots->begin(); botIter != _bots->end(); botIter++){
		(*botIter)->update();
	}

	for(QList<PPElement*>::iterator elemIter = _elements->begin(); elemIter != _elements->end(); elemIter++){
		(*elemIter)->update();
	}
}