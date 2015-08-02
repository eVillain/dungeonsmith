#ifndef NGN_INPUT_FUNCTORS_H
#define NGN_INPUT_FUNCTORS_H

//
//  InputFunctors.h
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
// InputEvent typedef which is used for identifying input events
typedef std::string typeInputEvent;

// Functor derived template class for events
class EventFunctorBase {
public:
    virtual bool operator()( const typeInputEvent& theEvent, const float& amount ) = 0;
};
// Functor derived template class for any engine event callbacks
template <class UnknownClass> class EventFunctor : public EventFunctorBase {
public:
    bool ( UnknownClass::*function )( const typeInputEvent& theEvent, const float& amount );           // Pointer to a member function
    UnknownClass* object;                                   // Pointer to an object instance
    
    // Constructor - takes a pointer to an object and a pointer to a member function
    // and stores them in two private variables
    EventFunctor( UnknownClass* objectPtr, bool( UnknownClass::*func )( const typeInputEvent& theEvent, const float& amount ) )
    { object = objectPtr; function = func; };
    
    // Override operator "()"
    virtual bool operator()( const typeInputEvent& theEvent, const float& amount )
    { return (*object.*function)( theEvent, amount ); };                // Execute member function
};

// Functor derived template class for mouse events
class MouseFunctorBase {
public:
    virtual bool operator()( const int& x, const int& y ) = 0;
};
// Functor derived template class for any engine event callbacks
template <class UnknownClass> class MouseFunctor : public MouseFunctorBase {
public:
    bool ( UnknownClass::*function )( const int& x, const int& y );           // Pointer to a member function
    UnknownClass* object;                                   // Pointer to an object instance
    
    // Constructor - takes a pointer to an object and a pointer to a member function
    // and stores them in two private variables
    MouseFunctor( UnknownClass* objectPtr, bool( UnknownClass::*func )( const int& x, const int& y ) )
    { object = objectPtr; function = func; };
    
    // Override operator "()"
    virtual bool operator()( const int& x, const int& y )
    { return (*object.*function)( x, y ); };                // Execute member function
};
#endif
