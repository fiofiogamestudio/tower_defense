#pragma once
#include "cocos2d.h"

#include "Pos.h"
using namespace cocos2d;
class Entity :public Layer{
private:
	Vec2 _pos;
	Sprite* _sprite;
	Animate* _anim;
public:
	Entity();
	~Entity();
	static Entity* create(Pos* pos);
	bool init(Pos* pos);
	void BindSprite(std::string path);
	//要先绑定Sprite之后再绑定Animation，不然动画会显示不出来
	void BindAnimation(std::string name, int size,float time);
	//绑定动画之后需要播放
	void SetAnimationPlay(bool play);
	void SetSpriteSize(Size size);
	bool IsContains(Point pos);
};