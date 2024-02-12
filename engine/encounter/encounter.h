#pragma once

#include "engine/types.h"
#include "engine/encounter/event.h"
#include "engine/system/time.h"
#include "engine/system/inlist.h"

namespace raid
{

class Encounter
{

public:

	Encounter();
	void Shutdown(EventPool* pool);

	void Begin(const TimeStamp& current = Time::GetCurrent());
	void End(const TimeStamp& current = Time::GetCurrent());
	
	Duration GetDuration(const TimeStamp& current = Time::GetCurrent()) const;

	inlist<EncounterEvent>& GetEvents() { return m_Events; }
	const inlist<EncounterEvent>& GetEvents() const { return m_Events; }

	void AddEvent(EncounterEvent* evt);

private:

	TimeStamp m_StartTime;
	TimeStamp m_EndTime;
	inlist<EncounterEvent> m_Events;
};

} // namespace raid
