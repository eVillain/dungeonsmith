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

#include "InputListener.h"
#include "glm/glm.hpp"
#include <SDL2/SDL_events.h>
#include <map>
#include <vector>

class Input {
public:
    static void ProcessInput();
    
    static void Initialize();
    static void Terminate();
    
    static void RegisterEventObserver( InputEventListener* observer );
    static void UnRegisterEventObserver( InputEventListener* observer );
    
    static void RegisterMouseObserver( MouseEventListener* observer );
    static void UnRegisterMouseObserver( MouseEventListener* observer );
    
    // Bind an input (Keyboard/Mouse/Joystick) to an Event
    static void Bind( std::string input, std::string event );

    static void SetDefaultBindings();
    
    // Keyboard text input
    static void StartTextInput( TextInputEventListener* observer );
    static void StopTextInput( TextInputEventListener* observer );
    static void UpdateTextInput();

private:
    static bool ProcessTextInput( const SDL_Event& event );
    // Map of inputs to events
    static std::map<std::string, std::string> InputBindings;
    // Observers of input events
    
    static std::vector<InputEventListener*> eventObserverList;
    static std::vector<MouseEventListener*> mouseObserverList;
    // Text input observer
    static TextInputEventListener* textInputObserver;
    // Current text input
    static std::string _inputText;
    // Helper function
    static glm::ivec2 ConvertSDLCoordToScreen( int x, int y );
};

#endif
