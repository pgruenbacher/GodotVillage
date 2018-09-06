/*******************************************************************************
 *
 *     Copyright : 2010 Arnaud Emilien
 *
 *******************************************************************************/

#include "Color.h"

Color::Color(FORMAT format) {
	setFormat(format);
}

void Color::setFormat(FORMAT format) {
	_format = format;
}

Color Color::GRAY(unsigned int gray) {
	Color color;
	color._format = GRAY_8;
	color._color.gray8 = gray;
	return color;
}

Color Color::GRAYF(float gray) {
	Color color;
	color._format = GRAY_32;
	color._color.gray32 = gray;
	return color;
}

Color Color::RGBX(unsigned int r, unsigned int g, unsigned int b) {
	Color color;
	color._format = RGB_8;
	color._color.rgba8.r = r;
	color._color.rgba8.g = g;
	color._color.rgba8.b = b;
	return color;
}

Color Color::RGBA(unsigned int r, unsigned int g, unsigned int b, unsigned int a) {
	Color color;
	color._format = RGBA_8;
	color._color.rgba8.r = r;
	color._color.rgba8.g = g;
	color._color.rgba8.b = b;
	//	color._color.rgba8.a = a;
	return color;
}

Color Color::RGBF(float r, float g, float b) {
	Color color;
	color._format = RGB_32;
	color._color.rgba32.r = r;
	color._color.rgba32.g = g;
	color._color.rgba32.b = b;
	return color;
}

Color Color::RGBAF(float r, float g, float b, float a) {
	Color color;
	color._format = RGBA_32;
	color._color.rgba32.r = r;
	color._color.rgba32.g = g;
	color._color.rgba32.b = b;
	//	color._color.rgba32.a = a;
	return color;
}

std::ostream &operator<<(std::ostream &o, Color color) {
	switch (color.getFormat()) {
		case Color::RGB_8:
			o << (float)color.r()
			  << ", "
			  << (float)color.g()
			  << ", "
			  << (float)color.b()
			  << std::endl;
			break;
		case Color::RGB_32:
			o << (float)color.rf()
			  << ", "
			  << (float)color.gf()
			  << ", "
			  << (float)color.bf()
			  << std::endl;
			break;
	}
	return o;
}
