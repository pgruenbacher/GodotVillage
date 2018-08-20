/*******************************************************************************
 *
 *     Copyright : 2010 Arnaud Emilien
 *
 *******************************************************************************/

#ifndef _QUAD_H
#define _QUAD_H

#include "../../Maths/Vector.h"
#include "../../Utils/Table.h"
#include "../../Engine/Mesh.h"
#include "Facade.h"

struct Quad{
	Vector3F _points[4];
	Mesh _mesh;

	Quad(){}

	Quad(Vector3F p0, Vector3F p1, Vector3F p2, Vector3F p3)
	{
		_points[0] = p0;
		_points[1] = p1;
		_points[2] = p2;
		_points[3] = p3; 
	}

	Vector3F& p0()
	{
		return _points[0];
	}
	Vector3F& p1()
	{
		return _points[1];
	}
	Vector3F& p2()
	{
		return _points[2];
	}
	Vector3F& p3()
	{
		return _points[3];
	}


	Vector3F p01()
	{
		return _points[1] - _points[0];
	}

	Vector3F p03()
	{
		return _points[3] - _points[0];
	}

	Vector3F p32()
	{
		return _points[2] - _points[3];
	}

	Vector3F p12()
	{
		return _points[2] - _points[1];
	}

	Vector3F normal()
	{
		return crossProduct(normalize(p01()), normalize(p03()));
	}		

	Quad& rotateToY()
	{
		Vector3F swap = _points[0];
		_points[0]    = _points[1];
		_points[1]    = _points[2];
		_points[2]    = _points[3];
		_points[3]    = swap;
		return *this;
	}
	
	Quad& revertToX()
	{
		Vector3F swap = _points[3];
		_points[3]    = _points[2];
		_points[2]    = _points[1];
		_points[1]    = _points[0];
		_points[0]    = swap;
		return *this;
	}

	float bottomLength()
	{
		return norm (_points[1] - _points[0]);
	}

	float topLength()
	{
		return norm (_points[2] - _points[3]);
	}

	Table<Quad> cutLeft(float width)
	{
		Table<Quad> result;
		Vector3F pp1, pp2;
		Vector3F p01 = _points[1] - _points[0];
		Vector3F p32 = _points[2] - _points[3];

		float topL = topLength();

		pp1 = _points[0] + width/topL * p01;
		pp2 = _points[3] + width/topL * p32;

		result.pushLast(Quad(_points[0], pp1, pp2, _points[3]));
		result.pushLast(Quad(pp1, _points[1], _points[2], pp2));
		
		return result;
	}	

	Table<Quad> subdiviseXwidthForced(float width)
	{
		Table<Quad> result = subdiviseXwidthBased(width);

		/*
		result[0].p0() = result[0].p1() - normalize(result[0].p01()) * width;
		result[0].p3() = result[0].p2() - normalize(result[0].p32()) * width;
		*/

		/*

		result[result.getSize()-1].p1() = result[0].p0() + normalize(result[0].p01()) * width;
		result[result.getSize()-1].p2() = result[0].p3() + normalize(result[0].p32()) * width;
		*/
		
		return result;
		
	}

	Table<Quad> subdiviseXwidthBased(float width)
	{	
		Table<Quad> result;
		Table<Quad> temp;
	
		float topL    = topLength();
		
		int nb = int(topL / width);
		float offset = (topL - float(nb) * width) / 2.f;

		temp = cutLeft(topL - offset);
		Quad right = temp[1];
		Quad toCut = temp[0];

		temp = toCut.cutLeft(offset);
		Quad left = temp[0];
		Quad toSubdivise = temp[1];

		temp = toSubdivise.subdiviseX(nb);

		result.pushLast(left);
		result += temp;
		result.pushLast(right);

		return result;		
	}

	Table<Quad> extrude(float depth, float scale = 1.f)
	{
		Table<Quad> result;
		Vector3F pp0, pp1, pp2, pp3;
		Vector3F n = normal();

		pp0 = _points[0] + n * depth + (1.f-scale) * p01() + (1.f-scale) * p03();
		pp1 = _points[1] + n * depth - (1.f-scale) * p01() + (1.f-scale) * p03();
		pp2 = _points[2] + n * depth - (1.f-scale) * p01() - (1.f-scale) * p03();
		pp3 = _points[3] + n * depth + (1.f-scale) * p01() - (1.f-scale) * p03();

		Quad wallRight  = Quad( p3(), p0(), pp0,  pp3  );
		Quad wallLeft   = Quad( p1(), p2(), pp2,  pp1  );
		Quad wallTop    = Quad( p2(), p3(), pp3,  pp2  );
		Quad wallBottom = Quad( p0(), p1(), pp1,  pp0  );
		Quad wallWindow = Quad( pp0,  pp1,  pp2,  pp3  );

		
		result.pushLast(wallWindow);
		result.pushLast(wallRight);
		result.pushLast(wallLeft);
		result.pushLast(wallTop);
		result.pushLast(wallBottom);

		return result;
	}

	Table<Quad> subdiviseX(unsigned int nb)
	{
		Table<Quad> result;
		
		Vector3F pp0, pp1, pp2, pp3;
		for(unsigned int i = 0; i < nb; ++i)
		{
			pp0 = float(nb-i) / float(nb) * _points[0]
				+ float(i) / float(nb) * _points[1];

			pp1 = float(nb-i-1) / float(nb) * _points[0]
				+ float(i+1) / float(nb) * _points[1];

			pp2 = float(nb-i-1) / float(nb) * _points[3]
				+ float(i+1) / float(nb) * _points[2];

			pp3 = float(nb-i) / float(nb) * _points[3]
				+ float(i) / float(nb) * _points[2];

			result.pushLast(Quad(pp0,pp1,pp2,pp3));
		}
		return result;		
	}

	Mesh generate()
	{
		_mesh = Mesh::plane(_points[0], _points[1], 
				    _points[2], _points[3]);
		return _mesh;
	}			
			

	Table<Quad> cylinder(unsigned int nbDiv)
	{
		Vector3F center0 = p0() + 0.5f * p03();
		Vector3F center1 = p1() + 0.5f * p12();
		
		float bottomRadius = norm ( p03() ) / 2.0;
		float topRadius    = norm ( p12() ) / 2.0;

		Table<Quad> result;
		
		Table<Vector3F> points;
		for (unsigned int i = 0; i < nbDiv+1; ++i)
		{
			Vector3F current = center0 
				+ bottomRadius * cosf(float(i) * M_PI / nbDiv) * normalize(p0() - center0)
				+ bottomRadius * sinf(float(i) * M_PI / nbDiv) * normal();
			points.pushLast(current);
		}
		for (unsigned int i = 0; i < nbDiv+1; ++i)
		{
			Vector3F current = center1 
				+ topRadius * cosf(float(i) * M_PI / nbDiv) * normalize(p1() - center1)
				+ topRadius * sinf(float(i) * M_PI / nbDiv) * normal();
			points.pushLast(current);
		}

		for (unsigned int i = 0; i < nbDiv; ++i)
		{
			result.pushLast( Quad(points[i], points[i+nbDiv+1], points[i+nbDiv+1+1], points[i+1]));
		}

		return result;		
	}

	Table<Vector3F> getPoints(int nb)
	{
		Table<Vector3F> points;
		switch(nb)
		{
		case 1:
			points.pushLast(p0());
			points.pushLast(p1());
			break;
		case 0 :
			points.pushLast(p1());
			points.pushLast(p2());
			break;
		case 3 :
			points.pushLast(p2());
			points.pushLast(p3());
			break;
		case 2 :
			points.pushLast(p3());
			points.pushLast(p0());
			break;
		}
		return points;
	}



}; // class Quad





struct FacadeSubdiv{
	int facadeNumber;
	int subdivNumber;
	Vector3F p0, p1;
	float heightMin;
	float width;
	bool possible;
};

std::ostream& operator<<(std::ostream& o, const Quad& quad);

float getDiffHeight(Vector3F p0, Vector3F p1);
Table<int> possibleStreetFacade(Quad quad);
Table<int> possibleFreeFacade(Quad quad);
float heightAt(Vector3F p0);
int findMinSlope(Quad quad, Table<int> points);
std::ostream& operator<<(std::ostream& o, const FacadeSubdiv& facade);
Table<FacadeSubdiv> subdivise(Vector3F p0, Vector3F p1, int facadeNumber, float width);
Table<FacadeSubdiv> subdivise(Quad quad, float width);
void display(Table<FacadeSubdiv> subdiv);
Facade::ConstraintType analysis(FacadeSubdiv facade);
Mesh generateTable(Table<Quad> table);


bool canContain(Quad quad, Quad to);
Quad translate(Quad quad, Vector3F dir);
Table<Vector3F> subdivise(Vector3F p0, Vector3F p1, float step);
bool validDoorPosition(Quad quad, Quad to);
Quad placeOnQuad(Quad quad, Quad to);
Vector3F randomMove();
bool positionDoor(Quad quad, Quad& to);
bool positionWindow(Quad quad, Quad& to);
Vector3F randomMove(Vector3F axis1=Vector3F(1,0,0), Vector3F axis2=Vector3F(0,1,0));
Table<Quad> extract(Quad quad, Quad to);


#endif // _QUAD_H
