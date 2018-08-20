/*******************************************************************************
 *
 *     Copyright : 2010 Arnaud Emilien
 *
 *******************************************************************************/

#ifndef _ChurchHouse_H
#define _ChurchHouse_H

#include "../../Engine/Mesh.h"
#include "WallE.h"
#include "WindowFacade.h"
#include "SquareRoof.h"
#include "SlopeRoof.h"
#include "HugeSlopeRoof.h"
#include "StraitSquareRoof.h"

class ChurchHouse{
public :

	Quad  _quad;
	Quad  _ground;
	Quad  _floor1;
	float _height;
	Mesh _mesh;

	ChurchHouse(Quad base, 
	      float height)	
		: _ground(base)
                , _quad(base)
		, _height(height)
	{
		_floor1 = Quad(base._points[0] + Vector3F(0, _height, 0),
			       base._points[1] + Vector3F(0, _height, 0),
			       base._points[2] + Vector3F(0, _height, 0),
			       base._points[3] + Vector3F(0, _height, 0));
	}		


	Mesh generate()
	{
		std::cout << "Generate ChurchHouse" << std::endl;

		_mesh.empty();	


		Table<Quad> walls;
		Quad floor;


		// WALLS & FLOOR
		Table<Quad> ext = _quad.extrude(_height);
		
			
		walls.pushLast(ext[4]);
		walls.pushLast(ext[3]);
		walls.pushLast(ext[1]);
		walls.pushLast(ext[2]);
		floor = ext[0];



		Quad doorq(Vector3F(0,0,0),
			   Vector3F(2,0,0),
			   Vector3F(2,4,0),
			   Vector3F(0,4,0));
		Table<Quad> doorquad;

		int doorPos = -1;
		for (unsigned int i = 0; i < walls.getSize(); ++i)
		{
			if (positionDoor(walls[i], doorq))
			{
				//doorquad = door.extrude(5.0);
				Door door(doorq);
				_mesh += door.generate();


				Table<Quad> doorWall = extract(walls[i], doorq);
				walls.pushLast(doorWall[0]); 
				walls.pushLast(doorWall[3]);
				_mesh += doorWall[1].generate();
				_mesh += doorWall[2].generate();
				doorPos = i;
				exit;
			}
		}

		for (unsigned int i = 0; i < walls.getSize(); ++i)
		{
			if (i == doorPos)
				continue;

			WindowFacade wfacade(walls[i], WindowFacade::CHURCH);
			_mesh += wfacade.generate();
		}


		StraitSquareRoof roof(floor);
		_mesh += roof.generate();


		return _mesh;
	}

}; // class ChurchHouse

#endif // _ChurchHouse_H
