#include <gtest/gtest.h>
#include <coco/Task.hpp>
#include <coco/TimedTask.hpp>
#include <coco/Callback.hpp>


using namespace coco;



void f1() {
	std::cout << "f1" << std::endl;
}

void f2() {
	std::cout << "f2" << std::endl;
}

void f0() {
	std::cout << "f0" << std::endl;
}


// Task

TEST(cocoTest, Task) {
	Task<std::function<void ()>> t1(f1);
	TaskWithParameters<std::function<void ()>, int> t2(f1);

	// test if getParameters can distinguish between Task and TaskWithParameters
	getParameters(t1).remove();
	getParameters(t2) = 5;
}


// TaskList

TEST(cocoTest, TaskList) {
	// create tasks
	Task<std::function<void ()>> task1(f1);
	Task<std::function<void ()>> task2(f2);
	Task<std::function<void ()>> task0(f0);

	// create task list
	TaskList<Task<std::function<void ()>>> taskList1;

	// doAll() on empty list
	taskList1.doAll();

}


// TimedTask

//using MTime = Time<MilliSeconds>;

TEST(cocoTest, TimedTask) {
	TimedTask<std::function<void ()>> t1(f1);
	TimedTask<std::function<void ()>> t2(f1, *1s);

	// test if getParameters can distinguish between Task and TaskWithParameters
	getParameters(t1).remove();
}


// TimedTaskList

TEST(cocoTest, TimedTaskList) {
	// create tasks
	TimedTask<std::function<void ()>> task1(f1, *1s);
	TimedTask<std::function<void ()>> task2(f2, *2s);
	TimedTask<std::function<void ()>> task0(f0, *0s);

	// create task list
	TimedTaskList<std::function<void ()>> taskList1;

	// doUntil() on empty list
	taskList1.doUntil(*0s);

	// add tasks
	EXPECT_TRUE(taskList1.empty());
	taskList1.add(task1);
	EXPECT_EQ(taskList1.getFirstTime(), *1s);
	taskList1.add(task2);
	EXPECT_EQ(taskList1.getFirstTime(), *1s);
	taskList1.add(task0);
	EXPECT_EQ(taskList1.getFirstTime(), *0s);
	EXPECT_FALSE(taskList1.empty());

	// visit all tasks
	taskList1.visitAll([](TimedTask<std::function<void ()>> &task) {
		std::cout << task.time.value << std::endl;
	});

	taskList1.doUntil(*1s);
}


class Foo {
public:
	void bar1() {
		std::cout << "bar1" << std::endl;
	}
	void bar2() {
		std::cout << "bar2" << std::endl;
	}
	void bar0() {
		std::cout << "bar0" << std::endl;
	}
};

TEST(cocoTest, TimedTaskList2) {
	Foo foo;

	// create callbacks
	Callback c1 = makeCallback<Foo, &Foo::bar1>(&foo);
	Callback c2 = makeCallback<Foo, &Foo::bar2>(&foo);
	Callback c0 = makeCallback<Foo, &Foo::bar0>(&foo);
	//c1();

	// create tasks
	TimedTask<Callback> task1(c1, *1s);
	TimedTask<Callback> task2(c2, *2s);
	TimedTask<Callback> task0(c0, *0s);

	// create task list
	TimedTaskList<Callback> taskList1;

	// doUntil() on empty list
	taskList1.doUntil(*0ms);

	// add tasks
	EXPECT_TRUE(taskList1.empty());
	EXPECT_EQ(taskList1.getFirstTime(*500ms), *500ms);
	taskList1.add(task1);
	EXPECT_EQ(taskList1.getFirstTime(), *1s);
	EXPECT_EQ(taskList1.getFirstTime(*500ms), *500ms);
	taskList1.add(task2);
	EXPECT_EQ(taskList1.getFirstTime(), *1s);
	EXPECT_EQ(taskList1.getFirstTime(*500ms), *500ms);
	taskList1.add(task0);
	EXPECT_EQ(taskList1.getFirstTime(), *0s);
	EXPECT_EQ(taskList1.getFirstTime(*500ms), *0s);
	EXPECT_FALSE(taskList1.empty());

	// visit all tasks
	taskList1.visitAll([](TimedTask<Callback> &task) {
		std::cout << task.time.value << std::endl;
	});

	// doUntil() on time before first time
	taskList1.doUntil(*-1s);

	taskList1.doUntil(*1s);
}
