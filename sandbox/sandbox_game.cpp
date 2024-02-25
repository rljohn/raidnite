#include "sandbox/pch.h"
#include "sandbox_game.h"

// imgui
#include "thirdparty/imgui/imgui.h"

namespace raid {
namespace sandbox {

using namespace raid;

void GameSandbox::Init()
{
	Game::SetEntityManager(&m_World);
	Game::SetEncounterLog(&m_EncounterLog);
	Game::SetDamageCalculator(&m_DamageCalculator);
	Game::SetCombatSystem(&m_CombatSystem);

	m_Widgets.push_back(&m_MapWidgets);

	for (Widget* w : m_Widgets)
	{
		w->Init();
	}
}

void GameSandbox::Update()
{
	for (Widget* w : m_Widgets)
	{
		if (ImGui::Begin(w->GetName()))
		{
			ImGui::PushID(w->GetName());
			w->Draw();
			ImGui::PopID();

			ImGui::End();
		}
	}
}

void GameSandbox::Shutdown()
{
	for (Widget* w : m_Widgets)
	{
		w->Shutdown();
	}

	Game::SetEntityManager(nullptr);
	Game::SetEncounterLog(nullptr);
	Game::SetDamageCalculator(nullptr);
	Game::SetCombatSystem(nullptr);
}

} // namespace sandbox
} // namespace raid