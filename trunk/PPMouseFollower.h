#ifndef PP_MOUSE_FOLLOWER_H
#define PP_MOUSE_FOLLOWER_H

#include "PPBot.h"

class PPMouseFollower : public PPBot{
private:
	PPMouseFollower(const PPMouseFollower&) {}

public:
	PPMouseFollower(void) : PPBot() {}

	virtual void setMousePosition(const QPoint& pos){
		setTarget(pos);
	}
};

#endif //PP_MOUSE_FOLLOWER_H