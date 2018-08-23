#include "Tower.h"

Tower::Tower()
{
	_cd = 2;
	_cost = 100;
	_timer = 0;
	vec_offset_fire = Vec2(0, 0);
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

void Tower::fire(float dt,Vec2 dir,Vector<Ammo*> &vec_ammo,Layer* scene,int order)
{
	_timer += dt;
	if (_timer > _cd) {
		_timer = 0;
		//发射子弹
		{
			//载入数据
			//log("size of vv_ammo: %d;size of ammo_fire: %d", vv_ammo.size(), vv_ammo_file.size());
			Size size = Size(vv_ammo_file.at(0).asInt(), vv_ammo_file.at(1).asInt());
			std::string path_sprite = vv_ammo_file.at(2).asString();
			//log("fire");
			Ammo* ammo = Ammo::create(vec_local+vec_offset_fire);
			ammo->BindSprite(path_sprite);
			ammo->SetSpriteSize(size);
			ammo->SetDir(dir);
			scene->addChild(ammo, order);
			vec_ammo.pushBack(ammo);

			//如果有动画则绑定动画
			bool flag = vv_ammo_file.at(3).asBool();
			if (flag) {
				int len_animation = vv_ammo_file.at(4).asInt();
				float inter_animation = (float)vv_ammo_file.at(5).asInt()/1000.0f;
				std::string name_animation = vv_ammo_file.at(6).asString();
				ammo->BindAnimation(name_animation, len_animation, inter_animation,size);
				ammo->SetAnimationPlay(true);
			}

			//设置子弹数值
			ammo->SetValuesByInfo(vv_ammo);
		}
		
		

	}
}

void Tower::BindAmmoByInfo(ValueVector vv,ValueVector vv_file)
{
	vv_ammo = vv;
	vv_ammo_file = vv_file;
}

//攻击 cd 花费
void Tower::SetValuesByInfo(ValueVector vv)
{
	float cd = (float)vv.at(0).asInt() / 1000.0f;
	int cost = vv.at(1).asInt();
	Vec2 offset = Vec2(vv.at(2).asInt(), vv.at(3).asInt());
	_cd = cd;
	_cost = cost;
	vec_offset_fire = offset;
}
