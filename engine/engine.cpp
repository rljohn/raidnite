#include "engine/pch.h"

#include "engine.h"
#include "engine/game/game.h"
#include "engine/system/args.h"

using namespace std::chrono;

namespace raid
{

Engine::Engine()
	: m_FrameCount(0)
	, m_Accumulation(0)
	, m_TimeStep(0)
	, m_TimeStepSecs(0)
	, m_BaseTimeStep(0)
{
	m_LastUpdate = steady_clock::now();
}

void Engine::Init(const Nanoseconds& frameTime)
{
	mainAssert(frameTime != Nanoseconds(0));

	m_BaseTimeStep = frameTime;
	SetTimeStep(frameTime);
	m_LastUpdate = steady_clock::now();

	SCommandLineManager::Instance().Init(::GetCommandLineW());
}

void Engine::Shutdown()
{
	SCommandLineManager::Instance().Shutdown();
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
		const GameFrame frame{ m_FrameCount, m_TimeStep, m_TimeStepSecs };
		Game::Update(frame);

		m_Accumulation -= m_TimeStep;
		m_FrameCount++;
	}

	m_LastUpdate = now;
}

void Engine::SetTimeStep(Nanoseconds nanos)
{
	m_TimeStep = nanos;
	m_TimeStepSecs = std::chrono::duration_cast<TimeStepSeconds>(nanos);
}

Milliseconds Engine::FramesToMillis(const Frame frames) const
{
	Nanoseconds nanos = m_BaseTimeStep * frames;
	return std::chrono::duration_cast<Milliseconds>(nanos);
}

Duration Engine::FramesToDuration(const Frame frames) const
{
	return m_BaseTimeStep * frames;
}

Frame Engine::DurationToFrames(const Duration& duration) const
{
	return duration / m_BaseTimeStep;
}

EngineRAII::EngineRAII()
{
	Previous = Game::GetEngine();
	Game::SetEngine(&Instance);
}

EngineRAII::~EngineRAII()
{
	Game::SetEngine(Previous);
}

} // namespace raid