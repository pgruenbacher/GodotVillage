/*******************************************************************************
 *
 *     Copyright : 2010 Arnaud Emilien
 *
 *******************************************************************************/

#ifndef _COST_FUNCTOR_GRADIENT_H
#define _COST_FUNCTOR_GRADIENT_H

#include "RoadCostFunctor.h"
#include <iostream>
#include "../../Maths/Vector.h"
#include "../../Image/PainterAlgorithms.h"
#include "../Road.h"
#include <math.h>

#define ISNAN(f) ((f)!=(f))


template <class C, class K, class V>
class CostFunctorGradient : public RoadCostFunctor<C,K,V >{
public :
	CostFunctorGradient(){}
	~CostFunctorGradient() {}
	
	virtual C evaluate(GraphArc<C,K,V>* arc, 
	                   Graph<C,K,V>* graph,
			   LinkedList<K>& path,
	                   Layout<C,K,V>& layout) 
	{
	        return C(0);
	}
}; // class CostFunctorGradient


template <class C, class K>
class CostFunctorGradient<C,K, Vector3<C> > : public RoadCostFunctor<C,K,Vector3<C> >{
private :
        K keySource;
	K keyDest;
	C result;
	Vector3<C> valN_0, valN_1, valN_2, valN_3;
	C travel, slope, curvature, curvature2;
	C dot;
	Vector3<C> cross;
	C dotCross;
	C acosinus;
	C road;
	C water_depth;
	C water;

public :
	CostFunctorGradient(){}
	~CostFunctorGradient() {}
	
	virtual C evaluate(GraphArc<C,K,Vector3<C> >* arc, 
	                   Graph<C,K,Vector3<C> >* graph,
			   LinkedList<K>& path,
	                   Layout<C,K,Vector3<C> >& layout) 
	{	        
	        keySource = arc->getSourceNodeKey();
	        keyDest   = arc->getDestNodeKey();
	        result    = C(0);
	        	        	        
	        // BUILDING
		/*
		  if (this->_buildingsLayout->getValueAt(keyDest) > 0.0)
		  return 9999999.0;
		*/
		
		
	        Table<Vector2I> pointsToTest = PainterAlgorithms::findBresenhamLinePoints(keySource,keyDest);
	        for (unsigned int i = 0; i < pointsToTest.getSize(); i += 2)//++i)
	        {
	                if (this->_buildingsLayout->getValueAt(pointsToTest[i]) > 0.0)
	                        return 9999999.0;
	                	        
	                // WALL
	                if (this->_wallsLayout->getValueAt(pointsToTest[i]) > 200.0)
	                        return 9999999.0;
	        }
		
		
	        
		// TRAVEL
	        valN_0 = graph->getNode(keyDest).getPair().getValue();
	        valN_1 = graph->getNode(keySource).getPair().getValue();
	        travel = norm(valN_0 - valN_1);
	        //std::cout << "travel : " << travel << std::endl;	
	        
	        // SLOPE
	        slope = layout.getValueAt(keyDest) - layout.getValueAt(keySource);
	        if (slope < 0.0)
	        {
	                slope = - slope;
	        }
	                
	        if (slope > this->_usedRoad._slopeMax)
	        {
	                return 9999999.0;
	        }
	        else
	        {
	                //slope = sqrt(slope);
                        slope = slope*slope;
	        }
	        //std::cout << "slope : " << slope << std::endl;	 
	        //std::cout << "this->_usedRoad._slopeWeight   " << this->_usedRoad._slopeWeight   << std::endl;	
	              	   
	        
	             
	        // CURVATURE
	        typename LinkedList<K>::LinkedListIterator it(&path);	        
	        if (it.hasNext())
	        {	                
	                it.next();
	        }
	        curvature  = C(0);
		curvature2 = C(0);
	        if (it.hasNext())
	        {	                
	                it.next();
	                valN_2   = graph->getNode(it.value()).getPair().getValue();
	                
			dot      = dotProduct( normalize(valN_0 - valN_1), normalize(valN_1 - valN_2) );
			cross    = crossProduct(normalize(valN_0 - valN_1), normalize(valN_1 - valN_2));
			dotCross = dotProduct(cross, Vector3<C>(0,0,1));

		        //std::cout << "dot" << dot << std::endl;
		        //std::cout << "acosinus" << acosinus << std::endl;
		        //std::cout << "dotCross" << dotCross << std::endl;
		
			acosinus  = ( abs(dot-1) < 0.001 ) ? 0 : acosf(dot);
	                curvature   = dotCross > 0 ? acosinus : - acosinus;
	        }
		//std::cout << "curvature" << curvature << std::endl;
		if (it.hasNext())
		{    
	                it.next();
	                valN_3 = graph->getNode(it.value()).getPair().getValue();
	                
			dot       = dotProduct( normalize(valN_1 - valN_2), normalize(valN_2 - valN_3) );
			cross = crossProduct(normalize(valN_1 - valN_2), normalize(valN_2 - valN_3));
			dotCross = dotProduct(cross, Vector3<C>(0,0,1));

			acosinus  = ( abs(dot-1) < 0.001 ) ? 0 : acosf(dot);
	                curvature2   = dotCross >= 0 ? acosinus : - acosinus;
			

			//curvature = abs(curvature) * abs(curvature) + abs(curvature - curvature2);
			curvature = abs(curvature);
		}
		if (curvature >=  this->_usedRoad._curvatureMax)
		        return 9999999.0;
		//std::cout << "curvature bis" << curvature << std::endl;
		
		
		// WATER     
		water_depth = this->_waterLayout->getValueAt(keyDest);
	        water = water_depth*water_depth;

	        /*
	        building = 0.0;
	        LinkedList<Building>::LinkedListIterator itb(this->_buildings);
	        while (itb.hasNext())
	        {
	               itb.next();
	               bpos.x = itb.value().getNode().getPosition().x;
	               bpos.y = itb.value().getNode().getPosition().z;
	               dist = norm (bpos - valN_0);
	               if ( dist <= 2.0* itb.value()._spaceMin)
	               {
        	                std::cout << "Building ! " << std::endl;
	                        building = 999999.0;
	               }
	        }
	        */

		
	        // TOTAL
	        result =  this->_usedRoad._travelWeight    * travel 
			+ this->_usedRoad._slopeWeight     * slope
			+ this->_usedRoad._curvatureWeight * curvature
			+ this->_usedRoad._waterWeight     * water;

		// ROAD
		road = this->_roadsLayout->getValueAt(keyDest);
		if (road > 0)
		{
			//std::cout << "On the road ! " << std::endl;
			result *= this->_usedRoad._roadsWeight;
		}

	        
	        //std::cout << "Result : " << result << std::endl;
	        
	        return result;
	}
};



#endif // _COST_FUNCTOR_GRADIENT_H
