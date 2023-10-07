#pragma once

#include <RTTB/Builder/Builder.hpp>

template<typename Type_>
struct Decode {
};

namespace rttb {
	template<>
	struct DecodePtr<int> {
		template<typename Type_>
		static DecodePtrReturn<::Decode, int, Type_*> decode(int resource) {
			return ::Decode<Type_*>::decode(resource);
		}
	};
}

class Base {
public:
	Base(int g);
	
	virtual std::string name();
	
	virtual ~Base() = default;

protected:
	int g{10};
};

class Derived : public Base {
public:
	Derived(int g);
	
	std::string name() override;
};

template<>
struct Decode<Derived*> {
	static orl::Option<Derived*> decode(int resource);
};

class OtherBase {
public:
	virtual std::string other_name();
	
	virtual ~OtherBase() = default;
};

class SuperDerived : public Derived, public OtherBase {
public:
	SuperDerived(int g);
	
	std::string name() override;
	
	std::string other_name() override;
};

template<>
struct Decode<SuperDerived*> {
	static orl::Option<SuperDerived*> decode(int resource);
};

template<typename T>
class TemplateBase {
public:
	TemplateBase(orl::Option<TemplateBase<T>*> part) :
		part(std::move(part)) {
	}
	
	orl::Option<TemplateBase<T>*>& get_part() {
		return part;
	}
	
	virtual ~TemplateBase() {
		if(part) {
			delete part.some();
		}
	}

private:
	orl::Option<TemplateBase<T>*> part;
};

template<typename T>
class TemplateDerived : Base, public TemplateBase<T> {
public:
	TemplateDerived(int g, orl::Option<TemplateBase<T>*> part) :
		Base(g), TemplateBase<T>(std::move(part)) {
	}
};

template<typename T>
struct Decode<TemplateDerived<T>*> {
	static orl::Option<TemplateDerived<T>*> decode(int resource) {
		if(resource > 100) {
			return new TemplateDerived<T>{resource / 100,
				rttb::Builder<int, TemplateBase<T> >::builder().build("TemplateDerived", resource / 100)
			};
		}
		return new TemplateDerived<T>{resource, {}};
	}
};

namespace detail {
	bool init();
}

void init();
