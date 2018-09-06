/*******************************************************************************
 *
 *     Copyright : 2010 Arnaud Emilien
 *
 *******************************************************************************/

#ifndef _LAYOUT_H
#define _LAYOUT_H

#include <iostream>

template <class C, class K, class V>
class Layout {
private:
	unsigned int _width;
	unsigned int _height;

public:
	Layout(unsigned int width,
			unsigned int height)
			: _width(width), _height(height) {
	}

	virtual C getValueAt(K key) = 0;

}; // class Layout

#endif // _LAYOUT_H
