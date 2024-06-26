#ifndef __SG_Engine_H__
#define __SG_Engine_H__

#include "SG_Engine/SEObject.h"
#include "SG_Engine/SEAutoreleasePool.h"
#include "SG_Engine/SEPoolManager.h"
#include "SG_Engine/SEWindow.h"
#include "SG_Engine/SESurface.h"
#include "SG_Engine/SEFont.h"
#include "SG_Engine/SERenderer.h"
#include "SG_Engine/SETexture.h"
#include "SG_Engine/SESprite.h"
#include "SG_Engine/SEScale9Sprite.h"
#include "SG_Engine/SEDirector.h"
#include "SG_Engine/SEScene.h"
#include "SG_Engine/SELayer.h"
#include "SG_Engine/SENode.h"
#include "SG_Engine/SEClipNode.h"
#include "SG_Engine/SEInputHandler.h"
#include "SG_Engine/SEEventListenerKeyboard.h"
#include "SG_Engine/SEActionInstant.h"
#include "SG_Engine/SEActionInterval.h"
#include "SG_Engine/SEActionEase.h"
#include "SG_Engine/SEActionManager.h"
#include "SG_Engine/SEFileUtils.h"
#include "SG_Engine/SETextureCache.h"
#include "SG_Engine/SESpriteFrame.h"
#include "SG_Engine/SEAnimation.h"
#include "SG_Engine/SEAnimationFrame.h"
#include "SG_Engine/SESpriteFrameCache.h"
#include "SG_Engine/SEEventDispatcher.h"
#include "SG_Engine/SEEventListenerTouchOneByOne.h"
#include "SG_Engine/SEEventListenerMouse.h"
#include "SG_Engine/SEEventListenerCustom.h"
#include "SG_Engine/SEEventCustom.h"
#include "SG_Engine/SEEventListenerAllAtOnce.h"
#include "SG_Engine/SEEventListenerKeyboardState.h"
#include "SG_Engine/SEEventListenerTextInput.h"
#include "SG_Engine/SERenderCommand.h"

#include "SG_Engine/SETouch.h"
#include "SG_Engine/SEMenu.h"
#include "SG_Engine/SEMenuItem.h"
#include "SG_Engine/SELabelTTF.h"
#include "SG_Engine/SELabelAtlas.h"
#include "SG_Engine/SELabelDotChar.h"
#include "SG_Engine/SEBMFontConfiguration.h"
#include "SG_Engine/SELabelBMFont.h"

#include "SG_Engine/SENotificationCenter.h"
#include "SG_Engine/SELoadingBar.h"
#include "SG_Engine/SEProgressTimer.h"
#include "SG_Engine/SEScheduler.h"
#include "SG_Engine/SEValue.h"
#include "SG_Engine/SEUserDefault.h"
#include "SG_Engine/SEStringUtils.h"
#include "SG_Engine/SELayerColor.h"
#include "SG_Engine/SESoundManager.h"
#include "SG_Engine/SEMusic.h"
#include "SG_Engine/SEChunk.h"

#include "SG_Engine/extensions/SDL_PxString.h"
#include "SG_Engine/extensions/SDL_gfx_mod.h"

#include "SG_Engine/ui/UIWidgetData.h"
#include "SG_Engine/ui/UIWidgetDataSetting.h"
#include "SG_Engine/ui/UIWidgetFactory.h"
#include "SG_Engine/ui/UIWidgetManager.h"
#include "SG_Engine/ui/UIWidgetSystem.h"
#include "SG_Engine/ui/UIButton.h"
#include "SG_Engine/ui/UICheckBox.h"
#include "SG_Engine/ui/UIRadioButton.h"
#include "SG_Engine/ui/UIRadioButtonGroup.h"
#include "SG_Engine/ui/UIEditBox.h"
#include "SG_Engine/ui/UISlider.h"

#include "SG_Engine/SEBase64.h"

#include "SG_Engine/SETileset.h"
#include "SG_Engine/SETMXLayer.h"
#include "SG_Engine/SETMXObjectGroup.h"
#include "SG_Engine/SETMXTiledMap.h"
#include "SG_Engine/SETMXImageLayer.h"

#include "SG_Engine/SEFastTiledMap.h"
#include "SG_Engine/SEFastLayer.h"

#include "SG_Engine/SESDLNS.h"
#include "SG_Engine/SEAnimationCache.h"
#include "SG_Engine/SEFontCache.h"

#include "rapidxml/rapidxml.hpp"
#include "rapidxml/rapidxml_utils.hpp"
#include "rapidxml/rapidxml_print.hpp"
#endif
