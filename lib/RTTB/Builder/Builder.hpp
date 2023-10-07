#pragma once

#include <vector>
#include <functional>
#include <absl/container/flat_hash_set.h>
#include <absl/container/flat_hash_map.h>

#include <RTTB/Decode/Decode.hpp>

namespace rttb {
	template<typename Resource_, typename Type_>
	using BuildFn = std::function<orl::Option<Type_*>(String const& name, Resource_ resource)>;
	
	template<typename Resource_>
	using DetermineFn = std::function<orl::Option<String>(Resource_)>;
	
	template<typename Resource_, typename Type_>
	struct DerivedData {
		BuildFn<Resource_, Type_> build;
		orl::Option<Type_*>(* implicit_build)(Resource_ resource);
	};
	
	template<typename Resource_, typename Type_>
	class Builder {
	public:
		using BuildFn = BuildFn<Resource_, Type_>;
		using DerivedData = DerivedData<Resource_, Type_>;
		using DerivedContainer = std::vector<DerivedData>;
		using NamesContainer = absl::flat_hash_set<String>;
		using DetermineFn = DetermineFn<Resource_>;
		using DetermineFnContainer = std::vector<DetermineFn>;
		
		template<typename Derived>
		std::enable_if_t<std::is_base_of_v<Type_, Derived>, void> add_type();
		
		void add_fn(BuildFn build_fn);
		
		void add_name(String&& name);
		
		void add_determine(DetermineFn determine_fn);
		
		orl::Option<Type_*> build(String const& name, Resource_ resource);
		
		orl::Option<Type_*> implicit_build(Resource_ resource);
		
		static Builder<Resource_, Type_>& builder();
		
	private:
		Builder() = default;
		
		DerivedContainer derived_;
		NamesContainer names_;
		DetermineFnContainer determine_fn_;
	};
}

#include "Builder.inl"