#pragma once

#include "engine/unit/group.h"

#include "widgets.h"

namespace raid {
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

	int m_PartySize = 1;
	char m_NameBuf[16] = {0};
	char m_TagBuf[16] = { 0 };
	char m_TitlePrefixBuf[16] = { 0 };
	char m_TitleSuffixBuf[16] = { 0 };
	bool m_ShowHealthWidgets = false;
};

} // namespace sandbox
} // namespace raid