#ifndef __SDL_UIWidgetManager_H__
#define __SDL_UIWidgetManager_H__
#include <string>
#include <memory>
#include "SG_Engine/SEObject.h"
#include "rapidxml/rapidxml.hpp"

NS_SDL_BEGIN
class Node;
namespace ui
{
	class UIWidgetData;
	class UIWidgetDataSetting;
	class UIWidgetSystem;

	class UIWidgetManager : public Object
	{
	private:
		UIWidgetDataSetting *_widgetDataSetting;
		UIWidgetSystem *_widgetSystem;

		static UIWidgetManager *_pInstance;

	private:
		UIWidgetManager();
		~UIWidgetManager();

	public:
		static UIWidgetManager *getInstance()
		{
			if (_pInstance == nullptr)
			{
				_pInstance = new UIWidgetManager();
			}
			return _pInstance;
		}
		static void purge();

		Node *createWidgetsWithXml(const std::string &xmlPath);

	private:
		UIWidgetData *createWidgetData(rapidxml::xml_node<> *node);

		void setWidgetBaseData(rapidxml::xml_node<> *node, UIWidgetData *widgetData);
		void setWidgetProperties(rapidxml::xml_node<> *properties, UIWidgetData *widgetData);
	};

}
NS_SDL_END
#endif
