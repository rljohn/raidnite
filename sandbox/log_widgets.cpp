#include "sandbox/pch.h"
#include "sandbox_game.h"

// raid
#include "engine/encounter/encounter_log.h"
#include "engine/encounter/encounter.h"

// sandbox
#include "log_widgets.h"

#include <algorithm>

namespace raid {
namespace sandbox {

void LogWidget::Init()
{
	SetEnabled(true);
}

void LogWidget::Draw(GameSandbox* sandbox)
{
	IEncounterLog* log = raid::Game::GetEncounterLog();
	if (!log)
		return;

	const std::vector<Encounter*>& encounters = log->GetEncounterList();
	if (encounters.size() == 0)
		return;

	int count = (int)(encounters.size() - 1);
	m_EncounterIndex = std::clamp(m_EncounterIndex, 0, count);

	int idx = 0;
	const char* previewValue = "";
	if (ImGui::BeginCombo("Encounters", previewValue))
	{
		for (Encounter* encounter : encounters)
		{
			
			idx++;
		}

		ImGui::EndCombo();
	}

	Encounter* encounter = encounters[m_EncounterIndex];
	
}

void LogWidget::Shutdown()
{
}

} // namespace sandbox
} // namespace raid