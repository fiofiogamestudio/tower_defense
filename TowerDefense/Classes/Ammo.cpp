#include "Ammo.h"
#define LIFE_MAX (15)

Ammo::Ammo()
{
	_damage = 10;
	_speed = 100;
	_dir = Vec2(1, 0);
	is_delay = false;

	type_extra = 0;
	//启用计时器
	timer_life = 0;
	time_life = LIFE_MAX;

	//对于子弹种类是波的处理
	is_wave = false;
	range_wave = 0;
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

void Ammo::SetDir(Vec2 dir)
{
	_dir = dir;
}

void Ammo::updateAmmo(float dt)
{
	Vec2 dir = _dir;
	dir.normalize();
	dir *= _speed;
	move(dir, dt);
	//15s之后自动销毁
	timer_life += dt;
	if (timer_life > (float)LIFE_MAX||timer_life>time_life) {
		SetActive(false);
	}
}

bool Ammo::coled()
{
	bool flag = GetActive();
	if (!is_delay) {
		if (GetActive()) {
			SetActive(false);
		}
	}
	return flag;
}


int Ammo::GetDamage()
{
	if (!is_delay) {
		return _damage;
	}
	else {
		float t = rand_0_1();
		if (t < (float)1/60.0f) {
			_damage -= 1;
			if (_damage <= 0)_damage = 1;
			return _damage;
		}
		else {
			return 0;
		}
	}
}

void Ammo::SetValuesByInfo(ValueVector vv)
{
	bool flag = vv.at(0).asBool();
	int damage = vv.at(1).asInt();
	int speed = vv.at(2).asInt();
	is_delay = flag;
	_damage = damage;
	_speed = speed;
	type_extra = vv.at(3).asInt();
	time_life = vv.at(4).asInt();
	is_wave = vv.at(5).asBool();
	range_wave = vv.at(6).asInt();
}

int Ammo::GetExtraType()
{
	return type_extra;
}

bool Ammo::IsWave()
{
	return is_wave;
}

int Ammo::GetWaveRange()
{
	return range_wave;
}

bool Ammo::IsDelay()
{
	return is_delay;
}

void Ammo::AddDamage(int i)
{
	_damage -= i;
	if (_damage <= 0) {
		SetActive(false);
	}
}
