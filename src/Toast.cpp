#include "Toast.h"
namespace Toast
{
	void makeSprite(Node*parent,Sprite*sprite,float delay)
	{
		//创建动画
		FadeIn*fadeIn = FadeIn::create(0.2f);
		FadeOut*fadeOut = FadeOut::create(0.2f);
		DelayTime*delayTime = DelayTime::create(delay);
		RemoveSelf*removeSelf = RemoveSelf::create();
		//运行动画
		sprite->runAction(Sequence::create(fadeIn,delayTime,fadeOut,removeSelf,nullptr));
		//设置位置
		Size visibleSize = Director::getInstance()->getVisibleSize();
		sprite->setPosition(visibleSize.width/2,visibleSize.height/4*3);
		//添加到parent结点
		parent->addChild(sprite);
	}
}