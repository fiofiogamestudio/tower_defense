#pragma once
#include "cocos2d.h"
#include "TowerBase.h"
using namespace cocos2d;

class SceneGame :public Layer {
public:
	static Scene* createScene();
	CREATE_FUNC(SceneGame);
	virtual bool init();
private:
	TMXTiledMap * _map;

	Vector<TowerBase*> vec_towerbase;
	int index_towerbase_selected;

	Node* node_UI_tower;
public:
	void initNode();

	void initUI();

	void initTouch();

	void initMap(int i);

	int GetGridIndex(Vec2 vec);
	bool IsTowerbaseSelected(int index,Point point);
	void DetectorVecTowebase(Point point);
};