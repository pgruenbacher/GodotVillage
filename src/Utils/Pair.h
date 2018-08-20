/*******************************************************************************
 *
 *     Copyright : 2010 Arnaud Emilien
 *
 *******************************************************************************/

#ifndef _PAIR_H
#define _PAIR_H

#include <iostream>

/** A pair contains a Key and a Value. */

template <class K, class V>
class Pair;

template <class K, class V>
std::ostream& operator<<(std::ostream& o, const Pair<K,V>& P );

template <class K, class V>
class Pair{
private :
        K _key;
        V _value;
public : 
	Pair(){}
        Pair(K key, V value) : _key(key), _value(value) {}
        K& getKey()   { return _key; }
        V& getValue() { return _value; } 
        
	bool operator<(const Pair<K,V>& p)
	{
		return _key < p._key;
	}
        
	bool operator>(const Pair<K,V>& p)
	{
		return _key > p._key;
	}
        friend std::ostream& operator<< <>(std::ostream& o, const Pair<K,V>& P );
};

template <class K, class V>
std::ostream& operator<<(std::ostream& o, const Pair<K,V>& P )
{
	o << P._key << ", " << P._value;
	return o;
}


#endif // _PAIR_H
