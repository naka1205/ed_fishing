#ifndef __SDL_FastLayer_H__
#define __SDL_FastLayer_H__
#include <vector>
#include <string>

#include "zlib/zlib.h"

#include "SG_Engine/SEBase64.h"
#include "SG_Engine/SEObject.h"
#include "SG_Engine/SELayer.h"

#include "SG_Engine/SEValue.h"
#include "SG_Engine/SERenderCommand.h"

#include "rapidxml/rapidxml.hpp"

NS_SDL_BEGIN

class Sprite;
class FastTiledMap;
class Texture;

class FastLayer : public Layer
{
public:
	FastLayer();
	~FastLayer();

	static FastLayer *create(rapidxml::xml_node<> *root);
	bool init(rapidxml::xml_node<> *root);

	virtual void draw();

	int getTileGIDAt(const Point &tileCoordinate) const;
	int getTileGIDAt(int tileCoordinateX, int tileCoordinateY) const;

	void removeTileAt(const Point &tileCoordinate);

	void setTileGID(int gid, const Point &tileCoordinate);

	std::string getLayerName() const;

private:
	bool initCarmark();

	void initBuffer();

	void fastDraw(int x, int y);
	void drawRegion(int srcX, int srcY, int width, int height, int destX, int destY);
	void drawTile(Renderer *renderer, int id, int destX, int destY);

	bool scroll(float x, float y);
	void updateBuffer(float x, float y);
	void copyBufferX(int indexMapX, int indexMapY, int tileColNum, int destX, int destY);
	void copyBufferY(int indexMapX, int indexMapY, int tileRowNum, int destX, int destY);

	int getIndexCarmarkX() const;
	int getIndexCarmarkY() const;

	int getBufferCarmarkX() const;
	int getBufferCarmarkY() const;

	int getIndexBufLastX() const;
	int getIndexBufLastY() const;

	int getCarTileRowNum() const;
	int getCarTileColNum() const;

private:
	bool parseData(rapidxml::xml_node<> *root);
	bool parseProperties(rapidxml::xml_node<> *root);

	std::string trim(const std::string &str);
	std::string decode(const std::string &encoding, const std::string &str);
	void uncompression(const std::string &compression, const std::string &decodeIDs);
	void setTiledMap(FastTiledMap *tiledMap);

private:
	friend class FastTiledMap;

private:
	std::string _layerName;

	std::vector<unsigned> _data;

	int _width;
	int _height;
	int _offsetX;
	int _offsetY;

	FastTiledMap *_tiledMap;
	ValueMap _properties;

	int _bufferWidth;
	int _bufferHeight;

	int _bufferRowTileNum;
	int _bufferColTileNum;

	int _tileWidth;
	int _tileHeight;

	Size _extraSize;

	Texture *_buffer;

	int _deltaWidth;
	int _deltaHeight;

	float _mapOffsetX;
	float _mapOffsetY;

	int _carmarkX;
	int _carmarkY;

	TextureCommand _textureCmds[4];
	unsigned _cmdIndex;

	Point _lastPosition;

	float _otherPosX;
	float _otherPosY;
};
NS_SDL_END
#endif
