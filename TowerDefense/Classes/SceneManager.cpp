#include "SceneManager.h"

void SceneManager::ToSceneGame()
{
	auto scene = SceneGame::createScene();
	Director::getInstance()->replaceScene(scene);
}

void SceneManager::ToSceneStart()
{
	auto scene = SceneStart::createScene();
	Director::getInstance()->replaceScene(scene);
}

void SceneManager::BeginWithSceneStart()
{
	auto scene = SceneStart::createScene();
	Director::getInstance()->runWithScene(scene);
}
