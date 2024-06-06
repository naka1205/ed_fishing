#ifndef __LevelLayer_H__
#define __LevelLayer_H__
#include <vector>
#include <string>

#include "SG_Engine.h"

using namespace SDL;

class LevelLayer : public SDL::Layer
{
	SDL_SYNTHESIZE(int, m_nRatio, Ratio); // 当前关卡的倍率
	SDL_SYNTHESIZE(float, m_fTax, Tax);	  // 当前税值
private:
	std::vector<int> m_netGunLevelIDs; // 网枪id集合
	std::vector<int> m_fishIDs;		   // 鱼的id集合
	std::vector<int> m_fishTideIDs;	   // 鱼潮的id集合
	std::vector<float> m_starFishStartTimes;

	TMXTiledMap *m_pTiledMap;

public:
	LevelLayer();
	~LevelLayer();
	static LevelLayer *create(const std::string &levelName);
	bool initWithLevelName(const std::string &levelName);

	std::vector<int> &getShownFishIdVec();
	std::vector<int> &getNetGunBulletVec();
	std::vector<int> &getFishTideVec();
	std::vector<float> &getStartFishStartTimeVec();

	std::string getBGM();

private:
	void parseFishID();
	void parseNetGun();
	void parseFishTide();
	void parseStarFishStartTime();

	template <typename T>
	void parse(const std::string &src, const std::string &token, std::vector<T> &vect);
};
#endif