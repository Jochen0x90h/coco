#include <gtest/gtest.h>
#include <coco/Coroutine.hpp>
#include <coco/String.hpp>


using namespace coco;

// helper object for coroutine tests
struct Object {
	Object(char const *name) : name(name) {
		std::cout << "enter " << name << std::endl;
	}
	~Object() {
		std::cout << "exit " << name << std::endl;
	}
	char const *name;
};


// wait lists
CoroutineTaskList<> taskList1;
CoroutineTaskList<> taskList2;

// wait functions
Awaitable<> wait1() {
	return {taskList1};
}
Awaitable<> wait2() {
	return {taskList2};
}

TEST(cocoTest, Awaitable) {
	{
		auto awaitable = wait1();
		// awaitable goes out of scope and therefore cancel() gets called
	}

	{
		auto awaitable = wait1();

		// execute task when no coroutine is waiting (task should be noop_coroutine_handle)
		taskList1.doAll();
	}
}

Coroutine coroutine() {
	// an object whose destructor gets called when the coroutine ends or gets cancelled
	Object o("coroutine()");

	std::cout << "wait1" << std::endl;
	co_await wait1();
	std::cout << "wait2" << std::endl;
	co_await wait2();
}

TEST(cocoTest, Coroutine) {
	// start coroutine
	coroutine();

	// resume coroutine
	std::cout << "resume wait1" << std::endl;
	EXPECT_FALSE(taskList1.empty());
	taskList1.doAll();
	EXPECT_TRUE(taskList1.empty());
	std::cout << "resume wait2" << std::endl;
	EXPECT_FALSE(taskList2.empty());
	taskList2.doAll();
	EXPECT_TRUE(taskList2.empty());
}

TEST(cocoTest, DestroyCoroutine) {
	// start coroutine
	Coroutine c = coroutine();

	// destroy coroutine
	c.destroy();
}

AwaitableCoroutine inner() {
	Object o("inner()");

	std::cout << "inner wait1" << std::endl;
	co_await wait1();
	std::cout << "inner wait2" << std::endl;
	co_await wait2();
}

int selectResult = 0;
Coroutine outer() {
	Object o("outer()");

	co_await inner();

	std::cout << "!outer select" << std::endl;
	int result = co_await select(wait1(), wait2());
	selectResult = result;
	switch (result) {
	case 1:
		std::cout << "selected 1" << std::endl;
		break;
	case 2:
		std::cout << "selected 2" << std::endl;
		break;
	}
}

TEST(cocoTest, NestedCoroutine) {
	// start outer coroutine
	outer();

	// resume inner coroutine
	std::cout << "!resume inner wait1" << std::endl;
	taskList1.doAll();
	std::cout << "!resume inner wait2" << std::endl;
	taskList2.doAll();

	std::cout << "!resume outer wait2" << std::endl;
	EXPECT_FALSE(taskList2.empty());
	taskList2.doAll();
	EXPECT_TRUE(taskList2.empty());
	EXPECT_EQ(selectResult, 2);
}

TEST(cocoTest, DestroyNestedCoroutine) {
	// start outer coroutine (enters inner() and waits on wait1())
	Coroutine c = outer();
	//Coroutine c = coroutine2();

	// destroy outer coroutine
	std::cout << "!destroy outer" << std::endl;
	c.destroy();
}

TEST(cocoTest, AwaitableCoroutine) {
	AwaitableCoroutine c = inner();

	// check that the coroutine is alive
	EXPECT_TRUE(c.isAlive());
	EXPECT_FALSE(c.hasFinished());
	EXPECT_FALSE(c.await_ready());

	// move assign to c2
	AwaitableCoroutine c2;
	c2 = std::move(c);

	// check that the c now reports ready
	EXPECT_TRUE(c.await_ready());

	// check that the coroutine is still running
	EXPECT_FALSE(c2.await_ready());

	// resume the coroutine
	taskList1.doAll();
	taskList2.doAll();

	// check that the coroutine has stopped
	EXPECT_TRUE(c2.await_ready());

	// should have no effect
	c.cancel();


	// start again and assign to finished c2
	c2 = inner();

	// start again and assign to c2 when it is still alive
	c2 = inner();

	// cancel the coroutine
	c2.cancel();

	// check that the coroutine has stopped
	EXPECT_TRUE(c2.await_ready());
}



// Coroutine with parameters
// -------------------------

class Parameters1 : public CoroutineTask {
public:
	// default constructor
	Parameters1(std::coroutine_handle<> task, int value) : CoroutineTask(task), value(value) {}

	// overload cancel e.g. to lock the task list against concurrent modification
	void cancel() {
		std::cout << "Parameters1::cancel" << std::endl;
		remove();
	}

	int value;
};

class MyTaskList : public TaskList<Parameters1> {
public:
	// overload add e.g. to lock the waitlist against concurrent modification
	void add(Parameters1 &node) {
		std::cout << "MyTaskList::add" << std::endl;
		TaskList<Parameters1>::add(node);
	}
};
MyTaskList myTaskList;

Awaitable<Parameters1> waitWithParams(int value) {
	return {myTaskList, value};
}

bool finished1 = false;
Coroutine waitWithParams1() {
	Object o("waitWithParams1()");

	co_await waitWithParams(5);
	std::cout << "resumed delay(5)" << std::endl;
	finished1 = true;
}

bool finished2 = false;
Coroutine waitWithParams2() {
	Object o("waitWithParams2()");
	switch (co_await select(wait1(), waitWithParams(10))) {
	case 1:
		std::cout << "resumed wait1" << std::endl;
		break;
	case 2:
		std::cout << "resumed delay(10)" << std::endl;
		break;
	}
	finished2 = true;
}

TEST(cocoTest, CoroutineValue) {
	waitWithParams1();
	waitWithParams2();

	myTaskList.doAll([] (Parameters1 const &p) {return p.value == 5;});
	EXPECT_TRUE(finished1);
	EXPECT_FALSE(finished2);
	myTaskList.doAll([] (Parameters1 const &p) {return p.value == 10;});
	EXPECT_TRUE(finished1);
	EXPECT_TRUE(finished2);
}


// Awaitable move constructor
// --------------------------

Coroutine move() {
	Awaitable<> a = wait1();
	Awaitable<> b(std::move(a));

	EXPECT_TRUE(a.hasFinished());
	co_await a;

	std::cout << "await b" << std::endl;
	co_await b;
	std::cout << "resumed from b" << std::endl;

	EXPECT_TRUE(a.hasFinished());
	EXPECT_TRUE(b.hasFinished());
}

TEST(cocoTest, AwaitableMove) {
	move();

	taskList1.doAll();
	EXPECT_TRUE(taskList1.empty());
}



// Barrier
// -------

Barrier barrier;

Coroutine waitForBarrier() {
	Object o("waitForBarrier()");
	co_await barrier.wait();
}

TEST(cocoTest, Barrier) {
	waitForBarrier();
	std::cout << "resume barrier" << std::endl;
	barrier.doAll();
}

struct BarrierParameters {
	int i;
	float f;
};
Barrier<BarrierParameters> barrierWithParameters;

Coroutine waitForBarrierWithParameters() {
	Object o("waitForBarrierWithParameters()");
	co_await barrierWithParameters.wait(1, 2.0f);
}

TEST(cocoTest, BarrierWithParameters) {
	waitForBarrierWithParameters();
	std::cout << "resume barrier" << std::endl;
	barrierWithParameters.doFirst([](BarrierParameters &p) {
		EXPECT_EQ(p.i, 1);
		return true;
	});
}

struct Resumer {
	Resumer(Barrier<> &barrier) : barrier(barrier) {}
	~Resumer() {
		std::cout << "Resumer::~Resumer()" << std::endl;
		this->barrier.doFirst();
	}
	Barrier<> &barrier;
};

Barrier<> resumeAfterReturnBarrier;
CoroutineTaskList<int> resumeAfterReturnList;

Coroutine resumedAfterReturn() {
	co_await resumeAfterReturnBarrier.wait();
	resumeAfterReturnList.doFirst([](int i) {
		std::cout << "resumed after return " << i << std::endl;
		return true;
	});
}

Awaitable<int> resumeAfterReturn(int i) {
	Resumer r(resumeAfterReturnBarrier);
	return {resumeAfterReturnList, i};
	// destructor of Resumer resumes the barrier here
}

TEST(cocoTest, ResumeBarrierAfterReturn) {
	resumedAfterReturn();
	auto awaitable = resumeAfterReturn(50);
	EXPECT_TRUE(awaitable.hasFinished());
}


// Semaphore
// ---------

Semaphore semaphore(3);

Coroutine worker1() {
	while (true) {
		co_await semaphore.wait();
		std::cout << "worker 1" << std::endl;
	}
}

Coroutine worker2() {
	while (true) {
		co_await semaphore.wait();
		std::cout << "worker 2" << std::endl;
	}
}

TEST(cocoTest, Semaphore) {
	auto c1 = worker1();
	auto c2 = worker2();
	for (int i = 0; i < 10; ++i) {
		semaphore.post();
	}

	// destroy coroutines so that no coroutine waits on the semaphore when it gets destroyed
	c1.destroy();
	c2.destroy();
}


// TimedTask
// ---------

CoroutineTimedTaskList timedTaskList;

Awaitable<CoroutineTimedTask> timedWait(Time time) {
	return {timedTaskList, time};
}

Coroutine timedCoroutine() {
	Object o("timedCoroutine()");

	std::cout << "timedWait" << std::endl;
	co_await timedWait(Time() + 1s);
}

TEST(cocoTest, CoroutineTimedTask) {
	timedCoroutine();
	EXPECT_FALSE(timedTaskList.empty());
	timedTaskList.doUntil(Time() + 1s);
	EXPECT_TRUE(timedTaskList.empty());
}
