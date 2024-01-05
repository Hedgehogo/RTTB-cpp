#include <gtest/gtest.h>

#include "_test/_init/init.hpp"

TEST(Builder, implicit_build_dyn) {
	init();
	
	{
		auto object = rttb::Builder<int, Base>::builder().implicit_build_dyn(27);
		ASSERT_TRUE(object.is_some());
		ASSERT_EQ(object.except().get_type_id(), rttb::type_id<SuperDerived>());
	}
}