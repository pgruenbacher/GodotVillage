/*******************************************************************************
 *
 *     Copyright : 2010 Arnaud Emilien
 *
 *******************************************************************************/

#ifndef ROADS_ATLAS_H
#define ROADS_ATLAS_H

class RoadsAtlas {
public:
	enum Params {
		_travelWeight,
		_curvatureWeight,
		_curvatureMax,
		_slopeWeight,
		_slopeMax,
		_waterWeight,
		_roadsWeight
	};

private:
	Table<Table<float> > _parameters;

public:
	RoadsAtlas() {
		for (unsigned int i = 0; i < Road::CUSTOM; ++i) {
			_parameters.pushLast(Table<float>());
			_parameters[i].resize(_roadsWeight + 1);
			for (unsigned int j = 0; j < _roadsWeight + 1; ++j) {
				_parameters[i][j] = 0;
			}
		}
	}

	void setParameters(Road::Type type, Params param, float value) {
		_parameters[type][param] = value;
	}

	float getParameter(Road::Type type, Params param) {
		return _parameters[type][param];
	}

	void updateParameters(Road &roadToParam, Road::Type type) {
		roadToParam._type = type;

		roadToParam._travelWeight = getParameter(roadToParam._type, _travelWeight);
		roadToParam._curvatureWeight = getParameter(roadToParam._type, _curvatureWeight);
		roadToParam._curvatureMax = getParameter(roadToParam._type, _curvatureMax);
		roadToParam._slopeWeight = getParameter(roadToParam._type, _slopeWeight);
		roadToParam._slopeMax = getParameter(roadToParam._type, _slopeMax);
		roadToParam._waterWeight = getParameter(roadToParam._type, _waterWeight);
		roadToParam._roadsWeight = getParameter(roadToParam._type, _roadsWeight);
	}

}; // class Atlas

#endif // ROADS_ATLAS_H
