#include "sandbox/pch.h"
#include "party_widgets.h"

// sandbox
#include "sandbox_imgui.h"

namespace raid {
namespace sandbox {

void PartyWidget::Init()
{
}

void PartyWidget::Draw()
{
	if (m_Party.IsValid())
	{
		DrawPartyWidgets();
	}
	else
	{
		ImGui::SliderInt("Party Size", &m_PartySize, 1, 20);
		if (ImGui::Button("Create Party"))
		{
			m_Party.Init(m_PartySize);
		}
	}
}

void PartyWidget::DrawPartyWidgets()
{
	ImGui::Text("Max Size: %u", m_Party.GetCapacity());
}

void PartyWidget::Shutdown()
{
}

} // namespace sandbox
} // namespace raid