#include "SceneGame.h"
#include "Pos.h"
#include "Entity.h"
#include "TowerBase.h"
#include "Monster.h"
#include "Tower.h"
#include "ui/CocosGUI.h"
#include "ReaderJson.h"

using namespace ui;

#define ROWS (32)
#define COLS (18)

#define TOWERBASE_Z (5)
#define FUNGUS_Z (6)
#define TOWER_Z (10)
#define MOSTER_Z (15)
#define AMMO_Z (16)
#define ANIM_Z (18)

//子弹时间的时候添加遮罩
#define MASK_Z (19)
#define MASK_P ("UI/mask.png")

#define UI_Z (20)
#define TIP_Z (30)

#define MONEY_P ("UI/money.png")
#define CORE_P ("UI/core.png")
#define WAVE_P ("UI/wave.png")

#define TB_P ("Maps/towerbase0.png")
#define TBA_P ("Maps/towerbase")

#define FGF_P ("Maps/fungus_fat0.png")
#define FGFA_P ("Maps/fungus_fat")

#define ICON_N ("UI/icon_tower")

#define BOOM_N ("Anim/boom")
#define ICE_N ("Anim/ice")
#define FIRE_N ("Anim/fire")

#define AIRSHIP_P ("Maps/airship0.png")
#define AIRSHIPA_N ("Maps/airship")

//几个Tip的路径
#define TIP_0_P ("Tip/tip0.png")
#define TIP_1_P ("Tip/tip1.png")
#define TIP_2_P ("Tip/tip2.png")
#define TIP_3_P ("Tip/tip3.png")
#define TIP_4_P ("Tip/tip4.png")


Scene * SceneGame::createScene()
{
	auto scene = Scene::create();
	auto layer = SceneGame::create();
	scene->addChild(layer);
	return scene;
}

bool SceneGame::init()
{
	if (!Layer::init()) {
		return false;
	}

	//播放BGM
	AudioManager::PlayBGM(BATTLE_M,1.5f);
	
	//初始化DataManager
	DataManager::initGame();

	//初始化节点
	initNode();
	//初始化UI
	initUI();
	//初始化点击/触摸控制器
	initTouch();
	//初始化地图
	initMap(DataManager::level_current);

	//读取信息
	LoadInfo();


	//测试生成塔(在index为0的塔基)
	//CreateTower(5);
	
	//测试生成怪物
	//CreateMonster(0,0);
	//CreateMonster(5, 1);

	//测试根据WaveInfo来生成怪物
	//CreateMonsterByWaveInfo(0);
	

	//测试显示动画特效
	//ShowAnimation(BOOM_N, 7, 0.1f, Size(160, 160), Vec2(100, 100));
	//ShowAnimation(ICE_N, 7, 0.5f, Size(160, 160),Vec2(100,100));


	//测试Tip的显示效果
	//ShowTip(TIP_0_P);
	//ShowTip(TIP_1_P);
	//ShowTip(TIP_2_P);

	//测试结束时候显示的面板
	//ShowEndMenu();

	//绑定update函数
	this->schedule(schedule_selector(SceneGame::update));
}

void SceneGame::update(float dt)
{
	//log("dt time: %f", dt);
	//胜利或者失败条件判断
	if (is_final && !is_win&&vec_struct_monster.size() <= 0&&DataManager::num_monster<=0&&DataManager::hp_current>0) {
		is_win = true;
		ShowTip(TIP_2_P);
		ShowEndMenu();
	}
	if (!is_lose&&DataManager::hp_current<=0) {
		is_lose = true;
		ShowTip(TIP_4_P);
		ShowEndMenu();
		ShowEndMenu();
	}
	//怪物移动
	{
		int num_monster = vec_monster.size();
		for (int i = 0; i < num_monster; i++) {
			//vec_monster.at(i)->move(Vec2(50, 0), dt);
			vec_monster.at(i)->moveByPath(dt);
		}
	}
	//防御塔攻击
	{
		int num_tower = vec_tower.size();
		for (int i = 0; i < num_tower; i++) {
			bool flag_fire = false;
			Vec2 dir_fire;
			//检测所有怪物
			{
				for (int j = 0; j < vec_monster.size(); j++) {
					float distance = vec_tower.at(i)->vec_local.distance(vec_monster.at(j)->getPosition());
					if (distance < 300.0f) {
						flag_fire = true;
						dir_fire = Vec2(vec_tower.at(i)->vec_local+vec_tower.at(i)->vec_offset_fire, vec_monster.at(j)->getPosition());
						break;
					}
				}
			}
			if (flag_fire) {
				vec_tower.at(i)->fire(dt, dir_fire, vec_ammo, this, AMMO_Z);
			}
		}
	}
	//子弹移动和碰撞检测
	{
		int num_ammo = vec_ammo.size();
		for (int i = 0; i < num_ammo; i++) {
			vec_ammo.at(i)->updateAmmo(dt);
			for (int j = 0; j < vec_monster.size(); j++) {
				bool flag_break = false;
				//对于子弹种类是波的情况下的检测
				if (vec_ammo.at(i)->IsWave()) {
					Vec2 pos1 = vec_ammo.at(i)->getPosition();
					Vec2 pos2 = vec_monster.at(j)->getPosition();
					int range = vec_ammo.at(i)->GetWaveRange();
					float distance = pos1.distance(pos2);
					if (distance < (float)range) {
						float random = rand_0_1() * 5;
						if(random<1.0f)
						vec_monster.at(j)->TakeDamage(vec_ammo.at(i)->GetDamage());
					}
				}
				//对于一般情况的检测
				else {
					if (vec_monster.at(j)->IsContains(vec_ammo.at(i)->getPosition()) && vec_ammo.at(i)->coled()) {
						//log("hit!");
						//结算碰撞
						vec_monster.at(j)->TakeDamage(vec_ammo.at(i)->GetDamage());
						//额外效果
						int extra = vec_ammo.at(i)->GetExtraType();
						//log("extra effect is: %d", a);
						if (extra != 0) {
							if (extra == 1) {
								ShowAnimation(ICE_N, 7, 0.1f, Size(160, 160), vec_monster.at(j)->getPosition());
								vec_monster.at(j)->SetFrozen(1.0f);
							}
							else if (extra == 2) {
								ShowAnimation(FIRE_N, 7, 0.1f, Size(80, 80), vec_monster.at(j)->getPosition());
							}
						}

						flag_break == true;
						break;
					}
				}
				if (flag_break)break;
			}
			//对于波的攻击方式，只能在这里临时加上了

		}
	}
	//销毁失活的子弹
	{
		for (int i = 0; i < vec_ammo.size(); i++) {
			if (!vec_ammo.at(i)->GetActive()) {
				Ammo* temp = vec_ammo.at(i);
				vec_ammo.erase(vec_ammo.begin() + i);
				temp->removeFromParent();
			}
		}
	}
	//销毁死亡的怪物
	{
		for (int i = 0; i < vec_monster.size(); i++) {
			if (!vec_monster.at(i)->GetActive()) {
				Monster* temp = vec_monster.at(i);
				//如果死亡的时候到达终点了，就将hp-1
				if (temp->IsDestination()) {
					AddHp(-1);
				}
				else {
					int money = 10 - index_money / 10;
					money = money < 1 ? 1 : money;
					AddMoney(money);
					index_money++;
				}
				//死亡动画
				{
					ShowAnimation(BOOM_N, 7, 0.08f, Size(160, 160), temp->getPosition());
				}
				vec_monster.erase(vec_monster.begin() + i);
				temp->removeFromParent();

				//计数器减一
				DataManager::num_monster--;
			}
		}
	}
	//销毁被拆除的塔
	{
		for (int i = 0; i < vec_tower.size(); i++) {
			if (!vec_tower.at(i)->GetActive()) {
				Tower* temp = vec_tower.at(i);
				vec_tower.erase(vec_tower.begin() + i);
				temp->removeFromParent();
			}
		}
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

	//如果游戏结束了，就不继续生成怪物了
	if (is_win || is_lose) {
		return;
	}
	//依次生成vec_struct_monster里面的怪物
	if (!is_final&&DataManager::num_monster<=0) {
		timer_wave += dt;
		if (timer_wave > time_wave) {
			timer_wave = 0;
			bool flag = CreateMonsterByWaveInfo(index_wave);
			index_wave++;
			if (flag) {
				is_final = true;
				log("final wave!");
				ShowTip(TIP_1_P);
			}
			else {
				log("create wave: %d", index_wave);
				ShowTip(TIP_0_P);
			}
		}
	}
	if (vec_struct_monster.size() != 0) {
		timer_monster += dt;
		if (timer_monster > time_monster) {
			timer_monster = 0;
			struct_monster* s = vec_struct_monster.at(0);
			std::vector<struct_monster*>::iterator iter = vec_struct_monster.begin();
			vec_struct_monster.erase(iter);
			CreateMonster(s->type, s->path);
		}
	}
}


void SceneGame::initNode()
{
	//初始化UI_tower节点
	{
		if (node_UI_tower != NULL) {
			CC_SAFE_DELETE(node_UI_tower);
		}
		node_UI_tower = Node::create();
		this->addChild(node_UI_tower, UI_Z);
		node_UI_tower->setVisible(false);
	}
	
}

void SceneGame::initUI()
{
	//初始化上面的UI
	{
		Sprite* ui_money = Sprite::create(MONEY_P);
		ui_money->setPosition(Vec2(100, 660));
		this->addChild(ui_money,UI_Z);

		text_money = Text::create(Value(DataManager::money_current).asString(), "fonts/LuckiestGuy.ttf", 30);
		ui_money->addChild(text_money);
		text_money->setPosition(Vec2(160, 15));


		Sprite* ui_core = Sprite::create(CORE_P);
		ui_core->setPosition(Vec2(280, 660));
		this->addChild(ui_core,UI_Z);

		text_hp = Text::create(Value(DataManager::hp_current).asString(), "fonts/LuckiestGuy.ttf", 30);
		ui_core->addChild(text_hp);
		text_hp->setPosition(Vec2(140, 15));

		/*Sprite* ui_wave = Sprite::create(WAVE_P);
		ui_wave->setPosition(Vec2(1000, 660));
		this->addChild(ui_wave, UI_Z);*/
	}
	//初始化panel_tower
	{
		panel_tower = Entity::create(new Pos(0,1));
		panel_tower->BindSprite("UI/panel_tower0.png");
		panel_tower->SetSpriteSize(Size(200, 80));
		panel_tower->BindAnimation("UI/panel_tower", 11, 0.2f,Size(200,80));
		panel_tower->SetAnimationPlay(true);
		panel_tower->setPosition(Vec2(0, 100));
		node_UI_tower->addChild(panel_tower, UI_Z);
		node_UI_tower->setVisible(false);
		
		group_empty = Node::create();
		panel_tower->addChild(group_empty);
		group_empty->setVisible(false);

		group_nempty = Node::create();
		panel_tower->addChild(group_nempty);
		group_nempty->setVisible(false);

		group_nempty1 = Node::create();
		panel_tower->addChild(group_nempty1);
		group_nempty1->setVisible(false);

		group_nempty2 = Node::create();
		panel_tower->addChild(group_nempty2);
		group_nempty2->setVisible(false);

		group_full = Node::create();
		panel_tower->addChild(group_full);
		group_full->setVisible(false);

		//空白地块的UI
			{
			std::string path_build = String::createWithFormat("%s%d.png", ICON_N, 0)->getCString();
			Button* button_build = Button::create(path_build, path_build, path_build);
			button_build->addTouchEventListener([&](Ref* sender, Widget::TouchEventType type) {
				if (type == ui::Widget::TouchEventType::ENDED) {
					CreateTower(0);
					CloseAllMenu();
				}
			});
			button_build->setPosition(Vec2(-50, 0));
			group_empty->addChild(button_build);

			std::string path_build1 = String::createWithFormat("%s%d.png", ICON_N, 3)->getCString();
			Button* button_build1 = Button::create(path_build1, path_build1, path_build1);
			button_build1->addTouchEventListener([&](Ref* sender, Widget::TouchEventType type) {
				if (type == ui::Widget::TouchEventType::ENDED) {
					CreateTower(3);
					CloseAllMenu();
				}
			});
			button_build1->setPosition(Vec2(0, 0));
			group_empty->addChild(button_build1);

			std::string path_build2 = String::createWithFormat("%s%d.png", ICON_N, 6)->getCString();
			Button* button_build2 = Button::create(path_build2, path_build2, path_build2);
			button_build2->addTouchEventListener([&](Ref* sender, Widget::TouchEventType type) {
				if (type == ui::Widget::TouchEventType::ENDED) {
					CreateTower(6);
					CloseAllMenu();
				}
			});
			button_build2->setPosition(Vec2(50, 0));
			group_empty->addChild(button_build2);

		}
		//升级和拆除的UI
		{
			//升级的UI要按照塔来分类
			//一类塔的升级UI
			{
				std::string path_upgrade1 = String::createWithFormat("%s%d.png", ICON_N, 1)->getCString();
				Button* button_upgrade1 = Button::create(path_upgrade1, path_upgrade1, path_upgrade1);
				button_upgrade1->addTouchEventListener([&](Ref* sender, Widget::TouchEventType type) {
					if (type == ui::Widget::TouchEventType::ENDED) {
						//升级(由于时间和架构原因，只能临时手动判断了)
						if((DataManager::money_current-50)>=0)
						{
							log("upgrade to tower1");
							vec_towerbase.at(index_towerbase_selected)->DestroyTower();
							CreateTower(1);
						}
						else {
							ShowTip(TIP_3_P, 0.3f, 1.1f, 0.7f);
						}
						CloseAllMenu();
					}
				});
				button_upgrade1->setPosition(Vec2(-50, 0));
				group_nempty->addChild(button_upgrade1);

				std::string path_upgrade2 = String::createWithFormat("%s%d.png", ICON_N, 2)->getCString();
				Button* button_upgrade2 = Button::create(path_upgrade2, path_upgrade2, path_upgrade2);
				button_upgrade2->addTouchEventListener([&](Ref* sender, Widget::TouchEventType type) {
					if (type == ui::Widget::TouchEventType::ENDED) {
						//升级
						if ((DataManager::money_current - 75) >= 0)
						{
							log("upgrade to tower2");
							vec_towerbase.at(index_towerbase_selected)->DestroyTower();
							CreateTower(2);
						}
						else {
							ShowTip(TIP_3_P, 0.3f, 1.1f, 0.7f);
						}
						CloseAllMenu();
					}
				});
				button_upgrade2->setPosition(Vec2(0, 0));
				group_nempty->addChild(button_upgrade2);
			}
			//二类塔的升级UI
			{
			std::string path_upgrade4 = String::createWithFormat("%s%d.png", ICON_N, 4)->getCString();
			Button* button_upgrade4 = Button::create(path_upgrade4, path_upgrade4, path_upgrade4);
			button_upgrade4->addTouchEventListener([&](Ref* sender, Widget::TouchEventType type) {
				if (type == ui::Widget::TouchEventType::ENDED) {
					//升级
					if ((DataManager::money_current - 75) >= 0)
					{
						log("upgrade to tower4");
						vec_towerbase.at(index_towerbase_selected)->DestroyTower();
						CreateTower(4);
					}
					else {
						ShowTip(TIP_3_P, 0.3f, 1.1f, 0.7f);
					}
					CloseAllMenu();
				}
			});
			button_upgrade4->setPosition(Vec2(-50, 0));
			group_nempty1->addChild(button_upgrade4);

			std::string path_upgrade5 = String::createWithFormat("%s%d.png", ICON_N, 5)->getCString();
			Button* button_upgrade5 = Button::create(path_upgrade5, path_upgrade5, path_upgrade5);
			button_upgrade5->addTouchEventListener([&](Ref* sender, Widget::TouchEventType type) {
				if (type == ui::Widget::TouchEventType::ENDED) {
					//升级
					if ((DataManager::money_current - 75) >= 0)
					{
						log("upgrade to tower5");
						vec_towerbase.at(index_towerbase_selected)->DestroyTower();
						CreateTower(5);
					}
					else {
						ShowTip(TIP_3_P, 0.3f, 1.1f, 0.7f);
					}
					CloseAllMenu();
				}
			});
			button_upgrade5->setPosition(Vec2(0, 0));
			group_nempty1->addChild(button_upgrade5);
			}
			//三类塔的升级UI
			{
				std::string path_upgrade7 = String::createWithFormat("%s%d.png", ICON_N, 7)->getCString();
				Button* button_upgrade7 = Button::create(path_upgrade7, path_upgrade7, path_upgrade7);
				button_upgrade7->addTouchEventListener([&](Ref* sender, Widget::TouchEventType type) {
					if (type == ui::Widget::TouchEventType::ENDED) {
						//升级
						if ((DataManager::money_current - 75) >= 0)
						{
							log("upgrade to tower7");
							vec_towerbase.at(index_towerbase_selected)->DestroyTower();
							CreateTower(7);
						}
						else {
							ShowTip(TIP_3_P, 0.3f, 1.1f, 0.7f);
						}
						CloseAllMenu();
					}
				});
				button_upgrade7->setPosition(Vec2(-50, 0));
				group_nempty2->addChild(button_upgrade7);

				std::string path_upgrade8 = String::createWithFormat("%s%d.png", ICON_N, 8)->getCString();
				Button* button_upgrade8 = Button::create(path_upgrade8, path_upgrade8, path_upgrade8);
				button_upgrade8->addTouchEventListener([&](Ref* sender, Widget::TouchEventType type) {
					if (type == ui::Widget::TouchEventType::ENDED) {
						//升级
						if ((DataManager::money_current -100) >= 0)
						{
							log("upgrade to tower8");
							vec_towerbase.at(index_towerbase_selected)->DestroyTower();
							CreateTower(8);
						}
						else {
							ShowTip(TIP_3_P, 0.3f, 1.1f, 0.7f);
						}
						CloseAllMenu();
					}
				});
				button_upgrade8->setPosition(Vec2(0, 0));
				group_nempty2->addChild(button_upgrade8);
			}

			//这里代码可以复用，但是懒得再加节点了
			{
				Button* button_destroy = Button::create("UI/icon_destroy.png", "UI/icon_destroy.png", "UI/icon_destroy.png");
				button_destroy->addTouchEventListener([&](Ref* sender, Widget::TouchEventType type) {
					if (type == ui::Widget::TouchEventType::ENDED) {
						//拆除
						vec_towerbase.at(index_towerbase_selected)->DestroyTower();
						CloseAllMenu();
					}
				});
				button_destroy->setPosition(Vec2(50, 0));
				group_nempty->addChild(button_destroy);

				Button* button_destroy1 = Button::create("UI/icon_destroy.png", "UI/icon_destroy.png", "UI/icon_destroy.png");
				button_destroy1->addTouchEventListener([&](Ref* sender, Widget::TouchEventType type) {
					if (type == ui::Widget::TouchEventType::ENDED) {
						//拆除
						vec_towerbase.at(index_towerbase_selected)->DestroyTower();
						CloseAllMenu();
					}
				});
				button_destroy1->setPosition(Vec2(50, 0));
				group_nempty1->addChild(button_destroy1);

				Button* button_destroy2 = Button::create("UI/icon_destroy.png", "UI/icon_destroy.png", "UI/icon_destroy.png");
				button_destroy2->addTouchEventListener([&](Ref* sender, Widget::TouchEventType type) {
					if (type == ui::Widget::TouchEventType::ENDED) {
						//拆除
						vec_towerbase.at(index_towerbase_selected)->DestroyTower();
						AddMoney(50);
						CloseAllMenu();
					}
				});
				button_destroy2->setPosition(Vec2(50, 0));
				group_nempty2->addChild(button_destroy2);
			}
			//不能再升级的时候的UI
			{
				Button* button_destroy3 = Button::create("TestUI/icon_tower.png", "TestUI/icon_tower.png", "TestUI/icon_tower.png");
				button_destroy3->addTouchEventListener([&](Ref* sender, Widget::TouchEventType type) {
					if (type == ui::Widget::TouchEventType::ENDED) {
						//拆除
						vec_towerbase.at(index_towerbase_selected)->DestroyTower();
						AddMoney(50);
						CloseAllMenu();
					}
				});
				button_destroy3->setPosition(Vec2(0, 0));
				group_full->addChild(button_destroy3);
			}

		}
	}
	//初始化mask
	{
		_mask = Sprite::create(MASK_P);
		this->addChild(_mask, MASK_Z);
		Size size = Director::getInstance()->getVisibleSize();
		_mask->setContentSize(size*2);
		_mask->setVisible(false);
	}
}

void SceneGame::initTouch()
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
		DetectorVecTowebase(point);
		return true;
	};
	listener->setSwallowTouches(true);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);

}

void SceneGame::initMap(int i)
{
	std::string path = String::createWithFormat("Maps/map%d.tmx", i)->getCString();
	if (_map != NULL) {
		CC_SAFE_DELETE(_map);
	}
	_map = TMXTiledMap::create(path);
	this->addChild(_map);
	//在GID为0的地图块添加可以点击的格子
	/*for (int i = 0; i < ROWS; i++) {
		for (int j = 0; j < COLS; j++) {
			Pos* pos =new Pos(i, j);
			if (GetGridIndex(pos) == 0) {
				TowerBase* towerbase = TowerBase::create(pos, 0);
				towerbase->BindSprite("TestSprites/tower.png");
				towerbase->SetSpriteSize(Size(80, 80));
				this->addChild(towerbase,TOWERBASE_Z);
				vec_towerbase.pushBack(towerbase);
			}
		}
	}*/
	//获取对象层信息，添加可以点击的格子
	{
		auto group_node_towerbase = _map->getObjectGroup("towerbase");
		auto vec_node_towerbase = group_node_towerbase->getObjects();
		int num = vec_node_towerbase.size();
		for (unsigned int i = 0; i < num; i++) {
			auto node_towerbase = vec_node_towerbase.at(i).asValueMap();
			float posx = node_towerbase.at("x").asFloat();
			float posy = node_towerbase.at("y").asFloat();
			TowerBase* towerbase = TowerBase::create(new Pos(posx, posy));
			towerbase->BindSprite(TB_P);
			towerbase->SetSpriteSize(Size(80, 80));
			towerbase->BindAnimation(TBA_P, 7, 0.1f,Size(80,80));
			towerbase->SetAnimationPlay(true);
			this->addChild(towerbase, TOWERBASE_Z);
			vec_towerbase.pushBack(towerbase);
		}
	}

	//获取对象层信息，添加fungus
	{
		auto group_node_fgf = _map->getObjectGroup("fungus");
		auto vec_node_fgf = group_node_fgf->getObjects();
		int num = vec_node_fgf.size();
		for (unsigned int i = 0; i < num; i++) {
			auto node_fgf = vec_node_fgf.at(i).asValueMap();
			float posx = node_fgf.at("x").asFloat();
			float posy = node_fgf.at("y").asFloat();
			Entity* fgf = Entity::create(new Pos(posx, posy));
			fgf->BindSprite(FGF_P);
			fgf->SetSpriteSize(Size(40, 40));
			fgf->BindAnimation(FGFA_P, 6, 0.2f, Size(40, 40));
			fgf->SetAnimationPlay(true);
			this->addChild(fgf, FUNGUS_Z);
		}
	}
	//获取对象层信息，添加飞船
	{
		auto group_node_airship = _map->getObjectGroup("airship");
		auto vec_node_airship = group_node_airship->getObjects();
		int num = vec_node_airship.size();
		for (unsigned int i = 0; i < num; i++) {
			auto node_fgf = vec_node_airship.at(i).asValueMap();
			float posx = node_fgf.at("x").asFloat();
			float posy = node_fgf.at("y").asFloat();
			Entity* fgf = Entity::create(new Pos(posx, posy));
			fgf->BindSprite(AIRSHIP_P);
			fgf->SetSpriteSize(Size(40, 40));
			fgf->BindAnimation(AIRSHIPA_N, 6, 0.15f, Size(80, 80));
			fgf->SetAnimationPlay(true);
			this->addChild(fgf, TOWER_Z);
		}
	}
	
	//获取对象层信息，添加路径节点
	{
		auto group_node_path = _map->getObjectGroup("path");
		auto vec_node_path = group_node_path->getObjects();
		int num = vec_node_path.size();
		for (unsigned int i = 0; i < num; i++) {
			auto node_path = vec_node_path.at(i).asValueMap();
			float posx = node_path.at("x").asFloat();
			float posy = node_path.at("y").asFloat();
			vec_path.pushBack(new Pos(posx, posy));
		}
	}
	//添加第二条路径的节点(同理)
	{
		if (_map->getObjectGroup("path1") != nullptr) {
			auto group_node_path = _map->getObjectGroup("path1");
			auto vec_node_path = group_node_path->getObjects();
			int num = vec_node_path.size();
			for (unsigned int i = 0; i < num; i++) {
				auto node_path = vec_node_path.at(i).asValueMap();
				float posx = node_path.at("x").asFloat();
				float posy = node_path.at("y").asFloat();
				vec_path1.pushBack(new Pos(posx, posy));
			}
		}
	}
}

int SceneGame::GetGridIndex(Pos* pos)
{
	auto maplayer = _map->getLayer("base");
	Vec2 vec = Vec2(pos->GetPosx(), COLS - 1 - pos->GetPosy());
	auto tileid = maplayer->getTileGIDAt(vec);
	return (int)tileid-1;
}

bool SceneGame::IsTowerbaseSelected(int index,Point point)
{
	TowerBase* towerbase = vec_towerbase.at(index);
	return towerbase->IsContains(point);
}

void SceneGame::DetectorVecTowebase(Point point)
{
	//flag：是否是有效点击
	bool flag_total = false;
	//遍历vec_towerbase，判断是否点击到Towerbase
	int size = vec_towerbase.size();
	for (int i = 0; i < size; i++) {
		bool flag = IsTowerbaseSelected(i, point);
		if (flag) {
			flag_total = true;
			index_towerbase_selected = i;
			{
				Vec2 pos = vec_towerbase.at(i)->getPosition();
				node_UI_tower->setPosition(pos);

				//log("selected: %d",i);
				//是否有塔
				has_tower_towerbase_selected = vec_towerbase.at(i)->HasTower();
				if (has_tower_towerbase_selected) {
					log("selected towerbase has tower!");
				}
				else {
					log("selected towerbase doesn't have tower!");
				}

				OpenMenu(vec_towerbase.at(i)->GetTypeIndex());
			}
			
		}
		else {
			log("not selected: %d", i);
		}
	}
	//判断是否点击到panel_tower
	panel_tower->SetPosFather(node_UI_tower->getPosition());
	if (panel_tower->IsContains(point)) {
		flag_total = true;
	}
	//如果不是有效点击，则执行
	if (!flag_total) {
		CloseAllMenu();
	}
}

void SceneGame::LoadInfo()
{
	//载入info_wave
	{
		ValueVector vec_name_wave;
		for (int i = 0; i < 9; i++) {
			vec_name_wave.push_back(Value(Value(i).asString()));
		}
		std::string path = String::createWithFormat("Info/level%d.json", DataManager::level_current)->getCString();
		info_wave = Info::create(vec_name_wave, path);
	}
	//载入info_monster
	{
		ValueVector vec_name_monster;
		vec_name_monster.push_back(Value("big_eye"));
		vec_name_monster.push_back(Value("small_eye"));
		vec_name_monster.push_back(Value("many_legs"));
		vec_name_monster.push_back(Value("few_legs"));
		vec_name_monster.push_back(Value("big_mouse"));
		vec_name_monster.push_back(Value("small_mouse"));	

		info_monster = Info::create(vec_name_monster, "Info/monster.json");
	}
	//载入info_monster_file
	{
		ValueVector vec_name_monster_file;
		vec_name_monster_file.push_back(Value("big_eye"));
		vec_name_monster_file.push_back(Value("small_eye"));
		vec_name_monster_file.push_back(Value("many_legs"));
		vec_name_monster_file.push_back(Value("few_legs"));
		vec_name_monster_file.push_back(Value("big_mouse"));
		vec_name_monster_file.push_back(Value("small_mouse"));

		info_monster_file = Info::create(vec_name_monster_file, "Info/monster_file.json");
	}

	//载入info_tower
	{
		ValueVector vec_name_tower;
		vec_name_tower.push_back(Value("plasma"));
		vec_name_tower.push_back(Value("plasma_up"));
		vec_name_tower.push_back(Value("plasma_up_up"));
		vec_name_tower.push_back(Value("cannon"));
		vec_name_tower.push_back(Value("cannon_up"));
		vec_name_tower.push_back(Value("cannon_up_up"));
		vec_name_tower.push_back(Value("boardcast"));
		vec_name_tower.push_back(Value("boardcast_up"));
		vec_name_tower.push_back(Value("boardcast_up"));

		info_tower = Info::create(vec_name_tower, "Info/tower.json");
	}

	//载入info_tower_file
	{
		ValueVector vec_name_tower_file;
		vec_name_tower_file.push_back(Value("plasma"));
		vec_name_tower_file.push_back(Value("plasma_up"));
		vec_name_tower_file.push_back(Value("plasma_up_up"));
		vec_name_tower_file.push_back(Value("cannon"));
		vec_name_tower_file.push_back(Value("cannon_up"));
		vec_name_tower_file.push_back(Value("cannon_up_up"));
		vec_name_tower_file.push_back(Value("boardcast"));
		vec_name_tower_file.push_back(Value("boardcast_up"));
		vec_name_tower_file.push_back(Value("boardcast_up"));

		info_tower_file = Info::create(vec_name_tower_file, "Info/tower_file.json");
	}
	
	//载入info_ammo
	{
		ValueVector vec_name_ammo;
		vec_name_ammo.push_back(Value("ammo_plasma"));
		vec_name_ammo.push_back(Value("ammo_plasma_up"));
		vec_name_ammo.push_back(Value("ammo_plasma_up_up"));
		vec_name_ammo.push_back(Value("ammo_cannon"));
		vec_name_ammo.push_back(Value("ammo_cannon_up"));
		vec_name_ammo.push_back(Value("ammo_cannon_up_up"));
		vec_name_ammo.push_back(Value("ammo_boardcast"));
		vec_name_ammo.push_back(Value("ammo_boardcast_up"));
		vec_name_ammo.push_back(Value("ammo_boardcast_up_up"));

		info_ammo = Info::create(vec_name_ammo, "Info/ammo.json");
	}

	//载入info_ammo_file
	{
		ValueVector vec_name_ammo_file;
		vec_name_ammo_file.push_back(Value("ammo_plasma"));
		vec_name_ammo_file.push_back(Value("ammo_plasma_up"));
		vec_name_ammo_file.push_back(Value("ammo_plasma_up_up"));
		vec_name_ammo_file.push_back(Value("ammo_cannon"));
		vec_name_ammo_file.push_back(Value("ammo_cannon_up"));
		vec_name_ammo_file.push_back(Value("ammo_cannon_up_up"));
		vec_name_ammo_file.push_back(Value("ammo_boardcast"));
		vec_name_ammo_file.push_back(Value("ammo_boardcast_up"));
		vec_name_ammo_file.push_back(Value("ammo_boardcast_up_up"));


		info_ammo_file = Info::create(vec_name_ammo_file, "Info/ammo_file.json");
	}
}


void SceneGame::CreateMonster(int type,int path)
{
	//创建怪物时候的音效
	AudioManager::PlayEffect(MONSTER_M);

	ValueVector vv_monster = info_monster->GetIntInfoVectorByID(type);
	ValueVector vv_monster_file = info_monster_file->GetStringInfoVectorByID(type);
	//读取文件数据
	//这里的index_path已经弃用
	//int index_path = vv_monster_file.at(0).asInt();
	Size size = Size(vv_monster_file.at(1).asInt(), vv_monster_file.at(2).asInt());
	int len_animation = vv_monster_file.at(3).asInt();
	float inter_animation = (float)vv_monster_file.at(4).asInt()/1000.0f;
	Vec2 offset_hp = Vec2(vv_monster_file.at(5).asInt(), vv_monster_file.at(6).asInt());

	std::string path_sprite = vv_monster_file.at(7).asString();
	std::string name_animation = vv_monster_file.at(8).asString();
	std::string path_hp = vv_monster_file.at(9).asString();
	std::string path_hp_slider = vv_monster_file.at(10).asString();
	//测试输出读取文件的信息
	/*{
		log("size_x: %f;size_y: %f;", size.width, size.height);
		log("len_animation: %d,inter_animation: %f;offset_hp_x: %f;offset_hp_y: %f", len_animation, inter_animation, offset_hp.x, offset_hp.y);
		log("path_sprite: %s", path_sprite.c_str());
		log("name_animation: %s", name_animation.c_str());
		log("path_hp: %s;path_hp_slider: %s", path_hp.c_str(), path_hp_slider.c_str());
	}*/


	//生成
	Monster* monster;
	if (path == 0) {
		monster = (Monster*)Monster::createByPath(vec_path);
	}
	else {
		monster = (Monster*)Monster::createByPath(vec_path1);
	}
	monster->BindSprite(path_sprite);
	monster->SetSpriteSize(size);
	monster->BindAnimation(name_animation, len_animation, inter_animation,size);
	monster->SetAnimationPlay(true);
	this->addChild(monster, MOSTER_Z);
	//添加到vec_monster里面
	vec_monster.pushBack(monster);

	//绑定血条
	monster->BindHp(path_hp,path_hp_slider,offset_hp);

	//设置参数
	monster->SetValuesByInfo(vv_monster);

	//计数器加一
	DataManager::num_monster++;
}

void SceneGame::CreateTower(int type)
{	
	//创建塔时候的额音效
	AudioManager::PlayEffect(CLICK_1_M);

	ValueVector vv_tower = info_tower->GetIntInfoVectorByID(type);
	ValueVector vv_tower_file = info_tower_file->GetStringInfoVectorByID(type);
	//读取文件数据
	Size size = Size(vv_tower_file.at(0).asInt(), vv_tower_file.at(1).asInt());
	int len_animation = vv_tower_file.at(2).asInt();
	float inter_animation = (float)vv_tower_file.at(3).asInt() / 1000.0f;
	std::string path_sprite = vv_tower_file.at(4).asString();
	std::string name_animation = vv_tower_file.at(5).asString();
	int type_tower = vv_tower_file.at(6).asInt();
	//测试输出读取文件的信息
	{
		log("size_x: %f;size_y: %f;", size.width, size.height);
		log("len_animation: %d,inter_animation: %f;", len_animation, inter_animation);
		log("path_sprite: %s", path_sprite.c_str());
		log("name_animation: %s", name_animation.c_str());
	}
	//建塔钱先检测一下金钱是否足够，如果够的话再减少金钱
	int cost = vv_tower.at(1).asInt();
	if (DataManager::money_current - cost >= 0) {
		AddMoney(-cost);
	}
	else {
		ShowTip(TIP_3_P,0.3f,1.1f,0.7f);
		return;
	}
	//这里建塔的过程
	Tower* tower = Tower::create();
	tower->BindSprite(path_sprite);
	tower->BindAnimation(name_animation, len_animation, inter_animation, size);
	tower->SetAnimationPlay(true);
	vec_towerbase.at(index_towerbase_selected)->SetTower(tower,type);
	tower->SetType(type_tower);
	//添加到vec_towet里面
	vec_tower.pushBack(tower);

	//绑定子弹
	ValueVector vv_ammo = info_ammo->GetIntInfoVectorByID(type);
	ValueVector vv_ammo_file = info_ammo_file->GetStringInfoVectorByID(type);
	tower->BindAmmoByInfo(vv_ammo, vv_ammo_file);
	//设置参数
	tower->SetValuesByInfo(vv_tower);
}

void SceneGame::CloseAllMenu()
{
	//关闭菜单时恢复正常的时间流速
	{
		Director::getInstance()->getScheduler()->setTimeScale(1.0f);
		_mask->setVisible(false);
	}
	node_UI_tower->setVisible(false);
	group_empty->setVisible(false);
	group_nempty->setVisible(false);
	group_nempty1->setVisible(false);
	group_nempty2->setVisible(false);
	group_full->setVisible(false);
}

void SceneGame::OpenMenu(int index)
{
	//打开菜单时添加时间减缓效果
	{
		Director::getInstance()->getScheduler()->setTimeScale(0.15f);
		_mask->setVisible(true);
	}
	node_UI_tower->setVisible(true);
	if (has_tower_towerbase_selected) {
		log("index of open_menu is: %d", index);
		switch (index)
		{
		case 0:
			group_nempty->setVisible(true);
			break;
		case 3:
			group_nempty1->setVisible(true);
			break;
		case 6:
			group_nempty2->setVisible(true);
			break;
		default:
			group_full->setVisible(true);
			break;
		}
	}
	else {
		group_empty->setVisible(true);
	}
}

void SceneGame::ShowTip(std::string path,float time_scaleto,float target_scale,float time_fadeout)
{
	auto tip = Sprite::create(path);
	Size size = Director::getInstance()->getVisibleSize();
	tip->setPosition(Vec2(size.width/2,size.height/2+50));
	this->addChild(tip, TIP_Z);
	auto scaleto = ScaleTo::create(time_scaleto, target_scale);
	auto fadein = FadeOut::create(time_fadeout);
	auto seq = Sequence::create(scaleto, fadein, CallFuncN::create([&](Node* sender) {
		sender->removeFromParent();
	}), nullptr);
	tip->runAction(seq);
}

void SceneGame::ShowAnimation(std::string path,int len,float time,Size size,Vec2 pos)
{
	std::string path_sprite = String::createWithFormat("%s0.png", path.c_str())->getCString();
	log("path of sprite is: %s;name of animation is: %s", path_sprite.c_str(), path.c_str());
	Entity* animation = Entity::create(new Pos(0, 0));
	animation->setPosition(pos);
	animation->BindSprite(path_sprite);
	animation->BindAnimation(path, len, time, size);
	animation->SetAnimationPlay(true,true);
	this->addChild(animation, ANIM_Z);
	//动作序列只能销毁Sprite，Sprite销毁了但是Entity还没有被销毁，所以这里只能手动销毁Entity
	vec_todestroy.pushBack(animation);
	animation->Destroy(3.0f);
}

bool SceneGame::CreateMonsterByWaveInfo(int index_wave)
{
	bool flag = false;
	ValueVector vv = info_wave->GetStringInfoVectorByID(index_wave);
	int type9 = vv.at(0).asInt();
	int path9 = vv.at(1).asInt();
	int num99 = vv.at(2).asInt();
	flag = vv.at(3).asBool();
	//测试
	//log("type9: %d;path9: %d;num99: %d", type9, path9, num99);
	int type, path, num;
	bool is_final = vv.at(3).asBool();
	do {
		type = type9 % 10;
		path = path9 % 10;
		num = num99 % 100;
		type9 /= 10;
		path9 /= 10;
		num99 /= 100;
		//测试
		//log("type: %d;path: %d;num: %d", type, path, num);
		for (int i = 0; i < num; i++) {
			struct_monster* s = new struct_monster{ type,path };
			vec_struct_monster.push_back(s);
		}
		
	} while (type9!=9);
	return flag;
}

void SceneGame::AddMoney(int money)
{
	int target = DataManager::money_current;
	target += money;
	target = (target < 0) ? 0 : target;
	DataManager::money_current = target;
	text_money->setText(Value(target).asString());
}

void SceneGame::AddHp(int hp)
{
	int target = DataManager::hp_current;
	target += hp;
	target = (target < 0) ? 0 : target;
	DataManager::hp_current = target;
	text_hp->setText(Value(target).asString());
}

void SceneGame::ShowEndMenu()
{
	Size size = Director::getInstance()->getVisibleSize();

	panel_endgame = Sprite::create("UI/panel_endgame.png");
	this->addChild(panel_endgame, UI_Z);
	panel_endgame->setPosition(size/2);
	panel_endgame->setContentSize(Size(400, 300));
	panel_endgame->setOpacity(0);
	auto moveby = MoveBy::create(2.0f,Vec2(0,0));
	auto scaleby = ScaleBy::create(0.3f,1.1f);
	auto fadein = FadeIn::create(0.3f);
	auto spawn = Spawn::create(scaleby, fadein, nullptr, nullptr);
	auto seq = Sequence::create(moveby, spawn, CallFunc::create([&]() {
		//生成按钮
		{
			std::string path_restart = "UI/button_restart.png";
			std::string path_back = "UI/button_back.png";
			std::string path_next = "UI/button_next.png";
			Vec2 pos = Vec2(190, 220);
			Vec2 pos1 = Vec2(190, 145);
			Vec2 pos2 = Vec2(190, 70);
			Vec2 pos3 = Vec2(190, 190);
			Vec2 pos4 = Vec2(190, 100);

			if (is_win) {
				Button* button_restart = Button::create(path_restart, path_restart, path_restart);
				panel_endgame->addChild(button_restart);
				button_restart->setPosition(pos);
				button_restart->addTouchEventListener([&](Ref* sender, Widget::TouchEventType type) {
					if (type == ui::Widget::TouchEventType::ENDED) {
						SceneManager::ToSceneGame();
					}
				});

				Button* button_back = Button::create(path_back, path_back, path_back);
				panel_endgame->addChild(button_back);
				button_back->setPosition(pos1);
				button_back->addTouchEventListener([&](Ref* sender, Widget::TouchEventType type) {
					if (type == ui::Widget::TouchEventType::ENDED) {
						SceneManager::ToSceneStart();
					}
				});

				Button* button_next = Button::create(path_next, path_next, path_next);
				panel_endgame->addChild(button_next);
				button_next->setPosition(pos2);
				button_next->addTouchEventListener([&](Ref* sender, Widget::TouchEventType type) {
					if (type == ui::Widget::TouchEventType::ENDED) {
						if (DataManager::level_current < 5) {
							DataManager::level_current++;
							SceneManager::ToSceneGame();
						}
						else {
							SceneManager::ToSceneStart();
						}
					}
				});
			}
			else {
				Button* button_restart = Button::create(path_restart, path_restart, path_restart);
				panel_endgame->addChild(button_restart);
				button_restart->setPosition(pos3);
				button_restart->addTouchEventListener([&](Ref* sender, Widget::TouchEventType type) {
					if (type == ui::Widget::TouchEventType::ENDED) {
						SceneManager::ToSceneGame();
					}
				});

				Button* button_back = Button::create(path_back, path_back, path_back);
				panel_endgame->addChild(button_back);
				button_back->setPosition(pos4);
				button_back->addTouchEventListener([&](Ref* sender, Widget::TouchEventType type) {
					if (type == ui::Widget::TouchEventType::ENDED) {
						SceneManager::ToSceneStart();
					}
				});
			}
			
		}
	}), nullptr);
	panel_endgame->runAction(seq);
}


