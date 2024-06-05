#ifndef __LevelChooseLayer_H__
#define __LevelChooseLayer_H__
#include<string>
#include "SG_Engine.h"

using namespace std;
using namespace SDL;

class LevelChooseLayerDelegate
{
public:
	virtual ~LevelChooseLayerDelegate(){}
	virtual void closeLevelChooseLevel()=0;
	virtual void enterLoadingLayer() = 0;
};

class LevelChooseLayer:public Layer
{
private:
	ui::Button*m_pCloseBtn;
	ui::Button*m_pLeftBtn;
	ui::Button*m_pRightBtn;
	ui::Button*m_pConfirmBtn;
	LabelAtlas*m_pGoldLabel;
	LevelChooseLayerDelegate*m_pDelegate;
	Node*m_pPageView;
	int m_nCurIndex;
	int m_nSize;
	bool m_bClickBtn;
public:
	LevelChooseLayer();
	~LevelChooseLayer();
	CREATE_FUNC(LevelChooseLayer);
	bool init();
	void setDelegate(LevelChooseLayerDelegate*pDelegate);
	int getTollgateIndex()const;
	void updateUI();
private:
	void movePage(int nextPage,float duration);
	void confirmBtnCallback(Object*sender);
	void closeThePage(Object*sender);
	void leftBtnCallback(Object*sender);
	void rightBtnCallback(Object*sender);
};
#endif