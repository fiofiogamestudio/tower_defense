#pragma once
#include "RecordManager.h"
class DataManager {
public:
	void init();

	//不封装了，静态数据
	static int level_current;
	static int num_monster;
	
	static int money_current;
	static int hp_current;


	//游戏场景初始化的数据
	static void initGame();

	//用来持久化的数据
	static int num_kill;
	static int num_win;
	static void initRecord();
	static void saveRecord();
};