//
//  Input.h
//  DungeonSmith
//
//  Takes keyboard/mouse/joystick events from SDL
//  and passes them to the currently active listener
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
    Input();
    
    void Initialize();
    void Terminate();
    
    void RegisterEventObserver( InputEventListener* observer );
    void UnRegisterEventObserver( InputEventListener* observer );
    
    void RegisterMouseObserver( MouseEventListener* observer );
    void UnRegisterMouseObserver( MouseEventListener* observer );
    
    // Bind an input (Keyboard/Mouse/Joystick) to an Event
    void Bind( std::string input, std::string event );

    void ProcessInput();
    
    void SetDefaultBindings();
    
    // Keyboard text input
    void StartTextInput( TextInputEventListener* observer );
    void StopTextInput( TextInputEventListener* observer );
    void UpdateTextInput();

private:
    bool ProcessTextInput( const SDL_Event& event );
    // Map of inputs to events
    std::map<std::string, std::string> _inputBindings;
    // Observers of input events
    
    std::vector<InputEventListener*> _inputEventListeners;
    std::vector<MouseEventListener*> _mouseEventListeners;
    // Text input observer
    TextInputEventListener* _textInputListener;
    // Current text input
    std::string _inputText;
    // Helper function
    glm::ivec2 ConvertSDLCoordToScreen( int x, int y );
};

#endif
