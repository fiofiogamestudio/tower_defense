#include "AudioManager.h"

void AudioManager::PreLoadAll()
{
	auto audio = SimpleAudioEngine::getInstance();
	audio->preloadBackgroundMusic(SPACE_M);
	audio->preloadBackgroundMusic(BATTLE_M);
	audio->preloadEffect(CLICK_M);
	audio->preloadEffect(CLICK_1_M);
	audio->preloadEffect(ZIZI_M);
	audio->preloadEffect(ZIZI1_M);
	audio->preloadEffect(FIRE_M);
	audio->preloadEffect(WAVE_M);
	audio->preloadEffect(MONSTER_M);
}

void AudioManager::PlayBGM(std::string path,float size)
{
	auto audio = SimpleAudioEngine::getInstance();
	audio->setBackgroundMusicVolume(size);
	audio->playBackgroundMusic(path.c_str(), true);
}

void AudioManager::PlayEffect(std::string path,float size)
{
	auto audio = SimpleAudioEngine::getInstance();
	audio->setEffectsVolume(size);
	audio->playEffect(path.c_str(), false);
}

void AudioManager::StopEffectAll()
{
	SimpleAudioEngine::getInstance()->stopAllEffects();
}
