#pragma once

#include "widgets.h"

namespace raid {
namespace sandbox {

class EngineWidget : public Widget
{
public:

	void Init() override;
	void Draw(GameSandbox* sandbox) override;
	const char* GetName() override { return "Engine"; }

private:

};

} // namespace sandbox
} // namespace raid