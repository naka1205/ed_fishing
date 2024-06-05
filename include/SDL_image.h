
#ifndef SDL_IMAGE_H_
#define SDL_IMAGE_H_

#include "SDL2/SDL.h"
#include "SDL2/SDL_version.h"
#include "SDL2/begin_code.h"

#ifdef __cplusplus
extern "C"
{
#endif

#define SDL_IMAGE_MAJOR_VERSION 2
#define SDL_IMAGE_MINOR_VERSION 8
#define SDL_IMAGE_PATCHLEVEL 1

#define SDL_IMAGE_VERSION(X)                  \
    {                                         \
        (X)->major = SDL_IMAGE_MAJOR_VERSION; \
        (X)->minor = SDL_IMAGE_MINOR_VERSION; \
        (X)->patch = SDL_IMAGE_PATCHLEVEL;    \
    }

#if SDL_IMAGE_MAJOR_VERSION < 3 && SDL_MAJOR_VERSION < 3

#define SDL_IMAGE_COMPILEDVERSION \
    SDL_VERSIONNUM(SDL_IMAGE_MAJOR_VERSION, SDL_IMAGE_MINOR_VERSION, SDL_IMAGE_PATCHLEVEL)
#endif

#define SDL_IMAGE_VERSION_ATLEAST(X, Y, Z)                            \
    ((SDL_IMAGE_MAJOR_VERSION >= X) &&                                \
     (SDL_IMAGE_MAJOR_VERSION > X || SDL_IMAGE_MINOR_VERSION >= Y) && \
     (SDL_IMAGE_MAJOR_VERSION > X || SDL_IMAGE_MINOR_VERSION > Y || SDL_IMAGE_PATCHLEVEL >= Z))

    extern DECLSPEC const SDL_version *SDLCALL IMG_Linked_Version(void);

    typedef enum
    {
        IMG_INIT_JPG = 0x00000001,
        IMG_INIT_PNG = 0x00000002,
        IMG_INIT_TIF = 0x00000004,
        IMG_INIT_WEBP = 0x00000008,
        IMG_INIT_JXL = 0x00000010,
        IMG_INIT_AVIF = 0x00000020
    } IMG_InitFlags;

    extern DECLSPEC int SDLCALL IMG_Init(int flags);

    extern DECLSPEC void SDLCALL IMG_Quit(void);

    extern DECLSPEC SDL_Surface *SDLCALL IMG_LoadTyped_RW(SDL_RWops *src, int freesrc, const char *type);

    extern DECLSPEC SDL_Surface *SDLCALL IMG_Load(const char *file);

    extern DECLSPEC SDL_Surface *SDLCALL IMG_Load_RW(SDL_RWops *src, int freesrc);

#if SDL_VERSION_ATLEAST(2, 0, 0)

    extern DECLSPEC SDL_Texture *SDLCALL IMG_LoadTexture(SDL_Renderer *renderer, const char *file);

    extern DECLSPEC SDL_Texture *SDLCALL IMG_LoadTexture_RW(SDL_Renderer *renderer, SDL_RWops *src, int freesrc);

    extern DECLSPEC SDL_Texture *SDLCALL IMG_LoadTextureTyped_RW(SDL_Renderer *renderer, SDL_RWops *src, int freesrc, const char *type);
#endif

    extern DECLSPEC int SDLCALL IMG_isAVIF(SDL_RWops *src);

    extern DECLSPEC int SDLCALL IMG_isICO(SDL_RWops *src);

    extern DECLSPEC int SDLCALL IMG_isCUR(SDL_RWops *src);

    extern DECLSPEC int SDLCALL IMG_isBMP(SDL_RWops *src);

    extern DECLSPEC int SDLCALL IMG_isGIF(SDL_RWops *src);

    extern DECLSPEC int SDLCALL IMG_isJPG(SDL_RWops *src);

    extern DECLSPEC int SDLCALL IMG_isJXL(SDL_RWops *src);

    extern DECLSPEC int SDLCALL IMG_isLBM(SDL_RWops *src);

    extern DECLSPEC int SDLCALL IMG_isPCX(SDL_RWops *src);

    extern DECLSPEC int SDLCALL IMG_isPNG(SDL_RWops *src);

    extern DECLSPEC int SDLCALL IMG_isPNM(SDL_RWops *src);

    extern DECLSPEC int SDLCALL IMG_isSVG(SDL_RWops *src);

    extern DECLSPEC int SDLCALL IMG_isQOI(SDL_RWops *src);

    extern DECLSPEC int SDLCALL IMG_isTIF(SDL_RWops *src);

    extern DECLSPEC int SDLCALL IMG_isXCF(SDL_RWops *src);

    extern DECLSPEC int SDLCALL IMG_isXPM(SDL_RWops *src);

    extern DECLSPEC int SDLCALL IMG_isXV(SDL_RWops *src);

    extern DECLSPEC int SDLCALL IMG_isWEBP(SDL_RWops *src);

    extern DECLSPEC SDL_Surface *SDLCALL IMG_LoadAVIF_RW(SDL_RWops *src);

    extern DECLSPEC SDL_Surface *SDLCALL IMG_LoadICO_RW(SDL_RWops *src);

    extern DECLSPEC SDL_Surface *SDLCALL IMG_LoadCUR_RW(SDL_RWops *src);

    extern DECLSPEC SDL_Surface *SDLCALL IMG_LoadBMP_RW(SDL_RWops *src);

    extern DECLSPEC SDL_Surface *SDLCALL IMG_LoadGIF_RW(SDL_RWops *src);

    extern DECLSPEC SDL_Surface *SDLCALL IMG_LoadJPG_RW(SDL_RWops *src);

    extern DECLSPEC SDL_Surface *SDLCALL IMG_LoadJXL_RW(SDL_RWops *src);

    extern DECLSPEC SDL_Surface *SDLCALL IMG_LoadLBM_RW(SDL_RWops *src);

    extern DECLSPEC SDL_Surface *SDLCALL IMG_LoadPCX_RW(SDL_RWops *src);

    extern DECLSPEC SDL_Surface *SDLCALL IMG_LoadPNG_RW(SDL_RWops *src);

    extern DECLSPEC SDL_Surface *SDLCALL IMG_LoadPNM_RW(SDL_RWops *src);

    extern DECLSPEC SDL_Surface *SDLCALL IMG_LoadSVG_RW(SDL_RWops *src);

    extern DECLSPEC SDL_Surface *SDLCALL IMG_LoadQOI_RW(SDL_RWops *src);

    extern DECLSPEC SDL_Surface *SDLCALL IMG_LoadTGA_RW(SDL_RWops *src);

    extern DECLSPEC SDL_Surface *SDLCALL IMG_LoadTIF_RW(SDL_RWops *src);

    extern DECLSPEC SDL_Surface *SDLCALL IMG_LoadXCF_RW(SDL_RWops *src);

    extern DECLSPEC SDL_Surface *SDLCALL IMG_LoadXPM_RW(SDL_RWops *src);

    extern DECLSPEC SDL_Surface *SDLCALL IMG_LoadXV_RW(SDL_RWops *src);

    extern DECLSPEC SDL_Surface *SDLCALL IMG_LoadWEBP_RW(SDL_RWops *src);

    extern DECLSPEC SDL_Surface *SDLCALL IMG_LoadSizedSVG_RW(SDL_RWops *src, int width, int height);

    extern DECLSPEC SDL_Surface *SDLCALL IMG_ReadXPMFromArray(char **xpm);

    extern DECLSPEC SDL_Surface *SDLCALL IMG_ReadXPMFromArrayToRGB888(char **xpm);

    extern DECLSPEC int SDLCALL IMG_SavePNG(SDL_Surface *surface, const char *file);

    extern DECLSPEC int SDLCALL IMG_SavePNG_RW(SDL_Surface *surface, SDL_RWops *dst, int freedst);

    extern DECLSPEC int SDLCALL IMG_SaveJPG(SDL_Surface *surface, const char *file, int quality);

    extern DECLSPEC int SDLCALL IMG_SaveJPG_RW(SDL_Surface *surface, SDL_RWops *dst, int freedst, int quality);

    typedef struct
    {
        int w, h;
        int count;
        SDL_Surface **frames;
        int *delays;
    } IMG_Animation;

    extern DECLSPEC IMG_Animation *SDLCALL IMG_LoadAnimation(const char *file);

    extern DECLSPEC IMG_Animation *SDLCALL IMG_LoadAnimation_RW(SDL_RWops *src, int freesrc);

    extern DECLSPEC IMG_Animation *SDLCALL IMG_LoadAnimationTyped_RW(SDL_RWops *src, int freesrc, const char *type);

    extern DECLSPEC void SDLCALL IMG_FreeAnimation(IMG_Animation *anim);

    extern DECLSPEC IMG_Animation *SDLCALL IMG_LoadGIFAnimation_RW(SDL_RWops *src);

    extern DECLSPEC IMG_Animation *SDLCALL IMG_LoadWEBPAnimation_RW(SDL_RWops *src);

#define IMG_SetError SDL_SetError

#define IMG_GetError SDL_GetError

#ifdef __cplusplus
}
#endif
#include "SDL2/close_code.h"

#endif
