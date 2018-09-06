/*******************************************************************************
 *
 *     Copyright : 2010 Arnaud Emilien
 *
 *******************************************************************************/

#ifndef _HOUSE_BLOCK_H
#define _HOUSE_BLOCK_H

#include "../../Engine/Mesh.h"
#include "DoorFacade.h"
#include "WallE.h"
#include "WindowFacade.h"
#include <iostream>

class HouseBlock {
public:
	Quad _ground;
	Quad _floor1;
	Mesh _mesh;
	int _door;

public:
	HouseBlock(Quad ground, Quad floor1, int door = -1)
			: _ground(ground), _floor1(floor1), _door(door) {
		generate();
	}

	Mesh generate() {
		_mesh.empty();

		Quad q0, q1, q2, q3;
		q0 = Quad(_ground._points[0],
				_ground._points[1],
				_floor1._points[1],
				_floor1._points[0]);

		q1 = Quad(_ground._points[1],
				_ground._points[2],
				_floor1._points[2],
				_floor1._points[1]);

		q2 = Quad(_ground._points[2],
				_ground._points[3],
				_floor1._points[3],
				_floor1._points[2]);

		q3 = Quad(_ground._points[3],
				_ground._points[0],
				_floor1._points[0],
				_floor1._points[3]);

		if (_door == 1) {
			DoorFacade doorFacade(q0);
			_mesh += doorFacade.generate();

		} else {
			WindowFacade windowFacade = WindowFacade(q0);
			_mesh += windowFacade.generate();
		}

		if (_door == 0) {
			DoorFacade doorFacade(q1);
			_mesh += doorFacade.generate();

		} else {
			WindowFacade windowFacade = WindowFacade(q1);
			_mesh += windowFacade.generate();
		}

		if (_door == 3) {
			DoorFacade doorFacade(q2);
			_mesh += doorFacade.generate();

		} else {
			WindowFacade windowFacade = WindowFacade(q2);
			_mesh += windowFacade.generate();
		}

		if (_door == 2) {
			DoorFacade doorFacade(q3);
			_mesh += doorFacade.generate();

		} else {
			WindowFacade windowFacade = WindowFacade(q3);
			_mesh += windowFacade.generate();
		}

		return _mesh;
	}

}; // class HouseBlock

#endif // _HOUSE_BLOCK_H
