#pragma once

#include "args.h"

namespace raid
{

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