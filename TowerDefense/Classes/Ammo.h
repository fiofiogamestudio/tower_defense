#pragma once
#include "Entity.h"
class Ammo :public Entity {
private:
	int _damage;
	int _speed;
	Vec2 _dir;

	//是否时持续伤害
	bool is_delay;

	//附加效果
	int type_extra;

	float timer_life;
	float time_life;
	//冲击波的效果
	bool is_wave;
	int range_wave;

public:
	Ammo();
	~Ammo();
	static Ammo* create(Vec2 vec);
	void SetDir(Vec2 dir);
	void updateAmmo(float dt);

	//碰撞
	bool coled();
	int GetDamage();

	//设置子弹数值
	void SetValuesByInfo(ValueVector vv);

	//附加效果
	int GetExtraType();
	//冲击波
	bool IsWave();
	int GetWaveRange();
};