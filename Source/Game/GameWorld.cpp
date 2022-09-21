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
#include <random>

#include <tge/light/LightManager.h>

GameWorld::GameWorld()
{}

GameWorld::~GameWorld()
{}


void GameWorld::InitScene()
{
	InitLights(myScene);
	InitModels(myScene);
}


Tga::Vector3f RandomValue(Tga::Vector3f aMin, Tga::Vector3f aMax)
{
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_real_distribution<float> distX(aMin.x, aMax.x);
	std::uniform_real_distribution<float> distY(aMin.y, aMax.y);
	std::uniform_real_distribution<float> distZ(aMin.z, aMax.z);

	return Tga::Vector3f(distX(gen), distY(gen), distZ(gen));


}

void GameWorld::InitLights(Scene& aScene)
{
	auto ambientLight = Tga::AmbientLight(Tga::Color(1.f, 1.f, 1.f), 10.0f);
	auto directionalLight = Tga::DirectionalLight(Tga::Transform(Tga::Vector3f(), Tga::Quaternionf(90.0f, 0.0f, 0.0f)), Tga::Color(1.0f, 1.0f, 1.0f), 10.0f);


	aScene.myAmbientLight = std::make_shared<Tga::AmbientLight>(ambientLight);
	aScene.myDirectionalLight = std::make_shared<Tga::DirectionalLight>(directionalLight);


	constexpr size_t pointLightAmm = 40;
	//constexpr size_t spotLightAmm = 40;

	const Tga::Vector3f bounds = Tga::Vector3f(15.f, 0.0f, 15.f);



	//Pointlight
	for (size_t i = 0; i < pointLightAmm; i++)
	{
		auto color = RandomValue(Tga::Vector3f(0, 0, 0), Tga::Vector3f(1, 1, 1));

		Tga::PointLight pl = Tga::PointLight(
			Tga::Transform(RandomValue(bounds * -1.0f, bounds)), Tga::Color(color.x, color.y, color.z), 10.0f, 10.0f);


		aScene.mySceneLights.push_back(std::make_shared<Tga::PointLight>(pl));

	}
}


void GameWorld::InitModels(Scene& aScene)
{
	constexpr float ToTGAUnits = 100.0f;

	//Floor
	auto& MF = Tga::ModelFactory::GetInstance();
	auto floor = MF.GetUnitCube();
	floor.SetLocation({ 0.0f, -1.0f, 0.0f });
	floor.SetScale(Tga::Vector3f{ 100.0f, 1.f, 100.0f } / ToTGAUnits);

	aScene.myModels.push_back(std::make_shared<Tga::ModelInstance>(floor));

	//Random Cubes
	constexpr float gridWidth = 10;
	constexpr float gridHeight = 10;
	constexpr float tileSpacing = 2.0f;

	for (float x = -(gridWidth / 2.0f); x < gridWidth / 2.0f; x++)
	{
		for (float z = -(gridHeight / 2.0f); z < gridHeight / 2.0f; z++)
		{
			auto cube = MF.GetUnitCube();

			cube.SetLocation(Tga::Vector3f{ x * tileSpacing, 0.0f, z * tileSpacing });
			cube.SetScale(Tga::Vector3f{ 1.0f, 1.0f, 1.0f } / ToTGAUnits);

			aScene.myModels.push_back(std::make_shared<Tga::ModelInstance>(cube));
		}
	}
}

void GameWorld::Init()
{
	InitScene();

	myCamera.Initialize(0, 0);
}
void GameWorld::Update(float aTimeDelta)
{
	UNREFERENCED_PARAMETER(aTimeDelta);

	myCamera.UpdateController(aTimeDelta);
}

void GameWorld::Render()
{
	SceneRendering::Render(myScene, myCamera);
}