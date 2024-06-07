#ifndef __SDL_FastTiledMap_H__
#define __SDL_FastTiledMap_H__

#include <vector>
#include <map>
#include <cmath>
#include <string>
#include <memory>
#include <algorithm>
#include <sstream>

#include "SEObject.h"
#include "SENode.h"
#include "SEValue.h"
#include "SETMXImageLayer.h"

#include "../rapidxml/rapidxml.hpp"

NS_SDL_BEGIN
class Tileset;
class FastLayer;
class TMXObjectGroup;
class Texture;

class FastTiledMap : public Node
{
public:
	FastTiledMap();
	~FastTiledMap();
	static FastTiledMap *create(const std::string &filename);
	bool init(const std::string &filename);

	Tileset *getTilesetForGID(int gid) const;

	Value getPropertyForName(const std::string &key) const;
	const ValueMap &getProperties() const;

	ValueMap getTilePropertiesForGID(int gid) const;
	bool getTilePropertiesForGID(int gid, ValueMap **value);

	Node *getLayer(const std::string &name);
	const Node *getLayer(const std::string &name) const;

	template <typename T>
	T getLayer(const std::string &name) { return dynamic_cast<T>(getLayer(name)); }

	Size getTileSize() const;

	Size getMapSize() const;

	const std::vector<TMXObjectGroup *> &getObjectGroups() const;
	const TMXObjectGroup *getObjectGroup(const std::string &name) const;

	const std::vector<Tileset *> &getTilesets() const;

private:
	bool parse(const std::string &filename);

	void parseTileset(rapidxml::xml_node<> *tileRoot);
	void parseLayer(rapidxml::xml_node<> *layerRoot);
	void parseImageLayer(rapidxml::xml_node<> *layerRoot);
	void parseProperties(rapidxml::xml_node<> *propRoot);
	void parseObjectGroup(rapidxml::xml_node<> *objectLayer);

	void loadImage(const std::string &filename);
	void parseProperties(rapidxml::xml_node<> *propertyRoot, ValueMap &properties);

private:
	std::vector<Tileset *> _tilesets;

	ValueMap _properties;

	std::vector<TMXObjectGroup *> _objectGroups;
	Size _mapSize;
	Size _tileSize;

	int _mapOrientation;

	std::string _resourcePath;
};
NS_SDL_END
#endif
