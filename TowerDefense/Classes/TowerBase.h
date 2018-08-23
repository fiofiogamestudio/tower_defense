#pragma once
#include "Entity.h"
#include "Tower.h"
class TowerBase :public Entity {
private:
	bool has_tower;
	int index_type;
	Tower* _tower;
public:
	TowerBase();
	~TowerBase();
	static TowerBase* create(Pos* pos);
	int GetTypeIndex();
	void SetTower(Tower* tower,int type);

	bool HasTower();

	void DestroyTower();
};