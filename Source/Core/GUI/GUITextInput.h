#ifndef NGN_GUI_TEXTINPUT_H
#define NGN_GUI_TEXTINPUT_H
//
//  GUITextInput.h
//  DungeonSmith
//
//  Created by The Drudgerist on 10/01/13.
//  Copyright (c) 2013 The Drudgerist. All rights reserved.
//

#include "GUIWidget.h"
#include "TextInputBehavior.h"
#include "InputFunctors.h"
#include "TextLabel.h"

#include <SDL2/SDL_events.h>

namespace GUI
{
    class GUITextInput: public GUIWidget
    {
    public:
        GUITextInput(const int posX, const int posY,
                     const int width, const int height,
                     const int depth,
                     const std::string defaultVal="<text input>");
        ~GUITextInput();
        
        // Overriden from UIWidget
        virtual void Draw();
        virtual void Update();
        virtual void SetFocus( const bool focus);
        virtual void SetActive( const bool active );
        virtual void SetVisible( const bool visible );
        // When clicked/pressed
        virtual void OnInteract( const bool interact );

        // Text input operations
        void StartTextInput();
        void StopTextInput();
        void ClearText();
        
        // Attach a behavior to pass the input text somewhere
        void SetBehavior( TextInputBehavior* behavior ) { _behavior = behavior; };
    private:
        TextInputBehavior* _behavior;
        // Regular events, we need them to accept/cancel text input
        EventFunctor<GUITextInput> _eventFunctor;
        bool OnEvent( const typeInputEvent& theEvent, const float& amount );
        // Text input parameters
        TextInputFunctor<GUITextInput> _textInputFunctor;
        void OnTextInputEvent( const std::string& inputText );
        bool _textInputActive;

        // Text input attributes
        TextLabel _label;
        std::string _inputText;
        std::string _defaultText;

        double _lastCursorBlink;
        bool _cursorBlink;
        
        // Sugar :)
    };
}   /* namespace GUI */

#endif
