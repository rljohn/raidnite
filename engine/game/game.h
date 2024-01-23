#pragma once

#include "engine/types.h"

namespace raid
{

// Forward declare Game classes
class World;

struct GameFrame
{
	World* World = nullptr;
	Duration DeltaTime;
};

} // namespace raid