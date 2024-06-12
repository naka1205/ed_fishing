#ifndef __ScrollLabel_H__
#define __ScrollLabel_H__
#include <vector>
#include "SG_Engine.h"
using namespace std;
using namespace SDL;

class ScrollLabel : public Node
{
private:
	int m_nCurIndex;
	int m_nTargetIndex;
	bool m_bDown;
	Sprite *m_pCurSprite;
	Sprite *m_pNextSprite;

	vector<Sprite *> m_sprites;

	bool m_bUpdated;
	float m_speed;

public:
	ScrollLabel();
	~ScrollLabel();

	static ScrollLabel *create(const vector<Sprite *> &vec, const Size &size, int curIndex);
	bool init(const vector<Sprite *> &vec, const Size &size, int curIndex);

	void setIndex(int index, float duration, bool down);

	virtual void update(float dt);

private:
	int getNexLogicIndex(int index);
	Point getNexLogicPos();
};

#endif