#ifndef __StartLayer_H__
#define __StartLayer_H__
#include<string>
#include "SG_Engine.h"
using namespace std;
using namespace SDL;

class StartLayerDelegate
{
public:
	virtual ~StartLayerDelegate(){}
	virtual void startGame()=0;
	virtual void helpGame()=0;
	virtual void exitGame()=0;
	virtual void changeMusic()=0;
	virtual void changeSound()=0;
};
class StartLayer:public Layer
{
private:
	ui::Button*m_pStartBtn;
	ui::Button*m_pHelpBtn;
	ui::Button*m_pExitBtn;
	ui::CheckBox*m_pMusicBtn;
	ui::CheckBox*m_pEffectBtn;

	StartLayerDelegate*m_pDelegate;
public:
	StartLayer();
	~StartLayer();
	CREATE_FUNC(StartLayer);
	bool init();
	void setActive(bool active);
	void setDelegate(StartLayerDelegate*pDelegate);
	void updateUI();
private:
	void startGame(Object*);
	void helpGame(Object*sender);
	void exitGame(Object*sender);
	void changeMusic(Object*,ui::CheckBox::EventType type);
	void changeSound(Object*,ui::CheckBox::EventType type);
};
#endif