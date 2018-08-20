/*******************************************************************************
 *
 *     Copyright : 2010 Arnaud Emilien
 *
 *******************************************************************************/

#ifndef _GRADIENT_DX_FILTER_H
#define _GRADIENT_DX_FILTER_H

#include "Filter.h"

class GradientDXFilter : public Filter{

public :
        QImage operator()(const QImage& image)
        {
                QImage result(image);
                        
                for (unsigned int h = 0; h < image.height(); ++h)
                {
                        for (unsigned int w = 0; w < image.width() - 1; ++w)
                        {
                                result.setPixel(w,h, image.pixel(w,h) - image.pixel(w+1,h));
                        }
                }
                
                return result;
        }

}; // class GradientDXFilter

#endif // _GRADIENT_DX_FILTER_H
