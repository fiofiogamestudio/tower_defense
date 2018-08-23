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
#define UI_Z (20)
#define TIP_Z (30)

#define MAX_P ("Tip/max_level.png")

#define TB_P ("Maps/towerbase0.png")
#define TBA_P ("Maps/towerbase")

#define FGF_P ("Maps/fungus_fat0.png")
#define FGFA_P ("Maps/fungus_fat")

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
	//初始化节点
	initNode();
	//初始化UI
	initUI();
	//初始化点击/触摸控制器
	initTouch();
	//初始化地图
	initMap(0);

	//读取怪物信息
	LoadInfo();
	
	//测试生成怪物
	CreateMonster(0);
	CreateMonster(1);
	
	//测试显示tip
	//ShowTip(MAX_P);

	//绑定update函数
	this->schedule(schedule_selector(SceneGame::update));
}

void SceneGame::update(float dt)
{
	//log("dt time: %f", dt);
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
				if (vec_monster.at(j)->IsContains(vec_ammo.at(i)->getPosition())&&vec_ammo.at(i)->coled()) {
					//log("hit!");
					//结算碰撞
					vec_monster.at(j)->TakeDamage(vec_ammo.at(i)->GetDamage());
					flag_break == true;
					break;
				}
				if (flag_break)break;
			}
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
				vec_monster.erase(vec_monster.begin() + i);
				temp->removeFromParent();
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

		//空白地块的UI
			{
			Button* button_build = Button::create("TestUI/icon_tower.png", "TestUI/icon_tower.png", "TestUI/icon_tower.png");
			button_build->addTouchEventListener([&](Ref* sender, Widget::TouchEventType type) {
				if (type == ui::Widget::TouchEventType::ENDED) {
					CreateTower(0);
					CloseAllMenu();
				}
			});
			button_build->setPosition(Vec2(-50, 0));
			group_empty->addChild(button_build);

			Button* button_build1 = Button::create("TestUI/icon_tower.png", "TestUI/icon_tower.png", "TestUI/icon_tower.png");
			button_build1->addTouchEventListener([&](Ref* sender, Widget::TouchEventType type) {
				if (type == ui::Widget::TouchEventType::ENDED) {
					CreateTower(0);
					CloseAllMenu();
				}
			});
			button_build1->setPosition(Vec2(0, 0));
			group_empty->addChild(button_build1);

			Button* button_build2 = Button::create("TestUI/icon_tower.png", "TestUI/icon_tower.png", "TestUI/icon_tower.png");
			button_build2->addTouchEventListener([&](Ref* sender, Widget::TouchEventType type) {
				if (type == ui::Widget::TouchEventType::ENDED) {
					CreateTower(0);
					CloseAllMenu();
				}
			});
			button_build2->setPosition(Vec2(50, 0));
			group_empty->addChild(button_build2);

		}
		//升级和拆除的UI
		{
			Button* button_upgrade=Button::create("TestUI/icon_tower.png", "TestUI/icon_tower.png", "TestUI/icon_tower.png");
			button_upgrade->addTouchEventListener([&](Ref* sender, Widget::TouchEventType type) {
				if (type == ui::Widget::TouchEventType::ENDED) {
					//升级
					{
						//先拆除旧塔 再建一个升级版本的塔						
						//log("type of tower to upgrade is: %d", vec_towerbase.at(index_towerbase_selected)->GetTypeIndex());
						int type_upgrade = vec_towerbase.at(index_towerbase_selected)->GetTypeIndex()+1;
						if (type_upgrade % 3 != 0) {
							vec_towerbase.at(index_towerbase_selected)->DestroyTower();
							CreateTower(type_upgrade);
						}
						else {
							//log("tower max level!");
							ShowTip(MAX_P);
						}
						
					}
					CloseAllMenu();
				}
			});
			button_upgrade->setPosition(Vec2(-30, 0));
			group_nempty->addChild(button_upgrade);
			Button* button_destroy = Button::create("TestUI/icon_tower.png", "TestUI/icon_tower.png", "TestUI/icon_tower.png");
			button_destroy->addTouchEventListener([&](Ref* sender, Widget::TouchEventType type) {
				if (type == ui::Widget::TouchEventType::ENDED) {
					//拆除
					vec_towerbase.at(index_towerbase_selected)->DestroyTower();
					CloseAllMenu();
				}
			});
			button_destroy->setPosition(Vec2(30, 0));
			group_nempty->addChild(button_destroy);
		}
		
		
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

	//获取对象层信息，添加fungus_fat
	{
		auto group_node_fgf = _map->getObjectGroup("fungus_fat");
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

				OpenMenu();
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
	//载入info_monster
	{
		ValueVector vec_name_monster;
		vec_name_monster.push_back(Value("big_eye"));
		vec_name_monster.push_back(Value("small_eye"));

		info_monster = Info::create(vec_name_monster, "Info/monster.json");
	}
	//载入info_monster_file
	{
		ValueVector vec_name_monster_file;
		vec_name_monster_file.push_back(Value("big_eye"));
		vec_name_monster_file.push_back(Value("small_eye"));

		info_monster_file = Info::create(vec_name_monster_file, "Info/monster_file.json");
	}

	//载入info_tower
	{
		ValueVector vec_name_tower;
		vec_name_tower.push_back(Value("plasma"));
		vec_name_tower.push_back(Value("plasma_up"));
		vec_name_tower.push_back(Value("plasma_up_up"));

		info_tower = Info::create(vec_name_tower, "Info/tower.json");
	}

	//载入info_tower_file
	{
		ValueVector vec_name_tower_file;
		vec_name_tower_file.push_back(Value("plasma"));
		vec_name_tower_file.push_back(Value("plasma_up"));
		vec_name_tower_file.push_back(Value("plasma_up_up"));

		info_tower_file = Info::create(vec_name_tower_file, "Info/tower_file.json");
	}
	
	//载入info_ammo
	{
		ValueVector vec_name_ammo;
		vec_name_ammo.push_back(Value("ammo_plasma"));
		vec_name_ammo.push_back(Value("ammo_plasma_up"));
		vec_name_ammo.push_back(Value("ammo_plasma_up_up"));

		info_ammo = Info::create(vec_name_ammo, "Info/ammo.json");
	}

	//载入info_ammo_file
	{
		ValueVector vec_name_ammo_file;
		vec_name_ammo_file.push_back(Value("ammo_plasma"));
		vec_name_ammo_file.push_back(Value("ammo_plasma_up"));
		vec_name_ammo_file.push_back(Value("ammo_plasma_up_up"));


		info_ammo_file = Info::create(vec_name_ammo_file, "Info/ammo_file.json");
	}
}


void SceneGame::CreateMonster(int type)
{
	ValueVector vv_monster = info_monster->GetIntInfoVectorByID(type);
	ValueVector vv_monster_file = info_monster_file->GetStringInfoVectorByID(type);
	//读取文件数据
	int index_path = vv_monster_file.at(0).asInt();
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
		log("index_path: %d;size_x: %f;size_y: %f;", index_path, size.width, size.height);
		log("len_animation: %d,inter_animation: %f;offset_hp_x: %f;offset_hp_y: %f", len_animation, inter_animation, offset_hp.x, offset_hp.y);
		log("path_sprite: %s", path_sprite.c_str());
		log("name_animation: %s", name_animation.c_str());
		log("path_hp: %s;path_hp_slider: %s", path_hp.c_str(), path_hp_slider.c_str());
	}*/


	//生成
	Monster* monster = (Monster*)Monster::createByPath(vec_path);
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
}

void SceneGame::CreateTower(int type)
{
	ValueVector vv_tower = info_tower->GetIntInfoVectorByID(type);
	ValueVector vv_tower_file = info_tower_file->GetStringInfoVectorByID(type);
	//读取文件数据
	Size size = Size(vv_tower_file.at(0).asInt(), vv_tower_file.at(1).asInt());
	int len_animation = vv_tower_file.at(2).asInt();
	float inter_animation = (float)vv_tower_file.at(3).asInt() / 1000.0f;
	std::string path_sprite = vv_tower_file.at(4).asString();
	std::string name_animation = vv_tower_file.at(5).asString();
	//测试输出读取文件的信息
	{
		log("size_x: %f;size_y: %f;", size.width, size.height);
		log("len_animation: %d,inter_animation: %f;", len_animation, inter_animation);
		log("path_sprite: %s", path_sprite.c_str());
		log("name_animation: %s", name_animation.c_str());
	}
	//这里建塔的过程
	Tower* tower = Tower::create();
	tower->BindSprite(path_sprite);
	tower->BindAnimation(name_animation, len_animation, inter_animation, size);
	tower->SetAnimationPlay(true);
	vec_towerbase.at(index_towerbase_selected)->SetTower(tower,type);
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
	node_UI_tower->setVisible(false);
	group_empty->setVisible(false);
	group_nempty->setVisible(false);
}

void SceneGame::OpenMenu()
{
	node_UI_tower->setVisible(true);
	if (has_tower_towerbase_selected) {
		group_nempty->setVisible(true);
	}
	else {
		group_empty->setVisible(true);
	}
}

void SceneGame::ShowTip(std::string path)
{
	auto tip = Sprite::create(path);
	Size size = Director::getInstance()->getVisibleSize();
	tip->setPosition(Vec2(size.width/2,size.height/2+50));
	this->addChild(tip, TIP_Z);
	auto scaleto = ScaleTo::create(0.3f, 1.1f);
	auto fadein = FadeOut::create(0.7f);
	auto seq = Sequence::create(scaleto, fadein, CallFuncN::create([&](Node* sender) {
		sender->removeFromParent();
	}), nullptr);
	tip->runAction(seq);
}

