#pragma once

#include "fmod/inc/fmod.hpp"
#pragma comment(lib, "fmod/lib/fmodex_vc.lib")

using namespace FMOD;

#define SOUNDBUFFER 60
#define EXTRACHANNELBUFFER 60
#define TOTALSOUNDBUFFER SOUNDBUFFER + EXTRACHANNELBUFFER

#define g_pSoundManager cSoundManager::GetInstance()

class cSoundManager
{
	SINGLETONE(cSoundManager);
private:
	typedef std::map<std::string, Sound**> arrSounds;
	typedef std::map<std::string, Sound**>::iterator arrSoundsIter;
	typedef std::map<std::string, Channel**> arrChannels;
	typedef std::map<std::string, Channel**>::iterator arrChannelsIter;

private:
	System* m_pSystem;
	Sound** m_ppSound;
	Channel** m_pChannel;

	arrSounds m_mapTotalSounds;
	arrChannels m_mapTotalChannels;

public:
	HRESULT Setup();
	void Update();

	HRESULT AddSound(std::string keyName, std::string soundName, bool bgm, bool loop);
	void Play(std::string keyName, float volume = 1.0f); // 0.0 ~ 1.0f -> 0 ~ 255
	void PlayPosition(std::string keyName, unsigned int position, FMOD_TIMEUNIT postype = FMOD_TIMEUNIT_MS, float volume = 1.0f);
	void Stop(std::string keyName);
	void Pause(std::string keyName);
	void Resume(std::string keyName);

	void SetFrequency(std::string keyName, float freq);
	float GetFrequency(std::string keyName);
	void ChangeFrequency(std::string keyName, float mag);

	void SetVolume(std::string keyName, float volume);

	float GetLength(std::string keyName);

	bool IsPlaySound(std::string keyName);
	bool IsPauseSound(std::string keyName);
};

