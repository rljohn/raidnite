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

	bool IsEnabled() const
	{
		return m_Enabled;
	}

	void SetEnabled(bool enabled)
	{
		m_Enabled = enabled;
	}

protected:

	bool m_Enabled = false;
};

} // namespace sandbox
} // namespace raid