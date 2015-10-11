//
//  SkyDome.h
//  Ingenium
//
//  Created by The Drudgerist on 31/12/13.
//  Copyright (c) 2013 The Drudgerist. All rights reserved.
//

#ifndef NGN_SKY_DOME_H
#define NGN_SKY_DOME_H

#include "RenderDefines.h"
#include "VertexBuffer_XYZW.h"
#include "Color.h"

class Shader;
class Camera;
class Texture;
class Light3D;

class SkyDome {

public:
    SkyDome();
    ~SkyDome();
    
    void Draw(const Camera& camera);
    void Update( double delta );

    inline glm::vec3 GetSunPos( void ) {
		float time = float((timeOfDay*timeFactor) - M_PI_2);
        return glm::vec3(-cosf(time), sinf(time), sunTilt);
    }
    inline Color GetSunColor() {
        float refractionFactor = (1.0f - sqrt(fmax(0.0f, GetSunPos().y)));
        glm::vec3 sc = 1.0f - glm::vec3(0.0f, 0.5f, 1.0f) * refractionFactor;
        return RGBAColor(sc.x, sc.y, sc.z, 1.0f);
    }
    inline Color GetLightColor() {
        float refractionFactor = (1.0f - sqrt(fmax(0.0f, GetSunPos().y)));
        glm::vec3 lc = 1.0f - glm::vec3(0.0f, 0.25f, 0.5f) * refractionFactor;
        float diffuseIntensity = 0.75f * fmin(1.0f, fmax(0.0f, (0.03125f + GetSunPos().y) / 0.0625f));
        return RGBAColor(lc.x, lc.y, lc.z, diffuseIntensity);
    }
    inline Color GetLightAmbient() {
        float refractionFactor = (1.0f - sqrt(fmax(0.0f, GetSunPos().y)));
        glm::vec3 lc = 1.0f - glm::vec3(0.0f, 0.25f, 0.5f) * refractionFactor;
        float ambientIntensity = 0.0625f + 0.1875f * fmin(1.0f, fmax(0.0f, (0.375f + GetSunPos().y) / 0.25f));
        return RGBAColor(lc.x, lc.y, lc.z, ambientIntensity);
    }
    inline Color GetFogColor() {
        float refractionFactor = (sqrt(fmax(0.0f, GetSunPos().y)));
        glm::vec3 sc = glm::vec3(0.5f, 0.6f, 0.8f) * refractionFactor;
        return RGBAColor(sc.x, sc.y, sc.z, 1.0f);
    }
private:
    int numDomeVerts;
    Shader* skyDomeShader;
    VertexBuffer_XYZW* buffer;

    float InnerRadius;
    float OuterRadius;
    
    bool renderSky;
    float timeOfDay;             // Used to calculate sun position in shaders
    float timeFactor;            // Used to calculate sun position in shaders
    int timeScale;               // Used to calculate sun position in shaders
    
    float sunTilt;
    
    void SetupVertices();
    void SetupShaders();
};

#endif /* defined(NGN_SKY_DOME_H) */
