#include "SG_Engine/ui/UIRadioButton.h"
#include "SG_Engine/ui/UIRadioButtonGroup.h"
#include "SG_Engine/SESprite.h"
NS_SDL_BEGIN
namespace ui
{
	RadioButton::RadioButton()
		: _radioButtonEventCallback(nullptr), _group(nullptr)
	{
	}
	RadioButton::~RadioButton()
	{
	}
	RadioButton *RadioButton::create(const std::string &background, const std::string &cross, TextureResType resType)
	{
		auto widget = new RadioButton();
		if (widget && widget->init(background, cross, resType))
			widget->autorelease();
		else
			SDL_SAFE_DELETE(widget);
		return widget;
	}
	RadioButton *RadioButton::create(const std::string &background, const std::string &backgroundSelected, const std::string &cross, const std::string &backgroundDisabled, const std::string &crossDisabled, TextureResType resType)
	{
		auto widget = new RadioButton();
		if (widget && widget->init(background, backgroundSelected, cross, backgroundDisabled, crossDisabled, resType))
			widget->autorelease();
		else
			SDL_SAFE_DELETE(widget);
		return widget;
	}
	void RadioButton::addEventListener(const ccRadioButtonCallback &callback)
	{
		_radioButtonEventCallback = callback;
	}
	bool RadioButton::touchBeganHook(Touch *touch, SDL_Event *event)
	{
		this->selected();
		return true;
	}
	void RadioButton::touchMovedInHook(Touch *touch, SDL_Event *event)
	{
		this->selected();
	}
	void RadioButton::touchMovedOutHook(Touch *touch, SDL_Event *event)
	{
		this->unselected();
	}
	void RadioButton::touchEndedHook(Touch *touch, SDL_Event *event)
	{

		bool selected = this->isSelected();

		this->updateStatus(false);
		if (selected)
			return;

		this->setSelected(!selected);

		if (_radioButtonEventCallback)
			_radioButtonEventCallback(this, EventType::SELECTED);

		if (_group)
			_group->setSelectedButton(this);
	}
	void RadioButton::touchCancelledHook(Touch *touch, SDL_Event *event)
	{
		this->unselected();
	}
	void RadioButton::updateStatus(bool selected)
	{

		if (selected)
			this->selected();
		else
			this->unselected();
	}
	void RadioButton::selected()
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
	void RadioButton::unselected()
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
	void RadioButton::dispatchSelectChangeEvent(bool selected)
	{
		if (_selected == selected)
			return;

		this->setSelected(selected);

		EventType type = selected ? EventType::SELECTED : EventType::UNSELECTED;
		if (_radioButtonEventCallback)
			_radioButtonEventCallback(this, type);
	}
}
NS_SDL_END
