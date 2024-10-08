#include "convert.hpp"


namespace coco {

std::optional<int> parseInt(String str) {
	if (str.size() <= 0)
		return {};

	// check for sign
	int i = 0;
	bool minus = str[0] == '-';
	if (minus || str[0] == '+') {
		if (str.size() == 1)
			return {};
		i = 1;
	}

	// parse integer
	int value = 0;
	for (; i < str.size(); ++i) {
		uint8_t ch = str[i];
		if (ch >= '0' && ch <= '9') {
			value = value * 10 + ch - '0';
		} else {
			// invalid character
			return {};
		}
	}

	return minus ? -value : value;
}

std::optional<float> parseFloat(String str) {
	if (str.size() <= 0)
		return {};

	// check for sign
	int i = 0;
	bool minus = str[0] == '-';
	if (minus || str[0] == '+') {
		if (str.size() == 1)
			return {};
		i = 1;
	}

	// check if there is only a decimal point
	if (str.size() == i + 1 && str[i] == '.')
		return {};

	// parse integer part
	float value = 0.0f;
	for (; i < str.size(); ++i) {
		char ch = str[i];
		if (ch == '.') {
			++i;
			break;
		} else if (ch >= '0' && ch <= '9') {
			float digit = ch - '0';
			value = value * 10.0f + digit;
		} else {
			// invalid character
			return {};
		}
	}

	// parse decimal places
	float decimal = 1.0f;
	for (; i < str.size(); ++i) {
		char ch = str[i];
		if (ch >= '0' && ch <= '9') {
			float digit = ch - '0';
			decimal *= 0.1f;
			value += digit * decimal;
		} else {
			// invalid character
			return {};
		}
	}

	return minus ? -value : value;
}
/*
int toString(int length, char *str, uint32_t value, int digitCount) {
	// enforce valid parameters
	if (digitCount > 10)
		digitCount = 10;

	// convert into buffer
	char buffer[10];
	char *end = buffer + 10;
	char *it = end;
	while (value > 0 || digitCount > 0) {
		--it;
		*it = '0' + value % 10;
		value /= 10;
		--digitCount;
	};

	// copy to output string
	int i;
	for (i = 0; i < length && it != end; ++i, ++it) {
		str[i] = *it;
	}
	return i;
}
*/
String toString(Array<char, 16> buffer, int64_t value, int digitCount) {
	// enforce valid parameters
	if (digitCount > 15)
		digitCount = 15;

	// handle sign
	bool hasSign = value < 0;
	if (hasSign)
		value = -value;

	// convert into buffer
	char *end = buffer.end();
	char *it = end;
	while (value > 0 || digitCount > 0) {
		--it;
		*it = '0' + value % 10;
		value /= 10;
		--digitCount;
	}
	if (hasSign) {
		--it;
		*it = '-';
	}

	return {it, int(end - it)};
}


static char const *hexTable = "0123456789abcdef";
/*
int hexToString(int length, char *str, uint64_t value, int digitCount) {
	int l = min(length, digitCount);
	for (int i = 0; i < l; ++i) {
		str[i] = hexTable[(value >> (digitCount - 1 - i) * 4) & 0xf];
	}
	return l;
}
*/
String toHexString(Array<char, 16> buffer, uint64_t value, int digitCount) {
	for (int i = 0; i < digitCount; ++i) {
		buffer[i] = hexTable[(value >> (digitCount - 1 - i) * 4) & 0xf];
	}
	return {buffer.data(), digitCount};
}


static float const roundTable[] = {0.5f,
	0.05f, 0.005f, 0.0005f,
	0.00005f, 0.000005f, 0.0000005f,
	0.00000005f, 0.000000005f, 0.0000000005f};

static float const powTable[] = {1.0f,
	10.0f, 100.0f, 1000.0f,
	10000.0f, 100000.0f, 1000000.0f,
	10000000.0f, 100000000.0f, 1000000000.0f};
/*
int toString(int length, char *str, float value, int digitCount, int decimalCount) {
	// enforce valid parameters
	bool suppressTrailingZeros = decimalCount >= 0;
	if (decimalCount < 0)
		decimalCount = -decimalCount;
	if (decimalCount > 9)
		decimalCount = 9;

	// handle sign
	int i = 0;
	if (value < 0) {
		if (0 < length)
			str[i++] = '-';
		value = -value;
	}

	// round
	value += roundTable[decimalCount];

	// extract integer and fractional part
	// check with https://godbolt.org/ and parameters -mthumb -mcpu=cortex-m4 -mfloat-abi=hard -mfpu=fpv4-sp-d16 -Os
	uint32_t ipart = uint32_t(value);
	uint32_t fpart = uint32_t((value - float(ipart)) * powTable[decimalCount]);

	// convert integer part
	i += toString(length - i, str + i, ipart, (fpart == 0 && digitCount == 0) ? 1 : digitCount);

	// remove trailing zeros
	if (suppressTrailingZeros) {
		while (decimalCount > 0 && fpart % 10 == 0) {
			fpart /= 10;
			--decimalCount;
		}
	}

	// convert decimal part
	if (decimalCount > 0 && i < length) {
		str[i++] = '.';

		// truncate to length
		while (i + decimalCount > length) {
			fpart /= 10;
			--decimalCount;
		}

		// convert decimal part
		i += decimalCount;
		char *it = str + i;
		while (decimalCount > 0) {
			--decimalCount;
			--it;
			*it = '0' + fpart % 10;
			fpart /= 10;
		}
	}

	return i;
}
*/


String toString(Array<char, 21> buffer, float value, int digitCount, int decimalCount) {
	// enforce valid parameters
	if (digitCount > 10)
		digitCount = 10;
	bool suppressTrailingZeros = decimalCount >= 0;
	if (decimalCount < 0)
		decimalCount = -decimalCount;
	if (decimalCount > 9)
		decimalCount = 9;

	// handle sign
	bool hasSign = value < 0;
	if (value < 0)
		value = -value;

	// round
	value += roundTable[decimalCount];

	// extract integer and fractional part
	// check with https://godbolt.org/ and parameters -mthumb -mcpu=cortex-m4 -mfloat-abi=hard -mfpu=fpv4-sp-d16 -Os
	uint32_t ipart = uint32_t(value);
	uint32_t fpart = uint32_t((value - float(ipart)) * powTable[decimalCount]);

	// if fractional part and digit count of integer part are zero, set to one digit to get at least a '0'
	if (fpart == 0 && digitCount == 0)
		digitCount = 1;

	// convert integer part into buffer
	char *end = buffer.data() + 11;
	char *it = end;
	while (ipart > 0 || digitCount > 0) {
		--it;
		*it = '0' + ipart % 10;
		ipart /= 10;
		--digitCount;
	}
	if (hasSign) {
		--it;
		*it = '-';
	}

	// remove trailing zeros
	if (suppressTrailingZeros) {
		while (decimalCount > 0 && fpart % 10 == 0) {
			fpart /= 10;
			--decimalCount;
		}
	}

	// convert fractional part into buffer
	if (decimalCount > 0) {
		*end = '.';
		end += 1 + decimalCount;

		// convert decimal part
		char *it = end;
		while (decimalCount > 0) {
			--decimalCount;
			--it;
			*it = '0' + fpart % 10;
			fpart /= 10;
		}
	}

	return {it, int(end - it)};
}

} // namespace coco
