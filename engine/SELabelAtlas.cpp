#include "SG_Engine/SELabelAtlas.h"
#include "SG_Engine/SEDirector.h"
#include "SG_Engine/SESurface.h"
#include "SG_Engine/SETextureCache.h"
#include "SG_Engine/SEFileUtils.h"
#include "SG_Engine/SESprite.h"
NS_SDL_BEGIN
LabelAtlas::LabelAtlas()
	: _widthPerGlyph(0), _heightPerGlyph(0), _startChar(0), _fontKerning(1.f), _sprite(nullptr)
{
}

LabelAtlas::~LabelAtlas()
{
}

LabelAtlas *LabelAtlas::create(const std::string &text, const std::string &charMapFile, unsigned int itemWidth, unsigned int itemHeight, unsigned int startChar)
{
	auto label = new LabelAtlas();

	if (label && label->init(text, charMapFile, itemWidth, itemHeight, startChar))
		label->autorelease();
	else
		SDL_SAFE_DELETE(label);

	return label;
}

LabelAtlas *LabelAtlas::createWithColorKey(const std::string &text, const std::string &charMapFile, unsigned int itemWidth, unsigned int itemHeight, const Color4B &color, unsigned int startChar)
{
	auto label = new LabelAtlas();
	if (label && label->initWithColorKey(text, charMapFile, itemWidth, itemHeight, color, startChar))
		label->autorelease();
	else
		SDL_SAFE_DELETE(label);
	return label;
}

bool LabelAtlas::init(const std::string &text, const std::string &charMapFile, unsigned int itemWidth, unsigned int itemHeight, unsigned int startChar)
{
	_filePath = charMapFile;
	_widthPerGlyph = itemWidth;
	_heightPerGlyph = itemHeight;
	_startChar = startChar;

	auto texture = Director::getInstance()->getTextureCache()->addImage(charMapFile, charMapFile);

	if (texture == nullptr)
		return false;

	this->setString(text);
	return true;
}

bool LabelAtlas::initWithColorKey(const std::string &text, const std::string &charMapFile, unsigned int itemWidth, unsigned int itemHeight, const Color4B &color, unsigned int startChar)
{
	_filePath = charMapFile;
	_widthPerGlyph = itemWidth;
	_heightPerGlyph = itemHeight;
	_startChar = startChar;

	if (Director::getInstance()->getTextureCache()->getTextureForKey(charMapFile) == nullptr)
	{
		auto fullpath = FileUtils::getInstance()->fullPathForFilename(charMapFile);
		if (fullpath.empty())
			return false;

		Surface *surface = Surface::create(fullpath);
		Uint32 colorKey = surface->mapRGBA(color.r, color.g, color.b, color.a);
		surface->setColorKey(colorKey);

		Renderer *renderer = Director::getInstance()->getRenderer();
		Texture *texture = Texture::createWithSurface(renderer, surface);

		Director::getInstance()->getTextureCache()->addTexture(texture, charMapFile);
	}
	this->setString(text);
	return true;
}

void LabelAtlas::draw()
{

	if (_parent == nullptr)
		_sprite->draw();
}

void LabelAtlas::setString(const std::string &text)
{
	if (_text == text)
		return;

	_text = text;

	if (strlen(text.c_str()) == 0)
	{
		if (_sprite != nullptr)
			_sprite->setVisible(false);
		return;
	}
	else if (_sprite != nullptr)
	{
		_sprite->setVisible(true);
	}
	Renderer *renderer = Director::getInstance()->getRenderer();
	Texture *texture = Director::getInstance()->getTextureCache()->getTextureForKey(_filePath);

	int textureWidth = 0, textureHeight = 0;
	texture->query(NULL, NULL, &textureWidth, &textureHeight);

	int rowItemsNum = textureWidth / _widthPerGlyph;

	int width = int((text.size() - 1) * _widthPerGlyph * _fontKerning + _widthPerGlyph);
	int height = _heightPerGlyph;

	Texture *bg = nullptr;

	if (_sprite != nullptr)
		_sprite->removeFromParent();

	_sprite = Sprite::createWithTexture(SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, width, height);
	_sprite->setPosition(0.5f * width, 0.5f * height);

	this->setContentSize(Size(width, height));
	this->addChild(_sprite);

	bg = _sprite->getTexture();
	bg->setBlendMode(SDL_BLENDMODE_BLEND);

	SDL_BlendMode oldBlendMode = SDL_BLENDMODE_NONE;
	renderer->getDrawBlendMode(&oldBlendMode);

	renderer->setDrawBlendMode(SDL_BLENDMODE_NONE);
	renderer->setTarget(bg);
	auto oldColor = renderer->getDrawColor();

	renderer->setDrawColor(0, 0, 0, 0);
	renderer->clear();

	SDL_Rect destRect = {0, 0, _widthPerGlyph, _heightPerGlyph};
	for (unsigned int i = 0; i < text.size(); i++)
	{
		unsigned int ch = text.at(i);
		if (ch < _startChar)
			continue;
		SDL_Rect srcRect = {0, 0, _widthPerGlyph, _heightPerGlyph};
		srcRect.x = (ch - _startChar) % rowItemsNum * _widthPerGlyph;
		srcRect.y = (ch - _startChar) / rowItemsNum * _heightPerGlyph;

		renderer->copy(texture, &srcRect, &destRect);

		destRect.x += int(_fontKerning * _widthPerGlyph);
	}

	renderer->setTarget(nullptr);

	renderer->setDrawColor(oldColor.r, oldColor.g, oldColor.b, oldColor.a);
	renderer->setDrawBlendMode(oldBlendMode);
}

float LabelAtlas::getFontKerning() const
{
	return _fontKerning;
}

void LabelAtlas::setFontKerning(float kerning)
{
	if (_fontKerning != kerning)
	{
		_fontKerning = kerning;
		auto temp = _text;
		_text.clear();
		this->setString(temp);
	}
}

Sprite *LabelAtlas::getInnerSprite() const
{
	return _sprite;
}
NS_SDL_END
