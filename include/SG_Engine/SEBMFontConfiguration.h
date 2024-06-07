#ifndef __SDL_BMFontConfiguration_H__
#define __SDL_BMFontConfiguration_H__

#include <map>
#include <string>
#include <cstring>
#include <sstream>

#include "SEObject.h"
#include "SEPlatformMarcos.h"

NS_SDL_BEGIN

struct BMFontDef
{
public:
	int x;
	int y;
	int width;
	int height;
	int xoffset;
	int yoffset;
	int xadvance;
	int page;

public:
	BMFontDef();
	BMFontDef(int x, int y, int width, int height, int xoffset, int yoffset, int xadvance, int page);
};

class BMFontConfiguration : public Object
{
public:
	std::map<unsigned int, BMFontDef> *_pBitmapFontMap;

	std::map<unsigned int, std::string> _atlasNames;

	unsigned int _commonHeight;

private:
	static std::map<std::string, BMFontConfiguration *> *g_pConfigurations;

public:
	BMFontConfiguration();
	~BMFontConfiguration();

	static BMFontConfiguration *createWithFile(const std::string &fontFile);
	static void removeFNTCache();

	static BMFontConfiguration *configurationWithFNTFile(const std::string &fontFile);
	bool initWithFNTFile(const std::string &fontFile);

private:
	void parseConfigFile(const std::string &fontFile);

	void parseInfoArguments(const std::string &text);

	void parseCommonArguments(const std::string &text);

	void parseImageFileName(const std::string &text, const std::string &fontFile);

	void parseChar(const std::string &text);
};
NS_SDL_END
#endif
