#include "SG_Engine/SEFastLayer.h"
#include "SG_Engine/SESprite.h"
#include "SG_Engine/SENode.h"
#include "SG_Engine/SETMXTiledMap.h"
#include "SG_Engine/SETileset.h"
#include "SG_Engine/SEFastTiledMap.h"
#include "SG_Engine/SEDirector.h"
#include "SG_Engine/SETextureCache.h"

NS_SDL_BEGIN

FastLayer::FastLayer()
	: _width(0), _height(0), _offsetX(0), _offsetY(0), _tiledMap(nullptr), _bufferWidth(0), _bufferHeight(0), _tileWidth(0), _tileHeight(0), _extraSize(0), _buffer(nullptr), _deltaWidth(0), _deltaHeight(0), _mapOffsetX(0), _mapOffsetY(0), _carmarkX(0), _carmarkY(0), _cmdIndex(0), _otherPosX(0), _otherPosY(0)
{
}

FastLayer::~FastLayer()
{
}

FastLayer *FastLayer::create(rapidxml::xml_node<> *root)
{
	auto layer = new FastLayer();

	if (layer && layer->init(root))
		layer->autorelease();
	else
		SDL_SAFE_DELETE(layer);

	return layer;
}

bool FastLayer::init(rapidxml::xml_node<> *root)
{
	bool visible = true;

	for (auto attribute = root->first_attribute(); attribute != nullptr; attribute = attribute->next_attribute())
	{
		std::string sName = attribute->name();
		const char *value = attribute->value();

		if (sName == "name")
			_layerName = value;
		else if (sName == "width")
			_width = atoi(value);
		else if (sName == "height")
			_height = atoi(value);
		else if (sName == "offsetx")
			_offsetX = atoi(value);
		else if (sName == "offsety")
			_offsetY = atoi(value);
		else if (sName == "visible")
			visible = (atoi(value) != 0);
	}

	this->setName(_layerName);
	this->setVisible(visible);

	bool ret = false;
	for (auto node = root->first_node(); node; node = node->next_sibling())
	{
		if (SDL_strcmp(node->name(), "data") == 0)
			ret |= parseData(node);
		else if (SDL_strcmp(node->name(), "properties") == 0)
			ret |= this->parseProperties(node);
	}

	return ret;
}

void FastLayer::draw()
{

	Point position = this->convertToWorldSpace(_position);

	if (position.equals(_lastPosition))
	{
		this->fastDraw(-_otherPosX, -_otherPosY);
		return;
	}

	float dx = _lastPosition.x - position.x;
	float dy = _lastPosition.y - position.y;
	_lastPosition = position;

	this->fastDraw(-_otherPosX - dx, -_otherPosY - dy);

	bool exit = false;
	if (_otherPosX != 0 && !IS_FLOAT_ZERO(dx))
	{
		_otherPosX += dx;
		exit = true;
	}
	if (_otherPosY != 0 && !IS_FLOAT_ZERO(dy))
	{
		_otherPosY += dy;
		exit = true;
	}
	if (exit)
	{
		return;
	}

	bool ret = this->scroll(dx, dy);
	if (!ret)
	{
		_otherPosX += dx;
		_otherPosY += dy;
	}
}

int FastLayer::getTileGIDAt(const Point &pos) const
{
	int x = (int)pos.x;
	int y = (int)pos.y;

	return getTileGIDAt(x, y);
}

int FastLayer::getTileGIDAt(int tileCoordinateX, int tileCoordinateY) const
{
	if (tileCoordinateY >= 0 && tileCoordinateY < _height && tileCoordinateX >= 0 && tileCoordinateX < _width)
	{
		int z = (int)(tileCoordinateX + tileCoordinateY * _width);
		return _data[z];
	}
	return 0;
}

void FastLayer::removeTileAt(const Point &pos)
{
	int gid = this->getTileGIDAt(pos);
	if (gid)
	{
		int z = (int)(pos.x + pos.y * _width);

		_data[z] = 0;

		Sprite *tileSprite = this->getChildByTag<Sprite *>(z);
		if (tileSprite)
			tileSprite->removeFromParent();
	}
}

void FastLayer::setTileGID(int gid, const Point &pos)
{

	auto oldGid = this->getTileGIDAt(pos);
	if (oldGid == gid)
		return;

	if (gid == 0)
	{
		this->removeTileAt(pos);
	}
	else
	{

		if (oldGid == 0)
		{
			SDL_assert(SDL_FALSE);
		}
		else
		{

			int x = (int)pos.x;
			int y = (int)pos.y;
			int z = x + y * _width;
			_data[z] = gid;
		}
	}
}

std::string FastLayer::getLayerName() const
{
	return _layerName;
}

bool FastLayer::initCarmark()
{
	auto tileSize = _tiledMap->getTileSize();
	_tileWidth = (int)tileSize.width;
	_tileHeight = (int)tileSize.height;

	_extraSize = tileSize;

	Size visibleSize = Director::getInstance()->getVisibleSize();
	int temp = 0;
	while (temp < visibleSize.width)
		temp += _tileWidth;
	_bufferWidth = int(temp + _extraSize.width);

	temp = 0;
	while (temp < visibleSize.height)
		temp += _tileHeight;
	_bufferHeight = int(temp + _extraSize.height);

	_bufferRowTileNum = _bufferWidth / (int)tileSize.width;
	_bufferColTileNum = _bufferHeight / (int)tileSize.height;

	_deltaWidth = int(_width * _tileWidth - visibleSize.width);
	_deltaHeight = int(_height * _tileHeight - visibleSize.height);

	_deltaWidth = _deltaWidth > 0 ? _deltaWidth : 0;
	_deltaHeight = _deltaHeight > 0 ? _deltaHeight : 0;

	this->initBuffer();

	return true;
}

void FastLayer::initBuffer()
{

	Renderer *renderer = Director::getInstance()->getRenderer();
	_buffer = Texture::create(renderer, SDL_PIXELFORMAT_RGB444, SDL_TEXTUREACCESS_TARGET, _bufferWidth, _bufferHeight);
	_buffer->setBlendMode(SDL_BLENDMODE_BLEND);
	SDL_SAFE_RETAIN(_buffer);
	Color4B oldColor = renderer->getDrawColor();

	renderer->setTarget(_buffer);
	renderer->setDrawColor(Color4B(0, 0, 0, 0));
	renderer->clear();

	auto visibleSize = Director::getInstance()->getVisibleSize();

	int endX = _width > _bufferRowTileNum ? _bufferRowTileNum : _width;
	int endY = _height > _bufferColTileNum ? _bufferColTileNum : _height;

	for (int i = 0; i < endY; i++)
	{
		for (int j = 0; j < endX; j++)
		{
			int id = this->getTileGIDAt(j, i);
			this->drawTile(renderer, id, j * _tileWidth, i * _tileHeight);
		}
	}
	renderer->setTarget(nullptr);
	renderer->setDrawColor(oldColor);
}

void FastLayer::fastDraw(int x, int y)
{
	auto visibleSize = Director::getInstance()->getVisibleSize();
	int visibleWidth = (int)visibleSize.width;
	int visibleHeight = (int)visibleSize.height;

	int tempX = (int)_mapOffsetX % _bufferWidth;
	int tempY = (int)_mapOffsetY % _bufferHeight;

	int rightWidth = _bufferWidth - tempX;
	int rightHeight = _bufferHeight - tempY;

	drawRegion(tempX, tempY, rightWidth, rightHeight, x, y);

	drawRegion(0, tempY, visibleWidth - rightWidth, rightHeight, x + rightWidth, y);

	drawRegion(tempX, 0, rightWidth, visibleHeight - rightHeight, x, y + rightHeight);

	drawRegion(0, 0, visibleWidth - rightWidth, visibleHeight - rightHeight, x + rightWidth, y + rightHeight);
}

void FastLayer::drawRegion(int srcX, int srcY, int width, int height, int destX, int destY)
{

	if (width <= 0 || height <= 0)
		return;

	auto visibleSize = Director::getInstance()->getVisibleSize();
	int visibleWidth = (int)visibleSize.width;
	int visibleHeight = (int)visibleSize.height;

	width = width > visibleWidth ? visibleWidth : width;
	height = height > visibleHeight ? visibleHeight : height;

	SDL_Rect srcRect = {srcX, srcY, width, height};
	SDL_Rect destRect = {destX, destY, width, height};
	SDL_RendererFlip flip = SDL_FLIP_NONE;

	Point anchorPointInPoints(_anchorPoint.x * _tileWidth, _anchorPoint.y * _tileHeight);
	auto renderer = Director::getInstance()->getRenderer();

	_textureCmds[_cmdIndex].init(_globalZOrder, _buffer, srcRect, destRect, _rotation, anchorPointInPoints, flip);
	_textureCmds[_cmdIndex].setOpacity(_displayedOpacity);
	_textureCmds[_cmdIndex].setColorMod(_colorMod);

	renderer->addCommand(&_textureCmds[_cmdIndex]);
	_cmdIndex = (_cmdIndex + 1) % 4;
}

void FastLayer::drawTile(Renderer *renderer, int id, int destX, int destY)
{

	if (id == 0)
	{
		return;
	}
	Tileset *tileset = _tiledMap->getTilesetForGID(id);
	Texture *texture = Director::getInstance()->getTextureCache()->getTextureForKey(tileset->source);

	Rect srcRect = tileset->getRectForGID(id);
	Rect destRect = Rect(destX, destY, _tileWidth, _tileHeight);

	renderer->copy(texture, srcRect, destRect);
}

bool FastLayer::scroll(float x, float y)
{
	x += _mapOffsetX;
	y += _mapOffsetY;

	if (x < 0 || y < 0)
		return false;

	if (x > _deltaWidth)
	{
		_mapOffsetX = _deltaWidth;
		return false;
	}
	if (y > _deltaHeight)
	{
		_mapOffsetY = _deltaHeight;
		return false;
	}

	this->updateBuffer(x, y);
	return true;
}

void FastLayer::updateBuffer(float x, float y)
{
	_mapOffsetX = x;
	_mapOffsetY = y;

	if (x > _carmarkX + _extraSize.width)
	{
		int indexMapLastX = getIndexBufLastX();

		if (indexMapLastX < _width)
		{
			copyBufferX(indexMapLastX, getIndexCarmarkY(),
						getCarTileColNum(),
						getBufferCarmarkX(), getBufferCarmarkY());
			_carmarkX += _tileWidth;
		}
	}

	if (x < _carmarkX)
	{
		_carmarkX -= _tileWidth;
		copyBufferX(getIndexCarmarkX(), getIndexCarmarkY(),
					getCarTileColNum(),
					getBufferCarmarkX(), getBufferCarmarkY());
	}

	if (y > _carmarkY + _extraSize.height)
	{
		int indexMapLastY = getIndexBufLastY();

		if (indexMapLastY < _height)
		{
			copyBufferY(getIndexCarmarkX(), indexMapLastY,
						getCarTileRowNum(),
						getBufferCarmarkX(), getBufferCarmarkY());
			_carmarkY += _tileHeight;
		}
	}

	if (y < _carmarkY)
	{
		_carmarkY -= _tileHeight;
		copyBufferY(getIndexCarmarkX(), getIndexCarmarkY(),
					getCarTileRowNum(),
					getBufferCarmarkX(), getBufferCarmarkY());
	}
}

void FastLayer::copyBufferX(int indexMapX, int indexMapY, int tileColNum, int destX, int destY)
{
	int vy = 0;

	Renderer *renderer = Director::getInstance()->getRenderer();
	Color4B oldColor = renderer->getDrawColor();

	renderer->setTarget(_buffer);
	renderer->setDrawColor(Color4B(0, 0, 0, 0));

	SDL_Rect rect = {destX, 0, _tileWidth, _tileHeight * _bufferColTileNum};
	renderer->fillRect(rect);

	for (int j = 0; j < tileColNum; j++)
	{
		vy = j * _tileHeight + destY;
		int id = this->getTileGIDAt(indexMapX, indexMapY + j);

		this->drawTile(renderer, id, destX, vy);
	}

	for (int k = tileColNum; k < _bufferColTileNum; k++)
	{
		vy = (k - tileColNum) * _tileHeight;
		int id = this->getTileGIDAt(indexMapX, indexMapY + k);

		this->drawTile(renderer, id, destX, vy);
	}
	renderer->setTarget(nullptr);
	renderer->setDrawColor(oldColor);
}

void FastLayer::copyBufferY(int indexMapX, int indexMapY, int tileRowNum, int destX, int destY)
{
	int vx = 0;

	Renderer *renderer = Director::getInstance()->getRenderer();
	Color4B oldColor = renderer->getDrawColor();

	renderer->setTarget(_buffer);
	renderer->setDrawColor(Color4B(0, 0, 0, 0));

	SDL_Rect rect = {0, destY, _tileWidth * _bufferRowTileNum, _tileHeight};
	renderer->fillRect(rect);

	for (int i = 0; i < tileRowNum; i++)
	{
		vx = i * _tileWidth + destX;
		int id = this->getTileGIDAt(indexMapX + i, indexMapY);

		this->drawTile(renderer, id, vx, destY);
	}

	for (int k = tileRowNum; k < _bufferRowTileNum; k++)
	{
		vx = (k - tileRowNum) * _tileWidth;
		int id = this->getTileGIDAt(indexMapX + k, indexMapY);

		this->drawTile(renderer, id, vx, destY);
	}
	renderer->setTarget(nullptr);
	renderer->setDrawColor(oldColor);
}

int FastLayer::getIndexCarmarkX() const
{
	return _carmarkX / _tileWidth;
}

int FastLayer::getIndexCarmarkY() const
{
	return _carmarkY / _tileHeight;
}

int FastLayer::getBufferCarmarkX() const
{
	return _carmarkX % _bufferWidth;
}

int FastLayer::getBufferCarmarkY() const
{
	return _carmarkY % _bufferHeight;
}

int FastLayer::getIndexBufLastX() const
{
	return (_carmarkX + _bufferWidth) / _tileWidth;
}

int FastLayer::getIndexBufLastY() const
{
	return (_carmarkY + _bufferHeight) / _tileHeight;
}

int FastLayer::getCarTileRowNum() const
{
	return (_bufferWidth - _carmarkX % _bufferWidth) / _tileWidth;
}

int FastLayer::getCarTileColNum() const
{
	return (_bufferHeight - _carmarkY % _bufferHeight) / _tileHeight;
}

bool FastLayer::parseData(rapidxml::xml_node<> *root)
{
	std::string encoding = root->first_attribute("encoding")->value();
	std::string compression = root->first_attribute("compression")->value();
	std::string data = root->value();
	std::string decodeIDs;

	decodeIDs = this->decode(encoding, trim(data));
	this->uncompression(compression, decodeIDs);

	return true;
}

bool FastLayer::parseProperties(rapidxml::xml_node<> *root)
{
	for (auto node = root->first_node(); node; node = node->next_sibling())
	{
		std::string key = node->first_attribute("name")->value();
		std::string value = node->first_attribute("value")->value();

		_properties.insert(std::make_pair(key, Value(value)));
	}
	return true;
}

std::string FastLayer::trim(const std::string &str)
{

	char *text = new char[str.length() + 1];

	memset(text, '\0', str.length() + 1);

	int i = 0;
	for (unsigned int j = 0; j < str.length();)
	{
		auto ch = str.at(j++);
		if (ch == '\t' || ch == '\n' || ch == '\r' || ch == ' ')
		{
			;
		}
		else
		{
			text[i++] = ch;
		}
	}
	text[i] = '\0';
	std::string trimstr = std::string(text);
	delete[] text;
	return trimstr;
}

std::string FastLayer::decode(const std::string &encoding, const std::string &str)
{
	std::string decodeIDs;
	if (encoding == "base64")
	{
		decodeIDs = base64_decode(str);
	}
	return decodeIDs;
}

void FastLayer::uncompression(const std::string &compression, const std::string &decodeIDs)
{
	if (compression == "zlib")
	{
		uLongf numGids = _width * _height * sizeof(unsigned);
		auto gids = std::vector<unsigned>(numGids);

		// unzip
		uncompress((Bytef *)&gids[0], &numGids, (const Bytef *)decodeIDs.c_str(), decodeIDs.size());

		// 赋值
		_data = std::vector<unsigned>(_width * _height);
		std::copy(gids.begin(), gids.begin() + _width * _height, _data.begin());
	}
}

void FastLayer::setTiledMap(FastTiledMap *tiledMap)
{
	_tiledMap = tiledMap;
}
NS_SDL_END