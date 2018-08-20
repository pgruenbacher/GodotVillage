/*******************************************************************************
 *
 *     Copyright : 2010 Arnaud Emilien
 *
 *******************************************************************************/

#ifndef WINDOW_FACADE_H
#define WINDOW_FACADE_H

#include "Quad.h"
#include "WallE.h"
#include "Window.h"
#include "Stair.h"
#include "../../Engine/Mesh.h"

class WindowFacade{
public :
	enum Type{ NORMAL,
                   CAVE,
                   CHURCH};

	Mesh _mesh;
	Quad _quad;
        Type _type;
        

	WindowFacade(){}

	WindowFacade(Quad quad, Type type = NORMAL)
		: _quad(quad)
                , _type(type)
	{}

	Mesh generate()
	{		
		std::cout << "Generate Window Facade" << std::endl;

		_mesh.empty();

		float bottomLength = _quad.bottomLength();
		float topLength    = _quad.topLength();
		
		float windowWidth  = 3.0f;
		int nbWindow = int(topLength / windowWidth);
		float offset = (topLength - float(nbWindow) * windowWidth) / 2.f;

		Table<Quad> div = _quad.subdiviseXwidthBased(windowWidth);
		
		WallE wall0, wall1;
		wall0 = WallE(div[0]);
		wall1 = WallE(div[div.getSize()-1]);

		_mesh += wall0.generate();
		_mesh += wall1.generate();



		if ( div.getSize() == 0)
			return _quad.generate();

		for (int i = 1; i < div.getSize() - 1; ++i)
		{	
			Quad windowq(Vector3F(0,0,0),
				     Vector3F(2,0,0),
				     Vector3F(2,1,0),
				     Vector3F(0,1,0));

			Quad windowqq(Vector3F(0,0,0),
				     Vector3F(2,0,0),
				     Vector3F(2,1,0),
				     Vector3F(0,1,0));

			Quad windowq2(Vector3F(0,0,0),
				     Vector3F(0.7,0,0),
				     Vector3F(0.7,0.7,0),
				     Vector3F(0,0.7,0));

			Quad windowq3(Vector3F(0,0,0),
				     Vector3F(0.3,0,0),
				     Vector3F(0.3,0.7,0),
				     Vector3F(0,0.7,0));

			Quad windowq4(Vector3F(0,0,0),
				     Vector3F(0.7,0,0),
				     Vector3F(0.7,0.3,0),
				     Vector3F(0,0.3,0));

			Quad windowq5(Vector3F(0,0,0),
				     Vector3F(0.3,0,0),
				     Vector3F(0.3,1.3,0),
				     Vector3F(0,1.3,0));

			if (positionWindow(div[i], windowq) && (_type!= CAVE) && (_type!= CHURCH) )
			{
				Window window(windowq, Window::SQUARE);
				_mesh += window.generate();

				Table<Quad> doorWall = extract(div[i], windowq);
				_mesh += doorWall[0].generate();
				_mesh += doorWall[3].generate();
				_mesh += doorWall[1].generate();
				_mesh += doorWall[2].generate();
			}
			else if (positionWindow(div[i], windowqq) && (_type== CAVE) && (_type!= CHURCH)  )
			{
				Window window(windowqq, Window::CAVE_SQUARE);
				_mesh += window.generate();

				Table<Quad> doorWall = extract(div[i], windowqq);
				_mesh += doorWall[0].generate();
				_mesh += doorWall[3].generate();
				_mesh += doorWall[1].generate();
				_mesh += doorWall[2].generate();
			}
			else if (positionWindow(div[i], windowq2)  && (_type!= CHURCH) )
			{
				Window window(windowq2, Window::SMALL);
				_mesh += window.generate();

				Table<Quad> doorWall = extract(div[i], windowq2);
				_mesh += doorWall[0].generate();
				_mesh += doorWall[3].generate();
				_mesh += doorWall[1].generate();
				_mesh += doorWall[2].generate();
			}
			else if (positionWindow(div[i], windowq3)  && (_type!= CHURCH) )
			{
				Window window(windowq3, Window::KILL);
				_mesh += window.generate();

				Table<Quad> doorWall = extract(div[i], windowq3);
				_mesh += doorWall[0].generate();
				_mesh += doorWall[3].generate();
				_mesh += doorWall[1].generate();
				_mesh += doorWall[2].generate();
			}
			else if (positionWindow(div[i], windowq4)  && (_type!= CHURCH) )
			{
				Window window(windowq4, Window::KILL2);
				_mesh += window.generate();

				Table<Quad> doorWall = extract(div[i], windowq4);
				_mesh += doorWall[0].generate();
				_mesh += doorWall[3].generate();
				_mesh += doorWall[1].generate();
				_mesh += doorWall[2].generate();
			}
			else if (positionWindow(div[i], windowq5)  && (_type== CHURCH) )
			{
				Window window(windowq5, Window::KILL2);
				_mesh += window.generate();

				Table<Quad> doorWall = extract(div[i], windowq5);
				_mesh += doorWall[0].generate();
				_mesh += doorWall[3].generate();
				_mesh += doorWall[1].generate();
				_mesh += doorWall[2].generate();
			}
                        else
			{
				_mesh += div[i].generate();
			}
		}	

		std::cout << "End Generate Window Facade" << std::endl;
	
			/*
			Table<Quad> temp;
			Quad quad = div[i].rotateToY();

			temp = quad.cutLeft(1.f);
			WallE wallBottom = WallE(temp[0]);

			temp = temp[1].cutLeft(1.5f);		
			WallE wallTop = WallE(temp[1]);

			temp = temp[0].revertToX().cutLeft(windowWidth);

			Window window = Window(temp[0]);

			_mesh += window.generate();
			_mesh += wallBottom.generate();
			_mesh += wallTop.generate();
			*/

		return _mesh;
	}

}; // class WindowFacade

#endif // WINDOW_FACADE_H
