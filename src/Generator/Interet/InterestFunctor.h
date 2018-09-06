/*******************************************************************************
 *
 *     Copyright : 2010 Arnaud Emilien
 *
 *******************************************************************************/

#ifndef INTEREST_FUNCTOR_H
#define INTEREST_FUNCTOR_H

template <class C, class K, class V>
class InterestFunctor {

public:
	virtual C evaluate(const K &key, HouseType housetype, Layout<C, K, V> *layout) = 0;

}; // class InterestFunctor

#endif // INTEREST_FUNCTOR_H
