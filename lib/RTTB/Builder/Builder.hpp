#pragma once

#include <vector>
#include <variant>
#include <functional>
#include <absl/container/flat_hash_set.h>
#include <absl/container/flat_hash_map.h>

#include <RTTB/Cast/Cast.hpp>
#include <RTTB/Dyn/Dyn.hpp>

namespace rttb {
	template<typename Resource_, typename Type_>
	struct TypeData {
		using CastFn = orl::Option<Type_*> (*)(Dyn& ptr);
		
		CastFn cast;
		
		TypeData(CastFn cast);
	};
	
	/// @brief Class storing information about one derived type.
	///
	/// @tparam Resource_ The resource on the basis of which an instance of the type is created.
	/// @tparam Type_ Base type for derived type.
	template<typename Resource_, typename Type_>
	class DerivedData {
	public:
		using TypeData = TypeData<Resource_, Type_>;
		
		DerivedData(TypeData type_data);
		
		orl::Option<Type_*> cast(Dyn& ptr) const;
		
	protected:
		TypeData data_;
	};
	
	/// @brief A class that stores information about a single type.
	///
	/// @tparam Resource_ The resource on the basis of which an instance of the type is constructed.
	/// @tparam Type_ The type whose information the class stores.
	template<typename Resource_, typename Type_>
	class Builder {
	public:
		using DerivedData = DerivedData<Resource_, Type_>;
		using DerivedContainer = std::vector<DerivedData>;
		template<typename Base, typename Return>
		using if_base_t = std::enable_if_t<std::is_base_of_v<Base, Type_>, Return>;
		template<typename Derived, typename Return>
		using if_derived_t = std::enable_if_t<std::is_base_of_v<Type_, Derived>, Return>;
		
		template<typename Base>
		if_base_t<Base, TypeData<Resource_, Base> > get_type_data();
		
		/// @brief Adds information about the existence of the derived type.
		///
		/// @tparam Derived Derived type.
		template<typename Derived>
		if_derived_t<Derived, void> add_type();
		
		/// @brief Performs a type check and returns a pointer if the check succeeds.
		///
		/// @param ptr Dyn pointer to an object.
		///
		/// @return Pointer to an object or nothing.
		orl::Option<Type_*> cast(Dyn& ptr) const;
		
		/// @brief Returns a reference to the only existing instance of the class
		static Builder<Resource_, Type_>& builder();
		
	private:
		template<typename Base>
		static orl::Option<Base*> cast_fn(Dyn& ptr);
		
		Builder() = default;
		
		Builder(const Builder&) = delete;
		
		Builder(Builder&&) = delete;
		
		Builder& operator=(const Builder&) = delete;
		
		const Builder& operator=(const Builder&) const = delete;
		
		DerivedContainer derived_;
	};
}

#include "Builder.inl"