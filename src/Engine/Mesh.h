/*******************************************************************************
 *
 *     Copyright : 2010 Arnaud Emilien
 *
 *******************************************************************************/

#ifndef _Mesh_H
#define _Mesh_H

#include "../Maths/Vector.h"
#include "../Utils/Table.h"
#include <iostream>
#include <string>

/**
        Vertices structure
*/
typedef Vector3F Vertex;

/**
        Triangles structure
*/
typedef Vector3I Triangle;

/**
        Neighbour
*/
typedef Vector3I Neighbour;

/**
        Texture Coordinates structure
*/
typedef Vector2F TexCoord;

/**
        Normal structure
*/
typedef Vector3F Normal;

/**
    Meshes
*/
class Mesh {
protected:
	Table<Vertex> _verticesArray;
	Table<TexCoord> _texCoordsArray;
	Table<Triangle> _trianglesArray;

	// Normals
	bool _normals;
	Table<Normal> _verticesNormalsArray;
	Table<Normal> _trianglesNormalsArray;
	bool _links;
	typedef Table<unsigned int> Links;
	Table<Links> _linkedTriangles;
	bool _neighbour;
	Table<Neighbour> _trianglesNeighbour;

	void _computeCorrespondences();
	void _computeNormals();
	void _computeTrianglesNormals();
	void _computeVerticesNormals();
	void _computeFaceNeighbour();

public:
	Table<Vertex> &getVerticesArray() {
		return _verticesArray;
	}
	Table<Triangle> &getTrianglesArray() {
		return _trianglesArray;
	}
	Table<TexCoord> &getTexCoordsArray() {
		return _texCoordsArray;
	}
	Table<Links> &getLinkedTriangles() {
		return _linkedTriangles;
	}
	Table<Normal> &getVerticesNormalsArray() {
		return _verticesNormalsArray;
	}
	Table<Normal> &getTrianglesNormalsArray() {
		return _trianglesNormalsArray;
	}

	Mesh();

	Mesh(const Mesh &mesh);

	~Mesh();

	static Mesh *plane(float width = 1.0f,
			float depth = 1.0f,
			unsigned int widthDivisions = 1,
			unsigned int depthDivisions = 1,
			unsigned int xTextureRepeat = 1,
			unsigned int yTextureRepeat = 1);

	static Mesh sphere(Vector3F p0,
			float radius,
			unsigned int nU = 7, unsigned int nV = 7);

	static Mesh plane(Vector3F p0, Vector3F p1,
			Vector3F p2, Vector3F p3);

	static Mesh cube(Vector3F p0, Vector3F p1,
			Vector3F p2, Vector3F p3,
			float height);

	static Mesh cube(Vector3F p0, Vector3F p1,
			float edge);

	static Mesh plane(Vector3F p0, Vector3F p1,
			float edge, float face);

	Mesh &operator+=(const Mesh &mesh);
	void empty();

	void exportToObj(const std::string &fileName);

	void prepareIllumination();
	void updateNeighbour();

}; //class Mesh

#endif
