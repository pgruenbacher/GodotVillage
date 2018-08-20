/*******************************************************************************
 *
 *     Copyright : 2010 Arnaud Emilien
 *
 *******************************************************************************/

#include "PainterAlgorithms.h"
#include <cmath>
#include <cstdlib>

Table<Vector2I> PainterAlgorithms::findBresenhamLinePoints(const Vector2I& start,
								 const Vector2I& end)
{
	Table<Vector2I> resultPoints;
                       
        int dx = std::abs(end.x - start.x);
        int dy = std::abs(end.y - start.y);
        
        int signX = (start.x < end.x) ? 1 : -1 ;
        int signY = (start.y < end.y) ? 1 : -1 ;
        
        int diff;
        
        int currentX = start.x;
        int currentY = start.y;
                
        // Set the color to the current pixel
	resultPoints.pushLast(Vector2I(currentX, currentY));
        
        // Move to the next pixel
        if (dx > dy)
        {
                diff = dx / 2;
                while (currentX != end.x)
                {
                        diff -= dy;
                        if (diff < 0)
                        {
                                currentY += signY;
                                diff += dx;
                        }
                        currentX += signX;
                        resultPoints.pushLast(Vector2I(currentX, currentY));
                }
        }
        else
        {
                diff = dy / 2;
                while (currentY != end.y)
                {
                        diff -= dx;
                        if (diff < 0)
                        {
                                currentX += signX;
                                diff += dy;
                        }
                        currentY += signY;
                        resultPoints.pushLast(Vector2I(currentX, currentY));
                }
        }      
	return resultPoints;
}

Table<Vector2I> PainterAlgorithms::findEmptyPolygonPoints(const Table<Vector2I>& points)
{
	Table<Vector2I> resultPoints;

	// Check the size.
	unsigned int size = points.getSize();
	if (size == 0)
		return resultPoints;

	// Find all the lines.	
        Vector2I start, end;
	for (unsigned int i = 0; i < size; ++i)
	{
	        start = points[i];
		end   = (i >= size-1) ? points[0] : points[i+1];
		
		Table<Vector2I> line = findBresenhamLinePoints(start,end);
		
	        for (unsigned int j = 0; j < line.getSize(); ++j)
	        {
	                resultPoints.pushLast(line[j]);
	        }
	 }
	
	return resultPoints;
}

Table<Vector2I> PainterAlgorithms::findFilledPolygonPoints(const Table<Vector2I>& points)
{
	Table<Vector2I> resultPoints;
	unsigned int size = points.getSize();
        if (size== 0)
                return resultPoints;
        
        // Find ymin and ymax
        int ymin = points[0].y;
        int ymax = points[0].y;
	for (int i = 1; i < size; ++i)
	{
	        if (points[i].y > ymax)
	                ymax = points[i].y;
	        if (points[i].y < ymin)
	                ymin = points[i].y;
	}
	
	// Initialize the active edge table
	Table< LinkedList< PainterAlgorithms::Edge > > activeEdgeTable;
	activeEdgeTable.resize(ymax - ymin + 1);
        Vector2I start, end;
	for (unsigned int i = 0; i < size; ++i)
	{
		start = points[i];
		end   = (i == size-1) ? points[0] : points[i+1];
		
		// Ignore horizontal lines
		if (start.y == end.y)
		{
		        continue;
		} 
		// Order with start lower than end
		else if (start.y > end.y)
		{
		        Vector2I swap = end;
		        end = start;
		        start = swap;
		}
		// Add the edge
	        PainterAlgorithms::Edge edge;
		edge.yUpperPoint = end.y;
		edge.x           = start.x;
		edge.w           = float(start.x - end.x) / float(start.y - end.y);
		
		activeEdgeTable[start.y - ymin].sortedInsert(edge);
	}	
	
        for (unsigned int y = ymin; y <= ymax; ++y)
        {                 
                LinkedList< PainterAlgorithms::Edge >::LinkedListIterator it(&activeEdgeTable[y - ymin]);
                while (it.hasNext())
                {
                        PainterAlgorithms::Edge e = it.next();                 
                }
        }
        
        // Filling the polygon
        for (unsigned int y = ymin; y <= ymax; ++y)
        {                
                // Fill between the edges
                LinkedList< PainterAlgorithms::Edge >::LinkedListIterator it(&activeEdgeTable[y - ymin]);
                while (y != ymin && it.hasNext())
                {
                        PainterAlgorithms::Edge e1 = it.next();
                        PainterAlgorithms::Edge e2 = it.next();
                        for (unsigned int x = e1.x + 1; x < e2.x; ++x)
                        {
				resultPoints.pushLast(Vector2I(x, y));
                        }
                }
                
                // Update the table
                while (activeEdgeTable[y - ymin].getSize() > 0)
                {
                        // Take one edge
                        PainterAlgorithms::Edge edge = activeEdgeTable[y - ymin].popFirst();
                                       
                        // If the current y is equal to the max y of the edge
                        // We do not add it to the list
                        if (edge.yUpperPoint == y + 1)
                        {
                                continue;
                        }
                        
                        // Update the x position of the edge
                        edge.x += edge.w;
                        
                        // Insert the edge in the activeEdgeTable
                        activeEdgeTable[y - ymin + 1].sortedInsert(edge);                        
                }
        }
	return resultPoints;
}
