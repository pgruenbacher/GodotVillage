/*******************************************************************************
 *
 *     Copyright : 2010 Arnaud Emilien
 *
 *******************************************************************************/

#ifndef GardenWall_H
#define GardenWall_H

#include "Quad.h"
#include "../../Engine/Mesh.h"
#include "../../Maths/Calcul.h"
#include "../../Utils/Table.h"

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

class GardenWall{
public :
	Quad _quad;
	Mesh _mesh;
        Table<Vector2F> _foot;

	GardenWall(Table<Vector2F> foot)
		: _foot(foot)
	{}

	Mesh generate()
	{
		_mesh.empty();

                std::cout << "Generate GardenWall" << std::endl;

                if (_foot.getSize() == 0)
                        return _mesh;

                Vector2F prev, cur;
                prev = _foot[_foot.getSize()-1];
		for (unsigned int i = 0; i < _foot.getSize(); ++i)
                {
                        cur = _foot[i];
                        Vector3F pos1, pos2;
                        pos1.x = prev.x;
                        pos1.y = heightAt(prev);
                        pos1.z = -prev.y;
                        pos2.x = cur.x;
                        pos2.y = heightAt(cur);
                        pos2.z = -cur.y;
			_mesh += Mesh::cube(pos1, pos2, 0.2);

                        pos1 += Vector3F(0, 0.2, 0);
                        pos2 += Vector3F(0, 0.2, 0);
			_mesh += Mesh::cube(pos1, pos2, 0.2);

                        prev = cur;
                }
			
		return _mesh;
	}


}; // class GardenWall

#endif // GardenWall_H
