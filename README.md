# Runtime Type Builders. 
Allows to construct type objects knowing only their name as a string and some universal resource 
for construction (example config).

# API
## Mandatory actions
**Some resource** is the type you pass as the first template parameter to rttb::Builder.

You must have a trait in your library, with an empty default implementation, which you will then 
specialize for classes that are possible to create from **some resource**.

Specializations of this structure must contain a `decode` function takes some resource as an argument 
and returns `orl::Option<{SomeType}*>`, where `{SomeType}` is the type you are specializing in.

Example:
```cpp
template<typename T>
struct Decode {};

template<>
struct Decode<SomeType*> {
	orl::Option<SomeType*> decode(int some_resource);
};
```

You must then specialize rttb::Decode for this trait as follows:
```cpp
namespace rttb {
	template<>
	struct DecodePtr<{SomeResource}> {
		template<typename Type_>
		static DecodePtrReturn<{Decode}, {SomeResource}, Type_{*?}> decode({SomeResource} resource) {
			return {Decode}::decode(resource);
		}
	};
}
```
Where `{Decode}` is your trait and `{SomeResource}` is **some resource**.

Example:
```cpp
namespace rttb {
	template<>
	struct DecodePtr<int> {
		template<typename Type_>
		static DecodePtrReturn<::Decode<Type*>, int, Type_*> decode(int resource) {
			return ::Decode<Type_*>::decode(resource);
		}
	};
}
```

## Builder

`rttb::Builder` is a class containing all API functions of the library, it is recommended to create 
global wrappers for them, as well as shortcuts.

If a type does not have your trait specialization, it is excluded from the sample, but does not 
cause a compilation error even if it has names.

### Function `rttb::Builder<{R}, {T}>::build()`
Accepts a type name and **some resource**, returns an object of the type with that name or nothing if 
no object could be constructed or no type with that name could be found.

Example:
```cpp
orl::Option<Base*> object = rttb::Builder<int, Base>::builder().build("Derived", 10);
```

### Function `rttb::Builder<{R}, {T}>::implicit_build()`
Accepts some resource, tries to determine a type name from that resource, and then calls 
`rttb::Builder<{R}, {T}>::build()` with the name found and the resource that was passed to it.

Example:
```cpp
orl::Option<Base*> object = rttb::Builder<rttb::String, Base>::builder().implicit_build("This is Derived");
```

### Function `rttb::Builder<{R}, {T}>::add_type<{D}>()`
Adds information that `{D}` is a descendant of `{T}`, if it is not, a compilation error will be generated.

Example:
```cpp
rttb::Builder<rttb::String, Base>::builder().add_type<Derived>();
```

### Function `rttb::Builder<{R}, {T}>::add_fn()` 
Accepts a function or lambda with the signature `orl::Option<Type_*>(rttb::String const&, {T})`. 
This function will be called when `rttb::Builder<{R}, {T}>::build()` is called, if it returns something, 
it will be returned as an object. It should return nothing when the first argument is the name of 
the added type.

Example:
```cpp
rttb::Builder<int, Base>::builder().add_fn(
	[](rttb::String const& name, int resource) -> orl::Option<Base*> {
		if(name == "FnDerived") {
			return new Derived{resource};
		}
		return {};
	}
);
```

### Function `rttb::Builder<{R}, {T}>::add_name()`
Adds a new name to the {T} type, if the `name` argument in the `rttb::Builder<{R}, {T}>::build()` 
function matches it, an attempt will be made to create an instance of {T}.

Example:
```cpp
rttb::Builder<int, Derived>::builder().add_name("Derived");
```

### Function `rttb::Builder<{R}, {T}>::add_determine()`
Accepts a function or lambda with the signature `orl::Option<String>({R})`. If during a call 
to `rttb::Builder<{R}, {T}>::implicit_build()` this function returns a name, `rttb::Builder<{R}, {T}>>::build()` 
will be called with that name as the first argument.

Example:
```cpp
rttb::Builder<rttb::String, Base>::builder().add_determine(
	[](rttb::String resource) -> orl::Option<rttb::String> {
		if(resource == "This is Derived") {
			return {"Derived"};
		}
		return {};
	}
);
```
