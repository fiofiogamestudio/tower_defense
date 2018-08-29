#pragma once
#include "cocos2d.h"
#include "SimpleAudioEngine.h"
#include "DateManager.h"
using namespace cocos2d;
using namespace CocosDenshion;

#define SPACE_M ("Audio/space.wav")
#define BATTLE_M ("Audio/battle.wav")
#define CLICK_M ("Audio/click.wav")
#define CLICK_1_M ("Audio/click1.wav")
#define ZIZI_M ("Audio/zizi.wav")
#define ZIZI1_M ("Audio/zizi1.wav")
#define FIRE_M ("Audio/fire.wav")
#define WAVE_M ("Audio/wave.wav")
#define MONSTER_M ("Audio/monster.wav")

class AudioManager {
public:
	static void PreLoadAll();
	static void PlayBGM(std::string path,float size=1.0f);
	static void PlayEffect(std::string path,float size=1.0f);
	static void StopEffectAll();

	static void CloseAudio();
	static void OpenAudio();
};