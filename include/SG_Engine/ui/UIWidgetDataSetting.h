#ifndef __SDL_UIWidgetDataSetting_H__
#define __SDL_UIWidgetDataSetting_H__
#include <string>
#include "SG_Engine/SEObject.h"
#include "SG_Engine/SEPoint.h"
#include "SG_Engine/SESize.h"

NS_SDL_BEGIN
namespace ui
{
class UIWidgetData;
class UIWidgetDataSetting:public Object
{
public:
	CREATE_FUNC(UIWidgetDataSetting);
	bool init();
	//设置基础属性
	void setBaseData(UIWidgetData*widgetData,const std::string&name,const std::string&value);
	//设置额外属性
	void setExteraData(UIWidgetData*widgetData,const std::string&name,const std::string&value);
private:
	Point convertStrToPoint(const std::string& text);
	Size convertStrToSize(const std::string& text);
};
//end namespace ui
}
NS_SDL_END
#endif
