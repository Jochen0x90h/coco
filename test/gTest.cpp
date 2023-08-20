#include <gtest/gtest.h>
#include <coco/align.hpp>
#include <coco/Array.hpp>
#include <coco/ArrayBuffer.hpp>
#include <coco/ArrayConcept.hpp>
#include <coco/convert.hpp>
#include <coco/CStringConcept.hpp>
#include <coco/enum.hpp>
#include <coco/Frequency.hpp>
#include <coco/IsSubclass.hpp>
#include <coco/LinkedList.hpp>
#include <coco/Queue.hpp>
#include <coco/PointerConcept.hpp>
#include <coco/PseudoRandom.hpp>
#include <coco/StreamOperators.hpp>
#include <coco/String.hpp>
#include <coco/StringBuffer.hpp>
#include <coco/StringConcept.hpp>
#include <coco/Time.hpp>
//#include <coco/platform/File.hpp>


using namespace coco;

// test data
constexpr String strings[] = {"a", "bar", "bar2", "foo", "foo2", "foobar", "foobar2", "z"};

// util
template <typename T>
int byteSize(const T &array) {
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
	b.append(50);
	EXPECT_FALSE(b.empty());
	EXPECT_EQ(b.size(), 1);
	EXPECT_EQ(std::size(b), 1);
	EXPECT_EQ(b[0], 50);
	EXPECT_EQ(b.data()[0], 50);
	EXPECT_EQ(std::data(b)[0], 50);

	// test overflow
	for (int i = 0; i < 10; ++i) {
		b.append(10);
	}
	EXPECT_FALSE(b.empty());
	EXPECT_EQ(b.size(), 7);

	// clear
	b.clear();
	EXPECT_TRUE(b.empty());
	EXPECT_EQ(b.size(), 0);

	// append array
	int a1[] = {10, 11, 12};
	b.append(a1);
	EXPECT_EQ(b.size(), 3);
	Array<int> b1(a1);
	b.append(b1);
	EXPECT_EQ(b.size(), 6);

	// append buffer
	ArrayBuffer<int, 10> b2;
	b2.append(1337);
	b2.append(0);
	b.append(b2);
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

template <typename T> requires (!ArrayConcept<T>)
bool testArrayConcept(T const &x) {return false;}

TEST(cocoTest, ArrayConcept) {
	int a1[] = {1, 2, 3};
	const int a2[] = {1, 2, 3};
	volatile int a3[] = {1, 2, 3};
	Array<int> a4(a1);
	Array<int, 3> a5(a1);
	ArrayBuffer<int, 10> b1;
	String s1 = "foo";
	int i1 = 0;
	const int i2 = 0;
	int *p1 = nullptr;

	EXPECT_TRUE(testArrayConcept(a1));
	EXPECT_TRUE(testArrayConcept(a2));
	EXPECT_TRUE(testArrayConcept(a3));
	EXPECT_TRUE(testArrayConcept(a4));
	EXPECT_TRUE(testArrayConcept(a5));
	EXPECT_TRUE(testArrayConcept(b1));
	EXPECT_TRUE(testArrayConcept(s1));
	EXPECT_FALSE(testArrayConcept(i1));
	EXPECT_FALSE(testArrayConcept(i2));
	EXPECT_FALSE(testArrayConcept(p1));
}


// convert
// -------

TEST(cocoTest, convert) {
	// parse int
	int i = *parseInt("-50");
	EXPECT_EQ(i, -50);
	auto i2 = parseInt("foo");
	EXPECT_EQ(i2, std::nullopt);

	// parse float
	float f = *parseFloat("50.99");
	EXPECT_EQ(f, 50.99f);
	auto f2 = parseFloat("bar");
	EXPECT_EQ(f2, std::nullopt);

	char buffer16[16];

	// int to string
	EXPECT_EQ(toString(buffer16, INT64_C(12345678900)), "12345678900");
	EXPECT_EQ(toString(buffer16, INT64_C(-12345678900)), "-12345678900");

	// hex to string
	EXPECT_EQ(toHexString(buffer16, INT64_C(0x123456789AB), 11), String("123456789ab"));

	char buffer21[21];
	EXPECT_EQ(toString(buffer21, 5.5f, 1, 2), "5.5");
	EXPECT_EQ(toString(buffer21, 5.5f, 2, -2), "05.50");
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
	EXPECT_FALSE(testCStringConcept(i1));

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


// Frequency
// ------

TEST(cocoTest, Frequency) {
	constexpr auto f = 5MHz;

	constexpr Frequency g = f * 2;
	constexpr Frequency h = f / 2;
	constexpr int i = f / 2Hz;

	EXPECT_EQ(g, 10MHz);
	EXPECT_EQ(h, 2500kHz);
	EXPECT_EQ(i, 2500000);
}


/*
// IsArray
// ----------

using CharArrayType = char[10];
using IntArrayType = int[10];

TEST(cocoTest, IsArray) {
	EXPECT_TRUE((IsArray<char[10]>::value));
	EXPECT_TRUE((IsArray<int[10]>::value));
	EXPECT_FALSE((IsArray<IntArrayType &>::value));
	EXPECT_FALSE((IsArray<IntArrayType *>::value));
	EXPECT_FALSE((IsArray<int>::value));
}


// IsCString
// ----------

TEST(cocoTest, IsCString) {
	EXPECT_TRUE((IsCString<const char *>::value));
	EXPECT_TRUE((IsCString<char[10]>::value));
	EXPECT_FALSE((IsCString<IntArrayType>::value));
	EXPECT_FALSE((IsArray<CharArrayType &>::value));
	EXPECT_FALSE((IsArray<CharArrayType *>::value));
	EXPECT_FALSE((IsArray<char>::value));
}
*/

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


// LinkedListNode
// --------------

struct ElementBaseClass {
	int value = 10;
};

struct MyListElement : public ElementBaseClass, public LinkedListNode {
	int foo;

	MyListElement(int foo) : foo(foo) {}
	MyListElement(LinkedList<MyListElement> &list, int foo) : LinkedListNode(list), foo(foo) {}
};

using MyList = LinkedList<MyListElement>;

TEST(cocoTest, LinkedList) {
	// create list
	MyList list;
	EXPECT_TRUE(list.empty());
	EXPECT_EQ(list.count(), 0);

	// create an element
	MyListElement element(10);
	EXPECT_FALSE(element.inList());

	// add element to list
	list.add(element);
	EXPECT_FALSE(list.empty());
	EXPECT_EQ(list.count(), 1);
	EXPECT_TRUE(element.inList());

	// create another element and add to list in constructor
	MyListElement element2(list, 50);
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

// additionally inherit from LinkedListNode2
struct MyListElement2 : public LinkedListNode2 {
	int value = 50;
};

using MyList2 = LinkedList2<MyListElement2>;

TEST(cocoTest, LinkedList2) {
	// create an element
	MyListElement2 element;

	// create list
	MyList2 list;
	list.add(element);

	// iterate over list
	for (MyListElement2 &element : list) {
		EXPECT_EQ(element.value, 50);
	}
}

// inherit from both LinkedListNode and LinkedListNode2
struct MyListElement12 : public MyListElement, public LinkedListNode2 {
	MyListElement12(int foo) : MyListElement(foo) {}
};

using MyList12 = LinkedList<MyListElement12>;
using MyList22 = LinkedList2<MyListElement12>;

TEST(cocoTest, LinkedList22) {
	// create an element
	MyListElement12 element(50);

	// create list 1
	MyList12 list;
	list.add(element);

	// create list 2
	MyList22 list2;
	list2.add(element);

	// iterate over list 1
	for (MyListElement12 &element : list) {
		EXPECT_EQ(element.value, 10);
		EXPECT_EQ(element.foo, 50);
	}

	// iterate over list 2
	for (MyListElement12 &element : list2) {
		EXPECT_EQ(element.value, 10);
		EXPECT_EQ(element.foo, 50);
	}
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

TEST(cocoTest, String) {
	// construct from c-string
	{
		// from c-string array
		String foo("foo");
		EXPECT_TRUE(foo == "foo");
		EXPECT_FALSE(foo != "foo");
		EXPECT_EQ(foo, "foo");
		EXPECT_EQ(foo.size(), 3);

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

		// todo String utf8(u8"äöü");
	}

	// construct from c-array
	{
		char ar[] = {'f', 'o', 'o'};
		char dummy = 'x';
		String foo(ar);
		EXPECT_EQ(foo, "foo");
		EXPECT_EQ(foo.size(), 3);
	}

	// construct from data
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
}


// StringBuffer
// ------------

TEST(cocoTest, StringBuffer) {
	const char *space = " ";

	StringBuffer<100> b;
	EXPECT_TRUE(b.empty());
	EXPECT_EQ(b.size(), 0);
	EXPECT_EQ(std::size(b), 0);
	EXPECT_EQ(b.MAX_SIZE, 100);
	EXPECT_EQ(b.capacity(), 100);

	// check if append omits the zero termination
	b.append("foo");
	EXPECT_EQ(b.size(), 3);
	b.clear();

	b << dec(123456) << ' ';
	b << dec(-99, 3) << " ";
	b << hex(0x5, 1) << space;
	b << hex(0xabcdef12) << " ";
	b << flt(0.0f) << " ";
	b << flt(0.0f, 0, 2) << " ";
	b << flt(0.001234567f, 9) << " ";
	b << flt(0.5f) << ' ';
	b << flt(0.5f, 0, 2) << " ";
	b << flt(1.0f, 1) << ' ';
	b << flt(1.0f, -1) << " ";
	b << flt(-5.9f) << " ";
	b << flt(100.9999f) << " ";
	b << flt(2000000000);
	EXPECT_EQ(b, "123456 -099 5 abcdef12 0 0 0.001234567 0.5 .5 1 1.0 -5.9 101 2000000000");
	EXPECT_STREQ(b.c_str(), "123456 -099 5 abcdef12 0 0 0.001234567 0.5 .5 1 1.0 -5.9 101 2000000000");

	StringBuffer<5> c;
	c << flt(0.000000001f, 9);
	EXPECT_EQ(c, "0.000");
	EXPECT_FALSE(c.empty());
	EXPECT_EQ(c.size(), 5);
	EXPECT_EQ(std::size(c), 5);
	EXPECT_EQ(c.MAX_SIZE, 5);
	EXPECT_EQ(c.capacity(), 5);
	for (int i = 0; i < 5; ++i) {
		EXPECT_EQ(c[i], "0.000"[i]);
		EXPECT_EQ(c.data()[i], "0.000"[i]);
		EXPECT_EQ(std::data(c)[i], "0.000"[i]);
	}

	// not equal is implemented automatically
	EXPECT_NE(b, c);

	// byte size
	EXPECT_EQ(byteSize(b), 71);
	EXPECT_EQ(byteSize(c), 5);
}


// StringConcept
// --------------

template <typename T> requires (StringConcept<T>)
bool testStringConcept(T const &x) {return true;}

template <typename T> requires (!StringConcept<T>)
bool testStringConcept(T const &x) {return false;}

TEST(cocoTest, StringConcept) {
	const char s1[] = "foo";
	const char *s2 = s1;
	char s3[] = {'f', 'o', 'o'};
	char dummy = 'x';
	char s4[] = {'f', 'o', 'o', 0};
	char *s5 = s4;
	String s6(s1);
	int i1 = 0;

	EXPECT_TRUE(testStringConcept(s1));
	EXPECT_TRUE(testStringConcept(s2));
	EXPECT_TRUE(testStringConcept(s3));
	EXPECT_TRUE(testStringConcept(s4));
	EXPECT_TRUE(testStringConcept(s5));
	EXPECT_TRUE(testStringConcept(s6));
	EXPECT_FALSE(testStringConcept(i1));
}


// Time
// ----

TEST(cocoTest, Time) {
	Time t1 = {0};
	Time t2 = {0x70000000};
	Time t3 = {0x90000000};

	EXPECT_TRUE(t1 < t2);
	EXPECT_TRUE(t2 < t3);
	EXPECT_TRUE(t3 < t1);
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
