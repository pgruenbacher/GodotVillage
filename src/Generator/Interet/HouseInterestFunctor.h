/*******************************************************************************
 *
 *     Copyright : 2010 Arnaud Emilien
 *
 *******************************************************************************/

#ifndef HOUSE_INTEREST_FUNCTOR_H
#define HOUSE_INTEREST_FUNCTOR_H

#include "../House.h"
#include "InterestFunctor.h"


template <class C, class K, class V>
class HouseInterestFunctor : public InterestFunctor<C,K,V>{

protected :
	HouseType _houseType;
	
public :
        HouseInterestFunctor(){}
        void setHouseType(HouseType type) { _houseType = type; }
        
	C evaluate(const K& position, HouseType housetype, Layout<C,K,V>* layout)
	{
	}


}; // class HouseInterestFunctor

#endif // HOUSE_INTEREST_FUNCTOR_H
