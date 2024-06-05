#include "SG_Engine/SEUserDefault.h"
#include "rapidxml/rapidxml.hpp"
#include "rapidxml/rapidxml_utils.hpp"
#include "rapidxml/rapidxml_print.hpp"

#include "SG_Engine/SEValue.h"
#include "SG_Engine/SEFileUtils.h"
NS_SDL_BEGIN

UserDefault *UserDefault::_pInstance = nullptr;
UserDefault::UserDefault()
{
}
UserDefault::~UserDefault()
{
}
UserDefault *UserDefault::getInstance()
{
	if (_pInstance == nullptr)
	{
		_pInstance = new UserDefault();
		_pInstance->init();
	}
	return _pInstance;
}
void UserDefault::purge()
{
	SDL_SAFE_DELETE(_pInstance);
}
bool UserDefault::init()
{
	_filePath = "save.xml";

	return true;
}
void UserDefault::setValueForKey(const std::string &key, const std::string &value)
{
	rapidxml::xml_document<> doc;

	std::string filePath = FileUtils::getInstance()->getWritablePath() + _filePath;
	std::unique_ptr<char> text_ptr = std::move(FileUtils::getInstance()->getUniqueDataFromFile(filePath));

	rapidxml::xml_node<> *root = nullptr;
	rapidxml::xml_node<> *node = nullptr;

	rapidxml::xml_node<> *head = doc.allocate_node(rapidxml::node_pi,
												   doc.allocate_string("xml version='1.0' encoding='UTF-8'"));
	doc.append_node(head);

	if (text_ptr != nullptr)
	{
		doc.parse<0>(text_ptr.get());

		root = doc.first_node();

		node = root->first_node(key.c_str());

		if (node)
		{
			root->remove_node(node);
			node = nullptr;
		}
		node = doc.allocate_node(rapidxml::node_element, key.c_str(), value.c_str());

		root->append_node(node);
	}
	else
	{

		root = doc.allocate_node(rapidxml::node_element, "root");
		doc.append_node(root);

		node = doc.allocate_node(rapidxml::node_element, key.c_str(), value.c_str());

		root->append_node(node);
	}
	std::string text;
	if (text_ptr)
		text += "<?xml version='1.0' encoding='UTF-8'?>\n";

	rapidxml::print(std::back_inserter(text), doc, 0);

	SDL_RWops *fout = SDL_RWFromFile(filePath.c_str(), "w");
	if (fout == nullptr)
		LOG("UserDefault::setValueForKey:%s\n", SDL_GetError());
	else
	{
		SDL_RWwrite(fout, text.c_str(), text.size(), 1);
		SDL_RWclose(fout);
	}
	text_ptr.reset();
}
void UserDefault::setBoolForKey(const std::string &key, bool value)
{
	if (key.empty())
		return;
	std::string text = value == true ? "true" : "false";
	setValueForKey(key, text);
}
void UserDefault::setIntegerForKey(const std::string &key, int value)
{
	if (key.empty())
		return;
	setValueForKey(key, Value(value).asString());
}
void UserDefault::setFloatForKey(const std::string &key, float value)
{
	if (key.empty())
		return;
	setValueForKey(key, Value(value).asString());
}
void UserDefault::setDoubleForKey(const std::string &key, double value)
{
	if (key.empty())
		return;
	setValueForKey(key, Value(value).asString());
}
void UserDefault::setStringForKey(const std::string &key, const std::string &value)
{
	if (key.empty())
		return;
	setValueForKey(key, value);
}
std::string UserDefault::getValueForKey(const std::string &key)
{
	rapidxml::xml_document<> doc;

	auto filePath = FileUtils::getInstance()->getWritablePath() + _filePath;
	std::unique_ptr<char> text_ptr = std::move(FileUtils::getInstance()->getUniqueDataFromFile(filePath));
	std::string text;

	try
	{
		if (text_ptr != nullptr)
		{
			doc.parse<0>(text_ptr.get());

			auto root = doc.first_node();

			auto node = root->first_node(key.c_str());

			if (node)
				text = node->value();
		}
	}
	catch (std::runtime_error e)
	{
	}

	text_ptr.reset();

	return text;
}
bool UserDefault::getBoolForKey(const std::string &key, bool defaultValue)
{
	auto text = getValueForKey(key);
	if (text.empty())
		return defaultValue;
	if (text == "true")
		return true;
	else if (text == "false")
		return false;
	return false;
}
int UserDefault::getIntegerForKey(const std::string &key, int defaultValue)
{
	auto text = getValueForKey(key);
	if (text.empty())
		return defaultValue;
	Value value = Value(text);
	return value.asInt();
}
float UserDefault::getFloatForKey(const std::string &key, float defaultValue)
{
	auto text = getValueForKey(key);
	if (text.empty())
		return defaultValue;
	Value value = Value(text);
	return value.asFloat();
}
double UserDefault::getDoubleForKey(const std::string &key, double defaultValue)
{
	auto text = getValueForKey(key);
	if (text.empty())
		return defaultValue;
	Value value = Value(text);
	return value.asDouble();
}
std::string UserDefault::getStringForKey(const std::string &key, const std::string &defaultValue)
{
	auto text = getValueForKey(key);
	if (text.empty())
		return defaultValue;
	return text;
}
bool UserDefault::isXMLExist()
{

	std::string filePath = FileUtils::getInstance()->getWritablePath() + _filePath;
	if (FileUtils::getInstance()->isFileExist(filePath))
		return true;
	return false;
}
NS_SDL_END
