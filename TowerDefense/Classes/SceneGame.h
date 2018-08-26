#pragma once

#include <vector>

#include "cocos2d.h"
#include "Pos.h"
#include "TowerBase.h"
#include "Monster.h"
#include "Tower.h"
#include "Ammo.h"

#include "Info.h"
#include "Struct.h"

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
	Node* group_full;

	Vector<Pos*> vec_path;
	Vector<Pos*> vec_path1;

	Vector<Monster*> vec_monster;

	Vector<Tower*> vec_tower;

	Vector<Ammo*> vec_ammo;

	Info* info_wave;

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

	void CreateMonster(int type,int path);

	void CreateTower(int type);

	void CloseAllMenu();

	void OpenMenu(int index);

	void ShowTip(std::string path, float time_scaleto=0.5f, float target_scale=1.1f, float time_fadeout=1.5f);

	void ShowAnimation(std::string path,int len,float time,Size size,Vec2 pos);

	//根据Wave来创建函数
	float timer_monster = 0;
	float time_monster = 0.5f;
	std::vector<struct_monster*> vec_struct_monster;
	//返回布尔值：是否是最后一波
	bool CreateMonsterByWaveInfo(int index_wave);
	float timer_wave = 0;
	float time_wave = 5.0f;
	int index_wave = 0;
	bool is_final = false;

};