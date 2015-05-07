//
//  main.m
//  DungeonSmith
//
//  Created by The Drudgerist on 19/04/15.
//  Copyright (c) 2015 The Drudgerist. All rights reserved.
//

#include <iostream>

#include "HyperVisor.h"

int main(int argc, char * arg[])
{
    // The HyperVisor will control our engine core as well as the main loop
    HyperVisor hv = HyperVisor();
    
    // Initialize engine systems - we pass in our command line arguments
    // to be be parsed and if possible executed by the CommandProcessor
    hv.Initialize(argc, arg);
    
    // Here we pass control over to the HyperVisor to execute the main loop.
    // When finished the engine will return a termination code.
    // Abnormal termination is signalled by a non-zero return.
    int errorCode = hv.Run();
    
    // Clean up
    hv.Terminate();
    
    return errorCode;
}

