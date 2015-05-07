#ifndef NGN_INPUT_FUNCTORS_H
#define NGN_INPUT_FUNCTORS_H
//
//  InputFunctors.h
//  Bloxelizer
//
//  Created by Ville-Veikko Urrila on 4/27/13.
//  Copyright (c) 2013 The Drudgerist. All rights reserved.
//

#include <string>
// InputEvent typedef which is used for identifying input events
typedef std::string typeInputEvent;

// Functor derived template class for events
class EventFunctorBase {
public:
    virtual void operator()( typeInputEvent theEvent, float amount ) = 0;
};
// Functor derived template class for any engine event callbacks
template <class UnknownClass> class EventFunctor : public EventFunctorBase {
public:
    void ( UnknownClass::*function )( typeInputEvent theEvent, float amount );           // Pointer to a member function
    UnknownClass* object;                                   // Pointer to an object instance
    
    // Constructor - takes a pointer to an object and a pointer to a member function
    // and stores them in two private variables
    EventFunctor( UnknownClass* objectPtr, void( UnknownClass::*func )( typeInputEvent theEvent, float amount ) )
    { object = objectPtr; function = func; };
    
    // Override operator "()"
    virtual void operator()( typeInputEvent theEvent, float amount )
    { (*object.*function)( theEvent, amount ); };                // Execute member function
};
#endif
