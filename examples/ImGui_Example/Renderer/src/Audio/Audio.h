#pragma once
#include <map>
#include <string>
#include <SDL_mixer.h>

namespace K9
{
	/* Loads and plays music. */
	class Music
	{
	public:
		/** Delete the copy constructor, move constructor and assignment operators. */
		Music(const Music&) = delete;
		Music(Music&&) = delete;
		Music& operator=(const Music&) = delete;
		Music& operator=(Music&) = delete;

		/// <summary>
		/// Return a static reference to the singleton instance.
		/// </summary>
		/// <returns> A singleton instance. </returns>
		static Music& Ref();

		/* Load Music from strFilePath csv file. */
		bool Init(const std::string& strFilePath,
				int nInitialVolume = MIX_MAX_VOLUME / 2);

		/*Plays the loaded music for `times` with optional `fadeInMs`.
			@param times How many times we'll play the music. If -1, infinite loop.
			@param fadeInMs How many milliseconds for the fade-in effect to complete.
			@note Overwrites any other music that may be playing */
		bool Play(const std::string& strMusicID, int nTimes = -1, int nFadeInMs = 0);

		/* Starts fading out the currently playing song, lasting  for `ms` milliseconds. */
		void FadeOut(int nMs);

		/* Stops the music (along with all musics currently playing) */
		void Stop();

		/* Pauses the music (along with all musics currently playing) */
		void Pause();

		/* Unpauses the music (along with all musics currently playing */
		void Unpause();

		/* Starts playing the music from the beginning again */
		void Restart();

		/* Rewinds the music to the start.
		Works both on already-playing and paused songs. */
		void Rewind();

		/* Sets the position of the _playing_ music to `seconds
			@note - only works for *.ogg and *.mp3 */
		void SetPosition(double seconds);

		/* Tells if the music is playing right now. */
		bool IsPlaying();

		/* Tells if the music is paused right now. */
		bool IsPaused();

		/* Tells if the music is fading in right now. */
		bool IsFadingIn();

		/* Tells if the music is fading out right now. */
		bool IsFadingOut();

		/*  Set the music's volume to `volume`.
			@return The previous volume */
		int SetVolume(int nVolume);

		/* Returns the current volume */
		int GetVolume();

		/* Returns the maximum value allowed for the volume.
		 @note Minimum is always zero. */
		int GetMaxVolume();

		/* Free loaded music */
		void Shutdown();

	private:
		Music();
		virtual ~Music() = default;

	private:
		/* Map, holding the music for the game
			key - music ID
			value - internal SDL2_mixer's data structure that handles music*/
		std::map<std::string, Mix_Music*> m_mapMusic;

		/* The file path of the currently loaded music */
		std::string m_strFilePath;

		/* ID of the music currently being played */
		std::string m_strCurrentMusicID;
	};
} // namespace K9