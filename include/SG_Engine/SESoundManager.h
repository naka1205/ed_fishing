#ifndef __SDL_SoundManager_H__
#define __SDL_SoundManager_H__
#include <unordered_map>
#include <string>
#include <functional>
#include <thread>
#include <mutex>
#include <deque>
#include <condition_variable>

#include "SDL_mixer.h"

#include "SG_Engine/SEObject.h"

NS_SDL_BEGIN
class Music;
class Chunk;

class SoundManager : public Object
{
protected:
	struct AsyncStruct;

private:
	static SoundManager *m_pInstance;

private:
	std::unordered_map<std::string, Music *> _musics;
	std::unordered_map<std::string, Chunk *> _effects;
	int _effectVolume;
	std::thread *_loadingThread;
	bool _needQuit;
	int _asyncRefCount;
	std::deque<AsyncStruct *> _asyncStructQueue;
	std::deque<AsyncStruct *> _requestQueue;
	std::deque<AsyncStruct *> _responseQueue;
	std::mutex _requestMutex;
	std::mutex _responseMutex;

	std::condition_variable _sleepCondition;

	bool _playingMusic;
	bool _playingChunk;

	std::string _playingMusicName;
	int _playingMusicLoops;

public:
	SoundManager();
	~SoundManager();

public:
	static SoundManager *getInstance();
	static void purge();

	Music *preloadBackgroundMusic(const std::string &file);

	int playBackgroundMusic(const std::string &file, int loops);

	int stopBackgroundMusic() const;

	int pauseBackgroundMusic() const;

	void resumeBackgroundMusic();

	void rewindBackgroundMusic();

	int isBackgroundPlaying() const;

	int getBackgroundMusicVolume() const;

	int setBackgroundMusicVolume(int volume) const;

	Chunk *preloadEffect(const std::string &file);

	void unloadEffect(const std::string &file);

	int playEffect(const std::string &file, int loops);

	void pauseEffect(int channel);

	void pauseAllEffect();

	void resumeEffect(int channel);

	void resumeAllEffect();

	int stopEffect(int channel);

	int stopAllEffect();

	int isEffectPlaying(int channel) const;

	int getEffectVolume() const;
	void setEffectVolume(int volume);

	void addSoundAsync(const std::string &filename, const std::string &type, std::function<void(Object *data, const std::string &)> callback);
	void waitForQuit();

	bool isPlayingMusic() const;
	void setPlayingMusic(bool var);

	bool isPlayingChunk() const;
	void setPlayingChunk(bool var);

private:
	void loadSound();
	void addSoundAsyncCallback(float dt);
};
NS_SDL_END
#endif
