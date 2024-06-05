#include "NetGunBullet.h"
#include "StaticData.h"

NetGunBullet::NetGunBullet()
	:m_nLv(0)
{
}
NetGunBullet::~NetGunBullet()
{
}
NetGunBullet*NetGunBullet::create(int lv)
{
	auto bullet = new NetGunBullet();
	if(bullet && bullet->initWithLv(lv))
		bullet->autorelease();
	else
		SDL_SAFE_DELETE(bullet);
	return bullet;
}
bool NetGunBullet::init()
{
	NetGunBullet::initWithLv(1);

	return true;
}

bool NetGunBullet::initWithLv(int lv)
{
	m_nLv = lv;
	//获取贴图格式
	auto spriteFormat = STATIC_DATA_STRING("net_gun_bullet_sprite_format");
	auto spriteName = StringUtils::format(spriteFormat.c_str(),lv);

	//获取贴图
	bool ret = this->bindSpriteWithSpriteFrameName(spriteName);

	return ret;
}

float NetGunBullet::getSpeed()const
{
	return 400;
}

float NetGunBullet::getDistance()const
{
	auto distance = 300;

	distance += (m_nLv - 1)*50;

	return distance;
}
