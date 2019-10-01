#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include "cocos2d.h"
#include "Box2D/Box2D.h"
#include "neural-network/Sequential.h"

using namespace cocos2d;

class HelloWorld : public cocos2d::Scene
{
public:
    static cocos2d::Scene* createScene();

	HelloWorld();
	virtual ~HelloWorld();

    virtual bool init();

	void start();

    void closeCallback(cocos2d::Ref* pSender);
    
    CREATE_FUNC(HelloWorld);

	DrawNode * drawNode;
	Label * bestScoreLabel;
	Label * geneticStatsLabel;

	size_t pipeCount;

	Sprite * skylineImages[2];

	std::vector<class Pipe*> listOfPipes;
	std::vector<class Bird*> listOfBirds;

	std::shared_ptr<mc::Sequential> bestBrain;
	float bestFitness;
	float mutationRate;
	float mutationRange;

	size_t bestScore;
	size_t generation;
	size_t speedUpRate;

	b2World * world;

	class b2DebugDraw * debugDraw;
	class b2PhysicsContactListener * contactListener;

	CustomCommand customCommand;

	bool onTouchBegan(Touch*  touch, Event *unused_event);
	void onTouchMoved(Touch*  touch, Event *unused_event);
	void onTouchEnded(Touch*  touch, Event *unused_event);

	void onKeyPressed(EventKeyboard::KeyCode keyCode, Event* event);
	void onKeyReleased(EventKeyboard::KeyCode keyCode, Event* event);

	void draw(Renderer *renderer, const Mat4& transform, uint32_t flags) override;

	void onDraw(const Mat4 &transform, uint32_t transformFlags);

	void update(float dt) override;
};

#endif // __HELLOWORLD_SCENE_H__
