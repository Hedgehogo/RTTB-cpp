#pragma once

#include <cstddef>
#include <type_traits>

namespace rttb {
	namespace detail {
		template<typename T>
		struct type_id {
			static T const* const ptr;
		};
		
		template<typename T>
		T const* const type_id<T>::ptr = nullptr;
	}
	
	template<typename T>
	constexpr auto type_id() -> size_t {
		return reinterpret_cast<size_t>(&detail::type_id<T>::ptr);
	}
}
