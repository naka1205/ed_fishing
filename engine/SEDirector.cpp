#include "SG_Engine/SEDirector.h"
#include "SG_Engine/SETexture.h"
#include "SG_Engine/SESurface.h"
#include "SG_Engine/SEPoint.h"
#include "SG_Engine/SEScene.h"
#include "SG_Engine/SEPoolManager.h"
#include "SG_Engine/SEInputHandler.h"
#include "SG_Engine/SEEventDispatcher.h"
#include "SG_Engine/SEActionManager.h"
#include "SG_Engine/SEFileUtils.h"
#include "SG_Engine/SETextureCache.h"
#include "SG_Engine/SESpriteFrameCache.h"
#include "SG_Engine/SELabelAtlas.h"
#include "SG_Engine/SEStringUtils.h"
#include "SG_Engine/SENotificationCenter.h"
#include "SG_Engine/SEScheduler.h"
#include "SG_Engine/SEFileUtils.h"
#include "SG_Engine/SESoundManager.h"
#include "SG_Engine/ui/UIWidgetManager.h"
#include "SG_Engine/SEUserDefault.h"
#include "SG_Engine/SELabelDotChar.h"
#include "SG_Engine/SEAnimationCache.h"
#include "SG_Engine/SEFontCache.h"
#include "SG_Engine/SEBMFontConfiguration.h"

NS_SDL_BEGIN
Director *Director::_pInstance = NULL;

Director::Director()
	: _window(nullptr), _renderer(nullptr), _running(true), _runningScene(nullptr), _nextScene(nullptr), _deleteRunningScene(false), _displayStates(true), _secondsPerFrame(1 / 60.f), _lastTick(0), _nextTick(0), _currentFrame(0), _lastFrame(60), _lastTime(0), _totalFrames(0), _renderCount(0), _showFPSLabel(nullptr), _inputHandler(nullptr), _eventDispatcher(nullptr), _actionManager(nullptr), _textureCache(nullptr), _spriteFrameCache(nullptr), _scheduler(nullptr)
{
}

Director::~Director()
{

	SDL_SAFE_RELEASE(_showFPSLabel);

	for (auto iter = _children.begin(); iter != _children.end();)
	{
		auto scene = *iter;

		scene->onExit();
		scene->cleanup();
		scene->release();

		iter = _children.erase(iter);
	}
	_runningScene = nullptr;
	_nextScene = nullptr;

	AnimationCache::purge();

	FileUtils::purge();

	NotificationCenter::purge();

	SoundManager::purge();

	UserDefault::purge();

	BMFontConfiguration::removeFNTCache();

	FontCache::purge();

	ui::UIWidgetManager::purge();

	SDL_SAFE_RELEASE(_inputHandler);
	SDL_SAFE_RELEASE(_eventDispatcher);

	SDL_SAFE_RELEASE(_actionManager);

	SDL_SAFE_RELEASE(_spriteFrameCache);

	SDL_SAFE_RELEASE(_textureCache);

	SDL_SAFE_RELEASE(_scheduler);

	SDL_SAFE_RELEASE(_renderer);
	SDL_SAFE_RELEASE(_window);

	SDL_Quit();
}

bool Director::init(const char *title, int x, int y, int width, int height, Uint32 flags)
{

	if (SDL_Init(SDL_INIT_TIMER | SDL_INIT_AUDIO | SDL_INIT_VIDEO | SDL_INIT_EVENTS) != 0)
	{
		LOG("%s", SDL_GetError());
		return false;
	}
	_window = Window::create(title, x, y, width, height, flags);
	_renderer = Renderer::create(_window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_TARGETTEXTURE | SDL_RENDERER_PRESENTVSYNC);

	_window->retain();
	_renderer->retain();

	FileUtils::getInstance()->setAppString(title);

	_inputHandler = new InputHandler();

	_eventDispatcher = new EventDispatcher();

	_actionManager = new ActionManager();

	_textureCache = new TextureCache();

	_spriteFrameCache = new SpriteFrameCache();

	_scheduler = new Scheduler();

	srand((unsigned int)time(NULL));

	printf("the window initlize successfully\n");

	return true;
}

void Director::update()
{
	Uint32 currentTick = SDL_GetTicks();
	if (currentTick >= _nextTick)
	{
		++_totalFrames;
		++_currentFrame;

		float dt = _secondsPerFrame; //(_nextTick - _lastTick)/1000.f;
		_lastTick = currentTick;	 //_nextTick;
		_nextTick = _lastTick + (Uint32)(_secondsPerFrame * 1000);

		this->drawScene(dt);
	}

	handleScene();

	updateState();
}

std::string Director::getPlatform() const
{
	return std::string(SDL_GetPlatform());
}

void Director::runWithScene(Scene *pScene)
{
	SDLASSERT(pScene != NULL, "the pScene should not null");
	pushScene(pScene);
}

void Director::replaceScene(Scene *pScene)
{
	SDLASSERT(pScene != NULL, "the pScene should not null");

	if (_runningScene == nullptr)
	{
		runWithScene(pScene);
		_deleteRunningScene = false;

		return;
	}
	if (_runningScene == pScene)
		return;

	pScene->retain();
	_deleteRunningScene = true;

	_children.pop_back();
	_children.push_back(pScene);
	_nextScene = pScene;
}

void Director::pushScene(Scene *pScene)
{
	SDLASSERT(pScene != NULL, "pScene should not null");

	pScene->retain();
	_children.push_back(pScene);
	_nextScene = pScene;
	_deleteRunningScene = false;
}

void Director::popScene()
{
	SDLASSERT(_runningScene != NULL, "the running scene should not null");
	int size = _children.size();

	if (size == 1)
	{
		_running = false;
	}
	else
	{
		_children.pop_back();

		_deleteRunningScene = true;
		_nextScene = _children.back();
	}
}

void Director::purge()
{

	SDL_SAFE_RELEASE_NULL(_pInstance);
}

void Director::setSecondsPerFrame(float secondPerFrame)
{
	_secondsPerFrame = secondPerFrame;
}

void Director::setDisplayStates(bool state)
{
	_displayStates = state;
}

int Director::renderSetScale(float scaleX, float scaleY)
{
	return _renderer->setScale(scaleX, scaleY);
}

int Director::setResolutionScale()
{
	Size originalSize = _window->getWindowSize();
	Size realSize = _window->getRealWindowSize();

	if (originalSize == realSize)
		return 0;

	Size scale = realSize / originalSize;

	return _renderer->setScale(scale.width, scale.height);
}

int Director::setLogicalSize()
{
	Size originalSize = _window->getWindowSize();
	Size realSize = _window->getRealWindowSize();

	if (originalSize == realSize)
		return 0;

	int w = (int)realSize.width;
	int h = (int)realSize.height;

	return _renderer->setLogicalSize(w, h);
}

void Director::handleScene()
{
	if (_nextScene)
	{
		if (_runningScene)
		{
			_runningScene->onExit();
			_runningScene->cleanup();
		}
		if (_deleteRunningScene)
		{
			_deleteRunningScene = false;
			_runningScene->release();
		}

		_runningScene = _nextScene;

		_runningScene->onEnter();

		_nextScene = nullptr;
	}
}

void Director::drawScene(float dt)
{

	if (_runningScene)
	{

		_renderer->clear();

		_actionManager->update(dt);

		_runningScene->visit();

		if (_showFPSLabel && _displayStates)
			_showFPSLabel->draw();

		_renderCount = _renderer->render();

		_scheduler->update(dt);

		_inputHandler->update();

		_renderer->present();
	}
}

void Director::updateState()
{
	if (SDL_GetTicks() - _lastTime >= 1000)
	{

		_lastFrame = _currentFrame;
		_currentFrame = 0;
		_lastTime = SDL_GetTicks();

		if (_displayStates)
		{
			if (_showFPSLabel == nullptr)
			{

				_showFPSLabel = LabelDotChar::create("FPS:60", 3, Color3B(0, 0, 0));

				_showFPSLabel->setAnchorPoint(Point::ZERO);
				_showFPSLabel->retain();
			}
			std::string text = StringUtils::format("FPS:%d/%d", _lastFrame, _renderCount);
			_showFPSLabel->setString(text);
			// printf("FPS:%d/%d\n", _lastFrame, _renderCount);
		}
	}
}

NS_SDL_END
