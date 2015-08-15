//
//  Camera.cpp
//  Bloxelizer
//
//  Created by The Drudgerist on 5/18/13.
//  Copyright (c) 2013 The Drudgerist. All rights reserved.
//

#include "Camera.h"
#include "Locator.h"
#include "Renderer.h"

#include <glm/gtx/rotate_vector.hpp>

#include "MathUtils.h"
#include "Random.h"
#include "Timer.h"

Camera::Camera() {
    fieldOfView = 45.0f;
    nearDepth = 0.01f;
    farDepth = 128.0f;
    focalDepth = 6.0;
    focalLength = 30.0;
    fStop = 10.0f;
    exposure = 4.0f;
    debugLens = false;
    autoFocus = false;
    
    shakeVect = glm::vec3();
    shakeAmount = 0.0f;
    shakeDecay = 0.85f;
    
    position = glm::vec3(0.0f,0.0f,0.0f);
    rotation = glm::vec3(0.0f,0.0f,0.0f);
    rotationSensitivity = glm::vec2(20.0f,20.0f);
    speed = glm::vec3(0.0f, 0.0f, 0.0f);
    movement = glm::vec3(0.0f,0.0f,0.0f);
    movementSpeedFactor = 1.0f;
    
    targetPosition = glm::vec3(0.0f,0.0f,0.0f);
    targetRotation = glm::vec3(0.0f,0.0f,0.0f);
    elasticMovement = false;
    autoRotate = false;
    thirdPerson = false;
    physicsClip = true;
    elasticity = 20.0f;
    distance = 6.0f;
    maxDistance = 20.0f;
    height = 2.0f;
}
Camera::~Camera() {
    
}
void Camera::Update( double delta ) {
    if ( delta <= 0.0 ) return;
    if ( delta >= 1.0 ) return;

    if ( shakeVect.x != 0.0f || shakeVect.y != 0.0f ) {
        position -= shakeVect;
    }
    if ( elasticMovement ) {
        if ( targetRotation.y - rotation.y < -180.0f ) targetRotation.y += 360.0f;
        else if ( targetRotation.y - rotation.y > 180.0f ) targetRotation.y -= 360.0f;

        // Dampen rotation
        float new_ratio = elasticity*0.02f * delta;
        float old_ratio = 1.0 - new_ratio;
        glm::vec3 new_dir = (rotation * old_ratio) + (targetRotation * new_ratio);
        rotation = new_dir;
        
        if ( glm::length(movement) > 0.0f ) {
            // Apply direct camera movement
            CalculateCameraMovement( movement );
            targetPosition += speed*float(delta);
        }
        if ( thirdPerson ) {
            // Find coordinate for camera
            glm::vec3 zVect = glm::vec3(0.0f,0.0f,1.0f);
            glm::vec3 zPos = zVect*distance;
            zPos = glm::rotateX(zPos, rotation.x);
            zPos = glm::rotateY(zPos, rotation.y);
            zPos = glm::rotateZ(zPos, -rotation.z);
            glm::vec3 behindTarget = targetPosition + zPos;
            behindTarget.y += height;
            glm::vec3 move = behindTarget - position;
            float dist = glm::length(move);
            if ( dist > 20.0f ) {
                move = glm::normalize(move)*20.0f;
            }
            speed = move*(elasticity*0.1f)*float(delta);
            
            if ( physicsClip ) {
                Locator::getPhysics().CameraCollisions(*this);
            } else {
                position = position+speed;
            }
        } else {
            glm::vec3 move = targetPosition - position;
            speed = move*elasticity*0.1f*float(delta);
            position = position+speed;
        }
    } else {
        CalculateCameraMovement( movement );
        rotation = targetRotation;
        position = position+speed*float(delta);
    }
    if ( shakeAmount != 0.0f ) {
        Random::RandomSeed(int(Timer::Microseconds()));
        int rX = Random::RandomInt(-1, 1);
        int rY = Random::RandomInt(-1, 1);
        shakeVect = glm::vec3(rX, rY, 0.0f)*shakeAmount;
        position += shakeVect;
        shakeAmount *= shakeDecay;
        if ( fabsf(shakeAmount) < 0.01f ) { shakeAmount = 0.0f; shakeVect = glm::vec3(); }
    }
    UpdateMatrices();
//    printf("Camera: %f,%f,%f - %f,%f,%f\n", position.x, position.y, position.z, rotation.x, rotation.y, rotation.z);
}
// Function to deal with mouse position changes, called whenever the mouse cursor moves
void Camera::CameraRotate( const float rotX, const float rotY ) {

    rotation.x += rotY;
    rotation.y += rotX;
    
    // Control looking up and down with the mouse forward/back movement
    MathUtils::Clamp((double)rotation.x, (double)-M_PI_2, (double)M_PI_2);
    
    // Looking left and right. Keep the angles in the range -180.0f (anticlockwise turn looking behind) to 180.0f (clockwise turn looking behind)
    if (rotation.y < -M_PI) {
        rotation.y += 2*M_PI;
    }
    if (rotation.y > M_PI) {
        rotation.y -= 2*M_PI;
    }
    targetRotation = rotation;
}

// Function to calculate which direction we need to move the camera and by what amount
void Camera::CalculateCameraMovement(const glm::vec3& direction) {
    // Break up our movement into components along the X, Y and Z axis
    float camMovementXComponent = 0.0f;
    float camMovementYComponent = 0.0f;
    float camMovementZComponent = 0.0f;
    // Forward/backward movement
    if (direction.z != 0.0f) {
        // Control X-Axis movement
        float pitchFactor = cos(rotation.x);
        camMovementXComponent += ( movementSpeedFactor * float(sin((rotation.y))) * direction.z ) * pitchFactor;
        // Control Y-Axis movement
        camMovementYComponent += movementSpeedFactor * float(sin((rotation.x))* -direction.z );
        // Control Z-Axis movement
        float yawFactor = (cos((rotation.x)));
        camMovementZComponent += ( movementSpeedFactor * float(cos((rotation.y))) * direction.z ) * yawFactor;
    }
    // Strafing
    if ( direction.x !=  0.0f ) {
        // Calculate our Y-Axis rotation in radians once here because we use it twice
        float yRotRad = (rotation.y);
        camMovementXComponent +=  movementSpeedFactor * float(cos(yRotRad)) * direction.x;
        camMovementZComponent +=  movementSpeedFactor * float(sin(yRotRad)) * -direction.x;
    }
    // Vertical movement
    if ( direction.y !=  0.0f ) {
        float xRotRad = MathUtils::ToRadians(rotation.x);
        float yRotRad = MathUtils::ToRadians(rotation.y);
        camMovementYComponent +=  movementSpeedFactor * float(cos(xRotRad)) * -direction.y;
        camMovementXComponent +=  movementSpeedFactor * float(sin(yRotRad)) * direction.y;
        camMovementZComponent +=  movementSpeedFactor * float(cos(yRotRad)) * direction.y;
    }
    // After combining our movements for any & all keys pressed, assign them to our camera speed along the given axis
    speed.x = camMovementXComponent;
    speed.y = camMovementYComponent;
    speed.z = camMovementZComponent;
    
    // Cap the speeds to our movementSpeedFactor (otherwise going forward and strafing at an angle is twice as fast as just going forward!)
    MathUtils::Clamp(speed.x, -movementSpeedFactor, movementSpeedFactor);
    MathUtils::Clamp(speed.y, -movementSpeedFactor, movementSpeedFactor);
    MathUtils::Clamp(speed.z, -movementSpeedFactor, movementSpeedFactor);
}

// Function to move the camera the amount we've calculated in the calculateCameraMovement function
void Camera::MoveCamera( double dt ) {
    position += speed*(float)dt;
}

void Camera::UpdateMatrices()
{
    glm::ivec2 windowSize = Locator::getRenderer().GetWindowSize();
    GLfloat aspectRatio = (windowSize.x > windowSize.y) ?
    GLfloat(windowSize.x)/GLfloat(windowSize.y) : GLfloat(windowSize.y)/GLfloat(windowSize.x);

    _model = glm::mat4();
    _model = glm::rotate(_model, -rotation.x, glm::vec3(1.0, 0.0, 0.0));
    _model = glm::rotate(_model, -rotation.y, glm::vec3(0.0, 1.0, 0.0));
    _model = glm::rotate(_model, -rotation.z, glm::vec3(0.0, 0.0, 1.0));
    _model = glm::translate(_model, glm::vec3(-position.x, -position.y, -position.z));
    
    _projection = glm::mat4();
    _projection = glm::perspective(fieldOfView, aspectRatio, nearDepth, farDepth);
    _viewport = glm::vec4(0, 0, windowSize.x, windowSize.y);
    
    _mvp = _projection*_model;
}
