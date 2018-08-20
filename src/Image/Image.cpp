/*******************************************************************************
 *
 *     Copyright : 2010 Arnaud Emilien
 *
 *******************************************************************************/

 
#include "Image.h"
#include <QImage>
#include <QColor>
#include <iostream>

#define AT(col, line) (((line)*_width)+ col)

void Image::_empty()
{
	if (_data)
	{
		delete[] _data;
		std::cout << "Delete image data" << std::endl;
	}
	_data = 0;
}

bool Image::load(std::string name)
{        
        QImage image;
        if (!image.load(name.c_str()))
        {
                std::cout << "Failed to load " << name << std::endl;
                return false;
        }
                
        _empty();
        _width  = image.width();
        _height = image.height();
        _format = Color::RGB_8;
        _data   = new Color[_width*_height];
        
        Color color(_format);
        QColor col;
        for (unsigned int i = 0; i < image.height(); ++i)
        {
                for (unsigned int j = 0; j < image.width(); ++j)
                {
                        col = image.pixel(j,i);
                        color.r() = float(col.red());
                        color.g() = float(col.green());
                        color.b() = float(col.blue());
                        setAt(j, i,color);
                }
        }
        return true;
}
	
void Image::convertToGLFormat(Image* image)
{
        switch (image->getFormat())
        {
                case Color::RGB_32 :
                {
                        float* data;
                        data = (float*) new Color[image->_width*image->_height];
                        for (unsigned int i = 0; i < image->_width*image->_height; ++i)
                        {
                                data[i*3+0] = image->_data[i].rf();
                                data[i*3+1] = image->_data[i].gf();
                                data[i*3+2] = image->_data[i].bf();
                        }
                        image->_empty();
                        image->_data = (Color*)data;
                }
                break;
                
                default :
                {
                        unsigned char* data;
                        data = (unsigned char*) new Color[image->_width*image->_height];
                        for (unsigned int i = 0; i < image->_width*image->_height; ++i)
                        {
                                data[i*3+0] = image->_data[i].r();
                                data[i*3+1] = image->_data[i].g();
                                data[i*3+2] = image->_data[i].b();
                        }
                        image->_empty();
                        image->_data = (Color*)data;
                }
		break;
        }          
}
	
Image::Image(unsigned int width, 
	     unsigned int height, 
	     Color::FORMAT   format)
	: _format(format)
	, _width(width)
	, _height(height)
	, _data(0)
{
	_data = new Color[_width*_height];
        for (unsigned int i = 0; i <_width*_height; ++i)
        {
                _data[i].setFormat(_format);
        }
}

Image::Image(const Image& image)
: _data(0)
{
	*this = image;
}

Image& Image::operator=(const Image& image)
{
        if (&image == this)
                return *this;
	_empty();
	_format = image._format;
	_width  = image._width;
	_height = image._height;
	_data   = new Color[_width*_height];
	for (unsigned int i = 0; i < _width*_height; ++i)
	{
		_data[i] = image._data[i];
	}
	
        return *this;
}
	
Image::~Image()
{
	_empty();
}
	
void Image::setAt(unsigned int column, unsigned int line, const Color& color)
{
	if (! _validFormat(color.getFormat()))
	{
	        std::cout << "ERROR setAt : Invalid format." << std::endl;
		return;
	}
	
	if (! validCoords(column, line))
	{
	        std::cout << "ERROR setAt : Invalid Coords." << std::endl;
		return;
	}

        //std::cout << "Set : " << " at " << line << " " << column << " : " << _data[AT(line,column)].rf() << std::endl;
	_data[AT(column, line)] = color;
	return;
}

Color Image::getAt(unsigned int column, unsigned int line)
{		
	if (! validCoords(column, line))
	{
	        std::cout << "ERROR getAt : Invalid Coords." << std::endl;
		return Color();
	}
	Color col = _data[AT(column, line)];
        //std::cout << "Get : " << " at " << line << " " << column << " : " << col.rf() << std::endl;
	return col;
}

void Image::fill(const Color& color)
{
	if (! _validFormat(color.getFormat()))
	{
	        std::cout << "ERROR setAt : Invalid format." << std::endl;
		return;
	}

	for (unsigned int i = 0; i < _width*_height; ++i)
	{
		_data[i] = color;
	}
}

