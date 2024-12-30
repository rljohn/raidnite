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

float RoundToSeconds(const Milliseconds& ms)
{
	float seconds = static_cast<float>(ms.count()) / 1000.0f;
	return roundf(seconds * 100.0f) / 100.0f;
}

TEST(EngineTest, TimeFrames)
{
	const std::chrono::nanoseconds frameTime(16666666);

	Engine e;
	e.Init(frameTime);

	Frame frames = 60;
	Milliseconds ms = e.FramesToMillis(frames);
	EXPECT_EQ(ms.count(), 999);

	float seconds = RoundToSeconds(ms);
	EXPECT_FLOAT_EQ(seconds, 1);

	frames = 61;
	ms = e.FramesToMillis(frames);
	EXPECT_EQ(ms.count(), 1016);
	seconds = RoundToSeconds(ms);
	EXPECT_FLOAT_EQ(seconds, 1.02f);

	Duration d = e.FramesToDuration(7800);

	Time::TimeDisplay display;
	Time::GetHMS(d, display);
	EXPECT_STREQ(display, "02:09");

	d = e.FramesToDuration(7801);
	Time::GetHMS(d, display);
	EXPECT_STREQ(display, "02:10");
}

TEST(EngineTest, Ticker)
{
	const std::chrono::nanoseconds frameTime(16666666);

	EngineRAII e;
	e.Instance.Init(frameTime);

	unsigned tickerCount = 0;

	Ticker t;
	t.Init(Milliseconds(100), [&]()
	{
		tickerCount++;
	});

	TimeStamp now = std::chrono::steady_clock::now();
	for (int i = 0; i < 100; i++)
	{
		e.Instance.Update(now, frameTime);
		t.Update(frameTime);

		now += frameTime;
	}

	// 16.666666ms * 100 updates = 1666.6666ms
	// Tick every 100ms = 16.66 ticks

	EXPECT_EQ(tickerCount, 16);

	// Ensure the minimum number of ticks required to hit 17 ticks
	// 100 x 16.6666 = 1666.66ms
	// To reach 17 ticks (1700ms), 1700-1666.66 = 33.34 ms
	// Is equivalent to 2.004 ticks, so we must tick 3 more times
	for (int i = 0; i < 3; i++)
	{
		e.Instance.Update(now, frameTime);
		t.Update(frameTime);

		now += frameTime;
	}

	EXPECT_EQ(tickerCount, 17);
}

TEST(EngineTest, FrameDelta)
{
	const std::chrono::nanoseconds frameTime(16666666);

	Engine e;
	e.Init(frameTime);

	class TestGameSystem : public IGameSystem
	{
	public:

		TestGameSystem() {}

		void Update(const GameFrame& frame) override
		{
			EXPECT_NEAR(frame.TimeStepSecs.count(), 0.01667, 0.00001);
		}
	};

	TestGameSystem system;

	TimeStamp now = std::chrono::steady_clock::now();

	Game::RegisterGameSystem(&system);
	e.Update(now, frameTime);
	Game::UnregisterGameSystem(&system);
}