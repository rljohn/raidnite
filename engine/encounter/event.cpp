#include "engine/pch.h"

#include "engine/entity/entity.h"
#include "event.h"

namespace raid
{

EncounterEvent::EncounterEvent()
	: m_Type(EncounterEventType::Invalid)
	, m_Frame(0)
	, m_Source(InvalidPlayerId)
	, m_Target(InvalidPlayerId)
	, m_Node(this)
{
	memset(&m_ExtraData1, 0, sizeof(m_ExtraData1));
	memset(&m_ExtraData2, 0, sizeof(m_ExtraData2));
}

void EncounterEvents::OnEntityCreated(EncounterEvent& event, Entity& entity)
{
	event.m_Type = EncounterEventType::EntityCreated;

	// TODO:
	// Pack data
}

} // namespace raid
