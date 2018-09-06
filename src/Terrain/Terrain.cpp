/*******************************************************************************
 *
 *     Copyright : 2010 Arnaud Emilien
 *
 *******************************************************************************/

#include "Terrain.h"
#include "../Generator/VillageGenerator.h"

Terrain::Terrain(unsigned int width,
		unsigned int height,
		float maxElevation)
		: _width(width), _height(height), _mesh(NULL), _maxElevation(maxElevation), _snow(Image(width, height, Color::RGB_32)), _heightmap(Image(width, height, Color::RGB_32)), _water(Image(width, height, Color::RGB_8)), _vegetation(Image(width, height, Color::RGB_8)), _rock(Image(width, height, Color::RGB_8)), _roads(Image(width, height, Color::RGB_8)), _houses(Image(width, height, Color::RGB_8)) {
	std::cout << "Generating Terrain... ";
	_mesh = Mesh::plane(width, height, width / 8, height / 8, 1, 1);
	_vegetation.fill(Color::RGBX(255, 255, 255));
	std::cout << "done. " << std::endl;
}

Terrain::~Terrain() {
	delete _mesh;
}

Image *Terrain::getHeightMap() {
	return &_heightmap;
}

Image *Terrain::getWater() {
	return &_water;
}

Image *Terrain::getVegetation() {
	return &_vegetation;
}

Image *Terrain::getRoads() {
	return &_roads;
}

Image *Terrain::getHouses() {
	return &_houses;
}

Image *Terrain::getRock() {
	return &_rock;
}

Image *Terrain::getSnow() {
	return &_snow;
}

float Terrain::getMaxElevation() {
	return _maxElevation;
}

unsigned int Terrain::getHeight() {
	return _height;
}

unsigned int Terrain::getWidth() {
	return _width;
}

void Terrain::setMaxElevation(float maxElevation) {
	_maxElevation = maxElevation;
}

void Terrain::setHeightMap(const Image &image2) {
	Image image = image2;
	if (image.getFormat() == Color::RGB_32) {
		_heightmap = image;
		return;
	}

	// Creation of a float precision heightmap
	unsigned int stepX = _width / image.getWidth();
	unsigned int stepY = _height / image.getHeight();
	_heightmap = Image(_width, _height, Color::RGB_32);
	{
		// COLUMNS
		Color col1, col2, colCur;
		col1 = Color(Color::RGB_8);
		col2 = Color(Color::RGB_8);
		colCur = Color(Color::RGB_32);
		for (unsigned int i = 0; i < image.getHeight(); ++i) {
			for (unsigned int j = 0; j < image.getWidth() - 1; ++j) {
				col1 = image.getAt(j, image.getHeight() - 1 - i);
				col2 = image.getAt(j + 1, image.getHeight() - 1 - i);
				for (unsigned int s = 0; s < stepX; ++s) {
					float t = float(s) / (float(stepX));
					colCur.rf() = 1.0 / 255.0 * ((1.0 - t) * float(col1.r()) + t * float(col2.r()));
					colCur.gf() = 1.0 / 255.0 * ((1.0 - t) * float(col1.g()) + t * float(col2.g()));
					colCur.bf() = 1.0 / 255.0 * ((1.0 - t) * float(col1.b()) + t * float(col2.b()));
					_heightmap.setAt(j * stepX + s, i * stepY, colCur);
				}
			}
		}

		// LINES
		col1 = Color(Color::RGB_32);
		col2 = Color(Color::RGB_32);
		colCur = Color(Color::RGB_32);
		for (unsigned int i = 0; i < image.getHeight() - 1; ++i) {
			for (unsigned int j = 0; j < _width; ++j) {
				col1 = _heightmap.getAt(j, i * stepY);
				col2 = _heightmap.getAt(j, (i + 1) * stepY);
				for (unsigned int s = 1; s < stepY; ++s) {
					float t = float(s) / (float(stepY));
					colCur.rf() = ((1.0 - t) * float(col1.rf()) + t * float(col2.rf()));
					colCur.gf() = ((1.0 - t) * float(col1.gf()) + t * float(col2.gf()));
					colCur.bf() = ((1.0 - t) * float(col1.bf()) + t * float(col2.bf()));
					_heightmap.setAt(j, i * stepY + s, colCur);
				}
			}
		}
	}
}

void Terrain::setSnow(const Image &image) {
	_snow = image;
}

void Terrain::setWater(const Image &image) {
	_water = image;
}

void Terrain::setVegetation(const Image &image) {
	_vegetation = image;
}

void Terrain::setRoads(const Image &image) {
	_roads = image;
}

void Terrain::setHouses(const Image &image) {
	_houses = image;
}

void Terrain::setRock(const Image &image) {
	_rock = image;
}

Mesh *Terrain::generateGeometry() {
	Mesh *result = Mesh::plane(_width, _height, _width / 2, _height / 2, 1, 1);
	for (unsigned int i = 0; i < _height / 2; ++i) {
		for (unsigned int j = 0; j < _width / 2; ++j) {
			float height = getHeightMap()->getAt(j / 2, i / 2).r() * getMaxElevation();
			if (VillageGenerator::_instance->_roadsLayout->getImage()->getAt(j / 2, i / 2).rf() > 0.0) {
				result->getVerticesArray()[j + _height / 2 * i].z = height - 1.f;
			} else {
				result->getVerticesArray()[j + _height / 2 * i].z = height;
			}
		}
	}
	return result;
}
