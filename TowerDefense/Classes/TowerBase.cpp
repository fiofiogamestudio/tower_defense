#include "TowerBase.h"

TowerBase::TowerBase()
{
	index_type = 0;
}

TowerBase::~TowerBase()
{
}

TowerBase * TowerBase::create(Vec2 vec, int type)
{
	TowerBase* tower = new TowerBase();
	if (tower&&tower->init(vec)) {
		tower->autorelease();
	}
	else {
		CC_SAFE_DELETE(tower);
	}
	return tower;

}

int TowerBase::GetTypeIndex()
{
	return index_type;
}
