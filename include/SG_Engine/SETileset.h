#ifndef __SDL_Tileset_H__
#define __SDL_Tileset_H__
#include <string>
#include <vector>
#include <unordered_map>

#include "SDL2/SDL.h"

#include "SG_Engine/SEObject.h"
#include "SG_Engine/SEValue.h"
#include "SG_Engine/SERect.h"

#include "rapidxml/rapidxml.hpp"

NS_SDL_BEGIN

struct TileAnimation
{
	int tileid;
	float duration;

public:
	TileAnimation()
		: tileid(0), duration(0.f) {}
	TileAnimation(int id, float duration)
		: tileid(id), duration(duration) {}
};
class Tileset : public Object
{
public:
	int firstGrid;
	int tileWidth;
	int tileHeight;

	int spacing;
	int margin;

	int width;
	int height;

	int tileCount;
	int column;

	std::string name;
	std::string source;

public:
	std::unordered_map<int, ValueMap> _properties;
	std::unordered_map<int, std::vector<TileAnimation>> _tileAnimations;

public:
	Tileset();
	~Tileset();
	static Tileset *create(rapidxml::xml_node<> *root);
	static Tileset *create(rapidxml::xml_node<> *root, rapidxml::xml_node<> *tsxRoot);

	bool init(rapidxml::xml_node<> *root);
	bool init(rapidxml::xml_node<> *root, rapidxml::xml_node<> *tsxRoot);

	Rect getRectForGID(int gid) const;

	ValueMap getPropertiesForGID(int id);
	std::unordered_map<int, std::vector<TileAnimation>> &getTileAnimations();

	const std::unordered_map<int, ValueMap> &getProperties() const;

private:
	void initlize(rapidxml::xml_node<> *root);
	void parseImage(rapidxml::xml_node<> *imageRoot);
	void parseTile(rapidxml::xml_node<> *tileRoot);
	void parseProperties(int id, rapidxml::xml_node<> *root);

	void parseTileAnimation(int id, rapidxml::xml_node<> *animationRoot);
};
NS_SDL_END
#endif
