#include "pch.h"
#include "test.h"

#include "engine/engine.h"
#include "engine/game/game.h"

using namespace raid;

TEST(EngineTest, TimeSteps)
{
	const std::chrono::nanoseconds frameTime(16666666);

	Engine e;
	e.Init(frameTime);

	class UnitTestSubsystem : public IGameSystem
	{
	public:

		void Update(const GameFrame& frame)
		{
			Count++;
		}

		int Count = 0;
	};

	UnitTestSubsystem tmp;
	Game::RegisterGameSystem(&tmp);

	// Updating at twice the FPS of our fixed step
	TimeStamp now = std::chrono::steady_clock::now();
	const std::chrono::nanoseconds frameTime120Fps(8333333);
	for (int i = 0; i < 10; i++)
	{
		e.Update(now, frameTime120Fps);
		now += frameTime120Fps;
	}
	EXPECT_EQ(tmp.Count, 5);

	// Updating a very slow frame, process multiple sub-tick updates.
	tmp.Count = 0;

	// We should receive 3 internal updates on this tick.
	const std::chrono::nanoseconds frameTime20Fps(49999998);
	e.Update(now, frameTime20Fps);
	now += frameTime20Fps;
	EXPECT_EQ(tmp.Count, 3);

	Game::UnregisterGameSystem(&tmp);
}