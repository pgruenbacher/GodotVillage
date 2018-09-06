/*******************************************************************************
 *
 *     Copyright : 2010 Arnaud Emilien
 *
 *******************************************************************************/

#ifndef _COST_FUNCTOR_H
#define _COST_FUNCTOR_H

#include "../Utils/Graph.h"
#include "../Utils/Layout.h"
#include "../Utils/Table.h"
#include <iostream>

template <class C, class K, class V>
class CostFunctor {
public:
	CostFunctor(){};
	~CostFunctor(){};
	virtual C evaluate(GraphArc<C, K, V> *arc,
			Graph<C, K, V> *graph,
			//Table< GraphNode<C,K,V> >& nodesPath,
			LinkedList<K> &path,
			Layout<C, K, V> &layout) = 0;

}; // class CostFunctor

#endif // _COST_FUNCTOR_H
