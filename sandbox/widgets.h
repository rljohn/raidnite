#pragma once

#include "thirdparty/imgui/imgui.h"

namespace raid {
namespace sandbox {

class GameSandbox;
class Widget
{
public:

	virtual void Init() {}
	virtual void Draw(GameSandbox* sandbox) = 0;
	virtual void Shutdown() {}
	virtual const char* GetName() = 0;
};

} // namespace sandbox
} // namespace raid