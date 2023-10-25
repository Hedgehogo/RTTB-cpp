#include <gtest/gtest.h>

#include "_test/_init/init.hpp"

TEST(Builder, cast) {
	init();
	
	{
		rttb::Dyn object{SuperDerived{15}};
		auto result = rttb::Builder<int, SuperDerived>::builder().cast(object);
		ASSERT_EQ(object.get_type_id(), rttb::type_id<SuperDerived>());
		ASSERT_TRUE(result);
		ASSERT_EQ(result.value()->name(), "SuperDerived-15");
		ASSERT_EQ(result.value()->other_name(), "SuperDerived");
	}
	
	{
		rttb::Dyn object{SuperDerived{15}};
		auto result = rttb::Builder<int, Derived>::builder().cast(object);
		ASSERT_EQ(object.get_type_id(), rttb::type_id<SuperDerived>());
		ASSERT_TRUE(result);
		ASSERT_EQ(result.value()->name(), "SuperDerived-15");
	}
	
	{
		rttb::Dyn object{SuperDerived{15}};
		auto result = rttb::Builder<int, OtherBase>::builder().cast(object);
		ASSERT_EQ(object.get_type_id(), rttb::type_id<SuperDerived>());
		ASSERT_TRUE(result);
		ASSERT_EQ(result.value()->other_name(), "SuperDerived");
	}
	
	{
		rttb::Dyn object{Derived{15}};
		auto result = rttb::Builder<int, SuperDerived>::builder().cast(object);
		ASSERT_EQ(object.get_type_id(), rttb::type_id<Derived>());
		ASSERT_FALSE(result);
	}
}