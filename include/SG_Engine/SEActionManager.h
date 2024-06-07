#ifndef __SDL_ActionManager_H__
#define __SDL_ActionManager_H__

#include <algorithm>
#include <vector>
#include "SG_Engine/SEObject.h"
#include "SG_Engine/SEVector.h"
NS_SDL_BEGIN
class Action;
class Node;
struct _hashElement;

class ActionManager : public Object
{
private:
	std::vector<_hashElement *> _tHashElements;
	_hashElement *_target;

	_hashElement *_currentTarget;

	bool _currentTargetSalvaged;

public:
	ActionManager();
	virtual ~ActionManager();

	void deleteHashElement(_hashElement *element);
	
	void deleteHashElementForTarget(Node *target);

	void pauseTarget(Node *target);

	void resumeTarget(Node *target);

	void removeAllActionsFromTarget(Node *target);

	void removeAllActions();

	void removeAction(Action *action);

	Action *getActionByTag(int tag, const Node *target) const;

	void removeActionByTag(int tag, const Node *target);

	void addAction(Action *action, Node *node, bool paused);

	void update(float dt);
};
NS_SDL_END
#endif
