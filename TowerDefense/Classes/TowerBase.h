#pragma once
#include "Entity.h"
class TowerBase :public Entity {
private:
	int index_type;
public:
	TowerBase();
	~TowerBase();
	static TowerBase* create(Vec2 vec, int type);
	int GetTypeIndex();
};