//included into Builder.hpp

namespace rttb {
	namespace detail {
		template<typename Resource, typename Base, typename Derived>
		orl::Option<Base*> build(String const& name, Resource resource) {
			if(auto result{Builder<Resource, Derived>::builder().build(name, std::forward<Resource>(resource))}) {
				return {result.some()};
			}
			return {};
		}
		
		template<typename Resource, typename Base, typename Derived>
		orl::Option<Base*> implicit_build(Resource resource) {
			if(auto result{Builder<Resource, Derived>::builder().implicit_build(std::forward<Resource>(resource))}) {
				return {result.some()};
			}
			return {};
		}
		
		template<typename Resource, typename Base>
		orl::Option<Base*> empty_implicit_build(Resource resource) {
			return {};
		}
	}
	
	template<typename Resource_, typename Type_>
	template<typename Derived>
	std::enable_if_t<std::is_base_of_v<Type_, Derived>, void>
	Builder<Resource_, Type_>::add_type() {
		derived_.push_back(DerivedData{
			detail::build<Resource_, Type_, Derived>,
			detail::implicit_build<Resource_, Type_, Derived>,
		});
	}
	
	template<typename Resource_, typename Type_>
	void Builder<Resource_, Type_>::add_fn(BuildFn build_fn) {
		derived_.push_back(DerivedData{
			std::move(build_fn),
			detail::empty_implicit_build<Resource_, Type_>,
		});
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
	orl::Option<Type_*> Builder<Resource_, Type_>::build(String const& name, Resource_ resource) {
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
	orl::Option<Type_*> Builder<Resource_, Type_>::implicit_build(Resource_ resource) {
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
	Builder<Resource_, Type_>& Builder<Resource_, Type_>::builder() {
		static Builder<Resource_, Type_> result{};
		return result;
	}
}
