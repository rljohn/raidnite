#include "sandbox/pch.h"
#include "map_widgets.h"

// sandbox
#include "sandbox_imgui.h"

// engine
#include "engine/game/game.h"


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
			Game::SetMap(m_Map);
		}
	}

	void MapWidget::DrawMapWidgets()
	{
		ImVec4* colors = ImGui::GetStyle().Colors;

		ImGui::Text("Map Created: %d x %d", m_Map->GetWidth(), m_Map->GetHeight());
		if (ImGui::BeginTable("Map Tiles", 11, ImGuiTableFlags_Borders))
		{
			ImGui::TableSetupColumn("",ImGuiTableColumnFlags_NoResize ); // Empty column for row headers
			for (int i = 0; i < m_Map->GetHeight(); i++)
			{
				char buf[16] = { 0 };
				sprintf_s(buf, "%d", i);
				ImGui::TableSetupColumn(buf);
			}

			ImGui::TableHeadersRow();
			for (int i = 0; i < m_Map->GetHeight(); i++)
			{
				char buf[16] = { 0 };
				sprintf_s(buf, "%d", i);
			}

			for (int i = 0; i < m_Map->GetHeight(); i++)
			{
				ImGui::TableNextRow();
				ImGui::TableSetColumnIndex(0);
				ImGui::TableSetBgColor(ImGuiTableBgTarget_CellBg, ImGui::GetColorU32(colors[ImGuiCol_TableHeaderBg]));
				ImGui::Text("%d", i);

				for (int j = 0; j < m_Map->GetWidth(); j++)
				{
					ImGui::TableSetColumnIndex(j+1);
					ImGui::Text("%d,%d", j, i);
				}
			}

			ImGui::EndTable();
		}

		if (ImGui::Button("Destroy Map"))
		{
			Game::SetMap(nullptr);
			delete m_Map;
			m_Map = nullptr;
		}
	}

	void MapWidget::Shutdown()
	{
	}

} // namespace sandbox
} // namespace raid