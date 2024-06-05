#include "SG_Engine/ui/UIRadioButtonGroup.h"
#include "SG_Engine/ui/UIRadioButton.h"

NS_SDL_BEGIN
namespace ui
{
	RadioButtonGroup::RadioButtonGroup()
		: _selectedButton(nullptr), _radioButtonGroupCallback(nullptr)
	{
	}
	RadioButtonGroup::~RadioButtonGroup()
	{
		_radioButtons.clear();
	}
	void RadioButtonGroup::addEventListener(const ccRadioButtonGroupCallback &callback)
	{
		_radioButtonGroupCallback = callback;
	}
	int RadioButtonGroup::getSelectedIndex() const
	{
		if (_selectedButton == nullptr)
			return -1;
		return _radioButtons.getIndex(_selectedButton);
	}

	RadioButton *RadioButtonGroup::getSelectedButton() const
	{
		return _selectedButton;
	}

	void RadioButtonGroup::setSelectedButton(unsigned int index)
	{
		SDL_assert(index < _radioButtons.size());

		auto radioButton = _radioButtons.at(index);
		this->setSelectedButton(radioButton);
	}
	void RadioButtonGroup::setSelectedButton(RadioButton *radioButton)
	{
		if (radioButton == _selectedButton)
			return;

		if (_selectedButton)
			_selectedButton->dispatchSelectChangeEvent(false);

		_selectedButton = radioButton;
		_selectedButton->dispatchSelectChangeEvent(true);

		if (_radioButtonGroupCallback)
			_radioButtonGroupCallback(_selectedButton, _radioButtons.getIndex(_selectedButton), EventType::SELECTED_CHANGE);
	}

	void RadioButtonGroup::unselectedButton()
	{
		if (_selectedButton == nullptr)
			return;

		if (_selectedButton)
			_selectedButton->dispatchSelectChangeEvent(false);
		_selectedButton = nullptr;
	}

	void RadioButtonGroup::setSelectedButtonWithoutEvent(unsigned int index)
	{
		SDL_assert(index < _radioButtons.size());
		auto radioButton = _radioButtons.at(index);
		this->setSelectedButtonWithoutEvent(radioButton);
	}
	void RadioButtonGroup::setSelectedButtonWithoutEvent(RadioButton *radioButton)
	{
		if (radioButton == _selectedButton)
			return;

		if (_selectedButton)
			_selectedButton->dispatchSelectChangeEvent(false);

		_selectedButton = radioButton;
		_selectedButton->dispatchSelectChangeEvent(true);
	}
	void RadioButtonGroup::addRadioButton(RadioButton *radioButton)
	{
		if (radioButton && radioButton->_group == nullptr)
		{
			radioButton->_group = this;

			_radioButtons.push_back(radioButton);

			this->setSelectedButtonWithoutEvent(radioButton);
		}
	}
	void RadioButtonGroup::removeRadioButton(RadioButton *radioButton)
	{
		if (radioButton)
		{
			if (_selectedButton == radioButton)
			{
				_selectedButton->dispatchSelectChangeEvent(false);
				_selectedButton = nullptr;
			}
			radioButton->_group = nullptr;
			_radioButtons.eraseObject(radioButton);
		}
	}
	void RadioButtonGroup::removeAllRadioButtons()
	{
		_radioButtons.clear();
		_selectedButton = nullptr;
		_radioButtonGroupCallback = nullptr;
	}
	RadioButton *RadioButtonGroup::getRadioButtonByIndex(int index)
	{
		return _radioButtons.at(index);
	}
	int RadioButtonGroup::getNumberOfRadioButtons() const
	{
		return _radioButtons.size();
	}

	Vector<RadioButton *> &RadioButtonGroup::getRadioButtonList()
	{
		return _radioButtons;
	}
}
NS_SDL_END