#pragma once

#include <cstdint>


// Duration and Time
namespace coco {

/**
	Duration, internal unit is one millisecond
*/
struct Duration {
	int32_t value;

	/**
		Convert duration to seconds
		@return duration in seconds
	*/
	int toSeconds() const {
		return value >> 10;
	}

	constexpr Duration &operator +=(Duration b) {
		this->value += b.value;
		return *this;
	}

	constexpr Duration &operator -=(Duration b) {
		this->value -= b.value;
		return *this;
	}

	constexpr Duration &operator *=(int b) {
		this->value *= b;
		return *this;
	}

	static constexpr Duration max() {return {0x7fffffff};}
};

constexpr Duration operator -(Duration a) {
	return {-a.value};
}

constexpr Duration operator +(Duration a, Duration b) {
	return {a.value + b.value};
}

constexpr Duration operator -(Duration a, Duration b) {
	return {a.value - b.value};
}

constexpr Duration operator *(int a, Duration b) {
	return {a * b.value};
}

constexpr Duration operator *(Duration a, int b) {
	return {a.value * b};
}

constexpr Duration operator *(uint32_t a, Duration b) {
	return {int32_t(a * b.value)};
}

constexpr Duration operator *(Duration a, uint32_t b) {
	return {int32_t(a.value * b)};
}

constexpr Duration operator *(float a, Duration b) {
	float f = a * float(b.value);
	return {int(f)}; // todo round
}

constexpr Duration operator *(Duration a, float b) {
	float f = float(a.value) * b;
	return {int(f)}; // todo round
}

constexpr Duration operator /(Duration a, int b) {
	return {a.value / b};
}

struct DurationDiv {
	Duration a;
	Duration b;

	operator int () {return a.value / b.value;}
	operator float () {return float(a.value) / float(b.value);}
};

/**
	Divide two durations
	@return quotient can be assigned to int or float
*/
constexpr DurationDiv operator /(Duration a, Duration b) {
	return {a.value, b.value};
}

constexpr Duration operator %(Duration a, Duration b) {
	return {a.value % b.value};
}

constexpr bool operator ==(Duration a, Duration b) {
	return a.value == b.value;
}

constexpr bool operator <(Duration a, Duration b) {
	return a.value < b.value;
}

constexpr bool operator <=(Duration a, Duration b) {
	return a.value <= b.value;
}

constexpr bool operator >(Duration a, Duration b) {
	return a.value > b.value;
}

constexpr bool operator >=(Duration a, Duration b) {
	return a.value >= b.value;
}

constexpr Duration min(Duration x, Duration y) {return {x.value < y.value ? x.value : y.value};}


/**
	Time, internal unit is one millisecond
*/
struct Time {
	uint32_t value;

	constexpr Time &operator +=(Duration b) {
		this->value += b.value;
		return *this;
	}

	constexpr Time &operator -=(Duration b) {
		this->value -= b.value;
		return *this;
	}

	bool operator ==(Time const &t) const {
		return this->value == t.value;
	}
};

constexpr Time operator +(Time a, Duration b) {
	return {a.value + b.value};
}

constexpr Time operator -(Time a, Duration b) {
	return {a.value - b.value};
}

constexpr Duration operator -(Time a, Time b) {
	return {int32_t(a.value - b.value)};
}

constexpr bool operator <(Time a, Time b) {
	return int32_t(a.value - b.value) < 0;
}

constexpr bool operator <=(Time a, Time b) {
	return int32_t(a.value - b.value) <= 0;
}

constexpr bool operator >(Time a, Time b) {
	return int32_t(a.value - b.value) > 0;
}

constexpr bool operator >=(Time a, Time b) {
	return int32_t(a.value - b.value) >= 0;
}

constexpr Time min(Time x, Time y) {return {x.value < y.value ? x.value : y.value};}


namespace literals {

/**
	Suffix for milliseconds, e.g. 100ms
*/
constexpr Duration operator "" ms(unsigned long long d) {
	return {int32_t(d)};
}

/**
	Suffix for seconds, e.g. 5s
*/
constexpr Duration operator "" s(unsigned long long d) {
	return {int32_t(d * 1000)};
}

/**
	Suffix for minutes, e.g. 3min
*/
constexpr Duration operator "" min(unsigned long long d) {
	return {int32_t(d * 60 * 1000)};
}

/**
	Suffix for hours, e.g. 8h
*/
constexpr Duration operator "" h(unsigned long long d) {
	return {int32_t(d * 60 * 60 * 1000)};
}

}

using namespace literals;

/*
struct Duration16 {
	uint16_t value;

	constexpr Duration16 &operator =(Duration d) {
		this->value = uint16_t(d.value);
		return *this;
	}

	operator Duration () {
		return {this->value};
	}
};

struct Time16 {
	uint16_t value;

	constexpr Time16 &operator =(Time t) {
		this->value = uint16_t(t.value);
		return *this;
	}

	Time expand(Time t) {
		int carry = int(uint16_t(t.value) < this->value);
		return {(t.value & 0xffff0000) + this->value - (carry << 16)};
	}
};
*/

} // namespace coco
