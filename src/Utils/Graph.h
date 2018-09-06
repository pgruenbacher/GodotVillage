/*******************************************************************************
 *
 *     Copyright : 2010 Arnaud Emilien
 *
 *******************************************************************************/

#ifndef _GRAPH_H
#define _GRAPH_H

#include "GraphArc.h"
#include "GraphNode.h"
#include "HashTable.h"
#include "Table.h"

template <class C, class K, class V>
class Graph {
protected:
	HashTable<K, GraphNode<C, K, V> > _nodes;
	HashTable<K, LinkedList<GraphArc<C, K, V> *> > _arcs;

public:
	/**
     *
     */
	Graph();

	/**
     *
     */
	virtual ~Graph();

	/**
     * Add a Node to the Graph.
     */
	virtual void addNode(const K &nodeKey, const V &nodeValue);

	/**
     *
     */
	virtual bool hasArcs(const K &nodeKey);

	/**
     *
     */
	virtual bool contains(const K &nodeKey);

	/**
     * Return the vector of the Arcs that begins from the node nodeKey.
     */
	virtual LinkedList<GraphArc<C, K, V> *> &getArcsFromNode(const K &nodeKey);

	/**
     *
     */
	virtual HashTable<K, GraphNode<C, K, V> > &getNodes();

	/**
     *
     */
	virtual GraphNode<C, K, V> &getNode(const K &nodeKey);

	/**
     * Create one arc, from the source Node to the destinatio Node.
     */
	virtual void addArc(const K &sourceNodeKey,
			const K &destNodeKey,
			const C &initCost);

	/**
     * Create two arcs, one for each direction.
     */
	virtual void addTwoArcs(const K &nodeKey1,
			const K &nodeKey2,
			const C &initCostNode1toNode2,
			const C &initCostNode2toNode1);

	/**
     *
     */
	virtual void detail();

}; // class Graph

// Includes the implementation
#include "Graph.impl"

#endif // _GRAPH_H
