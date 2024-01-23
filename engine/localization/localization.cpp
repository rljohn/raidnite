#include "engine/pch.h"

#include "localization.h"
#include "engine/system/check.h"
#include "engine/system/json.h"

namespace raid
{

///////////////////////////////
// LocalizationSet
///////////////////////////////

void LocalizationSet::AddEntry(size_t hash, std::string text)
{
    m_Strings.insert_or_assign(hash, text);
}

const char* LocalizationSet::GetEntry(size_t hash) const
{
    auto it = m_Strings.find(hash);
    if (it != m_Strings.end())
    {
        return it->second.c_str();
    }
    else
    {
        return nullptr;
    }
}

void LocalizationSet::Reset()
{
    m_Strings.clear();
}

///////////////////////////////
// LocalizationSystem
///////////////////////////////

LocalizationSystem::LocalizationSystem()
    : m_Language(Language::English)
{
}

void LocalizationSystem::SetLanguage(Language language)
{
    if (m_Language != language)
    {
        m_Language = language;
        m_LanguageChangeDlgt.Invoke(language);
    }
}

void LocalizationSystem::AddLocalizationSet(LocalizationSet* set)
{
    m_LocaliationSets.push_back(set);
}

void LocalizationSystem::RemoveLocalizationSet(LocalizationSet* set)
{
    m_LocaliationSets.remove(set);
}

const char* LocalizationSystem::GetEntry(size_t hash)
{
    for (const LocalizationSet* locSet : m_LocaliationSets)
    {
        const char* result = locSet->GetEntry(hash);
        if (result)
        {
            return result;
        }
    }

    return "<MISSING TEXT>";
}

///////////////////////////////
// LocalizationIO
///////////////////////////////

struct langaugeMapping
{
    Language lang;
    std::string langStr;
};

static langaugeMapping languageMap[] =
{
    { Language::English, "en" },
    { Language::French, "fr" },
    { Language::Italian, "it" },
    { Language::German, "de" },
    { Language::Spanish, "es" },
    { Language::Japanese, "ja" },
    { Language::Korean, "ko" },
    { Language::ChineseSimplified, "zhCN" },
    { Language::ChineseTraditional, "zhTW" }
};

Language LocalizationIO::GetLanguage(const std::string& lang)
{
    for (const langaugeMapping& m : languageMap)
    {
        if (m.langStr == lang)
            return m.lang;
    }

    return Language::English;
}

const std::string& LocalizationIO::GetLanguage(Language lang)
{
    for (const langaugeMapping& m : languageMap)
    {
        if (m.lang == lang)
            return m.langStr;
    }

    return languageMap[0].langStr;
}

struct localizationData
{
    std::string key;
    std::unordered_map<std::string, std::string> text;
};

struct localizationFile
{
    int version = 0;
    std::vector<localizationData> strings;
};

NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(localizationData, key, text);
NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(localizationFile, version, strings);

bool LocalizationIO::LoadLocalizationData(const char* data, Language language, LocalizationSet& system)
{
    using json = nlohmann::json;

    stry
    {
        json j = json::parse(data);
        scheckall(!j.is_discarded());

        localizationFile file = j;

        const std::string& langStr = GetLanguage(language);
        for (const localizationData& data : file.strings)
        {
            auto it = data.text.find(langStr);
            if (it != data.text.end())
            {
                size_t hash = StringHash{}(data.key);
                system.AddEntry(hash, it->second);
            }
        }

        return true;
    }
    sfinally
    {

    }

    return false;
}

} // namespace raid

