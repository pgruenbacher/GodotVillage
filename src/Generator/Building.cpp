/*******************************************************************************
 *
 *     Copyright : 2010 Arnaud Emilien
 *
 *******************************************************************************/

#include "Building.h"
#include "Building/GardenWall.h"

#include "Building/Church.h"
#include "VillageGenerator.h"

void Building::faceToRoad() {
	// CREATE FOOTPRINT
	Vector2F normal;
	Vector2F pos;
	pos.x = getNode().getPosition().x;
	pos.y = getNode().getPosition().z;
	normal = VillageGenerator::_instance->findClosestRoadNormal(*this);

	if (normal.x == 0.0 && normal.y == 0.0) {
		normal.y = 1.0;
	}

	if (VillageGenerator::_instance->_village.getRoads().getSize() == 0) {
		normal.x = 0.0;
		normal.y = 1.0;
	}

	float angle = orientedAngle(normal, Vector2F(0, 1));

	std::cout << "Normal : " << normal << std::endl;
	std::cout << "angle : " << angle << std::endl;
	Quaternion rotation = Quaternion::createFromAxisAndAngle(angle, Vector3F::YAXIS);
	getNode().setOrientation(rotation);

	Table<Vector2I> point;
	Vector2F forward2D, left2D;
	forward2D = normal;
	left2D = Vector2F(-normal.y, normal.x);

	_normal2D = normal;

	point.pushLast(pos + left2D * _spaceMin + forward2D * -_spaceMin);
	point.pushLast(pos + left2D * _spaceMin + forward2D * _spaceMin);
	point.pushLast(pos + left2D * -_spaceMin + forward2D * _spaceMin);
	point.pushLast(pos + left2D * -_spaceMin + forward2D * -_spaceMin);

	// Add the building
	_2DFootPrint = point;
}

Mesh Building::generateGeometry() {
	_mesh.empty();

	switch (_type) {

		case HOUSE: {
			if (_2DFootPrint.getSize() < 4)
				return _mesh;

			// CREATE GEOMETRY
			Vector3F p0 = Vector3F(_2DFootPrint[0].x, 0, _2DFootPrint[0].y);
			Vector3F p1 = Vector3F(_2DFootPrint[1].x, 0, _2DFootPrint[1].y);
			Vector3F p2 = Vector3F(_2DFootPrint[2].x, 0, _2DFootPrint[2].y);
			Vector3F p3 = Vector3F(_2DFootPrint[3].x, 0, _2DFootPrint[3].y);

			std::cout << "p0 " << p0 << std::endl;
			std::cout << "p1 " << p1 << std::endl;
			std::cout << "p2 " << p2 << std::endl;
			std::cout << "p3 " << p3 << std::endl;

			float elevation = VillageGenerator::_instance->_terrain->getMaxElevation();

			p0.y = elevation * VillageGenerator::_instance->_heightmapLayout->getValueAt(Vector2I(p0.x, p0.z));
			p1.y = elevation * VillageGenerator::_instance->_heightmapLayout->getValueAt(Vector2I(p1.x, p1.z));
			p2.y = elevation * VillageGenerator::_instance->_heightmapLayout->getValueAt(Vector2I(p2.x, p2.z));
			p3.y = elevation * VillageGenerator::_instance->_heightmapLayout->getValueAt(Vector2I(p3.x, p3.z));

			p0.z = -p0.z;
			p1.z = -p1.z;
			p2.z = -p2.z;
			p3.z = -p3.z;

			QuadFootPrint footPrint(Quad(p0, p1, p2, p3));
			_mesh = footPrint.generate();
			_mesh.prepareIllumination();
		} break;

		case VILLA: {
			if (_2DHousePrint.getSize() == 4) {
				// CREATE GEOMETRY
				Vector3F p0 = Vector3F(_2DHousePrint[0].x, 0, _2DHousePrint[0].y);
				Vector3F p1 = Vector3F(_2DHousePrint[1].x, 0, _2DHousePrint[1].y);
				Vector3F p2 = Vector3F(_2DHousePrint[2].x, 0, _2DHousePrint[2].y);
				Vector3F p3 = Vector3F(_2DHousePrint[3].x, 0, _2DHousePrint[3].y);

				std::cout << "p0 " << p0 << std::endl;
				std::cout << "p1 " << p1 << std::endl;
				std::cout << "p2 " << p2 << std::endl;
				std::cout << "p3 " << p3 << std::endl;

				float elevation = VillageGenerator::_instance->_terrain->getMaxElevation();

				p0.y = elevation * VillageGenerator::_instance->_heightmapLayout->getValueAt(Vector2I(p0.x, p0.z));
				p1.y = elevation * VillageGenerator::_instance->_heightmapLayout->getValueAt(Vector2I(p1.x, p1.z));
				p2.y = elevation * VillageGenerator::_instance->_heightmapLayout->getValueAt(Vector2I(p2.x, p2.z));
				p3.y = elevation * VillageGenerator::_instance->_heightmapLayout->getValueAt(Vector2I(p3.x, p3.z));

				p0.z = -p0.z;
				p1.z = -p1.z;
				p2.z = -p2.z;
				p3.z = -p3.z;

				QuadFootPrint footPrint(Quad(p0, p1, p2, p3));
				_mesh = footPrint.generate();
				_mesh.prepareIllumination();

				GardenWall gwall(_2DFootPrint);
				_mesh += gwall.generate();
				_mesh.prepareIllumination();
			}
		} break;

		case FIELD: {
			Field field(_2DFootPrint);
			_mesh = field.generate();
			_mesh.prepareIllumination();
		} break;

		case CHURCH: {
			if (_2DHousePrint.getSize() == 4) {
				// CREATE GEOMETRY
				Vector3F p0 = Vector3F(_2DHousePrint[0].x, 0, _2DHousePrint[0].y);
				Vector3F p1 = Vector3F(_2DHousePrint[1].x, 0, _2DHousePrint[1].y);
				Vector3F p2 = Vector3F(_2DHousePrint[2].x, 0, _2DHousePrint[2].y);
				Vector3F p3 = Vector3F(_2DHousePrint[3].x, 0, _2DHousePrint[3].y);

				std::cout << "p0 " << p0 << std::endl;
				std::cout << "p1 " << p1 << std::endl;
				std::cout << "p2 " << p2 << std::endl;
				std::cout << "p3 " << p3 << std::endl;

				float elevation = VillageGenerator::_instance->_terrain->getMaxElevation();

				p0.y = elevation * VillageGenerator::_instance->_heightmapLayout->getValueAt(Vector2I(p0.x, p0.z));
				p1.y = elevation * VillageGenerator::_instance->_heightmapLayout->getValueAt(Vector2I(p1.x, p1.z));
				p2.y = elevation * VillageGenerator::_instance->_heightmapLayout->getValueAt(Vector2I(p2.x, p2.z));
				p3.y = elevation * VillageGenerator::_instance->_heightmapLayout->getValueAt(Vector2I(p3.x, p3.z));

				p0.z = -p0.z;
				p1.z = -p1.z;
				p2.z = -p2.z;
				p3.z = -p3.z;

				Church footPrint(Quad(p0, p1, p2, p3));
				_mesh = footPrint.generate();
				_mesh.prepareIllumination();
			}
		} break;

		default:
			break;
	}

	return _mesh;
}
