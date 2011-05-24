#ifndef PP_BOT_FOLLOWER_H
#define PP_BOT_FOLLOWER_H

#include "PPBot.h"

class PPBotFollower : public PPBot{
private:
	PPBot* _leader;
	PPBotFollower(const PPBotFollower&) {}

public:
	PPBotFollower(void) : PPBot(), _leader(0) {}
	PPBotFollower(PPBot* leader) : PPBot(){ _leader = leader; }
	
	void setLeader(PPBot* leader) { _leader = leader; }
	
	virtual void update(void){
		if(_leader != 0){
			setTarget(_leader->getPosition());
		}

		PPBot::update();
	}
};

#endif //PP_BOT_FOLLOWER_H