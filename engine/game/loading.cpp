#include "engine/pch.h"
#include "loading.h"

namespace raid
{
void ILoadDelegate::Reset()
{
	m_MapLoaded = false;
	m_GameLoaded = false;
}

bool ILoadDelegate::BeginLoadGame(const LoadContext& ctx)
{
	Reset();
	return LoadGameData(ctx);
}


void ILoadDelegate::OnMapLoaded()
{
	m_MapLoaded = true;
}

void ILoadDelegate::OnGameLoaded()
{
	m_GameLoaded = true;
}

} // namespace raid