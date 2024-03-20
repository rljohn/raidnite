#pragma once

#include "widgets.h"

namespace raid {
namespace sandbox {

class LogWidget : public Widget
{
public:

	void Init() override;
	void Draw(GameSandbox* sandbox) override;
	void Shutdown() override;

	const char* GetName() override { return "Log"; }

private:

	int m_EncounterIndex;
};

} // namespace sandbox
} // namespace raid