#ifndef B2_CONTACT_SOLVER_H
#define B2_CONTACT_SOLVER_H

#include "../../common/b2Math.h"
#include "../../collision/b2Collision.h"
#include "../b2TimeStep.h"

class b2Contact;
class b2Body;
class b2StackAllocator;
struct b2ContactPositionConstraint;

struct b2VelocityConstraintPoint
{
	b2Vec2 rA;
	b2Vec2 rB;
	float32 normalImpulse;
	float32 tangentImpulse;
	float32 normalMass;
	float32 tangentMass;
	float32 velocityBias;
};

struct b2ContactVelocityConstraint
{
	b2VelocityConstraintPoint points[b2_maxManifoldPoints];
	b2Vec2 normal;
	b2Mat22 normalMass;
	b2Mat22 K;
	int32 indexA;
	int32 indexB;
	float32 invMassA, invMassB;
	float32 invIA, invIB;
	float32 friction;
	float32 restitution;
	float32 tangentSpeed;
	int32 pointCount;
	int32 contactIndex;
};

struct b2ContactSolverDef
{
	b2TimeStep step;
	b2Contact** contacts;
	int32 count;
	b2Position* positions;
	b2Velocity* velocities;
	b2StackAllocator* allocator;
};

class b2ContactSolver
{
public:
	b2ContactSolver(b2ContactSolverDef* def);
	~b2ContactSolver();

	void InitializeVelocityConstraints();

	void WarmStart();
	void SolveVelocityConstraints();
	void StoreImpulses();

	bool SolvePositionConstraints();
	bool SolveTOIPositionConstraints(int32 toiIndexA, int32 toiIndexB);

	b2TimeStep m_step;
	b2Position* m_positions;
	b2Velocity* m_velocities;
	b2StackAllocator* m_allocator;
	b2ContactPositionConstraint* m_positionConstraints;
	b2ContactVelocityConstraint* m_velocityConstraints;
	b2Contact** m_contacts;
	int m_count;
};

#endif

