#include <iostream>
#include <string>
#include <vector>
#include <sstream>

#include "engine/system/container.h"
#include "engine/system/singleton.h"
#include "engine/system/stringutil.h"

namespace raid
{

// Forward declaration
class CommandLineManager;

// Base class for Command Line Argument objcts
class CommandLineArg
{
public:

    CommandLineArg(CommandLineManager& manager, const wchar_t* name);
    virtual ~CommandLineArg();

    // Pure virtual parse function that each type of Arg will implement
    virtual bool Parse(const std::wstring& arg, const std::wstring& next) = 0;

    // Case sensitive name to pattern 
    const std::wstring& GetName() const { return m_Name; }

    bool HasValue() const { return m_HasValue; }

#if RAID_DEBUG
    using DisplayBuffer = char[256];
    virtual void GetDisplay(DisplayBuffer& buffer) = 0;
#endif

protected:

    CommandLineManager& m_Owner;
    std::wstring m_Name;
    bool m_HasValue;
};

// BoolArgument class
class BoolArgument : public CommandLineArg
{
public:
    BoolArgument(CommandLineManager& manager, const wchar_t* name)
        : CommandLineArg(manager, name), m_Value(false) {}

    bool Parse(const std::wstring& arg, const std::wstring& /* next */ ) override
    {
        // If we match the argument, simply enable the boolean argument.
        if (arg == L'-' + m_Name)
        {
            m_Value = true;
            m_HasValue = true;
        }

        return m_HasValue;
    }

    void Reset() { m_Value = false; m_HasValue = false; }
    void SetValue(bool value) { m_Value = value; m_HasValue = true; }
    bool GetValue() const { return m_Value; }
    operator bool() const { return m_Value; }

#if RAID_DEBUG
    void GetDisplay(CommandLineArg::DisplayBuffer& buffer) override
    {
        sprintf_s(buffer, "-%ls=%s", m_Name.c_str(), stringutil::LogBool(m_Value));
    }
#endif

private:
    bool m_Value;
};

// IntArgument class
class IntArgument : public CommandLineArg
{
public:

    IntArgument(CommandLineManager& manager, const wchar_t* name)
        : CommandLineArg(manager, name), m_Value(0) 
    {
    }

    bool Parse(const std::wstring& arg, const std::wstring& next) override;

    void Reset() { m_Value = 0; m_HasValue = false; }
    void SetValue(int value) { m_Value = value; m_HasValue = true; }
    int GetValue() const { return m_Value; }
    operator int() const { return m_Value; }

#if RAID_DEBUG
    void GetDisplay(DisplayBuffer& buffer) override
    {
        sprintf_s(buffer, "-%ls=%d", m_Name.c_str(), m_Value);
    }
#endif

private:
    int m_Value;
};

// StringArgument class
class StringArgument : public CommandLineArg
{
public:
    StringArgument(CommandLineManager& manager, const wchar_t* name)
        : CommandLineArg(manager, name)
    {
    }

    bool Parse(const std::wstring& arg, const std::wstring& next) override;

    void Reset() { m_Value.clear(); m_HasValue = false; }
    void SetValue(const wchar_t* value) { m_Value = value; m_HasValue = true; }
    const std::wstring& GetValue() const { return m_Value; }
    operator const wchar_t* () const { return m_Value.c_str(); }

#if RAID_DEBUG
    void GetDisplay(DisplayBuffer& buffer) override
    {
        sprintf_s(buffer, "-%ls=%ls", m_Name.c_str(), m_Value.c_str());
    }
#endif

private:
    std::wstring m_Value;
};

// CommandLineManager class
class CommandLineManager
{
public:

    void AddArgument(CommandLineArg* arg)
    {
        m_Args.push_back(arg);
    }

    void RemoveArgument(CommandLineArg* arg)
    {
        VectorRemove(m_Args, arg);
    }

    bool Init(const wchar_t* args);
    void Shutdown();
    size_t GetArgCount() { return m_Args.size(); }
    CommandLineArg* GetArg(size_t index)
    {
        if (index < m_Args.size())
            return m_Args[index];
        else
            return nullptr;
    }

private:

    std::vector<CommandLineArg*> m_Args;
};

class SCommandLineManager : public Singleton<CommandLineManager>
{

};

// Helper macro for arguments included in shipping versions of the game.
#define SHIPPING_BOOL_PARAM(name, desc) raid::BoolArgument Arg_ ## name(SCommandLineManager::Instance(), WIDESTR(name));
#define SHIPPING_INT_PARAM(name, desc) raid::IntArgument Arg_ ## name(SCommandLineManager::Instance(), WIDESTR(name));
#define SHIPPING_STRING_PARAM(name, desc) raid::StringArgument Arg_ ## name(SCommandLineManager::Instance(), WIDESTR(name));

// Helper macro for externally defined arguments
#define SHIPPING_BOOL_XPARAM(name) extern raid::BoolArgument Arg_ ## name;
#define SHIPPING_INT_XPARAM(name) extern raid::IntArgument Arg_ ## name;
#define SHIPPING_STRING_XPARAM(name) extern raid::StringArgument Arg_ ## name;

// Macros for debug parameters that are removed from shipping builds.
#if RAID_RELEASE
#define BOOL_PARAM(name)
#define BOOL_XPARAM(name)
#define INT_PARAM(name)
#define INT_XPARAM(name)
#define STRING_PARAM(name)
#define STRING_XPARAM(name)
#else
#define BOOL_PARAM(name, desc) SHIPPING_BOOL_PARAM(name, desc)
#define BOOL_XPARAM(name) SHIPPING_BOOL_XPARAM(name)
#define INT_PARAM(name, des) SHIPPING_BOOL_PARAM(name, desc)
#define INT_XPARAM(name) SHIPPING_BOOL_XPARAM(name)
#define STRING_PARAM(name, des) SHIPPING_INT_PARAM(name, desc)
#define STRING_XPARAM(name) SHIPPING_STRING_XPARAM(name)
#endif

// Helper struct to run ONE time if an argument is passed (for automation)
struct ArgAutomationHelper
{
    bool HasProcessed = false;

    ArgAutomationHelper(const BoolArgument& arg)
        : Arg(arg)
    {
    }

    bool Proceed()
    {
        if (HasProcessed)
            return false;

        HasProcessed = true;
        return Arg.GetValue();
    }

    const BoolArgument& Arg;
};

#define AUTOMATION_HELPER(name) \
    static ArgAutomationHelper Auto ## name(Arg_ ## name); \

#define AUTOMATION_HELPERX(name) \
    BOOL_XPARAM(name) \
    static ArgAutomationHelper Auto ## name(Arg_ ## name); \

} // namespace raid