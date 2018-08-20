/*******************************************************************************
 *
 *     Copyright : 2010 Arnaud Emilien
 *
 *******************************************************************************/

#ifndef STAIR_H
#define STAIR_H

#include "../../Engine/Mesh.h"
#include "Quad.h"

class Stair{
public :
	Quad _quad;
	Mesh _mesh;

	Stair(Quad quad)
		: _quad(quad)
	{}

	Mesh generate()
	{
		std::cout << "Generate Stair" << std::endl;
		_mesh.empty();

		float step = 0.15;
		float stepDepth = 0.1;
		int nbStep = int(norm(_quad.p03()) / step);
		Quad current = _quad.rotateToY().rotateToY();
		for (unsigned int i = 0; i < nbStep; ++i)
		{
			Table<Quad> temp = current.rotateToY().cutLeft(step);

			Table<Quad> extrude = temp[0].revertToX().extrude(stepDepth * float(i));
			_mesh += generateTable(extrude);

			current = temp[1].revertToX();
		}
			

		return _mesh;
	}

}; // class Stair

#endif // STAIR_H
