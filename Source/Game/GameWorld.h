#pragma once
#include <tge/render/RenderCommon.h>
#include "SR/ReflectHelper.h"
namespace Tga
{
	class Texture;
}

class GameWorld
{
public:
	GameWorld();
	~GameWorld();

	void Init();
	void Update(float aTimeDelta);
	void Render();

	Tga::Vector2f myLogoPosition;
	float myLogoRotation;

private:
	Tga::Texture* myTga2dLogoTexture;


};



template<>
inline auto Reflect::RegisterElement<GameWorld>()
{
	return Class("Game World",
		Member("Logo Position", &GameWorld::myLogoPosition),
		Member("Logo Rotation", &GameWorld::myLogoRotation)
	);
}

