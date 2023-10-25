#pragma once

#include <string>
#include <RTTB/Builder/Builder.hpp>

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

template<typename T>
class TemplateBase {
public:
	TemplateBase(std::optional<TemplateBase<T>*> part) :
		part(std::move(part)) {
	}
	
	std::optional<TemplateBase<T>*>& get_part() {
		return part;
	}
	
	virtual ~TemplateBase() {
		if(part) {
			delete part.value();
		}
	}

private:
	std::optional<TemplateBase<T>*> part;
};

template<typename T>
class TemplateDerived : Base, public TemplateBase<T> {
public:
	TemplateDerived(int g, std::optional<TemplateBase<T>*> part) :
		Base(g), TemplateBase<T>(std::move(part)) {
	}
};

namespace detail {
	bool init();
}

void init();
