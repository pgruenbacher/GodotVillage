/*******************************************************************************
 *
 *     Copyright : 2010 Arnaud Emilien
 *
 *******************************************************************************/

#ifndef BUILDINGS_ATLAS_H
#define BUILDINGS_ATLAS_H

#include "../Utils/Table.h"
#include "Building.h"

class BuildingsAtlas{
public :        
        enum Params{
	                // INTEREST
                        _sociabilityWeight,
                        _sociabilityMin,
                        _sociabilityBest,
                        _sociabilityMax,
                        
	                _roadsWeight,
	                _roadsMax,
	                
	                _altitudeWeight,
	                _altitudeMax,
	                
	                _dominationWeight,
	                _dominationR,	                             
	                
	                _sunWeight,
	                
	                _fortificationWeight,
	                _fortificationRmax,
	                
	                _militaryWeight,
	                _militaryRmax,
	                
	                _waterWeight,
	                _waterRmax,
	                
	                _cultWeight,
	                _cultRmax,
	                
	                // FEASABILITY
	                _spaceMin,
	                _slopeVarMax,
	                _slopeMax,
	                _waterFeasability,
	                _vegetationFeasability
	               };    
	               
private :
        Table< Table<float> > _parameters;

public :
        BuildingsAtlas()
        {
                for (unsigned int i = 0; i < Building::CUSTOM; ++i)
                {
                        _parameters.pushLast(Table<float> ());
                        _parameters[i].resize(_vegetationFeasability+1);
                        for (unsigned int j = 0; j < _vegetationFeasability+1; ++j)
                        {
                                _parameters[i][j] = 0;
                        }
                }
        }        


        void setParameters(Building::Type type, Params param, float value)
        {
                _parameters[type][param] = value;
        }
        

        void updateParameters(Building& buildingToParam, Building::Type type)
        {
                buildingToParam._type = type;
                
                buildingToParam._sociabilityWeight = getParameter(buildingToParam._type, _sociabilityWeight);
                buildingToParam._sociabilityMin = getParameter(buildingToParam._type, _sociabilityMin);
                buildingToParam._sociabilityBest = getParameter(buildingToParam._type, _sociabilityBest);
                buildingToParam._sociabilityMax = getParameter(buildingToParam._type, _sociabilityMax);
                buildingToParam._roadsWeight = getParameter(buildingToParam._type, _roadsWeight);
                buildingToParam._roadsMax = getParameter(buildingToParam._type, _roadsMax);
                buildingToParam._altitudeWeight = getParameter(buildingToParam._type, _altitudeWeight);
                buildingToParam._altitudeMax = getParameter(buildingToParam._type, _altitudeMax);
                buildingToParam._dominationWeight = getParameter(buildingToParam._type, _dominationWeight);
                buildingToParam._dominationR = getParameter(buildingToParam._type, _dominationR);
                buildingToParam._sunWeight = getParameter(buildingToParam._type, _sunWeight);
                buildingToParam._fortificationWeight = getParameter(buildingToParam._type, _fortificationWeight);
                buildingToParam._fortificationRmax = getParameter(buildingToParam._type, _fortificationRmax);
                buildingToParam._militaryWeight = getParameter(buildingToParam._type, _militaryWeight);
                buildingToParam._militaryRmax = getParameter(buildingToParam._type, _militaryRmax);
                buildingToParam._waterWeight = getParameter(buildingToParam._type, _waterWeight);
                buildingToParam._waterRmax = getParameter(buildingToParam._type, _waterRmax);
                buildingToParam._cultWeight = getParameter(buildingToParam._type, _cultWeight);
                buildingToParam._cultRmax = getParameter(buildingToParam._type, _cultRmax);
                buildingToParam._spaceMin = getParameter(buildingToParam._type, _spaceMin);
                buildingToParam._slopeVarMax = getParameter(buildingToParam._type, _slopeVarMax);
                buildingToParam._slopeMax = getParameter(buildingToParam._type, _slopeMax);
                buildingToParam._waterFeasability = getParameter(buildingToParam._type, _waterFeasability);
                buildingToParam._vegetationFeasability = getParameter(buildingToParam._type, _vegetationFeasability);
        }
        
        float getParameter(Building::Type type, Params param)
        {
                return _parameters[type][param];
        }


}; // class Atlas

#endif // BUILDINGS_ATLAS_H

