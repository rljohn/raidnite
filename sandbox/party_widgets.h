#pragma once

#include "engine/unit/group.h"

#include "widgets.h"

namespace raid {
class Map;

namespace sandbox {
class PartyWidget : public Widget
{
public:

	void Init() override;
	void Draw(GameSandbox* sandbox) override;
	void Shutdown() override;

	const char* GetName() override { return "Party"; }

private:

	void DrawPartyWidgets(GameSandbox* sandbox);
	void AddRandomUnit(GameSandbox* sandbox, int numUnits);

	void AddUnit(GameSandbox* sandbox, Map* map, const Position& spawnPos, const char* name, 
				 const char* tag, const char* prefix, const char* suffix);

	int m_PartySize = 1;
	char m_NameBuf[16] = {0};
	char m_TagBuf[16] = { 0 };
	char m_TitlePrefixBuf[16] = { 0 };
	char m_TitleSuffixBuf[16] = { 0 };
	bool m_ShowHealthWidgets = false;
};

} // namespace sandbox
} // namespace raid