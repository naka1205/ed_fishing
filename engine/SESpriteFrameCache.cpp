#include "SG_Engine/SESpriteFrameCache.h"
#include "SG_Engine/SESpriteFrame.h"
#include "SG_Engine/SEDirector.h"
#include "SG_Engine/SETexture.h"
#include "SG_Engine/SESurface.h"
#include "SG_Engine/SEStringUtils.h"
#include "SG_Engine/SETextureCache.h"
#include "SG_Engine/SEFileUtils.h"
#include "SG_Engine/SERenderer.h"

NS_SDL_BEGIN
SpriteFrameCache::SpriteFrameCache()
{
}

SpriteFrameCache::~SpriteFrameCache()
{
	for (auto it = _spriteFrames.begin(); it != _spriteFrames.end();)
	{
		auto spriteFrame = it->second;
		++it;
		SDL_SAFE_DELETE(spriteFrame);
	}
	_spriteFrames.clear();
}

bool SpriteFrameCache::addSpriteFramesWithFile(const std::string &xml, const std::string &textureFile)
{
	bool bRet = false;

	std::string texturePath;
	auto fullPath = FileUtils::getInstance()->fullPathForFilename(xml);

	if (fullPath.empty())
		return false;

	std::unique_ptr<char> chars = FileUtils::getInstance()->getUniqueDataFromFile(fullPath);

	std::unique_ptr<char> points = std::move(chars);

	auto text = points.get();

	rapidxml::xml_document<> doc;

	doc.parse<0>(text);
	rapidxml::xml_node<> *root = doc.first_node();

	if (textureFile.empty())
	{
		texturePath = root->first_attribute("imagePath")->value();

		texturePath = FileUtils::getInstance()->fullPathForFilename(texturePath);

		if (texturePath.empty())
		{
			texturePath = xml;
			size_t startPos = texturePath.find_last_of('.');
			texturePath.erase(startPos);
			texturePath.append(".png");
		}
	}
	else
		texturePath = textureFile;

	auto texture = Director::getInstance()->getTextureCache()->addImage(texturePath);
	if (texture == nullptr)
		bRet = false;
	else
	{
		bRet = this->addSpriteFramesWithFile(root, texture);
	}

	points.reset();

	return bRet;
}

bool SpriteFrameCache::addSpriteFramesWithFile(const std::string &xml, Texture *texture)
{
	bool bRet = false;

	auto fullPath = FileUtils::getInstance()->fullPathForFilename(xml);
	if (fullPath.empty())
		return false;
	std::unique_ptr<char> points = std::move(FileUtils::getInstance()->getUniqueDataFromFile(fullPath));
	auto text = points.get();

	rapidxml::xml_document<> doc;

	doc.parse<0>(text);
	rapidxml::xml_node<> *root = doc.first_node();

	bRet = this->addSpriteFramesWithFile(root, texture);

	points.reset();

	return bRet;
}

bool SpriteFrameCache::addSpriteFramesWithFile(rapidxml::xml_node<> *root, Texture *texture)
{
	std::string texturePath;

	texturePath = Director::getInstance()->getTextureCache()->getTextureFilePath(texture);

	for (auto node = root->first_node(); node; node = node->next_sibling())
	{
		std::string name = node->first_attribute("n")->value();
		int x = atoi(node->first_attribute("x")->value());
		int y = atoi(node->first_attribute("y")->value());
		int w = atoi(node->first_attribute("w")->value());
		int h = atoi(node->first_attribute("h")->value());

		bool rotated = false;
		auto r = node->first_attribute("r");

		if (r)
		{
			rotated = true;
		}

		int oX = node->first_attribute("oX") == nullptr ? 0 : atoi(node->first_attribute("oX")->value());
		int oY = node->first_attribute("oY") == nullptr ? 0 : atoi(node->first_attribute("oY")->value());

		int oW = 0, oH = 0;
		if (node->first_attribute("oW"))
			oW = atoi(node->first_attribute("oW")->value());
		else if (r)
			oW = h;
		else
			oW = w;
		if (node->first_attribute("oH"))
			oH = atoi(node->first_attribute("oH")->value());
		else if (r)
			oH = w;
		else
			oH = h;

		Point anchorPoint(0.5f, 0.5f);
		if (node->first_attribute("pX") != nullptr)
			anchorPoint.x = atof(node->first_attribute("pX")->value());
		if (node->first_attribute("pY") != nullptr)
			anchorPoint.y = atof(node->first_attribute("pY")->value());

		if (_spriteFrames.find(name) == _spriteFrames.end())
		{
			auto realTexture = this->createTexture(texture, Rect((float)x, (float)y, (float)w, (float)h), Point((float)oX, (float)oY), Size((float)oW, (float)oH), rotated);

			Director::getInstance()->getTextureCache()->addTexture(realTexture, name);

			SpriteFrame *spriteFrame = SpriteFrame::createWithTexture(
				realTexture,
				Rect(0.f, 0.f, (float)oW, (float)oH),
				rotated,
				Point((float)oX, (float)oY),
				Size((float)oW, (float)oH),
				anchorPoint);

			spriteFrame->setTextureFilename(texturePath);
			spriteFrame->setTexturePackerRect(Rect(oX, oY, w, h));

			_spriteFrames.insert(std::make_pair(name, spriteFrame));

			spriteFrame->retain();
		}
		else
		{
#ifdef SDL_ENGINE_DEBUG
			printf("该资源已经被加载:%s", name.c_str());
#endif
		}
	}
	return true;
}
SpriteFrame *SpriteFrameCache::getSpriteFrameByName(const std::string &name) const
{
	auto it = _spriteFrames.find(name);
	if (it != _spriteFrames.end())
		return it->second;
	return nullptr;
}
void SpriteFrameCache::removeSpriteFrameByName(const std::string &name)
{
	auto iter = _spriteFrames.find(name);
	if (iter == _spriteFrames.end())
		return;
	auto spriteFrame = iter->second;
	_spriteFrames.erase(iter);
	spriteFrame->release();
}
void SpriteFrameCache::removeSpriteFramesByFileName(const std::string &filePath)
{
	for (auto it = _spriteFrames.begin(); it != _spriteFrames.end(); ++it)
	{
		auto spriteFrame = it->second;
		if (spriteFrame->getTextureFilename() == filePath)
		{
			it = _spriteFrames.erase(it);
			SDL_SAFE_RELEASE(spriteFrame);
			return;
		}
	}
}

Texture *SpriteFrameCache::createTexture(Texture *texture, const Rect &srcRect, const Point &offset, const Size &originalSize, bool rotated)
{
	Texture *target = texture;

	auto renderer = Director::getInstance()->getRenderer();
	Color4B oldColor = renderer->getDrawColor();

	Texture *realTexture = Texture::create(renderer, SDL_PIXELFORMAT_RGBA4444, SDL_TEXTUREACCESS_TARGET, (int)originalSize.width, (int)originalSize.height);
	realTexture->setBlendMode(SDL_BLENDMODE_BLEND);

	renderer->setTarget(realTexture);
	renderer->setDrawColor(Color4B(0, 0, 0, 0));
	renderer->clear();

	Rect destRect(offset.x, offset.y, srcRect.size.width, srcRect.size.height);
	float rotation = 0.f;
	SDL_Point center = {int(srcRect.size.width / 2), int(srcRect.size.height / 2)};

	if (rotated)
	{
		center.x = center.y = (int)srcRect.size.width / 2;

		rotation = -90.f;

		destRect.origin.x = offset.x;
		destRect.origin.y = offset.y;

		int n = int(srcRect.size.width);

		if (n % 2 == 0)
			destRect.origin.y -= 1;
	}
	SDL_Rect src = srcRect.getSDL_Rect();
	SDL_Rect dest = destRect.getSDL_Rect();

	renderer->copyEx(target, &src, &dest, rotation, &center, SDL_FLIP_NONE);

	renderer->setTarget(nullptr);
	renderer->setDrawColor(oldColor);

	return realTexture;
}
NS_SDL_END
