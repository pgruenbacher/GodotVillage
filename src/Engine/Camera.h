#ifndef _Camera_H
#define _Camera_H

/*******************************************************************************
*
*     Camera.h
*
*     Auteur      :     Arnaud Emilien
*     Historique  :
*
*******************************************************************************/

#include "Node.h"
#include "../Maths/Vector.h"
#include "../Maths/TransformationMatrix.h"

class Camera
{
private :
    /** The scene node associated to the camera. */
    Node*    _node;

    /** The angle of view of the camera. */
    float    _viewAngle;

    /** Ratio equal to width/height. */
    float    _ratio;

    /** The minimum distance to draw objects. */
    float    _nearDistance;

    /** The maximum distance to draw objects. */
    float    _farDistance;

    TransformMatrix     _viewMatrix;
    TransformMatrix     _projectionMatrix;

public :
    /** Default constructor. */
    Camera( Vector3F position     = Vector3F::ZERO,
            Quaternion quat       = Quaternion::ZERO,
            float    viewAngle    = 45.f,
            float    ratio        = 10.0 / 9.0,
            float    nearDistance = 1.0f,
            float    farDistance  = 1000.f);


    /** Return the associated node.*/
    Node* getNode();

    void updateTransformMatrix();

    /** Return the projection matrix of the camera. */
    TransformMatrix getProjectionMatrix();

    /** Return the view matrix of the camera. */
    TransformMatrix getViewMatrix();

    /** Return the target of the camera. */
    Vector3F getTarget () const ;

    /** Return the view angle. */
    float getViewAngle() const;

    /** Return the near distance. */
    int getNearDistance() const;

    /** Return the far distance. */
    int getFarDistance() const;

    /** Set the view angle of the camera. */
    void setViewAngle(float viewAngle);

    /** Set the near distance of the camera. */
    void setNearDistance(float near);

    /** Set the far distance of the camera. */
    void setFarDistance(float far);


    void setRatio(float ratio);


}; // Camera

#endif
