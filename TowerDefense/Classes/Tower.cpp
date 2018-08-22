#include "Tower.h"

Tower::Tower()
{
	_atk = 0;
	_cd = 1;
	_cost = 100;
	_timer = 0;
}

Tower::~Tower()
{
}

Tower * Tower::create()
{
	Tower* tower = new Tower();
	if (tower&&tower->init(new Pos(0,0))) {
		tower->autorelease();
	}
	else {
		CC_SAFE_DELETE(tower);
	}
	return tower;
}

void Tower::fire(float dt,Vec2 offset,Vec2 dir,Vector<Ammo*> &vec_ammo,Layer* scene,int order)
{
	//设置offset，修正子弹发射方向
	vec_offset = offset;

	_timer += dt;
	if (_timer > _cd) {
		_timer = 0;
		//发射子弹
		{
			log("fire");
			Ammo* ammo = Ammo::create(vec_local+offset);
			ammo->BindSprite("TestSprites/ammo.png");
			ammo->SetSpriteSize(Size(10, 10));
			ammo->SetDir(dir);
			scene->addChild(ammo, order);
			vec_ammo.pushBack(ammo);
		}
		
		

	}
}
