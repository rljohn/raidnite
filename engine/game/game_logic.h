#pragma once

namespace raid {
namespace GameLogic {

double GetTilesPerSecond(const double speedStat)
{
	// 100 speed = 1 tile per second
	return speedStat / 100.0;
}

} // namespace GameLogic
} // namespace raid