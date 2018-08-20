/*******************************************************************************
 *
 *     Copyright : 2010 Arnaud Emilien
 *
 *******************************************************************************/

#include "Mesh.h"

#include <iostream>
#include <fstream>
#include <sstream>

Mesh::Mesh()
{
}

Mesh::~Mesh()
{
}

Mesh::Mesh(const Mesh& mesh)
    : _verticesArray(mesh._verticesArray)
    , _texCoordsArray(mesh._texCoordsArray)
    , _trianglesArray(mesh._trianglesArray)
    , _verticesNormalsArray(mesh._verticesNormalsArray)
    , _trianglesNormalsArray(mesh._trianglesNormalsArray)
    , _linkedTriangles(mesh._linkedTriangles)
    , _trianglesNeighbour(mesh._trianglesNeighbour)
{
    _normals        = false;
    _links          = false;
    _neighbour      = false;
}

Mesh Mesh::sphere(Vector3F p0,
                  float radius,
                  unsigned int nU, unsigned int nV)
{
    Mesh result;

    Vertex  poleNVertex,poleSVertex;
    Normal  poleNNormale,poleSNormale;
    TexCoord poleNtexCoord,poleStexCoord;

    // POLE NORD : theta = 0
    poleNVertex[0] = 0.f;
    poleNVertex[1] = 0.f;
    poleNVertex[2] = radius;
    poleNNormale[0] = 0.f;
    poleNNormale[1] = 0.f;
    poleNNormale[2] = 1.f;
    poleNtexCoord[1] = 0.f;
    poleNtexCoord[0] = 0.f;

    result._verticesArray.insert(p0 + poleNVertex);
    result._verticesNormalsArray.insert(poleNNormale);

    for(unsigned int u=1; u < nU; ++u)
    {
        for(unsigned int v=0; v < nV; ++v)
        {
            Vertex  vertex;
            Normal  normale;
            TexCoord texCoord;

            float theta = u / float(nU) * M_PI;
            float phi 	= v / float(nV) * M_PI * 2;
            //int index 	= u +(nU+1)*v;


            // position
            vertex[0] = sin(theta) * cos(phi) * radius;
            vertex[1] = sin(theta) * sin(phi) * radius;
            vertex[2] = cos(theta) * radius;

            // normal
            normale[0] = sin(theta) * cos(phi);
            normale[1] = sin(theta) * sin(phi);
            normale[2] = cos(theta);

            // texture coordinates
            texCoord[1] = u / float(nU);
            texCoord[0] = v / float(nV-1);

            result._verticesArray.insert(p0 + vertex);
            result._verticesNormalsArray.insert(normale);
        }
    }

    // POLE SUD : theta = M_PI
    poleSVertex[0] = 0.f;
    poleSVertex[1] = 0.f;
    poleSVertex[2] = -radius;
    poleSNormale[0] = 0.f;
    poleSNormale[1] = 0.f;
    poleSNormale[2] = -1.f;
    poleStexCoord[1] = 1.f;
    poleStexCoord[0] = 1.f;

    result._verticesArray.insert(p0 + poleSVertex);
    result._verticesNormalsArray.insert(poleSNormale);

    // POLYGONES
    int index = 0;
    int vindex;

    // POLE NORD
    for (int v=0; v<nV; ++v)
    {
        Triangle triangle;
        triangle[0] = 0;
        triangle[1] = v+1;
        triangle[2] = 1 + (v+1)%nV ;
        result._trianglesArray.insert(triangle);
    }

    for(int u=1; u<nU-1; ++u)
    {
        for(int v=0; v<nV; ++v)
        {
            Triangle triangle;
            vindex 	= 1 + v + (u-1)*nV;

            triangle[0] = vindex;
            triangle[2] = 1 + (v+1)%nV + (u-1)*nV;
            triangle[1] = 1 + (v+1)%nV + u*nV;

            result._trianglesArray.insert(triangle);

            triangle[0] = vindex;
            triangle[1] = vindex + nV;
            triangle[2] = 1 + (v+1)%nV + u*nV;

            result._trianglesArray.insert(triangle);
        }
    }

    // POLE SUD
    for (int v=0; v<nV; ++v)
    {
        Triangle triangle;
        triangle[0] = v + 1 + (nU-2)*nV;
        triangle[2] = (v+1)%nV + 1 + (nU-2)*nV;
        triangle[1] = (nU-1)*nV + 1;
        result._trianglesArray.insert(triangle);
    }

    return result;
}

Mesh Mesh::plane(Vector3F p0, Vector3F p1,
                 float edge, float depth)
{
    float height = norm(p1 - p0);
    Vector3F pp0, pp1, pp2, pp3;

    Vector3F normalY, normalX;

    if (dotProduct(Vector3F(0,1,0), normalize(p1 - p0)) != 1.0)
        normalX = normalize(crossProduct(Vector3F(0,1,0), normalize(p1 - p0)));
    else
        normalX = Vector3F(1,0,0);

    normalY = normalize(crossProduct(normalize(p1-p0), normalX));
    pp0 = p0 + depth/2.f * normalX + edge/2.f * normalY;
    pp1 = p0 - depth/2.f * normalX + edge/2.f * normalY;
    pp2 = p0 - depth/2.f * normalX - edge/2.f * normalY;
    pp3 = p0 + depth/2.f * normalX - edge/2.f * normalY;

    return cube(pp3, pp2, pp1, pp0, height);
}

Mesh Mesh::cube(Vector3F p0, Vector3F p1,
                float edge)
{
    float height = norm(p1 - p0);
    Vector3F pp0, pp1, pp2, pp3;

    Vector3F normalY, normalX;

    if (dotProduct(Vector3F(0,1,0), normalize(p1 - p0)) != 1.0)
        normalX = normalize(crossProduct(Vector3F(0,1,0), normalize(p1 - p0)));
    else
        normalX = Vector3F(1,0,0);

    normalY = normalize(crossProduct(normalize(p1-p0), normalX));
    pp0 = p0 + edge/2.f * normalX + edge/2.f * normalY;
    pp1 = p0 - edge/2.f * normalX + edge/2.f * normalY;
    pp2 = p0 - edge/2.f * normalX - edge/2.f * normalY;
    pp3 = p0 + edge/2.f * normalX - edge/2.f * normalY;

    return cube(pp3, pp2, pp1, pp0, height);
}

Mesh Mesh::cube(Vector3F p3, Vector3F p2,
                Vector3F p1, Vector3F p0,
                float height)
{

    Mesh result;

    result += (plane (p3, p2, p1, p0));
    Vector3F normal = normalize(crossProduct( normalize(p1-p0), normalize(p2-p0)));
    result += (plane (p3 + normal * height, p2 + normal * height, p2, p3));
    result += (plane (p2 + normal * height , p1 + normal * height, p1, p2));
    result += (plane (p1 + normal * height, p0 + normal * height, p0 , p1));
    result += (plane (p0 + normal * height, p3 + normal * height, p3, p0));
    result += (plane (p0 + normal * height, p1 + normal * height, p2 + normal * height, p3 + normal * height));

    return result;
}

Mesh Mesh::plane(Vector3F p0, Vector3F p1,
                 Vector3F p2, Vector3F p3)
{
    // Allocation
    Mesh result;

    result._verticesArray.insert(p0);
    result._verticesArray.insert(p1);
    result._verticesArray.insert(p2);
    result._verticesArray.insert(p3);

    result._trianglesArray.insert(Vector3F(0, 1, 2));
    result._trianglesArray.insert(Vector3F(2, 3, 0));

    return result;

}

Mesh* Mesh::plane(float width,
                  float depth,
                  unsigned int widthDivisions,
                  unsigned int depthDivisions,
                  unsigned int xTextureRepeat,
                  unsigned int yTextureRepeat)
{
    // Allocation
    Mesh* result = new Mesh();

    result->_verticesArray.preAllocate(widthDivisions*depthDivisions);
    result->_texCoordsArray.preAllocate(widthDivisions*depthDivisions);
    result->_trianglesArray.preAllocate((widthDivisions-1)*(depthDivisions-1));

    // Insertion of the points
    float stepX = width / float(widthDivisions-1);
    float stepZ = depth / float(depthDivisions-1);
    float dx, dz;
    dz = 0.f;
    for (unsigned int d = 0; d < depthDivisions; ++d)
    {
        dx = 0.f;
        for (unsigned int w = 0; w < widthDivisions; ++w)
        {
            result->_verticesArray.insert(Vector3F(dx, 0.f, -dz));
            result->_texCoordsArray.insert(Vector2F(float(xTextureRepeat)*dx/float(width),
                                                    float(yTextureRepeat)*dz/float(depth)));
            dx +=  stepX;
        }
        dz +=  stepZ;
    }

    // Creation of the triangles
    Vector3UI triangle;
    for (unsigned int d = 0; d < depthDivisions - 1; ++d)
    {
        for (unsigned int w = 0; w < widthDivisions - 1; ++w)
        {
            triangle = Vector3UI(d * widthDivisions + w,
                                 d * widthDivisions + w + 1,
                                 (d+1) * widthDivisions + w);
            result->_trianglesArray.insert(triangle);

            triangle = Vector3UI(d * widthDivisions + w + 1,
                                 (d+1) * widthDivisions + w + 1,
                                 (d+1) * widthDivisions + w);
            result->_trianglesArray.insert(triangle);
        }
    }

    return result;
}


Mesh& Mesh::operator+=(const Mesh& mesh)
{
    // Save the number of vertices
    unsigned int offset = _verticesArray.getSize();

    // Vertices copy
    for(unsigned int i = 0; i < mesh._verticesArray.getSize(); ++i)
    {
        _verticesArray.insert(mesh._verticesArray[i]);
    }

    // Triangles copy (with an offset !)
    for(unsigned int i = 0; i < mesh._trianglesArray.getSize(); ++i)
    {
        _trianglesArray.insert((mesh._trianglesArray[i]) + Vector3I(offset, offset, offset));
    }
    return *this;
}


void Mesh::empty()
{
    _verticesArray.empty();
    _trianglesArray.empty();
    _texCoordsArray.empty();
}



void Mesh::prepareIllumination()
{
    if (_verticesArray.getSize() == 0)
        return;
    _computeCorrespondences();
    _computeNormals();
}

void Mesh::_computeCorrespondences()
{
    if(_links)
    {
        return;
    }
    _linkedTriangles.empty();

    // Get the triangle Array from the Table
    Triangle *currentTriangle = _trianglesArray.getArray();
    // resize the linked triangles array
    _linkedTriangles.resize(_verticesArray.getSize());
    // For each vertex of the triangle we add the triangle id to the triangles correspondence list of the vertex
    unsigned int size = _trianglesArray.getSize();

    for ( unsigned int i = 0; i < size ; ++i )
    {
        (_linkedTriangles[currentTriangle->x]).insert(i);
        (_linkedTriangles[currentTriangle->y]).insert(i);
        (_linkedTriangles[currentTriangle->z]).insert(i);
        ++currentTriangle;

    }
    _links = true;
}

void Mesh::_computeNormals()
{
    if(_normals)
    {
        return;
    }

    _computeTrianglesNormals();
    _computeVerticesNormals();
    _normals = true;
}

void Mesh::updateNeighbour()
{
    _computeFaceNeighbour();
}

void Mesh::_computeTrianglesNormals()
{
    // Predeclaration
    Vector3F p1, p2, p3;
    Vector3F p1p2, p1p3;
    Normal normal;

    // Get the triangle Array from the Table
    Triangle *currentTriangle = _trianglesArray.getArray();
    // Empty the triangles normals array
    _trianglesNormalsArray.empty();
    _trianglesNormalsArray.resize(_trianglesArray.getSize());

    // For each vertex of the triangle we add the triangle id to the triangles correspondence list of the vertex
    for ( unsigned int i = 0; i < _trianglesArray.getSize() ; ++i )
    {
        // get the vertex of the triangle
        p1 = _verticesArray[currentTriangle->x];
        p2 = _verticesArray[currentTriangle->y];
        p3 = _verticesArray[currentTriangle->z];

        p1p2 = p2 - p1;
        p1p3 = p3 - p1;

        // normal computation
        normal = crossProduct(p1p2, p1p3);
        normal = normalize(normal);

        _trianglesNormalsArray[i] = normal;

        // increment the current triangle
        ++currentTriangle;
    }
}


void Mesh::_computeVerticesNormals()
{
    // Empty the normal array first
    _verticesNormalsArray.empty();
    _verticesNormalsArray.resize(_verticesArray.getSize());
    // Compute the normals for each vertex
    Normal normal;
    unsigned int nbVertices = _verticesArray.getSize();
    for(unsigned int i = 0; i < _linkedTriangles.getSize(); ++i)
    {
        normal = Vector3F(0.f, 0.f, 0.f);
        unsigned int size = _linkedTriangles[i].getSize();
        for(unsigned int j = 0; j < size; ++j)
        {
            normal += _trianglesNormalsArray[_linkedTriangles[i][j]];
        }
        normal = normalize(normal);
        _verticesNormalsArray[i] = normal;
    }
}

void Mesh::_computeFaceNeighbour()
{
    if ( _neighbour )
    {
        return;
    }

    // init the neighbour table
    _trianglesNeighbour.resize( _trianglesArray.getSize() );

    // for each triangle
    // we search for neighbour
    for ( unsigned int i = 0 ;
            i < _trianglesNeighbour.getSize() ;
            ++i )
    {
        Triangle currT = _trianglesArray[i];
        _trianglesNeighbour[i] = Neighbour( -1,-1,-1 );
        // for each vertex of the triangle
        // we search the opposing face
        for ( unsigned vert = 0 ; vert < 3; ++vert )
        {
            // triangles linked with the vert+1%3 vertice
            Table<unsigned int> t1 = _linkedTriangles[currT[(vert+1)%3]];
            // triangles linked with the vert+2%3 vertice
            Table<unsigned int> t2 = _linkedTriangles[currT[(vert+2)%3]];
            for ( unsigned int k = 0 ; k < t1.getSize() ; ++k )
            {
                if ( i == t1[k] )
                {
                    continue;
                }
                if ( t2.contains(t1[k] ) )
                {
                    // we found the corresponding neighbour
                    _trianglesNeighbour[i][vert] = t1[k];
                    break;
                }
            }
        }
    }
    _neighbour = true;
}



void Mesh::exportToObj(const std::string& filename)
{

    std::ofstream file(filename.c_str(), ios::out);

    if(file.is_open())
    {
        for (unsigned int i = 0; i < _verticesArray.getSize(); ++i)
        {
            file << "v"
                 << " "
                 << _verticesArray[i].x
                 << " "
                 << _verticesArray[i].y
                 << " "
                 << _verticesArray[i].z
                 << "\n";
        }

        for (unsigned int i = 0; i < _trianglesArray.getSize(); ++i)
        {
            file << "f"
                 << " "
                 << _trianglesArray[i].x + 1
                 << " "
                 << _trianglesArray[i].y + 1
                 << " "
                 << _trianglesArray[i].z + 1
                 << "\n";
        }

        file.close();
    }

}
