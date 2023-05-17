#pragma once

#include "assert.hpp"
#include "IsSubclass.hpp"
#include <utility>

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

/**
 * Linked list node for TaskList head and and tasks
 */
class TaskNode {
public:
	// default constructor with no initialization
	TaskNode() = default;

	// delete copy constructor
	TaskNode(TaskNode const &) = delete;

	/**
	 * Destructor. It is an error to destroy a waitlist node that is still part of a list
	 */
	~TaskNode() {
		assert(!isInList());
	}

	/**
	 * Set the node to "not in list" state
	 */
	void setNotInList() {
		this->next = nullptr;
	}

	/**
	 * Add a node before this node
	 * @param node node to add
	 */
	void addBefore(TaskNode &node) noexcept {
		node.prev = this->prev;
		node.next = this;
		this->prev->next = &node;
		this->prev = &node;
	}

	/**
	 * Add a node after this node
	 * @param node node to add
	 */
	void addAfter(TaskNode &node) noexcept {
		node.prev = this;
		node.next = this->next;
		this->next->prev = &node;
		this->next = &node;
	}

	/**
	 * Check if a node is part of a list. Can only be called after either setNotInList() has been called or the node has been added to a list
	 */
	bool isInList() const {
		return this->next != nullptr;
	}

	/**
	 * Pass waiting coroutine to another node and set this node to "not in list". Can only be called if this node is in
	 * a list
	 * @param node node to pass the waiting coroutine to
	 */
	void pass(TaskNode &node) noexcept {
		this->prev->next = &node;
		this->next->prev = &node;
		node.prev = this->prev;
		node.next = this->next;

		// set to "not in list"
		this->next = nullptr;
	}

	/**
	 * Remove the node from the list and set to "not in list". Can only be called if the node is in a list
	 */
	void remove() noexcept {
		this->next->prev = this->prev;
		this->prev->next = this->next;

		// set to "not in list"
		this->next = nullptr;
	}

	TaskNode *next;
	TaskNode *prev;
};


/**
 * Task containing a suspended coroutine.
 * Custom task classes can be derived from this class. A task needs these methods, a default implementation is provided:
 *
 * cancel(): Cancel the task (e.g. stop IO transfer or destroy the awaitable coroutine).
 * take(Task &task): Optional for move assignment, take the waiting coroutine from the given node
 *
 * The cancel() method gets called either by Awaitable::cancel() or if an Awaitable goes out of scope where the task
 * is still part of a task list. It has to remove the task from the task list by calling remove() and optionally can
 * cancel some ongoing operation such as an IO transfer and maybe needs to protect the task list against concurrent 
 * modifications.
 */
class Task : public TaskNode {
public:
	void cancel() noexcept {remove();}

	void take(Task &task) {
		task.pass(*this);

		// don't need to move handle because it is set only during co_await
	}

	// handle of waiting coroutine
	std::coroutine_handle<> handle = nullptr;
};


/**
 * Task with parameters to be stored on the stack of the task
 * @tparam T parameters
 */
template <typename T>
class TaskWithParameters : public Task {
public:
	template <typename ...Args>
	explicit TaskWithParameters(Args &&...args) : parameters{std::forward<Args>(args)...} {}

	T parameters;
};


template <typename T, int>
struct TaskSelector {
	using Task = TaskWithParameters<T>;
	static T &get(Task *e) {return e->parameters;}
};

// specialization for T being derived from Task
template <typename T>
struct TaskSelector<T, 1> {
	using Task = T;
	static T &get(Task *e) {return *e;}
};


/**
 * List of tasks (e.g. waiting coroutines)
 * @tparam T list task
 */
template <typename T = Task>
class TaskList {
public:

	// select task type
	using Selector = TaskSelector<T, IsSubclass<T, TaskNode>::value>;
	using Task = typename Selector::Task;

	TaskList() {
		this->head.next = this->head.prev = &this->head;
	}

	/**
	 * Destructor, list should be empty when the destructor is called
	 */
	~TaskList() {
		this->head.remove();
	}

	void add(TaskNode &node) {
		node.prev = this->head.prev;
		node.next = &this->head;
		this->head.prev->next = &node;
		this->head.prev = &node;
	}

	/**
	 * Check if the list is empty
	 * @return true if empty
	 */
	bool empty() {
		return this->head.next == &this->head;
	}


	/**
	 * Check if a task is the first task in the list
	 * @praram task task to check
	 */
	bool isFirst(const Task &task) {
		auto *current = this->head.next;
		auto *end = &this->head;
		if (current != end) {
			if (&task == &Selector::get(static_cast<Task*>(current)))
				return true;
		}
		return false;
	}

	/**
	 * Start first task
	 * @param start function that starts the first task (e.g. start IO operation)
	 */
	/*template <typename F>
	void startFirst(const F &start) {
		auto first = static_cast<Task*>(this->head.next);
		auto end = &this->head;
		if (first != end) {
			// start next task
			start(Selector::get(static_cast<Task*>(first)));
		}
	}*/

	/**
	 * Start next task and resume first task
	 * @param start function that starts the next task (e.g. start IO operation)
	 */
	/*template <typename F>
	void startNextResumeFirst(const F &start) {
		auto first = static_cast<Task*>(this->head.next);
		auto end = &this->head;
		if (first != end) {
			auto next = first->next;
			if (next != end) {
				// start next task
				start(Selector::get(static_cast<Task*>(next)));
			}
			
			// remove task from list (special implementation of remove() may lock interrupts to avoid race condition)
			first->remove();

			// resume coroutine if it is waiting
			if (first->handle)
				first->handle.resume();
		}
	}*/

	/**
	 * Start next task and finish/resume first task
	 * @param startNext function that starts the next task (e.g. start IO operation)
	 * @param finishFirst functon that finishes the first task (e.g. set received size and status)
	 */
	/*template <typename F1, typename F2>
	void startNextResumeFirst(const F1 &start, const F2 &finish) {
		auto first = static_cast<Task*>(this->head.next);
		auto end = &this->head;
		if (first != end) {
			auto next = first->next;
			if (next != end) {
				// start next task
				start(Selector::get(static_cast<Task*>(next)));
			}

			// finish first task
			finish(Selector::get(static_cast<Task*>(first)));
			
			// remove task from list (special implementation of remove() may lock interrupts to avoid race condition)
			first->remove();

			// resume coroutine if it is waiting
			if (first->handle)
				first->handle.resume();
		}
	}*/




	/**
	 * Check if the list contains the given task
	 * @praram task task to check
	 */
	bool contains(const Task &task) {
		auto *current = this->head.next;
		auto *end = &this->head;
		while (current != end) {
			if (&task == &Selector::get(static_cast<Task*>(current)))
				return true;
			current = current->next;
		}
		return false;
	}

	/**
	 * Check if the list contains a task for which the predicate is true
	 * @param predicate boolean predicate function for the list tasks
	 * @return true if an task is contained
	 */
	template <typename P>
	bool contains(const P &predicate) {
		auto *current = this->head.next;
		auto *end = &this->head;
		while (current != end) {
			if (predicate(Selector::get(static_cast<Task*>(current))))
				return true;
			current = current->next;
		}
		return false;
	}



	/**
	 * Get first task. List must not be empty
	 * @return first task
	 */
	/*auto &getFirst() {
		assert(!empty());
		auto first = this->head.next;
		return Selector::get(static_cast<Task*>(first));
	}*/

	/**
	 * Visit the first task
	 * @tparam V visitor type, e.g. a lambda function
	 * @param visitor visitor
	 * @return true if a first task exists
	 */
	template <typename V>
	bool visitFirst(const V &visitor) {
		auto first = this->head.next;
		auto end = &this->head;
		if (first != end) {
			visitor(Selector::get(static_cast<Task*>(first)));
			return true;
		}
		return false;
	}

	/**
	 * Visit the second task. Practical for starting the next pending operation before dealing with the current
	 * operation that has just finished.
	 * @tparam V visitor type, e.g. a lambda function
	 * @param visitor visitor
	 * @return true if a second task exists
	 */
	template <typename V>
	bool visitSecond(const V &visitor) {
		auto first = this->head.next;
		auto end = &this->head;
		if (first != end) {
			auto second = first->next;
			if (second != end) {
				visitor(Selector::get(static_cast<Task*>(second)));
				return true;
			}
		}
		return false;
	}

	/**
	 * Visit all tasks
	 * @tparam V visitor type, e.g. a lambda function
	 * @param visitor visitor
	 */
	template <typename V>
	void visitAll(const V &visitor) {
		auto it = this->head.next;
		auto end = &this->head;
		while (it != end) {
			visitor(Selector::get(static_cast<Task*>(it)));
			it = it->next;
		}
	}

	/**
	 * Take the first task
	 * @tparam V visitor type, e.g. a lambda function
	 * @param list task list where to add the task
	 * @param visitor visitor
	 * @return true if a first task exists
	 */
	/*template <typename V>
	bool takeFirst(TaskList &list, const V &visitor) {
		auto first = this->head.next;
		auto end = &this->head;
		if (first != end) {
			visitor(Selector::get(static_cast<Task*>(first)));
			
			// move task to other list
			first->remove();
			list.add(*first);

			return true;
		}
		return false;
	}*/

	/**
	 * Remove and resume first waiting coroutine
	 * @return true when a coroutine was resumed, false when the list was empty
	 */
	bool resumeFirst() {
		auto first = static_cast<Task*>(this->head.next);
		auto end = &this->head;
		if (first != end) {
			// remove task from list (special implementation of remove() may lock interrupts to avoid race condition)
			first->remove();

			// resume coroutine if it is waiting
			if (first->handle)
				first->handle.resume();
			return true;
		}
		return false;
	}

	/**
	 * Resume all waiting coroutines that were waiting when resumeAll() was called
	 */
	void resumeAll() {
		// add end marker node
		TaskNode end;
		this->head.addBefore(end);

		// iterate over tasks
		while (this->head.next != &end) {
			auto *current = static_cast<Task*>(this->head.next);

			// remove task from list (special implementation of remove() may lock interrupts to avoid race condition)
			current->remove();

			// resume coroutine if it is waiting
			if (current->handle)
				current->handle.resume();
		}

		// remove temporary nodes
		end.remove();
	}

	/**
	 * Remove and resume first waiting coroutine when the predicate is true
	 * @param predicate boolean predicate function for the first list task
	 * @return true when list was not empty
	 */
	template <typename P>
	void resumeFirst(const P &predicate) {
		auto first = static_cast<Task*>(this->head.next);
		auto end = &this->head;
		if (first != end) {
			if (predicate(Selector::get(first))) {
				// remove task from list (special implementation of remove() may lock interrupts to avoid race condition)
				first->remove();

				// resume coroutine if it is waiting
				if (first->handle)
					first->handle.resume();
			}
		}
	}

	/**
	 * Resume one waiting coroutine for which the predicate is true (and remove it from the list)
	 * @param predicate boolean predicate function for the list tasks
	 */
	template <typename P>
	void resumeOne(const P &predicate) {
		auto current = static_cast<Task*>(this->head.next);
		auto end = &this->head;
		while (current != end) {
			if (predicate(Selector::get(current))) {
				// remove task from list (special implementation of remove() may lock interrupts to avoid race condition)
				current->remove();

				// resume coroutine if it is waiting
				if (current->handle)
					current->handle.resume();
				return;
			}
			current = static_cast<Task*>(current->next);
		}
	}

	/**
	 * Resume all waiting coroutines for which the predicate is true (and remove them from the list)
	 * @param predicate boolean predicate function for the list tasks
	 */
	template <typename P>
	void resumeAll(P const &predicate) {
		// add iterator node at the beginning
		TaskNode it;
		this->head.addAfter(it);

		// add end marker node
		TaskNode end;
		this->head.addBefore(end);

		// iterate over tasks
		while (it.next != &end) {
			auto current = static_cast<Task*>(it.next);

			if (predicate(Selector::get(current))) {
				// remove task from list (special implementation of remove() may lock interrupts to avoid race condition)
				current->remove();

				// resume coroutine if it is waiting
				if (current->handle)
					current->handle.resume();
			} else {
				// advance iterator node
				it.remove();
				current->addAfter(it);
			}
		}

		// remove temporary nodes
		it.remove();
		end.remove();
	}

	TaskNode head;
};

} // namespace coco
