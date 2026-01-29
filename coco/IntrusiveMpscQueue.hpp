#pragma once

#include <atomic>


namespace coco {

/// @brief Node for IntrusiveMpscQueue
////
struct IntrusiveMpscQueueNode {
    /// @brief Pointer to next element. Use atomic to be able to use queues for cross-thread communication
    ///
    std::atomic<IntrusiveMpscQueueNode *> next;

    /// @brief Default constructor
    ///
    IntrusiveMpscQueueNode() = default;

    /// @brief Delete copy constructor
    ///
    IntrusiveMpscQueueNode(IntrusiveMpscQueueNode const &) = delete;
};


/// @brief Intrusive multiple producer single consumer queue
/// https://www.1024cores.net/home/lock-free-algorithms/queues
/// https://www.1024cores.net/home/lock-free-algorithms/queues/intrusive-mpsc-node-based-queue
///
/// @tparam T element type, must derive from IntrusiveMpscQueueNode
template <typename T>
class IntrusiveMpscQueue {
public:
    using Node = IntrusiveMpscQueueNode;

    IntrusiveMpscQueue() : head(&stub), tail(&stub) {}

    /// @brief Push an element to the queue. This may happen in multiple threads or different interrupt service routines
    /// @param element element to add
    void push(T &element) {
        pushInternal(element);
    }

    /// @brief Pop an element from the queue. This can happen in only one thread, for example an event loop.
    /// @return the oldest element or nullptr if the queue was empty
    T *pop() {
        Node* tail = this->tail;
        Node* next = tail->next;

        if (tail == &this->stub) {
            if (next == nullptr)
                return nullptr;
            this->tail = next;
            tail = next;
            next = next->next;
        }

        if (next != nullptr) {
            this->tail = next;
            return &static_cast<T &>(*tail); // cast reference instead of pointer to avoid null check
        }

        Node* head = this->head;

        if (tail != head)
            return nullptr;

        // push stub
        pushInternal(this->stub);

        next = tail->next;

        if (next != nullptr) {
            this->tail = next;
            return &static_cast<T &>(*tail); // cast reference instead of pointer to avoid null check
        }

        return nullptr;
    }

protected:
    void pushInternal(Node &n) {
        n.next = nullptr;
        Node *prev = this->head.exchange(&n);
        prev->next = &n;
    }

    Node stub;
    std::atomic<Node *> head; // push() adds to head
    Node *tail; // pop() removes from tail
};

} // namespace coco
