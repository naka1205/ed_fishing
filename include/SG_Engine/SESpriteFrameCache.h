#ifndef __SDL_SpriteFrameCache_H__
#define __SDL_SpriteFrameCache_H__
#include <map>
#include <string>
#include <unordered_map>
#include <memory>
#include <cstdlib>
#include <vector>

#include "SG_Engine/SEObject.h"
#include "SG_Engine/SERect.h"
#include "SG_Engine/SESize.h"
#include "SG_Engine/SEPoint.h"

#include "rapidxml/rapidxml.hpp"
#include "rapidxml/rapidxml_utils.hpp"

NS_SDL_BEGIN
class Texture;
class SpriteFrame;

class SpriteFrameCache : public Object
{
private:
	std::map<std::string, SpriteFrame *> _spriteFrames;

public:
	SpriteFrameCache();
	virtual ~SpriteFrameCache();

	bool addSpriteFramesWithFile(const std::string &xml, const std::string &textureFile = "");
	bool addSpriteFramesWithFile(const std::string &xml, Texture *texture);

	SpriteFrame *getSpriteFrameByName(const std::string &name) const;
	void removeSpriteFrameByName(const std::string &name);

	void removeSpriteFramesByFileName(const std::string &filePath);

	static Texture *createTexture(Texture *texture, const Rect &srcRect, const Point &offset, const Size &originalSize, bool rotated);

private:
	bool addSpriteFramesWithFile(rapidxml::xml_node<> *root, Texture *texture);
};
NS_SDL_END
#endif
