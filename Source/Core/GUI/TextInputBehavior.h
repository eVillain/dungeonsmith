//
//  TextInputBehavior.h
//  DungeonSmith
//
//  Created by The Drudgerist on 10/08/15.
//  Copyright (c) 2015 The Drudgerist. All rights reserved.
//

#ifndef __DungeonSmith__TextInputBehavior__
#define __DungeonSmith__TextInputBehavior__

#include <functional>
#include <string>

namespace GUI
{
    class TextInputBehavior
    {
    public:
        virtual ~TextInputBehavior(){};
        virtual void Trigger(const std::string& inputText) = 0;
    };
    
    class TextInputBehaviorLambda : public TextInputBehavior
    {
    public:
        TextInputBehaviorLambda( std::function<void(const std::string&)> func );
        void Trigger(const std::string& inputText);
    private:
        std::function<void(const std::string&)> function;
    };
    
    template <class UnknownClass>
    class TextInputBehaviorCallback : public TextInputBehavior {
    public:
        TextInputBehaviorCallback( UnknownClass* objectPtr = NULL,
                                  void( UnknownClass::*func )( std::string& ) = NULL ) :
        object(objectPtr),
        function(func)
        { }
        void Trigger(const std::string& inputText)
        {
            if ( object && function ) {
                (*object.*function)( inputText );
            }
        }
    private:
        // Text input callback attributes
        void ( UnknownClass::*function )( const std::string& );
        UnknownClass* object;
    };
    
    class TextInputBehaviorStaticCallback : public TextInputBehavior {
    public:
        TextInputBehaviorStaticCallback(void( *func )( const std::string& ) = NULL ) :
        function(func)
        { }
        void Trigger(const std::string& inputText)
        {
            if ( function ) {
                (function)( inputText );
            }
        }
    private:
        void ( *function )( const std::string& );
    };
}
#endif /* defined(__DungeonSmith__TextInputBehavior__) */
