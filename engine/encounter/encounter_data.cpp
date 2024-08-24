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

// bool
template<>
uint8_t EncounterData::PackageField(const bool& source, EncounterField& target, uint8_t offset)
{
	uint8_t* ptr = target.Buffer;
	size_t size = sizeof(uint8_t);

	uint8_t val = source ? 1 : 0;
	memcpy(ptr + offset, &val, size);
	offset += (uint8_t)size;

	return offset;
}

template<>
uint8_t EncounterData::UnpackageField(const EncounterField& source, bool& target, uint8_t offset)
{
	const uint8_t* ptr = source.Buffer;
	size_t size = sizeof(uint8_t);

	uint8_t val;
	memcpy(&val, ptr + offset, size);
	target = (val == 1);
	offset += (uint8_t)size;
	return offset;
}

// UnitPositionChangedEvent
template<>
void EncounterData::PackageData<EntityPositionChangedEvent>(const EntityPositionChangedEvent& gameEvent, EncounterEvent& encounterEvent)
{
	encounterEvent.m_Source = gameEvent.GetEntity()->GetId();

	uint8_t offset = 0;
	offset = PackageField(gameEvent.m_Previous, encounterEvent.m_ExtraData1, offset);
	offset = PackageField(gameEvent.m_Position, encounterEvent.m_ExtraData1, offset);
}

template<>
void EncounterData::UnpackageData<EntityPositionChangedEvent>(const EncounterEvent& encounterEvent, EntityPositionChangedEvent& gameEvent)
{
	uint8_t offset = 0;
	offset = UnpackageField(encounterEvent.m_ExtraData1, gameEvent.m_Previous, offset);
	offset = UnpackageField(encounterEvent.m_ExtraData1, gameEvent.m_Position, offset);
	gameEvent.SetEntity(Game::GetEntityManager()->FindEntity(encounterEvent.m_Source));
}

// UnitOccupancyChangedEvent
template<>
void EncounterData::PackageData<EntityOccupancyChangedEvent>(const EntityOccupancyChangedEvent& gameEvent, EncounterEvent& encounterEvent)
{
	encounterEvent.m_Source = gameEvent.GetEntity() ? gameEvent.GetEntity()->GetId() : InvalidEntityId;

	uint8_t offset = 0;
	offset = PackageField(gameEvent.m_Position, encounterEvent.m_ExtraData1, offset);
}

template<>
void EncounterData::UnpackageData<EntityOccupancyChangedEvent>(const EncounterEvent& encounterEvent, EntityOccupancyChangedEvent& gameEvent)
{
	uint8_t offset = 0;
	offset = UnpackageField(encounterEvent.m_ExtraData1, gameEvent.m_Position, offset);
	gameEvent.SetEntity(Game::GetEntityManager()->FindEntity(encounterEvent.m_Source));
}

template<>
void EncounterData::PackageData<TilePropertiesChangedEvent>(const TilePropertiesChangedEvent& gameEvent, EncounterEvent& encounterEvent)
{
	uint8_t offset = 0;
	offset = PackageField(gameEvent.m_Position, encounterEvent.m_ExtraData1, offset);
	offset = PackageField(gameEvent.m_AllowsMovement, encounterEvent.m_ExtraData1, offset);
	offset = PackageField(gameEvent.m_AllowsOccupancy, encounterEvent.m_ExtraData1, offset);
}

template<>
void EncounterData::UnpackageData<TilePropertiesChangedEvent>(const EncounterEvent& encounterEvent, TilePropertiesChangedEvent& gameEvent)
{
	uint8_t offset = 0;
	offset = UnpackageField(encounterEvent.m_ExtraData1, gameEvent.m_Position, offset);
	offset = UnpackageField(encounterEvent.m_ExtraData1, gameEvent.m_AllowsMovement, offset);
	offset = UnpackageField(encounterEvent.m_ExtraData1, gameEvent.m_AllowsOccupancy, offset);
}


} // namespace raid