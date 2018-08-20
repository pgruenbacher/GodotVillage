/*******************************************************************************
 *
 *     Copyright : 2010 Arnaud Emilien
 *
 *******************************************************************************/

#ifndef STRAIT_SQUARE_ROOF_H
#define STRAIT_SQUARE_ROOF_H

#include "../../Engine/Mesh.h"
#include "TilesRoof.h"
#include "TileLine.h"
#include "Quad.h"
#include "Triangg.h"
#include "Chimney.h"

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

class StraitSquareRoof{
public :

	Quad _quad;
	Mesh _mesh;

        StraitSquareRoof(Quad quad)
		: _quad(quad)
	{}

	Mesh generate()
	{
		_mesh.empty();

		Vector3F pp0, pp1;

		pp0 = _quad.p0() + _quad.p01() / 2.f;
		pp1 = _quad.p3() + _quad.p32() / 2.f;

		pp0.y += 2.f;
		pp1.y += 2.f;
		
		Quad q1( _quad.p0(), pp0, pp1, _quad.p3());
		Quad q2( _quad.p2(), pp1,  pp0, _quad.p1());
		Triangg t1 (_quad.p0(), _quad.p1(), pp0);
		Triangg t2 (pp1, _quad.p2(), _quad.p3());

		//_mesh += q1.generate();
		//_mesh += q2.generate();
		_mesh += t1.generate();
		_mesh += t2.generate();


		float random = float(rand() % 1000) / 1000.f;
		Tile::Type type;
		if (random < 0.5)	    
		{
			type = Tile::ROUND;		
		}
		else if (random < 0.85)
		{
			type = Tile::FLAT;
		}
		else
		{
			type = Tile::CHAOS;
		}

		TilesRoof tiles1(q1, type);
		TilesRoof tiles2(q2, type);
		Quad q3(pp0 - normalize(_quad.p01()) * 0.05f, 
			pp1 - normalize(_quad.p01()) * 0.05f, 
			pp1 + normalize(_quad.p01()) * 0.05f, 
			pp0 + normalize(_quad.p01()) * 0.05f);
		//leLine tiles3(q3);

		_mesh += tiles1.generate();
		_mesh += tiles2.generate();
		//_mesh += tiles3.generate();
		
		Chimney ch(q2.rotateToY());
		_mesh += ch.generate();

		return _mesh;

	}
	

}; // class StraitSquareRoof

#endif // STRAIT_SQUARE_ROOF_H
