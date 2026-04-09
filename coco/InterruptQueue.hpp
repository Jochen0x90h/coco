#pragma once

#include "IntrusiveMpscQueue.hpp"


namespace coco {

template <typename T>
class InterruptQueue {
public:
    using Node = IntrusiveMpscQueueNode;

    /// @brief Determine if the queue is empty
    ///
    bool empty() {
        return head_.next == nullptr;
    }

    /// @brief Determine if the queue contains no nodes or one node
    ///
    bool emptyOrOne() {
        return head_.next == nullptr || head_.next == tail_;
    }

    /// @brief Clear the queue.
    ///
    void clear() {
        head_.next = nullptr;
        tail_ = &head_;
    }

    /// @brief Insert an element at the end/behind back() of the queue (push_back).
    /// @param element Element to insert at the end
    /// @return true if the queue was empty
    bool push(T &element) {
        Node &node = element;
        node.next = nullptr;
        bool wasEmpty = head_.next == nullptr;

        // add new node to tail
        tail_.load()->next = &node;
        tail_ = &node;
        return wasEmpty;
    }

    /// @brief Insert an element at the end/behind back() of the queue (push_back).
    /// @tparam G Type of guard, e.g. nvic::Guard
    /// @param guard Guard for locking interrupts while push() is executed
    /// @param element Element to insert at the end
    /// @return true if the queue was empty
    template <typename G>
    bool guardedPush(const G &guard, T &element) {
        return push(element);
    }

    /// @brief Visit the first element if it exists.
    /// @tparam V Type of visitor
    /// @param visitor Visitor
    template <typename V>
    void visitFirst(const V &visitor) {
        // get front node
        Node *head = head_.next;

        // check if the list is empty
        if (head == nullptr)
            return;

        visitor(static_cast<T &>(*head));
    }

    /// @brief Remove the first/front() element from the queue (pop_front).
    /// @return The first element or nullptr if the queue was empty
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

    /// @brief If the queue is not empty, the first/front() element gets removed if the function returns true.
    /// Calls a function (nextFunction) with the next element if it exists.
    /// @tparam N Type of the next function, e.g. [](auto &element) { element.startNext(); }
    /// @param nextFunction function to be called with the next element
    /// @return The first element or nullptr if the queue was empty
    template <typename N>
    T *pop(const N &nextFunction) {
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

        // pop succeeded
        return &static_cast<T &>(*head);
    }

    /// @brief Pop the first/front() element from the queue if the predicate returns true.
    /// The predicate function must not modify the queue.
    /// @tparam P Type of the predicate function, e.g. [](auto &element) { return element.isReady(); }
    /// @param predicate Predicate to determine if the first/front() element should be removed.
    /// @return The first element or nullptr if the queue was empty or if the predicate returned false
    template <typename P>
    T *popIf(const P &predicate) {
        Node *head = head_.next;
        if (head == nullptr)
            return nullptr;

        // reject if predicate returns false
        if (!predicate(static_cast<T &>(*head)))
            return nullptr;

        // remove the node
        Node *next = head->next;
        head_.next = next;
        if (next == nullptr)
            tail_ = &head_;

        // pop succeeded
        return &static_cast<T &>(*head);
    }

    /// @brief Pop the first/front() element from the queue if the predicate returns true.
    /// The predicate function must not modify the queue while the removeFunction may modify this queue or add the
    /// element to another queue. The nextfunction is called before removeFunction and must not modify the queue.
    /// @tparam P Type of the predicate function, e.g. [](auto &element) { return element.isReady(); }
    /// @tparam N Type of the next function, e.g. [](auto &element) { element.startNext(); }
    /// @param predicate predicate to determine if the first/front() element should be removed
    /// @param nextFunction Function to be called with the next element when the first element is removed from the queue and there is a next element
    /// @return The first element or nullptr if the queue was empty or if the predicate returned false
    template <typename P, typename N>
    T *popIf(const P &predicate, const N &nextFunction) {
        Node *head = head_.next;
        if (head == nullptr)
            return nullptr;

        // reject if predicate returns false
        if (!predicate(static_cast<T &>(*head)))
            return nullptr;

        // remove the node
        Node *next = head->next;
        head_.next = next;
        if (next == nullptr)
            tail_ = &head_;
        else
            nextFunction(static_cast<T &>(*next));

        // pop succeeded
        return &static_cast<T &>(*head);
    }

    /// @brief Remove an element from the queue.
    /// @param element Element to remove
    /// @return true if the element was removed, false otherwise
    bool remove(T &element) {
        Node &node = element;
        Node *prev = &head_;

        while (true) {
            Node *current = prev->next;
            if (current == nullptr)
                return false;
            if (current == &node) {
                // remove the node
                Node *next = current->next;
                prev->next = next;
                if (next == nullptr)
                    tail_ = prev;

                // successfully removed the node
                return true;
            }

            // go to next node
            prev = current;
        }
    }

    /// @brief Remove an element from the queue while a guard is active.
    /// @tparam G Type of guard, e.g. nvic::Guard
    /// @param guard Guard for locking interrupts while remove() is executed
    /// @param element Element to remove
    /// @return true if the element was removed, false otherwise
    template <typename G>
    bool guardedRemove(const G &guard, T &element) {
        return remove(element);
    }

    /// @brief Remove an element from the queue unless it is the first element.
    /// @param element Element to remove
    /// @return true if the element was removed, false otherwise
    bool removeExceptFirst(T &element) {
        Node &node = element;
        Node *current = head_.next;

        // do not remove first node
        if (current == nullptr || current == &node)
            return false;

        // iterate over all other nodes
        while (true) {
            Node *prev = current;
            current = current->next;
            if (current == nullptr) {
                // node not found
                return false;
            }
            if (current == &node) {
                // remove the node
                Node *next = current->next;
                prev->next = next;
                if (next == nullptr)
                    tail_ = prev;

                // successfully removed the node
                return true;
            }
        }
    }

    /// @brief Remove an element from the queue unless it is the first element while a guard is active.
    /// @tparam G Type of guard, e.g. nvic::Guard
    /// @param guard Guard for locking interrupts while remove() is executed
    /// @param element Element to remove
    /// @return true if the element was removed, false otherwise
    template <typename G>
    bool guardedRemoveExceptFirst(const G &guard, T &element) {
        return removeExceptFirst(element);
    }

    /// @brief Remove the first element from the queue for which the predicate returns true.
    /// @tparam P Type of the predicate function, e.g. [](auto &element, int index) { return index == 0 && element.isReady(); }
    /// @param predicate Predicate to determine if the element should be removed
    /// @return Pointer to the removed element, or nullptr if no element was removed
    template <typename P>
    T *removeIf(const P &predicate) {
        Node *prev = &head_;
        int index = 0;
        while (true) {
            Node *current = prev->next;
            if (current == nullptr)
                return nullptr;
            if (predicate(static_cast<T &>(*current), index)) {
                // remove the node
                Node *next = current->next;
                prev->next = next;
                if (next == nullptr)
                    tail_ = prev;

                // successfully removed the node
                return static_cast<T *>(current);
            }
            prev = current;
            ++index;
        }
    }

    template <typename G, typename P>
    T *guardedRemoveIf(const G &guard, const P &predicate) {
        return removeIf(predicate);
    }

    /// @brief Remove the first element from the queue for which the predicate returns true.
    /// @tparam P Type of the predicate function, e.g. [](auto &element, int index) { return index == 0 && element.isReady(); }
    /// @tparam V Type of the next visitor function, e.g. [](auto &element, int index) { if (index == 0) element.startNext(); }
    /// @param predicate Condition function to determine if the first/front() element should be removed
    /// @param nextVisitor Function to be called when visiting the next element
    /// @return Pointer to the removed element, or nullptr if no element was removed
    template <typename P, typename V>
    T *removeIf(const P &predicate, const V &nextVisitor) {
        Node *prev = &head_;
        int index = 0;
        while (true) {
            Node *current = prev->next;
            if (current == nullptr)
                return nullptr;
            if (predicate(static_cast<T &>(*current), index)) {
                // remove the node
                Node *next = current->next;
                prev->next = next;
                if (next == nullptr)
                    tail_ = prev;
                else
                    nextVisitor(static_cast<T &>(*next), index);

                // successfully removed the node
                return &static_cast<T &>(*current);
            }
            prev = current;
            ++index;
        }
    }

    template <typename G, typename V, typename P>
    T *guardedRemoveIf(const G &guard, const P &predicate, const V &nextVisitor) {
        return removeIf(predicate, nextVisitor);
    }

    /// @brief Get first element
    /// @return Reference to first element
    T &front() {
        return static_cast<T &>(*this->head_.next.load());
    }

    /// @brief Get first element if it exists
    /// @return Pointer to first element or nullptr if the queue is empty
    T *frontOrNull() {
        return static_cast<T *>(this->head_.next.load());
    }

    /// @brief Get last element
    /// @return Last element or nullptr if the queue is empty
    T &back() {
        return static_cast<T &>(*this->tail.load());
    }

protected:

    Node head_; // pop() removes from head
    std::atomic<Node *> tail_ = &head_; // push() adds to tail
};

} // namespace coco
