#pragma once

#include "assert.hpp"


namespace coco {

/**
	Queue with fixed size where elements can be added at the back and taken out at the front

	Example when count() is 6:
	getFront()     getBack() getNextBack()
    v                   v   v
	_________________________....
	| 0 | 1 | 2 | 3 | 4 | 5 | 6 : -> addBack()
	-------------------------....
*/
template <typename Element, int N>
class Queue {
public:
	/**
		Check if the queue is empty
		@return true if queue is empty
	*/
	bool empty() const {return this->siz == 0;}

	/**
		Check if the queue is full
		@return true if queue is full
	*/
	bool full() const {return this->siz >= N;}

	/**
		Get number of elements in queue
		@return number of elements
	*/
	int size() const {return this->siz;}

	/**
		Get the element at the front of the queue. The queue must not be empty
		@return element at front
	*/
	Element &front() {
		assert(this->siz > 0);
		return this->elements[this->index];
	}

	/**
		Get the element at the back of the queue. The queue must not be empty
		@return element at back
	*/
	Element &back() {
		assert(this->siz > 0);
		return this->elements[(this->index + this->siz - 1) % N];
	}

	/**
	 * Get the next element behind the back of the queue. The queue must not be full
	 * @return element behind the back
	 */
	//Element &getNextBack() {
	//	assert(this->c < N);
	//	return this->elements[(this->front + this->c) % N];
	//}

	/**
		Get the element at given index
	*/
	Element &operator [](int index) {
		assert(uint32_t(index) < uint32_t(this->siz));
		return this->elements[(this->index + index) % N];
	}

	/**
		Add a new uninitialized element to the back of the queue. If the queue is full, the front element gets removed.
		If the element was already written to using getNext(), it is remains untouched and is now part of the queue.
	*/
	void pushBack() {
		auto siz = this->siz;
		if (siz == N) {
			this->index = (this->index + 1) % N;
		} else {
			this->siz = siz + 1;
		}
	}

	/**
		Add a new element to the back of the queue. If the queue is full, the front element gets removed.
	*/
	void pushBack(Element const &element) {
		this->elements[(this->index + this->siz) % N] = element;
		auto siz = this->siz;
		if (siz == N) {
			this->index = (this->index + 1) % N;
		} else {
			this->siz = siz + 1;
		}
	}

	/**
		Remove the element at front and make the next element the front element.
		A pointer to the old front element stays valid until the queue is modified again
	*/
	void popFront() {
		auto siz = this->siz;
		if (siz > 0) {
			this->siz = siz - 1;
			this->index = (this->index + 1) % N;
		}
	}

	/**
		Remove the element at the back
	*/
	void popBack() {
		auto siz = this->siz;
		if (siz > 0) {
			this->siz = siz - 1;
		}
	}

	/**
		Clear the queue
	*/
	void clear() {this->siz = 0;}
	void clear() volatile {this->siz = 0;}

protected:

	// queue elements
	Element elements[N];

	// queue tail where elements are taken out
	int index = 0;

	// number of elements in queue
	int siz = 0;
};

} // namespace coco
