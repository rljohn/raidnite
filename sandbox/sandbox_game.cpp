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
}

void GameSandbox::Update()
{
	if (ImGui::Begin("Test"))
	{
		ImGui::End();
	}
}

void GameSandbox::Shutdown()
{
	Game::SetEntityManager(nullptr);
	Game::SetEncounterLog(nullptr);
	Game::SetDamageCalculator(nullptr);
	Game::SetCombatSystem(nullptr);
}

} // namespace sandbox
} // namespace raid