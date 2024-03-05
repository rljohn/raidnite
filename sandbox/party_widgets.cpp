#include "sandbox/pch.h"
#include "party_widgets.h"

// sandbox
#include "sandbox_imgui.h"
#include "sandbox_game.h"

namespace raid {
namespace sandbox {

void PartyWidget::Init()
{
}

void PartyWidget::Draw(GameSandbox* sandbox)
{
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

		std::string display;
		if (!names.GetTitlePrefix().empty())
		{
			display += names.GetTitlePrefix();
			display += " ";
		}

		if (!names.GetName().empty())
		{
			display += names.GetName();
		}

		if (!names.GetTitleSuffix().empty())
		{
			display += ", ";
			display += names.GetTitleSuffix();
		}

		ImGui::Text("%s", display.c_str());

		if (!names.GetTag().empty())
		{
			ImGui::Text("<%s>", names.GetTag().c_str());
		}

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

		ImGui::PopID();
	}

	ImGui::Separator();

	ImGui::InputText("Name", m_NameBuf, COUNTOF(m_NameBuf));
	ImGui::InputText("Tag", m_TagBuf, COUNTOF(m_TagBuf));
	ImGui::InputText("Title Prefix", m_TitlePrefixBuf, COUNTOF(m_TitlePrefixBuf));
	ImGui::InputText("Title Suffix", m_TitleSuffixBuf, COUNTOF(m_TitleSuffixBuf));

	if (ImGui::Button("Add Unit"))
	{
		raid::UnitSpawner& spawner = sandbox->GetUnitSpawner();
		if (Unit* unit = dynamic_cast<Unit*>(spawner.SpawnEntity()))
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

void PartyWidget::Shutdown()
{
}

} // namespace sandbox
} // namespace raid