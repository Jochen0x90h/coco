#pragma once

#include "assert.hpp"
#include <cstdint>


namespace coco {

/**
 * Array with known size that only references the data
 * @tparam T type of buffer elements
 * @tparam N size of buffer
 */
template <typename T, int N = -1>
class Array {
public:

	/**
	 * Construct from pointer
	 */
	explicit Array(T *data) noexcept : d(data) {}

	/**
	 * Construct from C-array or C-string
	 */
	template <typename T2>
	constexpr Array(T2 (&array)[N]) noexcept : d(array) {}

	/**
	 * Default copy constructor
	 */
	Array(Array const &array) = default;

	/**
	 * Copy construct from buffer with different data type (e.g. non-const to const)
	 */
	template <typename T2>
	Array(Array<T2, N> array) noexcept : d(array.data()) {}

	/**
	 * Default assignment operator
	 */
	Array &operator =(Array const &array) = default;

	/**
	 * Assignment operator from buffer with different data type (e.g. non-const to const)
	 */
	template <typename T2>
	Array &operator =(Array<T2, N> array) {
		this->d = array.data();
		return *this;
	}

	/**
	 * Check if the array is empty
	 * @return true when empty
	 */
	static bool isEmpty() {return N == 0;}

	/**
	 * Number of elements in the array
	 * @return number of elements
	 */
	static int count() {return N;}

	/**
	 * Fill whole buffer with a value
	 * @param value fill value
	 */
	void fill(T const &value) {
		for (auto &element : *this) {
			element = value;
		}
	}

	/**
	 * Assign from an iterator, also cast each value to destination type
	 * @param src source iterator
	 */
	template <typename InputIt>
	void assign(InputIt src) {
		auto it = begin();
		auto e = end();
		for (; it < e; ++it, ++src) {
			*it = T(*src);
		}
	}

	/**
	 * Comparison operator
	 */
	bool operator ==(Array<T const, N> array) const {
		auto b = array.data();
		for (int i = 0; i < N; ++i) {
			if (this->d[i] != b[i])
				return false;
		}
		return true;
	}

	/**
	 * Element-wise xor assignment operator
	 */
	Array &operator ^=(Array<T const, N> array) {
		auto b = array.data();
		for (int i = 0; i < N; ++i)
			this->d[i] ^= b[i];
		return *this;
	}

	/**
	 * Index operator
	 */
	T &operator [](int index) const {
		assert(uint32_t(index) < N);
		return this->d[index];
	}

	template <int M>
	Array<T, M> array(int index) const {
		assert(uint32_t(index) <= N - M);
		return Array<T, M>(this->d + index);
	}

	/**
	 * Get pointer to data
	 */
	T *data() {return this->d;}

	/**
	 * Iterators
	 */
	T *begin() const {return this->d;}
	T *end() const {return this->d + N;}

protected:

	T *d;
};


/**
 * Array with variable size that only references the data, similar to std::span
 * @tparam T type of buffer element, e.g. int const for a bufferof constant integers
 */
template <typename T>
class Array<T, -1> {
public:

	/**
	 * Default constructor
	 */
	constexpr Array() noexcept : d(nullptr), length(0) {}

	/**
	 * Construct from C-array or C-string
	 */
	template <int N>
	constexpr Array(T (&array)[N]) noexcept : d(array), length(N) {}

	/**
	 * Construct from lenth and data
	 */
	Array(T *data, int length) noexcept : d(data), length(length) {}

	/**
	 * Default copy constructor
	 */
	Array(Array const &array) = default;

	/**
	 * Copy construct from buffer with different data type (e.g. non-const to const)
	 */
	template <typename T2>
	Array(Array<T2> array) noexcept : d(array.data()), length(array.size()) {}

	/**
	 * Default assignment operator
	 */
	Array &operator =(Array const &array) = default;

	/**
	 * Check if the array is empty
	 * @return true when empty
	 */
	bool empty() {return this->length <= 0;}

	/**
	 * Number of elements in the array
	 * @return number of elements
	 */
	int size() const {return this->length;}

	/**
	 * Fill whole buffer with a value
	 * @param value fill value
	 */
	void fill(T const &value) {
		for (auto &element : *this) {
			element = value;
		}
	}

	/**
	 * Assign from an iterator, also cast each value to destination type
	 * @param src source iterator
	 */
	template <typename InputIt>
	void assign(InputIt src) {
		auto it = begin();
		auto e = end();
		for (; it < e; ++it, ++src) {
			*it = T(*src);
		}
	}

	/**
	 * Index operator
	 */
	T &operator [](int index) const {
		assert(uint32_t(index) < this->length);
		return this->d[index];
	}

	/**
	 * Get pointer to the data
	 */
	T *data() {return this->d;}

	/**
	 * Iterators
	 */
	T *begin() const {return this->d;}
	T *end() const {return this->d + this->length;}

protected:

	T *d;
	int length;
};

} // namespace coco
