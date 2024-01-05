#pragma once

#include <vector>
#include <variant>
#include <functional>
#include <absl/container/flat_hash_set.h>
#include <absl/container/flat_hash_map.h>

#include <RTTB/Cast/Cast.hpp>
#include <RTTB/Decode/Decode.hpp>
#include <RTTB/Dyn/Dyn.hpp>

namespace rttb {
	template<typename Resource_, typename Type_>
	using BuildFn = std::function<orl::Option<Type_*>(String const& name, Resource_ resource)>;
	
	template<typename Resource_>
	using DetermineFn = std::function<orl::Option<String>(Resource_)>;
	
	template<typename Resource_, typename Type_>
	struct TypeData {
		using CastFn = orl::Option<Type_*> (*)(Dyn& ptr);
		using BuildFn = orl::Option<Type_*> (*)(String const& name, Resource_ resource);
		using BuildDynFn = orl::Option<Dyn> (*)(String const& name, Resource_ resource);
		using ImplicitBuildFn = orl::Option<Type_*> (*)(Resource_ resource);
		using ImplicitBuildDynFn = orl::Option<Dyn> (*)(Resource_ resource);
		
		CastFn cast;
		BuildFn build;
		BuildDynFn build_dyn;
		ImplicitBuildFn implicit_build;
		ImplicitBuildDynFn implicit_build_dyn;
		
		TypeData(
			CastFn cast,
			BuildFn build,
			BuildDynFn build_dyn,
			ImplicitBuildFn implicit_build,
			ImplicitBuildDynFn implicit_build_dyn
		);
	};
	
	/// @brief Class storing information about one derived type.
	///
	/// @tparam Resource_ The resource on the basis of which an instance of the type is created.
	/// @tparam Type_ Base type for derived type.
	template<typename Resource_, typename Type_>
	class DerivedData {
	public:
		using TypeData = TypeData<Resource_, Type_>;
		using BuildFn = BuildFn<Resource_, Type_>;
		
		DerivedData(TypeData type_data);
		
		DerivedData(BuildFn build_fn);
		
		orl::Option<Type_*> cast(Dyn& ptr) const;
		
		orl::Option<Type_*> build(String const& name, Resource_ resource) const;
		
		orl::Option<Dyn> build_dyn(String const& name, Resource_ resource) const;
		
		orl::Option<Type_*> implicit_build(Resource_ resource) const;
		
		orl::Option<Dyn> implicit_build_dyn(Resource_ resource) const;
	
	protected:
		std::variant<TypeData, BuildFn> data_;
	};
	
	/// @brief A class that stores information about a single type.
	///
	/// @tparam Resource_ The resource on the basis of which an instance of the type is constructed.
	/// @tparam Type_ The type whose information the class stores.
	template<typename Resource_, typename Type_>
	class Builder {
	public:
		using BuildFn = BuildFn<Resource_, Type_>;
		using DerivedData = DerivedData<Resource_, Type_>;
		using DerivedContainer = std::vector<DerivedData>;
		using NamesContainer = absl::flat_hash_set<String>;
		using DetermineFn = DetermineFn<Resource_>;
		using DetermineFnContainer = std::vector<DetermineFn>;
		template<typename Base, typename Return>
		using if_base_t = std::enable_if_t<std::is_base_of_v<Base, Type_>, Return>;
		template<typename Derived, typename Return>
		using if_derived_t = std::enable_if_t<std::is_base_of_v<Type_, Derived>, Return>;
		
		template<typename Base>
		if_base_t<Base, TypeData<Resource_, Base> > get_type_data();
		
		/// @brief Gets the set of names associated with the type
		///
		/// @return Set of names.
		NamesContainer const& get_names();
		
		/// @brief Adds information about the existence of the derived type.
		///
		/// @tparam Derived Derived type.
		template<typename Derived>
		if_derived_t<Derived, void> add_type();
		
		/// @brief Adds a function that can construct a type.
		///
		///	Accepts a function or lambda with the signature `orl::Option<Type_*>(rttb::String const&, {T})`.
		/// This function will be called when `rttb::Builder<{R}, {T}>::build()` is called, if it returns something, it will be returned as an object.
		/// It should return nothing when the first argument is the name of the added type.
		///
		/// @param build_fn Construction function.
		void add_fn(BuildFn build_fn);
		
		/// @brief Adds an additional name to the type.
		///
		/// @param name Additional name.
		void add_name(String&& name);
		
		/// @brief Adds a function that attempts to determine the type name from the resource.
		///
		/// @param determine_fn Determine function.
		void add_determine(DetermineFn determine_fn);
		
		/// @brief Performs a type check and returns a pointer if the check succeeds.
		///
		/// @param ptr Dyn pointer to an object.
		///
		/// @return Pointer to an object or nothing.
		orl::Option<Type_*> cast(Dyn& ptr) const;
		
		/// @brief Constructs an instance of the type by type name from the resource.
		///
		/// @param name Type name.
		/// @param resource Resource from which an instance of the type is constructed.
		///
		/// @return An instance of the class or nothing in case of failure.
		orl::Option<Type_*> build(String const& name, Resource_ resource) const;
		
		/// @brief Constructs an instance of the type by type name from the resource.
		///
		/// @param name Type name.
		/// @param resource Resource from which an instance of the type is constructed.
		///
		/// @return An instance of the class or nothing in case of failure.
		orl::Option<Dyn> build_dyn(String const& name, Resource_ resource) const;
		
		/// @brief Constructs an instance of a type from a resource, attempting to determine a type based on the resource.
		///
		/// @param resource Resource from which an instance of the type is constructed.
		///
		/// @return An instance of the class or nothing in case of failure.
		orl::Option<Type_*> implicit_build(Resource_ resource) const;
		
		/// @brief Constructs an instance of a type from a resource, attempting to determine a type based on the resource.
		///
		/// @param resource Resource from which an instance of the type is constructed.
		///
		/// @return An instance of the class or nothing in case of failure.
		orl::Option<Dyn> implicit_build_dyn(Resource_ resource) const;
		
		/// @brief Returns a reference to the only existing instance of the class
		static Builder<Resource_, Type_>& builder();
	
	private:
		template<typename Base>
		static orl::Option<Base*> cast_fn(Dyn& ptr);
		
		template<typename Base>
		static orl::Option<Base*> build_fn(String const& name, Resource_ resource);
		
		template<typename Base>
		static orl::Option<Dyn> build_dyn_fn(String const& name, Resource_ resource);
		
		template<typename Base>
		static orl::Option<Base*> implicit_build_fn(Resource_ resource);
		
		template<typename Base>
		static orl::Option<Dyn> implicit_build_dyn_fn(Resource_ resource);
		
		Builder() = default;
		
		Builder(const Builder&) = delete;
		
		Builder(Builder&&) = delete;
		
		Builder& operator=(const Builder&) = delete;
		
		const Builder& operator=(const Builder&) const = delete;
		
		DerivedContainer derived_;
		NamesContainer names_;
		DetermineFnContainer determine_fn_;
	};
}

#include "Builder.inl"