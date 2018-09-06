/*******************************************************************************
 *
 *     Copyright : 2010 Arnaud Emilien
 *
 *******************************************************************************/

#ifndef HEAP_H
#define HEAP_H

#include "Table.h"
#include <iostream>

template <class T>
class Heap : public Table<T> {

private:
	bool _ascending;

private:
	void _descent(unsigned int index) {
		if (!_ascending) {
			if (index * 2 + 2 < Table<T>::_size) {
				if (Table<T>::_array[index * 2 + 2] > Table<T>::_array[index * 2 + 1]) {
					if (Table<T>::_array[index] < Table<T>::_array[index * 2 + 2]) {
						Table<T>::swap(index, index * 2 + 2);
						_descent(index * 2 + 2);
					}
				} else {
					if (Table<T>::_array[index] < Table<T>::_array[index * 2 + 1]) {
						Table<T>::swap(index, index * 2 + 1);
						_descent(index * 2 + 1);
					}
				}

			} else if (index * 2 + 1 < Table<T>::_size) {
				if (Table<T>::_array[index] < Table<T>::_array[index * 2 + 1]) {
					Table<T>::swap(index, index * 2 + 1);
					_descent(index * 2 + 1);
				}
			}
		} else {
			if (index * 2 + 2 < Table<T>::_size) {
				if (Table<T>::_array[index * 2 + 2] < Table<T>::_array[index * 2 + 1]) {
					if (Table<T>::_array[index] > Table<T>::_array[index * 2 + 2]) {
						Table<T>::swap(index, index * 2 + 2);
						_descent(index * 2 + 2);
					}
				} else {
					if (Table<T>::_array[index] > Table<T>::_array[index * 2 + 1]) {
						Table<T>::swap(index, index * 2 + 1);
						_descent(index * 2 + 1);
					}
				}

			} else if (index * 2 + 1 < Table<T>::_size) {
				if (Table<T>::_array[index] > Table<T>::_array[index * 2 + 1]) {
					Table<T>::swap(index, index * 2 + 1);
					_descent(index * 2 + 1);
				}
			}
		}
	}

	void _climb(unsigned int index) {
		if (!_ascending) {
			if (index > 0) {
				if (Table<T>::_array[index] > Table<T>::_array[(index - 1) / 2]) {
					Table<T>::swap(index, (index - 1) / 2);
					_climb((index - 1) / 2);
				}
			}
		} else {
			if (index > 0) {
				if (Table<T>::_array[index] < Table<T>::_array[(index - 1) / 2]) {
					Table<T>::swap(index, (index - 1) / 2);
					_climb((index - 1) / 2);
				}
			}
		}
	}

	void removeAt(unsigned int index) {}

public:
	Heap(bool ascending = false, unsigned int allocation = 0)
			: Table<T>(allocation), _ascending(ascending) {
	}

	~Heap() {
		Table<T>::empty();
	}

	T popFirst() {
		T result = Table<T>::_array[0];

		Table<T>::_array[0] = Table<T>::_array[Table<T>::_size - 1];
		_descent(0);

		--Table<T>::_size;

		Table<T>::_reallocateForRemove();

		return result;
	}

	void remove(const T &element) {
		T *current = Table<T>::_array;
		for (unsigned int index = 0; index < Table<T>::_size; ++index) {
			if (element == (*current)) {
				if (index != Table<T>::_size - 1) {
					Table<T>::_array[index] = Table<T>::_array[Table<T>::_size - 1];

					--Table<T>::_size;

					Table<T>::_reallocateForRemove();

					_climb(index);
					_descent(index);
				} else {
					--Table<T>::_size;
					Table<T>::_reallocateForRemove();
				}

				return;
			}
			++current;
		}
		std::cout << "Heap remove " << element << " not found ! " << std::endl;
	}

	void insert(const T &element) {
		Table<T>::_reallocateForInsert();

		++Table<T>::_size;

		Table<T>::_array[Table<T>::_size - 1] = element;

		_climb(Table<T>::_size - 1);
	}

	void detail() {
		Heap<T> copy = *this;
		unsigned int index = 0;
		while (copy.getSize() > 0) {
			std::cout << index << " :    "
					  << copy.popFirst()
					  << std::endl;
			++index;
		}
	}

	/*
	
	bool contains(const T& element)
	{
		return _contains(1, element);
	}
	
private :
	bool _contains(unsigned int index, const T& element)
	{
		if(index > Table<T>::_size)
			return false;
		T val = Table<T>::_array[index-1];
		if( val == element)
			return true;
		if(_ascending && val  > element)
			return false;
		if(!_ascending && val < element)
			return false;
		return ( _contains(2*index, element) || _contains(2*index + 1, element) );
	}
	*/

}; // class Heap

#endif // HEAP_H
