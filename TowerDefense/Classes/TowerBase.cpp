#include "TowerBase.h"

TowerBase::TowerBase()
{
	index_type = 0;

	has_tower = false;
}

TowerBase::~TowerBase()
{
}

TowerBase * TowerBase::create(Pos* pos)
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

void TowerBase::SetTower(Tower * tower,int type)
{
	_tower = tower;
	_tower->vec_local = getPosition();
	this->addChild(_tower);
	_tower->setPosition(Vec2(0, 0));

	has_tower = true;

	index_type = type;
}

bool TowerBase::HasTower()
{
	return has_tower;
}

void TowerBase::DestroyTower()
{
	has_tower = false;
	if (_tower != NULL && _tower->GetActive()) {
		_tower->SetActive(false);
	}
}

