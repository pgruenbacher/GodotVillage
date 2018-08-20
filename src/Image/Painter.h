/*******************************************************************************
 *
 *     Copyright : 2010 Arnaud Emilien
 *
 *******************************************************************************/

#ifndef _PAINTER_H
#define _PAINTER_H

#include "Image.h"
#include "../Maths/Vector.h"
#include "../Utils/Table.h"
#include "../Utils/LinkedList.h"
#include <iostream>

class Painter{

public :
        static void drawRectangle(const Vector2I& bottomLeft,
                                  const Vector2I& topRight,
                                  Image* image,
                                  const Vector3F& color);
                                  

        static void drawPoints(Table<Vector2I>& points,
                               Image* image,
                               const Vector3F& color);

	/**
	   Draw a line with the Bresenham algorithm on the given Image.
	   @param the begening of the line.
	   @param the end of the line.
	   @param the Image to draw in.
	   @param the color of drawing.
	*/
	static void drawBresenhamLine(const Vector2I& start,
                                      const Vector2I& end,
                                      Image* image,
                                      const Vector3F& color,
				      int width = 1);
	
	/**
	   Draw an empty polygon on the given Image. 
	   The algorithm will automatically close the polygon.
	   @param the list of points of the polygon. 
	   @param the Image to draw in.
	   @param the color of drawing.
	*/
	   
        static void drawEmptyPolygon(const Table<Vector2I>& points,
                                     Image* image,
                                     const Vector3F& color);
     	/**
	   Draw a filled polygon on the given Image. 
	   The algorithm will automatically close the polygon.
	   @param the list of points of the polygon. 
	   @param the Image to draw in.
	   @param the color of drawing.
	*/                                 
        static void drawFilledPolygon(const Table<Vector2I>& points,
				      Image* image,
				      const Vector3F& color);

        static void drawCircle(const Vector2I& origin,
				 int radius,
				 Image* image,
				 const Vector3F& color);

	static void drawGaussian(const Vector2I& origin,
				 int radius,
				 Image* image,
				 const Vector3F& color,
				 float alpha = 1.f);

	static Vector3F getGaussian(const Vector2I& origin,
				    int radius,
				    Image* image);

	static Vector3F getColor(const Vector2I& position,
				 Image* image);

        static void drawPoint(const Vector2I& point,
			      Image* image,
			      const Vector3F& color);

}; // class Painter

#endif // _PAINTER_H
