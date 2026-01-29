#pragma once

#include <atomic>


namespace coco {

struct IntrusiveQueueNode {
    // pointer to next element. Use atomic to be able to use queues for cross-thread communication
    IntrusiveQueueNode *next;


    /// @brief Default constructor.
    ///
    IntrusiveQueueNode() = default;

    /// @brief Delete copy constructor.
    ///
    IntrusiveQueueNode(IntrusiveQueueNode const &) = delete;
};


/// @brief Intrusive queue.
/// push() pushes at back/end/head of queue
/// pop() pops from front/begin/tail of queue
/// Also see https://en.cppreference.com/w/cpp/container/queue
///
/// @tparam T element type, must derive from IntrusiveQueueNode
template <typename T>
class IntrusiveQueue {
public:
    using Node = IntrusiveQueueNode;

    /// @brief Determine if the queue is empty
    ///
    bool empty() {
        return this->head == nullptr;
    }

    /// @brief Insert an element at the end/behind back() of the queue (push_back).
    /// @return true if the queue was empty
    bool push(T &element) {
        Node &node = element;
        node.next = nullptr;
        Node *prev = this->head;
        bool wasEmpty = prev == nullptr;
        if (wasEmpty)
            this->tail = &node;
        else
            prev->next = &node;
        this->head = &node;
        return wasEmpty;
    }

    /// @brief Remove the first/front() element from the queue (pop_front).
    /// @return removed element or nullptr if queue was empty
    T *pop() {
        Node *tail = this->tail;
        if (tail != nullptr) {
            Node *next = tail->next;
            this->tail = next;
            if (next == nullptr)
                this->head = nullptr;
            return &static_cast<T &>(*tail);
        }
        return nullptr;
    }

    /// @brief Remove an element
    /// @param element Element to remove
    void remove(T &element) {
        Node &node = element;

        Node *tail = this->tail;

        // check if the list is empty
        if (tail == nullptr)
            return;

        // check if tail/front() node
        if (&node == tail) {
            // remove the node
            Node *next = tail->next;
            this->tail = next;
            if (next == nullptr)
                this->head = nullptr;

            // successfully removed the node
            return;
        }

        Node *current = tail;
        while (true) {
            Node *next = current->next;
            if (next == nullptr)
                break;
            if (next == &node) {
                // remove the node
                Node *next2 = next->next;
                current->next = next2;
                if (next2 == nullptr)
                    this->head = current;

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
        return static_cast<T &>(*this->tail);
    }

    /// @brief Get last element
    /// @return last element or nullptr if the queue is empty
    T &back() {
        return static_cast<T &>(*this->head);
    }

protected:
    Node * head = nullptr; // push() adds to head
    Node * tail = nullptr; // pop() removes from tail
};

} // namespace coco
