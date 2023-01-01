#include <gtest/gtest.h>
#include <coco/Array.hpp>
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

// array with fixed size
TEST(cocoTest, ArrayN) {
	int a1[] = {10, 11, 12};
	int const a2[] = {20, 21, 22};
	
	// construct arrays from c-arrays
	Array<int, 3> b1(a1);
	Array<int const, 3> b2(a2);
	Array<char const, 4> str("foo");
		
	EXPECT_EQ(b1[1], a1[1]);
	EXPECT_EQ(b2[1], a2[1]);
	EXPECT_EQ(str[1], 'o');

	EXPECT_FALSE(b1.isEmpty());
	EXPECT_FALSE(b2.isEmpty());
	EXPECT_EQ(b1.count(), 3);
	EXPECT_EQ(b2.count(), 3);
	
	// construct const array from non-const array
	Array<int const, 3> b3(b1);
	EXPECT_EQ(b3.count(), 3);

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
}

// array with variable size
TEST(cocoTest, Array) {
	int a1[] = {10, 11, 12};
	int const a2[] = {20, 21, 22};
	
	// construct arrays from c-arrays
	Array<int> b1(a1);
	Array<int const> b2(a2);
	Array<char const> str("foo");
	
	EXPECT_EQ(b1[1], a1[1]);
	EXPECT_EQ(b2[1], a2[1]);
	EXPECT_EQ(str[1], 'o');

	EXPECT_FALSE(b1.empty());
	EXPECT_FALSE(b2.empty());
	EXPECT_EQ(b1.size(), 3);
	EXPECT_EQ(b2.size(), 3);

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
	EXPECT_TRUE(list.isEmpty());
	EXPECT_EQ(list.count(), 0);

	// create an element
	MyListElement element(10);
	EXPECT_FALSE(element.isInList());

	// add element to list
	list.add(element);
	EXPECT_FALSE(list.isEmpty());
	EXPECT_EQ(list.count(), 1);
	EXPECT_TRUE(element.isInList());

	// create another element and add to list in constructor
	MyListElement element2(list, 50);
	EXPECT_FALSE(list.isEmpty());
	EXPECT_EQ(list.count(), 2);
	EXPECT_TRUE(element.isInList());

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
	EXPECT_FALSE(element.isInList());
	EXPECT_TRUE(element2.isInList());

	element2.remove();
	EXPECT_EQ(list.count(), 0);
	EXPECT_FALSE(element2.isInList());

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
	EXPECT_EQ(b.string(), "123456 -099 5 abcdef12 0 0 0.001234567 0.5 .5 1 1.0 -5.9 101 2000000000");

	StringBuffer<5> c = flt(0.000000001f, 9);
	EXPECT_EQ(c.string(), "0.000");

	//StringBuffer<32> d = 'x' + hex(0xff) + String("foo") + dec(50) + '-';
	//EXPECT_EQ(d.string(), "x000000fffoo50-");*/
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
