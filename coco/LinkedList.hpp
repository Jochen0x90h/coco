#pragma once


namespace coco {

/**
 * Linked list node for list head and elements to be inherited from, e.g. class Foo : public LinkedListNode<Foo> {};
 */
class LinkedListNode {
public:
	/**
	 * Construct an empty list or an element that is "not in list"
	 */
	LinkedListNode() noexcept {
		this->next = this->prev = this;
	}

	/**
	 * Construct a new list element and add to given list
	 */
	LinkedListNode(LinkedListNode &list) {
		this->next = &list;
		this->prev = list.prev;
		list.prev->next = this;
		list.prev = this;
	}

	/**
	 * Delete copy constructor
	 */
	LinkedListNode(LinkedListNode const &) = delete;

	/**
	 * Destructor removes the node from the list
	 */
	~LinkedListNode() {
		// remove this element from the list
		this->next->prev = this->prev;
		this->prev->next = this->next;
	}

	/**
	 * Return true if the node part of a list
	 */
	bool inList() const {
		return this->next != this;
	}

	/**
	 * Remove this element from the list
	 */
	void remove() noexcept {
		this->next->prev = this->prev;
		this->prev->next = this->next;

		// set to "not in list"
		this->next = this;
		this->prev = this;
	}

	LinkedListNode *next;
	LinkedListNode *prev;
};

/**
 * Linked list. The list elements must inherit from LinkedListNode
 * @tparam T list element type that inherits LinkedListNode, e.g class Element : public LinkedListNode {};
 */
template <typename T>
class LinkedList : public LinkedListNode {
public:
	/**
	 * Return true if the node is an empty list head
	 */
	bool empty() const {
		return this->next == this;
	}

	/**
	 * Count the number of elements in the list which is O(n)
	 * @return number of elements
	 */
	int count() const {
		int count = 0;
		auto node = this->next;
		while (node != this) {
			node = node->next;
			++count;
		}
		return count;
	}

	/**
	 * Add one or multiple elements at the end of the list
	 * @param node element to add, can be part of a "ring" of nodes
	 */
	void add(T &element) {
		LinkedListNode &node = element;
		auto p = node.prev;
		node.prev->next = this;
		node.prev = this->prev;
		this->prev->next = &node;
		this->prev = p;
	}

	/**
	 * Add one list to another, take care to remove the other list from the "ring" of nodes afterwards
	 * @param node list to add
	 */
	void add(LinkedList &list) {
		LinkedListNode &node = list;
		auto p = node.prev;
		node.prev->next = this;
		node.prev = this->prev;
		this->prev->next = &node;
		this->prev = p;
	}

	/**
	 * Iterator. Do not remove() an element that an iterator points to
	 */
	struct Iterator {
		LinkedListNode *node;
		T &operator *() {return *static_cast<T *>(this->node);}
		T *operator ->() {return static_cast<T *>(this->node);}
		Iterator &operator ++() {this->node = this->node->next; return *this;}
		Iterator &operator --() {this->node = this->node->prev; return *this;}
		bool operator ==(Iterator it) const {return this->node == it.node;}
		bool operator !=(Iterator it) const {return this->node != it.node;}
	};

	Iterator begin() {return {this->next};}
	Iterator end() {return {this};}

	T &get(int index) {
		int count = 0;
		auto node = this->next;
		while (node != this) {
			if (index == 0)
				return *static_cast<T *>(node);
			node = node->next;
			--index;
		}
		assert(false);
		return *(T *)nullptr;
	}
};


/**
 * Second implementation to be able to add two linked list nodes to a class via inheritance (so that the class can be part of two lists)
 */
class LinkedListNode2 {
public:
	LinkedListNode2() noexcept {
		this->next = this->prev = this;
	}
	
	LinkedListNode2(LinkedListNode2 &list) {
		this->next = &list;
		this->prev = list.prev;
		list.prev->next = this;
		list.prev = this;
	}
	
	LinkedListNode2(LinkedListNode2 const &) = delete;
	
	~LinkedListNode2() {
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

	LinkedListNode2 *next;
	LinkedListNode2 *prev;
};

/**
 * Second implemenation of linked list. The list elements must inherit from LinkedListNode2
 * @tparam T list element type that inherits LinkedListNode, e.g class Element : public LinkedListNode2 {};
 */
template <typename T>
class LinkedList2 : public LinkedListNode2 {
public:
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

	void add(T &element) {
		LinkedListNode2 &node = element;
		auto p = node.prev;
		node.prev->next = this;
		node.prev = this->prev;
		this->prev->next = &node;
		this->prev = p;
	}

	void add(LinkedList2 &list) {
		LinkedListNode2 &node = list;
		auto p = node.prev;
		node.prev->next = this;
		node.prev = this->prev;
		this->prev->next = &node;
		this->prev = p;
	}

	struct Iterator {
		LinkedListNode2 *node;
		T &operator *() {return *static_cast<T *>(this->node);}
		T *operator ->() {return static_cast<T *>(this->node);}
		Iterator &operator ++() {this->node = this->node->next; return *this;}
		Iterator &operator --() {this->node = this->node->prev; return *this;}
		bool operator ==(Iterator it) const {return this->node == it.node;}
		bool operator !=(Iterator it) const {return this->node != it.node;}
	};

	Iterator begin() {return {this->next};}
	Iterator end() {return {this};}
};

} // namespace coco
