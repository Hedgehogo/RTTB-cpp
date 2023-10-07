#include "init.hpp"

Base::Base(int g) : g(g) {
}

std::string Base::name() {
	return "Base";
}

Derived::Derived(int g) : Base(g) {
}

std::string Derived::name() {
	return "Derived";
}

orl::Option<Derived*> Decode<Derived*>::decode(int resource) {
	return new Derived{resource};
}

std::string OtherBase::other_name() {
	return "Base";
}

SuperDerived::SuperDerived(int g) : Derived(g) {
}

std::string SuperDerived::name() {
	return std::string("SuperDerived") + std::to_string(g);
}

std::string SuperDerived::other_name() {
	return "SuperDerived";
}

orl::Option<SuperDerived*> Decode<SuperDerived*>::decode(int resource) {
	return new SuperDerived{resource};
}

template<typename T>
void template_init() {
	rttb::Builder<int, TemplateBase<T> >::builder().template add_type<TemplateDerived<T> >();
	rttb::Builder<int, TemplateDerived<T> >::builder().add_name("TemplateDerived");
}

bool detail::init() {
	rttb::Builder<int, Base>::builder().add_type<Derived>();
	rttb::Builder<int, Derived>::builder().add_name("Derived");
	rttb::Builder<int, Derived>::builder().add_type<SuperDerived>();
	rttb::Builder<int, OtherBase>::builder().add_type<SuperDerived>();
	rttb::Builder<int, SuperDerived>::builder().add_name("SuperDerived");
	rttb::Builder<int, Derived>::builder().add_fn(
		[](rttb::String const& name, int resource) -> orl::Option<Derived*> {
			if(name == "FnDerived") {
				return new SuperDerived{resource + 100};
			}
			return {};
		}
	);
	template_init<bool>();
	
	rttb::Builder<int, Derived>::builder().add_determine(
		[](int resource) -> orl::Option<rttb::String> {
			if(resource == 27) {
				return {"SuperDerived"};
			}
			return {};
		}
	);
	
	return false;
}

void init() {
	[[maybe_unused]] static bool result{detail::init()};
}
