#ifndef __SDL_AnimationCache_H__
#define __SDL_AnimationCache_H__

#include <string>
#include <vector>
#include <unordered_map>
#include "SEObject.h"
#include "SEValue.h"

NS_SDL_BEGIN

class Animation;

class AnimationCache : public Object
{
private:
	static AnimationCache *m_pInstance;
	std::unordered_map<std::string, Animation *> _animations;

private:
	AnimationCache();
	~AnimationCache();

public:
	static AnimationCache *getInstance();
	static void purge();
	bool init();

	void addAnimation(Animation *animation, const std::string &name);

	void removeAnimation(const std::string &name);

	Animation *getAnimation(const std::string &name);

	void addAnimationsWithFile(const std::string &plist);

	void addAnimationWithGIF(const std::string &gif, const std::string &name, bool restoreOriginalFrame = true, int loops = -1);

	void addAnimationsWithValueMap(const ValueMap &valueMap, const std::string &plist);

private:
	void parseVersion1(const ValueMap &animations);
	void parseVersion2(const ValueMap &animations);
};
NS_SDL_END
#endif
