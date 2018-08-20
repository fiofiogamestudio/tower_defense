#include "SceneStart.h"

Scene * SceneStart::createScene()
{
	auto scene = Scene::create();
	auto layer = SceneStart::create();
	scene->addChild(layer);
	return scene;
}

bool SceneStart::init()
{
	if (!Layer::init()) {
		return false;
	}
}
