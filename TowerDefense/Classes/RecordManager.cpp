#include "RecordManager.h"


int RecordManager::GetKeyNum(std::string key)
{
	auto record = UserDefault::getInstance();
	int num = record->getIntegerForKey(key.c_str());
	return num;
}

void RecordManager::SetKeyNum(std::string key, int num)
{
	auto record = UserDefault::getInstance();
	record->setIntegerForKey(key.c_str(), num);
}

void RecordManager::ResetKeyNum()
{
	SetKeyNum(KILL_KEY, 0);
	SetKeyNum(WIN_KEY, 0);
}
