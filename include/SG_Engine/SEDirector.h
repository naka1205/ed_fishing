#ifndef __SDL_Director_H__
#define __SDL_Director_H__

#include <vector>
#include <algorithm>
#include <ctime>
#include <cstdlib>

#include "SDL2/SDL.h"

#include "SEWindow.h"
#include "SERenderer.h"
#include "SESize.h"
#include "SERect.h"

NS_SDL_BEGIN
class Scene;
class InputHandler;
class EventDispatcher;
class ActionManager;
class TextureCache;
class SpriteFrameCache;
class LabelDotChar;
class Scheduler;

class Director : public Object
{
private:
	static Director *_pInstance;

private:
	Window *_window;
	Renderer *_renderer;

	bool _running;

	std::vector<Scene *> _children;

	Scene *_runningScene;

	Scene *_nextScene;

	bool _deleteRunningScene;

	bool _displayStates;

	float _secondsPerFrame;
	Uint32 _lastTick;
	Uint32 _nextTick;

	int _currentFrame;
	int _lastFrame;
	long _lastTime;
	unsigned int _totalFrames;

	unsigned int _renderCount;

	LabelDotChar *_showFPSLabel;

	InputHandler *_inputHandler;
	EventDispatcher *_eventDispatcher;

	ActionManager *_actionManager;

	TextureCache *_textureCache;

	SpriteFrameCache *_spriteFrameCache;

	Scheduler *_scheduler;

private:
	Director();
	~Director();

public:
	inline static Director *getInstance()
	{
		if (_pInstance == nullptr)
		{
			_pInstance = new Director();
		}
		return _pInstance;
	}
	static void purge();

	bool init(const char *title, int x, int y, int width, int height, Uint32 flags);
	void update();

	bool isRunning() const { return _running; }
	void setRunning(bool running) { _running = running; }

	std::string getPlatform() const;

	void runWithScene(Scene *pScene);
	void replaceScene(Scene *pScene);
	void pushScene(Scene *pScene);
	void popScene();

	void setSecondsPerFrame(float secondPerFrame);
	void setDisplayStates(bool state);

	int renderSetScale(float scaleX, float scaleY);

	int setResolutionScale();

	int setLogicalSize();

	unsigned int getTotalFrames() const
	{
		return _totalFrames;
	}
	inline Window *getWindow() const
	{
		return _window;
	}
	inline Renderer *getRenderer() const
	{
		return _renderer;
	}
	inline EventDispatcher *getEventDispatcher() const
	{
		return _eventDispatcher;
	}
	inline ActionManager *getActionManager() const
	{
		return _actionManager;
	}
	inline TextureCache *getTextureCache() const
	{
		return _textureCache;
	}
	inline SpriteFrameCache *getSpriteFrameCache() const
	{
		return _spriteFrameCache;
	}
	inline Scheduler *getScheduler() const
	{
		return _scheduler;
	}
	Size getVisibleSize() const
	{
		return _window->getWindowSize();
	}

private:
	void handleScene();

	void drawScene(float dt);

	void updateState();
};
NS_SDL_END
#endif
