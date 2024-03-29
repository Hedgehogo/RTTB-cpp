//included into Builder.hpp

namespace rttb {
	template<typename Resource_, typename Type_>
	TypeData<Resource_, Type_>::TypeData(
		CastFn cast,
		BuildFn build,
		BuildDynFn build_dyn,
		ImplicitBuildFn implicit_build,
		ImplicitBuildDynFn implicit_build_dyn
	) :
		cast(cast),
		build(build),
		build_dyn(build_dyn),
		implicit_build(implicit_build),
		implicit_build_dyn(implicit_build_dyn) {
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
	auto DerivedData<Resource_, Type_>::cast(Dyn& ptr) const -> orl::Option<Type_*> {
		if(auto type_data{std::get_if<0>(&data_)}) {
			return type_data->cast(ptr);
		}
		return {};
	}
	
	template<typename Resource_, typename Type_>
	auto DerivedData<Resource_, Type_>::cast(Dyn const& ptr) const -> orl::Option<Type_ const*> {
		if(auto type_data{std::get_if<0>(&data_)}) {
			return {type_data->cast(const_cast<Dyn&>(ptr)).some()};
		}
		return {};
	}
	
	template<typename Resource_, typename Type_>
	auto DerivedData<Resource_, Type_>::build(String const& name, Resource_ resource) const -> orl::Option<Type_*> {
		if(auto type_data{std::get_if<0>(&data_)}) {
			return type_data->build(name, std::forward<Resource_>(resource));
		}
		auto& build_fn{std::get<1>(data_)};
		return build_fn(name, std::forward<Resource_>(resource));
	}
	
	template<typename Resource_, typename Type_>
	auto DerivedData<Resource_, Type_>::build_dyn(String const& name, Resource_ resource) const -> orl::Option<Dyn> {
		if(auto type_data{std::get_if<0>(&data_)}) {
			return type_data->build_dyn(name, std::forward<Resource_>(resource));
		}
		auto& build_fn{std::get<1>(data_)};
		for(auto& result: build_fn(name, std::forward<Resource_>(resource))) {
			return Dyn{std::move(result)};
		}
		return {};
	}
	
	template<typename Resource_, typename Type_>
	auto DerivedData<Resource_, Type_>::implicit_build(Resource_ resource) const -> orl::Option<Type_*> {
		if(auto type_data{std::get_if<0>(&data_)}) {
			return type_data->implicit_build(std::forward<Resource_>(resource));
		}
		return {};
	}
	
	template<typename Resource_, typename Type_>
	auto DerivedData<Resource_, Type_>::implicit_build_dyn(Resource_ resource) const -> orl::Option<Dyn> {
		if(auto type_data{std::get_if<0>(&data_)}) {
			return type_data->implicit_build_dyn(std::forward<Resource_>(resource));
		}
		return {};
	}
	
	template<typename Resource_, typename Type_>
	template<typename Base>
	typename Builder<Resource_, Type_>::template if_base_t<Base, TypeData<Resource_, Base> >
	Builder<Resource_, Type_>::get_type_data() {
		return {cast_fn<Base>, build_fn<Base>, build_dyn_fn<Base>, implicit_build_fn<Base>, implicit_build_dyn_fn<Base>};
	}
	
	template<typename Resource_, typename Type_>
	auto Builder<Resource_, Type_>::get_names() -> NamesContainer const& {
		return tnl::Storage<Type_>::storage().names();
	}
	
	template<typename Resource_, typename Type_>
	template<typename Derived>
	auto Builder<Resource_, Type_>::add_type() -> if_derived_t<Derived, void> {
		derived_.push_back(DerivedData{Builder<Resource_, Derived>::builder().template get_type_data<Type_>()});
	}
	
	template<typename Resource_, typename Type_>
	auto Builder<Resource_, Type_>::add_fn(BuildFn build_fn) -> void {
		derived_.push_back(DerivedData{std::move(build_fn)});
	}
	
	template<typename Resource_, typename Type_>
	auto Builder<Resource_, Type_>::add_name(String&& name) -> void {
		tnl::Storage<Type_>::storage().add_name(std::move(name));
	}
	
	template<typename Resource_, typename Type_>
	auto Builder<Resource_, Type_>::add_determine(DetermineFn determine_fn) -> void {
		determine_fn_.push_back(determine_fn);
	}
	
	template<typename Resource_, typename Type_>
	auto Builder<Resource_, Type_>::cast(Dyn& ptr) const -> orl::Option<Type_*> {
		if(ptr.type_id_ == type_id<Type_>()) {
			return {reinterpret_cast<Type_*>(ptr.ptr_)};
		}
		for(const auto& item: derived_) {
			for(auto result: item.cast(ptr)) {
				return {result};
			}
		}
		return {};
	}
	
	template<typename Resource_, typename Type_>
	auto Builder<Resource_, Type_>::build(String const& name, Resource_ resource) const -> orl::Option<Type_*> {
		if constexpr(detail::has_decode_ptr_v<Resource_, Type_>) {
			if(tnl::Storage<Type_>::storage().names().contains(name)) {
				return DecodePtr<Resource_>::template decode<Type_>(resource);
			}
		}
		for(const auto& item: derived_) {
			for(auto result: item.build(name, resource)) {
				return {result};
			}
		}
		return {};
	}
	
	template<typename Resource_, typename Type_>
	auto Builder<Resource_, Type_>::build_dyn(String const& name, Resource_ resource) const -> orl::Option<Dyn> {
		if constexpr(detail::has_decode_ptr_v<Resource_, Type_>) {
			if(tnl::Storage<Type_>::storage().names().contains(name)) {
				for(auto& result: DecodePtr<Resource_>::template decode<Type_>(resource)) {
					return Dyn{std::move(result)};
				}
			}
		}
		for(const auto& item: derived_) {
			for(auto& result: item.build_dyn(name, resource)) {
				return {std::move(result)};
			}
		}
		return {};
	}
	
	template<typename Resource_, typename Type_>
	auto Builder<Resource_, Type_>::implicit_build(Resource_ resource) const -> orl::Option<Type_*> {
		for(const auto& item: determine_fn_) {
			for(auto& result: item(resource)) {
				return build(result, std::forward<Resource_>(resource));
			}
		}
		for(const auto& item: derived_) {
			for(auto& result: item.implicit_build(resource)) {
				return {std::move(result)};
			}
		}
		return {};
	}
	
	template<typename Resource_, typename Type_>
	auto Builder<Resource_, Type_>::implicit_build_dyn(Resource_ resource) const -> orl::Option<Dyn> {
		for(const auto& item: determine_fn_) {
			for(auto& result: item(resource)) {
				return build_dyn(result, std::forward<Resource_>(resource));
			}
		}
		for(const auto& item: derived_) {
			for(auto& result: item.implicit_build_dyn(resource)) {
				return {std::move(result)};
			}
		}
		return {};
	}
	
	template<typename Resource_, typename Type_>
	auto Builder<Resource_, Type_>::builder() -> Builder<Resource_, Type_>& {
		static Builder<Resource_, Type_> result{};
		return result;
	}
	
	template<typename Resource_, typename Type_>
	template<typename Base>
	auto Builder<Resource_, Type_>::cast_fn(Dyn& ptr) -> orl::Option<Base*> {
		for(auto result: builder().cast(ptr)) {
			return {result};
		}
		return {};
	}
	
	template<typename Resource_, typename Type_>
	template<typename Base>
	auto Builder<Resource_, Type_>::build_fn(String const& name, Resource_ resource) -> orl::Option<Base*> {
		for(auto result: builder().build(name, std::forward<Resource_>(resource))) {
			return {result};
		}
		return {};
	}
	
	template<typename Resource_, typename Type_>
	template<typename Base>
	auto Builder<Resource_, Type_>::build_dyn_fn(String const& name, Resource_ resource) -> orl::Option<Dyn> {
		return builder().build_dyn(name, std::forward<Resource_>(resource));
	}
	
	template<typename Resource_, typename Type_>
	template<typename Base>
	auto Builder<Resource_, Type_>::implicit_build_fn(Resource_ resource) -> orl::Option<Base*> {
		for(auto result: builder().implicit_build(std::forward<Resource_>(resource))) {
			return {result};
		}
		return {};
	}
	
	template<typename Resource_, typename Type_>
	template<typename Base>
	auto Builder<Resource_, Type_>::implicit_build_dyn_fn(Resource_ resource) -> orl::Option<Dyn> {
		return builder().implicit_build_dyn(std::forward<Resource_>(resource));
	}
}
