#pragma once
#include "cocos2d.h"
#include "SimpleAudioEngine.h"
using namespace cocos2d;
using namespace CocosDenshion;

#define SPACE_M ("Audio/space.mp3")
#define BATTLE_M ("Audio/battle.mp3")
#define CLICK_M ("Audio/click.mp3")
#define CLICK_1_M ("Audio/click1.mp3")
#define ZIZI_M ("Audio/zizi.mp3")
#define ZIZI1_M ("Audio/zizi1.mp3")
#define FIRE_M ("Audio/fire.mp3")
#define WAVE_M ("Audio/wave.mp3")
#define MONSTER_M ("Audio/monster.mp3")

class AudioManager {
public:
	static void PreLoadAll();
	static void PlayBGM(std::string path,float size=1.0f);
	static void PlayEffect(std::string path,float size=1.0f);
	static void StopEffectAll();
};