//included into Dyn.hpp

namespace rttb {
	template<typename T>
	Dyn::Dyn(T&& object) :
		ptr_(reinterpret_cast<void*>(new T{std::move(object)})),
		type_index_(std::type_index{typeid(T)}),
		destroy_fn_(destroy<T>) {
	}
	
	template<typename T>
	void Dyn::destroy(void* object) {
		delete reinterpret_cast<T*>(object);
	}
}
