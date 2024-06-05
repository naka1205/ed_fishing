#include "SG_Engine.h"
#include "Asset.hpp"
#include "GameScene.h"
#include "StartScene.h"
#include "LoadingLayer.h"
#include "FlashScene.h"

std::function<void()> registered_loop;
void main_loop()
{
	registered_loop();
}

char *emscripten_run_script_string(const char *script);

int main(int argc, char *argv[])
{

	if (!Director::getInstance()->init("Fishing", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 960, 640, SDL_WINDOW_SHOWN))
	{
		return 0;
	}

	FileUtils::getInstance()->addSearchPath(RESOURCES_DIR);

	SDL_SetHint(SDL_HINT_RENDER_VSYNC, "1");

	Director::getInstance()->runWithScene(FlashScene::create());
	Director::getInstance()->setDisplayStates(true);
	Director::getInstance()->setSecondsPerFrame(1 / 60.f);
	Director::getInstance()->setResolutionScale();

	registered_loop = [&]()
	{
		Director::getInstance()->update();
		PoolManager::getInstance()->getCurReleasePool()->clear();
	};

#ifdef __EMSCRIPTEN__
	emscripten_set_main_loop(main_loop, 0, 1);
#else
	while (Director::getInstance()->isRunning())
	{
		main_loop();
	}

	GameScene::purge();

	Director::getInstance()->purge();
	PoolManager::getInstance()->purge();

#endif

	return 1;
}
