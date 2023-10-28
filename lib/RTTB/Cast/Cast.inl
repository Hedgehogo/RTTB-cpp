//included into Cast.hpp

namespace rttb {
	template<typename Base_, typename Derived_>
	Base_* Cast<Base_, Derived_>::cast(void* derived) {
		return reinterpret_cast<Derived_*>(derived);
	}
}
