/*******************************************************************************
 *
 *     Copyright : 2010 Arnaud Emilien
 *
 *******************************************************************************/

#ifndef _HOUSES_COST_FUNCTOR_H
#define _HOUSES_COST_FUNCTOR_H

#include <iostream>
#include "../../PathFinding/CostFunctor.h"
#include "../Road.h"


template<class C, class K, class V>
class CostFunctorHouses : public CostFunctor<C,K,V>{

public :

	CostFunctorHouses(){}
	~CostFunctorHouses() {}
	
	C evaluate(GraphArc<C,K,V>& arc, 
	                   Graph<C,K,V>* graph,
			   Table<K>& path,
			   Layout<C,K,V>& layout)
	{
	        K keyDest   = arc.getDestNodeKey();
	        C cost      = layout.getValueAt(keyDest);
	        return cost*C(999999);
	}
	                     
	
}; // class CostFunctorHouses


#endif // _HOUSES_FUNCTOR_VEGETATION_H
