#pragma once
#include "Entity.h"
class Ammo :public Entity {
private:
	int _damage;
	int _speed;
	Vec2 _dir;
public:
	Ammo();
	~Ammo();
	static Ammo* create(Vec2 vec);
	void SetValues(int damage, int speed,Vec2 dir);
	void SetDir(Vec2 dir);
	void moveAmmo(float dt);
};