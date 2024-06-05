#ifndef __SDL_Slider_H__
#define __SDL_Slider_H__

#include <string>
#include "SG_Engine/ui/UIWidget.h"
#include "SG_Engine/SEPoint.h"

NS_SDL_BEGIN
class Scale9Sprite;
class Sprite;

namespace ui
{

	class Slider : public Widget
	{
	public:
		enum class EventType
		{
			ON_PERCENTAGE_CHANGED,

			ON_SLIDEBALL_DOWN,
			ON_SLIDEBALL_UP,
			ON_SLIDEBALL_CANCEL,
		};
		typedef std::function<void(Slider *, EventType)> ccSliderCallback;

		Slider();
		virtual ~Slider();

		static Slider *create();

		static Slider *create(Scale9Sprite *bar, Sprite *slidBallNormal);

		bool init();
		bool init(Scale9Sprite *bar, Sprite *slidBallNormal);

		void loadBarSprite(Scale9Sprite *bar);

		void loadProgressBarSprite(Scale9Sprite *progressBar);

		void loadSlidBallSpriteNormal(Sprite *slidBallNormalSprite);

		void loadSlidBallSpritePressed(Sprite *slidBallPressedSprite);

		void loadSlidBallSpriteDisabled(Sprite *slidBallDisabledSprite);

		void addEventListener(const ccSliderCallback &callback);

		float getPercent() const;

		void setPercent(float value);

		float getMaxPercent() const;

		void setMaxPercent(float percent);

	protected:
		virtual bool touchBeganHook(Touch *touch, SDL_Event *event);
		virtual void touchMovedInHook(Touch *touch, SDL_Event *event);
		virtual void touchMovedOutHook(Touch *touch, SDL_Event *event);
		virtual void touchEndedHook(Touch *touch, SDL_Event *event);
		virtual void touchCancelledHook(Touch *touch, SDL_Event *event);

	private:
		// 更新显示
		void updatePercent();

	protected:
		Scale9Sprite *_bar;
		Scale9Sprite *_progressBar;

		Sprite *_slidBallNormalSprite;
		Sprite *_slidBallPressedSprite;
		Sprite *_slidBallDisabledSprite;

		float _barLength;
		float _percent;
		// default is 100
		float _maxPercent;

		ccSliderCallback _callback;
		// touchBeganHook点击点
		Point _startPos;
	};
} // end namespace ui
NS_SDL_END
#endif
