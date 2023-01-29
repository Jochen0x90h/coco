#pragma once

#include "assert.hpp"
#include <iterator>


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
	 * Explicitly construct from any data using reinterpret_cast
	 */
	template <typename T2>
	explicit Array(T2 *data) noexcept : buffer(reinterpret_cast<T2 *>(data)) {}

	/**
	 * Construct from C-array or C-string
	 */
	constexpr Array(T (&array)[N]) noexcept : buffer(array) {}

	/**
	 * Copy construct from array with compatible data type (e.g. non-const to const)
	 */
	template <typename T2>
	Array(Array<T2, N> array) noexcept : buffer(array.data()) {}

	/**
	 * Default copy constructor
	 */
	Array(Array const &array) = default;

	/**
	 * Default assignment operator
	 */
	Array &operator =(Array const &array) = default;

	/**
	 * Assignment operator from buffer with different data type (e.g. non-const to const)
	 */
	template <typename T2>
	Array &operator =(Array<T2, N> array) {
		this->buffer = array.data();
		return *this;
	}

	/**
	 * Check if the array is empty
	 * @return true when empty
	 */
	static bool empty() {return N == 0;}

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
	 * Element-wise xor assignment operator
	 */
	/*Array &operator ^=(Array<T const, N> array) {
		auto b = array.data();
		for (int i = 0; i < N; ++i)
			this->buffer[i] ^= b[i];
		return *this;
	}*/

	/**
	 * Index operator
	 */
	T &operator [](int index) const {
		assert(uint32_t(index) < N);
		return this->buffer[index];
	}

	/**
	 * Get an array staring at the given index
	 */
	template <int M>
	Array<T, M> array(int index) const {
		assert(uint32_t(index) <= N - M);
		return Array<T, M>(this->buffer + index);
	}

	/**
	 * Array access data() and size() which is O(1)
	 */
	T *data() {return this->buffer;}
	const T *data() const {return this->buffer;}
	static int size() {return N;}

	/**
	 * Iterators begin() and end()
	 */
	T *begin() {return this->buffer;}
	T *end() {return this->buffer + N;}
	const T *begin() const {return this->buffer;}
	const T *end() const {return this->buffer + N;}

protected:

	T *buffer;
};


/**
 * Array with variable size that only references the data, similar to std::span
 * @tparam T type of array element, e.g. Array<const int> for an array of constant integers
 */
template <typename T>
class Array<T, -1> {
public:

	/**
	 * Default constructor
	 */
	constexpr Array() noexcept : buffer(nullptr), length(0) {}

	/**
	 * Construct from data and lenth
	 */
	Array(T *data, int length) noexcept : buffer(data), length(length) {}

	/**
	 * Explicitly construct from any data and length using reinterpret_cast
	 */
	template <typename T2>
	explicit Array(T2 *data, int length) noexcept : buffer(reinterpret_cast<T2 *>(data)), length(length) {}

	/**
	 * Construct from container supporting std::data() and std::size() with compatible data type (e.g. non-const to const)
	 */
	template <typename T2>
	constexpr Array(T2 &container) noexcept : buffer(std::data(container)), length(std::size(container)) {}

	/**
	 * Default copy constructor
	 */
	Array(Array const &) = default;


	/**
	 * Default assignment operator
	 */
	Array &operator =(Array const &) = default;

	/**
	 * Check if the array is empty
	 * @return true when empty
	 */
	bool empty() {return this->length <= 0;}


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
		return this->buffer[index];
	}

	/**
	 * Array access data() and size() which is O(1)
	 */
	T *data() {return this->buffer;}
	const T *data() const {return this->buffer;}
	int size() const {return this->length;}

	/**
	 * Iterators begin() and end()
	 */
	T *begin() {return this->buffer;}
	T *end() {return this->buffer + this->length;}
	const T *begin() const {return this->buffer;}
	const T *end() const {return this->buffer + this->length;}

protected:

	T *buffer;
	int length;
};


/**
 * Array comparison operator
 * @param array first array to compare
 * @param container second array of any type that supports std::data() and std::size()
 */
template <typename T, int N, typename T2>
bool operator ==(const Array<T, N> &a, const T2 &b) {
	if (a.size() != std::size(b))
		return false;

	auto da = a.data();
	auto db = std::data(b);
	for (int i = 0; i < a.size(); ++i) {
		if (da[i] != db[i])
			return false;
	}
	return true;
}

} // namespace coco
