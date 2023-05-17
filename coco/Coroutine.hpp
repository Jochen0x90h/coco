#pragma once

#include "Task.hpp"


namespace coco {

//template <typename T = Task>
//using Waitlist = TaskList<T>;

/**
 * This type is returned from functions/methods that can be awaited on using co_await. It behaves like an unique_ptr
 * to a resource and therefore can only be moved, but not copied.
 */
template <typename T = Task>
struct Awaitable {
	typename TaskList<T>::Task task;

	Awaitable() {
		this->task.setNotInList();
	}

	template <typename L, typename ...Args>
	Awaitable(L &list, Args &&...args) noexcept : task(std::forward<Args>(args)...) {
		// make sure the task list is derived from TaskList<T>
		static_assert(IsSubclass<L, TaskList<T>>::value);

		// add task to task list
		list.add(this->task);
#ifdef COROUTINE_DEBUG_PRINT
		std::cout << "Awaitable add" << std::endl;
#endif
	}

	Awaitable(Awaitable const &) = delete;

	/**
	 * Move constructor, only supported when the task class implements take()
	 */
	Awaitable(Awaitable &&a) noexcept {
#ifdef COROUTINE_DEBUG_PRINT
		std::cout << "Awaitable move" << std::endl;
#endif
		// take waiting coroutine from other task
		if (a.task.isInList())
			this->task.take(a.task);
	}

	~Awaitable() {
#ifdef COROUTINE_DEBUG_PRINT
		std::cout << "Awaitable destructor" << std::endl;
#endif
		if (this->task.isInList()) {
#ifdef COROUTINE_DEBUG_PRINT
			std::cout << "Awaitable cancel" << std::endl;
#endif
			this->task.cancel();
		}
	}

	/**
	 * Move assignment, only supported when the task class implements take()
	 */
	Awaitable &operator =(Awaitable &&a) noexcept {
		// cancel existing job
		if (this->task.isInList())
			this->task.cancel();

		// take waiting coroutine from other task
		if (a.task.isInList())
			this->task.take(a.task);
		return *this;
	}

	bool isAlive() const noexcept {
		return this->task.isInList();
	}

	/**
	 * Determine if the operation or coroutine has finished
	 * @return true when finished, false when still in progress (coroutine: running or co_awaiting)
	 */
	bool hasFinished() const noexcept {
		return !this->task.isInList();
	}

	/**
	 * Cancel the operation or coroutine
	 */
	void cancel() {
#ifdef COROUTINE_DEBUG_PRINT
		std::cout << "Awaitable cancel" << std::endl;
#endif
		if (this->task.isInList())
			this->task.cancel();
	}

	/**
	 * Used by co_await to determine if the operation has finished (ready to continue)
	 * @return true when finished
	 */
	bool await_ready() const noexcept {
		// is ready when the task is "not in list"
		return !this->task.isInList();
	}

	/**
	 * Used by co_await to store the handle of the calling coroutine before suspending
	 */
	void await_suspend(std::coroutine_handle<> handle) noexcept {
#ifdef COROUTINE_DEBUG_PRINT
		std::cout << "Awaitable await_suspend" << std::endl;
#endif
		// set the coroutine handle
		this->task.handle = handle;
	}

	/**
	 * Used by co_await to determine the return value of co_await
	 */
	void await_resume() noexcept {
#ifdef COROUTINE_DEBUG_PRINT
		std::cout << "Awaitable await_resume" << std::endl;
#endif
	}


	/**
	 * An awaitable function or method can also be a coroutine, therefore define a promise_type
	 */
	struct promise_type {
		// the task list is part of the coroutine promise
		TaskList<T> list;

		~promise_type() {
#ifdef COROUTINE_DEBUG_PRINT
			std::cout << "AwaitableCoroutine ~promise_type" << std::endl;
#endif
			// the coroutine exits normally or gets destroyed
			this->list.resumeAll();
		}

		Awaitable get_return_object() {
			auto handle = std::coroutine_handle<promise_type>::from_promise(*this);
			return {this->list, handle};
		}

		std::suspend_never initial_suspend() noexcept {
			return {};
		}

		std::suspend_never final_suspend() noexcept {
			return {};
		}

		void unhandled_exception() {
		}

		// the coroutine exits normally
		void return_void() {
#ifdef COROUTINE_DEBUG_PRINT
			std::cout << "AwaitableCoroutine return_void" << std::endl;
#endif
		}
	};
};


class AwaitableCoroutineTask : public TaskNode {
public:

	AwaitableCoroutineTask() = default;
	explicit AwaitableCoroutineTask(std::coroutine_handle<> context) : context(context) {}

	void take(AwaitableCoroutineTask &element) {
		element.pass(*this);
		this->context = element.context;
	}

	void cancel() {
		remove();
		this->context.destroy();
	}


	// handle of waiting coroutine
	std::coroutine_handle<> handle = nullptr;

	// handle of awaitable coroutine
	std::coroutine_handle<> context = nullptr;
};

using AwaitableCoroutine = Awaitable<AwaitableCoroutineTask>;



/**
 * Simple detached coroutine for asynchronous processing. If the return value is kept, it can be destroyed later unless
 * it already has destroyed itself.
 *
 * Use like this:
 * Coroutine foo() {
 *   co_await bar();
 * }
 * void main() {
 *   Coroutine c = foo();
 *
 *   // we are sure that the coroutine is still alive
 *   c.destroy();
 * }
 */
struct Coroutine {
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
	
	std::coroutine_handle<> handle;
	
	/**
	 * Destroy the coroutine if it is still alive and suspended (coroutine has called co_await).
	 * Call only once and when it is sure that the coroutine is still alive, e.g. when it contains an infinite loop.
	 */
	void destroy() {
		this->handle.destroy();
	}
};


/**
 * Helper class to obtain the handle of a coroutine
 *//*
struct CoroutineHandle {
	std::coroutine_handle<> handle;

	bool await_ready() noexcept {
		return false;
	}

	void await_suspend(std::coroutine_handle<> handle) noexcept {
		this->handle = handle;
		handle.resume();
	}

	void await_resume() noexcept {
	}
	
	void destroy() {
		if (this->handle) {
			this->handle.destroy();
			this->handle = nullptr;
		}
	}
};*/



// helper struct
template <typename A1, typename A2>
struct Awaitable2 {
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

/**
 * Wait on two awaitables and return 1 if the first is ready and 2 if the second is ready, e.g.
 * switch (co_await select(read(data, length), delay(1s))) {
 * case 1:
 *   // read is ready
 *   break;
 * case 2:
 *   // timeout
 *   break;
 * }
 */
template <typename A1, typename A2>
[[nodiscard]] inline Awaitable2<A1, A2> select(A1 &&a1, A2 &&a2) {
	return {a1, a2};
}


// helper struct
template <typename A1, typename A2, typename A3>
struct Awaitable3 {
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

/**
 * Wait on three awaitables and return 1 if the first is ready, 2 if the second is ready and 3 if the third is ready
 */
template <typename A1, typename A2, typename A3>
[[nodiscard]] inline Awaitable3<A1, A2, A3> select(A1 &&a1, A2 &&a2, A3 &&a3) {
	return {a1, a2, a3};
}


// helper struct
template <typename A1, typename A2, typename A3, typename A4>
struct Awaitable4 {
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

/**
 * Wait on four awaitables and return index 1 to 4 of the awaitable that is ready
 */
template <typename A1, typename A2, typename A3, typename A4>
[[nodiscard]] inline Awaitable4<A1, A2, A3, A4> select(A1 &&a1, A2 &&a2, A3 &&a3, A4 &&a4) {
	return {a1, a2, a3, a4};
}



/**
 * Simple barrier on which a data consumer coroutine can wait until it gets resumed by a data producer.
 * If a resume method gets called by a data producer while no consumer is waiting, the event/data gets lost.
 * @tparam T
 */
template <typename T = Task>
class Barrier : public TaskList<T> {
public:

	/**
	 * Wait until a data producer passes data (using e.g. resumeFirst() or resumeAll()).
	 * Call this as a data consumer
	 * @return use co_await on return value to wait for data
	 */
	template <typename ...Args>
	[[nodiscard]] Awaitable<T> wait(Args &&...args) {
		return {*this, std::forward<Args>(args)...};
	}
};


/**
 * Manual reset event
 */
class Event {
public:
	/**
	 * Construct an event in cleared state
	 */
	Event() = default;

	/**
	 * Set the event
	 */
	void set() {
		//this->state = this->taskList.isEmpty();
		//this->taskList.resumeFirst();
		this->state = true;
		this->taskList.resumeAll();
	}

	void clear() {
		this->state = false;
	}

	/**
	 * Wait until the event is set
	 */
	[[nodiscard]] Awaitable<> wait() {
		if (this->state) {
			//this->state = false;
			return {};
		}
		return {this->taskList};
	}


	// list of waiting coroutines
	TaskList<> taskList;
	bool state = false;
};


class Semaphore {
public:

	/**
	 * Construct a semaphore with a given number of initial tokens that can be handed out
	 * @param n number of initial tokens
	 */
	explicit Semaphore(int n) : n(n) {}
	
	/**
	 * Post a token and resume the next coroutine waiting for a token
	 */
	void post() {
		this->n += 1 - int(this->taskList.resumeFirst());
	}

	/**
	 * Wait for a token to become available
	 */
	[[nodiscard]] Awaitable<> wait() {
		// check if tokens are available
		if (this->n > 0) {
			--this->n;
			return {};
		}

		// wait until token is available
		return {this->taskList};
	}


	// number of tokens
	int n;
	
	// list of waiting coroutines
	TaskList<> taskList;
};

} // namespace coco
