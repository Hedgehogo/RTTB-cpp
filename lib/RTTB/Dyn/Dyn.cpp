#include "Dyn.hpp"

namespace rttb {
	Dyn::Dyn(Dyn&& other) : ptr_(other.ptr_), type_id_(other.type_id_), destroy_fn_(other.destroy_fn_) {
		other.destroy_fn_ = not_destroy;
	}
	
	Dyn::~Dyn() {
		destroy_fn_(ptr_);
	}
	
	auto Dyn::operator=(Dyn&& other) -> Dyn& {
		ptr_ = other.ptr_;
		type_id_ = other.type_id_;
		destroy_fn_ = other.destroy_fn_;
		other.destroy_fn_ = not_destroy;
		return *this;
	}
	
	auto Dyn::not_destroy(void* object) -> void {
	}
	
	auto Dyn::get_type_id() const -> size_t {
		return type_id_;
	}
}