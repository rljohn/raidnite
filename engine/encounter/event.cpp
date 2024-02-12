#include "engine/pch.h"
#include "event.h"

namespace raid
{

EncounterEvent::EncounterEvent()
	: m_Type(EncounterEventType::Invalid)
	, m_Time()
	, m_Source(InvalidPlayerId)
	, m_Target(InvalidPlayerId)
	, m_Node(this)
{
	memset(&m_ExtraData1, 0, sizeof(m_ExtraData1));
	memset(&m_ExtraData2, 0, sizeof(m_ExtraData2));
}

} // namespace raid
