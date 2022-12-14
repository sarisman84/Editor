#include "stdafx.h"
#include <tge/light/LightManager.h>
#include <tge/engine.h>

using namespace Tga;

LightManager::LightManager()
	: myAmbientLight(Color{ 1, 1, 1 }, 1.0)
	, myDirectionalLight({}, Color{ 0, 0, 0 }, 0.0)
{}

LightManager::~LightManager()
{
}

void LightManager::AddPointLight(const PointLight& aPointLight)
{
	if (myLights.size() >= NUMBER_OF_LIGHTS_ALLOWED)
	{
		ERROR_PRINT("%s%i%s", "We only allow ", NUMBER_OF_LIGHTS_ALLOWED, " lights at the same time and you are trying to push more");
		return;
	}
	myLights.push_back(aPointLight);
}

void Tga::LightManager::AddSpotLight(const Spotlight& aSpotLight)
{
	if (myLights.size() >= NUMBER_OF_LIGHTS_ALLOWED)
	{
		ERROR_PRINT("%s%i%s", "We only allow ", NUMBER_OF_LIGHTS_ALLOWED, " lights at the same time and you are trying to push more");
		return;
	}
	myLights.push_back(aSpotLight);
}

void Tga::LightManager::AddLight(const Light& aLight)
{
	if (myLights.size() >= NUMBER_OF_LIGHTS_ALLOWED)
	{
		ERROR_PRINT("%s%i%s", "We only allow ", NUMBER_OF_LIGHTS_ALLOWED, " lights at the same time and you are trying to push more");
		return;
	}
	myLights.push_back(aLight);
}
