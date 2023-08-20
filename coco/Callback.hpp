#pragma once


namespace coco {

/**
	Lightweight method callback
*/
struct Callback {
	void *object;
	void (*method)(void *object);

	void operator ()() {
		this->method(this->object);
	}
};

/**
	Create a callback. If there is a class Foo with method bar() then call makeCallback<Foo, &Foo::bar>(&foo) where foo
	is an instance of Foo.
*/
template <typename T, auto method>
inline Callback makeCallback(void *object) {
	return {
		object,
		[](void *object, auto... args) { (reinterpret_cast<T *>(object)->*method)(args...); }
	};
}

} // namespace coco
