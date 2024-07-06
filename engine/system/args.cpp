#include "engine/pch.h"

#include "args.h"

#include <Windows.h>
#include <shellapi.h>

#include <string>

namespace raid
{

///////////////////////////////////
// CommandLineArg
///////////////////////////////////

CommandLineArg::CommandLineArg(CommandLineManager& manager, const wchar_t* name)
    : m_Name(name)
    , m_HasValue(false)
    , m_Owner(manager)
{
    // Register at creation
    m_Owner.AddArgument(this);
}

CommandLineArg::~CommandLineArg()
{
    // Auto-Unregister when the argument is removed
    m_Owner.RemoveArgument(this);
}

bool IntArgument::Parse(const std::wstring& arg, const std::wstring& next)
{
    // Is this a two-part argument with a space?
    std::wstring n = L'-' + m_Name;
    if (arg == n)
    {
        // If a next value was provided, try to parse it as an int
        if (!next.empty())
        {
            wchar_t* endPtr = nullptr;
            m_Value = wcstol(next.c_str(), &endPtr, 10);
            m_HasValue = (endPtr != next.c_str());
        }
    }
    else
    {
        // One-part argument with equals delimiter?
        std::wstring prefix = n + L'=';
        if (arg.find(prefix) == 0)
        {
            // Parse the RHS as an integer
            wchar_t* endPtr = nullptr;
            std::wstring suffix = arg.substr(prefix.size());
            m_Value = wcstol(suffix.c_str(), &endPtr, 10);
            m_HasValue = (endPtr != suffix.c_str());
        }
    }

    return m_HasValue;
}

bool StringArgument::Parse(const std::wstring& arg, const std::wstring& next)
{
    // Is this a two-part argument with a space?
    std::wstring n = L'-' + m_Name;
    if (arg == n)
    {
        if (!next.empty())
        {
            m_Value = next;
            m_HasValue = true;
        }
    }
    else
    {
        // One-part argument with equals delimiter?
        std::wstring prefix = n + L'=';
        if (arg.find(prefix) == 0)
        {
            m_Value = arg.substr(prefix.size());
            m_HasValue = true;
        }
    }

    return m_HasValue;
}

//////////////////////////////////////
// CommandLineManager
//////////////////////////////////////

bool CommandLineManager::Init(const wchar_t* args)
{
    int nArgs = 0;
    LPWSTR* szArglist = ::CommandLineToArgvW(args, &nArgs);
    if (szArglist == nullptr)
    {
        return false;
    }

    std::vector<std::wstring> arguments;
    for (int i = 1; i < nArgs; ++i)
    {
        std::wstring ws(szArglist[i]);
        std::wstring arg(ws.begin(), ws.end());
        arguments.push_back(arg);
    }
    ::LocalFree(szArglist);

    std::wstring empty;
    for (int j = 0; j < arguments.size(); j++)
    {
        const std::wstring& arg = arguments[j];

        if (arg.empty() || arg[0] != '-')
            continue;

        const std::wstring& next = j < arguments.size() - 1 ? arguments[j + 1] : empty;
        
        for (const auto& cmdArg : m_Args)
        {
            if (cmdArg->HasValue())
                continue;

            if (cmdArg->Parse(arg, next))
            {
                break;
            }
        }
    }
    return true;
}

void CommandLineManager::Shutdown()
{
    m_Args.clear();
}

} // namespace raid