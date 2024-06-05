#include "SG_Engine/SEFastTiledMap.h"
#include "SG_Engine/SETileset.h"
#include "SG_Engine/SETMXObjectGroup.h"
#include "SG_Engine/SEFastLayer.h"
#include "SG_Engine/SETexture.h"
#include "SG_Engine/SESprite.h"
#include "SG_Engine/SEDirector.h"
#include "SG_Engine/SETextureCache.h"
#include "SG_Engine/SEFileUtils.h"
#include "SG_Engine/SEActionInterval.h"
#include "SG_Engine/SESpriteFrame.h"
#include "SG_Engine/SEAnimationFrame.h"
#include "SG_Engine/SEAnimation.h"
#include "SG_Engine/SETMXImageLayer.h"
#include "SG_Engine/SEStringUtils.h"

NS_SDL_BEGIN

FastTiledMap::FastTiledMap()
	: _mapSize(Size::ZERO), _tileSize(Size::ZERO), _mapOrientation(90)
{
}

FastTiledMap::~FastTiledMap()
{
	_properties.clear();

	// release tileset
	for (auto iter = _tilesets.begin(); iter != _tilesets.end();)
	{
		auto tileset = *iter;
		iter = _tilesets.erase(iter);
		tileset->release();
	}

	// release object group
	for (auto iter = _objectGroups.begin(); iter != _objectGroups.end();)
	{
		auto objectGroup = *iter;
		iter = _objectGroups.erase(iter);
		objectGroup->release();
	}
}

FastTiledMap *FastTiledMap::create(const std::string &filename)
{
	auto tiledMap = new FastTiledMap();

	if (tiledMap && tiledMap->init(filename))
		tiledMap->autorelease();
	else
		SDL_SAFE_DELETE(tiledMap);

	return tiledMap;
}

bool FastTiledMap::init(const std::string &filename)
{

	this->setAnchorPoint(Point(0.f, 0.f));

	int startPos = filename.find_last_of('/');
	if (startPos != -1)
		_resourcePath = filename.substr(0, startPos);

	auto file = FileUtils::getInstance()->fullPathForFilename(filename);
	if (file.empty())
		return false;

	return this->parse(file);
}

Tileset *FastTiledMap::getTilesetForGID(int gid) const
{
	if (gid == 0)
		return nullptr;

	// get the tileset by gid
	for (unsigned int i = 0; i < _tilesets.size(); i++)
	{
		auto firstGrid = _tilesets.at(i)->firstGrid;
		auto tileCount = _tilesets.at(i)->tileCount;

		if (gid >= firstGrid && gid < firstGrid + tileCount)
			return _tilesets.at(i);
	}
	return nullptr;
}

Value FastTiledMap::getPropertyForName(const std::string &key) const
{
	if (_properties.find(key) != _properties.end())
		return _properties.at(key);

	return Value();
}

const ValueMap &FastTiledMap::getProperties() const
{
	return _properties;
}

ValueMap FastTiledMap::getTilePropertiesForGID(int gid) const
{
	Tileset *tileset = this->getTilesetForGID(gid);

	if (tileset != nullptr)
	{
		return tileset->getPropertiesForGID(gid - tileset->firstGrid);
	}
	return ValueMap();
}

bool FastTiledMap::getTilePropertiesForGID(int gid, ValueMap **value)
{
	// get tileset
	auto tileset = this->getTilesetForGID(gid);

	if (tileset == nullptr)
		return false;

	// get properties
	auto &properties = tileset->_properties;

	auto iter = properties.find(gid - tileset->firstGrid);
	if (iter != properties.end())
	{
		*value = &iter->second;
		return true;
	}
	return false;
}

Size FastTiledMap::getTileSize() const
{
	return _tileSize;
}

Size FastTiledMap::getMapSize() const
{
	return _mapSize;
}

const std::vector<TMXObjectGroup *> &FastTiledMap::getObjectGroups() const
{
	return _objectGroups;
}

const TMXObjectGroup *FastTiledMap::getObjectGroup(const std::string &name) const
{
	for (auto it = _objectGroups.begin(); it != _objectGroups.end(); it++)
	{
		auto objectGroup = *it;

		if (objectGroup->getGroupName() == name)
			return objectGroup;
	}
	return nullptr;
}

const std::vector<Tileset *> &FastTiledMap::getTilesets() const
{
	return _tilesets;
}

Node *FastTiledMap::getLayer(const std::string &name)
{
	if (name.empty())
		return nullptr;

	auto iter = std::find_if(_children.begin(), _children.end(), [name](Node *node)
							 { return node->getName() == name; });

	if (iter != _children.end())
	{
		return *iter;
	}

	return nullptr;
}

const Node *FastTiledMap::getLayer(const std::string &name) const
{
	if (name.empty())
		return nullptr;

	auto iter = std::find_if(_children.begin(), _children.end(), [name](Node *node)
							 { return node->getName() == name; });

	if (iter != _children.end())
	{
		return *iter;
	}

	return nullptr;
}

bool FastTiledMap::parse(const std::string &filename)
{
	std::unique_ptr<char> text_ptr = std::move(FileUtils::getInstance()->getUniqueDataFromFile(filename));

	rapidxml::xml_document<> doc;
	doc.parse<0>(text_ptr.get());

	rapidxml::xml_node<> *root = doc.first_node();
	std::string orientation = root->first_attribute("orientation")->value();

	std::string renderorder = root->first_attribute("renderorder")->value();

	auto width = atoi(root->first_attribute("width")->value());
	auto height = atoi(root->first_attribute("height")->value());
	auto tileWidth = atoi(root->first_attribute("tilewidth")->value());
	auto tileHeight = atoi(root->first_attribute("tileheight")->value());

	if (renderorder != "right-down")
		return false;

	_mapSize = Size((float)width, (float)height);
	_tileSize = Size((float)tileWidth, (float)tileHeight);

	if (orientation == "orthogonal")
		_mapOrientation = 90;

	for (auto node = root->first_node(); node; node = node->next_sibling())
	{
		std::string name = node->name();

		if (name == "tileset")
			this->parseTileset(node);
		else if (name == "layer")
			this->parseLayer(node);
		else if (name == "properties")
			this->parseProperties(node);
		else if (name == "objectgroup")
			this->parseObjectGroup(node);
		else if (name == "imagelayer")
			this->parseImageLayer(node);
	}
	text_ptr.reset();

	return true;
}

void FastTiledMap::parseTileset(rapidxml::xml_node<> *tileRoot)
{

	auto attribute = tileRoot->first_attribute("source");
	Tileset *tileset = nullptr;

	if (attribute != nullptr)
	{
		auto source = attribute->value();

		std::string filename = FileUtils::getInstance()->fullPathForFilename(source);

		if (filename.empty())
		{
			std::string temp = _resourcePath + "/" + source;
			filename = FileUtils::getInstance()->fullPathForFilename(temp);
		}
		if (filename.empty())
			return;

		std::unique_ptr<char> text_ptr = std::move(FileUtils::getInstance()->getUniqueDataFromFile(filename));
		rapidxml::xml_document<> doc;

		doc.parse<0>(text_ptr.get());

		rapidxml::xml_node<> *tsxRoot = doc.first_node();

		tileset = Tileset::create(tileRoot, tsxRoot);
	}
	else
	{
		tileset = Tileset::create(tileRoot);
	}

	if (tileset != nullptr)
	{
		tileset->retain();
		_tilesets.push_back(tileset);

		this->loadImage(tileset->source);
	}
}

void FastTiledMap::parseLayer(rapidxml::xml_node<> *layerRoot)
{
	FastLayer *layer = FastLayer::create(layerRoot);

	layer->setTiledMap(this);

	layer->initCarmark();

	this->addChild(layer);
}

void FastTiledMap::parseImageLayer(rapidxml::xml_node<> *layerRoot)
{
	TMXImageLayer *layer = TMXImageLayer::create();

	this->addChild(layer);

	for (auto node = layerRoot->first_node(); node; node = node->next_sibling())
	{
		if (SDL_strcmp(node->name(), "image") == 0)
		{
			auto image = node->first_attribute("source")->value();
			this->loadImage(image);

			layer->setSprite(image);
		}
		else if (SDL_strcmp(node->name(), "properties") == 0)
		{
			for (auto child = node->first_node(); child; child = child->next_sibling())
			{
				std::string key = child->first_attribute("name")->value();
				std::string value = child->first_attribute("value")->value();

				layer->_properties.insert(std::make_pair(key, Value(value)));
			}
		}
	}
	int offsetX = 0, offsetY = 0;

	for (auto attribute = layerRoot->first_attribute(); attribute != nullptr; attribute = attribute->next_attribute())
	{
		auto name = attribute->name();
		if (SDL_strcmp("name", name) == 0)
		{
			layer->setName(attribute->value());
		}
		else if (SDL_strcmp("offsetx", name) == 0)
			offsetX = atoi(attribute->value());
		else if (SDL_strcmp("offsety", name) == 0)
			offsetY = atoi(attribute->value());
	}

	layer->setPosition((float)offsetX, (float)offsetY);
}

void FastTiledMap::parseProperties(rapidxml::xml_node<> *propRoot)
{
	this->parseProperties(propRoot, _properties);
}

void FastTiledMap::parseObjectGroup(rapidxml::xml_node<> *objectRoot)
{
	auto objectGroup = TMXObjectGroup::create();

	for (auto attri = objectRoot->first_attribute(); attri != nullptr;
		 attri = attri->next_attribute())
	{
		std::string name = std::string(attri->name());

		if (name == "name")
		{
			objectGroup->setGroupName(attri->value());
		}
	}

	for (auto node = objectRoot->first_node(); node != nullptr;
		 node = node->next_sibling())
	{
		ValueMap dict;

		for (auto attri = node->first_attribute(); attri != nullptr;
			 attri = attri->next_attribute())
		{
			auto key = attri->name();
			auto value = attri->value();

			dict.insert(std::make_pair(key, Value(value)));
		}
		for (auto child = node->first_node(); child != nullptr; child = child->next_sibling())
		{
			if (SDL_strcmp(child->name(), "properties") == 0)
			{
				ValueMap properties;

				this->parseProperties(child, properties);
				dict.insert(std::make_pair("properties", Value(properties)));
			}
			else
			{
				std::string type = child->name();
				ValueVector pointArray;
				std::stringstream pointStream;

				for (auto attri = child->first_attribute(); attri != nullptr;
					 attri = attri->next_attribute())
				{
					if (SDL_strcmp(attri->name(), "points") == 0)
					{
						pointStream.clear();
						pointStream << attri->value();

						std::string pointPair;

						while (std::getline(pointStream, pointPair, ' '))
						{
							ValueMap pointDict;
							int x = 0, y = 0;
							SDL_sscanf(pointPair.c_str(), "%d,%d", &x, &y);
							pointDict["x"] = x;
							pointDict["y"] = y;

							pointArray.push_back(Value(pointDict));
						}
					}
				}
				dict.insert(std::make_pair(type, Value(pointArray)));
			}
		}
		objectGroup->addObject(dict);
	}

	objectGroup->retain();
	_objectGroups.push_back(objectGroup);
}

void FastTiledMap::loadImage(const std::string &filename)
{

	auto fullPath = FileUtils::getInstance()->fullPathForFilename(filename);

	if (fullPath.empty() && !_resourcePath.empty())
	{
		fullPath = _resourcePath + "/" + filename;
	}
	Director::getInstance()->getTextureCache()->addImage(fullPath, filename);
}

void FastTiledMap::parseProperties(rapidxml::xml_node<> *propRoot, ValueMap &properties)
{
	for (auto node = propRoot->first_node(); node; node = node->next_sibling())
	{
		std::string key;
		std::string value;
		std::string type = "string";

		for (auto attri = node->first_attribute(); attri != nullptr; attri = attri->next_attribute())
		{
			if (SDL_strcmp(attri->name(), "name") == 0)
			{
				key = std::string(attri->value());
			}
			else if (SDL_strcmp(attri->name(), "value") == 0)
			{
				value = std::string(attri->value());
			}
			else if (SDL_strcmp(attri->name(), "type") == 0)
			{
				type = std::string(attri->value());
			}
		}

		if (type == "bool" && value == "true")
		{
			properties.insert(std::make_pair(key, Value(true)));
		}
		else if (type == "bool" && value == "false")
		{
			properties.insert(std::make_pair(key, Value(false)));
		}
		else if (type == "float")
		{
			properties.insert(std::make_pair(key, Value(atof(value.c_str()))));
		}
		else
		{
			properties.insert(std::make_pair(key, Value(value)));
		}
	}
}

NS_SDL_END
