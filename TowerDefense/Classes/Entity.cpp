#include "Entity.h"

Entity::Entity()
{
	_pos = Vec2(0, 0);
}

Entity::~Entity()
{
}

Entity * Entity::create(Vec2 vec)
{
	Entity* entity = new Entity();
	if (entity&&entity->init(vec)) {
		entity->autorelease();
	}
	else {
		CC_SAFE_DELETE(entity);
	}
	return entity;
	
}

bool Entity::init(Vec2 vec)
{
	Vec2 pos = Vec2(vec.x * 40, vec.y * 40);
	setPosition(pos);
	return true;
}

void Entity::BindSprite(std::string path)
{
	_sprite = Sprite::create(path);
	_sprite->setPosition(_pos);
	_sprite->setContentSize(Size(80, 80));
	this->addChild(_sprite);
}

void Entity::SetSpriteSize(Size size)
{
	_sprite->setContentSize(size);
}

bool Entity::IsContains(Point pos)
{
	bool flag = false;
	Vec2 anchor = getPosition();
	Vec2 size = _sprite->getContentSize();
	if (pos.x > anchor.x - size.x / 2 && pos.x < anchor.x + size.x / 2) {
		if (pos.y > anchor.y - size.y / 2 && pos.y < anchor.y + size.y / 2) {
			flag = true;
		}
	}
	//这种方法不行,因为_sprite默认坐标会在原点
	//return _sprite->getBoundingBox().containsPoint(pos);
	return flag;
}
