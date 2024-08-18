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

// PositionVector2D
template<> uint8_t EncounterData::PackageField(const PositionVector2D& source, EncounterField& target, uint8_t offset);
template<> uint8_t EncounterData::UnpackageField(const EncounterField& source, PositionVector2D& target, uint8_t offset);

// Unit Position Changed
template<> void EncounterData::PackageData<UnitPositionChangedEvent>(const UnitPositionChangedEvent& gameEvent, EncounterEvent& encounterEvent);
template<> void EncounterData::UnpackageData<UnitPositionChangedEvent>(const EncounterEvent& encounterEvent, UnitPositionChangedEvent& gameEvent);

} // namespace raid