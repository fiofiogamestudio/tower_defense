#pragma once
#include "Entity.h"
#include "Pos.h"
class Monster :public Entity {
private:
	int _hp;
	int _speed;
	Vector<Pos*> _path;
	int index_target;

	//ÑªÌõ
	Sprite* sprite_hp;
	Sprite* sprite_hp_bg;
public:
	Monster();
	~Monster();
	static Monster* createByPath(Vector<Pos*> path);
	void moveByPath(float dt);

	//°ó¶¨ÑªÌõ
	void BindHp(std::string path,std::string path1);
};