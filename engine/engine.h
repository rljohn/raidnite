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

	int64_t GetFrameCount() { return m_FrameCount; }

private:

	int64_t m_FrameCount;
	Nanoseconds m_TimeStep;
	std::chrono::steady_clock::time_point m_LastUpdate;
	Nanoseconds m_Accumulation;
};

} // namespace raid