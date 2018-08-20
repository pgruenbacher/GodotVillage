/*******************************************************************************
 *
 *     Copyright : 2010 Arnaud Emilien
 *
 *******************************************************************************/

#ifndef Cave_H
#define Cave_H

#include "../../Engine/Mesh.h"
#include "Quad.h"
#include "Door.h"
#include "SlopeRoof.h"
#include "SquareRoof.h"
#include "Floor.h"

class Cave{
public :
	Quad _quad;
	Mesh _mesh;
	float _height;

	Cave (Quad quad, float height)
		: _quad(quad)
		, _height(height)
	{}

	Mesh generate()
	{
		std::cout << "Generate Cave" << std::endl;

		_mesh.empty();

		Table<Quad> walls;
		Quad floor;


		// WALLS & FLOOR
		Table<Quad> ext = _quad.extrude(_height);
		
			
		walls.pushLast(ext[1]);
		walls.pushLast(ext[2]);
		walls.pushLast(ext[3]);
		walls.pushLast(ext[4]);
		floor = ext[0];

		//_mesh += generateTable(walls);
		

		//_mesh += walls[1].generate();
		//_mesh += walls[2].generate();
		//_mesh += walls[3].generate();


		Quad doorq(Vector3F(0,0,0),
			   Vector3F(1,0,0),
			   Vector3F(1,2,0),
			   Vector3F(0,2,0));
		Table<Quad> doorquad;


		int doorPos = -1;

		/*
		Table<int> streets = possibleStreetFacade(_quad);
		for (unsigned int i = 0; i < streets.getSize(); ++i)
		{
			walls.pushLast(ext[streets[i]+1]);
		}
			
		Table<int> free    = possibleFreeFacade(_quad);
		for (unsigned int i = 0; i < free.getSize(); ++i)
		{
			walls.pushLast(ext[free[i]+1]);
		}
		*/	


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

			WindowFacade wfacade(walls[i], WindowFacade::CAVE);
			_mesh += wfacade.generate();
		}

		Floor floorr(floor, 3.0, 1);
		_mesh += floorr.generate();
		
		/*
		// DOORABLE FACADES
		float doorFacadeWidth = 2.f;
		float doorFacadeHeight = 2.f;

		Table<int> streets = possibleStreetFacade(_quad);
		Table<int> free    = possibleFreeFacade(_quad);
		if (streets.getSize() > 0)
		{
			for (unsigned int i = 0; i < streets.getSize(); ++i)
			{
				Table<Vector3F> pts = _quad.getPoints(streets[i]);
				Quad facade(pts[0], 
					    pts[1], 
					    pts[1] + Vector3F(0, doorFacadeHeight, 0), 
					    pts[0] + Vector3F(0, doorFacadeHeight, 0));
				if (
		}
		else
		{
		}
				*/

		// WINDOWS

		// OTHER FACADES
		

		

		return _mesh;
	}

}; // class Cave

#endif // Cave_H
