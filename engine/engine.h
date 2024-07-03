#pragma once

#include "types.h"

namespace raid
{

class Engine
{
public:

	Engine();

	void Init(const Nanoseconds& frameTime);
	void Update();
	void Update(const TimeStamp& now, const Nanoseconds& duration);
	void Shutdown();

	int64_t GetFrameCount() const { return m_FrameCount; }

	Nanoseconds GetTimeStep() const { return m_TimeStep; }
	void SetTimeStep(Nanoseconds nanos) { m_TimeStep = nanos; }

	Milliseconds FramesToMillis(const Frame frames) const;
	Duration FramesToDuration(const Frame frames) const;

private:

	int64_t m_FrameCount;
	Nanoseconds m_BaseTimeStep;
	Nanoseconds m_TimeStep;
	std::chrono::steady_clock::time_point m_LastUpdate;
	Nanoseconds m_Accumulation;
};

} // namespace raid