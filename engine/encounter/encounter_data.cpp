#include "engine/pch.h"
#include "encounter_data.h"

namespace raid
{

// PositionVector2D
template<> 
uint8_t EncounterData::PackageField(const PositionVector2D& source, EncounterField& target, uint8_t offset)
{
	uint8_t* ptr = target.Buffer;
	size_t size = PositionVector2D::SizeOfT;

	memcpy(ptr + offset, &source.GetX(), size); 
	offset += (uint8_t)size;
	memcpy(ptr + offset, &source.GetY(), size);
	offset += (uint8_t)size;

	return offset;
}

template<> 
uint8_t EncounterData::UnpackageField(const EncounterField& source, PositionVector2D& target, uint8_t offset)
{
	const uint8_t* ptr = source.Buffer;
	size_t size = PositionVector2D::SizeOfT;

	uint16_t x, y;
	memcpy(&x, ptr + offset, size);
	offset += (uint8_t)size;
	memcpy(&y, ptr + offset, size);
	offset += (uint8_t)size;

	target.SetXY(x, y);
	return offset;
}

// UnitPositionChangedEvent
template<>
void EncounterData::PackageData<UnitPositionChangedEvent>(const UnitPositionChangedEvent& gameEvent, EncounterEvent& encounterEvent)
{
	encounterEvent.m_Source = gameEvent.GetEntity()->GetId();

	uint8_t offset = 0;
	offset = PackageField(gameEvent.m_Previous, encounterEvent.m_ExtraData1, offset);
	offset = PackageField(gameEvent.m_Position, encounterEvent.m_ExtraData1, offset);
}

template<>
void EncounterData::UnpackageData<UnitPositionChangedEvent>(const EncounterEvent& encounterEvent, UnitPositionChangedEvent& gameEvent)
{
	uint8_t offset = 0;
	offset = UnpackageField(encounterEvent.m_ExtraData1, gameEvent.m_Previous, offset);
	offset = UnpackageField(encounterEvent.m_ExtraData1, gameEvent.m_Position, offset);
	gameEvent.SetEntity(Game::GetEntityManager()->FindEntity(encounterEvent.m_Source));
}

} // namespace raid