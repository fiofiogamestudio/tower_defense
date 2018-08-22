#pragma once
#include "cocos2d.h"
#include "Pos.h"
#include "TowerBase.h"
#include "Monster.h"
#include "Tower.h"
#include "Ammo.h"

#include "Info.h"
using namespace cocos2d;

class SceneGame :public Layer {
public:
	static Scene* createScene();
	CREATE_FUNC(SceneGame);
	virtual bool init();
	void update(float dt);
public:
	TMXTiledMap * _map;

	Vector<TowerBase*> vec_towerbase;
	int index_towerbase_selected;

	Node* node_UI_tower;

	Vector<Pos*> vec_path;

	Vector<Monster*> vec_monster;

	Vector<Tower*> vec_tower;

	Vector<Ammo*> vec_ammo;

	Info* info_monster;
	Info* info_tower;
	Info* info_ammo;
public:
	void initNode();

	void initUI();

	void initTouch();

	void initMap(int i);

	int GetGridIndex(Pos* pos);
	bool IsTowerbaseSelected(int index,Point point);
	void DetectorVecTowebase(Point point);

	void LoadInfo();

	void CreateMonster(int type);
};