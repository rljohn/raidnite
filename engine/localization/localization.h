#pragma once

#include "engine/types.h"
#include "engine/system/delegate.h"

namespace raid
{
	enum class Language
	{
		English = 0,
		French = 1,
		Italian = 2,
		German = 3,
		Spanish = 4,
		Japanese = 5,
		Korean = 6,
		ChineseSimplified = 7,
		ChineseTraditional = 8
	};

	struct LocalizationSet
	{
	public:

		void AddEntry(size_t hash, std::string text);
		const char* GetEntry(size_t hash) const;
		void Reset();

	private:

		std::unordered_map<size_t, std::string> m_Strings;
	};

	class LocalizationSystem
	{
	public:

		LocalizationSystem();

		// Language Change
		void SetLanguage(Language language);
		Language GetLanguage() const { return m_Language; }

		// Language Change Events
		using LanguageChangedDelegate = Delegate<Language>;
		LanguageChangedDelegate& LanguageChanged() { return m_LanguageChangeDlgt; }

		void AddLocalizationSet(LocalizationSet* set);
		void RemoveLocalizationSet(LocalizationSet* set);

		// Finds a localization string from any of the registered localization sets
		const char* GetEntry(size_t hash);

	private:

		Language m_Language;
		LanguageChangedDelegate m_LanguageChangeDlgt;

		std::list<LocalizationSet*> m_LocaliationSets;
	};

	class LocalizationIO
	{
	public:

		static bool LoadLocalizationData(const char* data, Language language, LocalizationSet& system);

	private:

		static Language GetLanguage(const std::string& lang);
		static const std::string& GetLanguage(Language lang);
	};

} // namespace raid
