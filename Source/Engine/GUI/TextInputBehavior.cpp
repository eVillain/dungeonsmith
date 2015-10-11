//
//  TextInputBehavior.cpp
//  DungeonSmith
//
//  Created by The Drudgerist on 10/08/15.
//  Copyright (c) 2015 The Drudgerist. All rights reserved.
//

#include "TextInputBehavior.h"

namespace GUI
{
    TextInputBehaviorLambda::TextInputBehaviorLambda( std::function<void(const std::string& inputText)> func) :
    function(func)
    { }
    
    void TextInputBehaviorLambda::Trigger(const std::string& inputText)
    {
        if ( function )
        {
            function(inputText);
        }
    }
    
    
}   /* namespace GUI */
