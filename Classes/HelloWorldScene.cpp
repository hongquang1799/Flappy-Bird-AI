#include "HelloWorldScene.h"
#include "SimpleAudioEngine.h"
#include "b2DebugDraw.h"
#include "b2PhysicsContactListener.h"
#include "Bird.h"
#include "Pipe.h"

#define BEGIN_PIPE_DISTANCE 1700
#define PIPE_GAP 600
#define PIPE_SPACE 380
#define PIPE_SPEED -350
#define PIPE_WIDTH 156
#define PIPE_HEIGHT 1200

#define PIPE_LOWER_SPACE_HEIGHT 300
#define PIPE_UPPER_SPACE_HEIGHT 1500

#define BIRD_GRAVITY -1.2f
#define BIRD_FLAP_ACC 25.f
#define BIRD_WIDTH 102
#define BIRD_HEIGHT 72

USING_NS_CC;

Scene* HelloWorld::createScene()
{
    return HelloWorld::create();
}

HelloWorld::HelloWorld()
{
}

HelloWorld::~HelloWorld()
{
	delete debugDraw;
	delete contactListener;
	delete world;
}

bool HelloWorld::init()
{
    if ( !Scene::init() )
    {
        return false;
    }

    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

	world = new b2World(b2Vec2(-9.8f, 0.f));
	
	contactListener = new b2PhysicsContactListener();
	world->SetContactListener(new b2PhysicsContactListener());

	debugDraw = new b2DebugDraw(32.f /*ptm*/);
	uint32 flags = 0;
	flags += b2Draw::e_shapeBit;
	flags += b2Draw::e_jointBit;
	//flags += b2Draw::e_aabbBit;
	/*flags += b2Draw::e_pairBit;
	flags += b2Draw::e_centerOfMassBit;*/
	//flags += b2Draw::e_particleBit;
	debugDraw->SetFlags(flags);
	world->SetDebugDraw(debugDraw);

	auto touchListener = EventListenerTouchOneByOne::create();
	touchListener->onTouchBegan = CC_CALLBACK_2(HelloWorld::onTouchBegan, this);
	touchListener->onTouchMoved = CC_CALLBACK_2(HelloWorld::onTouchMoved, this);
	touchListener->onTouchEnded = CC_CALLBACK_2(HelloWorld::onTouchEnded, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(touchListener, this);

	auto keyBoardListener = EventListenerKeyboard::create();
	keyBoardListener->onKeyReleased = CC_CALLBACK_2(HelloWorld::onKeyReleased, this);
	keyBoardListener->onKeyPressed = CC_CALLBACK_2(HelloWorld::onKeyPressed, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(keyBoardListener, this);                                              

	pipeCount = ((size_t)visibleSize.width / PIPE_GAP + 2) * 2;

	for (size_t i = 0; i < 2; i++)
	{
		auto skyline = Sprite::create("skyline.png");
		skyline->setAnchorPoint(Vec2(1.f, 0.5f));
		skyline->setScale(visibleSize.height / skyline->getContentSize().height);
		addChild(skyline, 0);

		skylineImages[i] = skyline;
	}

	bestScoreLabel = Label::create("", "fonts/RobotoCondensed-Regular.ttf", 75.f);
	bestScoreLabel->setPosition(Vec2(visibleSize.width / 2, visibleSize.height * 0.95f));
	addChild(bestScoreLabel, 5);

	geneticStatsLabel = Label::create("", "fonts/RobotoCondensed-Regular.ttf", 50.f);
	geneticStatsLabel->setPosition(Vec2(visibleSize.width * 0.1, visibleSize.height * 0.88f));
	addChild(geneticStatsLabel, 5);

	CollideableSprite * edge_up = CollideableSprite::createWithFileName("");
	edge_up->setPosition(visibleSize.width / 2, visibleSize.height);
	edge_up->setTag(3);
	edge_up->setShapeAsBox(visibleSize.width, 50.f, world);
	addChild(edge_up, 2);

	CollideableSprite * edge_down = CollideableSprite::createWithFileName("");
	edge_down->setPosition(visibleSize.width / 2, 0.f);
	edge_down->setTag(3);
	edge_down->setShapeAsBox(visibleSize.width, 50.f, world);
	addChild(edge_down, 2);

	for (int i = 0; i < pipeCount; i += 2)
	{
		Pipe * pipe_down = Pipe::createWithFileName("pipe_down.png");
		pipe_down->setScale(PIPE_WIDTH / pipe_down->getContentSize().width, PIPE_HEIGHT / pipe_down->getContentSize().height);
		pipe_down->setTag(2);
		pipe_down->setShapeAsBox(PIPE_WIDTH, PIPE_HEIGHT, world);
		addChild(pipe_down, 2);

		Pipe * pipe_up = Pipe::createWithFileName("pipe_up.png");
		pipe_up->setScale(PIPE_WIDTH / pipe_up->getContentSize().width, PIPE_HEIGHT / pipe_up->getContentSize().height);
		pipe_up->setTag(2);
		pipe_up->setShapeAsBox(PIPE_WIDTH, PIPE_HEIGHT, world);
		addChild(pipe_up, 2);

		listOfPipes.push_back(pipe_down);
		listOfPipes.push_back(pipe_up);
	}

	bestFitness = 0.f;
	mutationRate = 0.5f;
	mutationRange = 0.05f;
	generation = 0;
	speedUpRate = 1;

	start();

	drawNode = DrawNode::create();
	addChild(drawNode);

	scheduleUpdate();

	return true;
}

void HelloWorld::start()
{
	Bird::aliveBirdCount = 100;
	bestScore = 0;
	generation++;

	CCLOG("start with generation %d", generation);
	auto visibleSize = Director::getInstance()->getVisibleSize();

	skylineImages[0]->setPosition(visibleSize.width, visibleSize.height / 2);
	skylineImages[1]->setPosition(skylineImages[0]->getPositionX()
		+ skylineImages[1]->getContentSize().width * skylineImages[1]->getScale(), visibleSize.height / 2);

	for (int i = 0; i < pipeCount; i += 2)
	{
		Pipe * pipe_down = listOfPipes[i];
		Pipe * pipe_up = listOfPipes[i + 1];

		pipe_down->setPosition(BEGIN_PIPE_DISTANCE + PIPE_GAP * i / 2,
			random(PIPE_LOWER_SPACE_HEIGHT, PIPE_UPPER_SPACE_HEIGHT - PIPE_SPACE) - PIPE_HEIGHT / 2);
		pipe_up->setPosition(pipe_down->getPositionX(), pipe_down->getPositionY() + PIPE_HEIGHT + PIPE_SPACE);
	}

	for (size_t i = 0; i < Bird::aliveBirdCount; i++)
	{
		Bird * bird = Bird::createWithFrameName({ "bird_one.png", "bird_two.png", "bird_three.png" });
		bird->setPosition(visibleSize.width / 3, visibleSize.height / 3 * 2);
		bird->setScale(BIRD_WIDTH / bird->getContentSize().width, BIRD_HEIGHT / bird->getContentSize().height);
		bird->setTag(1);
		bird->setShapeAsBox(BIRD_WIDTH, BIRD_HEIGHT, world);
		bird->setGravity(BIRD_GRAVITY);
		addChild(bird, 3);

		if (bestBrain && i != 0) bird->mutate(bestBrain, mutationRate, mutationRange);

		listOfBirds.push_back(bird);
	}	
}

void HelloWorld::update(float dt)
{
	dt = 1.0f / 60.f;

	Size visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 deltaMove = Vec2(PIPE_SPEED * dt, 0);
	Pipe * pairOfClosestPipe[2];
	mc::Matrix vision;
	vision.allocateMemory(1, 5);
	
	for (size_t i = 0; i < speedUpRate; i++)
	{
		world->Step(dt, 8, 3);

		// skylines update
		
		if (Bird::aliveBirdCount > 0)
		{
			skylineImages[0]->setPositionX(skylineImages[0]->getPosition().x + deltaMove.x);
			skylineImages[1]->setPositionX(skylineImages[1]->getPosition().x + deltaMove.x);
		}
		
		if (skylineImages[0]->getPosition().x < 0.f)
		{
			std::swap(skylineImages[0], skylineImages[1]);

			skylineImages[1]->setPositionX(skylineImages[0]->getPositionX()
				+ skylineImages[1]->getContentSize().width * skylineImages[1]->getScale());

			
		}


		// pipes update
		float distanceXToClosestPipe = 1000000.f;	
		for (size_t i = 0; i < pipeCount; i += 2)
		{
			auto pipe_down = listOfPipes[i];
			auto pipe_up = listOfPipes[i + 1];

			if (pipe_down->getPositionX() + PIPE_WIDTH / 2 < 0.f)
			{
				listOfPipes.erase(listOfPipes.begin(), listOfPipes.begin() + 2);

				pipe_down->setPosition(listOfPipes.back()->getPositionX() + PIPE_GAP,
					random(PIPE_LOWER_SPACE_HEIGHT, PIPE_UPPER_SPACE_HEIGHT - PIPE_SPACE) - PIPE_HEIGHT / 2);
				pipe_up->setPosition(pipe_down->getPositionX(), pipe_down->getPositionY() + PIPE_HEIGHT + PIPE_SPACE);

				listOfPipes.push_back(pipe_down);
				listOfPipes.push_back(pipe_up);
			}
			else if (Bird::aliveBirdCount > 0)
			{			
				pipe_down->move(deltaMove);
				pipe_up->move(deltaMove);
				pipe_down->update(dt);
				pipe_up->update(dt);
			}

			float distX = pipe_down->getPositionX() - listOfBirds[0]->getPositionX();

			if (distX < PIPE_WIDTH && distX > -PIPE_WIDTH) 
				distX = 0.00001f;

			if (distX > 0.f && distX < distanceXToClosestPipe)
			{
				distanceXToClosestPipe = distX;
				pairOfClosestPipe[0] = pipe_down;
				pairOfClosestPipe[1] = pipe_up;
			}
		}


		// birds update
		for (auto it = listOfBirds.begin(); it != listOfBirds.end();)
		{
			Bird * bird = *it;
			if (!bird->isAlive() && bird->getPositionY() < -150.f)
			{
				bird->removeFromParent();
				it = listOfBirds.erase(it);
			}
			else
			{
				vision(0) = bird->getPosition().y / visibleSize.height;
				vision(1) = (visibleSize.height - bird->getPosition().y) / visibleSize.height;
				vision(2) = distanceXToClosestPipe / visibleSize.width;
				vision(3) = (bird->getPositionY() - pairOfClosestPipe[0]->getPosition().y - PIPE_HEIGHT / 2) / visibleSize.height;
				vision(4) = (pairOfClosestPipe[1]->getPosition().y - bird->getPositionY() - PIPE_HEIGHT / 2) / visibleSize.height;

				//drawNode->clear();
				//drawNode->drawLine(bird->getPosition(), bird->getPosition() - Vec2(0, bird->getPosition().y), Color4F::RED);
				//drawNode->drawLine(bird->getPosition(), bird->getPosition() + Vec2(0, visibleSize.height - bird->getPosition().y), Color4F::RED);
				//drawNode->drawLine(bird->getPosition(), bird->getPosition() + Vec2(distanceXToClosestPipe, 0), Color4F::RED);
				//drawNode->drawLine(bird->getPosition(), bird->getPosition() - Vec2(0, bird->getPositionY() - pairOfClosestPipe[0]->getPosition().y - PIPE_HEIGHT / 2), Color4F::GREEN);
				//drawNode->drawLine(bird->getPosition(), bird->getPosition() + Vec2(0, pairOfClosestPipe[1]->getPosition().y - bird->getPositionY() - PIPE_HEIGHT / 2), Color4F::RED);

				if (bird->getDistanceXToClosestPipe() < 0.00002f && distanceXToClosestPipe > 1.f)
					bird->increaseScore();

				if (bird->getScore() > bestScore)
					bestScore = bird->getScore();

				bird->setDistanceXToClosestPipe(distanceXToClosestPipe);

				bird->execute(vision);
				bird->update(dt);

				it++;
			}
		}

		if (listOfBirds.size() == 1 && listOfBirds.front()->getFitness() > bestFitness)
		{
			bestBrain = listOfBirds.front()->getBrain();
			bestFitness = listOfBirds.front()->getFitness();
		}
		else if (listOfBirds.empty())
		{
			start();
		}
	}

	bestScoreLabel->setString(String::createWithFormat("Best Score: %d", bestScore)->getCString());
	geneticStatsLabel->setString(String::createWithFormat("Generation: %d\nMutation Rate: %d%\nMutation Range: %.2f\n\nSpeed: %d", generation, (int)(mutationRate * 100.f), mutationRange, speedUpRate)->getCString());
}

void HelloWorld::closeCallback(Ref* pSender)
{
    //Close the cocos2d-x game scene and quit the application
    Director::getInstance()->end();

    /*To navigate back to native iOS screen(if present) without quitting the application  ,do not use Director::getInstance()->end() as given above,instead trigger a custom event created in RootViewController.mm as below*/

    //EventCustom customEndEvent("game_scene_close_event");
    //_eventDispatcher->dispatchEvent(&customEndEvent);
}

bool HelloWorld::onTouchBegan(Touch * touch, Event * unused_event)
{
	if (touch->getLocation().x > Director::getInstance()->getVisibleSize().width / 2)
	{
		speedUpRate = MAX(1, MIN(20, speedUpRate + 1));
	}
	else 
	{
		speedUpRate = MAX(1, MIN(20, speedUpRate - 1));
	}

	return true;
}

void HelloWorld::onTouchMoved(Touch * touch, Event * unused_event)
{
}

void HelloWorld::onTouchEnded(Touch * touch, Event * unused_event)
{
}

void HelloWorld::onKeyPressed(EventKeyboard::KeyCode keyCode, Event * event)
{
	if (keyCode == EventKeyboard::KeyCode::KEY_SPACE) {}
		//listOfBirds[0]->flap(BIRD_FLAP_ACC);
	else if (keyCode == EventKeyboard::KeyCode::KEY_LEFT_ARROW)
	{
		speedUpRate = MAX(1, MIN(20, speedUpRate-1));
	}
	else if (keyCode == EventKeyboard::KeyCode::KEY_RIGHT_ARROW)
	{
		speedUpRate = MAX(1, MIN(20, speedUpRate + 1));
	}
	else if (keyCode == EventKeyboard::KeyCode::KEY_ESCAPE)
	{
		closeCallback(this);
	}
}

void HelloWorld::onKeyReleased(EventKeyboard::KeyCode keyCode, Event * event)
{
}

void HelloWorld::draw(Renderer * renderer, const Mat4 & transform, uint32_t flags)
{
	/*customCommand.init(100000.f);
	customCommand.func = CC_CALLBACK_0(HelloWorld::onDraw, this, transform, flags);
	renderer->addCommand(&customCommand);*/
}

void HelloWorld::onDraw(const Mat4 & transform, uint32_t transformFlags)
{
	kmGLPushMatrix();
	kmGLLoadMatrix(&transform);

	GL::enableVertexAttribs(cocos2d::GL::VERTEX_ATTRIB_FLAG_POSITION);
	world->DrawDebugData();

	CHECK_GL_ERROR_DEBUG();

	kmGLPopMatrix();
}
