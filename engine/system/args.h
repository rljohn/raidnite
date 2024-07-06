#include <iostream>
#include <string>
#include <vector>
#include <sstream>

#include "engine/system/container.h"

namespace raid
{

// Forward declaration
class CommandLineManager;

// Base class for Command Line Argument objcts
class CommandLineArg
{
public:

    CommandLineArg(const std::wstring& name);
    virtual ~CommandLineArg();

    // Pure virtual parse function that each type of Arg will implement
    virtual bool Parse(const std::wstring& arg, const std::wstring& next) = 0;

    // Case sensitive name to pattern 
    const std::wstring& GetName() const { return m_Name; }

    bool HasValue() const { return m_HasValue; }

protected:

    std::wstring m_Name;
    bool m_HasValue;
};

// BoolArgument class
class BoolArgument : public CommandLineArg
{
public:
    BoolArgument(const std::wstring& name)
        : CommandLineArg(name), m_Value(false) {}

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

    bool GetValue() const { return m_Value; }
    operator bool() const { return m_Value; }

private:
    bool m_Value;
};

// IntArgument class
class IntArgument : public CommandLineArg
{
public:

    IntArgument(const std::wstring& name)
        : CommandLineArg(name), m_Value(0) 
    {
    }

    bool Parse(const std::wstring& arg, const std::wstring& next) override;

    int GetValue() const { return m_Value; }
    operator int() const { return m_Value; }

private:
    int m_Value;
};

// StringArgument class
class StringArgument : public CommandLineArg
{
public:
    StringArgument(const std::wstring& name)
        : CommandLineArg(name)
    {
    }

    bool Parse(const std::wstring& arg, const std::wstring& next) override;

    const std::wstring& GetValue() const { return m_Value; }
    operator const wchar_t* () const { return m_Value.c_str(); }

private:
    std::wstring m_Value;
};

// CommandLineManager class
class CommandLineManager
{
public:

    static void AddArgument(CommandLineArg* arg)
    {
        m_Args.push_back(arg);
    }

    static void RemoveArgument(CommandLineArg* arg)
    {
        VectorRemove(m_Args, arg);
    }

    static bool Init(const wchar_t* args);
    static void Shutdown();
    static size_t GetArgCount() { return m_Args.size(); }

private:
    static std::vector<CommandLineArg*> m_Args;
};

} // naemspace raid