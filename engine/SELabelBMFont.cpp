#include "SG_Engine/SELabelBMFont.h"
#include "SG_Engine/SESprite.h"
#include "SG_Engine/SEBMFontConfiguration.h"
#include "SG_Engine/SEDirector.h"
#include "SG_Engine/SETextureCache.h"
#include "SG_Engine/SEStringUtils.h"

NS_SDL_BEGIN

LabelBMFont::LabelBMFont()
	: _pConfiguration(nullptr), _width(0), _color(255, 255, 255)
{
	_cascadeOpacity = true;
}

LabelBMFont::~LabelBMFont()
{
	SDL_SAFE_RELEASE_NULL(_pConfiguration);
}

LabelBMFont *LabelBMFont::create(const std::string &text, const std::string &fontFile)
{
	auto label = new LabelBMFont();

	if (label && label->init(text, fontFile))
		label->autorelease();
	else
		SDL_SAFE_DELETE(label);

	return label;
}

LabelBMFont *LabelBMFont::create(const std::string &text, const std::string &fontFile, int width, const Color3B &color)
{
	auto label = new LabelBMFont();

	if (label && label->init(text, fontFile, width, color))
		label->autorelease();
	else
		SDL_SAFE_DELETE(label);

	return label;
}

bool LabelBMFont::init(const std::string &text, const std::string &fontFile)
{

	_pConfiguration = BMFontConfiguration::createWithFile(fontFile);
	SDL_SAFE_RETAIN(_pConfiguration);

	this->setString(text);

	return true;
}

bool LabelBMFont::init(const std::string &text, const std::string &fontFile, int width, const Color3B &color)
{

	_pConfiguration = BMFontConfiguration::createWithFile(fontFile);
	SDL_SAFE_RETAIN(_pConfiguration);

	_width = width;
	_color = color;

	this->setString(text);

	return true;
}

Color3B LabelBMFont::getColor() const
{
	return _color;
}

void LabelBMFont::setColor(const Color3B &color)
{
	for (auto it = _children.begin(); it != _children.end(); it++)
	{
		auto node = *it;

		node->setColorMod(color);
	}
	_color = color;
}

void LabelBMFont::setString(const std::string &text)
{
	if (_text == text)
		return;
	_text = text;

	for (auto child : _children)
	{
		child->setVisible(false);
	}

	this->createFontChars();
}

void LabelBMFont::createFontChars()
{
	if (_text.empty())
		return;

	float nextFontPositionX = 0.f;
	float nextFontPositionY = 0.f;

	unsigned int totalHeight = 0;
	float longestLine = 0.f;
	Size tmpSize;

	int utf8len = StringUtils::cc_utf8_strlen(_text.c_str(), -1);
	if (utf8len == 0)
		return;

	unsigned short *pUniStr = new unsigned short[utf8len + 1];
	pUniStr[utf8len] = 0;

	const char *p = _text.c_str();
	for (int i = 0; i < utf8len; i++)
	{
		pUniStr[i] = StringUtils::cc_utf8_get_char(p);
		p = cc_utf8_next_char(p);
	}

	unsigned int stringLen = StringUtils::cc_wcslen(pUniStr);

	totalHeight = _pConfiguration->_commonHeight;

	for (unsigned int i = 0; i < stringLen; i++)
	{
		unsigned short c = pUniStr[i];

		if (c == '\n')
		{
			nextFontPositionX = 0;
			nextFontPositionY += _pConfiguration->_commonHeight;
			totalHeight += _pConfiguration->_commonHeight;
			continue;
		}

		auto it = _pConfiguration->_pBitmapFontMap->find(c);

		auto &fontDef = it->second;
		Rect rect = Rect(fontDef.x, fontDef.y, fontDef.width, fontDef.height);

		Sprite *fontChar = this->getChildByTag<Sprite *>(i);
		auto texture = Director::getInstance()->getTextureCache()->addImage(_pConfiguration->_atlasNames[fontDef.page]);

		if (fontChar == nullptr)
		{
			fontChar = Sprite::createWithTexture(texture, rect);

			this->addChild(fontChar, 0, i);
		}
		else
		{
			fontChar->initWithTexture(texture, rect, false, rect.origin, rect.size);
			fontChar->setVisible(true);
			fontChar->setOpacity(255);
		}

		if (_width != 0 && nextFontPositionX + fontDef.xadvance > _width)
		{
			nextFontPositionX = 0;
			nextFontPositionY += _pConfiguration->_commonHeight;
			totalHeight += _pConfiguration->_commonHeight;
		}

		fontChar->setPosition(nextFontPositionX + fontDef.xoffset + fontDef.width / 2.0f,
							  (float)nextFontPositionY + fontDef.yoffset + fontDef.height / 2.0f);

		nextFontPositionX += fontDef.xadvance;

		fontChar->setColorMod(_color);

		if (longestLine < nextFontPositionX)
		{
			longestLine = nextFontPositionX;
		}
	}

	tmpSize.width = (float)longestLine;
	tmpSize.height = (float)totalHeight;

	this->setContentSize(tmpSize);

	delete[] pUniStr;
}

NS_SDL_END
