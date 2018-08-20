/*******************************************************************************
 *
 *     Copyright : 2010 Arnaud Emilien
 *
 *******************************************************************************/

#ifndef _IMPLICIT_GRIDED_GRAPH_H
#define _IMPLICIT_GRIDED_GRAPH_H

#include "Graph.h"


enum GridedGraphNeighbourhood{ FOUR
			       , EIGHT
			       , SIXTEEN
			       , THIRTY_TWO
			       , FOURTY_TWO
};

template <class C, class V>
class ImplicitGridedGraph : public Graph<C,Vector2I,V>{
private :
        /** Prohibited inherited method. */
	void addNode(const Vector2I& nodeKey, const V& nodeValue){}
	
        /** Prohibited inherited method. */
        HashTable<Vector2I, GraphNode<C,Vector2I,V> > h;
	HashTable<Vector2I, GraphNode<C,Vector2I,V> >& getNodes(){ return h;}
		
        /** Prohibited inherited method. */
	void addArc(const Vector2I& sourceNodeKey, 
		    const Vector2I& destNodeKey,
			const C& initCost){}
		    
        /** Prohibited inherited method. */
	void addTwoArcs(const Vector2I& nodeKey1, 
			const Vector2I& nodeKey2,
			const C& initCostNode1toNode2,
			const C& initCostNode2toNode1){}
			
protected :
	unsigned int                           _gridWidth;
	unsigned int                           _gridHeight;
	unsigned int                           _scale;
	LinkedList< GraphArc <C,Vector2I,V> >  _arcsPattern;
	LinkedList< GraphArc <C,Vector2I,V>* > _arcCache;
	LinkedList< GraphArc <C,Vector2I,V>* > _arcCache2;
	GraphNode<C,Vector2I,V>                _nodeCache;
   
	V                                      _getNodeValue(const Vector2I& nodeKey);
	LinkedList< GraphArc <C,Vector2I,V>* >& _createArcs(const Vector2I& nodeKey);
	bool                                   _isACorrectKey(const Vector2I& nodeKey);

public :	
	/**
	 *
	 */
	ImplicitGridedGraph(unsigned int gridWidth,
	                    unsigned int gridHeight,
	                    LinkedList< GraphArc <C,Vector2I,V> >& arcsPattern,
			    unsigned int scale = 1);
	/**
	 *
	 */
	ImplicitGridedGraph(unsigned int gridWidth,
	                    unsigned int gridHeight,
	                    GridedGraphNeighbourhood neighbourhood,
			    unsigned int scale = 1);
	
	/**
	 *
	 */
	void setPattern(LinkedList< GraphArc <C,Vector2I,V> >& arcsPattern);
	
	/**
	 *
	 */
	~ImplicitGridedGraph();
	
	/**
	 *
	 */
	bool hasArcs(const Vector2I& nodeKey);
	
	/**
	 *
	 */
	bool contains(const Vector2I& nodeKey);
	
	/**
	 * Return the vector of the Arcs that begins from the node nodeKey.
	 */
	LinkedList< GraphArc<C,Vector2I,V>* >&  getArcsFromNode(const Vector2I& nodeKey);	
		
	/**
	 *
	 */
	GraphNode<C,Vector2I,V>& getNode(const Vector2I& nodeKey);

	/**
	 *
	 */
	void detail();

}; // class ImplicitGridedGraph

// Includes the implementation
#include "ImplicitGridedGraph.impl"

#endif // _IMPLICIT_GRIDED_GRAPH_H
