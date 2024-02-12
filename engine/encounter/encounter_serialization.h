#pragma once

#include "engine/encounter/encounter_log.h"

namespace raid {

class IEncounterSerialization
{
public:

	virtual bool Load(const std::wstring& filename, EncounterLog& encounter) = 0;
	virtual bool Save(const std::wstring& filename, const EncounterLog& encounter) = 0;

};

class EncounterSerialization : public IEncounterSerialization
{
public:

	std::wstring GetDefaultPath();
	bool Load(const std::wstring& filename, EncounterLog& encounter) override;
	bool Save(const std::wstring& filename, const EncounterLog& encounter) override;
};

} // namespace raid