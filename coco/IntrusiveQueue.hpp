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

    /// @brief Determine if the queue is empty
    ///
    bool empty() {
        return this->tail == nullptr;
    }

    /// @brief Clear the queue.
    ///
    void clear() {
        this->tail = nullptr;
        this->head = nullptr;
    }

    /// @brief Insert an element at the end/behind back() of the queue (push_back).
    /// @return true if the queue was empty
    bool push(T &element) {
        Node &node = element;
        node.next = nullptr;
        Node *prev = this->tail;
        bool wasEmpty = prev == nullptr;
        if (wasEmpty)
            this->head = &node;
        else
            prev->next = &node;
        this->tail = &node;
        return wasEmpty;
    }

    /// @brief Remove the first/front() element from the queue (pop_front).
    /// @return removed element or nullptr if queue was empty
    T *pop() {
        Node *head = this->head;
        if (head != nullptr) {
            Node *next = head->next;
            this->head = next;
            if (next == nullptr)
                this->tail = nullptr;
            return &static_cast<T &>(*head);
        }
        return nullptr;
    }

    /// @brief If the queue is not empty, the first/front() element gets removed if the function returns true.
    /// @param function Function to determine if the first/front() element should be removed
    /// @return -1: the queue is empty, 0: pop was rejected by the function, 1: pop succeeded
    template <typename F>
    int pop(const F &function) {
        Node *head = this->head;
        if (head != nullptr) {
            Node *next = head->next;
            if (function(static_cast<T &>(*head))) {
                // remove the node
                this->head = next;
                if (next == nullptr)
                    this->tail = nullptr;

                // pop succeeded
                return 1;
            }

            // remove was rejected
            return 0;
        }

        // list is empty
        return -1;
    }

    /// @brief Remove an element
    /// @param element Element to remove
    void remove(T &element) {
        Node &node = element;

        Node *head = this->head;

        // check if the list is empty
        if (head == nullptr)
            return;

        // check if head/front() node
        if (&node == head) {
            // remove the node
            Node *next = head->next;
            this->head = next;
            if (next == nullptr)
                this->tail = nullptr;

            // successfully removed the node
            return;
        }

        Node *current = head;
        while (true) {
            Node *next = current->next;
            if (next == nullptr)
                break;
            if (next == &node) {
                // remove the node
                Node *next2 = next->next;
                current->next = next2;
                if (next2 == nullptr)
                    this->tail = current;

                // successfully removed the node
                return;
            }
            current = next;
        }

        // the node is not in the list
    }

    /// @brief Get first element
    /// @return first element or nullptr if the queue is empty
    T &front() {
        return static_cast<T &>(*this->head);
    }

    /// @brief Get last element
    /// @return last element or nullptr if the queue is empty
    T &back() {
        return static_cast<T &>(*this->tail);
    }

protected:
    Node * tail = nullptr; // push() adds to tail
    Node * head = nullptr; // pop() removes from head
};

} // namespace coco
