#pragma once

#include <cstddef>
#include <utility>
#include <type_traits>

#include <RTTB/type_id/type_id.hpp>

namespace rttb {
	/// @brief A class storing (owning) an object of any type.
	/// @note To get the object inside, use Builder<R, T>::cast()
	class Dyn {
	public:
		using DestroyFn = void(*)(void* ptr);
		
		template<typename T>
		Dyn(T&& object);
		
		/// @note Use only when pointer stability is required. Use only with a pointer to the final class.
		template<typename T, typename = std::enable_if_t<!std::is_abstract_v<T> > >
		Dyn(T* ptr);
		
		Dyn(Dyn const& other) = delete;
		
		Dyn(Dyn&& other);
		
		~Dyn();
		
		auto get_type_id() const -> size_t;
		
		auto operator=(Dyn const& other) -> Dyn& = delete;
		
		auto operator=(Dyn&& other) -> Dyn&;
		
	private:
		template<typename T>
		static auto destroy(void* object) -> void;
		
		static auto not_destroy(void* object) -> void;
		
		template<typename Resource_, typename Type_>
		friend class DerivedData;
		
		template<typename Resource_, typename Type_>
		friend class Builder;
		
		void* ptr_;
		size_t type_id_;
		DestroyFn destroy_fn_;
	};
}

#include "Dyn.inl"