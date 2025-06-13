#include "sandbox/pch.h"
#include "unit_widgets.h"

#include "sandbox_game.h"

#include "engine/localization/localization.h"
#include "engine/system/log/logging.h"
#include "engine/unit/states/state.h"

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

	DrawSpawnWidgets(sandbox, map);
	ImGui::Separator();
	DrawEntityWidgets(sandbox, map);
}

void UnitWidget::DrawSpawnWidgets(GameSandbox* sandbox, Map* map)
{

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
			// Give the NPC aggro/hostile AI
			unit->CreateAi<AIComponent>();
			unit->GetAggro().SetBehaviour(AggroBehaviour::Aggro);

			NameComponent& names = unit->GetName();
			names.SetName(m_UnitList[m_UnitListIdx]);

			if (IAttribute* attr = unit->GetAttribute<AttributeType::Faction>())
			{
				attr->SetValue(SandboxFactions::Enemy);
			}
		}
	}
}

void UnitWidget::DrawEntityWidgets(GameSandbox* sandbox, Map* map)
{
	if (ImGui::CollapsingHeader("Entities"))
	{

		IEntityManager* mgr = Game::GetEntityManager();
		if (!mgr)
			return;

		size_t count = mgr->GetEntityCount();
		for (size_t i = 0; i < count; i++)
		{
			ImGui::PushID((int)i);
			if (Entity* e = mgr->GetEntity(i))
			{
				std::optional<Position> pos;
				Position occ;

				if (TransformComponent* t = e->GetComponent<TransformComponent>())
				{
					pos = t->GetPosition();
					occ = t->GetOccupyingTile();
				}

				if (NameComponent* name = e->GetComponent<NameComponent>())
				{
					if (pos.has_value())
					{
						ImGui::Text("%lld: %s (%d,%d) (Occ: %d,%d)",
							i, name->GetName().c_str(),
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
					continue;
				}

				if (StateMachineComponent* state = e->GetComponent<StateMachineComponent>())
				{
					DrawEntityState(state);
				}

				if (AttributesComponent* attrs = e->GetComponent<AttributesComponent>())
				{
					DrawEntityAttributes(attrs);
				}

				if (AggroComponent* aggro = e->GetComponent<AggroComponent>())
				{
					DrawAggroTable(aggro);
				}
			}
			ImGui::PopID();

			if (i < count - 1)
				ImGui::Separator();
		}
	}
}

void UnitWidget::DrawEntityState(StateMachineComponent* state)
{
	StateType t = state->GetCurrentStateType();
	ImGui::Text("State: %s", StateTypeToString(t));
}

void UnitWidget::DrawEntityAttributes(AttributesComponent* attrs)
{
	if (!attrs) return;
	
	auto* f = attrs->GetAttribute<AttributeType::Faction>();
	if (!f) return;

	FactionId faction = f->As<FactionId>();
	
	if (IFactionManager* factionMgr = Game::GetFactionManager())
	{
		LocalizationKey key = factionMgr->GetFactionName(faction);
		if (key != InvalidLocalizationKey)
		{
			if (ILocalizationSystem* localization = Game::GetLocalizationSystem())
			{
				const char* text = localization->GetEntry(key);
				ImGui::Text("Faction: %s (%d)", text, faction);
			}
		}
	}
}

void UnitWidget::DrawAggroTable(AggroComponent* comp)
{
	if (comp->Count() == 0)
		return;

	if (ImGui::TreeNode("Aggro Table"))
	{
		for (int i = 0; i < comp->Count(); i++)
		{
			for (Entity* e : *comp)
			{
				ImGui::Text("%d", e->GetId());
			}
		}

		ImGui::TreePop();
	}
}

void UnitWidget::Shutdown()
{

}

} // namespace sandbox
} // namespace raid