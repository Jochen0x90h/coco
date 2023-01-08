#include <gtest/gtest.h>
#include <coco/Array.hpp>
#include <coco/Buffer.hpp>
#include <coco/convert.hpp>
#include <coco/enum.hpp>
#include <coco/IsSubclass.hpp>
#include <coco/LinkedList.hpp>
#include <coco/StreamOperators.hpp>
#include <coco/String.hpp>
#include <coco/StringBuffer.hpp>
#include <coco/Time.hpp>


using namespace coco;

// test data
constexpr String strings[] = {"a", "bar", "bar2", "foo", "foo2", "foobar", "foobar2", "z"}; 


// Array
// -----

template <typename T>
int byteSize(const T &array) {
	return std::size(array) * sizeof(*std::data(array));
}


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


// Buffer
// ------

TEST(cocoTest, Buffer) {
	Buffer<int, 7> b;
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
	Buffer<int, 10> b2;
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
	const Buffer<int, 7> &cb = b;
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


// convert
// -------

TEST(cocoTest, convert) {
	// int
	int i = *parseInt("-50");
	EXPECT_EQ(i, -50);
	auto i2 = parseInt("foo");
	EXPECT_EQ(i2, std::nullopt);

	// float
	float f = *parseFloat("50.99");
	EXPECT_EQ(f, 50.99f);
	auto f2 = parseFloat("bar");
	EXPECT_EQ(f2, std::nullopt);
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


// IsArray
// ----------

using ArrayType = int[10];

TEST(cocoTest, IsArray) {
	EXPECT_TRUE((IsArray<ArrayType>::value));
	EXPECT_FALSE((IsArray<ArrayType &>::value));
	EXPECT_FALSE((IsArray<ArrayType *>::value));
	EXPECT_FALSE((IsArray<int>::value));
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


// LinkedListNode
// --------------

struct ElementBaseClass {
	int i;
};

struct MyListElement : public ElementBaseClass, public LinkedListNode {
	int foo;

	MyListElement(int foo) : foo(foo) {}
	MyListElement(LinkedList<MyListElement> &list, int foo) : LinkedListNode(list), foo(foo) {}
};

using MyList = LinkedList<MyListElement>;

TEST(cocoTest, LinkedListNode) {
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


// String
// ------

TEST(cocoTest, String) {
	// constructor from c-string
	{
		String foo("foo");
		EXPECT_TRUE(foo == "foo");
		EXPECT_FALSE(foo != "foo");
		EXPECT_EQ(foo, "foo");
		EXPECT_EQ(foo.size(), 3);

		char const *cstr = "bar";
		EXPECT_EQ(length(cstr), 3);
		String bar(cstr);
		EXPECT_EQ(bar, "bar");
		EXPECT_EQ(bar.size(), 3);
		
		// from r-value
		String bar2(reinterpret_cast<char const *>(cstr));
		EXPECT_EQ(bar2, "bar");
		EXPECT_EQ(bar2.size(), 3);
	}
	
	// constructor from c-array
	{
		char ar[] = {'f', 'o', 'o'};
		char dummy = 'x';
		EXPECT_EQ(length(ar), 3);
		String foo(ar);
		EXPECT_EQ(foo, "foo");
		EXPECT_EQ(foo.size(), 3);
	}

	// less operator
	for (int j = 0; j < std::size(strings); ++j) {
		for (int i = 0; i < std::size(strings); ++i) {
			EXPECT_EQ(i < j, strings[i] < strings[j]);
		}
	}
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


// Time
// ----
TEST(systemTest, Time) {
	Time t1 = {0};
	Time t2 = {0x70000000};
	Time t3 = {0x90000000};

	EXPECT_TRUE(t1 < t2);
	EXPECT_TRUE(t2 < t3);
	EXPECT_TRUE(t3 < t1);
}

int main(int argc, char **argv) {
	testing::InitGoogleTest(&argc, argv);
	int success = RUN_ALL_TESTS();	
	return success;
}
