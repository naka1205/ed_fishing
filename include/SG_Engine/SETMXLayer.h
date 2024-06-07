#ifndef __SDL_TMXLayer_H__
#define __SDL_TMXLayer_H__
#include <vector>
#include <string>

#include "SG_Engine/SEBase64.h"
#include "SG_Engine/SEObject.h"
#include "SG_Engine/SELayer.h"
#include "SG_Engine/SEValue.h"

#include "rapidxml/rapidxml.hpp"
#include "zlib/zlib.h"

NS_SDL_BEGIN
class Sprite;
class TMXTiledMap;
class TMXLayer : public Layer
{
private:
	friend class TMXTiledMap;

private:
	std::string _layerName;

	std::vector<unsigned> _data;
	int _width;
	int _height;
	int _offsetX;
	int _offsetY;
	TMXTiledMap *_tiledMap;
	ValueMap _properties;

public:
	TMXLayer();
	~TMXLayer();
	static TMXLayer *create(rapidxml::xml_node<> *root);
	bool init(rapidxml::xml_node<> *root);

	int getTileGIDAt(const Point &tileCoordinate) const;
	int getTileGIDAt(int tileCoordinateX, int tileCoordinateY) const;

	Sprite *getTileAt(const Point &tileCoordinate);
	Sprite *getTileAt(int tileCoordinateX, int tileCoordinateY);

	void removeTileAt(const Point &tileCoordinate);

	void setTileGID(int gid, const Point &tileCoordinate);
	std::string getLayerName() const;

private:
	bool parseData(rapidxml::xml_node<> *root);
	bool parseProperties(rapidxml::xml_node<> *root);

	std::string trim(const std::string &str);
	std::string decode(const std::string &encoding, const std::string &str);
	void uncompression(const std::string &compression, const std::string &decodeIDs);
	void setTiledMap(TMXTiledMap *tiledMap);
};
NS_SDL_END
#endif
