#include "stdafx.h"
#include "GameWorld.h"
#include <tge/engine.h>
#include <tge/error/ErrorManager.h>
#include <tge/graphics/GraphicsEngine.h>
#include <tge/sprite/sprite.h>
#include <tge/drawers/SpriteDrawer.h>
#include <tge/texture/TextureManager.h>
#include <tge/drawers/ModelDrawer.h>
#include <tge/model/ModelFactory.h>
#include <tge/model/ModelInstance.h>

GameWorld::GameWorld()
{}

GameWorld::~GameWorld()
{}

void GameWorld::Init()
{
	myCamera.Initialize(0,0);
}
void GameWorld::Update(float aTimeDelta)
{
	UNREFERENCED_PARAMETER(aTimeDelta);
}

void GameWorld::Render()
{
	/*auto& graphicsEngine = Tga::Engine::GetInstance()->GetGraphicsEngine();

	graphicsEngine.SetCamera(*myCamera.GetCamera());
	auto cpy = myObjects;
	for (auto& obj : cpy)
	{
		auto cube = Tga::ModelFactory::GetInstance().GetUnitCube();
		cube.SetLocation(obj.myPosition);
		cube.SetRotation(obj.myRotation);
		cube.SetScale(obj.mySize);

		graphicsEngine.GetModelDrawer().Draw(cube);
	}*/

	SceneRendering::Render(myScene, myCamera);
}