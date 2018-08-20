/*******************************************************************************
 *
 *     Copyright : 2010 Arnaud Emilien
 *
 *******************************************************************************/

#ifndef _BUILDING_H
#define _BUILDING_H

#include <iostream>


#include "../Engine/Object3D.h"
#include "../Engine/Mesh.h"
#include "Building/QuadFootPrint.h"
#include "Building/Field.h"

class Building : public Object3D{
public :
	enum Type{
		HOUSE,
		VILLA,
		GARDEN,

		TRADE,

		CHURCH,
		STATUE,

		FIELD,
		WATER_MILL,
		WIND_MILL,

		TOWER_DEFENSE,
		CASTLE,
		WALL,
		
	        CUSTOM
	};
public :
        unsigned int    _age;

	Vector2F _normal2D;
        
        float _sociabilityWeight;
        float _sociabilityMin;
        float _sociabilityBest;
        float _sociabilityMax;
                        
        float _roadsWeight;
        float _roadsMax;
	                
        float _altitudeWeight;
        float _altitudeMax;
	                
        float _dominationWeight;
        float _dominationR;
	                
        float _sunWeight;
	                
        float _fortificationWeight;
        float _fortificationRmax;
	                
        float _militaryWeight;
        float _militaryRmax;
	                
        float _waterWeight;
        float _waterRmax;
	                
        float _cultWeight;
        float _cultRmax;
	                
        float _spaceMin;
        float _slopeVarMax;
        float _slopeMax;
        float _waterFeasability;
        float _vegetationFeasability;
                
        Type _type;

	Mesh _mesh;

	Mesh generateGeometry();
	void faceToRoad();
	

	Table<Vector2F> _2DFootPrint;
	Table<Vector2F> _2DHousePrint;
        

        
}; // class Building


#endif // _BUILDING_H
