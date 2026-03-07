#pragma once

#include <atomic>


namespace coco {

/// @brief Intrusisive queue node.
/// Elements of the queue must inherit the IntrusiveQueueNode.
struct IntrusiveQueueNode {
    /// @brief Pointer to next element.
    ///
    IntrusiveQueueNode *next;


    /// @brief Default constructor.
    ///
    IntrusiveQueueNode() = default;

    /// @brief Constructor with next pointer.
    /// @param next Next pointer
    IntrusiveQueueNode(IntrusiveQueueNode *next) : next(next) {}

    /// @brief Delete copy constructor.
    ///
    IntrusiveQueueNode(IntrusiveQueueNode const &) = delete;
};


/// @brief Intrusive queue.
/// Elements of the queue must inherit the IntrusiveQueueNode. This queue is not thread-safe.
/// push() pushes at back/end/tail of queue
/// pop() pops from front/begin/head of queue
/// Also see https://en.cppreference.com/w/cpp/container/queue
/// @tparam T queue element type that inherits IntrusiveQueueNode, e.g. class Element : public IntrusiveQueueNode {};
template <typename T>
class IntrusiveQueue {
public:
    using Node = IntrusiveQueueNode;

    /// @brief Default constructor.
    ///
    IntrusiveQueue() {
    }

    /// @brief Determine if the queue is empty
    ///
    bool empty() {
        return head_.next == nullptr;
    }

    /// @brief Clear the queue.
    ///
    void clear() {
        head_ = nullptr;
        tail_ = &head_;
    }

    /// @brief Insert an element at the end/behind back() of the queue (push_back).
    /// @return true if the queue was empty
    bool push(T &element) {
        Node &node = element;
        node.next = nullptr;

        bool wasEmpty = head_.next == nullptr;
        tail_->next = &node;
        tail_ = &node;
        return wasEmpty;
    }

    /// @brief Pop the first/front() element from the queue (equivalent to pop_front).
    /// @return Removed element or nullptr if queue was empty
    T *pop() {
        Node *head = head_.next;
        if (head == nullptr)
            return nullptr;

        // remove the head node
        Node *next = head->next;
        head_.next = next;
        if (next == nullptr)
            tail_ = &head_;

        // pop succeeded
        return &static_cast<T &>(*head);
    }

    /// @brief Pop the first/front() element from the queue (equivalent to pop_front).
    /// @tparam R Type of the remove function, e.g. [](auto &element) { element.cleanup(); }
    /// @param removeFunction Function to be called when the first element is removed from the queue
    /// @return Removed element or nullptr if queue was empty
    template <typename R>
    T *pop(const R &removeFunction) {
        Node *head = head_.next;
        if (head == nullptr)
            return nullptr;

        // remove the node
        Node *next = head->next;
        head_.next = next;
        if (next == nullptr)
            tail_ = &head_;

        // call remove function when the element is not part of the queue any more
        auto &element = static_cast<T &>(*head);
        removeFunction(element);

        // pop succeeded
        return &element;
    }

    /// @brief Pop the first/front() element from the queue (equivalent to pop_front).
    /// @tparam N Type of the next function, e.g. [](auto &element) { element.startNext(); }
    /// @tparam R Type of the remove function, e.g. [](auto &element) { element.cleanup(); }
    /// @param nextFunction Function to be called with the next element when the first element is removed from the queue and there is a next element
    /// @param removeFunction Function to be called when the first element is removed from the queue
    /// @return Removed element or nullptr if queue was empty
    template <typename N, typename R>
    T *pop(const N &nextFunction, const R &removeFunction) {
        Node *head = head_.next;
        if (head == nullptr)
            return nullptr;

        // remove the node
        Node *next = head->next;
        head_.next = next;
        if (next == nullptr)
            tail_ = &head_;
        else
            nextFunction(static_cast<T &>(*next));

        // call remove function when the element is not part of the queue any more
        auto &element = static_cast<T &>(*head);
        removeFunction(element);

        // pop succeeded
        return &element;
    }

    /// @brief Pop the first/front() element from the queue if the condition returns true.
    /// The condition function must not modify the queue.
    /// @tparam C Type of the condition function, e.g. [](auto &element) { return element.isReady(); }
    /// @param condition Condition function to determine if the first/front() element should be removed.
    /// @return Removed element or nullptr if queue was empty or if the condition returned false
    template <typename C>
    T *popIf(const C &condition) {
        Node *head = head_.next;
        if (head == nullptr)
            return nullptr;

        // reject if condition returns false
        if (!condition(static_cast<T &>(*head)))
            return nullptr;

        // remove the node
        Node *next = head->next;
        head_.next = next;
        if (next == nullptr)
            tail_ = &head_;

        // pop succeeded
        return &static_cast<T &>(*head);
    }

    /// @brief Pop the first/front() element from the queue if the condition returns true.
    /// The condition function must not modify the queue while the removeFunction may modify this queue or add the
    /// element to another queue.
    /// @tparam C Type of the condition function, e.g. [](auto &element) { return element.isReady(); }
    /// @tparam R Type of the remove function, e.g. [](auto &element) { element.cleanup(); }
    /// @param condition Condition function to determine if the first/front() element should be removed
    /// @param removeFunction Function to be called when an element is removed from the queue
    /// @return Removed element or nullptr if queue was empty or if the condition returned false
    template <typename C, typename R>
    T *popIf(const C &condition, const R &removeFunction) {
        Node *head = head_.next;
        if (head == nullptr)
            return nullptr;

        // reject if condition returns false
        if (!condition(static_cast<T &>(*head)))
            return nullptr;

        // remove the node
        Node *next = head->next;
        head_.next = next;
        if (next == nullptr)
            tail_ = &head_;

        // call remove function when the element is not part of the queue any more
        auto &element = static_cast<T &>(*head);
        removeFunction(element);

        // pop succeeded
        return &element;
    }

    /// @brief Pop the first/front() element from the queue if the condition returns true.
    /// The condition function must not modify the queue while the removeFunction may modify this queue or add the
    /// element to another queue. The nextfunction is called before removeFunction and must not modify the queue.
    /// @tparam C Type of the condition function, e.g. [](auto &element) { return element.isReady(); }
    /// @tparam N Type of the next function, e.g. [](auto &element) { element.startNext(); }
    /// @tparam R Type of the remove function, e.g. [](auto &element) { element.cleanup(); }
    /// @param condition Condition function to determine if the first/front() element should be removed
    /// @param nextFunction Function to be called with the next element when the first element is removed from the queue and there is a next element
    /// @param removeFunction Function to be called when an element is removed from the queue
    /// @return Removed element or nullptr if queue was empty or if the condition returned false
    template <typename C, typename N, typename R>
    T *popIf(const C &condition, const N &nextFunction, const R &removeFunction) {
        Node *head = head_.next;
        if (head == nullptr)
            return nullptr;

        // reject if condition returns false
        if (!condition(static_cast<T &>(*head)))
            return nullptr;

        // remove the node
        Node *next = head->next;
        head_.next = next;
        if (next == nullptr)
            tail_ = &head_;
        else
            nextFunction(static_cast<T &>(*next));

        // call remove function when the element is not part of the queue any more
        auto &element = static_cast<T &>(*head);
        removeFunction(element);

        // pop succeeded
        return &element;
    }

    /// @brief Remove an element
    /// @param element Element to remove
    /// @return true if the element was removed, false if the element was not in the queue
    bool remove(T &element) {
        Node &node = element;

        Node *current = &head_;

        while (true) {
            Node *next = current->next;
            if (next == nullptr)
                break;
            if (&node == next) {
                // remove the node
                current->next = next->next;

                if (next == tail_)
                    tail_ = current;

                // successfully removed the node
                return true;
            }
            current = current->next;
        }

        // the node is not in the list
        return false;
    }

    /// @brief Get first/head element.
    /// Make sure the queue is not empty before calling this function.
    /// @return first element
    T &front() {
        return static_cast<T &>(*head_.next);
    }

    /// @brief Get last/tail element.
    /// Make sure the queue is not empty before calling this function.
    /// @return last element
    T &back() {
        return static_cast<T &>(*tail_);
    }

protected:
    Node head_ = {nullptr}; // pop() removes from head
    Node *tail_ = &head_; // push() adds to tail
};

} // namespace coco
