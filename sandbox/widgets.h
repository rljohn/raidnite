#pragma once

#include "thirdparty/imgui/imgui.h"

namespace raid {
namespace sandbox {

class Widget
{
public:

	virtual void Init() = 0;
	virtual void Draw() = 0;
	virtual void Shutdown() = 0;
	virtual const char* GetName() = 0;
};

} // namespace sandbox
} // namespace raid