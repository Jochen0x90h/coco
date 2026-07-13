#pragma once

#include "IsSubclass.hpp"
//#include "Task.hpp"
//#include "TimedTask.hpp"
#include "IntrusiveList.hpp"
#include "IntrusiveSortedTaskList.hpp"
#include "IntrusiveTaskList.hpp"
#include "Time.hpp"

#ifdef __clang__
#include <experimental/coroutine>
namespace std {
    using namespace std::experimental::coroutines_v1;
}
#else
#include <coroutine>
#endif


//#define COROUTINE_DEBUG_PRINT
#ifdef COROUTINE_DEBUG_PRINT
#include <iostream>
#endif


namespace coco {

/// @brief Coroutine task
/// @tparam T Additional value type
template <typename T = void>
class CoroutineTask : public IntrusiveListNode {
public:
    //using Node = IntrusiveListNode;

    template <typename ...Args>
    CoroutineTask(const std::coroutine_handle<> &coroutine, Args &&...args)
        : coroutine(coroutine), value{std::forward<Args>(args)...} {}

    void operator ()() {coroutine.resume();}

    // coroutine handle
    std::coroutine_handle<> coroutine;

    T value;
};

// specialization without value
template <>
class CoroutineTask<void> : public IntrusiveListNode {
public:
    using Node = IntrusiveListNode;

    CoroutineTask(const std::coroutine_handle<> &coroutine)
        : coroutine(coroutine) {}

    void operator ()() {coroutine.resume();}

    // coroutine handle
    std::coroutine_handle<> coroutine;
};

template <typename T = void>
using CoroutineTaskList = IntrusiveTaskList<CoroutineTask<T>>;

using CoroutineTimedTask = CoroutineTask<TimeMilliseconds<>>;
using CoroutineTimedTaskList = IntrusiveSortedTaskList<CoroutineTimedTask>;


/////////

/*
using CoroutineTask = Task<std::coroutine_handle<>>;
using CoroutineTimedTask = TimedTask<std::coroutine_handle<>>;


template <typename P, int>
struct CoroutineTaskSelector {
    using Task = TaskWithParameters<std::coroutine_handle<>, P>;
};

// specialization for T being derived from CoroutineTask
template <typename T>
struct CoroutineTaskSelector<T, 1> {
    using Task = T;
};

template <typename T = CoroutineTask>
using CoroutineTaskList = TaskList<typename CoroutineTaskSelector<T, IsSubclass<T, CoroutineTask>::value>::Task>;
using CoroutineTimedTaskList = TimedTaskList<std::coroutine_handle<>>;
*/


/// @brief This type is returned from functions/methods that can be awaited on using co_await.
/// It behaves like an unique_ptr to a resource and therefore can only be moved, but not copied.
/// @tparam T task type
template <typename T/* = CoroutineTask*/>
struct Awaitable {
    using Task = T;//typename CoroutineTaskSelector<T, IsSubclass<T, CoroutineTask>::value>::Task;
    Task task;


    Awaitable() : task(std::noop_coroutine()) {
    }

    /// @brief Constructor.
    /// @tparam L task list type
    /// @param list task list
    /// @param args arguments for task
    template <typename L, typename ...Args>
    Awaitable(L &list, Args &&...args) noexcept : task(std::noop_coroutine(), std::forward<Args>(args)...) {
        // add task to task list (can use unsafe as the task is not yet part of a list)
        list.addUnsafe(this->task);
#ifdef COROUTINE_DEBUG_PRINT
        std::cout << "Awaitable add" << std::endl;
#endif
    }

    // delete copy constructor
    Awaitable(Awaitable const &) = delete;

    /// @brief Move constructor.
    ///
    Awaitable(Awaitable &&a) noexcept : task(std::move(a.task)) {
#ifdef COROUTINE_DEBUG_PRINT
        std::cout << "Awaitable move" << std::endl;
#endif
    }

    /// @brief Move assignment.
    ///
    Awaitable &operator =(Awaitable &&a) {
        this->task = std::move(a.task);
        return *this;
    }

    /// @brief Determine if the operation or coroutine has finished.
    /// @return true when finished, false when still in progress (coroutine: running or co_awaiting)
    bool hasFinished() const noexcept {
        return !this->task.inList();
    }

    /// @brief Used by co_await to determine if the operation has finished (ready to continue).
    /// @return true when finished
    bool await_ready() const noexcept {
        // is ready when the task is "not in list"
        return !this->task.inList();
    }

    /// @brief Used by co_await to store the handle of the calling coroutine before suspending.
    /// @param handle coroutine handle
    void await_suspend(std::coroutine_handle<> handle) noexcept {
#ifdef COROUTINE_DEBUG_PRINT
        std::cout << "Awaitable await_suspend" << std::endl;
#endif
        // set the coroutine handle
        this->task.coroutine = handle;
    }

    /// @brief Used by co_await to determine the return value of co_await.
    ///
    void await_resume() noexcept {
#ifdef COROUTINE_DEBUG_PRINT
        std::cout << "Awaitable await_resume" << std::endl;
#endif
    }


    // methods for AwaitableCoroutine

    bool isAlive() const noexcept {
        return this->task.inList();
    }

    /// @brief Cancel the operation or coroutine.
    ///
    void cancel() {
#ifdef COROUTINE_DEBUG_PRINT
        std::cout << "Awaitable cancel" << std::endl;
#endif
        this->task.cancel();
    }


    /// @brief An awaitable function or method can also be a coroutine, therefore define a promise_type.
    ///
    struct promise_type {
        // the task list is part of the coroutine promise
        //CoroutineTaskList<T> list;
        IntrusiveTaskList<T> list;

        ~promise_type() {
#ifdef COROUTINE_DEBUG_PRINT
            std::cout << "AwaitableCoroutine ~promise_type" << std::endl;
#endif
            // the coroutine exits normally or gets destroyed
            //this->list.doAll();
        }

        Awaitable get_return_object() {
            auto handle = std::coroutine_handle<promise_type>::from_promise(*this);
            return {this->list, handle};
        }

        auto initial_suspend() noexcept {
            return std::suspend_never{};
        }

        auto final_suspend() noexcept {
#ifdef COROUTINE_DEBUG_PRINT
            std::cout << "AwaitableCoroutine promise_type::final_suspend" << std::endl;
#endif
            // the coroutine exits normally, not called on destroy() of handle
            this->list.doAll();

            return std::suspend_never{};
        }

        void unhandled_exception() {
        }

        // the coroutine exits normally
        void return_void() {
#ifdef COROUTINE_DEBUG_PRINT
            std::cout << "AwaitableCoroutine promise_type::return_void" << std::endl;
#endif
        }
    };
};


class AwaitableCoroutineTask : public CoroutineTask<> {
public:

    AwaitableCoroutineTask(std::coroutine_handle<> task) : CoroutineTask(task) {}
    AwaitableCoroutineTask(std::coroutine_handle<> task, std::coroutine_handle<> context)
        : CoroutineTask(task), context(context) {}


    ~AwaitableCoroutineTask() {
        if (inList()) {
            // prevent effect of doAll() in promise_type::~promise_type
            //remove(); // call doAll() in final_suspend() of promise_type
            this->context.destroy();
        }
    }

    AwaitableCoroutineTask &operator =(AwaitableCoroutineTask &&task) {
        bool il = inList();
        IntrusiveListNode::operator =(std::move(task)); // is also remove()
        if (il)
            this->context.destroy();
        this->context = task.context;
        return *this;
    }

    void cancel() {
        // is not in list if couroutine has already finished or cancel() has been called before
        if (inList()) {
            // prevent effect of doAll() in promise_type::~promise_type
            //remove(); // call doAll() in final_suspend() of promise_type
            this->context.destroy();
        }
    }


    // handle of awaitable coroutine
    std::coroutine_handle<> context;
};

/// @brief Awaitable coroutine.
///
/// Use like this:
/// AwaitableCoroutine foo() {
///     co_await bar();
/// }
/// Coroutine coro() {
///     // bad: this immediately destroys the coroutine
///     foo();
///
///     // start coroutine and wait until it is finished
///     co_await foo();
///
///     // start coroutine and keep a handle
///     AwaitableCoroutine c = foo();
///
///     // wait until coroutine is finished
///     co_await c;
/// }
using AwaitableCoroutine = Awaitable<AwaitableCoroutineTask>;



/// @brief Simple detached coroutine handle for asynchronous processing.
/// The handle can be used to destroy the coroutine later unless it already has destroyed itself.
/// The default constructor initializes the coroutine handle with nullptr.
///
/// Use like this:
/// Coroutine foo() {
///     co_await bar();
/// }
/// void main() {
///     // start coroutine which is repsonsible to destroy itself
///     foo();
///
///     // start coroutine and keep a handle
///     Coroutine c = foo();
///
///     // only if we are sure that the coroutine is still alive!
///     c.destroy();
/// }
struct Coroutine {
    std::coroutine_handle<> handle;

    /// @brief Promise type that configures the coroutine
    ///
    struct promise_type {
        Coroutine get_return_object() noexcept {
#ifdef COROUTINE_DEBUG_PRINT
            std::cout << "Coroutine get_return_object" << std::endl;
#endif
            return {std::coroutine_handle<promise_type>::from_promise(*this)};
        }
        std::suspend_never initial_suspend() noexcept {
#ifdef COROUTINE_DEBUG_PRINT
            std::cout << "Coroutine initial_suspend" << std::endl;
#endif
            return {};
        }
        std::suspend_never final_suspend() noexcept {return {};}
        void unhandled_exception() noexcept {}
        void return_void() noexcept {}
    };

    /// @brief Destroy the coroutine if it is still alive and suspended (coroutine has called co_await).
    /// Important: Call only when it is sure that the coroutine is still alive, e.g. when it contains an infinite loop.
    void destroy() {
        if (handle) {
            handle.destroy();
            handle = nullptr;
        }
    }

    /// @brief Clear the reference to the coroutine
    ///
    void clear() {
        handle = nullptr;
    }

    /// @brief Cast to bool to check if the coroutine handle is empty
    ///
    operator bool() const {
        return handle != nullptr;
    }
};



// helper struct
template <typename A1, typename A2>
struct Select2 {
    A1 &a1;
    A2 &a2;

    bool await_ready() noexcept {
        return this->a1.await_ready() || this->a2.await_ready();
    }

    void await_suspend(std::coroutine_handle<> handle) noexcept {
        this->a1.await_suspend(handle);
        this->a2.await_suspend(handle);
    }

    int await_resume() noexcept {
        // check which awaitable was resumed (removed from its list)
        int result = 0;
        if (this->a2.await_ready())
            result = 2;
        if (this->a1.await_ready())
            result = 1;

        // set handles to zero because only one awaitable (e.g. a1) was resumed
        // while the other (e.g. a2) may continue to exist without a coroutine waiting on it
        await_suspend(nullptr);
        return result;
    }
};

/// @brief Wait on two awaitables and return 1 if the first is ready and 2 if the second is ready, e.g.
/// switch (co_await select(read(data, length), delay(1s))) {
/// case 1:
///     // read is ready
///     break;
/// case 2:
///     // timeout
///     break;
/// }
template <typename A1, typename A2>
[[nodiscard]] inline Select2<A1, A2> select(A1 &&a1, A2 &&a2) {
    return {a1, a2};
}


// helper struct
template <typename A1, typename A2, typename A3>
struct Select3 {
    A1 &a1;
    A2 &a2;
    A3 &a3;

    bool await_ready() noexcept {
        return this->a1.await_ready() || this->a2.await_ready() || this->a3.await_ready();
    }

    void await_suspend(std::coroutine_handle<> handle) noexcept {
        this->a1.await_suspend(handle);
        this->a2.await_suspend(handle);
        this->a3.await_suspend(handle);
    }

    int await_resume() noexcept {
        // check which awaitable was resumed (removed from its list)
        int result = 0;
        if (this->a3.await_ready())
            result = 3;
        if (this->a2.await_ready())
            result = 2;
        if (this->a1.await_ready())
            result = 1;

        // set handles to zero because only one awaitable (e.g. a1) was resumed
        // while the others (e.g. a2 and a3) may continue to exist without a coroutine waiting on it
        await_suspend(nullptr);
        return result;
    }
};

/// @brief Wait on three awaitables and return 1 if the first is ready, 2 if the second is ready and 3 if the third is ready.
///
template <typename A1, typename A2, typename A3>
[[nodiscard]] inline Select3<A1, A2, A3> select(A1 &&a1, A2 &&a2, A3 &&a3) {
    return {a1, a2, a3};
}


// helper struct
template <typename A1, typename A2, typename A3, typename A4>
struct Select4 {
    A1 &a1;
    A2 &a2;
    A3 &a3;
    A4 &a4;

    bool await_ready() noexcept {
        return this->a1.await_ready() || this->a2.await_ready() || this->a3.await_ready() || this->a4.await_ready();
    }

    void await_suspend(std::coroutine_handle<> handle) noexcept {
        this->a1.await_suspend(handle);
        this->a2.await_suspend(handle);
        this->a3.await_suspend(handle);
        this->a4.await_suspend(handle);
    }

    int await_resume() noexcept {
        // check which awaitable was resumed (removed from its list)
        int result = 0;
        if (this->a4.await_ready())
            result = 4;
        if (this->a3.await_ready())
            result = 3;
        if (this->a2.await_ready())
            result = 2;
        if (this->a1.await_ready())
            result = 1;

        // set handles to zero because only one awaitable (e.g. a1) was resumed
        // while the others (e.g. a2, a3 and a4) may continue to exist without a coroutine waiting on it
        await_suspend(nullptr);
        return result;
    }
};

/// @brief Wait on four awaitables and return index 1 to 4 of the awaitable that is ready.
///
template <typename A1, typename A2, typename A3, typename A4>
[[nodiscard]] inline Select4<A1, A2, A3, A4> select(A1 &&a1, A2 &&a2, A3 &&a3, A4 &&a4) {
    return {a1, a2, a3, a4};
}


// helper struct
template <typename A1, typename A2, typename A3, typename A4, typename A5>
struct Select5 {
    A1 &a1;
    A2 &a2;
    A3 &a3;
    A4 &a4;
    A5 &a5;

    bool await_ready() noexcept {
        return this->a1.await_ready() || this->a2.await_ready() || this->a3.await_ready() || this->a4.await_ready() || this->a5.await_ready();
    }

    void await_suspend(std::coroutine_handle<> handle) noexcept {
        this->a1.await_suspend(handle);
        this->a2.await_suspend(handle);
        this->a3.await_suspend(handle);
        this->a4.await_suspend(handle);
        this->a5.await_suspend(handle);
    }

    int await_resume() noexcept {
        // check which awaitable was resumed (removed from its list)
        int result = 0;
        if (this->a5.await_ready())
            result = 5;
        if (this->a4.await_ready())
            result = 4;
        if (this->a3.await_ready())
            result = 3;
        if (this->a2.await_ready())
            result = 2;
        if (this->a1.await_ready())
            result = 1;

        // set handles to zero because only one awaitable (e.g. a1) was resumed
        // while the others (e.g. a2, a3 and a4) may continue to exist without a coroutine waiting on it
        await_suspend(nullptr);
        return result;
    }
};

/// @brief Wait on four awaitables and return index 1 to 4 of the awaitable that is ready
///
template <typename A1, typename A2, typename A3, typename A4, typename A5>
[[nodiscard]] inline Select5<A1, A2, A3, A4, A5> select(A1 &&a1, A2 &&a2, A3 &&a3, A4 &&a4, A5 &&a5) {
    return {a1, a2, a3, a4, a5};
}

} // namespace coco
