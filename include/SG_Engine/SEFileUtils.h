#ifndef __SDL_FileUtils_H__
#define __SDL_FileUtils_H__

#include <string>
#include <map>
#include <vector>
#include <memory>
#include <stack>
#include <algorithm>
#include <stdio.h>

#include "SDL2/SDL.h"
#include "SEObject.h"
#include "SEValue.h"

#include "rapidxml/rapidxml.hpp"
#include "rapidxml/rapidxml_print.hpp"

NS_SDL_BEGIN
class FileUtils : public Object
{
private:
	static FileUtils *_pInstance;

	std::map<std::string, std::string> _fullPathCache;

	std::vector<std::string> _searchPathArray;

	std::string _org;
	std::string _app;

private:
	FileUtils();
	~FileUtils();

public:
	static FileUtils *getInstance()
	{
		if (_pInstance == nullptr)
		{
			_pInstance = new FileUtils();
		}
		return _pInstance;
	}
	static void purge();

	std::string fullPathForFilename(const std::string &filename);

	void addSearchPath(const std::string &path);

	bool isAbsolutePath(const std::string &filename) const;

	bool isFileExist(const std::string &filename) const;

	std::string valueMapToString(ValueMap &dict);
	ValueMap stringToValueMap(char *text);

	std::string getDataFromFile(const std::string &filename);

	ValueMap getValueMapFromFile(const std::string &plist);
	ValueVector getValueVectorFromFile(const std::string &plist);

	bool writeValueMapToFile(ValueMap &dict, const std::string &fullpath);
	bool writeValueVectorToFile(ValueVector &array, const std::string &fullpath);

	std::unique_ptr<char> getUniqueDataFromFile(const std::string &filename, size_t *sz = nullptr);

	std::string getWritablePath() const;

	void setOrgString(const std::string &org) { _org = org; }
	std::string getOrgString() const { return _org; }
	void setAppString(const std::string &app) { _app = app; }
	std::string getAppString() const { return _app; }

private:
	std::string getFullPath(const std::string &filename, const std::string &path) const;
	rapidxml::xml_node<> *generateElementForArray(ValueVector &array, rapidxml::xml_document<> &doc);
	rapidxml::xml_node<> *generateElementForDict(ValueMap &dict, rapidxml::xml_document<> &doc);
	rapidxml::xml_node<> *generateElementForObject(const Value &value, rapidxml::xml_document<> &doc);
};
NS_SDL_END
#endif
