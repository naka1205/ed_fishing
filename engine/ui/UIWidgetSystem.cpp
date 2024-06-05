#include "SG_Engine/ui/UIWidgetSystem.h"
#include "SG_Engine/ui/UIWidgetFactory.h"

#include "SG_Engine/SENode.h"

NS_SDL_BEGIN
namespace ui
{
	UIWidgetSystem::UIWidgetSystem()
		: _widgetFactory(nullptr)
	{
	}
	UIWidgetSystem::~UIWidgetSystem()
	{
		SDL_SAFE_RELEASE_NULL(_widgetFactory);
	}
	bool UIWidgetSystem::init()
	{
		_widgetFactory = UIWidgetFactory::create();
		SDL_SAFE_RETAIN(_widgetFactory);

		return true;
	}
	Node *UIWidgetSystem::createWidgetsByData(UIWidgetData *widgetData)
	{
		Node *widget = _widgetFactory->createWidgetByData(widgetData);

		auto cldDataList = widgetData->getCldDataList();
		for (auto cldData : cldDataList)
		{
			Node *child = this->createWidgetsByData(cldData);
			widget->addChild(child);
		}
		return widget;
	}

}
NS_SDL_END
