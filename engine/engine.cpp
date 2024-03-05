#include "engine/pch.h"
#include "engine.h"

using namespace std::chrono;

namespace raid
{

Engine::Engine()
	: m_FrameCount(0)
	, m_Accumulation(0)
	, m_TimeStep(0)
{
	m_LastUpdate = steady_clock::now();
}

void Engine::Init(const Nanoseconds& frameTime)
{
	m_TimeStep = frameTime;
	m_LastUpdate = steady_clock::now();
}

void Engine::Shutdown()
{

}

void Engine::Update()
{
	steady_clock::time_point now = steady_clock::now();
	steady_clock::duration elapsed = now - m_LastUpdate;
	Update(now, elapsed);
}

void Engine::Update(const TimeStamp& now, const Nanoseconds& duration)
{
	m_Accumulation += duration;

	while (m_Accumulation >= m_TimeStep)
	{
		const GameFrame frame{ m_FrameCount, m_TimeStep };
		Game::Update(frame);

		m_Accumulation -= m_TimeStep;
		m_FrameCount++;
	}

	m_LastUpdate = now;
}


} // namespace raid