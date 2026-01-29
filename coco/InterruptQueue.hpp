#pragma once

#include "IntrusiveMpscQueue.hpp"


namespace coco {

/// @brief Intrusive queue for tasks that need to be handled by an interrupt.
/// push() pushes at back/end/tail of queue
/// pop() pops from front/begin/head of queue
/// Also see https://en.cppreference.com/w/cpp/container/queue
///
/// @tparam T element type, must derive from IntrusiveMpscQueueNode
template <typename T>
class InterruptQueue {
public:
    using Node = IntrusiveMpscQueueNode;

    /// @brief Determine if the queue is empty
    ///
    bool empty() {
        return this->tail.load() == nullptr;
    }

    /// @brief Determine if the queue contains no nodes or one node
    ///
    bool emptyOrOne() {
        return this->tail.load() == this->head.load();
    }

    /// @brief Clear the queue
    ///
    void clear() {
        this->tail = nullptr;
        this->head = nullptr;
    }

    /// @brief Insert an element at the end/behind back() of the queue (push_back).
    /// @param element Element to insert at the end
    /// @return true if the queue was empty
    bool push(T &element) {
        return pushInternal(element);
    }

    /// @brief Insert an element at the end/behind back() of the queue (push_back).
    /// @param guard Guard for locking interrupts while push() is executed
    /// @param element Element to insert at the end
    /// @return true if the queue was empty
    template <typename G>
    bool push(const G &guard, T &element) {
        return pushInternal(element);
    }

    /// @brief Visit the first element if it exists
    /// @tparam V Type of visitor
    /// @param visitor Visitor
    template <typename V>
    void visitFirst(const V &visitor) {
        // get front node
        Node *head = this->head;

        // check if the list is empty
        if (head == nullptr)
            return;

        visitor(static_cast<T &>(*head));
    }

    /// @brief Remove an element from the queue.
    /// @param element Element to remove
    /// @param removeFront Set to false to prevent removal of the front element
    /// @return -1: element not found, 0: remove was rejected (element is at front and removeFront is false), 1: remove succeeded
    int remove(T &element, bool removeFront = true) {
        return removeInternal(element, removeFront);
    }

    /// @brief Remove an element from the queue while a guard is active.
    /// @param guard Guard for locking interrupts while remove() is executed
    /// @param element Element to remove
    /// @param removeFront Set to false to prevent removal of the front element
    /// @return -1: element not found, 0: remove was rejected (element is at front and removeFront is false), 1: remove succeeded
    template <typename G>
    int remove(const G &guard, T &element, bool removeFront = true) {
        return removeInternal(element, removeFront);
    }

    /// @brief Remove an element from the queue.
    /// @tparam P Type of predicate function
    /// @tparam V Type of visitor function
    /// @param element Element to remove
    /// @param frontPredicate If the element is the front element, the predicate determines if the element should be removed
    /// @param nextVisitor Visitor called on the next element if the front element was removed
    /// @return -1: element not found, 0: remove was rejected (element is at front and frontPredicate returned false), 1: remove succeeded
    template <typename P, typename V>
    int remove(T &element, const P &frontPredicate, const V &nextVisitor) {
        return removeInternal(element, frontPredicate, nextVisitor);
    }

    /// @brief Remove an element from the queue while a guard is active.
    /// @param guard Guard for locking interrupts while remove() is executed
    /// @param element Element to remove
    /// @param frontPredicate If the element is the front element, the predicate determines if the element should be removed
    /// @param nextVisitor Visitor called on the next element if the front element was removed
    /// @return -1: element not found, 0: remove was rejected (element is at front and frontPredicate returned false), 1: remove succeeded
    template <typename G, typename P, typename V>
    int remove(const G &guard, T &element, const P &frontPredicate, const V &nextVisitor) {
        return removeInternal(element, frontPredicate, nextVisitor);
    }

    /// @brief Remove the first element from the queue for which the predicate returns true.
    /// @return true if a node was removed, false if no node was removed
    template <typename P>
    bool remove(const P &predicate) {
        return removeInternal(predicate);
    }

    /// @brief Remove the first/front() element from the queue (pop_front).
    /// @return The first element or nullptr if the queue was empty
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
    /// @return -1: the list is empty, 0: pop was rejected by the function, 1: pop succeeded
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

    /// @brief If the queue is not empty, the first/front() element gets removed if the function returns true.
    /// Calls a function (nextFunction) with the next element if it exists.
    /// @param function function to determine if the first/front() element should be removed
    /// @param nextFunction function to be called with the next element
    /// @return -1: the list is empty, 0: pop was rejected by the function, 1: pop succeeded, 2: pop succeeded and nextFunction was called
    template <typename F, typename G>
    int pop(const F &function, const G &nextFunction) {
        Node *head = this->head;
        if (head != nullptr) {
            Node *next = head->next;
            if (function(static_cast<T &>(*head))) {
                // remove the node
                this->head = next;
                if (next == nullptr) {
                    this->tail = nullptr;

                    // pop succeeded, but no next element
                    return 1;
                }

                nextFunction(static_cast<T &>(*next));

                // pop succeeded and nextFunction was called
                return 2;
            }
            // remove was rejected
            return 0;
        }
        // list is empty
        return -1;
    }

    /// @brief Get first element
    /// @return Reference to first element
    T &front() {
        return static_cast<T &>(*this->head.load());
    }

    /// @brief Get first element if it exists
    /// @return Pointer to first element or nullptr if the queue is empty
    T *frontOrNull() {
        return static_cast<T *>(this->head.load());
    }

    /// @brief Get last element
    /// @return Last element or nullptr if the queue is empty
    T &back() {
        return static_cast<T &>(*this->tail.load());
    }

protected:
    bool pushInternal(Node &node) {
        node.next = nullptr;

        // link from current tail to new node
        Node *prev = this->tail;
        bool wasEmpty = prev == nullptr;
        if (wasEmpty)
            this->head = &node;
        else
            prev->next = &node;

        // add new node at tail
        this->tail = &node;
        return wasEmpty;
    }

    int removeInternal(Node &node, bool removeFront) {
        // get front node
        Node *head = this->head;

        // check if the list is empty
        if (head == nullptr)
            return -1;

        // check if head/front() node
        if (&node == head) {
            if (removeFront) {
                // remove the node
                Node *next = head->next;
                this->head = next;
                if (next == nullptr)
                    this->tail = nullptr;

                // successfully removed the node
                return 1;
            }
            // remove was rejected
            return 0;
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
                return 1;
            }
            current = next;
        }

        // the node is not in the list
        return -1;
    }

    template <typename P, typename V>
    int removeInternal(Node &node, const P &frontPredicate, const V &nextVisitor) {
        // get front node
        Node *head = this->head;

        // check if the list is empty
        if (head == nullptr)
            return -1;

        // check if head/front() node
        if (&node == head) {
            if (frontPredicate(static_cast<T &>(*head))) {
                // remove the node
                Node *next = head->next;
                this->head = next;
                if (next == nullptr)
                    this->tail = nullptr;
                else
                    nextVisitor(static_cast<T &>(*next));

                // successfully removed the node
                return 1;
            }
            // remove was rejected
            return 0;
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
                return 1;
            }
            current = next;
        }

        // the node is not in the list
        return -1;
    }

    template <typename P>
    bool removeInternal(const P &predicate) {
        auto *ptr = &this->head;

        // get front node
        Node *current = *ptr;

        Node *last = nullptr;
        while (current != nullptr) {
            // get next here as the predicate may overwrite next when it returns true
            Node *next = current->next;
            if (predicate(static_cast<T &>(*current))){
                // remove the node
                *ptr = next;
                if (next == nullptr)
                    this->tail = last;

                // removed a node
                return true;
            }
            last = current;
            ptr = &current->next;
            current = next;
        }

        // no node was removed
        return false;
    }

    std::atomic<Node *> tail; // push() adds to tail, back() element
    std::atomic<Node *> head; // pop() removes from head, front() element
};

} // namespace coco
