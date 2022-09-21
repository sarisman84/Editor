#include "stdafx.h"
#include "SceneRenderer.h"
#include <tge/texture/texture.h>
#include <tge/texture/TextureManager.h>
#include <tge/model/ModelInstance.h>
#include <tge/model/Model.h>
#include <tge/math/Vector.h>
#include <tge/graphics/GraphicsEngine.h>
#include <tge/graphics/DX11.h>
#include <tge/render/RenderCommon.h>
#include <tge/light/LightManager.h>
#include <tge/math/CommonMath.h>
#include <tge/drawers/ModelDrawer.h>

#include <algorithm>
#include "Utilities/CameraController.h"

void RenderPass(const Scene& aScene, unsigned int aModelIndex, std::vector<std::shared_ptr<Tga::Light>>& someLights, Tga::BlendState aBlendState, const unsigned int aMaxCount = 8)
{

	auto& lm = Tga::Engine::GetInstance()->GetLightManager();
	auto& gm = Tga::Engine::GetInstance()->GetGraphicsEngine();
	lm.ClearLights();

	if (aBlendState == Tga::BlendState::Disabled)
	{
		if (aScene.myAmbientLight)
			lm.SetAmbientLight(*aScene.myAmbientLight);
		if (aScene.myDirectionalLight)
			lm.SetDirectionalLight(*aScene.myDirectionalLight);
	}

	auto model = aScene.myModels[aModelIndex];
	model->myBlendState = aBlendState;

	//Tga::DX11::RenderStateManager->SetBlendState(aBlendState);



	int size = static_cast<int>(someLights.size());


	const int count = std::max(0, size - 1);
	const int awda = std::max(0, size - static_cast<int>(aMaxCount));
	if(!someLights.empty())
	for (int i = count; i >= awda; --i)
	{
		lm.AddLight(*someLights[i]);
		someLights.pop_back();
	}




	gm.GetModelDrawer().DrawPbr(*model);


}




bool IsInView(const Tga::Vector3f& aDirA, const Tga::Vector3f& aDirB) 
{
	float dot = aDirA.Dot(aDirB);

	return dot > 0.5f;
}


bool BoundsCheck(std::shared_ptr<Tga::ModelInstance> aModel, std::shared_ptr<Tga::Light> aLight)
{
	//Taken from this: https://developer.mozilla.org/en-US/docs/Games/Techniques/3D_collision_detection
	auto localModelCenter = aModel->GetModel()->GetMeshData(0).Bounds.Center;
	auto modelCenter = aModel->GetTransform().GetPosition() + localModelCenter;


	auto lightCenter = aLight->GetTransform().GetPosition();

	auto distance = std::abs((modelCenter - lightCenter).Length());
	auto modelRadius = aModel->GetTransform().GetScale().x * 100.0f;

	bool isSpotlight = aLight->GetInnerCone() != 0 && aLight->GetOuterCone() != 0;

	return distance <=  modelRadius + aLight->GetRange() && (isSpotlight ? IsInView(aLight->GetTransform().GetMatrix().GetForward(), (modelCenter - lightCenter)) : true);


}




void RegisterLights(const Scene& aScene, unsigned int anIndex, std::vector<std::shared_ptr<Tga::Light>>& anOutput)
{
	auto model = aScene.myModels[anIndex];

	size_t size = aScene.mySceneLights.size();

	for (size_t i = 0; i < size; i++)
	{
		if (BoundsCheck(model, aScene.mySceneLights[i]))
		{
			anOutput.push_back(aScene.mySceneLights[i]);
		}
	}
}

void SceneRendering::Render(const Scene& aNewScene, const CameraController& aCamera)
{
	Scene copy = aNewScene;

	Tga::Engine::GetInstance()->GetGraphicsEngine().SetCamera(*aCamera.GetCamera());

	Tga::DX11::BackBuffer->SetAsActiveTarget(Tga::DX11::DepthBuffer);
	Tga::DX11::RenderStateManager->SetDepthStencilState(Tga::DepthStencilState::WriteEqual);

	for (unsigned int i = 0; i < aNewScene.myModels.size(); i++)
	{
		if (aNewScene.myModels[i] == nullptr) continue;

		std::vector<std::shared_ptr<Tga::Light>> lights;

		//Get lights that affect model
		RegisterLights(aNewScene, i, lights);


		//Do a normal render pass
		RenderPass(aNewScene, i, lights, Tga::BlendState::Disabled);

		while (!lights.empty())
		{
			//Render any extra light sources as additive render passes.
			RenderPass(aNewScene, i, lights, Tga::BlendState::AdditiveBlend);
		}
	}

}


template<>
inline auto Reflect::RegisterElement<Tga::Light>()
{
	return Class("Light",
		Member("Color", &Tga::Light::myColor),
		Member("Inner Angle", &Tga::Light::myInnerAngle),
		Member("Outer Angle", &Tga::Light::myOuterAngle),
		Member("Range", &Tga::Light::myRange),
		Member("Transform", &Tga::Light::myTransform)
		);
}