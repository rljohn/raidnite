#include "sandbox/pch.h"
#include "map_widgets.h"

namespace raid {
namespace sandbox {

	void MapWidget::Init()
	{
	}

	void MapWidget::Draw()
	{
		if (m_Map == nullptr)
		{
			CreateMapWidgets();
		}
		else
		{
			DrawMapWidgets();
		}
	}

	void MapWidget::CreateMapWidgets()
	{
		ImGui::SliderInt("Width", &m_Width, 0, 100);
		ImGui::SliderInt("Height", &m_Height, 0, 100);

		if (ImGui::Button("Create Map"))
		{
			m_Map = new Map();
			m_Map->BuildMap(m_Width, m_Height);
		}
	}

	void MapWidget::DrawMapWidgets()
	{
		ImGui::Text("Map Created: %d x %d", m_Map->GetWidth(), m_Map->GetHeight());
	}

	void MapWidget::Shutdown()
	{
	}

} // namespace sandbox
} // namespace raid