#include <gtest/gtest.h>

#include "_test/_init/init.hpp"

static_assert(!rttb::detail::has_decode_v<Decode, int, Base*>);
static_assert(rttb::detail::has_decode_v<Decode, int, Derived*>);
static_assert(!rttb::detail::has_decode_ptr_v<int, Base>);
static_assert(rttb::detail::has_decode_ptr_v<int, Derived>);

TEST(Builder, build) {
	init();
	
	{
		auto object = rttb::Builder<int, Base>::builder().build("Derived", 10);
		ASSERT_TRUE(object.is_some());
		ASSERT_EQ(object.except()->name(), "Derived");
		delete object.except();
	}
	
	{
		auto object = rttb::Builder<int, Base>::builder().build("SuperDerived", 10);
		ASSERT_TRUE(object.is_some());
		ASSERT_EQ(object.except()->name(), "SuperDerived10");
		delete object.except();
	}
	
	{
		auto object = rttb::Builder<int, OtherBase>::builder().build("SuperDerived", 10);
		ASSERT_TRUE(object.is_some());
		ASSERT_EQ(object.except()->other_name(), "SuperDerived");
		delete object.except();
	}
	
	{
		auto object = rttb::Builder<int, Base>::builder().build("Other", 10);
		ASSERT_FALSE(object.is_some());
	}
	
	{
		auto object = rttb::Builder<int, Base>::builder().build("FnDerived", 10);
		ASSERT_TRUE(object.is_some());
		ASSERT_EQ(object.except()->name(), "SuperDerived110");
		delete object.except();
	}
	
	{
		auto object = rttb::Builder<int, TemplateBase<bool> >::builder().build("TemplateDerived", 120);
		ASSERT_TRUE(object.is_some());
		ASSERT_TRUE(object.except()->get_part().is_some());
		ASSERT_FALSE(object.except()->get_part().except()->get_part().is_some());
		delete object.except();
	}
}