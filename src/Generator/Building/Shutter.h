/*******************************************************************************
 *
 *     Copyright : 2010 Arnaud Emilien
 *
 *******************************************************************************/

#ifndef SHUTTER_H
#define SHUTTER_H

#include "Quad.h"
#include "../../Engine/Mesh.h"


class Shutter{
public :
	enum Type{ SQUARE,
		   STYLIZED};
		   
	Mesh _mesh;
	Type _type;
	Quad _quad;

	Shutter(Quad quad)
		: _quad(quad)
	{}

	Mesh generate()
	{
		_mesh.empty();

		
		_type = SQUARE;

		
		switch(_type)
		{
		case SQUARE :
		{
			Table<Quad> extrude = _quad.extrude(0.05);
			for (unsigned int i = 0; i < extrude.getSize(); ++i)
			{
				_mesh += extrude[i].generate();
			}
			_mesh += Quad(_quad.p3(), _quad.p2(), _quad.p1(), _quad.p0()).generate();
		}
		break;

		case STYLIZED :
		{
			Vector3F pp0, pp1, pp2, pp3, pp4, pp5;
			pp0 = _quad.p0() + 0.05f * normalize(_quad.p01()) + 0.05f * normalize(_quad.p03());
			pp1 = _quad.p1() - 0.05f * normalize(_quad.p01()) + 0.05f * normalize(_quad.p03());
			pp2 = _quad.p0() + 0.5f * _quad.p03() + 0.05f * normalize(_quad.p01()) ;
			pp3 = _quad.p1() + 0.5f * _quad.p12() - 0.05f * normalize(_quad.p01());
			pp4 = _quad.p3() + 0.05f * normalize(_quad.p01()) - 0.05f * normalize(_quad.p03());
			pp5 = _quad.p2() - 0.05f * normalize(_quad.p01()) - 0.05f * normalize(_quad.p03());

			_mesh += Mesh::plane(pp0 + Vector3F(0,0.1,0), pp1 + Vector3F(0,0.1,0), 0.20, 0.05);
			_mesh += Mesh::plane(pp2, pp3, 0.20, 0.05);
			_mesh += Mesh::plane(pp4 - Vector3F(0,0.1,0), pp5 - Vector3F(0,0.1,0), 0.20, 0.05);
			_mesh += Mesh::plane(pp0, pp4, 0.05, 0.15);
			_mesh += Mesh::plane(pp1, pp5, 0.05, 0.15);

			float height =  norm(_quad.p03());
			int nb = height / 0.10;

			
			

		}
		break;
		}	


		return _mesh;

	}
};

#endif // SHUTTER_H
