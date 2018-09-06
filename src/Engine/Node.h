#ifndef _Node_H
#define _Node_H

/*!*****************************************************************************
*
*       \file       :       Node.h
*
*       \author     :       Arnaud Emilien
*       \date        :       28/02/10
*
*******************************************************************************/

#include "../Maths/TransformationMatrix.h"
#include "../Maths/Vector.h"
#include "../Utils/Table.h"

// PREDECLARATION
class Camera;

class Node {

protected:
	/** The position of the Node.
        The position is relative to the father Node position.
    */
	Vector3F _position;

	/** The orientation of the Node.
        The orientation is relative to the father Node orientation.
    */
	Quaternion _orientation;

	/** The scale of the Node.
        The scale is relative to the father Node scale.
    */
	Vector3F _scale;

	/** Local Landmark. */
	Vector3F _forward;
	Vector3F _up;
	Vector3F _left;

	/** The node transformMatrix. */
	TransformMatrix _transform;

	/** The Father of this node. */
	Node *_father;

	/** List of the child nodes. */
	Table<Node *> _childs;

private:
	/** Update the transform matrix of the Node. */
	void _updateTransform();

	/** Update the local Landmark of the Node. */
	void _updateLandmark();

public:
	/** Default constructor.
        @param the initial position of the Node.
        @param the initial Eulerian angles of the Node.
        @param the initial scale of the Node.
    */
	Node(const Vector3F &position = Vector3F::ZERO,
			const Quaternion &orientation = Quaternion::ZERO,
			const Vector3F &scale = Vector3F::UNIT);

	/** Node destructor. */
	virtual ~Node();

	/** Translate the node.
        @param the displacement to do.
    */
	void translate(const Vector3F &displacement);

	/** Translate the node.
        @param the displacement to do on X axis.
        @param the displacement to do on Y axis.
        @param the displacement to do on Z axis.
    */
	void translate(float dx, float dy, float dz);

	/** Translate the in the local landmark.
        @param the displacement to on the do on the Forward axis.
    */
	void moveForward(float displacement);

	/** Translate the in the local landmark.
        @param the displacement to on the do on the Left axis.
    */
	void moveLeft(float displacement);

	/** Translate the in the local landmark.
        @param the displacement to on the do on the Up axis.
    */
	void moveUp(float displacement);

	/** Rotate the node.
        @param the quaternion of the rotation.
    */
	void rotate(const Quaternion &rotation);

	void rotateAroundForward(float angle);
	void rotateAroundLeft(float angle);
	void rotateAroundUp(float angle);

	void rotateAroundAxis(float angle, const Vector3F &axis);

	/** Rotate the node.
        @param the rotation around X axis.
        @param the rotation around Y axis.
        @param the rotation around Z axis.
    */
	void rotate(float rx, float ry, float rz);

	/** Change the scale of the node.
        @param the new scale.
    */
	void setScale(const Vector3F &newScale);

	/** Change the scale of the node.
        @param the new X scale.
        @param the new Y scale.
        @param the new Z scale.
    */
	void setScale(float rx, float ry, float rz);

	/** Return the position of the Node.
        The data is relative to the father Node.
        @return the position of the Node.
    */
	Vector3F getPosition() const;

	/** Return the scale of the Node.
        The data is relative to the father Node.
        @return the scale of the Node.
    */
	Vector3F getScale() const;

	/** Return the orientation of the Node.
        The data is relative to the father Node.
        @return the position of the Node.
    */
	Quaternion getOrientation() const;

	/** Return a vector from the local landmark.
        The data is relative to the father Node.
        @return the forward vector.
    */
	Vector3F getForward() const;

	/** Return a vector from the local landmark.
        The data is relative to the father Node.
        @return the left vector.
    */
	Vector3F getLeft() const;

	/** Return a vector from the local landmark.
        The data is relative to the father Node.
        @return the up vector.
    */
	Vector3F getUp() const;

	/** Return the position of the Node.
        The data is relative to the father Node.
        @return the position of the Node.
    */
	Vector3F getPositionWorld() const;

	/** Return the scale of the Node.
        The data is given in the world landmark.
        @return the position of the Node.
    */
	Vector3F getScaleWorld() const;

	/** Return the orientation of the Node.
        The data is given in the world landmark.
        @return the scale of the Node.
    */
	Quaternion getOrientationWorld() const;

	/** Return a vector from the local landmark.
        The data is given in the world landmark.
        @return the forward vector.
    */
	Vector3F getForwardWorld() const;

	/** Return a vector from the local landmark.
        The data is given in the world landmark.
        @return the left vector.
    */
	Vector3F getLeftWorld() const;

	/** Return a vector from the local landmark.
        The data is given in the world landmark.
        @return the up vector.
    */
	Vector3F getUpWorld() const;

	/***/
	Vector3F getDirection() const;

	/** Set the position of the Node.
        @param the new position.
    */
	void setPosition(const Vector3F &position);

	/** Set the orientation of the Node.
        @param the new orientation.
    */
	void setOrientation(const Quaternion &orientation);

	/** Draw the node on screen. */
	//void draw(SmartPointer<Camera> camera);

	/** Get the transformation matrix.
        The data is relative to the father Node.
        @return the transformation matrix.
    */
	TransformMatrix getTransform() const;

	/** Get the transformation matrix.
        The data is given in the world landmark.
        @return the transformation matrix.
    */
	TransformMatrix getTransformWorld() const;

	/** Add a Node to the child list.
        @param the Node to add in the child list.
        @return the error message, if any.
    */
	//Error::Errors addChildNode(SmartPointer<Node> node);

	/** Remove a Node from the child list.
        @param the Node to remove from the child list.
        @return the error message, if any.
    */
	//Error::Errors removeChildNode(SmartPointer<Node> node);

	/** Return the father Node of this Node.
        @return the father Node.
    */
	//SmartPointer<Node> getFatherNode();

}; // class Node

#endif //_Node_H
