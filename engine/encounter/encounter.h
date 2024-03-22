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

	void Begin(const Frame frame);
	void End(const Frame frame);
	
	Frame GetDuration(const Frame current) const;

	inlist<EncounterEvent>& GetEvents() { return m_Events; }
	const inlist<EncounterEvent>& GetEvents() const { return m_Events; }

	void AddEvent(EncounterEvent* evt);

private:

	void UpdateName();

	char m_Name[64];
	Frame m_StartFrame;
	Frame m_EndFrame;
	inlist<EncounterEvent> m_Events;
};

} // namespace raid
