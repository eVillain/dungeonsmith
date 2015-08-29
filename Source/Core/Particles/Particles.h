//
//  Particles.h
//  Ingenium
//
//  Created by The Drudgerist on 01/03/14.
//  Copyright (c) 2014 The Drudgerist. All rights reserved.
//

#ifndef NGN_PARTICLE_MANAGER_H
#define NGN_PARTICLE_MANAGER_H

#include "ParticleSys.h"
#include <string>
#include <map>

class Particles {
    std::map<int, ParticleSys*> m_systems;
    static int nextParticleSysID;
    bool paused;    // Lets you freeze all systems
public:
    Particles();
    ~Particles();
    
    ParticleSys* AddSystem( const std::string filePath, const std::string fileName );
    int GetSystemID( ParticleSys* system );
    void RemoveSystem( ParticleSys* system );
    void RemoveSystem( const int sysID );
    void Update( double dt );
    void Draw();
    void DrawUnlitParticles();
    void DrawLitParticles();

    bool IsPaused() { return paused; };
    void Pause() { paused = true; };
    void Resume() { paused = false; };
};
#endif /* defined(NGN_PARTICLE_MANAGER_H) */
