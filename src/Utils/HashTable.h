/*******************************************************************************
 *
 *     Copyright : 2010 Arnaud Emilien
 *
 *******************************************************************************/

#ifndef HASHTABLE_H
#define HASHTABLE_H

#include <cstdio>
#include <cstring>
#include <iostream>

#include "../Maths/Vector.h"
#include "Iterator.h"
#include "Pair.h"

template <class Key>
unsigned long hash(Key element) {
	return 0;
}

template <>
inline unsigned long hash<int>(int i) {
	return i;
}

template <>
inline unsigned long hash<Vector2I>(Vector2I vector) {
	return hash(vector.x + vector.x * vector.y);
}

template <>
inline unsigned long hash<const char *>(const char *str) {
	// Bernstein
	unsigned long hash = 5381;
	while (*str != '\0') {
		int c = *str;
		hash = ((hash << 5) + hash) + c;
		str++;
	}
	std::cout << "Hash : " << hash << std::endl;
	return hash;
}

template <>
inline unsigned long hash<unsigned int>(unsigned int i) {
	return i;
}

template <>
inline unsigned long hash<const std::string>(const std::string str) {
	return hash(str.c_str());
}

template <class Key, class Value>
class HashTable {

private:
	class Element {

	public:
		Pair<Key, Value> _pair;
		Element *_next;

		Element(Pair<Key, Value> pair)
				: _pair(pair), _next(NULL) {}

		~Element() {}

		Pair<Key, Value> getPair() {
			return _pair;
		}

		void setNext(Element *next) {
			_next = next;
		}

		Element *getNext() const {
			return _next;
		}

	}; // class Element

private:
	unsigned int _size;
	unsigned int _allocatedSize;
	Element **_elementsArray;

	unsigned int _getIndex(const Key &key) const {
		return hash(key) % _allocatedSize;
	}

public:
	HashTable(unsigned int size = 53810)
			: _size(0), _allocatedSize(size) {
		if (_allocatedSize == 0) {
			std::cout << "ERROR - Tried to create an HashTable with size 0" << std::endl;
		}
		_elementsArray = new Element *[_allocatedSize];
		for (unsigned int i = 0; i < _allocatedSize; ++i) {
			_elementsArray[i] = NULL;
			//std::cout << "NULL" << std::endl;
		}
	}

	~HashTable() {
		empty();
		delete[] _elementsArray;
	}

	unsigned int getSize() const {
		return _size;
	}

	void empty() {
		Element *temp = NULL;
		Element *toDelete = NULL;
		for (unsigned int i = 0; i < _allocatedSize; ++i) {
			temp = _elementsArray[i];
			while (temp != NULL) {
				toDelete = temp;
				temp = temp->getNext();
				delete toDelete;
			}
			_elementsArray[i] = NULL;
		}
		_size = 0;
	}

	bool contains(const Key &key) const {
		Element *temp = _elementsArray[_getIndex(key)];
		if (temp == NULL) {
			return false;
		} else {
			while (temp != NULL) {
				if (temp->_pair.getKey() == key) {
					return true;
				}
				temp = temp->getNext();
			}
			return false;
		}
	}

	Pair<Key, Value> &find(const Key &key) {
		Element *temp = _elementsArray[_getIndex(key)];
		while (temp != NULL) {
			if (temp->getPair().getKey() == key) {
				return temp->_pair;
			}
			temp = temp->getNext();
		}
		// Will segfault if the user does not use "contains()"
		return temp->_pair;
	}

	Value &operator[](const Key &key) {
		Element *temp = _elementsArray[_getIndex(key)];
		while (temp != NULL) {
			if (temp->getPair().getKey() == key) {
				return temp->_pair.getValue();
			}
			temp = temp->getNext();
		}
		return temp->_pair.getValue();
	}

	void insert(const Key &key, const Value &value) {
		insert(Pair<Key, Value>(key, value));
	}

	void insert(Pair<Key, Value> pair) {
		Key key = pair.getKey();
		unsigned int index = _getIndex(key);
		//std::cout << "index " << index << std::endl;
		Element *temp = _elementsArray[index];
		if (temp == NULL) {
			_elementsArray[index] = new Element(pair);
		} else {
			while (temp->getNext() != NULL) {
				temp = temp->getNext();
				if (temp->getPair().getKey() == key) {
					// HASHTABLE CONTAINS THE KEY
					std::cout << "WARNING - HashTable contains key, pair not inserted" << std::endl;
					return;
				}
			}
			temp->setNext(new Element(pair));
		}
		++_size;
	}

	Pair<Key, Value> remove(const Key &key) {
		unsigned int index = _getIndex(key);
		Element *temp = _elementsArray[index];
		if (temp == NULL) {
			return Pair<Key, Value>();
		} else if (temp->getPair().getKey() == key) {
			Pair<Key, Value> result = temp->getPair();
			_elementsArray[index] = temp->getNext();
			--_size;
			delete temp;
			return result;
		} else {
			while (temp->getNext() != NULL) {
				if (temp->getNext()->getPair().getKey() == key) {
					Element *toDelete = temp->getNext();
					Pair<Key, Value> result = temp->getNext()->getPair();
					temp->setNext(temp->getNext()->getNext());
					--_size;
					delete toDelete;
					return result;
				}
				temp = temp->getNext();
			}
			return Pair<Key, Value>();
		}
	}

	void detail() {
		std::cout << "HASHTABLE  size=" << getSize() << std::endl;
		HashTableIterator it(this);
		while (it.hasNext()) {
			it.next();
			std::cout << "Key ["
					  << it.value().getKey()
					  << "] = \"" << it.value().getValue()
					  << "\""
					  << " pos : "
					  << it.getPosition()
					  << std::endl;
		}
	}

	class HashTableIterator : public IteratorBase<Pair<Key, Value> > {
	public:
		HashTableIterator(HashTable<Key, Value> *hTable)
				: _parent(hTable),
				  _current(NULL),
				  _position(-1) {}

		int getPosition() {
			return _position;
		}

		bool hasNext() {
			// cas trivial => _position > _allocatedSize
			if (_position + 1 > _parent->_allocatedSize) {
				return false;
			}
			// sinon on cherche s'il exite un suivant
			//  - dans la liste courante
			if (_current != NULL) {
				// si l'element current a un suivant
				// alors il y a un suivant
				// => captain obvious to the rescue
				if (_current->_next != NULL) {
					return true;
				}
			}
			//  - sinon, on cherche dans le tableau
			//    jusqu'a trouve un element non null
			for (unsigned long val = _position + 1;
					val < _parent->_allocatedSize;
					++val) {
				// on trouve un element non null
				if (_parent->_elementsArray[val] != NULL) {
					return true;
				}
			}
			return false;
		}

		Pair<Key, Value> &next() {
			// si il y a un element suivant
			// a l'element courant, on le prend
			if (_current != NULL && _current->_next != NULL) {
				_current = _current->_next;
				return _current->_pair;
			}
			// sinon on cherche dans les positions supérieures
			for (unsigned long val = _position + 1;
					val < _parent->_allocatedSize;
					++val) {
				if (_parent->_elementsArray[val] != NULL) {
					_position = val;
					_current = _parent->_elementsArray[val];
					return _current->_pair;
				}
			}
			return _current->_pair;
		}

		Pair<Key, Value> &value() {
			return _current->_pair;
		}

	private:
		HashTable<Key, Value> *_parent;
		Element *_current;
		long long int _position;
	};

	friend class HashTableIterator;
}; // class HashTable

#endif // HASHTABLE_H
