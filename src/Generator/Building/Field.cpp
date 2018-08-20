/*******************************************************************************
 *
 *     Copyright : 2010 Arnaud Emilien
 *
 *******************************************************************************/

#include "Field.h"
#include "../VillageGenerator.h"
#include "../../Maths/Calcul.h"

Mesh Field::generate()
	{
                std::cout << "Generate Field " << std::endl;
                std::cout << "_foot.getSize() " << _foot.getSize() << std::endl;

		_mesh.empty();
		_vines.empty();



                Vector2F dir = normalize(_foot[0] - _foot[1]);

                if (dir.x == 0.0 && dir.y == 0.0)
                        dir.y = 1.0;

                Vector2F normal;
                normal.x = -dir.y;      
                normal.y = dir.x;

                float maxX, maxY, minX, minY;
                maxX = dotProduct(_foot[1] - _foot[0], dir);
                minX = dotProduct(_foot[1] - _foot[0], dir);
                maxY = dotProduct(_foot[1] - _foot[0], normal);
                minY = dotProduct(_foot[1] - _foot[0], normal);

                for (unsigned int i = 1; i < _foot.getSize(); ++i)
                {
                        maxX = MAX(maxX, dotProduct(_foot[i] - _foot[0], dir));
                        minX = MIN(minX, dotProduct(_foot[i] - _foot[0], dir));
                        maxY = MAX(maxY, dotProduct(_foot[i] - _foot[0], normal));
                        minY = MIN(minY, dotProduct(_foot[i] - _foot[0], normal));
                }

                for (float x = minX + 0.5; x < maxX - 0.5; x += 10.0)
                {       
                        for (float y = minY + 0.5; y < maxY - 0.5; y += 3.0)
                        {
                                Vector2F curr = _foot[0] + x * dir + y * normal;
                                //std::cout << "curr " << curr << std::endl;
                                Vector3F pos;
                                pos.x = curr.x;
                                pos.y = 0.0;
                                pos.z = - curr.y;
                                pos.y = heightAt(curr);
                                //std::cout << "pos " << pos << std::endl;
                                if (distanceToAPolygon(curr, _foot) < 0.0)
                                {
                                        _mesh += Mesh::cube(pos, pos + Vector3F(0.01, 1.5f, 0), 1.4);
                                }
                        }
                }
		
		return _mesh;		
		
	}
