#include "convert.hpp"


namespace coco {


namespace detail {

    char *dec(char *end, uint32_t value, int digitCount) {
        char *it = end;
        while (value > 0 || digitCount > 0) {
            --it;
            *it = '0' + value % 10;
            value /= 10;
            --digitCount;
        }
        return it;
    }

    char *dec(char *end, uint64_t value, int digitCount) {
        char *it = end;
        while (value > 0 || digitCount > 0) {
            --it;
            *it = '0' + value % 10;
            value /= 10;
            --digitCount;
        }
        return it;
    }


    /*static double const roundTable[11] = {
        0.5, 0.05, 0.005, 0.0005, 0.00005,
        0.000005, 0.0000005, 0.00000005, 0.000000005, 0.0000000005,
        0.00000000005};

    static double const powTable[11] = {
        1.0, 10.0, 100.0, 1000.0, 10000.0,
        100000.0, 1000000.0, 10000000.0, 100000000.0, 1000000000.0,
        10000000000.0};*/

    char *dec(char *end, double value, int digitCount, int decimalCount) {
        // suppress trailing zeros (50.1 vs 50.1000)
        bool suppressTrailingZeros = decimalCount >= 0;
        if (decimalCount < 0)
            decimalCount = -decimalCount;

        // handle sign
        bool hasSign = value < 0;
        if (hasSign)
            value = -value;

        // calc round and power-of-ten values
        double round = 0.5;
        double pow = 1.0;
        for (int i = 0; i < decimalCount; ++i) {
            round /= 10.0;
            pow *= 10.0;
        }

        // round
        value += round;//roundTable[decimalCount];

        // extract integer and fractional part
        // check with https://godbolt.org/ and parameters -mthumb -mcpu=cortex-m4 -mfloat-abi=hard -mfpu=fpv4-sp-d16 -Os
        uint64_t ipart = uint64_t(value);
        uint64_t fpart = uint64_t((value - double(ipart)) * pow /*Table[decimalCount]*/);

        // if fractional part and digit count of integer part are zero, set to one digit to get at least a '0'
        if (fpart == 0 && digitCount == 0)
            digitCount = 1;

        // remove trailing zeros
        if (suppressTrailingZeros) {
            while (decimalCount > 0 && fpart % 10 == 0) {
                fpart /= 10;
                --decimalCount;
            }
        }

        char *it = end;

        // convert decimal part
        while (decimalCount > 0) {
            --decimalCount;
            --it;
            *it = '0' + fpart % 10;
            fpart /= 10;
        }

        // add decimal point
        if (it < end)
            *--it = '.';

        // convert integer part into buffer
        while (ipart > 0 || digitCount > 0) {
            --it;
            *it = '0' + ipart % 10;
            ipart /= 10;
            --digitCount;
        }

        // add sign
        if (hasSign)
            *--it = '-';

        return it;
    }


    const char *hexTable = "0123456789abcdef";

    char *hex(char *end, uint32_t value, int digitCount) {
        char *b = end;
        while (value > 0 || digitCount > 0) {
            --b;
            *b = hexTable[value & 0xf];
            value >>= 4;
            --digitCount;
        }
        return b;
    }

    char *hex(char *end, uint64_t value, int digitCount) {
        char *b = end;
        while (value > 0 || digitCount > 0) {
            --b;
            *b = hexTable[value & 0xf];
            value >>= 4;
            --digitCount;
        }
        return b;
    }

    char *utf8(char *end, uint32_t code) {
        char *b = end;
        int bits = 0;
        if (code >= 128) {
            --b;
            *b = (code & 63) | 0x80;
            code >>= 6;
            bits = 0xc0;

            uint32_t limit = 32;
            while (code >= limit) {
                --b;
                *b = (code & 63) | 0x80;
                code >>= 6;
                limit >>= 1;
                bits = (bits >> 1) | 0x80;
            }
        }
        --b;
        *b = bits | code;
        return b;
    }
} // namespace detail

ConvertedValue<int> utf8(String str) {
    if (str.empty())
        return {};
    char head = str[0];
    int length = 1;
    if (head & 0x80) {
        if ((head & 0x40) == 0)
            return {};
        ++length;
        char h = head;
        while (h & 0x20) {
            h <<= 1;
            ++length;
        }
    }
    if (str.size() < length)
        return {};
    int code = uint8_t(head) & (0xff >> length);
    for (int i = 1; i < length; ++i) {
        code <<= 6;
        uint8_t byte = str[i];
        if ((byte & 0xc0) != 0x80)
            return {};
        code |= byte & 0x3f;
    }
    return {code, length};
}









//



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
*/


/*
int hexToString(int length, char *str, uint64_t value, int digitCount) {
    int l = min(length, digitCount);
    for (int i = 0; i < l; ++i) {
        str[i] = hexTable[(value >> (digitCount - 1 - i) * 4) & 0xf];
    }
    return l;
}

String toHexString(Array<char, 16> buffer, uint64_t value, int digitCount) {
    for (int i = 0; i < digitCount; ++i) {
        buffer[i] = detail::hexTable[(value >> (digitCount - 1 - i) * 4) & 0xf];
    }
    return {buffer.data(), digitCount};
}
*/

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

/*
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
    value += detail::roundTable[decimalCount];

    // extract integer and fractional part
    // check with https://godbolt.org/ and parameters -mthumb -mcpu=cortex-m4 -mfloat-abi=hard -mfpu=fpv4-sp-d16 -Os
    uint32_t ipart = uint32_t(value);
    uint32_t fpart = uint32_t((value - float(ipart)) * detail::powTable[decimalCount]);

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
}*/

} // namespace coco
