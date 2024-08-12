#include "sandbox/pch.h"
#include "map_widgets.h"

// sandbox
#include "sandbox_imgui.h"
#include "sandbox_game.h"

// engine
#include "engine/game/game.h"
#include "engine/system/stringutil.h"
#include "engine/system/automation.h"

namespace raid {
namespace sandbox {

BOOL_XPARAM(QuickPlay);

void MapWidget::Init()
{
	m_Enabled = true;
}

void MapWidget::Draw(GameSandbox* sandbox)
{
	if (sandbox->GetMap() == nullptr)
	{
		if (sandbox->GetGameInstance().IsLoading())
		{
			CreateMapWidgets(sandbox);
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

void MapWidget::CreateMapWidgets(GameSandbox* sandbox)
{
	ImGui::SliderInt("Width", &m_Width, 0, 100);
	ImGui::SliderInt("Height", &m_Height, 0, 100);

	AUTOMATION_HELPER(QuickPlay);
	if (ImGui::Button("Create Map") || AutoQuickPlay.Proceed())
	{
		sandbox->BuildMap(m_Width, m_Height);
	}
}

void MapWidget::DrawMapWidgets(GameSandbox* sandbox)
{
	ImVec4* colors = ImGui::GetStyle().Colors;

	ImGui::Text("Map Created: %d x %d", sandbox->GetMap()->GetWidth(), sandbox->GetMap()->GetHeight());
	if (ImGui::BeginTable("Map Tiles", 11, ImGuiTableFlags_Borders))
	{
		ImGui::TableSetupColumn("",ImGuiTableColumnFlags_NoResize ); // Empty column for row headers
		for (int i = 0; i < sandbox->GetMap()->GetHeight(); i++)
		{
			char buf[16] = { 0 };
			sprintf_s(buf, "%d", i);
			ImGui::TableSetupColumn(buf);
		}

		ImGui::TableHeadersRow();
		for (int i = 0; i < sandbox->GetMap()->GetHeight(); i++)
		{
			char buf[16] = { 0 };
			sprintf_s(buf, "%d", i);
		}

		for (int i = 0; i < sandbox->GetMap()->GetHeight(); i++)
		{
			ImGui::TableNextRow();
			ImGui::TableSetColumnIndex(0);
			ImGui::TableSetBgColor(ImGuiTableBgTarget_CellBg, ImGui::GetColorU32(colors[ImGuiCol_TableHeaderBg]));
			ImGui::Text("%d", i);

			for (int j = 0; j < sandbox->GetMap()->GetWidth(); j++)
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
		const Position pos = sandbox->GetMap()->GetPlayerSpawnPosition();
		int x = pos.GetX(); int y = pos.GetY();
		if (ImGui::SliderInt("X", &x, 0, sandbox->GetMap()->GetWidth() - 1))
		{
			sandbox->GetMap()->SetPlayerSpawnPosition(Position(x, y));
		}
		if (ImGui::SliderInt("Y", &y, 0, sandbox->GetMap()->GetHeight() - 1))
		{
			sandbox->GetMap()->SetPlayerSpawnPosition(Position(x, y));
		}
		ImGui::PopID();
	}
}

const char* MapWidget::GetMapIcon(GameSandbox* sandbox, const int x, const int y) const
{
	static char buffer[8] = { 0 };
	buffer[0] = '\0';

	Tile* t = sandbox->GetMap()->GetTile(x, y);
	if (!t) return buffer;

	bool drawOccupant = true;
	Entity* occupant = t->GetOccupant();

	for(const Entity* e : t->GetActiveEntities())
	{
		if (e == occupant)
		{
			drawOccupant = false;
		}

		if (NameComponent* name = e->GetComponent<NameComponent>())
		{
			const std::string& n = name->GetName();
			strncpy_s(buffer, n.c_str(), 3);
		}
	}

	if (t->IsOccupied() && drawOccupant)
	{
		strcpy_s(buffer, "o");
	}

	return buffer;
}

void MapWidget::Shutdown()
{
}

} // namespace sandbox
} // namespace raid