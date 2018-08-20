/*******************************************************************************
 *
 *     Copyright : 2010 Arnaud Emilien
 *
 *******************************************************************************/

#ifndef INTEREST_FUNCTOR_HEIGHT_H
#define INTEREST_FUNCTOR_HEIGHT_H

#include "HouseInterestFunctor.h"
#include <iostream>

template <class C, class K, class V>
class InterestFunctorHeight : public HouseInterestFunctor<C,K,V>{
public :
        InterestFunctorHeight(){}
        
	C evaluate(const K& position, HouseType housetype, Layout<C,K,V>* layout)
	{
	        C value = housetype.heightInterestWeight * layout->getValueAt(position);
	        return  value*value ;
	}

}; // class InterestFunctorHeight


#endif // INTEREST_FUNCTOR_HEIGHT_H
