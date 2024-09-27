#pragma once

#include <atomic>


namespace coco {

/**
 * Node for IntrusiveMpscQueue
 */
struct IntrusiveMpscQueueNode {
    /**
     * Pointer to next element. Use atomic to be able to use queues for cross-thread communication
     */
    std::atomic<IntrusiveMpscQueueNode *> next;

    /**
     * Default constructor
     */
    IntrusiveMpscQueueNode() = default;

    /**
     * Delete copy constructor
     */
    IntrusiveMpscQueueNode(IntrusiveMpscQueueNode const &) = delete;
};


/**
 * Intrusive multiple producer single consumer queue
 * https://www.1024cores.net/home/lock-free-algorithms/queues
 * https://www.1024cores.net/home/lock-free-algorithms/queues/intrusive-mpsc-node-based-queue
 *
 * @tparam T element type, must derive from IntrusiveMpscQueueNode
 */
template <typename T>
class IntrusiveMpscQueue {
public:
    using Node = IntrusiveMpscQueueNode;

    IntrusiveMpscQueue() : head(&stub), tail(&stub) {}

    /**
     * Push an element to the queue. This may happen in multiple threads or different interrupt service routines
     * @param element element to add
     */
    void push(T &element) {
        pushInternal(element);
    }

    /**
     * Pop an element from the queue. This can happen in only one thread, for example an event loop.
     * @return the oldest element or nullptr if the queue was empty
     */
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
            return &static_cast<T &>(*tail); // avoid null check when casting
        }

        Node* head = this->head;

        if (tail != head)
            return nullptr;

        // push stub
        pushInternal(this->stub);

        next = tail->next;

        if (next != nullptr) {
            this->tail = next;
            return &static_cast<T &>(*tail); // avoid null check when casting
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


/**
 * Intrusive queue for tasks that need to be handled by an interrupt.
 * push() pushes at back/end/head of queue
 * pop() pops from front/begin/tail of queue
 * Also see https://en.cppreference.com/w/cpp/container/queue
 *
 * @tparam T element type, must derive from IntrusiveMpscQueueNode
 */
template <typename T>
class InterruptQueue {
public:
    using Node = IntrusiveMpscQueueNode;

    /**
     * Determine if the queue is empty
     */
    bool empty() {
        return this->head.load() == nullptr;
    }

    /**
     * Clear the queue
     */
    void clear() {
        this->head = nullptr;
        this->tail = nullptr;
    }

    /**
     * Insert an element at the end/behind back() of the queue (push_back).
     * @return true if the queue was empty
     */
    bool push(T &element) {
        return pushInternal(element);
    }

    template <typename G>
    bool push(const G &guard, T &element) {
        return pushInternal(element);
    }

    /**
     * Visit the first element if it exists
     */
    template <typename V>
    void visitFirst(const V &visitor) {
        // get front node
        Node *tail = this->tail;

        // check if the list is empty
        if (tail == nullptr)
            return;

        visitor(static_cast<T &>(*tail));
    }

    /**
     * Remove an element from the queue.
     * @param element element to remove
     * @param removeFront false to prevent removal of the front element
     * @return 1 if remove succeeded, 0 if remove was rejected (element is at front and removeFront true), -1 if not found
     */
    int remove(T &element, bool removeFront = true) {
        return removeInternal(element, removeFront);
    }

    template <typename G>
    int remove(const G &guard, T &element, bool removeFront = true) {
        return removeInternal(element, removeFront);
    }

    /**
     * Remove an element from the queue.
     * @tparam P type of predicate function
     * @tparam V type of visitor function
     * @param element element to remove
     * @param frontPredicate if the element is the front element, the predicate determines if the element should be removed
     * @param nextVisitor visitor called on the next element if the front element was removed
     * @return 1 if remove succeeded, 0 if remove was rejected by the function, -1 if not found
     */
    template <typename P, typename V>
    int remove(T &element, const P &frontPredicate, const V &nextVisitor) {
        return removeInternal(element, frontPredicate, nextVisitor);
    }

    template <typename G, typename P, typename V>
    int remove(const G &guard, T &element, const P &frontPredicate, const V &nextVisitor) {
        return removeInternal(element, frontPredicate, nextVisitor);
    }

    /**
     * Remove the first element from the queue for which the predicate returns true.
     * @return true if a node was removed, false if no node was removed
     */
    template <typename P>
    bool remove(const P &predicate) {
        return removeInternal(predicate);
    }

    /**
     * Remove the first/front() element from the queue (pop_front).
     * @return the first element or nullptr if the queue was empty
     */
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

    /**
     * If the queue is not empty, the first/front() element gets removed if the function returns true.
     * @param function function to determine if the first/front() element should be removed
     * @return 1 if pop succeeded, 0 pop was rejected by the function, -1 the list is empty
     */
    template <typename F>
    int pop(const F &function) {
        Node *tail = this->tail;
        if (tail != nullptr) {
            Node *next = tail->next;
            if (function(static_cast<T &>(*tail))) {
                // remove the node
                this->tail = next;
                if (next == nullptr)
                    this->head = nullptr;
                // successfully removed the node
                return 1;
            }
            // remove was rejected
            return 0;
        }
        // list is empty
        return -1;
    }

    /**
     * If the queue is not empty, the first/front() element gets removed if the function returns true.
     * Call another function with the next element if it exists.
     * @param function function to determine if the first/front() element should be removed
     * @param nextFunction function to be called with the next element
     * @return 1 if pop succeeded, 0 pop was rejected by the function, -1 the list is empty
     */
    template <typename F, typename G>
    int pop(const F &function, const G &nextFunction) {
        Node *tail = this->tail;
        if (tail != nullptr) {
            Node *next = tail->next;
            if (function(static_cast<T &>(*tail))) {
                // remove the node
                this->tail = next;
                if (next == nullptr)
                    this->head = nullptr;
                else
                    nextFunction(static_cast<T &>(*next));

                // successfully removed the node
                return 1;
            }
            // remove was rejected
            return 0;
        }
        // list is empty
        return -1;
    }

    /**
     * Get first element
     * @return reference to first element
     */
    T &front() {
        return static_cast<T &>(*this->tail.load());
    }

    /**
     * Get first element if it exists
     * @return pointer to first element or nullptr if the queue is empty
     */
    T *frontOrNull() {
        return static_cast<T *>(this->tail.load());
    }

    /**
     * Get last element
     * @return last element or nullptr if the queue is empty
     */
    T &back() {
        return static_cast<T &>(*this->head.load());
    }

protected:
    bool pushInternal(Node &node) {
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

    int removeInternal(Node &node, bool removeFront) {
        // get front node
        Node *tail = this->tail;

        // check if the list is empty
        if (tail == nullptr)
            return -1;

        // check if tail/front() node
        if (&node == tail) {
            if (removeFront) {
                // remove the node
                Node *next = tail->next;
                this->tail = next;
                if (next == nullptr)
                    this->head = nullptr;

                // successfully removed the node
                return 1;
            }
            // remove was rejected
            return 0;
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
        Node *tail = this->tail;

        // check if the list is empty
        if (tail == nullptr)
            return -1;

        // check if tail/front() node
        if (&node == tail) {
            if (frontPredicate(static_cast<T &>(*tail))) {
                // remove the node
                Node *next = tail->next;
                this->tail = next;
                if (next == nullptr)
                    this->head = nullptr;
                else
                    nextVisitor(static_cast<T &>(*next));

                // successfully removed the node
                return 1;
            }
            // remove was rejected
            return 0;
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
                return 1;
            }
            current = next;
        }

        // the node is not in the list
        return -1;
    }

    template <typename P>
    bool removeInternal(const P &predicate) {
        auto *ptr = &this->tail;

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
                    this->head = last;

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

    std::atomic<Node *> head; // push() adds to head, back() element
    std::atomic<Node *> tail; // pop() removes from tail, front() element
};

} // namespace coco
