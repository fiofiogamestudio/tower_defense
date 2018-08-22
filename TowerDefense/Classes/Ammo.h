#pragma once
#include "Entity.h"
class Ammo :public Entity {
private:
	int _damage;
	int _speed;
	Vec2 _dir;

	//是否时持续伤害
	bool is_delay;
	//是否已经碰撞
	bool is_active;
public:
	Ammo();
	~Ammo();
	static Ammo* create(Vec2 vec);
	void SetValues(int damage, int speed,Vec2 dir);
	void SetDir(Vec2 dir);
	void moveAmmo(float dt);

	//碰撞
	bool coled();
};