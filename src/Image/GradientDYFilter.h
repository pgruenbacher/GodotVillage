/*******************************************************************************
 *
 *     Copyright : 2010 Arnaud Emilien
 *
 *******************************************************************************/

#ifndef _GRADIENT_DY_FILTER_H
#define _GRADIENT_DY_FILTER_H

#include "Filter.h"

class GradientDYFilter : public Filter {

public:
	QImage operator()(const QImage &image) {
		QImage result(image);

		for (unsigned int h = 0; h < image.height() - 1; ++h) {
			for (unsigned int w = 0; w < image.width(); ++w) {
				result.setPixel(w, h, image.pixel(w, h) - image.pixel(w, h + 1));
			}
		}

		return result;
	}

}; // class GradientDYFilter

#endif // _GRADIENT_DY_FILTER_H
