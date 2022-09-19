#pragma once
#include <tge/light/LightManager.h>
#include <tge/primitives/LinePrimitive.h>
#include <tge/drawers/LineDrawer.h>
#include <tge/math/color.h>
#include <tge/math/Vector.h>
#include <tge/math/Quaternion.h>

struct Gizmo
{
	Tga::LineMultiPrimitive myShape;

	Tga::Vector3f* myFromOffset;
	Tga::Vector3f* myToOffset;

	Tga::Vector3f myPosition;
	Tga::Rotator myRotation;
	Tga::Vector3f myScale;
	int myShapeCount;
};



Gizmo CreateCrossGizmo(Tga::Color aColor) {
	Tga::LineMultiPrimitive gizmo;
	Gizmo result;
	const int lineCount = 4;

	result.myShapeCount = lineCount;

	result.myFromOffset = new Tga::Vector3f[lineCount];
	result.myFromOffset[0] = Tga::Vector3f(-1.0f, 0.0f, 0.0f);
	result.myFromOffset[1] = Tga::Vector3f(0.0f, -1.0f, 0.0f);
	result.myFromOffset[2] = Tga::Vector3f(0.0f, 0.0f, -1.0f);
	result.myFromOffset[3] = Tga::Vector3f(0.0f, 0.0f, 0.0f);

	gizmo.fromPositions = new Tga::Vector3f[lineCount];


	result.myToOffset = new Tga::Vector3f[4];
	result.myToOffset[0] = Tga::Vector3f(1.0f, 0.0f, 0.0f);
	result.myToOffset[1] = Tga::Vector3f(0.0f, 1.0f, 0.0f);
	result.myToOffset[2] = Tga::Vector3f(0.0f, 0.0f, 0.0f);
	result.myToOffset[3] = Tga::Vector3f(0.0f, 0.0f, 1.0f);

	gizmo.toPositions = new Tga::Vector3f[lineCount];


	gizmo.colors = new Tga::Color[lineCount];
	aColor.myA = 0.8f;
	gizmo.colors[0] = Tga::Color(1.0f, 0.f, 0.f, 0.8f);
	gizmo.colors[1] = Tga::Color(0.0f, 1.f, 0.f, 0.8f);
	gizmo.colors[2] = Tga::Color(0.0f, 0.f, 1.f, 0.8f);
	gizmo.colors[3] = Tga::Color(0.0f, 1.f, 1.f, 0.8f);




	//memcpy(gizmo.fromPositions, fromPos, sizeof(Tga::Vector3f) * 2);
	//memcpy(gizmo.toPositions, toPos, sizeof(Tga::Vector3f) * 2);



	gizmo.count = lineCount;
	result.myScale = { 1,1,1 };




	result.myShape = gizmo;


	return result;
}


Tga::Vector3f LocalizeToRotation(Tga::Rotator aRotation, Tga::Vector3f aDir)
{
	/*float x = cos(aRotation.x) + aDir.x;
	float y = sin(aRotation.y) + aDir.y;
	float z = tan(aRotation.z) + aDir.z;*/

	float DegToRad = 3.14f / 180.f;

	//auto x = Tga::Matrix3x3f::CreateRotationAroundX(aRotation.x * DegToRad);
	//auto y = Tga::Matrix3x3f::CreateRotationAroundY(aRotation.y * DegToRad);
	//auto z = Tga::Matrix3x3f::CreateRotationAroundZ(aRotation.z * DegToRad);

	Tga::Quaternionf q = Tga::Quaternionf(aRotation * DegToRad);

	return  Tga::Quaternionf::RotateVectorByQuaternion(q, aDir);

	//return Tga::Vector3f(x, y, z).GetNormalized();
}


void DrawGizmo(Gizmo aGizmo)
{
	using namespace Tga;

	auto LD = Engine::GetInstance()->GetGraphicsEngine().GetLineDrawer();



	for (size_t i = 0; i < aGizmo.myShapeCount; i++)
	{
		aGizmo.myShape.fromPositions[i] = (LocalizeToRotation(aGizmo.myRotation, aGizmo.myFromOffset[i]) * aGizmo.myScale) + (aGizmo.myPosition);
		aGizmo.myShape.toPositions[i] = (LocalizeToRotation(aGizmo.myRotation, aGizmo.myToOffset[i]) * aGizmo.myScale) + (aGizmo.myPosition);
	}






	LD.Draw(aGizmo.myShape);


}

