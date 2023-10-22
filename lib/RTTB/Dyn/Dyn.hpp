#pragma once

#include <cstddef>
#include <utility>
#include <type_traits>

#include <RTTB/type_id/type_id.hpp>

namespace rttb {
	template<typename Ptr_>
	using PtrType = decltype(*std::declval<Ptr_>());
	
	template<typename BasePtr_, typename DerivedPtr_>
	constexpr auto is_dyn_convertible_v = std::is_constructible_v<BasePtr_, DerivedPtr_>;
	
	template<typename Ptr_>
	class Dyn {
	public:
		using Type = PtrType<Ptr_>;
		
		Dyn(Ptr_ ptr) : ptr_(ptr), type_id_(type_id<Type>()) {
		}
		
		template<typename BasePtr_, typename = std::enable_if_t<is_dyn_convertible_v<BasePtr_, const Ptr_&> > >
		Dyn(Dyn<BasePtr_> const& other) : ptr_(other.ptr_), type_id_(other.type_id_) {
		}
		
		template<typename BasePtr_, typename = std::enable_if_t<is_dyn_convertible_v<BasePtr_, Ptr_&&> > >
		Dyn(Dyn<BasePtr_>&& other) : ptr_(std::move(other.ptr_)), type_id_(other.type_id_) {
		}
		
		template<typename = std::enable_if_t<std::is_copy_constructible_v<Ptr_> > >
		Dyn(Dyn const& other) : ptr_(other.ptr_), type_id_(other.type_id_) {
		}
		
		template<typename = std::enable_if_t<std::is_move_constructible_v<Ptr_> > >
		Dyn(Dyn&& other) : ptr_(std::move(other.ptr_)), type_id_(other.type_id_) {
		}
		
		Ptr_& get_ptr() {
			return ptr_;
		}
		
		Ptr_ const& get_ptr() const {
			return ptr_;
		}
		
		size_t get_type_id() {
			return type_id_;
		}
		
		template<typename = std::enable_if_t<std::is_copy_constructible_v<Ptr_> > >
		Dyn& operator=(Dyn const& other) {
			ptr_ = other.ptr_;
			type_id_ = other.type_id_;
			return *this;
		}
		
		template<typename = std::enable_if_t<std::is_copy_constructible_v<Ptr_> > >
		Dyn& operator=(Dyn&& other) {
			ptr_ = std::move(other.ptr_);
			type_id_ = other.type_id_;
			return *this;
		}
		
		operator Ptr_&() {
			return ptr_;
		}
		
		operator Ptr_ const&() const {
			return ptr_;
		}
	
	protected:
		Ptr_ ptr_;
		size_t type_id_;
	};
}

#include "Dyn.inl"