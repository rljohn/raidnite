#include "game.h"

// imgui
#include "imgui.h"

// 

namespace raid {
namespace sandbox {

void Game::Init()
{

}

void Game::Update()
{
	if (ImGui::Begin("Test"))
	{
		ImGui::End();
	}
}

void Game::Shutdown()
{
}

} // namespace sandbox
} // namespace raid