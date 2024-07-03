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

	for (const Encounter* e : encounters)
	{
		EncounterLog::DisplayString buffer;
		log->GetDisplayString(*e, buffer);

		if (ImGui::TreeNode(buffer))
		{
			for (const auto& evt : e->GetEvents())
			{
				
			}

			ImGui::TreePop();
		}
	}
	
}

void LogWidget::Shutdown()
{
}

} // namespace sandbox
} // namespace raid