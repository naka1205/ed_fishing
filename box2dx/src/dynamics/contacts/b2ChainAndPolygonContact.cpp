#include "./b2ChainAndPolygonContact.h"
#include "../b2Fixture.h"
#include "../../common/b2BlockAllocator.h"
#include "../../collision/shapes/b2ChainShape.h"
#include "../../collision/shapes/b2EdgeShape.h"


#include <new>

b2Contact* b2ChainAndPolygonContact::Create(b2Fixture* fixtureA, int32 indexA, b2Fixture* fixtureB, int32 indexB, b2BlockAllocator* allocator)
{
	void* mem = allocator->Allocate(sizeof(b2ChainAndPolygonContact));
	return new (mem) b2ChainAndPolygonContact(fixtureA, indexA, fixtureB, indexB);
}

void b2ChainAndPolygonContact::Destroy(b2Contact* contact, b2BlockAllocator* allocator)
{
	((b2ChainAndPolygonContact*)contact)->~b2ChainAndPolygonContact();
	allocator->Free(contact, sizeof(b2ChainAndPolygonContact));
}

b2ChainAndPolygonContact::b2ChainAndPolygonContact(b2Fixture* fixtureA, int32 indexA, b2Fixture* fixtureB, int32 indexB)
: b2Contact(fixtureA, indexA, fixtureB, indexB)
{
	b2Assert(m_fixtureA->GetType() == b2Shape::e_chain);
	b2Assert(m_fixtureB->GetType() == b2Shape::e_polygon);
}

void b2ChainAndPolygonContact::Evaluate(b2Manifold* manifold, const b2Transform& xfA, const b2Transform& xfB)
{
	b2ChainShape* chain = (b2ChainShape*)m_fixtureA->GetShape();
	b2EdgeShape edge;
	chain->GetChildEdge(&edge, m_indexA);
	b2CollideEdgeAndPolygon(	manifold, &edge, xfA,
								(b2PolygonShape*)m_fixtureB->GetShape(), xfB);
}
