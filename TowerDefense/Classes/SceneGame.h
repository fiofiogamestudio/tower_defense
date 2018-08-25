#pragma once
#include "cocos2d.h"
#include "Pos.h"
#include "TowerBase.h"
#include "Monster.h"
#include "Tower.h"
#include "Ammo.h"

#include "Info.h"

#include "DateManager.h"
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
	Node* group_nempty1;
	Node* group_nempty2;

	Vector<Pos*> vec_path;
	Vector<Pos*> vec_path1;

	Vector<Monster*> vec_monster;

	Vector<Tower*> vec_tower;

	Vector<Ammo*> vec_ammo;

	Info* info_monster;
	Info* info_monster_file;

	Info* info_tower;
	Info* info_tower_file;

	Info* info_ammo;
	Info* info_ammo_file;

	Vector<Entity*> vec_todestroy;

	Sprite* _mask;
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

	void OpenMenu(int index);

	void ShowTip(std::string path);

	void ShowAnimation(std::string path,int len,float time,Size size,Vec2 pos);

};