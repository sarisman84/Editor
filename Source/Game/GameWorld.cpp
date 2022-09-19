#include "stdafx.h"
#include "GameWorld.h"
#include <tge/engine.h>
#include <tge/error/ErrorManager.h>
#include <tge/graphics/GraphicsEngine.h>
#include <tge/sprite/sprite.h>
#include <tge/drawers/SpriteDrawer.h>
#include <tge/texture/TextureManager.h>
#include <tge/drawers/ModelDrawer.h>

GameWorld::GameWorld()
{}

GameWorld::~GameWorld()
{}

void GameWorld::Init()
{
	myTga2dLogoTexture = Tga::Engine::GetInstance()->GetTextureManager().GetTexture(L"sprites/tga_logo.dds");
}
void GameWorld::Update(float aTimeDelta)
{
	UNREFERENCED_PARAMETER(aTimeDelta);
}

void GameWorld::Render()
{
	Tga::SpriteDrawer& spriteDrawer(Tga::Engine::GetInstance()->GetGraphicsEngine().GetSpriteDrawer());
	Tga::SpriteSharedData sharedData = {};
	sharedData.myTexture = myTga2dLogoTexture;

	Tga::Vector2ui intResolution = Tga::Engine::GetInstance()->GetRenderSize();
	Tga::Vector2f resolution = { (float)intResolution.x, (float)intResolution.y };

	// Create instance data. 
	Tga::Sprite2DInstanceData spriteInstance = {};
	spriteInstance.myPivot = { 0.5f, 0.5f };
	spriteInstance.myPosition = (Tga::Vector2f{ 0.5f, 0.5f } + myLogoPosition) * resolution;
	spriteInstance.mySize = Tga::Vector2f{ 0.75f, 0.75f }*resolution.y;
	spriteInstance.myRotation = myLogoRotation;

	spriteDrawer.Draw(sharedData, spriteInstance);
}