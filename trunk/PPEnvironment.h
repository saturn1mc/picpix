#ifndef PP_ENVIRONMENT_H
#define PP_ENVIRONMENT_H

#include <Qt\qlist.h>
#include <Qt\qpainter.h>

#include "PPBot.h"

class PPEnvironment{
private:
	QList<PPBot*>* _bots;
	PPEnvironment(const PPEnvironment&) {}

public:
	PPEnvironment(void) {
		_bots = new QList<PPBot*>();
	}

	virtual ~PPEnvironment(void){
		for(QList<PPBot*>::iterator botIter = _bots->begin(); botIter != _bots->end(); botIter++){
			delete(*botIter);
		}

		_bots->clear();

		delete(_bots);
	}

	void addBot(PPBot* bot){ 
		_bots->append(bot); 
		bot->setEnvironment(this);
	}

	const QList<PPBot*>* getBots(void) const{
		return _bots;
	}

	void setMousePosition(const QPoint& pos);

	void draw(QPainter* painter);
	void update(void);
};

#endif //PP_ENVIRONMENT_H