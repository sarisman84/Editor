#pragma once
#include <imgui/imgui.h>
#include <imgui/misc/cpp/imgui_stdlib.h>
#include <tge/math/Vector.h>
#include <tge/math/color.h>
#include "Reflect.h"



static int DefaultResizeCallback(ImGuiInputTextCallbackData* data)
{
	if (data->EventFlag == ImGuiInputTextFlags_CallbackResize)
	{
		ImVector<char>* my_str = (ImVector<char>*)data->UserData;
		IM_ASSERT(my_str->begin() == data->Buf);
		my_str->resize(data->BufSize); // NB: On resizing calls, generally data->BufSize == data->BufTextLen + 1
		data->Buf = my_str->begin();
	}
	return 0;
}



#pragma region TGA DEFINES
template<>
inline auto Reflect::RegisterElement <Tga::Vector3f>()
{
	return Class("Vector3 Float",
		Member("X", &Tga::Vector3f::x),
		Member("Y", &Tga::Vector3f::y),
		Member("Z", &Tga::Vector3f::z)
	);
}

template<>
inline void Reflect::SerializeElement<Tga::Vector3f>(const std::string& anID, const Tga::Vector3f& aVal, json& aJsonIns)
{
	aJsonIns[anID]["X"] = aVal.x;
	aJsonIns[anID]["Y"] = aVal.y;
	aJsonIns[anID]["Z"] = aVal.z;
}

template<>
inline void Reflect::DeserializeElement<Tga::Vector3f>(const std::string& anID, Tga::Vector3f& aVal, const json& aJsonIns)
{
	aVal.x = aJsonIns[anID]["X"];
	aVal.y = aJsonIns[anID]["Y"];
	aVal.z = aJsonIns[anID]["Z"];
}


template<>
inline auto Reflect::RegisterElement <Tga::Vector2f>()
{
	return Class("Vector2 Float",
		Member("X", &Tga::Vector2f::x),
		Member("Y", &Tga::Vector2f::y)
	);
}

template<>
inline void Reflect::SerializeElement<Tga::Vector2f>(const std::string& anID, const Tga::Vector2f& aVal, json& aJsonIns)
{
	aJsonIns[anID]["X"] = aVal.x;
	aJsonIns[anID]["Y"] = aVal.y;
}

template<>
inline void Reflect::DeserializeElement<Tga::Vector2f>(const std::string& anID, Tga::Vector2f& aVal, const json& aJsonIns)
{
	aVal.x = aJsonIns[anID]["X"];
	aVal.y = aJsonIns[anID]["Y"];
}



template<>
inline auto Reflect::RegisterElement <Tga::Color>()
{
	return Class("Color",
		Member("R", &Tga::Color::myR),
		Member("G", &Tga::Color::myG),
		Member("B", &Tga::Color::myB),
		Member("A", &Tga::Color::myA)
	);
}

template<>
inline void Reflect::SerializeElement<Tga::Color>(const std::string& anID, const Tga::Color& aVal, json& aJsonIns)
{
	aJsonIns[anID]["R"] = aVal.myR;
	aJsonIns[anID]["G"] = aVal.myG;
	aJsonIns[anID]["B"] = aVal.myB;
	aJsonIns[anID]["A"] = aVal.myA;
}


template<>
inline void Reflect::DeserializeElement<Tga::Color>(const std::string& anID, Tga::Color& aVal, const json& aJsonIns)
{
	aVal.myR = aJsonIns[anID]["R"];
	aVal.myG = aJsonIns[anID]["G"];
	aVal.myB = aJsonIns[anID]["B"];
	aVal.myA = aJsonIns[anID]["A"];
}


#pragma endregion


#pragma region PRIMITIVE_SERIALIZE_DEF

template<>
inline void Reflect::SerializeElement<float>(const std::string& anID, const float& aVal, json& aJsonIns)
{
	aJsonIns[anID] = aVal;
}

template<>
inline void Reflect::SerializeElement<int>(const std::string& anID, const int& aVal, json& aJsonIns)
{
	aJsonIns[anID] = aVal;
}

template<>
inline void Reflect::SerializeElement<bool>(const std::string& anID, const bool& aVal, json& aJsonIns)
{
	aJsonIns[anID] = aVal;
}

template<>
inline void Reflect::SerializeElement<std::string>(const std::string& anID, const std::string& aVal, json& aJsonIns)
{
	aJsonIns[anID] = aVal;
}


template<>
inline void Reflect::DeserializeElement<float>(const std::string& anID, float& aVal, const json& aJsonIns)
{
	aVal = aJsonIns[anID];
}

template<>
inline void Reflect::DeserializeElement<int>(const std::string& anID, int& aVal, const json& aJsonIns)
{
	aVal = aJsonIns[anID];
}

template<>
inline void Reflect::DeserializeElement<bool>(const std::string& anID, bool& aVal, const json& aJsonIns)
{
	aVal = aJsonIns[anID];
}

template<>
inline void Reflect::DeserializeElement<std::string>(const std::string& anID, std::string& aVal, const json& aJsonIns)
{
	aVal = aJsonIns[anID];
}



#pragma endregion


#pragma region INSPECT DEFINITIONS

template<>
inline void Reflect::InspectElement<float>(const char* aName, float* aVal)
{
	ImGui::Text(aName);
	ImGui::SameLine();
	std::string name = std::string("##") + aName;

	ImGui::DragFloat(name.c_str(), aVal, 0.01f);
}


template<>
inline void Reflect::InspectElement<int>(const char* aName, int* aVal)
{
	ImGui::Text(aName);
	ImGui::SameLine();
	std::string name = std::string("##") + aName;

	ImGui::DragInt(name.c_str(), aVal, 0.01f);
}


template<>
inline void Reflect::InspectElement<bool>(const char* aName, bool* aVal)
{
	ImGui::Text(aName);
	ImGui::SameLine();
	std::string name = std::string("##") + aName;

	ImGui::Checkbox(name.c_str(), aVal);
}


template<>
inline void Reflect::InspectElement<std::string>(const char* aName, std::string* aVal)
{

	ImGui::Text(aName);
	ImGui::SameLine();
	std::string name = std::string("##") + aName;

	ImGui::InputText(name.c_str(), aVal, 0, DefaultResizeCallback);
}

#pragma endregion