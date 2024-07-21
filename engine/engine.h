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
	void SetTimeStep(Nanoseconds nanos);

	Milliseconds FramesToMillis(const Frame frames) const;
	Duration FramesToDuration(const Frame frames) const;

private:

	int64_t m_FrameCount;
	TimeStep m_BaseTimeStep;
	TimeStep m_TimeStep;
	TimeStepSeconds m_TimeStepSecs;
	std::chrono::steady_clock::time_point m_LastUpdate;
	Nanoseconds m_Accumulation;
};

} // namespace raid