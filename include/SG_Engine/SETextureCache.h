#ifndef __SDL_TextureCache_H__
#define __SDL_TextureCache_H__

#include <deque>
#include <mutex>
#include <string>
#include <thread>
#include <functional>
#include <unordered_map>
#include <condition_variable>

#include "SG_Engine/SEObject.h"
NS_SDL_BEGIN
class Texture;

class TextureCache : public Object
{
protected:
	struct AsyncStruct;

private:
	std::unordered_map<std::string, Texture *> _textures;
	std::thread *_loadingThread;
	bool _needQuit;
	int _asyncRefCount;
	std::deque<AsyncStruct *> _asyncStructQueue;
	std::deque<AsyncStruct *> _requestQueue;
	std::deque<AsyncStruct *> _responseQueue;
	std::mutex _requestMutex;
	std::mutex _responseMutex;

	std::condition_variable _sleepCondition;

public:
	TextureCache();
	~TextureCache();

	Texture *addImage(const std::string &filename);
	Texture *addImage(const std::string &filename, const std::string &key);
	void addImageAsync(const std::string &filename, const std::function<void(Texture *)> &callback);

	Texture *addTexture(Texture *texture, const std::string &key);

	Texture *getTextureForKey(const std::string &key) const;

	std::string getTextureFilePath(Texture *texture) const;

	void removeUnusedTextures();

	void removeTextureForKey(const std::string &key);
	void removeAllTextures();
	void waitForQuit();

private:
	void loadImage();
	void addImageAsyncCallback(float dt);
};
NS_SDL_END
#endif
