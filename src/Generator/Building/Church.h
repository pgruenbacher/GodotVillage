/*******************************************************************************
 *
 *     Copyright : 2010 Arnaud Emilien
 *
 *******************************************************************************/

#ifndef CHURCH_H
#define CHURCH_H

#include "Facade.h"
#include "Quad.h"
#include "HouseBlock.h"
#include "Roof.h"
#include "Floor.h"
#include "SpaceFloor.h"
#include "StraitSquareRoof.h"
#include "SquareRoof.h"
#include "SlopeRoof.h"
#include "HugeSlopeRoof.h"
#include "WindowFacade.h"
#include "DoorFacade.h"
#include "Chimney.h"
#include "../../Maths/Calcul.h"
#include "../../Utils/Table.h"
#include "../../Engine/Mesh.h"

class Church{
public :
	Quad _quad;
	HouseBlock*   _block;
	Table<Floor*> _floors;
	Roof* _roof;
	Mesh _mesh;

public :
	Church(const Quad& quad)
		: _quad(quad)
	{
	}	

	Mesh generate();
	

}; // class Church

#endif // _QUAD_FOOT_PRINT_H
