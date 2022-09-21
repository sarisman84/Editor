#pragma once
#include <vector>
#include <memory>
#include "SR/ReflectHelper.h"

class CameraController;

namespace Tga
{
	class Light;
	class AmbientLight;
	class DirectionalLight;
	class ModelInstance;

}


struct Scene
{
	std::vector<std::shared_ptr<Tga::Light>> mySceneLights;
	std::vector<std::shared_ptr<Tga::ModelInstance>> myModels;

	std::shared_ptr<Tga::AmbientLight> myAmbientLight;
	std::shared_ptr<Tga::DirectionalLight> myDirectionalLight;
};







namespace SceneRendering
{
	void Render(const Scene& aNewScene, const CameraController& aCamera);
}
