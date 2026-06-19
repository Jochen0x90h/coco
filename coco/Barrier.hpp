#pragma once

#include "Coroutine.hpp"


namespace coco {

/// @brief Simple barrier on which a data consumer coroutine can wait until it gets resumed by a data producer.
/// If a resume method gets called by a data producer while no consumer is waiting, the event/data gets lost.
/// @tparam T Optional value type
template <typename T = void>
class Barrier : public CoroutineTaskList<T> {
public:

    // task type
    using Task = CoroutineTask<T>;


    /// @brief Wait until resumed by doFirst() or doAll().
    /// When using arguments, this can be used in a data producer/consumer scheme where consumers call untilResumed and
    /// producers call doFirst(data) or doAll(data) with data.
    ///
    /// @return use co_await on return value to wait until resumed
    template <typename ...Args>
    [[nodiscard]] Awaitable<Task> untilResumed(Args &&...args) {
        return {*this, std::forward<Args>(args)...};
    }

    using CoroutineTaskList<T>::doFirst;
    using CoroutineTaskList<T>::doAll;

    template <typename P>
    bool doFirst(const P &predicate) {
        auto &node = this->node_;
        if (node.next != &node) {
            auto &task = static_cast<Task &>(*node.next);
            if (predicate(task.value)) {
                // remove task from list
                task.remove();

                // execute task
                task();

                return true;
            }
        }
        return false;
    }

    template <typename P>
    void doAll(P const &predicate) {
        auto &node = this->node_;
        if (node.next == &node)
            return;

        // temporary head for tasks to execute
        IntrusiveListNode head;

        // get tasks for which predicate returns true and link them to temporary head
        auto next = node.next;
        while (next != &node) {
            auto current = next;
            next = next->next;

            if (predicate(static_cast<Task &>(*current))) {
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
            first();
        }
    }
};

} // namespace coco
