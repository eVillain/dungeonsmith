//
//  LightSystem3D.cpp
//  Ingenium
//
//  Created by The Drudgerist on 11/01/14.
//  Copyright (c) 2014 The Drudgerist. All rights reserved.
//

#include "LightSystem3D.h"
#include "Camera.h"
#include "ShaderFactory.h"
#include <glm/gtx/rotate_vector.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "Locator.h"
#include "Timer.h"
#include "GBuffer.h"
#include "StencilUtils.h"

static const GLfloat square2D_coords[] = {
    -0.5,-0.5, 0.0, 1.0,
    0.5,-0.5, 0.0, 1.0,
    0.5, 0.5, 0.0, 1.0,
    -0.5, 0.5, 0.0, 1.0,
};
static const GLfloat square2D_texCoords[] = {
    0.0, 0.0,
    1.0, 0.0,
    1.0, 1.0,
    0.0, 1.0,
};

LightSystem3D::LightSystem3D() {
    renderedLights = 0;
    _shaderLight = nullptr;
}

LightSystem3D::~LightSystem3D( ) {
}

void LightSystem3D::Initialize()
{
    _shaderLight = ShaderFactory::LoadFromFile("forward_light_pass.fsh", "forward_light_pass.vsh");
    // Prepare VAO for light render
    glGenVertexArrays(1, &_vao);
    glGenBuffers(1, &_vbo);
    glBindVertexArray(_vao);
    glBindBuffer(GL_ARRAY_BUFFER, _vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(square2D_coords)+sizeof(square2D_texCoords)+sizeof(GLfloat)*3*4, square2D_coords, GL_STATIC_DRAW);
    // Vertices & texcoords, frustum gets uploaded later
    glBufferSubData(GL_ARRAY_BUFFER, sizeof(square2D_coords), sizeof(square2D_texCoords), square2D_texCoords);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, 0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, (GLvoid *)(sizeof(square2D_coords)));
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, (GLvoid *)(sizeof(square2D_coords)+sizeof(square2D_texCoords)));
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);
}

void LightSystem3D::Terminate()
{
    ShaderFactory::ClearShader(_shaderLight);
    _shaderLight = nullptr;

    glDeleteBuffers(1, &_vbo);
    glDeleteVertexArrays(1, &_vao);
}

void LightSystem3D::GetLightsForArea(glm::vec3 pos, glm::vec3 radius, std::vector<Light3D*> &containedLights) {
    for ( Light3D* light : _lights ) {
        float lightRadius = light->position.w;
        if (light->position.x+lightRadius < pos.x-radius.x ||
            light->position.x-lightRadius > pos.x+radius.x||
            light->position.y+lightRadius < pos.y-radius.y ||
            light->position.y-lightRadius > pos.y+radius.y ||
            light->position.z+lightRadius < pos.x-radius.z ||
            light->position.z-lightRadius > pos.x+radius.z )
        { continue; }
        else
        { containedLights.push_back(light); }
    }
}

void LightSystem3D::RenderLighting( const Camera& camera, const GBuffer& gBuffer ) {
    Color fogColor = COLOR_FOG_DEFAULT;
    bool fogDensity = 0.75f;
    bool fogHeightFalloff = 0.25f;
    bool fogExtinctionFalloff = 20.0f;
    bool fogInscatteringFalloff = 20.0f;
    
    glm::ivec2 windowSize = Locator::getRenderer().GetWindowSize();
    float noiseSeed = Timer::Milliseconds();

    glm::mat4 model = glm::mat4();
    model = glm::rotate(model, -camera.rotation.x, glm::vec3(1.0, 0.0, 0.0));
    model = glm::rotate(model, -camera.rotation.y, glm::vec3(0.0, 1.0, 0.0));
    model = glm::rotate(model, -camera.rotation.z, glm::vec3(0.0, 0.0, 1.0));
    
    glm::vec4 viewport = glm::vec4(0, 0, windowSize.x, windowSize.y);
    glm::mat4 proj = glm::mat4();
    GLfloat aspectRatio = (windowSize.x > windowSize.y) ?
    GLfloat(windowSize.x)/GLfloat(windowSize.y) : GLfloat(windowSize.y)/GLfloat(windowSize.x);
    proj = glm::perspective(camera.fieldOfView, aspectRatio, camera.nearDepth, camera.farDepth);
    
    // Frustum far plane corner coordinates
    glm::vec3 viewVerts[4];
    float cz = 1.0f;
    viewVerts[0] = glm::unProject(glm::vec3(0.0f, 0.0f, cz), model, proj, viewport);
    viewVerts[1] = glm::unProject(glm::vec3(windowSize.x, 0.0f, cz), model, proj, viewport);
    viewVerts[2] = glm::unProject(glm::vec3(windowSize.x, windowSize.y, cz), model, proj, viewport);
    viewVerts[3] = glm::unProject(glm::vec3(0.0f, windowSize.y, cz), model, proj, viewport);
    
    // Parameters for linearizing depth value
    glm::vec2 depthParameter = glm::vec2( camera.farDepth / ( camera.farDepth - camera.nearDepth ),
                                         camera.farDepth * camera.nearDepth / ( camera.nearDepth - camera.farDepth ) );

    // Prepare VAO for light render
    glBindVertexArray(_vao);
    glBindBuffer(GL_ARRAY_BUFFER, _vbo);
    glBufferSubData(GL_ARRAY_BUFFER, sizeof(square2D_coords)+sizeof(square2D_texCoords), sizeof(GLfloat)*3*4, viewVerts);

    // Bind all the G-Buffer textures
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, gBuffer.GetDiffuse());
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, gBuffer.GetSpecular());
    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_2D, gBuffer.GetNormal());
    glActiveTexture(GL_TEXTURE3);
    glBindTexture(GL_TEXTURE_2D, gBuffer.GetDepth());
//    glActiveTexture(GL_TEXTURE4);
//    glBindTexture(GL_TEXTURE_2D, ao_texture);
//    glActiveTexture(GL_TEXTURE5);
//    glBindTexture(GL_TEXTURE_2D, shadow_texture);
//    glActiveTexture(GL_TEXTURE6);
//    glBindTexture(GL_TEXTURE_CUBE_MAP, shadow_cubeMap);

    // Draw only over geometry, not over sky
    Stencil::Enable();
    Stencil::SetKeepGreater(Stencil::Layer_Solid);
    
    glEnable(GL_BLEND);
    glBlendFunc(GL_ONE, GL_ONE);
    
    // Render all lights
    _shaderLight->Begin();
    _shaderLight->setUniform1iv("diffuseMap", 0);
    _shaderLight->setUniform1iv("specularMap", 1);
    _shaderLight->setUniform1iv("normalMap", 2);
    _shaderLight->setUniform1iv("depthMap", 3);
    _shaderLight->setUniform2fv("depthParameter", depthParameter);
    _shaderLight->setUniform1fv("farDepth", camera.farDepth);
    _shaderLight->setUniform1fv("nearDepth", camera.nearDepth);
    _shaderLight->setUniform3fv("camPos", camera.position);
    _shaderLight->setUniform1iv("renderSSAO", false );
    _shaderLight->setUniform1iv("ssaoMap", 4 );
    _shaderLight->setUniform1iv("renderFog", false);
    _shaderLight->setUniform1fv("fogDensity", fogDensity);
    _shaderLight->setUniform1fv("fogHeightFalloff", fogHeightFalloff);
    _shaderLight->setUniform1fv("fogExtinctionFalloff", fogExtinctionFalloff);
    _shaderLight->setUniform1fv("fogInscatteringFalloff", fogInscatteringFalloff);
    _shaderLight->setUniform3fv("fogColor", fogColor.r,fogColor.g,fogColor.b);
    
    for ( Light3D* _light : _lights ) {
        Light3D& light = *_light;
        if ( !light.active ) continue;
        _shaderLight->setUniform4fv("lightPosition", light.position);
        _shaderLight->setUniform4fv("lightAmbient", light.ambient);
        _shaderLight->setUniform4fv("lightDiffuse", light.diffuse);
        _shaderLight->setUniform4fv("lightSpecular", light.specular);
        _shaderLight->setUniform3fv("lightAttenuation", light.attenuation);
        _shaderLight->setUniform3fv("lightSpotDirection", light.direction);
        _shaderLight->setUniform1fv("lightSpotCutoff", light.spotCutoff);
        _shaderLight->setUniform1fv("lightSpotExponent", light.spotExponent);
        _shaderLight->setUniform1fv("seed", noiseSeed);
        
        glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
    }
    
    _shaderLight->End();
    Stencil::Disable();

    glActiveTexture(GL_TEXTURE6); glBindTexture(GL_TEXTURE_2D, 0);
    glActiveTexture(GL_TEXTURE5); glBindTexture(GL_TEXTURE_2D, 0);
    glActiveTexture(GL_TEXTURE4); glBindTexture(GL_TEXTURE_2D, 0);
    glActiveTexture(GL_TEXTURE3); glBindTexture(GL_TEXTURE_2D, 0);
    glActiveTexture(GL_TEXTURE2); glBindTexture(GL_TEXTURE_2D, 0);
    glActiveTexture(GL_TEXTURE1); glBindTexture(GL_TEXTURE_2D, 0);
    glActiveTexture(GL_TEXTURE0); glBindTexture(GL_TEXTURE_2D, 0);
}


void LightSystem3D::Add(Light3D* newLight) {
    bool containedLight = false;
    for (unsigned int i = 0; i < _lights.size(); ++i) {
        if (_lights.at(i) == newLight) {
            containedLight = true;
            break;
        }
    }
    if ( containedLight == false ) {
        _lights.push_back(newLight);
    }
}

void LightSystem3D::Remove(Light3D* oldLight) {
    for (unsigned int i = 0; i < _lights.size(); ++i) {
        if (_lights.at(i) == oldLight) {
            _lights.erase(_lights.begin() + i);
            break;
        }
    }
}

const bool LightSystem3D::Contains(Light3D* theLight) {
    return std::find(_lights.begin(), _lights.end(), theLight) != _lights.end();
}

const unsigned long LightSystem3D::NumLights() {
    return (int)_lights.size();
}

void LightSystem3D::Clear() {
    printf("[LightSystem3D] Clearing %lu lights\n", _lights.size());
    for (unsigned int i = 0; i < _lights.size(); ++i) {
        delete _lights.at(i);
    }
    _lights.clear();
}

