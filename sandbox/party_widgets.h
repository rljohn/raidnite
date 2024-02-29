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

	raid::Group<8> m_Party;
};

} // namespace sandbox
} // namespace raid