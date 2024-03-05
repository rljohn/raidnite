#include "sandbox/pch.h"
#include "engine_widgets.h"
#include "sandbox_game.h"

namespace raid {
namespace sandbox {

void EngineWidget::Draw(GameSandbox* sandbox)
{
	ImGui::Text("Frame: %llu", sandbox->GetEngine().GetFrameCount());
}

} // namespace sandbox
} // namespace raid