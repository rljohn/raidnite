#pragma once

#include "widgets.h"

#include "engine/types.h"

namespace raid {
namespace sandbox {

class UnitWidget : public Widget
{
public:

	void Init() override;
	void Draw(GameSandbox* sandbox) override;
	void Shutdown() override;

	const char* GetName() override { return "Units"; }

private:

	std::vector<std::string> m_UnitList;
	int m_UnitListIdx = 0;

	raid::Position m_SpawnPosition;
};

} // namespace sandbox
} // namespace raid