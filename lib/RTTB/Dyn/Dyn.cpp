#include "Dyn.hpp"

namespace rttb {
	Dyn::Dyn(Dyn&& other) : ptr_(other.ptr_), type_id_(other.type_id_), destroy_fn_(other.destroy_fn_) {
		other.destroy_fn_ = not_destroy;
	}
	
	Dyn::~Dyn() {
		destroy_fn_(ptr_);
	}
	
	Dyn& Dyn::operator=(Dyn&& other) {
		ptr_ = other.ptr_;
		type_id_ = other.type_id_;
		destroy_fn_ = other.destroy_fn_;
		other.destroy_fn_ = not_destroy;
		return *this;
	}
	
	void Dyn::not_destroy(void* object) {
	}
}