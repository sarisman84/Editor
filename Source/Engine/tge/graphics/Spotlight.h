#pragma once
#include "Light.h"

namespace Tga
{
	class Spotlight : public Light
	{


	public:
		Spotlight(
			const Transform& someTransform,
			const Color& aColor,
			float aIntensity,
			float someRange,
			float anInnerCone,
			float anOuterCone
		) : Light(someTransform, aColor, aIntensity)
		{
			myInnerAngle = anInnerCone;
			myOuterAngle = anOuterCone;
			myRange = someRange;
		}
		virtual ~Spotlight() = default;

		void SetRange(float someRange);
		void SetInnerCone(float anInnerCone);
		void SetOuterCone(float anOuterCone);




	
		

	};

}

