#ifndef __SDL_ClipNode_H__
#define __SDL_ClipNode_H__
#include "SDL2/SDL.h"

#include "SG_Engine/SENode.h"
#include "SG_Engine/SERenderCommand.h"
NS_SDL_BEGIN

class ClipNode : public Node
{
protected:

	CustomCommand _before;
	CustomCommand _after;

public:
    ClipNode();
    virtual ~ClipNode();

    CREATE_FUNC(ClipNode);
    bool init();

    virtual void visit();
    virtual void beforeVisit();
    virtual void afterVisit();
};
NS_SDL_END
#endif
