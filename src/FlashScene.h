#ifndef __FlashScene_H__
#define __FlashScene_H__
#include "SG_Engine.h"
using namespace SDL;

class FlashScene:public Scene
{
public:
	FlashScene();
	~FlashScene();
	CREATE_FUNC(FlashScene);
	bool init();
private:
	void preload(float dt);
};
#endif