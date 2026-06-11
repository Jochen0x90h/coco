#pragma once

#include <utility> // std::declval


namespace coco {

/// @brief Intrusive sorted task list.
/// List elements are sorted by a value (e.g. time in milliseconds).
/// Current implementation is naive so that nodes can remove themselves.
/// @tparam T Task type, either inherits from the value node type N or is identical to it
/// @tparam V Value node type (with value member and inheriting next and prev members), base cass of task type
/// @tparam N Node type (with next and prev members), base class of value node
template <typename T, typename V = T, typename N = IntrusiveListNode>
class IntrusiveSortedTaskList {
public:
    // the list element
    using Task = T;

    // value node (has value member and inherits next and prev members), base class of Task
    using ValueNode = V;

    // value type (e.g. time in milliseconds)
    using Value = decltype(std::declval<ValueNode>().value);

    // node (has next and prev members), base class of ValueNode
    using Node = N;


    /// @brief Return true if the list is empty.
    ///
    bool empty() const {
        return node_.next == &node_;
    }

    /// Get the first value in the list if it is before a given maximum value, otherwise return the maximum value.
    /// @param maxValue Maximum value
    Value nextValue(Value maxValue) const {
        if (empty())
            return maxValue;
        Value value = static_cast<ValueNode &>(*node_.next).value;
        return value < maxValue ? value : maxValue;
    }

    /// Add a task.
    /// If the task is already in this or another list, it gets removed first.
    /// @param task Task to add
    /// @param value Value for the task
    void add(Task &task, Value value) {
        // find point where to insert the new task
        auto current = node_.next;
        while (current != &node_) {
            if (static_cast<ValueNode &>(*current).value > value)
                break;
            current = current->next;
        }

        // cast
        auto &valueNode = static_cast<ValueNode &>(task);
        auto &node = static_cast<Node &>(valueNode);

        // set value
        valueNode.value = value;

        // remove
        node.next->prev = node.prev;
        node.prev->next = node.next;

        // insert element before current
        node.prev = current->prev;
        node.next = current;
        current->prev->next = &node;
        current->prev = &node;
    }

    /// @brief Add a task without removing it first. The task must not already be in this or another list.
    /// The value for the task must already be set.
    /// @param task Task to add
    void addUnsafe(Task &task) {
        // find point where to insert the new task
        auto current = node_.next;
        while (current != &node_) {
            if (static_cast<ValueNode &>(*current).value > task.value)
                break;
            current = current->next;
        }

        // cast
        auto &valueNode = static_cast<ValueNode &>(task);
        auto &node = static_cast<Node &>(valueNode);

        // insert element before current
        node.prev = current->prev;
        node.next = current;
        current->prev->next = &node;
        current->prev = &node;
    }

    /// @brief Remove and execute all tasks until a given value by calling operator ().
    /// Applies only to tasks that are in the list on entry of doUntil(). The executed tasks may add new tasks to the
    /// list or remove tasks from the list.
    /// @param value Value until which to remove and execute tasks
    void doUntil(Value value) {
        // find point until where to execute all tasks
        auto current = node_.next;
        while (current != &node_) {
            if (static_cast<ValueNode &>(*current).value > value)
                break;
            current = current->next;
        }

        // return if first node has greater value or if list is empty
        if (current->prev == &node_)
            return;

        // temporary head for tasks to execute
        Node head(node_.next, current->prev);

        // link nodes to temporary head
        node_.next->prev = &head;
        current->prev->next = &head;

        // list contains remaining nodes
        node_.next = current;
        current->prev = &node_;

        // execute tasks
        while (head.next != &head) {
            Node &node = *head.next;

            // remove
            node.next->prev = node.prev;
            node.prev->next = node.next;
            node.next = &node;
            node.prev = &node;

            // call operator ()
            auto &valueNode = static_cast<ValueNode &>(node);
            auto &task = static_cast<Task &>(valueNode);
            task();
        }
    }

    /// @brief Execute all tasks until a given value.
    /// Applies only to tasks that are in the list on entry of doUntil(). The executed tasks may add new tasks to the
    /// list or remove tasks from the list.
    /// @tparam F Type of executeFunction
    /// @param value Value until which to remove and execute tasks
    /// @param executeFunction Function called on an element to execute the task (is removed from the list before function gets called)
    template <typename F>
    void doUntil(Value value, const F &executeFunction) {
        // find point until where to execute all tasks
        auto current = node_.next;
        while (current != &node_) {
            if (static_cast<ValueNode &>(*current).value > value)
                break;
            current = current->next;
        }

        // return if first node has greater value or if list is empty
        if (current->prev == &node_)
            return;

        // temporary head for tasks to execute
        Node head(node_.next, current->prev);

        // link nodes to temporary head
        node_.next->prev = &head;
        current->prev->next = &head;

        // list contains remaining nodes
        node_.next = current;
        current->prev = &node_;

        // execute tasks
        while (head.next != &head) {
            Node &node = *head.next;

            // remove
            node.next->prev = node.prev;
            node.prev->next = node.next;
            node.next = &node;
            node.prev = &node;

            // call execute function
            auto &valueNode = static_cast<ValueNode &>(node);
            auto &task = static_cast<Task &>(valueNode);
            executeFunction(task);
        }
    }

protected:
    Node node_;
};

} // namespace coco
