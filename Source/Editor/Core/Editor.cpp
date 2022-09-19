#include "Editor.h"
#include "GameWorld.h"

#include "SR/ReflectHelper.h"
#include <nlohmann/json.hpp>
#include <imgui/imgui.h>

void Editor::Update(GameWorld* aGameWorld)
{
	using namespace nlohmann;

	ImGui::Begin("Property Editor");

	Reflect::InspectMembers<GameWorld>(aGameWorld, "Game World");

	ImGui::End();
}
