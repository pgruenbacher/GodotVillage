#ifndef FACADE_H
#define FACADE_H

#include "../../Maths/Vector.h"

class Facade{

public :
        enum ConstraintType{ FREE
                           , STREET
			   , NEIGHBOUR};

	ConstraintType _constraint;   



	static ConstraintType analysis(Vector3F p0, Vector3F p1, Vector3F normal);
        

}; // class Facade

#endif // FACADE_H
