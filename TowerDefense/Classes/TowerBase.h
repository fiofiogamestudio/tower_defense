#pragma once
#include "Entity.h"
#include "Tower.h"
class TowerBase :public Entity {
private:
	int index_type;
	Tower* _tower;
public:
	TowerBase();
	~TowerBase();
	static TowerBase* create(Pos* pos, int type);
	int GetTypeIndex();
	void SetTower(Tower* tower);
};