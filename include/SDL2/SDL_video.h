#ifndef SDL_video_h_
#define SDL_video_h_

#include "SDL_stdinc.h"
#include "SDL_pixels.h"
#include "SDL_rect.h"
#include "SDL_surface.h"

#include "begin_code.h"

#ifdef __cplusplus
extern "C"
{
#endif


    typedef struct
    {
        Uint32 format;    /**< pixel format */
        int w;            /**< width, in screen coordinates */
        int h;            /**< height, in screen coordinates */
        int refresh_rate; /**< refresh rate (or zero for unspecified) */
        void *driverdata; /**< driver-specific data, initialize to 0 */
    } SDL_DisplayMode;

    typedef struct SDL_Window SDL_Window;

    typedef enum
    {
        SDL_WINDOW_FULLSCREEN = 0x00000001,    /**< fullscreen window */
        SDL_WINDOW_OPENGL = 0x00000002,        /**< window usable with OpenGL context */
        SDL_WINDOW_SHOWN = 0x00000004,         /**< window is visible */
        SDL_WINDOW_HIDDEN = 0x00000008,        /**< window is not visible */
        SDL_WINDOW_BORDERLESS = 0x00000010,    /**< no window decoration */
        SDL_WINDOW_RESIZABLE = 0x00000020,     /**< window can be resized */
        SDL_WINDOW_MINIMIZED = 0x00000040,     /**< window is minimized */
        SDL_WINDOW_MAXIMIZED = 0x00000080,     /**< window is maximized */
        SDL_WINDOW_MOUSE_GRABBED = 0x00000100, /**< window has grabbed mouse input */
        SDL_WINDOW_INPUT_FOCUS = 0x00000200,   /**< window has input focus */
        SDL_WINDOW_MOUSE_FOCUS = 0x00000400,   /**< window has mouse focus */
        SDL_WINDOW_FULLSCREEN_DESKTOP = (SDL_WINDOW_FULLSCREEN | 0x00001000),
        SDL_WINDOW_FOREIGN = 0x00000800,          /**< window not created by SDL */
        SDL_WINDOW_ALLOW_HIGHDPI = 0x00002000,    /**< window should be created in high-DPI mode if supported.
                                                       On macOS NSHighResolutionCapable must be set true in the
                                                       application's Info.plist for this to have any effect. */
        SDL_WINDOW_MOUSE_CAPTURE = 0x00004000,    /**< window has mouse captured (unrelated to MOUSE_GRABBED) */
        SDL_WINDOW_ALWAYS_ON_TOP = 0x00008000,    /**< window should always be above others */
        SDL_WINDOW_SKIP_TASKBAR = 0x00010000,     /**< window should not be added to the taskbar */
        SDL_WINDOW_UTILITY = 0x00020000,          /**< window should be treated as a utility window */
        SDL_WINDOW_TOOLTIP = 0x00040000,          /**< window should be treated as a tooltip */
        SDL_WINDOW_POPUP_MENU = 0x00080000,       /**< window should be treated as a popup menu */
        SDL_WINDOW_KEYBOARD_GRABBED = 0x00100000, /**< window has grabbed keyboard input */
        SDL_WINDOW_VULKAN = 0x10000000,           /**< window usable for Vulkan surface */
        SDL_WINDOW_METAL = 0x20000000,            /**< window usable for Metal view */

        SDL_WINDOW_INPUT_GRABBED = SDL_WINDOW_MOUSE_GRABBED /**< equivalent to SDL_WINDOW_MOUSE_GRABBED for compatibility */
    } SDL_WindowFlags;

#define SDL_WINDOWPOS_UNDEFINED_MASK 0x1FFF0000u
#define SDL_WINDOWPOS_UNDEFINED_DISPLAY(X) (SDL_WINDOWPOS_UNDEFINED_MASK | (X))
#define SDL_WINDOWPOS_UNDEFINED SDL_WINDOWPOS_UNDEFINED_DISPLAY(0)
#define SDL_WINDOWPOS_ISUNDEFINED(X) \
    (((X) & 0xFFFF0000) == SDL_WINDOWPOS_UNDEFINED_MASK)

#define SDL_WINDOWPOS_CENTERED_MASK 0x2FFF0000u
#define SDL_WINDOWPOS_CENTERED_DISPLAY(X) (SDL_WINDOWPOS_CENTERED_MASK | (X))
#define SDL_WINDOWPOS_CENTERED SDL_WINDOWPOS_CENTERED_DISPLAY(0)
#define SDL_WINDOWPOS_ISCENTERED(X) \
    (((X) & 0xFFFF0000) == SDL_WINDOWPOS_CENTERED_MASK)

    typedef enum
    {
        SDL_WINDOWEVENT_NONE,
        SDL_WINDOWEVENT_SHOWN,
        SDL_WINDOWEVENT_HIDDEN,
        SDL_WINDOWEVENT_EXPOSED,

        SDL_WINDOWEVENT_MOVED,

        SDL_WINDOWEVENT_RESIZED,
        SDL_WINDOWEVENT_SIZE_CHANGED,

        SDL_WINDOWEVENT_MINIMIZED,
        SDL_WINDOWEVENT_MAXIMIZED,
        SDL_WINDOWEVENT_RESTORED,

        SDL_WINDOWEVENT_ENTER,
        SDL_WINDOWEVENT_LEAVE,
        SDL_WINDOWEVENT_FOCUS_GAINED,
        SDL_WINDOWEVENT_FOCUS_LOST,
        SDL_WINDOWEVENT_CLOSE,
        SDL_WINDOWEVENT_TAKE_FOCUS,
        SDL_WINDOWEVENT_HIT_TEST,
        SDL_WINDOWEVENT_ICCPROF_CHANGED,
        SDL_WINDOWEVENT_DISPLAY_CHANGED
    } SDL_WindowEventID;

    typedef enum
    {
        SDL_DISPLAYEVENT_NONE,
        SDL_DISPLAYEVENT_ORIENTATION,
        SDL_DISPLAYEVENT_CONNECTED,
        SDL_DISPLAYEVENT_DISCONNECTED
    } SDL_DisplayEventID;

    typedef enum
    {
        SDL_ORIENTATION_UNKNOWN,
        SDL_ORIENTATION_LANDSCAPE,
        SDL_ORIENTATION_LANDSCAPE_FLIPPED,
        SDL_ORIENTATION_PORTRAIT,
        SDL_ORIENTATION_PORTRAIT_FLIPPED
    } SDL_DisplayOrientation;

    typedef enum
    {
        SDL_FLASH_CANCEL,
        SDL_FLASH_BRIEFLY,
        SDL_FLASH_UNTIL_FOCUSED
    } SDL_FlashOperation;

    typedef void *SDL_GLContext;

    typedef enum
    {
        SDL_GL_RED_SIZE,
        SDL_GL_GREEN_SIZE,
        SDL_GL_BLUE_SIZE,
        SDL_GL_ALPHA_SIZE,
        SDL_GL_BUFFER_SIZE,
        SDL_GL_DOUBLEBUFFER,
        SDL_GL_DEPTH_SIZE,
        SDL_GL_STENCIL_SIZE,
        SDL_GL_ACCUM_RED_SIZE,
        SDL_GL_ACCUM_GREEN_SIZE,
        SDL_GL_ACCUM_BLUE_SIZE,
        SDL_GL_ACCUM_ALPHA_SIZE,
        SDL_GL_STEREO,
        SDL_GL_MULTISAMPLEBUFFERS,
        SDL_GL_MULTISAMPLESAMPLES,
        SDL_GL_ACCELERATED_VISUAL,
        SDL_GL_RETAINED_BACKING,
        SDL_GL_CONTEXT_MAJOR_VERSION,
        SDL_GL_CONTEXT_MINOR_VERSION,
        SDL_GL_CONTEXT_EGL,
        SDL_GL_CONTEXT_FLAGS,
        SDL_GL_CONTEXT_PROFILE_MASK,
        SDL_GL_SHARE_WITH_CURRENT_CONTEXT,
        SDL_GL_FRAMEBUFFER_SRGB_CAPABLE,
        SDL_GL_CONTEXT_RELEASE_BEHAVIOR,
        SDL_GL_CONTEXT_RESET_NOTIFICATION,
        SDL_GL_CONTEXT_NO_ERROR,
        SDL_GL_FLOATBUFFERS
    } SDL_GLattr;

    typedef enum
    {
        SDL_GL_CONTEXT_PROFILE_CORE = 0x0001,
        SDL_GL_CONTEXT_PROFILE_COMPATIBILITY = 0x0002,
        SDL_GL_CONTEXT_PROFILE_ES = 0x0004
    } SDL_GLprofile;

    typedef enum
    {
        SDL_GL_CONTEXT_DEBUG_FLAG = 0x0001,
        SDL_GL_CONTEXT_FORWARD_COMPATIBLE_FLAG = 0x0002,
        SDL_GL_CONTEXT_ROBUST_ACCESS_FLAG = 0x0004,
        SDL_GL_CONTEXT_RESET_ISOLATION_FLAG = 0x0008
    } SDL_GLcontextFlag;

    typedef enum
    {
        SDL_GL_CONTEXT_RELEASE_BEHAVIOR_NONE = 0x0000,
        SDL_GL_CONTEXT_RELEASE_BEHAVIOR_FLUSH = 0x0001
    } SDL_GLcontextReleaseFlag;

    typedef enum
    {
        SDL_GL_CONTEXT_RESET_NO_NOTIFICATION = 0x0000,
        SDL_GL_CONTEXT_RESET_LOSE_CONTEXT = 0x0001
    } SDL_GLContextResetNotification;

    extern DECLSPEC int SDLCALL SDL_GetNumVideoDrivers(void);

    extern DECLSPEC const char *SDLCALL SDL_GetVideoDriver(int index);

    extern DECLSPEC int SDLCALL SDL_VideoInit(const char *driver_name);

    extern DECLSPEC void SDLCALL SDL_VideoQuit(void);

    extern DECLSPEC const char *SDLCALL SDL_GetCurrentVideoDriver(void);

    extern DECLSPEC int SDLCALL SDL_GetNumVideoDisplays(void);

    extern DECLSPEC const char *SDLCALL SDL_GetDisplayName(int displayIndex);

    extern DECLSPEC int SDLCALL SDL_GetDisplayBounds(int displayIndex, SDL_Rect *rect);

    extern DECLSPEC int SDLCALL SDL_GetDisplayUsableBounds(int displayIndex, SDL_Rect *rect);

    extern DECLSPEC int SDLCALL SDL_GetDisplayDPI(int displayIndex, float *ddpi, float *hdpi, float *vdpi);

    extern DECLSPEC SDL_DisplayOrientation SDLCALL SDL_GetDisplayOrientation(int displayIndex);

    extern DECLSPEC int SDLCALL SDL_GetNumDisplayModes(int displayIndex);

    extern DECLSPEC int SDLCALL SDL_GetDisplayMode(int displayIndex, int modeIndex,
                                                   SDL_DisplayMode *mode);

    extern DECLSPEC int SDLCALL SDL_GetDesktopDisplayMode(int displayIndex, SDL_DisplayMode *mode);

    extern DECLSPEC int SDLCALL SDL_GetCurrentDisplayMode(int displayIndex, SDL_DisplayMode *mode);

    extern DECLSPEC SDL_DisplayMode *SDLCALL SDL_GetClosestDisplayMode(int displayIndex, const SDL_DisplayMode *mode, SDL_DisplayMode *closest);

    extern DECLSPEC int SDLCALL SDL_GetPointDisplayIndex(const SDL_Point *point);

    extern DECLSPEC int SDLCALL SDL_GetRectDisplayIndex(const SDL_Rect *rect);

    extern DECLSPEC int SDLCALL SDL_GetWindowDisplayIndex(SDL_Window *window);

    extern DECLSPEC int SDLCALL SDL_SetWindowDisplayMode(SDL_Window *window,
                                                         const SDL_DisplayMode *mode);

    extern DECLSPEC int SDLCALL SDL_GetWindowDisplayMode(SDL_Window *window,
                                                         SDL_DisplayMode *mode);

    extern DECLSPEC void *SDLCALL SDL_GetWindowICCProfile(SDL_Window *window, size_t *size);

    extern DECLSPEC Uint32 SDLCALL SDL_GetWindowPixelFormat(SDL_Window *window);

    extern DECLSPEC SDL_Window *SDLCALL SDL_CreateWindow(const char *title,
                                                         int x, int y, int w,
                                                         int h, Uint32 flags);

    extern DECLSPEC SDL_Window *SDLCALL SDL_CreateWindowFrom(const void *data);

    extern DECLSPEC Uint32 SDLCALL SDL_GetWindowID(SDL_Window *window);

    extern DECLSPEC SDL_Window *SDLCALL SDL_GetWindowFromID(Uint32 id);

    extern DECLSPEC Uint32 SDLCALL SDL_GetWindowFlags(SDL_Window *window);

    extern DECLSPEC void SDLCALL SDL_SetWindowTitle(SDL_Window *window,
                                                    const char *title);

    extern DECLSPEC const char *SDLCALL SDL_GetWindowTitle(SDL_Window *window);

    extern DECLSPEC void SDLCALL SDL_SetWindowIcon(SDL_Window *window,
                                                   SDL_Surface *icon);

    extern DECLSPEC void *SDLCALL SDL_SetWindowData(SDL_Window *window,
                                                    const char *name,
                                                    void *userdata);

    extern DECLSPEC void *SDLCALL SDL_GetWindowData(SDL_Window *window,
                                                    const char *name);

    extern DECLSPEC void SDLCALL SDL_SetWindowPosition(SDL_Window *window,
                                                       int x, int y);

    extern DECLSPEC void SDLCALL SDL_GetWindowPosition(SDL_Window *window,
                                                       int *x, int *y);

    extern DECLSPEC void SDLCALL SDL_SetWindowSize(SDL_Window *window, int w,
                                                   int h);

    extern DECLSPEC void SDLCALL SDL_GetWindowSize(SDL_Window *window, int *w,
                                                   int *h);

    extern DECLSPEC int SDLCALL SDL_GetWindowBordersSize(SDL_Window *window,
                                                         int *top, int *left,
                                                         int *bottom, int *right);

    extern DECLSPEC void SDLCALL SDL_GetWindowSizeInPixels(SDL_Window *window,
                                                           int *w, int *h);

    extern DECLSPEC void SDLCALL SDL_SetWindowMinimumSize(SDL_Window *window,
                                                          int min_w, int min_h);

    extern DECLSPEC void SDLCALL SDL_GetWindowMinimumSize(SDL_Window *window,
                                                          int *w, int *h);

    extern DECLSPEC void SDLCALL SDL_SetWindowMaximumSize(SDL_Window *window,
                                                          int max_w, int max_h);

    extern DECLSPEC void SDLCALL SDL_GetWindowMaximumSize(SDL_Window *window,
                                                          int *w, int *h);

    extern DECLSPEC void SDLCALL SDL_SetWindowBordered(SDL_Window *window,
                                                       SDL_bool bordered);

    extern DECLSPEC void SDLCALL SDL_SetWindowResizable(SDL_Window *window,
                                                        SDL_bool resizable);

    extern DECLSPEC void SDLCALL SDL_SetWindowAlwaysOnTop(SDL_Window *window,
                                                          SDL_bool on_top);

    extern DECLSPEC void SDLCALL SDL_ShowWindow(SDL_Window *window);

    extern DECLSPEC void SDLCALL SDL_HideWindow(SDL_Window *window);

    extern DECLSPEC void SDLCALL SDL_RaiseWindow(SDL_Window *window);

    extern DECLSPEC void SDLCALL SDL_MaximizeWindow(SDL_Window *window);

    extern DECLSPEC void SDLCALL SDL_MinimizeWindow(SDL_Window *window);

    extern DECLSPEC void SDLCALL SDL_RestoreWindow(SDL_Window *window);

    extern DECLSPEC int SDLCALL SDL_SetWindowFullscreen(SDL_Window *window,
                                                        Uint32 flags);

    extern DECLSPEC SDL_Surface *SDLCALL SDL_GetWindowSurface(SDL_Window *window);

    extern DECLSPEC int SDLCALL SDL_UpdateWindowSurface(SDL_Window *window);

    extern DECLSPEC int SDLCALL SDL_UpdateWindowSurfaceRects(SDL_Window *window,
                                                             const SDL_Rect *rects,
                                                             int numrects);

    extern DECLSPEC void SDLCALL SDL_SetWindowGrab(SDL_Window *window,
                                                   SDL_bool grabbed);

    extern DECLSPEC void SDLCALL SDL_SetWindowKeyboardGrab(SDL_Window *window,
                                                           SDL_bool grabbed);

    extern DECLSPEC void SDLCALL SDL_SetWindowMouseGrab(SDL_Window *window,
                                                        SDL_bool grabbed);

    extern DECLSPEC SDL_bool SDLCALL SDL_GetWindowGrab(SDL_Window *window);

    extern DECLSPEC SDL_bool SDLCALL SDL_GetWindowKeyboardGrab(SDL_Window *window);

    extern DECLSPEC SDL_bool SDLCALL SDL_GetWindowMouseGrab(SDL_Window *window);

    extern DECLSPEC SDL_Window *SDLCALL SDL_GetGrabbedWindow(void);

    extern DECLSPEC int SDLCALL SDL_SetWindowMouseRect(SDL_Window *window, const SDL_Rect *rect);

    extern DECLSPEC const SDL_Rect *SDLCALL SDL_GetWindowMouseRect(SDL_Window *window);

    extern DECLSPEC int SDLCALL SDL_SetWindowBrightness(SDL_Window *window, float brightness);

    extern DECLSPEC float SDLCALL SDL_GetWindowBrightness(SDL_Window *window);

    extern DECLSPEC int SDLCALL SDL_SetWindowOpacity(SDL_Window *window, float opacity);

    extern DECLSPEC int SDLCALL SDL_GetWindowOpacity(SDL_Window *window, float *out_opacity);

    extern DECLSPEC int SDLCALL SDL_SetWindowModalFor(SDL_Window *modal_window, SDL_Window *parent_window);

    extern DECLSPEC int SDLCALL SDL_SetWindowInputFocus(SDL_Window *window);

    extern DECLSPEC int SDLCALL SDL_SetWindowGammaRamp(SDL_Window *window,
                                                       const Uint16 *red,
                                                       const Uint16 *green,
                                                       const Uint16 *blue);

    extern DECLSPEC int SDLCALL SDL_GetWindowGammaRamp(SDL_Window *window,
                                                       Uint16 *red,
                                                       Uint16 *green,
                                                       Uint16 *blue);

    typedef enum
    {
        SDL_HITTEST_NORMAL,
        SDL_HITTEST_DRAGGABLE,
        SDL_HITTEST_RESIZE_TOPLEFT,
        SDL_HITTEST_RESIZE_TOP,
        SDL_HITTEST_RESIZE_TOPRIGHT,
        SDL_HITTEST_RESIZE_RIGHT,
        SDL_HITTEST_RESIZE_BOTTOMRIGHT,
        SDL_HITTEST_RESIZE_BOTTOM,
        SDL_HITTEST_RESIZE_BOTTOMLEFT,
        SDL_HITTEST_RESIZE_LEFT
    } SDL_HitTestResult;

    typedef SDL_HitTestResult(SDLCALL *SDL_HitTest)(SDL_Window *win,
                                                    const SDL_Point *area,
                                                    void *data);

    extern DECLSPEC int SDLCALL SDL_SetWindowHitTest(SDL_Window *window,
                                                     SDL_HitTest callback,
                                                     void *callback_data);

    extern DECLSPEC int SDLCALL SDL_FlashWindow(SDL_Window *window, SDL_FlashOperation operation);

    extern DECLSPEC void SDLCALL SDL_DestroyWindow(SDL_Window *window);

    extern DECLSPEC SDL_bool SDLCALL SDL_IsScreenSaverEnabled(void);

    extern DECLSPEC void SDLCALL SDL_EnableScreenSaver(void);

    extern DECLSPEC void SDLCALL SDL_DisableScreenSaver(void);

    extern DECLSPEC int SDLCALL SDL_GL_LoadLibrary(const char *path);

    extern DECLSPEC void *SDLCALL SDL_GL_GetProcAddress(const char *proc);

    extern DECLSPEC void SDLCALL SDL_GL_UnloadLibrary(void);

    extern DECLSPEC SDL_bool SDLCALL SDL_GL_ExtensionSupported(const char
                                                                   *extension);

    extern DECLSPEC void SDLCALL SDL_GL_ResetAttributes(void);

    extern DECLSPEC int SDLCALL SDL_GL_SetAttribute(SDL_GLattr attr, int value);

    extern DECLSPEC int SDLCALL SDL_GL_GetAttribute(SDL_GLattr attr, int *value);

    extern DECLSPEC SDL_GLContext SDLCALL SDL_GL_CreateContext(SDL_Window *
                                                                   window);

    extern DECLSPEC int SDLCALL SDL_GL_MakeCurrent(SDL_Window *window,
                                                   SDL_GLContext context);

    extern DECLSPEC SDL_Window *SDLCALL SDL_GL_GetCurrentWindow(void);

    extern DECLSPEC SDL_GLContext SDLCALL SDL_GL_GetCurrentContext(void);

    extern DECLSPEC void SDLCALL SDL_GL_GetDrawableSize(SDL_Window *window, int *w,
                                                        int *h);

    extern DECLSPEC int SDLCALL SDL_GL_SetSwapInterval(int interval);

    extern DECLSPEC int SDLCALL SDL_GL_GetSwapInterval(void);

    extern DECLSPEC void SDLCALL SDL_GL_SwapWindow(SDL_Window *window);

    extern DECLSPEC void SDLCALL SDL_GL_DeleteContext(SDL_GLContext context);

#ifdef __cplusplus
}
#endif
#include "close_code.h"

#endif
