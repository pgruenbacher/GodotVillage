/*******************************************************************************
 *
 *     Copyright : 2010 Arnaud Emilien
 *
 *******************************************************************************/

#include "QuadFootPrint.h"
#include "Cave.h"









Mesh QuadFootPrint::generate()
{
	_mesh.empty();

	// Find the min and the max height for the current quad	
	float minY = _quad._points[0].y;
	minY = MIN(minY, _quad._points[1].y);
	minY = MIN(minY, _quad._points[2].y);
	minY = MIN(minY, _quad._points[3].y);

	float maxY = _quad._points[0].y;
	maxY = MAX(maxY, _quad._points[1].y);
	maxY = MAX(maxY, _quad._points[2].y);
	maxY = MAX(maxY, _quad._points[3].y);


	Quad ground = _quad;
	ground.p0().y = minY;
	ground.p1().y = minY;
	ground.p2().y = minY;
	ground.p3().y = minY;

	float height;
	float rd = float((rand() % 1000)+1) / 1000.0;
	height = float ( rd*rd* 3.0);

	Cave cave(ground, height);
	_mesh += cave.generate();

	/*
	// Subdivise the facades into small possibles positions for the doors
	float doorWidth = 2.0;

	Table<FacadeSubdiv> subdiv = subdivise(ground, doorWidth);
	std::cout << "--- Subdiv ---" << std::endl;
	display(subdiv);

	// Keep only the facades that respect the cave height constraint
	float caveGround = minY;
	float maxCaveHeight = 2.0;

	Table<FacadeSubdiv> possibleDoorPlaces;
	for (unsigned int i = 0; i < subdiv.getSize(); ++i)
	{
		if ((subdiv[i].heightMin  < caveGround + maxCaveHeight)
			&& subdiv[i].width >= doorWidth )
		{
			possibleDoorPlaces.pushLast(subdiv[i]);
		}
	}
	std::cout << "--- Possible Door Places ---" << std::endl;
	display(possibleDoorPlaces);

	// Keep only the faces that are not constraint by the neighbourhood
	Table<FacadeSubdiv> neighbourhoodFree;
	for (unsigned int i = 0; i < possibleDoorPlaces.getSize(); ++i)
	{
		if (analysis(possibleDoorPlaces[i]) != Facade::NEIGHBOUR)
		{
			neighbourhoodFree.pushLast(possibleDoorPlaces[i]);
		}
	}
	std::cout << "--- Neighbourhood free ---" << std::endl;
	display(neighbourhoodFree);

	// Keep the street views
	Table<FacadeSubdiv> streetViews;
	for (unsigned int i = 0; i < neighbourhoodFree.getSize(); ++i)
	{
		if (analysis(possibleDoorPlaces[i]) == Facade::STREET)
		{
			streetViews.pushLast(possibleDoorPlaces[i]);
		}
	}
	std::cout << "--- Street views ---" << std::endl;
	display(streetViews);
	

	// Choose te door facade
	// On the street if possible
	if (streetViews.getSize())
	{
		*/
	
	
	

	return _mesh;
}


/*
Mesh QuadFootPrint::generate()
{
	_mesh.empty();

	// Find the min and the max height for the current quad	
	float minY = _quad._points[0].y;
	minY = MIN(minY, _quad._points[1].y);
	minY = MIN(minY, _quad._points[2].y);
	minY = MIN(minY, _quad._points[3].y);

	float maxY = _quad._points[0].y;
	maxY = MAX(maxY, _quad._points[1].y);
	maxY = MAX(maxY, _quad._points[2].y);
	maxY = MAX(maxY, _quad._points[3].y);


	// Create the base of the House
	// At the min elevation
	Vector3F ground_p0 = _quad._points[0];
	Vector3F ground_p1 = _quad._points[1];
	Vector3F ground_p2 = _quad._points[2];
	Vector3F ground_p3 = _quad._points[3];

	ground_p0.y = minY;
	ground_p1.y = minY;
	ground_p2.y = minY;
	ground_p3.y = minY;



	Table<int> streetView = possibleStreetFacade(_quad);
	Table<int> freeView   = possibleFreeFacade(_quad);

	int door;
	// Random select the door facade from stree views, if any
	if (streetView.getSize() > 0)
	{
		door = findMinSlope(_quad, streetView);
	}
	// Random select the door facade from free views, if any
	else if (freeView.getSize() > 0)
	{
		door = findMinSlope(_quad, freeView);
	}
	// No door!
	else
	{
		door = -1;
	}
		

		


	// Create the ground floor
	// At the max elevation + height
	float groundFloorHeight = 3.f;

	Vector3F floor_p0 = _quad._points[0];
	Vector3F floor_p1 = _quad._points[1];
	Vector3F floor_p2 = _quad._points[2];
	Vector3F floor_p3 = _quad._points[3];

	floor_p0.y = minY + groundFloorHeight;
	floor_p1.y = minY + groundFloorHeight;
	floor_p2.y = minY + groundFloorHeight;
	floor_p3.y = minY + groundFloorHeight;

	// Generate ground floor
	Quad ground(ground_p0, ground_p1, ground_p2, ground_p3);
	Quad floor(floor_p0, floor_p1, floor_p2, floor_p3);


	if (door >= 0)
	{			
		float doorFacadeHeight = fabs(getDiffHeight(_quad.getPoints(door)[0], _quad.getPoints(door)[1]));

		float maxDoor = MAX(heightAt(_quad.getPoints(door)[0]), heightAt(_quad.getPoints(door)[1]));
		ground.p0().y = maxDoor;
		ground.p1().y = maxDoor;
		ground.p2().y = maxDoor;
		ground.p3().y = maxDoor;
		
		Quad cave0 = ground;
		cave0.p0() -= Vector3F(0, groundFloorHeight, 0);
		cave0.p1() -= Vector3F(0, groundFloorHeight, 0);
		cave0.p2() -= Vector3F(0, groundFloorHeight, 0);
		cave0.p3() -= Vector3F(0, groundFloorHeight, 0);
		
		floor.p0().y = ground.p0().y + groundFloorHeight;
		floor.p1().y = ground.p1().y + groundFloorHeight;
		floor.p2().y = ground.p2().y + groundFloorHeight;
		floor.p3().y = ground.p3().y + groundFloorHeight;
		
		
		Floor* floor1 = new Floor(cave0 ,groundFloorHeight);
		_mesh += floor1->generate();
		
		_block = new HouseBlock(ground, floor, door);
		_mesh += _block->generate();

	}
	else
	{
		Floor* floor1 = new Floor(ground, groundFloorHeight);
		_mesh += floor1->generate();
	}
		
		
	// Generate floors
	unsigned int nbFloors = 2;
	Quad floorBase = floor;
	for (unsigned int i = 0; i < nbFloors; ++i)
	{
		float floorHeight = 3.f;
		Floor* floor1 = new Floor(floorBase, floorHeight);
		_mesh += floor1->generate();
		_floors.pushLast(floor1);

		// Update next base
		floorBase._points[0] += Vector3F(0, floorHeight, 0);
		floorBase._points[1] += Vector3F(0, floorHeight, 0);
		floorBase._points[2] += Vector3F(0, floorHeight, 0);
		floorBase._points[3] += Vector3F(0, floorHeight, 0);
	}


	// Generate roof
	float roofHeight = 0.5f;

	float random = float(rand() % 1000) / 1000.f;
	if (random < 0.2)
	{			
		SquareRoof roof(floorBase);
		_mesh += roof.generate();
	}
	else if (random < 0.4)
	{
		SlopeRoof roof(floorBase);
		_mesh += roof.generate();
	} 
	else if (random < 0.6)
	{
		HugeSlopeRoof roof(floorBase);
		_mesh += roof.generate();
	}
	else 
	{
		StraitSquareRoof roof(floorBase);
		_mesh += roof.generate();
	}

	return _mesh;		
}
*/

