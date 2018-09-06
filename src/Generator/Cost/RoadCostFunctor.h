/*******************************************************************************
 *
 *     Copyright : 2010 Arnaud Emilien
 *
 *******************************************************************************/

#ifndef _ROAD_COST_FUNCTOR_H
#define _ROAD_COST_FUNCTOR_H

#include "../../PathFinding/CostFunctor.h"
#include "../../Utils/ImageLayout.h"
#include "../Road.h"
#include <iostream>

template <class C, class K, class V>
class RoadCostFunctor : public CostFunctor<C, K, V> {
protected:
	Road _usedRoad;
	ImageLayout<C, K, V> *_roadsLayout;
	ImageLayout<C, K, V> *_waterLayout;
	ImageLayout<C, K, V> *_vegetationLayout;
	ImageLayout<C, K, V> *_rockLayout;
	ImageLayout<C, K, V> *_buildingsLayout;
	ImageLayout<C, K, V> *_wallsLayout;
	//LinkedList<Building>* _buildings;

public:
	void setRoad(const Road &road) {
		_usedRoad = road;
		std::cout << "setRoad " << std::endl;
		std::cout << "_usedRoad._slopeWeight " << _usedRoad._slopeWeight << std::endl;
	}
	void setRoadsLayout(ImageLayout<C, K, V> *roadsLayout) {
		_roadsLayout = roadsLayout;
	}
	void setWaterLayout(ImageLayout<C, K, V> *waterLayout) {
		_waterLayout = waterLayout;
	}
	void setRockLayout(ImageLayout<C, K, V> *rockLayout) {
		_rockLayout = rockLayout;
	}
	void setVegetationLayout(ImageLayout<C, K, V> *vegetationLayout) {
		_vegetationLayout = vegetationLayout;
	}
	/*
	void setBuildings(LinkedList<Building>* buildings)
	{
		_buildings = buildings;
	}
	*/
	void setBuildingsLayout(ImageLayout<C, K, V> *buildings) {
		_buildingsLayout = buildings;
	}
	void setWallsLayout(ImageLayout<C, K, V> *walls) {
		_wallsLayout = walls;
	}

public:
	RoadCostFunctor() {}
	~RoadCostFunctor() {}

	virtual C evaluate(GraphArc<C, K, V> *arc,
			Graph<C, K, V> *graph,
			LinkedList<K> &path,
			Layout<C, K, V> &layout) = 0;

}; // class CostFunctorVegetation

#endif // _COST_FUNCTOR_VEGETATION_H
