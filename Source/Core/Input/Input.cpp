//
//  Input.cpp
//  NeverEngine
//
//  Created by Ville-Veikko Urrila on 10/10/12.
//  Copyright (c) 2013 The Drudgerist. All rights reserved.
//

#include <SDL2/SDL.h>
#include "Input.h"
#include "Locator.h"
#include "CommandProcessor.h"

std::map<std::string, typeInputEvent> Input::InputBindings;      // Map of inputs to events
std::vector<EventFunctorBase*> Input::eventObserverList;


void Input::Initialize() {
    // Add our binding mechanic to our console commands
    CommandProcessor::AddCommand("bind", Command<std::string,std::string>(Input::Bind));

    // Bind escape key to "back" action
    Bind("Escape", "back");
    // Bind return key to "start" action
    Bind("Return", "start");
    // Bind delete and backspace for text input
    Bind("Delete", "deleteRight");
    Bind("BackSpace", "deleteLeft");

    // Bind P key to "Pause" action
    Bind("P", "pause");
    // Bind ` key to "ShowConsole" action
    Bind("C", "console");
}

void Input::Terminate() {
    // Clear up all bindings
    InputBindings.clear();
    eventObserverList.clear();
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
void PrintKeyInfo( SDL_KeyboardEvent *key ){
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
    while( SDL_PollEvent( &event ) ){
        //User requests quit, terminate gracefully
        if( event.type == SDL_QUIT )
        {
            Locator::getHyperVisor().Stop();
            return;
        }
        else
        {
            // Amount variable depends on press/release or axis status
            // Range -1.0 ~ 1.0
            float amount = 0.0f;
            std::string input;
            /* We are only worried about SDL_KEYDOWN and SDL_KEYUP events for now */
            switch( event.type ){
                case SDL_KEYDOWN:
                    amount = 1.0f;
                    input = SDL_GetKeyName( event.key.keysym.sym );
                    break;                    
                case SDL_KEYUP:
                    amount = -1.0f;
                    input = SDL_GetKeyName( event.key.keysym.sym );
                    break;
                default:
                    break;
            }
            if ( input.length() )
            {
                printf("Input: %s\n", input.c_str());
                if ( InputBindings.find(input) != InputBindings.end() )
                {
                    printf("Bound to: %s\n", InputBindings[input].c_str());
                    if ( eventObserverList.size() )
                    {
                        (*eventObserverList[0])( InputBindings[input], amount );
                    }
                }
            }
        }
    }
}

//========================================================================
// Mouse coordinate translations
//========================================================================
void Input::MouseToUI(double *x, double *y) {
    int scrnWidth, scrnHeight;
    *y = (scrnHeight-*y) - scrnHeight/2;
    *x -= scrnWidth/2;
}
