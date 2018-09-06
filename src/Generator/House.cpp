/*******************************************************************************
 *
 *     Copyright : 2010 Arnaud Emilien
 *
 *******************************************************************************/

#include "House.h"

std::ostream &operator<<(std::ostream &o, const House &house) {
	Table<Vector2I> border = house.getBorder();
	for (unsigned int i = 0; i < border.getSize(); ++i) {
		o << border[i] << " ,  ";
	}
	return o;
}
