#include "SimpleAudioEngine.h"
#include "Bird.h"
#include "neural-network/layer/Dense.h"

size_t Bird::aliveBirdCount = 1;

Bird * Bird::createWithFileName(const std::string & fileName)
{
	Bird *sprite = new (std::nothrow) Bird();

	if (sprite && sprite->initWithFileName(fileName))
	{
		sprite->autorelease();
		return sprite;
	}
	CC_SAFE_DELETE(sprite);
	return nullptr;
}

Bird * Bird::createWithFrameName(const std::vector<std::string>& frameNames)
{
	Bird *sprite = new (std::nothrow) Bird();

	if (sprite && sprite->initWithFrameName(frameNames))
	{
		sprite->autorelease();
		return sprite;
	}
	CC_SAFE_DELETE(sprite);
	return nullptr;
}

Bird::Bird() 
{
	bGenerateContactEvent = true;

	alive = true;
	score = 0;
	distanceXToClosestPipe = 1.f;

	linearSpeed = angularSpeed = 0.f;
	gravity = 0;

	brain = std::shared_ptr<mc::Sequential>(new mc::Sequential());
	brain->SetInputUnit(5);

	auto d1 = brain->AddDense(1, "sigmoid");
	d1->GetWeights().randomize(-1.f, 1.f);
	d1->GetBiases().randomize(-1.f, 1.f);
	/*auto d2 = brain->AddDense(1, "sigmoid");
	d2->GetWeights().randomize(-1.f, 1.f);
	d2->GetBiases().randomize(-1.f, 1.f);*/

	brain->SetLoss("meanSquaredError");
	brain->SetOptimizer("sgd", 0.5f);
}

Bird::~Bird()
{
}

void Bird::setGravity(float g)
{
	gravity = g;
}

bool Bird::initWithFileName(const std::string & fileName)
{
	if (!CollideableSprite::initWithFileName(fileName))
	{
		return false;
	}

	return true;
}

bool Bird::initWithFrameName(const std::vector<std::string>& frameNames)
{
	if (!CollideableSprite::initWithFileName(frameNames[0]))
	{
		return false;
	}

	Rect rect;
	rect.size = getTexture()->getContentSize();
	Vector<SpriteFrame*> frames;
	for (auto name : frameNames)
	{
		
		frames.pushBack(SpriteFrame::create(name, rect));
	}

	auto animation = Animation::createWithSpriteFrames(frames, 0.15f);

	runAction(RepeatForever::create(Animate::create(animation)));

	return true;
}

void Bird::mutateMatrix(mc::Matrix dst, mc::Matrix target, float mutationRate, float mutationRange)
{
	for (int i = 0; i < dst.size(); i++)
	{
		if (random<float>(0.f, 1.f) <= mutationRate)
			dst(i) = target(i) + random<float>(-mutationRange, mutationRange);
	}
}

void Bird::update(float dt)
{
	linearSpeed += gravity;
	angularSpeed += 1.0f;

	setPositionY(getPositionY() + linearSpeed);
	setRotation(clampf(-30, getRotation() + angularSpeed, 90));

	CollideableSprite::update(dt);
}

void Bird::onBeginContactEvent(CollideableSprite * other)
{
	if (alive && (other->getTag() == 2 || other->getTag() == 3))
	{
		alive = false;
		linearSpeed = 0.f;
		aliveBirdCount--;
	}
}

void Bird::mutate(std::shared_ptr<mc::Sequential> target, float mutationRate, float mutationRange)
{
	mutateMatrix(brain->GetDense(0)->GetWeights(),
		target->GetDense(0)->GetWeights(),
		mutationRate, mutationRange);
	mutateMatrix(brain->GetDense(0)->GetBiases(),
		target->GetDense(0)->GetBiases(),
		mutationRate, mutationRange);
}

void Bird::randomizeBrain()
{
}

void Bird::flap(float acc)
{
	if (!alive)
	{
		return;
	}

	linearSpeed = acc;
	angularSpeed = -25.f;
}

void Bird::execute(mc::Matrix & input)
{
	mc::Matrix output = brain->Predict(input);
	
	if (output(0) > 0.5f)
	{
		flap(25.f);
	}
}

