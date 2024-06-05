#include "SG_Engine/SEActionManager.h"
#include "SG_Engine/SENode.h"
#include "SG_Engine/SEAction.h"

NS_SDL_BEGIN
typedef struct _hashElement
{

	Vector<Action *> actions;

	Node *target;

	unsigned int actionIndex;

	Action *currentAction;

	bool currentActionSalvaged;

	bool paused;

public:
	_hashElement() : target(nullptr), actionIndex(-1), currentAction(nullptr), currentActionSalvaged(false), paused(true)
	{
	}
} tHashElement;
ActionManager::ActionManager() : _target(nullptr),
								 _currentTarget(nullptr), _currentTargetSalvaged(false)
{
}
ActionManager::~ActionManager()
{
	removeAllActions();
	//_tHashElements.clear();
}

void ActionManager::deleteHashElement(tHashElement *element)
{

	auto iter = std::find(_tHashElements.begin(), _tHashElements.end(), element);
	if (iter != _tHashElements.end())
	{
		_tHashElements.erase(iter);

		element->actions.clear();

		element->target->release();

		delete element;
	}
}
void ActionManager::deleteHashElementForTarget(Node *target)
{
	auto iter = std::find_if(_tHashElements.begin(), _tHashElements.end(), [target](tHashElement *ele)
							 { return target == ele->target; });
	if (iter != _tHashElements.end())
	{
		tHashElement *element = *iter;
		deleteHashElement(element);
	}
}
void ActionManager::pauseTarget(Node *target)
{
	auto iter = std::find_if(_tHashElements.begin(), _tHashElements.end(), [target](tHashElement *ele)
							 { return ele->target == target; });
	if (iter != _tHashElements.end())
	{
		auto element = *iter;
		element->paused = true;
	}
}
void ActionManager::resumeTarget(Node *target)
{
	auto iter = std::find_if(_tHashElements.begin(), _tHashElements.end(), [target](tHashElement *ele)
							 { return ele->target == target; });
	if (iter != _tHashElements.end())
	{
		auto element = *iter;
		element->paused = false;
	}
}
void ActionManager::removeAllActionsFromTarget(Node *target)
{

	auto iter = std::find_if(_tHashElements.begin(), _tHashElements.end(), [target](tHashElement *ele)
							 { return ele->target == target; });

	if (iter != _tHashElements.end())
	{
		tHashElement *element = *iter;

		if (element == _currentTarget)
		{
			_currentTargetSalvaged = true;
		}
		else
		{
			deleteHashElement(element);
		}
	}
}
void ActionManager::removeAllActions()
{
	for (unsigned int i = 0; i < _tHashElements.size(); i++)
	{
		auto element = _tHashElements.at(i);
		removeAllActionsFromTarget(element->target);
	}
}
void ActionManager::removeAction(Action *action)
{

	if (action == nullptr)
		return;

	Node *originalTarget = action->getOriginalTarget();

	tHashElement *element = nullptr;
	auto iter = std::find_if(_tHashElements.begin(), _tHashElements.end(), [originalTarget](tHashElement *e)
							 { return e->target == originalTarget; });

	if (iter != _tHashElements.end())
	{
		element = *iter;

		action->stop();
		element->actions.eraseObject(action);
	}
}
Action *ActionManager::getActionByTag(int tag, const Node *target) const
{
	if (target)
	{

		auto iter = std::find_if(_tHashElements.begin(), _tHashElements.end(), [&](tHashElement *element)
								 { return element->target == target; });

		if (iter != std::end(_tHashElements))
		{

			auto &actions = (*iter)->actions;
			auto actionIter = std::find_if(actions.begin(), actions.end(), [&](Action *action)
										   { return action->getTag() == tag; });
			if (actionIter != (*iter)->actions.end())
			{
				return *actionIter;
			}
		}
	}
	return nullptr;
}
void ActionManager::removeActionByTag(int tag, const Node *target)
{
	if (target)
	{

		auto iter = std::find_if(_tHashElements.begin(), _tHashElements.end(), [&](tHashElement *element)
								 { return element->target == target; });
		//
		if (iter != std::end(_tHashElements))
		{
			auto &actions = (*iter)->actions;
			auto actionIter = std::find_if(actions.begin(), actions.end(), [&](Action *action)
										   { return action->getTag() == tag; });
			if (actionIter != actions.end())
			{
				auto action = *actionIter;
				action->stop();
				actions.erase(actionIter);
			}
		}
	}
}
void ActionManager::addAction(Action *action, Node *target, bool paused)
{
	SDLASSERT(action != NULL, "");
	SDLASSERT(target != NULL, "");
	tHashElement *elem = nullptr;

	auto iter = std::find_if(_tHashElements.begin(), _tHashElements.end(), [target](tHashElement *ele) -> bool
							 { return target == ele->target; });

	if (iter == _tHashElements.end())
	{
		elem = new tHashElement();
		elem->paused = paused;

		elem->target = target;
		_tHashElements.push_back(elem);

		target->retain();
	}
	else
		elem = *iter;

	elem->actions.push_back(action);

	action->startWithTarget(target);
}
void ActionManager::update(float dt)
{

	for (unsigned int i = 0; i < _tHashElements.size(); i++)
	{
		_currentTarget = _tHashElements.at(i);
		_currentTargetSalvaged = false;

		if (!_currentTarget->paused)
		{

			for (_currentTarget->actionIndex = 0; _currentTarget->actionIndex < _currentTarget->actions.size(); ++_currentTarget->actionIndex)
			{

				_currentTarget->currentAction = _currentTarget->actions.at(_currentTarget->actionIndex);

				if (_currentTarget->currentAction == nullptr)
					continue;

				_currentTarget->currentActionSalvaged = false;

				_currentTarget->currentAction->step(dt);

				if (_currentTarget->currentActionSalvaged)
				{
					_currentTarget->currentAction->release();
				}
				else if (_currentTarget->currentAction->isDone())
				{
					_currentTarget->currentAction->stop();

					auto otherAction = _currentTarget->currentAction;
					_currentTarget->currentAction = nullptr;

					removeAction(otherAction);
				}
				_currentTarget->currentAction = nullptr;
			}
		}

		if (_currentTargetSalvaged)
		{

			deleteHashElement(_currentTarget);
		}
	}

	_currentTarget = nullptr;
}
NS_SDL_END
