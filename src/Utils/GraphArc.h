/*******************************************************************************
 *
 *     Copyright : 2010 Arnaud Emilien
 *
 *******************************************************************************/

#ifndef _GRAPH_ARC_H
#define _GRAPH_ARC_H

// Pre-declaration
template <class C, class K, class V>
class Graph;

// Pre-declaration
template <class C, class K, class V>
class GraphNode;

/** Element that links two Nodes of the Graph. */
template <class C, class K, class V>
class GraphArc {
private:
	K _sourceNodeKey;
	K _destNodeKey;
	C _travelCost;

public:
	GraphArc(const K &sourceNodeKey,
			const K &destNodeKey,
			const C &cost)
			: _sourceNodeKey(sourceNodeKey), _destNodeKey(destNodeKey), _travelCost(cost) {}

	GraphArc() {}

	inline void setSourceNodeKey(const K &key) { _sourceNodeKey = key; }
	inline void setDestNodeKey(const K &key) { _destNodeKey = key; }
	inline void setTravelCost(const C &cost) { _travelCost = cost; }

	inline K getSourceNodeKey() const { return _sourceNodeKey; }
	inline K getDestNodeKey() const { return _destNodeKey; }
	inline C getTravelCost() const { return _travelCost; }

	friend class Graph<C, K, V>;
};

template <class C, class K, class V>
std::ostream &operator<<(std::ostream &o, const GraphArc<C, K, V> &A) {
	o << A.getSourceNodeKey()
	  << " -> "
	  << A.getDestNodeKey()
	  << " ,  "
	  << A.getTravelCost();
	return o;
}

#endif // _GRAPH_ARC_H
