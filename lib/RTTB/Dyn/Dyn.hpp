#pragma once

#include <cstddef>
#include <utility>
#include <type_traits>

#include <RTTB/type_id/type_id.hpp>

namespace rttb {
	class Dyn {
	public:
		using DestroyFn = void(*)(void* ptr);
		
		template<typename T>
		Dyn(T&& object);
		
		Dyn(Dyn const& other) = delete;
		
		Dyn(Dyn&& other);
		
		~Dyn();
		
		Dyn& operator=(Dyn const& other) = delete;
		
		Dyn& operator=(Dyn&& other);
		
	private:
		template<typename T>
		static void destroy(void* object);
		
		static void not_destroy(void* object);
		
		template<typename Resource_, typename Type_>
		friend struct Builder;
		
		void* ptr_;
		size_t type_id_;
		DestroyFn destroy_fn_;
	};
}

#include "Dyn.inl"