#include "SG_Engine/SEClipNode.h"
#include "SG_Engine/SEDirector.h"
#include "SG_Engine/SERenderer.h"

NS_SDL_BEGIN

ClipNode::ClipNode()
{
}

ClipNode::~ClipNode()
{
}

bool ClipNode::init()
{
	Size visibleSize = Director::getInstance()->getVisibleSize();

	this->setContentSize(visibleSize);

	this->setAnchorPoint(Point(0.5f, 0.5f));

	return true;
}

void ClipNode::visit()
{

	Renderer *renderer = Director::getInstance()->getRenderer();

	_before.init(0.f);
	_before.func = [this]()
	{
		beforeVisit();
	};
	renderer->addCommand(&_before);

	Node::visit();

	_after.init(0.f);
	_after.func = [this]()
	{
		afterVisit();
	};
	renderer->addCommand(&_after);
}

void ClipNode::beforeVisit()
{
	Size size = this->getContentSize();
	auto pos = this->convertToWorldSpace(Point::ZERO);

	SDL_Rect rect = {(int)pos.x, (int)pos.y, (int)size.width, (int)size.height};

	Renderer *renderer = Director::getInstance()->getRenderer();

	renderer->setClipRect(&rect);
}

void ClipNode::afterVisit()
{

	Renderer *renderer = Director::getInstance()->getRenderer();

	renderer->setClipRect(nullptr);
}

NS_SDL_END
