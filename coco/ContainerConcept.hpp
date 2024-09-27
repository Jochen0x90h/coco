#pragma once

#include "ArrayConcept.hpp"


namespace coco {

/**
	Container concept, expects support of std::begin() and std::end().
	Applies to C arrays, coco::Array, coco::ArrayBuffer, coco::String, std::string, std::vector, std::list etc.
	Usage:
	template <typename T> requires (ArrayConcept<T>) void foo(const T& container) {...}
*/
template <typename T>
concept ContainerConcept = IsCArray<T> || requires(T t) {
	{std::begin(t)};
	{std::end(t)};
};

} // namespace coco
