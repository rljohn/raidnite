#pragma once

#include "engine/game/game_events.h"
#include "engine/encounter/event.h"

namespace raid
{

class EncounterData
{
public:

	template<typename T>
	static void PackageData(const T& gameEvent, EncounterEvent& encounterEvent);

	template<typename T>
	static void UnpackageData(const EncounterEvent& encounterEvent, T& gameEvent);

	template<typename T>
	static uint8_t PackageField(const T& source, EncounterField& target, uint8_t offset = 0);

	template<typename T>
	static uint8_t UnpackageField(const EncounterField& source, T& target, uint8_t offset = 0);
};

// Utility Macros
#define DECLARE_ENCOUNTER_FIELD(Type) \
template<> uint8_t EncounterData::PackageField(const Type& source, EncounterField& target, uint8_t offset); \
template<> uint8_t EncounterData::UnpackageField(const EncounterField& source, Type& target, uint8_t offset);

#define DECLARE_ENCOUNTER_DATA(Type) \
template<> void EncounterData::PackageData<Type>(const Type& source, EncounterEvent& target); \
template<> void EncounterData::UnpackageData<Type>(const EncounterEvent& source, Type& target);

// Declare Fields
DECLARE_ENCOUNTER_FIELD(PositionVector2D)
DECLARE_ENCOUNTER_FIELD(bool)

// Declare Events
DECLARE_ENCOUNTER_DATA(EntityOccupancyChangedEvent)
DECLARE_ENCOUNTER_DATA(EntityPositionChangedEvent)
DECLARE_ENCOUNTER_DATA(TilePropertiesChangedEvent);

} // namespace raid