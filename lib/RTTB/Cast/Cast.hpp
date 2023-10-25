#pragma once

#include <option_result/option_result.hpp>

namespace rttb {
	/// @brief Structure containing functions of conversion from a derived class, passed as void*, to a base class.
	///
	/// @tparam Base_ Base class.
	/// @tparam Derived_ Derived class.
	template<typename Base_, typename Derived_>
	struct Cast {
		static Base_* cast(void* derived);
	};
}

#include "Cast.inl"