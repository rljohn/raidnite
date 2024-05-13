#pragma once

#include "engine/game/game.h"

namespace raid
{
	
enum class GameState
{
	None,
	LoadingGame,
	Active,
	EndGame,
};

enum class MapState
{
	None,
	LoadingMap,
	Active
};

class GameInstance : public IGameSystem
{

public:

	void Update(const GameFrame& frame) override;

	GameState GetGameState() const { return m_GameState; }
	void BeginLoadGame();
	void EndLoadGame();
	void StartGame();
	void EndGame();

	MapState GetMapState() const { return m_MapState; }
	void BeginLoadMap();
	void EndLoadMap();
	
private:

	void UpdateState(const GameFrame& frame);

	GameState m_GameState;
	MapState m_MapState;
};

} // namespace raid
