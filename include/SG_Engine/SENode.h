#ifndef __SDL_Node_H__
#define __SDL_Node_H__
#include <vector>
#include <string>
#include <assert.h>
#include <algorithm>

#include "SG_Engine/SEObject.h"
#include "SG_Engine/SEPoint.h"
#include "SG_Engine/SESize.h"
#include "SG_Engine/SERect.h"
#include "SG_Engine/SEMath.h"
#include "SG_Engine/SEColor.h"
#include "SG_Engine/SEScheduler.h"

NS_SDL_BEGIN

class Renderer;
class EventDispatcher;
class ActionManager;
class Action;

class Node : public Object
{
protected:
	//子孩子
	std::vector<Node*> _children;
	//the node's name,you can find it by the name
	std::string _name;
	//the tag，如果没赋值，值为-1
	int _tag;
	//the parent
	Node*_parent;
	//is visible
	bool _visible;

	int _localZOrder;
	//global z order
	float _globalZOrder;
	//judge the child is sorted
	bool _reorderChildDirty;
	//check the node whether run
	bool _running;

	Point _position;

	Size _contentSize;
	

	float _rotation;

	float _scaleX;
	float _scaleY;

	Point _anchorPoint;
	bool _flipX;
	bool _flipY;

	Uint8 _realOpacity;
	Uint8 _displayedOpacity;
	bool _cascadeOpacity;

	Color3B _colorMod;

	bool _cascadeScale;

	void* _userdata;
	Object* _userObject;

	EventDispatcher* _eventDispatcher;
	ActionManager* _actionManager;
	Scheduler* _scheduler;
protected:
	Node();

	friend class Director;
public:
	virtual ~Node();
	CREATE_FUNC(Node);
	bool init();
	virtual void update(float dt);

	Node* getParent() const { return _parent; }
	void setParent(Node* parent);

	void addChild(Node* child);

	void addChild(Node* child,int localZOrder);
	void addChild(Node* child,int localZOrder,int tag);
	void removeChild(Node* child,bool cleanup = false);

	void removeFromParent();

	void removeAllChildren();

	void cleanup();

	virtual void onEnter();

	virtual void onExit();

	void resume();
	void pause();

	void pauseSchedulerAndActions();

	void resumeSchedlerAndActions();
	virtual void visit();

	virtual void draw();

	bool isVisible();
	void setVisible(bool isVisible);

	void setPositionX(float x);
	void setPositionY(float y);
	void setPosition(const Point& pos);
	void setPosition(float x,float y);

	Point getPosition() const;
	float getPositionX()const;
	float getPositionY()const;

	Point getWorldPosition()const;

	Point convertToNodeSpace(const Point& worldPoint)const;

	Point convertToNodeSpaceAR(const Point& worldPoint)const;
	Point convertToWorldSpace(const Point& nodePoint)const;
	Point convertToWorldSpaceAR(const Point& nodePoint);

	Size getContentSize() const;
	void setContentSize(const Size& size);

	Rect getBoundingBox() const;
	float getRotation();

	void setRotation(float rotation);

	float getScaleX();
	void setScaleX(float scaleX);
	float getScaleY();
	void setScaleY(float scaleY);
	void setScale(float scaleX,float scaleY);
	Point getScale()const;
	Point getWorldScale()const;

	Point getAnchorPoint()const;
	void setAnchorPoint(const Point& anchorPoint);

	void setName(std::string name);
	std::string getName() const;
	void setTag(int tag);
	int getTag();

	Node* getChildByName(std::string name);
	Node* getChildByTag(int tag);

	template<typename T>
	T getChildByName(std::string name) { return dynamic_cast<T>(getChildByName(name)); }
	template<typename T>
	T getChildByTag(int tag) { return dynamic_cast<T>(getChildByTag(tag)); }

	const std::vector<Node*>& getChildren() { return _children; }

	void sortAllChildren();
	static bool nodeComparisonLess(Node* n1, Node* n2);

	int getLocalZOrder() const;
	void setLocalZOrder(int localZOrder);

	float getGlobalZOrder() const; 
	void setGlobalZOrder(float globalZOrder);

	virtual bool isRunning() const{return _running;}

	bool isFlipX();
	bool isFlipY();
	void setFlipX(bool flipX);
	void setFlipY(bool flipY);

	void setOpacity(Uint8 opacity);
	Uint8 getOpacity() const;
	Uint8 getDisplayedOpacity() const;

	void updateCascadeOpacity();
	void disableCascadeOpacity();
	void updateDisplayedOpacity(Uint8 parentOpacity);

	void setCascadeOpacityEnabled(bool enabled);
	bool isCascadeOpacityEnabled() const;

	void setColorMod(const Color3B&color);
	void setColorMod(Uint8 r,Uint8 g,Uint8 b);
	Color3B getColorMod()const;

	void setCascadeScaleEnabled(bool enabled);
	bool isCascadeScaleEnabled()const;

	void setUserData(void*userdata);
	void* getUserData()const;

	void setUserObject(Object*sender);
	Object* getUserObject()const;

	void runAction(Action*action);
	void stopAllActions();
	Action*getActionByTag(int tag);
	void stopActionByTag(int tag);
	void stopAction(Action*action);

	void scheduleUpdate();
	void unscheduleUpdate();

	void schedule(const ccScheduleCallback&callback,const std::string& key,float interval=0.f,unsigned int repeat=SDL_REPEAT_FOREVER,float delay=0.f);
	void schedule(SEL_SCHEDULE selector,float interval,unsigned int repeat,float delay=0.f);
	
	void unschedule(const std::string& key);
	void unschedule(SEL_SCHEDULE selector);

	void scheduleOnce(const ccScheduleCallback& callback,const std::string&key,float delay=0.f);
	void scheduleOnce(SEL_SCHEDULE selector,float delay = 0.f);
	void unscheduleAllCallbacks();

	void removeAllListeners();
};
NS_SDL_END
#endif
