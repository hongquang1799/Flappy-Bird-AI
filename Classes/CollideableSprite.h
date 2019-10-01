#ifndef COLLIDEABLE_SPRITE_H
#define COLLIDEABLE_SPRITE_H

#include "cocos2d.h"
#include "Box2D/Box2D.h"

using namespace cocos2d;

class CollideableSprite : public cocos2d::Sprite
{
public:
	static CollideableSprite * createWithFileName(const std::string& fileName);

	CollideableSprite();
	virtual ~CollideableSprite(); 

	void update(float dt) override;

	void setPosition(float x, float y) override;

	void setPosition(const Vec2& position) override;

	void removeFromParent() override;

	virtual void setShapeAsCircle(float radius, b2World * world);

	virtual void setShapeAsBox(float width, float height, b2World * world);

	virtual void onBeginContactEvent(CollideableSprite * other);

	bool bGenerateContactEvent;
protected:
	virtual bool initWithFileName(const std::string& fileName);

	virtual void configurateBody(float gravityScale, bool isDynamic, b2World * world);

	void updateBodyTransform();

	bool positionDirty;

	b2World * psworld;
	
	b2Body * body;
};

#endif // __HELLOWORLD_SCENE_H__
