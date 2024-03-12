#pragma once

#include "component.h"

namespace raid
{

class NameComponent : public Component
{
public:
    
    NameComponent(Entity& parent)
        : Component(parent)
    {
    }

    // Getters
    const std::string& GetName() const { return m_Name; }
    const std::string& GetTitlePrefix() const { return m_TitlePrefix; }
    const std::string& GetTitleSuffix() const { return m_TitleSuffix; }
    const std::string& GetTag() const { return m_Tag; }

    // Setters
    void SetName(const std::string& name) { m_Name = name; }
    void SetTitlePrefix(const std::string& titlePrefix) { m_TitlePrefix = titlePrefix; }
    void SetTitleSuffix(const std::string& titleSuffix) { m_TitleSuffix = titleSuffix; }
    void SetTag(const std::string& tag) { m_Tag = tag; }

private:

	std::string m_Name;
	std::string m_TitlePrefix;
	std::string m_TitleSuffix;
	std::string m_Tag;
};

}