/*******************************************************************************
 *
 *     Copyright : 2010 Arnaud Emilien
 *
 *******************************************************************************/

#ifndef _IMAGE_LAYOUT_H
#define _IMAGE_LAYOUT_H

#include "../Image/Color.h"
#include "../Image/Image.h"
#include "Layout.h"

template <class C, class K, class V>
class ImageLayout : public Layout<C, K, V> {
protected:
	Image *_image;
	bool _contains;

public:
	ImageLayout(Image *image, bool contains = false)
			: Layout<C, K, V>(image->getWidth(), image->getHeight()), _image(image), _contains(contains) {
	}

	~ImageLayout() {
		if (_contains)
			delete _image;
	}

	Image *getImage() {
		return _image;
	}

	C getValueAt(K key) {
		// Check the Validity of the Key
		if (!_image->validCoords(key.x, key.y)) {
			return C();
		}

		switch (_image->getFormat()) {
			case Color::RGB_32:
				return C(Color(_image->getAt(key.x, key.y)).rf());
				break;
			default:
				return C(Color(_image->getAt(key.x, key.y)).r());
				break;
		}
	}

}; // class ImageLayout

#endif // _IMAGE_LAYOUT_H
