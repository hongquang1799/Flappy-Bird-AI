#ifndef my_contact_listener_h
#define my_contact_listener_h

#include "cocos2d.h"
#include "Box2D/Box2D.h"


using namespace cocos2d;
using namespace std;

class b2PhysicsContactListener : public b2ContactListener
{
public:
	b2PhysicsContactListener(){}
	~b2PhysicsContactListener(){}

	virtual void BeginContact(b2Contact* contact);
	virtual void EndContact(b2Contact* contact);
	virtual void PreSolve(b2Contact* contact, const b2Manifold* oldManifold);
};

#endif
