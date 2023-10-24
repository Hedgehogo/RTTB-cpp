#pragma once

#include <cstddef>

namespace rttb {
	/// @brief Gets a unique identifier of type.
	///
	/// @param Type_ Type.
	///
	/// @return Unique identifier of type.
	template<typename Type_>
	constexpr size_t type_id();
}

#include "type_id.inl"