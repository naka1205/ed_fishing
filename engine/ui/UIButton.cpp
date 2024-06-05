#include "SG_Engine/ui/UIButton.h"
#include "SG_Engine/SEObject.h"
#include "SG_Engine/SESprite.h"
#include "SG_Engine/SETouch.h"
#include "SG_Engine/SEPoint.h"
#include "SG_Engine/SERect.h"
NS_SDL_BEGIN
namespace ui
{
	Button::Button()
		: _normalSprite(nullptr), _selectedSprite(nullptr), _disabledSprite(nullptr)
	{
	}

	Button::~Button()
	{
	}

	Button *Button::create(const std::string &normalImage, const std::string &selectedImage, const std::string &disableImage, TextureResType resType)
	{
		auto button = new Button();
		if (button && button->init(normalImage, selectedImage, disableImage, resType))
			button->autorelease();
		else
			SDL_SAFE_DELETE(button);
		return button;
	}

	bool Button::init(const std::string &normalImage, const std::string &selectedImage, const std::string &disabledImage, TextureResType resType)
	{
		_resType = resType;
		this->loadNormalImage(normalImage);
		this->loadSelectedImage(selectedImage);
		this->loadDisabledImage(disabledImage);

		this->setTouchEnabled(true);

		if (_selectedSprite)
			_selectedSprite->setVisible(false);
		if (_disabledSprite)
			_disabledSprite->setVisible(false);

		return true;
	}
	void Button::loadNormalImage(const std::string &normalImage)
	{
		if (normalImage.empty())
			return;

		if (_normalSprite == nullptr)
		{
			_normalSprite = Sprite::create();
			this->addChild(_normalSprite);
		}

		switch (_resType)
		{
		case TextureResType::LOCAL:
			_normalSprite->setTexture(normalImage);
			break;
		case TextureResType::XML:
			_normalSprite->setSpriteFrame(normalImage);
			break;
		}
		Size size = _normalSprite->getContentSize();
		this->setContentSize(size);
		_normalSprite->setPosition(size.width * 0.5f, size.height * .5f);
	}
	void Button::loadSelectedImage(const std::string &selectedImage)
	{
		if (selectedImage.empty())
			return;

		if (_selectedSprite == nullptr)
		{
			_selectedSprite = Sprite::create();
			this->addChild(_selectedSprite);
		}

		switch (_resType)
		{
		case TextureResType::LOCAL:
			_selectedSprite->setTexture(selectedImage);
			break;
		case TextureResType::XML:
			_selectedSprite->setSpriteFrame(selectedImage);
			break;
		}
		Size size = _normalSprite->getContentSize();
		_selectedSprite->setPosition(size.width * 0.5f, size.height * .5f);
	}
	void Button::loadDisabledImage(const std::string &disabledImage)
	{
		if (disabledImage.empty())
			return;

		if (_disabledSprite == nullptr)
		{
			_disabledSprite = Sprite::create();
			this->addChild(_disabledSprite);
		}

		switch (_resType)
		{
		case TextureResType::LOCAL:
			_disabledSprite->setTexture(disabledImage);
			break;
		case TextureResType::XML:
			_disabledSprite->setSpriteFrame(disabledImage);
			break;
		}
		Size size = _normalSprite->getContentSize();
		_disabledSprite->setPosition(size.width * 0.5f, size.height * .5f);
	}

	void Button::setOpacity(Uint8 opacity)
	{
		Node::setOpacity(opacity);

		if (_normalSprite != nullptr)
			_normalSprite->setOpacity(opacity);
		if (_selectedSprite != nullptr)
			_selectedSprite->setOpacity(opacity);
		if (_disabledSprite != nullptr)
			_disabledSprite->setOpacity(opacity);
	}

	Uint8 Button::getOpacity() const
	{
		return Node::getOpacity();
	}

	void Button::setTouchEnabled(bool enable)
	{
		bool curEnabled = this->getTouchEnabled();
		if (enable == curEnabled)
			return;
		Widget::setTouchEnabled(enable);

		if (enable == false && _disabledSprite)
		{
			_disabledSprite->setVisible(true);
			_normalSprite->setVisible(false);
		}
		else if (enable == true && _disabledSprite)
		{
			_disabledSprite->setVisible(false);
			_normalSprite->setVisible(true);
		}
	}

	Node *Button::getVirtualRenderer()
	{
		return nullptr;
	}

	bool Button::touchBeganHook(Touch *touch, SDL_Event *event)
	{
		this->selected();
		return true;
	}
	void Button::touchMovedInHook(Touch *touch, SDL_Event *event)
	{
		this->selected();
	}
	void Button::touchMovedOutHook(Touch *touch, SDL_Event *event)
	{
		this->unselected();
	}
	void Button::touchEndedHook(Touch *touch, SDL_Event *event)
	{
		this->unselected();
	}
	void Button::touchCancelledHook(Touch *touch, SDL_Event *event)
	{
		this->unselected();
	}
	void Button::selected()
	{
		if (_bTouchEnabled && _normalSprite)
		{
			if (_disabledSprite)
				_disabledSprite->setVisible(false);
			if (_selectedSprite)
			{
				_normalSprite->setVisible(false);
				_selectedSprite->setVisible(true);
			}
			else
				_normalSprite->setVisible(true);
		}
	}
	void Button::unselected()
	{
		if (_bTouchEnabled && _normalSprite)
		{
			_normalSprite->setVisible(true);
			if (_selectedSprite)
				_selectedSprite->setVisible(false);
			if (_disabledSprite)
				_disabledSprite->setVisible(false);
		}
	}
}
NS_SDL_END
