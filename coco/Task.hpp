#pragma once

#include "assert.hpp"
#include "LinkedList.hpp"
#include <utility>


namespace coco {


/**
	Task
	@tparam T task type, e.g. Callback, std::coroutine_handle<> or std::function
*/
template <typename T>
class Task : public LinkedListNode {
public:
	Task(const T &task) : task(task) {}

	void cancel() noexcept {remove();}

	// function or coroutine waiting for the event to occur
	T task;
};

/**
	Task with parameters (mainly for parameters of awaitable functions/methods)
	@tparam T task type
	@tparam P parameters
*/
template <typename T, typename P>
class TaskWithParameters : public Task<T> {
public:
	template <typename ...Args>
	explicit TaskWithParameters(const T &task, Args &&...args)
		: Task<T>(task), parameters{std::forward<Args>(args)...} {}

	P parameters;
};

template <typename T>
T &getParameters(T &task) {
	return task;
}

template <typename T, typename P>
P &getParameters(TaskWithParameters<T, P> &task) {
	return task.parameters;
}



/**
	List of tasks (e.g. waiting coroutines)
	@tparam T task type
*/
template <typename T>
class TaskList : public LinkedListNode {
public:
	using Task = T;

	/**
		Check if the list is empty
		@return true if empty
	*/
	bool empty() const {
		return this->next == this;
	}

	/**
		Add a task. Must not already be in a list
		@param task task to add
	*/
	void add(Task &task) {
		assert(!task.inList());

		task.prev = this->prev;
		task.next = this;
		this->prev->next = &task;
		this->prev = &task;
	}

	/**
		Visit all tasks
		@tparam V visitor type, e.g. a lambda function
		@param visitor visitor
	*/
	template <typename V>
	void visitAll(const V &visitor) {
		auto current = this->next;
		while (current != this) {
			visitor(static_cast<Task &>(*current));
			current = current->next;
		}
	}

	/**
		Remove and execute the first task
		@return true when a coroutine was resumed, false when the list was empty
	*/
	bool doFirst() {
		if (this->next != this) {
			auto &first = static_cast<Task &>(*this->next);

			// remove task from list
			first.remove();

			// execute task
			first.task();

			return true;
		}
		return false;
	}

	/**
		Remove and execute all tasks that are in the list on entry of doAll()
	*/
	void doAll() {
		if (this->next == this)
			return;

		// temporary head for tasks to execute
		LinkedListNode head(this->next, this->prev);

		// link nodes to temporary head
		this->next->prev = &head;
		this->prev->next = &head;

		// empty list
		this->next = this;
		this->prev = this;

		// execute tasks
		while (head.next != &head) {
			auto &first = static_cast<Task &>(*head.next);

			// remove task from list
			first.remove();

			// execute task
			first.task();
		}
	}

	/**
		Remove and execute the first task when its predicate is true
		@param predicate boolean predicate function that determines if the first task should be executed
		@return true when a task was executed
	*/
	template <typename P>
	bool doFirst(const P &predicate) {
		if (this->next != this) {
			auto &first = static_cast<Task &>(*this->next);
			if (predicate(getParameters(first))) {
				// remove task from list
				first.remove();

				// execute task
				first.task();

				return true;
			}
		}
		return false;
	}


	/**
		Remove and execute all tasks that are in the list on entry of doAll() and for which the predicate returns true
		@param predicate boolean predicate function that selects the tasks to execute
	*/
	template <typename P>
	void doAll(P const &predicate) {
		// add iterator node at the beginning
		LinkedListNode it(*this->next);

		// add end marker node
		LinkedListNode end(*this);

		// iterate over tasks
		while (it.next != &end) {
			auto &current = static_cast<Task &>(*it.next);

			if (predicate(getParameters(current))) {
				// remove task from list
				current.remove();

				// execute task
				current.task();
			} else {
				// advance iterator node
				it.advance();
			}
		}
	}

};

} // namespace coco
