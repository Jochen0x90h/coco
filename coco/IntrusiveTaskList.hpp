#pragma once

#include <utility> // std::declval


namespace coco {

/// @brief Intrusive task list.
/// IntrusiveTaskList has doFirst() and doAll() methods that remove and execute tasks. The tasks must have an
/// operator () and inherit from a node type that has next and prev members, e.g. IntrusiveListNode.
/// @tparam T Task type with operator (), inherits from the node type N
/// @tparam N Node type with next and prev members, defaults to IntrusiveListNode
template <typename T, typename N = IntrusiveListNode>
class IntrusiveTaskList {
public:
    // task
    using Task = T;

    // node (has next and prev members), base class of task
    using Node = N;


    /// @brief Return true if the list is empty.
    ///
    bool empty() const {
        return node_.next == &node_;
    }

    /// Add a task.
    /// If the task is already in this or another list, it gets removed first.
    /// @param task Task to add
    void add(Task &task) {
        // cast
        auto &node = static_cast<Node &>(task);

        // remove
        node.next->prev = node.prev;
        node.prev->next = node.next;

        // insert element at end of list
        node.prev = node_.prev;
        node.next = &node_;
        node_.prev->next = &node;
        node_.prev = &node;
    }

    /// @brief Add a task without removing it first. The task must not already be in this or another list.
    /// @param task Task to add
    void addUnsafe(Task &task) {
        // cast
        auto &node = static_cast<Node &>(task);

        // insert element at end of list
        node.prev = node_.prev;
        node.next = &node_;
        node_.prev->next = &node;
        node_.prev = &node;
    }

    /// @brief Visit all tasks.
    /// @tparam V visitor type, e.g. a lambda function
    /// @param visitor visitor
    template <typename V>
    void visitAll(const V &visitor) {
        auto current = node_.next;
        while (current != &node_) {
            visitor(static_cast<Task &>(*current));
            current = current->next;
        }
    }

    /// @brief Remove and execute the first task by calling operator ().
    /// @return true when a task was executed, false when the list was empty
    bool doFirst() {
        if (node_.next != &node_) {
            auto &task = static_cast<Task &>(*node_.next);

            // remove task from list
            task.remove();

            // execute task
            task();

            return true;
        }
        return false;
    }

    /// @brief Remove and execute the first task if a predicate returns true by calling operator ().
    /// @tparam P Type of predicate function
    /// @param predicate Boolean predicate function that determines if the first task should be executed
    /// @return true when a task was executed, false when the list was empty
    template <typename P>
    bool doFirst(const P &predicate) {
        if (node_.next != &node_) {
            auto &task = static_cast<Task &>(*node_.next);
            if (predicate(task)) {
                // remove task from list
                task.remove();

                // execute task
                task();

                return true;
            }
        }
        return false;
    }

    /// @brief Remove and execute all tasks by calling operator ().
    /// Applies only to tasks that are in the list on entry of doAll(). The executed tasks may add new tasks to the
    /// list or remove tasks from the list.
    void doAll() {
        if (node_.next == &node_)
            return;

        // temporary head for tasks to execute
        IntrusiveListNode head(node_.next, node_.prev);

        // link nodes to temporary head
        node_.next->prev = &head;
        node_.prev->next = &head;

        // clear list
        node_.next = &node_;
        node_.prev = &node_;

        // execute tasks
        while (head.next != &head) {
            auto &task = static_cast<Task &>(*head.next);

            // remove task from list
            task.remove();

            // execute task
            task();
        }
    }

    /// @brief Remove and execute all tasks for which a predicate returns true by calling operator ().
    /// @tparam P Type of predicate function
    /// @param predicate Boolean predicate function that selects the tasks to execute
    template <typename P>
    void doAll(P const &predicate) {
        if (node_.next == &node_)
            return;

        // temporary head for tasks to execute
        IntrusiveListNode head;

        // get tasks for which predicate returns true and link them to temporary head
        auto next = node_.next;
        while (next != &node_) {
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

    /// @brief Expose the internal root node for use in IntrusiveListNode constructor.
    /// @return Root node
    Node &node() {return node_;}

protected:
    Node node_;
};

} // namespace coco
