#pragma once
#include "Entity.h"
class TowerBase :public Entity {
private:
	int index_type;
public:
	TowerBase();
	~TowerBase();
	static TowerBase* create(Pos* pos, int type);
	int GetTypeIndex();
};