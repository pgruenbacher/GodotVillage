/*******************************************************************************
 *
 *     Copyright : 2010 Arnaud Emilien
 *
 *******************************************************************************/

#ifndef TRIANGG_H
#define TRIANGG_H

class Triangg{
public :
	Vector3F _points[3];
	Mesh _mesh;

	Triangg(){}

	Triangg(Vector3F p0, Vector3F p1, Vector3F p2)
	{
		_points[0] = p0;
		_points[1] = p1;
		_points[2] = p2; 
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
	Vector3F p01()
	{
		return _points[1] - _points[0];
	}

	Vector3F p02()
	{
		return _points[2] - _points[0];
	}
	Vector3F normal()
	{
		return crossProduct(normalize(p01()), normalize(p02()));
	}


	Mesh generate()
	{
		_mesh.empty();

		_mesh.getVerticesArray().insert(_points[0]);
		_mesh.getVerticesArray().insert(_points[1]);
		_mesh.getVerticesArray().insert(_points[2]);

		_mesh.getTrianglesArray().insert(Vector3I(0,1,2));

		return _mesh;
	}
}; // class Triangg

#endif // TRIANGG_H
