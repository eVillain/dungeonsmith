//
//  HyperVisor.h
//  DungeonSmith
//
//  Created by The Drudgerist on 20/04/15.
//  Copyright (c) 2015 The Drudgerist. All rights reserved.
//

#ifndef NGN_HYPERVISOR_H
#define NGN_HYPERVISOR_H

#include "InputListener.h"

class HyperVisor : public InputEventListener
{
public:
    static const std::string VERSION() { return "0.1a"; };
    HyperVisor();
    ~HyperVisor();
    
    void Initialize(int argc, char * arg[]);
    void Terminate();
    
    int Run();
    
    void Stop(const int errorCode=0);
    
    bool OnEvent( const std::string& theEvent, const float& amount );

private:
    double timeAtLastFrameStart;
    int errorCode;
    bool quit;
    bool initialized;
};

#endif /* defined(NGN_HYPERVISOR_H) */
