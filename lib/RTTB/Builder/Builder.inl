//included into Builder.hpp

#include <RTTB/type_id/type_id.hpp>

namespace rttb {
	template<typename Resource_, typename Type_>
	TypeData<Resource_, Type_>::TypeData(CastFn cast) :
		cast(cast) {
	}
	
	template<typename Resource_, typename Type_>
	DerivedData<Resource_, Type_>::DerivedData(TypeData type_data) :
		data_(std::move(type_data)) {
	}
	
	template<typename Resource_, typename Type_>
	template<typename Base>
	typename Builder<Resource_, Type_>::template if_base_t<Base, TypeData<Resource_, Base> >
	Builder<Resource_, Type_>::get_type_data() {
		return {cast_fn<Base>};
	}
	
	template<typename Resource_, typename Type_>
	template<typename Derived>
	typename Builder<Resource_, Type_>::template if_derived_t<Derived, void>
	Builder<Resource_, Type_>::add_type() {
		derived_.push_back(DerivedData{Builder<Resource_, Derived>::builder().template get_type_data<Type_>()});
	}
	
	template<typename Resource_, typename Type_>
	std::optional<Type_*> DerivedData<Resource_, Type_>::cast(Dyn& ptr) const {
		return data_.cast(ptr);
	}
	
	template<typename Resource_, typename Type_>
	Builder<Resource_, Type_>& Builder<Resource_, Type_>::builder() {
		static Builder<Resource_, Type_> result{};
		return result;
	}
	
	template<typename Resource_, typename Type_>
	std::optional<Type_*> Builder<Resource_, Type_>::cast(Dyn& ptr) const {
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
	template<typename Base>
	std::optional<Base*> Builder<Resource_, Type_>::cast_fn(Dyn& ptr) {
		if(auto result{builder().cast(ptr)}) {
			return {result.value()};
		}
		return {};
	}
}
