#pragma once

#include <cassert>


namespace coco {

/// @brief Intrusive list node.
/// Elements of the list must inherit the IntrusiveListNode.
class IntrusiveListNode {
public:
    /// @brief Construct an empty list or an element that is "not in list".
    ///
    IntrusiveListNode() noexcept {
        this->next = this->prev = this;
    }

    /// Construct an list node with given next/prev pointers, mainly for internal use.
    ///
    IntrusiveListNode(IntrusiveListNode *next, IntrusiveListNode *prev) noexcept : next(next), prev(prev) {}

    /// Construct a new list element and add to given list.
    ///
    IntrusiveListNode(IntrusiveListNode &list) {
        this->next = &list;
        this->prev = list.prev;
        list.prev->next = this;
        list.prev = this;
    }

    // Delete copy constructor
    IntrusiveListNode(IntrusiveListNode const &) = delete;

    /// @brief Move constructor replaces the given node in the chain of nodes.
    ///
    IntrusiveListNode(IntrusiveListNode &&node) {
        // replace node
        node.prev->next = this;
        node.next->prev = this;
        this->next = node.next;
        this->prev = node.prev;

        // set node to "not in list" state
        node.next = &node;
        node.prev = &node;
    }

    /// @brief Destructor removes the node from the list.
    ///
    ~IntrusiveListNode() {
        // remove this element from the list
        this->next->prev = this->prev;
        this->prev->next = this->next;
    }

    /// @brief Move assignment removes itself and then replaces the given node in the chain of nodes.
    ///
    IntrusiveListNode &operator =(IntrusiveListNode &&node) {
        // remove this
        this->next->prev = this->prev;
        this->prev->next = this->next;

        // replace node
        node.prev->next = this;
        node.next->prev = this;
        this->next = node.next;
        this->prev = node.prev;

        // set node to "not in list" state
        node.next = &node;
        node.prev = &node;
        return *this;
    }

    /// @brief Return true if the node part of a list.
    ///
    bool inList() const {
        return this->next != this;
    }

    /// Remove this element from the list.
    ///
    void remove() noexcept {
        this->next->prev = this->prev;
        this->prev->next = this->next;

        // set to "not in list"
        this->next = this;
        this->prev = this;
    }

    /*void advance() noexcept {
        auto next = this->next;

        // remove
        next->prev = this->prev;
        this->prev->next = next;

        // add after next
        this->next = next->next;
        this->prev = next;
        next->next->prev = this;
        next->next = this;
    }*/

    IntrusiveListNode *next;
    IntrusiveListNode *prev;
};

/// @brief Intrusive list.
/// Elements of the list must inherit the IntrusiveListNode. This list is not thread-safe.
/// @tparam T list element type that inherits IntrusiveListNode, e.g. class Element : public IntrusiveListNode {};
template <typename T>
class IntrusiveList : public IntrusiveListNode {
public:
    using Node = IntrusiveListNode;

    /// @brief Return true if the list is empty.
    ///
    bool empty() const {
        return this->next == this;
    }

    /// Count the number of elements in the list which is O(n).
    /// @return number of elements
    int count() const {
        int count = 0;
        auto current = this->next;
        while (current != this) {
            current = current->next;
            ++count;
        }
        return count;
    }

    /// @brief Clear the list.
    ///
    void clear() {remove();}

    /// @brief Add one or multiple elements at the end of the list
    /// @param node element to add, can be part of a "ring" of nodes
    void add(T &element) {
        Node &node = element;
        auto p = node.prev;
        node.prev->next = this;
        node.prev = this->prev;
        this->prev->next = &node;
        this->prev = p;
    }

    /// @brief Add one list to another, take care to remove the other list from the "ring" of nodes afterwards
    /// @param node list to add
    void add(IntrusiveList &list) {
        Node &node = list;
        auto p = node.prev;
        node.prev->next = this;
        node.prev = this->prev;
        this->prev->next = &node;
        this->prev = p;
    }

    /// @brief Iterator. Do not remove() an element that an iterator points to.
    ///
    struct Iterator {
        Node *node;
        T &operator *() {return static_cast<T &>(*this->node);}
        T *operator ->() {return &static_cast<T &>(*this->node);}
        Iterator &operator ++() {this->node = this->node->next; return *this;}
        Iterator &operator --() {this->node = this->node->prev; return *this;}
        bool operator ==(Iterator it) const {return this->node == it.node;}
        bool operator !=(Iterator it) const {return this->node != it.node;}
    };

    Iterator begin() {return {this->next};}
    Iterator end() {return {this};}

    /// @brief Get an element at the given index without bounds checking.
    /// @param index index of element to get
    T &get(int index) {
        int count = 0;
        auto node = this->next;
        while (node != this) {
            if (index == 0)
                return static_cast<T &>(*node);
            node = node->next;
            --index;
        }
        assert(false);
        return *(T *)nullptr;
    }
};


/// @brief Second implementation to be able to add an element to two linked lists.
/// E.g. class Element : public IntrusiveListNode, public IntrusiveListNode2 {};
class IntrusiveListNode2 {
public:
    IntrusiveListNode2() noexcept {
        this->next = this->prev = this;
    }

    IntrusiveListNode2(IntrusiveListNode2 &list) {
        this->next = &list;
        this->prev = list.prev;
        list.prev->next = this;
        list.prev = this;
    }

    IntrusiveListNode2(IntrusiveListNode2 const &) = delete;

    ~IntrusiveListNode2() {
        // remove this element from the list
        this->next->prev = this->prev;
        this->prev->next = this->next;
    }

    bool inList2() const {
        return this->next != this;
    }

    void remove2() noexcept {
        this->next->prev = this->prev;
        this->prev->next = this->next;

        // set to "not in list"
        this->next = this;
        this->prev = this;
    }

    IntrusiveListNode2 *next;
    IntrusiveListNode2 *prev;
};

/// @brief Second implementation to be able to add an element to two linked lists.
/// @tparam T list element type that inherits IntrusiveListNode2,
/// e.g. class Element : public IntrusiveListNode, public IntrusiveListNode2 {};
template <typename T>
class IntrusiveList2 : public IntrusiveListNode2 {
public:
    using Node = IntrusiveListNode2;

    bool empty() const {
        return this->next == this;
    }

    int count() const {
        int count = 0;
        auto node = this->next;
        while (node != this) {
            node = node->next;
            ++count;
        }
        return count;
    }

    void clear() {remove2();}

    void add(T &element) {
        Node &node = element;
        auto p = node.prev;
        node.prev->next = this;
        node.prev = this->prev;
        this->prev->next = &node;
        this->prev = p;
    }

    void add(IntrusiveList2 &list) {
        Node &node = list;
        auto p = node.prev;
        node.prev->next = this;
        node.prev = this->prev;
        this->prev->next = &node;
        this->prev = p;
    }

    struct Iterator {
        Node *node;
        T &operator *() {return static_cast<T &>(*this->node);}
        T *operator ->() {return &static_cast<T &>(*this->node);}
        Iterator &operator ++() {this->node = this->node->next; return *this;}
        Iterator &operator --() {this->node = this->node->prev; return *this;}
        bool operator ==(Iterator it) const {return this->node == it.node;}
        bool operator !=(Iterator it) const {return this->node != it.node;}
    };

    Iterator begin() {return {this->next};}
    Iterator end() {return {this};}
};

} // namespace coco
