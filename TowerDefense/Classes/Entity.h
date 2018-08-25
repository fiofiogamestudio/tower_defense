#pragma once
#include "cocos2d.h"

#include "Pos.h"
using namespace cocos2d;
class Entity :public Layer{
private:
	Vec2 _pos;
	Sprite* _sprite;
	Animate* _anim;

	Vec2 pos_father;

	bool is_active;

	bool has_life;
	float time_life;
	float timer_life;
public:
	Entity();
	~Entity();
	static Entity* create(Pos* pos);
	bool init(Pos* pos);
	void BindSprite(std::string path);
	Sprite* GetSprite();
	//要先绑定Sprite之后再绑定Animation，不然动画会显示不出来
	void BindAnimation(std::string name, int len,float time,Size size);
	//绑定动画之后需要播放
	void SetAnimationPlay(bool play,bool once=false);
	void SetSpriteSize(Size size);
	bool IsContains(Point pos);
	//移动
	void move(Vec2 dir, float dt);
	//设置父节点的坐标
	void SetPosFather(Vec2 pos);
	//封装active
	void SetActive(bool active);
	bool GetActive();

	void updateLife(float dt);
	void Destroy(float delay);
};