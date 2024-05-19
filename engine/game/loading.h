#pragma once

#include <functional>
#include <string>

namespace raid
{

struct LoadContext
{
	std::string MapName;
	std::string LevelScript;
};

class ILoadDelegate
{
public:

	bool BeginLoadGame(const LoadContext& ctx);
	
	bool IsMapLoaded() const { return m_MapLoaded; }
	bool IsGameLoaded() const { return m_GameLoaded; }
	bool IsReady() const { return IsGameLoaded() && IsMapLoaded(); }

	void OnGameLoaded();
	void OnMapLoaded();

protected:

	virtual bool LoadGameData(const LoadContext& ctx) = 0;
	
	void Reset();

	bool m_MapLoaded = false;
	bool m_GameLoaded = false;
};

} // namespace raid