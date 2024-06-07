#ifndef B2_CHAIN_SHAPE_H
#define B2_CHAIN_SHAPE_H

#include "./b2Shape.h"

class b2EdgeShape;

/// A chain shape is a free form sequence of line segments.
/// The chain has two-sided collision, so you can use inside and outside collision.
/// Therefore, you may use any winding order.
/// Since there may be many vertices, they are allocated using b2Alloc.
/// Connectivity information is used to create smooth collisions.
/// WARNING: The chain will not collide properly if there are self-intersections.
class b2ChainShape : public b2Shape
{
public:
	b2ChainShape();

	/// The destructor frees the vertices using b2Free.
	~b2ChainShape();

	/// Create a loop. This automatically adjusts connectivity.
	/// @param vertices an array of vertices, these are copied
	/// @param count the vertex count
	void CreateLoop(const b2Vec2* vertices, int32 count);

	/// Create a chain with isolated end vertices.
	/// @param vertices an array of vertices, these are copied
	/// @param count the vertex count
	void CreateChain(const b2Vec2* vertices, int32 count);

	/// Establish connectivity to a vertex that precedes the first vertex.
	/// Don't call this for loops.
	void SetPrevVertex(const b2Vec2& prevVertex);

	/// Establish connectivity to a vertex that follows the last vertex.
	/// Don't call this for loops.
	void SetNextVertex(const b2Vec2& nextVertex);

	/// Implement b2Shape. Vertices are cloned using b2Alloc.
	b2Shape* Clone(b2BlockAllocator* allocator) const;

	/// @see b2Shape::GetChildCount
	int32 GetChildCount() const;

	/// Get a child edge.
	void GetChildEdge(b2EdgeShape* edge, int32 index) const;

	/// This always return false.
	/// @see b2Shape::TestPoint
	bool TestPoint(const b2Transform& transform, const b2Vec2& p) const;

	/// Implement b2Shape.
	bool RayCast(b2RayCastOutput* output, const b2RayCastInput& input,
					const b2Transform& transform, int32 childIndex) const;

	/// @see b2Shape::ComputeAABB
	void ComputeAABB(b2AABB* aabb, const b2Transform& transform, int32 childIndex) const;

	/// Chains have zero mass.
	/// @see b2Shape::ComputeMass
	void ComputeMass(b2MassData* massData, float32 density) const;

	/// The vertices. Owned by this class.
	b2Vec2* m_vertices;

	/// The vertex count.
	int32 m_count;

	b2Vec2 m_prevVertex, m_nextVertex;
	bool m_hasPrevVertex, m_hasNextVertex;
};

inline b2ChainShape::b2ChainShape()
{
	m_type = e_chain;
	m_radius = b2_polygonRadius;
	m_vertices = NULL;
	m_count = 0;
	m_hasPrevVertex = false;
	m_hasNextVertex = false;
}

#endif
