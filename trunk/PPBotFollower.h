#ifndef PP_BOT_FOLLOWER_H
#define PP_BOT_FOLLOWER_H

#include "PPBot.h"

class PPBotFollower : public PPBot{
private:
	PPBot* _leader;
	double _separation;

	PPBotFollower(const PPBotFollower&) {}

protected:
	virtual void computeCorrections(void){
		if(_leader != 0){
			if(_leader->getPersonalSpace().boundingRect().intersects(*_boundingBox)){
				//Obstacle avoidance
				(*_correction) = avoidSteering();

				if(_correction->length() == 0){
					//Avoid leader
					QVector2D sepVect((*_position) - _leader->getPosition());
					QPointF onLeaderPath(_leader->getPosition() + (_leader->getForward() * sepVect.length()).toPointF());
					QVector2D avoidLeaderVect((*_position) - onLeaderPath);
					
					(*_correction) += (avoidLeaderVect.normalized() * 20.0);
				}
			}
			else{
				PPBot::computeCorrections();
			}
		}
		else{
			PPBot::computeCorrections();
		}
	}

public:
	PPBotFollower(void) : PPBot(), _leader(0), _separation (40) {}
	PPBotFollower(PPBot* leader) : PPBot(), _separation (40) { _leader = leader; }
	
	void setLeader(PPBot* leader) { _leader = leader; }
	
	virtual void update(void){
		if(_leader != 0){

			QVector2D sepVect(_leader->getPosition().x() - _position->x(), _leader->getPosition().y() - _position->y());
			sepVect.normalize();
			sepVect *= (_separation + (_radius / 2.0));

			setTarget(QPointF(_leader->getPosition() - sepVect.toPointF()));
		}

		PPBot::update();
	}
};

#endif //PP_BOT_FOLLOWER_H