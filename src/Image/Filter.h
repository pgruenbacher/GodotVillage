/*******************************************************************************
 *
 *     Copyright : 2010 Arnaud Emilien
 *
 *******************************************************************************/

#ifndef FILTER_H
#define FILTER_H

#include <QImage>

class Filter {

public:
	virtual QImage operator()(const QImage &image) = 0;

}; // class Filter

#endif // FILTER_H
