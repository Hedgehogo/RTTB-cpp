#include <gtest/gtest.h>

#include "_test/_init/init.hpp"

TEST(Builder, implicit_build) {
	init();
	
	{
		auto object = rttb::Builder<int, Base>::builder().implicit_build(27);
		ASSERT_TRUE(object.is_some());
		ASSERT_EQ(object.except()->name(), "SuperDerived-27");
		delete object.except();
	}
}