#include "./BuildingsAtlas.h"


const map<std::string,BuildingsAtlas::Params> BuildingsAtlas:: paramMap =  BuildingsAtlas::create_map();
const map<std::string,Building::Type> BuildingsAtlas:: typeMap =  BuildingsAtlas::create_type_map();


BuildingsAtlas::BuildingsAtlas() {
  for (unsigned int i = 0; i < Building::CUSTOM; ++i) {
    _parameters.pushLast(Table<float>());
    _parameters[i].resize(_vegetationFeasability + 1);
    for (unsigned int j = 0; j < _vegetationFeasability + 1; ++j) {
      _parameters[i][j] = 0;
    }
  }
}

void BuildingsAtlas::updateParameters(Building &buildingToParam, Building::Type type) {
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