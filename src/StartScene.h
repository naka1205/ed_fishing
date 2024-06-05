#ifndef __StartScene_H__
#define __StartScene_H__
#include<string>
#include "SG_Engine.h"
#include "StartLayer.h"
#include "LevelChooseLayer.h"
#include "HelpLayer.h"

using namespace std;
using namespace SDL;

class StartScene:public Scene
	,public StartLayerDelegate,public LevelChooseLayerDelegate
	,public HelpLayerDelegate
{
private:
	StartLayer*m_pStartLayer;
	LevelChooseLayer*m_pLevelChooseLayer;
	HelpLayer*m_pHelpLayer;
public:
	StartScene();
	~StartScene();
	CREATE_FUNC(StartScene);
	bool init();
	void enterTollgateLayer();
private:
	void startGame();
	void helpGame();
	void exitGame();

	void changeMusic();
	void changeSound();
	void asyncLoadMusicCallback(Object*sound,const std::string&type);
private://delegate
	virtual void closeLevelChooseLevel();
	virtual void closeHelpLayer();
	//进入加载界面
	virtual void enterLoadingLayer();
};
#endif