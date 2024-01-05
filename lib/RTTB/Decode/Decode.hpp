#pragma once

#include <string>
#include <option_result/option_result.hpp>

namespace rttb {
	using String = std::string;
	
	/// @brief A trait responsible for loading types from a specific resource.
	///
	/// @tparam Resource_ The resource on the basis of which an instance of the type is created.
	///
	/// The specialization of this trait must set a specific value to Resource_, and must contain a function with a similar signature:
	///	@code
	/// template<typename Type>
	/// DecodePtrReturn< <Decode>, <Resource>, Type*> decode(<Resource>);
	/// @endcode
	///
	/// Example of specialization:
	///	@code
	///	template<typename Type_>
	///	struct Decode {};
	///
	///	namespace rttb {
	/// 	template<>
	///		struct DecodePtr<Node const&> {
	///			template<typename Type>
	/// 		DecodePtrReturn<::Decode, Node const&, Type*> decode(Node const& node) {
	///				return Decode<Type>::decode(node);
	///			}
	///		};
	///	}
	/// @endcode
	template<typename Resource_>
	struct DecodePtr {
	};
	
	namespace detail {
		template<typename Resource_>
		struct HasDecodePtrHelper {
			template<typename Type_>
			static auto has_decode_ptr(Type_&&) -> decltype(DecodePtr<Resource_>::template decode<Type_>(std::declval<Resource_>()), std::true_type{}) {
				return {};
			}
			
			[[maybe_unused]] static std::false_type has_decode_ptr(...) {
				return {};
			}
		};
		
		template<typename Resource_, typename Type_>
		using has_decode_ptr = decltype(HasDecodePtrHelper<Resource_>::has_decode_ptr(std::declval<Type_>()));
		
		template<typename Resource_, typename Type_>
		constexpr auto has_decode_ptr_v = has_decode_ptr<Resource_, Type_>::value;
		
		template<typename Resource_>
		struct HasDecodeHelper {
			template<typename Decode_>
			static auto has_decode(Decode_&&) -> decltype(Decode_::decode(std::declval<Resource_>()), std::true_type{}) {
				return {};
			}
			
			[[maybe_unused]] static std::false_type has_decode(...) {
				return {};
			}
		};
		
		template<typename Decode_, typename Resource_>
		using has_decode = decltype(HasDecodeHelper<Resource_>::has_decode(std::declval<Decode_>()));
		
		template<typename Decode_, typename Resource_>
		constexpr bool has_decode_v = detail::has_decode<Decode_, Resource_>::value;
	}
	
	template<typename Decode_, typename Resource_, typename Type_>
	using DecodePtrReturn = std::enable_if_t<detail::has_decode_v<Decode_, Resource_>, orl::Option<Type_> >;
}
