#ifndef PIPE_H
#define PIPE_H

#include "CollideableSprite.h"

class Pipe : public CollideableSprite
{
public:
	static Pipe * createWithFileName(const std::string& fileName);

	Pipe();
	virtual ~Pipe();

	void update(float dt) override;

	virtual void onBeginContactEvent(CollideableSprite * other);

	virtual void move(const Vec2& delta);
protected:
	virtual bool initWithFileName(const std::string& fileName);
};

#endif // __HELLOWORLD_SCENE_H__
