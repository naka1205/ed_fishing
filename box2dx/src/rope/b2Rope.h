#ifndef B2_ROPE_H
#define B2_ROPE_H

#include "../common/b2Math.h"

class b2Draw;

/// 
struct b2RopeDef
{
	b2RopeDef()
	{
		vertices = NULL;
		count = 0;
		masses = NULL;
		gravity.SetZero();
		damping = 0.1f;
		k2 = 0.9f;
		k3 = 0.1f;
	}

	///
	b2Vec2* vertices;

	///
	int32 count;

	///
	float32* masses;

	///
	b2Vec2 gravity;

	///
	float32 damping;

	/// Stretching stiffness
	float32 k2;

	/// Bending stiffness. Values above 0.5 can make the simulation blow up.
	float32 k3;
};

/// 
class b2Rope
{
public:
	b2Rope();
	~b2Rope();

	///
	void Initialize(const b2RopeDef* def);

	///
	void Step(float32 timeStep, int32 iterations);

	///
	int32 GetVertexCount() const
	{
		return m_count;
	}

	///
	const b2Vec2* GetVertices() const
	{
		return m_ps;
	}

	///
	void Draw(b2Draw* draw) const;

	///
	void SetAngle(float32 angle);

private:

	void SolveC2();
	void SolveC3();

	int32 m_count;
	b2Vec2* m_ps;
	b2Vec2* m_p0s;
	b2Vec2* m_vs;

	float32* m_ims;

	float32* m_Ls;
	float32* m_as;

	b2Vec2 m_gravity;
	float32 m_damping;

	float32 m_k2;
	float32 m_k3;
};

#endif
