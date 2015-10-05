//
//  EditorGUI.h
//  DungeonSmith
//
//  Created by eVillain on 05/10/15.
//  Copyright © 2015 The Drudgerist. All rights reserved.
//

#ifndef EditorGUI_h
#define EditorGUI_h

#include <memory>
#include "Menu.h"

class Editor;

class EditorGUI
{
public:
    EditorGUI(Editor& owner);
    
private:
    Editor& _owner;
    void Initialize();
    
    std::unique_ptr<GUI::Menu> _menu;
    
    std::unique_ptr<GUI::Menu> _cameraMenu;

};

#endif /* EditorGUI_h */
