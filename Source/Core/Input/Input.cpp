//
//  Input.cpp
//  DungeonSmith
//
//  Created by The Drudgerist on 10/10/12.
//  Copyright (c) 2013 The Drudgerist. All rights reserved.
//

#include <SDL2/SDL.h>
#include "Input.h"
#include "Locator.h"
#include "CommandProcessor.h"
#include "StringUtil.h"
#include "RangeReverseAdapter.h"
#include "Renderer.h"

std::map<std::string, typeInputEvent> Input::InputBindings;
std::vector<EventFunctorBase*> Input::eventObserverList;
std::vector<MouseFunctorBase*> Input::mouseObserverList;
TextInputFunctorBase* Input::textInputObserver = nullptr;
std::string Input::_inputText = "";

void Input::Initialize() {
    // Add our binding mechanic to our console commands
    CommandProcessor::AddCommand("bind", Command<std::string,std::string>(Input::Bind));

    SetDefaultBindings();
}

void Input::Terminate() {
    // Clear up all bindings
    InputBindings.clear();
    eventObserverList.clear();
    textInputObserver = nullptr;
}

void Input::RegisterEventObserver( EventFunctorBase* observer ) {
    eventObserverList.push_back( observer );
}

void Input::UnRegisterEventObserver( EventFunctorBase* observer ) {
    // Try to find observer in list
    bool found = false;
    for ( unsigned int i=0; i < eventObserverList.size(); i++ ) {
        if ( eventObserverList[i] == observer ) {
            found = true;
            eventObserverList.erase( eventObserverList.begin() + i );
            break;
        }
    }
    if (!found) {
        printf("[Input] tried to unregister non-existent EventObserver functor\n");
    }
}

void Input::RegisterMouseObserver( MouseFunctorBase* observer ) {
    mouseObserverList.push_back( observer );
}


void Input::UnRegisterMouseObserver( MouseFunctorBase* observer ) {
    std::vector<MouseFunctorBase*>::iterator it = std::find(mouseObserverList.begin(), mouseObserverList.end(), observer);
    if ( it != mouseObserverList.end() )
    {
        mouseObserverList.erase(it);
    }
}


void Input::Bind( std::string input, std::string event ) {
    printf("[Input] binding %s to %s\n", input.c_str(), event.c_str());
    InputBindings[input] = event;
}

/* Print modifier info */
void PrintModifiers( Uint32 mod ){
    printf( "Modifers: " );
    
    /* If there are none then say so and return */
    if( mod == KMOD_NONE ){
        printf( "None\n" );
        return;
    }
    
    /* Check for the presence of each SDLMod value */
    /* This looks messy, but there really isn't    */
    /* a clearer way.                              */
    if( mod & KMOD_NUM ) printf( "NUMLOCK " );
    if( mod & KMOD_CAPS ) printf( "CAPSLOCK " );
    if( mod & KMOD_LCTRL ) printf( "LCTRL " );
    if( mod & KMOD_RCTRL ) printf( "RCTRL " );
    if( mod & KMOD_RSHIFT ) printf( "RSHIFT " );
    if( mod & KMOD_LSHIFT ) printf( "LSHIFT " );
    if( mod & KMOD_RALT ) printf( "RALT " );
    if( mod & KMOD_LALT ) printf( "LALT " );
    if( mod & KMOD_CTRL ) printf( "CTRL " );
    if( mod & KMOD_SHIFT ) printf( "SHIFT " );
    if( mod & KMOD_ALT ) printf( "ALT " );
    if( mod & KMOD_LGUI ) printf( "LGUI " );
    if( mod & KMOD_RGUI ) printf( "RGUI " );
    if( mod & KMOD_MODE ) printf( "MODE " );
    if( mod & KMOD_RESERVED ) printf( "RESERVED " );
    printf( "\n" );
}

/* Print all information about a key event */
void PrintKeyInfo( SDL_KeyboardEvent *key )
{
    /* Is it a release or a press? */
    if( key->type == SDL_KEYUP )
        printf( "Release:- " );
    else
        printf( "Press:- " );
    
    /* Print the hardware scancode first */
    printf( "Scancode: 0x%02X", key->keysym.scancode );
    /* Print the name of the key */
    printf( ", Name: %s", SDL_GetKeyName( key->keysym.sym ) );
    printf( "\n" );
    /* Print modifier info */
    PrintModifiers( key->keysym.mod );
}

void Input::ProcessInput() {
    //Event handler
    SDL_Event event;
    
    /* Poll for events. SDL_PollEvent() returns 0 when there are no  */
    /* more events on the event queue, our while loop will exit when */
    /* that occurs.                                                  */
    while( SDL_PollEvent( &event ) )
    {
        //User requests quit, terminate gracefully
        if( event.type == SDL_QUIT )
        {
            CommandProcessor::ExecuteCommand("quit");
            return;
        }
        
        // Amount variable depends on press/release or axis status
        // Range -1.0 ~ 1.0
        float amount = 0.0f;
        std::string input;
        
        // Check for text input, that takes precedence over regular events
        if (event.type == SDL_KEYDOWN)
        {
            if ( event.key.repeat == 0 )
            {
                amount = 1.0f;
                input = SDL_GetKeyName( event.key.keysym.sym );
            }
        }
        else if (event.type == SDL_KEYUP)
        {
            if ( event.key.repeat == 0 )
            {
                amount = -1.0f;
                input = SDL_GetKeyName( event.key.keysym.sym );
            }
        }
        else if (event.type == SDL_MOUSEMOTION)
        {
            glm::ivec2 adjustedCoords = ConvertSDLCoordToScreen(event.motion.x, event.motion.y);
            for ( MouseFunctorBase* func : mouseObserverList )
            {
                bool swallowed = (*func)( adjustedCoords.x, adjustedCoords.y );
                if ( swallowed ) // Event was swallowed, don't propagate
                {
                    break;
                }
            }
        }
        else if (event.type == SDL_MOUSEBUTTONDOWN)
        {
            amount = 1.0f;
            input = "MouseButton" + StringUtil::IntToString(event.button.button);
        }
        else if (event.type == SDL_MOUSEBUTTONUP)
        {
            amount = -1.0f;
            input = "MouseButton" + StringUtil::IntToString(event.button.button);
        }
        else if (event.type == SDL_MOUSEWHEEL)
        { /* TODO: Implement mousewheel :) */}
        
        // We have to check for input events first, blocking the regular events
        if ( textInputObserver && ProcessTextInput(event))
        {
            // Call the registered observer
            (*textInputObserver)(_inputText);
        }
        else if ( input.length() )   // Regular events
        {
//          printf("Input: %s\n", input.c_str());
            if ( InputBindings.find(input) != InputBindings.end() )
            {
                if (textInputObserver && input != "Escape" && input != "Return")
                {
                    return;
                }
//           printf("Bound to: %s\n", InputBindings[input].c_str());
                for ( EventFunctorBase* func : eventObserverList )
                {
                    bool swallowed = (*func)( InputBindings[input], amount );
                    if ( swallowed )
                    { break; }  // Event was swallowed, don't propagate
                }
            }
        }
    }
}

bool Input::ProcessTextInput(const SDL_Event &event)
{
    if( event.type == SDL_KEYDOWN )
    {
        //Handle backspace
        if( event.key.keysym.sym == SDLK_BACKSPACE && _inputText.length() > 0 )
        {
            //lop off character
            _inputText.pop_back();
            return true;
        }
        //Handle copy
        else if( event.key.keysym.sym == SDLK_c && SDL_GetModState() & KMOD_CTRL )
        {
            SDL_SetClipboardText( _inputText.c_str() );
            return false;
        }
        //Handle paste
        else if( event.key.keysym.sym == SDLK_v && SDL_GetModState() & KMOD_CTRL )
        {
            _inputText = SDL_GetClipboardText();
            return true;
        }
    }
    //Special text input event
    else if( event.type == SDL_TEXTINPUT )
    {
        //Not copy or pasting
        if( !( (event.text.text[ 0 ] == 'c' || event.text.text[ 0 ] == 'C' ) &&
              ( event.text.text[ 0 ] == 'v' || event.text.text[ 0 ] == 'V' ) && SDL_GetModState() & KMOD_CTRL ) )
        {
            //Append character
            _inputText += event.text.text;
            return true;
        }
    }
    return false;
}

void Input::SetDefaultBindings()
{
    // Bind escape key to "back" action
    Bind("Escape", "back");
    // Bind return key to "start" action
    Bind("Return", "start");
    // Bind delete and backspace for text input
    Bind("Delete", "deleteRight");
    Bind("BackSpace", "deleteLeft");
    
    Bind("Up", "lookup");
    Bind("Down", "lookdown");
    Bind("Left", "lookleft");
    Bind("Right", "lookright");
    Bind("W", "moveforward");
    Bind("A", "moveleft");
    Bind("S", "moveback");
    Bind("D", "moveright");
    
    Bind("Left Shift", "run");
    Bind("Left Option", "sneak");

    // Bind P key to "Pause" action
    Bind("P", "pause");
    // Bind ` key to "ShowConsole" action
    Bind("C", "console");
    
    // Mouse bindings
    Bind("MouseButton1", "shoot");
}

// Text input
void Input::StartTextInput( TextInputFunctorBase* observer )
{
    //Enable text input
    SDL_StartTextInput();
    textInputObserver = observer;
    _inputText.clear();
}

void Input::UpdateTextInput()
{
    

}

void Input::StopTextInput( TextInputFunctorBase* observer )
{
    textInputObserver = nullptr;
    // Disable text input
    SDL_StopTextInput();
}


glm::ivec2 Input::ConvertSDLCoordToScreen( int x, int y )
{
    glm::ivec2 windowSize = Locator::getRenderer().GetWindowSize();
    return glm::ivec2(x - windowSize.x*0.5, windowSize.y*0.5 - y);
}
