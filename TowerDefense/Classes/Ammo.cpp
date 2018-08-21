#include "Ammo.h"

Ammo::Ammo()
{
	_damage = 10;
	_speed = 100;
	_dir = Vec2(1, 0);
}

Ammo::~Ammo()
{
}

Ammo * Ammo::create(Vec2 vec)
{
	Ammo* ammo = new Ammo();
	ammo->setPosition(vec);
	return ammo;
}

void Ammo::SetValues(int damage, int speed,Vec2 dir)
{
	_damage = damage;
	_speed = speed;
	_dir = dir;
}

void Ammo::SetDir(Vec2 dir)
{
	_dir = dir;
}

void Ammo::moveAmmo(float dt)
{
	Vec2 dir = _dir;
	dir.normalize();
	dir *= _speed;
	move(dir, dt);
}
