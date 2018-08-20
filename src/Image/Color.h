/*******************************************************************************
 *
 *     Copyright : 2010 Arnaud Emilien
 *
 *******************************************************************************/


#ifndef TWR_COLOR_H
#define TWR_COLOR_H

#include <iostream>

class Color;
std::ostream& operator<<(std::ostream&,  Color color);


class Color{
public :
	enum FORMAT { 
		GRAY_8,
		GRAY_32,			       
		RGB_8,
		RGB_32,
		RGBA_8,
		RGBA_32,
	};
	
private :
	union ColorData{
		unsigned char gray8;

		float         gray32; 

		struct {unsigned char r;
			unsigned char g;
			unsigned char b;
			//unsigned char a;
		} rgba8;

		struct {float r;
			float g;
			float b;
			//float a;
		} rgba32;

	} _color;

	FORMAT _format;

public :
	Color(FORMAT format = RGB_8);

        void  setFormat(FORMAT format);
	inline FORMAT         getFormat() const { return _format;         }

	inline unsigned char& gray()            { return _color.gray8;    }
	inline float&         grayf()           { return _color.gray32;   }
	inline unsigned char& r()               { return _color.rgba8.r;  }
 	inline unsigned char& g()               { return _color.rgba8.g;  }
	inline unsigned char& b()               { return _color.rgba8.b;  }
	//inline unsigned char& a()               { return _color.rgba8.a;  }
	inline float&         rf()              { return _color.rgba32.r; }
	inline float&         gf()              { return _color.rgba32.g; }
	inline float&         bf()              { return _color.rgba32.b; }
	//inline float&         af()              { return _color.rgba32.a; }

	static Color GRAY(unsigned int gray);
	static Color GRAYF(float gray);
	static Color RGBX(unsigned int r, unsigned int g, unsigned int b);
	static Color RGBA(unsigned int r, unsigned int g, unsigned int b, unsigned int a);
	static Color RGBF(float r, float g, float b);
	static Color RGBAF(float r, float g, float b, float a);

	friend std::ostream& operator<<(std::ostream&,  Color color);

}; // class Color


#endif // TWR_COLOR_H
