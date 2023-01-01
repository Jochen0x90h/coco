#pragma once

#include "String.hpp"


namespace coco {

/**
 * String buffer with fixed maximum length
 * @tparam N number of 8 bit chars in the buffer
 */
template <int N>
class StringBuffer {
public:
	StringBuffer() : index(0) {}

	template <typename T>
	StringBuffer(T const &value) : index(0) {
		(*this) << value;
	}

	bool empty() const {return this->index == 0;}

	int size() const {return this->index;}

	void clear() {
		this->index = 0;
#ifdef DEBUG
		this->buffer[0] = 0;
#endif
	}

	void resize(int length) {
		this->index = length;
#ifdef DEBUG
		this->buffer[this->index] = 0;
#endif
	}

	char operator [](int index) const {return this->buffer[index];}
	char &operator [](int index) {return this->buffer[index];}

	char const *data() const {return this->buffer;}

	char const *begin() const {return this->buffer;}
	char const *end() const {return this->buffer + this->index;}

	operator String() const {
		return {this->buffer, this->index};
	}
	String string() const {
		return {this->buffer, this->index};
	}


	void write(char ch) {
		if (this->index < N)
			this->buffer[this->index++] = ch;
#ifdef DEBUG
		this->buffer[this->index] = 0;
#endif
	}

	void write(const String &str) {
		int l = std::min(N - this->index, str.size());
		std::copy(str.begin(), str.begin() + l, this->buffer + index);
		this->index += l;
#ifdef DEBUG
		this->buffer[this->index] = 0;
#endif
	}

	/**
	 * Stream a single character
	 */
	StringBuffer &operator <<(char ch) {
		write(ch);
		return *this;
	}

	/**
	 * Stream a string, either C-string or coco::String
	 */
	template <typename T> requires StringConcept<T>
	StringBuffer &operator <<(T const &str) {
		write(str);
		return *this;
	}

protected:

#ifdef DEBUG
	char buffer[N + 1];
#else
	char buffer[N];
#endif
	int index;
};

} // namespace coco
