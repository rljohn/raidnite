#include "sandbox/pch.h"
#include "unit_widgets.h"

#include "sandbox_game.h"

#include "engine/system/log/logging.h"

#include <optional>

namespace raid {
namespace sandbox {

void UnitWidget::Init()
{
	m_UnitList.push_back(std::string("Target Dummy"));
	m_UnitList.push_back(std::string("Tank Dummy"));

	m_Enabled = true;
}

void UnitWidget::Draw(GameSandbox* sandbox)
{
	Map* map = Game::GetMap();
	if (!map)
	{
		return;
	}

	if (ImGui::BeginCombo("Enemies", m_UnitList[m_UnitListIdx].c_str()))
	{
		for (int i = 0; i < m_UnitList.size(); i++)
		{
			bool is_selected = (m_UnitListIdx == i);
			if (ImGui::Selectable(m_UnitList[i].c_str(), is_selected))
				m_UnitListIdx = i;

			if (is_selected)
				ImGui::SetItemDefaultFocus();
		}
		ImGui::EndCombo();
	}

	int x = m_SpawnPosition.GetX(); int y = m_SpawnPosition.GetY();
	if (ImGui::SliderInt("X", &x, 0, map->GetWidth() - 1))
	{
		m_SpawnPosition.SetX(x);
	}
	if (ImGui::SliderInt("Y", &y, 0, map->GetHeight() - 1))
	{
		m_SpawnPosition.SetY(y);
	}

	if (ImGui::Button("Spawn Unit"))
	{
		Position spawnPos;
		bool success = map->GetNearestUnoccupiedTile(m_SpawnPosition, spawnPos);
		mapAssert(success);

		raid::UnitSpawner& spawner = sandbox->GetUnitSpawner();
		if (Unit* unit = dynamic_cast<Unit*>(spawner.SpawnEntity(map, spawnPos)))
		{
			NameComponent& names = unit->GetName();
			names.SetName(m_UnitList[m_UnitListIdx]);

			
		}
	}

	ImGui::Separator();
	ImGui::Text("Entities");

	if (IEntityManager* mgr = Game::GetEntityManager())
	{
		size_t count = mgr->GetEntityCount();
		for (size_t i = 0; i < count; i++)
		{
			ImGui::PushID((int)i);
			if (Entity* e = mgr->GetEntity(i))
			{
				std::optional<Position> pos;
				Position occ;

				if (TransformComponent * t = e->GetComponent<TransformComponent>())
				{
					pos = t->GetPosition();
					occ = t->GetOccupyingTile();
				}

				if (NameComponent* name = e->GetComponent<NameComponent>())
				{
					if (pos.has_value())
					{
						ImGui::Text("%lld: %s (%d,%d) (Occ: %d,%d)", 
							i , name->GetName().c_str(),
							pos.value().GetX(), pos.value().GetY(), 
							occ.GetX(), occ.GetY());
					}
					else
					{
						ImGui::Text("%lld: %s", i, name->GetName().c_str());
					}
				}

				ImGui::SameLine();

				if (ImGui::Button("Destroy"))
				{
					raid::UnitSpawner& spawner = sandbox->GetUnitSpawner();
					spawner.DestroyEntity(map, e);
					i--;
				}
			}
			ImGui::PopID();
		}
	}
}

void UnitWidget::Shutdown()
{

}

} // namespace sandbox
} // namespace raid