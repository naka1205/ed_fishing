#ifndef B2_FRICTION_JOINT_H
#define B2_FRICTION_JOINT_H

#include "./b2Joint.h"

/// Friction joint definition.
struct b2FrictionJointDef : public b2JointDef
{
	b2FrictionJointDef()
	{
		type = e_frictionJoint;
		localAnchorA.SetZero();
		localAnchorB.SetZero();
		maxForce = 0.0f;
		maxTorque = 0.0f;
	}

	/// Initialize the bodies, anchors, axis, and reference angle using the world
	/// anchor and world axis.
	void Initialize(b2Body* bodyA, b2Body* bodyB, const b2Vec2& anchor);

	/// The local anchor point relative to bodyA's origin.
	b2Vec2 localAnchorA;

	/// The local anchor point relative to bodyB's origin.
	b2Vec2 localAnchorB;

	/// The maximum friction force in N.
	float32 maxForce;

	/// The maximum friction torque in N-m.
	float32 maxTorque;
};

/// Friction joint. This is used for top-down friction.
/// It provides 2D translational friction and angular friction.
class b2FrictionJoint : public b2Joint
{
public:
	b2Vec2 GetAnchorA() const;
	b2Vec2 GetAnchorB() const;

	b2Vec2 GetReactionForce(float32 inv_dt) const;
	float32 GetReactionTorque(float32 inv_dt) const;

	/// The local anchor point relative to bodyA's origin.
	const b2Vec2& GetLocalAnchorA() const { return m_localAnchorA; }

	/// The local anchor point relative to bodyB's origin.
	const b2Vec2& GetLocalAnchorB() const  { return m_localAnchorB; }

	/// Set the maximum friction force in N.
	void SetMaxForce(float32 force);

	/// Get the maximum friction force in N.
	float32 GetMaxForce() const;

	/// Set the maximum friction torque in N*m.
	void SetMaxTorque(float32 torque);

	/// Get the maximum friction torque in N*m.
	float32 GetMaxTorque() const;

	/// Dump joint to dmLog
	void Dump();

protected:

	friend class b2Joint;

	b2FrictionJoint(const b2FrictionJointDef* def);

	void InitVelocityConstraints(const b2SolverData& data);
	void SolveVelocityConstraints(const b2SolverData& data);
	bool SolvePositionConstraints(const b2SolverData& data);

	b2Vec2 m_localAnchorA;
	b2Vec2 m_localAnchorB;

	// Solver shared
	b2Vec2 m_linearImpulse;
	float32 m_angularImpulse;
	float32 m_maxForce;
	float32 m_maxTorque;

	// Solver temp
	int32 m_indexA;
	int32 m_indexB;
	b2Vec2 m_rA;
	b2Vec2 m_rB;
	b2Vec2 m_localCenterA;
	b2Vec2 m_localCenterB;
	float32 m_invMassA;
	float32 m_invMassB;
	float32 m_invIA;
	float32 m_invIB;
	b2Mat22 m_linearMass;
	float32 m_angularMass;
};

#endif
