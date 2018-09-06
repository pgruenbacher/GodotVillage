/*******************************************************************************
 *
 *     Copyright : 2010 Arnaud Emilien
 *
 *******************************************************************************/

#ifndef TWR_IMAGE_H
#define TWR_IMAGE_H

#include "../Maths/Vector.h"
#include "Color.h"
#include <iostream>
#include <string>

class Image {
private:
	Color *_data;
	Color::FORMAT _format;
	unsigned int _width;
	unsigned int _height;

	void _empty();

	inline bool _validFormat(Color::FORMAT format) const {
		return _format == format;
	}

public:
	Image(unsigned int width,
			unsigned int height,
			Color::FORMAT format);

	Image(const Image &image);

	~Image();

	static void convertToGLFormat(Image *image);

	inline char *getData() { return (char *)_data; }

	bool load(std::string name);
	inline unsigned int getWidth() const { return _width; }
	inline unsigned int getHeight() const { return _height; }
	inline Color::FORMAT getFormat() const { return _format; }

	void setAt(unsigned int column, unsigned int line, const Color &color);
	Color getAt(unsigned int column, unsigned int line);
	Color getAt(Vector2I pos) { return getAt(pos.x, pos.y); }
	void setAt(Vector2I pos, const Color &color) { setAt(pos.x, pos.y, color); }
	void fill(const Color &color);
	Image &operator=(const Image &image);

	bool validCoords(Vector2I pos) { return validCoords(pos.x, pos.y); }

	inline bool validCoords(unsigned int col, unsigned int line) const {
		if (((line < _height) && (col < _width)))
			return true;
		return false;
	}

}; // class Image

#endif // TWR_IMAGE_H
