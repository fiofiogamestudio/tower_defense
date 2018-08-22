#pragma once
#include "Entity.h"
#include "Pos.h"
class Monster :public Entity {
private:
	int _speed;
	int _hp;
	Vector<Pos*> _path;
	int index_target;

	//血条
	Sprite* sprite_hp;
	Sprite* sprite_hp_bg;
	Vec2 anchor_hp;

	int hp_current;
	void ResizeHpSliderAndHp(int hp);
	void RefHp();
public:
	Monster();
	~Monster();
	static Monster* createByPath(Vector<Pos*> path);
	void moveByPath(float dt);

	//绑定血条
	void BindHp(std::string path,std::string path1,Vec2 offset);

	//设置参数
	void SetValuesByInfo(ValueVector vv);

	//接受伤害
	void TakeDamage(int damage);
};