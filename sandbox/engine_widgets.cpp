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
		for (size_t i = 0; i < SCommandLineManager::Instance().GetArgCount(); i++)
		{
			if (CommandLineArg* arg = SCommandLineManager::Instance().GetArg(i))
			{
				if (arg->GetType() == CommandLineArg::Type::Bool)
				{
					if (BoolArgument* b = static_cast<BoolArgument*>(arg))
					{
						bool enabled = b->GetValue();
						if (ImGui::Checkbox(stringutil::WideStringToUtf8(b->GetName()).c_str(), &enabled))
						{
							b->SetValue(enabled);
						}
					}
				}
				else if (arg->GetType() == CommandLineArg::Type::Int)
				{
					if (IntArgument* i = static_cast<IntArgument*>(arg))
					{
						int value = i->GetValue();
						if (ImGui::InputInt(stringutil::WideStringToUtf8(i->GetName()).c_str(), &value))
						{
							i->SetValue(value);
						}
					}
				}
				else if (arg->GetType() == CommandLineArg::Type::String)
				{
					if (StringArgument* s = static_cast<StringArgument*>(arg))
					{
						char buffer[1024] = { 0 };
						strcpy_s(buffer, stringutil::WideStringToUtf8(s->GetValue()).c_str());
						if (ImGui::InputText(stringutil::WideStringToUtf8(s->GetName()).c_str(), buffer, COUNTOF(buffer)))
						{
							s->SetValue(stringutil::Utf8StringToWide(std::string(buffer)).c_str());
						}
					}
				}
			}
		}
	}
}

} // namespace sandbox
} // namespace raid