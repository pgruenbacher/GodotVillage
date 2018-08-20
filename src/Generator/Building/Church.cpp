/*******************************************************************************
 *
 *     Copyright : 2010 Arnaud Emilien
 *
 *******************************************************************************/

#include "Church.h"
#include "ChurchHouse.h"


Mesh Church::generate()
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
	height = (10.0);

        ChurchHouse house(ground, height);
        _mesh += house.generate();

        Vector3F p = _quad.p0() + 0.5f * _quad.p01() + 3.f * normalize(_quad.p03());        
        Vector3F p0, p1, p2, p3;
        p0 = p - 1.0f * normalize(_quad.p01()) - 1.5f * normalize(_quad.p03());
        p1 = p + 1.0f * normalize(_quad.p01()) - 1.5f * normalize(_quad.p03());
        p2 = p + 1.0f * normalize(_quad.p01()) + 1.5f * normalize(_quad.p03());
        p3 = p - 1.0f * normalize(_quad.p01()) + 1.5f * normalize(_quad.p03());
        
        Quad bt(p0, p1, p2, p3);
        Table<Quad> ext = bt.extrude(13);
        _mesh += generateTable(ext);

        Quad tp = ext[0];
        Table<Quad> ext2 = tp.extrude(2);
        _mesh += generateTable(ext2);



        _mesh += Mesh::cube(p + Vector3F(0, 15,0), p + Vector3F(0.01, 18,0), 0.3);
        _mesh += Mesh::cube(p + Vector3F(0, 17,0) - 1.f * normalize(_quad.p01()), p + Vector3F(0.01, 17,0) + 1.f * normalize(_quad.p01()), 0.3);
        
        
        


        return _mesh;

}
