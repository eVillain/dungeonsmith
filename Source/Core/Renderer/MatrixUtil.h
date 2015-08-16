//
//  MatrixUtil.h
//  DungeonSmith
//
//  Created by The Drudgerist on 17/05/15.
//  Copyright (c) 2015 The Drudgerist. All rights reserved.
//

#ifndef __DungeonSmith__MatrixUtil__
#define __DungeonSmith__MatrixUtil__

#include "RenderDefines.h"

const float UI_NEARDEPTH = -100.0;
const float UI_FARDEPTH = 100.0;

class Camera;

class MatrixUtil {
public:
    static void GetUIMatrix(glm::mat4& target,
                            const float width,
                            const float height,
                            const float nearDepth=UI_NEARDEPTH,
                            const float farDepth=UI_FARDEPTH );

    static void GetGameMatrix(glm::mat4 &target,
                              const float width,
                              const float height,
                              const Camera& camera );
};

#endif /* defined(__DungeonSmith__MatrixUtil__) */
