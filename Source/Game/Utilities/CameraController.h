#pragma once
#include <tge/math/Transform.h>
#include <Dragonite/Input.h>
#include "../SR/ReflectHelper.h"

namespace Tga {
	class Camera;
	class InputManager;

}




class CameraController
{
public:
	


	CameraController();
	~CameraController();
	void Initialize(float aMovementSpeedAmm, float aRotationSpeedAmm, float someFOV = 90.0f, float aMinPlaneVal = 0.01f, float aFarPlaneVal = 1000000.0f);
	void UpdateController(const float aDt);
	inline Tga::Camera* GetCamera() const noexcept
	{
		return myCamera;
	}


	Tga::Vector3f myPosition;
	Tga::Vector3f myRotation;
	float myMovementSpeed;
	float myRotationSpeed;
	float mySprintMultipler;

	inline bool& InputState()noexcept
	{
		return myInputState;
	}

private:
	bool myInputState;

	
	Tga::Transform myTransform;
	
	Tga::Rotator myAngularVelocity;
	Tga::Vector3f myVelocity;

	

	Tga::Camera* myCamera = nullptr;
};



template<>
inline auto Reflect::RegisterElement<CameraController>()
{
	return Class("Camera Controller",
		Member("Position", &CameraController::myPosition),
		Member("Rotation", &CameraController::myRotation),
		Member("Movement Speed", &CameraController::myMovementSpeed),
		Member("Rotation Speed", &CameraController::myRotationSpeed),
		Member("Sprint Multiplier", &CameraController::mySprintMultipler)
	);
}
