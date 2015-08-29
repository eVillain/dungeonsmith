//
//  DynamicCubes.h
//  DungeonSmith
//
//  Created by eVillain on 29/08/15.
//  Copyright (c) 2015 The Drudgerist. All rights reserved.
//

#ifndef __DungeonSmith__DynamicCubes__
#define __DungeonSmith__DynamicCubes__

#include "RenderDefines.h"
#include "RenderDataDefines.h"
#include "Color.h"
#include "MeshInstanceColored.h"
#include "Physics.h"
#include <map>
#include <utility>

class DynamicCubes
{
public:
    DynamicCubes();
    ~DynamicCubes();
    
    void Update(const double deltaTime);
    void Draw( const glm::mat4 mvp );
    
    const uint32_t Add(const glm::vec3& position, const float size, const Color& color);
    void Remove(const uint32_t cube);

    const size_t Count() const { return _cubes.size(); }
    btRigidBody& GetBody(const uint32_t cube);
    Instance_Position_Rotation_Color& GetInstance(const uint32_t cube);
private:
    std::map<const uint32_t, std::pair<btRigidBody*, MeshInstanceID> > _cubes;

    std::vector<uint32_t> _removedIDs;
    uint32_t _counter;
    MeshInstanceColored * _cubeBuffer;
};

#endif /* defined(__DungeonSmith__DynamicCubes__) */
