/*******************************************************************************
 *
 *     Copyright : 2010 Arnaud Emilien
 *
 *******************************************************************************/

#ifndef _FLOOR_H
#define _FLOOR_H

#include "../../Engine/Mesh.h"
#include "WallE.h"
#include "WindowFacade.h"
#include "SquareRoof.h"
#include "StraitSquareRoof.h"
#include "SlopeRoof.h"
#include "HugeSlopeRoof.h"

class Floor{
public :

	Quad  _quad;
	Quad  _ground;
	Quad  _floor1;
	float _height;
	Mesh _mesh;
	int _floor;

	Floor(Quad base, 
	      float height,
		int floor)	
		: _ground(base)
                , _quad(base)
		, _height(height)
		, _floor(floor)
	{
		_floor1 = Quad(base._points[0] + Vector3F(0, _height, 0),
			       base._points[1] + Vector3F(0, _height, 0),
			       base._points[2] + Vector3F(0, _height, 0),
			       base._points[3] + Vector3F(0, _height, 0));
	}		


	Mesh generate()
	{
		std::cout << "Generate Floor" << std::endl;

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



		Quad doorq(Vector3F(0,0,0),
			   Vector3F(1,0,0),
			   Vector3F(1,2,0),
			   Vector3F(0,2,0));
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

			WindowFacade wfacade(walls[i]);
			_mesh += wfacade.generate();
		}

                /*
		WindowFacade wall1, wall2, wall3, wall4;
		wall1 = WindowFacade(Quad(_ground._points[0], 
				   _ground._points[1], 
				   _floor1._points[1], 
				   _floor1._points[0]));
		
		wall2 = WindowFacade(Quad(_ground._points[1], 
				   _ground._points[2], 
				   _floor1._points[2], 
				   _floor1._points[1]));
		
		wall3 = WindowFacade(Quad(_ground._points[2], 
				   _ground._points[3], 
				   _floor1._points[3], 
				   _floor1._points[2]));
		
		wall4 = WindowFacade(Quad(_ground._points[3], 
				   _ground._points[0], 
				   _floor1._points[0], 
				   _floor1._points[3]));
		
		_mesh += wall1.generate();
		_mesh += wall2.generate();
		_mesh += wall3.generate();
		_mesh += wall4.generate();



                */

                float test = float (rand()%1000) / 1000.0;
                test = test*test*test;
                if ((test < 0.1 || _floor < 4) && _floor > 1)
                {
			float test2 = float (rand()%1000) / 1000.0;
			if (test < 0.1)
			{
				SquareRoof roof(floor);
				_mesh += roof.generate();
			}
			else if (test < 0.4)
			{
				StraitSquareRoof roof(floor);
				_mesh += roof.generate();
			}
			else if (test < 0.6)
			{
				HugeSlopeRoof roof(floor);
				_mesh += roof.generate();
			}
			else
			{
				SlopeRoof roof(floor);
				_mesh += roof.generate();
			}
                }
                else
                {
		        Floor roof(floor, 3.0, _floor+1);
		        _mesh += roof.generate();
                }

		return _mesh;
	}

}; // class Floor

#endif // _FLOOR_H
