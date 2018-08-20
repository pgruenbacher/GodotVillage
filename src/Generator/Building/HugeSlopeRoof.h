/*******************************************************************************
 *
 *     Copyright : 2010 Arnaud Emilien
 *
 *******************************************************************************/

#ifndef HUGE_SLOPE_ROOF
#define HUGE_SLOPE_ROOF

#include "../../Engine/Mesh.h"
#include "TilesRoof.h"
#include "Quad.h"
#include "Chimney.h"

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

class HugeSlopeRoof{

public :
	Quad _quad;
	Mesh _mesh;

	HugeSlopeRoof(){}

	HugeSlopeRoof(Quad quad)
		: _quad(quad)
	{}

	Mesh generate()
	{
		_mesh.empty();

		float backHeight  = 3.0f;
		float frontHeight = 0.1f;
		float frontDepth  = 2.0f;
		float frontOffset = (backHeight - frontHeight) * (frontDepth / norm( _quad.p01()));

		Vector3F pp0, pp1, pp2, pp3, pp4, pp5;
		pp0 = _quad.p0() - frontDepth * normalize(_quad.p01()) - frontOffset * _quad.normal();
		pp1 = _quad.p3() - frontDepth * normalize(_quad.p01()) - frontOffset * _quad.normal();
		pp2 = pp0 + _quad.normal() * frontHeight;
		pp3 = pp1 + _quad.normal() * frontHeight;
		pp4 = _quad.p1() + (frontHeight + backHeight) * _quad.normal();
		pp5 = _quad.p2() + (frontHeight + backHeight) * _quad.normal();

		Quad q0(_quad.p0(), pp0, pp1, _quad.p3());
		Quad q1(pp0, pp2, pp3, pp1);
		Quad q2(pp2, pp4, pp5, pp3);
		Quad q3(_quad.p0(), _quad.p1(), pp4, pp2);
		Quad q4(_quad.p3(), pp3, pp5, _quad.p2());
		Quad q5(_quad.p1(), _quad.p2(), pp5, pp4);
		Triangg t0(pp0, _quad.p0(), pp2);
		Triangg t1(pp1, pp3, _quad.p3()); 


		Vector3F bb0 = pp0 + 0.2f * normalize(_quad.p01());
		Vector3F bb1 = pp1 + 0.2f * normalize(_quad.p01());		
		_mesh += Mesh::cube(bb0, bb1, 0.2);

		Vector3F cc0 = bb0 - Vector3F(0, 0.1, 0) + 0.4f * normalize(_quad.p03()) - 0.1f * normalize(_quad.p01());
		Vector3F cc1 = _quad.p0() + 0.4f * normalize(_quad.p03());
		cc1.y = cc0.y;		
		_mesh += Mesh::cube(cc0, cc1, 0.15);

		Vector3F cc2 = bb1 - Vector3F(0, 0.1, 0) - 0.4f * normalize(_quad.p03()) - 0.1f * normalize(_quad.p01());
		Vector3F cc3 = _quad.p3() - 0.4f * normalize(_quad.p03());
		cc3.y = cc2.y;		
		_mesh += Mesh::cube(cc2, cc3, 0.15);

		Vector3F dd0 = cc0 + 0.4f * normalize(cc1-cc0);
		Vector3F dd1 = cc1 - Vector3F(0, 0.7, 0);
		_mesh += Mesh::cube(dd0, dd1, 0.1);

		Vector3F dd2 = cc2 + 0.4f * normalize(cc3-cc2);
		Vector3F dd3 = cc3 - Vector3F(0, 0.7, 0);
		_mesh += Mesh::cube(dd2, dd3, 0.1);


		Vector3F hh0 = dd1 + Vector3F(0, -0.1, 0);
		Vector3F hh1 = dd1 + Vector3F(0.01, 0.9, 0);
		Vector3F hh2 = dd3 + Vector3F(0, -0.1, 0);
		Vector3F hh3 = dd3 + Vector3F(0.01, 0.9, 0);

		_mesh += Mesh::cube(hh0, hh1, 0.2); 
		_mesh += Mesh::cube(hh2, hh3, 0.2);
		


		_mesh += q0.generate();
		_mesh += q1.generate();
		_mesh += q3.generate();
		_mesh += q4.generate();
		_mesh += q5.generate();
		_mesh += t0.generate();
		_mesh += t1.generate();


		//_mesh += q2.generate();

		float random = float(rand() % 1000) / 1000.f;
		Tile::Type type;
		if (random > 0.5)	    
		{
			type = Tile::ROUND;		
		}
		else
		{
			type = Tile::FLAT;
		}


		TilesRoof tiles(q2, type);
		_mesh += tiles.generate();


		Chimney ch(q2.rotateToY());
		_mesh += ch.generate();
		

		return _mesh;
	}

}; // class HugeSlopeRoof


#endif // HUGE_SLOPE_ROOF
