//
//  ParticleSys.h
//  DungeonSmith
//
//  Created by The Drudgerist on 01/03/14.
//  Copyright (c) 2014 The Drudgerist. All rights reserved.
//
//  Defines a particle system and particles contained within
//  Heavily inspired by Cocos2D and Particle Designer

#ifndef NGN_PARTICLE_SYS_H
#define NGN_PARTICLE_SYS_H

#include "RenderDefines.h"
#include "Texture.h"
#include "VertexBuffer_XYZW_RGBA.h"
#include "Dictionary.h"
#include <vector>

class IRenderer;

/** The Particle emitter lives forever. */
#define ParticleSystemDurationInfinity -1

/** The starting size of the particle is equal to the ending size. */
#define ParticleSystemStartSizeEqualToEndSize -1

/** The starting radius of the particle is equal to the ending radius.  */
#define ParticleSystemStartRadiusEqualToEndRadius -1

/** Contains the values of each individual particle. */
typedef struct sParticle {
public:
    glm::vec3   pos;
    glm::vec3   startPos;
    
	Color       color;
	Color       deltaColor;
    
	float		size;
	float		deltaSize;
    
	float		rotation;
	float		deltaRotation;
    
	float		timeToLive;
    
    union {
		// Mode A
		struct {
            float       dirX, dirY, dirZ;
			float		radialAccel;
			float		tangentialAccel;
		} A;
		// Mode B
		struct {
			float		angle;
			float		degreesPerSecond;
			float		radius;
			float		deltaRadius;
		} B;
	} mode;
    
} Particle;

enum ParticleSysMode {          // System mode (Gravity or Radial)
    ParticleSysGravity = 0,
    ParticleSysRadial = 1
};

enum ParticleSysDimensions {    // System dimensions (2D or 3D)
    ParticleSys2D = 0,
    ParticleSys3D = 1
};
enum ParticleSysLighting {      // System lighting (self lit or requires lighting pass)
    ParticleSysLightOff = 0,
    ParticleSysLightOn = 1
};
class ParticleSys {

public:
    
    ParticleSys( const std::string filePath, const std::string fileName );
    ~ParticleSys();
    void InitFromFile( const std::string filePath, const std::string fileName );
    void SaveToFile( const std::string filePath, const std::string fileName );
    
    void Update( const double dt );
    void Draw( IRenderer& renderer);

    void StopSystem();
    bool AddParticle();
    
    glm::vec3 position;
    int dimensions;     // 0=2D or 1=3D
    int lighting;       // 0 = self-lit, 1 = needs lighting
    bool active;        // Is particle system active
    float emissionRate; // Rate of particle emission
    int particleCount;  // Current number of particles
    float elapsed;      // Amount of time system has run
    float emitCounter;  // Time remaining for particle emission
    
    float angle;        // Particle spawn angle
    float angleVar;     // Particle spawn angle variance
    int blendFuncSrc, blendFuncDst;
    float duration;
    int emitterType;    // Gravity or Radial
    Color finishColor, finishColorVar;
    float finishParticleSize, finishParticleSizeVar;
    glm::vec3 gravity;
    
    int maxParticles;
    float maxRadius, maxRadiusVar;
    float minRadius, minRadiusVar;
    float lifeSpan, lifeSpanVar;
    
    float radialAccel, radialAccelVar;
    float rotPerSec, rotPerSecVar;
    float rotEnd, rotEndVar;
    float rotStart, rotStartVar;
    glm::vec3 sourcePos;
    glm::vec3 sourcePosVar;
    float speed, speedVar;
    Color startColor, startColorVar;
    
    float startSize, startSizeVar;  // Start size and variance
    float tangAccel, tangAccelVar;  // Tangential acceleration and variance
    
    std::string texFileName;    // Texture filename
private:
    Texture* texture;
    VertexBuffer_XYZW_RGBA* vBuffer;
    Particle* particles; // Array of particles
};

#endif /* defined(NGN_PARTICLE_SYS_H) */
