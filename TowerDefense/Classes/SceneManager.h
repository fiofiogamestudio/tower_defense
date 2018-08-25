#pragma once
#include "cocos2d.h"
#include "SceneGame.h"
#include "SceneStart.h"

using namespace cocos2d;
class SceneManager {
public:
	static void ToSceneGame();
	static void ToSceneStart();
	static void BeginWithSceneStart();
};