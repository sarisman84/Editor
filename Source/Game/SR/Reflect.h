#pragma once
#include <type_traits>
#include <typeinfo>

#include <memory>
#include <vector>
#include <iostream>
#include <tuple>

#include <nlohmann/json.hpp>

#include <sstream>
#include <string> 
#include <fstream>


#pragma warning (disable: 4180)

namespace Reflect
{
	using namespace nlohmann;

	template<typename TClass>
	inline auto RegisterElement() {
		return std::make_tuple();
	}

	template<typename T>
	inline void InspectElement(const char* /*aName*/, T* /*aValue*/) {
	}

	template<typename T>
	inline void SerializeElement(const std::string& /*anID*/, const T& /*aVal*/, json& /*aJsonIns*/)
	{
	}

	template<typename T>
	inline void DeserializeElement(const std::string& /*anID*/, T& /*aVal*/, const json& /*aJsonIns*/)
	{

	}


	namespace Internal
	{


		template<typename TClass, typename T>
		class Member
		{

		public:
			using OwnerType = TClass;
			using MType = T;
			using MemberPtr = T TClass::*;
			Member(const char* aName, MemberPtr aMemberPtr)
			{
				myMemberPtr = aMemberPtr;
				myName = aName;
			}



			const char* GetName() noexcept {
				return myName;
			}

			//std::shared_ptr<float>*
			T* GetValue(TClass* anInstance) noexcept {
				return &(anInstance->*myMemberPtr);
			}

			void SetValue(TClass* anInstance, const T& aValue) noexcept {
				anInstance->*myMemberPtr = aValue;
			}


			void WriteToJson(TClass* anInstance, json& aJsonFile)
			{
				std::string name = myName;

				/*const void* address = static_cast<const void*>(anInstance);
				std::stringstream ss;
				ss << address;
				name += ss.str();*/

				SerializeElement<T>(name, *GetValue(anInstance), aJsonFile);
			}

			void ReadFromJson(TClass* anInstance, json& aJsonFile)
			{
				std::string name = myName;

				/*	const void* address = static_cast<const void*>(anInstance);
					std::stringstream ss;
					ss << address;
					name += ss.str();*/

				if (aJsonFile.contains(name))
					DeserializeElement<T>(name, *GetValue(anInstance), aJsonFile);
			}






		private:
			const char* myName;
			MemberPtr myMemberPtr;

		};




		template<typename TClass, typename TTuple>
		struct MetaData
		{
			using Type = TClass;
			static TTuple myMembers;
		};


		template<typename TClass, typename TTuple>
		TTuple MetaData<TClass, TTuple>::myMembers = RegisterElement<TClass>();



		template<size_t I = 0, typename Func, typename... Tp>
		inline typename std::enable_if<I == sizeof...(Tp), void>::type
			for_each(std::tuple<Tp...>, Func) {

		}


		template<size_t I = 0, typename Func, typename... Tp>
		inline typename std::enable_if < I < sizeof...(Tp), void>::type
			for_each(std::tuple<Tp...> aTuple, Func aCallback) {


			aCallback(std::get<I>(aTuple));
			for_each<I + 1, Func, Tp...>(aTuple, aCallback);
		}



		template<typename>
		struct is_template : std::false_type
		{
		};

		template<template<typename...> class Tmpl, typename ...Args>
		struct is_template<Tmpl<Args...>> : std::true_type
		{
		};


		template<typename Test, template<typename...> class Ref>
		struct is_specialization : std::false_type
		{
		};

		template<template<typename...> class Ref, typename... Args>
		struct is_specialization<Ref<Args...>, Ref> : std::true_type
		{
		};


		template<typename, typename>
		struct tuple_holds;

		template<typename ...Ts, typename T>
		struct tuple_holds<std::tuple<Ts...>, T>
			: std::disjunction<std::is_same<Ts, T>...>
		{
		};



	}


	template<typename TClass>
	const auto& GetMembers() {
		return Internal::MetaData<TClass, decltype(RegisterElement<TClass>())>::myMembers;
	}

	template<typename TClass, size_t I>
	const auto& GetMember() {
		return std::get<I>(GetMembers<TClass>());
	}


	template<typename... Members>
	constexpr auto Class(Members&&... someMembers) {
		return std::make_tuple(std::forward<Members>(someMembers)...);
	}


	template<typename TClass, typename T>
	constexpr auto Member(const char* aName, T TClass::* aPtr) {
		return Internal::Member<TClass, T>(aName, aPtr);
	}


	template<typename C>
	void SerializeMembers(C* anInstance, const char* aPath)
	{
		using nlohmann::json;

		std::ofstream ofs(aPath);

		if (!ofs || ofs.fail()) return;

		json obj;
		Reflect::SerializeMembers(anInstance, obj);
		ofs << obj;
		ofs.close();
	}

	template<typename C>
	void SerializeMembers(C* anInstance, nlohmann::json& aJsonIns)
	{
		Internal::for_each(Reflect::GetMembers<C>(), [&anInstance, &aJsonIns](auto member)
			{
				if constexpr (Internal::is_template<decltype(member)>::value)
				{
					using MemberType = decltype(member)::MType;
					using TupleType = decltype(RegisterElement<MemberType>());

					Reflect::SerializeElement(member.GetName(), *member.GetValue(anInstance), aJsonIns);

					if constexpr (
						!std::is_same<TupleType, std::tuple<>>::value ||
						Internal::is_specialization<MemberType, std::vector>::value ||
						Internal::is_specialization<MemberType, std::shared_ptr>::value)
					{
						json& childMember = aJsonIns[member.GetName()];
						Reflect::SerializeMembers(member.GetValue(anInstance), childMember);
					}

				}
			});
	}


	template<typename C>
	void SerializeMembers(std::vector<C>* anInstance, nlohmann::json& aJsonIns)
	{
		aJsonIns["Size"] = anInstance->size();
		for (size_t i = 0; i < anInstance->size(); i++)
		{
			auto elementIns = &anInstance->at(i);
			Reflect::SerializeMembers(elementIns, aJsonIns);
		}


	}


	template<typename C>
	void SerializeMembers(std::vector<std::shared_ptr<C>>* anInstance, nlohmann::json& aJsonIns)
	{
		aJsonIns["Size"] = anInstance->size();
		for (size_t i = 0; i < anInstance->size(); i++)
		{
			auto elementIns = &anInstance->at(i);
			if (anInstance->at(i))
				Reflect::SerializeMembers(elementIns, aJsonIns);
		}


	}


	template<typename C>
	void SerializeMembers(std::shared_ptr<C>* anInstance, nlohmann::json& aJsonIns)
	{
		if (anInstance)
			Reflect::SerializeMembers(anInstance->get(), aJsonIns);
	}



	template<typename C>
	void DeserializeMembers(C* anInstance, const char* aPath)
	{
		using nlohmann::json;
		std::ifstream ifs(aPath);

		if (!ifs || ifs.fail() || ifs.peek() == EOF)
		{
			ifs.close();
			std::ofstream ofs(aPath);
			ofs << "{}";
			ofs.close();
			Reflect::DeserializeMembers(anInstance, aPath);
			return;
		}

		json ins = json::parse(ifs);
		ifs.close();


		Reflect::DeserializeMembers(anInstance, ins);
	}


	template<typename C>
	void DeserializeMembers(C* anInstance, nlohmann::json& aJsonIns)
	{
		Internal::for_each(Reflect::GetMembers<C>(), [&anInstance, &aJsonIns](auto member)
			{
				if constexpr (Internal::is_template<decltype(member)>::value)
				{
					using MemberType = decltype(member)::MType;
					using TupleType = decltype(RegisterElement<MemberType>());

					if (aJsonIns.contains(member.GetName()))
					{
						Reflect::DeserializeElement(member.GetName(), *member.GetValue(anInstance), aJsonIns);
					}

					if constexpr (
						!std::is_same<TupleType, std::tuple<>>::value ||
						Internal::is_specialization<MemberType, std::vector>::value ||
						Internal::is_specialization<MemberType, std::shared_ptr>::value)
					{
						json& childMember = aJsonIns[member.GetName()];
						Reflect::DeserializeMembers(member.GetValue(anInstance), childMember);
					}

				}
			});

	}

	template<typename C>
	void DeserializeMembers(std::vector<C>* anInstance, nlohmann::json& aJsonIns)
	{
		anInstance->resize(aJsonIns.contains("Size") ? aJsonIns["Size"] : 0);

		for (size_t i = 0; i < anInstance->size(); i++)
		{
			auto elementIns = &anInstance->at(i);
			Reflect::DeserializeMembers(elementIns, aJsonIns);
		}

	}


	template<typename C>
	void DeserializeMembers(std::vector<std::shared_ptr<C>>* anInstance, nlohmann::json& aJsonIns)
	{
		anInstance->resize(aJsonIns.contains("Size") ? aJsonIns["Size"] : 0);

		for (size_t i = 0; i < anInstance->size(); i++)
		{
			auto elementIns = &anInstance->at(i);
			if (anInstance->at(i))
				Reflect::DeserializeMembers(elementIns, aJsonIns);
		}

	}

	template<typename C>
	void DeserializeMembers(std::shared_ptr<C>* anInstance, nlohmann::json& aJsonIns)
	{
		if (*anInstance)
			Reflect::DeserializeMembers(anInstance->get(), aJsonIns);
	}



	template<typename C>
	inline void InspectMembers(std::shared_ptr<C>* anInstance, const char* aName) {
		Reflect::InspectMembers(anInstance->get(), aName);
	}


	template<typename C>
	inline void InspectMembers(std::vector<std::shared_ptr<C>>* anInstance, const char* aName) {
		std::string name = "Vector (";
		name += aName;
		name += ")";
		if (ImGui::TreeNode(name.c_str()))
		{
			for (size_t i = 0; i < anInstance->size(); i++)
			{
				name = "<Ptr> Element ";
				name += std::to_string(i);
				auto elementIns = &anInstance->at(i);
				Reflect::InspectMembers(elementIns, name.c_str());
			}

			if (ImGui::Button("Add"))
			{
				anInstance->emplace_back();
			}

			ImGui::TreePop();
		}

	}


	template<typename C>
	inline void InspectMembers(std::vector<C>* anInstance, const char* aName) {
		std::string name = "Vector (";
		name += aName;
		name += ")";
		if (ImGui::TreeNode(name.c_str()))
		{
			for (size_t i = 0; i < anInstance->size(); i++)
			{
				std::string name = "Element ";
				name += std::to_string(i);
				auto elementIns = &anInstance->at(i);
				Reflect::InspectMembers(elementIns, name.c_str());
			}

			if (ImGui::Button("Add"))
			{
				anInstance->emplace_back();
			}

			ImGui::TreePop();
		}

	}


	template<typename C>
	void InspectMembers(C* anInstance, const char* aName)
	{

		if (ImGui::TreeNode(aName))
		{
			Internal::for_each(Reflect::GetMembers<C>(), [&anInstance](auto member)
				{


					if constexpr (Internal::is_template<decltype(member)>::value)
					{
						using MemberType = decltype(member)::MType;
						using TupleType = decltype(RegisterElement<MemberType>());



						if constexpr (
							!std::is_same<TupleType, std::tuple<>>::value ||
							Internal::is_specialization<MemberType, std::vector>::value ||
							Internal::is_specialization<MemberType, std::shared_ptr>::value)
						{
							Reflect::InspectMembers(member.GetValue(anInstance), member.GetName());
						}

						Reflect::InspectElement(member.GetName(), member.GetValue(anInstance));

					}

				});

			ImGui::TreePop();
		}

	}













}


template<typename TClass, typename Func>
void ForEachMember(TClass& anInstance, Func&& aCallback)
{

	Internal::for_each(GetMembers<TClass>(), [&aCallback, &anInstance](auto member)
		{
			aCallback(member.GetValue(anInstance));
			IterateMembers<decltype(member)::MType>(member.GetValue(anInstance), aCallback);
		});

}










