/*******************************************************************************
 *
 *     Camera.cpp
 *
 *     Auteur      :     Arnaud Emilien
 *     Historique  :     04/02/10
 *
 *******************************************************************************/

#include "Camera.h"
#include "../Maths/TransformationMatrix.h"
#include <math.h>
#include <iostream>


Camera::Camera(Vector3F position,
               Quaternion orientation,
               float viewAngle,
               float ratio,
               float near,
               float far)
    : _node(new Node(position, orientation))
    , _viewAngle(viewAngle)
    , _ratio(ratio)
    , _nearDistance(near)
    , _farDistance(far)
{
}

void Camera::updateTransformMatrix()
{
    // Update the view matrix
    /*
    _viewMatrix = TransformMatrix::rotation(- (_node->getOrientationWorld()))
    * TransformMatrix::translation(- (_node->getPositionWorld()));
    */
    _viewMatrix = TransformMatrix::invert(_node->getTransformWorld());

    // Update the projection matrix
    _projectionMatrix =  TransformMatrix::projection(_nearDistance,
                         _farDistance,
                         _viewAngle,
                         _ratio);
}

TransformMatrix Camera::getViewMatrix()
{
    return _viewMatrix;
}

TransformMatrix Camera::getProjectionMatrix()
{
    return _projectionMatrix;
}

Node* Camera::getNode()
{
    return _node;
}

Vector3F Camera::getTarget() const
{
    return _node->getPosition()+_node->getForward();
}

float Camera::getViewAngle() const
{
    return _viewAngle;
}

int Camera::getNearDistance() const
{
    return _nearDistance;
}

int Camera::getFarDistance() const
{
    return _farDistance;
}

void Camera::setViewAngle(float angleView)
{
    _viewAngle = angleView;
}

void Camera::setNearDistance(float near)
{
    _nearDistance = near;
}

void Camera::setFarDistance(float far)
{
    _farDistance = far;
}

void Camera::setRatio(float ratio)
{
    _ratio = ratio;
}


