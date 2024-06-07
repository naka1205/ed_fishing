#ifndef __SDL_TMXTiledMap_H__
#define __SDL_TMXTiledMap_H__
#include <vector>
#include <map>
#include <cmath>
#include <string>
#include <memory>
#include <algorithm>
#include <sstream>

#include "SG_Engine/SEObject.h"
#include "SG_Engine/SENode.h"
#include "SG_Engine/SEValue.h"
#include "SG_Engine/SETMXLayer.h"
#include "SG_Engine/SETMXImageLayer.h"

#include "rapidxml/rapidxml.hpp"

NS_SDL_BEGIN
class Tileset;
class TMXLayer;
class TMXObjectGroup;
class Animate;
class Texture;

class TMXTiledMap : public Node
{
private:
	std::vector<Tileset *> _tilesets;

	ValueMap _properties;

	std::vector<TMXObjectGroup *> _objectGroups;
	Size _mapSize;
	Size _tileSize;
	int _mapOrientation;
	std::string _resourcePath;

public:
	TMXTiledMap();
	~TMXTiledMap();
	static TMXTiledMap *create(const std::string &filename);
	bool init(const std::string &filename);

	Tileset *getTilesetForGID(int gid) const;

	Value getPropertyForName(const std::string &key) const;
	ValueMap &getProperties();

	ValueMap getTilePropertiesForGID(int gid) const;
	bool getTilePropertiesForGID(int gid, ValueMap **value);

	Node *getLayer(const std::string &name);
	template <typename T>
	T getLayer(const std::string &name) { return dynamic_cast<T>(getLayer(name)); }

	Size getTileSize() const;

	Size getMapSize() const;

	std::vector<TMXObjectGroup *> &getObjectGroups();
	TMXObjectGroup *getObjectGroup(const std::string &name);

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

	Animate *tryToCreateAnimate(int id, Tileset *tileset);
};
NS_SDL_END
#endif
