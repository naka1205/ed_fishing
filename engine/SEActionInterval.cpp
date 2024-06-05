#include "SG_Engine/SEActionInterval.h"
#include "SG_Engine/SENode.h"
#include "SG_Engine/SEMath.h"
#include "SG_Engine/SESprite.h"
#include "SG_Engine/SEActionInstant.h"
#include "SG_Engine/SEAnimation.h"
#include "SG_Engine/SESpriteFrame.h"
#include "SG_Engine/SEAnimationFrame.h"
#include "SG_Engine/SEDirector.h"
#include "SG_Engine/SEEventDispatcher.h"

NS_SDL_BEGIN
ActionInterval::ActionInterval()
	: _elapsed(0.f), _bFirstTick(true)
{
}

ActionInterval::~ActionInterval()
{
}

bool ActionInterval::initWithDuration(float d)
{

	_duration = d;

	if (_duration == 0)
		_duration = FLT_EPSILON;

	_elapsed = 0.f;

	_bFirstTick = true;
	return true;
}
bool ActionInterval::isDone() const
{
	return _elapsed >= _duration;
}
void ActionInterval::step(float dt)
{

	if (_bFirstTick)
	{
		_elapsed = 0;
		_bFirstTick = false;
	}
	else
		_elapsed += dt;

	this->update(MAX(0,
					 MIN(1, _elapsed / MAX(_duration, FLT_EPSILON))));
}
void ActionInterval::startWithTarget(Node *target)
{
	FiniteTimeAction::startWithTarget(target);
	_elapsed = 0.f;
	_bFirstTick = true;
}
ActionInterval *ActionInterval::clone() const
{
	return nullptr;
}
ActionInterval *ActionInterval::reverse() const
{
	return NULL;
}

void ActionInterval::setAmplitudeRate(float amp)
{
}
float ActionInterval::getAmplitudeRate() const
{
	return 0;
}

MoveTo *MoveTo::create(float duration, const Point &position)
{
	auto moveTo = new MoveTo();
	if (moveTo && moveTo->initWithDuration(duration, position))
		moveTo->autorelease();
	else
		SDL_SAFE_DELETE(moveTo);
	return moveTo;
}
bool MoveTo::initWithDuration(float duration, const Point &position)
{
	if (ActionInterval::initWithDuration(duration))
	{
		_endPosition = position;
		return true;
	}
	return false;
}
void MoveTo::startWithTarget(Node *target)
{
	ActionInterval::startWithTarget(target);

	_startPosition = target->getPosition();
	_deltaPosition = _endPosition - _startPosition;
}
void MoveTo::update(float time)
{

	_target->setPosition(_startPosition + _deltaPosition * time);
}
MoveTo *MoveTo::reverse() const
{
	return MoveTo::create(_duration, Point(-_endPosition.x, -_endPosition.y));
}
MoveTo *MoveTo::clone() const
{
	return MoveTo::create(_duration, _endPosition);
}

MoveBy::MoveBy() : MoveTo()
{
}
MoveBy::~MoveBy()
{
}
MoveBy *MoveBy::create(float duration, const Point &relativePosition)
{
	auto action = new MoveBy();
	if (action && action->initWithDuration(duration, relativePosition))
		action->autorelease();
	else
		SDL_SAFE_DELETE(action);
	return action;
}
bool MoveBy::initWithDuration(float duration, const Point &relativePosition)
{
	if (ActionInterval::initWithDuration(duration))
	{
		_deltaPosition = relativePosition;
		return true;
	}
	return false;
}
void MoveBy::startWithTarget(Node *target)
{
	ActionInterval::startWithTarget(target);
	_startPosition = target->getPosition();
	_endPosition = _startPosition + _deltaPosition;
}
MoveBy *MoveBy::clone() const
{
	return MoveBy::create(_duration, _deltaPosition);
}
MoveBy *MoveBy::reverse() const
{
	return MoveBy::create(_duration, Point::ZERO - _deltaPosition);
}

Animate::Animate()
	: _originalFrame(nullptr), _nextFrame(0), _animation(nullptr), _executedLoops(0), _splitTimes(new std::vector<float>()), _frameDisplayedEvent(nullptr), _frameDisplayedEventInfo()
{
}
Animate::~Animate()
{
	delete _splitTimes;
	SDL_SAFE_DELETE(_frameDisplayedEvent);

	SDL_SAFE_RELEASE(_animation);
	SDL_SAFE_RELEASE(_originalFrame);
}
Animate *Animate::create(Animation *animation)
{
	auto animate = new Animate();
	if (animate && animate->initWithAnimation(animation))
		animate->autorelease();
	else
		SDL_SAFE_DELETE(animate);
	return animate;
}
bool Animate::initWithAnimation(Animation *animation)
{
	SDLASSERT(animation != nullptr, "error");

	float singleDuration = animation->getDuration();
	if (ActionInterval::initWithDuration(singleDuration * animation->getLoops()))
	{
		_nextFrame = 0;
		_originalFrame = nullptr;
		_executedLoops = 0;
		animation->retain();
		_animation = animation;

		int totalFramesNum = animation->getTotalFrames();

		_splitTimes->reserve(totalFramesNum);
		std::vector<AnimationFrame *> frames = animation->getFrames();
		_splitTimes->push_back(0.f);

		for (auto frame : frames)
		{
			float delayUnit = frame->getDelayUnits() / singleDuration;
			_splitTimes->push_back(_splitTimes->back() + delayUnit);
		}
		_splitTimes->pop_back();
		return true;
	}
	return false;
}
void Animate::startWithTarget(Node *target)
{
	ActionInterval::startWithTarget(target);
	if (_animation->getRestoreOriginalFrame())
	{
		Sprite *sprite = static_cast<Sprite *>(target);
		_originalFrame = sprite->getSpriteFrame();
		_originalFrame->retain();
	}
	_nextFrame = 0;
	_executedLoops = 0;
}

void Animate::update(float time)
{

	if (time < 1.f)
	{
		time *= _animation->getLoops();

		unsigned int loopsNumber = (unsigned int)time;
		if (loopsNumber > _executedLoops)
		{
			_nextFrame = 0;
			++_executedLoops;
		}
		//
		time = fmodf(time, 1.f);
	}
	auto &frames = _animation->getFrames();
	auto numberOfFrames = frames.size();
	SpriteFrame *displayFrame = nullptr;
	for (unsigned int i = _nextFrame; i < numberOfFrames; i++)
	{
		float splitTime = _splitTimes->at(i);

		if (splitTime <= time)
		{
			displayFrame = frames.at(i)->getSpriteFrame();
			AnimationFrame *frame = frames.at(i);

			static_cast<Sprite *>(_target)->setSpriteFrame(displayFrame);

			const ValueMap &dict = frame->getUserInfo();

			if (!dict.empty())
			{
				if (_frameDisplayedEvent == nullptr)
					_frameDisplayedEvent = new EventCustom(AnimationFrameDisplayedNotification);

				_frameDisplayedEventInfo.target = _target;
				_frameDisplayedEventInfo.userInfo = &dict;

				_frameDisplayedEvent->setUserData(&_frameDisplayedEventInfo);
				Director::getInstance()->getEventDispatcher()->dispatchCustomEvent(_frameDisplayedEvent);
			}

			_nextFrame = i + 1;
		}
		else
			break;
	}
}
void Animate::stop()
{
	if (_animation->getRestoreOriginalFrame() && _target)
	{
		static_cast<Sprite *>(_target)->setSpriteFrame(_originalFrame);
	}
	Action::stop();
}
Animate *Animate::clone() const
{
	return Animate::create(_animation);
}
Animate *Animate::reverse() const
{
	auto frames = _animation->getFrames();
	std::vector<AnimationFrame *> reverse_frames;

	for (int i = frames.size() - 1; i >= 0; i--)
		reverse_frames.push_back(frames.at(i));

	Animation *animation = Animation::createWithAnimationFrames(reverse_frames);

	animation->setLoops(_animation->getLoops());
	animation->setRestoreOriginalFrame(_animation->getRestoreOriginalFrame());

	return Animate::create(animation);
}

ScaleTo *ScaleTo::create(float duration, float scaleX, float scaleY)
{
	auto scaleTo = new ScaleTo();
	if (scaleTo && scaleTo->initWithScaleTo(duration, scaleX, scaleY))
		scaleTo->autorelease();
	else
		SDL_SAFE_DELETE(scaleTo);
	return scaleTo;
}
ScaleTo *ScaleTo::create(float duration, float scale)
{
	auto scaleTo = new ScaleTo();
	if (scaleTo && scaleTo->initWithScaleTo(duration, scale, scale))
		scaleTo->autorelease();
	else
		SDL_SAFE_DELETE(scaleTo);
	return scaleTo;
}
bool ScaleTo::initWithScaleTo(float duration, float scaleX, float scaleY)
{
	if (ActionInterval::initWithDuration(duration))
	{
		_endScaleX = scaleX;
		_endScaleY = scaleY;
		return true;
	}
	return false;
}
void ScaleTo::startWithTarget(Node *target)
{
	ActionInterval::startWithTarget(target);
	_startScaleX = target->getScaleX();
	_startScaleY = target->getScaleY();
	_deltaX = _endScaleX - _startScaleX;
	_deltaY = _endScaleY - _startScaleY;
}
void ScaleTo::update(float time)
{
	_target->setScale(_startScaleX + time * _deltaX, _startScaleY + time * _deltaY);
}
ScaleTo *ScaleTo::clone() const
{
	return ScaleTo::create(_duration, _endScaleX, _endScaleY);
}
ScaleTo *ScaleTo::reverse() const
{
	return nullptr;
}

ScaleBy *ScaleBy::create(float duration, float scale)
{
	auto scaleBy = new ScaleBy();
	if (scaleBy && scaleBy->initWithDuration(duration, scale, scale))
		scaleBy->autorelease();
	else
		SDL_SAFE_DELETE(scaleBy);
	return scaleBy;
}
ScaleBy *ScaleBy::create(float duration, float scaleX, float scaleY)
{
	auto scaleBy = new ScaleBy();
	if (scaleBy && scaleBy->initWithDuration(duration, scaleX, scaleY))
		scaleBy->autorelease();
	else
		SDL_SAFE_DELETE(scaleBy);
	return scaleBy;
}
bool ScaleBy::initWithDuration(float duration, float scaleX, float scaleY)
{
	if (ActionInterval::initWithDuration(duration))
	{
		_deltaX = scaleX;
		_deltaY = scaleY;
		return true;
	}
	return false;
}
void ScaleBy::startWithTarget(Node *target)
{
	ActionInterval::startWithTarget(target);
	_startScaleX = target->getScaleX();
	_startScaleY = target->getScaleY();
	_endScaleX = _startScaleX + _deltaX;
	_endScaleY = _startScaleY + _deltaY;
}
ScaleBy *ScaleBy::clone() const
{
	return ScaleBy::create(_duration, _deltaX, _deltaY);
}
ScaleBy *ScaleBy::reverse() const
{
	return nullptr;
}

RotateTo::RotateTo() : _startRotation(0), _endRotation(0), _deltaRotation(0)
{
}
RotateTo *RotateTo::create(float duration, float rotate)
{
	auto rotateTo = new RotateTo();
	if (rotateTo && rotateTo->initWithDuration(duration, rotate))
		rotateTo->autorelease();
	else
		SDL_SAFE_DELETE(rotateTo);
	return rotateTo;
}
bool RotateTo::initWithDuration(float duration, float rotate)
{
	if (ActionInterval::initWithDuration(duration))
	{
		_endRotation = rotate;
		return true;
	}
	return false;
}
void RotateTo::startWithTarget(Node *target)
{
	ActionInterval::startWithTarget(target);
	_startRotation = target->getRotation();
	_deltaRotation = _endRotation - _startRotation;
}
void RotateTo::update(float time)
{
	_target->setRotation(_startRotation + _deltaRotation * time);
}
RotateTo *RotateTo::clone() const
{
	return RotateTo::create(_duration, _endRotation);
}
RotateTo *RotateTo::reverse() const
{
	return RotateTo::create(_duration, 0 - _endRotation);
}

RotateBy *RotateBy::create(float duration, float rotation)
{
	auto rotate = new RotateBy();
	if (rotate && rotate->initWithDuration(duration, rotation))
		rotate->autorelease();
	else
		SDL_SAFE_DELETE(rotate);
	return rotate;
}
bool RotateBy::initWithDuration(float duration, float rotation)
{
	if (ActionInterval::initWithDuration(duration))
	{
		_deltaRotation = rotation;
		return true;
	}
	return false;
}
void RotateBy::startWithTarget(Node *target)
{
	ActionInterval::startWithTarget(target);
	_startRotation = target->getRotation();
	_endRotation = _startRotation + _deltaRotation;
}
RotateBy *RotateBy::clone() const
{
	return RotateBy::create(_duration, _deltaRotation);
}
RotateBy *RotateBy::reverse() const
{
	return RotateBy::create(_duration, 0 - _deltaRotation);
}

Blink::~Blink()
{
}
Blink *Blink::create(float duration, int times)
{
	auto blink = new Blink();
	if (blink && blink->initWithDuration(duration, times))
		blink->autorelease();
	else
		SDL_SAFE_DELETE(blink);
	return blink;
}
bool Blink::initWithDuration(float duration, int times)
{
	if (ActionInterval::initWithDuration(duration))
	{
		_times = times;
		return true;
	}
	return false;
}
void Blink::startWithTarget(Node *target)
{
	ActionInterval::startWithTarget(target);
	_originalState = target->isVisible();
}
void Blink::update(float time)
{
	if (_target && !isDone())
	{
		float slice = 1.f / _times;
		float m = fmodf(time, slice);
		_target->setVisible(m > slice / 2 ? true : false);
	}
}
Blink *Blink::clone() const
{
	return Blink::create(_duration, _times);
}
Blink *Blink::reverse() const
{
	return Blink::create(_duration, _times);
}

FadeTo *FadeTo::create(float duration, Uint8 opacity)
{
	auto fade = new FadeTo();
	if (fade && fade->initWithDuration(duration, opacity))
		fade->autorelease();
	else
		SDL_SAFE_DELETE(fade);
	return fade;
}
bool FadeTo::initWithDuration(float duration, Uint8 opacity)
{
	if (ActionInterval::initWithDuration(duration))
	{
		_toOpacity = opacity;
		return true;
	}
	return false;
}
void FadeTo::startWithTarget(Node *target)
{
	ActionInterval::startWithTarget(target);
	_fromOpacity = target->getOpacity();
}
void FadeTo::update(float time)
{
	if (_target)
	{
		Uint8 opacity = Uint8(_fromOpacity + (_toOpacity - _fromOpacity) * time);
		_target->setOpacity(opacity);
	}
}
FadeTo *FadeTo::clone() const
{
	return FadeTo::create(_duration, _toOpacity);
}
FadeTo *FadeTo::reverse() const
{
	return nullptr;
}

FadeIn *FadeIn::create(float duration)
{
	auto fade = new FadeIn();
	if (fade && fade->initWithDuration(duration, 255))
		fade->autorelease();
	else
		SDL_SAFE_DELETE(fade);
	return fade;
}
void FadeIn::startWithTarget(Node *target)
{
	ActionInterval::startWithTarget(target);
	_fromOpacity = target->getOpacity();
	_toOpacity = 255;
}
FadeIn *FadeIn::clone() const
{
	return FadeIn::create(_duration);
}
FadeTo *FadeIn::reverse() const
{
	return FadeTo::create(_duration, 0);
}

FadeOut *FadeOut::create(float duration)
{
	auto fade = new FadeOut();
	if (fade && fade->initWithDuration(duration, 0))
		fade->autorelease();
	else
		SDL_SAFE_DELETE(fade);
	return fade;
}
void FadeOut::startWithTarget(Node *target)
{
	ActionInterval::startWithTarget(target);
	_fromOpacity = target->getOpacity();
	_toOpacity = 0;
}
FadeOut *FadeOut::clone() const
{
	return FadeOut::create(_duration);
}
FadeTo *FadeOut::reverse() const
{
	return FadeTo::create(_duration, 255);
}

TintTo *TintTo::create(float duration, Uint8 r, Uint8 g, Uint8 b)
{
	auto tint = new TintTo();
	if (tint && tint->initWithDuration(duration, r, g, b))
		tint->autorelease();
	else
		SDL_SAFE_DELETE(tint);
	return tint;
}
bool TintTo::initWithDuration(float duration, Uint8 r, Uint8 g, Uint8 b)
{
	if (ActionInterval::initWithDuration(duration))
	{
		_toColor.r = r;
		_toColor.g = g;
		_toColor.b = b;
		return true;
	}
	return false;
}
void TintTo::startWithTarget(Node *target)
{
	ActionInterval::startWithTarget(target);
	_fromColor = target->getColorMod();
}
void TintTo::update(float time)
{
	if (_target)
	{
		Uint8 r = Uint8(_fromColor.r + (_toColor.r - _fromColor.r) * time);
		Uint8 g = Uint8(_fromColor.g + (_toColor.g - _fromColor.g) * time);
		Uint8 b = Uint8(_fromColor.b + (_toColor.b - _fromColor.b) * time);

		_target->setColorMod(r, g, b);
	}
}
TintTo *TintTo::clone() const
{
	return TintTo::create(_duration, _toColor.r, _toColor.g, _toColor.b);
}
TintTo *TintTo::reverse() const
{
	return nullptr;
}

TintBy *TintBy::create(float duration, short r, short g, short b)
{
	auto tint = new TintBy();
	if (tint && tint->initWithDuration(duration, r, g, b))
		tint->autorelease();
	else
		SDL_SAFE_DELETE(tint);
	return tint;
}
bool TintBy::initWithDuration(float duration, short r, short g, short b)
{
	if (ActionInterval::initWithDuration(duration))
	{
		_deltaR = r;
		_deltaG = g;
		_deltaB = b;
		return true;
	}
	return false;
}
void TintBy::startWithTarget(Node *target)
{
	ActionInterval::startWithTarget(target);
	Color3B color = target->getColorMod();

	_fromR = color.r;
	_fromG = color.g;
	_fromB = color.b;
}
void TintBy::update(float time)
{
	if (_target)
	{
		Uint8 r = Uint8(_fromR + _deltaR * time);
		Uint8 g = Uint8(_fromG + _deltaG * time);
		Uint8 b = Uint8(_fromB + _deltaB * time);

		Color3B color = Color3B(r, g, b);
		_target->setColorMod(color);
	}
}
TintBy *TintBy::clone() const
{
	return TintBy::create(_duration, _deltaR, _deltaG, _deltaB);
}
TintBy *TintBy::reverse() const
{
	return TintBy::create(_duration, -_deltaR, -_deltaG, -_deltaB);
}

DelayTime *DelayTime::create(float duration)
{
	auto delaytime = new DelayTime();
	if (delaytime && delaytime->initWithDuration(duration))
		delaytime->autorelease();
	else
		SDL_SAFE_DELETE(delaytime);
	return delaytime;
}
void DelayTime::update(float time)
{
	;
}
DelayTime *DelayTime::clone() const
{
	return DelayTime::create(_duration);
}
DelayTime *DelayTime::reverse() const
{
	return DelayTime::create(_duration);
}

Sequence::Sequence()
	: _split(0.f), _last(-1)
{
	memset(_actions, 0, sizeof(_actions));
}
Sequence::~Sequence()
{
	SDL_SAFE_RELEASE(_actions[0]);
	SDL_SAFE_RELEASE(_actions[1]);
}
FiniteTimeAction *Sequence::create(FiniteTimeAction *action1, ...)
{
	va_list params;
	va_start(params, action1);
	FiniteTimeAction *pNow = nullptr;
	FiniteTimeAction *pPrev = action1;

	if (action1)
	{
		pNow = va_arg(params, FiniteTimeAction *);
		while (pNow)
		{
			pPrev = createWithTwoActions(pPrev, pNow);
			pNow = va_arg(params, FiniteTimeAction *);
		}
	}
	va_end(params);
	return pPrev;
}

FiniteTimeAction *Sequence::create(const std::vector<FiniteTimeAction *> &actions)
{
	auto iter = actions.begin();

	FiniteTimeAction *action1 = *iter++;

	FiniteTimeAction *pNow = nullptr;
	FiniteTimeAction *pPrev = action1;

	if (action1)
	{
		for (; iter != actions.end(); iter++)
		{
			pNow = *iter;
			pPrev = createWithTwoActions(pPrev, pNow);
		}
	}

	return pPrev;
}

Sequence *Sequence::createWithTwoActions(FiniteTimeAction *action1, FiniteTimeAction *action2)
{
	auto sequence = new Sequence();
	if (sequence && sequence->initWithTwoActions(action1, action2))
		sequence->autorelease();
	else
		SDL_SAFE_DELETE(sequence);
	return sequence;
}
bool Sequence::initWithTwoActions(FiniteTimeAction *action1, FiniteTimeAction *action2)
{
	SDLASSERT(action1 != nullptr && action2 != nullptr, "");
	float duration = action1->getDuration() + action2->getDuration();
	if (ActionInterval::initWithDuration(duration))
	{
		_actions[0] = action1;
		action1->retain();
		_actions[1] = action2;
		action2->retain();
		return true;
	}
	return false;
}
void Sequence::startWithTarget(Node *target)
{
	ActionInterval::startWithTarget(target);
	_split = _actions[0]->getDuration() / _duration;
	_last = -1;
}
void Sequence::stop()
{
	if (_last != -1)
	{
		_actions[_last]->stop();
	}
	ActionInterval::stop();
}
void Sequence::update(float time)
{
	int found = 0;

	float new_t = 0.f;

	if (time < _split)
	{
		found = 0;

		if (_split != 0)
			new_t = time / _split;
		else
			new_t = 1;
	}
	else
	{

		found = 1;

		if (_split == 1)
			new_t = 1;
		else
			new_t = (time - _split) / (1 - _split);
	}

	if (found == 1)
	{

		if (_last == -1)
		{

			_actions[0]->startWithTarget(_target);
			_actions[0]->update(1.f);
			_actions[0]->stop();
		}
		else if (_last == 0)
		{
			_actions[0]->update(1.f);
			_actions[0]->stop();
		}
	}

	if (found == _last && _actions[found]->isDone())
	{
		return;
	}

	if (found != _last)
	{
		_actions[found]->startWithTarget(_target);
	}

	_actions[found]->update(new_t);

	_last = found;
}
Sequence *Sequence::clone() const
{
	return nullptr;
}
Sequence *Sequence::reverse() const
{
	return nullptr;
}

Spawn::~Spawn()
{
	SDL_SAFE_RELEASE(_actionOne);
	SDL_SAFE_RELEASE(_actionTwo);
}
FiniteTimeAction *Spawn::create(FiniteTimeAction *action1, ...)
{
	va_list list;
	va_start(list, action1);
	FiniteTimeAction *pPrev = action1;
	FiniteTimeAction *pNow = nullptr;
	if (action1)
	{
		pNow = va_arg(list, FiniteTimeAction *);
		while (pNow)
		{
			pPrev = Spawn::createWithTwoActions(pPrev, pNow);
			pNow = va_arg(list, FiniteTimeAction *);
		}
	}
	va_end(list);
	return pPrev;
}
Spawn *Spawn::createWithTwoActions(FiniteTimeAction *action1, FiniteTimeAction *action2)
{
	auto spawn = new Spawn();
	if (spawn && spawn->initWithTwoActions(action1, action2))
		spawn->autorelease();
	else
		SDL_SAFE_DELETE(spawn);
	return spawn;
}
bool Spawn::initWithTwoActions(FiniteTimeAction *action1, FiniteTimeAction *action2)
{
	SDLASSERT(action1 != nullptr && action2 != nullptr, "");
	float duration1 = action1->getDuration();
	float duration2 = action2->getDuration();

	if (ActionInterval::initWithDuration(MAX(duration1, duration2)))
	{
		_actionOne = action1;
		_actionTwo = action2;

		if (duration1 > duration2)
		{
			_actionTwo = Sequence::createWithTwoActions(_actionTwo, DelayTime::create(duration1 - duration2));
		}
		else if (duration1 < duration2)
		{
			_actionOne = Sequence::createWithTwoActions(_actionOne, DelayTime::create(duration2 - duration1));
		}

		_actionOne->retain();
		_actionTwo->retain();
		return true;
	}
	return false;
}
void Spawn::startWithTarget(Node *target)
{
	ActionInterval::startWithTarget(target);
	_actionOne->startWithTarget(target);
	_actionTwo->startWithTarget(target);
}
void Spawn::stop()
{
	_actionOne->stop();
	_actionTwo->stop();
	ActionInterval::stop();
}
void Spawn::update(float time)
{
	_actionOne->update(time);
	_actionTwo->update(time);
}
Spawn *Spawn::clone() const
{
	return nullptr;
}
Spawn *Spawn::reverse() const
{
	return nullptr;
}

JumpBy *JumpBy::create(float duration, const Point &position, float height, int jumps)
{
	auto jump = new JumpBy();
	if (jump && jump->initWithDuration(duration, position, height, jumps))
		jump->autorelease();
	else
		SDL_SAFE_DELETE(jump);
	return jump;
}
bool JumpBy::initWithDuration(float duration, const Point &position, float height, int jumps)
{
	if (ActionInterval::initWithDuration(duration))
	{
		_deltaPosition = position;
		_height = height;
		_jumps = jumps;
		return true;
	}
	return false;
}
void JumpBy::startWithTarget(Node *target)
{
	ActionInterval::startWithTarget(target);
	_startPosition = target->getPosition();
}
void JumpBy::update(float time)
{
	if (_target)
	{

		float frac = fmodf(time * _jumps, 1.f);

		float y = _height * 4 * frac * (1 - frac);
		y += _deltaPosition.y * time;
		float x = _deltaPosition.x * time;
		_target->setPosition(Point(_startPosition.x + x, _startPosition.y + y));
	}
}
JumpBy *JumpBy::clone() const
{
	return JumpBy::create(_duration, _deltaPosition, _height, _jumps);
}
JumpBy *JumpBy::reverse() const
{
	return JumpBy::create(_duration, Point::ZERO - _deltaPosition, _height, _jumps);
}

JumpTo *JumpTo::create(float duration, const Point &position, float height, int jumps)
{
	auto jump = new JumpTo();
	if (jump && jump->initWithDuration(duration, position, height, jumps))
		jump->autorelease();
	else
		SDL_SAFE_DELETE(jump);
	return jump;
}

void JumpTo::startWithTarget(Node *target)
{
	ActionInterval::startWithTarget(target);
	_startPosition = target->getPosition();

	_deltaPosition -= _startPosition;
}
JumpTo *JumpTo::clone() const
{
	return JumpTo::create(_duration, _startPosition + _deltaPosition, _height, _jumps);
}
JumpTo *JumpTo::reverse() const
{
	return JumpTo::create(_duration, _startPosition - _deltaPosition, _height, _jumps);
}

Repeat::Repeat()
	: _totalTimes(0), _curTimes(-1), _nextDt(0), _bActionInstant(false), _pInnerAction(nullptr)
{
}
Repeat::~Repeat()
{
	SDL_SAFE_RELEASE(_pInnerAction);
}
Repeat *Repeat::create(FiniteTimeAction *action, unsigned int times)
{
	auto repeat = new Repeat();
	if (repeat && repeat->initWithAction(action, times))
		repeat->autorelease();
	else
		SDL_SAFE_DELETE(repeat);
	return repeat;
}
bool Repeat::initWithAction(FiniteTimeAction *action, unsigned int times)
{
	SDLASSERT(action != nullptr, "");
	float d = times * action->getDuration();
	if (ActionInterval::initWithDuration(d))
	{
		_totalTimes = times;
		action->retain();
		_pInnerAction = action;

		_bActionInstant = dynamic_cast<ActionInstant *>(action) == nullptr ? true : false;
		if (_bActionInstant)
		{
			_totalTimes -= 1;
		}
		_curTimes = 0;
		return true;
	}
	return false;
}
bool Repeat::isDone() const
{
	return _curTimes == _totalTimes;
}
void Repeat::startWithTarget(Node *target)
{
	ActionInterval::startWithTarget(target);
	_nextDt = _pInnerAction->getDuration() / _duration;
	_curTimes = 0;
	_pInnerAction->startWithTarget(target);
}
void Repeat::update(float dt)
{

	if (dt > _nextDt)
	{

		while (dt > _nextDt && _curTimes < _totalTimes)
		{
			_curTimes++;
			_pInnerAction->update(1.f);
			_pInnerAction->stop();
			_pInnerAction->startWithTarget(_target);

			_nextDt += _pInnerAction->getDuration() / _duration;
		}
		//
		if (dt >= 1.f && _curTimes < _totalTimes)
		{
			_curTimes++;
		}

		if (!_bActionInstant)
		{

			if (_curTimes == _totalTimes)
			{
				_pInnerAction->update(1.f);
				_pInnerAction->stop();
			}
			else
			{
				_pInnerAction->update(dt - (_nextDt - _pInnerAction->getDuration() / _duration));
			}
		}
	}
	else
		_pInnerAction->update(fmodf(dt * _totalTimes, 1.0));
}
void Repeat::stop()
{
	_pInnerAction->stop();
	ActionInterval::stop();
}
Repeat *Repeat::clone() const
{
	return Repeat::create(_pInnerAction->clone(), _totalTimes);
}
Repeat *Repeat::reverse() const
{
	return Repeat::create(_pInnerAction->reverse(), _totalTimes);
}

RepeatForever *RepeatForever::create(ActionInterval *action)
{
	auto repeat = new RepeatForever();
	if (repeat && repeat->initWithAction(action))
		repeat->autorelease();
	else
		SDL_SAFE_DELETE(repeat);
	return repeat;
}
bool RepeatForever::initWithAction(ActionInterval *action)
{
	SDLASSERT(action != nullptr, "action should not nullptr");
	_pInnerAction = action;
	action->retain();
	return true;
}
void RepeatForever::startWithTarget(Node *target)
{
	ActionInterval::startWithTarget(target);
	_pInnerAction->startWithTarget(target);
}
void RepeatForever::step(float dt)
{
	_pInnerAction->step(dt);

	if (_pInnerAction->isDone())
	{
		float diff = _pInnerAction->getElapsed() - _pInnerAction->getDuration();
		_pInnerAction->startWithTarget(_target);

		_pInnerAction->step(0);

		_pInnerAction->step(diff);
	}
}
RepeatForever *RepeatForever::clone() const
{
	auto action = RepeatForever::create(_pInnerAction->clone());

	action->setTag(this->getTag());

	return action;
}
RepeatForever *RepeatForever::reverse() const
{
	return RepeatForever::create(_pInnerAction->reverse());
}

static inline float bezierat(float a, float b, float c, float d, float t)
{
	return (powf(1 - t, 3) * a +
			3 * t * (powf(1 - t, 2)) * b +
			3 * powf(t, 2) * (1 - t) * c +
			powf(t, 3) * d);
}

BezierBy *BezierBy::create(float d, const ccBezierConfig &config)
{
	auto bezier = new BezierBy();
	if (bezier && bezier->initWithDuration(d, config))
		bezier->autorelease();
	else
		SDL_SAFE_DELETE(bezier);
	return bezier;
}
bool BezierBy::initWithDuration(float d, const ccBezierConfig &config)
{
	if (ActionInterval::initWithDuration(d))
	{
		_config = config;
		return true;
	}
	return false;
}
void BezierBy::startWithTarget(Node *target)
{
	ActionInterval::startWithTarget(target);
	_previousPosition = _startPosition = target->getPosition();
}
void BezierBy::update(float dt)
{
	if (_target)
	{
		float xa = 0;
		float xb = _config.controlPoint_1.x;
		float xc = _config.controlPoint_2.x;
		float xd = _config.endPosition.x;

		float ya = 0;
		float yb = _config.controlPoint_1.y;
		float yc = _config.controlPoint_2.y;
		float yd = _config.endPosition.y;

		float x = bezierat(xa, xb, xc, xd, dt);
		float y = bezierat(ya, yb, yc, yd, dt);

		_target->setPosition(_startPosition + Point(x, y));
	}
}
BezierBy *BezierBy::clone() const
{
	return BezierBy::create(_duration, _config);
}
BezierBy *BezierBy::reverse() const
{
	ccBezierConfig r;

	r.endPosition = Point::ZERO - _config.endPosition;
	r.controlPoint_1 = _config.controlPoint_2 - _config.endPosition;
	r.controlPoint_2 = _config.controlPoint_1 - _config.endPosition;

	BezierBy *action = BezierBy::create(_duration, r);
	return action;
}

BezierTo *BezierTo::create(float d, const ccBezierConfig &config)
{
	auto bezier = new BezierTo();
	if (bezier && bezier->initWithDuration(d, config))
		bezier->autorelease();
	else
		SDL_SAFE_DELETE(bezier);
	return bezier;
}
bool BezierTo::initWithDuration(float d, const ccBezierConfig &config)
{
	if (ActionInterval::initWithDuration(d))
	{
		_toConfig = config;
		return true;
	}
	return false;
}
void BezierTo::startWithTarget(Node *target)
{
	BezierBy::startWithTarget(target);
	_config.controlPoint_1 = _toConfig.controlPoint_1 - _startPosition;
	_config.controlPoint_2 = _toConfig.controlPoint_2 - _startPosition;
	_config.endPosition = _toConfig.endPosition - _startPosition;
}
BezierTo *BezierTo::clone() const
{
	return BezierTo::create(_duration, _toConfig);
}
BezierTo *BezierTo::reverse() const
{
	return nullptr;
}
NS_SDL_END
