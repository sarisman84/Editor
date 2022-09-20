#pragma once
#include <vector>
#include <tge/graphics/AmbientLight.h>
#include <tge/graphics/DirectionalLight.h>
#include <tge/graphics/PointLight.h>
#include <tge/graphics/Spotlight.h>

namespace Tga
{
	class LightManager
	{
	public:
		LightManager();
		~LightManager();

		/*size_t GetPointLightCount() const { return myPointLights.size(); }
		size_t GetSpotLightCount() const { return mySpotLights.size(); }*/
		/*size_t GetLightCount() const { return GetPointLightCount() + GetSpotLightCount() + 2; }*/

		inline size_t GetLightCount() const { return myLights.size(); }

		void ClearLights() { myLights.clear(); }
		void AddPointLight(const PointLight& aPointLight);
		void AddSpotLight(const Spotlight& aSpotLight);
		void AddLight(const Light& aLight);
		const Light* GetLights() const { return myLights.data(); };




		void SetDirectionalLight(DirectionalLight light) { myDirectionalLight = light; }
		const DirectionalLight& GetDirectionalLight() const { return myDirectionalLight; }

		void SetAmbientLight(AmbientLight light) { myAmbientLight = light; }
		const AmbientLight& GetAmbientLight() const { return myAmbientLight; }

	private:
		/*std::vector<PointLight> myPointLights;
		std::vector<Spotlight> mySpotLights;*/

		std::vector<Light> myLights;

		DirectionalLight myDirectionalLight;
		AmbientLight myAmbientLight;
	};
}

