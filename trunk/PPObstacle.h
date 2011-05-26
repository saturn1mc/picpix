#ifndef PP_OBSTACLE_H
#define PP_OBSTACLE_H

#include "PPElement.h"

class PPObstacle : public PPElement{
private:
	PPObstacle(const PPObstacle&) {}

public:
	PPObstacle(void) : PPElement() {}
	
	virtual bool shouldAvoid(void) const { return true; }
	virtual bool shouldSeek(void) const { return false; }
	virtual bool shouldFollow(void) const { return false; }

	virtual void draw(QPainter* painter){ 
		PPElement::draw(painter);
	}

	virtual void update(void){
		PPElement::update();
	}
};

#endif //PP_OBSTACLE_H