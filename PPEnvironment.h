#ifndef PP_ENVIRONMENT_H
#define PP_ENVIRONMENT_H

#include <Qt\qlist.h>
#include <Qt\qpainter.h>

#include "PPBot.h"
#include "PPElement.h"

class PPEnvironment{
private:
	QList<PPBot*>* _bots;
	QList<PPElement*>* _elements;
	PPEnvironment(const PPEnvironment&) {}

public:
	PPEnvironment(void) {
		_bots = new QList<PPBot*>();
		_elements = new QList<PPElement*>();
	}

	virtual ~PPEnvironment(void){
		//Delete bots
		for(QList<PPBot*>::iterator botIter = _bots->begin(); botIter != _bots->end(); botIter++){
			delete(*botIter);
		}

		_bots->clear();

		delete(_bots);

		//Delete elements
		for(QList<PPElement*>::iterator elemIter = _elements->begin(); elemIter != _elements->end(); elemIter++){
			delete(*elemIter);
		}

		_elements->clear();

		delete(_elements);
	}

	void addBot(PPBot* bot){ 
		_bots->append(bot); 
		bot->setEnvironment(this);
	}

	const QList<PPBot*>* getBots(void) const{
		return _bots;
	}

	void addElement(PPElement* element){ 
		_elements->append(element);
	}

	const QList<PPElement*>* getElements(void) const{
		return _elements;
	}

	void setMousePosition(const QPoint& pos);

	void draw(QPainter* painter);
	void update(void);
};

#endif //PP_ENVIRONMENT_H