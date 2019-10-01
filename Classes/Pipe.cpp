#include "SimpleAudioEngine.h"
#include "Pipe.h"

Pipe * Pipe::createWithFileName(const std::string & fileName)
{
	Pipe *sprite = new (std::nothrow) Pipe();

	if (sprite && sprite->initWithFileName(fileName))
	{
		sprite->autorelease();
		return sprite;
	}
	CC_SAFE_DELETE(sprite);
	return nullptr;
}

Pipe::Pipe()
{
	
}

Pipe::~Pipe()
{
}

bool Pipe::initWithFileName(const std::string & fileName)
{
	if (!CollideableSprite::initWithFileName(fileName))
	{
		return false;
	}

	return true;
}

void Pipe::update(float dt)
{
	CollideableSprite::update(dt);
}

void Pipe::onBeginContactEvent(CollideableSprite * other)
{
}

void Pipe::move(const Vec2 & delta)
{
	setPosition(getPosition() + delta);
}
