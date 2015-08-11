//
//  SkyDome.cpp
//  Ingenium
//
//  Created by The Drudgerist on 31/12/13.
//  Copyright (c) 2013 The Drudgerist. All rights reserved.
//

#include "SkyDome.h"
#include "Console.h"
#include "RenderDefines.h"
#include "StencilUtils.h"

#include "Shader.h"
#include "ShaderFactory.h"
#include "Locator.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include "glm/gtc/noise.hpp"

#include "Camera.h"
#include "MatrixUtil.h"
#include "GLErrorUtil.h"

SkyDome::SkyDome() {
    renderSky = true;
    
    InnerRadius = 6356.7523142;
    OuterRadius = 6356.7523142 * 1.157313;
    
    timeOfDay = 8.0f;                // Start off in morning
    timeFactor = float(M_PI/12.0f);  // 24 hour full cycle equals 2*pi radians
    timeScale = 1.0;                     // 1 = 1 hour per second
    sunTilt = 0.05f;                 // Alignment along North-South axis
    
    buffer = NULL;
    SetupVertices();
    
    SetupShaders();

    Console::AddVar( new ConsoleVar(timeOfDay), "timeOfDay");
    Console::AddVar( new ConsoleVar(timeScale), "timeScale");
}

SkyDome::~SkyDome() {
    Console::RemoveVar( "timeOfDay" );
    Console::RemoveVar( "timeScale" );
    delete buffer;
    buffer = NULL;
}

void SkyDome::Draw(const Camera& camera) {
    if ( buffer != NULL && renderSky ) {
        glm::vec2 windowSize = Locator::getRenderer().GetWindowSize();
        glm::mat4 mvp;
        MatrixUtil::GetGameMatrix(mvp, windowSize.x, windowSize.y, camera);
        glm::vec3 offset = glm::vec3(0,0.98,0);
        mvp = glm::translate(mvp, camera.position-offset);

        CHECK_GL_ERROR();

        glDisable(GL_BLEND);
        glEnable(GL_DEPTH_TEST);
        glDepthMask(GL_FALSE);

        skyDomeShader->Begin();
        skyDomeShader->setUniformM4fv("MVP", mvp);
        skyDomeShader->setUniform3fv("v3LightPos", GetSunPos());

        buffer->Bind();
        glDrawArrays(GL_TRIANGLES, 0, buffer->Count());
        buffer->Unbind();
        
        skyDomeShader->End();

        glDepthMask(GL_TRUE);
        glEnable(GL_BLEND);
    }
}

void SkyDome::Update( double delta ) {
    timeOfDay += delta*timeScale;
    if ( timeOfDay > 24.0f ) { timeOfDay -= 24.0f; }
}

void SkyDome::SetupShaders()
{
    float Kr = 0.0030f;
	float Km = 0.0015f;
	float ESun = 16.0f;
	float g = -0.75f;
    
	float Scale = 1.0f / (OuterRadius - InnerRadius);
	float ScaleDepth = 0.25f;
	float ScaleOverScaleDepth = Scale / ScaleDepth;
    
    skyDomeShader = ShaderFactory::LoadFromFile("deferred_sky_dome.fsh", "deferred_sky_dome.vsh");
    skyDomeShader->Begin();
    skyDomeShader->setUniform3fv("v3CameraPos", 0.0f, InnerRadius, 0.0f);
    skyDomeShader->setUniform3fv("v3InvWavelength", 1.0f / powf(0.650f, 4.0f), 1.0f / powf(0.570f, 4.0f), 1.0f / powf(0.475f, 4.0f));
    skyDomeShader->setUniform1fv("fCameraHeight", InnerRadius);
    skyDomeShader->setUniform1fv("fCameraHeight2", InnerRadius * InnerRadius);
    skyDomeShader->setUniform1fv("fInnerRadius", InnerRadius);
    skyDomeShader->setUniform1fv("fInnerRadius2", InnerRadius * InnerRadius);
    skyDomeShader->setUniform1fv("fOuterRadius", OuterRadius);
    skyDomeShader->setUniform1fv("fOuterRadius2", OuterRadius * OuterRadius);
    skyDomeShader->setUniform1fv("fKrESun", Kr * ESun);
    skyDomeShader->setUniform1fv("fKmESun", Km * ESun);
    skyDomeShader->setUniform1fv("fKr4PI", Kr * 4.0f * (float)M_PI);
    skyDomeShader->setUniform1fv("fKm4PI", Km * 4.0f * (float)M_PI);
    skyDomeShader->setUniform1fv("fScale", Scale);
    skyDomeShader->setUniform1fv("fScaleDepth", ScaleDepth);
    skyDomeShader->setUniform1fv("fScaleOverScaleDepth", ScaleOverScaleDepth);
    skyDomeShader->setUniform1fv("g", g);
    skyDomeShader->setUniform1fv("g2", g * g);
    skyDomeShader->setUniform1iv("Samples", 4);
}

void SkyDome::SetupVertices()
{
    if ( buffer != NULL )
    {
        delete buffer;
        buffer = NULL;
    }
    
    unsigned int rings = 16;
    unsigned int sectors = 16;
    
    numDomeVerts = 6*rings*(sectors-1)*2 +(3*rings);
    
    buffer = new VertexBuffer_XYZW(numDomeVerts);
    
    glm::vec4 va, vb, vc, vd;
	const float stepa = (float)M_PI * 2.0f / rings;
    const float startb = asin(InnerRadius / OuterRadius);
    const float stepb = ((float)(M_PI_2) - startb) / sectors;
    // Side ring
	for(unsigned int y = 0; y < sectors-1; y++)
	{
		float b = startb + stepb * y;
		for(unsigned int x = 0; x < rings; x++)
		{
			float a = stepa * x;
            
			va = glm::vec4(sin(a) * cos(b), sin(b), -cos(a) * cos(b), 1.0f) * OuterRadius;
			vb = glm::vec4(sin(a + stepa) * cos(b), sin(b), -cos(a + stepa) * cos(b), 1.0f) * OuterRadius;
			vc = glm::vec4(sin(a + stepa) * cos(b + stepb), sin(b + stepb), -cos(a + stepa) * cos(b + stepb), 1.0f) * OuterRadius;
			vd = glm::vec4(sin(a) * cos(b + stepb), sin(b + stepb), -cos(a) * cos(b + stepb), 1.0f) * OuterRadius;
			buffer->Buffer(va);
			buffer->Buffer(vb);
			buffer->Buffer(vc);
			buffer->Buffer(vc);
			buffer->Buffer(vd);
			buffer->Buffer(va);
		}
	}

    // Top
	float b = startb + stepb * (sectors-1);
	for(unsigned int x = 0; x < rings; x++)
	{
		float a = stepa * x;
		va = glm::vec4(sin(a) * cos(b), sin(b), -cos(a) * cos(b), 1.0f) * OuterRadius;
		vb = glm::vec4(sin(a + stepa) * cos(b), sin(b), -cos(a + stepa) * cos(b), 1.0f) * OuterRadius;
		vc = glm::vec4(0.0f, sin(b), 0.0f, 1.0f) * OuterRadius;
		buffer->Buffer(va);
		buffer->Buffer(vb);
		buffer->Buffer(vc);
	}
    buffer->Upload();
}
