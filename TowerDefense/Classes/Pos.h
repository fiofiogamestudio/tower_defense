#pragma once
#include "cocos2d.h"
using namespace cocos2d;
class Pos :public Node{
private:
	int posx;
	int posy;
public:
	Pos(int x, int y);
	~Pos();
	static Pos* createPos(float x, float y);
	int GetPosx();
	int GetPosy();
};