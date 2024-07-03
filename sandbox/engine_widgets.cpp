#include "sandbox/pch.h"
#include "engine_widgets.h"
#include "sandbox_game.h"

namespace raid {
namespace sandbox {

void EngineWidget::Init()
{
	SetEnabled(true);
}

void EngineWidget::Draw(GameSandbox* sandbox)
{
	Engine& engine = sandbox->GetEngine();

	const Frame frames = engine.GetFrameCount();
	ImGui::Text("Frame: %llu", frames);

	const Duration d = engine.FramesToDuration(frames);
	Time::TimeDisplay display;
	Time::GetHMS(d, display);
	ImGui::Text("Time: %s", display);

	int64_t nanos = engine.GetTimeStep().count();
	if (ImGui::InputScalar("Tick Speed", ImGuiDataType_S64, &nanos))
	{
		engine.SetTimeStep(std::chrono::nanoseconds(nanos));
	}

	if (ImGui::Button("0.5x"))
	{
		engine.SetTimeStep(std::chrono::nanoseconds(nanos*2));
	}
	ImGui::SameLine();
	if (ImGui::Button("2x"))
	{
		engine.SetTimeStep(std::chrono::nanoseconds(nanos/2));
	}
}

} // namespace sandbox
} // namespace raid