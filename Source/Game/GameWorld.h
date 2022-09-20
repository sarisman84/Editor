#pragma once
#include <tge/render/RenderCommon.h>
#include "SR/ReflectHelper.h"
#include "Utilities/CameraController.h"
#include "SceneRenderer.h"
namespace Tga
{
	class Texture;
	class ModelInstance;
}


struct Object
{
	Tga::Vector3f myPosition;
	Tga::Vector3f mySize;
	Tga::Vector3f myRotation;

};


template<>
inline auto Reflect::RegisterElement<Object>()
{
	return Class("Game World",
		Member("Position", &Object::myPosition),
		Member("Size", &Object::mySize),
		Member("Rotation", &Object::myRotation)
	);
}


class GameWorld
{
public:
	GameWorld();
	~GameWorld();

	void Init();
	void Update(float aTimeDelta);
	void Render();

	CameraController myCamera;
	Scene myScene;

};






template<>
inline auto Reflect::RegisterElement<GameWorld>()
{
	return Class("Game World",
		Member("Main Camera", &GameWorld::myCamera),
		Member("Scene", &GameWorld::myScene)
	);
}

