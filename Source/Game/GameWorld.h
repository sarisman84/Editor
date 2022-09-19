#pragma once
#include <tge/render/RenderCommon.h>

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
private:
	Tga::Texture* myTga2dLogoTexture;
};