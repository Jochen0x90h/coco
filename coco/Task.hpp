#pragma once

#include "IntrusiveList.hpp"
#include <cassert>
#include <utility>


namespace coco {


/**
 * Task
 * @tparam F task function, e.g. Callback, std::coroutine_handle<> or std::function
 */
template <typename F>
class Task : public IntrusiveListNode {
public:
    Task(const F &task) : task(task) {}

    void cancel() noexcept {remove();}

    // function or coroutine waiting for the event to occur
    F task;
};

/**
 * Task with parameters (mainly for parameters of awaitable functions/methods)
 * @tparam T task type
 * @tparam P parameters
 */
template <typename F, typename P>
class TaskWithParameters : public Task<F> {
public:
    template <typename ...Args>
    explicit TaskWithParameters(const F &task, Args &&...args)
        : Task<F>(task), parameters{std::forward<Args>(args)...} {}

    P parameters;
};

template <typename T>
T &getParameters(T &task) {
    return task;
}

template <typename F, typename P>
P &getParameters(TaskWithParameters<F, P> &task) {
    return task.parameters;
}



/**
 * List of tasks (e.g. waiting coroutines)
 * @tparam T task type
 */
template <typename T>
class TaskList : public IntrusiveListNode {
public:
    using Task = T;

    /**
     * Check if the list is empty
     * @return true if empty
     */
    bool empty() const {
        return this->next == this;
    }

    /**
     * Add a task. Must not already be in a list
     * @param task task to add
     */
    void add(Task &task) {
        assert(!task.inList());

        task.prev = this->prev;
        task.next = this;
        this->prev->next = &task;
        this->prev = &task;
    }

    /**
     * Visit all tasks
     * @tparam V visitor type, e.g. a lambda function
     * @param visitor visitor
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
     * Remove and execute the first task
     * @return true when a coroutine was resumed, false when the list was empty
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
     * Remove and execute all tasks that are in the list on entry of doAll()
     */
    void doAll() {
        if (this->next == this)
            return;

        // temporary head for tasks to execute
        IntrusiveListNode head(this->next, this->prev);

        // link nodes to temporary head
        this->next->prev = &head;
        this->prev->next = &head;

        // clear list
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
     * Remove and execute the first task when its predicate is true
     * @param predicate boolean predicate function that determines if the first task should be executed
     * @return true when a task was executed
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
     * Remove and execute all tasks that are in the list on entry of doAll() and for which the predicate returns true
     * @param predicate boolean predicate function that selects the tasks to execute
     */
    template <typename P>
    void doAll(P const &predicate) {
        if (this->next == this)
            return;

        // temporary head for tasks to execute
        IntrusiveListNode head;

        auto next = this->next;
        while (next != this) {
            auto current = next;
            next = next->next;

            if (predicate(getParameters(static_cast<Task &>(*current)))) {
                current->remove();

                // add to temporary head
                current->prev = head.prev;
                current->next = &head;
                head.prev->next = current;
                head.prev = current;
            }
        }

        // execute tasks
        while (head.next != &head) {
            auto &first = static_cast<Task &>(*head.next);

            // remove task from list
            first.remove();

            // execute task
            first.task();
        }


/*
        // add iterator node at the beginning
        IntrusiveListNode it(*this->next);

        // add end marker node
        IntrusiveListNode end(*this);

        // iterate over tasks
        while (it.next != &end) {
            auto &current = static_cast<Task &>(*it.next);

            if (predicate(getParameters(current))) {
                // remove task from list
                current.remove();

                // execute task
                current.task();

                int xx = 0;
            } else {
                // advance iterator node
                it.advance();
            }
        }*/
    }
};

} // namespace coco
