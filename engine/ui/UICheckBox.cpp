#include "SG_Engine/ui/UICheckBox.h"
#include "SG_Engine/SESprite.h"
NS_SDL_BEGIN
namespace ui
{
	CheckBox::CheckBox()
		: _checkBoxEventCallback(nullptr)
	{
	}
	CheckBox::~CheckBox()
	{
	}
	CheckBox *CheckBox::create(const std::string &background, const std::string &cross, TextureResType resType)
	{
		auto widget = new CheckBox();
		if (widget && widget->init(background, cross, resType))
			widget->autorelease();
		else
			SDL_SAFE_DELETE(widget);
		return widget;
	}
	CheckBox *CheckBox::create(const std::string &background, const std::string &backgroundSelected, const std::string &cross, const std::string &backgroundDisabled, const std::string &crossDisabled, TextureResType resType)
	{
		auto widget = new CheckBox();
		if (widget && widget->init(background, backgroundSelected, cross, backgroundDisabled, crossDisabled, resType))
			widget->autorelease();
		else
			SDL_SAFE_DELETE(widget);
		return widget;
	}
	void CheckBox::addEventListener(const ccCheckBoxCallback &callback)
	{
		_checkBoxEventCallback = callback;
	}
	bool CheckBox::touchBeganHook(Touch *touch, SDL_Event *event)
	{
		bool selected = this->isSelected();
		this->updateStatus(selected);
		return true;
	}
	void CheckBox::touchMovedInHook(Touch *touch, SDL_Event *event)
	{
		bool selected = this->isSelected();
		this->updateStatus(selected);
	}
	void CheckBox::touchMovedOutHook(Touch *touch, SDL_Event *event)
	{
		bool selected = this->isSelected();
		this->updateStatus(!selected);
	}
	void CheckBox::touchEndedHook(Touch *touch, SDL_Event *event)
	{
		bool selected = this->isSelected();

		this->updateStatus(selected);

		this->setSelected(!selected);

		EventType type = this->isSelected() ? EventType::SELECTED : EventType::UNSELECTED;

		if (_checkBoxEventCallback)
			_checkBoxEventCallback(this, type);
	}
	void CheckBox::touchCancelledHook(Touch *touch, SDL_Event *event)
	{
		this->unselected();
	}
	void CheckBox::updateStatus(bool selected)
	{

		if (selected)
			this->unselected();
		else
			this->selected();
	}
	void CheckBox::selected()
	{

		if (_backgroundSelectedSprite)
		{
			_backgroundSelectedSprite->setVisible(true);
			_backgroundSelectedSprite->setScale(1.1f, 1.1f);
			_backgroundSprite->setVisible(false);
		}
		else
		{
			_backgroundSprite->setScale(1.1f, 1.1f);
		}
	}
	void CheckBox::unselected()
	{
		if (_backgroundSelectedSprite)
		{
			_backgroundSelectedSprite->setVisible(false);
			_backgroundSelectedSprite->setScale(1.f, 1.f);
			_backgroundSprite->setVisible(true);
		}
		else
		{
			_backgroundSprite->setScale(1.f, 1.f);
		}
	}
}
NS_SDL_END
