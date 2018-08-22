#pragma once
#include "Entity.h"
#include "Ammo.h"
class Tower :public Entity {
private:
	int _atk;
	int _cost;
	float _cd;
	float _timer;
public:
	Vec2 vec_local;
	Vec2 vec_offset;
	Tower();
	~Tower();
	static Tower* create();
	void fire(float dt,Vec2 offset,Vec2 dir,Vector<Ammo*> &vec_ammo,Layer* scene,int order);
};
