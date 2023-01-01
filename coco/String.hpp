#pragma once

#include "IsArray.hpp"
#include <limits>
#include <algorithm>


namespace coco {

/**
 * Check if a type is a C-style string, e.g. IsCString<Foo>::value
 * @tparam T type to check if it is an array
 */
template <typename T>
struct IsCString {
	static constexpr bool value = false;
};
template <int N>
struct IsCString<char[N]> {
	static constexpr bool value = true;
};
template <>
struct IsCString<char const *> {
	static constexpr bool value = true;
};

// c-string concept, either char array or char pointer
template <typename T>
concept CStringConcept = IsCString<T>::value;


constexpr int getLength(char const *str, False) {
	int length = 0;
	while (str[length] != 0)
		++length;
	return length;
}

template <int N>
constexpr int getLength(char const (&str)[N], True) {
	int length = 0;
	while (length < N && str[length] != 0)
		++length;
	return length;
}

template <typename T> requires CStringConcept<T>
constexpr int length(T const &str) {
	return getLength(str, IsArray<T>());
}


/**
 * String, only references the data
 */
struct String {
	friend bool operator ==(String a, String b);
	friend bool operator <(String a, String b);
public:
	constexpr String() noexcept
		: d(), length(0)
	{}

	String(String &str) = default;

	String(String const &str) = default;

	/**
	 * Construct from c-string (const char *) or char array (char[])
	 */
	template <typename T> requires CStringConcept<T>
	constexpr String(T const &str) noexcept
		: d(str), length(getLength(str, IsArray<T>()))
	{}

	/**
	 * Construct String from any type of data
	 */
	String(void const *data, int length)
		: d(reinterpret_cast<char const*>(data)), length(length)
	{}

	/**
	 * Default assignment operator
	 */
	String &operator =(String const &str) = default;

	bool empty() const {return this->length <= 0;}
	
	int size() const {return this->length;}
	
	/**
	 * Get a substring
	 * @param startIndex start of substring
	 * @return substring that references the data of this string
	 *
	 */
	String substring(int startIndex) const {
		return String(this->d + startIndex, std::max(this->length - startIndex, 0));
	}

	/**
	 * Get a substring
	 * @param startIndex start of substring
	 * @param endIndex end of substring (not included)
	 * @return substring that references the data of this string
	 *
	 */
	String substring(int startIndex, int endIndex) const {
		return String(this->d + startIndex, std::max(std::min(endIndex, this->length) - startIndex, 0));
	}

	/**
	 * return the index of the first occurrence of a character
	 * @param ch character to search
	 * @param startIndex index where to start searching
	 * @param defaultValue value to return when the character is not found
	 * @return index of first occurrence of the character
	 */
	int indexOf(char ch, int startIndex = 0, int defaultValue = -1) const {
		for (int i = startIndex; i < this->length; ++i) {
			if (this->d[i] == ch)
				return i;
		}
		return defaultValue;
	}

	/**
	 * return the index of the last occurrence of a character
	 * @param ch character to search
	 * @param startIndex index where to start searching
	 * @param defaultValue value to return when the character is not found
	 * @return index of first occurrence of the character
	 */
	int lastIndexOf(char ch, int startIndex = std::numeric_limits<int>::max(), int defaultValue = -1) const {
		int i = std::min(startIndex, this->length);
		while (i > 0) {
			--i;
			if (this->d[i] == ch)
				return i;
		}
		return defaultValue;
	}

	/**
	 * Index operator
	 */
	constexpr char const operator [](int index) const {return this->d[index];}

	/**
	 * Calculate a hash of the string
	 * http://www.cse.yorku.ca/~oz/hash.html
	 * @return dj2 hash of string
	 */
	uint32_t hash() const {
		uint32_t h = 5381;
		for (char c : *this) {
			h = (h << 5) + h + uint8_t(c); // hash * 33 + c
		}
		return h;
	}

	const char *data() const {return this->d;}	
	char const *begin() const {return this->d;}
	char const *end() const {return this->d + this->length;}

protected:

	const char *d;
	int length;
};

inline bool operator ==(String a, String b) {
	if (a.length != b.length)
		return false;
	for (int i = 0; i < a.length; ++i) {
		if (a.d[i] != b.d[i])
			return false;
	}
	return true;
}

//inline bool operator !=(String a, String b) {return !operator ==(a, b);}

inline bool operator <(String a, String b) {
	int length = std::min(a.length, b.length);
	for (int i = 0; i < length; ++i) {
		auto c = (unsigned char)a.d[i];
		auto d = (unsigned char)b.d[i];
		if (c < d)
			return true;
		if (c > d)
			return false;
	}
	return a.length < b.length;
}


/**
 * Assign a string to a fixed size c-string
 * @param str fixed size c-string of type char[N]
 * @param s source string
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


/**
 * Check if a type is a string, either C-string or coco::String, IsString<Foo>::value
 * @tparam T type to check if it is an array
 */
template <typename T>
struct IsString {
	static constexpr bool value = false;
};
template <int N>
struct IsString<char[N]> {
	static constexpr bool value = true;
};
template <>
struct IsString<char const *> {
	static constexpr bool value = true;
};
template <>
struct IsString<String> {
	static constexpr bool value = true;
};

// string concept, either C-string or coco::String
template <typename T>
concept StringConcept = IsString<T>::value;

} // namespace coco
