#include "./BuildingsAtlas.h"
#include "../ThirdParty/tinyxml/tinyxml.h"


const BuildingsAtlas::ParamMap BuildingsAtlas::paramMap =  BuildingsAtlas::create_map();
const BuildingsAtlas::TypeMap BuildingsAtlas::typeMap =  BuildingsAtlas::create_type_map();

bool loadFromXML(const std::string& xmlName);

BuildingsAtlas::BuildingsAtlas() {
  for (unsigned int i = 0; i < Building::CUSTOM; ++i) {
    _parameters.pushLast(Table<float>());
    _parameters[i].resize(NUM_PARAMS);
    for (unsigned int j = 0; j < NUM_PARAMS; ++j) {
      _parameters[i][j] = 0;
    }
  }

  loadFromXML("/home/pgruenbacher/build/Village_clean/data/Atlas/buildingsAtlas.xml");
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


bool BuildingsAtlas::loadBuilding(TiXmlElement* elem) {
  const char* ctype = elem->Attribute("type");
  if (!ctype) return false;
  const std::string typeName = std::string(ctype);
  if (BuildingsAtlas::typeMap.find(typeName) == BuildingsAtlas::typeMap.end()) {
    return false;
  }
  Building::Type type = BuildingsAtlas::typeMap.find(typeName)->second;

  BuildingsAtlas::ParamMap::const_iterator it = BuildingsAtlas::paramMap.begin();
  for ( ; it != BuildingsAtlas::paramMap.end(); ++it) {
    const std::string& key = it->first;
    const BuildingsAtlas::Params param = it->second;
    if (!elem->Attribute(key)) continue;
    double v;
    if ( elem->Attribute( key, &v ) ) {
      float value = static_cast< float >( v );
      _parameters[type][param] = value;
      // std::cout << " Type " << typeName << " " << key << ls" " << value << std::endl;
    }
  }

}

bool BuildingsAtlas::loadBuildings(TiXmlNode* rootNode) {
  TiXmlNode* node = 0;
  for (node = rootNode->FirstChild(); node; node = node->NextSibling()) {
    TiXmlElement* element = node->ToElement();
    if (!element) continue;

    loadBuilding(element);

  }
  return true;
}

bool BuildingsAtlas::loadFromXML(const std::string& xmlName) {
  TiXmlDocument xml(xmlName);
  bool valid = xml.LoadFile();

  if (!valid) {  // load xml file
    std::cerr << "Could not load atlas specification xml " << xmlName << ".\n";
    return false;
  }

  TiXmlElement* rootNode = xml.RootElement();
  if (!rootNode) {
    std::cerr << "Root element does not exist\n.";
    return false;
  }

  // Walk all the nodes of the root node.
  TiXmlNode* node = 0;
  int count = 0;
  for (node = rootNode->FirstChild(); node; node = node->NextSibling()) {
    if (node->ToComment()) continue;
    loadBuildings(node);
    count++;
  }
  std::cout << "Counted Nodes " << count << std::endl;
  return true;
}