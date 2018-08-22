#include "Monster.h"
#define HP_S (80);

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
	monster->index_target = 1;
	if (monster&&monster->init(path.at(0))) {
		monster->autorelease();
	}
	else {
		CC_SAFE_DELETE(monster);
	}
	return monster;
}

void Monster::moveByPath(float dt)
{
	Pos* pos_target = _path.at(index_target);
	Vec2 vec_target = Vec2(pos_target->GetPosx() * 40, pos_target->GetPosy() * 40);
	Vec2 vec_current = this->getPosition();
	float distance = vec_target.distance(vec_current);
	if (distance > 1.0f) {
		Vec2 dir = Vec2(vec_current, vec_target);
		dir.normalize();
		dir *= 50;
		move(dir,dt);
	}
	else {
		if (index_target < _path.size() - 1) {
			index_target++;
		}
	}
}

void Monster::BindHp(std::string path,std::string path1)
{
	Vec2 offset = Vec2(5, 50);
	//ÑªÌõ
	sprite_hp = Sprite::create(path);
	this->addChild(sprite_hp);
	sprite_hp->setPosition(offset);
	//ÑªÌõ±³¾°
	sprite_hp_bg = Sprite::create(path1);
	this->addChild(sprite_hp_bg);
	sprite_hp_bg->setPosition(offset);
}
