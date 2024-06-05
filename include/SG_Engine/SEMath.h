#ifndef __SDL_Math_H__
#define __SDL_Math_H__

#include <cstdlib>
#include <cmath>
#include "SG_Engine/SEPlatformMarcos.h"

NS_SDL_BEGIN
#ifndef FLT_EPSILON
#define FLT_EPSILON 1.192092896e-07F
#endif
#define MAX_UINT 0xffffffff
#define SDL_REPEAT_FOREVER (MAX_UINT - 1)
#define MAX(x,y) ((x)>(y)?(x):(y))
#define MIN(x,y) ((x)>(y)?(y):(x))
#define RANDOM(start,end) (fmodf((float)rand(),(end)-(start))+(start))
#define RANDOM_0_1() (rand()/double(RAND_MAX))

#define RANDOM_MINUS1_1() ((2.f * rand()/(double)RAND_MAX) -1.f)
#define SDL_ANGLE_TO_DEGREE(angle) (3.1415926/180 * (angle))
#define SDL_DEGREE_TO_ANGLE(degree) (180.0/3.1415926 *(degree))
NS_SDL_END
#endif
