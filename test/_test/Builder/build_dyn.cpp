#include <gtest/gtest.h>

#include "_test/_init/init.hpp"

TEST(Builder, build_dyn) {
	init();
	
	{
		auto object = rttb::Builder<int, Base>::builder().build_dyn("Derived", 10);
		ASSERT_TRUE(object.is_some());
		ASSERT_EQ(object.except().get_type_id(), rttb::type_id<Derived>());
	}
	
	{
		auto object = rttb::Builder<int, Base>::builder().build_dyn("SuperDerived", 10);
		ASSERT_TRUE(object.is_some());
		ASSERT_EQ(object.except().get_type_id(), rttb::type_id<SuperDerived>());
	}
	
	{
		auto object = rttb::Builder<int, OtherBase>::builder().build_dyn("SuperDerived", 10);
		ASSERT_TRUE(object.is_some());
		ASSERT_EQ(object.except().get_type_id(), rttb::type_id<SuperDerived>());
	}
	
	{
		auto object = rttb::Builder<int, Base>::builder().build_dyn("Other", 10);
		ASSERT_FALSE(object.is_some());
	}
	
	{
		auto object = rttb::Builder<int, Base>::builder().build_dyn("FnDerived", 10);
		ASSERT_TRUE(object.is_some());
		ASSERT_EQ(object.except().get_type_id(), rttb::type_id<Derived>());
	}
	
	{
		auto object = rttb::Builder<int, TemplateBase<bool> >::builder().build_dyn("TemplateDerived", 120);
		ASSERT_TRUE(object.is_some());
		ASSERT_EQ(object.except().get_type_id(), rttb::type_id<TemplateDerived<bool> >());
	}
}