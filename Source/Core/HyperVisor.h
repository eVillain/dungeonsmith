//
//  HyperVisor.h
//  DungeonSmith
//
//  Created by The Drudgerist on 20/04/15.
//  Copyright (c) 2015 The Drudgerist. All rights reserved.
//

#ifndef NGN_HYPERVISOR_H
#define NGN_HYPERVISOR_H

#include "InputFunctors.h"

class HyperVisor {
public:
    static const std::string VERSION() { return "0.1a"; };
    HyperVisor();
    ~HyperVisor();
    
    void Initialize(int argc, char * arg[]);
    void Terminate();
    
    int Run();
    void Stop(const int errorCode=0); // TODO: Add reason for stop as parameter for error handling
    
private:
    double timeAtLastFrameStart;
    int errorCode;
    bool quit;
    bool initialized;
    EventFunctor<HyperVisor> eventFunctor;
    bool OnEvent( const typeInputEvent& theEvent, const float& amount );
};

#endif /* defined(NGN_HYPERVISOR_H) */
