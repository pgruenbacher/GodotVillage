/*******************************************************************************
 *
 *     Copyright : 2010 Arnaud Emilien
 *
 *******************************************************************************/

#ifndef CHURCH_H
#define CHURCH_H

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

class Church {
public:
	Quad _quad;
	HouseBlock *_block;
	Table<Floor *> _floors;
	Roof *_roof;
	Mesh _mesh;

public:
	Church(const Quad &quad)
			: _quad(quad) {
	}

	Mesh generate();

}; // class Church

#endif // _QUAD_FOOT_PRINT_H
