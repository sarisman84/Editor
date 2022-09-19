#pragma once
#include <typeinfo>
#include <vector>
#include <memory>

#define SERIALIZE_IMPL(x,y,z,w) PropertyManager::ApplyProperty(typeid(x), (void*)&x, y ,z, w)

#define SERIALIZE(x,y) SERIALIZE_IMPL(x, y ,false, false)
#define SERIALIZE_PRESET(x,y) SERIALIZE_IMPL(x, y ,true, false)
#define DEEP_SERIALIZE(x,y) SERIALIZE_IMPL(x, y ,false, true)
#define DEEP_SERIALIZE_PRESET(x,y) SERIALIZE_IMPL(x, y ,true, true)


using Type = std::type_info;
using Data = void*;
using TypeName = const char*;

class PropertyInterface
{
public:
	PropertyInterface() = default;
	virtual ~PropertyInterface() = default;
	virtual void OnPropertyInterface(const Type& aType, Data someData, TypeName aName, const bool aSaveAsPreset, const bool aResetOnEdit) = 0;
};



class PropertyManager
{
public:
	static void RegisterInterface(std::shared_ptr<PropertyInterface> anInterface)
	{
		myInterfaces.push_back(anInterface);
	}


	static void ApplyProperty(const Type& aType, Data someData, TypeName aName, bool aSaveAsPresetFlag, bool aResetOnEditFlag)
	{
		for (auto& inter : myInterfaces)
		{
			inter->OnPropertyInterface(aType, someData, aName, aSaveAsPresetFlag, aResetOnEditFlag);
		}
	}



private:
	inline static std::vector<std::shared_ptr<PropertyInterface>> myInterfaces;
};