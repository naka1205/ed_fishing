#ifndef __GoldCounter_H__
#define __GoldCounter_H__
#include <cmath>
#include <vector>
#include "SG_Engine.h"

using namespace std;
using namespace SDL;
class ScrollLabel;

class GoldCounter : public ClipNode
{
private:
	vector<ScrollLabel *> m_labels;
	int m_nNumber;

public:
	GoldCounter();
	~GoldCounter();
	static GoldCounter *create(int number);
	bool init(int number);
	virtual void update(float dt);


	void setGoldNumber(int number);
};
#endif