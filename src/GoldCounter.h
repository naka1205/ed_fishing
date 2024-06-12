#ifndef __GoldCounter_H__
#define __GoldCounter_H__
#include <cmath>
#include <vector>
#include "SG_Engine.h"

using namespace std;
using namespace SDL;
class ScrollLabel;

class GoldCounter : public Node
{
private:
	vector<ScrollLabel *> m_labels;
	int m_nNumber;

	CustomCommand beforeCommand;
	CustomCommand afterCommand;

public:
	GoldCounter();
	~GoldCounter();
	static GoldCounter *create(int number);
	bool init(int number);
	virtual void update(float dt);
	virtual void visit();
	virtual void beforeVisit();
	virtual void afterVisit();

	void setGoldNumber(int number);
};
#endif