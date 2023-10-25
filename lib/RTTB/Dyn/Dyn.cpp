#include "Dyn.hpp"

namespace rttb {
	Dyn::Dyn(Dyn&& other) : ptr_(other.ptr_), type_index_(other.type_index_), destroy_fn_(other.destroy_fn_) {
		other.destroy_fn_ = not_destroy;
	}
	
	Dyn::~Dyn() {
		destroy_fn_(ptr_);
	}
	
	Dyn& Dyn::operator=(Dyn&& other) {
		ptr_ = other.ptr_;
		type_index_ = other.type_index_;
		destroy_fn_ = other.destroy_fn_;
		other.destroy_fn_ = not_destroy;
		return *this;
	}
	
	void Dyn::not_destroy(void* object) {
	}
	
	std::type_index Dyn::get_type_index() const {
		return type_index_;
	}
}