#include "SG_Engine/ui/UIEditBox.h"
#include "SG_Engine/SEEventListenerKeyboard.h"
#include "SG_Engine/SEEventListenerTextInput.h"
#include "SG_Engine/SELabelDotChar.h"
#include "SG_Engine/SELabel.h"
#include "SG_Engine/SEScale9Sprite.h"
#include "SG_Engine/SETouch.h"
#include "SG_Engine/SEActionInterval.h"

NS_SDL_BEGIN
namespace ui
{
	EditBox::EditBox()
		: _bg(nullptr), _label(nullptr), _cursor(nullptr), _index(0), _maxLength(-1), _mode(InputMode::ANY), _flag(InputFlag::SENSITIVE), _delegate(nullptr), _textInputEnabled(false), _keyboardListener(nullptr), _inputListener(nullptr)
	{
	}

	EditBox::~EditBox()
	{
	}

	EditBox *EditBox::create(const Size &size, Scale9Sprite *bg, Label *label)
	{
		auto widget = new EditBox();

		if (widget && widget->init(size, bg, label))
			widget->autorelease();
		else
			SDL_SAFE_DELETE(widget);

		return widget;
	}

	bool EditBox::init(const Size &size, Scale9Sprite *bg, Label *label)
	{
		this->setContentSize(size);
		Point pos = Point(size.width * 0.5f, size.height * 0.5f);

		_bg = bg;
		_bg->setPosition(pos);
		_bg->setPreferredSize(size);
		this->addChild(_bg, -1);

		_label = label;
		_label->setPosition(pos);
		this->addChild(_label);

		_cursor = LabelDotChar::create("|", 3, Color3B(0, 0, 0));
		_cursor->setPosition(pos);
		this->addChild(_cursor, 10);

		this->setTouchEnabled(true);
		SDL_StopTextInput();

		return true;
	}

	void EditBox::setTextInputEnabled(bool enabled)
	{
		if (enabled == _textInputEnabled)
			return;
		_textInputEnabled = enabled;

		if (enabled)
		{
			_keyboardListener = EventListenerKeyboard::create();
			_keyboardListener->onKeyPressed = SDL_CALLBACK_2(EditBox::onKeyPressed, this);
			_eventDispatcher->addEventListener(_keyboardListener, this);

			_inputListener = EventListenerTextInput::create();
			_inputListener->onTextInput = SDL_CALLBACK_2(EditBox::onTextInput, this);
			_eventDispatcher->addEventListener(_inputListener, this);
		}
		else
		{
			_eventDispatcher->removeEventListener(_keyboardListener);
			_keyboardListener = nullptr;

			_eventDispatcher->removeEventListener(_inputListener);
			_inputListener = nullptr;
		}
	}

	bool EditBox::getTextInputEnabled() const
	{
		return _textInputEnabled;
	}

	const std::string &EditBox::getText() const
	{
		return _text;
	}

	void EditBox::setText(const std::string &text)
	{
		if (text == _text)
			return;
		_text.clear();

		bool ret = false;
		for (size_t i = 0; i < text.size(); i++)
		{
			switch (_mode)
			{
			case EditBox::InputMode::ANY:
				ret = true;
				break;
			case EditBox::InputMode::NUMERIC:
				ret = this->isNumeric(text[i]);
				break;
			case EditBox::InputMode::DECIMAL:
				ret = this->isDecimal(text[i]);
				break;
			}
			if (ret)
				_text.append(1, text[i]);
		}

		this->updateLabelString();
		_index = 0;
		this->updateCursor();
	}

	unsigned int EditBox::getMaxLength() const
	{
		return _maxLength;
	}

	void EditBox::setMaxLength(unsigned int len)
	{
		_maxLength = len;
	}

	EditBox::InputMode EditBox::getInputMode() const
	{
		return _mode;
	}

	void EditBox::setInputMode(EditBox::InputMode mode)
	{
		_mode = mode;
	}

	EditBox::InputFlag EditBox::getInputFlag() const
	{
		return _flag;
	}
	void EditBox::setInputFlag(EditBox::InputFlag flag)
	{
		_flag = flag;
	}

	void EditBox::setDelegate(EditBoxDelegate *pDelegate)
	{
		_delegate = pDelegate;
	}

	Label *EditBox::getLabel()
	{
		return _label;
	}

	void EditBox::setLabel(Label *label)
	{
		SDL_SAFE_RETAIN(label);

		if (_label != nullptr)
		{
			label->setPosition(_label->getPosition());
			_label->removeFromParent();
		}
		_label = label;
		this->addChild(_label);

		SDL_SAFE_RELEASE(label);
	}

	bool EditBox::hitTest(Touch *touch)
	{
		return true;
	}

	bool EditBox::touchBeganHook(Touch *touch, SDL_Event *event)
	{
		auto pos = touch->getLocation();

		pos = this->getParent()->convertToNodeSpace(pos);

		auto rect = this->getBoundingBox();
		bool ret = false;

		if (rect.containsPoint(pos))
		{

			if (!SDL_IsTextInputActive())
			{
				SDL_StartTextInput();
				this->setTextInputEnabled(true);

				_delegate->editBoxEditingDidBegin(this);

				this->showCursor();
			}

			pos = this->convertToNodeSpace(pos);
			auto number = strlen(_text.c_str());
			auto rect2 = _label->getBoundingBox();
			if (number == 0 || pos.x < rect2.origin.x)
				_index = 0;
			else if (pos.x > rect2.origin.x + rect2.size.width)
				_index = number;
			else
			{
				auto relativePosX = pos.x - rect2.origin.x;
				_index = 0;
				int width = int(rect2.size.width / number);
				while (relativePosX > (0.5f + _index) * width)
					_index++;
			}
			this->updateCursor();
			ret = true;
		}
		else if (SDL_IsTextInputActive())
		{
			SDL_StopTextInput();
			this->setTextInputEnabled(false);

			_delegate->editBoxEditingEndWithAction(this, EditBoxEndAction::UNKNOWN);

			this->hideCursor();
			ret = true;
		}
		return ret;
	}

	void EditBox::touchMovedInHook(Touch *touch, SDL_Event *event)
	{
	}

	void EditBox::touchMovedOutHook(Touch *touch, SDL_Event *event)
	{
	}

	void EditBox::touchEndedHook(Touch *touch, SDL_Event *event)
	{
	}

	void EditBox::touchCancelledHook(Touch *touch, SDL_Event *event)
	{
	}

	void EditBox::onKeyPressed(SDL_Keycode keycode, SDL_Event *event)
	{
		switch (keycode)
		{
		case SDLK_BACKSPACE:

			if (_index > 0)
			{
				//_text[--_index] = '\0';
				_text.erase(_index - 1, 1);
				_index--;
				_delegate->editBoxTextChanged(this, _text);

				this->updateLabelString();
				this->updateCursor();
			}
			break;
		case SDLK_RETURN:
			SDL_StopTextInput();
			this->setTextInputEnabled(false);

			this->hideCursor();
			_delegate->editBoxEditingEndWithAction(this, EditBoxEndAction::RETURN);
			_delegate->editBoxReturn(this);
			break;
		case SDLK_LEFT:
			if (_index > 0)
			{
				_index--;
				this->updateCursor();
			}
			break;
		case SDLK_RIGHT:
			if (_index < strlen(_text.c_str()))
			{
				_index++;
				this->updateCursor();
			}
			break;
		case SDLK_UP:
			break;
		case SDLK_DOWN:
			break;
		}
	}

	void EditBox::onTextInput(char text[32], SDL_Event *event)
	{
		auto number = strlen(_text.c_str());
		// beyond the max length
		if (number >= _maxLength)
			return;

		bool ret = false;
		switch (_mode)
		{
		case EditBox::InputMode::NUMERIC:
			ret = this->isNumeric(text[0]);
			break;
		case EditBox::InputMode::DECIMAL:
			ret = this->isDecimal(text[0]);
			break;
		case EditBox::InputMode::ANY:
			ret = true;
			break;
		}
		if (!ret)
			return;
		_text.insert(_index, text);

		_index += 1;

		this->updateLabelString();
		this->updateCursor();
		_delegate->editBoxTextChanged(this, _text);
	}

	void EditBox::showCursor()
	{
		_cursor->setVisible(true);

		auto blink = Blink::create(1, 1);
		auto repeat = RepeatForever::create(blink);
		repeat->setTag(1);

		_cursor->stopActionByTag(1);
		_cursor->runAction(repeat);
	}

	void EditBox::hideCursor()
	{
		_cursor->stopActionByTag(1);
		_cursor->setVisible(false);
	}

	void EditBox::updateCursor()
	{

		int number = strlen(_text.c_str());

		if (number == 0)
		{
			_cursor->setPosition(_label->getPosition());
		}
		else
		{
			auto rect = _label->getBoundingBox();
			int width = rect.size.width / number;

			int posX = (_index)*width + rect.origin.x;
			// printf("number:%d width:%d posX:%d\n", number, width, posX);
			_cursor->setPositionX(posX);
		}
	}
	bool EditBox::isNumeric(char c)
	{
		return c >= '0' && c <= '9';
	}

	bool EditBox::isDecimal(char c)
	{
		return (c >= '0' && c <= '9') || c == '.';
	}
	void EditBox::updateLabelString()
	{
		if (_flag == InputFlag::PASSWORD)
		{
			auto number = strlen(_text.c_str());
			std::string text = std::string(number, '*');
			_label->setString(text);
		}
		else
		{
			_label->setString(_text);
		}
	}

}
NS_SDL_END
