//included into Builder.hpp

#include <RTTB/type_id/type_id.hpp>

namespace rttb {
	template<typename Resource_, typename Type_>
	TypeData<Resource_, Type_>::TypeData(CastFn cast, BuildFn build, ImplicitBuildFn implicit_build) :
		cast(cast), build(build), implicit_build(implicit_build) {
	}
	
	template<typename Resource_, typename Type_>
	DerivedData<Resource_, Type_>::DerivedData(TypeData type_data) :
		data_(std::move(type_data)) {
	}
	
	template<typename Resource_, typename Type_>
	DerivedData<Resource_, Type_>::DerivedData(BuildFn build_fn) :
		data_(std::move(build_fn)) {
	}
	
	template<typename Resource_, typename Type_>
	template<typename Base>
	typename Builder<Resource_, Type_>::template if_base_t<Base, TypeData<Resource_, Base> >
	Builder<Resource_, Type_>::get_type_data() {
		return {cast_fn<Base>, build_fn<Base>, implicit_build_fn<Base>};
	}
	
	template<typename Resource_, typename Type_>
	template<typename Derived>
	typename Builder<Resource_, Type_>::template if_derived_t<Derived, void>
	Builder<Resource_, Type_>::add_type() {
		derived_.push_back(DerivedData{Builder<Resource_, Derived>::builder().template get_type_data<Type_>()});
	}
	
	template<typename Resource_, typename Type_>
	void Builder<Resource_, Type_>::add_fn(BuildFn build_fn) {
		derived_.push_back(DerivedData{std::move(build_fn)});
	}
	
	template<typename Resource_, typename Type_>
	void Builder<Resource_, Type_>::add_name(String&& name) {
		names_.insert(name);
	}
	
	template<typename Resource_, typename Type_>
	void Builder<Resource_, Type_>::add_determine(DetermineFn determine_fn) {
		determine_fn_.push_back(determine_fn);
	}
	
	template<typename Resource_, typename Type_>
	orl::Option<Type_*> DerivedData<Resource_, Type_>::cast(Dyn& ptr) const {
		if(auto type_data{std::get_if<0>(&data_)}) {
			return type_data->cast(ptr);
		}
		return {};
	}
	
	template<typename Resource_, typename Type_>
	orl::Option<Type_*> DerivedData<Resource_, Type_>::build(const String& name, Resource_ resource) const {
		if(auto type_data{std::get_if<0>(&data_)}) {
			return type_data->build(name, std::forward<Resource_>(resource));
		}
		auto& build_fn{std::get<1>(data_)};
		return build_fn(name, std::forward<Resource_>(resource));
	}
	
	template<typename Resource_, typename Type_>
	Builder<Resource_, Type_>& Builder<Resource_, Type_>::builder() {
		static Builder<Resource_, Type_> result{};
		return result;
	}
	
	template<typename Resource_, typename Type_>
	orl::Option<Type_*> DerivedData<Resource_, Type_>::implicit_build(Resource_ resource) const {
		if(auto type_data{std::get_if<0>(&data_)}) {
			return type_data->implicit_build(std::forward<Resource_>(resource));
		}
		return {};
	}
	
	template<typename Resource_, typename Type_>
	orl::Option<Type_*> Builder<Resource_, Type_>::cast(Dyn& ptr) const {
		if(ptr.type_id_ == type_id<Type_>()) {
			return {reinterpret_cast<Type_*>(ptr.ptr_)};
		}
		for(const auto& item: derived_) {
			if(auto result{item.cast(ptr)}) {
				return result;
			}
		}
		return {};
	}
	
	template<typename Resource_, typename Type_>
	orl::Option<Type_*> Builder<Resource_, Type_>::build(String const& name, Resource_ resource) const {
		if constexpr(detail::has_decode_ptr_v<Resource_, Type_>) {
			if(names_.contains(name)) {
				return DecodePtr<Resource_>::template decode<Type_>(resource);
			}
		}
		for(const auto& item: derived_) {
			if(auto result{item.build(name, resource)}) {
				return result;
			}
		}
		return {};
	}
	
	template<typename Resource_, typename Type_>
	orl::Option<Type_*> Builder<Resource_, Type_>::implicit_build(Resource_ resource) const {
		for(const auto& item: determine_fn_) {
			if(auto result{item(resource)}) {
				return build(result.some(), std::forward<Resource_>(resource));
			}
		}
		for(const auto& item: derived_) {
			if(auto result{item.implicit_build(resource)}) {
				return result;
			}
		}
		return {};
	}
	
	template<typename Resource_, typename Type_>
	template<typename Base>
	orl::Option<Base*> Builder<Resource_, Type_>::cast_fn(Dyn& ptr) {
		if(auto result{builder().cast(ptr)}) {
			return {result.some()};
		}
		return {};
	}
	
	template<typename Resource_, typename Type_>
	template<typename Base>
	orl::Option<Base*> Builder<Resource_, Type_>::build_fn(String const& name, Resource_ resource) {
		if(auto result{builder().build(name, std::forward<Resource_>(resource))}) {
			return {result.some()};
		}
		return {};
	}
	
	template<typename Resource_, typename Type_>
	template<typename Base>
	orl::Option<Base*> Builder<Resource_, Type_>::implicit_build_fn(Resource_ resource) {
		if(auto result{builder().implicit_build(std::forward<Resource_>(resource))}) {
			return {result.some()};
		}
		return {};
	}
}
