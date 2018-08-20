/*!*****************************************************************************
 *
 *       \file       :       Node.cpp
 *
 *       \author     :       Arnaud Emilien
 *       \date        :      28/02/10
 *
 *******************************************************************************/

#include <GL/glew.h>

#include "Node.h"
#include "Camera.h"
#include "Mesh.h"
#include <cmath>
#include <stdio.h>



Node::Node(const Vector3F& position, const Quaternion& orientation, const Vector3F& scale)
    : _position(position)
    , _orientation(orientation)
    , _scale(scale)
    , _forward(-Vector3F::ZAXIS)
    , _left(-Vector3F::XAXIS)
    , _father(NULL)
{
    _updateTransform();
    _updateLandmark();
}

Node::~Node()
{
}


Vector3F Node::getPosition() const
{
    return _position;
}

Vector3F Node::getPositionWorld() const
{
    if(_father == NULL)
    {
        return _father->getTransformWorld()*_position;
    }
    return _position;
}

Vector3F Node::getScale() const
{
    return _scale;
}

Vector3F Node::getScaleWorld() const
{
    if(_father == NULL)
    {
        /** TO DO */
    }
    return _scale;
}

Quaternion Node::getOrientation() const
{
    return _orientation;
}

Quaternion Node::getOrientationWorld() const
{
    if(_father == NULL)
    {
        /** TO DO */
    }
    return _orientation;
}

Vector3F Node::getForward() const
{
    return _forward;
}

Vector3F Node::getUp() const
{
    return _up;
}

Vector3F Node::getLeft() const
{
    return _left;
}

void Node::setPosition(const Vector3F& position)
{
    _position = position;

    _updateTransform();
    _updateLandmark();
}
void Node::setOrientation(const Quaternion& orientation)
{
    _orientation = orientation;

    _updateTransform();
    _updateLandmark();
}

void Node::translate(const Vector3F& move)
{
    _position += move;

    _updateTransform();
    _updateLandmark();
}

void Node::translate(float dx, float dy, float dz)
{
    _position += Vector3F(dx,dy,dz);

    _updateTransform();
    _updateLandmark();
}

void Node::rotate(float rx, float ry, float rz)
{
    rotate(Quaternion::createFromAxisAndAngle(rx,Vector3F::XAXIS));
    rotate(Quaternion::createFromAxisAndAngle(ry,Vector3F::YAXIS));
    rotate(Quaternion::createFromAxisAndAngle(rz,Vector3F::ZAXIS));
}

void Node::moveForward(float displacement)
{
    _position += _forward * displacement;

    _updateTransform();
    _updateLandmark();
}

void Node::moveLeft(float displacement)
{
    _position += _left * displacement;

    _updateTransform();
    _updateLandmark();
}

void Node::moveUp(float displacement)
{
    _position += _up * displacement;

    _updateTransform();
    _updateLandmark();
}

void Node::_updateTransform()
{
    _transform = TransformMatrix::translation(_position)
                 * TransformMatrix::scaling(_scale)
                 * TransformMatrix::rotation(_orientation);
}

void Node::_updateLandmark()
{
    _forward = normalize(TransformMatrix::rotation(_orientation) * Vector3F(0.0, 0.0, - 1.0));
    _left    = normalize(TransformMatrix::rotation(_orientation) * Vector3F(-1.0, 0.0, 0.0));
    _up      = crossProduct(_forward, _left);
}

void Node::rotateAroundForward(float angle)
{
    rotate(Quaternion::createFromAxisAndAngle(angle, _forward));
}

void Node::rotateAroundLeft(float angle)
{
    rotate(Quaternion::createFromAxisAndAngle(angle, _left));
}

void Node::rotateAroundUp(float angle)
{
    rotate(Quaternion::createFromAxisAndAngle(angle, _up));
}

void Node::rotateAroundAxis(float angle, const Vector3F& axis)
{
    rotate(Quaternion::createFromAxisAndAngle(angle, axis));
}

void Node::rotate(const Quaternion& rot)
{
    _orientation = rot * _orientation;

    _updateTransform();
    _updateLandmark();
}

void Node::setScale(const Vector3F& newScale)
{
    _scale = newScale;

    _updateTransform();
    _updateLandmark();
}

void Node::setScale(float sx, float sy, float sz)
{
    _scale = Vector3F(sx, sy, sz);

    _updateTransform();
    _updateLandmark();
}


/*
void Node::draw(SmartPointer<Camera> camera)
{

	// Bind the transformation matrix to the material to draw the object
	SmartPointer<Material> mat = RessourcesManager::getInstance()->getMaterial("TowerEngine_DefaultMaterial");
	SmartPointer<Shader> shader = mat->getShader();
	shader->bind();

	TransformMatrix model   = getTransformWorld();
	TransformMatrix modelViewMatrix  = camera->getViewMatrix() * model;
	NormalMatrix    normal = TransformMatrix::normal(modelViewMatrix);
	shader->linkMatrix4x4("_modelViewMatrix",  modelViewMatrix);
	shader->linkMatrix4x4("_inverseModelViewMatrix",  TransformMatrix::invert(modelViewMatrix));
	shader->linkMatrix4x4("_modelMatrix",      model);
	shader->linkMatrix4x4("_projectionMatrix", camera->getProjectionMatrix());
	shader->linkMatrix4x4("_viewMatrix",       camera->getViewMatrix());
	shader->linkMatrix3x3("_normalMatrix",     normal);



	SmartPointer<Mesh> mesh;

	// X
	shader->linkMatrix4x4("_modelViewMatrix",  modelViewMatrix
			      * TransformMatrix::translation(Vector3F(5.f, 0.f, 0.f)));
	mesh = Mesh::box(10.f, 0.5f, 0.5f);
	mesh->draw();
	shader->linkMatrix4x4("_modelViewMatrix",  modelViewMatrix
			      * TransformMatrix::translation(Vector3F(10.f, 0.f, 0.f)));
	mesh = Mesh::box(15.f, 0.2f, 0.2f);
	mesh->draw();

	// Y
	shader->linkMatrix4x4("_modelViewMatrix",  modelViewMatrix
			      * TransformMatrix::translation(Vector3F(0.f, 10.f, 0.f)));
	mesh = Mesh::box(0.5f, 15.f, 0.5f);
	mesh->draw();

	// Z
	shader->linkMatrix4x4("_modelViewMatrix",  modelViewMatrix
			      * TransformMatrix::translation(Vector3F(0.f, 0.f, 15.f)));
	mesh = Mesh::box(0.5f, 0.5f, 10.f);
	mesh->draw();


	shader->unbind();
}
*/

TransformMatrix Node::getTransform() const
{
    return _transform;
}

/*

SmartPointer<Node> Node::getFatherNode()
{
	return _father;
}

Error::Errors Node::addChildNode(SmartPointer<Node> node)
{
	if(node->_father.isNull())
	{
		_childs.insert(node);
		node->_father = SmartPointer<Node>(this);
	}
}
*/

TransformMatrix Node::getTransformWorld() const
{
    if(_father == NULL)
    {
        return _father->getTransformWorld() * _transform;
    }
    return _transform;
}

