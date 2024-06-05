#ifndef __SDL_NS_H__
#define __SDL_NS_H__
#include <cstdlib>
#include <string>
#include <vector>

#include "SG_Engine/SEPoint.h"
#include "SG_Engine/SESize.h"
#include "SG_Engine/SERect.h"
NS_SDL_BEGIN

Point PointFromString(const std::string &str, char token = ',');

Size SizeFromString(const std::string &str, char token = ',');

Rect RectFromString(const std::string &str, char token = ',');
NS_SDL_END
#endif
