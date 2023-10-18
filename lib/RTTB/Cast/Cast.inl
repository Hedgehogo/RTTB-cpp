//included into Cast.hpp

namespace rttb {
	template<typename Base_, typename Derived_>
	Base_* Cast<Base_, Derived_>::cast(void* derived) {
		return reinterpret_cast<Derived_*>(derived);
	}
	
	template<typename Base_, typename Derived_>
	orl::Option<Base_*> Cast<Base_, Derived_>::option_cast(orl::Option<void*> derived) {
		if(derived) {
			return {reinterpret_cast<Derived_*>(derived.some())};
		}
		return {};
	}
}
