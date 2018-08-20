#pragma once
#include "cocos2d.h"
using namespace cocos2d;
class SceneStart :public Layer {
public:
	Scene * createScene();
	CREATE_FUNC(SceneStart);
	virtual bool init();
};