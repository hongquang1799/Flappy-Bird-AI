#include "b2PhysicsContactListener.h"
#include "cocos2d.h"
#include "CollideableSprite.h"

void b2PhysicsContactListener::BeginContact(b2Contact* contact)
{
	CollideableSprite * sprite1 = (CollideableSprite *)contact->GetFixtureA()->GetBody()->GetUserData();
	CollideableSprite * sprite2 = (CollideableSprite *)contact->GetFixtureB()->GetBody()->GetUserData();

	if (sprite1->bGenerateContactEvent) sprite1->onBeginContactEvent(sprite2);
	if (sprite2->bGenerateContactEvent) sprite2->onBeginContactEvent(sprite1);
}

void b2PhysicsContactListener::EndContact(b2Contact* contact)
{
}

void b2PhysicsContactListener::PreSolve(b2Contact* contact, const b2Manifold* oldManifold)
{
}

