#pragma once

#include <option_result/option_result.hpp>

namespace rttb {
	template<typename Base_, typename Derived_>
	struct Cast {
		static Base_* cast(void* derived);
		
		static orl::Option<Base_*> option_cast(orl::Option<void*> derived);
	};
}

#include "Cast.inl"