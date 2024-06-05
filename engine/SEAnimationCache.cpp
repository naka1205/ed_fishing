#include "SG_Engine/SEAnimationCache.h"
#include "SG_Engine/SEAnimation.h"
#include "SG_Engine/SEFileUtils.h"
#include "SG_Engine/SEDirector.h"
#include "SG_Engine/SESpriteFrameCache.h"
#include "SG_Engine/SEAnimationFrame.h"

#include "SG_Engine/SETexture.h"
#include "SG_Engine/SESurface.h"
#include "SG_Engine/SESpriteFrame.h"
#include "SG_Engine/extensions/SDL_anigif.h"

NS_SDL_BEGIN
AnimationCache *AnimationCache::m_pInstance = nullptr;

AnimationCache::AnimationCache()
{
}
AnimationCache::~AnimationCache()
{
	for (auto iter = _animations.begin(); iter != _animations.end();)
	{
		auto animation = iter->second;
		animation->release();

		iter = _animations.erase(iter);
	}
}
AnimationCache *AnimationCache::getInstance()
{
	if (m_pInstance == nullptr)
	{
		m_pInstance = new AnimationCache();
		m_pInstance->init();
	}
	return m_pInstance;
}
void AnimationCache::purge()
{
	if (m_pInstance)
		SDL_SAFE_RELEASE_NULL(m_pInstance);
}
bool AnimationCache::init()
{
	return true;
}
void AnimationCache::addAnimation(Animation *animation, const std::string &name)
{
	_animations.insert(std::make_pair(name, animation));
	animation->retain();
}
void AnimationCache::removeAnimation(const std::string &name)
{
	auto iter = _animations.find(name);
	if (iter == _animations.end())
		return;
	_animations.erase(iter);

	auto animation = iter->second;
	animation->release();
}
Animation *AnimationCache::getAnimation(const std::string &name)
{
	auto it = _animations.find(name);
	Animation *animation = nullptr;

	if (it != _animations.end())
	{
		animation = it->second;
	}
	return animation;
}
void AnimationCache::addAnimationsWithFile(const std::string &filename)
{
	if (filename.empty())
	{
		LOG("the filename is empty,AnimationCache::addAnimationsWithFile\n");
		return;
	}
	auto valueMap = FileUtils::getInstance()->getValueMapFromFile(filename);

	this->addAnimationsWithValueMap(valueMap, filename);
}

void AnimationCache::addAnimationWithGIF(const std::string &gif, const std::string &name, bool restoreOriginalFrame, int loops)
{
	std::string fullname = FileUtils::getInstance()->fullPathForFilename(gif);
	if (fullname.empty())
	{
		LOG("the filename is empty,AnimationCache::addAnimationsWithGIF\n");
		return;
	}

	int length = AG_LoadGIF(fullname.c_str(), nullptr, 0);
	AG_Frame *ag_frames = new AG_Frame[length];
	AG_LoadGIF(fullname.c_str(), ag_frames, length);

	std::vector<AnimationFrame *> frames(length);
	Renderer *renderer = Director::getInstance()->getRenderer();
	Size originalSize;
	for (int i = 0; i < length; i++)
	{

		Surface *surface = Surface::create(ag_frames[i].surface);
		Texture *texture = Texture::createWithSurface(renderer, surface);
		Size size(ag_frames[i].surface->w, ag_frames[i].surface->h);
		if (i == 0)
			originalSize = size;
		Point offset(ag_frames[i].x, ag_frames[i].y);
		texture = SpriteFrameCache::createTexture(texture, Rect(Point::ZERO, size), offset, originalSize, false);

		SpriteFrame *frame = SpriteFrame::createWithTexture(texture, Rect(Point::ZERO, originalSize));

		float delayUnits = ag_frames[i].delay / 1000.f;
		AnimationFrame *animationFrame = AnimationFrame::create(frame, delayUnits);
		frames[i] = animationFrame;
	}

	Animation *animation = Animation::createWithAnimationFrames(frames, loops);
	animation->setRestoreOriginalFrame(restoreOriginalFrame);
	this->addAnimation(animation, name);
	delete[] ag_frames;
}

void AnimationCache::addAnimationsWithValueMap(const ValueMap &valueMap, const std::string &filename)
{

	if (valueMap.find("animations") == valueMap.end())
	{
		LOG("AnimationCache: No animations were found in provided dictionary\n");
		return;
	}
	const ValueMap &animations = valueMap.at("animations").asValueMap();

	unsigned int version = 1;

	if (valueMap.find("properties") != valueMap.end())
	{
		const ValueMap &properties = valueMap.at("properties").asValueMap();
		version = properties.at("format").asInt();
		const ValueVector &spritesheets = properties.at("spritesheets").asValueVector();
		for (const auto &value : spritesheets)
		{
			std::string path = value.asString();
			Director::getInstance()->getSpriteFrameCache()->addSpriteFramesWithFile(path);
		}
	}
	switch (version)
	{
	case 1:
		this->parseVersion1(animations);
		break;
	case 2:
		this->parseVersion2(animations);
		break;
	}
}
void AnimationCache::parseVersion1(const ValueMap &animations)
{
	auto frameCache = Director::getInstance()->getSpriteFrameCache();
	for (auto iter = animations.cbegin(); iter != animations.cend(); ++iter)
	{

		auto it = _animations.find(iter->first);
		if (it != _animations.end())
			continue;

		const ValueMap &animationDict = iter->second.asValueMap();
		const ValueVector &animationNames = animationDict.at("frames").asValueVector();
		float delay = animationDict.at("delay").asFloat();

		unsigned int loops = 1;
		bool restoreOriginalFrame = false;

		if (animationDict.find("loops") != animationDict.end())
			loops = animationDict.at("loops").asInt();
		if (animationDict.find("restoreOriginalFrame") != animationDict.end())
			restoreOriginalFrame = animationDict.at("restoreOriginalFrame").asBool();

		Animation *animation = nullptr;

		if (animationNames.empty())
		{
			LOG("AnimationCache: Animation '%s' found in dictionary without any frames - cannot add to animation cache.\n", iter->first.c_str());
			continue;
		}

		std::vector<AnimationFrame *> frames;
		for (auto &frameName : animationNames)
		{
			SpriteFrame *spriteFrame = frameCache->getSpriteFrameByName(frameName.asString());
			AnimationFrame *animFrame = AnimationFrame::create(spriteFrame, delay);

			frames.push_back(animFrame);
		}
		animation = Animation::createWithAnimationFrames(frames, loops);
		animation->setRestoreOriginalFrame(restoreOriginalFrame);

		this->addAnimation(animation, iter->first);
	}
}
void AnimationCache::parseVersion2(const ValueMap &animations)
{
}
NS_SDL_END
