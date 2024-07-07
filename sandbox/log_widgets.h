#pragma once

#include "widgets.h"

namespace raid {

struct EncounterEvent;
class IEncounerLog;

namespace sandbox {

class LogWidget : public Widget
{
public:

	void Init() override;
	void Draw(GameSandbox* sandbox) override;
	void DrawEvent(IEncounterLog* log, const EncounterEvent& evt);
	void Shutdown() override;

	const char* GetName() override { return "Event Log"; }

private:

	int m_EncounterIndex;
};

} // namespace sandbox
} // namespace raid