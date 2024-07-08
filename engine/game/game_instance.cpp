#include "engine/pch.h"
#include "game_instance.h"

#include "engine/game/game_events.h"
#include "engine/map/map.h"
#include "engine/system/log/logging.h"

namespace raid
{
void GameInstance::Init(std::shared_ptr<ILoadDelegate> loadDlgt)
{
	m_LoadDelegate = loadDlgt;
}

void GameInstance::Shutdown()
{
	m_LoadDelegate = nullptr;
}

void GameInstance::Reset()
{
	SetGameState(GameState::None);
}

void GameInstance::Update(const GameFrame& frame)
{
	UpdateState(frame);
}

bool GameInstance::BeginLoadGame(const LoadContext& context)
{
	m_LoadType = LoadType::Starting;
	return Load(context);
}

void GameInstance::StartGame()
{
	mainVerbose("GameInstance::StartGame");

	GameStartEvent e;
	Game::DispatchGameEvent(&e);
}

bool GameInstance::BeginEndGame(const LoadContext& context)
{
	GameEndEvent e;
	Game::DispatchGameEvent(&e);

	m_LoadType = LoadType::Ending;
	return Load(context);
}

bool GameInstance::Load(const LoadContext& context)
{
	if (m_LoadDelegate)
	{
		if (m_LoadDelegate->BeginLoadGame(context))
		{
			SetGameState(GameState::Loading);
			return true;
		}
	}

	SetGameState(GameState::FatalError);
	return false;
}

void GameInstance::EndGame()
{
	mainVerbose("GameInstance::EndGame");
}

static const char* GameStateToString(GameState state)
{
	switch (state)
	{
	case GameState::None: return "None";
	case GameState::Loading: return "Loading";
	case GameState::Active: return "Active";
	case GameState::EndGame: return "EndGame";
	case GameState::FatalError: return "FatalError";
	}

	return "Unknown";
}

void GameInstance::SetGameState(GameState state)
{
	if (m_GameState == state)
		return;

	mainDisplay("SetGameState::{}", GameStateToString(state));
	m_GameState = state;
}

void GameInstance::UpdateState(const GameFrame& frame)
{
	switch (m_GameState)
	{
	case GameState::None:
		break;
	case GameState::Loading:
		UpdateGameLoad(frame);
		break;
	case GameState::Active:
		UpdateGameActive(frame);
		break;
	case GameState::EndGame:
		UpdateGameEnd(frame);
		break;
	case GameState::FatalError:
		break;
	}
}

void GameInstance::UpdateGameLoad(const GameFrame& frame)
{
	if (!m_LoadDelegate)
	{
		SetGameState(GameState::FatalError);
		return;
	}

	if (m_LoadDelegate)
	{
		if (m_LoadDelegate->IsReady())
		{
			if (m_LoadType == LoadType::Starting)
			{
				StartGame();
				SetGameState(GameState::Active);
			}
			else
			{
				EndGame();
				SetGameState(GameState::EndGame);
			}
		}
	}
}

void GameInstance::UpdateGameActive(const GameFrame& frame)
{
}

void GameInstance::UpdateGameEnd(const GameFrame& frame)
{
}


} // namespace raid
