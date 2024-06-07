#include "../../common//b2BlockAllocator.h"
#include "../../collision//b2TimeOfImpact.h"
#include "..//b2WorldCallbacks.h"
#include "../b2Fixture.h"
#include "../b2Body.h"
#include "./b2CircleContact.h"

#include <new>

b2Contact* b2CircleContact::Create(b2Fixture* fixtureA, int32, b2Fixture* fixtureB, int32, b2BlockAllocator* allocator)
{
	void* mem = allocator->Allocate(sizeof(b2CircleContact));
	return new (mem) b2CircleContact(fixtureA, fixtureB);
}

void b2CircleContact::Destroy(b2Contact* contact, b2BlockAllocator* allocator)
{
	((b2CircleContact*)contact)->~b2CircleContact();
	allocator->Free(contact, sizeof(b2CircleContact));
}

b2CircleContact::b2CircleContact(b2Fixture* fixtureA, b2Fixture* fixtureB)
	: b2Contact(fixtureA, 0, fixtureB, 0)
{
	b2Assert(m_fixtureA->GetType() == b2Shape::e_circle);
	b2Assert(m_fixtureB->GetType() == b2Shape::e_circle);
}

void b2CircleContact::Evaluate(b2Manifold* manifold, const b2Transform& xfA, const b2Transform& xfB)
{
	b2CollideCircles(manifold,
					(b2CircleShape*)m_fixtureA->GetShape(), xfA,
					(b2CircleShape*)m_fixtureB->GetShape(), xfB);
}
