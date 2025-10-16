#include "engine/pch.h"
#include "engine/system/frame.h"
#include "engine/engine.h"

namespace raid
{

Milliseconds GetTimeSince(const Frame frame, const Frame since)
{
	if (Engine* engine = Game::GetEngine())
	{
		Frame duration = frame - since;
		return engine->FramesToMillis(duration);
	}

	return Milliseconds(0);
}

}