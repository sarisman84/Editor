#pragma once
#include <tge/math/Transform.h>
#include <Dragonite/Input.h>

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

	inline bool& InputState()noexcept
	{
		return myInputState;
	}

private:
	bool myInputState;

	
	Tga::Transform myTransform;
	float myMovementSpeed;
	float myRotationSpeed;
	float mySprintMultipler;
	Tga::Rotator myAngularVelocity;
	Tga::Vector3f myVelocity;

	

	Tga::Camera* myCamera = nullptr;
};

