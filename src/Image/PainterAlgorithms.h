/*******************************************************************************
 *
 *     Copyright : 2010 Arnaud Emilien
 *
 *******************************************************************************/

#ifndef _PAINTER_ALGORITHMS_H
#define _PAINTER_ALGORITHMS_H

#include "../Maths/Vector.h"
#include "../Utils/Table.h"
#include "../Utils/LinkedList.h"
#include <iostream>


class PainterAlgorithms{
private :
	/** Structure used to compute the active edge table. */
	struct Edge{
		unsigned int yUpperPoint;
		float x;
		float w;
		
		bool operator<= (const Edge& e) const
			{
				if (yUpperPoint < e.yUpperPoint)
					return true;
				else if (yUpperPoint == e.yUpperPoint && x <= e.x)
					return true;
				return false;
			}
		
		friend std::ostream& operator<<(std::ostream& o, const Edge& e);
	}; // struct Edge
	
public :
        /**
	   Compute a line with the Bresenham algorithm and return the list of the points.
	   @param the begening of the line.
	   @param the end of the line.
	   @return the list of points.
	*/
	static Table<Vector2I> findBresenhamLinePoints(const Vector2I& start,
							const Vector2I& end);
	/**
	   Compute an empty polygon and return the list of the points.
	   @param the list of points corresponding to its border.
	   @return the list of points.
	*/
	static Table<Vector2I> findEmptyPolygonPoints(const Table<Vector2I>& points);	
	
	/**
	   Compute an filled polygon and return the list of the points.
	   @param the list of points corresponding to its border.
	   @return the list of points.
	*/
	static Table<Vector2I> findFilledPolygonPoints(const Table<Vector2I>& points);

}; // class PainterAlgorithms

#endif // _PAINTER_ALGORITHMS_H
