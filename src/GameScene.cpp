
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

	b2Vec2 gravity(0.f, 0.f);
	s_pWorld = new b2World(gravity);
	s_pWorld->SetAllowSleeping(true);			// 允许休眠
	s_pWorld->SetContinuousPhysics(true); // 连续物理测试
	s_pWorld->SetContactListener(this);

	this->preloadResources();

	// 关卡层
	int tollgateIndex = FishingJoyData::getInstance()->getTollgateIndex();
	auto tollgateName = StringUtils::format("level/level%d.tmx", tollgateIndex + 1);
	m_pLevelLayer = LevelLayer::create(tollgateName);
	this->addChild(m_pLevelLayer);

	// 鱼层
	m_pFishLayer = FishLayer::create();
	m_pFishLayer->setDelegate(this);
	m_pFishLayer->reset();
	this->addChild(m_pFishLayer);

	// 效果层
	m_pEffectLayer = EffectLayer::create();
	this->addChild(m_pEffectLayer);

	// ui
	m_pPanelLayer = PanelLayer::create();
	m_pPanelLayer->setDelegate(this);
	m_pPanelLayer->update();
	this->addChild(m_pPanelLayer);

	// 事件层
	m_pTouchLayer = TouchLayer::create();
	m_pTouchLayer->setDelegate(this);
	this->addChild(m_pTouchLayer);

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
	m_pCannon->setPosition(m_pPanelLayer->getConnonBornPos());
	this->addChild(m_pCannon);

	// 添加金币计数器
	m_pGoldTimer = GoldTimer::create();
	m_pGoldTimer->setDelegate(this);
	m_pGoldTimer->setPosition(STATIC_DATA_POINT("gold_timer_pos"));
	this->addChild(m_pGoldTimer);

	// 奖励层
	m_pRewardLayer = RewardLayer::create();
	m_pRewardLayer->setDelegate(this);
	this->addChild(m_pRewardLayer);

	// 暂停层
	m_pPauseLayer = PauseLayer::create();
	m_pPauseLayer->setDelegate(this);
	m_pPauseLayer->updateUI();
	m_pPauseLayer->setPosition(0, -visibleSize.height);

	this->addChild(m_pPauseLayer);

	this->playBackgroundMusic();

	this->scheduleUpdate();

	return true;
}
void GameScene::update(float dt)
{

	const int PTM_RATIO = GB2ShapeCache::getInstance()->getPtmRatio();
	// PhysicalEngine::getInstance()->PTM_RATIO;

	for (b2Body *body = s_pWorld->GetBodyList(); body; body = body->GetNext())
	{
		if (body->GetUserData())
		{
			Entity *node = static_cast<Entity *>(body->GetUserData());

			b2Vec2 position = b2Vec2(node->getPositionX() / PTM_RATIO, node->getPositionY() / PTM_RATIO);

			auto sprite = node->getSprite();

			float angle = SDL_ANGLE_TO_DEGREE(sprite->getRotation() + 180.f);

			body->SetTransform(position, angle);
		}
	}

	s_pWorld->Step(dt, 0, 0);

	m_pGoldTimer->update(dt);
	m_pFishLayer->update(dt);
	m_pBulletLayer->update(dt);
	m_pFishingEntityLayer->update(dt);
}
void GameScene::preloadResources()
{

	auto spriteFrameCache = Director::getInstance()->getSpriteFrameCache();

	auto values = StaticData::getInstance()->getValueForKey("fish_src_filepath")->asValueVector();

	for (auto value : values)
	{
		spriteFrameCache->addSpriteFramesWithFile(value.asString());
	}

	AnimationCache::getInstance()->addAnimationsWithFile(STATIC_DATA_STRING("animation_filepath"));
}

void GameScene::purge()
{
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

	if (entity->getType() == Entity::Type::Fish)
	{
		Fish *fish = static_cast<Fish *>(entity);
		bullet->collided(fish);

		if (m_pFishLayer->getCurState() != FishLayer::State::Group)
			fish->collided(TriggingType::NetGunBullet);

		if (fish->getFishType() == FishType::StarFish)
		{
			fish->caught();

			bullet->setMakeFishingEntity(false);
		}
	}
}

void GameScene::collisionOfFishingEntity(FishingEntity *fishingEntity, Entity *entity)
{

	Fish *fish = nullptr;

	if (entity->getType() == Entity::Type::Fish)
	{
		fish = static_cast<Fish *>(entity);
	}
	if (fish == nullptr)
		return;

	float scope = 0.f;

	if (fishingEntity->getFishingType() == FishingType::Laser)
	{
		scope = 1.f;
	}

	else if (fish->isInvulnerable())
	{
		scope = 0.f;
	}
	else if (fishingEntity->getFishingType() == FishingType::FishingNet)
	{
		auto fishingNet = static_cast<FishingNet *>(fishingEntity);

		int ratio = m_pLevelLayer->getRatio();
		int gold = STATIC_FISH_GOLD(fish->getID()) * ratio;

		float tax = m_pLevelLayer->getTax();

		scope = fishingNet->getLevel() * (1 - tax) / gold;
	}

	else if (fishingEntity->getFishingType() == FishingType::Pearl)
	{
		scope = 0.4f;
	}
	else if (fishingEntity->getFishingType() == FishingType::Fog)
	{
		scope = 0.4f;
	}

	fishingEntity->collided();

	float success = RANDOM_0_1();

	if (success < scope)
	{
		fish->caught();

		this->addExp(STATIC_FISH_EXP(fish->getID()));

		if (fishingEntity->getFishingType() != FishingType::Laser)
		{
			this->addEnergy(STATIC_FISH_ENERGY(fish->getID()));
		}
	}

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
			SoundManager::getInstance()->playEffect(STATIC_DATA_STRING("change_net_gun"), 0);
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
			SoundManager::getInstance()->playEffect(STATIC_DATA_STRING("change_net_gun"), 0);
		}
	}
}

void GameScene::pauseBtnCallback()
{

	this->gamePause();

	Size visibleSize = Director::getInstance()->getVisibleSize();
	MoveBy *move = MoveBy::create(0.5f, Point(0, visibleSize.height));
	EaseExponentialOut *action = EaseExponentialOut::create(move);

	m_pPauseLayer->runAction(action);
}

void GameScene::continueGame()
{
	Size visibleSize = Director::getInstance()->getVisibleSize();

	this->gameResume();

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

	FishingJoyData::getInstance()->setPlayingMusic(playingMusic);

	this->playBackgroundMusic();
}

void GameScene::operateChunk()
{
	bool bPlayingChunk = FishingJoyData::getInstance()->isPlayingChunk();
	bPlayingChunk = !bPlayingChunk;

	FishingJoyData::getInstance()->setPlayingChunk(bPlayingChunk);
}

void GameScene::changeBullet(ItemButton *button, bool oldVisible, bool visible)
{
	auto selectedBulletName = button->getName();
	auto pos = button->getPosition();

	if (oldVisible == visible)
	{
		m_pEffectLayer->changeSelectedMark("bullet", pos);

		FishingJoyData::getInstance()->setSelectedBullet(selectedBulletName);
	}
	else if (visible)
	{
		m_pEffectLayer->addSelectedMark("bullet", pos);
	}
	else
	{
		m_pEffectLayer->removeSelectedMark("bullet");
	}
}

bool GameScene::onTouchBegan(Touch *touch, SDL_Event *event)
{
	if (m_pCannon->isCanFire() == false)
		return true;

	auto pos = touch->getLocation();

	m_pCannon->aimAt(pos);

	auto fishingJoyData = FishingJoyData::getInstance();

	bool bPlayingChunk = fishingJoyData->isPlayingChunk();

	if (!m_pCannon->isCanFire())
		return true;

	auto bulletName = fishingJoyData->getSelectedBullet();

	if (m_pCannon->getType() == Cannon::Type::Laser)
	{
		auto afterEnergy = fishingJoyData->getEnergy() - 100;

		fishingJoyData->setEnergy(afterEnergy);
		m_pPanelLayer->updateEnergy();
		m_pPanelLayer->stopEnergyAnimation();

		if (bPlayingChunk)
		{
			auto laserFire = STATIC_DATA_STRING("laser_fire_chunk");
			SoundManager::getInstance()->playEffect(laserFire, 0);
		}
	}

	else
	{

		auto curBulletNum = fishingJoyData->getBulletNumByName(bulletName);

		auto afterBulletNum = curBulletNum - 1;

		fishingJoyData->alterSelectedBulletNum(-1);
	}

	if (m_pCannon->isBelongNetBulletType(m_pCannon->getType()))
	{

		int ratio = m_pLevelLayer->getRatio();
		int needGold = fishingJoyData->getGoldByCannon(m_pCannon) * ratio;
		bool bRet = this->subGold(needGold);

		if (!bRet)
			return true;

		if (bPlayingChunk)
		{
			SoundManager::getInstance()->playEffect(STATIC_DATA_STRING("net_gun_shoot_chunk"), 0);
		}
	}

	m_pPanelLayer->updateBulletBtns();

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

	auto degree = SDL_ANGLE_TO_DEGREE(rotation);
	Size cannonSize = m_pCannon->getContentSize();

	bornPos.x += SDL_sinf(degree) * cannonSize.height / 2;
	bornPos.y -= SDL_cosf(degree) * cannonSize.height / 2;

	if (m_pCannon->getType() == Cannon::Type::Laser)
	{
		m_pFishingEntityLayer->addLaser(bornPos, rotation);
	}
	else
	{

		auto bulletName = FishingJoyData::getInstance()->getSelectedBullet();
		auto netGunBullet = STATIC_DATA_STRING("NetGunBullet");
		auto strengthenBullet = STATIC_DATA_STRING("SterengthenBullet");
		auto fogBullet = STATIC_DATA_STRING("FogBullet");
		auto pearlBullet = STATIC_DATA_STRING("PearlBullet");

		if (bulletName == netGunBullet)
		{
			auto bulletLv = m_pCannon->getNetGunBulletLv();

			m_pBulletLayer->addNetGunBullet(bulletLv, bornPos, pos, rotation);
		}

		else if (bulletName == strengthenBullet)
		{
			m_pBulletLayer->addStrengthenBullet(bornPos, pos, rotation);
		}

		else if (bulletName == fogBullet)
		{
			m_pBulletLayer->addFogBullet(bornPos, pos, rotation);
		}

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

	if (m_pCannon->getType() == Cannon::Type::Laser)
	{

		auto oldType = m_pCannon->getLastType();

		m_pCannon->setType(oldType);

		m_pPanelLayer->showAddAndSubBtn();

		if (bPlayingChunk)
		{
			auto sChangeLaser = STATIC_DATA_STRING("change_laser_chunk");
			SoundManager::getInstance()->playEffect(sChangeLaser, 0);
		}
	}
	auto fishingJoyData = FishingJoyData::getInstance();
	auto bulletNum = fishingJoyData->getSelectedBulletNum();

	if (bulletNum <= 0)
	{
		auto netGunBullet = STATIC_DATA_STRING("NetGunBullet");

		fishingJoyData->setSelectedBullet(netGunBullet);
		m_pPanelLayer->setSelectedBulletBtn(netGunBullet);
	}
}

void GameScene::shootAt(Bullet *bullet, Fish *fish)
{
	switch (bullet->getBulletType())
	{
	case BulletType::NetGunBullet:
	{
		int lv = static_cast<NetGunBullet *>(bullet)->getLevel();
		this->addFishingNet(lv, bullet->getPosition(), 0);
	}
	break;
	case BulletType::StrengthenBullet:
	{

		int lv = rand() % 7 + 1;

		this->addFishingNet(lv, bullet->getPosition(), 0);

		auto totalFrames = Director::getInstance()->getTotalFrames();
		auto key = StringUtils::format("strengthen%d", totalFrames);

		this->schedule(SDL_CALLBACK_1(GameScene::randomMakeFishingNets, this, lv), key, 0.2f, 8);
	}
	break;
	case BulletType::FogBullet:
	{
		int lv = rand() % 7 + 1;
		auto pos = bullet->getPosition();

		this->addFishingNet(lv, pos, 0);

		if (fish == nullptr)
			break;

		m_pFishingEntityLayer->addFog(fish, pos, bullet->getRotation());

		fish->setInvulnerable(true);

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

	m_pEffectLayer->addGoldAnimation(pos);

	m_pEffectLayer->addFadeScoreAction(number, pos);

	auto fishingJoy = FishingJoyData::getInstance();
	auto gold = fishingJoy->getGold() + number;

	fishingJoy->setGold(gold);

	m_pPanelLayer->updateGold();

	bool bPlayingChunk = FishingJoyData::getInstance()->isPlayingChunk();

	if (bPlayingChunk)
	{
		SoundManager::getInstance()->playEffect(STATIC_DATA_STRING("get_gold_chunk"), 0);
	}
}

void GameScene::showWaveAction()
{
	m_pEffectLayer->showWaveAction();

	bool bPlayingChunk = FishingJoyData::getInstance()->isPlayingChunk();

	if (bPlayingChunk)
	{
		SoundManager::getInstance()->playEffect(STATIC_DATA_STRING("wave_coming_chunk"), 0);
	}
}

void GameScene::addChestLightAnim(const string &name, const Point &pos)
{
	m_pEffectLayer->addChestLightAnim(name, pos);

	bool bPlayingChunk = FishingJoyData::getInstance()->isPlayingChunk();

	if (bPlayingChunk)
	{
		SoundManager::getInstance()->playEffect(STATIC_DATA_STRING("open_chest_chunk"), 0);
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
		bRet = false;
	}
	else
	{
		fishingJoy->setGold(gold);

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

	if (afterEnergy >= 100)
	{

		m_pPanelLayer->runEnergyAnimation();
		m_pPanelLayer->hideAddAndSubBtn();

		m_pCannon->setType(Cannon::Type::Laser);

		if (bPlayingChunk)
		{
			auto sChangeLaser = STATIC_DATA_STRING("change_laser_chunk");
			SoundManager::getInstance()->playEffect(sChangeLaser, 0);
		}
	}

	fishingJoyData->setEnergy(afterEnergy);

	m_pPanelLayer->updateEnergy();
}

void GameScene::addExp(float exp)
{

	auto exps = FishingJoyData::getInstance()->getExpOfCurLevel();

	auto curExp = FishingJoyData::getInstance()->getExp();
	auto afterExp = curExp + exp;

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

	float delay = m_pEffectLayer->showBatteryMoving(bornPos);
	auto frames = Director::getInstance()->getTotalFrames();

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

	float delay = m_pEffectLayer->showPearlBulletMoving(bornPos);
	auto frames = Director::getInstance()->getTotalFrames();

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

	m_pPanelLayer->updateBulletBtns();
}

void GameScene::addFogBullet(int count, const Point &bornPos)
{

	float delay = m_pEffectLayer->showFogBulletMoving(bornPos);
	auto frames = Director::getInstance()->getTotalFrames();

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

	m_pPanelLayer->updateBulletBtns();
}

void GameScene::addStrengthenBullet(int count, const Point &bornPos)
{

	float delay = m_pEffectLayer->showSterengthenBulletMoving(bornPos);
	auto frames = Director::getInstance()->getTotalFrames();

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

	m_pPanelLayer->updateBulletBtns();
}

void GameScene::addItem(const string &itemName, const Point &bornPos, int number)
{

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

	m_pEffectLayer->showLevelUpAction();

	this->schedule(SDL_CALLBACK_1(GameScene::randomMakeFishingNets, this, 1), "levelUp", 0.5f, 8);

	if (bPlayingChunk)
	{
		SoundManager::getInstance()->playEffect(STATIC_DATA_STRING("level_up_chunk"), 0);
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

	int ratio = m_pLevelLayer->getRatio();
	auto fishID = fish->getID();

	this->addGold(STATIC_FISH_GOLD(fishID) * ratio, fish->getPosition());

	auto &fishRewardVec = StaticData::getInstance()->getFishRewards();
	auto iter = find_if(fishRewardVec.begin(), fishRewardVec.end(), [fishID](const FishReward &fishReward)
											{ return fishReward.fishID == fishID; });

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
	else
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
		SoundManager::getInstance()->playEffect(STATIC_DATA_STRING("fishing_net_show_chunk"), 0);
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