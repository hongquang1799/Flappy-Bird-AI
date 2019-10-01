#include "SimpleAudioEngine.h"
#include "CollideableSprite.h"

#define ptm 32.f

CollideableSprite * CollideableSprite::createWithFileName(const std::string & fileName)
{
	CollideableSprite *sprite = new (std::nothrow) CollideableSprite();

	if (sprite && sprite->initWithFileName(fileName))
	{
		sprite->autorelease();
		return sprite;
	}
	CC_SAFE_DELETE(sprite);
	return nullptr;
}

CollideableSprite::CollideableSprite() : 
positionDirty(true),
psworld(NULL),
body(NULL)
{
	bGenerateContactEvent = false;
}

CollideableSprite::~CollideableSprite()
{
}

bool CollideableSprite::initWithFileName(const std::string & fileName)
{
	if (fileName.empty())
	{
		if (!Sprite::init())
		{
			return false;
		}
	}
	else
	{
		if (!Sprite::initWithFile(fileName))
		{
			return false;
		}
	}
	
	return true;
}

void CollideableSprite::configurateBody(float gravityScale, bool isDynamic, b2World * world)
{
	if (psworld == NULL) psworld = world;

	if (body == NULL)
	{
		b2BodyDef bd;
		bd.gravityScale = gravityScale;
		bd.type = (isDynamic) ? b2_dynamicBody : b2_staticBody;
		bd.userData = this;

		body = psworld->CreateBody(&bd);

		setPosition(getPosition());
	}
}

void CollideableSprite::updateBodyTransform()
{
	if (positionDirty)
	{
		b2Vec2 position = b2Vec2(getPositionX() / ptm, getPositionY() / ptm);
		float angle = -1 * CC_DEGREES_TO_RADIANS(getRotation());

		body->SetTransform(position, angle);
		body->SetAwake(true);

		positionDirty = false;
	}
}

void CollideableSprite::update(float dt)
{
	updateBodyTransform();
}

void CollideableSprite::setPosition(float x, float y)
{
	positionDirty = true;
	Sprite::setPosition(x, y);

	if (body) updateBodyTransform();
}

void CollideableSprite::setPosition(const Vec2& position)
{
	positionDirty = true;
	Sprite::setPosition(position);

	if (body) updateBodyTransform();
}

void CollideableSprite::removeFromParent()
{
	psworld->DestroyBody(body);
	body = NULL;

	Sprite::removeFromParent();
}

void CollideableSprite::setShapeAsCircle(float radius, b2World * world)
{
	configurateBody(0.f, false, world);

	b2CircleShape circleShape;
	circleShape.m_radius = radius / ptm;

	b2FixtureDef fd;
	fd.density = 1.0f;
	fd.friction = 0.5f;
	fd.restitution = 0.5f;
	fd.shape = &circleShape;

	body->CreateFixture(&fd)->SetSensor(true);
}

void CollideableSprite::setShapeAsBox(float width, float height, b2World * world)
{
	configurateBody(0.f, true, world);

	b2PolygonShape polygonShape;
	polygonShape.SetAsBox(width / 2 / ptm, height / 2 / ptm);

	b2FixtureDef fd;
	fd.density = 1.0f;
	fd.friction = 0.5f;
	fd.restitution = 0.5f;
	fd.shape = &polygonShape;

	body->CreateFixture(&fd)->SetSensor(true);
}

void CollideableSprite::onBeginContactEvent(CollideableSprite * other)
{
	/*CCLOG("%s-%s", this->getName().c_str(), other->getName().c_str());*/
}
