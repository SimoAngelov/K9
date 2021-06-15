#include "Audio.h"
#include <fstream>
#include <iostream>
#include <CSVParser/CSVIterator.h>

namespace K9
{
	Music::Music()
		: m_mapMusic{}, m_strFilePath{ "" }, m_strCurrentMusicID{}
	{
	}

	Music& Music::Ref()
	{
		static Music ref;
		return ref;
	}

	bool Music::Init(const std::string& strFilePath, int nInitialVolume)
	{
		Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048);

		int nFlags = MIX_INIT_OGG | MIX_INIT_MP3;
		if ((Mix_Init(nFlags) & nFlags) != nFlags)
		{
			std::cerr << "MusicLoader::Error Loading SDL_Mix Library because: " << Mix_GetError() << "\n";
			return false;
		}

		m_strFilePath = strFilePath;
		std::ifstream iStream;
		iStream.open(m_strFilePath.c_str());

		if (!iStream.is_open())
		{
			std::cerr << "MusicLoader::Error opening " << m_strFilePath << "\n";
			return false;
		}

		for (CSVIterator it(iStream); it != CSVIterator(); ++it)
		{
			if (it->GetRow() > 0)
			{
				std::string strMusicID = (*it)[0];
				std::string strMusicPath = (*it)[1];

				Mix_Music* ptrMusic = Mix_LoadMUS(strMusicPath.c_str());

				if (ptrMusic == nullptr)
				{
					std::cerr << "MusicLoader::Error! Couldn't load " << strMusicPath <<
						" because " << Mix_GetError() << "\n";
					iStream.close();
					return false;
				}

				std::cout << "MusicLoader::Loaded " << strMusicPath << "\n";

				m_mapMusic.insert(std::pair<std::string, Mix_Music*>(strMusicID, ptrMusic));
			}
		}
		
		iStream.close();
		SetVolume(nInitialVolume);
		return true;
	}


	bool Music::Play(const std::string& strMusicID, int nTimes, int nFadeInMs)
	{
		if (m_mapMusic.find(strMusicID) == m_mapMusic.end())
		{
			return false;
		}

		m_strCurrentMusicID = strMusicID;

		if (!m_mapMusic.at(m_strCurrentMusicID))
		{
			return false;
		}

		if (Mix_PlayMusic(m_mapMusic.at(m_strCurrentMusicID), nTimes) == -1)
		{
			std::cerr << "Mix_FadeInMusic: Couldn't play music on '" << m_strFilePath + "'\n";

			return false;
		}

		std::cout << "Music::Play\n";
		return true;
	}

	void Music::FadeOut(int nMs)
	{
		Mix_FadeOutMusic(nMs);
	}

	void Music::Stop()
	{
		Mix_HaltMusic();
	}

	void Music::Pause()
	{
		Mix_PauseMusic();
	}

	void Music::Unpause()
	{
		Mix_ResumeMusic();
	}

	void Music::Restart()
	{
		Stop();
		Play(m_strCurrentMusicID);
	}

	void Music::Rewind()
	{
		Mix_RewindMusic();
	}

	void Music::SetPosition(double dSeconds)
	{
		Rewind();
		if (Mix_SetMusicPosition(dSeconds) == -1)
		{
			std::cerr << "Mix_SetMusicPosition: Couldn't do that:" <<
				std::string(Mix_GetError()) << "\n";
		}
	}

	bool Music::IsPlaying()
	{
		return (Mix_PlayingMusic() && !IsPaused());
	}

	bool Music::IsPaused()
	{
		return (bool)Mix_PausedMusic();
	}

	bool Music::IsFadingIn()
	{
		return (Mix_FadingMusic() == MIX_FADING_IN);
	}

	bool Music::IsFadingOut()
	{
		return (Mix_FadingMusic() == MIX_FADING_OUT);
	}

	int Music::SetVolume(int nVolume)
	{
		if (nVolume > GetMaxVolume())
		{
			std::cerr << "Mix_VolumeMusic: Tried to overflow volume\n";
			return GetVolume();
		}

		return Mix_VolumeMusic(nVolume);
	}

	int Music::GetVolume()
	{
		return Mix_VolumeMusic(-1);
	}

	int Music::GetMaxVolume()
	{
		return MIX_MAX_VOLUME;
	}

	void Music::Shutdown()
	{
		for (auto& musicPair : m_mapMusic)
		{
			if (musicPair.second)
			{
				Mix_FreeMusic(musicPair.second);
			}
		}

		Mix_Quit();
	}
} // namespace K9

