#pragma once

#include "engine/unit/group.h"

#include "widgets.h"

namespace raid {
namespace sandbox {

class PartyWidget : public Widget
{
public:

	void Init() override;
	void Draw() override;
	void Shutdown() override;

	const char* GetName() override { return "Party"; }

private:

	void DrawPartyWidgets();

	raid::Group m_Party;
	int m_PartySize = 8;
};

} // namespace sandbox
} // namespace raid