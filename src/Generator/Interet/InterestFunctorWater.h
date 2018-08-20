/*******************************************************************************
 *
 *     Copyright : 2010 Arnaud Emilien
 *
 *******************************************************************************/

#ifndef INTEREST_FUNCTOR_WATER_H
#define INTEREST_FUNCTOR_WATER_H

#include "HouseInterestFunctor.h"
#include <iostream>

template <class C, class K, class V>
class InterestFunctorWater : public HouseInterestFunctor<C,K,V>{
public :
        InterestFunctorWater(){}
        
	C evaluate(const K& position, HouseType housetype, Layout<C,K,V>* layout)
	{
	        C value = housetype.waterInterestWeight * layout->getValueAt(position);
	        return  value*value ;
	}

}; // class InterestFunctorWater


#endif // INTEREST_FUNCTOR_WATER_H
