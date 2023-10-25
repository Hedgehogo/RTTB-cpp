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

std::string OtherBase::other_name() {
	return "OtherBase";
}

SuperDerived::SuperDerived(int g) : Derived(g) {
}

std::string SuperDerived::name() {
	return std::string("SuperDerived-") + std::to_string(g);
}

std::string SuperDerived::other_name() {
	return "SuperDerived";
}

template<typename T>
void template_init() {
	rttb::Builder<int, TemplateBase<T> >::builder().template add_type<TemplateDerived<T> >();
}

bool detail::init() {
	rttb::Builder<int, Base>::builder().add_type<Derived>();
	rttb::Builder<int, Derived>::builder().add_type<SuperDerived>();
	rttb::Builder<int, OtherBase>::builder().add_type<SuperDerived>();
	template_init<bool>();
	return false;
}

void init() {
	[[maybe_unused]] static bool result{detail::init()};
}
