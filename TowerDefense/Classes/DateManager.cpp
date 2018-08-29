#include "DateManager.h"

void DataManager::init()
{
}

void DataManager::initGame()
{
	//进入场景时不会初始化关卡
	//level_current = 0;
	num_monster = 0;
	
	money_current = 500;
	if (!is_perfect) {
		hp_current = 5;
	}
	else {
		hp_current = 1;
	}
}

void DataManager::initRecord()
{
	num_kill = RecordManager::GetKeyNum(KILL_KEY);
	num_win = RecordManager::GetKeyNum(WIN_KEY);
	int audio = RecordManager::GetKeyNum(AUDIO_KEY);
	//由于默认值是0,所以这里规定0代表有声音，不是零代表没有声音
	if (audio == 0) {
		is_audio = true;
	}
	else {
		is_audio = true;
	}
}

void DataManager::saveRecord()
{
	RecordManager::SetKeyNum(KILL_KEY, num_kill);
	RecordManager::SetKeyNum(WIN_KEY, num_win);
	int audio = is_audio ? 0 : 1;
	RecordManager::SetKeyNum(AUDIO_KEY, audio);
}

int DataManager::level_current = 0;
int DataManager::num_monster = 0;

int DataManager::money_current = 500;
int DataManager::hp_current = 5;

int DataManager::num_kill = 0;
int DataManager::num_win = 0;
bool DataManager::is_audio = true;

float DataManager::val_stronger = 1.0f;
float DataManager::val_faster = 1.0f;
bool DataManager::is_perfect = false;