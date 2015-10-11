#ifndef NGN_INPUT_FUNCTORS_H
#define NGN_INPUT_FUNCTORS_H

//
//  InputListener.h
// TODO: UPDATE THIS TEXT :)
//  This class provides the input event system via a functor.
//  Usage:
//  1 - create an event functor and point it to an instance method with
//      the following signature: bool(typeInputEvent, float)
//  2 - register the event functor with Input::RegisterEventObserver
//  3 - the boolean return function dictates whether the event was swallowed
//      false means the event should propagate to other listeners
//  Created by The Drudgerist on 4/27/13.
//  Copyright (c) 2013 The Drudgerist. All rights reserved.
//

#include <string>
#include "glm.hpp"

class InputEventListener
{
    friend class Input;
private:
    virtual bool OnEvent( const std::string& event, const float& amount ) = 0;
};

class MouseEventListener
{
    friend class Input;
private:
    virtual bool OnMouse( const glm::ivec2& coords ) = 0;
};

class TextInputEventListener
{
    friend class Input;
private:
    virtual void OnTextInput( const std::string& text ) = 0;
};

#endif
