#ifndef PP_ENVIRONMENT_H
#define PP_ENVIRONMENT_H

#include <Qt\qlist.h>
#include <Qt\qpainter.h>

#include "PPObject.h"

class PPEnvironment{
private:
	QList<PPObject*> _objects;

	PPEnvironment(const PPEnvironment&) {}

public:
	PPEnvironment(void) {}

	virtual ~PPEnvironment(void){
		for(QList<PPObject*>::iterator objIter = _objects.begin(); objIter != _objects.end(); objIter++){
			delete(*objIter);
		}
	}

	void addObject(PPObject* object){ _objects.append(object); }
	void setMousePosition(const QPoint& pos);

	void draw(QPainter* painter);
	void update(void);
};

#endif //PP_ENVIRONMENT_H