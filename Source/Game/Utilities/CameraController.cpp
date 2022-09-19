#include "stdafx.h"
#include "CameraController.h"
#include <tge/graphics/Camera.h>

#include <tge/engine.h>


CameraController::CameraController() = default;



void CameraController::Initialize(float aMovementSpeedAmm, float aRotationSpeedAmm, float someFOV, float aMinPlaneVal, float aFarPlaneVal)
{
	using CommonUtilities::Mouse;

	Mouse::Init();

	myCamera = new Tga::Camera();

	Tga::Vector2ui intResolution = Tga::Engine::GetInstance()->GetRenderSize();
	Tga::Vector2f resolution = { (float)intResolution.x, (float)intResolution.y };

	myCamera->SetPerspectiveProjection(someFOV, resolution, aMinPlaneVal, aFarPlaneVal);

	myRotationSpeed = aRotationSpeedAmm;
	myMovementSpeed = aMovementSpeedAmm;

	mySprintMultipler = 3.0f;

	myTransform = myCamera->GetTransform();


	Tga::Engine::GetInstance()->AddWinProcCallback([](HWND aWinInstance, UINT aMessage, WPARAM anWParam, LPARAM anLParam) -> HRESULT
		{
			using CommonUtilities::Keyboard;
			using CommonUtilities::Mouse;
			aMessage;
			anWParam;
			anLParam;
			Keyboard::Update(aMessage, anWParam, anLParam);
			Mouse::UpdateEvents(aWinInstance, aMessage, anWParam, anLParam);

			return 0;
		});
}


CameraController::~CameraController()
{


	if (myCamera)
	{
		delete myCamera;
		myCamera = nullptr;
	}


}

void CameraController::UpdateController(const float aDt)
{
	using namespace Tga;
	using CommonUtilities::Keyboard;
	using CommonUtilities::Mouse;


	Mouse::EndFrame();

	bool input = Mouse::GetButton(Mouse::Key::RightMouseButton);

	if (input != myInputState)
	{
		Mouse::SetVisibility(!input);
		Mouse::SetCaptureState(input ? Mouse::CaptureState::Confined : Mouse::CaptureState::None);
		myInputState = input;
	}



	myVelocity = Tga::Vector3f(0);
	if (input)
	{
		float speed = Keyboard::GetButton(Keyboard::Key::LeftShift) ? myMovementSpeed * mySprintMultipler : myMovementSpeed;


		if (Keyboard::GetButton(Keyboard::Key::W)) // W
		{
			myVelocity += myTransform.GetMatrix().GetForward() * speed;
		}
		if (Keyboard::GetButton(Keyboard::Key::S)) // S
		{
			myVelocity += myTransform.GetMatrix().GetForward() * -speed;
		}
		if (Keyboard::GetButton(Keyboard::Key::A)) // A
		{
			myVelocity += myTransform.GetMatrix().GetRight() * -speed;
		}
		if (Keyboard::GetButton(Keyboard::Key::D)) // D
		{
			myVelocity += myTransform.GetMatrix().GetRight() * speed;
		}
		if (Keyboard::GetButton(Keyboard::Key::Space))
		{
			myVelocity += myTransform.GetMatrix().GetUp() * speed;
		}
		if (Keyboard::GetButton(Keyboard::Key::Control))
		{
			myVelocity += myTransform.GetMatrix().GetUp() * -speed;
		}

		myPosition = myCamera->GetTransform().GetPosition() + myVelocity * myMovementSpeed * aDt;


		const Vector2f mouseDelta = Mouse::GetMouseDelta();

		myAngularVelocity.X = mouseDelta.Y;
		myAngularVelocity.Y = mouseDelta.X;

		myTransform.AddRotation(myAngularVelocity * myRotationSpeed);

		myRotation = myTransform.GetRotation();
		
	}

	myTransform.SetRotation(myRotation);
	myCamera->SetPosition(myPosition);
	myCamera->SetRotation(myRotation);
}
