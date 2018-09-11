/*******************************************************************************
 *
 *     Copyright : 2010 Arnaud Emilien
 *
 *******************************************************************************/

#ifndef BUILDINGS_ATLAS_H
#define BUILDINGS_ATLAS_H

#include "../Utils/Table.h"
#include "Building.h"
#include <map>

class BuildingsAtlas {
public:
	enum Params {
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
		_vegetationFeasability,

		NUM_PARAMS
	};

	typedef map<std::string, Building::Type> TypeMap;
	typedef map<std::string, Params> ParamMap;
	static const TypeMap typeMap;
  static const ParamMap paramMap;
  static void setParam(Building &building, Params param, float value);

private:
	Table<Table<float> > _parameters;


  static TypeMap create_type_map() {
    TypeMap m;
    m["HOUSE"] = Building::HOUSE;
		m["VILLA"] = Building::VILLA;
		m["GARDEN"] = Building::GARDEN;
		m["TRADE"] = Building::TRADE;
		m["CHURCH"] = Building::CHURCH;
		m["STATUE"] = Building::STATUE;
		m["FIELD"] = Building::FIELD;
		m["WATER_MILL"] = Building::WATER_MILL;
		m["WIND_MILL"] = Building::WIND_MILL;
		m["TOWER_DEFENSE"] = Building::TOWER_DEFENSE;
		m["CASTLE"] = Building::CASTLE;
		m["WALL"] = Building::WALL;
		m["CUSTOM"] = Building::CUSTOM;
		return m;
  }


  static ParamMap create_map() {
    ParamMap m;
		// INTEREST
		m["sociabilityWeight"] = _sociabilityWeight;
		m["sociabilityMin"] = _sociabilityMin;
		m["sociabilityBest"] = _sociabilityBest;
		m["sociabilityMax"] = _sociabilityMax;

		m["roadsWeight"] = _roadsWeight;
		m["roadsMax"] = _roadsMax;

		m["altitudeWeight"] = _altitudeWeight;
		m["altitudeMax"] = _altitudeMax;

		m["dominationWeight"] = _dominationWeight;
		m["dominationR"] = _dominationR;

		m["sunWeight"] = _sunWeight;

		m["fortificationWeight"] = _fortificationWeight;
		m["fortificationRmax"] = _fortificationRmax;

		m["militaryWeight"] = _militaryWeight;
		m["militaryRmax"] = _militaryRmax;

		m["waterWeight"] = _waterWeight;
		m["waterRmax"] = _waterRmax;

		m["cultWeight"] = _cultWeight;
		m["cultRmax"] = _cultRmax;

		// FEASABILITY
		m["spaceMin"] = _spaceMin;
		m["slopeVarMax"] = _slopeVarMax;
		m["slopeMax"] = _slopeMax;
		m["waterFeasability"] = _waterFeasability;
		m["vegetationFeasability"] = _vegetationFeasability;
		return m;
  }

public:
	BuildingsAtlas();

	void setParameters(Building::Type type, Params param, float value) {
		_parameters[type][param] = value;
	}

	void updateParameters(Building &buildingToParam, Building::Type type);

	float getParameter(Building::Type type, Params param) {
		return _parameters[type][param];
	}

}; // class Atlas


#endif // BUILDINGS_ATLAS_H
