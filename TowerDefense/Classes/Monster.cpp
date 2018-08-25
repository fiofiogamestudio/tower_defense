#include "Monster.h"
#define HP_S (80);

void Monster::ResizeHpSliderAndHp(int hp)
{
	Size size_hp_pre = sprite_hp->getContentSize();
	sprite_hp->setContentSize(Size(hp_current, size_hp_pre.height));
	float scale = (float)hp_current / (float)HP_S;
	sprite_hp_bg->setScaleX(scale);
}

void Monster::RefHp()
{
	
	float scale_x = (float)hp_current / (float)_hp;
	int offset = (_hp-hp_current) / 2;

	sprite_hp->setScaleX(scale_x);
	sprite_hp->setPosition(anchor_hp + Vec2(-offset, 0));
}

Monster::Monster()
{
	_speed = 100;
	_hp = 80;
	hp_current = _hp;
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
	if (distance > 5.0f) {
		Vec2 dir = Vec2(vec_current, vec_target);
		dir.normalize();
		dir *= _speed;
		move(dir,dt);
	}
	else {
		if (index_target < _path.size() - 1) {
			index_target++;
		}
	}
}

void Monster::BindHp(std::string path,std::string path1,Vec2 offset)
{
	//ÑªÌõ
	sprite_hp = Sprite::create(path);
	this->addChild(sprite_hp);
	sprite_hp->setPosition(offset);
	anchor_hp = offset;
	//ÑªÌõ±³¾°
	sprite_hp_bg = Sprite::create(path1);
	this->addChild(sprite_hp_bg);
	sprite_hp_bg->setPosition(offset);
}

void Monster::SetValuesByInfo(ValueVector vec)
{
	_speed = vec.at(0).asInt();
	_hp = vec.at(1).asInt();
	hp_current = _hp;
	//Ë¢ÐÂÑª²Û¾«Áé
	ResizeHpSliderAndHp(_hp);
}

void Monster::TakeDamage(int damage)
{
	hp_current -= damage;
	hp_current = (hp_current >= 0) ? hp_current : 0;
	RefHp();
	if (hp_current == 0) {
		SetActive(false);
	}
}
