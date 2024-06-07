#ifndef __SDL_EditBox_H__
#define __SDL_EditBox_H__

#include <string>
#include "SG_Engine/ui/UIWidget.h"

NS_SDL_BEGIN
class Scale9Sprite;
class Label;
class LabelDotChar;
class EventListenerKeyboard;
class EventListenerTextInput;

namespace ui
{

	class EditBox;
	enum class EditBoxEndAction
	{
		UNKNOWN,
		RETURN,
	};
	class EditBoxDelegate
	{
	public:
		virtual ~EditBoxDelegate() {}

		virtual void editBoxEditingDidBegin(EditBox *) {}

		virtual void editBoxEditingEndWithAction(EditBox *, EditBoxEndAction) {}

		virtual void editBoxTextChanged(EditBox *, const std::string &) {}

		virtual void editBoxReturn(EditBox *) = 0;
	};

	class EditBox : public Widget
	{
	public:
		enum class InputMode
		{
			ANY,
			NUMERIC,
			DECIMAL,
		};
		enum class InputFlag
		{
			SENSITIVE,
			PASSWORD,
		};

	public:
		EditBox();
		~EditBox();

		static EditBox *create(const Size &size, Scale9Sprite *bg, Label *label);
		bool init(const Size &size, Scale9Sprite *bg, Label *label);

		void setTextInputEnabled(bool enabled);
		bool getTextInputEnabled() const;

		const std::string &getText() const;
		void setText(const std::string &text);

		unsigned int getMaxLength() const;
		void setMaxLength(unsigned int len);

		InputMode getInputMode() const;
		void setInputMode(InputMode mode);

		InputFlag getInputFlag() const;
		void setInputFlag(InputFlag flag);

		void setDelegate(EditBoxDelegate *pDelegate);

		Label *getLabel();
		void setLabel(Label *label);

	protected:
		virtual bool hitTest(Touch *touch);

		virtual bool touchBeganHook(Touch *touch, SDL_Event *event);
		virtual void touchMovedInHook(Touch *touch, SDL_Event *event);
		virtual void touchMovedOutHook(Touch *touch, SDL_Event *event);
		virtual void touchEndedHook(Touch *touch, SDL_Event *event);
		virtual void touchCancelledHook(Touch *touch, SDL_Event *event);

	private:
		void onKeyPressed(SDL_Keycode keycode, SDL_Event *event);
		void onTextInput(char text[32], SDL_Event *event);

		void showCursor();
		void hideCursor();
		// 更新光标的位置
		void updateCursor();
		// 判断是否是整数
		bool isNumeric(char c);
		// 判断是否是实数
		bool isDecimal(char c);
		// 设置label显示的字符
		void updateLabelString();

	private:
		Scale9Sprite *_bg;

		// show text
		Label *_label;
		// cursor and the position
		LabelDotChar *_cursor;
		unsigned int _index;

		// max length,default is max
		unsigned int _maxLength;
		std::string _text;
		InputMode _mode;
		InputFlag _flag;
		EditBoxDelegate *_delegate;
		bool _textInputEnabled;
		EventListenerKeyboard *_keyboardListener;
		EventListenerTextInput *_inputListener;
	};
} // end namespace ui
NS_SDL_END
#endif
