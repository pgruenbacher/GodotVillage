/*******************************************************************************
 *
 *     Copyright : 2010 Arnaud Emilien
 *
 *******************************************************************************/

#include "../VillageGenerator.h"

#include "Window.h"

Mesh Window::generate() {
	std::cout << "Generate Window" << std::endl;
	_mesh.empty();

	Vector2I pos0(_quad.p0().x, -_quad.p0().z);
	float terrainHeight0 = VillageGenerator::_instance->_heightmapLayout->getValueAt(pos0) * VillageGenerator::_instance->_terrain->getMaxElevation();

	Vector2I pos1(_quad.p1().x, -_quad.p1().z);
	float terrainHeight1 = VillageGenerator::_instance->_heightmapLayout->getValueAt(pos1) * VillageGenerator::_instance->_terrain->getMaxElevation();

	/*
		if (terrainHeight0 >= _quad.p0().y 
			|| terrainHeight1 >= _quad.p1().y )
		{
			_mesh += _quad.generate();
			return _mesh;
		}
                */

	switch (_type) {
		case SQUARE: {
			float totalWidth = norm(_quad.p01());
			float windowWidth = 1.f;
			float offset = (totalWidth - windowWidth) / 2.f;
			Table<Quad> partDiv1 = _quad.cutLeft(totalWidth - offset);
			Table<Quad> partDiv2 = partDiv1[0].cutLeft(offset);

			_mesh += partDiv1[1].generate();
			_mesh += partDiv2[0].generate();

			Quad windowPart = partDiv2[1];

			Table<Quad> extrusion = windowPart.extrude(-0.3);

			for (unsigned int i = 1; i < extrusion.getSize(); ++i) {
				WallE wall = WallE(extrusion[i]);
				_mesh += wall.generate();
			}

			// WINDOW
			Quad quad = extrusion[0];
			Table<Quad> divLR = quad.subdiviseX(2);

			Table<Quad> divL = divLR[0].rotateToY().subdiviseX(3);
			Table<Quad> divR = divLR[1].rotateToY().subdiviseX(3);

			Mesh glass;

			for (unsigned int i = 0; i < divL.getSize(); ++i) {
				Table<Quad> ext = divL[i].extrude(0.0, 0.9);
				Table<Quad> temp = ext[0].extrude(-0.05);
				for (unsigned int j = 1; j < ext.getSize(); ++j) {
					_mesh += ext[j].generate();
				}
				for (unsigned int j = 1; j < temp.getSize(); ++j) {
					_mesh += temp[j].generate();
				}
				glass += temp[0].generate();
			}

			for (unsigned int i = 0; i < divR.getSize(); ++i) {
				Table<Quad> ext = divR[i].extrude(0.0, 0.9);
				Table<Quad> temp = ext[0].extrude(-0.05);
				for (unsigned int j = 1; j < ext.getSize(); ++j) {
					_mesh += ext[j].generate();
				}
				for (unsigned int j = 1; j < temp.getSize(); ++j) {
					_mesh += temp[j].generate();
				}
				glass += temp[0].generate();
			}

			// SHUTTERS SUPPORT
			Vector3F ss0 = windowPart.p0() - 0.05f * normalize(windowPart.p01()) + 0.2f * normalize(windowPart.p03()) + 0.05f * windowPart.normal();

			Vector3F ss1 = windowPart.p1() + 0.05f * normalize(windowPart.p01()) + 0.2f * normalize(windowPart.p03()) + 0.05f * windowPart.normal();

			Vector3F ss2 = windowPart.p2() + 0.05f * normalize(windowPart.p32()) - 0.2f * normalize(windowPart.p03()) + 0.05f * windowPart.normal();

			Vector3F ss3 = windowPart.p3() - 0.05f * normalize(windowPart.p32()) - 0.2f * normalize(windowPart.p03()) + 0.05f * windowPart.normal();

			Mesh wood;

			wood += Mesh::cube(ss0 + Vector3F(0.0, 0.05, 0), ss0 - Vector3F(0.01, 0.05, 0), 0.02);
			wood += Mesh::cube(ss1 + Vector3F(0.0, 0.05, 0), ss1 - Vector3F(0.01, 0.05, 0), 0.02);
			wood += Mesh::cube(ss2 + Vector3F(0.0, 0.05, 0), ss2 - Vector3F(0.01, 0.05, 0), 0.02);
			wood += Mesh::cube(ss3 + Vector3F(0.0, 0.05, 0), ss3 - Vector3F(0.01, 0.05, 0), 0.02);

			wood += Mesh::cube(ss0, ss0 - 0.05f * normalize(windowPart.p01()) + 0.05f * windowPart.normal(), 0.015);
			wood += Mesh::cube(ss1, ss1 + 0.05f * normalize(windowPart.p01()) + 0.05f * windowPart.normal(), 0.015);
			wood += Mesh::cube(ss2, ss2 + 0.05f * normalize(windowPart.p32()) + 0.05f * windowPart.normal(), 0.015);
			wood += Mesh::cube(ss3, ss3 - 0.05f * normalize(windowPart.p32()) + 0.05f * windowPart.normal(), 0.015);

			// Style SHUTTERS
			Vector3F pp0, pp1, pp2, pp3, pp4, pp5, pp6, pp7;
			pp0 = windowPart.p0() - 0.6f * normalize(windowPart.p01()) + 0.05f * windowPart.normal();
			pp1 = windowPart.p0() - 0.1f * normalize(windowPart.p01()) + 0.05f * windowPart.normal();
			pp2 = windowPart.p3() - 0.1f * normalize(windowPart.p32()) + 0.05f * windowPart.normal();
			pp3 = windowPart.p3() - 0.6f * normalize(windowPart.p32()) + 0.05f * windowPart.normal();
			Shutter left(Quad(pp0, pp1, pp2, pp3));
			wood += left.generate();

			pp4 = windowPart.p1() + 0.1f * normalize(windowPart.p01()) + 0.05f * windowPart.normal();
			pp5 = windowPart.p1() + 0.6f * normalize(windowPart.p01()) + 0.05f * windowPart.normal();
			pp6 = windowPart.p2() + 0.6f * normalize(windowPart.p32()) + 0.05f * windowPart.normal();
			pp7 = windowPart.p2() + 0.1f * normalize(windowPart.p32()) + 0.05f * windowPart.normal();
			Shutter right(Quad(pp4, pp5, pp6, pp7));
			wood += right.generate();

			wood.prepareIllumination();
			VillageGenerator::_instance->_wood.pushLast(wood);

			glass.prepareIllumination();
			VillageGenerator::_instance->_glass.pushLast(glass);

		} break;
		case CAVE_SQUARE: {
			float totalWidth = norm(_quad.p01());
			float windowWidth = 1.f;
			float offset = (totalWidth - windowWidth) / 2.f;
			Table<Quad> partDiv1 = _quad.cutLeft(totalWidth - offset);
			Table<Quad> partDiv2 = partDiv1[0].cutLeft(offset);

			_mesh += partDiv1[1].generate();
			_mesh += partDiv2[0].generate();

			Quad windowPart = partDiv2[1];

			Table<Quad> extrusion = windowPart.extrude(-0.3);

			for (unsigned int i = 1; i < extrusion.getSize(); ++i) {
				WallE wall = WallE(extrusion[i]);
				_mesh += wall.generate();
			}

			// WINDOW
			Quad quad = extrusion[0];
			Table<Quad> divLR = quad.subdiviseX(2);

			Table<Quad> divL = divLR[0].rotateToY().subdiviseX(3);
			Table<Quad> divR = divLR[1].rotateToY().subdiviseX(3);

			for (unsigned int i = 0; i < divL.getSize(); ++i) {
				Table<Quad> ext = divL[i].extrude(0.0, 0.9);
				Table<Quad> temp = ext[0].extrude(-0.05);
				for (unsigned int j = 1; j < ext.getSize(); ++j) {
					_mesh += ext[j].generate();
				}
				for (unsigned int j = 0; j < temp.getSize(); ++j) {
					_mesh += temp[j].generate();
				}
			}

			for (unsigned int i = 0; i < divR.getSize(); ++i) {
				Table<Quad> ext = divR[i].extrude(0.0, 0.9);
				Table<Quad> temp = ext[0].extrude(-0.05);
				for (unsigned int j = 1; j < ext.getSize(); ++j) {
					_mesh += ext[j].generate();
				}
				for (unsigned int j = 0; j < temp.getSize(); ++j) {
					_mesh += temp[j].generate();
				}
			}

			// BARREAUX
			Table<Vector3F> ptops = subdivise(quad.p0(), quad.p1(), 0.15);
			Table<Vector3F> pbots = subdivise(quad.p3(), quad.p2(), 0.15);

			for (unsigned int i = 0; i < ptops.getSize(); ++i) {
				_mesh += Mesh::cube(ptops[i] + 0.1f * normalize(_quad.normal()),
						pbots[i] + 0.1f * normalize(_quad.normal()) + Vector3F(0.f, 0.01f, 0.f),
						0.015);
			}

		} break;

		case SMALL: {
			float totalWidth = norm(_quad.p01());
			float windowWidth = 0.8f;
			float offset = (totalWidth - windowWidth) / 2.f;
			Table<Quad> partDiv1 = _quad.cutLeft(totalWidth - offset);
			Table<Quad> partDiv2 = partDiv1[0].cutLeft(offset);

			_mesh += partDiv1[1].generate();
			_mesh += partDiv2[0].generate();

			Quad windowPart = partDiv2[1];

			Table<Quad> extrusion = windowPart.extrude(-0.3);

			for (unsigned int i = 1; i < extrusion.getSize(); ++i) {
				WallE wall = WallE(extrusion[i]);
				_mesh += wall.generate();
			}

			// WINDOW
			Quad quad = extrusion[0];
			Table<Quad> divLR = quad.subdiviseX(2);

			Table<Quad> divL = divLR[0].rotateToY().subdiviseX(3);
			Table<Quad> divR = divLR[1].rotateToY().subdiviseX(3);

			for (unsigned int i = 0; i < divL.getSize(); ++i) {
				Table<Quad> ext = divL[i].extrude(0.0, 0.9);
				Table<Quad> temp = ext[0].extrude(-0.05);
				for (unsigned int j = 1; j < ext.getSize(); ++j) {
					_mesh += ext[j].generate();
				}
				for (unsigned int j = 0; j < temp.getSize(); ++j) {
					_mesh += temp[j].generate();
				}
			}

			for (unsigned int i = 0; i < divR.getSize(); ++i) {
				Table<Quad> ext = divR[i].extrude(0.0, 0.9);
				Table<Quad> temp = ext[0].extrude(-0.05);
				for (unsigned int j = 1; j < ext.getSize(); ++j) {
					_mesh += ext[j].generate();
				}
				for (unsigned int j = 0; j < temp.getSize(); ++j) {
					_mesh += temp[j].generate();
				}
			}

		} break;

		case KILL: {
			float totalWidth = norm(_quad.p01());
			float windowWidth = 0.3f;
			float offset = (totalWidth - windowWidth) / 2.f;
			Table<Quad> partDiv1 = _quad.cutLeft(totalWidth - offset);
			Table<Quad> partDiv2 = partDiv1[0].cutLeft(offset);

			_mesh += partDiv1[1].generate();
			_mesh += partDiv2[0].generate();

			Quad windowPart = partDiv2[1];

			Table<Quad> extrusion = windowPart.extrude(-0.3);
			VillageGenerator::_instance->_glass.pushLast(extrusion[0].generate());
			_mesh += extrusion[1].generate();
			_mesh += extrusion[2].generate();
			_mesh += extrusion[3].generate();
			_mesh += extrusion[4].generate();

		} break;

		case KILL2: {
			float totalWidth = norm(_quad.p01());
			float windowWidth = 0.7f;
			float offset = (totalWidth - windowWidth) / 2.f;
			Table<Quad> partDiv1 = _quad.cutLeft(totalWidth - offset);
			Table<Quad> partDiv2 = partDiv1[0].cutLeft(offset);

			_mesh += partDiv1[1].generate();
			_mesh += partDiv2[0].generate();

			Quad windowPart = partDiv2[1];

			Table<Quad> extrusion = windowPart.extrude(-0.3);
			VillageGenerator::_instance->_glass.pushLast(extrusion[0].generate());
			_mesh += extrusion[1].generate();
			_mesh += extrusion[2].generate();
			_mesh += extrusion[3].generate();
			_mesh += extrusion[4].generate();

		} break;
	}

	return _mesh;
}
