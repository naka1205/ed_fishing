#include "SG_Engine/SEProgressTimer.h"
#include "SG_Engine/SESprite.h"
#include "SG_Engine/SEDirector.h"

NS_SDL_BEGIN
ProgressTimer::ProgressTimer()
	: _percentage(0.f), _type(Type::BAR), _sprite(nullptr), _bg(nullptr), _reverseDirection(false)
{
}
ProgressTimer::~ProgressTimer()
{
	SDL_SAFE_RELEASE(_sprite);
}
ProgressTimer *ProgressTimer::create(Sprite *sprite)
{
	auto progressTimer = new ProgressTimer();
	if (progressTimer && progressTimer->initWithSprite(sprite))
		progressTimer->autorelease();
	else
		SDL_SAFE_DELETE(progressTimer);
	return progressTimer;
}
bool ProgressTimer::initWithSprite(Sprite *sprite)
{
	setAnchorPoint(Point(0.5f, 0.5f));

	_type = Type::RADIAL;
	_reverseDirection = false;
	setMidpoint(Point(0.5f, 0.5f));
	setBarChangeRate(Point(0, 1));

	setSprite(sprite);

	setPercentage(0.0f);

	return true;
}
void ProgressTimer::setPercentage(float percentage)
{
	_percentage = percentage;

	if (_percentage < 0)
		_percentage = 0.f;
	if (_percentage > 100.f)
		_percentage = 100.f;

	switch (_type)
	{
	case Type::BAR:
		this->updateBar();
		break;
	case Type::RADIAL:
		this->updateRadial();
		break;
	}
}
void ProgressTimer::setType(Type type)
{
	_type = type;
}
void ProgressTimer::setSprite(Sprite *sprite)
{
	if (_sprite != sprite)
	{
		SDL_SAFE_RELEASE(_sprite);
		_sprite = sprite;
		SDL_SAFE_RETAIN(_sprite);

		if (_bg)
			_bg->removeFromParent();

		Size size = _sprite->getContentSize();
		_bg = Sprite::createWithTexture(SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, (int)size.width, (int)size.height);

		_bg->setPosition(_anchorPoint.x * size.width, _anchorPoint.y * size.height);

		this->setContentSize(size);

		_bg->getTexture()->setBlendMode(SDL_BLENDMODE_BLEND);
		this->addChild(_bg);
	}
}
void ProgressTimer::setMidpoint(const Point &midpoint)
{
	_midpoint = midpoint;
}
void ProgressTimer::setBarChangeRate(const Point &barChangeRate)
{
	_barChangeRate = barChangeRate;
}

void ProgressTimer::setRotation(float rotation)
{
	Node::setRotation(rotation);

	if (_bg)
		_bg->setRotation(rotation);
}

void ProgressTimer::updateBar()
{
	const int count = 4;
	Sint16 vx[count];
	Sint16 vy[count];

	Point hasRate = (Point(1, 1) - _barChangeRate);

	Point percent = hasRate + Point(_percentage / 100.f * _barChangeRate.x, _percentage / 100.f * _barChangeRate.y);

	Size size = _sprite->getContentSize();
	Size destSize = Size(size.width * percent.x, size.height * percent.y);

	Point destPos = Point(_midpoint.x * size.width, _midpoint.y * size.height);

	SDL_Rect destRect = {int(destPos.x - destSize.width * _midpoint.x), int(destPos.y - destSize.height * _midpoint.y), int(destSize.width), int(destSize.height)};

	vx[0] = destRect.x;
	vy[0] = destRect.y;
	vx[1] = destRect.x + destRect.w;
	vy[1] = destRect.y;
	vx[2] = destRect.x + destRect.w;
	vy[3] = destRect.y + destRect.h;
	vx[3] = destRect.x;
	vy[2] = destRect.y + destRect.h;

	this->texturedPolygon(vx, vy, 4);
}
void ProgressTimer::updateRadial()
{

	auto alpha = _percentage / 100.f;

	float angle = float(2 * M_PI * (_reverseDirection ? (1 - alpha) : alpha));

	Point topMid = Point(_midpoint.x, 0.f);

	Point percentagePt = topMid.rotateByAngle(_midpoint, angle);

	int index = 0;

	Point hit = Point::ZERO;
	if (alpha == 0.f)
	{
		hit = topMid;
		index = 0;
	}
	else if (alpha == 1.f)
	{
		hit = topMid;
		index = 4;
	}
	else
	{
		float min_t = FLT_MAX;

		for (int i = 0; i < 4; i++)
		{

			int pIndex = (i + 3) % 4;
			Point edgePtA = this->boundaryTexCoord(i % 4);
			Point edgePtB = this->boundaryTexCoord(pIndex);

			if (i == 0)
			{
				edgePtB = edgePtA.lerp(edgePtB, 1 - _midpoint.x);
			}
			else if (i == 4)
			{
				edgePtA = edgePtB.lerp(edgePtA, 1 - _midpoint.x);
			}

			float s = 0, t = 0;

			if (Point::isLineIntersect(edgePtA, edgePtB, _midpoint, percentagePt, &s, &t))
			{

				if ((i == 0 || i == 4))
				{

					if (!(0.f <= s && s <= 1.f))
					{
						continue;
					}
				}

				if (t >= 0.f)
				{
					if (t < min_t)
					{
						min_t = t;
						index = i;
					}
				}
			}
		}

		hit = _midpoint + ((percentagePt - _midpoint) * min_t);
	}

	int count = 3 + index;

	Sint16 vx[8], vy[8];

	memset(vx, 0, sizeof(vx));
	memset(vy, 0, sizeof(vy));

	vx[0] = Sint16(this->textureCoordFromAlphaPoint(_midpoint).x);
	vy[0] = Sint16(this->textureCoordFromAlphaPoint(_midpoint).y);

	vx[1] = Sint16(this->textureCoordFromAlphaPoint(topMid).x);
	vy[1] = Sint16(this->textureCoordFromAlphaPoint(topMid).y);

	for (int i = 0; i < index; i++)
	{
		Point boundry = this->boundaryTexCoord(i);
		vx[i + 2] = (Sint16)this->textureCoordFromAlphaPoint(boundry).x;
		vy[i + 2] = (Sint16)this->textureCoordFromAlphaPoint(boundry).y;
	}
	vx[count - 1] = (Sint16)this->textureCoordFromAlphaPoint(hit).x;
	vy[count - 1] = (Sint16)this->textureCoordFromAlphaPoint(hit).y;

	this->texturedPolygon(vx, vy, count);
}
Point ProgressTimer::boundaryTexCoord(int index)
{
	if (index == 0)
		return Point(1, 0);
	else if (index == 1)
		return Point(1, 1);
	else if (index == 2)
		return Point(0, 1);
	return Point(0, 0);
}
void ProgressTimer::texturedPolygon(Sint16 *vx, Sint16 *vy, int n)
{
	auto renderer = Director::getInstance()->getRenderer();

	Texture *bg = _bg->getTexture();

	SDL_BlendMode oldBlendMode = SDL_BLENDMODE_NONE;
	renderer->getDrawBlendMode(&oldBlendMode);

	renderer->setDrawBlendMode(SDL_BLENDMODE_NONE);
	renderer->setTarget(bg);
	auto oldColor = renderer->getDrawColor();

	renderer->setDrawColor(0, 0, 0, 0);
	renderer->clear();

	auto texture = _sprite->getTexture()->getSDL_Texture();
	auto render = renderer->getSDL_Renderer();

	::texturedPolygonMod(render, vx, vy, n, texture, 0, 0);

	renderer->setTarget(nullptr);

	renderer->setDrawColor(oldColor.r, oldColor.g, oldColor.b, oldColor.a);
	renderer->setDrawBlendMode(oldBlendMode);
}
Point ProgressTimer::textureCoordFromAlphaPoint(Point alpha)
{
	Size size = this->getContentSize();
	return Point(size.width * alpha.x, size.height * alpha.y);
}
NS_SDL_END
