#pragma once
#include <type_traits>
#include <typeinfo>

#include <vector>
#include <iostream>
#include <tuple>

#include "nlohmann/json.hpp"

#include <sstream>
#include <string> 


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
	inline void SerializeElement(const std::string& anID, const T& aVal, json& aJsonIns)
	{
		aJsonIns[anID] = aVal;
	}

	template<typename T>
	inline void DeserializeElement(const std::string& anID, T& aVal, const json& aJsonIns)
	{
		aVal = aJsonIns[anID];
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

			T* GetValue(TClass* anInstance) noexcept {
				return &(anInstance->*myMemberPtr);
			}

			void SetValue(TClass* anInstance, const T& aValue) noexcept {
				anInstance->*myMemberPtr = aValue;
			}


			void WriteToJson(TClass* anInstance, json& aJsonFile)
			{
				std::string name = myName;

				const void* address = static_cast<const void*>(anInstance);
				std::stringstream ss;
				ss << address;
				name += ss.str();

				SerializeElement<T>(name, *GetValue(anInstance), aJsonFile);
			}

			void ReadFromJson(TClass* anInstance, json& aJsonFile)
			{
				std::string name = myName;

				const void* address = static_cast<const void*>(anInstance);
				std::stringstream ss;
				ss << address;
				name += ss.str();


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
		struct is_template<Tmpl<Args...>> : std::true_type {

		};


		template<typename, typename>
		struct tuple_holds;

		template<typename ...Ts, typename T>
		struct tuple_holds<std::tuple<Ts...>, T>
			: std::disjunction<std::is_same<Ts, T>...> {};




		template<typename TClass>
		void SerializeMembers_Impl(TClass* anInstance, json& aJsonFile)
		{
			Internal::for_each(GetMembers<TClass>(), [&aJsonFile, &anInstance](auto member)
				{
					if constexpr (Internal::is_template<decltype(member)>::value)
					{
						using MemberType = decltype(member)::MType;
						using TupleType = decltype(RegisterElement<MemberType>());

						if constexpr (!std::is_same<TupleType, std::tuple<>>::value)
							SerializeMembers_Impl<MemberType>(member.GetValue(anInstance), aJsonFile);

						member.WriteToJson(anInstance, aJsonFile);

					}
				});

		}



		template<typename TClass>
		void DeserializeMembers_Impl(TClass* anInstance, json& aJsonFile)
		{
			Internal::for_each(GetMembers<TClass>(), [&aJsonFile, &anInstance](auto member)
				{
					if constexpr (Internal::is_template<decltype(member)>::value)
					{
						using MemberType = decltype(member)::MType;
						using TupleType = decltype(RegisterElement<MemberType>());

						if constexpr (!std::is_same<TupleType, std::tuple<>>::value)
							DeserializeMembers_Impl<MemberType>(member.GetValue(anInstance), aJsonFile);

						member.ReadFromJson(anInstance, aJsonFile);

					}
				});

		}

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



	template<typename TClass, typename Func>
	void IterateMembers(TClass* anInstance, Func&& aCallback)
	{
		Internal::for_each(GetMembers<TClass>(), [&anInstance, &aCallback](auto& member)
			{

				if constexpr (Internal::is_template<decltype(member)>::value)
				{
					using MemberType = decltype(member)::MType;
					using TupleType = decltype(RegisterElement<MemberType>());
					if constexpr (!std::is_same<TupleType, std::tuple<>>::value)
						IterateMembers<MemberType>(member.GetValue(anInstance), aCallback);

					aCallback(anInstance, member);
				}

			});
	}


	template<typename TClass>
	void InspectMembers(TClass* anInstance, const char* aName)
	{
		if (ImGui::TreeNode(aName))
		{
			Internal::for_each(GetMembers<TClass>(), [&anInstance](auto member)
				{
					if constexpr (Internal::is_template<decltype(member)>::value)
					{
						using MemberType = decltype(member)::MType;
						using TupleType = decltype(RegisterElement<MemberType>());

						if constexpr (!std::is_same<TupleType, std::tuple<>>::value)
							InspectMembers<MemberType>(member.GetValue(anInstance), member.GetName());
						InspectElement<MemberType>(member.GetName(), member.GetValue(anInstance));

					}
				});
			ImGui::TreePop();
		}
	}


	template<typename TClass>
	void SerializeMembers(TClass* anInstance, const char* aPath)
	{
		std::ofstream ofs(aPath);
		if (!ofs || ofs.fail()) return;


		json jsonIns = json();

		Internal::for_each(GetMembers<TClass>(), [&jsonIns, &anInstance](auto member)
			{
				if constexpr (Internal::is_template<decltype(member)>::value)
				{
					using MemberType = decltype(member)::MType;
					using TupleType = decltype(RegisterElement<MemberType>());

					if constexpr (!std::is_same<TupleType, std::tuple<>>::value)
						Internal::SerializeMembers_Impl<MemberType>(member.GetValue(anInstance), jsonIns);

					member.WriteToJson(anInstance, jsonIns);

				}
			});

		if (!jsonIns.is_null())
			ofs << jsonIns;
		ofs.close();

	}



	template<typename TClass>
	void DeserializeMembers(TClass* anInstance, const char* aPath)
	{
		std::ifstream ifs(aPath);
		if (!ifs || ifs.fail()) return;


		json jsonIns = nlohmann::json::parse(ifs);

		ifs.close();

		Internal::for_each(GetMembers<TClass>(), [&jsonIns, &anInstance](auto member)
			{
				if constexpr (Internal::is_template<decltype(member)>::value)
				{
					using MemberType = decltype(member)::MType;
					using TupleType = decltype(RegisterElement<MemberType>());

					if constexpr (!std::is_same<TupleType, std::tuple<>>::value)
						Internal::DeserializeMembers_Impl<MemberType>(member.GetValue(anInstance), jsonIns);

					member.ReadFromJson(anInstance, jsonIns);

				}
			});

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









