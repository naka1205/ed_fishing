#ifndef _SDL_ANIGIF_H
#define _SDL_ANIGIF_H

#include "SDL2/SDL.h"
#include "SDL2/begin_code.h"

#ifdef __cplusplus
extern "C"
{
#endif

    typedef struct
    {
        SDL_Surface *surface;
        int x, y;
        int disposal;
        int delay;
        int user;
    } AG_Frame;

#define AG_DISPOSE_NA 0
#define AG_DISPOSE_NONE 1
#define AG_DISPOSE_RESTORE_BACKGROUND 2
#define AG_DISPOSE_RESTORE_PREVIOUS 3

    extern DECLSPEC int AG_isGIF(SDL_RWops *src);
    extern DECLSPEC int AG_LoadGIF(const char *file, AG_Frame *frames, int maxFrames);
    extern DECLSPEC void AG_FreeSurfaces(AG_Frame *frames, int nFrames);
    extern DECLSPEC int AG_LoadGIF_RW(SDL_RWops *src, AG_Frame *frames, int size);

#ifdef __cplusplus
}
#endif
#include "SDL2/close_code.h"

#endif
