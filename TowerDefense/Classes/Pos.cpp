#include "Pos.h"

Pos::Pos(int x, int y)
{
	posx = x;
	posy = y;
}

Pos::Pos(float x, float y)
{
	posx = x / 40;
	posy = y / 40;
}

Pos::~Pos()
{
}

int Pos::GetPosx()
{
	return posx;
}

int Pos::GetPosy()
{
	return posy;
}
