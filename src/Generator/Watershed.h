/*******************************************************************************
 *
 *     Copyright : 2010 Arnaud Emilien
 *
 *******************************************************************************/

#ifndef WATER_SHED
#define WATER_SHED


#include "../Maths/Vector.h"
#include "../Image/Image.h"
#include "../Image/Color.h"
#include "../Utils/Table.h"
#include "../Utils/LinkedList.h"
#include "Road.h"
#include "Building.h"
#include <queue>

#include <functional>

class Watershed{
private :
	struct Source{
		LinkedList<Building>::LinkedListIterator building;
		int id;
		Vector2F pos;
		Vector2F normal;
		Vector2F tangent;
		float max;
		Vector2F origin;
		float distToOrigin;

	Source() : building(NULL) {}

		bool operator>(const Watershed::Source& s1) const
		{
			
			return distToOrigin < s1.distToOrigin;
		}	
		bool operator<(const Watershed::Source& s1) const
		{
			
			return distToOrigin > s1.distToOrigin;
		}	
	};
		
		
	struct Comparator : public std::binary_function<Source*, Source*, bool>
	{
		bool operator()(const Source* lhs, const Source* rhs) const
		{
			return (*lhs) < (*rhs);
		}
	};


	Image* _image;

	Table<Source> _sources;
	Table<Road>   _roads;

	unsigned int _width;
	unsigned int _height;
	

public :
	Image* getImage() { return _image; }

	void neighbours(Source* source, std::priority_queue<Source*, vector<Source*>, Comparator>& result);

	Watershed(unsigned int width,
		  unsigned int height);

	void addRoad(Road road);

	void paintRoad(Road& road);

	void addSource(LinkedList<Building>::LinkedListIterator building,
		       Vector2F pos, 
		       Vector2F normal,
		       float max);

	void perform();

	void convexify(unsigned int id);
	void quadrify(unsigned int id);
	void getPolygon(unsigned int id);
	void getQuadIn(unsigned int id);
	LinkedList<Vector2F> findPoints (unsigned int id);
	LinkedList<Vector2F> findQuad(LinkedList<Vector2F> points);
	LinkedList<Vector2F> findQuad2(LinkedList<Vector2F> points);
	void simplify(LinkedList<Vector2F>& points);


	Watershed::Source* neighbour(Watershed::Source* source, float dx, float dy);
	bool validNeigh(Watershed::Source* neigh);
	void addNeigh(Watershed::Source* source, float dx, float dy, std::priority_queue<Source*, vector<Source*>, Comparator>& result);

}; // class Watershed


#endif // WATER_SHED
