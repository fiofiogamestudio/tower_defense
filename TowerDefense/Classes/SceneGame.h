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
	bool has_tower_towerbase_selected;

	Node* node_UI_tower;
	Entity* panel_tower;
	Node* group_empty;
	Node* group_nempty;

	Vector<Pos*> vec_path;

	Vector<Monster*> vec_monster;

	Vector<Tower*> vec_tower;

	Vector<Ammo*> vec_ammo;

	Info* info_monster;
	Info* info_monster_file;

	Info* info_tower;
	Info* info_tower_file;

	Info* info_ammo;
	Info* info_ammo_file;

	Vector<Node*> vec_todestroy;
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

	void CreateTower(int type);

	void CloseAllMenu();

	void OpenMenu();

	void ShowTip(std::string path);

};