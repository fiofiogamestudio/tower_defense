#include "Pos.h"

Pos::Pos(int x, int y)
{
	posx = x;
	posy = y;
}

Pos::~Pos()
{
}

Pos * Pos::createPos(float x, float y)
{
	int posx = x / 40;
	int posy = y / 40;
	return new Pos(posx,posy);
}

int Pos::GetPosx()
{
	return posx;
}

int Pos::GetPosy()
{
	return posy;
}
