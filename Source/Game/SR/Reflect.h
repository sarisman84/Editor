#pragma once
#include <type_traits>
#include <typeinfo>

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
						{
							json& childElement = aJsonFile[member.GetName()];
							Internal::SerializeMembers_Impl<MemberType>(member.GetValue(anInstance), childElement);
						}


						if constexpr (Internal::is_specialization<MemberType, std::vector>::value)
						{

							MemberType* val = member.GetValue(anInstance);

							aJsonFile[member.GetName()]["size"] = val->size();
							json& vector = aJsonFile[member.GetName()];
							for (size_t i = 0; i < val->size(); i++)
							{
								using ListType = MemberType::value_type;

								ListType& element = val->at(i);

								std::string name = "Element ";
								name += std::to_string(i);


								Internal::SerializeMembers_Impl<ListType>(&element, vector[name]);
							}

						}
						else
						{
							member.WriteToJson(anInstance, aJsonFile);
						}
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
						{
							json& childElement = aJsonFile[member.GetName()];
							Internal::DeserializeMembers_Impl<MemberType>(member.GetValue(anInstance), childElement);


						}

						if constexpr (Internal::is_specialization<MemberType, std::vector>::value)
						{
							MemberType* val = member.GetValue(anInstance);
							json vector = aJsonFile[member.GetName()];
							if (vector.contains("size"))
								val->resize(vector["size"]);

							for (size_t i = 0; i < val->size(); i++)
							{
								using ListType = MemberType::value_type;

								ListType& element = val->at(i);

								std::string name = "Element ";
								name += std::to_string(i);

								json childElement = vector[name];
								Internal::DeserializeMembers_Impl<ListType>(&element, childElement);
							}/**/
						}
						else
						{
							member.ReadFromJson(anInstance, aJsonFile);
						}
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

	/*template <typename T> //TODO: Use templates!
	void InspectMembers(std::shared_ptr<T>& aptr, const char* aName) 
	{

	}*/

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
						std::cout << "Element: " << member.GetName() << " is " << typeid(MemberType).name() << std::endl;
						if constexpr (!std::is_same<TupleType, std::tuple<>>::value)
						{
							if constexpr (Internal::is_specialization<MemberType, std::shared_ptr>::value)
								Reflect::InspectMembers<MemberType>(member.GetValue(anInstance)->get(), member.GetName());
							else
								Reflect::InspectMembers<MemberType>(member.GetValue(anInstance), member.GetName());

						}

						//std::cout << member.GetName() << ": " << typeid(MemberType).name() << "->" << Internal::is_specialization<MemberType, std::vector>::value << std::endl;
						if constexpr (Internal::is_specialization<MemberType, std::vector>::value)
						{
							if (ImGui::TreeNode(member.GetName()))
							{
								MemberType* val = member.GetValue(anInstance);

								for (size_t i = 0; i < val->size(); i++)
								{
									using ListType = MemberType::value_type;

									ListType& element = val->at(i);

									std::string name = "Element ";
									name += std::to_string(i);

									Reflect::InspectMembers<ListType>(&element, name.c_str());
								}/**/


								if (ImGui::Button("Add"))
								{
									val->emplace_back();
								}

								ImGui::TreePop();
							}

						}
						else
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
					{
						json& childElement = jsonIns[member.GetName()];
						Internal::SerializeMembers_Impl<MemberType>(member.GetValue(anInstance), childElement);
					}


					if constexpr (Internal::is_specialization<MemberType, std::vector>::value)
					{

						MemberType* val = member.GetValue(anInstance);

						jsonIns[member.GetName()]["size"] = val->size();
						json& vector = jsonIns[member.GetName()];
						for (size_t i = 0; i < val->size(); i++)
						{
							using ListType = MemberType::value_type;

							ListType& element = val->at(i);

							std::string name = "Element ";
							name += std::to_string(i);


							Internal::SerializeMembers_Impl<ListType>(&element, vector[name]);
						}/**/

					}
					else
					{
						member.WriteToJson(anInstance, jsonIns);
					}




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
		if (!ifs || ifs.fail()) return; //Return if it couldnt find a file

		if (ifs.peek() == EOF)
		{
			ifs.close();
			return;
		} //Return if the file is empty

		json jsonIns = nlohmann::json::parse(ifs);

		ifs.close();

		Internal::for_each(GetMembers<TClass>(), [&jsonIns, &anInstance](auto member)
			{
				if constexpr (Internal::is_template<decltype(member)>::value)
				{
					using MemberType = decltype(member)::MType;
					using TupleType = decltype(RegisterElement<MemberType>());


					if constexpr (!std::is_same<TupleType, std::tuple<>>::value)
					{
						json& childElement = jsonIns[member.GetName()];
						Internal::DeserializeMembers_Impl<MemberType>(member.GetValue(anInstance), childElement);
					}

					if constexpr (Internal::is_specialization<MemberType, std::vector>::value)
					{
						MemberType* val = member.GetValue(anInstance);
						json vector = jsonIns[member.GetName()];
						if (vector.contains("size"))
							val->resize(vector["size"]);

						for (size_t i = 0; i < val->size(); i++)
						{
							using ListType = MemberType::value_type;

							ListType& element = val->at(i);

							std::string name = "Element ";
							name += std::to_string(i);

							json childElement = vector[name];
							Internal::DeserializeMembers_Impl<ListType>(&element, childElement);
						}/**/
					}
					else
					{
						member.ReadFromJson(anInstance, jsonIns);
					}



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










