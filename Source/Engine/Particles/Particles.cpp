//
//  Particles.cpp
//  DungeonSmith
//
//  Created by The Drudgerist on 01/03/14.
//  Copyright (c) 2014 The Drudgerist. All rights reserved.
//

#include "Particles.h"
#include "Locator.h"
#include "Renderer.h"

int Particles::nextParticleSysID = 0;

Particles::Particles() {
    paused = false;
}

Particles::~Particles() {
    
}

void Particles::Update( double dt ) {
    if ( paused ) return;
    std::map<int,ParticleSys*>::iterator it;
    for (it=m_systems.begin(); it!=m_systems.end(); it++) {
        it->second->Update(dt);
    }
}

ParticleSys* Particles::AddSystem( const std::string filePath, const std::string fileName ) {
    ParticleSys* sys = new ParticleSys( filePath, fileName );
    int sysID = nextParticleSysID++;
    m_systems[sysID] = sys;
    return sys;
}
int Particles::GetSystemID( ParticleSys *system ) {
    std::map<int, ParticleSys*>::iterator it;
    for (it=m_systems.begin(); it != m_systems.end(); it++) {
        if ( it->second == system ) {
            return it->first;
        }
    }
    return -1;
}
void Particles::RemoveSystem( ParticleSys* system ) {
    std::map<int, ParticleSys*>::iterator it;
    for (it=m_systems.begin(); it != m_systems.end(); it++) {
        if ( it->second == system ) {
            // Found system to delete
            m_systems.erase(it);
            delete system;
            system = NULL;
            return;
        }
    }
}
void Particles::RemoveSystem( const int sysID ) {
    if ( m_systems.find(sysID) != m_systems.end() ) {
        ParticleSys* system = m_systems[sysID];
        m_systems.erase(sysID);
        delete system;
        system = NULL;
        return;
    }
}
void Particles::Draw()
{
    IRenderer& renderer = Locator::getRenderer();
    std::map<int,ParticleSys*>::iterator it;
    for (it=m_systems.begin(); it!=m_systems.end(); it++) {
        it->second->Draw(renderer);
    }
}
void Particles::DrawUnlitParticles()
{
    IRenderer& renderer = Locator::getRenderer();
    std::map<int,ParticleSys*>::iterator it;
    for (it=m_systems.begin(); it!=m_systems.end(); it++) {
        if ( it->second->lighting == ParticleSysLightOff ) {
            it->second->Draw(renderer);
        }
    }
}
void Particles::DrawLitParticles()
{
    IRenderer& renderer = Locator::getRenderer();
    std::map<int,ParticleSys*>::iterator it;
    for (it=m_systems.begin(); it!=m_systems.end(); it++) {
        if ( it->second->lighting == ParticleSysLightOn ) {
            it->second->Draw(renderer);
        }
    }
}
