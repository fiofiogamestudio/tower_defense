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

//点击星球后的一些UI的文件路径
#define ROAD_P ("UI/icon_road.png")
#define LEVELBG_P ("UI/icon_level_bg.png")
#define LEVELBG1_P ("UI/icon_level_bg1.png")
#define LEVELBG2_P ("UI/icon_level_bg2.png")
#define FLAG_P ("UI/icon_flag.png")

//点击空间站后的一些UI的文件路径
#define RECORD_P ("UI/record.png")

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

	//停止所有的音效同时播放BGM
	AudioManager::StopEffectAll();
	AudioManager::PlayBGM(SPACE_M);

	//修正屏幕
	this->setPosition(Vec2(40,-25));
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
	//退出按钮
	{
		std::string path_button = "UI/quit.png";
		button_quit = Button::create(path_button, path_button, path_button);
		button_quit->addTouchEventListener([&](Ref* sender, Widget::TouchEventType type) {
			if (type == ui::Widget::TouchEventType::ENDED) {
				if (can_click) {
					//播放点击音效
					AudioManager::PlayEffect(CLICK_M);
					//退出游戏
					DataManager::saveRecord();

					Director::getInstance()->end();
				}
			}
		});
		this->addChild(button_quit, TITLE_Z);
		button_quit->setPosition(Vec2(70, 660));
	}
	//声音控制按钮
	{
		std::string path_audio = "UI/audio.png";
		button_audio = Button::create(path_audio, path_audio, path_audio);
		button_audio->addTouchEventListener([&](Ref* sender, Widget::TouchEventType type) {
			if (type == ui::Widget::TouchEventType::ENDED) {
				if (can_click) {
					//播放点击音效
					AudioManager::PlayEffect(CLICK_M);
					
					if (DataManager::is_audio) {
						AudioManager::CloseAudio();
						sprite_audio->setVisible(true);
					}
					else {
						AudioManager::OpenAudio();
						sprite_audio->setVisible(false);
					}
				}
			}
		});
		this->addChild(button_audio, TITLE_Z);
		button_audio->setPosition(Vec2(1113, 660));

		std::string path_audio1 = "UI/audio1.png";
		sprite_audio = Sprite::create(path_audio1);
		this->addChild(sprite_audio, TITLE_Z);
		sprite_audio->setPosition(button_audio->getPosition());
		/*if (DataManager::is_audio) {
			log("is audio!");
		}
		else {
			log("is not audio!");
		}*/
		//初始化显示
		if (DataManager::is_audio) {
			sprite_audio->setVisible(false);
		}
	}
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
	//添加空间站的统计信息面板，隐藏面板及其子节点
	{
		Vec2 pos = Vec2(800, 350);
		Size size = Size(600, 500);
		_record = Sprite::create(RECORD_P);
		this->addChild(_record, TITLE_Z+1);
		_record->setContentSize(size);
		_record->setPosition(pos);

		//统计信息面板显示的数字
		{
			text_kill= Text::create(Value(DataManager::num_kill).asString(), "fonts/LuckiestGuy.ttf", 30);
			_record->addChild(text_kill);
			text_kill->setPosition(Vec2(320, 295));

			text_win=Text::create(Value(DataManager::num_win).asString(), "fonts/LuckiestGuy.ttf", 30);
			_record->addChild(text_win);
			text_win->setPosition(Vec2(320, 242));
		}

		_record->setOpacity(0);
		auto children = _record->getChildren();
		for (auto child : children) {
			child->setOpacity(0);
		}
	}
	//添加星球挑战模式的按钮
	{
		Vec2 pos = Vec2(240, 500);
		Vec2 pos1 = Vec2(240, 420);
		Vec2 pos2 = Vec2(240, 340);
		std::string path_stronger = "UI/stronger.png";
		std::string path_faster = "UI/faster.png";
		std::string path_perfect = "UI/perfect.png";
		std::string path_mode = "UI/mode.png";

		button_stronger = Button::create(path_stronger, path_stronger, path_stronger);
		this->addChild(button_stronger, TITLE_Z);
		button_stronger->setPosition(pos);
		sprite_stronger = Sprite::create(path_mode);
		this->addChild(sprite_stronger,TITLE_Z);
		sprite_stronger->setPosition(pos);
		if (DataManager::val_stronger < 1.05f) {
			sprite_stronger->setVisible(false);
		}
		button_stronger->addTouchEventListener([&](Ref* sender, Widget::TouchEventType type) {
			if (type == ui::Widget::TouchEventType::ENDED) {
				if (can_click) {
					//播放点击音效
					AudioManager::PlayEffect(CLICK_M);

					if (DataManager::val_stronger < 1.05f) {
						DataManager::val_stronger = 1.1f;
						sprite_stronger->setVisible(true);
					}
					else {
						DataManager::val_stronger = 1.0f;
						sprite_stronger->setVisible(false);
					}
				}
			}
		});
		

		button_faster = Button::create(path_faster, path_faster, path_faster);
		this->addChild(button_faster, TITLE_Z);
		button_faster->setPosition(pos1);
		sprite_faster = Sprite::create(path_mode);
		this->addChild(sprite_faster, TITLE_Z);
		sprite_faster->setPosition(pos1);
		if (DataManager::val_faster < 1.05f) {
			sprite_faster->setVisible(false);
		}
		button_faster->addTouchEventListener([&](Ref* sender, Widget::TouchEventType type) {
			if (type == ui::Widget::TouchEventType::ENDED) {
				if (can_click) {
					//播放点击音效
					AudioManager::PlayEffect(CLICK_M);

					if (DataManager::val_faster < 1.05f) {
						DataManager::val_faster = 1.1f;
						sprite_faster->setVisible(true);
					}
					else {
						DataManager::val_faster = 1.0f;
						sprite_faster->setVisible(false);
					}
				}
			}
		});

		button_perfect = Button::create(path_perfect, path_perfect, path_perfect);
		this->addChild(button_perfect, TITLE_Z);
		button_perfect->setPosition(pos2);
		sprite_perfect = Sprite::create(path_mode);
		this->addChild(sprite_perfect,TITLE_Z);
		sprite_perfect->setPosition(pos2);
		if (!DataManager::is_perfect) {
			sprite_perfect->setVisible(false);
		}
		button_perfect->addTouchEventListener([&](Ref* sender, Widget::TouchEventType type) {
			if (type == ui::Widget::TouchEventType::ENDED) {
				if (can_click) {
					//播放点击音效
					AudioManager::PlayEffect(CLICK_M);

					if (DataManager::is_perfect) {
						DataManager::is_perfect = false;
						sprite_perfect->setVisible(false);
					}
					else {
						DataManager::is_perfect = true;
						sprite_perfect->setVisible(true);
					}
				}
			}
		});

		//隐藏显示
		button_stronger->setOpacity(0);
		button_faster->setOpacity(0);
		button_perfect->setOpacity(0);
		sprite_stronger->setOpacity(0);
		sprite_faster->setOpacity(0);
		sprite_perfect->setOpacity(0);
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
		auto scaleup = ScaleTo::create(0.8f, 1.015f);
		auto scaledown = ScaleTo::create(0.8f, 1.0f);
		auto seq = Sequence::create(scaleup, scaledown,nullptr,nullptr);
		auto repeat = RepeatForever::create(seq);
		planet->runAction(repeat);
		//添加点击星球后的UI
		{
			node_menu_level = Node::create();
			planet->addChild(node_menu_level);

			Sprite* road = Sprite::create(ROAD_P);
			node_menu_level->addChild(road);
			road->setPosition(Vec2(185,225));

			std::vector<Vec2> vv_pos;
			vv_pos.push_back(Vec2(73, 168));
			vv_pos.push_back(Vec2(108, 204));
			vv_pos.push_back(Vec2(140, 184));
			vv_pos.push_back(Vec2(200, 190));
			vv_pos.push_back(Vec2(210, 230));
			vv_pos.push_back(Vec2(283, 221));
			//vv_pos.push_back(Vec2(300, 290));
			unsigned int index_button = 0;
			for (auto pos : vv_pos) {
				Button* button_level_bg = Button::create(LEVELBG_P,LEVELBG1_P,LEVELBG2_P);
				button_level_bg->_ID = index_button;
				button_level_bg->addTouchEventListener([&](Ref* sender, Widget::TouchEventType type) {
					if (type == ui::Widget::TouchEventType::ENDED) {
						if (can_click) {
							//播放点击音效
							AudioManager::PlayEffect(CLICK_M);

							log("tag is: %d", sender->_ID);
							DataManager::level_current = sender->_ID;

							SceneManager::ToSceneGame();
						}
					}
				});
				node_menu_level->addChild(button_level_bg);
				button_level_bg->setPosition(pos);
				index_button++;
			}

			Vector<Node*> children = node_menu_level->getChildren();
			for (auto child : children) {
				child->setOpacity(0);
			}
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
	//播放点击音效
	AudioManager::PlayEffect(CLICK_M);

	//放大星球
	auto scaleto = ScaleTo::create(0.8f, 1.45f);
	auto moveto = MoveTo::create(0.8f, Vec2(280, -205));
	auto spawn = Spawn::create(scaleto, moveto, nullptr, nullptr);
	node_UI->runAction(spawn);

	//隐藏标题和退出按钮和声音按钮
	auto fadeout = FadeOut::create(0.3f);
	_title->runAction(fadeout);
	button_quit->runAction(fadeout->clone());
	button_audio->runAction(fadeout->clone());
	sprite_audio->runAction(fadeout->clone());

	//显示关卡icon
	Vector<Node*> children = node_menu_level->getChildren();
	for (auto child : children) {
		log("show");
		auto fadein = FadeIn::create(1.6f);
		child->runAction(fadein);
	}

	//显示挑战模式中的面板
	auto fadein = FadeIn::create(1.0f);

	auto moveby1 = MoveBy::create(0.8f, Vec2(0, 0));
	auto fadein1 = FadeIn::create(0.3f);
	auto seq1 = Sequence::create(moveby1, fadein1, nullptr);
	button_stronger->runAction(seq1);
	button_faster->runAction(seq1->clone());
	button_perfect->runAction(seq1->clone());
	sprite_stronger->runAction(seq1->clone());
	sprite_faster->runAction(seq1->clone());
	sprite_perfect->runAction(seq1->clone());
	


	DisableClick(0.8f);
}

void SceneStart::OpenStation()
{
	//播放点击音效
	AudioManager::PlayEffect(CLICK_M);

	//放大空间站
	auto scaleto = ScaleTo::create(0.8f, 1.5f);
	auto moveto = MoveTo::create(0.8f, Vec2(-950, -120));
	auto spawn = Spawn::create(scaleto, moveto, nullptr, nullptr);
	node_UI->runAction(spawn);

	//隐藏标题和退出按钮
	auto fadeout = FadeOut::create(0.3f);
	_title->runAction(fadeout);
	button_quit->runAction(fadeout->clone());
	button_audio->runAction(fadeout->clone());
	sprite_audio->runAction(fadeout->clone());

	//显示record面板及其子节点
	auto fadein = FadeIn::create(1.0f);
	
	auto moveby1 = MoveBy::create(0.8f, Vec2(0, 0));
	auto fadein1 = FadeIn::create(0.3f);
	auto seq1 = Sequence::create(moveby1, fadein1, nullptr);
	_record->runAction(seq1);
	auto children1 = _record->getChildren();
	for (auto child1 : children1) {
		child1->runAction(seq1->clone());
	}

	DisableClick(0.8f);
}

void SceneStart::ReturnToSpace()
{
	//播放点击音效
	AudioManager::PlayEffect(CLICK_M);

	//缩小视野
	//log("return!");
	auto scaleto = ScaleTo::create(0.8f, 1.0f);
	auto moveto = MoveTo::create(0.8f, Vec2(0, 0));
	auto spawn = Spawn::create(scaleto, moveto, nullptr, nullptr);
	node_UI->runAction(spawn);

	//显示标题和退出按钮
	auto fadein = FadeIn::create(0.3f);
	_title->runAction(fadein);
	button_quit->runAction(fadein->clone());
	button_audio->runAction(fadein->clone());
	sprite_audio->runAction(fadein->clone());

	//隐藏关卡icon
	Vector<Node*> children = node_menu_level->getChildren();
	for (auto child : children) {
		auto fadeout = FadeOut::create(0.8f);
		child->runAction(fadeout);
	}

	//关闭显示record面板
	auto fadeout1 = FadeOut::create(0.3f);
	_record->runAction(fadeout1);
	auto children1 = _record->getChildren();
	for (auto child1 : children1) {
		child1->runAction(fadeout1->clone());
	}

	//关闭显示挑战模式中的按钮
	button_stronger->runAction(fadeout1->clone());
	button_faster->runAction(fadeout1->clone());
	button_perfect->runAction(fadeout1->clone());
	sprite_stronger->runAction(fadeout1->clone());
	sprite_faster->runAction(fadeout1->clone());
	sprite_perfect->runAction(fadeout1->clone());



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



