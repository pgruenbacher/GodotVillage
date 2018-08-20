/*******************************************************************************
 *
 *     Copyright : 2010 Arnaud Emilien
 *
 *******************************************************************************/

#include "Facade.h"

#include "../VillageGenerator.h"

Facade::ConstraintType Facade::analysis(Vector3F p0, Vector3F p1, Vector3F normal)
	{
		float length = norm (p1 - p0);
		for (unsigned int i = 0; i < int(length); ++i)
		{
			Vector3F pos = p0 * float(int(length) -1 -i)/float(int(length)-1)
				+ p1 * float(i)/float(int(length)-1);
			for (unsigned int j = 0; j < 5; ++j)
			{
				Vector3F current = pos + normal * float(j);
				Vector2I get;
				get.x = current.x;
				get.y = - current.z;
				
				float house = VillageGenerator::_instance->_buildingsLayout->getValueAt(get);
				float roads = VillageGenerator::_instance->_roadsColorLayout->getValueAt(get);

				if (house > 0.0)
					return NEIGHBOUR;
				if (roads > 0.0)				
					return STREET;
				
			}
		}
		return FREE;
	}                        
        
