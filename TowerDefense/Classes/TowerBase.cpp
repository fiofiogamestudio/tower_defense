#include "TowerBase.h"

TowerBase::TowerBase()
{
	index_type = 0;
}

TowerBase::~TowerBase()
{
}

TowerBase * TowerBase::create(Pos* pos, int type)
{
	TowerBase* tower = new TowerBase();
	if (tower&&tower->init(pos)) {
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

void TowerBase::SetTower(Tower * tower)
{
	_tower = tower;
	_tower->vec_local = getPosition();
	this->addChild(_tower);
	_tower->setPosition(Vec2(0, 0));
	
}

