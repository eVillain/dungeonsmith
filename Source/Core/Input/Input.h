//
//  Input.h
//  NeverEngine
//
//  Takes keyboard/mouse/joystick events from GLFW
//  and passes them to the currently active Scene
//
//  Created by Ville-Veikko Urrila on 10/10/12.
//  Copyright (c) 2013 The Drudgerist. All rights reserved.
//

#ifndef NGN_INPUT_MANAGER_H
#define NGN_INPUT_MANAGER_H

#include <map>
#include <vector>
#include "glm/glm.hpp"

#include "InputFunctors.h"

class Input {
private:
    static std::map<std::string, typeInputEvent> InputBindings;      // Map of inputs to events
    static std::vector<EventFunctorBase*> eventObserverList;
    
    // Keyboard text input
    static void StartTextInput();
    static void UpdateTextInput();
    static void CancelTextInput();

    
public:
    static void ProcessInput();
    
    static void Initialize();
    static void Terminate();
    
    static void RegisterEventObserver( EventFunctorBase* observer );
    static void UnRegisterEventObserver( EventFunctorBase* observer );
    
    // Bind an input (Keyboard/Mouse/Joystick) to an Event
    static void Bind( std::string input, std::string event );

    // Convert mouse coordinates to UI space (deprecated)
    static void MouseToUI(double* x, double* y);
    
};

#endif
