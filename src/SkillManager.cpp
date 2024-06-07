#include "SkillManager.h"
#include "Skill.h"

SkillManager::SkillManager()
{
}

SkillManager::~SkillManager()
{
	for(auto iter = m_skilles.begin();iter != m_skilles.end();)
	{
		auto skill = iter->second;
		delete skill;

		iter = m_skilles.erase(iter);
	}
}

bool SkillManager::init()
{
	Skill*skill1 = new SkillNothing(0.7f,3.f);
	Skill*skill2 = new SkillSpeedUp(0.4,3.f);
	Skill*skill3 = new SkillScared(1.f);
	Skill*skill4 = new SkillInvulnerable(1.f,4.f);

	m_skilles.insert(make_pair("SkillNothing",skill1));
	m_skilles.insert(make_pair("SkillSpeedUp",skill2));
	m_skilles.insert(make_pair("SkillScared",skill3));
	m_skilles.insert(make_pair("SkillInvulnerable",skill4));

	return true;
}

Skill*SkillManager::getDefaultSkill(const string&skillName)
{
	auto iter = m_skilles.find(skillName);

	SDL_assert(iter != m_skilles.end());

	return iter->second;
}