#include "sandbox/pch.h"
#include "party_widgets.h"

// engine
#include "engine/entity/transform.h"

// sandbox
#include "sandbox_imgui.h"
#include "sandbox_game.h"

#include <stdio.h>

namespace raid {
namespace sandbox {

BOOL_XPARAM(QuickPlay);

void PartyWidget::Init()
{
	m_Enabled = true;
}

void PartyWidget::Draw(GameSandbox* sandbox)
{
	if (!sandbox->GetGameInstance().IsActive())
	{
		ImGui::Text("No game instance active");
		return;
	}

	if (sandbox->GetParty().IsValid())
	{
		DrawPartyWidgets(sandbox);
	}
	else
	{
		ImGui::SliderInt("Party Size", &m_PartySize, 1, 20);
		if (ImGui::Button("Create Party"))
		{
			sandbox->GetParty().Init(m_PartySize);
		}

		if (raid::Game::GetMap())
		{
			ImGui::SameLine();

			AUTOMATION_HELPER(QuickPlay);
			if (ImGui::Button("Quick Party") || AutoQuickPlay.Proceed())
			{
				sandbox->GetParty().Init(m_PartySize);
				AddRandomUnit(sandbox, m_PartySize);
			}
		}
	}
}

void PartyWidget::DrawPartyWidgets(GameSandbox* sandbox)
{
	raid::Group& party = sandbox->GetParty();
	ImGui::Text("Max Size: %u", party.GetCapacity());

	ImGui::Checkbox("Show HP Widgets", &m_ShowHealthWidgets);

	for (std::size_t i = 0; i < party.GetSize(); i++)
	{
		ImGui::PushID((int)i);

		ImGui::Separator();
		raid::Entity* entity = party.GetEntity(i);
		raid::Unit* unit = static_cast<raid::Unit*>(entity);

		NameComponent& names = *entity->GetComponent<NameComponent>();

		// TITLE
		std::string display;
		if (!names.GetTitlePrefix().empty())
		{
			display += names.GetTitlePrefix();
			display += " ";
		}

		// NAME
		if (!names.GetName().empty())
		{
			display += names.GetName();
		}

		// SUFFIX
		if (!names.GetTitleSuffix().empty())
		{
			display += ", ";
			display += names.GetTitleSuffix();
		}

		ImGui::Text("%s", display.c_str());

		// TAG
		if (!names.GetTag().empty())
		{
			ImGui::Text("<%s>", names.GetTag().c_str());
		}

		// HEALTH
		raid::IPower* hp = unit->GetPower<raid::PowerType::Health>();
		if (hp)
		{
			ImGui::Text("%d / %d (%.2f%%)", hp->GetCurrent(), hp->GetMax(), hp->GetPercent());

			if (m_ShowHealthWidgets)
			{
				float min = hp->GetMin();
				float max = hp->GetMax();
				float current = hp->GetCurrent();

				if (ImGui::InputScalar("Min HP", ImGuiDataType_Float, &min))
				{
					hp->SetBaseValues(min, max);
				}
				if (ImGui::InputScalar("Max HP", ImGuiDataType_Float, &max))
				{
					hp->SetBaseValues(min, max);
				}
				if (ImGui::InputScalar("Current HP", ImGuiDataType_Float, &current))
				{
					hp->SetCurrent(current);
				}
			}
		}

		// POSITION
		TransformComponent* transform = unit->GetComponent<TransformComponent>();
		if (transform)
		{
			Position pos = transform->GetPosition();
			ImGui::Text("Pos: %d,%d", pos.GetX(), pos.GetY());
			ImGui::SameLine();
			if (ImGui::Button(ICON_FK_ARROW_LEFT))
			{
				pos.SetX(pos.GetX() - 1);
				transform->SetPosition(pos);
			}
			ImGui::SameLine();
			if (ImGui::Button(ICON_FK_ARROW_RIGHT))
			{
				pos.SetX(pos.GetX() + 1);
				transform->SetPosition(pos);
			}
			ImGui::SameLine();
			if (ImGui::Button(ICON_FK_ARROW_UP))
			{
				pos.SetY(pos.GetY() - 1);
				transform->SetPosition(pos);
			}
			ImGui::SameLine();
			if (ImGui::Button(ICON_FK_ARROW_DOWN))
			{
				pos.SetY(pos.GetY() + 1);
				transform->SetPosition(pos);
			}
		}

		ImGui::PopID();
	}

	ImGui::Separator();

	ImGui::InputText("Name", m_NameBuf, COUNTOF(m_NameBuf));
	ImGui::InputText("Tag", m_TagBuf, COUNTOF(m_TagBuf));
	ImGui::InputText("Title Prefix", m_TitlePrefixBuf, COUNTOF(m_TitlePrefixBuf));
	ImGui::InputText("Title Suffix", m_TitleSuffixBuf, COUNTOF(m_TitleSuffixBuf));

	if (raid::Game::GetMap())
	{
		if (ImGui::Button(ICON_FK_PLUS_CIRCLE "Add Unit"))
		{
			Map* map = raid::Game::GetMap();
			if (map)
			{
				Position tmp = map->GetPlayerSpawnPosition();
				
				Position spawnPosition;
				map->GetNearestUnoccupiedTile(tmp, tmp, spawnPosition);

				raid::UnitSpawner& spawner = sandbox->GetUnitSpawner();
				if (Unit* unit = dynamic_cast<Unit*>(spawner.SpawnEntity(map, spawnPosition)))
				{
					party.AddUnit(unit);

					NameComponent& names = unit->GetName();
					names.SetName(m_NameBuf);
					names.SetTag(m_TagBuf);
					names.SetTitlePrefix(m_TitlePrefixBuf);
					names.SetTitleSuffix(m_TitleSuffixBuf);
				}
			}
		}

		ImGui::SameLine();
		if (ImGui::Button("Add Random Unit"))
		{
			AddRandomUnit(sandbox, 1);
		}
	}

	if (ImGui::Button("Destroy Party"))
	{
		sandbox->GetParty().Shutdown();
	}
}

void PartyWidget::Shutdown()
{
}

void PartyWidget::AddRandomUnit(GameSandbox* sandbox, int numUnits)
{
	Position pos = Position(0, 0);
	Map* map = raid::Game::GetMap();
	if (map)
	{
		pos = map->GetPlayerSpawnPosition();
	}

	raid::Group& party = sandbox->GetParty();
	for (int i = 0; i < numUnits; i++)
	{
		raid::UnitSpawner& spawner = sandbox->GetUnitSpawner();
		if (Unit* unit = dynamic_cast<Unit*>(spawner.SpawnEntity(map, pos)))
		{
			party.AddUnit(unit);

			static int count = 0;
			NameComponent& names = unit->GetName();

			char buf[32] = { 0 };
			sprintf_s(buf, "Random Unit %d", ++count);
			names.SetName(buf);
			names.SetTag("Random");
		}
	}
	
}

} // namespace sandbox
} // namespace raid