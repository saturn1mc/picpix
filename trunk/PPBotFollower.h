#ifndef PP_BOT_FOLLOWER_H
#define PP_BOT_FOLLOWER_H

#include "PPBot.h"

class PPBotFollower : public PPBot{
private:
	PPBot* _leader;
	double _separation;

	PPBotFollower(const PPBotFollower&) {}

public:
	PPBotFollower(void) : PPBot(), _leader(0), _separation (40) {}
	PPBotFollower(PPBot* leader) : PPBot(), _separation (40) { _leader = leader; }
	
	void setLeader(PPBot* leader) { _leader = leader; }
	
	virtual void update(void){
		if(_leader != 0){

			QVector2D sepVect(_leader->getPosition().x() - _position->x(), _leader->getPosition().y() - _position->y());
			sepVect.normalize();
			sepVect *= (_separation + (_radius / 2.0));

			QPointF target(_leader->getPosition() - sepVect.toPointF());

			setTarget(target);
		}

		PPBot::update();
	}
};

#endif //PP_BOT_FOLLOWER_H