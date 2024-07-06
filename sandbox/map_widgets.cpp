#include "sandbox/pch.h"
#include "map_widgets.h"

// sandbox
#include "sandbox_imgui.h"
#include "sandbox_game.h"

// engine
#include "engine/game/game.h"
#include "engine/system/stringutil.h"

namespace raid {
namespace sandbox {

BOOL_XPARAM(QuickPlay);

void MapWidget::Init()
{
	m_Enabled = true;
}

void MapWidget::Draw(GameSandbox* sandbox)
{
	if (m_Map == nullptr)
	{
		if (sandbox->GetGameInstance().IsLoading())
		{
			CreateMapWidgets();
		}
		else 
		{
			ImGui::Text("Waiting for Map Load State");
		}
	}
	else
	{
		DrawMapWidgets(sandbox);
	}
}

void MapWidget::CreateMapWidgets()
{
	ImGui::SliderInt("Width", &m_Width, 0, 100);
	ImGui::SliderInt("Height", &m_Height, 0, 100);

	AUTOMATION_HELPER(QuickPlay);
	if (ImGui::Button("Create Map") || AutoQuickPlay.Proceed())
	{
		m_Map = new Map();
		m_Map->BuildMap(m_Width, m_Height);
		Game::SetMap(m_Map);
	}
}

void MapWidget::DrawMapWidgets(GameSandbox* sandbox)
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

				const char* mapIcon = GetMapIcon(sandbox, j, i);
				if (!raid::stringutil::NullOrEmpty(mapIcon))
				{
					ImGui::Text(mapIcon);
				}
					
			}
		}

		ImGui::EndTable();
	}

	// Player Spawning
	{
		ImGui::PushID("Player Spawn");
		ImGui::Text("Player Spawn");
		const Position pos = m_Map->GetPlayerSpawnPosition();
		int x = pos.GetX(); int y = pos.GetY();
		if (ImGui::SliderInt("X", &x, 0, m_Map->GetWidth() - 1))
		{
			m_Map->SetPlayerSpawnPosition(Position(x, y));
		}
		if (ImGui::SliderInt("Y", &y, 0, m_Map->GetHeight() - 1))
		{
			m_Map->SetPlayerSpawnPosition(Position(x, y));
		}
		ImGui::PopID();
	}

	if (ImGui::Button("Destroy Map"))
	{
		Game::SetMap(nullptr);
		delete m_Map;
		m_Map = nullptr;
	}
}

const char* MapWidget::GetMapIcon(GameSandbox* sandbox, const int x, const int y) const
{
	raid::Group& group = sandbox->GetParty();
	for (size_t idx = 0; idx < group.GetSize(); idx++)
	{
		Entity* e = group.GetEntity(idx);
		if (TransformComponent* transform = e->GetComponent<TransformComponent>())
		{
			auto& pos = transform->GetPosition();
			if (pos.GetX() == x && pos.GetY() == y)
			{
				return ICON_FK_CIRCLE_O;
			}
		}
	}

	return nullptr;
}

void MapWidget::Shutdown()
{
}

} // namespace sandbox
} // namespace raid