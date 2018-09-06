/*******************************************************************************
 *
 *     Copyright : 2010 Arnaud Emilien
 *
 *******************************************************************************/

#ifndef _COST_FUNCTOR_VEGETATION_H
#define _COST_FUNCTOR_VEGETATION_H

#include "RoadCostFunctor.h"
#include <iostream>

template <class C, class K, class V>
class CostFunctorVegetation : public RoadCostFunctor<C, K, V> {
public:
	CostFunctorVegetation() {}
	~CostFunctorVegetation() {}

	virtual C evaluate(GraphArc<C, K, V> &arc,
			Graph<C, K, V> *graph,
			//Table< GraphNode<C,K,V> >& nodesPath,
			Table<K> &path,
			Layout<C, K, V> &layout) {
		//std::cout << "CostFunctorVegetation :";
		K keySource = arc.getSourceNodeKey();
		K keyDest = arc.getDestNodeKey();
		C valSource = layout.getValueAt(keySource);
		C valDest = layout.getValueAt(keyDest);
		C result = valDest;
		std::cout << " vegetation result : " << result << std::endl;
		return result;
	}
}; // class CostFunctorVegetation

#endif // _COST_FUNCTOR_VEGETATION_H
