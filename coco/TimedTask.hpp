#pragma once

#include "assert.hpp"
#include "Task.hpp"
#include "Time.hpp"


namespace coco {

/**
	Timed task
	@tparam T task function, e.g. Callback, std::coroutine_handle<> or std::function
*/
template <typename T>
class TimedTask : public Task<T> {
public:
	TimedTask(const T &task) : Task<T>(task) {}
	TimedTask(const T &task, Time time) : Task<T>(task), time(time) {}

	void cancelAndSet(Time time) {
		Task<T>::remove();
		this->time = time;
	}

//protected:

	Time time;
};

/**
	List of timed events
	@tparam T task function, e.g. Callback, std::coroutine_handle<> or std::function
*/
template <typename T>
class TimedTaskList : public LinkedListNode {
public:
	using Task = TimedTask<T>;

	/**
		Check if the list is empty
		@return true if empty
	*/
	bool empty() const {
		return this->next == this;
	}

	/**
		Get the first time in the list
	*/
	Time getFirstTime() const {
		assert(this->next != this);
		return static_cast<Task &>(*this->next).time;
	}

	/**
		Get the first time in the list if it is before a given maximum time
	*/
	Time getFirstTime(Time maxTime) const {
		if (this->next == this)
			return maxTime;
		auto &first = static_cast<Task &>(*this->next);
		return first.time < maxTime ? first.time : maxTime;
	}

	/**
		Add a task. Must not already be in a list
		@param task task to add
		@param time scheduled execution time
	*/
	void add(Task &task) {
		assert(!task.inList());

		// find point where to insert the task
		auto current = this->next;
		while (current != this) {
			if (static_cast<Task &>(*current).time > task.time)
				break;
			current = current->next;
		}

		// insert task before current
		task.prev = current->prev;
		task.next = current;
		current->prev->next = &task;
		current->prev = &task;
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
		Remove and execute tasks that are in the list on entry of doUntil() until given gime
	*/
	void doUntil(Time time) {
		// find point until where to execute all tasks
		auto current = this->next;
		while (current != this) {
			if (static_cast<Task &>(*current).time > time)
				break;
			current = current->next;
		}
		if (current->prev == this)
			return;

		// temporary head for tasks to execute
		LinkedListNode head(this->next, current->prev);

		// link nodes to temporary head
		this->next->prev = &head;
		current->prev->next = &head;

		// list contains remaining nodes
		this->next = current;
		current->prev = this;

		// execute tasks
		while (head.next != &head) {
			auto &current = static_cast<Task &>(*head.next);
			current.remove();
			current.task();
		}
	}
};

} // namespace coco
