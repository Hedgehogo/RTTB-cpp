//included into Cast.hpp

namespace rttb {
	template<typename Base_, typename Derived_>
	auto Cast<Base_, Derived_>::cast(void* derived) -> Base_* {
		return reinterpret_cast<Derived_*>(derived);
	}
}
