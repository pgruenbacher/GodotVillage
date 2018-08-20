/*******************************************************************************
 *
 *     Copyright : 2010 Arnaud Emilien
 *
 *******************************************************************************/

#ifndef _PATH_FINDING_H
#define _PATH_FINDING_H

#include "../Utils/HashTable.h"
#include "../Utils/Table.h"
#include "../Utils/Heap.h"
#include "../Utils/Layout.h"
#include "../Maths/Vector.h"
#include "CostFunctor.h"

template <class C, class K, class V>
class PathFinding
{
private :
    class KeyAndCost
    {
    private :
        K _key;
        static HashTable<K, C> _estimatedTotalTravelCosts;

    public :
        KeyAndCost(K key)
            : _key(key)
        {
        }

        KeyAndCost()
        {
        }

        K getKey()
        {
            return _key;
        }

        bool operator>(const KeyAndCost& keyAndCost) const
        {
            return _estimatedTotalTravelCosts[_key] > _estimatedTotalTravelCosts[keyAndCost._key];
        }

        bool operator<(const KeyAndCost& keyAndCost) const
        {
            return _estimatedTotalTravelCosts[_key] < _estimatedTotalTravelCosts[keyAndCost._key];
        }

        bool operator==(const KeyAndCost& keyAndCost) const
        {
            return _key == keyAndCost._key;
        }

        static HashTable<K, C>& getEstimatedTotalCosts()
        {
            return _estimatedTotalTravelCosts;
        }

        friend class PathFinding;
    };

public :
    static K getMinCost(Heap<K>& nodesToVisit, HashTable<K,C>& estimatedTotalTravelCosts);

    static C computeTravelCost(GraphArc<C,K,V>* arc,
                               Graph<C,K,V>* graph,
                               LinkedList< K >& path,
                               LinkedList< Pair<CostFunctor<C,K,V>*, Layout<C,K,V>* > >& costLayouts);

    static LinkedList<K> aStar(const K& sourceNodeKey,
                               const K& targetNodeKey,
                               Graph<C,K,V>* graph,
                               LinkedList< Pair<CostFunctor<C,K,V>*, Layout<C,K,V>* > >& costLayouts);

    static LinkedList<K> reconstructPath(HashTable<K,K>& fathers, K& goal, bool unlimited = true, unsigned int maxLength = 2);

    static LinkedList< GraphNode<C,K,V> > reconstructNodesPath(Table<K>& path, Graph<C,K,V>* graph);

}; // class PathFinding

// Implementation include
#include "PathFinding.impl"

#endif // _PATH_FINDING_H
