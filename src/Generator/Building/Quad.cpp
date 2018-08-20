/*******************************************************************************
 *
 *     Copyright : 2010 Arnaud Emilien
 *
 *******************************************************************************/


#include "Quad.h"
#include "../VillageGenerator.h"





float getDiffHeight(Vector3F p0, Vector3F p1)
{
	Vector2I pos0, pos1;
	pos0.x = p0.x;
	pos0.y = -p0.z;
	pos1.x = p1.x;
	pos1.y = -p1.z;
	float h0, h1;
	h0 = VillageGenerator::_instance->_heightmapLayout->getValueAt(pos0) 
		* VillageGenerator::_instance->_terrain->getMaxElevation();
	h1 = VillageGenerator::_instance->_heightmapLayout->getValueAt(pos1) 
		* VillageGenerator::_instance->_terrain->getMaxElevation();
	return h1 - h0;
}

Table<int> possibleStreetFacade(Quad quad)
{
	// Neighbourhood analysis to find where to put the door
	// FRONT
	Facade::ConstraintType frontConstraint = Facade::analysis(quad.p0(), 
								  quad.p1(), 
								  -1.f * normalize(quad.p03() ) );

	// RIGHT
	Facade::ConstraintType rightConstraint = Facade::analysis(quad.p1(), 
								  quad.p2(), 
								  normalize(quad.p01() ) );
	// BACK
	Facade::ConstraintType backConstraint = Facade::analysis(quad.p2(), 
								 quad.p3(), 
								 normalize(quad.p03() ) );
		
	// LEFT
	Facade::ConstraintType leftConstraint = Facade::analysis(quad.p3(), 
								 quad.p0(), 
								 - 1.f * normalize(quad.p01() ) );
			
		
	std::cout << " Front : " << frontConstraint
		  << " Right : " << rightConstraint
		  << " Back : "  << backConstraint
		  << " Left : "  << leftConstraint
		  << std::endl;
		

	Table<int> streetView;
	if (frontConstraint == Facade::STREET)
	{
		streetView.pushLast(0);
	}
	if (rightConstraint == Facade::STREET)
	{
		streetView.pushLast(1);
	}
	if (backConstraint == Facade::STREET)
	{
		streetView.pushLast(2);
	}
	if (leftConstraint == Facade::STREET)
	{
		streetView.pushLast(3);
	}

	return streetView;		
}

Table<int> possibleFreeFacade(Quad quad)
{		

	// Neighbourhood analysis to find where to put the door
	// FRONT
	Facade::ConstraintType frontConstraint = Facade::analysis(quad.p0(), 
								  quad.p1(), 
								  -1.f * normalize(quad.p03() ) );

	// RIGHT
	Facade::ConstraintType rightConstraint = Facade::analysis(quad.p1(), 
								  quad.p2(), 
								  normalize(quad.p01() ) );
	// BACK
	Facade::ConstraintType backConstraint = Facade::analysis(quad.p2(), 
								 quad.p3(), 
								 normalize(quad.p03() ) );
		
	// LEFT
	Facade::ConstraintType leftConstraint = Facade::analysis(quad.p3(), 
								 quad.p0(), 
								 - 1.f * normalize(quad.p01() ) );
			

	std::cout << " Front : " << frontConstraint
		  << " Right : " << rightConstraint
		  << " Back : "  << backConstraint
		  << " Left : "  << leftConstraint
		  << std::endl;
		
	Table<int> freeView;

	if (frontConstraint == Facade::FREE)
	{
		freeView.pushLast(0);
	}
	if (rightConstraint == Facade::FREE)
	{
		freeView.pushLast(1);
	}
	if (backConstraint == Facade::FREE)
	{
		freeView.pushLast(2);
	}
	if (leftConstraint == Facade::FREE)
	{
		freeView.pushLast(3);
	}
	return freeView;
}	
	

	
float heightAt(Vector3F p0)
{	
	Vector2I pos0;
	pos0.x = p0.x;
	pos0.y = -p0.z;
	float h0;
	h0 = VillageGenerator::_instance->_heightmapLayout->getValueAt(pos0) 
		* VillageGenerator::_instance->_terrain->getMaxElevation();
	return h0;
}



int findMinSlope(Quad quad, Table<int> points)
{
	int min = 0;
	float minf = getDiffHeight(quad.getPoints(points[0])[0], quad.getPoints(points[0])[1]);
	for (unsigned int i = 1; i < points.getSize(); ++i)
	{
		if (minf >= fabs(getDiffHeight(quad.getPoints(points[i])[0], quad.getPoints(points[i])[1])))
		{
			min = i;
			minf = fabs(getDiffHeight(quad.getPoints(points[i])[0], quad.getPoints(points[i])[1]));
		}
	}
	return min;
}





std::ostream& operator<<(std::ostream& o, const FacadeSubdiv& facade)
{
	o << facade.facadeNumber
	  << ","
	  << facade.subdivNumber
	  << " : (" 
	  << facade.p0
	  << ")->("
	  << facade.p1
	  << ") , heightMin :"
	  << facade.heightMin
	  << " , width :"
	  << facade.width;

	return o;
}



Table<FacadeSubdiv> subdivise(Vector3F p0, Vector3F p1, int facadeNumber, float width)
{
	Table<FacadeSubdiv> result;
	float length = norm(p1-p0);
	int nb = int(length / width);
	float offset = (length-float(nb)*width)/2.0;

	// FIRST
	FacadeSubdiv first;
	first.facadeNumber = facadeNumber;
	first.subdivNumber = 0;
	first.p0 = p0;
	first.p1 = p0 + offset * normalize(p1-p0);
	first.heightMin = 0;
	first.width = offset;

	result.pushLast(first);

	// MIDDLE
	for (unsigned int i = 0; i < nb; ++i)
	{		
		FacadeSubdiv middle;
		middle.facadeNumber = facadeNumber;
		middle.subdivNumber = i+1;
		middle.p0 = p0 + (float(i) * width + offset) * normalize(p1-p0);
		middle.p1 = p0 + (float(i+1) * width + offset) * normalize(p1-p0);
		middle.heightMin = MAX(heightAt(middle.p0), heightAt(middle.p1));
		middle.width = width;
		
		result.pushLast(middle);
	}

	// LAST
	first.facadeNumber = facadeNumber;
	first.subdivNumber = nb + 1;
	first.p0 = p1 - offset * normalize(p1-p0);
	first.p1 = p1;
	first.heightMin = 0;
	first.width = offset;

	result.pushLast(first);

	return result;	
}






Table<FacadeSubdiv> subdivise(Quad quad, float width)
{
	Table<FacadeSubdiv> result;

	// FRONT
	result += subdivise(quad.p0(), quad.p1(), 0, width);

	// RIGHT
	result += subdivise(quad.p1(), quad.p2(), 1, width);

	// BACK
	result += subdivise(quad.p2(), quad.p3(), 2, width);

	// LEFT
	result += subdivise(quad.p3(), quad.p0(), 3, width);

	return result;
}

void display(Table<FacadeSubdiv> subdiv)
{
	for (unsigned int i = 0; i < subdiv.getSize(); ++i)
	{
		std::cout << subdiv[i] << std::endl;
	}	
}


Facade::ConstraintType analysis(FacadeSubdiv facade)
{		
	Vector3F p0 = facade.p0;
	Vector3F p1 = facade.p1;
	float length = norm (p1 - p0);
	float frontView = 5.0;
	Vector3F normal = crossProduct( p1-p0, Vector3F(0,1,0));

	for (unsigned int i = 0; i < int(length); ++i)
	{
		Vector3F pos = p0 * float(int(length) -1 -i)/float(int(length)-1)
			+ p1 * float(i)/float(int(length)-1);
		for (unsigned int j = 0; j < int(frontView); ++j)
		{
			Vector3F current = pos + normal * float(j);
			Vector2I get;
			get.x = current.x;
			get.y = - current.z;
				
			float house = VillageGenerator::_instance->_buildingsLayout->getValueAt(get);
			float roads = VillageGenerator::_instance->_roadsColorLayout->getValueAt(get);

			if (house > 0.0)
				return Facade::NEIGHBOUR;
			if (roads > 0.0)				
				return Facade::STREET;				
		}
	}
	return Facade::FREE;
}




Mesh generateTable(Table<Quad> table)
{
	Mesh result;
	for (unsigned int i = 0; i < table.getSize(); ++i)
	{
		result += table[i].generate();
	}
	return result;
}



bool canContain(Quad quad, Quad to)
{
	float quadWidthT = norm(quad.p01());
	float quadWidthB = norm(quad.p32());
	float toWidthT = norm(to.p01());
	float toWidthB = norm(to.p32());

	float quadHeightR = norm(quad.p03());
	float quadHeightL = norm(quad.p12());
	float toHeightR = norm(to.p03());
	float toHeightL = norm(to.p12());

	if (quadWidthT > toWidthT
	    && quadWidthT > toWidthB
	    && quadWidthB > toWidthT
	    && quadWidthB > toWidthB
	    && quadHeightR > toHeightR
	    && quadHeightR > toHeightL
	    && quadHeightL > toHeightR
	    && quadHeightL > toHeightL)
	{
		return true;
	}
	return false;
}
	
Quad translate(Quad quad, Vector3F dir)
{
	Quad result = quad;
	result.p0() += dir;
	result.p1() += dir;
	result.p2() += dir;
	result.p3() += dir;
	return result;
}

Table<Vector3F> subdivise(Vector3F p0, Vector3F p1, float step)
{
	float length = norm(p1-p0);
	int nb = int(length/step);
	float offset = (length - float(nb)*step);
	
	Vector3F p01 = normalize(p1-p0);
	Table<Vector3F> result;
	for (unsigned int i = 0; i < nb; ++i)
	{
		result.pushLast(p0 + step * (float(i) + 0.5f) * p01);
	}
	return result;
}

bool validWindowPosition(Quad quad, Quad to)
{	
	float maxHeight = MIN(quad.p3().y, quad.p2().y) - 0.2;
	float minHeight = MAX(heightAt(to.p0()), heightAt(to.p1()));

	minHeight = MAX(minHeight, quad.p0().y);
	minHeight = MAX(minHeight, quad.p1().y);

        minHeight +=  0.2;

	if (to.p0().y > minHeight
	    && to.p1().y > minHeight
	    && to.p2().y < maxHeight
	    && to.p3().y < maxHeight)
	{
		float dot1 = dotProduct(to.p0() - quad.p0(), normalize(quad.p01()));
		float dot2 = dotProduct(to.p1() - quad.p0(), normalize(quad.p01()));
		if ( dot1 > 0.1
		     && dot2 < norm(quad.p01()) - 0.1)
		{
			//std::cout << "validWindowPosition Quad : " << quad << "  /  to " << to << std::endl;
			//std::cout << "dot1 " << dot1 << " dot2 " << dot2 << " norm(quad.p01() " << norm(quad.p01()) << std::endl;
			return true;
		}
	}
	return false;
}

	

bool validDoorPosition(Quad quad, Quad to)
{	
	float maxHeight = MIN(quad.p3().y, quad.p2().y) - 0.2;
	float minHeight = MAX(heightAt(to.p0()), heightAt(to.p1()));

	float maxStair = 2.f;
	if (to.p0().y > minHeight + maxStair)
		return false;

	minHeight = MAX(minHeight, quad.p0().y);
	minHeight = MAX(minHeight, quad.p1().y);

        minHeight +=  0.1;

	if (to.p0().y > minHeight
	    && to.p1().y > minHeight
	    && to.p2().y < maxHeight
	    && to.p3().y < maxHeight)
	{
		float dot1 = dotProduct(to.p0() - quad.p0(), normalize(quad.p01()));
		float dot2 = dotProduct(to.p1() - quad.p0(), normalize(quad.p01()));
		if ( dot1 > 0.1
		     && dot2 < norm(quad.p01()) - 0.1)
		{
			//std::cout << "validDoorPosition Quad : " << quad << "  /  to " << to << std::endl;
			//std::cout << "dot1 " << dot1 << " dot2 " << dot2 << " norm(quad.p01() " << norm(quad.p01()) << std::endl;
			return true;
		}
	}
	return false;
}

Quad placeOnQuad(Quad quad, Quad to)
{
	Quad result;
	Vector3F p01 = normalize(quad.p01());
	Vector3F p03 = normalize(quad.p03());

	result.p0() = quad.p0() 
		+ to.p0().x * p01
		+ to.p0().y * p03;

	result.p1() = quad.p0() 
		+ to.p1().x * p01
		+ to.p1().y * p03;

	result.p2() = quad.p0() 
		+ to.p2().x * p01
		+ to.p2().y * p03;

	result.p3() = quad.p0() 
		+ to.p3().x * p01
		+ to.p3().y * p03;

	return result;
}


Vector3F randomMove(Vector3F axis1, Vector3F axis2)
{
	unsigned int neighbour = rand() % 8;     
        Vector3F move;          
        switch (neighbour) {
                case 0 :
                        move = Vector3F(-1,-1,0);
                        break;
                case 1 :
                        move = Vector3F(-1,1,0);
                        break;
                case 2 :
                        move = Vector3F(1,1,0);
                        break;
                case 3 :
                        move = Vector3F(1,-1,0);
                        break;
                case 4 :
                        move = Vector3F(-1,0,0);
                        break;
                case 5 :
                        move = Vector3F(1,0,0);
                        break;
                case 6 :
                        move = Vector3F(0,-1,0);
                        break;
                case 7 :
                        move = Vector3F(0,1,0);
                        break;
        }
        return move.x * axis1 + move.y * axis2;	
}

bool positionWindow(Quad quad, Quad& to)
{
	// CHECK
	if (! canContain(quad, to))
	{
		std::cout << "Can not contain" << std::endl;
		return false;
	}
	

	// PLACE DOOR ON QUAD
	to = placeOnQuad(quad, to);
	
	
	to = translate(to, float(norm(quad.p01()) / 2.0 - norm(to.p01()) / 2.0) * normalize(quad.p01()));
	to = translate(to, float(norm(quad.p03()) / 2.0 - norm(to.p03()) / 2.0) * normalize(quad.p03()));
	
	int nbMaxTest = 1000;
	int nbTest = 0;  
	float moveStep = 0.1;

	while (nbTest < nbMaxTest)
	{			
		if (validWindowPosition(quad,to))
		{
			//std::cout << "Window positioned in " << nbTest << " tests." << std::endl;
			return true;
		}

		
		to = translate(to, moveStep * randomMove(normalize(quad.p01()), normalize(quad.p03())));

		nbTest++;
	}

	
	//std::cout << "Window positionning failed " << std::endl;
	return false;
	
}

bool positionDoor(Quad quad, Quad& to)
{
	// CHECK
	if (! canContain(quad, to))
	{
		std::cout << "Can not contain" << std::endl;
		return false;
	}

	// PLACE DOOR ON QUAD
	to = placeOnQuad(quad, to);
	
	
	to = translate(to, float(norm(quad.p01()) / 2.0 - norm(to.p01()) / 2.0) * normalize(quad.p01()));
	to = translate(to, Vector3F(0, 0.2, 0));
	
	int nbMaxTest = 1000;
	int nbTest = 0;  
	float moveStep = 0.1;

	while (nbTest < nbMaxTest)
	{			
		if (validDoorPosition(quad,to))
		{
			std::cout << "Door positioned in " << nbTest << " tests." << std::endl;
			return true;
		}

		
		to = translate(to, moveStep * randomMove(normalize(quad.p01()), normalize(quad.p03())));

		nbTest++;
	}

	
	//std::cout << "Door positionning failed " << std::endl;
	return false;
	
}

Table<Quad> extract(Quad quad, Quad to)
{
	Table<Quad> result;
   

	Vector3F b0, b1, b2, b3;	
	b0 = quad.p0() + dotProduct(to.p0() - quad.p0(), normalize(quad.p01())) * normalize(quad.p01());
	b1 = quad.p0() + dotProduct(to.p1() - quad.p0(), normalize(quad.p01())) * normalize(quad.p01());
	b2 = quad.p3() + dotProduct(to.p2() - quad.p3(), normalize(quad.p32())) * normalize(quad.p32());
	b3 = quad.p3() + dotProduct(to.p3() - quad.p3(), normalize(quad.p32())) * normalize(quad.p32());
		
	result.pushLast( Quad (quad.p0(), b0, b3, quad.p3()) );
	result.pushLast( Quad (b0, b1, to.p1(), to.p0()) );
	result.pushLast( Quad (to.p3(), to.p2(), b2, b3));
	result.pushLast( Quad (b1, quad.p1(), quad.p2(), b2) );

	return result;
}

std::ostream& operator<<(std::ostream& o, const Quad& quad)
{
	o << quad._points[0]
	  << ", "
	  << quad._points[1]
	  << ", "
	  << quad._points[2]
	  << ", "
	  << quad._points[3];
	return o;
}
