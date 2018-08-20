/*******************************************************************************
 *
 *     Copyright : 2010 Arnaud Emilien
 *
 *******************************************************************************/

#ifndef GAUSSIAN_BLUR_FILTER_H
#define GAUSSIAN_BLUR_FILTER_H

#include "Filter.h"
#include <iostream>
#include <QColor>

class GaussianBlurFilter : public Filter{
private :
        int _xsize;
        int _ysize;
        
public :
        static Table<float> computeGaussianPyramid(int x, int intervall = 4)
        {
                //std::cout << "pyramid size " << x << std::endl;
                Table<float> result;
                for (int i = -x * intervall; i <= x *intervall; ++i)
                {
			double w = exp(-double(i*i) / (double(2.0*x*x))) / (sqrt(2.0 * M_PI) * double(x));
			
                        result.pushLast(w);
                        //std::cout << " pyramid " << i << " : " << result[i+x] << std::endl;
                }
                return result;
        }

private :
        
        void applyX(const QImage& source, QImage* image)
        {
                std::cout << "image " << image->width() << " " << image->height() << std::endl;
                Table<float> pyramid = computeGaussianPyramid(_xsize);
                Vector3F col;
                Vector3F color;
                for (int h = 0; h < source.height(); ++h)
                {
                        for (int w = 0; w < source.width(); ++w)
                        {
				color = Vector3F(0,0,0);
                                for (int i = -_xsize; i <= _xsize; ++i)
                                {
                                        if (w+i >= 0 && w+i < source.width())
                                        {
                                                col.x = QColor(source.pixel(w+i,h)).red();
                                                col.y = QColor(source.pixel(w+i,h)).blue();
                                                col.z = QColor(source.pixel(w+i,h)).green();
                                                
                                                color += pyramid[i + _xsize] * col;
                                        }
                                }             
                                //std::cout << "image setpixel " << w << " " << h << " : " << color << std::endl; 
                                image->setPixel(w,h, qRgb(color.x, color.y, color.z));
                        }
                }
        }        
        
        void applyY(const QImage& source, QImage* image)
        {
        
                std::cout << "applyY " << _ysize << std::endl;
                
                Table<float> pyramid = computeGaussianPyramid(_ysize);
                //std::cout << "pyramid size " << pyramid.getSize() << std::endl;
                Vector3F col;
                Vector3F color;
                for (int h = 0; h < source.height(); ++h)
                {
			// std::cout << "h " << h << std::endl;
                        for (int w = 0; w < source.width(); ++w)
                        {                 
				color = Vector3F(0,0,0);
                                for (int i = - _ysize; i <= _ysize; ++i)
                                {         
                                                                               
                                        if (h+i >= 0 && h+i < source.height())
                                        {
                                                col.x = QColor(source.pixel(w,h+i)).red();
                                                col.y = QColor(source.pixel(w,h+i)).blue();
                                                col.z = QColor(source.pixel(w,h+i)).green();
                                                //std::cout << "image col " << col << std::endl; 
                                                //std::cout << "i + _ysize " << i + _ysize << std::endl; 
                                                color += pyramid[i + _ysize] * col;
                                                //color += col;
                                        }
                                }                
                                //std::cout << "image setpixel " << w << " " << h << " : " << color << std::endl;
                                image->setPixel(w,h, qRgb(color.x, color.y, color.z));
                        }
                }
        }
        
public :

        GaussianBlurFilter(unsigned int xsize, unsigned int ysize)
        : _xsize(xsize)
        , _ysize(ysize)
        {
        }
        
        QImage operator()(const QImage& image)
        {
                QImage result(image);
                QImage result1(image);
                if (_xsize)
                        applyX(image, &result1);
                if (_ysize)
                        applyY(result1, &result);
                return result;
        }
   


}; // class GaussianBlurFilter

#endif // GAUSSIAN_BLUR_FILTER_H
