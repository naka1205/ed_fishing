#ifndef __SELabel_H__
#define __SELabel_H__

#include <string>
#include <cstring>

#include "SG_Engine/SENode.h"
#include "SG_Engine/SETexture.h"
#include "SG_Engine/SELabel.h"
#include "SG_Engine/SEColor.h"

NS_SDL_BEGIN
class Sprite;
class LabelAtlas : public Label
{
private:
	std::string _filePath;

	int _widthPerGlyph;
	int _heightPerGlyph;

	unsigned int _startChar;

	float _fontKerning;

	Sprite *_sprite;

public:
	LabelAtlas();
	virtual ~LabelAtlas();

	static LabelAtlas *create(const std::string &text, const std::string &charMapFile, unsigned int itemWidth, unsigned int itemHeight, unsigned int startChar = 0);
	static LabelAtlas *createWithColorKey(const std::string &text, const std::string &charMapFile, unsigned int itemWidth, unsigned int itemHeight, const Color4B &color, unsigned int startChar = 0);
	bool init(const std::string &text, const std::string &charMapFile, unsigned int itemWidth, unsigned int itemHeight, unsigned int startChar);
	bool initWithColorKey(const std::string &text, const std::string &charMapFile, unsigned int itemWidth, unsigned int itemHeight, const Color4B &color, unsigned int startChar = 0);

	virtual void draw();
	virtual void setString(const std::string &label);

	float getFontKerning() const;

	void setFontKerning(float kerning);

	Sprite *getInnerSprite() const;
};
NS_SDL_END
#endif
