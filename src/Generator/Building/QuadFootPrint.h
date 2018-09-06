/*******************************************************************************
 *
 *     Copyright : 2010 Arnaud Emilien
 *
 *******************************************************************************/

#ifndef _QUAD_FOOT_PRINT_H
#define _QUAD_FOOT_PRINT_H

#include "../../Engine/Mesh.h"
#include "../../Maths/Calcul.h"
#include "../../Utils/Table.h"
#include "Chimney.h"
#include "DoorFacade.h"
#include "Facade.h"
#include "Floor.h"
#include "HouseBlock.h"
#include "HugeSlopeRoof.h"
#include "Quad.h"
#include "Roof.h"
#include "SlopeRoof.h"
#include "SpaceFloor.h"
#include "SquareRoof.h"
#include "StraitSquareRoof.h"
#include "WindowFacade.h"

class QuadFootPrint {
public:
	Quad _quad;
	HouseBlock *_block;
	Table<Floor *> _floors;
	Roof *_roof;
	Mesh _mesh;

public:
	QuadFootPrint(const Quad &quad)
			: _quad(quad) {
	}

	Mesh generate();

}; // class QuadFootPrint

#endif // _QUAD_FOOT_PRINT_H
