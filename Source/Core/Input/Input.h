//
//  Input.h
//  DungeonSmith
//
//  Takes keyboard/mouse/joystick events from GLFW
//  and passes them to the currently active Scene
//
//  Created by The Drudgerist on 10/10/12.
//  Copyright (c) 2013 The Drudgerist. All rights reserved.
//

#ifndef NGN_INPUT_MANAGER_H
#define NGN_INPUT_MANAGER_H

#include <map>
#include <vector>
#include "glm/glm.hpp"

#include "InputFunctors.h"

class Input {
public:
    static void ProcessInput();
    
    static void Initialize();
    static void Terminate();
    
    static void RegisterEventObserver( EventFunctorBase* observer );
    static void UnRegisterEventObserver( EventFunctorBase* observer );
    
    static void RegisterMouseObserver( MouseFunctorBase* observer );
    static void UnRegisterMouseObserver( MouseFunctorBase* observer );
    
    // Bind an input (Keyboard/Mouse/Joystick) to an Event
    static void Bind( std::string input, std::string event );

    static void SetDefaultBindings();
    
    // Keyboard text input
    static void StartTextInput( TextInputFunctorBase* observer );
    static void StopTextInput( TextInputFunctorBase* observer );
    static void UpdateTextInput();

private:
    // Map of inputs to events
    static std::map<std::string, typeInputEvent> InputBindings;
    // Observers of input events
    static std::vector<EventFunctorBase*> eventObserverList;
    static std::vector<MouseFunctorBase*> mouseObserverList;
    static TextInputFunctorBase* textInputObserver;
    // Helper function
    static glm::ivec2 ConvertSDLCoordToScreen( int x, int y );
};

#endif
