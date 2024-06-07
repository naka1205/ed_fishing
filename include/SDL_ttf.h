#ifndef SDL_TTF_H_
#define SDL_TTF_H_

#include "SDL2/SDL.h"
#include "SDL2/begin_code.h"


#ifdef __cplusplus
extern "C" {
#endif


#define SDL_TTF_MAJOR_VERSION   2
#define SDL_TTF_MINOR_VERSION   20
#define SDL_TTF_PATCHLEVEL      2


#define SDL_TTF_VERSION(X)                          \
{                                                   \
    (X)->major = SDL_TTF_MAJOR_VERSION;             \
    (X)->minor = SDL_TTF_MINOR_VERSION;             \
    (X)->patch = SDL_TTF_PATCHLEVEL;                \
}


#define TTF_MAJOR_VERSION   SDL_TTF_MAJOR_VERSION
#define TTF_MINOR_VERSION   SDL_TTF_MINOR_VERSION
#define TTF_PATCHLEVEL      SDL_TTF_PATCHLEVEL
#define TTF_VERSION(X)      SDL_TTF_VERSION(X)

#if SDL_TTF_MAJOR_VERSION < 3 && SDL_MAJOR_VERSION < 3

#define SDL_TTF_COMPILEDVERSION \
    SDL_VERSIONNUM(SDL_TTF_MAJOR_VERSION, SDL_TTF_MINOR_VERSION, SDL_TTF_PATCHLEVEL)
#endif


#define SDL_TTF_VERSION_ATLEAST(X, Y, Z) \
    ((SDL_TTF_MAJOR_VERSION >= X) && \
     (SDL_TTF_MAJOR_VERSION > X || SDL_TTF_MINOR_VERSION >= Y) && \
     (SDL_TTF_MAJOR_VERSION > X || SDL_TTF_MINOR_VERSION > Y || SDL_TTF_PATCHLEVEL >= Z))

#ifndef SDL_DEPRECATED
#define SDL_DEPRECATED
#endif

extern DECLSPEC const SDL_version * SDLCALL TTF_Linked_Version(void);

extern DECLSPEC void SDLCALL TTF_GetFreeTypeVersion(int *major, int *minor, int *patch);

extern DECLSPEC void SDLCALL TTF_GetHarfBuzzVersion(int *major, int *minor, int *patch);

#define UNICODE_BOM_NATIVE  0xFEFF
#define UNICODE_BOM_SWAPPED 0xFFFE


extern DECLSPEC void SDLCALL TTF_ByteSwappedUNICODE(SDL_bool swapped);


typedef struct _TTF_Font TTF_Font;

extern DECLSPEC int SDLCALL TTF_Init(void);

extern DECLSPEC TTF_Font * SDLCALL TTF_OpenFont(const char *file, int ptsize);

extern DECLSPEC TTF_Font * SDLCALL TTF_OpenFontIndex(const char *file, int ptsize, long index);

extern DECLSPEC TTF_Font * SDLCALL TTF_OpenFontRW(SDL_RWops *src, int freesrc, int ptsize);

extern DECLSPEC TTF_Font * SDLCALL TTF_OpenFontIndexRW(SDL_RWops *src, int freesrc, int ptsize, long index);

extern DECLSPEC TTF_Font * SDLCALL TTF_OpenFontDPI(const char *file, int ptsize, unsigned int hdpi, unsigned int vdpi);

extern DECLSPEC TTF_Font * SDLCALL TTF_OpenFontIndexDPI(const char *file, int ptsize, long index, unsigned int hdpi, unsigned int vdpi);

extern DECLSPEC TTF_Font * SDLCALL TTF_OpenFontDPIRW(SDL_RWops *src, int freesrc, int ptsize, unsigned int hdpi, unsigned int vdpi);

extern DECLSPEC TTF_Font * SDLCALL TTF_OpenFontIndexDPIRW(SDL_RWops *src, int freesrc, int ptsize, long index, unsigned int hdpi, unsigned int vdpi);

extern DECLSPEC int SDLCALL TTF_SetFontSize(TTF_Font *font, int ptsize);

extern DECLSPEC int SDLCALL TTF_SetFontSizeDPI(TTF_Font *font, int ptsize, unsigned int hdpi, unsigned int vdpi);

#define TTF_STYLE_NORMAL        0x00
#define TTF_STYLE_BOLD          0x01
#define TTF_STYLE_ITALIC        0x02
#define TTF_STYLE_UNDERLINE     0x04
#define TTF_STYLE_STRIKETHROUGH 0x08

extern DECLSPEC int SDLCALL TTF_GetFontStyle(const TTF_Font *font);

extern DECLSPEC void SDLCALL TTF_SetFontStyle(TTF_Font *font, int style);

extern DECLSPEC int SDLCALL TTF_GetFontOutline(const TTF_Font *font);

extern DECLSPEC void SDLCALL TTF_SetFontOutline(TTF_Font *font, int outline);

#define TTF_HINTING_NORMAL          0
#define TTF_HINTING_LIGHT           1
#define TTF_HINTING_MONO            2
#define TTF_HINTING_NONE            3
#define TTF_HINTING_LIGHT_SUBPIXEL  4

extern DECLSPEC int SDLCALL TTF_GetFontHinting(const TTF_Font *font);

extern DECLSPEC void SDLCALL TTF_SetFontHinting(TTF_Font *font, int hinting);

#define TTF_WRAPPED_ALIGN_LEFT      0
#define TTF_WRAPPED_ALIGN_CENTER    1
#define TTF_WRAPPED_ALIGN_RIGHT     2

extern DECLSPEC int SDLCALL TTF_GetFontWrappedAlign(const TTF_Font *font);

extern DECLSPEC void SDLCALL TTF_SetFontWrappedAlign(TTF_Font *font, int align);

extern DECLSPEC int SDLCALL TTF_FontHeight(const TTF_Font *font);

extern DECLSPEC int SDLCALL TTF_FontAscent(const TTF_Font *font);

extern DECLSPEC int SDLCALL TTF_FontDescent(const TTF_Font *font);

extern DECLSPEC int SDLCALL TTF_FontLineSkip(const TTF_Font *font);

extern DECLSPEC int SDLCALL TTF_GetFontKerning(const TTF_Font *font);

extern DECLSPEC void SDLCALL TTF_SetFontKerning(TTF_Font *font, int allowed);

extern DECLSPEC long SDLCALL TTF_FontFaces(const TTF_Font *font);

extern DECLSPEC int SDLCALL TTF_FontFaceIsFixedWidth(const TTF_Font *font);

extern DECLSPEC const char * SDLCALL TTF_FontFaceFamilyName(const TTF_Font *font);

extern DECLSPEC const char * SDLCALL TTF_FontFaceStyleName(const TTF_Font *font);

extern DECLSPEC int SDLCALL TTF_GlyphIsProvided(TTF_Font *font, Uint16 ch);

extern DECLSPEC int SDLCALL TTF_GlyphIsProvided32(TTF_Font *font, Uint32 ch);

extern DECLSPEC int SDLCALL TTF_GlyphMetrics(TTF_Font *font, Uint16 ch,
                        int *minx, int *maxx,
                        int *miny, int *maxy, int *advance);


extern DECLSPEC int SDLCALL TTF_GlyphMetrics32(TTF_Font *font, Uint32 ch,
                        int *minx, int *maxx,
                        int *miny, int *maxy, int *advance);


extern DECLSPEC int SDLCALL TTF_SizeText(TTF_Font *font, const char *text, int *w, int *h);


extern DECLSPEC int SDLCALL TTF_SizeUTF8(TTF_Font *font, const char *text, int *w, int *h);


extern DECLSPEC int SDLCALL TTF_SizeUNICODE(TTF_Font *font, const Uint16 *text, int *w, int *h);


extern DECLSPEC int SDLCALL TTF_MeasureText(TTF_Font *font, const char *text, int measure_width, int *extent, int *count);


extern DECLSPEC int SDLCALL TTF_MeasureUTF8(TTF_Font *font, const char *text, int measure_width, int *extent, int *count);


extern DECLSPEC int SDLCALL TTF_MeasureUNICODE(TTF_Font *font, const Uint16 *text, int measure_width, int *extent, int *count);


extern DECLSPEC SDL_Surface * SDLCALL TTF_RenderText_Solid(TTF_Font *font,
                const char *text, SDL_Color fg);


extern DECLSPEC SDL_Surface * SDLCALL TTF_RenderUTF8_Solid(TTF_Font *font,
                const char *text, SDL_Color fg);


extern DECLSPEC SDL_Surface * SDLCALL TTF_RenderUNICODE_Solid(TTF_Font *font,
                const Uint16 *text, SDL_Color fg);


extern DECLSPEC SDL_Surface * SDLCALL TTF_RenderText_Solid_Wrapped(TTF_Font *font,
                const char *text, SDL_Color fg, Uint32 wrapLength);


extern DECLSPEC SDL_Surface * SDLCALL TTF_RenderUTF8_Solid_Wrapped(TTF_Font *font,
                const char *text, SDL_Color fg, Uint32 wrapLength);


extern DECLSPEC SDL_Surface * SDLCALL TTF_RenderUNICODE_Solid_Wrapped(TTF_Font *font,
                const Uint16 *text, SDL_Color fg, Uint32 wrapLength);


extern DECLSPEC SDL_Surface * SDLCALL TTF_RenderGlyph_Solid(TTF_Font *font,
                Uint16 ch, SDL_Color fg);


extern DECLSPEC SDL_Surface * SDLCALL TTF_RenderGlyph32_Solid(TTF_Font *font,
                Uint32 ch, SDL_Color fg);


extern DECLSPEC SDL_Surface * SDLCALL TTF_RenderText_Shaded(TTF_Font *font,
                const char *text, SDL_Color fg, SDL_Color bg);


extern DECLSPEC SDL_Surface * SDLCALL TTF_RenderUTF8_Shaded(TTF_Font *font,
                const char *text, SDL_Color fg, SDL_Color bg);


extern DECLSPEC SDL_Surface * SDLCALL TTF_RenderUNICODE_Shaded(TTF_Font *font,
                const Uint16 *text, SDL_Color fg, SDL_Color bg);


extern DECLSPEC SDL_Surface * SDLCALL TTF_RenderText_Shaded_Wrapped(TTF_Font *font,
                const char *text, SDL_Color fg, SDL_Color bg, Uint32 wrapLength);


extern DECLSPEC SDL_Surface * SDLCALL TTF_RenderUTF8_Shaded_Wrapped(TTF_Font *font,
                const char *text, SDL_Color fg, SDL_Color bg, Uint32 wrapLength);


extern DECLSPEC SDL_Surface * SDLCALL TTF_RenderUNICODE_Shaded_Wrapped(TTF_Font *font,
                const Uint16 *text, SDL_Color fg, SDL_Color bg, Uint32 wrapLength);


extern DECLSPEC SDL_Surface * SDLCALL TTF_RenderGlyph_Shaded(TTF_Font *font,
                Uint16 ch, SDL_Color fg, SDL_Color bg);


extern DECLSPEC SDL_Surface * SDLCALL TTF_RenderGlyph32_Shaded(TTF_Font *font,
                Uint32 ch, SDL_Color fg, SDL_Color bg);


extern DECLSPEC SDL_Surface * SDLCALL TTF_RenderText_Blended(TTF_Font *font,
                const char *text, SDL_Color fg);


extern DECLSPEC SDL_Surface * SDLCALL TTF_RenderUTF8_Blended(TTF_Font *font,
                const char *text, SDL_Color fg);


extern DECLSPEC SDL_Surface * SDLCALL TTF_RenderUNICODE_Blended(TTF_Font *font,
                const Uint16 *text, SDL_Color fg);


extern DECLSPEC SDL_Surface * SDLCALL TTF_RenderText_Blended_Wrapped(TTF_Font *font,
                const char *text, SDL_Color fg, Uint32 wrapLength);


extern DECLSPEC SDL_Surface * SDLCALL TTF_RenderUTF8_Blended_Wrapped(TTF_Font *font,
                const char *text, SDL_Color fg, Uint32 wrapLength);


extern DECLSPEC SDL_Surface * SDLCALL TTF_RenderUNICODE_Blended_Wrapped(TTF_Font *font,
                const Uint16 *text, SDL_Color fg, Uint32 wrapLength);


extern DECLSPEC SDL_Surface * SDLCALL TTF_RenderGlyph_Blended(TTF_Font *font,
                Uint16 ch, SDL_Color fg);


extern DECLSPEC SDL_Surface * SDLCALL TTF_RenderGlyph32_Blended(TTF_Font *font,
                Uint32 ch, SDL_Color fg);


extern DECLSPEC SDL_Surface * SDLCALL TTF_RenderText_LCD(TTF_Font *font,
                const char *text, SDL_Color fg, SDL_Color bg);


extern DECLSPEC SDL_Surface * SDLCALL TTF_RenderUTF8_LCD(TTF_Font *font,
                const char *text, SDL_Color fg, SDL_Color bg);


extern DECLSPEC SDL_Surface * SDLCALL TTF_RenderUNICODE_LCD(TTF_Font *font,
                const Uint16 *text, SDL_Color fg, SDL_Color bg);


extern DECLSPEC SDL_Surface * SDLCALL TTF_RenderText_LCD_Wrapped(TTF_Font *font,
                const char *text, SDL_Color fg, SDL_Color bg, Uint32 wrapLength);


extern DECLSPEC SDL_Surface * SDLCALL TTF_RenderUTF8_LCD_Wrapped(TTF_Font *font,
                const char *text, SDL_Color fg, SDL_Color bg, Uint32 wrapLength);


extern DECLSPEC SDL_Surface * SDLCALL TTF_RenderUNICODE_LCD_Wrapped(TTF_Font *font,
                const Uint16 *text, SDL_Color fg, SDL_Color bg, Uint32 wrapLength);


extern DECLSPEC SDL_Surface * SDLCALL TTF_RenderGlyph_LCD(TTF_Font *font,
                Uint16 ch, SDL_Color fg, SDL_Color bg);


extern DECLSPEC SDL_Surface * SDLCALL TTF_RenderGlyph32_LCD(TTF_Font *font,
                Uint32 ch, SDL_Color fg, SDL_Color bg);



#define TTF_RenderText(font, text, fg, bg)  \
    TTF_RenderText_Shaded(font, text, fg, bg)
#define TTF_RenderUTF8(font, text, fg, bg)  \
    TTF_RenderUTF8_Shaded(font, text, fg, bg)
#define TTF_RenderUNICODE(font, text, fg, bg)   \
    TTF_RenderUNICODE_Shaded(font, text, fg, bg)

extern DECLSPEC void SDLCALL TTF_CloseFont(TTF_Font *font);


extern DECLSPEC void SDLCALL TTF_Quit(void);


extern DECLSPEC int SDLCALL TTF_WasInit(void);


extern SDL_DEPRECATED DECLSPEC int TTF_GetFontKerningSize(TTF_Font *font, int prev_index, int index);


extern DECLSPEC int TTF_GetFontKerningSizeGlyphs(TTF_Font *font, Uint16 previous_ch, Uint16 ch);


extern DECLSPEC int TTF_GetFontKerningSizeGlyphs32(TTF_Font *font, Uint32 previous_ch, Uint32 ch);


extern DECLSPEC int TTF_SetFontSDF(TTF_Font *font, SDL_bool on_off);


extern DECLSPEC SDL_bool TTF_GetFontSDF(const TTF_Font *font);


#define TTF_SetError    SDL_SetError


#define TTF_GetError    SDL_GetError


typedef enum
{
  TTF_DIRECTION_LTR = 0,
  TTF_DIRECTION_RTL,
  TTF_DIRECTION_TTB,
  TTF_DIRECTION_BTT
} TTF_Direction;

extern SDL_DEPRECATED DECLSPEC int SDLCALL TTF_SetDirection(int direction);

extern SDL_DEPRECATED DECLSPEC int SDLCALL TTF_SetScript(int script);

extern DECLSPEC int SDLCALL TTF_SetFontDirection(TTF_Font *font, TTF_Direction direction);

extern DECLSPEC int SDLCALL TTF_SetFontScriptName(TTF_Font *font, const char *script);

#ifdef __cplusplus
}
#endif
#include "SDL2/close_code.h"

#endif