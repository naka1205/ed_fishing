#ifndef SDL_touch_h_
#define SDL_touch_h_

#include "SDL_stdinc.h"
#include "SDL_error.h"
#include "SDL_video.h"

#include "begin_code.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef Sint64 SDL_TouchID;
typedef Sint64 SDL_FingerID;

typedef enum
{
    SDL_TOUCH_DEVICE_INVALID = -1,
    SDL_TOUCH_DEVICE_DIRECT,            
    SDL_TOUCH_DEVICE_INDIRECT_ABSOLUTE, 
    SDL_TOUCH_DEVICE_INDIRECT_RELATIVE
} SDL_TouchDeviceType;

typedef struct SDL_Finger
{
    SDL_FingerID id;
    float x;
    float y;
    float pressure;
} SDL_Finger;


#define SDL_TOUCH_MOUSEID ((Uint32)-1)


#define SDL_MOUSE_TOUCHID ((Sint64)-1)


extern DECLSPEC int SDLCALL SDL_GetNumTouchDevices(void);


extern DECLSPEC SDL_TouchID SDLCALL SDL_GetTouchDevice(int index);


extern DECLSPEC const char* SDLCALL SDL_GetTouchName(int index);


extern DECLSPEC SDL_TouchDeviceType SDLCALL SDL_GetTouchDeviceType(SDL_TouchID touchID);


extern DECLSPEC int SDLCALL SDL_GetNumTouchFingers(SDL_TouchID touchID);


extern DECLSPEC SDL_Finger * SDLCALL SDL_GetTouchFinger(SDL_TouchID touchID, int index);


#ifdef __cplusplus
}
#endif
#include "close_code.h"

#endif


