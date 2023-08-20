#pragma once

#include "Array.hpp"
#include "String.hpp"
#include "StringConcept.hpp"


namespace coco {

/**
	Buffer with fixed maximum size
	@tparam T element type
	@tparam N maximum size
*/
template <typename T, int N>
class ArrayBuffer {
public:
	static constexpr int MAX_SIZE = N;

	/**
		Default constructor
	*/
	ArrayBuffer() : length() {}

	/**
		Construct from any container supporting std::begin() and std::end()
		@tparam T2 container type
		@param container container used to initialize the buffer
	*/
	template <typename T2>
	ArrayBuffer(const T2 &container) {
		auto it1 = this->buffer;
		auto end1 = this->buffer + N;
		auto it2 = std::begin(container);
		auto end2 = std::end(container);
		int l = 0;
		for (; it1 != end1 && it2 != end2; ++it1, ++it2, ++l)
			*it1 = *it2;
		this->length = l;
	}

	/**
		Assign from any container supporting std::begin() and std::end()
		@tparam T2 container type
		@param container container used to initialize the buffer
	*/
	template <typename T2>
	ArrayBuffer &operator =(const T2 &container) {
		auto it1 = this->buffer;
		auto end1 = this->buffer + N;
		auto it2 = std::begin(container);
		auto end2 = std::end(container);
		int l = 0;
		for (; it1 != end1 && it2 != end2; ++it1, ++it2, ++l)
			*it1 = *it2;
		this->length = l;
		return *this;
	}

	/**
		Check if the buffer is empty
		@return true when empty
	*/
	bool empty() const {return this->length == 0;}

	/**
		Number of elements in the buffer which is O(1)
		@return number of elements
	*/
	int size() const {return this->length;}

	/**
		Capacity of the buffer, number of pre-allocated elements
		@return capacity
	*/
	int capacity() const {return N;}

	/**
		Clear the buffer
	*/
	void clear() {
		this->length = 0;
	}

	/**
		Resize the buffer
		@param size new size, gets clamped to the maximum size of the buffer
	*/
	void resize(int size) {
		assert(uint32_t(size) <= N);
		this->length = std::min(size, N);
	}

	/**
		Fill whole buffer with a value
		@param value fill value
	*/
	void fill(const T &value) {
		for (auto &element : *this) {
			element = value;
		}
	}

	/**
		Index operator
	*/
	const T &operator [](int index) const {
		assert(uint32_t(index) < this->length);
		return this->buffer[index];
	}
	T &operator [](int index) {
		assert(uint32_t(index) < this->length);
		return this->buffer[index];
	}

	/**
		Append an element to the container
		@param element element to append
	*/
	void append(const T &element) {
		if (this->length < N) {
			T &e = this->buffer[this->length];
			e = element;
			++this->length;
		}
	}

	/**
		Append any container supporting std::begin() and std::end() to the buffer
		@tparam T2 container type
		@param container container whose contents are appended
	*/
	template <typename T2>
	void append(const T2 &container) {
		auto it1 = this->buffer + this->length;
		auto end1 = this->buffer + N;
		auto it2 = std::begin(container);
		auto end2 = std::end(container);
		int l = this->length;
		for (; it1 != end1 && it2 != end2; ++it1, ++it2, ++l)
			*it1 = *it2;
		this->length = l;
	}

	/**
		Get pointer to data
	*/
	T *data() {return this->buffer;}
	const T *data() const {return this->buffer;}

	/**
		Iterators
	*/
	T *begin() {return this->buffer;}
	T *end() {return this->buffer + this->length;}
	const T *begin() const {return this->buffer;}
	const T *end() const {return this->buffer + this->length;}


	int length;
	T buffer[N];
};


/**
	Partial specialization for char to be used as StringBuffer<N>
	@tparam N maximum size
*/
template <int N>
class ArrayBuffer<char, N> {
public:
	static constexpr int MAX_SIZE = N;

	/**
		Default constructor
	*/
	ArrayBuffer() : length() {}

	/**
		Construct from any container supporting std::begin() and std::end()
		@tparam T2 container type
		@param container container used to initialize the buffer
	*/
	template <typename T2>
	ArrayBuffer(const T2 &container) {
		auto it1 = this->buffer;
		auto end1 = this->buffer + N;
		auto it2 = std::begin(container);
		auto end2 = std::end(container);
		int l = 0;
		for (; it1 != end1 && it2 != end2; ++it1, ++it2, ++l)
			*it1 = *it2;
		this->length = l;
	}

	/**
		Assign from any container supporting std::begin() and std::end()
		@tparam T2 container type
		@param container container used to initialize the buffer
	*/
	template <typename T2>
	ArrayBuffer &operator =(const T2 &container) {
		auto it1 = this->buffer;
		auto end1 = this->buffer + N;
		auto it2 = std::begin(container);
		auto end2 = std::end(container);
		int l = 0;
		for (; it1 != end1 && it2 != end2; ++it1, ++it2, ++l)
			*it1 = *it2;
		this->length = l;
		return *this;
	}

	/**
		Check if the buffer is empty
		@return true when empty
	*/
	bool empty() const {return this->length == 0;}

	/**
		Number of elements in the buffer which is O(1)
		@return number of elements
	*/
	int size() const {return this->length;}

	/**
		Capacity of the buffer, number of pre-allocated elements
		@return capacity
	*/
	int capacity() const {return N;}

	/**
		Clear the buffer
	*/
	void clear() {
		this->length = 0;
#ifdef DEBUG
		this->buffer[0] = 0;
#endif
	}

	/**
		Resize the buffer
		@param size new size, gets clamped to the maximum size of the buffer
	*/
	void resize(int size) {
		assert(uint32_t(size) <= N);
		this->length = std::min(size, N);
#ifdef DEBUG
		this->buffer[this->length] = 0;
#endif
	}

	/**
		Fill whole buffer with a value
		@param value fill value
	*/
	void fill(const char value) {
		for (auto &element : *this) {
			element = value;
		}
	}

	/**
		Index operator
	*/
	const char &operator [](int index) const {
		assert(uint32_t(index) < this->length);
		return this->buffer[index];
	}
	char &operator [](int index) {
		assert(uint32_t(index) < this->length);
		return this->buffer[index];
	}

	/**
		Append a single character to the buffer
		@param ch char to append
	*/
	void append(char ch) {
		auto buffer = this->buffer;
		auto capacity = N;

		if (this->length < capacity)
			buffer[this->length++] = ch;
#ifdef DEBUG
		buffer[this->length] = 0;
#endif
	}

	/**
		Append any container supporting std::begin() and std::end() to the buffer
		@tparam T2 container type
		@param container container to append
	*/
	template <typename T2> requires (!StringConcept<T2>)
	void append(const T2 &container) {
		auto buffer = this->buffer;
		auto capacity = N;

		auto it1 = buffer + this->length;
		auto it2 = std::begin(container);
		auto end1 = buffer + capacity;
		auto end2 = std::end(container);
		int l = this->length;
		for (; it1 != end1 && it2 != end2; ++it1, ++it2, ++l)
			*it1 = *it2;
		this->length = l;
#ifdef DEBUG
		buffer[this->length] = 0;
#endif
	}

	template <typename T2> requires (StringConcept<T2>)
	void append(const T2 &string) {
		auto buffer = this->buffer;
		auto capacity = N;

		String s(string);
		int l = std::min(capacity - this->length, s.size());
		std::copy(s.begin(), s.begin() + l, buffer + this->length);
		this->length += l;
#ifdef DEBUG
		buffer[this->length] = 0;
#endif
	}

	/**
		Stream a single character
	*/
	ArrayBuffer &operator <<(char ch) {
		append(ch);
		return *this;
	}

	/**
		Stream a string, either C-string or coco::String
	*/
	template <typename T> requires (StringConcept<T>)
	ArrayBuffer &operator <<(T const &string) {
		append(String(string));
		return *this;
	}

	/**
		Convert to String
		@return string that references this buffer
	*/
	operator String() const {
		return {this->buffer, this->length};
	}

	/**
		Get as string
		@return string that references this buffer
	*/
	String string() const {
		return {this->buffer, this->length};
	}

	/**
		Get a substring
		@param startIndex start of substring
		@return string that references the a part of this buffer
	*/
	String substring(int startIndex) const {
		return {this->buffer + startIndex, std::max(this->length - startIndex, 0)};
	}

	/**
		Get a substring
		@param startIndex start of substring
		@param endIndex end of substring (not included)
		@return string that references a part of this buffer
	*/
	String substring(int startIndex, int endIndex) const {
		return {this->buffer + startIndex, std::max(std::min(endIndex, this->length) - startIndex, 0)};
	}

	/**
		Get pointer to data
	*/
	char *data() {return this->buffer;}
	const char *data() const {return this->buffer;}
	const char *c_str() {
		this->buffer[this->length] = 0;
		return this->buffer;
	}

	/**
		Iterators
	*/
	char *begin() {return this->buffer;}
	char *end() {return this->buffer + this->length;}
	const char *begin() const {return this->buffer;}
	const char *end() const {return this->buffer + this->length;}


	int length;
	char buffer[N + 1];
};

} // namespace coco
