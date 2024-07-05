#pragma once

#include "engine/game/game.h"
#include "engine/game/loading.h"

namespace raid
{
	
enum class GameState
{
	None,
	Loading,
	Active,
	EndGame,
	FatalError
};

enum class LoadType
{
	Starting,
	Ending
};

class GameInstance : public IGameSystem
{

public:

	void Init(std::shared_ptr< ILoadDelegate> loadDlgt);
	void Update(const GameFrame& frame) override;
	void Shutdown();
	void Reset();

	GameState GetGameState() const { return m_GameState; }

	bool IsLoading() const { return GetGameState() == GameState::Loading; }
	bool IsActive() const { return GetGameState() == GameState::Active; }

	bool BeginLoadGame(const LoadContext& context);
	void StartGame();

	bool BeginEndGame(const LoadContext& context);
	void EndGame();
	
private:

	bool Load(const LoadContext& context);

	void SetGameState(GameState state);
	void UpdateState(const GameFrame& frame);

	void UpdateGameLoad(const GameFrame& frame);
	void UpdateGameActive(const GameFrame& frame);
	void UpdateGameEnd(const GameFrame& frame);

	std::shared_ptr<ILoadDelegate> m_LoadDelegate;
	GameState m_GameState;
	LoadType m_LoadType;
};

} // namespace raid
