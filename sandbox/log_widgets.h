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
	void DrawEvent(IEncounterLog* log, const EncounterEvent& evt) const;
	void Shutdown() override;

	const char* GetName() override { return "Event Log"; }

private:

	void DrawFilters();

	int m_EncounterIndex = 0;
	bool m_Filter[EncounterEventType::MAX];
};

} // namespace sandbox
} // namespace raid