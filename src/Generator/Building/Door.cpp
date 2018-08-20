/*******************************************************************************
 *
 *     Copyright : 2010 Arnaud Emilien
 *
 *******************************************************************************/

#include "Door.h"
#include "../VillageGenerator.h"

	Mesh Door::generate()
	{
		std::cout << "Generate Door" << std::endl;

		float totalWidth = norm( _quad.p01());		
		float windowWidth = 1.f;
		float offset = (totalWidth - windowWidth) / 2.f;
		Table<Quad> partDiv1 = _quad.cutLeft(totalWidth-offset);
		Table<Quad> partDiv2 = partDiv1[0].cutLeft(offset);

		_mesh += partDiv1[1].generate();
		_mesh += partDiv2[0].generate();



		Quad windowPart = partDiv2[1];


		Table<Quad> extrusion = windowPart.extrude(- 0.3);

	        VillageGenerator::_instance->_wood.pushLast(extrusion[0].generate());
                _mesh += extrusion[1].generate();
                _mesh += extrusion[2].generate();
                _mesh += extrusion[3].generate();
                _mesh += extrusion[4].generate();

		Quad stairQuad(windowPart.p0() + Vector3F(0.0, -2.0, 0.0),
			       windowPart.p1() + Vector3F(0.0, -2.0, 0.0),
			       windowPart.p1(),
			       windowPart.p0());
		_mesh += Stair(stairQuad).generate();

		return _mesh;
	}
