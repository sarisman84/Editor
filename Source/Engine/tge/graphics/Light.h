#pragma once
#include <tge/Math/Color.h>
#include <tge/Math/Transform.h>
#include <tge/Math/Vector.h>

namespace Tga
{
	class Light
	{
	public:
		Color myColor;
		float myIntensity;
		Transform myTransform;
		float myRange = 10.0f;
		float myInnerAngle, myOuterAngle;

	public:

		virtual ~Light() = default;

		Light(
			const Transform& someTransform,
			const Color& aColor,
			float aIntensity);

		FORCEINLINE float GetIntensity() const { return myIntensity; }
		FORCEINLINE Color GetColor() const { return myColor; }

		FORCEINLINE Transform GetTransform() const { return myTransform; }
		FORCEINLINE void SetTransform(Transform newTransform) { myTransform = newTransform; }
		FORCEINLINE void SetPosition(Vector3f newPosition) { myTransform.SetPosition(newPosition); }
		FORCEINLINE void SetRotation(Rotator newRotation) { myTransform.SetRotation(newRotation); }
		FORCEINLINE void SetColor(Color newColor) { myColor = newColor; }
		FORCEINLINE void SetIntensity(float newIntensity) { myIntensity = newIntensity; }

		FORCEINLINE float GetRange() const { return myRange; }
		FORCEINLINE float GetInnerCone() const { return myInnerAngle; }
		FORCEINLINE float GetOuterCone() const { return myOuterAngle; }



		FORCEINLINE void SetRange(float aValue) { myRange = aValue; }
		FORCEINLINE void SetInnerCone(float aValue) { myInnerAngle = aValue; }
		FORCEINLINE void SetOuterCone(float aValue) { myOuterAngle = aValue; }

	};

} // namespace Tga