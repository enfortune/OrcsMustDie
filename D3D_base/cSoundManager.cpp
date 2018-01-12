#include "stdafx.h"
#include "cSoundManager.h"


cSoundManager::cSoundManager()
{
}


cSoundManager::~cSoundManager()
{
	this->Destroy();
}


HRESULT cSoundManager::Setup()
{
	//FMOD 사운드 엔진을 사용하겠다고 선언
	System_Create(&m_pSystem);

	m_pSystem->init(TOTALSOUNDBUFFER, FMOD_INIT_NORMAL, NULL);

	m_ppSound = new Sound*[TOTALSOUNDBUFFER];
	m_pChannel = new Channel*[TOTALSOUNDBUFFER];

	memset(m_ppSound, 0, sizeof(Sound*) * (TOTALSOUNDBUFFER));
	memset(m_pChannel, 0, sizeof(Channel*) * (TOTALSOUNDBUFFER));

	return S_OK;
}

void cSoundManager::Update()
{
	m_pSystem->update();
}

void cSoundManager::Destroy()
{
	//사운드 삭제
	if (m_pChannel != NULL || m_ppSound != NULL)
	{
		for (int i = 0; i < TOTALSOUNDBUFFER; i++)
		{
			if (m_pChannel != NULL)
			{
				if (m_pChannel[i]) m_pChannel[i]->stop();
			}

			if (m_ppSound != NULL)
			{
				if (m_ppSound != NULL) m_ppSound[i]->release();
			}
		}
	}

	//메모리 지우기
	SAFE_DELETE_ARRAY(m_pChannel);
	SAFE_DELETE_ARRAY(m_ppSound);

	//시스템 닫기 
	if (m_pSystem != NULL)
	{
		m_pSystem->release();
		m_pSystem->close();
	}
}


HRESULT cSoundManager::AddSound(std::string keyName, std::string soundName, bool bgm, bool loop)
{
	if (m_mapTotalSounds.size() != m_mapTotalChannels.size()) return E_FAIL;

	if (loop)
	{
		if (bgm)
		{
			m_pSystem->createStream(soundName.c_str(), FMOD_LOOP_NORMAL, NULL, &m_ppSound[m_mapTotalSounds.size()]);
		}
		else
		{
			m_pSystem->createSound(soundName.c_str(), FMOD_LOOP_NORMAL, NULL, &m_ppSound[m_mapTotalSounds.size()]);
		}
	}
	else
	{
		if (bgm)
		{
			m_pSystem->createStream(soundName.c_str(), FMOD_DEFAULT, NULL, &m_ppSound[m_mapTotalSounds.size()]);
		}
		else
		{
			m_pSystem->createSound(soundName.c_str(), FMOD_DEFAULT, NULL, &m_ppSound[m_mapTotalSounds.size()]);
		}
	}

	m_mapTotalSounds.insert(make_pair(keyName, &m_ppSound[m_mapTotalSounds.size()]));
	m_mapTotalChannels.insert(make_pair(keyName, &m_pChannel[m_mapTotalChannels.size()]));

	return S_OK;
}

void cSoundManager::Play(std::string keyName, float volume)// 0.0 ~ 1.0f -> 0 ~ 255
{
	/*arrSoundsIter iter = m_mapTotalSounds.begin();

	int count = 0;

	for (iter; iter != m_mapTotalSounds.end(); ++iter, count++)
	{
	if (keyName == iter->first)
	{
	m_pSystem->playSound(FMODm_pChannel_FREE, *iter->second, false, &m_pChannel[count]);

	m_pChannel[count]->setVolume(volume);
	break;
	}
	}*/

	if (m_mapTotalSounds.find(keyName) != m_mapTotalSounds.end())
	{
		m_pSystem->playSound(FMOD_CHANNEL_FREE, *m_mapTotalSounds.find(keyName)->second, false, m_mapTotalChannels.find(keyName)->second);

		(*m_mapTotalChannels.find(keyName)->second)->setVolume(volume);
	}
}

void cSoundManager::PlayPosition(std::string keyName, unsigned int position, FMOD_TIMEUNIT postype, float volume)
{
	/*arrSoundsIter iter = m_mapTotalSounds.begin();

	int count = 0;

	for (iter; iter != m_mapTotalSounds.end(); ++iter, count++)
	{
	if (keyName == iter->first)
	{
	m_pSystem->playSound(FMODm_pChannel_FREE, *iter->second, false, &m_pChannel[count]);
	m_pChannel[count]->setPosition(position, postype);
	m_pChannel[count]->setVolume(volume);
	break;
	}
	}*/

	if (m_mapTotalSounds.find(keyName) != m_mapTotalSounds.end())
	{
		m_pSystem->playSound(FMOD_CHANNEL_FREE, *m_mapTotalSounds.find(keyName)->second, false, m_mapTotalChannels.find(keyName)->second);
		(*m_mapTotalChannels.find(keyName)->second)->setPosition(position, postype);
		(*m_mapTotalChannels.find(keyName)->second)->setVolume(volume);
	}

}

void cSoundManager::Stop(std::string keyName)
{
	/*arrSoundsIter iter = m_mapTotalSounds.begin();

	int count = 0;

	for (iter; iter != m_mapTotalSounds.end(); ++iter, count++)
	{
	if (keyName == iter->first)
	{
	m_pChannel[count]->stop();
	break;
	}
	}*/

	if (m_mapTotalSounds.find(keyName) != m_mapTotalSounds.end())
	{
		(*m_mapTotalChannels.find(keyName)->second)->stop();
	}
}

void cSoundManager::Pause(std::string keyName)
{
	/*arrSoundsIter iter = m_mapTotalSounds.begin();

	int count = 0;

	for (iter; iter != m_mapTotalSounds.end(); ++iter, count++)
	{
	if (keyName == iter->first)
	{
	m_pChannel[count]->setPaused(true);
	break;
	}
	}*/

	if (m_mapTotalSounds.find(keyName) != m_mapTotalSounds.end())
	{
		(*m_mapTotalChannels.find(keyName)->second)->setPaused(true);
	}
}

void cSoundManager::Resume(std::string keyName)
{
	/*arrSoundsIter iter = m_mapTotalSounds.begin();

	int count = 0;

	for (iter; iter != m_mapTotalSounds.end(); ++iter, count++)
	{
	if (keyName == iter->first)
	{
	m_pChannel[count]->setPaused(false);
	break;
	}
	}*/

	if (m_mapTotalSounds.find(keyName) != m_mapTotalSounds.end())
	{
		(*m_mapTotalChannels.find(keyName)->second)->setPaused(false);
	}
}

void cSoundManager::SetFrequency(std::string keyName, float freq)
{
	if (m_mapTotalSounds.find(keyName) != m_mapTotalSounds.end())
	{
		(*m_mapTotalChannels.find(keyName)->second)->setFrequency(freq);
	}
}

float cSoundManager::GetFrequency(std::string keyName)
{
	if (m_mapTotalSounds.find(keyName) != m_mapTotalSounds.end())
	{
		float freq;
		(*m_mapTotalChannels.find(keyName)->second)->getFrequency(&freq);
		return freq;
	}

	return 0.0f;
}

void cSoundManager::ChangeFrequency(std::string keyName, float mag)
{
	if (m_mapTotalSounds.find(keyName) != m_mapTotalSounds.end())
	{
		float freq;
		(*m_mapTotalChannels.find(keyName)->second)->getFrequency(&freq);
		(*m_mapTotalChannels.find(keyName)->second)->setFrequency(freq * mag);
	}
}

void cSoundManager::SetVolume(std::string keyName, float volume)
{
	if (m_mapTotalSounds.find(keyName) != m_mapTotalSounds.end())
	{
		(*m_mapTotalChannels.find(keyName)->second)->setVolume(volume);
	}
}

float cSoundManager::GetLength(std::string keyName)
{
	if (m_mapTotalSounds.find(keyName) != m_mapTotalSounds.end())
	{
		unsigned int n;
		(*m_mapTotalSounds.find(keyName)->second)->getLength(&n, FMOD_TIMEUNIT_MS);
		return static_cast<float>(n) / 1000;
	}
	return 0;
}

bool cSoundManager::IsPlaySound(std::string keyName)
{
	bool isPlay = false;
	/*arrSoundsIter iter = m_mapTotalSounds.begin();

	int count = 0;

	for (iter; iter != m_mapTotalSounds.end(); ++iter, count++)
	{
	if (keyName == iter->first)
	{
	m_pChannel[count]->isPlaying(&isPlay);
	break;
	}
	}*/

	if (m_mapTotalSounds.find(keyName) != m_mapTotalSounds.end())
	{
		(*m_mapTotalChannels.find(keyName)->second)->isPlaying(&isPlay);
	}

	return isPlay;
}

bool cSoundManager::IsPauseSound(std::string keyName)
{
	bool isPause;
	/*arrSoundsIter iter = m_mapTotalSounds.begin();

	int count = 0;

	for (iter; iter != m_mapTotalSounds.end(); ++iter, count++)
	{
	if (keyName == iter->first)
	{
	m_pChannel[count]->getPaused(&isPause);
	break;
	}
	}*/

	if (m_mapTotalSounds.find(keyName) != m_mapTotalSounds.end())
	{
		(*m_mapTotalChannels.find(keyName)->second)->getPaused(&isPause);
	}

	return isPause;
}

