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


template<>
inline auto Reflect::RegisterElement<Scene>()
{
	return Class("Scene",
		Member("Lights", &Scene::mySceneLights),
		Member("Models", &Scene::myModels),
		Member("Ambient Light", &Scene::myAmbientLight),
		Member("Directional Light", &Scene::myDirectionalLight)
	);
}




namespace SceneRendering
{
	void Render(const Scene& aNewScene, const CameraController& aCamera);
}
