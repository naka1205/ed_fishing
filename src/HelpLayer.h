#ifndef __HelpLayer_H__
#define __HelpLayer_H__
#include "SG_Engine.h"
using namespace SDL;

class HelpLayerDelegate
{
public:
	virtual ~HelpLayerDelegate(){}
	virtual void closeHelpLayer()=0;
};
class HelpLayer:public Layer
{
private:
	ui::Button*m_pCloseBtn;
	HelpLayerDelegate*m_pDelegate;
public:
	HelpLayer();
	~HelpLayer();
	CREATE_FUNC(HelpLayer);
	bool init();
	void setDelegate(HelpLayerDelegate*pDelegate);
private:
	void closeBtnCallback(Object*sender);
};
#endif