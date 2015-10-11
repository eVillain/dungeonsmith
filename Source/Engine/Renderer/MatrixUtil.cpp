//
//  MatrixUtil.cpp
//  DungeonSmith
//
//  Created by The Drudgerist on 17/05/15.
//  Copyright (c) 2015 The Drudgerist. All rights reserved.
//

#include "MatrixUtil.h"
#include "Camera.h"
#include <glm/gtc/matrix_transform.hpp>

// Matrix functionality
void MatrixUtil::GetUIMatrix(glm::mat4& target,
                             const float width,
                             const float height,
                             const float nearDepth,
                             const float farDepth)
{
    // 2D projection with origin (0,0) at center of window
    float hw = width*0.5f;
    float hh = height*0.5f;
    target = glm::ortho(-hw, hw, -hh, hh, nearDepth, farDepth);
}

void MatrixUtil::GetGameMatrix(glm::mat4 &target,
                               const float width,
                               const float height,
                               const Camera& camera ) {
    GLfloat aspectRatio = (width > height) ? width/height : height/width;
    target = glm::perspective(camera.fieldOfView, aspectRatio, camera.nearDepth, camera.farDepth);
    target = glm::rotate(target, -camera.rotation.x, glm::vec3(1.0, 0.0, 0.0));
    target = glm::rotate(target, -camera.rotation.y, glm::vec3(0.0, 1.0, 0.0));
    target = glm::rotate(target, -camera.rotation.z, glm::vec3(0.0, 0.0, 1.0));
    target = glm::translate(target, glm::vec3(-camera.position.x, -camera.position.y, -camera.position.z));
}