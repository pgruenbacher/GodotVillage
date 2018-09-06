/*******************************************************************************
 *
 *     Copyright : 2010 Arnaud Emilien
 *
 *******************************************************************************/

#ifndef LINKED_LIST_H
#define LINKED_LIST_H

#include <cstdlib>

#include "Iterator.h"

template <class T>
class LinkedList;

template <class T>
LinkedList<T> &merge(LinkedList<T> &list1, LinkedList<T> &list2);
template <class T>
LinkedList<T> &sortedMerge(LinkedList<T> &list1, LinkedList<T> &list2);

template <class T>
class LinkedList {

private:
	struct Link {
		T _value;
		Link *_prev;
		Link *_next;
		Link(const T &value)
				: _value(value), _prev(NULL), _next(NULL) {}
		Link *getPrev() { return _prev; }
		Link *getNext() { return _next; }
		T &getValue() { return _value; }
	}; // class Link

private:
	/** Number of elements in the list. */
	unsigned int _size;

	/** Pointer to the first element of the list. */
	Link *_first;

	/** Pointer to the last element of the list. */
	Link *_last;

public:
	/** Default constructor. */
	LinkedList();

	/** Copy constructor. */
	LinkedList(const LinkedList &list);

	template <class U>
	LinkedList(const LinkedList<U> &list);

	/** Note : The destructor empty the list. */
	~LinkedList();

	/** Deep copy. */
	LinkedList<T> &operator=(const LinkedList<T> &list1);

	/** Return the size of the list*/
	unsigned int getSize() const;

	/** Merge two lists. */
	friend LinkedList<T> &merge<>(LinkedList<T> &list1, LinkedList<T> &list2);

	/** Merge two lists, and sort it. */
	friend LinkedList<T> &sortedMerge<>(LinkedList<T> &list1, LinkedList<T> &list2);

	/** Insert the element in the queue (LILO). */
	void fastInsert(const T &element);

	/** Insert the element by sorting it. */
	void sortedInsert(const T &element);

	/** Insert the element in queue. */
	void insert(const T &element);

	/** Insert the element in the top of the list. */
	void pushFirst(const T &element);

	/** Insert the element in the queue of the list. */
	void pushLast(const T &element);

	/** Insert the list in the queue of the list. */
	void pushLast(LinkedList<T> &list);

	/** Return true if the element is in the list. */
	bool contains(const T &element);

	/** Remove the element of the list. */
	void remove(const T &element);

	/** Return the first element of the list. */
	T getFirst() const;

	/** Return the last element of the list. */
	T getLast() const;

	/** Return the first element of the list, and remove it.*/
	T popFirst();

	/** Return the last element of the list, and remove it.*/
	T popLast();

	/** Delete all the elements of the list. */
	void empty();

	/** Print all the element of the list. */
	void detail();

public:
	class LinkedListIterator : public IteratorBase<T> {

	private:
		LinkedList<T> *_parent;
		Link *_current;

	public:
		LinkedListIterator(LinkedList<T> *list)
				: _parent(list), _current(NULL) {}

		bool hasPrev() {
			if (_current == NULL) {
				return false;
			}
			if (_current->getPrev() != NULL) {
				return true;
			}
			return false;
		}

		bool hasNext() {
			if (_current == NULL) {
				return (_parent->getSize() > 0);
			}

			if (_current->getNext() != NULL) {
				return true;
			}

			return false;
		}

		T &next() {
			if (_current == NULL) {
				_current = _parent->_first;
			} else {
				_current = _current->getNext();
			}

			return _current->getValue();
		}

		T &prev() {
			_current = _current->getPrev();
			return _current->getValue();
		}

		T &value() {
			return _current->getValue();
		}

		Link *getCurrent() {
			return _current;
		}
	}; // LinkedListIterator

	void removeAt(LinkedListIterator &it);

	friend class LinkedListIterator;
	template <class>
	friend class LinkedList;

}; // LinkedList

#include "LinkedList.impl"

#endif // LINKED_LIST_H
