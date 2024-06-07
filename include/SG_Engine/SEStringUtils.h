#ifndef __SDL_StringUtils_H__
#define __SDL_StringUtils_H__

#include <cstdarg>
#include <string>
#include <cstring>
#include <sstream>
#include <functional>

#include "SDL2/SDL.h"

#include "SG_Engine/SEPlatformMarcos.h"
#include "SG_Engine/SEValue.h"

NS_SDL_BEGIN

static const char utf8_skip_data[256] = {
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1,
    2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
    2, 2, 2, 2, 2, 2, 2,
    3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 4, 4, 4, 4, 4, 4, 4, 4, 5,
    5, 5, 5, 6, 6, 1, 1
};

static const char *const g_utf8_skip = utf8_skip_data;

#define cc_utf8_next_char(p) (char *)((p) + g_utf8_skip[*(unsigned char *)(p)])

namespace StringUtils
{
    std::string format(const char *format, ...);

    ValueVector split(const std::string &src, const std::string &token);
    ValueVector split(const std::string &src, char ch);
    void split(const std::string &src, const std::string &token, const std::function<void(int, const Value &)> &callback);

    template <typename T>
    std::string toString(T t)
    {
        std::stringstream ss;
        ss << t;
        return ss.str();
    }

    long cc_utf8_strlen(const char *p, int max);

    unsigned int cc_utf8_get_char(const char *p);

    int cc_wcslen(const unsigned short *str);

}
NS_SDL_END
#endif
