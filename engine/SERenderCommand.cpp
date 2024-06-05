#include "SG_Engine/SERenderCommand.h"
#include "SG_Engine/SETexture.h"
#include "SG_Engine/SERenderer.h"

NS_SDL_BEGIN
RenderCommand::RenderCommand()
	: _globalZOrder(0.f)
{
}

RenderCommand::~RenderCommand()
{
}

void RenderCommand::execute(Renderer *renderer)
{
}

TextureCommand::TextureCommand()
	: _texture(nullptr), _rotation(0.f), _flip(SDL_FLIP_NONE), _opacity(255), _colorMod(255, 255, 255)
{
}

TextureCommand::~TextureCommand()
{
	SDL_SAFE_RELEASE_NULL(_texture);
}

void TextureCommand::init(float globalZOrder, Texture *texture, const Rect &srcRect, const Rect &destRect, float rotation, const Point &center, SDL_RendererFlip flip)
{
	_globalZOrder = globalZOrder;

	if (_texture != texture)
	{
		SDL_SAFE_RETAIN(texture);
		SDL_SAFE_RELEASE(_texture);

		_texture = texture;
	}
	_srcRect = srcRect;
	_destRect = destRect;
	_rotation = rotation;
	_center = center;
	_flip = flip;
}

void TextureCommand::execute(Renderer *renderer)
{

	_texture->setAlphaMod(_opacity);
	_texture->setColorMod(_colorMod.r, _colorMod.g, _colorMod.b);

	renderer->copyEx(_texture, _srcRect, _destRect, _rotation, _center, _flip);
}

CustomCommand::CustomCommand()
	: func(nullptr)
{
}

CustomCommand::~CustomCommand()
{
}

void CustomCommand::init(float globalZOrder)
{
	_globalZOrder = globalZOrder;
}

void CustomCommand::execute(Renderer *renderer)
{
	if (func)
	{
		func();
	}
}
NS_SDL_END
