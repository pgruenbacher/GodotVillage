/*******************************************************************************
 *
 *     Copyright : 2010 Arnaud Emilien
 *
 *******************************************************************************/

#ifndef ITERATOR_H
#define ITERATOR_H

/** Template base class for iterators. */
template <class _Iterator_Value_>
class IteratorBase {
protected:
	// protected constructor
	IteratorBase() {}
public:
	/** Does the iterator have a next element ? 
	    @return the iteration has a next element.
	*/
	virtual bool hasNext() = 0;
	
	/** Jump to the next element, if possible. 
	    @return the value of the element.*/
	virtual _Iterator_Value_& next() = 0;
	
	/** Return the value of last jumped over element.
	    Node: You need to call next() once before calling value().
	    @return the value of the element. 
	*/
	virtual _Iterator_Value_& value() = 0;  
};

#endif // ITERATOR_H
