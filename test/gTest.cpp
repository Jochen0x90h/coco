#include <gtest/gtest.h>
#include <coco/platform/compiler.hpp>
#include <coco/align.hpp>
#include <coco/Array.hpp>
#include <coco/ArrayBuffer.hpp>
#include <coco/ArrayConcept.hpp>
#include <coco/bits.hpp>
#include <coco/ContainerConcept.hpp>
#include <coco/convert.hpp>
#include <coco/CStringConcept.hpp>
#include <coco/enum.hpp>
#include <coco/Frequency.hpp>
#include <coco/IsSubclass.hpp>
#include <coco/IntrusiveList.hpp>
#include <coco/IntrusiveQueue.hpp>
#include <coco/Queue.hpp>
#include <coco/PointerConcept.hpp>
#include <coco/PseudoRandom.hpp>
#include <coco/StreamOperators.hpp>
#include <coco/String.hpp>
#include <coco/StringBuffer.hpp>
#include <coco/StringConcept.hpp>
#include <coco/Time.hpp>
#include <coco/Vector2.hpp>
//#include <coco/platform/File.hpp>
#include <vector>
#include <string>
#include <list>
#include <iomanip>


using namespace coco;

// test data
constexpr String strings[] = {"a", "bar", "bar2", "foo", "foo2", "foobar", "foobar2", "z"};

// util (also test __FORCE_INLINE from coco/platform/compiler.hpp)
template <typename T>
COCO_FORCE_INLINE int byteSize(const T &array) {
    return std::size(array) * sizeof(*std::data(array));
}


// align
// -----

TEST(cocoTest, align) {
    EXPECT_EQ(align2(2), 2);
    EXPECT_EQ(align2(3), 4);

    EXPECT_EQ(align4(4), 4);
    EXPECT_EQ(align4(5), 8);

    EXPECT_EQ(align8(8), 8);
    EXPECT_EQ(align8(9), 16);

    EXPECT_EQ(align16(16), 16);
    EXPECT_EQ(align16(17), 32);

    EXPECT_EQ(align32(32), 32);
    EXPECT_EQ(align32(33), 64);

    EXPECT_EQ(align(4, 4), 4);
    EXPECT_EQ(align(5, 4), 8);

    EXPECT_EQ(align(32, 32), 32);
    EXPECT_EQ(align(33, 32), 64);
}


// Array
// -----

// array with fixed size
TEST(cocoTest, ArrayN) {

    // construct array from c-array
    int a1[] = {10, 11, 12};
    Array<int, 3> b1(a1);
    EXPECT_EQ(a1, b1);
    EXPECT_EQ(b1.size(), 3);
    EXPECT_EQ(std::size(b1), 3);
    EXPECT_FALSE(b1.empty());
    for (int i = 0; i < 3; ++i) {
        EXPECT_EQ(b1[i], a1[i]);
        EXPECT_EQ(b1.data()[i], a1[i]);
        EXPECT_EQ(std::data(b1)[i], a1[i]);
    }
    {
        int i = 0;
        for (int e : b1) {
            EXPECT_EQ(e, a1[i++]);
        }
    }

    // construct array from const c-array
    const int a2[] = {20, 21, 22};
    Array<const int, 3> b2(a2);
    EXPECT_EQ(a2, b2);
    EXPECT_EQ(b2.size(), 3);
    EXPECT_EQ(std::size(b2), 3);
    EXPECT_FALSE(b2.empty());
    for (int i = 0; i < 3; ++i) {
        EXPECT_EQ(b2[i], a2[i]);
        EXPECT_EQ(b2.data()[i], a2[i]);
        EXPECT_EQ(std::data(b2)[i], a2[i]);
    }
    {
        int i = 0;
        for (int e : b2) {
            EXPECT_EQ(e, a2[i++]);
        }
    }

    // construct array from const c-string
    Array<const char, 4> str("foo");
    EXPECT_EQ(str.size(), 4);
    EXPECT_EQ(std::size(str), 4);
    EXPECT_FALSE(str.empty());
    for (int i = 0; i < 4; ++i) {
        EXPECT_EQ(str[i], "foo"[i]);
        EXPECT_EQ(str.data()[i], "foo"[i]);
        EXPECT_EQ(std::data(str)[i], "foo"[i]);
    }

    // construct const array from non-const array
    Array<const int, 3> b3(b1);
    EXPECT_EQ(b3.size(), 3);

    // comparison
    EXPECT_TRUE(b1 != b2);
    EXPECT_TRUE(b1 == b3);

    // sub-array
    EXPECT_EQ(b1.array<1>(1).size(), 1);
    EXPECT_EQ(b1.subarray(1).size(), 2);
    EXPECT_EQ(b1.subarray(1, 2).size(), 1);
    EXPECT_EQ(b1.array<1>(1)[0], 11);
    EXPECT_EQ(b1.subarray(1)[0], 11);
    EXPECT_EQ(b1.subarray(1, 2)[0], 11);

    // assign a value
    b1[1] = 10;
    EXPECT_EQ(b1[1], 10);
    //b2[1] = 10; // should not compile

    // assign from other buffer
    b3 = b1;
    EXPECT_EQ(b3[1], b1[1]);
    b3 = b2;
    EXPECT_EQ(b3[1], b2[1]);

    // fill with value and check count
    b1.fill(50);
    int count = 0;
    for (int i : b1) {
        ++count;
        EXPECT_EQ(i, 50);
    }
    EXPECT_EQ(count, 3);

    // byte size
    EXPECT_EQ(byteSize(a1), 3 * sizeof(int));
    EXPECT_EQ(byteSize(a2), 3 * sizeof(int));
    EXPECT_EQ(byteSize(b1), 3 * sizeof(int));
    EXPECT_EQ(byteSize(b2), 3 * sizeof(int));
    EXPECT_EQ(byteSize(str), 4);

}

// array with variable size
TEST(cocoTest, Array) {

    // construct array from c-array
    int a1[] = {10, 11, 12};
    Array<int> b1(a1);
    EXPECT_EQ(a1, b1);
    EXPECT_EQ(b1.size(), 3);
    EXPECT_EQ(std::size(b1), 3);
    EXPECT_FALSE(b1.empty());
    for (int i = 0; i < 3; ++i) {
        EXPECT_EQ(b1[i], a1[i]);
        EXPECT_EQ(b1.data()[i], a1[i]);
        EXPECT_EQ(std::data(b1)[i], a1[i]);
    }
    {
        int i = 0;
        for (int e : b1) {
            EXPECT_EQ(e, a1[i++]);
        }
    }

    // construct array from const c-array
    const int a2[] = {20, 21, 22};
    Array<const int> b2(a2);
    EXPECT_EQ(a2, b2);
    EXPECT_EQ(b2.size(), 3);
    EXPECT_EQ(std::size(b2), 3);
    EXPECT_FALSE(b2.empty());
    for (int i = 0; i < 3; ++i) {
        EXPECT_EQ(b2[i], a2[i]);
        EXPECT_EQ(b2.data()[i], a2[i]);
        EXPECT_EQ(std::data(b2)[i], a2[i]);
    }
    {
        int i = 0;
        for (int e : b2) {
            EXPECT_EQ(e, a2[i++]);
        }
    }

    // construct array from const c-string
    Array<const char> str("foo");
    EXPECT_EQ(str.size(), 4);
    EXPECT_EQ(std::size(str), 4);
    EXPECT_FALSE(str.empty());
    for (int i = 0; i < 4; ++i) {
        EXPECT_EQ(str[i], "foo"[i]);
        EXPECT_EQ(str.data()[i], "foo"[i]);
        EXPECT_EQ(std::data(str)[i], "foo"[i]);
    }

    // construct const array from non-const array
    Array<int const> b3(b1);

    // sub-array
    EXPECT_EQ(b1.subarray(1).size(), 2);
    EXPECT_EQ(b1.subarray(1, 2).size(), 1);
    EXPECT_EQ(b1.subarray(1)[0], 11);
    EXPECT_EQ(b1.subarray(1, 2)[0], 11);

    // assign a value
    b1[1] = 10;
    EXPECT_EQ(b1[1], 10);
    //b2[1] = 10; // should not compile

    // assign from other buffer
    b3 = b1;
    EXPECT_EQ(b3[1], b1[1]);
    b3 = b2;
    EXPECT_EQ(b3[1], b2[1]);

    // fill with value and check count
    b1.fill(50);
    int count = 0;
    for (int i : b1) {
        ++count;
        EXPECT_EQ(i, 50);
    }
    EXPECT_EQ(count, 3);

    // assign
    b1.assign(a2);
    EXPECT_EQ(b1[2], 22);

    // byte size
    EXPECT_EQ(byteSize(a1), 3 * sizeof(int));
    EXPECT_EQ(byteSize(a2), 3 * sizeof(int));
    EXPECT_EQ(byteSize(b1), 3 * sizeof(int));
    EXPECT_EQ(byteSize(b2), 3 * sizeof(int));
    EXPECT_EQ(byteSize(str), 4);
}


// ArrayBuffer
// -----------

TEST(cocoTest, ArrayBuffer) {
    ArrayBuffer<int, 7> b;
    EXPECT_TRUE(b.empty());
    EXPECT_EQ(b.size(), 0);
    EXPECT_EQ(b.capacity(), 7);
    EXPECT_EQ(b.MAX_SIZE, 7);

    // append element
    b.push_back(50);
    EXPECT_FALSE(b.empty());
    EXPECT_EQ(b.size(), 1);
    EXPECT_EQ(std::size(b), 1);
    EXPECT_EQ(b[0], 50);
    EXPECT_EQ(b.data()[0], 50);
    EXPECT_EQ(std::data(b)[0], 50);

    // test overflow
    for (int i = 0; i < 10; ++i) {
        b.push_back(10);
    }
    EXPECT_FALSE(b.empty());
    EXPECT_EQ(b.size(), 7);

    // clear
    b.clear();
    EXPECT_TRUE(b.empty());
    EXPECT_EQ(b.size(), 0);

    // append array
    int a1[] = {10, 11, 12};
    b.append_range(a1);
    EXPECT_EQ(b.size(), 3);
    Array<int> b1(a1);
    b.append_range(b1);
    EXPECT_EQ(b.size(), 6);

    // append buffer
    ArrayBuffer<int, 10> b2;
    b2.push_back(1337);
    b2.push_back(0);
    b.append_range(b2);
    EXPECT_EQ(b.size(), 7);
    EXPECT_EQ(b[6], 1337);

    // iterate over buffer
    int count = 0;
    for (int i : b) {
        ++count;
    }
    EXPECT_EQ(count, 7);

    // resize
    b2.resize(5);
    EXPECT_EQ(b2.size(), 5);

    // fill
    b2.fill(1337);
    for (int i : b2)
        EXPECT_EQ(i, 1337);

    // const buffer
    const ArrayBuffer<int, 7> &cb = b;
    EXPECT_FALSE(cb.empty());
    EXPECT_EQ(cb.size(), 7);
    EXPECT_EQ(std::size(cb), 7);
    EXPECT_EQ(cb[1], 11);
    EXPECT_EQ(cb.data()[1], 11);
    EXPECT_EQ(std::data(cb)[1], 11);

    // iterate over const buffer
    count = 0;
    for (int i : cb) {
        ++count;
    }
    EXPECT_EQ(count, 7);

    // byte size
    EXPECT_EQ(byteSize(b), 7 * sizeof(int));
    EXPECT_EQ(byteSize(b2), 5 * sizeof(int));
    EXPECT_EQ(byteSize(cb), 7 * sizeof(int));

}


// ArrayConcept
// ------------

template <typename T> requires (ArrayConcept<T>)
bool testArrayConcept(T const &x) {return true;}

template <typename T>
bool testArrayConcept(T const &x) {return false;}

TEST(cocoTest, ArrayConcept) {
    // basic types
    int i1 = 0;
    const int i2 = 0;
    int *p1 = nullptr;
    char *p2 = nullptr;
    const char *p3 = nullptr;
    EXPECT_FALSE(testArrayConcept(i1));
    EXPECT_FALSE(testArrayConcept(i2));
    EXPECT_FALSE(testArrayConcept(p1));
    EXPECT_FALSE(testArrayConcept(p2));
    EXPECT_FALSE(testArrayConcept(p3));

    // C arrays
    int a1[] = {1, 2, 3};
    const int a2[] = {1, 2, 3};
    volatile int a3[] = {1, 2, 3};
    char a4[] = {'f', 'o', 'o'};
    const char a5[] = "foo";
    EXPECT_TRUE(testArrayConcept(a1));
    EXPECT_TRUE(testArrayConcept(a2));
    EXPECT_TRUE(testArrayConcept(a3));
    EXPECT_TRUE(testArrayConcept(a4));
    EXPECT_TRUE(testArrayConcept(a5));

    // coco types
    Array<int> a6;
    Array<int, 3> a7(a1);
    ArrayBuffer<int, 10> b1;
    StringBuffer<10> b2;
    String s1 = "foo";
    EXPECT_TRUE(testArrayConcept(a6));
    EXPECT_TRUE(testArrayConcept(a7));
    EXPECT_TRUE(testArrayConcept(b1));
    EXPECT_TRUE(testArrayConcept(b2));
    EXPECT_TRUE(testArrayConcept(s1));

    // std types
    std::vector<int> v1;
    std::string s2;
    std::list<int> l1;
    EXPECT_TRUE(testArrayConcept(v1));
    EXPECT_TRUE(testArrayConcept(s2));
    EXPECT_FALSE(testArrayConcept(l1));
}


template <typename T> requires (ArrayConcept<T, char>)
bool testArrayConceptChar(T const &x) {return true;}

template <typename T>
bool testArrayConceptChar(T const &x) {return false;}

TEST(cocoTest, ArrayConceptChar) {
    // basic types
    int i1 = 0;
    const int i2 = 0;
    int *p1 = nullptr;
    char *p2 = nullptr;
    const char *p3 = nullptr;
    EXPECT_FALSE(testArrayConceptChar(i1));
    EXPECT_FALSE(testArrayConceptChar(i2));
    EXPECT_FALSE(testArrayConceptChar(p1));
    EXPECT_FALSE(testArrayConceptChar(p2));
    EXPECT_FALSE(testArrayConceptChar(p3));

    // C arrays
    int a1[] = {1, 2, 3};
    const int a2[] = {1, 2, 3};
    volatile int a3[] = {1, 2, 3};
    char a4[] = {'f', 'o', 'o'};
    const char a5[] = "foo";
    EXPECT_FALSE(testArrayConceptChar(a1));
    EXPECT_FALSE(testArrayConceptChar(a2));
    EXPECT_FALSE(testArrayConceptChar(a3));
    EXPECT_TRUE(testArrayConceptChar(a4));
    EXPECT_TRUE(testArrayConceptChar(a5));

    // coco types
    Array<int> a6;
    Array<int, 3> a7(a1);
    ArrayBuffer<int, 10> b1;
    StringBuffer<10> b2;
    String s1 = "foo";
    EXPECT_FALSE(testArrayConceptChar(a6));
    EXPECT_FALSE(testArrayConceptChar(a7));
    EXPECT_FALSE(testArrayConceptChar(b1));
    EXPECT_TRUE(testArrayConceptChar(b2));
    EXPECT_TRUE(testArrayConceptChar(s1));

    // std types
    std::vector<int> v1;
    std::string s2;
    std::list<int> l1;
    EXPECT_FALSE(testArrayConceptChar(v1));
    EXPECT_TRUE(testArrayConceptChar(s2));
    EXPECT_FALSE(testArrayConceptChar(l1));
}


// bits
// ----

enum class ExtractEnum {
    FOO_1 = 1 << 4,
    FOO_MASK = 15 << 4
};

TEST(cocoTest, bits) {
    EXPECT_EQ(firstBit(6), 2);

    EXPECT_EQ(extract(int(0x50000000), int(0xf0000000)), 5);

    EXPECT_EQ(extract(ExtractEnum::FOO_1, ExtractEnum::FOO_MASK), 1);

    EXPECT_EQ(parity(uint8_t(4)), 1);
    EXPECT_EQ(parity(uint16_t(4)), 1);
    EXPECT_EQ(parity(uint32_t(4)), 1);
    EXPECT_EQ(parity(uint8_t(0xff)), 0);
    EXPECT_EQ(parity(uint16_t(0xffff)), 0);
    EXPECT_EQ(parity(uint32_t(0xffffffff)), 0);
    EXPECT_EQ(parity(UINT64_C(0xffffffff00000001)), 1);
    EXPECT_EQ(parity(ExtractEnum::FOO_1), 1);


    EXPECT_EQ(popcount(ExtractEnum::FOO_MASK), 4);
    EXPECT_EQ(popcountBefore(0x0a, 0x08), 1);
}

enum class PopcountBeforeEnum {
    NONE = 0,
    COMBINED = 1,
    A = 2,
    B = 4,
    C = 8,
    D = 16
};
COCO_ENUM(PopcountBeforeEnum)

template <PopcountBeforeEnum I>
struct PopcountBeforeStruct {
    int i[std::max(popcount(I), 1)];

    template <PopcountBeforeEnum I2> requires ((I2 & I) == I2 || (I & PopcountBeforeEnum::COMBINED) != 0)
    operator PopcountBeforeStruct<I2> () const {
        PopcountBeforeStruct<I2> s;
        for (int bits = int(I2); bits != 0; bits &= bits - 1) {
            int bit = bits & ~(bits - 1);
            s.i[popcountBefore(I2, bit)] = (int(I) & bit) != 0 ? this->i[popcountBefore(I, bit)] : this->i[0];
        }
        return s;
    }
};

TEST(cocoTest, popcountBefore) {
    {
        PopcountBeforeStruct<PopcountBeforeEnum::B | PopcountBeforeEnum::C | PopcountBeforeEnum::D> s{10, 50, 1337};
        PopcountBeforeStruct<PopcountBeforeEnum::D> s2 = s;
        EXPECT_EQ(s2.i[0], 1337);
    }
    {
        PopcountBeforeStruct<PopcountBeforeEnum::COMBINED> s{1337};
        PopcountBeforeStruct<PopcountBeforeEnum::D> s2 = s;
        EXPECT_EQ(s2.i[0], 1337);
    }
    {
        PopcountBeforeStruct<PopcountBeforeEnum::NONE> s;
    }
}


// ContainerConcept
// ----------------

template <typename T> requires (ContainerConcept<T>)
bool testContainerConcept(T const &x) {return true;}

template <typename T> requires (!ContainerConcept<T>)
bool testContainerConcept(T const &x) {return false;}

TEST(cocoTest, ContainerConcept) {
    // basic types
    int i1 = 0;
    const int i2 = 0;
    int *p1 = nullptr;
    char *p2 = nullptr;
    const char *p3 = nullptr;
    EXPECT_FALSE(testContainerConcept(i1));
    EXPECT_FALSE(testContainerConcept(i2));
    EXPECT_FALSE(testContainerConcept(p1));
    EXPECT_FALSE(testContainerConcept(p2));
    EXPECT_FALSE(testContainerConcept(p3));

    // C arrays
    int a1[] = {1, 2, 3};
    const int a2[] = {1, 2, 3};
    volatile int a3[] = {1, 2, 3};
    char a4[] = {'f', 'o', 'o'};
    const char a5[] = "foo";
    EXPECT_TRUE(testContainerConcept(a1));
    EXPECT_TRUE(testContainerConcept(a2));
    EXPECT_TRUE(testContainerConcept(a3));
    EXPECT_TRUE(testContainerConcept(a4));
    EXPECT_TRUE(testContainerConcept(a5));

    // coco types
    Array<int> a6;
    Array<int, 3> a7(a1);
    ArrayBuffer<int, 10> b1;
    StringBuffer<10> b2;
    String s1 = "foo";
    EXPECT_TRUE(testContainerConcept(a6));
    EXPECT_TRUE(testContainerConcept(a7));
    EXPECT_TRUE(testContainerConcept(b1));
    EXPECT_TRUE(testContainerConcept(b2));
    EXPECT_TRUE(testContainerConcept(s1));

    // std types
    std::vector<int> v1;
    std::string s2;
    std::list<int> l1;
    EXPECT_TRUE(testContainerConcept(v1));
    EXPECT_TRUE(testContainerConcept(s2));
    EXPECT_TRUE(testContainerConcept(l1));
}


// convert
// -------

TEST(cocoTest, convert) {
    // parse int
    int i = *parseInt("-50");
    EXPECT_EQ(i, -50);
    auto i2 = parseInt("foo");
    EXPECT_EQ(i2, std::nullopt);
    auto i3 = parseInt("");
    EXPECT_EQ(i3, std::nullopt);

    // parse float
    float f = *parseFloat("50.99");
    EXPECT_EQ(f, 50.99f);
    auto f2 = parseFloat("bar");
    EXPECT_EQ(f2, std::nullopt);
    auto f3 = parseFloat("");
    EXPECT_EQ(f3, std::nullopt);

    char buffer16[16];

    // int to string
    /*EXPECT_EQ(toString(buffer16, INT64_C(12345678900)), "12345678900");
    EXPECT_EQ(toString(buffer16, INT64_C(-12345678900)), "-12345678900");

    // hex to string
    EXPECT_EQ(toHexString(buffer16, INT64_C(0x123456789AB), 11), String("123456789ab"));

    char buffer21[21];
    EXPECT_EQ(toString(buffer21, 5.5f, 1, 2), "5.5");
    EXPECT_EQ(toString(buffer21, 5.5f, 2, -2), "05.50");*/
}

TEST(cocoTest, convert_dec) {
    // integer to string
    EXPECT_EQ(dec(10), "10");
    EXPECT_EQ(dec(UINT64_C(12345678901234567890)), "12345678901234567890");

    // enum to string
    EXPECT_EQ(dec(ExtractEnum::FOO_1, 3), "016");

    // floating point number to string
    EXPECT_EQ(dec(50.0f, 3), "50");
    EXPECT_EQ(dec(50.0f, -3), "50.000");
    EXPECT_EQ(dec(-50.1234f, 3), "-50.123");
    EXPECT_EQ(dec(1337e6, 3), "1337000000");
    EXPECT_EQ(dec(50.0f, 3, 3), "050");
    EXPECT_EQ(dec(50.0f, 3, -3), "050.000");
    EXPECT_EQ(dec(-50.1234f, 3, 3), "-050.123");
    EXPECT_EQ(dec(1337e6, 3, 3), "1337000000");

    // std::atomic to string
    std::atomic<int> a = 50;
    EXPECT_EQ(dec(a), "50");
    std::atomic<ExtractEnum> e = ExtractEnum::FOO_1;
    EXPECT_EQ(dec(e), "16");
    std::atomic<float> f = -50.1234f;
    EXPECT_EQ(dec(f), "-50.123");
    std::atomic<double> d = 1337e6;
    EXPECT_EQ(dec(d), "1337000000");

    // string to int
    EXPECT_EQ(dec<int>("1337"), ConvertedValue<int>(1337, 4));
    EXPECT_EQ(dec<int>("+1337"), ConvertedValue<int>(1337, 5));
    EXPECT_EQ(dec<int>("-1337"), ConvertedValue<int>(1337, 5));
    EXPECT_EQ(dec<unsigned int>("1337"), ConvertedValue<unsigned int>(1337, 4));
}

TEST(cocoTest, convert_hex) {
    EXPECT_EQ(hex(0x10), "00000010");
    EXPECT_EQ(hex(UINT64_C(0x1234567812345678)), "1234567812345678");

    ExtractEnum e = ExtractEnum::FOO_1;
    EXPECT_EQ(hex(e), "00000010");
    EXPECT_EQ(hex(e, 3), "010");

    std::atomic<int> a = 0x50;
    EXPECT_EQ(hex(a), "00000050");
    EXPECT_EQ(hex(a, 1, 3), " 50");

}

TEST(cocoTest, convert_utf8) {
    EXPECT_EQ(*utf8("a"), 'a');
    EXPECT_EQ(utf8("a").length, 1);
    EXPECT_EQ(*utf8("ä"), 0x00e4);
    EXPECT_EQ(utf8("ä").length, 2);
    EXPECT_EQ(*utf8("😊"), 0x1F60A);
    EXPECT_EQ(utf8("😊").length, 4);

    StringBuffer<32> b;
    b << utf8(65) << ' ' << utf8(0x00e4);
    EXPECT_EQ(b.string(), "A ä");

    auto converted = utf8(-1);
    EXPECT_EQ(converted.begin, converted.data);
    EXPECT_EQ(converted.length, 7);
}


// CStringConcept
// --------------

template <typename T> requires (CStringConcept<T>)
bool testCStringConcept(T const &x) {return true;}

template <typename T> requires (!CStringConcept<T>)
bool testCStringConcept(T const &x) {return false;}

TEST(cocoTest, CStringConcept) {
    const char s1[] = "foo";
    const char *s2 = s1;
    char s3[] = {'f', 'o', 'o'};
    char dummy = 'x';
    char s4[] = {'f', 'o', 'o', 0};
    char *s5 = s4;
    int i1 = 0;

    EXPECT_TRUE(testCStringConcept(s1));
    EXPECT_TRUE(testCStringConcept(s2));
    EXPECT_TRUE(testCStringConcept(s3));
    EXPECT_TRUE(testCStringConcept(s4));
    EXPECT_TRUE(testCStringConcept(s5));
    EXPECT_FALSE(testCStringConcept(i1)); // int does not fulfill the C-string concept

    EXPECT_EQ(length(s1), 3);
    EXPECT_EQ(length(s2), 3);
    EXPECT_EQ(length(s3), 3);
    EXPECT_EQ(length(s4), 3);
    EXPECT_EQ(length(s5), 3);
}


// enum
// ----

enum class Flags : uint8_t {
    FOO = 1,
    BAR = 2
};
COCO_ENUM(Flags);

TEST(cocoTest, cocoEnum) {
    Flags a = Flags::FOO;
    a |= Flags::BAR;
    EXPECT_EQ(a, Flags::FOO | Flags::BAR);

    a &= ~Flags::FOO;
    EXPECT_EQ(a, Flags::BAR);
}


// IsSubclass
// ----------

struct BaseClass {};
struct Subclass : public BaseClass {};

TEST(cocoTest, IsSubclass) {
    EXPECT_TRUE((IsSubclass<Subclass, BaseClass>::value));
    EXPECT_FALSE((IsSubclass<Subclass &, BaseClass>::value));
    EXPECT_FALSE((IsSubclass<Subclass *, BaseClass>::value));
    EXPECT_FALSE((IsSubclass<BaseClass, Subclass>::value));
    EXPECT_FALSE((IsSubclass<int, BaseClass>::value));
}


// IntrusiveList
// -------------

struct ElementBaseClass {
    int value = 10;
};

struct TestListElement : public ElementBaseClass, public IntrusiveListNode {
    int foo;

    TestListElement(int foo) : foo(foo) {}
    TestListElement(IntrusiveList<TestListElement> &list, int foo) : IntrusiveListNode(list), foo(foo) {}
};

using TestList = IntrusiveList<TestListElement>;

TEST(cocoTest, IntrusiveList) {
    // create list
    TestList list;
    EXPECT_TRUE(list.empty());
    EXPECT_EQ(list.count(), 0);

    // create an element
    TestListElement element(10);
    EXPECT_FALSE(element.inList());

    // add element to list
    list.add(element);
    EXPECT_FALSE(list.empty());
    EXPECT_EQ(list.count(), 1);
    EXPECT_TRUE(element.inList());
    EXPECT_EQ(&list.get(0), &element);

    // create another element and add to list in constructor
    TestListElement element2(list, 50);
    EXPECT_FALSE(list.empty());
    EXPECT_EQ(list.count(), 2);
    EXPECT_TRUE(element.inList());

    // iterate over elements
    int count = 0;
    for (auto &e : list) {
        EXPECT_EQ(e.foo, count == 0 ? 10 : 50);
        ++count;
    }
    EXPECT_EQ(count, 2);

    // remove elements
    element.remove();
    EXPECT_EQ(list.count(), 1);
    EXPECT_FALSE(element.inList());
    EXPECT_TRUE(element2.inList());

    element2.remove();
    EXPECT_EQ(list.count(), 0);
    EXPECT_FALSE(element2.inList());

    // call remove second time
    element.remove();
    element2.remove();
}

// additionally inherit from IntrusiveListNode2
struct TestListElement2 : public IntrusiveListNode2 {
    int value = 50;
};

using TestList2 = IntrusiveList2<TestListElement2>;

TEST(cocoTest, IntrusiveList2) {
    // create an element
    TestListElement2 element;

    // create list
    TestList2 list;
    list.add(element);

    // iterate over list
    for (TestListElement2 &element : list) {
        EXPECT_EQ(element.value, 50);
    }
}

// inherit from both IntrusiveListNode and IntrusiveListNode2
struct TestListElement12 : public TestListElement, public IntrusiveListNode2 {
    TestListElement12(int foo) : TestListElement(foo) {}
};

using TestList12 = IntrusiveList<TestListElement12>;
using TestList22 = IntrusiveList2<TestListElement12>;

TEST(cocoTest, IntrusiveList22) {
    // create an element
    TestListElement12 element(50);

    // create list 1
    TestList12 list;
    list.add(element);

    // create list 2
    TestList22 list2;
    list2.add(element);

    // iterate over list 1
    for (TestListElement12 &element : list) {
        EXPECT_EQ(element.value, 10);
        EXPECT_EQ(element.foo, 50);
    }

    // iterate over list 2
    for (TestListElement12 &element : list2) {
        EXPECT_EQ(element.value, 10);
        EXPECT_EQ(element.foo, 50);
    }
}


// IntrusiveQueue
// --------------

struct TestIntrusiveQueueElement : public ElementBaseClass, public IntrusiveQueueNode {
    int foo;
};

TEST(cocoTest, IntrusiveQueue) {
    IntrusiveQueue<TestIntrusiveQueueElement> queue;

    TestIntrusiveQueueElement e1;
    TestIntrusiveQueueElement e2;

    // add elements
    EXPECT_TRUE(queue.empty());
    EXPECT_EQ(queue.pop(), nullptr);
    queue.push(e1);
    EXPECT_FALSE(queue.empty());
    queue.push(e2);

    // check front and back
    EXPECT_EQ(&queue.front(), &e1);
    EXPECT_EQ(&queue.back(), &e2);

    // remove elements
    EXPECT_EQ(queue.pop(), &e1);
    EXPECT_EQ(queue.pop(), &e2);
    EXPECT_EQ(queue.pop(), nullptr);
    EXPECT_TRUE(queue.empty());
}


// PointerConcept
// --------------

template <typename T> requires (PointerConcept<T>)
bool testPointerConcept(T x) {return true;}

template <typename T> requires (!PointerConcept<T>)
bool testPointerConcept(T const &x) {return false;}

TEST(cocoTest, PointerConcept) {
    int *p1 = nullptr;
    const int *p2 = nullptr;
    int *const p3 = nullptr;
    const int *const p4 = nullptr;
    int i1 = 0;
    const int i2 = 0;

    EXPECT_TRUE(testPointerConcept(p1));
    EXPECT_TRUE(testPointerConcept(p2));
    EXPECT_TRUE(testPointerConcept(p3));
    EXPECT_TRUE(testPointerConcept(p4));
    EXPECT_FALSE(testPointerConcept(i1));
    EXPECT_FALSE(testPointerConcept(i2));
}


// PseudoRandom
// ------------

TEST(cocoTest, PseudoRandom) {

    // check if sequence does not repeat for 1 mio numbers
    std::vector<uint32_t> numbers(1000000, 0);

    // xor shift
    {
        XorShiftRandom random(1);
        for (int i = 0; i < std::size(numbers); ++i) {
            auto number = random.draw();
            numbers[i] = number;
        }
        std::sort(std::begin(numbers), std::end(numbers));
        int count = 0;
        int maxCount = 0;
        for (int i = 1; i < std::size(numbers); ++i) {
            if (numbers[i-1] == numbers[i]) {
                ++count;
                if (count > maxCount)
                    maxCount = count;
            }
        }
        EXPECT_EQ(maxCount, 0);
    }

    // kiss
    {
        KissRandom random;
        for (int i = 0; i < std::size(numbers); ++i) {
            auto number = random.draw();
            numbers[i] = number;
        }
        std::sort(std::begin(numbers), std::end(numbers));
        int count = 0;
        int maxCount = 0;
        for (int i = 1; i < std::size(numbers); ++i) {
            if (numbers[i-1] == numbers[i]) {
                ++count;
                if (count > maxCount)
                    maxCount = count;
            }
        }
        EXPECT_EQ(maxCount, 106); // kiss can draw some equal numbers
    }
}


// Queue
// -----

TEST(cocoTest, Queue) {
    using Q = Queue<int, 4>;
    Q queue;

    // check if initially empty
    EXPECT_TRUE(queue.empty());

    // add and remove one element
    queue.pushBack(5);
    EXPECT_EQ(queue.back(), 5);
    queue.popFront();
    EXPECT_TRUE(queue.empty());

    // add elements until queue is full
    int i = 0;
    //int &head = queue.getNextBack();
    while (!queue.full()) {
        queue.pushBack(1000 + i);
        ++i;
    }
    EXPECT_EQ(i, 4);
    EXPECT_EQ(queue.size(), 4);
    //EXPECT_EQ(head, 1000);

    // remove one element
    queue.popFront();
    EXPECT_FALSE(queue.empty());
    EXPECT_FALSE(queue.full());
    EXPECT_EQ(queue.size(), 3);
    EXPECT_EQ(queue.front(), 1000 + 1);

    // clear and "resurrect" first element (is used in radio driver)
    queue.clear();
    queue.pushBack();
    EXPECT_EQ(queue.size(), 1);
    EXPECT_EQ(queue.front(), 1000 + 1);
}


// String
// ------

String stringParameter(const String &str) {return str;}

TEST(cocoTest, String) {
    // construct String from c-string
    {
        // from c-string array
        String foo("foo");
        EXPECT_TRUE(foo == "foo");
        EXPECT_FALSE(foo != "foo");
        EXPECT_EQ(foo, "foo");
        EXPECT_EQ(foo.size(), 3);

        // from c-string array containing a zero
        String f0o("f\0o");
        EXPECT_TRUE(f0o == "f");
        EXPECT_FALSE(f0o != "f");
        EXPECT_EQ(f0o, "f");
        EXPECT_EQ(f0o.size(), 1);

        // from c-string pointer
        const char *cstr = "bar";
        String bar(cstr);
        EXPECT_TRUE(bar == "bar");
        EXPECT_FALSE(bar != "bar");
        EXPECT_EQ(bar, "bar");
        EXPECT_EQ(bar.size(), 3);

        // from r-value
        String bar2(reinterpret_cast<const char *>(cstr));
        EXPECT_EQ(bar2, "bar");
        EXPECT_EQ(bar2.size(), 3);

        // implicit conversion
        EXPECT_EQ(stringParameter("foo"), "foo");
        EXPECT_EQ(stringParameter(cstr), "bar");
        EXPECT_EQ(stringParameter(reinterpret_cast<const char *>(cstr)), "bar");

        // todo String utf8(u8"äöü");
    }

    // construct String from c-array
    {
        char array[] = {'f', 'o', 'o'};
        char dummy = 'x';
        String foo(array);
        EXPECT_EQ(foo, "foo");
        EXPECT_EQ(foo.size(), 3);

        // implicit conversion
        EXPECT_EQ(stringParameter(array), "foo");
    }

    // construct String from std::string
    {
        std::string str = "foo";
        String foo(str);
        EXPECT_EQ(foo, "foo");
        EXPECT_EQ(foo.size(), 3);

        // implicit conversion
        EXPECT_EQ(stringParameter(str), "foo");
    }

    // construct std::string from String
    {
        String str = "foo";
        std::string foo(str);
        EXPECT_EQ(foo, "foo");
        EXPECT_EQ(foo.size(), 3);
    }

    // construct String from std::string_view
    {
        std::string_view str = "bar";
        String bar(str);
        EXPECT_EQ(bar, "bar");
        EXPECT_EQ(bar.size(), 3);

        // implicit conversion
        EXPECT_EQ(stringParameter(str), "bar");
    }


    // construct std::string_view from String
    {
        String str = "bar";
        std::string_view bar(str);
        EXPECT_EQ(bar, "bar");
        EXPECT_EQ(bar.size(), 3);
    }

    // construct String from data and size
    {
        const char *cstr = "foo";
        String f(cstr, 1);
        EXPECT_EQ(f, "f");

        uint8_t data[] = {'b', 'a', 'r'};
        String b(data, 1);
        EXPECT_EQ(b, "b");
    }

    // less operator
    for (int j = 0; j < std::size(strings); ++j) {
        for (int i = 0; i < std::size(strings); ++i) {
            EXPECT_EQ(i < j, strings[i] < strings[j]);
        }
    }

    // substring
    EXPECT_EQ(String("foo").substring(1), "oo");
    EXPECT_EQ(String("foo").substring(0, 2), "fo");
    EXPECT_EQ(String("foo").substring(1, 99), "oo");

    // startsWith
    EXPECT_TRUE(String("foo").startsWith("fo"));
    EXPECT_FALSE(String("foo").startsWith("bar"));
    EXPECT_FALSE(String("foo").startsWith("fooo"));

    // endsWith
    EXPECT_TRUE(String("foo").endsWith("oo"));
    EXPECT_FALSE(String("foo").endsWith("bar"));
    EXPECT_FALSE(String("foo").endsWith("fooo"));

    // trim
    EXPECT_EQ(String("   ").trim(), "");
    EXPECT_EQ(String("  foo  ").trim(), "foo");
    EXPECT_EQ(String("\t\r\n  bar \t\r\n").trim(), "bar");
    EXPECT_EQ(String("\0foo\0", 5).trim(), String("foo"));

    // compare
    {
        String coco_foo = "foo";
        std::string std_foo = "foo";

        String coco_bar = "bar";
        std::string std_bar = "bar";

        EXPECT_TRUE(coco_foo == coco_foo);
        EXPECT_TRUE(coco_foo == std_foo);
        EXPECT_TRUE(std_foo == coco_foo);

        EXPECT_TRUE(coco_foo != coco_bar);
        EXPECT_TRUE(coco_foo != std_bar);
        EXPECT_TRUE(std_foo != coco_bar);

        EXPECT_TRUE(coco_bar < coco_foo);
        EXPECT_TRUE(coco_bar < std_foo);
        EXPECT_TRUE(std_bar < coco_foo);

        EXPECT_TRUE(coco_foo > coco_bar);
        EXPECT_TRUE(coco_foo > std_bar);
        EXPECT_TRUE(std_foo > coco_bar);
    }

    // indexOf
    {
        EXPECT_EQ(String("foo").indexOf('x'), -1);
        EXPECT_EQ(String("abc").indexOf('b'), 1);

        EXPECT_EQ(String("foo").indexOf("bar"), -1);
        EXPECT_EQ(String("aaabc").indexOf("aabc"), 1);
        EXPECT_EQ(String("ababc").indexOf("abc"), 2);
    }

    // convert to string_view
    {
        String str("foo");
        std::string_view view = str;
        EXPECT_EQ(view, "foo");
    }
}

// StringBuffer, StreamOperators
// -----------------------------

TEST(cocoTest, StringBuffer) {
    const char *space = " ";

    // create a buffer and check size/capacity
    StringBuffer<100> b;
    EXPECT_TRUE(b.empty());
    EXPECT_EQ(b.size(), 0);
    EXPECT_EQ(std::size(b), 0);
    EXPECT_EQ(b.CAPACITY, 100);
    EXPECT_EQ(b.capacity(), 100);

    // check if assignment of string omits the zero termination
    b = "foo";
    EXPECT_EQ(b.size(), 3);
    b = std::string("bar");
    EXPECT_EQ(b.size(), 3);
    b.clear();

    // check if append omits the zero termination
    b.append_range("foo");
    EXPECT_EQ(b.size(), 3);
    b.append_range(std::string("bar"));
    EXPECT_EQ(b.size(), 6);
    b.clear();

    // check if resize clears the elements
    b.resize(3);
    EXPECT_EQ(b[2], 0);
    b.resize(0);

    // check if stream operators are compatible with different string types
    b << String();
    b << std::string();
    b << std::string_view();

    // check stream operators
    b << dec(int16_t(-99), 3) << ' ';
    b << dec(-123456, 7) << ' ';
    b << dec(UINT64_C(1234567890123)) << ' ';
    b << hex(0x5, 2) << " ";
    b << hex(uint16_t(0x1337)) << ' ';
    b << hex(UINT64_C(0xabcdef12), 1) << space;
    b << dec(0.0f) << " ";
    b << dec(0.0f, 0, 2) << " ";
    b << dec(0.001234567f, 9) << " ";
    b << dec(0.5f) << ' ';
    b << dec(0.5f, 0, 2) << " ";
    b << dec(1.0f, 1) << ' ';
    b << dec(1.0f, -1) << " ";
    b << dec(-5.9f) << " ";
    b << dec(100.9999f) << " ";
    b << dec(2000000000);
    EXPECT_EQ(b, "-099 -0123456 1234567890123 05 1337 abcdef12 0 0 0.001234567 0.5 .5 1 1.0 -5.9 101 2000000000");
    EXPECT_STREQ(b.c_str(), "-099 -0123456 1234567890123 05 1337 abcdef12 0 0 0.001234567 0.5 .5 1 1.0 -5.9 101 2000000000");
    EXPECT_EQ(byteSize(b), 93);

    // create another buffer
    StringBuffer<5> c;

    // assign a float and check content
    c << dec(0.000000001f, 9);
    EXPECT_EQ(c, "0.000");
    EXPECT_FALSE(c.empty());
    EXPECT_EQ(c.size(), 5);
    EXPECT_EQ(std::size(c), 5);
    EXPECT_EQ(c.CAPACITY, 5);
    EXPECT_EQ(c.capacity(), 5);
    for (int i = 0; i < 5; ++i) {
        EXPECT_EQ(c[i], "0.000"[i]);
        EXPECT_EQ(c.data()[i], "0.000"[i]);
        EXPECT_EQ(std::data(c)[i], "0.000"[i]);
    }
    EXPECT_EQ(byteSize(c), 5);

    // check if buffers are not equal (not equal is implemented automatically)
    EXPECT_NE(b, c);

    // convert to string_view
    b.clear();
    b << "foo";
    std::string_view view = b;
    EXPECT_EQ(view, "foo");


    // Vector2
    /*b.clear();
    int2 i2 = {10, 100};
    float2 f2 = {1.1f, -5.5f};
    b << dec(i2) << ' ' << hex(i2, 2) << ' ' << flt(f2);
    EXPECT_EQ(b, "10 100 0a 64 1.1 -5.5");*/
}


// StringConcept
// --------------

template <typename T> requires (StringConcept<T>)
bool testStringConcept(T const &x) {return true;}

template <typename T> requires (!StringConcept<T>)
bool testStringConcept(T const &x) {return false;}

TEST(cocoTest, StringConcept) {
    // basic types
    int i1 = 0;
    const int i2 = 0;
    int *p1 = nullptr;
    char *p2 = nullptr;
    const char *p3 = nullptr;
    EXPECT_FALSE(testStringConcept(i1));
    EXPECT_FALSE(testStringConcept(i2));
    EXPECT_FALSE(testStringConcept(p1));
    EXPECT_TRUE(testStringConcept(p2));
    EXPECT_TRUE(testStringConcept(p3));

    // C arrays
    int a1[] = {1, 2, 3};
    const int a2[] = {1, 2, 3};
    volatile int a3[] = {1, 2, 3};
    char a4[] = {'f', 'o', 'o'};
    const char a5[] = "foo";
    EXPECT_FALSE(testStringConcept(a1));
    EXPECT_FALSE(testStringConcept(a2));
    EXPECT_FALSE(testStringConcept(a3));
    EXPECT_TRUE(testStringConcept(a4));
    EXPECT_TRUE(testStringConcept(a5));

    // coco types
    Array<int> a6;
    Array<int, 3> a7(a1);
    ArrayBuffer<int, 10> b1;
    StringBuffer<10> b2;
    String s1 = "foo";
    EXPECT_FALSE(testStringConcept(a6));
    EXPECT_FALSE(testStringConcept(a7));
    EXPECT_FALSE(testStringConcept(b1));
    EXPECT_TRUE(testStringConcept(b2));
    EXPECT_TRUE(testStringConcept(s1));

    // std types
    std::vector<int> v1;
    std::string s2;
    std::list<int> l1;
    EXPECT_FALSE(testStringConcept(v1));
    EXPECT_TRUE(testStringConcept(s2));
    EXPECT_FALSE(testStringConcept(l1));
}


// Unit
// ----

template <int P>
using Value = Unit<int, P, 0>;

template <int P>
using AbsoluteValue = AbsoluteUnit<int, P, 0>;

TEST(cocoTest, Unit) {
    Value<0> a(10);
    Value<3> b(59);
    Value<0> c(100000);
    Value<0> x;
    AbsoluteValue<0> p(0);
    AbsoluteValue<0> q(0x70000000);
    AbsoluteValue<0> r(0x90000000);

    EXPECT_EQ(int(a), 10);
    EXPECT_EQ(int(b), 59000);
    EXPECT_EQ(float(a), 10.0f);
    EXPECT_EQ(float(b), 59000.0f);

    EXPECT_FALSE(a == b);
    EXPECT_TRUE(a != b);
    EXPECT_TRUE(a < b);
    EXPECT_FALSE(q == p);
    EXPECT_TRUE(p < q);
    EXPECT_TRUE(q < r);
    EXPECT_TRUE(r < p);

    EXPECT_EQ(-a, (Value<0>(-10)));

    x = a;
    x += b;
    EXPECT_EQ(x, (Value<0>(59010)));
    EXPECT_EQ(a + b, (Value<0>(59010)));
    EXPECT_EQ(p + b, (AbsoluteValue<0>(59000)));

    x = a;
    x -= b;
    EXPECT_EQ(x, (Value<0>(-58990)));
    EXPECT_EQ(a - b, (Value<0>(-58990)));
    EXPECT_EQ(p - b, (AbsoluteValue<0>(-59000)));

    x = a;
    x *= c;
    EXPECT_EQ(x, (Value<0>(1000000)));
    EXPECT_EQ(a * 5, (Value<0>(50)));
    EXPECT_EQ(a * b, (Value<0>(590000)));

    x = a;
    x /= 2;
    EXPECT_EQ(x, (Value<0>(5)));
    EXPECT_EQ(b / 5, (Value<3>(11)));
    EXPECT_EQ(b / a, (Value<0>(5900)));

    EXPECT_EQ(c % a, (Value<0>(0)));
}

TEST(cocoTest, Duration) {
    // cast
    {
        auto a = 3s;
        EXPECT_FLOAT_EQ(int(a), 3);
        EXPECT_FLOAT_EQ(float(a), 3.0f);
    }

    // comparison
    {
        EXPECT_TRUE(5us == 5000ns);
        EXPECT_TRUE(5ms == 5000us);
        EXPECT_TRUE(5s == 5000ms);
        EXPECT_TRUE(5min == 300s);
        EXPECT_TRUE(5h == 300min);
        EXPECT_TRUE(5s > 5ms);
    }

    // addition/subtraction
    {
        auto a = 4s;
        auto b = 1s;
        auto c = 100ms;
        EXPECT_EQ(a + b, 5s);
        EXPECT_EQ(a - b, 3s);
        EXPECT_EQ(b + c, 1100ms);
        EXPECT_EQ(a - c, 3900ms);
        a += b;
        EXPECT_EQ(a, 5s);
        a -= b;
        EXPECT_EQ(a, 4s);
    }

    // multiplication
    {
        auto a = 4s;
        EXPECT_EQ(a * 2, 8s);
        EXPECT_EQ(2 * a, 8s);
        a *= 3;
        EXPECT_EQ(a, 12s);
    }

    // division
    {
        auto a = 5s;
        EXPECT_EQ(a / 2, 2s);
        EXPECT_EQ(int(a / 1s), 5);
        EXPECT_EQ(int(a / 2us), 2500000);

    }

    // modulus
    {
        auto a = 1100ms;
        EXPECT_EQ(a % 1000ms, 100ms);
    }

    //EXPECT_EQ(Seconds::max(), Seconds(0x7fffffff));
}

TEST(cocoTest, Time) {
    auto t1 = TimeMilliseconds<>(0);
    auto t2 = TimeMilliseconds<>(0x70000000);
    auto t3 = TimeMilliseconds<>(0x90000000);

    // test comparison on number circle
    EXPECT_TRUE(t1 < t2);
    EXPECT_TRUE(t2 < t3);
    EXPECT_TRUE(t3 < t1);

    EXPECT_EQ(t2 - t1, Milliseconds<>(0x70000000));
    EXPECT_EQ(t2 - t1, Milliseconds<>(t2.value));
    EXPECT_EQ(*(t2 - t1), t2);
}

TEST(cocoTest, Frequency) {
    Hertz<> defaultIinitialized;
    //EXPECT_EQ(defaultIinitialized, 0Hz); // is not zero-initialized

    // cast
    {
        auto a = 3kHz;
        EXPECT_FLOAT_EQ(int(a), 3000);
        EXPECT_FLOAT_EQ(float(a), 3000.0f);
    }

    // comparison
    {
        EXPECT_TRUE(5kHz == 5000Hz);
        EXPECT_TRUE(5MHz == 5000kHz);
        EXPECT_TRUE(5GHz == 5000MHz);
        EXPECT_TRUE(5kHz > 5Hz);
        EXPECT_TRUE(5MHz < 5GHz);
    }

    // addition/subtraction
    {
        auto a = 4MHz;
        auto b = 1MHz;
        auto c = 100kHz;
        EXPECT_EQ(a + b, 5MHz);
        EXPECT_EQ(a - b, 3MHz);
        EXPECT_EQ(b + c, 1100kHz);
        EXPECT_EQ(a - c, 3900kHz);
        a += b;
        EXPECT_EQ(a, 5MHz);
        a -= b;
        EXPECT_EQ(a, 4MHz);
    }

    // multiplication
    {
        auto a = 4MHz;
        EXPECT_EQ(a * 2, 8MHz);
        EXPECT_EQ(2 * a, 8MHz);
        a *= 3;
        EXPECT_EQ(a, 12MHz);
    }

    // division
    {
        auto a = 5MHz;
        EXPECT_EQ(a / 2, 2MHz);
        EXPECT_EQ(int(a / 1MHz), 5);
        EXPECT_EQ(int(a / 1000kHz), 5);
        EXPECT_EQ(int(5000MHz / 2GHz), 2);
        a /= 2;
        EXPECT_EQ(a, 2MHz);
    }

    // modulus
    {
        auto a = 15MHz;
        EXPECT_EQ(a % 10MHz, 5MHz);
        EXPECT_EQ(a % 10000kHz, 5MHz);
    }

    // test construction from other base type
    Milliseconds<uint8_t> x(1ms);
    EXPECT_EQ(x.value, 1);

    // test bitrate calculation
    Kilohertz<> clock = 160MHz;
    Nanoseconds<> bitTime = 1040ns;
    Nanoseconds<> resetTime = 50us;
    int bitRate = int32_t(clock * bitTime);
    EXPECT_FLOAT_EQ(bitRate, 166);
    int resetCount = int(resetTime / bitTime);
    EXPECT_FLOAT_EQ(resetCount, 48);

    // test implicit int cast when units cancel out
    int i = 10Hz * 10s;
    EXPECT_EQ(i, 100);
}

namespace nordic {
    // https://devzone.nordicsemi.com/f/nordic-q-a/391/uart-baudrate-register-values
    // https://docs.nordicsemi.com/bundle/ps_nrf52840/page/uart.html#ariaid-title34
    uint32_t calcBaudRateRegister(Hertz<> baudRate) {
        // baud rate in Hz (register value = baudRate * 2^32 / 16000000)
        int brr = (int64_t(baudRate.value) << 32) / 16000000;
        return (brr + 0x800) & 0xFFFFF000;
    }
    uint32_t calcBaudRateRegister(Nanoseconds<> bitDuration) {
        // bit duration in nanoseconds (register value = (1e9/bitDuration) * 2^32 / 16000000)
        int brr = INT64_C(268435456000) / int(bitDuration.value);
        return (brr + 0x800) & 0xFFFFF000;
    }
}

namespace st {
    uint32_t calcBaudRateRegister(Hertz<> clock, Hertz<> baudRate, bool over8) {
        // baud rate in Hz (register value = clock / baudRate)
        uint32_t brr = (int(clock) + (baudRate.value >> 1)) / baudRate.value;
        if (over8)
            brr = ((brr & ~7) << 1) | (brr & 7);
        return brr;
    }
    uint32_t calcBaudRateRegister(Hertz<> clock, Nanoseconds<> bitDuration, bool over8) {
        // bit duration in nanoseconds (register value = clock / (1e9/bitDuration) = clock * bitDuration / 1e9)
        uint32_t brr = (int64_t(clock) * bitDuration.value + 500000000) / 1000000000;
        if (over8)
            brr = ((brr & ~7) << 1) | (brr & 7);
        return brr;
    }

    uint32_t calcBaudRateRegisterLp(Hertz<> clock, Hertz<> baudRate) {
        // baud rate in Hz (register value = 256 * clock / baudRate)
        uint32_t brr = (int64_t(clock) * 256 + (baudRate.value >> 1)) / baudRate.value;
        return brr;
    }
    uint32_t calcBaudRateRegisterLp(Hertz<> clock, Nanoseconds<> bitDuration) {
        // baud duration in nanoseconds (register value = 256 * clock / (1e9/bitDuration) = 256 * clock * bitDuration / 1e9)
        uint32_t brr = (int64_t(clock) * 256 * bitDuration.value + 500000000) / 1000000000;
        return brr;
    }
}

TEST(cocoTest, BaudRate) {
    // baud rate calculation for Nordic
    {
        auto br1 = nordic::calcBaudRateRegister(38400Hz);
        //std::cout << "0x" << std::hex << br1 << std::endl;
        EXPECT_EQ(br1, 0x009D5000);
        EXPECT_EQ(nordic::calcBaudRateRegister(115200Hz), 0x01D7E000);
        EXPECT_EQ(nordic::calcBaudRateRegister(1MHz), 0x10000000);

        // use duration
        EXPECT_EQ(nordic::calcBaudRateRegister(1000ns), 0x10000000);
        EXPECT_EQ(nordic::calcBaudRateRegister(1us), 0x10000000);
    }

    // baud rate calculation for ST (STM32G4 reference manual 40.5.7)
    {
        auto br1 = st::calcBaudRateRegister(8MHz, 9600Hz, false);
        //std::cout << "0x" << std::hex << br1 << std::endl;
        EXPECT_EQ(br1, 0x341);
        EXPECT_EQ(st::calcBaudRateRegister(8MHz, 9600Hz, true), 0x681);
        EXPECT_EQ(st::calcBaudRateRegister(48MHz, 921600Hz, false), 0x34);
        EXPECT_EQ(st::calcBaudRateRegister(48MHz, 921600Hz, true), 0x64);

        // use duration
        EXPECT_EQ(st::calcBaudRateRegister(48MHz, 1085ns, false), 0x34);
        EXPECT_EQ(st::calcBaudRateRegister(48MHz, 1085ns, true), 0x64);
    }

    // baud rate calculation for ST low power (STM32G4 reference manual 41.4.7)
    {
        auto br1 = st::calcBaudRateRegisterLp(32768Hz, 300Hz);
        //std::cout << "0x" << std::hex << br1 << std::endl;
        EXPECT_EQ(br1, 0x6D3A);
        EXPECT_EQ(st::calcBaudRateRegisterLp(32768Hz, 9600Hz), 0x36A); // is closer than 0x369

        // use duration
        EXPECT_EQ(st::calcBaudRateRegisterLp(32768Hz, 3333333ns), 0x6D3A);
        EXPECT_EQ(st::calcBaudRateRegisterLp(32768Hz, 104167ns), 0x36A);
    }
}


// Vector2, Vector3, Vector4
// -------------------------

TEST(cocoTest, Vector2) {
    {
        int2 a = {1, 2};
        int2 b = {2, 1};
        int s = 1;

        +a;
        -a;

        s + b;
        a + s;
        a + b;

        s - b;
        a - s;
        a - b;

        s * b;
        a * s;
        a * b;

        a / s;
        a / b;

        StringBuffer<100> buffer;
        buffer << hex(a.x, 2) << ' ';
        buffer << vector(a, [] (auto &s, int c) {s << "0x" << hex(c, 2);}, ", ");
        EXPECT_EQ(buffer.string(), "01 0x01, 0x02");
    }
    {
        float2 a = {1.0f, 2.0f};
        float2 b = {2.0f, 1.0f};
        float s = 1.0f;

        +a;
        -a;

        s + b;
        a + s;
        a + b;

        s - b;
        a - s;
        a - b;

        s * b;
        a * s;
        a * b;

        a / s;
        a / b;
    }
}

TEST(cocoTest, Vector3) {
    {
        int3 a = {1, 2, 3};
        int3 b = {3, 2, 1};
        int s = 1;

        +a;
        -a;

        s + b;
        a + s;
        a + b;

        s - b;
        a - s;
        a - b;

        s * b;
        a * s;
        a * b;

        a / s;
        a / b;

        StringBuffer<100> buffer;
        buffer << hex(a.x, 2) << ' ';
        buffer << vector(a, [] (auto &s, int c) {s << "0x" << hex(c, 2);}, ", ");
        EXPECT_EQ(buffer.string(), "01 0x01, 0x02, 0x03");

        // also works with std::ostream
        //std::cout << vector(a, [] (auto &s, int c) {s << "0x" << hex(c, 2);}, ", ");
    }
    {
        float3 a = {1.0f, 2.0f, 3.0f};
        float3 b = {3.0f, 2.0f, 1.0f};
        float s = 1.0f;

        +a;
        -a;

        s + b;
        a + s;
        a + b;

        s - b;
        a - s;
        a - b;

        s * b;
        a * s;
        a * b;

        a / s;
        a / b;
    }
}

TEST(cocoTest, Vector4) {
    {
        int4 a = {1, 2, 3, 4};
        int4 b = {4, 3, 2, 1};
        int s = 1.0f;

        +a;
        -a;

        s + b;
        a + s;
        a + b;

        s - b;
        a - s;
        a - b;

        s * b;
        a * s;
        a * b;

        a / s;
        a / b;

        StringBuffer<100> buffer;
        buffer << hex(a.x, 2) << ' ';
        buffer << vector(a, [] (auto &s, int c) {s << "0x" << hex(c, 2);}, ", ");
        EXPECT_EQ(buffer.string(), "01 0x01, 0x02, 0x03, 0x04");
    }
    {
        float4 a = {1.0f, 2.0f, 3.0f, 4.0f};
        float4 b = {4.0f, 3.0f, 2.0f, 1.0f};
        float s = 1.0f;

        +a;
        -a;

        s + b;
        a + s;
        a + b;

        s - b;
        a - s;
        a - b;

        s * b;
        a * s;
        a * b;

        a / s;
        a / b;
    }
}




// File
// ----
/*
TEST(cocoTest, File) {
    File f("foo.txt", File::Mode::READ_WRITE | File::Mode::TRUNCATE);
    char str[4];

    EXPECT_EQ(f.write(0, "foo", 3), 3);
    EXPECT_EQ(f.read(0, str, 3), 3);
    str[3] = 0;
    EXPECT_STREQ(str, "foo");

    EXPECT_EQ(f.write(3, "bar", 3), 3);
    EXPECT_EQ(f.read(3, str, 3), 3);
    str[3] = 0;
    EXPECT_STREQ(str, "bar");
}*/


int main(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);
    int success = RUN_ALL_TESTS();
    return success;
}
