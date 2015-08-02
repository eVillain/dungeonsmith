//
//  LightSystem3D.h
//  Ingenium
//
//  Created by The Drudgerist on 11/01/14.
//  Copyright (c) 2014 The Drudgerist. All rights reserved.
//

#ifndef NGN_LIGHT_SYSTEM3D_H
#define NGN_LIGHT_SYSTEM3D_H

#include "Light3D.h"
#include "Camera.h"
#include <vector>

class Shader;
class GBuffer;

class LightSystem3D {
public:    
    LightSystem3D();
    ~LightSystem3D();
    
    void Initialize();
    void Terminate();
    
    void Add(Light3D* newLight);
    void Remove(Light3D* oldLight);
    void Clear();
    
    void RenderLighting( const Camera& camera, const GBuffer& gBuffer );

    const unsigned long NumLights();
    const bool Contains(Light3D* theLight);
        
    void GetLightsForArea(glm::vec3 pos, glm::vec3 radius, std::vector<Light3D*>& containedLights);
    std::vector<Light3D*>& GetLights() { return _lights; };
    
private:
    std::vector<Light3D*> _lights;		/**< The list of Light pointers */
    unsigned long renderedLights;        // Number of lights on screen in last render pass
    
    GLuint _vao, _vbo;
    Shader* _shaderLight;
};

#endif /* defined(NGN_LIGHT_SYSTEM3D_H) */
