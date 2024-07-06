#include "sandbox/pch.h"
#include "sandbox_game.h"

// engine
#include "engine/system/fileutil.h"
#include "engine/system/log/logging.h"

// imgui
#include "thirdparty/imgui/imgui.h"

namespace raid {
namespace sandbox {

using namespace raid;

void GameSandbox::Init()
{
	// Logging
	FileUtil::SetProjectName(std::wstring(L"GameSandbox"));
	std::wstring logPath = FileUtil::GetAppDataFolder() + L"\\sandbox.log";
	m_Logger = raid::LogSystem::CreateLogger("GameSandbox", logPath);
	LogSystem::SetDefaultLogger(m_Logger);

	// 60 FPS update loop
	Nanoseconds timeStep(16666666);
	m_Engine.Init(timeStep);

	// Encounter pooling
	m_EncounterLog.Init(std::make_unique<EventPool>());

	// Game Service Locators
	Game::RegisterGameSystem(&m_GameInstance);
	Game::SetEngine(&m_Engine);
	Game::SetEntityManager(&m_World);
	Game::SetEncounterLog(&m_EncounterLog);
	Game::SetDamageCalculator(&m_DamageCalculator);
	Game::SetCombatSystem(&m_CombatSystem);

	m_Widgets.push_back(&m_EngineWidgets);
	m_Widgets.push_back(&m_MapWidgets);
	m_Widgets.push_back(&m_PartyWidgets);
	m_Widgets.push_back(&m_LogWidgets);
	m_Widgets.push_back(&m_GameInstanceWidgets);

	for (Widget* w : m_Widgets)
	{
		w->Init();
	}
}

void GameSandbox::Update()
{
	m_Engine.Update();

	if (ImGui::BeginMainMenuBar()) 
	{
		if (ImGui::BeginMenu("File")) 
		{
			ImGui::MenuItem("New", "Ctrl+N");
			ImGui::EndMenu();
		}

		if (ImGui::BeginMenu("Widgets"))
		{
			for (Widget* w : m_Widgets)
			{
				bool enabled = w->IsEnabled();

				if (ImGui::MenuItem(w->GetName(), "", &enabled))
				{
					w->SetEnabled(enabled);
				}
			}
			ImGui::EndMenu();
		}

		ImGui::EndMainMenuBar();
	}

	for (Widget* w : m_Widgets)
	{
		if (!w->IsEnabled())
			continue;

		bool open = true;
		if (ImGui::Begin(w->GetName(), &open))
		{
			ImGui::PushID(w->GetName());
			w->Draw(this);
			ImGui::PopID();

			ImGui::End();
		}
		w->SetEnabled(open);
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
	Game::SetEngine(nullptr);

	m_EncounterLog.Shutdown();
	m_Engine.Shutdown();

	m_Logger->Shutdown();
	LogSystem::SetDefaultLogger(nullptr);
	delete m_Logger;
	m_Logger = nullptr;
}

} // namespace sandbox
} // namespace raid