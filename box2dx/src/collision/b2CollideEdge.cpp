#include "./b2Collision.h"
#include "./shapes/b2CircleShape.h"
#include "./shapes/b2EdgeShape.h"
#include "./shapes/b2PolygonShape.h"

void b2CollideEdgeAndCircle(b2Manifold* manifold,
							const b2EdgeShape* edgeA, const b2Transform& xfA,
							const b2CircleShape* circleB, const b2Transform& xfB)
{
	manifold->pointCount = 0;
	
	b2Vec2 Q = b2MulT(xfA, b2Mul(xfB, circleB->m_p));
	
	b2Vec2 A = edgeA->m_vertex1, B = edgeA->m_vertex2;
	b2Vec2 e = B - A;
	
	float32 u = b2Dot(e, B - Q);
	float32 v = b2Dot(e, Q - A);
	
	float32 radius = edgeA->m_radius + circleB->m_radius;
	
	b2ContactFeature cf;
	cf.indexB = 0;
	cf.typeB = b2ContactFeature::e_vertex;

	if (v <= 0.0f)
	{
		b2Vec2 P = A;
		b2Vec2 d = Q - P;
		float32 dd = b2Dot(d, d);
		if (dd > radius * radius)
		{
			return;
		}

		if (edgeA->m_hasVertex0)
		{
			b2Vec2 A1 = edgeA->m_vertex0;
			b2Vec2 B1 = A;
			b2Vec2 e1 = B1 - A1;
			float32 u1 = b2Dot(e1, B1 - Q);
			
			if (u1 > 0.0f)
			{
				return;
			}
		}
		
		cf.indexA = 0;
		cf.typeA = b2ContactFeature::e_vertex;
		manifold->pointCount = 1;
		manifold->type = b2Manifold::e_circles;
		manifold->localNormal.SetZero();
		manifold->localPoint = P;
		manifold->points[0].id.key = 0;
		manifold->points[0].id.cf = cf;
		manifold->points[0].localPoint = circleB->m_p;
		return;
	}

	if (u <= 0.0f)
	{
		b2Vec2 P = B;
		b2Vec2 d = Q - P;
		float32 dd = b2Dot(d, d);
		if (dd > radius * radius)
		{
			return;
		}

		if (edgeA->m_hasVertex3)
		{
			b2Vec2 B2 = edgeA->m_vertex3;
			b2Vec2 A2 = B;
			b2Vec2 e2 = B2 - A2;
			float32 v2 = b2Dot(e2, Q - A2);
			
			if (v2 > 0.0f)
			{
				return;
			}
		}
		
		cf.indexA = 1;
		cf.typeA = b2ContactFeature::e_vertex;
		manifold->pointCount = 1;
		manifold->type = b2Manifold::e_circles;
		manifold->localNormal.SetZero();
		manifold->localPoint = P;
		manifold->points[0].id.key = 0;
		manifold->points[0].id.cf = cf;
		manifold->points[0].localPoint = circleB->m_p;
		return;
	}
	
	float32 den = b2Dot(e, e);
	b2Assert(den > 0.0f);
	b2Vec2 P = (1.0f / den) * (u * A + v * B);
	b2Vec2 d = Q - P;
	float32 dd = b2Dot(d, d);
	if (dd > radius * radius)
	{
		return;
	}
	
	b2Vec2 n(-e.y, e.x);
	if (b2Dot(n, Q - A) < 0.0f)
	{
		n.Set(-n.x, -n.y);
	}
	n.Normalize();
	
	cf.indexA = 0;
	cf.typeA = b2ContactFeature::e_face;
	manifold->pointCount = 1;
	manifold->type = b2Manifold::e_faceA;
	manifold->localNormal = n;
	manifold->localPoint = A;
	manifold->points[0].id.key = 0;
	manifold->points[0].id.cf = cf;
	manifold->points[0].localPoint = circleB->m_p;
}

struct b2EPAxis
{
	enum Type
	{
		e_unknown,
		e_edgeA,
		e_edgeB
	};
	
	Type type;
	int32 index;
	float32 separation;
};

struct b2TempPolygon
{
	b2Vec2 vertices[b2_maxPolygonVertices];
	b2Vec2 normals[b2_maxPolygonVertices];
	int32 count;
};

struct b2ReferenceFace
{
	int32 i1, i2;
	
	b2Vec2 v1, v2;
	
	b2Vec2 normal;
	
	b2Vec2 sideNormal1;
	float32 sideOffset1;
	
	b2Vec2 sideNormal2;
	float32 sideOffset2;
};

struct b2EPCollider
{
	void Collide(b2Manifold* manifold, const b2EdgeShape* edgeA, const b2Transform& xfA,
				 const b2PolygonShape* polygonB, const b2Transform& xfB);
	b2EPAxis ComputeEdgeSeparation();
	b2EPAxis ComputePolygonSeparation();
	
	enum VertexType
	{
		e_isolated,
		e_concave,
		e_convex
	};
	
	b2TempPolygon m_polygonB;
	
	b2Transform m_xf;
	b2Vec2 m_centroidB;
	b2Vec2 m_v0, m_v1, m_v2, m_v3;
	b2Vec2 m_normal0, m_normal1, m_normal2;
	b2Vec2 m_normal;
	VertexType m_type1, m_type2;
	b2Vec2 m_lowerLimit, m_upperLimit;
	float32 m_radius;
	bool m_front;
};

void b2EPCollider::Collide(b2Manifold* manifold, const b2EdgeShape* edgeA, const b2Transform& xfA,
						   const b2PolygonShape* polygonB, const b2Transform& xfB)
{
	m_xf = b2MulT(xfA, xfB);
	
	m_centroidB = b2Mul(m_xf, polygonB->m_centroid);
	
	m_v0 = edgeA->m_vertex0;
	m_v1 = edgeA->m_vertex1;
	m_v2 = edgeA->m_vertex2;
	m_v3 = edgeA->m_vertex3;
	
	bool hasVertex0 = edgeA->m_hasVertex0;
	bool hasVertex3 = edgeA->m_hasVertex3;
	
	b2Vec2 edge1 = m_v2 - m_v1;
	edge1.Normalize();
	m_normal1.Set(edge1.y, -edge1.x);
	float32 offset1 = b2Dot(m_normal1, m_centroidB - m_v1);
	float32 offset0 = 0.0f, offset2 = 0.0f;
	bool convex1 = false, convex2 = false;
	
	if (hasVertex0)
	{
		b2Vec2 edge0 = m_v1 - m_v0;
		edge0.Normalize();
		m_normal0.Set(edge0.y, -edge0.x);
		convex1 = b2Cross(edge0, edge1) >= 0.0f;
		offset0 = b2Dot(m_normal0, m_centroidB - m_v0);
	}

	if (hasVertex3)
	{
		b2Vec2 edge2 = m_v3 - m_v2;
		edge2.Normalize();
		m_normal2.Set(edge2.y, -edge2.x);
		convex2 = b2Cross(edge1, edge2) > 0.0f;
		offset2 = b2Dot(m_normal2, m_centroidB - m_v2);
	}
	
	if (hasVertex0 && hasVertex3)
	{
		if (convex1 && convex2)
		{
			m_front = offset0 >= 0.0f || offset1 >= 0.0f || offset2 >= 0.0f;
			if (m_front)
			{
				m_normal = m_normal1;
				m_lowerLimit = m_normal0;
				m_upperLimit = m_normal2;
			}
			else
			{
				m_normal = -m_normal1;
				m_lowerLimit = -m_normal1;
				m_upperLimit = -m_normal1;
			}
		}
		else if (convex1)
		{
			m_front = offset0 >= 0.0f || (offset1 >= 0.0f && offset2 >= 0.0f);
			if (m_front)
			{
				m_normal = m_normal1;
				m_lowerLimit = m_normal0;
				m_upperLimit = m_normal1;
			}
			else
			{
				m_normal = -m_normal1;
				m_lowerLimit = -m_normal2;
				m_upperLimit = -m_normal1;
			}
		}
		else if (convex2)
		{
			m_front = offset2 >= 0.0f || (offset0 >= 0.0f && offset1 >= 0.0f);
			if (m_front)
			{
				m_normal = m_normal1;
				m_lowerLimit = m_normal1;
				m_upperLimit = m_normal2;
			}
			else
			{
				m_normal = -m_normal1;
				m_lowerLimit = -m_normal1;
				m_upperLimit = -m_normal0;
			}
		}
		else
		{
			m_front = offset0 >= 0.0f && offset1 >= 0.0f && offset2 >= 0.0f;
			if (m_front)
			{
				m_normal = m_normal1;
				m_lowerLimit = m_normal1;
				m_upperLimit = m_normal1;
			}
			else
			{
				m_normal = -m_normal1;
				m_lowerLimit = -m_normal2;
				m_upperLimit = -m_normal0;
			}
		}
	}
	else if (hasVertex0)
	{
		if (convex1)
		{
			m_front = offset0 >= 0.0f || offset1 >= 0.0f;
			if (m_front)
			{
				m_normal = m_normal1;
				m_lowerLimit = m_normal0;
				m_upperLimit = -m_normal1;
			}
			else
			{
				m_normal = -m_normal1;
				m_lowerLimit = m_normal1;
				m_upperLimit = -m_normal1;
			}
		}
		else
		{
			m_front = offset0 >= 0.0f && offset1 >= 0.0f;
			if (m_front)
			{
				m_normal = m_normal1;
				m_lowerLimit = m_normal1;
				m_upperLimit = -m_normal1;
			}
			else
			{
				m_normal = -m_normal1;
				m_lowerLimit = m_normal1;
				m_upperLimit = -m_normal0;
			}
		}
	}
	else if (hasVertex3)
	{
		if (convex2)
		{
			m_front = offset1 >= 0.0f || offset2 >= 0.0f;
			if (m_front)
			{
				m_normal = m_normal1;
				m_lowerLimit = -m_normal1;
				m_upperLimit = m_normal2;
			}
			else
			{
				m_normal = -m_normal1;
				m_lowerLimit = -m_normal1;
				m_upperLimit = m_normal1;
			}
		}
		else
		{
			m_front = offset1 >= 0.0f && offset2 >= 0.0f;
			if (m_front)
			{
				m_normal = m_normal1;
				m_lowerLimit = -m_normal1;
				m_upperLimit = m_normal1;
			}
			else
			{
				m_normal = -m_normal1;
				m_lowerLimit = -m_normal2;
				m_upperLimit = m_normal1;
			}
		}		
	}
	else
	{
		m_front = offset1 >= 0.0f;
		if (m_front)
		{
			m_normal = m_normal1;
			m_lowerLimit = -m_normal1;
			m_upperLimit = -m_normal1;
		}
		else
		{
			m_normal = -m_normal1;
			m_lowerLimit = m_normal1;
			m_upperLimit = m_normal1;
		}
	}
	
	m_polygonB.count = polygonB->m_count;
	for (int32 i = 0; i < polygonB->m_count; ++i)
	{
		m_polygonB.vertices[i] = b2Mul(m_xf, polygonB->m_vertices[i]);
		m_polygonB.normals[i] = b2Mul(m_xf.q, polygonB->m_normals[i]);
	}
	
	m_radius = 2.0f * b2_polygonRadius;
	
	manifold->pointCount = 0;
	
	b2EPAxis edgeAxis = ComputeEdgeSeparation();
	
	if (edgeAxis.type == b2EPAxis::e_unknown)
	{
		return;
	}
	
	if (edgeAxis.separation > m_radius)
	{
		return;
	}
	
	b2EPAxis polygonAxis = ComputePolygonSeparation();
	if (polygonAxis.type != b2EPAxis::e_unknown && polygonAxis.separation > m_radius)
	{
		return;
	}
	
	const float32 k_relativeTol = 0.98f;
	const float32 k_absoluteTol = 0.001f;
	
	b2EPAxis primaryAxis;
	if (polygonAxis.type == b2EPAxis::e_unknown)
	{
		primaryAxis = edgeAxis;
	}
	else if (polygonAxis.separation > k_relativeTol * edgeAxis.separation + k_absoluteTol)
	{
		primaryAxis = polygonAxis;
	}
	else
	{
		primaryAxis = edgeAxis;
	}
	
	b2ClipVertex ie[2];
	b2ReferenceFace rf;
	if (primaryAxis.type == b2EPAxis::e_edgeA)
	{
		manifold->type = b2Manifold::e_faceA;
		
		int32 bestIndex = 0;
		float32 bestValue = b2Dot(m_normal, m_polygonB.normals[0]);
		for (int32 i = 1; i < m_polygonB.count; ++i)
		{
			float32 value = b2Dot(m_normal, m_polygonB.normals[i]);
			if (value < bestValue)
			{
				bestValue = value;
				bestIndex = i;
			}
		}
		
		int32 i1 = bestIndex;
		int32 i2 = i1 + 1 < m_polygonB.count ? i1 + 1 : 0;
		
		ie[0].v = m_polygonB.vertices[i1];
		ie[0].id.cf.indexA = 0;
		ie[0].id.cf.indexB = static_cast<uint8>(i1);
		ie[0].id.cf.typeA = b2ContactFeature::e_face;
		ie[0].id.cf.typeB = b2ContactFeature::e_vertex;
		
		ie[1].v = m_polygonB.vertices[i2];
		ie[1].id.cf.indexA = 0;
		ie[1].id.cf.indexB = static_cast<uint8>(i2);
		ie[1].id.cf.typeA = b2ContactFeature::e_face;
		ie[1].id.cf.typeB = b2ContactFeature::e_vertex;
		
		if (m_front)
		{
			rf.i1 = 0;
			rf.i2 = 1;
			rf.v1 = m_v1;
			rf.v2 = m_v2;
			rf.normal = m_normal1;
		}
		else
		{
			rf.i1 = 1;
			rf.i2 = 0;
			rf.v1 = m_v2;
			rf.v2 = m_v1;
			rf.normal = -m_normal1;
		}		
	}
	else
	{
		manifold->type = b2Manifold::e_faceB;
		
		ie[0].v = m_v1;
		ie[0].id.cf.indexA = 0;
		ie[0].id.cf.indexB = static_cast<uint8>(primaryAxis.index);
		ie[0].id.cf.typeA = b2ContactFeature::e_vertex;
		ie[0].id.cf.typeB = b2ContactFeature::e_face;
		
		ie[1].v = m_v2;
		ie[1].id.cf.indexA = 0;
		ie[1].id.cf.indexB = static_cast<uint8>(primaryAxis.index);		
		ie[1].id.cf.typeA = b2ContactFeature::e_vertex;
		ie[1].id.cf.typeB = b2ContactFeature::e_face;
		
		rf.i1 = primaryAxis.index;
		rf.i2 = rf.i1 + 1 < m_polygonB.count ? rf.i1 + 1 : 0;
		rf.v1 = m_polygonB.vertices[rf.i1];
		rf.v2 = m_polygonB.vertices[rf.i2];
		rf.normal = m_polygonB.normals[rf.i1];
	}
	
	rf.sideNormal1.Set(rf.normal.y, -rf.normal.x);
	rf.sideNormal2 = -rf.sideNormal1;
	rf.sideOffset1 = b2Dot(rf.sideNormal1, rf.v1);
	rf.sideOffset2 = b2Dot(rf.sideNormal2, rf.v2);
	
	b2ClipVertex clipPoints1[2];
	b2ClipVertex clipPoints2[2];
	int32 np;
	
	np = b2ClipSegmentToLine(clipPoints1, ie, rf.sideNormal1, rf.sideOffset1, rf.i1);
	
	if (np < b2_maxManifoldPoints)
	{
		return;
	}
	
	np = b2ClipSegmentToLine(clipPoints2, clipPoints1, rf.sideNormal2, rf.sideOffset2, rf.i2);
	
	if (np < b2_maxManifoldPoints)
	{
		return;
	}
	
	if (primaryAxis.type == b2EPAxis::e_edgeA)
	{
		manifold->localNormal = rf.normal;
		manifold->localPoint = rf.v1;
	}
	else
	{
		manifold->localNormal = polygonB->m_normals[rf.i1];
		manifold->localPoint = polygonB->m_vertices[rf.i1];
	}
	
	int32 pointCount = 0;
	for (int32 i = 0; i < b2_maxManifoldPoints; ++i)
	{
		float32 separation;
		
		separation = b2Dot(rf.normal, clipPoints2[i].v - rf.v1);
		
		if (separation <= m_radius)
		{
			b2ManifoldPoint* cp = manifold->points + pointCount;
			
			if (primaryAxis.type == b2EPAxis::e_edgeA)
			{
				cp->localPoint = b2MulT(m_xf, clipPoints2[i].v);
				cp->id = clipPoints2[i].id;
			}
			else
			{
				cp->localPoint = clipPoints2[i].v;
				cp->id.cf.typeA = clipPoints2[i].id.cf.typeB;
				cp->id.cf.typeB = clipPoints2[i].id.cf.typeA;
				cp->id.cf.indexA = clipPoints2[i].id.cf.indexB;
				cp->id.cf.indexB = clipPoints2[i].id.cf.indexA;
			}
			
			++pointCount;
		}
	}
	
	manifold->pointCount = pointCount;
}

b2EPAxis b2EPCollider::ComputeEdgeSeparation()
{
	b2EPAxis axis;
	axis.type = b2EPAxis::e_edgeA;
	axis.index = m_front ? 0 : 1;
	axis.separation = FLT_MAX;
	
	for (int32 i = 0; i < m_polygonB.count; ++i)
	{
		float32 s = b2Dot(m_normal, m_polygonB.vertices[i] - m_v1);
		if (s < axis.separation)
		{
			axis.separation = s;
		}
	}
	
	return axis;
}

b2EPAxis b2EPCollider::ComputePolygonSeparation()
{
	b2EPAxis axis;
	axis.type = b2EPAxis::e_unknown;
	axis.index = -1;
	axis.separation = -FLT_MAX;

	b2Vec2 perp(-m_normal.y, m_normal.x);

	for (int32 i = 0; i < m_polygonB.count; ++i)
	{
		b2Vec2 n = -m_polygonB.normals[i];
		
		float32 s1 = b2Dot(n, m_polygonB.vertices[i] - m_v1);
		float32 s2 = b2Dot(n, m_polygonB.vertices[i] - m_v2);
		float32 s = b2Min(s1, s2);
		
		if (s > m_radius)
		{
			axis.type = b2EPAxis::e_edgeB;
			axis.index = i;
			axis.separation = s;
			return axis;
		}
		
		if (b2Dot(n, perp) >= 0.0f)
		{
			if (b2Dot(n - m_upperLimit, m_normal) < -b2_angularSlop)
			{
				continue;
			}
		}
		else
		{
			if (b2Dot(n - m_lowerLimit, m_normal) < -b2_angularSlop)
			{
				continue;
			}
		}
		
		if (s > axis.separation)
		{
			axis.type = b2EPAxis::e_edgeB;
			axis.index = i;
			axis.separation = s;
		}
	}
	
	return axis;
}

void b2CollideEdgeAndPolygon(	b2Manifold* manifold,
							 const b2EdgeShape* edgeA, const b2Transform& xfA,
							 const b2PolygonShape* polygonB, const b2Transform& xfB)
{
	b2EPCollider collider;
	collider.Collide(manifold, edgeA, xfA, polygonB, xfB);
}
