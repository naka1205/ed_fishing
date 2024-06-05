#include "SG_Engine/SEBMFontConfiguration.h"
#include "SG_Engine/SEFileUtils.h"
#include "SG_Engine/SEDirector.h"
#include "SG_Engine/SETextureCache.h"

NS_SDL_BEGIN
std::map<std::string, BMFontConfiguration *> *BMFontConfiguration::g_pConfigurations = nullptr;

BMFontDef::BMFontDef()
	: x(0), y(0), width(0), height(0), xoffset(0), yoffset(0), xadvance(0), page(0)
{
}

BMFontDef::BMFontDef(int x, int y, int width, int height, int xoffset, int yoffset, int xadvance, int page)
	: x(x), y(y), width(width), height(height), xoffset(xoffset), yoffset(yoffset), xadvance(xadvance), page(page)
{
}

BMFontConfiguration::BMFontConfiguration()
	: _pBitmapFontMap(nullptr), _commonHeight(0)
{
}

BMFontConfiguration::~BMFontConfiguration()
{
	SDL_SAFE_DELETE(_pBitmapFontMap);
}

BMFontConfiguration *BMFontConfiguration::createWithFile(const std::string &fontFile)
{
	BMFontConfiguration *pRet = nullptr;

	if (g_pConfigurations == nullptr)
	{
		g_pConfigurations = new std::map<std::string, BMFontConfiguration *>();
	}

	auto it = g_pConfigurations->find(fontFile);

	if (it != g_pConfigurations->end())
	{
		pRet = it->second;
	}
	else
	{
		pRet = BMFontConfiguration::configurationWithFNTFile(fontFile);

		g_pConfigurations->insert(std::make_pair(fontFile, pRet));
		SDL_SAFE_RETAIN(pRet);
	}
	return pRet;
}

void BMFontConfiguration::removeFNTCache()
{
	if (g_pConfigurations != nullptr)
	{
		for (auto it = g_pConfigurations->begin(); it != g_pConfigurations->end();)
		{
			auto config = it->second;
			SDL_SAFE_RELEASE(config);

			it = g_pConfigurations->erase(it);
		}
		SDL_SAFE_DELETE(g_pConfigurations);
	}
}

BMFontConfiguration *BMFontConfiguration::configurationWithFNTFile(const std::string &fontFile)
{
	auto pRet = new BMFontConfiguration();

	if (pRet && pRet->initWithFNTFile(fontFile))
		pRet->autorelease();
	else
		SDL_SAFE_DELETE(pRet);

	return pRet;
}

bool BMFontConfiguration::initWithFNTFile(const std::string &fontFile)
{

	if (fontFile.empty())
		return false;

	if (_pBitmapFontMap == nullptr)
	{
		_pBitmapFontMap = new std::map<unsigned int, BMFontDef>();
	}
	this->parseConfigFile(fontFile);

	return true;
}

void BMFontConfiguration::parseConfigFile(const std::string &fontFile)
{

	std::unique_ptr<char> uniqueData = std::move(FileUtils::getInstance()->getUniqueDataFromFile(fontFile));

	std::istringstream in = std::istringstream(uniqueData.get());

	std::string line;

	while (getline(in, line))
	{
		if (line.substr(0, strlen("info face")) == "info face")
			this->parseInfoArguments(line);
		else if (line.substr(0, strlen("common lineHeight")) == "common lineHeight")
			this->parseCommonArguments(line);
		else if (line.substr(0, strlen("page id")) == "page id")
			this->parseImageFileName(line, fontFile);
		else if (line.substr(0, strlen("char id")) == "char id")
			this->parseChar(line);
	}
	uniqueData.reset();
}

void BMFontConfiguration::parseInfoArguments(const std::string &text)
{
}

void BMFontConfiguration::parseCommonArguments(const std::string &text)
{

	auto index1 = text.find("lineHeight=");
	auto index2 = text.find(' ', index1);
	auto len = strlen("lineHeight=");
	auto str = text.substr(index1 + len, index2 - index1 - len);

	_commonHeight = SDL_atoi(str.c_str());
}

void BMFontConfiguration::parseImageFileName(const std::string &text, const std::string &fontFile)
{

	auto index1 = text.find("id=");
	auto index2 = text.find(' ', index1);
	auto len = strlen("id=");
	auto str = text.substr(index1 + len, index2 - index1 - len);
	int id = SDL_atoi(str.c_str());

	index1 = text.find("file=\"", index2);
	len = strlen("file=\"");
	index2 = text.find('\"', index1 + len);
	str = text.substr(index1 + len, index2 - index1 - len);

	auto pos = fontFile.find_last_of("/");
	std::string relativePath;

	if (pos != std::string::npos)
	{
		relativePath = fontFile.substr(0, pos + 1);
	}

	auto fullpath = FileUtils::getInstance()->fullPathForFilename(str);
	if (fullpath.empty() && !relativePath.empty())
	{
		fullpath = FileUtils::getInstance()->fullPathForFilename(relativePath + str);
	}

	_atlasNames.insert(std::make_pair(id, relativePath + str));

	Director::getInstance()->getTextureCache()->addImage(fullpath, relativePath + str);
}

void BMFontConfiguration::parseChar(const std::string &text)
{
	BMFontDef def;

	auto index1 = text.find("id=");
	auto index2 = text.find(' ', index1);
	auto len = strlen("id=");
	auto str = text.substr(index1 + len, index2 - index1 - len);
	int id = SDL_atoi(str.c_str());

	index1 = text.find("x=", index2);
	index2 = text.find(' ', index1);
	len = strlen("x=");
	str = text.substr(index1 + len, index2 - index1 - len);
	def.x = SDL_atoi(str.c_str());

	index1 = text.find("y=", index2);
	index2 = text.find(' ', index1);
	len = strlen("y=");
	str = text.substr(index1 + len, index2 - index1 - len);
	def.y = SDL_atoi(str.c_str());

	index1 = text.find("width=", index2);
	index2 = text.find(' ', index1);
	len = strlen("width=");
	str = text.substr(index1 + len, index2 - index1 - len);
	def.width = SDL_atoi(str.c_str());

	index1 = text.find("height=", index2);
	index2 = text.find(' ', index1);
	len = strlen("height=");
	str = text.substr(index1 + len, index2 - index1 - len);
	def.height = SDL_atoi(str.c_str());

	index1 = text.find("xoffset=", index2);
	index2 = text.find(' ', index1);
	len = strlen("xoffset=");
	str = text.substr(index1 + len, index2 - index1 - len);
	def.xoffset = SDL_atoi(str.c_str());

	index1 = text.find("yoffset=", index2);
	index2 = text.find(' ', index1);
	len = strlen("yoffset=");
	str = text.substr(index1 + len, index2 - index1 - len);
	def.yoffset = SDL_atoi(str.c_str());

	index1 = text.find("xadvance=", index2);
	index2 = text.find(' ', index1);
	len = strlen("xadvance=");
	str = text.substr(index1 + len, index2 - index1 - len);
	def.xadvance = SDL_atoi(str.c_str());

	index1 = text.find("page=", index2);
	index2 = text.find(' ', index1);
	len = strlen("page=");
	str = text.substr(index1 + len, index2 - index1 - len);
	def.page = SDL_atoi(str.c_str());

	_pBitmapFontMap->insert(std::make_pair(id, def));
}

NS_SDL_END
