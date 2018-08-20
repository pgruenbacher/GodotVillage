/*******************************************************************************
 *
 *     Copyright : 2010 Arnaud Emilien
 *
 *******************************************************************************/

#ifndef _COST_FUNCTOR_WATER_H
#define _COST_FUNCTOR_WATER_H

#include "RoadCostFunctor.h"
#include <iostream>


template<class C, class K, class V>
class CostFunctorWater : public RoadCostFunctor<C,K,V>{
public :
	CostFunctorWater(){}
	~CostFunctorWater() {}
	
	virtual C evaluate(GraphArc<C,K,V>& arc, 
	                   Graph<C,K,V>* graph,
			   Table<K>& path,
			   Layout<C,K,V>& layout) 
	{
	        //K keySource = arc.getSourceNodeKey();
	        K keyDest   = arc.getDestNodeKey();
	        //C valSource = layout.getValueAt(keySource);
	        C valDest   = layout.getValueAt(keyDest);
	        C result(0);	        
	        
	        result = this->_usedRoad._waterWeight * pow (valDest, 2.0);
	        
	        return result;
	}
	
}; // class CostFunctorWater


#endif // _COST_FUNCTOR_WATER_H
