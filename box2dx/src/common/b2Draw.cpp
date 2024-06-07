#include "./b2Draw.h"

b2Draw::b2Draw()
{
	m_drawFlags = 0;
}

void b2Draw::SetFlags(uint32 flags)
{
	m_drawFlags = flags;
}

uint32 b2Draw::GetFlags() const
{
	return m_drawFlags;
}

void b2Draw::AppendFlags(uint32 flags)
{
	m_drawFlags |= flags;
}

void b2Draw::ClearFlags(uint32 flags)
{
	m_drawFlags &= ~flags;
}
