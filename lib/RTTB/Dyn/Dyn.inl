//included into Dyn.hpp

namespace rttb {
	template<typename T>
	Dyn::Dyn(T&& object) :
		ptr_(reinterpret_cast<void*>(new T{std::move(object)})),
		type_id_(type_id<T>()),
		destroy_fn_(destroy<T>) {
	}
	
	template<typename T, typename>
	Dyn::Dyn(T* ptr) :
		ptr_(reinterpret_cast<void*>(ptr)),
		type_id_(type_id<T>()),
		destroy_fn_(destroy<T>) {
	}
	
	template<typename T>
	auto Dyn::destroy(void* object) -> void {
		delete reinterpret_cast<T*>(object);
	}
}
