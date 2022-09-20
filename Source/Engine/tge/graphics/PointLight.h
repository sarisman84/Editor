#pragma once
#include "Light.h"

namespace Tga
{

	class PointLight : public Light
	{
	protected:


	public:
		PointLight(
			const Transform& someTransform, 
			const Color& aColor, 
			float aIntensity, 
			float someRange
		) : Light(someTransform, aColor, aIntensity) 
		{ 
			myRange = someRange; 
		}
		virtual ~PointLight() = default;

		void SetRange(float someRange);

		// Inherited via Light
	
	};

} // namespace Tga