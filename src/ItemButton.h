#ifndef __ItemButton_H__
#define __ItemButton_H__

#include<string>
#include "SG_Engine.h"

using namespace std;
using namespace SDL;

class NumberSprite;
/*按钮类型*/
enum class ButtonType
{
	None,
	SuperWeapon,//超级武器
	HalfEnergy,//一半的能量值
	Bullet,//子弹
};
class ItemButton:public MenuItem
{
private:
	Sprite*m_pNormalSprite;
	Sprite*m_pDisabelSprite;
	NumberSprite*m_pNumberSprite;
	ButtonType m_buttonType;
public:
	ItemButton();
	~ItemButton();
	CREATE_FUNC(ItemButton);
	static ItemButton*create(Sprite*normalSprite,unsigned int number,ButtonType type);

	bool init();
	bool init(Sprite*normalSprite,unsigned int number,ButtonType type);
	//设置正常按钮
	void setNormalSprite(Sprite*normalSprite);
	Sprite*getNormalSprite()const;
	//设置数
	void setNumber(unsigned int number);
	unsigned int getNumber()const;

	void setButtonType(ButtonType type);
	ButtonType getButtonType()const;
public://override
	virtual void setEnabled(bool enable);
	//override
	virtual void selected();
	virtual void unselected();
private:
	void updateImagesVisibility();
};
#endif