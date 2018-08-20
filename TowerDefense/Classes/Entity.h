#pragma once
#include "cocos2d.h"
using namespace cocos2d;
class Entity :public Layer{
private:
	Vec2 _pos;
	Sprite* _sprite;
public:
	Entity();
	~Entity();
	static Entity* create(Vec2 vec);
	bool init(Vec2 vec);
	void BindSprite(std::string path);
	void SetSpriteSize(Size size);
	bool IsContains(Point pos);
};