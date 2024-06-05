
#include "GameScene.h"
#include "FishingJoyData.h"
#include "LevelLayer.h"
#include "StaticData.h"
#include "GB2ShapeCacheX.h"
#include "B2EntityFactory.h"
#include "NetGunBullet.h"
#include "FishingNet.h"
#include "EffectLayer.h"
#include "Fish.h"
#include "Bullet.h"
#include "ItemButton.h"
#include "StartScene.h"

#include "SDL2/SDL.h"
b2World *GameScene::s_pWorld = nullptr;

GameScene::GameScene()
		: m_pLevelLayer(nullptr), m_pTouchLayer(nullptr), m_pPanelLayer(nullptr), m_pCannon(nullptr), m_pGoldTimer(nullptr), m_pBulletLayer(nullptr), m_pFishingEntityLayer(nullptr), m_pFishLayer(nullptr), m_pRewardLayer(nullptr), m_pEffectLayer(nullptr), m_pPauseLayer(nullptr)
{
}
GameScene::~GameScene()
{
	B2EntityFactory::getInstance()->purge();
	SDL_SAFE_DELETE(s_pWorld);
}
bool GameScene::init()
{
	Size visibleSize = Director::getInstance()->getVisibleSize();
	// 初始化物理世界
	b2Vec2 gravity(0.f, 0.f);
	s_pWorld = new b2World(gravity);
	s_pWorld->SetAllowSleeping(true);			// 允许休眠
	s_pWorld->SetContinuousPhysics(true); // 连续物理测试
	s_pWorld->SetContactListener(this);
	// 预加载 todo
	this->preloadResources();
	// 关卡层
	int tollgateIndex = FishingJoyData::getInstance()->getTollgateIndex();
	auto tollgateName = StringUtils::format("level/level%d.tmx", tollgateIndex + 1);
	m_pLevelLayer = LevelLayer::create(tollgateName);
	this->addChild(m_pLevelLayer);
	// ui
	m_pPanelLayer = PanelLayer::create();
	this->addChild(m_pPanelLayer);
	m_pPanelLayer->setDelegate(this);
	// 事件层
	m_pTouchLayer = TouchLayer::create();
	this->addChild(m_pTouchLayer);
	m_pTouchLayer->setDelegate(this);

	// 鱼层
	m_pFishLayer = FishLayer::create();
	m_pFishLayer->setDelegate(this);
	m_pFishLayer->reset();
	this->addChild(m_pFishLayer);
	// 网枪弹层
	m_pBulletLayer = BulletLayer::create();
	m_pBulletLayer->setDelegate(this);
	this->addChild(m_pBulletLayer);
	// 渔网层
	m_pFishingEntityLayer = FishingEntityLayer::create();
	m_pFishingEntityLayer->setDelegate(this);
	this->addChild(m_pFishingEntityLayer);
	// 大炮层
	m_pCannon = Cannon::create();
	m_pCannon->setDelegate(this);
	m_pCannon->setNetGunBullets(m_pLevelLayer->getNetGunBulletVec());

	this->addChild(m_pCannon);
	// 设置位置
	m_pCannon->setPosition(m_pPanelLayer->getConnonBornPos());
	// 添加金币计数器
	m_pGoldTimer = GoldTimer::create();
	m_pGoldTimer->setDelegate(this);
	m_pGoldTimer->setPosition(STATIC_DATA_POINT("gold_timer_pos"));

	this->addChild(m_pGoldTimer);
	// 效果层
	m_pEffectLayer = EffectLayer::create();
	this->addChild(m_pEffectLayer);
	// 奖励层
	m_pRewardLayer = RewardLayer::create();
	m_pRewardLayer->setDelegate(this);
	this->addChild(m_pRewardLayer);
	// 暂停层
	m_pPauseLayer = PauseLayer::create();
	m_pPauseLayer->setDelegate(this);
	m_pPauseLayer->updateUI();
	// 设置位置出界
	m_pPauseLayer->setPosition(0, -visibleSize.height);

	this->addChild(m_pPauseLayer);
	// 刷新当前面板显示内容
	m_pPanelLayer->updateSelf();
	// 播放背景音乐
	this->playBackgroundMusic();

	this->scheduleUpdate();

	return true;
}
void GameScene::update(float dt)
{
	// 让shape跟随精灵移动
	const int PTM_RATIO = GB2ShapeCache::getInstance()->getPtmRatio();
	// PhysicalEngine::getInstance()->PTM_RATIO;

	for (b2Body *body = s_pWorld->GetBodyList(); body; body = body->GetNext())
	{
		if (body->GetUserData())
		{
			Entity *node = static_cast<Entity *>(body->GetUserData());
			b2Vec2 position = b2Vec2(node->getPositionX() / PTM_RATIO, node->getPositionY() / PTM_RATIO);
			// 获取角度，角度跟随内部精灵
			auto sprite = node->getSprite();
			// 暂时这样 应该是以y轴翻转
			// todo 在B2Entity重写，根据图片大小重新设置位置
			float angle = SDL_ANGLE_TO_DEGREE(sprite->getRotation() + 180.f);

			body->SetTransform(position, angle);
		}
	}

	// 速度和位置都不进行更正
	s_pWorld->Step(dt, 0, 0);

	m_pGoldTimer->update(dt);
	m_pFishLayer->update(dt);
	m_pBulletLayer->update(dt);
	m_pFishingEntityLayer->update(dt);
}
void GameScene::preloadResources()
{
	// 获取图片资源数组
	auto spriteFrameCache = Director::getInstance()->getSpriteFrameCache();
	ValueVector values = StaticData::getInstance()->getValueForKey("fish_src_filepath").asValueVector();

	for (auto value : values)
	{
		auto filename = value.asString();
		spriteFrameCache->addSpriteFramesWithFile(filename);
	}
	// 加载动画
	AnimationCache::getInstance()->addAnimationsWithFile(STATIC_DATA_STRING("animation_filepath"));
}

void GameScene::purge()
{
	// 删除单例类
	FishingJoyData::purge();
	StaticData::purge();
	GB2ShapeCache::getInstance()->purge();
}

void GameScene::operateAllSchedulerAndActions(Node *node, GameState state)
{
	switch (state)
	{
	case GameState::Pause:
		node->pauseSchedulerAndActions();
		break;
	case GameState::Resume:
		node->resumeSchedlerAndActions();
		break;
	}
	const std::vector<Node *> &children = node->getChildren();

	for (auto child : children)
	{
		this->operateAllSchedulerAndActions(child, state);
	}
}

void GameScene::gamePause()
{
	this->operateAllSchedulerAndActions(m_pBulletLayer, GameState::Pause);
	this->operateAllSchedulerAndActions(m_pFishingEntityLayer, GameState::Pause);
	this->operateAllSchedulerAndActions(m_pFishLayer, GameState::Pause);
	this->operateAllSchedulerAndActions(m_pCannon, GameState::Pause);

	this->unscheduleUpdate();

	m_pTouchLayer->setTouchEnable(false);
	m_pPanelLayer->setBtnEnable(false);
}

void GameScene::gameResume()
{
	this->operateAllSchedulerAndActions(m_pBulletLayer, GameState::Resume);
	this->operateAllSchedulerAndActions(m_pFishingEntityLayer, GameState::Resume);
	this->operateAllSchedulerAndActions(m_pFishLayer, GameState::Resume);
	this->operateAllSchedulerAndActions(m_pCannon, GameState::Resume);

	this->scheduleUpdate();

	m_pTouchLayer->setTouchEnable(true);
	m_pPanelLayer->setBtnEnable(true);
}

void GameScene::playBackgroundMusic()
{
	bool bPlayingMusic = FishingJoyData::getInstance()->isPlayingMusic();

	if (bPlayingMusic)
	{
		auto sBGM = m_pLevelLayer->getBGM();

		SoundManager::getInstance()->playBackgroundMusic(sBGM, -1);
	}
	else if (SoundManager::getInstance()->isBackgroundPlaying())
	{
		SoundManager::getInstance()->stopBackgroundMusic();
	}
}

b2World *GameScene::getWorld()
{
	return s_pWorld;
}

void GameScene::collisionOfBullet(Bullet *bullet, Entity *entity)
{
	// 和Fish发生碰撞
	if (entity->getType() == Entity::Type::Fish)
	{
		Fish *fish = static_cast<Fish *>(entity);
		bullet->collided(fish);
		// 鱼和网枪弹发生碰撞,只有不是Group时才会处理逻辑
		if (m_pFishLayer->getCurState() != FishLayer::State::Group)
			fish->collided(TriggingType::NetGunBullet);
		// 如果和海星碰撞
		if (fish->getFishType() == FishType::StarFish)
		{
			fish->caught();
			// 不生成渔网
			bullet->setMakeFishingEntity(false);
		}
	}
}

void GameScene::collisionOfFishingEntity(FishingEntity *fishingEntity, Entity *entity)
{

	Fish *fish = nullptr;
	// 渔网和鱼发生碰撞
	if (entity->getType() == Entity::Type::Fish)
	{
		fish = static_cast<Fish *>(entity);
	}
	if (fish == nullptr)
		return;

	float scope = 0.f;
	// 如果是激光，那么成功率为1
	if (fishingEntity->getFishingType() == FishingType::Laser)
	{
		scope = 1.f;
	}
	// 此时鱼处于无敌成功率为0
	else if (fish->isInvulnerable())
	{
		scope = 0.f;
	}
	else if (fishingEntity->getFishingType() == FishingType::FishingNet)
	{
		auto fishingNet = static_cast<FishingNet *>(fishingEntity);

		int ratio = m_pLevelLayer->getRatio();
		int gold = STATIC_FISH_GOLD(fish->getID()) * ratio;
		// 损耗值 随关卡不同而不同
		float tax = m_pLevelLayer->getTax();

		scope = fishingNet->getLevel() * (1 - tax) / gold;
	}
	// todo
	else if (fishingEntity->getFishingType() == FishingType::Pearl)
	{
		scope = 0.4f;
	}
	else if (fishingEntity->getFishingType() == FishingType::Fog)
	{
		scope = 0.4f;
	}
	// 发生碰撞
	fishingEntity->collided();

	float success = RANDOM_0_1();
	// 鱼被捕捉成功
	if (success < scope)
	{
		fish->caught();
		// 获取这条鱼的经验值 并增加经验
		auto exp = STATIC_FISH_EXP(fish->getID());
		this->addExp(exp);
		// 只有不是激光才增加能量
		if (fishingEntity->getFishingType() != FishingType::Laser)
		{
			auto energy = STATIC_FISH_ENERGY(fish->getID());
			this->addEnergy(energy);
		}
	}
	// 鱼捕捉失败
	else if (m_pFishLayer->getCurState() != FishLayer::State::Group)
	{
		fish->collided(TriggingType::Fishing);
	}
}

void GameScene::BeginContact(b2Contact *contact)
{
	auto fixtureA = contact->GetFixtureA();
	auto fixtureB = contact->GetFixtureB();

	auto bodyA = fixtureA->GetBody();
	auto bodyB = fixtureB->GetBody();

	B2Entity *entityA = static_cast<B2Entity *>(bodyA->GetUserData());
	B2Entity *entityB = static_cast<B2Entity *>(bodyB->GetUserData());

	// 仅对于鱼 保证每帧仅有一次
	if (!entityA->isCollided() && entityA->getType() == Entity::Type::Fish)
	{
		entityA->setCollided(true);

		if (entityB->getType() == Entity::Type::Fishing)
			this->collisionOfFishingEntity(static_cast<FishingEntity *>(entityB), entityA);
		else if (entityB->getType() == Entity::Type::Bullet)
			this->collisionOfBullet(static_cast<Bullet *>(entityB), entityA);
	}
	else if (!entityB->isCollided() && entityB->getType() == Entity::Type::Fish)
	{
		entityB->setCollided(true);

		if (entityA->getType() == Entity::Type::Fishing)
			this->collisionOfFishingEntity(static_cast<FishingEntity *>(entityA), entityB);
		else if (entityA->getType() == Entity::Type::Bullet)
			this->collisionOfBullet(static_cast<Bullet *>(entityA), entityB);
	}
	else if (entityA->getType() == Entity::Type::Fishing)
	{
		this->collisionOfFishingEntity(static_cast<FishingEntity *>(entityA), entityB);
	}
	else if (entityA->getType() == Entity::Type::Bullet)
	{
		this->collisionOfBullet(static_cast<Bullet *>(entityA), entityB);
	}
}
void GameScene::EndContact(b2Contact *contact)
{
	auto fixtureA = contact->GetFixtureA();
	auto fixtureB = contact->GetFixtureB();

	auto bodyA = fixtureA->GetBody();
	auto bodyB = fixtureB->GetBody();

	B2Entity *entityA = static_cast<B2Entity *>(bodyA->GetUserData());
	B2Entity *entityB = static_cast<B2Entity *>(bodyB->GetUserData());

	entityA->setCollided(false);
	entityB->setCollided(false);
}

void GameScene::addCannonLevel()
{
	if (m_pCannon->isCanFire())
	{
		m_pCannon->addLevel();

		bool bPlayingChunk = FishingJoyData::getInstance()->isPlayingChunk();

		if (bPlayingChunk)
		{
			auto sChangeNetGun = STATIC_DATA_STRING("change_net_gun");
			SoundManager::getInstance()->playEffect(sChangeNetGun, 0);
		}
	}
}

void GameScene::subCannonLevel()
{
	if (m_pCannon->isCanFire())
	{
		m_pCannon->subLevel();

		bool bPlayingChunk = FishingJoyData::getInstance()->isPlayingChunk();

		if (bPlayingChunk)
		{
			auto sChangeNetGun = STATIC_DATA_STRING("change_net_gun");
			SoundManager::getInstance()->playEffect(sChangeNetGun, 0);
		}
	}
}

void GameScene::pauseBtnCallback()
{
	// 暂停游戏
	this->gamePause();
	// 动画显示选择界面
	Size visibleSize = Director::getInstance()->getVisibleSize();
	MoveBy *move = MoveBy::create(0.5f, Point(0, visibleSize.height));
	EaseExponentialOut *action = EaseExponentialOut::create(move);

	m_pPauseLayer->runAction(action);
}

void GameScene::continueGame()
{
	Size visibleSize = Director::getInstance()->getVisibleSize();
	// 游戏恢复
	this->gameResume();
	// 动画显示选择界面
	MoveBy *move = MoveBy::create(0.5f, Point(0, -visibleSize.height));
	EaseExponentialOut *action = EaseExponentialOut::create(move);

	m_pPauseLayer->runAction(action);
}

void GameScene::turnStartLayer()
{
	StartScene *scene = StartScene::create();

	Director::getInstance()->replaceScene(scene);
}

void GameScene::chooseTollgate()
{
	StartScene *scene = StartScene::create();
	scene->enterTollgateLayer();

	Director::getInstance()->replaceScene(scene);
}

void GameScene::operateMusic()
{
	auto soundManager = SoundManager::getInstance();
	bool playingMusic = FishingJoyData::getInstance()->isPlayingMusic();
	playingMusic = !playingMusic;
	// 回写
	FishingJoyData::getInstance()->setPlayingMusic(playingMusic);
	// 内部进行回调，判断是否播放背景音乐
	this->playBackgroundMusic();
}

void GameScene::operateChunk()
{
	bool bPlayingChunk = FishingJoyData::getInstance()->isPlayingChunk();
	bPlayingChunk = !bPlayingChunk;
	// 回写
	FishingJoyData::getInstance()->setPlayingChunk(bPlayingChunk);
}

void GameScene::changeBullet(ItemButton *button, bool oldVisible, bool visible)
{
	auto selectedBulletName = button->getName();
	auto pos = button->getPosition();

	// todo
	auto spriteName = "bullet";
	// 改变特效位置
	if (oldVisible == visible)
	{
		m_pEffectLayer->changeSelectedMark(spriteName, pos);
		// 设置当前选中的子弹
		FishingJoyData::getInstance()->setSelectedBullet(selectedBulletName);
	}
	else if (visible)
	{
		m_pEffectLayer->addSelectedMark(spriteName, pos);
	}
	else
	{
		m_pEffectLayer->removeSelectedMark(spriteName);
	}
}

bool GameScene::onTouchBegan(Touch *touch, SDL_Event *event)
{
	if (m_pCannon->isCanFire() == false)
		return true;

	auto pos = touch->getLocation();

	m_pCannon->aimAt(pos);

	auto fishingJoyData = FishingJoyData::getInstance();
	// 是否播放音效
	bool bPlayingChunk = fishingJoyData->isPlayingChunk();

	if (!m_pCannon->isCanFire())
		return true;
	// 获取当前选中的子弹名字
	auto bulletName = fishingJoyData->getSelectedBullet();
	// 消耗能量槽
	if (m_pCannon->getType() == Cannon::Type::Laser)
	{
		auto afterEnergy = fishingJoyData->getEnergy() - 100;

		fishingJoyData->setEnergy(afterEnergy);
		m_pPanelLayer->updateEnergy();
		m_pPanelLayer->stopEnergyAnimation();
		// 播放激光武器转换音效
		if (bPlayingChunk)
		{
			auto laserFire = STATIC_DATA_STRING("laser_fire_chunk");
			SoundManager::getInstance()->playEffect(laserFire, 0);
		}
	}
	// 特殊的子弹
	else
	{
		// 获取当前子弹的数目
		auto curBulletNum = fishingJoyData->getBulletNumByName(bulletName);
		// 减少一
		auto afterBulletNum = curBulletNum - 1;
		// 减少对应的子弹的数目
		fishingJoyData->alterSelectedBulletNum(-1);
	}
	// 无论装备什么 都 消耗金币
	if (m_pCannon->isBelongNetBulletType(m_pCannon->getType()))
	{
		// 获取需要的金币
		// 获取当前倍率
		int ratio = m_pLevelLayer->getRatio();
		int needGold = fishingJoyData->getGoldByCannon(m_pCannon) * ratio;
		bool bRet = this->subGold(needGold);
		// 当前的金币不够，则直接退出
		if (!bRet)
			return true;

		if (bPlayingChunk)
		{
			auto netGunShoot = STATIC_DATA_STRING("net_gun_shoot_chunk");
			SoundManager::getInstance()->playEffect(netGunShoot, 0);
		}
	}
	m_pPanelLayer->updateBulletBtns();
	// 播放 发射动画
	m_pCannon->fire(pos);

	return true;
}

void GameScene::onTouchMoved(Touch *touch, SDL_Event *event)
{
	if (m_pCannon->isCanFire() == false)
		return;

	auto pos = touch->getLocation();

	m_pCannon->aimAt(pos);
}

void GameScene::onTouchEnded(Touch *touch, SDL_Event *event)
{
}

void GameScene::fireCallback(const Point &pos)
{
	auto type = m_pCannon->getType();
	auto bornPos = m_pCannon->getPosition();
	auto rotation = m_pCannon->getRotation();
	// 重新确定出生位置
	auto degree = SDL_ANGLE_TO_DEGREE(rotation);
	Size cannonSize = m_pCannon->getContentSize();

	bornPos.x += SDL_sinf(degree) * cannonSize.height / 2;
	bornPos.y -= SDL_cosf(degree) * cannonSize.height / 2;
	// 发射激光
	if (m_pCannon->getType() == Cannon::Type::Laser) // 交给FishingNetLayer
	{
		m_pFishingEntityLayer->addLaser(bornPos, rotation);
	}
	else
	{
		// 获取当前选中的子弹名称
		auto bulletName = FishingJoyData::getInstance()->getSelectedBullet();
		auto netGunBullet = STATIC_DATA_STRING("NetGunBullet");
		auto strengthenBullet = STATIC_DATA_STRING("SterengthenBullet");
		auto fogBullet = STATIC_DATA_STRING("FogBullet");
		auto pearlBullet = STATIC_DATA_STRING("PearlBullet");

		// 如果是渔网弹
		if (bulletName == netGunBullet)
		{
			auto bulletLv = m_pCannon->getNetGunBulletLv();

			m_pBulletLayer->addNetGunBullet(bulletLv, bornPos, pos, rotation);
		}
		// 加强弹
		else if (bulletName == strengthenBullet)
		{
			m_pBulletLayer->addStrengthenBullet(bornPos, pos, rotation);
		}
		// 迷雾弹
		else if (bulletName == fogBullet)
		{
			m_pBulletLayer->addFogBullet(bornPos, pos, rotation);
		}
		// 珍珠弹
		else if (bulletName == pearlBullet)
		{
			m_pFishingEntityLayer->addPearl(bornPos, rotation);
		}

		this->fireEnd();
	}
}

void GameScene::fireEnd()
{
	m_pCannon->fireEnd();
	bool bPlayingChunk = FishingJoyData::getInstance()->isPlayingChunk();

	// 是否切换炮塔
	if (m_pCannon->getType() == Cannon::Type::Laser)
	{
		// 回到上一级状态
		auto oldType = m_pCannon->getLastType();

		m_pCannon->setType(oldType);

		m_pPanelLayer->showAddAndSubBtn();
		// 是否播放音效
		if (bPlayingChunk)
		{
			auto sChangeLaser = STATIC_DATA_STRING("change_laser_chunk");
			SoundManager::getInstance()->playEffect(sChangeLaser, 0);
		}
	}
	auto fishingJoyData = FishingJoyData::getInstance();
	auto bulletNum = fishingJoyData->getSelectedBulletNum();
	// 没有当前炮弹，则自动切换成 网枪弹
	if (bulletNum <= 0)
	{
		auto netGunBullet = STATIC_DATA_STRING("NetGunBullet");
		// PanleLayer 修改贴图 数目 name
		fishingJoyData->setSelectedBullet(netGunBullet);
		m_pPanelLayer->setSelectedBulletBtn(netGunBullet);
	}
}

void GameScene::shootAt(Bullet *bullet, Fish *fish)
{
	switch (bullet->getBulletType())
	{
	case BulletType::NetGunBullet: // 渔网弹
	{
		int lv = static_cast<NetGunBullet *>(bullet)->getLevel();
		this->addFishingNet(lv, bullet->getPosition(), 0);
	}
	break;
	case BulletType::StrengthenBullet: // 强化弹
	{
		// 随机1~7级渔网 todo
		int lv = rand() % 7 + 1;
		// 在终点生成一个渔网
		this->addFishingNet(lv, bullet->getPosition(), 0);
		// 随机生成一个字符串
		auto totalFrames = Director::getInstance()->getTotalFrames();
		auto key = StringUtils::format("strengthen%d", totalFrames);
		// 回调函数
		this->schedule(SDL_CALLBACK_1(GameScene::randomMakeFishingNets, this, lv), key, 0.2f, 8);
	}
	break;
	case BulletType::FogBullet: // 迷雾弹
	{
		int lv = rand() % 7 + 1;
		auto pos = bullet->getPosition();
		// 在终点生成一个渔网
		this->addFishingNet(lv, pos, 0);

		if (fish == nullptr)
			break;
		// 如果和鱼发生碰撞，就生成迷雾
		m_pFishingEntityLayer->addFog(fish, pos, bullet->getRotation());
		// 设置鱼的状态为无敌
		fish->setInvulnerable(true);
		// 设置鱼的死亡回调函数
		auto deadCallback = [](Fish *fish)
		{
			auto uniqueID = StringUtils::toString(fish->getUniqueID());
			Director::getInstance()->getEventDispatcher()->dispatchCustomEvent(uniqueID);
		};
		fish->setDeadCallback(deadCallback);
	}
	break;
	}
}

void GameScene::addGold(int number, const Point &pos)
{
	// 显示金币动画
	m_pEffectLayer->addGoldAnimation(pos);
	// 显示飘动分值
	m_pEffectLayer->addFadeScoreAction(number, pos);
	// 增加金币
	auto fishingJoy = FishingJoyData::getInstance();
	auto gold = fishingJoy->getGold() + number;

	fishingJoy->setGold(gold);
	// 更新panel
	m_pPanelLayer->updateGold();
	// 是否播放音效
	bool bPlayingChunk = FishingJoyData::getInstance()->isPlayingChunk();

	if (bPlayingChunk)
	{
		auto sGetGold = STATIC_DATA_STRING("get_gold_chunk");
		SoundManager::getInstance()->playEffect(sGetGold, 0);
	}
}

void GameScene::showWaveAction()
{
	m_pEffectLayer->showWaveAction();
	// 是否播放音效
	bool bPlayingChunk = FishingJoyData::getInstance()->isPlayingChunk();

	if (bPlayingChunk)
	{
		auto sWaveComing = STATIC_DATA_STRING("wave_coming_chunk");
		SoundManager::getInstance()->playEffect(sWaveComing, 0);
	}
}

void GameScene::addChestLightAnim(const string &name, const Point &pos)
{
	m_pEffectLayer->addChestLightAnim(name, pos);
	// 是否播放音效特效
	bool bPlayingChunk = FishingJoyData::getInstance()->isPlayingChunk();

	if (bPlayingChunk)
	{
		auto sOpenChest = STATIC_DATA_STRING("open_chest_chunk");
		SoundManager::getInstance()->playEffect(sOpenChest, 0);
	}
}

void GameScene::removeChestLightAnim(const string &name)
{
	m_pEffectLayer->removeChestLightAnim(name);
}

bool GameScene::subGold(int number)
{
	auto fishingJoy = FishingJoyData::getInstance();
	auto gold = fishingJoy->getGold() - number;
	bool bRet = false;

	if (gold < 0)
	{
		// todo 提示
		bRet = false;
	}
	else
	{
		fishingJoy->setGold(gold);
		// 更新panel
		m_pPanelLayer->updateGold();

		bRet = true;
	}
	return true;
}

void GameScene::addEnergy(float delta)
{
	auto fishingJoyData = FishingJoyData::getInstance();
	auto curEnergy = fishingJoyData->getEnergy();
	auto afterEnergy = curEnergy + delta;
	bool bPlayingChunk = FishingJoyData::getInstance()->isPlayingChunk();
	// 能量达到100以上
	if (afterEnergy >= 100)
	{
		// 展示能量充满动画
		m_pPanelLayer->runEnergyAnimation();
		m_pPanelLayer->hideAddAndSubBtn();
		// 切换到激光炮
		m_pCannon->setType(Cannon::Type::Laser);

		if (bPlayingChunk)
		{
			auto sChangeLaser = STATIC_DATA_STRING("change_laser_chunk");
			SoundManager::getInstance()->playEffect(sChangeLaser, 0);
		}
	}
	// 设置当前能量
	fishingJoyData->setEnergy(afterEnergy);

	m_pPanelLayer->updateEnergy();
}

void GameScene::addExp(float exp)
{
	// 获取当前等级的总经验值
	auto exps = FishingJoyData::getInstance()->getExpOfCurLevel();
	// 当前的经验值
	auto curExp = FishingJoyData::getInstance()->getExp();
	auto afterExp = curExp + exp;
	// 是否升级
	if (afterExp >= exps)
	{
		afterExp = afterExp - exps;

		this->levelUp();
	}
	FishingJoyData::getInstance()->setExp(afterExp);

	m_pPanelLayer->updateExp();
	m_pPanelLayer->updateLevel();
}

void GameScene::addBattery(int count, const Point &bornPos)
{
	// 先播放电池特效
	float delay = m_pEffectLayer->showBatteryMoving(bornPos);
	auto frames = Director::getInstance()->getTotalFrames();
	// 在移动完成后增加当前容量
	string key = StringUtils::format("battery%d", frames);
	auto lambda = [this, count](float dt)
	{
		this->addBattery(count);
	};
	this->scheduleOnce(lambda, key, delay);
}

void GameScene::addBattery(int count)
{
	auto fishingJoyData = FishingJoyData::getInstance();
	fishingJoyData->setHalfEnergyNum(fishingJoyData->getHalfEnergyNum() + count);

	m_pPanelLayer->updateHalfEnergyBtn();
}

void GameScene::addPearlBullet(int count, const Point &bornPos)
{
	// 先播放特效
	float delay = m_pEffectLayer->showPearlBulletMoving(bornPos);
	auto frames = Director::getInstance()->getTotalFrames();
	// 在移动完成后增加当前容量
	string key = StringUtils::format("pearl%d", frames);
	auto lambda = [this, count](float dt)
	{
		this->addPearlBullet(count);
	};
	this->scheduleOnce(lambda, key, delay);
}

void GameScene::addPearlBullet(int count)
{
	auto fishingJoyData = FishingJoyData::getInstance();
	fishingJoyData->setPearlBullet(fishingJoyData->getPearlBullet() + count);
	// 刷新
	m_pPanelLayer->updateBulletBtns();
}

void GameScene::addFogBullet(int count, const Point &bornPos)
{
	// 先播放特效
	float delay = m_pEffectLayer->showFogBulletMoving(bornPos);
	auto frames = Director::getInstance()->getTotalFrames();
	// 在移动完成后增加当前容量
	string key = StringUtils::format("fog%d", frames);
	auto lambda = [this, count](float dt)
	{
		this->addFogBullet(count);
	};
	this->scheduleOnce(lambda, key, delay);
}

void GameScene::addFogBullet(int count)
{
	auto fishingJoyData = FishingJoyData::getInstance();
	fishingJoyData->setFogBullet(fishingJoyData->getFogBullet() + count);
	// 刷新
	m_pPanelLayer->updateBulletBtns();
}

void GameScene::addStrengthenBullet(int count, const Point &bornPos)
{
	// 先播放特效
	float delay = m_pEffectLayer->showSterengthenBulletMoving(bornPos);
	auto frames = Director::getInstance()->getTotalFrames();
	// 在移动完成后增加当前容量
	string key = StringUtils::format("strength%d", frames);
	auto lambda = [this, count](float dt)
	{
		this->addStrengthenBullet(count);
	};
	this->scheduleOnce(lambda, key, delay);
}

void GameScene::addStrengthenBullet(int count)
{
	auto fishingJoyData = FishingJoyData::getInstance();
	fishingJoyData->setSterengthenBullet(fishingJoyData->getSterengthenBullet() + count);
	// 刷新
	m_pPanelLayer->updateBulletBtns();
}

void GameScene::addItem(const string &itemName, const Point &bornPos, int number)
{
	// 增加对应的物品
	if (itemName == "gold")
	{
		this->addGold(number, bornPos);
	}
	else if (itemName == "exp")
	{
		this->addExp(number);
	}
	else if (itemName == "battery")
	{
		this->addBattery(number, bornPos);
	}
	else if (itemName == "FogBullet")
	{
		this->addFogBullet(number, bornPos);
	}
	else if (itemName == "PearlBullet")
	{
		this->addPearlBullet(number, bornPos);
	}
	else if (itemName == "SterengthenBullet")
	{
		this->addStrengthenBullet(number, bornPos);
	}
	else if (itemName == "energy")
	{
		this->addEnergy(number);
	}
}

int GameScene::getRandomFishTideID() const
{
	auto vec = m_pLevelLayer->getFishTideVec();

	int index = rand() % vec.size();

	return vec.at(index);
}

void GameScene::levelUp()
{
	auto lv = FishingJoyData::getInstance()->getLevel();
	bool bPlayingChunk = FishingJoyData::getInstance()->isPlayingChunk();

	FishingJoyData::getInstance()->setLevel(lv + 1);
	// 出现升级 图片
	m_pEffectLayer->showLevelUpAction();
	// 随机出现渔网
	string key = "levelUp";
	this->schedule(SDL_CALLBACK_1(GameScene::randomMakeFishingNets, this, 1), key, 0.5f, 8);

	if (bPlayingChunk)
	{
		auto sLevelUp = STATIC_DATA_STRING("level_up_chunk");
		SoundManager::getInstance()->playEffect(sLevelUp, 0);
	}
}

void GameScene::randomMakeFishingNets(float dt, int lv)
{
	Size visibleSize = Director::getInstance()->getVisibleSize();

	float x = RANDOM_0_1() * visibleSize.width;
	float y = RANDOM_0_1() * visibleSize.height;

	this->addFishingNet(lv, Point(x, y), 0);
}

void GameScene::fishCaughtCallback(Fish *fish)
{
	// 鱼被捕捉
	// 获取当前倍率
	int ratio = m_pLevelLayer->getRatio();
	auto fishID = fish->getID();
	// 添加金币
	auto gold = STATIC_FISH_GOLD(fishID);
	this->addGold(gold * ratio, fish->getPosition());

	// 根据当前的鱼的id，判断是否有宝箱掉落
	auto &fishRewardVec = StaticData::getInstance()->getFishRewards();
	auto iter = find_if(fishRewardVec.begin(), fishRewardVec.end(), [fishID](const FishReward &fishReward)
											{ return fishReward.fishID == fishID; });
	// 是否是出现宝箱
	if (iter == fishRewardVec.end())
	{
		return;
	}
	auto &fishReward = *iter;
	auto itemName = fishReward.type;
	auto number = fishReward.number;
	auto scope = fishReward.scope;

	if (itemName == "chest")
	{
		ui::Button *btn = ui::Button::create("chest.png");

		btn->setPosition(fish->getPosition());
		btn->setSwallowTouches(true);
		btn->addClickEventListener(SDL_CALLBACK_1(GameScene::showChestCallback, this));

		this->addChild(btn);
	}
	else // 增加其他道具 添加成功率判断
	{
		float success = RANDOM_0_1();

		if (success < scope)
			this->addItem(itemName, fish->getPosition(), number);
	}
}

void GameScene::showChestCallback(Object *sender)
{
	if (!m_pRewardLayer->isClickChest())
		return;

	// 移除这个按钮
	Node *node = static_cast<Node *>(sender);
	node->removeFromParent();

	this->gamePause();

	m_pRewardLayer->showChest();
}

void GameScene::addFishingNet(int lv, const Point &pos, float rotation)
{
	m_pFishingEntityLayer->addFishingNet(lv, pos, rotation);

	bool bPlayingChunk = FishingJoyData::getInstance()->isPlayingChunk();

	if (bPlayingChunk)
	{
		auto sFishingNet = STATIC_DATA_STRING("fishing_net_show_chunk");
		SoundManager::getInstance()->playEffect(sFishingNet, 0);
	}
}

vector<int> &GameScene::getShownFishIdVec()
{
	return m_pLevelLayer->getShownFishIdVec();
}

vector<float> &GameScene::getStartFishStartTimeVec()
{
	return m_pLevelLayer->getStartFishStartTimeVec();
}

void GameScene::showGroupComeMoving()
{
	m_pEffectLayer->showGroupComeMoving();
}