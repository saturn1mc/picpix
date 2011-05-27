#ifndef PP_MOUSE_FOLLOWER_H
#define PP_MOUSE_FOLLOWER_H

#include "PPBot.h"

class PPMouseFollower : public PPBot{
private:
	PPMouseFollower(const PPMouseFollower&) {}

protected:
	virtual void updatePersonalSpace(void){
		_personalSpace->clear();
		(*_personalSpace) << QPointF((*_position) + ((*_side) * _radius * 10.0).toPointF() - ((*_forward) * _radius * 2.0).toPointF());
		(*_personalSpace) << QPointF((*_position) - ((*_side) * _radius * 10.0).toPointF() - ((*_forward) * _radius * 2.0).toPointF());
		(*_personalSpace) << QPointF((*_position) - ((*_side) * _radius * 10.0).toPointF() + ((*_forward) * _radius * 20.0).toPointF());
		(*_personalSpace) << QPointF((*_position) + ((*_side) * _radius * 10.0).toPointF() + ((*_forward) * _radius * 20.0).toPointF());
	}

public:
	PPMouseFollower(void) : PPBot() {}

	virtual void setMousePosition(const QPoint& pos){
		setTarget(pos);
	}

	virtual void draw(QPainter* painter) const{
		PPBot::draw(painter);
	}
};

#endif //PP_MOUSE_FOLLOWER_H