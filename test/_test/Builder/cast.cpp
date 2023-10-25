#include <gtest/gtest.h>

#include "_test/_init/init.hpp"

TEST(Builder, cast) {
	init();
	
	{
		rttb::Dyn object{SuperDerived{15}};
		auto result = rttb::Builder<int, SuperDerived>::builder().cast(object);
		ASSERT_EQ(object.get_type_index(), std::type_index{typeid(SuperDerived)});
		ASSERT_TRUE(result.is_some());
		ASSERT_EQ(result.except()->name(), "SuperDerived-15");
		ASSERT_EQ(result.except()->other_name(), "SuperDerived");
	}
	
	{
		rttb::Dyn object{SuperDerived{15}};
		auto result = rttb::Builder<int, Derived>::builder().cast(object);
		ASSERT_EQ(object.get_type_index(), std::type_index{typeid(SuperDerived)});
		ASSERT_TRUE(result.is_some());
		ASSERT_EQ(result.except()->name(), "SuperDerived-15");
	}
	
	{
		rttb::Dyn object{SuperDerived{15}};
		auto result = rttb::Builder<int, OtherBase>::builder().cast(object);
		ASSERT_EQ(object.get_type_index(), std::type_index{typeid(SuperDerived)});
		ASSERT_TRUE(result.is_some());
		ASSERT_EQ(result.except()->other_name(), "SuperDerived");
	}
	
	{
		rttb::Dyn object{Derived{15}};
		auto result = rttb::Builder<int, SuperDerived>::builder().cast(object);
		ASSERT_EQ(object.get_type_index(), std::type_index{typeid(Derived)});
		ASSERT_FALSE(result.is_some());
	}
}