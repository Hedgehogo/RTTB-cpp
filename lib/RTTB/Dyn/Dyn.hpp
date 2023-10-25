#pragma once

#include <cstddef>
#include <utility>
#include <type_traits>
#include <typeindex>

namespace rttb {
	/// @brief A class storing (owning) an object of any type.
	/// @note To get the object inside, use Builder<R, T>::cast()
	class Dyn {
	public:
		using DestroyFn = void(*)(void* ptr);
		
		template<typename T>
		Dyn(T&& object);
		
		Dyn(Dyn const& other) = delete;
		
		Dyn(Dyn&& other);
		
		~Dyn();
		
		std::type_index get_type_index() const;
		
		Dyn& operator=(Dyn const& other) = delete;
		
		Dyn& operator=(Dyn&& other);
		
	private:
		template<typename T>
		static void destroy(void* object);
		
		static void not_destroy(void* object);
		
		template<typename Resource_, typename Type_>
		friend struct Builder;
		
		void* ptr_;
		std::type_index type_index_;
		DestroyFn destroy_fn_;
	};
}

#include "Dyn.inl"