#include "GoldCounter.h"
#include "ScrollLabel.h"

GoldCounter::GoldCounter()
		: m_nNumber(0)
{
}

GoldCounter::~GoldCounter()
{
}

GoldCounter *GoldCounter::create(int number)
{
	auto node = new GoldCounter();

	if (node && node->init(number))
		node->autorelease();
	else
		SDL_SAFE_DELETE(node);

	return node;
}

bool GoldCounter::init(int number)
{

	// 添加图片资源
	vector<Sprite *> vec;

	Texture *texture = Director::getInstance()->getTextureCache()->getTextureForKey("ui_text_01.png");

	vec.push_back(Sprite::createWithTexture(texture, Rect(0, 0, 26, 34)));
	vec.push_back(Sprite::createWithTexture(texture, Rect(26, 0, 26, 34)));
	vec.push_back(Sprite::createWithTexture(texture, Rect(52, 0, 26, 34)));
	vec.push_back(Sprite::createWithTexture(texture, Rect(78, 0, 26, 34)));
	vec.push_back(Sprite::createWithTexture(texture, Rect(104, 0, 26, 34)));
	vec.push_back(Sprite::createWithTexture(texture, Rect(130, 0, 26, 34)));
	vec.push_back(Sprite::createWithTexture(texture, Rect(156, 0, 26, 34)));
	vec.push_back(Sprite::createWithTexture(texture, Rect(182, 0, 26, 34)));
	vec.push_back(Sprite::createWithTexture(texture, Rect(208, 0, 26, 34)));
	vec.push_back(Sprite::createWithTexture(texture, Rect(234, 0, 26, 34)));

	Size size = vec.at(0)->getContentSize();
	int curNumber = m_nNumber = number;

	for (int i = 0; i < 7; i++)
	{
		// 计算各个位数的值
		int e = pow(10, (6 - i));
		int b = curNumber / e;

		curNumber -= b * e;
		// 创建label
		ScrollLabel *label = ScrollLabel::create(vec, size, b);
		// 设置位置
		label->setPosition(i * size.width + size.width / 2, size.height / 2);

		m_labels.push_back(label);

		this->addChild(label);
	}

	return true;
}

void GoldCounter::update(float dt)
{
	for (auto iter = m_labels.begin(); iter != m_labels.end(); iter++)
	{
		auto label = *iter;
		label->update(dt);
	}
}

void GoldCounter::visit()
{

	Renderer *renderer = Director::getInstance()->getRenderer();

	beforeCommand.init(0.f);
	beforeCommand.func = [this]()
	{
		beforeVisit();
	};
	renderer->addCommand(&beforeCommand);

	Node::visit();

	afterCommand.init(0.f);
	afterCommand.func = [this]()
	{
		afterVisit();
	};
	renderer->addCommand(&afterCommand);
}

void GoldCounter::beforeVisit()
{
	Size size = this->getContentSize();
	auto pos = this->convertToWorldSpace(Point::ZERO);

	SDL_Rect rect = {(int)pos.x, (int)pos.y, (int)size.width, (int)size.height};

	Renderer *renderer = Director::getInstance()->getRenderer();

	renderer->setClipRect(&rect);
}

void GoldCounter::afterVisit()
{

	Renderer *renderer = Director::getInstance()->getRenderer();

	renderer->setClipRect(nullptr);
}

void GoldCounter::setGoldNumber(int number)
{
	// 是增加还是减少
	bool bDown = number > m_nNumber;

	int oldNum = m_nNumber;
	int newNum = number;

	int i = 0;

	for (auto iter = m_labels.begin(); iter != m_labels.end(); iter++)
	{
		// 计算将来各个位数的值
		int e1 = pow(10, (6 - i));
		int b1 = number / e1;

		number -= b1 * e1;

		int e2 = pow(10, (6 - i));
		int b2 = oldNum / e2;

		oldNum -= b2 * e2;

		if (b1 != b2)
		{
			auto label = *iter;

			label->setIndex(b1, 3.f, bDown);
		}
		i++;
	}

	m_nNumber = newNum;
}