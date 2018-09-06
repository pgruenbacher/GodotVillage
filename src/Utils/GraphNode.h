/*******************************************************************************
 *
 *     Copyright : 2010 Arnaud Emilien
 *
 *******************************************************************************/

#ifndef _GRAPH_NODE_H
#define _GRAPH_NODE_H

#include "Pair.h"

// Pre-declaration
template <class C, class K, class V>
class Graph;

/** Element that composes the Graph. */
template <class C, class K, class V>
class GraphNode {
private:
	Pair<K, V> _pair;

public:
	GraphNode() {}
	GraphNode(K key, V value)
			: _pair(key, value) {}

	Pair<K, V> getPair() const { return _pair; }

	friend class Graph<C, K, V>;
};

template <class C, class K, class V>
std::ostream &operator<<(std::ostream &o, const GraphNode<C, K, V> &N) {
	o << N.getPair();
	return o;
}

#endif // _GRAPH_NODE_H
