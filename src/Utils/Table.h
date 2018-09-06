/*******************************************************************************
 *
 *     Copyright : 2010 Arnaud Emilien
 *
 *******************************************************************************/

#ifndef TABLE_H
#define TABLE_H

#include <iostream>

template <class T>
class Table {

protected:
	unsigned int _size;
	unsigned int _allocatedSize;
	T *_array;

	void _reallocateForRemove() {
		if (_size < _allocatedSize / 4) {
			_reallocate(_allocatedSize / 4);
		}
	}

	void _reallocateForInsert() {
		if (_allocatedSize == 0) {
			_reallocate(2);
		}
		if (_size >= _allocatedSize) {
			_reallocate(_allocatedSize * 2);
		}
	}

	void _reallocate(unsigned int newSize) {
		//std::cout << " REALLOCATE : size : " << newSize << std::endl;
		if (_array) {
			T *newArray = new T[newSize];
			for (unsigned int i = 0; i < _size; ++i) {
				newArray[i] = _array[i];
			}
			delete[] _array;
			_array = NULL;
			_array = newArray;
		} else {
			_array = new T[newSize];
		}
		_allocatedSize = newSize;
	}

public:
	Table(unsigned int allocation = 0)
			: _size(0), _allocatedSize(allocation), _array(NULL) {
		if (allocation) {
			_reallocate(allocation);
		}
	}

	template <class U>
	Table(const Table<U> &table)
			: _size(0), _allocatedSize(0), _array(NULL) {
		_reallocate(table.getSize());
		_size = table.getSize();
		for (unsigned int i = 0; i < _size; ++i) {
			_array[i] = table[i];
		}
	}

	Table(const Table &table)
			: _size(0), _allocatedSize(0), _array(NULL) {
		_reallocate(table._allocatedSize);
		_size = table._size;
		for (unsigned int i = 0; i < _size; ++i) {
			_array[i] = table._array[i];
		}
	}

	Table &operator=(const Table &table) {
		empty();
		_reallocate(table._allocatedSize);
		_size = table._size;
		for (unsigned int i = 0; i < table._size; ++i) {
			_array[i] = table._array[i];
		}
		return *this;
	}

	Table &operator+=(const Table &table) {
		for (unsigned int i = 0; i < table.getSize(); ++i) {
			insert(table[i]);
		}
		return *this;
	}

	~Table() {
		empty();
		//std::cout<<"DELETE TABLE"<<std::endl;
	}

	void swap(unsigned int father, unsigned int son) {
		T temp = _array[son];
		_array[son] = _array[father];
		_array[father] = temp;
	}

	unsigned int getSize() const {
		return _size;
	}

	T *getArray() {
		return _array;
	}

	T &getFirst() const {
		return _array[0];
	}

	T &getLast() const {
		return _array[_size - 1];
	}

	virtual T popFirst() {
		T result = _array[0];

		_array[0] = _array[_size - 1];

		--_size;

		_reallocateForRemove();

		return result;
	}

	virtual T popLast() {
		T result = _array[_size - 1];

		--_size;

		_reallocateForRemove();

		return result;
	}

	void preAllocate(unsigned int size) {
		if ((size < _allocatedSize && size < _size) || (size > _allocatedSize)) {
			_reallocate(size);
		}
	}

	void resize(unsigned int size) {
		// If there is more elements than the final size,
		// we remove them.
		while (size < _size) {
			removeAt(_size - 1);
		}
		// Reallocation
		_reallocate(size);
		_size = _allocatedSize;
	}

	virtual void pushLast(const T &element) {
		insert(element);
	}

	virtual void insert(const T &element) {
		_reallocateForInsert();

		++_size;

		_array[_size - 1] = element;
	}

	T &operator[](unsigned int index) {
		return _array[index];
	}

	const T &operator[](unsigned int index) const {
		return _array[index];
	}

	bool contains(const T &element) {
		T *current = _array;
		for (unsigned int i = 0; i < _size; ++i) {
			if (element == *current) {
				return true;
			}
			++current;
		}
		return false;
	}

	void remove(const T &element) {
		T *current = _array;
		for (unsigned int index = 0; index < _size; ++index) {
			if (element == (*current)) {
				fastRemoveAt(index);
				return;
			}
			++current;
		}
	}

	int indexOf(const T &element) {
		T *current = _array;
		int index = -1;
		for (index = 0; index < _size; ++index) {
			if (element == (*current)) {
				return index;
			}
			++current;
		}
		return index;
	}

	T &getAt(unsigned int index) {
		return _array[index];
	}

	virtual void fastRemoveAt(unsigned int index) {
		if (index >= _size)
			return;

		if (_size > 1)
			_array[index] = _array[_size - 1];

		_size--;

		_reallocateForRemove();
	}

	virtual void removeAt(unsigned int index) {
		if (index >= _size)
			return;

		for (unsigned int i = index; i < _size - 1; ++i) {
			_array[i] = _array[i + 1];
		}
		_size--;

		_reallocateForRemove();
	}

	void empty() {
		_size = 0;
		_allocatedSize = 0;
		if (_array) {
			delete[] _array;
		}
		_array = NULL;
	}

	void detail() {
		std::cout << "----- Table : -----\n";
		if (_size == 0) {
			std::cout << "Empty." << std::endl;
		} else {
			std::cout << "Size : " << _size << std::endl;

			for (unsigned int i = 0; i < _size; ++i) {
				std::cout << "i : " << i << ", " << _array[i] << std::endl;
			}
		}
	}

public:
	class TableFunctor {
	public:
		virtual void operator()(T *element) = 0;
	};

	void apply(TableFunctor &functor) {

		T *current = _array;
		for (unsigned int i = 0; i < _size; ++i) {
			functor(current);
			++current;
		}
	}

}; // class Table

template <class T>
std::ostream &operator<<(std::ostream &o, const Table<T> &table) {
	return o;
}

#endif // HEAP_H
