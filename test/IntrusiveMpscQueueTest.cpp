#include <gtest/gtest.h>
#include <coco/InterruptQueue.hpp>
#include <coco/IntrusiveMpscQueue.hpp>
#include <semaphore>
#include <thread>


// test for InterruptQueue and IntrusiveMpscQueue

using namespace coco;


class Foo {
public:
    int i = 5;
};

// dummy queue element which uses multiple inheritance, i.e. casting from Buffer to AtomicNextNode needs to add an offset
class Element : public Foo, public IntrusiveMpscQueueNode {
public:
    std::binary_semaphore s{0};
};


// MpscQueue

TEST(cocoTest, IntrusiveMpscQueue_SingleThreaded) {
    IntrusiveMpscQueue<Element> queue;
    Element e1, e2, e3;

    // queue is initially empty
    EXPECT_EQ(queue.pop(), nullptr);

    // push some elements
    queue.push(e1);
    queue.push(e2);

    // pop elements and check
    EXPECT_EQ(queue.pop(), &e1);
    EXPECT_EQ(queue.pop(), &e2);
    EXPECT_EQ(queue.pop(), nullptr);
}

constexpr int COUNT = 10000;

TEST(cocoTest, IntrusiveMpscQueue_MultiThreaded) {
    IntrusiveMpscQueue<Element> queue;
    Element e1, e2, e3, e4;
    int c1 = 0, c2 = 0, c3 = 0, c4 = 0;
    std::atomic<int> finishCount;

    // multiple producers
    std::thread t1([&queue, &e1, &finishCount] {
        //std::cout << "started thread 1" << std::endl;
        for (int i = 0; i < COUNT; ++i) {
            queue.push(e1);
            e1.s.acquire();
        }
        ++finishCount;
    });
    std::thread t2([&queue, &e2, &finishCount] {
        //std::cout << "started thread 2" << std::endl;
        for (int i = 0; i < COUNT; ++i) {
            queue.push(e2);
            e2.s.acquire();
        }
        ++finishCount;
    });
    std::thread t34([&queue, &e3, &e4, &finishCount] {
        //std::cout << "started thread 3/4" << std::endl;
        for (int i = 0; i < COUNT; ++i) {
            queue.push(e3);
            queue.push(e4);
            e3.s.acquire();
            e4.s.acquire();
        }
        ++finishCount;
    });

    // single consumer
    while (finishCount < 3) {
        auto b = queue.pop();
        if (b == &e1) {
            ++c1;
            e1.s.release();
        } else if (b == &e2) {
            ++c2;
            e2.s.release();
        } else if (b == &e3) {
            // check correct order of e3 and e4
            EXPECT_EQ(c3, c4);
            ++c3;
            e3.s.release();
        } else if (b == &e4) {
            ++c4;
            // check correct order of e3 and e4
            EXPECT_EQ(c3, c4);
            e4.s.release();
        } else {
            // no elements in queue: yield to producers
            std::this_thread::yield();
        }
    }

    t1.join();
    t2.join();
    t34.join();

    EXPECT_EQ(c1, COUNT);
    EXPECT_EQ(c2, COUNT);
    EXPECT_EQ(c3, COUNT);
    EXPECT_EQ(c4, COUNT);
}



// InterruptQueue

bool testGuardActive = false;

struct TestGuard {
    [[nodiscard]] TestGuard() {
        testGuardActive = true;
    }
    TestGuard(const TestGuard &) = delete;
    TestGuard &operator=(const TestGuard &) = delete;
    ~TestGuard() {
        testGuardActive = false;
    }
};

/*
struct MutexGuard {
    [[nodiscard]] MutexGuard(std::mutex &mutex) : mutex(mutex) {
        mutex.lock();
    }
    ~MutexGuard() {
        mutex.unlock();
    }

    std::mutex &mutex;
};
*/
TEST(cocoTest, InterruptQueue_clear) {
    InterruptQueue2<Element> queue;
    Element e1, e2;

    EXPECT_TRUE(queue.empty());
    EXPECT_TRUE(queue.emptyOrOne());

    // clear has no effect
    queue.clear();
    EXPECT_TRUE(queue.empty());
    EXPECT_TRUE(queue.emptyOrOne());

    // add element
    EXPECT_TRUE(queue.push(e1));
    EXPECT_FALSE(queue.empty());
    EXPECT_TRUE(queue.emptyOrOne());

    // add element
    EXPECT_FALSE(queue.push(e2));
    EXPECT_FALSE(queue.empty());
    EXPECT_FALSE(queue.emptyOrOne());

    // clear has effect
    queue.clear();
    EXPECT_TRUE(queue.empty());
    EXPECT_TRUE(queue.emptyOrOne());
}

TEST(cocoTest, InterruptQueue_pop) {
    InterruptQueue2<Element> queue;
    Element e1, e2;
    Element *result;

    // push e1
    if (queue.guardedPush(TestGuard(), e1)) {
        // when adding the first element push should return true
        // the guard should be inactive again
        EXPECT_FALSE(testGuardActive);
    } else {
        FAIL();
    }

    // push e2
    EXPECT_FALSE(queue.guardedPush(TestGuard(), e2));

    // pop first element (e1)
    result = queue.popIf(
        [](Element &e) {
            // return true to remove the element
            return true;
        },
        [&e1](Element &removed) {
            // check that the removed element is e1
            EXPECT_EQ(&removed, &e1);

            // overwrite next pointer, queue should be immune to this
            removed.next = nullptr;
        }
    );
    EXPECT_EQ(result, &e1);

    // pop second element (e2)
    result = queue.popIf(
        [](Element &e) {
            // return true to remove the element
            return true;
        },
        [](Element &next) {
            // no next element
            FAIL();
        },
        [&e2](Element &removed) {
            // check that the removed element is e1
            EXPECT_EQ(&removed, &e2);

            // overwrite next pointer, queue should be immune to this
            removed.next = nullptr;
        }
    );

    // re-add elements
    queue.push(e1);
    queue.push(e2);

    // pop first element (e1)
    bool nextCalled = false;
    result = queue.popIf(
        [](Element &e) {
            // return true to remove the element
            return true;
        },
        [&e2, &nextCalled](Element &next) {
            // check that the next element is e2
            EXPECT_EQ(&next, &e2);

            // mark that next was called
            nextCalled = true;
        },
        [&e1](Element &removed) {
            // check that the removed element is e1
            EXPECT_EQ(&removed, &e1);

            // overwrite next pointer, queue should be immune to this
            removed.next = nullptr;
        }
    );
    EXPECT_EQ(result, &e1);
    EXPECT_TRUE(nextCalled);
    EXPECT_FALSE(queue.empty());

    // re-add e1 behind e2
    EXPECT_FALSE(queue.push(e1));

    // pop second element (e2)
    result = queue.pop(
        [&e2](Element &removed) {
            // check that the removed element is e1
            EXPECT_EQ(&removed, &e2);

            // overwrite next pointer
            removed.next = nullptr;
        }
    );
    EXPECT_EQ(result, &e2);
    EXPECT_FALSE(queue.empty());

    // pop and reject
    result = queue.popIf([](Element &e) {return false;});
    EXPECT_EQ(result, nullptr);
    result = queue.popIf([](Element &e) {return false;}, [](Element &removed) {});
    EXPECT_EQ(result, nullptr);
    result = queue.popIf([](Element &e) {return false;}, [](Element &next) {}, [](Element &removed) {});
    EXPECT_EQ(result, nullptr);

    // pop e1
    EXPECT_EQ(queue.pop(), &e1);

    // queue is now empty again
    EXPECT_TRUE(queue.empty());

    // try to pop empty queue
    EXPECT_EQ(queue.pop(), nullptr);
    result = queue.pop([](Element &e) {return true;});
    EXPECT_EQ(result, nullptr);
    result = queue.pop([](Element &e) {return true;}, [](Element &next) {});
    EXPECT_EQ(result, nullptr);
}

TEST(cocoTest, InterruptQueue_remove) {
    InterruptQueue2<Element> queue;
    Element e1, e2, e3;
    bool result;
    bool nextCalled;

    // queue is initially empty
    EXPECT_TRUE(queue.empty());
    EXPECT_EQ(queue.pop(), nullptr);
    EXPECT_EQ(queue.frontOrNull(), nullptr);

    // push some elements
    EXPECT_TRUE(queue.push(e1)); // push e1
    EXPECT_FALSE(queue.removeExceptFirst(e1)); // can't remove e1 as it is the front element
    EXPECT_FALSE(queue.empty()); // therefore the queue is not empty
    EXPECT_EQ(queue.remove(e2), false); // e2 is not in list
    EXPECT_EQ(&queue.front(), &e1); // e1 is still first element
    EXPECT_EQ(queue.frontOrNull(), &e1);
    EXPECT_FALSE(queue.push(e2)); // push e2
    EXPECT_FALSE(queue.push(e3)); // push e3
    EXPECT_EQ(queue.frontOrNull(), &e1); // e1 is still first element

    // remove e3
    EXPECT_TRUE(queue.remove(e3)); // remove succeeds
    EXPECT_EQ(queue.frontOrNull(), &e1);

    // reject removal of e1
    result = queue.removeIf(
        [&e1](Element &e, int index) {
            // e1 is first element
            if (&e == &e1)
                EXPECT_EQ(index, 0);
            return false;
        },
        [] (Element &next, int index) {
            // not called as e1 is not removed
            FAIL();
        },
        [&e1](Element &removed) {
            // not called as e1 is not removed
            FAIL();
        });

    // remove e1
    nextCalled = false;
    result = queue.removeIf(
        [&e1](Element &e, int index) {
            // e1 is first element
            if (&e == &e1)
                EXPECT_EQ(index, 0);
            return &e == &e1;
        },
        [&e2, &nextCalled] (Element &next, int index) {
            // e2 is next element
            EXPECT_EQ(&next, &e2);

            // e1 is first element
            EXPECT_EQ(index, 0);

            nextCalled = true;
        },
        [&e1](Element &e) {
            // check that the removed element is e1
            EXPECT_EQ(&e, &e1);

            // overwrite next pointer, queue should be immune to this
            e.next = nullptr;
        });
    EXPECT_TRUE(nextCalled);
    EXPECT_EQ(queue.frontOrNull(), &e2);
    EXPECT_FALSE(queue.empty());

    // remove e2
    nextCalled = false;
    result = queue.removeIf(
        [&e2](Element &e, int index) {
            // e2 is now the only element
            EXPECT_EQ(index, 0);
            return &e == &e2;
        },
        [] (Element &next, int index) {
            // no next element
            FAIL();
        },
        [&e2](Element &e) {
            // check that the removed element is e2
            EXPECT_EQ(&e, &e2);

            // overwrite next pointer, queue should be immune to this
            e.next = nullptr;
        });
    EXPECT_FALSE(nextCalled);
    EXPECT_EQ(queue.frontOrNull(), nullptr);
    EXPECT_EQ(queue.pop(), nullptr);
    EXPECT_TRUE(queue.empty());
}

std::mutex mutex;

TEST(cocoTest, InterruptQueueMultiThreaded) {
    InterruptQueue2<Element> queue;
    Element e1, e2, e3;
    int c1 = 0, c2 = 0;
    std::atomic<int> c3 = 0;
    std::atomic<int> finishCount = 0;

    // single producer
    std::thread t([&queue, &e1, &e2, &e3, &c3, &finishCount] {
        //std::cout << "started thread" << std::endl;
        for (int i = 0; i < COUNT; ++i) {
            queue.guardedPush(std::lock_guard(mutex), e1);
            queue.guardedPush(std::lock_guard(mutex), e2);
            queue.guardedPush(std::lock_guard(mutex), e3);

            if (queue.guardedRemoveExceptFirst(std::lock_guard(mutex), e3)) {
                // remove succeeded
                ++c3;
            } else {
                // acquire semaphore, needs to wait until release() was called in the consumer thread
                e3.s.acquire();
            }

            // acquire semaphores, needs to wait until release() was called in the consumer thread
            e1.s.acquire();
            e2.s.acquire();
        }
        ++finishCount;
    });

    // single consumer
    while (finishCount < 1) {
        // simulate entry of interrupt service routine
        mutex.lock();

        auto b = queue.pop();

        // simulate exit of interrupt service routine
        mutex.unlock();

        if (b == &e1) {
            // check correct order of e3 and e4
            EXPECT_EQ(c1, c2);
            ++c1;
            e1.s.release();
        } else if (b == &e2) {
            ++c2;
            // check correct order of e3 and e4
            EXPECT_EQ(c1, c2);
            e2.s.release();
        } else if (b == &e3) {
            ++c3;
            e3.s.release();
        } else {
            // no elements in queue: yield to producers
            std::this_thread::yield();
        }
    }

    t.join();

    EXPECT_EQ(c1, COUNT);
    EXPECT_EQ(c2, COUNT);
    EXPECT_EQ(c3, COUNT);
}
