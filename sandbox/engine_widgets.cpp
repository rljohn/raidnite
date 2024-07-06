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

	ImGui::Separator();

	if (ImGui::CollapsingHeader("Arguments"))
	{
		if (ImGui::TreeNode("Enabled"))
		{
			for (size_t i = 0; i < SCommandLineManager::Instance().GetArgCount(); i++)
			{
				CommandLineArg* arg = SCommandLineManager::Instance().GetArg(i);
				if (arg && arg->HasValue())
				{
					CommandLineArg::DisplayBuffer buffer;
					arg->GetDisplay(buffer);
					ImGui::Text("%s", buffer);
				}
			}

			ImGui::TreePop();
		}
		if (ImGui::TreeNode("Disabled"))
		{
			for (size_t i = 0; i < SCommandLineManager::Instance().GetArgCount(); i++)
			{
				CommandLineArg* arg = SCommandLineManager::Instance().GetArg(i);
				if (arg && !arg->HasValue())
				{
					ImGui::Text("%ls", arg->GetName().c_str());
				}
			}

			ImGui::TreePop();
		}
	}
}

} // namespace sandbox
} // namespace raid