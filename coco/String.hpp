#pragma once

#include "CStringConcept.hpp"
#include <limits>
#include <algorithm>


namespace coco {

inline bool isWhiteSpace(char ch) {
	return ch == ' ' || ch == '\t' || ch == '\r' || ch == '\n';
}

/**
	String, only references the data
*/
struct String {
	friend bool operator ==(String a, String b);
	friend bool operator <(String a, String b);
public:
	constexpr String() noexcept
		: buffer(), length(0)
	{}

	String(const String &str) = default;

	/**
		Construct from c-string (const char *) or char array (char[])
	*/
	template <typename T> requires (CStringConcept<T>)
	constexpr String(T const &str) noexcept
		: buffer(str), length(coco::length(str))
	{}

	/**
		Construct String from any type of data
	*/
	constexpr String(const char *data, int length)
		: buffer(data), length(length)
	{}

	/**
		Construct/cast String from any type of data
	*/
	explicit String(const void *data, size_t length)
		: buffer(reinterpret_cast<char const*>(data)), length(int(length))
	{}

	/**
		Default assignment operator
	*/
	String &operator =(String const &str) = default;

	bool empty() const {return this->length <= 0;}

	/**
		Get a substring
		@param startIndex start of substring
		@return substring that references the data of this string
	*/
	String substring(int startIndex) const {
		return String(this->buffer + startIndex, std::max(this->length - startIndex, 0));
	}

	/**
		Get a substring
		@param startIndex start of substring
		@param endIndex end of substring (not included)
		@return substring that references the data of this string
	*/
	String substring(int startIndex, int endIndex) const {
		return {this->buffer + startIndex, std::max(std::min(endIndex, this->length) - startIndex, 0)};
	}

	/**
		Remove whitespaces at the beginning and end of the string
	*/
	String trim() const {
		int s = 0;
		while (s < this->length && isWhiteSpace(buffer[s]))
			++s;
		int e = this->length;
		while (e > s && isWhiteSpace(buffer[e - 1]))
			--e;
		return {this->buffer + s, e - s};
	}

	/**
		return the index of the first occurrence of a character
		@param ch character to search
		@param startIndex index where to start searching
		@param defaultValue value to return when the character is not found
		@return index of first occurrence of the character
	*/
	int indexOf(char ch, int startIndex = 0, int defaultValue = -1) const {
		for (int i = startIndex; i < this->length; ++i) {
			if (this->buffer[i] == ch)
				return i;
		}
		return defaultValue;
	}

	/**
		return the index of the last occurrence of a character
		@param ch character to search
		@param startIndex index where to start searching
		@param defaultValue value to return when the character is not found
		@return index of first occurrence of the character
	*/
	int lastIndexOf(char ch, int startIndex = std::numeric_limits<int>::max(), int defaultValue = -1) const {
		int i = std::min(startIndex, this->length);
		while (i > 0) {
			--i;
			if (this->buffer[i] == ch)
				return i;
		}
		return defaultValue;
	}

	/**
		Index operator
	*/
	constexpr char const operator [](int index) const {return this->buffer[index];}

	/**
		Calculate a hash of the string
		http://www.cse.yorku.ca/~oz/hash.html
		@return dj2 hash of string
	*/
	uint32_t hash() const {
		uint32_t h = 5381;
		for (char c : *this) {
			h = (h << 5) + h + uint8_t(c); // hash	33 + c
		}
		return h;
	}

	/**
		Array access data() and size() which is O(1)
	*/
	const char *data() const {return this->buffer;}
	int size() const {return this->length;}

	/**
		Iterators begin() and end()
	*/
	char const *begin() const {return this->buffer;}
	char const *end() const {return this->buffer + this->length;}

protected:

	const char *buffer;
	int length;
};

inline bool operator ==(String a, String b) {
	if (a.length != b.length)
		return false;
	for (int i = 0; i < a.length; ++i) {
		if (a.buffer[i] != b.buffer[i])
			return false;
	}
	return true;
}


inline bool operator <(String a, String b) {
	int length = std::min(a.length, b.length);
	for (int i = 0; i < length; ++i) {
		auto c = (unsigned char)a.buffer[i];
		auto d = (unsigned char)b.buffer[i];
		if (c < d)
			return true;
		if (c > d)
			return false;
	}
	return a.length < b.length;
}


/**
	Assign a string to a fixed size c-string
	@param str fixed size c-string of type char[N]
	@param s source string
*/
template <int N>
inline void assign(char (&str)[N], String const &s) {
	// copy string
	int count = std::min(N, s.size());
	for (int i = 0; i < count; ++i) {
		str[i] = s[i];
	}

	// pad with zeros
	for (int i = count; i < N; ++i) {
		str[i] = 0;
	}
}

} // namespace coco
