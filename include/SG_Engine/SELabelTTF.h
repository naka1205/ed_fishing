#ifndef __LabelTTF_H__
#define __LabelTTF_H__
#include <string>

#include "SG_Engine/SENode.h"
#include "SG_Engine/SEColor.h"
#include "SG_Engine/SELabel.h"

NS_SDL_BEGIN
class Sprite;

class LabelTTF : public Label
{
private:
	std::string _ttfPath;
	int _size;
	Color4B _color;
	Uint32 _wrapLine;
	Sprite *_sprite;

public:
	LabelTTF();
	~LabelTTF();

	static LabelTTF *create(const std::string &text, int size = 20, const Color4B &color = Color4B(0, 0, 0), const std::string &ttfPath = "fonts/Marker Felt.ttf");
	static LabelTTF *createWithWrapLine(const std::string &text, Uint32 wrapLine, int size = 20, const Color4B &color = Color4B(0, 0, 0), const std::string &ttfPath = "fonts/Marker Felt.ttf");

	bool init(const std::string &text, int size, const Color4B &color, const std::string &ttfPath);
	bool initWithWrapLine(const std::string &text, Uint32 wrapLine, int size, const Color4B &color, const std::string &ttfPath);

	void setWrapLine(Uint32 wrapLine);

	void setColor(const Color4B &color);
	Color4B getColor() const;

	virtual void setString(const std::string &label);
	virtual std::string getString() const;

private:
	void updateInnerSprite();
};
NS_SDL_END
#endif
