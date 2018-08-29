#pragma once
#include "cocos2d.h"
using namespace cocos2d;

#define KILL_KEY ("kill")
#define WIN_KEY ("win")
#define AUDIO_KEY ("audio")

class RecordManager {
public:
	static int GetKeyNum(std::string key);
	static void SetKeyNum(std::string key, int num);
	static void ResetKeyNum();
};