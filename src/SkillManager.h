#ifndef __SkillManager_H__
#define __SkillManager_H__
#include<map>
#include<vector>
#include "SG_Engine.h"
using namespace std;
using namespace SDL;

class Skill;

class SkillManager
{
private:
	map<string,Skill* > m_skilles;
public:
	SkillManager();
	~SkillManager();

	bool init();

	Skill*getDefaultSkill(const string&skillName);
};
#endif