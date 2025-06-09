#include "sandbox/pch.h"
#include "sandbox_game.h"

// raid
#include "engine/encounter/encounter_log.h"
#include "engine/encounter/encounter.h"
#include "engine/encounter/encounter_data.h"
#include "engine/system/log/logging.h"

// sandbox
#include "log_widgets.h"

#include <algorithm>

namespace raid {
namespace sandbox {

void LogWidget::Init()
{
	SetEnabled(true);

    for (bool& v : m_Filter)
    {
        v = true;
    }
}

void LogWidget::Draw(GameSandbox* sandbox)
{
    IEncounterLog* log = raid::Game::GetEncounterLog();
    if (!log)
        return;

    DrawFilters();

    const std::vector<Encounter*>& encounters = log->GetEncounterList();
    if (encounters.size() == 0)
        return;

    int i = 0;
    for (const Encounter* e : encounters)
    {
        EncounterLog::DisplayString buffer;
        log->GetDisplayString(*e, buffer);

        char display[256] = { 0 };
        sprintf_s(display, "%s###Log%d", buffer, i);

        if (ImGui::TreeNode(display))
        {
            for (const auto& evt : e->GetEvents())
            {
                DrawEvent(log, evt);
            }

            ImGui::TreePop();
        }
    }
}

const char* EncounterEventTypeToString(EncounterEventType eventType)
{
    switch (eventType)
    {
    case EncounterEventType::Invalid: return "Invalid";
    case EncounterEventType::GameStart: return "GameStart";
    case EncounterEventType::GameEnd: return "GameEnd";
    case EncounterEventType::ZoneEnter: return "ZoneEnter";
    case EncounterEventType::ZoneExit: return "ZoneExit";
    case EncounterEventType::EncounterStart: return "EncounterStart";
    case EncounterEventType::EncounterEnd: return "EncounterEnd";
    case EncounterEventType::TilePropertiesChanged: return "TilePropertiesChanged";
    case EncounterEventType::EntityCreated: return "EntityCreated";
    case EncounterEventType::EntityDied: return "EntityDied";
    case EncounterEventType::EntityDestroyed: return "EntityDestroyed";
    case EncounterEventType::AbilityStart: return "AbilityStart";
    case EncounterEventType::AbilityEnd: return "AbilityEnd";
    case EncounterEventType::HealthChanged: return "HealthChanged";
    case EncounterEventType::ManaChanged: return "ManaChanged";
    case EncounterEventType::AuraGained: return "AuraGained";
    case EncounterEventType::AuraRefreshed: return "AuraRefreshed";
    case EncounterEventType::AuraRemoved: return "AuraRemoved";
    case EncounterEventType::OccupancyChanged: return "OccupancyChanged";
    case EncounterEventType::PositionChanged: return "PositionChanged";
    case EncounterEventType::TargetChanged: return "TargetChanged";
    case EncounterEventType::AggroBegin: return "AggroBegin";
    case EncounterEventType::AggroEnd: return "AggroEnd";
    default: 
        mainAssertf(false, "Unknown event type: %d", eventType);
        return "Unknown";
    }
}

void LogWidget::DrawFilters()
{
    if (!ImGui::TreeNode("Filter"))
        return;

    for (int i = 0; i < EncounterEventType::MAX; i++)
    {
        ImGui::Checkbox(EncounterEventTypeToString((EncounterEventType)i), &m_Filter[i]);
    }

    ImGui::TreePop();
}

 void LogWidget::DrawEvent(IEncounterLog* log, const EncounterEvent & evt)
{
     if (!m_Filter[evt.m_Type])
         return;

     EncounterLog::DisplayString buffer;
     log->GetDisplayString(evt, buffer);

     ImGui::Text("[%s] %s", buffer, EncounterEventTypeToString(evt.m_Type));

     switch (evt.m_Type)
     {
     case EncounterEventType::Invalid:
         break;
     case EncounterEventType::GameStart:
         break;
     case EncounterEventType::GameEnd:
         break;
     case EncounterEventType::ZoneEnter:
         break;
     case EncounterEventType::ZoneExit:
         break;
     case EncounterEventType::EncounterStart:
         break;
     case EncounterEventType::EncounterEnd:
         break;
     case EncounterEventType::EntityCreated:
         break;
     case EncounterEventType::EntityDied:
         break;
     case EncounterEventType::EntityDestroyed:
         break;
     case EncounterEventType::AbilityStart:
         break;
     case EncounterEventType::AbilityEnd:
         break;
     case EncounterEventType::HealthChanged:
         break;
     case EncounterEventType::ManaChanged:
         break;
     case EncounterEventType::AuraGained:
         break;
     case EncounterEventType::AuraRefreshed:
         break;
     case EncounterEventType::AuraRemoved:
         break;
     case EncounterEventType::OccupancyChanged:
     {
         ImGui::SameLine();
         EntityOccupancyChangedEvent e(nullptr);
         EncounterData::UnpackageData(evt, e);
         ImGui::Text("%lld: %d,%d", evt.m_Source, e.m_Position.GetX(), e.m_Position.GetY());
         break;
     }
     case EncounterEventType::PositionChanged:
     {
         ImGui::SameLine();
         EntityPositionChangedEvent e(nullptr);
         EncounterData::UnpackageData(evt, e);
         ImGui::Text("%lld: %d,%d -> %d,%d", evt.m_Source, 
             e.m_Previous.GetX(), e.m_Previous.GetY(), 
             e.m_Position.GetX(), e.m_Position.GetY());
         break;
     }
     case EncounterEventType::TargetChanged:
         ImGui::SameLine();
         ImGui::Text("%lld: %lld", evt.m_Source, evt.m_Target);
         break;
     case EncounterEventType::AggroBegin:
         ImGui::SameLine();
         ImGui::Text("%lld: %lld", evt.m_Source, evt.m_Target);
         break;
     case EncounterEventType::AggroEnd:
         ImGui::SameLine();
         ImGui::Text("%lld: %lld", evt.m_Source, evt.m_Target);
         break;
     default:
         break;
     }
}

void LogWidget::Shutdown()
{
}

} // namespace sandbox
} // namespace raid