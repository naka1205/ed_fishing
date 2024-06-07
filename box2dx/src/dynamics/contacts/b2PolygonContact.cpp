#include "./b2PolygonContact.h"
#include "../../common/b2BlockAllocator.h"
#include "../../collision/b2TimeOfImpact.h"
#include "../b2Body.h"
#include "../b2Fixture.h"
#include "../b2WorldCallbacks.h"

#include <new>

b2Contact* b2PolygonContact::Create(b2Fixture* fixtureA, int32, b2Fixture* fixtureB, int32, b2BlockAllocator* allocator)
{
	void* mem = allocator->Allocate(sizeof(b2PolygonContact));
	return new (mem) b2PolygonContact(fixtureA, fixtureB);
}

void b2PolygonContact::Destroy(b2Contact* contact, b2BlockAllocator* allocator)
{
	((b2PolygonContact*)contact)->~b2PolygonContact();
	allocator->Free(contact, sizeof(b2PolygonContact));
}

b2PolygonContact::b2PolygonContact(b2Fixture* fixtureA, b2Fixture* fixtureB)
	: b2Contact(fixtureA, 0, fixtureB, 0)
{
	b2Assert(m_fixtureA->GetType() == b2Shape::e_polygon);
	b2Assert(m_fixtureB->GetType() == b2Shape::e_polygon);
}

void b2PolygonContact::Evaluate(b2Manifold* manifold, const b2Transform& xfA, const b2Transform& xfB)
{
	b2CollidePolygons(	manifold,
						(b2PolygonShape*)m_fixtureA->GetShape(), xfA,
						(b2PolygonShape*)m_fixtureB->GetShape(), xfB);
}
