#include "Monster.h"

Monster::Monster()
{
}

Monster::~Monster()
{
}

Monster * Monster::createByPath(Vector<Pos*> path)
{
	Monster* monster = new Monster();
	monster->_path = path;
	if (monster&&monster->init(path.at(0))) {
		monster->autorelease();
	}
	else {
		CC_SAFE_DELETE(monster);
	}
	return monster;
}
