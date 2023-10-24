#include <gtest/gtest.h>

#include "_test/_init/init.hpp"

static_assert(!rttb::detail::has_decode_v<Decode, int, Base*>);
static_assert(rttb::detail::has_decode_v<Decode, int, Derived*>);
static_assert(!rttb::detail::has_decode_ptr_v<int, Base>);
static_assert(rttb::detail::has_decode_ptr_v<int, Derived>);

TEST(Builder, implicit_build) {
	init();
	
	{
		auto object = rttb::Builder<int, Base>::builder().implicit_build(27);
		ASSERT_TRUE(object.is_some());
		ASSERT_EQ(object.except()->name(), "SuperDerived-27");
		delete object.except();
	}
}