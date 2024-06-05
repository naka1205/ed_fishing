#ifndef __NumberSprite_H__
#define __NumberSprite_H__
#include<string>
#include "SG_Engine.h"
using namespace std;
using namespace SDL;

class NumberSprite:public Node
{
private:
	unsigned int m_nNumber;
	Sprite*m_pSprite;
	LabelAtlas*m_pLabel;
public:
	NumberSprite();
	~NumberSprite();
	static NumberSprite*create(unsigned int number);
	bool init(unsigned int number);

	void setNumber(unsigned int number);
	unsigned int getNumber()const;
private:
	void handleSprite();
	void handleLabel();
};
#endif