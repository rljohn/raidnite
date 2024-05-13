#include "engine/pch.h"
#include "game_instance.h"

#include "engine/game/game_events.h"

namespace raid
{

void GameInstance::Update(const GameFrame& frame)
{

}

void GameInstance::BeginLoadGame()
{
}

void GameInstance::EndLoadGame()
{
}

void GameInstance::BeginLoadMap()
{

}

void GameInstance::EndLoadMap()
{
}

void GameInstance::StartGame()
{
	GameStartEvent e;
	Game::DispatchGameEvent(&e);
}

void GameInstance::EndGame()
{
	GameEndEvent e;
	Game::DispatchGameEvent(&e);
}

void GameInstance::UpdateState(const GameFrame& frame)
{
	
}

} // namespace raid
