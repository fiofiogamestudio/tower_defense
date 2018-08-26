#include "SceneStart.h"
#include "ui/CocosGUI.h"
using namespace ui;

#define STAR_Z (1)
#define METEORB_Z (5)
#define UI_Z (8)
#define METEORF_Z (10)
#define TITLE_Z (15)

#define STAR_P ("UI/star0.png")
#define STAR_N ("UI/star")

#define PLANET_P ("UI/planet.png")
#define STATION_P ("UI/station.png")
#define TITLE_P ("UI/title.png")

//流星的plist路径
#define METEOR_P ("Particle/meteor.plist")
//流星的材质路径(好像加了没有什么用)
#define METEORT_P ("Particle/meteor.png")

Scene * SceneStart::createScene()
{
	auto scene = Scene::create();
	auto layer = SceneStart::create();
	scene->addChild(layer);
	return scene;
}

bool SceneStart::init()
{
	if (!Layer::init()) {
		return false;
	}
	//修正屏幕
	this->setPosition(Vec2(40,-20));
	this->setScale(1.1f);

	//初始化随机数种子
	{
		struct timeval now;
		gettimeofday(&now, NULL);
		srand((unsigned int)(now.tv_sec * 1000 + now.tv_usec / 1000));
	}
	//初始化UI
	initUI();
	//初始化点击
	initTouch();
	//初始化计时器
	{
		timer_star = 0;
		time_star1 = 1.0f;
		timer_star = 0;
		time_star1 = 2.0f;
		timer_star2 = 0;
		time_star2 = 3.0f;
		timer_meteor = 0;
		time_meteor = 10.0f;
		timer_meteor1 = 0;
		time_meteor1 = 20.0f;

		can_click = true;
		timer_click = 0;
		time_click = 0;
	}

	//测试生成星星
	//ShowStar(Vec2(100,50));
	
	//测试流星粒子特效
	ShowMeteor();
	ShowMeteor();
	

	//绑定update函数
	this->schedule(schedule_selector(SceneStart::update));
}

void SceneStart::initUI()
{
	node_UI = Node::create();
	this->addChild(node_UI,UI_Z);
	//显示标题
	{
		Vec2 pos_title = Vec2(850, 550);

		_title = Sprite::create(TITLE_P);
		//title->setPosition(pos_title);
		//下面添加到clip节点下了
		//this->addChild(title, TITLE_Z);

		//标题添加闪光特效
		Size size_clip =_title->getContentSize();
		Sprite* spark = Sprite::create("UI/spark.png");
		spark->setPosition(-size_clip.width, 0);
		ClippingNode* clip = ClippingNode::create();
		clip->setPosition(_title->getPosition());
		clip->setAlphaThreshold(0.05f);
		clip->setContentSize(size_clip);
		clip->setStencil(_title);

		//标题在最上层
		this->addChild(clip, TITLE_Z);
		clip->addChild(_title, 1);
		clip->addChild(spark, 2);

		clip->setPosition(pos_title);
		
		auto moveto = MoveTo::create(2.5f, Vec2(size_clip.width, 0));
		auto moveback = MoveTo::create(2.5f, Vec2(-size_clip.width, 0));
		spark->runAction(RepeatForever::create(Sequence::create(moveto, moveback, nullptr, nullptr)));
	}
	//显示星球
	{
		Button* planet = Button::create(PLANET_P, PLANET_P, PLANET_P);
		planet->addTouchEventListener([&](Ref* sender, Widget::TouchEventType type) {
			if (type == ui::Widget::TouchEventType::ENDED) {
				//log("click planet!");
				if(can_click)
				OpenPlanet();
			}
		});
		planet->setPosition(Vec2(350, 400));
		node_UI->addChild(planet);
		//为星球添加缩放动画
		auto scaleup = ScaleTo::create(0.8f, 1.01f);
		auto scaledown = ScaleTo::create(0.8f, 1.0f);
		auto seq = Sequence::create(scaleup, scaledown,nullptr,nullptr);
		auto repeat = RepeatForever::create(seq);
		planet->runAction(repeat);
		//添加点击星球后的UI
		{
			node_menu_level = Node::create();
			planet->addChild(node_menu_level);

		}
	}
	//显示空间站
	{
		Button* station = Button::create(STATION_P, STATION_P, STATION_P);
		station->addTouchEventListener([&](Ref* sender, Widget::TouchEventType type) {
			if (type == ui::Widget::TouchEventType::ENDED) {
				//log("click station!");
				if(can_click)
				OpenStation();
			}
		});
		station->setPosition(Vec2(850, 300));
		node_UI->addChild(station);
		//为空间站添加悬浮动画
		auto moveup = MoveBy::create(0.5f, Vec2(0, 5));
		auto movedown = MoveBy::create(0.5f, Vec2(0, -5));
		auto seq = Sequence::create(moveup, movedown,nullptr,nullptr);
		auto repeat = RepeatForever::create(seq);
		station->runAction(repeat);
	}

}

void SceneStart::initTouch()
{
	auto listener = EventListenerTouchOneByOne::create();
	listener->onTouchBegan = [](Touch* touch, Event* event) {
		//点击/触摸开始时
		return true;
	};
	listener->onTouchMoved = [](Touch* touch, Event* event) {
		//点击/触摸点移动时
		return true;
	};
	listener->onTouchEnded = [&](Touch* touch, Event* event) {
		//点击/触摸结束时
		Point pos = touch->getLocationInView();
		Point point = Director::getInstance()->convertToGL(pos);
		//log("pos%f,%f", pos.x, pos.y);
		//log("point%f,%f", point.x, point.y);
		if(can_click)
		ReturnToSpace();
		return true;
	};
	listener->setSwallowTouches(true);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);

}

void SceneStart::update(float dt)
{
	if (can_click) {
		log("a");
	}
	else {
		log("b");
	}
	//根据计时器来随机生成星星
	{
		CreateStarByRandom(dt);
		CreateStarByRandom1(dt);
		CreateStarByRandom2(dt);;
	}
	//根据计时器来随机生成流星
	{
		CreateMeteorByRandom(dt);
		CreateMeteorByRandom1(dt);
	}
	//销毁被手动加入销毁列表的物体
	{
		//先执行计时器
		for (int i = 0; i < vec_todestroy.size(); i++) {
			vec_todestroy.at(i)->updateLife(dt);
		}
		//然后执行销毁
		for (int i = 0; i < vec_todestroy.size(); i++) {
			if (!vec_todestroy.at(i)->GetActive()) {
				Entity* temp = vec_todestroy.at(i);
				vec_todestroy.erase(vec_todestroy.begin() + i);
				temp->removeFromParent();
			}
		}
		//log("size of vec_todestroy is %d", vec_todestroy.size());
	}

	//刷新点击计时器
	updateClick(dt);
}

void SceneStart::ShowStar(Vec2 pos)
{
	Entity* star = Entity::create(new Pos(0, 0));
	star->setPosition(pos);

	//随机缩放值
	float scale = rand_0_1() + 1.0f;

	star->setScale(scale);
	star->BindSprite(STAR_P);

	//随机播放速度
	float time = (rand_0_1() + 1.0f) / 20.0f;

	star->BindAnimation(STAR_N, 6, time, Size(10,10));
	star->SetAnimationPlay(true, true);
	this->addChild(star,STAR_Z);
	//手动销毁Entity
	vec_todestroy.pushBack(star);
	star->Destroy(3.0f);
}

void SceneStart::ShowMeteor()
{
	auto meteor = ParticleSystemQuad::create(METEOR_P);
	meteor->setTextureWithRect(Director::getInstance()->getTextureCache()->addImage(METEORT_P), Rect(0, 0, 10, 10));
	meteor->setBlendAdditive(true);
	meteor->setAutoRemoveOnFinish(true);
	meteor->setPosition(Vec2(1280+640*rand_0_1(),720+360*rand_0_1()));

	//初始化随机数种子(测试用)
	/*{
		struct timeval now;
		gettimeofday(&now, NULL);
		srand((unsigned int)(now.tv_sec * 1000 + now.tv_usec / 1000));
	}*/

	//随机图层
	float rz = rand_0_1();
	if (rz < 0.5f) {
		this->addChild(meteor, METEORF_Z);
	}
	else {
		this->addChild(meteor, METEORB_Z);
	}

	//下面的数值都是乱调的
	

	//随机生命周期
	float time_life =3*( 1.0f+rand_0_1() * 9);
	meteor->setDuration(time_life);


	//随机大小,与生命周期成正比
	meteor->setScale(0.003f*time_life);

	//随机旋转方向，在0到90度之间
	Size size = Director::getInstance()->getVisibleSize();
	float x = -size.width*2-rand_0_1()*500.0f;
	float y = -size.height*2-rand_0_1()*500.0f;
	float rotation = MATH_RAD_TO_DEG(atan(x/y));
	//log("x: %f,y: %f,rotation: %f",x,y, rotation);
	meteor->setRotation(rotation);
	Vec2 dir = Vec2(x,y);

	
	auto moveby = MoveBy::create(10.0f+time_life/3, dir);
	meteor->runAction(moveby);

}

void SceneStart::CreateStarByRandom(float dt)
{
	timer_star += dt;
	if (timer_star > time_star) {
		timer_star = 0;
		time_star = 1.5f + rand_0_1();
		int num = (int)(2.5f + rand_0_1()*3);
		for (int i = 0; i < num; i++) {
			float posx = Director::getInstance()->getVisibleSize().width*rand_0_1();
			float posy = Director::getInstance()->getVisibleSize().height*rand_0_1();
			ShowStar(Vec2(posx, posy));
			//log("create star!");
		}
	}
}

void SceneStart::CreateStarByRandom1(float dt)
{
	timer_star1 += dt;
	if (timer_star1 > time_star1) {
		timer_star1 = 0;
		time_star1 = 2.5f + rand_0_1();
		int num = (int)(1.5f + rand_0_1());
		for (int i = 0; i < num; i++) {
			float posx = Director::getInstance()->getVisibleSize().width*rand_0_1();
			float posy = Director::getInstance()->getVisibleSize().height*rand_0_1();
			ShowStar(Vec2(posx, posy));
			//log("create star1!");
		}
	}
}

void SceneStart::CreateStarByRandom2(float dt)
{
	timer_star2 += dt;
	if (timer_star2 > time_star2) {
		timer_star2 = 0;
		time_star2 = 3.5f + rand_0_1() * 2;
		int num = (int)(1.5f + rand_0_1());
		for (int i = 0; i < num; i++) {
			float posx = Director::getInstance()->getVisibleSize().width*rand_0_1();
			float posy = Director::getInstance()->getVisibleSize().height*rand_0_1();
			ShowStar(Vec2(posx, posy));
			//log("create star1!");
		}
	}
}

void SceneStart::CreateMeteorByRandom(float dt)
{
	timer_meteor += dt;
	if (timer_meteor > time_meteor) {
		timer_meteor = 0;
		time_meteor = 10.0f + rand_0_1()*5.0f;
		ShowMeteor();
	}
}

void SceneStart::CreateMeteorByRandom1(float dt)
{
	timer_meteor1 += dt;
	if (timer_meteor1 > time_meteor1) {
		timer_meteor1 = 0;
		time_meteor1 = 20.0f + rand_0_1()*10.0f;
		ShowMeteor();
	}
}

void SceneStart::OpenPlanet()
{
	auto scaleto = ScaleTo::create(0.8f, 1.45f);
	auto moveto = MoveTo::create(0.8f, Vec2(280, -205));
	auto spawn = Spawn::create(scaleto, moveto, nullptr, nullptr);
	node_UI->runAction(spawn);

	auto fadeout = FadeOut::create(0.3f);
	_title->runAction(fadeout);

	DisableClick(0.8f);
}

void SceneStart::OpenStation()
{
	auto scaleto = ScaleTo::create(0.8f, 1.5f);
	auto moveto = MoveTo::create(0.8f, Vec2(-850, -100));
	auto spawn = Spawn::create(scaleto, moveto, nullptr, nullptr);
	node_UI->runAction(spawn);

	auto fadeout = FadeOut::create(0.3f);
	_title->runAction(fadeout);

	DisableClick(0.8f);
}

void SceneStart::ReturnToSpace()
{
	log("return!");
	auto scaleto = ScaleTo::create(0.8f, 1.0f);
	auto moveto = MoveTo::create(0.8f, Vec2(0, 0));
	auto spawn = Spawn::create(scaleto, moveto, nullptr, nullptr);
	node_UI->runAction(spawn);

	auto fadein = FadeIn::create(0.3f);
	_title->runAction(fadein);

	DisableClick(0.8f);
}

void SceneStart::updateClick(float dt)
{
	if (can_click)return;
	timer_click += dt;
	if (timer_click > time_click) {
		can_click = true;
	}
}

void SceneStart::DisableClick(float time)
{
	can_click = false;
	timer_click = 0;
	time_click = time;
}



