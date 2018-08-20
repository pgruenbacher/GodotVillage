/*******************************************************************************
 *
 *     Copyright : 2010 Arnaud Emilien
 *
 *******************************************************************************/


#include "../VillageGenerator.h"
#include "Tile.h"

	Mesh Tile::generate()
	{
		_mesh.empty();
		
		Quad quad = _quad;
		quad.p0() += Vector3F(0, -0.02, 0);
		quad.p1() += Vector3F(0, -0.01, 0);
		quad.p2() += Vector3F(0, +0.00, 0);
		quad.p3() += Vector3F(0, -0.02, 0);

		if (_type == FLAT)
		{
			Table<Quad> quads = quad.extrude(0.05);
			for (unsigned int i = 0; i < quads.getSize(); ++i)
			{
				_mesh += quads[i].generate();
			}
			return _mesh;
		}
		else if (_type == CHAOS)
		{
			quad.p0() += Vector3F(0, 1.0, 0) * float(rand() % 1000) / 20000.f;
			quad.p1() += Vector3F(0, 1.0, 0) * float(rand() % 1000) / 20000.f;
			quad.p2() += Vector3F(0, 1.0, 0) * float(rand() % 1000) / 20000.f;
			quad.p3() += Vector3F(0, 1.0, 0) * float(rand() % 1000) / 20000.f;

			Table<Quad> quads = quad.extrude(0.05);
			for (unsigned int i = 0; i < quads.getSize(); ++i)
			{
				_mesh += quads[i].generate();
			}
			return _mesh;
			
		}

		/*
		Table<Quad> quads = quad.cylinder(5);
		for (unsigned int i = 0; i < quads.getSize(); ++i)
		{
			_mesh += quads[i].generate();
		}
		*/

		int nbDiv = 5;


		Vector3F center0 = quad.p0() + 0.5f * quad.p03();
		Vector3F center1 = quad.p1() + 0.5f * quad.p12();
		
		float bottomRadius = norm ( quad.p03() ) / 2.0;
		float topRadius    = norm ( quad.p12() ) / 2.0;

		float bottomRadius2 = norm ( quad.p03() ) / 2.0 - 0.05;
		float topRadius2    = norm ( quad.p12() ) / 2.0 - 0.05;

		Table<Quad> result;
		
		Table<Vector3F> points;
		for (unsigned int i = 0; i < nbDiv+1; ++i)
		{
			Vector3F current = center0 
				+ bottomRadius * cosf(float(i) * M_PI / nbDiv) * normalize(quad.p0() - center0)
				+ bottomRadius * sinf(float(i) * M_PI / nbDiv) * quad.normal();
			points.pushLast(current);
		}
		for (unsigned int i = 0; i < nbDiv+1; ++i)
		{
			Vector3F current = center1 
				+ topRadius * cosf(float(i) * M_PI / nbDiv) * normalize(quad.p1() - center1)
				+ topRadius * sinf(float(i) * M_PI / nbDiv) * quad.normal();
			points.pushLast(current);
		}

		for (unsigned int i = 0; i < nbDiv; ++i)
		{
			result.pushLast( Quad(points[i], points[i+nbDiv+1], points[i+nbDiv+1+1], points[i+1]));
		}



		Table<Vector3F> points2;
		for (unsigned int i = 0; i < nbDiv+1; ++i)
		{
			Vector3F current = center0 
				+ bottomRadius2 * cosf(float(i) * M_PI / nbDiv) * normalize(quad.p0() - center0)
				+ bottomRadius2 * sinf(float(i) * M_PI / nbDiv) * quad.normal();
			points2.pushLast(current);
		}
		for (unsigned int i = 0; i < nbDiv+1; ++i)
		{
			Vector3F current = center1 
				+ topRadius2 * cosf(float(i) * M_PI / nbDiv) * normalize(quad.p1() - center1)
				+ topRadius2 * sinf(float(i) * M_PI / nbDiv) * quad.normal();
			points2.pushLast(current);
		}


		for (unsigned int i = 0; i < nbDiv; ++i)
		{
			result.pushLast( Quad(points[i], points[i+1], points2[i+1],  points2[i]));
		}

		for (unsigned int i = 0; i < nbDiv; ++i)
		{
			result.pushLast( Quad(points[i+nbDiv+1+1], points[i+nbDiv+1], points2[i+nbDiv+1],  points2[i+nbDiv+1+1]));
		}

		for (unsigned int i = 0; i < nbDiv; ++i)
		{
			result.pushLast( Quad(points2[i+1], points2[i],  points2[i+nbDiv+1], points2[i+nbDiv+1+1]));
		}
		



		for (unsigned int i = 0; i < result.getSize(); ++i)
		{
			_mesh += result[i].generate();
		}

                _mesh.prepareIllumination();
		VillageGenerator::_instance->_tiles.pushLast(_mesh);
		return Mesh();
	}
