#ifndef B2_CHAIN_AND_CIRCLE_CONTACT_H
#define B2_CHAIN_AND_CIRCLE_CONTACT_H

#include "./b2Contact.h"

class b2BlockAllocator;

class b2ChainAndCircleContact : public b2Contact
{
public:
	static b2Contact* Create(	b2Fixture* fixtureA, int32 indexA,
								b2Fixture* fixtureB, int32 indexB, b2BlockAllocator* allocator);
	static void Destroy(b2Contact* contact, b2BlockAllocator* allocator);

	b2ChainAndCircleContact(b2Fixture* fixtureA, int32 indexA, b2Fixture* fixtureB, int32 indexB);
	~b2ChainAndCircleContact() {}

	void Evaluate(b2Manifold* manifold, const b2Transform& xfA, const b2Transform& xfB);
};

#endif
