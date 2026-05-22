#pragma once

#include <utility> // std::declval


namespace coco {

/// @brief Intrusive timeout queue.
/// @tparam E Element type, needs a Node type, a next and prev pointer and a time
/// @tparam T Time node type (node with next, prev and time) in case the element inerits from multiple nodes
template <typename E, typename T = E>
class IntrusiveTimeoutQueue {
public:
    // the queue element
    using Element = E;

    // time node (has next, prev and time members), base class of Element
    using TimeNode = T;

    // time type
    using Time = decltype(std::declval<TimeNode>().time);

    // node (has next and prev members), base class of TimeNode
    using Node = TimeNode::Node;


    /// @brief Return true if the list is empty.
    ///
    bool empty() const {
        return node_.next == &node_;
    }

    /// Get the first time in the list if it is before a given maximum time, otherwise return the maximum time.
    /// @param maxTime Maximum time
    Time getFirstTime(Time maxTime) const {
        if (empty())
            return maxTime;
        Time time = static_cast<TimeNode &>(*node_.next).time;
        return time < maxTime ? time : maxTime;
    }

    /// Add am element. Must not already be in a list or queue.
    /// @param task task to add
    void add(Element &element, Time time) {
        // find point where to insert the new element
        auto current = node_.next;
        while (current != &node_) {
            if (static_cast<TimeNode &>(*current).time > time)
                break;
            current = current->next;
        }

        // set time
        auto &timeNode = static_cast<TimeNode &>(element);
        timeNode.time = time;

        // remove
        auto &node = static_cast<Node &>(timeNode);
        node.next->prev = node.prev;
        node.prev->next = node.next;

        // insert element before current
        node.prev = current->prev;
        node.next = current;
        current->prev->next = &node;
        current->prev = &node;
    }

    /// @brief Remove and execute tasks until given gime.
    /// Applies only to tasks that are in the list on entry of doUntil(). The executed tasks may add new tasks to the
    /// list or remove tasks from the list.
    /// @tparam F Type of timeoutFunction
    /// @param time Time until wich to remove and execute tasks
    /// @param timeoutFunction Function called on an element which has timed out (is removed from the queue before function gets called)
    template <typename F>
    void doUntil(Time time, const F &timeoutFunction) {
        // find point until where to execute all tasks
        auto current = node_.next;
        while (current != &node_) {
            if (static_cast<TimeNode &>(*current).time > time)
                break;
            current = current->next;
        }

        // return if first node has greater time or if list is empty
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

            // call timeout function
            auto &timeNode = static_cast<TimeNode &>(node);
            auto &element = static_cast<Element &>(timeNode);
            timeoutFunction(element);
        }
    }

protected:
    Node node_;
};

} // namespace coco
