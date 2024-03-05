#include "engine/pch.h"

#include "encounter_serialization.h"
#include "encounter.h"

#include "engine/system/fileutil.h"

#include <iostream>
#include <fstream>
#include <sstream>

namespace raid
{

std::wstring EncounterSerialization::GetDefaultPath()
{
    std::wstring path = FileUtil::GetAppDataFolder();
    path += L"\\encounter.log";
    return path;
}

bool EncounterSerialization::Load(const std::wstring& filename, EncounterLog& encounterLog)
{
    std::ifstream infile(filename);
    if (!infile.is_open()) 
    {
        return false;
    }

    char delimiter = '\0';

    Encounter* encounter = nullptr;

    std::string line;
    while (std::getline(infile, line)) 
    {
        std::istringstream iss(line);

        EncounterEvent* evt = encounterLog.LoadEvent();
        if (!evt)
        {
            continue;
        }

        int etype;
        if (!(iss >> etype >> delimiter
            >> evt->m_Frame >> delimiter
            >> evt->m_Source >> delimiter
            >> evt->m_Target >> delimiter
            >> evt->m_ExtraData1.Int64 >> delimiter
            >> evt->m_ExtraData2.Int64))
        {
            return false;
        }

        evt->m_Type = (EncounterEventType)etype;

        if (evt->m_Type == EncounterEventType::EncounterStart)
        {
            encounter = encounterLog.CreateEncounter();
        }

        if (encounter)
        {
            encounter->GetEvents().push_back(evt->m_Node);
        }
    }

    infile.close();
    return true;
}

bool EncounterSerialization::Save(const std::wstring& filename, const EncounterLog& encounterLog)
{
    if (!FileUtil::CreateDirectoryForFile(filename))
    {
        return false;
    }

    std::ofstream outfile(filename);
    if (!outfile.is_open()) 
    {
        return false;
    }

    for (const Encounter* encounter : encounterLog.GetEncounterList())
    {
        for (const EncounterEvent& event : encounter->GetEvents())
        {
            outfile << event.m_Type << ","
                << event.m_Frame << ","
                << event.m_Source << ","
                << event.m_Target << ","
                << event.m_ExtraData1.Int64 << ","
                << event.m_ExtraData2.Int64 << std::endl;
        }
    }


    outfile.close();
    return true;
}

} // namespace raid