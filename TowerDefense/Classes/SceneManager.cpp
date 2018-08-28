#include "SceneManager.h"

void SceneManager::ToSceneGame()
{
	auto scene = SceneGame::createScene();
	Director::getInstance()->replaceScene(TransitionFade::create(0.5, scene, Color3B(0, 0, 0)));
}

void SceneManager::ToSceneStart()
{
	auto scene = SceneStart::createScene();
	Director::getInstance()->replaceScene(TransitionFade::create(0.5, scene, Color3B(0, 0, 0)));
}

void SceneManager::BeginWithSceneStart()
{
	auto scene = SceneStart::createScene();
	Director::getInstance()->runWithScene(TransitionFade::create(0.5, scene, Color3B(0, 0, 0)));
}
