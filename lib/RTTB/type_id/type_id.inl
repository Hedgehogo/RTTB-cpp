//included into type_id.hpp

namespace rttb {
	template<typename Type_>
	constexpr size_t type_id() {
		return reinterpret_cast<size_t>(type_id<Type_>);
	}
}