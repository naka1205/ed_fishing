#ifndef __ScoreSprite_H__
#define __ScoreSprite_H__
#include <string>
#include "SG_Engine.h"
using namespace std;
using namespace SDL;

class ScoreSprite : public Node
{
private:
	Sprite *m_pPrizeSign;		 // 乘号精灵
	LabelAtlas *m_pPrizeNum; // 数字精灵
public:
	ScoreSprite();
	~ScoreSprite();

	static ScoreSprite *create(int score);

	bool init(int score);

	float runFadeOutAction();

	void setScore(int score);
};
#endif