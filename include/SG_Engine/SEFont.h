#ifndef __SDL_Font_H__
#define __SDL_Font_H__
#include <string>

#include "SDL_ttf.h"

#include "SG_Engine/SEObject.h"
#include "SG_Engine/SESurface.h"
#include "SG_Engine/SEColor.h"

NS_SDL_BEGIN

class Font : public Object
{
private:
	TTF_Font *_font;

	std::string _path;
	int _size;

public:
	Font();
	virtual ~Font();
	static Font *create(const char *path, int fontSize);
	bool init(const char *path, int fontSize);
	void destroyFont();

	Surface *ttfRenderText_Solid(const char *text, const Color4B &color);
	Surface *ttfRenderText_Shaded(const char *text, const Color4B &fg, const Color4B &bg);
	Surface *ttfRenderText_Blended(const char *text, const Color4B &color);

	Surface *ttfRenderText_Blended_Wrapped(const char *text, const Color4B &color, Uint32 wrapLine);

	Surface *ttfRenderUTF8_Solid(const char *text, const Color4B &color);
	Surface *ttfRenderUTF8_Shaded(const char *text, const Color4B &fg, const Color4B &bg);
	Surface *ttfRenderUTF8_Blended(const char *text, const Color4B &color);
	Surface *ttfRenderUTF8_Blended_Wrapped(const char *text, const Color4B &color, Uint32 wrapLine);

	Surface *ttfRenderUNICODE_Solid(const Uint16 *text, const Color4B &color);
	Surface *ttfRenderUNICODE_Shaded(const Uint16 *text, const Color4B &fg, const Color4B &bg);
	Surface *ttfRenderUNICODE_Blended(const Uint16 *text, const Color4B &color);
	Surface *ttfRenderUNICODE_Blended_Wrapped(const Uint16 *text, const Color4B &color, Uint32 wrapLine);

	Surface *ttfRenderGlyph_Solid(Uint16 ch, const Color4B &color);
	Surface *ttfRenderGlyph_Shaded(Uint16 ch, const Color4B &fg, const Color4B &bg);
	Surface *ttfRenderGlyph_Blended(Uint16 ch, const Color4B &color);

	int ttfFontHeight() const;

	int ttfFontLineSkip() const;

	int ttfFontDescent() const;

	int ttfFontAscent() const;

	int ttfGlyphMetrics(Uint16 ch, int *minX, int *maxX, int *minY, int *maxY, int *advance);

	int ttfSizeText(const char *text, int *w, int *h);
	int ttfSizeUTF8(const char *text, int *w, int *h);
	int ttfSizeUNICODE(const Uint16 *text, int *w, int *h);

	bool ttfSetFontStyle(int style);
	int ttfGetFontStyle() const;

	bool ttfSetFontOutline(int outline);
	int ttfGetFontOutline() const;

	bool ttfSetFontHinting(int hinting);
	int ttfGetFontHinting() const;

	bool ttfSetFontKerning(int allowed);
	int ttfGetFontKerning() const;

	const char *ttfFontFaceFamilyName();

	long ttfFontFaces() const;
};
NS_SDL_END
#endif
