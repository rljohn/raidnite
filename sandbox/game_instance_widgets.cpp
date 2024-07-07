#include "sandbox/pch.h"
#include "game_instance_widgets.h"

// sandbox
#include "sandbox_imgui.h"
#include "sandbox_game.h"

// engine
#include "engine/game/game.h"
#include "engine/game/game_instance.h"
#include "engine/game/loading.h"
#include "engine/system/stringutil.h"
#include "engine/system/automation.h"

namespace raid {
using namespace stringutil;

namespace sandbox {

BOOL_XPARAM(QuickPlay);
STRING_XPARAM(MapName);

GameInstanceWidget::GameInstanceWidget()
{
	StringSetEmpty(m_MapName);
}

void GameInstanceWidget::Init()
{
	SetEnabled(true);
	m_LoadDlgt = std::make_shared<LoadDlgt>();

	if (Arg_MapName.HasValue())
	{
		std::string name = stringutil::WideStringToUtf8(Arg_MapName.GetValue());
		strcpy_s(m_MapName, name.c_str());
	}
}

void GameInstanceWidget::Draw(GameSandbox* sandbox)
{
	DrawGameStateWidgets(sandbox);
}

void GameInstanceWidget::Shutdown()
{
	m_LoadDlgt = nullptr;
}

static const char* LogState(GameState state)
{
	switch (state)
	{
	case GameState::None:
		return "None";
		case GameState::Loading:
		return "Loading";
	case GameState::Active:
		return "Active";
	case GameState::EndGame:
		return "EndGame";
	}

	return "Unknown";
}

void GameInstanceWidget::DrawGameStateWidgets(GameSandbox* sandbox)
{
	GameInstance& instance = sandbox->GetGameInstance();
	ImGui::Text("State: %s", LogState(instance.GetGameState()));
	
	switch (instance.GetGameState())
	{
		case GameState::None:
			DrawGameStateWidgets_None(instance);
			break;
		case GameState::Loading:
			DrawGameStateWidgets_Loading(instance);
			break;
		case GameState::Active:
			DrawGameStateWidgets_Active(instance);
			break;
		case GameState::EndGame:
			DrawGameStateWidgets_End(instance);
			break;
	}
}

void GameInstanceWidget::DrawGameStateWidgets_None(GameInstance& instance)
{
	ImGui::InputText("Map Name", m_MapName, COUNTOF(m_MapName));

	AUTOMATION_HELPER(QuickPlay);
	if (ImGui::Button("Begin Game") || AutoQuickPlay.Proceed())
	{
		instance.Init(std::static_pointer_cast<raid::ILoadDelegate>(m_LoadDlgt));

		raid::LoadContext ctx;
		ctx.MapName = m_MapName;
		ctx.LevelScript = "";
		instance.BeginLoadGame(ctx);
	}
}

void GameInstanceWidget::DrawGameStateWidgets_Loading(GameInstance& instance)
{
	if (!m_LoadDlgt->IsMapLoaded())
	{
		AUTOMATION_HELPER(QuickPlay);
		if (ImGui::Button("OnMapLoaded") || (Game::GetMap() && AutoQuickPlay.Proceed()))
		{
			m_LoadDlgt->OnMapLoaded();
		}
	}
	else if (!m_LoadDlgt->IsGameLoaded())
	{
		AUTOMATION_HELPER(QuickPlay);
		if (ImGui::Button("OnGameLoaded") || (Game::GetMap() && AutoQuickPlay.Proceed()))
		{
			m_LoadDlgt->OnGameLoaded();
		}
	}
}

void GameInstanceWidget::DrawGameStateWidgets_Active(GameInstance& instance)
{
	if (ImGui::Button("End Game"))
	{
		raid::LoadContext ctx;
		instance.BeginEndGame(ctx);
	}
}

void GameInstanceWidget::DrawGameStateWidgets_End(GameInstance& instance)
{
	if (ImGui::Button("Reset"))
	{
		instance.Reset();

		if (Game::GetEncounterLog())
		{
			Game::GetEncounterLog()->Clear();
		}
		
		if (Game::GetEntityManager())
		{
			Game::GetEntityManager()->Reset();
		}
	}
}


} // namespace sandbox
} // namespace raid