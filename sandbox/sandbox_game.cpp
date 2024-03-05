#include "sandbox/pch.h"
#include "sandbox_game.h"

// imgui
#include "thirdparty/imgui/imgui.h"

namespace raid {
namespace sandbox {

using namespace raid;

void GameSandbox::Init()
{
	// 60 FPS update loop
	Nanoseconds timeStep(16666666);
	m_Engine.Init(timeStep);

	Game::SetEntityManager(&m_World);
	Game::SetEncounterLog(&m_EncounterLog);
	Game::SetDamageCalculator(&m_DamageCalculator);
	Game::SetCombatSystem(&m_CombatSystem);

	m_Widgets.push_back(&m_EngineWidgets);
	m_Widgets.push_back(&m_MapWidgets);
	m_Widgets.push_back(&m_PartyWidgets);

	for (Widget* w : m_Widgets)
	{
		w->Init();
	}
}

void GameSandbox::Update()
{
	m_Engine.Update();

	for (Widget* w : m_Widgets)
	{
		if (ImGui::Begin(w->GetName()))
		{
			ImGui::PushID(w->GetName());
			w->Draw(this);
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

	m_Engine.Shutdown();
}

} // namespace sandbox
} // namespace raid