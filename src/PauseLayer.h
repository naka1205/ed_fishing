#ifndef __PauseLayer_H__
#define __PauseLayer_H__
#include "SG_Engine.h"
using namespace SDL;
class PauseLayerDelegate
{
public:
	virtual ~PauseLayerDelegate(){}
	virtual void continueGame()=0;
	virtual void turnStartLayer()=0;
	virtual void chooseTollgate()=0;
	virtual void operateMusic()=0;
	virtual void operateChunk()=0;
};
class PauseLayer:public Layer
{
private:
	ui::Button*m_pContinueBtn;
	ui::Button*m_pMainBtn;
	ui::Button*m_pTollgateBtn;
	ui::CheckBox*m_pMusicBtn;
	ui::CheckBox*m_pChunkBtn;

	PauseLayerDelegate*m_pDelegate;
public:
	PauseLayer();
	~PauseLayer();
	CREATE_FUNC(PauseLayer);
	bool init();
	void updateUI();
public:
	void setDelegate(PauseLayerDelegate*pDelegate);
private:
	void continueBtnCallback(Object*obj);
	void mainBtnCallback(Object*obj);
	void tollgateBtnCallback(Object*sender);
	void musicBtnCallback(Object*sender);
	void chunkBtnCallback(Object*sender);
};
#endif