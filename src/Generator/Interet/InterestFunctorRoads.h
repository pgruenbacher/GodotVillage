/*******************************************************************************
 *
 *     Copyright : 2010 Arnaud Emilien
 *
 *******************************************************************************/

#ifndef INTEREST_FUNCTOR_ROADS_H
#define INTEREST_FUNCTOR_ROADS_H

#include "HouseInterestFunctor.h"
#include <iostream>

template <class C, class K, class V>
class InterestFunctorRoads : public HouseInterestFunctor<C, K, V> {
public:
	InterestFunctorRoads() {}

	C evaluate(const K &position, HouseType housetype, Layout<C, K, V> *layout) {
		C value = housetype.roadsInterestWeight * layout->getValueAt(position);
		return value * value;
	}

}; // class InterestFunctorRoads

#endif // INTEREST_FUNCTOR_ROADS_H
