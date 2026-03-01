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
        this->head = nullptr;
        this->tail = nullptr;
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
        if (head == nullptr)
            return nullptr;

        // remove the node
        Node *next = head->next;
        this->head = next;
        if (next == nullptr)
            this->tail = nullptr;

        // pop succeeded
        return &static_cast<T &>(*head);
    }

    template <typename F>
    T *pop(const F &removeFunction) {
        Node *head = this->head;
        if (head == nullptr)
            return nullptr;

        // remove the node
        Node *next = head->next;
        this->head = next;
        if (next == nullptr)
            this->tail = nullptr;

        // call remove function when the element is not part of the queue any more
        auto &element = static_cast<T &>(*head);
        removeFunction(element);

        // pop succeeded
        return &element;
    }

    template <typename P>
    T *popp(const P &predicate) {
        Node *head = this->head;
        if (head == nullptr)
            return nullptr;

        // reject if predicate returns false
        if (!predicate(static_cast<T &>(*head)))
            return nullptr;

        // remove the node
        Node *next = head->next;
        this->head = next;
        if (next == nullptr)
            this->tail = nullptr;

        // pop succeeded
        return &static_cast<T &>(*head);
    }

    /// @brief Pop the first/front() element from the queue if the predicate returns true.
    /// The predicate function must not modify the queue while the removeFunction may modify this queue or add the
    /// element to another queue.
    /// @param predicate Predicate function to determine if the first/front() element should be removed
    /// @param removeFunction Function to be called when an element is removed from the queue
    /// @return removed element or nullptr if queue was empty or if the predicate returned false
    template <typename P, typename F>
    T *popp(const P &predicate, const F &removeFunction) {
        Node *head = this->head;
        if (head == nullptr)
            return nullptr;

        // reject if predicate returns false
        if (!predicate(static_cast<T &>(*head)))
            return nullptr;

        // remove the node
        Node *next = head->next;
        this->head = next;
        if (next == nullptr)
            this->tail = nullptr;

        // call remove function when the element is not part of the queue any more
        auto &element = static_cast<T &>(*head);
        removeFunction(element);

        // pop succeeded
        return &element;
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
    Node *head = nullptr; // pop() removes from head
    Node * tail = nullptr; // push() adds to tail
};

} // namespace coco
