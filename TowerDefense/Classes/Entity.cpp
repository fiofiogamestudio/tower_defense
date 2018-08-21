#include "Entity.h"

Entity::Entity()
{
	_pos = Vec2(0, 0);
}

Entity::~Entity()
{
}

Entity * Entity::create(Pos* pos)
{
	Entity* entity = new Entity();
	if (entity&&entity->init(pos)) {
		entity->autorelease();
	}
	else {
		CC_SAFE_DELETE(entity);
	}
	return entity;
	
}

bool Entity::init(Pos* pos)
{
	Vec2 vec = Vec2(pos->GetPosx() * 40, pos->GetPosy() * 40);
	setPosition(vec);
	return true;
}

void Entity::BindSprite(std::string path)
{
	_sprite = Sprite::create(path);
	_sprite->setPosition(_pos);
	_sprite->setContentSize(Size(80, 80));
	this->addChild(_sprite);
}

void Entity::BindAnimation(std::string name, int size,float time)
{
	Vector<SpriteFrame*> vec_frame;
	for (int i = 0; i < size; i++) {
		//这里name必须设置成c_str()，不然读取会出现乱码
		std::string path = String::createWithFormat("%s%d.png", name.c_str(), i)->getCString();
		SpriteFrame* frame = SpriteFrame::create(path,Rect(0,0,80,80));
		vec_frame.pushBack(frame);
	}
	Animation* animation = Animation::createWithSpriteFrames(vec_frame,time);
	_anim = Animate::create(animation);
}

void Entity::SetAnimationPlay(bool play)
{
	if (_anim == NULL) {
		log("not bind animation yet!");
		return;
	}
	if (play);
	_sprite->runAction(RepeatForever::create(_anim));
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

void Entity::move(Vec2 dir, float dt)
{
	Vec2 pos = this->getPosition();
	pos += dir * dt;
	this->setPosition(pos);
}
