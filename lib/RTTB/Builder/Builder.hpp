#pragma once

#include <vector>
#include <variant>
#include <functional>
#include <absl/container/flat_hash_set.h>
#include <absl/container/flat_hash_map.h>

#include <RTTB/Cast/Cast.hpp>
#include <RTTB/Decode/Decode.hpp>

namespace rttb {
	template<typename Resource_, typename Type_>
	using BuildFn = std::function<orl::Option<Type_*>(String const& name, Resource_ resource)>;
	
	template<typename Resource_>
	using DetermineFn = std::function<orl::Option<String>(Resource_)>;
	
	template<typename Resource_>
	struct TypeData {
		using CastFn = orl::Option<void*> (*)(void* object, size_t type_id);
		using BuildFn = orl::Option<void*> (*)(String const& name, Resource_ resource);
		using ImplicitBuildFn = orl::Option<void*> (*)(Resource_ resource);
		
		CastFn cast;
		BuildFn build;
		ImplicitBuildFn implicit_build;
		
		TypeData(CastFn cast, BuildFn build, ImplicitBuildFn implicit_build);
	};
	
	/// @brief Class storing information about one derived type.
	///
	/// @tparam Resource_ The resource on the basis of which an instance of the type is created.
	/// @tparam Type_ Base type for derived type.
	template<typename Resource_, typename Type_>
	class DerivedData {
	public:
		using CastFn = Type_* (*)(void* derived);
		using TypeData = TypeData<Resource_>;
		
		DerivedData(TypeData& type_data, CastFn cast_fn);
		
		DerivedData(BuildFn<Resource_, Type_> build_fn);
		
		orl::Option<Type_*> cast(void* object, size_t type_id) const;
		
		orl::Option<Type_*> build(String const& name, Resource_ resource) const;
		
		orl::Option<Type_*> implicit_build(Resource_ resource) const;
	
	protected:
		std::variant<std::pair<TypeData*, CastFn>, BuildFn<Resource_, Type_> > data_;
	};
	
	/// @brief A class that stores information about a single type.
	///
	/// @tparam Resource_ The resource on the basis of which an instance of the type is constructed.
	/// @tparam Type_ The type whose information the class stores.
	template<typename Resource_, typename Type_>
	class Builder {
	public:
		using BuildFn = BuildFn<Resource_, Type_>;
		using TypeData = TypeData<Resource_>;
		using DerivedData = DerivedData<Resource_, Type_>;
		using DerivedContainer = std::vector<DerivedData>;
		using NamesContainer = absl::flat_hash_set<String>;
		using DetermineFn = DetermineFn<Resource_>;
		using DetermineFnContainer = std::vector<DetermineFn>;
		
		size_t get_type_id();
		
		TypeData& get_type_data();
		
		/// @brief Adds information about the existence of the derived type.
		///
		/// @tparam Derived Derived type.
		template<typename Derived>
		std::enable_if_t<std::is_base_of_v<Type_, Derived>, void> add_type();
		
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
		///	@note If you store an object of unknown type, don't forget to take care of its correct deletion.
		///
		/// @param object Void pointer to an object of unknown type.
		/// @param type_id Type id.
		///
		/// @return Pointer to an object or nothing.
		orl::Option<Type_*> cast(void* object, size_t type_id);
		
		/// @brief Constructs an instance of the type by type name from the resource.
		///
		/// @param name Type name.
		/// @param resource Resource from which an instance of the type is constructed.
		///
		/// @return An instance of the class or nothing in case of failure.
		orl::Option<Type_*> build(String const& name, Resource_ resource);
		
		/// @brief Constructs an instance of a type from a resource, attempting to determine a type based on the resource.
		///
		/// @param resource Resource from which an instance of the type is constructed.
		///
		/// @return An instance of the class or nothing in case of failure.
		orl::Option<Type_*> implicit_build(Resource_ resource);
		
		/// @brief Returns a reference to the only existing instance of the class
		static Builder<Resource_, Type_>& builder();
		
	private:
		static orl::Option<void*> cast_fn(void* object, size_t type_id);
		
		static orl::Option<void*> build_fn(String const& name, Resource_ resource);
		
		static orl::Option<void*> implicit_build_fn(Resource_ resource);
		
		Builder();
		
		Builder(const Builder&) = delete;
		
		Builder(Builder&&) = delete;
		
		Builder& operator=(const Builder&) = delete;
		
		const Builder& operator=(const Builder&) const = delete;
		
		TypeData type_data_;
		DerivedContainer derived_;
		NamesContainer names_;
		DetermineFnContainer determine_fn_;
	};
}

#include "Builder.inl"