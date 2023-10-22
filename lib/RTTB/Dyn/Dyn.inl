//included into Dyn.hpp

namespace rttb {
	template<typename T>
	Dyn::Dyn(T&& object) :
		ptr_(reinterpret_cast<void*>(new T{std::move(object)})),
		type_id_(type_id<T>()),
		destroy_fn_(destroy<T>) {
	}
}
